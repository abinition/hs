/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/

/*
 * Modifications:
 *
 * $Log: hs.c,v $
 * Revision 1.46  2008-05-03 21:43:55  bergsma
 * Use giLineCount and giProgram count together to better determine the
 * location of a program token code.
 *
 * Revision 1.45  2007-06-20 22:32:48  bergsma
 * Use popRdata, not popRvalue before data_getNext
 *
 * Revision 1.44  2007-05-26 22:08:43  bergsma
 * ADD -w "Ward" flag
 *
 * Revision 1.43  2007-04-12 16:48:03  bergsma
 * Put in fix for spinning.
 *
 * Revision 1.42  2007-03-26 00:23:06  bergsma
 * Only allow returnToStdin when the instance
 * is the parent instance and its not in a QUERY state.
 *
 * Revision 1.41  2007-03-15 01:13:53  bergsma
 * Another attempt to find and stop when instances spin.
 *
 * Revision 1.40  2007-02-15 03:24:52  bergsma
 * Added comments
 *
 * Revision 1.39  2006-12-09 00:06:44  bergsma
 * Move gpsAI and gpsAImain to global external status out of hs.c.
 *
 * Revision 1.38  2006/10/27 17:24:07  bergsma
 * Remove debug statement
 *
 * Revision 1.37  2006/10/12 23:09:09  bergsma
 * Fix problems with STATE_SLEEP state.
 *
 * Revision 1.36  2006/10/12 00:12:11  bergsma
 * Little problems with SLEEP. :-)
 *
 * Revision 1.35  2006/09/25 05:12:19  bergsma
 * Problem 90% solved for renamed parent concept issues.
 *
 * Revision 1.34  2006/08/22 13:28:36  bergsma
 * Missing \n in help text
 *
 * Revision 1.33  2006/01/05 17:37:48  bergsma
 * *** empty log message ***
 *
 * Revision 1.32  2005/12/10 00:30:30  bergsma
 * HS 3.6.5
 *
 * Revision 1.31  2005/11/23 16:58:22  bergsma
 * Refresh gpAImain.
 *
 * Revision 1.30  2005/10/25 16:41:33  bergsma
 * Comment out problematic ReleaseStringUTFChars
 *
 * Revision 1.29  2005/10/20 00:34:03  bergsma
 * problems with freeing memory allocated by NewString
 *
 * Revision 1.28  2005/10/15 21:42:44  bergsma
 * Memory leak with JAVA interface
 *
 * Revision 1.27  2005/08/03 14:00:20  bergsma
 * no message
 *
 * Revision 1.26  2005/07/23 22:34:04  bergsma
 * Added gHyp_hs_shutdown
 *
 * Revision 1.25  2005/06/20 01:00:23  bergsma
 * Dashboard 2005
 *
 * Revision 1.24  2005/06/18 22:25:43  bergsma
 * Dashboard 2005
 *
 * Revision 1.23  2005/04/22 19:27:34  bergsma
 * Compile errors unless return statement is in exceptionHandler function.
 *
 * Revision 1.22  2005/04/13 13:45:54  bergsma
 * HS 3.5.6
 * Added sql_toexternal.
 * Fixed handling of strings ending with bs (odd/even number of backslashes)
 * Better handling of exception condition.
 *
 * Revision 1.21  2005/04/03 17:36:19  bergsma
 * HS 3.54  (FIX OF FLOATING POINT OVERFLOW IN TLOGFEED).
 * 1. Don't delete LISting files.
 * 2. PackStart in aeqssp_autofil not being cleared - was causing an
 * unpack operation when not required.
 *
 * Revision 1.20  2005/03/30 04:05:30  bergsma
 * Add signal handler
 *
 * Revision 1.19  2005/01/25 05:55:29  bergsma
 * Separate SIGINT from SIGTERM.
 *
 * Revision 1.18  2005/01/18 20:42:17  jbergsma
 * Enable the jeval (JavaScript call) method for the WebPickle ATL project
 *
 * Revision 1.17  2004/12/17 17:39:41  jbergsma
 * Fixes and ifdefs for AS_ATL (WebPickle) to compile correctly with cpp in the hsx project
 *
 * Revision 1.16  2004/10/16 04:43:11  bergsma
 * Fixed memory access violation in JNI calls.
 *
 * Revision 1.15  2004/07/23 18:47:16  bergsma
 * fixed bug in JAVA->HS interface where gpClass global variable was not being freed.
 *
 * Revision 1.14  2004/07/01 02:17:55  bergsma
 * JNI interface changes
 *
 * Revision 1.13  2004/06/13 14:12:02  bergsma
 * JNI interface and DLL loading.
 *
 * Revision 1.12  2004/05/15 02:10:29  bergsma
 * Added callback disable.
 *
 * Revision 1.11  2004/04/29 02:09:26  bergsma
 * Added -y (debug CGI scripts) option
 *
 * Revision 1.10  2003/04/16 01:48:51  bergsma
 * Removed ^M
 *
 * Revision 1.9  2003/04/16 01:41:38  bergsma
 * Message Routing
 * ---------------
 * Lift the restriction whereby a hyperscript which specifies a default listen
 * socket with the -n switch does not also have to be the root,
 * ie: specify the -r switch.
 * The restriction has been moved to gHyp_router_message,
 * whereby messages that need to be routed to another host are only sent
 * out on the "-n" socket channel if that hyperscript is also the root.
 * Files affected: hs.c and router.c
 *
 * Revision 1.8  2003/01/28 02:03:09  bergsma
 * Return giCondition upon exit.
 *
 * Revision 1.7  2003/01/17 12:11:14  bergsma
 * Make sure -g option always creates randomly generated concept name.
 *
 * Revision 1.6  2003/01/12 07:15:33  bergsma
 * V3.1.0
 * Return cond as _exit status.
 * Do not force timestamp to be on if quiet mode is requested.
 *
 * Revision 1.5  2002/11/18 23:07:10  bergsma
 * When -g (cgi) option is specified, then make the concept name a random number
 * so that this would accomodate a web server that spawned multiple hyperscripts
 * at the same time, because fifo's cannot be shared.
 *
 * Revision 1.4  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.3  2002/10/23 00:12:17  bergsma
 * Required argument added to gHyp_hyp_traceReset
 *
 * Revision 1.2  2002/10/12 02:19:12  bergsma
 * -n option is only allowed with -r option
 *
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"       /* System Interface and Function Prototypes */

