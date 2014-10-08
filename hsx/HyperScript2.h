// HyperScript2.h : Declaration of the CHyperScript2

/*
 * $Id$
 *
 * $Log$
 * Revision 1.4  2005-01-24 19:38:24  jbergsma
 * Clean up implementation of CHSMailSlot
 *
 * Revision 1.3  2005/01/18 19:49:09  jbergsma
 * Added static members for token queues and mailslot.
 *
 * Revision 1.2  2004/12/26 17:31:00  jbergsma
 * Added a new class to handle client mailslots
 *
 * Revision 1.1  2004/12/17 17:57:31  jbergsma
 * HyperScript interface functions and COM objects
 *
 *
 */

#ifndef __HYPERSCRIPT2_H_
#define __HYPERSCRIPT2_H_

#include "resource.h"       // main symbols
#include "hsx.h"

class CHSTokenQueue;
class CHSMailSlot;

/////////////////////////////////////////////////////////////////////////////
// CHyperScript2
class ATL_NO_VTABLE CHyperScript2 : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHyperScript2, &CLSID_HyperScript2>,
	public IDispatchImpl<IHyperScript2, &IID_IHyperScript2, &LIBID_HSXLib>
{
public:
	CHyperScript2()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HYPERSCRIPT2)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHyperScript2)
	COM_INTERFACE_ENTRY(IHyperScript2)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Declare us safe for scripting
BEGIN_CATEGORY_MAP(CHyperScript2)
	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
END_CATEGORY_MAP()

// IHyperScript2
public:
	STDMETHODIMP Eval(/*[in]*/BSTR bstr);
	STDMETHODIMP Parse(/*[in]*/BSTR bstr);

	static CHSTokenQueue m_hsTokenParseQueue;
	static CHSTokenQueue m_hsTokenEvalQueue;
	static CHSMailSlot m_hsMailSlot;
};

// CHSTokenQueue

#include <process.h>
#include <string>
#include <queue>

using namespace std;

class CHSTokenQueue {
public:
	CHSTokenQueue( char *eventName ) {
		Init();
		// Create an event for tokens with auto-reset and in the unsignaled state
		m_token_event_name = new string( _T(eventName) );
		m_token_event_handle = CreateEvent(NULL, FALSE, FALSE, m_token_event_name->c_str());
	}

	~CHSTokenQueue() {
		Term();
		CloseHandle(m_token_event_handle);
		delete m_token_event_name; }

	HANDLE get_Token_Event_Handle() const {
		return m_token_event_handle; }

	STDMETHODIMP get_Token(char **token) {
		string t;
		size_t len;

		Lock();
		if (m_tokens.empty()) {
			Unlock();
			*token = NULL;
			return S_FALSE;
		}
		t = m_tokens.front();
		len = t.length();
		*token = new char[len+1];
		strncpy(*token, t.c_str(), len);
		(*token)[len] = '\0';
		m_tokens.pop();
		Unlock();
		return S_OK; }

	STDMETHODIMP set_Token(const string& token) {
		if (token.length() == 0) return S_FALSE;
		Lock();
		m_tokens.push(token);
		Unlock();
		if (SetEvent(m_token_event_handle) == 0) {
			DWORD error = GetLastError();
			return E_FAIL;
		}
		return S_OK; }

	STDMETHODIMP check_Tokens() {
		Lock();
		if (m_tokens.empty()) {
			Unlock();
			return S_FALSE;
		}
		Unlock();
		if (SetEvent(m_token_event_handle) == 0) {
			DWORD error = GetLastError();
			return E_FAIL;
		}
		return S_OK; }

private:
	queue<string> m_tokens;
	CRITICAL_SECTION m_cs;
	string *m_token_event_name;
	HANDLE m_token_event_handle;

	void Init() { InitializeCriticalSection(&m_cs); }
	void Lock() { EnterCriticalSection(&m_cs); }
	void Unlock() { LeaveCriticalSection(&m_cs); }
	void Term() { DeleteCriticalSection(&m_cs); }
};

// CHSMailSlot

#include "interface.h"

