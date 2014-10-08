/*
 * $Id$
 *
 * Description	: Interface functions for calling HyperScript.
 * Author		: John Bergsma <johnbergsma@hotmail.com>
 * 
 ******************************************************************************!
 !                HyperScript Software Source Code.                            !
 !                                                                             !
 !          ***** Copyright: (c) 2004 Abinition Inc                        !
 !          ***** All rights reserved - Licensed Material.                     !
 !          ***** Program property of Ab Initio Software                       !
 !                                                                             !
 !******************************************************************************
 * 
 * Purpose		:
 * These functions implement an interface that could be constructed using any
 * means. For example, these functions can be called directly if HyperScript
 * is compiled into the client, or they could be exported in a dll that the
 * client loads at run-time, or exposed through a COM interface.
 *
 * $Log$
 * Revision 1.11  2008-06-12 00:01:15  bergsma
 * Fix arguments for javascript calls
 *
 * Revision 1.10  2008-06-07 02:49:24  bergsma
 * no message
 *
 * Revision 1.9  2008-06-06 00:48:36  bergsma
 * no message
 *
 * Revision 1.8  2008-04-07 17:32:07  bergsma
 * Remove check for STATE_SLEEP
 *
 * Revision 1.7  2005-08-16 16:23:28  bergsma
 * no message
 *
 * Revision 1.6  2005/02/06 18:16:58  jbergsma
 * Get arguments to the end-of-string, not to the next ')'.
 *
 * Revision 1.5  2005/01/25 06:00:41  bergsma
 * Removed SIGINT.  Use SIGTERM only to force HS to close fatally.
 *
 * Revision 1.4  2005/01/24 19:40:26  jbergsma
 * Free the BSTR buffers in jeval
 *
 * Revision 1.3  2005/01/18 19:51:18  jbergsma
 * Added a function jeval to execute JavaScript from HyperScript
 *
 * Revision 1.2  2004/12/26 17:32:48  jbergsma
 * Added new functions to handle client mailslots and the Eval method.
 *
 * Revision 1.1  2004/12/17 17:58:18  jbergsma
 * Interface functions between HyperScript and the WebPickle control
 *
 *
 */

static const char *cvsid = "$Header$";

#include "stdafx.h"
#include "WebPickle.h"
#include "HyperScript2.h"
#include "utilx.h"
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "auto.h"

static SOCKET gsSocket = INVALID_SOCKET ;
static sInstance *gpAI = NULL ;		/* HyperScript instance */
static sInstance *gpAImain = NULL ;	/* Main HyperScript instance */

char gzClientInstanceName[INSTANCE_SIZE+1];	// This client instance name
char gzClientConcept[TARGET_SIZE+1] ;		// This client concept name
char gzClientConceptPath[MAX_PATH_SIZE+1] ;	// This client concept path

// Notify the client that the mailslot is ready
void gHyp_notify_client_mailslot ( )
{
	SetEvent(CHyperScript2::m_hsMailSlot.get_MailSlot_Event_Handle());
}

/*
 * Set the name of this client concept
 */
void gHyp_client_set_concept_name ( char *name )
{
	strcpy ( gzClientInstanceName, name );
	strcpy ( gzClientConcept, gzClientInstanceName );
	strcat ( gzClientConcept, "/" );
	strcat ( gzClientConcept, gHyp_hs_get_concept_name() );
}

/*
 * Get the name of this client concept
 */
char * gHyp_client_get_concept_name ( )
{
	return gzClientConcept;
}

/* 
 * Set the path of this client concept
 */
void gHyp_client_set_concept_path ( )
{
	strcpy ( gzClientConceptPath, gHyp_hs_get_concept_path() );
	strcat ( gzClientConceptPath, "\\" );
	strcat ( gzClientConceptPath, gzClientInstanceName );
}

/*
 * Get the path of this client concept
 */
char * gHyp_client_get_concept_path ( )
{
	return gzClientConceptPath;
}

/*
 * Execute a JavaScript function
 */