#ifdef AS_VMS
#include <ssdef.h>              /* SS$_ defines */
#endif

#ifndef F_OK
#define F_OK            0       /* does file exist */
#define X_OK            1       /* is it executable by caller */
#define W_OK            2       /* writable by caller */
#define R_OK            4       /* readable by caller */
#endif

/********************** EXTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/********************** FUNCTION DECLARATIONS ********************************/
#ifdef AS_PROMIS
#if defined ( AS_VMS ) && defined ( __cplusplus )
extern "C" void aeqSsp_automan_closeFiles ();
extern "C" void aeqSsp_automan_initTrFiles ();
extern "C" void aeqSsp_automan_initDbFiles ();
extern "C" void aeqSsp_automan_disableCallback() ;
extern "C" void cfc_initimage() ;
#else
extern void aeqSsp_automan_closeFiles ();
extern void aeqSsp_automan_initTrFiles ();
extern void aeqSsp_automan_initDbFiles ();
extern void aeqSsp_automan_disableCallback() ;
extern void cfc_initimage() ;
#endif
#endif

#ifdef AS_ATL
#include "interface.h"
#endif /* AS_ATL */

#ifdef AS_JNI
#include <jni.h>
#include "HyperScript2.h"

static JNIEnv *gpEnv = NULL ;
static jclass gpClass = NULL;
static jobject gpObject = NULL ;
static jmethodID gpMID_jeval = NULL ;
static jmethodID gpMID_output = NULL ;
static jmethodID gpMID_shutdown = NULL ;

static SOCKET gsSocket = INVALID_SOCKET ;
static char gzBuffer[MAX_MESSAGE_SIZE];

JNIEXPORT jboolean JNICALL Java_HyperScript2_initJNI
  (JNIEnv * env, jobject obj)
{
  jclass cls1 = (*env)->GetObjectClass(env, obj);
  if (cls1 == 0) return FALSE ;
  gpClass = (*env)->NewGlobalRef(env, cls1);
  if ( gpClass == 0) return FALSE ;

  gpObject = obj ;
  gpEnv = env ;

  gpMID_jeval = (*gpEnv)->GetMethodID(  gpEnv, 
  	     				gpClass, 
					"jeval", 
					"(Ljava/lang/String;)V" ) ;

  gpMID_output = (*gpEnv)->GetMethodID(	gpEnv, 
					gpClass, 
					"output", 
					"(Ljava/lang/String;)V" );

  gpMID_shutdown = (*gpEnv)->GetMethodID(gpEnv, 
					gpClass, 
					"shutdown", 
					"(Ljava/lang/String;)V" );


  return TRUE ;
}