class CHSMailSlot {
public:
	CHSMailSlot ( char *eventName ) {
		m_inBoxHandle = INVALID_HANDLE_VALUE;
		m_outBoxHandle = INVALID_HANDLE_VALUE;
		// Create an event for mailslot creation with manual-reset and in the unsignaled state
		m_mailslot_event_name = new string( _T(eventName) );
		m_mailslot_event_handle = CreateEvent(NULL, TRUE, FALSE, m_mailslot_event_name->c_str());
		m_inBoxName = NULL;
		m_outBoxName = NULL;
		m_inBoxHandle = INVALID_HANDLE_VALUE;
		m_outBoxHandle = INVALID_HANDLE_VALUE;
	}

	~CHSMailSlot () {
		if (m_inBoxName)
			delete m_inBoxName;
		if (m_outBoxName)
			delete m_outBoxName;
		CloseHandle ( m_mailslot_event_handle );
		delete m_mailslot_event_name;
		if (m_inBoxHandle != INVALID_HANDLE_VALUE)
			CloseHandle ( m_inBoxHandle );
		if (m_outBoxHandle != INVALID_HANDLE_VALUE)
			CloseHandle ( m_outBoxHandle );
	}

	HANDLE get_MailSlot_Event_Handle() {
		return m_mailslot_event_handle;
	}

	STDMETHODIMP init_MailSlot ( char * name ) {
		gHyp_client_set_concept_name ( name ) ;
		gHyp_client_set_concept_path ( ) ;
		m_inBoxName = new string();
		m_inBoxName->assign( gHyp_hs_get_mailslot_name ( ) ) ;
		m_outBoxName = new string();
		m_outBoxName->assign( gHyp_client_get_concept_path ( ) );
		return S_OK;
	}

	STDMETHODIMP get_Inbox_Name ( const char **name ) {
		if (m_inBoxName)
			*name = m_inBoxName->c_str();
		else
			*name = NULL;
		return S_OK;
	}

	STDMETHODIMP get_Outbox_Name ( const char **name ) {
		if (m_outBoxName)
			*name = m_outBoxName->c_str();
		else
			*name = NULL;
		return S_OK;
	}

	STDMETHODIMP open_Inbox ( ) {
		if ( m_inBoxName )
			m_inBoxHandle = gHyp_client_open_mailslot ( m_inBoxName->c_str(),
							FALSE,		/* Open existing. */
							FALSE,		/* No read */
							TRUE ) ;	/* Write-only */
		return (m_inBoxHandle == INVALID_HANDLE_VALUE ? E_FAIL : S_OK);
	}

	STDMETHODIMP open_Outbox ( ) {
		if ( m_outBoxName )
			m_outBoxHandle = gHyp_client_open_mailslot ( m_outBoxName->c_str(),
							TRUE,		/* Create. */
							TRUE,		/* Read-only */
							FALSE ) ;	/* No write */
		return (m_outBoxHandle == INVALID_HANDLE_VALUE ? E_FAIL : S_OK);
	}

	STDMETHODIMP write_Inbox ( char *pMsg ) {
		int msgLen = strlen(pMsg);
		ULONG nbytes;
		if ( m_inBoxHandle != INVALID_HANDLE_VALUE )
			return (WriteFile ( m_inBoxHandle, pMsg, msgLen, &nbytes, NULL ) == 0 ? E_FAIL : S_OK);
		else
			return E_FAIL;
	}

	STDMETHODIMP read_Outbox ( char **pMsg ) {
		ULONG nbytes;
		if ( m_outBoxHandle != INVALID_HANDLE_VALUE )
			return (ReadFile ( m_outBoxHandle, *pMsg, MAX_MESSAGE_SIZE, &nbytes, NULL ) < 0 ? E_FAIL : S_OK);
		else
			return E_FAIL;
	}

private:
	string *m_inBoxName;
	string *m_outBoxName;
	string *m_mailslot_event_name;
	HANDLE m_mailslot_event_handle;
	HANDLE m_inBoxHandle;
	HANDLE m_outBoxHandle;
};

// HyperScript Token Parse Function
UINT __stdcall HyperScriptParseTokens ( IN void * );
// HyperScript Token Eval Function
UINT __stdcall HyperScriptEvalTokens ( IN void * );
// Client Instance Init Function
UINT __stdcall ClientInstanceInit ( IN void * );

#endif //__HYPERSCRIPT2_H_