void gHyp_client_jeval ( char *command )
{
	if (command != NULL)
	{
		string strSrc(command);
		string strFunc = GetNextToken(strSrc, "(", true);
		string strArgs = GetNextToken(strSrc, "\0", true);
		strArgs.erase(strArgs.find_last_of(')'));

		/* TODO: If arguments have single quotes at the 
		 * first and last positions, then erase them.
		 * The algorithm in place now is faulty.
		 */

		/* Remove quote characters */
		int idx = 0;
		int len = strArgs.length();
		//gHyp_util_debug("Arguments before: %s",strArgs.c_str());
		while(idx < len && (idx = strArgs.find_first_of('\'', idx)) < len)
		{
			if (idx == -1) break;

			if ( (idx == 0 ) || (idx > 0 && strArgs[idx-1] != '\\') ) {

			  strArgs.erase(idx, 1);
			  --len;

			}
			else if ( idx > 0 && strArgs[idx-1] == '\\' ) {

			  strArgs.erase(idx-1, 1 );
			  --len ;
					  		
			}
			idx++ ;
			//strArgs.replace(idx, 1, "\\'" );
			//len = strArgs.length();
		}

		//gHyp_util_debug("Calling js function: %s",strFunc.c_str());
		//gHyp_util_debug("with arguments: %s",strArgs.c_str());
		CComBSTR bstrFunc(strFunc.c_str());
		CComBSTR bstrArgs(strArgs.c_str());

		IWebPickleUI *pWebPickleUI = NULL;

		/* Get the IWebPickleUI interface from the GIT */
		HRESULT hr = g_pGIT->GetInterfaceFromGlobal( CWebPickle::m_dwIWebPickleUICookie,
							IID_IWebPickleUI, (void**)&pWebPickleUI );

		if (SUCCEEDED(hr)) {
			CComQIPtr<IWebPickleUI> spWebPickleUI = pWebPickleUI;

			if (spWebPickleUI) {
				/* Call the CWebPickle::JSCall method to run the JavaScript */
				hr = spWebPickleUI->JSCall(bstrFunc, bstrArgs);

				spWebPickleUI.Release();
			}
		}

		bstrFunc.Empty();
		bstrArgs.Empty();
	}
}

/*
 * Get the name of the HyperScript concept
 */
char *gHyp_hs_get_concept_name ( )
{
	return gzConcept;
}

/*
 * Get the HyperScript concept path
 */
char *gHyp_hs_get_concept_path ( )
{
	return gzConceptPath;
}

/*
 * Get the name of the HyperScript inbox mailslot
 */
char *gHyp_hs_get_mailslot_name ( )
{
	return gzInboxPath;
}

/*
 * Get the HyperScript state
 */
sBYTE gHyp_hs_get_state ( )
{
	sBYTE state;
	if (gpAI)
		state = gHyp_instance_getState ( gpAI ) ;
	else
		state = -1;
	return state;
}

/*
 * Open or Create a client mailslot
 */
HANDLE gHyp_client_open_mailslot (	const char *path,
									sLOGICAL create, 
									sLOGICAL isRead, 
									sLOGICAL isWrite )
{
	HANDLE h;
	int error;
	if ((h = gHyp_sock_fifo ( (char *) path, create, isRead, isWrite, FALSE )) == INVALID_HANDLE_VALUE)
		error = GetLastError();
	return h;
}

/*
 * Output to the HSListBox
 */
void gHyp_hs_output_listbox( const char *pStr )
{
	CWebPickle::m_hsListBox.Puts( pStr );
}

/*
 * Process Eval tokens by putting them
 * on the HyperScript MailSlot inBox.
 * Construct an AIP message of the form:
 *
 * |target|event|EVAL|sender||\"token\"|||
 *
 * where EVAL is a HyperScript enabled method:
 *
 * list EVAL ( list token ) {
 *    *token ;
 * } ;
 *
 * enable ( EVAL ) ;
 *
 */
UINT gHyp_hs_eval( const char *token )
{
	char		message[MAX_MESSAGE_SIZE+1];
	char		tokenEval[MAX_MESSAGE_SIZE+1];
	int		n ;

	// 1. insert a backslash character before all double quote characters
	//    (because 'token' is being enclosed in a string with double quotes.
	// 2. change "|" to \174
	//    (because 'token' is being enclosed in an AIP message that uses pipe delimiters.
	n = gHyp_util_unparseString ( tokenEval, (char*) token, strlen(token), MAX_MESSAGE_SIZE, TRUE, FALSE, FALSE,"" ) ;

	// 3. divide the token up into smaller chunks, separating them by "|".
	//    (because a single segment can't be longer than 128 characters).
	//    (we need to divide at the correct places, that will be after a ", )
	n = gHyp_util_breakStringForMsg( tokenEval, n, MAX_MESSAGE_SIZE ) ;

	//sprintf ( message, "|%s|event|EVAL|%s||\\\"%s\\\"|||",
	sprintf ( message, "|%s|event|EVAL|%s||token|%s|||",
				gHyp_hs_get_concept_name(), gHyp_client_get_concept_name(), tokenEval);

	return ( CHyperScript2::m_hsMailSlot.write_Inbox ( message ) == S_OK ? 1 : 0 );
}

/*
 * Initialize HyperScript on first call.
 * Parse and execute command tokens.
 * Returns 0 on error, 1 on success.
 * Can block if in IDLE state.
 */