static char eval[MAX_OUTPUT_LENGTH+1];
static void lHyp_hs_jeval ( char *token )
{
  jstring jtoken ;  
  strcpy ( eval, token ) ;
  if ( gpEnv == NULL ) return ;
  jtoken = (*gpEnv)->NewStringUTF(gpEnv, eval );
  (*gpEnv)->CallVoidMethod( gpEnv, gpObject, gpMID_jeval, jtoken );
  (*gpEnv) ->ReleaseStringUTFChars(gpEnv, jtoken, eval ) ;
}


static char output[MAX_OUTPUT_LENGTH+1];
static void lHyp_hs_output ( char *token )
{
  jstring joutput ;
  strcpy ( output, token ) ;
  if ( gpEnv == NULL ) return ;
  joutput = (*gpEnv)->NewStringUTF(gpEnv, output );
  (*gpEnv)->CallVoidMethod( gpEnv, gpObject, gpMID_output, joutput );

  /* The following call has been problematic, causing fatal
   * dumps of the HS.DLL.  Commenting it is not right, but
   * does seem to prevent crashes.  
   * 
   */
  (*gpEnv) ->ReleaseStringUTFChars(gpEnv, joutput, output ) ;
}

void gHyp_hs_output ( char *token )
{
  lHyp_hs_output ( token ) ;
}

static void lHyp_hs_shutdown ( char *token )
{
  jstring jtoken ;
  if ( gpEnv == NULL ) return ;
  jtoken = (*gpEnv)->NewStringUTF(gpEnv, token );
  (*gpEnv)->CallVoidMethod( gpEnv, gpObject, gpMID_shutdown, jtoken );
  (*gpEnv) ->ReleaseStringUTFChars(gpEnv, jtoken, token ) ;
}

void gHyp_hs_shutdown ( char *token )
{
  lHyp_hs_shutdown ( token ) ;
}

JNIEXPORT jboolean JNICALL Java_HyperScript2_connectLoopback
  (JNIEnv * env, jobject obj, jstring jAddr, jint jPort)
{
  const char *pAddr ;

  pAddr = (*env)->GetStringUTFChars ( env, jAddr, 0 ) ;

  gsSocket = gHyp_tcp_requestJNI ( (char*) pAddr, (int) jPort ) ;

  (*env) ->ReleaseStringUTFChars( env, jAddr, pAddr ) ;

  if ( gsSocket == INVALID_SOCKET )
    return FALSE ;
  else
    return TRUE ;
}

JNIEXPORT void JNICALL Java_HyperScript2_closeLoopback (JNIEnv *env, jobject obj)
{
  if ( gsSocket != INVALID_SOCKET ) gHyp_sock_closeJNI ( gsSocket ) ;
  gsSocket = INVALID_SOCKET ;
}

JNIEXPORT jint JNICALL Java_HyperScript2_readLoopback
  (JNIEnv * env, jobject obj, jbyteArray jBuffer, jint jMaxBytes )
{
  int 
    i,
    timeout = 60 * 1000,
    nBytes ;

  jbyte 
    *pjb ;

  nBytes = gHyp_sock_readJNI ( gsSocket,
				gzBuffer, 
				(int) jMaxBytes,
				timeout,
				&nBytes ) ;

  if ( nBytes > 0 ) {

    pjb = (*env)->GetByteArrayElements( env, jBuffer, NULL ); 

    for ( i=0 ; i<nBytes ; i++ ) { 
      pjb[i] = gzBuffer[i] ;
    } 
    (*env)->ReleaseByteArrayElements( env, jBuffer, pjb, 0); 
  }

  return nBytes ;
}

JNIEXPORT jint JNICALL Java_HyperScript2_writeLoopback
  (JNIEnv * env, jobject obj, jstring jCmd )
{
  const char *pCmd ;
  int nBytes ;

  pCmd = (*env)->GetStringUTFChars ( env, jCmd, 0 ) ;
  nBytes = gHyp_sock_writeJNI (  gsSocket, 
				 (char*) pCmd, 
				  strlen(pCmd) );

  (*env) ->ReleaseStringUTFChars( env, jCmd, pCmd ) ;

  return nBytes ; 

}

JNIEXPORT jstring JNICALL Java_HyperScript2_hs(JNIEnv *env, jobject obj, jstring token )
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

  gpObject = obj ;
  gpEnv = env ;

  pToken = (*env)->GetStringUTFChars ( env, token, 0 ) ;
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
			      debugMask,
			      MAX_EXPRESSION )) {

      (*env) ->ReleaseStringUTFChars(env, token, pToken ) ;
      return (*env)->NewStringUTF ( env, "%INIT" );

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

  (*env) ->ReleaseStringUTFChars(env, token, pToken ) ;

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
      (*env)->DeleteGlobalRef( env, gpClass );

      return (*env)->NewStringUTF ( env, "%LOAD" );
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
    gpAImain = gHyp_concept_getConceptInstance ( gpsConcept ) ;

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
    (*env)->DeleteGlobalRef( env, gpClass );
    return (*env)->NewStringUTF ( env, "%DEATH" );
    gpEnv = NULL ;
  } 
  
  return (*env)->NewStringUTF ( env, "$ACK" );
}

#endif /* AS_JNI */

#if defined( AS_JNI ) || defined( AS_ATL )

void gHyp_hs_jeval ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: jeval()
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pValue,
      *pData ;

    char
      *pStr,
      value[VALUE_SIZE+1],
      command[MAX_OUTPUT_LENGTH+1],
      *pCmd,
      *pEndCmd ;

    int
      ss,
      valueLen,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sLOGICAL
      isVector ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
	gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: jeval ( command )" ) ;
   
    /* Get command to execute */
    pData = gHyp_stack_popRdata ( pStack, pAI ) ;

    /* Construct the message to be sent to the shell. */
    pCmd = command ;
    pEndCmd = pCmd + MAX_OUTPUT_LENGTH ;    
    pValue = NULL ;
    isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;
    ss = gHyp_data_getSubScript ( pData ) ; 
    context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					     pValue,
					     &context,
					     ss ))) {
      valueLen = gHyp_data_getStr ( pValue, 
				    value, 
				    VALUE_SIZE, 
				    context,
				    isVector ) ;
      pStr = value ;
      
      if ( (pCmd + valueLen) > pEndCmd ) 
	gHyp_instance_error ( pAI, STATUS_IO,
			      "\n",
			      MAX_OUTPUT_LENGTH ) ;
      sprintf ( pCmd, "%s", pStr ) ;
      pCmd += valueLen ;   
    }
    if ( context== -2 && ss != -1 ) 
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in jeval()",ss) ;
    *pCmd = '\0' ;	

#ifdef AS_JNI
    lHyp_hs_jeval ( command ) ;
#else /* AS_ATL */
	gHyp_client_jeval ( command ) ;
#endif

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

#endif /* defined( AS_JNI ) || defined( AS_ATL ) */

#if defined ( AS_DLL ) && !defined ( AS_ATL )

static HINSTANCE ghInstance = NULL;

BOOL APIENTRY DllMain(HINSTANCE hModule, 
		      DWORD ul_reason_for_call, 
		      LPVOID lpReserved
		     )
{

    switch (ul_reason_for_call) {

	case DLL_PROCESS_ATTACH :
	  /*printf( "DllMain: DLL_PROCESS_ATTACH for hModule = 0x%x\n", hModule);*/
	    ghInstance = hModule;
	    break;

	case DLL_PROCESS_DETACH :
	{
	  /*gHyp_hs_shutdown("DLL DETACH EVENT") ;*/

	  /*printf( "DllMain: DLL_PROCESS_DETACH\n");*/
	  break;
	}

	//case DLL_THREAD_ATTACH :
	//case DLL_THREAD_DETACH :
    }

    return(TRUE) ;
}

#endif

/********************** FUNCTION DEFINITIONS ********************************/

#ifdef AS_VMS                                                        
static unsigned int lHyp_hs_exceptionHandler ( void *sigarr, void *mecharr )
{
  /* Called when VMS exception occurs, re-signals to 'C' handlers */
  gHyp_util_log ( "Exception condition occurred - exiting" ) ;

  if ( gpsConcept ) {
    /* If the method was invoked from a query, then send all replies */
    if ( gpAImain ) {
      while ( gHyp_instance_replyMessage ( 
	      gpAImain,
	      gHyp_frame_getMethodData(gHyp_instance_frame(gpAImain) ) ) ) ;
    }
    guRunFlags &= ~RUN_QUIET ;

#ifdef AS_PROMIS
  aeqSsp_automan_closeFiles ( ) ;
  gHyp_promis_cleanFields ( -1 ) ;
#endif

    gHyp_concept_quit ( gpsConcept ) ;
    gpsConcept = NULL ;

    if ( gsSocketToCancel != INVALID_SOCKET ) 
      gHyp_sock_cancelIO(gsSocketToCancel) ;

  }    

  /* We never want to resignal - all exceptions are fatal */

  /* Exit with giCondition instead */
  exit(giCondition);

  return SS$_RESIGNAL ;
}
#endif

#if !(defined ( AS_DLL ) || defined ( AS_ATL ))
int main ( int argc, char * argv[] )
{
  /* Description:
   *
   *    Mainline program for HyperScript interpretor
   *
   * Arguments:
   *
   *    Command line arguments.  See usage text below.
   *
   * Return value:
   *
   *    0
   */

#define NUM_OPTIONS 18

  char *usage[NUM_OPTIONS] =
  {
    "usage: -c                     (compile only)\n",
    "       -d mask                (debug mask: OR of {1,2,4,8,..,512} )\n",
    "       -e method              (enable 'method' to receive messages)\n",
    "       -f file                (input from 'file' instead of stdin)\n",
    "       -g                     (parse cgi POST or GET)\n",
    "       -h                     (print this help)\n",
    "       -i                     (interactive mode)\n",
    "       -l log                 (output to 'log' instead of stdout)\n",
    "       -n service/port        (tcp/ip listen service name or port)\n",
    "       -q                     (quiet mode)\n",
    "       -r                     (ROOT mode)\n",
    "       -s stream              (input from 'stream' instead of stdin)\n",
    "       -t [instance#]object   (set target name of program)\n",
    "       -v                     (echo input stream)\n",
    "       -w                     (ward fileio functions\n",
    "       -x                     (double expression size)\n",
    "       -y                     (Debug cgi scripts)\n",
    "       [arg1 [arg2 [arg3 [...]]]]\n"
  };

  sFrame
    *pFrame ;

  int
    i,
    c,
    m,
    n,
    t,
    condition,
    stmtIndex,
    maxExprSize=MAX_EXPRESSION ;

  unsigned
    runFlags,
    debugMask ;
  
  sLOGICAL
    errflg = FALSE,
    verify = FALSE,
    methodsEnabled=FALSE,
    interpretor=FALSE,
    interactive=FALSE,
    compileOnly=FALSE,
    quiet=FALSE,
    cgiParse=FALSE,
    streaming=FALSE,
    endOfFile=FALSE,
    debugCGI=FALSE;
  
  sHyp
    *pHyp ;

  sData
    *pAIdata,
    *pMethodVariable ;
  
  char
    *pStream,
    target[TARGET_SIZE+1],
    service[OBJECT_SIZE+1],
    method[10][METHOD_SIZE+1],
    program[MAX_PATH_SIZE+1],
    stream[MAX_INPUT_LENGTH+1] ;

#ifndef _WINDOWS_
  char
        *pChar ;
#endif

#ifdef AS_PROMIS
  /* All PROMIS images require cfc_initimage */
  cfc_initimage() ;
#endif

#ifdef AS_VMS
  /* VMS says this handler prevents bad behavior from longjmp */
  VAXC$ESTABLISH ( &lHyp_hs_exceptionHandler ) ;
#endif

  /* Initialize */
  gsLog = stdout ;
  target[0] = '\0' ;
  m = 0 ;       /* Index into method array */
  program[0] = '\0' ;
  service[0] = '\0' ;
  gzLogName[0] = '\0' ;
  stream[0] = '\0' ;
  debugMask = 0 ;
  runFlags = 0 ;
     
  /* Get command line arguments and set flags, options */
  while ((c = gHyp_util_getopt ( argc, argv,"cd:e:f:ghil:n:qrs:t:vwxy")) != EOF) {
  
    switch ( c ) {
      

    case 'c' : 
      compileOnly = TRUE ;
      runFlags |= RUN_COMPILEONLY ;
      break ;   

    case 'd' : 
      i = sscanf( gzOptarg, "%u", &debugMask ) ;
      if ( i != 1 ) debugMask = 0 ; 
      break ;   
      
    case 'e' :
      n = MIN ( strlen ( gzOptarg ), METHOD_SIZE ) ;
      strncpy ( method[m], gzOptarg, n ) ;
      method[m][n] = '\0' ;
      m++ ;
      break ;   
      
    case 'f' : 
      
#ifdef AS_UNIX
      /* Determine if this is an interpretor call by seeing
       * if the argument is an executable program. If so, then
       * take the argument as the name of the target.
       */
      if ( access ( gzOptarg, (R_OK|X_OK) ) != -1 ) {
        
	if ( !cgiParse && !target[0] ) {
          interpretor = TRUE ;
          pChar = strrchr ( gzOptarg, '/' ) ;
          if ( pChar ) 
	    pChar++ ;
          else
            pChar = gzOptarg ;
	  n = MIN ( strlen ( pChar ), MAX_PATH_SIZE ) ;
          strncpy ( target, pChar, n ) ;
	  target[n] = '\0' ;
        
	  /* Add -e CONNECT -e DISCONNECT -e ABORT */
	  strcpy ( method[m++], "CONNECT"  ) ;
	  strcpy ( method[m++], "DISCONNECT" ) ;
	  strcpy ( method[m++], "ABORT" ) ;

	}
      }
#endif
      /* Must be a .hyp file */
      n = MIN ( strlen ( gzOptarg ), MAX_PATH_SIZE ) ;
      strncpy ( program, gzOptarg, n ) ;
      program[n] = '\0' ;
      break ;

     case 'g' : 
      quiet = TRUE ;
      runFlags |= RUN_QUIET ;
      cgiParse = TRUE ;
      break ;   
      
    case 'i' : 
      interactive = TRUE ;
      runFlags |= RUN_INTERACTIVE ;
      break ;   
          
    case 'l' : 
      n = MIN ( strlen ( gzOptarg ), MAX_PATH_SIZE ) ;
      strncpy ( gzLogName, gzOptarg, n ) ;
      gzLogName[n] = '\0' ;
      break ;   
               
    case 'n' : 
      n = MIN ( strlen ( gzOptarg ), OBJECT_SIZE ) ;
      strncpy ( service, gzOptarg, n ) ;
      service[n] = '\0' ;
      break ;   
               
    case 'q' : 
      quiet = TRUE ;
      runFlags |= RUN_QUIET ;
      break ;                    

    case 'r' : 
      runFlags |= RUN_ROOT ;
      break ;                    

    case 'w' : 
      runFlags |= RUN_RESTRICTED ;
      break ;                    

    case 's' :
      streaming = TRUE ;
      n = MIN ( strlen ( gzOptarg ), MAX_INPUT_LENGTH ) ;
      strncpy ( stream, gzOptarg, n ) ;
      stream[n] = '\0' ;
      strcat ( stream, " ;"  ) ;
      break ;   
      
    case 't' :
      n = MIN ( strlen ( gzOptarg ), TARGET_SIZE ) ;
      strncpy ( target, gzOptarg, n ) ;
      target[n] = '\0' ;
      break ;   

    case 'v' :
      verify = TRUE ;
      runFlags |= RUN_VERIFY ;
      break ;   

    case 'x' :
      maxExprSize *=2 ;
      break ;   

    case 'y' :
      debugCGI = TRUE ;
      break ;

    case 'h' :
    case '?':
    default:
      errflg = TRUE ;
      break;
    }
  }
   
  /* Get arguments */
  for ( i=giOptind; i<argc; i++ ) {       
    if ( i == giOptind && interpretor ) {
      /* Last argument is assigned to a log file */
      n = MIN ( strlen ( argv[argc-1] ), MAX_PATH_SIZE ) ;
      strncpy ( gzLogName, argv[argc-1], n ) ;
      gzLogName[n] = '\0' ;
    }
  }
  
  if ( errflg ) {
    for (i = 0; i < NUM_OPTIONS; i++)
      fprintf (stderr, usage[i] );
   exit (2) ;
  }

  if ( debugCGI ) {

    gHyp_util_output("Content-type: text/plain\n\n");

    for ( i=0; i<argc; i++ ) 
      gHyp_util_logInfo ( "argv[%d] = %s", i, argv[i] ) ;
  }

  /* Set default program source if not already set */
  if ( program[0] ) {
    if ( (gsPP = fopen ( program, "r" )) == NULL ) {
      gHyp_util_sysError ( "Failed to open program '%s'", program ) ;
      exit(2) ;
    }
  } 
  else {
    strcpy ( program, "stdin" ) ;
    gsPP = stdin ;
  }

  /* Check argument compatability *
  if ( service[0] && !(runFlags & RUN_ROOT)) {
      gHyp_util_logError ( "-n option is only valid when -r option is specified" ) ;
      exit(2) ;
  }
  */

  /* Set random target name if cgi script else set default target name if not already set. */
  if ( cgiParse ) 
    strcpy ( target, gHyp_util_random8() ) ;
  else if ( target[0] == '\0' ) 
    gHyp_util_getprocessname ( target ) ;


  /* 
   *    hs -rt router -n autorouter
   *            /local/fifo/router              ROOT fifo inbox
   *            /local/fifo/.router             ROOT directory
   *            - no outbox -                   
   *                                            
   *    ROOT Suffix R = "/local/fifo/.router"
   *
   *    hs -t concept
   *            R/concept                       concept fifo inbox              
   *            R/.concept                      concept directory
   *            /local/fifo/router              parent fifo outbox              
   *    
   *    hs -t child/concept
   *            R/.concept/child                child fifo inbox
   *            R/.concept/.child               child directory
   *            R/concept                       parent fifo outbox              
   *    
   *    hs -t subchild/child/concept
   *            R/.concept/.child/subchild      subchild fifo inbox
   *            R/.concept/.child/.subchild     subchild directory  
   *            R/.concept/child                parent fifo outbox       
   */

  /* Set global copy of run flags */
  guRunFlags = runFlags ;

  /* Create and initialize a concept with the new instance */
  gpsConcept = gHyp_concept_new () ;
  if ( !gHyp_concept_init ( gpsConcept, 
                            target,
                            service,
                            debugMask,
                            maxExprSize )) return 1 ;

#ifdef AS_PROMIS
  aeqSsp_automan_initDbFiles () ;
  aeqSsp_automan_initTrFiles () ;
  gHyp_promis_initFields() ;
#endif

  gpAI = gpAImain = gHyp_concept_getConceptInstance ( gpsConcept ) ;

  pFrame = gHyp_instance_frame ( gpAI ) ;
  if ( cgiParse ) gHyp_cgi_parse ( gpAI, pFrame ) ;

  /* When using HyperScript put() function, print out timestamps */
  /*if ( !interactive ) guTimeStamp = TRUE ;*/

  /* Disable execution if just compiling */
  if ( compileOnly ) gHyp_instance_reset ( gpAI, STATE_PARSE, FALSE ) ;

  if ( gsPP == stdin && !quiet ) 
    gHyp_util_logInfo ( "Ready to load program" ) ;

  /* Load and execute HyperScript from input stream.
   * Parse and execute the program until the end of input is reached 
   */
  condition = COND_SILENT ;
  while ( 1 ) {

    /* Execute HyperScript. */
    if ( guSIGTERM ) break ;

    pAIdata = gHyp_concept_nextInstanceData ( gpsConcept ) ;
    if ( !pAIdata ) break ;

    gpAI = (sInstance*) gHyp_data_getObject ( pAIdata ) ;
    gpAImain = gHyp_concept_getConceptInstance ( gpsConcept ) ;
    condition = gHyp_instance_run ( gpAI ) ;

    if ( guSIGTERM ) break ;
        
    if ( condition == COND_FATAL ) {

      /* Fatal errors (-2) are never recoverable */
      break ;

    }
    else if ( condition == COND_ERROR ) {

      /* The instance has an error */

      /* Run time-errors are recoverable in "interactive" mode */
      if ( interactive && gsPP == stdin && gpAI == gpAImain ) {

        /* Recover by backing up to the previous good statement */
        pFrame = gHyp_instance_frame ( gpAI ) ;
        stmtIndex = gHyp_frame_getStatementIndex ( pFrame ) ;
        gHyp_util_logInfo ( "Ignoring last statement.  Please continue." ) ;
        gHyp_hyp_traceReset (gHyp_data_getLabel(gHyp_frame_getMethodVariable(pFrame))) ;
        gHyp_frame_resetLevel ( gHyp_frame_level ( pFrame ) ) ;
        gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
        pHyp = gHyp_frame_getHyp ( pFrame ) ;
        gHyp_hyp_setHypCount ( pHyp, stmtIndex ) ; 
        gHyp_frame_setHypIndex ( pFrame, stmtIndex ) ;
        gHyp_instance_clearError ( gpAI ) ;
        gHyp_instance_reset ( gpAI, STATE_PARSE, TRUE ) ;
        gHyp_concept_setReturnToStdIn ( gpsConcept, TRUE ) ;
      }
      else {

        /* Un-caught errors in the main instance begets them all */
        if ( gpAI == gpAImain ) break ;

        /* Otherwise a instance can die and not affect the other instances */
        gHyp_data_detach ( pAIdata ) ;
        gHyp_data_delete ( pAIdata ) ;
      }
    }
    else {

      /* COND_NORMAL or COND_SILENT */
      
      if ( gpAI == gpAImain &&
           gHyp_concept_returnToStdIn ( gpsConcept ) &&
           gHyp_instance_getState ( gpAImain ) != STATE_QUERY ) {
                
        /* See if more input can be loaded into the program */
        pFrame = gHyp_instance_frame ( gpAImain ) ;
        pHyp = gHyp_frame_getHyp ( pFrame ) ;
        
        if ( streaming ) {
          
          if ( !stream[0] ) break ;

          /* Input came from -s "stmt" */         
          pStream = gHyp_load_fromStream ( gpAImain, pHyp, stream, giLineCount++ ) ;
          if ( pStream == NULL || *pStream ) {
            gHyp_util_logError ( "Failed to load HyperScript stream" ) ;
            break ;
          }
          
          /* Got to parse it and then we can not return for more stdin. */
          gHyp_instance_setState ( gpAImain, STATE_PARSE ) ;

          stream[0] = '\0' ;
        }
        else if ( !endOfFile && gHyp_hyp_source ( gpAImain, pHyp, gsPP, program, TRUE ) > 0 ) {
          
          /* More tokens loaded - set PARSE state and call gHyp_instance_run */
          gHyp_instance_setState ( gpAImain, STATE_PARSE ) ;

#ifdef AS_PROMIS
	  aeqSsp_automan_disableCallback () ; ;
#endif

        }
        else {
          
	  /* Disable return to stdin so we don't get caught in an endless loop */
          gHyp_concept_setReturnToStdIn ( gpsConcept, FALSE ) ;

	  /* If already end-of-file from before, then continue */
	  if ( endOfFile ) continue ;

	  /*gHyp_util_debug("STATE is %d", gHyp_instance_getState ( gpAImain ) ) ;*/
	  /* End-of-file or ^D (^Z) typed or just returning to IDLE */
          
          if ( !quiet ) {
            gHyp_util_output ( "End-of-file: HyperScript program completed loading\n") ;
            gHyp_util_logInfo ( "%d errors detected.",guErrorCount ) ;
          }
          if ( program[0] ) fclose ( gsPP ) ;
	  endOfFile = TRUE ;
          
          pHyp = gHyp_frame_getHyp ( pFrame ) ;
          t = pHyp ? gHyp_hyp_getHypCount ( pHyp ) : 0 ;
          if ( !quiet ) 
            gHyp_util_logInfo ( "Program: Lines: %u, Tokens: %u", 
                                giProgramCount, t ) ; 
          
          if ( guErrorCount > 0 && !interactive ) break ;
          if ( compileOnly ) break ;
          
          /* If one or more "-e method" arguments are specified, then idle */
          for ( i=0; i<m; i++ ) {
            
            pMethodVariable = gHyp_frame_findMethodVariable ( pFrame, method[i], gpAImain ) ;
            if ( pMethodVariable ) {
              gHyp_method_enable ( (sMethod*) gHyp_data_getObject ( pMethodVariable ) ) ;
              methodsEnabled = TRUE ;
            }
          }
          
          /* At least one of the methods specified must be defined */   
          if ( !methodsEnabled ) break ;
        
          /* Set IDLE state, waiting for first message on fifo/mailbox device. */
	  /*gHyp_util_debug("Reverting to IDLE");*/
          gHyp_instance_reset ( gpAImain, STATE_IDLE, TRUE ) ;
        }
      }
      else {

	/* gpAI != gpAImain ||
           !gHyp_concept_returnToStdIn ( gpsConcept ) ||
           gHyp_instance_getState ( gpAImain ) == STATE_QUERY ) 
	 */

	/*gHyp_util_debug("Nothing to do here");*/
      }
    }
  } 

#ifdef AS_PROMIS
  aeqSsp_automan_closeFiles ( ) ;
  gHyp_promis_cleanFields ( -1 ) ;
#endif

  /*gHyp_util_debug("Quiting");*/
  gHyp_concept_quit ( gpsConcept ) ;  

#ifdef PROFILE
    exit(2) ;
#endif

  return giCondition ;
}
#endif
