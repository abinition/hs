// HyperScript2.cpp : Implementation of CHyperScript2

/*
 * $Id$
 *
 * Description	: Implementation of the IHyperScript2 COM interface
 * Author		: John Bergsma <johnbergsma@hotmail.com>
 * 
 ******************************************************************************!
 !                HyperScript Software Source Code.                            !
 !                                                                             !
 !          ***** Copyright: (c) 2004 Ab Initio Software                       !
 !          ***** All rights reserved - Licensed Material.                     !
 !          ***** Program property of Ab Initio Software                       !
 !                                                                             !
 !******************************************************************************
 * 
 * Purpose		:
 * Provide COM methods to execute HyperScript functions and retreive
 * properties.
 *
 * $Log$
 * Revision 1.7  2008-06-06 00:48:36  bergsma
 * no message
 *
 * Revision 1.6  2008-05-12 17:49:05  bergsma
 * *** empty log message ***
 *
 * Revision 1.5  2005-02-11 19:17:40  jbergsma
 * When adding tokens to the Parse queue make sure the last one gets set.
 *
 * Revision 1.4  2005/01/24 19:37:00  jbergsma
 * Break up large code segments into individual lines for parse
 *
 * Revision 1.3  2005/01/18 19:47:22  jbergsma
 * Prefer static class members over globals.
 * CoInitialize the threads.
 *
 * Revision 1.2  2004/12/26 17:29:49  jbergsma
 * Implement the Eval method using mailslots
 *
 * Revision 1.1  2004/12/17 17:57:31  jbergsma
 * HyperScript interface functions and COM objects
 *
 *
 */

static const char *cvsid = "$Header$";

#include "stdafx.h"
#include "HyperScript2.h"
#include "WebPickle.h"
#include "utilx.h"

#include <atlconv.h>
#include <string>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CHyperScript2

#define DEFAULT_CLIENT_NAME	"x"	// Client default name

// static members (i.e. "globals")
//
// HyperScript Token Parse Queue
CHSTokenQueue CHyperScript2::m_hsTokenParseQueue( "HSTokenParseEvent" );
// HyperScript Token Eval Queue
CHSTokenQueue CHyperScript2::m_hsTokenEvalQueue( "HSTokenEvalEvent" );
// Client MailSlots
CHSMailSlot CHyperScript2::m_hsMailSlot( "HSMailSlotEvent" );

// Thread Function to process the HyperScript Token Parse Queue.
// Run the actual parse function as a separate thread because
// it can block if HyperScript is in the IDLE state.

UINT __stdcall HyperScriptParseTokens ( IN void *pParam )
{
	HANDLE token_event_handle = NULL;
	char *token = NULL;

	if ((token_event_handle = CHyperScript2::m_hsTokenParseQueue.get_Token_Event_Handle()) == NULL)
		return 1;		// if token queue is not initialized properly

	bool bInitialized = FALSE;
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
		bInitialized = TRUE;

	// Wait for the Parse method to put tokens on the queue
	while (WaitForSingleObject(token_event_handle, INFINITE) == WAIT_OBJECT_0) {
		CHyperScript2::m_hsTokenParseQueue.get_Token(&token);
		if (token == NULL)
			continue;				// if token is not valid

		// TODO - deal with the HyperScript return message (eg $ACK)
		gHyp_hs_parse ( token ) ;	// call HyperScript Parse

		delete[] token;				// we own this now

		// Check if more tokens on the queue
		CHyperScript2::m_hsTokenParseQueue.check_Tokens();
	}

	if (bInitialized)
		CoUninitialize();

	return 0;			// thread completed successfully
}

// Function to initialize the Client concept and MailSlots

UINT __stdcall ClientInstanceInit ( IN void *pParam )
  {
	char *name = ( char * ) pParam;
	HANDLE mailslot_event_handle = NULL;

	if ((mailslot_event_handle = CHyperScript2::m_hsMailSlot.get_MailSlot_Event_Handle()) == NULL)
		return 1;		// if mailslot is not initialized properly

	// Wait for HyperScript to initialize itself and set up the mailslot we want to open
	while (WaitForSingleObject(mailslot_event_handle, INFINITE) == WAIT_OBJECT_0) {
		CHyperScript2::m_hsMailSlot.init_MailSlot ( name );
		if (CHyperScript2::m_hsMailSlot.open_Inbox ( ) == E_FAIL)
			return 1;
		if (CHyperScript2::m_hsMailSlot.open_Outbox ( ) == E_FAIL)
			return 1;
		break;
	}

	return 0;			// completed successfully
}

// Thread Function to process the HyperScript Token Eval Queue.
// Run the actual eval function as a separate thread because
// it has to wait until HyperScript, the EVAL method, and the
// mailslots have been initialized.

UINT __stdcall HyperScriptEvalTokens ( IN void *pParam )
{
	HANDLE token_event_handle = NULL;
	char *token = NULL;
	static BOOL firstcall = TRUE;

	if ((token_event_handle = CHyperScript2::m_hsTokenEvalQueue.get_Token_Event_Handle()) == NULL)
		return 1;		// if token queue is not initialized properly

	// Initialize this client instance
	if (ClientInstanceInit ( (void *) DEFAULT_CLIENT_NAME ) != 0)
		return 1;		// if client fails to initialize

	bool bInitialized = FALSE;
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
		bInitialized = TRUE;

	// Wait for the Eval method to put tokens on the queue
	while (WaitForSingleObject(token_event_handle, INFINITE) == WAIT_OBJECT_0) {
		CHyperScript2::m_hsTokenEvalQueue.get_Token(&token);
		if (token == NULL)
			continue;				// if token is not valid

		// TODO - deal with the HyperScript return message (eg $ACK)
		gHyp_hs_eval ( token ) ;	// call HyperScript Eval

		delete[] token;				// we own this now

		// Check if more tokens on the queue
		CHyperScript2::m_hsTokenEvalQueue.check_Tokens();
	}

	if (bInitialized)
		CoUninitialize();

	return 0;			// thread completed successfully
}

// Parse a BSTR containing HyperScript tokens

STDMETHODIMP CHyperScript2::Parse(BSTR bstr)
{
	if (bstr != NULL) {

		USES_CONVERSION;	// declare locals used by the ATL macros

		string str(W2CA(bstr));

		// TODO - provide some sort of error handling here

		// Put the tokens on the Parse Queue for HyperScriptParseTokens
		//
		// Break the string up into individual lines because it could
		// be a huge chunk of code to parse and that can overflow the
		// input buffer. Parse will accept incomplete statements.
		//
		string token;
		while(!(token = GetNextToken(str, "\n")).empty())
			m_hsTokenParseQueue.set_Token(token);

		// In case the last line was not terminated by a newline
		if (!str.empty())
			m_hsTokenParseQueue.set_Token(str);
	}

	return S_OK;
}

// Eval a BSTR containing a HyperScript token

STDMETHODIMP CHyperScript2::Eval(BSTR bstr)
{
	if (bstr != NULL) {

		USES_CONVERSION;	// declare locals used by the ATL macros

		string str(W2CA(bstr));

		// TODO - provide some sort of error handling here

		// Unparse the string to avoid conflicts with double quote and pipe
		UnParseString( str ) ;

		// Put the token on the Eval Queue for HyperScriptEvalTokens
		//
		// Eval must have a complete statement so we will allow multiple
		// lines in the token string.
		//
		m_hsTokenEvalQueue.set_Token(str);
	}

	return S_OK;
}