UINT gHyp_hs_parse( const char *token )
{

  static char		stream[MAX_INPUT_LENGTH+1] ;
  static char		terminator ;
  static char*		pStream ;
  static char*		pEndStream ;
  static int		lineCount = 0 ;
  static int		status ;

  sData
    *pAIdata;

  const char *pToken ;

  char
    self[TARGET_SIZE+1] ;

  int
    debugMask,
    tokenLen,
    stat;

  sHyp
    *pHyp ;

  pToken = (const char *) token ;
  tokenLen = strlen ( pToken ) ;

  /* Initialize return values. */
  if ( !gpsConcept ) {
  
    gHyp_util_getprocessname( self) ;

    gHyp_util_strip ( self,  strlen ( self )  ) ;
    guRunFlags = RUN_QUIET ;
    debugMask = 0; /*1023 ;*/

    gpsConcept = gHyp_concept_new () ;

    if ( !gHyp_concept_init ( gpsConcept,
			      self,
			      "", // No router "service"
                              "", // Determine "localhost"
			      debugMask,
			      MAX_EXPRESSION )) {

      return 0;

    }

    gpAI = gpAImain = gHyp_concept_getConceptInstance ( gpsConcept ) ;
    gHyp_instance_setState ( gpAI, STATE_PARSE ) ;
    guTimeStamp = FALSE ;
    
    lineCount = 0 ;
    stream[0] = '\0' ;
    pStream = stream ;
    pEndStream = stream + MAX_INPUT_LENGTH ;

  }

  /* Check if the last line still had pending tokens. */
  if ( pStream > stream ) {
    if ( terminator == '"' )
      gHyp_util_logWarning ( 
	"Nonterminated string constant on line %d", lineCount ) ;
    else	
      gHyp_util_logWarning ( 
	"Nonterminated 'identifier' on line %d", lineCount ) ;

    *pStream++ = terminator ;
    *pStream = '\0' ;
  }

  lineCount++ ;

  if ( tokenLen > 0 ) {

    /* Add token to HyperScript's input buffer. */
    strcpy ( pStream, pToken ) ;
    pStream += tokenLen ;
    *pStream = '\0' ;
  }

      /* Try loading if there's anything there */
  if ( stream[0] ) {

    /* Load from stream. */
    /*gHyp_util_debug(stream);*/
    pHyp = gHyp_frame_getHyp ( gHyp_instance_frame ( gpAI ) ) ;
    pStream = gHyp_load_fromStream ( gpAI, pHyp, stream, lineCount ) ;
 
    /* If load was fatal, (-1) then quit */
    if ( pStream == NULL ) { 

      gHyp_concept_quit ( gpsConcept ) ;
      gpsConcept = NULL ;

      return 0;
    }
  }

  if ( *pStream ) {

    /* Some of the stream was not loaded because of an unterminated literal
     * or identifier.
     * Don't process the stream yet. */
    terminator = *pStream ;	
    
    /* Shift the part that wasn't loaded to the front of the buffer */
    strcpy ( stream, pStream ) ;
    pStream = stream + strlen ( stream ) ;
  }
  else {
    /* There is no outstanding stream to load. */
    pStream = stream ;
    stream[0] = '\0' ;
  }
   

  /* Execute HyperScript. */
  pAIdata = gHyp_concept_nextInstanceData ( gpsConcept ) ;
  if ( pAIdata ) {

    gpAI = (sInstance*) gHyp_data_getObject ( pAIdata ) ;

    while ( (stat=gHyp_instance_run(gpAI)) > COND_FATAL ) {

      if ( guSIGTERM ) { stat = COND_FATAL ; break ; }
        
      if ( stat == COND_ERROR ) {

        /* The instance has an error */

        /* Un-caught errors in the main instance begets them all */
        if ( gpAI == gpAImain ) break ;

        /* Otherwise a instance can die and not affect the other instances */
        gHyp_data_detach ( pAIdata ) ;
        gHyp_data_delete ( pAIdata ) ;

        pAIdata = gHyp_concept_nextInstanceData ( gpsConcept ) ;
        if ( pAIdata ) {
           gpAI = (sInstance*) gHyp_data_getObject ( pAIdata ) ;    
	}
	else {
	  stat = COND_FATAL ;
	  break ;
	}
      }
      else {

        /* COND_NORMAL or COND_SILENT */
      
	/* Check to see if we are going back for more input */
        if ( gpAI == gpAImain &&
	     gHyp_concept_returnToStdIn ( gpsConcept ) &&
	     gHyp_instance_getState ( gpAImain ) != STATE_QUERY ) {
      
          /* HyperScript finished running */
          giJmpLevel = -1 ;	/* Disallow longjmp */
          giJmpEnabled = FALSE ;
	  gHyp_instance_setState ( gpAI, STATE_PARSE ) ;
          break ;
	}
        else {
          pAIdata = gHyp_concept_nextInstanceData ( gpsConcept ) ;
          if ( pAIdata ) {
            gpAI = (sInstance*) gHyp_data_getObject ( pAIdata ) ;    
	  }
	  else {
	    stat = COND_FATAL ;
	    break ;
	  }
	}
      }
    } 	
  }
  else
    stat = COND_FATAL ;

  if ( stat < 0 ) {
    
    /* Death to instance */
    gHyp_concept_quit ( gpsConcept ) ;
    gpsConcept = NULL ;
    if ( gsSocket != INVALID_SOCKET ) gHyp_sock_closeJNI ( gsSocket ) ;
    gsSocket = INVALID_SOCKET ;
    return 0;
  } 
  
  return 1;
}

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif