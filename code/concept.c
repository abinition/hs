/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!***************************************************************************/
/* Modifications: 
 *
 * $Log: concept.c,v $
 * Revision 1.38  2006/08/22 18:44:37  bergsma
 * Resolve Win32 problem with unlink
 *
 * Revision 1.37  2006/08/22 13:28:13  bergsma
 * Comment out 'unlink' to gzPIDfileSpec
 *
 * Revision 1.36  2006/08/09 00:51:22  bergsma
 * Undo last change to createNetwork
 *
 * Revision 1.35  2006/08/08 20:50:59  bergsma
 * In createNetwork, prevent closing of valid socket on another IP channel.
 *
 * Revision 1.34  2006/01/20 04:09:34  bergsma
 * Reassign fixes
 *
 * Revision 1.33  2006/01/20 02:55:09  bergsma
 * Fix to reassign port
 *
 * Revision 1.32  2006/01/19 20:37:40  bergsma
 * Added gHyp_concept_getInstForPlaceHolderFd
 *
 * Revision 1.31  2006/01/16 18:56:35  bergsma
 * HS 3.6.6
 * 1. Save query timeout events.  Don't let queries repeat indefinitely.
 * 2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 * Revision 1.30  2005/11/23 16:52:32  bergsma
 * Added swapDevices in renameTo function
 *
 * Revision 1.29  2005/10/20 00:31:43  bergsma
 * Reconnect logic.  Debug statements only.
 *
 * Revision 1.28  2005/10/15 21:42:09  bergsma
 * Added renameto functionality.
 *
 * Revision 1.27  2005/08/21 17:58:35  bergsma
 * no message
 *
 * Revision 1.26  2005/03/30 16:45:09  bergsma
 * HS 3.5.4.   In VMS, do not set the process name if the name is already set to the
 * correct HS concept name. NB:  In VMS 7.3-2 process names are case sensitive.
 *
 * Revision 1.25  2005/03/16 23:53:21  bergsma
 * V 3.5.1 - fixes for use with DECC compiler.
 *
 * Revision 1.24  2005/01/31 06:03:12  bergsma
 * Test eventTime <= gsCurTime  is better than eventTime == gsCurTime
 *
 * Revision 1.23  2005/01/10 20:03:05  bergsma
 * Initialize giJmpOverride value
 *
 * Revision 1.22  2004/11/19 03:45:58  bergsma
 * New gpsTempData2 variable to catch memoiry leaks.
 *
 * Revision 1.21  2004/07/01 02:18:48  bergsma
 * Log message changes
 *
 * Revision 1.20  2004/06/13 14:05:07  bergsma
 * Cosmetic log statements.
 *
 * Revision 1.19  2004/04/29 08:30:17  bergsma
 * Device Id is unique and is used interchangeably between PORT, HSMS, SECSI, and HTTP sockets.
 *
 * Revision 1.18  2003/11/16 20:58:30  bergsma
 * Typo in log statement.
 *
 * Revision 1.17  2003/04/11 01:54:13  bergsma
 * Use gpsTempData variable to hold data that could be leaked when
 * gHyp_instance_error is invoked.
 *
 * Revision 1.16  2003/04/04 16:41:01  bergsma
 * Change arguments to sock_logger - more descriptive.
 *
 * Revision 1.15  2003/01/28 01:59:46  bergsma
 * Added AUTOBIN. Default is /local/bin
 * Initialized giCondition to COND_SILENT (0).
 *
 * Revision 1.14  2003/01/14 15:52:46  bergsma
 * If -r option was not specified, and concept is the AUTOROUTER, then set
 * RUN_ROOT flag.
 *
 * Revision 1.13  2003/01/14 02:31:57  bergsma
 * Always DISCONNECT from parent when both inbox and outbox are defined.
 *
 * Revision 1.12  2003/01/12 13:55:46  bergsma
 * V3.1.0
 * 1. Add unlink() definition for AS_VMS
 * 2. Check that root hyperscript name matches AUTOROUTER environment variable
 * 3. Translate environment variables HOME or DOCUMENT_ROOT
 * 4. Use gzInstance to track which instance is currently executing
 * 5. Do not automatically open the inbox, wait until needed.  This allows
 * the moveto() function to decide exactly where the concept should reside
 * in the hyperscript tree.
 * 6. Added writeParent, where HyperScript will attempt to start the parent
 * if the message is CONNECT
 * 7. Added the moveto() function
 *
 * Revision 1.11  2002/11/28 14:32:28  bergsma
 * 1. unlink ( gzPIDfile ) when the program exits.
 * 2. fix spin problem with maxTries in gHyp_concept_write
 *
 * Revision 1.10  2002/11/20 20:38:39  bergsma
 * Removed ^M
 *
 * Revision 1.9  2002/11/20 20:33:00  bergsma
 * A HyperScript can be started as an instance other than the parent instance.
 * The gzRoot variable is set here. The changes reflect the new capabilities
 * of HyperScript to send messages up or down the hierarchy to specific
 * instances.
 *
 * Revision 1.8  2002/11/12 20:11:37  bergsma
 * Removed redundant if statement in openReader
 *
 * Revision 1.7  2002/10/27 20:41:33  bergsma
 * New 4th argument to gHyp_sock_fifo calls.
 *
 * Revision 1.6  2002/10/24 01:48:10  bergsma
 * In concept_into, for VMS, openReader will always fail for clients,
 * so it is not called.
 *
 * Revision 1.5  2002/10/23 00:04:14  bergsma
 * Detect failures for gHyp_concept_openReader and gHyp_concept_openWriter.
 *
 * Revision 1.4  2002/09/24 17:19:29  bergsma
 * Added flags giResolveFlag1 and giResolveFlag2, plus string variable
 * gzResolveAddr.
 *
 * Revision 1.3  2002/09/21 22:10:55  bergsma
 * Make localhost lowercase before putting to alias table.
 *
 * Revision 1.2  2002/09/09 20:44:40  bergsma
 * In concept_write, return FALSE if concept_openWriter fails
 *
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/
#include "auto.h"       /* System Interface and Function Prototypes */

#ifdef AS_TRUE64
#include "mem.h"
#endif

/********************** EXTERNAL GLOBAL VARIABLES ****************************/

/* File names and other string variables */
char            gzLogName[MAX_PATH_SIZE+1] ;
FILE            *gsLog ;        /* Log */

/* Socket channels */
int             giServicePort ; /* Service Channel id */
int             giARservicePort ;       /* Autorouter Service Channel id */
SOCKET          giLoopback ;		/* Loopback socket outgoing */
sBYTE		giLoopbackType ;	/* Loopback socket type */
HANDLE          giInboxLoopback ;       /* Loopback socket to inbox */
SOCKET          giListenLoopback_R ;      /* Loopback socket (read) to listen socket */
SOCKET          giListenLoopback_W ;      /* Loopback socket (write) to listen socket */

sLOGICAL	giResolveFlag1 ;
sLOGICAL	giResolveFlag2 ;
char		gzResolveAddr[HOST_SIZE+1];

/* Run-time variables */
int             giPID ;         /* PID of process */
char		gzPIDfileSpec[MAX_PATH_SIZE+1] ; /* PID file */
time_t          gsCurTime ;     /* Current time, updated regularily. */
sLOGICAL        guTimeStamp ;   /* Integer timeStamp. */
unsigned short  guRunFlags ;    /* General flags */

/* Environment variables */
char            gzAUTOROUTER[OBJECT_SIZE+1];    /* AUTOROUTER name, i.e. "router" */
char            gzHOME[MAX_PATH_SIZE+1];  /* DOCUMENT_ROOT path */
char            gzAUTOFIFO[MAX_PATH_SIZE+1];    /* AUTOFIFO path */
char            gzAUTORUN[MAX_PATH_SIZE+1];     /* AUTORUN path */
char            gzAUTOBIN[MAX_PATH_SIZE+1];     /* AUTOBIN path */
char            gzAUTOLOG[MAX_PATH_SIZE+1];     /* AUTOLOG path */
char            gzAUTOSPOOL[MAX_PATH_SIZE+1];     /* AUTOSPOOL path */
char            gzInstance[INSTANCE_SIZE+1] ;
char            gzConcept[TARGET_SIZE+1] ;
char            gzConceptPath[MAX_PATH_SIZE+1] ;
char            gzParent[MAX_PATH_SIZE+1] ;
char            gzRoot[MAX_PATH_SIZE+1] ;
char            gzInboxPath[MAX_PATH_SIZE+1] ;
char            gzOutboxPath[MAX_PATH_SIZE+1] ;
char            gzLocalHost[MAX_PATH_SIZE+1] ;
char            gzLocalAddr[MAX_PATH_SIZE+1] ;

/* HyperScript code variables */
sData*		gpsTempData ;	/* If defined, its a leak */
sData*		gpsTempData2 ;	/* If defined, its a leak */
int             giMaxExprSize ; /* Heap allocation for expression */
int             giMaxStackDepth ; /* Heap allocation for stack */
sConcept*       gpsConcept ;    /* Global reference to concept */
unsigned short  guDebugFlags ;  /* Debug flags */
unsigned        guErrorCount ;  /* HyperScript error count */
int	        giLineCount ;
int	        giProgramCount ;
int		giCondition ;
sLOGICAL        giJmpEnabled ; /* If longjmp is enabled from handlers*/
SOCKET          gsSocketToCancel ; /* If cancel I/O is needed. */

int             giJmpLevel ; /* Current depth of saved jmp env */
jmp_buf         gsJmpStack[MAX_JMP_LEVEL+1] ; /* For setjmp,longjmp */ 
jmp_buf         gsJmpOverride ; /* Overrides gsJmpStack */ 
sLOGICAL        giJmpOverride ; /* Overrides gsJmpStack */ 
FILE*           gsPP ;          /* Hyperscript input stream */
sAImsg*         gpsAImsg  ;     /* Working message structure */

/* Variables for the gHyp_util_getopt function call - in util.c */

/*#if !defined(__cplusplus) && !defined(AS_VMS)*/
int             giOpterr = 1 ;  /* Useless, never set or used */
int             giOptind = 1 ;  /* Index into parent argv vector */
/*#endif*/
int             giOptopt ;      /* Character checked for validity */
char            *gzOptarg ;     /* Argument associated with option */

/* Global signal flags */
unsigned                guSIGIO ;       /* Set when SIGIO */
unsigned                guSIGPIPE ;     /* Set when SIGPIPE */
unsigned                guSIGALRM ;     /* Set when SIGALRM */
unsigned                guSIGINT ;      /* Set when SIGINT */
unsigned                guSIGTERM ;     /* Set when SIGTERM */
unsigned                guSIGHUP ;      /* Set when SIGHUP */
unsigned                guSIGMBX ;      /* Set when SIGMBX */

/********************** INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/


#ifdef AS_VMS
#ifdef __cplusplus
extern "C" int sys$setprn ( sDescr* ) ;
extern "C" int lib$set_logical ( sDescr*, sDescr*, sDescr*, int, int );
#else
extern int sys$setprn ( sDescr* ) ;
extern int lib$set_logical ( sDescr*, sDescr*, sDescr*, int, int );
#endif
#endif

#if defined (AS_VMS) && defined ( __cplusplus )
extern "C" int unlink ( const char * ) ;
#else
#ifndef AS_WINDOWS
extern int unlink ( const char * ) ;
#endif
#endif

#ifndef AS_WINDOWS
#ifdef __cplusplus
extern "C" pid_t getpid(void);
extern "C" unsigned int sleep(unsigned int seconds);
#else
extern pid_t getpid(void);
extern unsigned int sleep(unsigned int seconds);
#endif
#endif

/* There is one concept object for all instances. */

struct concept_t
{
  struct {
    sData       *pInstances ;                           /* Ring of instances. */
    sHash       *(*tokens)[MAX_HASH_TABLE_SIZE] ;       /* Common Token Storage Area */    
    sLOGICAL    returnToStdIn ;                 /* If true, more stdin input is allowed */      
  } exec ;      /* Execution area */
  
  struct {
    /* Links:
     *
     * 
     */
    HANDLE      outbox ;
    OVERLAPPED  outboxOverlapped ;
    SOCKET      listenTCP ;
    sSSL*	pSSL ;  /* If listenTCP requires SSL */
    HANDLE      inbox ;
    sLOGICAL    hasRegistered ;
    sData       *pClients ;     /* Ring of client concept channels */
    sData       *pHosts ;       /* Ring of remote concept channels */
    sData       *pSockets ;     /* Ring of application sockets */
  } link ;

} ;

/***************        FUNCTION DEFINITIONS ********************************/

sConcept* gHyp_concept_new ()
{
  /* Description:
   *
   *    Allocate memory for a new instance object.
   *
   * Arguments:
   *
   *    none
   *
   * Return value:
   *
   *    Pointer to new instance object.
   */
  /* Create new instance */
  sConcept *pConcept ;
  pConcept = (sConcept*) AllocMemory ( sizeof ( sConcept ) ) ;
  assert ( pConcept ) ;
  return pConcept ;
}

static sLOGICAL lHyp_concept_initEnvironment ( char *concept )
{

/*
  FILE
    *pidFile ;

  char
    pidStr[15] ;
*/

#ifdef AS_VMS
  int           status ;
  char		processName[TARGET_SIZE+1] ;
  makeDSCz      ( concept_d, concept ) ;
  makeDSCz      ( tempMailbox_d, "LNM$TEMPORARY_MAILBOX" ) ;
  makeDSCz      ( system_d,      "LNM$SYSTEM" ) ;
  makeDSCz      ( processDir_d,  "LNM$PROCESS_DIRECTORY" ) ;

  /* First, get the process name, and only if it is
   * different, do we set it.
   */
  gHyp_util_getprocessname ( processName ) ;
  gHyp_util_lowerCase ( processName, strlen ( processName ) ) ;
  
  /* We assume concept_d is in lower case. :-) */
  if ( strcmp ( processName, concept ) != 0 ) {

    /* Not the same, set the process name */
    status = sys$setprn ( &concept_d ) ;
    if ( !gHyp_util_check ( status, 1 ) ) 
      return gHyp_util_logError ( "Failed to set process name" );
  }

  if ( (guRunFlags & RUN_ROOT) ) {
    /* Define all mailboxes to be created SYSTEM wide and TEMPORARY */
    status = lib$set_logical (  &tempMailbox_d,
                                &system_d,
                                &processDir_d,
                                0,0 ) ;
    if ( !gHyp_util_check ( status, 1 ) ) 
      return gHyp_util_logError ( 
        "Failed to set temporary mailboxes to system logical name table" ) ;
  }
#endif

  if ( (guRunFlags & RUN_ROOT ) ) {
    if ( strcmp ( concept, gzAUTOROUTER ) != 0 ) 
      return gHyp_util_logError ( 
        "Root concept '%s' does not match environment variable AUTOROUTER='%s'",
        concept, gzAUTOROUTER ) ;
  }

#ifndef AS_WINDOWS
  /* Get the PID of the process */
  giPID = getpid() ;
#else
  giPID = GetCurrentProcessId();
; 
#endif

  /* Create <concept>.pid *
  sprintf ( gzPIDfileSpec, "%s.pid", concept ) ;
  pidFile = fopen ( gzPIDfileSpec, "w" ) ;
  if ( pidFile != NULL ) {
#ifdef AS_VMS
    sprintf ( pidStr, "%08x\n", giPID ) ;
#else
    sprintf ( pidStr, "%u\n", giPID ) ;
#endif
    fputs ( pidStr, pidFile ) ;
    fclose ( pidFile ) ;
   }    
  */

  return TRUE ;
}

static sLOGICAL lHyp_concept_initVariables ()
{
  char
    *pVar ;

#ifndef AS_VMS
  int
    i,
    n;
#endif

  /* Determine HOME from the HOME or DOCUMENT_ROOT  */
  gzHOME[0] = '\0' ;
  if ( ( pVar = getenv ( "HOME" ) ) != NULL ) {
    strcpy ( gzHOME, pVar ) ;
  }
  else if ( ( pVar = getenv ( "DOCUMENT_ROOT" ) ) != NULL ) {
    strcpy ( gzHOME, pVar ) ;
  }
  else {
    gHyp_util_logWarning ( "Environment variable or HOME or DOCUMENT_ROOT was not defined" ) ;
    gHyp_util_logInfo ( "Defaulting to ''" ) ;
  }

#ifndef AS_VMS
  n = strlen ( gzHOME ) ;
  for ( i=0; i<n; i++ ) {
#ifdef AS_WINDOWS
    if ( gzHOME[i] == '/' ) gzHOME[i] = '\\' ;
    if ( i == n-1 && gzHOME[i] == '\\' ) gzHOME[i] = '\0' ;
#else
    if ( gzHOME[i] == '\\' )  gzHOME[i] = '/' ;
    if ( i == n-1 && gzHOME[i] == '/' ) gzHOME[i] = '\0' ;
#endif
  }
#endif

  /* Determine the ROOT mailbox or fifo name  */
  gzAUTOROUTER[0] = '\0' ;
  if ( ( pVar = getenv ( "AUTOROUTER" ) ) != NULL ) {
    strcpy ( gzAUTOROUTER, pVar ) ;
    gHyp_util_lowerCase ( gzAUTOROUTER, strlen ( gzAUTOROUTER ) ) ; 
  }
  else {
    gHyp_util_logWarning ( "Environment variable AUTOROUTER was not defined" ) ;
    gHyp_util_logInfo ( "Defaulting to router" ) ;
    strcpy ( gzAUTOROUTER, "router" ) ;
  }

  /* Determine the FIFO area */
  gzAUTOFIFO[0] = '\0' ;
  if ( ( pVar = getenv ( "AUTOFIFO" ) ) != NULL ) {
    strcpy ( gzAUTOFIFO, pVar ) ;
    gHyp_util_lowerCase ( gzAUTOFIFO, strlen ( gzAUTOFIFO ) ) ; 
  }
  else {
    gHyp_util_logWarning ( "Environment variable AUTOFIFO was not defined" ) ;
#ifdef AS_VMS
    gHyp_util_logInfo ( "Defaulting to mbx" ) ;
    strcpy ( gzAUTOFIFO, "mbx" ) ;
#else
#ifdef AS_WINDOWS
   gHyp_util_logInfo ( "Defaulting to \\\\.\\mailslot" ) ;
   strcpy ( gzAUTOFIFO, "\\\\.\\mailslot" ) ;
#else
    gHyp_util_logInfo ( "Defaulting to %s/local/fifo",gzHOME ) ;
    sprintf ( gzAUTOFIFO, "%s/local/fifo" , gzHOME ) ;
#endif
#endif
  }

  /* Determine the RUN area */
  gzAUTORUN[0] = '\0' ;
  if ( ( pVar = getenv ( "AUTORUN" ) ) != NULL ) {
    strcpy ( gzAUTORUN, pVar ) ;
    gHyp_util_lowerCase ( gzAUTORUN, strlen ( gzAUTORUN ) ) ; 
  }
  else {
    gHyp_util_logWarning ( "Environment variable AUTORUN was not defined" ) ;
#ifdef AS_VMS
    gHyp_util_logInfo ( "Defaulting to current directory" ) ;
#else
#ifdef AS_WINDOWS
    gHyp_util_logInfo ( "Defaulting to %s\\local\\run", gzHOME ) ;
    sprintf ( gzAUTORUN, "%s\\local\\run", gzHOME ) ;
#else
    gHyp_util_logInfo ( "Defaulting to %s/local/run", gzHOME ) ;
    sprintf ( gzAUTORUN, "%s/local/run", gzHOME ) ;
#endif
#endif
  }  

  /* Determine the BIN area */
  gzAUTOBIN[0] = '\0' ;
  if ( ( pVar = getenv ( "AUTOBIN" ) ) != NULL ) {
    strcpy ( gzAUTOBIN, pVar ) ;
    gHyp_util_lowerCase ( gzAUTOBIN, strlen ( gzAUTOBIN ) ) ; 
  }
  else {
    gHyp_util_logWarning ( "Environment variable AUTOBIN was not defined" ) ;
#ifdef AS_VMS
    gHyp_util_logInfo ( "Defaulting to current directory" ) ;
#else
#ifdef AS_WINDOWS
    gHyp_util_logInfo ( "Defaulting to %s\\local\\bin", gzHOME ) ;
    sprintf ( gzAUTOBIN, "%s\\local\\bin", gzHOME ) ;
#else
    gHyp_util_logInfo ( "Defaulting to %s/local/bin", gzHOME ) ;
    sprintf ( gzAUTOBIN, "%s/local/bin", gzHOME ) ;
#endif
#endif
  }  



  /* Determine the LOG area */
  gzAUTOLOG[0] = '\0' ;
  if ( ( pVar = getenv ( "AUTOLOG" ) ) != NULL ) {
    strcpy ( gzAUTOLOG, pVar ) ;
    gHyp_util_lowerCase ( gzAUTOLOG, strlen ( gzAUTOLOG ) ) ; 
  }
  else {
    gHyp_util_logWarning ( "Environment variable AUTOLOG was not defined" ) ;
#ifdef AS_VMS
    gHyp_util_logInfo ( "Defaulting to current directory" ) ;
#else
#ifdef AS_WINDOWS
    gHyp_util_logInfo ( "Defaulting to %s\\local\\log", gzHOME ) ;
    sprintf ( gzAUTOLOG, "%s\\local\\log", gzHOME ) ;
#else
    gHyp_util_logInfo ( "Defaulting to %s/local/log", gzHOME ) ;
    sprintf ( gzAUTOLOG, "%s/local/log", gzHOME ) ;
#endif
#endif
  }

  /* Determine the SPOOL area */
  gzAUTOSPOOL[0] = '\0' ;
  if ( ( pVar = getenv ( "AUTOSPOOL" ) ) != NULL ) {
    strcpy ( gzAUTOSPOOL, pVar ) ;
    gHyp_util_lowerCase ( gzAUTOSPOOL, strlen ( gzAUTOSPOOL ) ) ; 
  }
  else {
    gHyp_util_logWarning ( "Environment variable AUTOSPOOL was not defined" ) ;
#ifdef AS_VMS
    gHyp_util_logInfo ( "Defaulting to current directory" ) ;
#else
#ifdef AS_WINDOWS
    gHyp_util_logInfo ( "Defaulting to %s\\local\\spool", gzHOME ) ;
    sprintf ( gzAUTOSPOOL, "%s\\local\\spool", gzHOME ) ;
#else
    gHyp_util_logInfo ( "Defaulting to %s/local/spool", gzHOME ) ;
    sprintf ( gzAUTOSPOOL, "%s/local/spool", gzHOME ) ;
#endif
#endif
  }
  return TRUE ;
}

SOCKET gHyp_concept_serviceHandle ( sConcept *pConcept ) 
{
  return pConcept->link.listenTCP ;
}

sSSL* gHyp_concept_getSSL ( sConcept *pConcept ) 
{
  return pConcept->link.pSSL ;
}

void gHyp_concept_setSSL ( sConcept *pConcept, sSSL *pSSL ) 
{
  pConcept->link.pSSL = pSSL ;
}

SOCKET gHyp_concept_createService( sConcept *pConcept, char *pService ) 
{

  if ( pService && *pService ) {

    /* Create/bind INET socket for connection requests from remote networks */

    if ( pConcept->link.listenTCP != INVALID_SOCKET ) {
      gHyp_sock_close ( pConcept->link.listenTCP, SOCKET_TCP, pService, gzLocalHost ) ;
    }

    if ( (pConcept->link.listenTCP = gHyp_tcp_make (pService,gzLocalHost,TRUE)) == INVALID_SOCKET ) {
      gHyp_util_logError ( "Failed to create listen port for service '%s' at '%s'", pService, gzLocalHost ) ;

      return INVALID_SOCKET ;
    }
    else {

      /* Assign the port used (see gHyp_tcp_make) to the service port */
      giARservicePort = giServicePort ;

      /* Request a TCP loopback interface */
      giJmpOverride = TRUE ;
      if ( (giListenLoopback_W = gHyp_tcp_request ( gzLocalHost, giARservicePort )) == 
	   INVALID_SOCKET )
         gHyp_util_logError ( "Failed to create loopback listen handle to '%s' on port %d", 
                                      gzLocalHost, giARservicePort ) ;
      else
        gHyp_util_logInfo ( "Created loopback listen handle to '%s' on port %d", 
                                      gzLocalHost, giARservicePort ) ;
      giJmpOverride = FALSE ;
    }
    return pConcept->link.listenTCP ;
  }
  else
    return INVALID_SOCKET ;
}

sLOGICAL gHyp_concept_init ( sConcept *pConcept, 
                             char *target,
                             char *service,
                             unsigned debugFlags,
                             int maxExprSize )
{
  sInstance
    *pAI ;

  char
    instance[INSTANCE_SIZE+1],
    concept[OBJECT_SIZE+1],
    parent[OBJECT_SIZE+1],
    root[OBJECT_SIZE+1],
    host[HOST_SIZE+1],
    path[MAX_PATH_SIZE+1] ;

  /* Initialize global variables */
  guDebugFlags = debugFlags ;
  guErrorCount = 0 ;
  giProgramCount = 0 ;
  giCondition = COND_SILENT ;
  giLineCount = 0 ;
  gsCurTime = time ( NULL ) ;
  giJmpLevel = -1 ;
  giJmpEnabled = FALSE ;
  giJmpOverride = FALSE ;
  gpsConcept = pConcept ;
  gpsAImsg = gHyp_aimsg_new ( ) ; 
  giMaxExprSize = maxExprSize ;
  giMaxStackDepth = giMaxExprSize * 2 ;
  gpsTempData = NULL ;
  gpsTempData2 = NULL ;

  pConcept->link.hasRegistered = FALSE ;

  if ( guRunFlags & RUN_QUIET )
    guTimeStamp = FALSE ;
  else
    guTimeStamp = TRUE ;

   /* Set timestamp */
  tzset();
  gsCurTime = time ( NULL ) ;

  /* Open logs - if defined with -l option. */
  if ( !gHyp_util_openLog() ) return FALSE ;

  /* Initialize the client and network socket tables */
  if ( !gHyp_sock_init () ) return FALSE ;

  /* For dynamic associated array of alias names and their ip addresses */ 
  gHyp_tcp_createAliasTable() ;

  /* Get current host name */
  gHyp_util_gethostname ( gzLocalHost ) ;
  gHyp_util_lowerCase ( gzLocalHost, strlen ( gzLocalHost ) ) ;

  /* Resolve it (get ip address) - and put it in the Alias table. */
  gHyp_tcp_resolveHost ( gzLocalHost, gzLocalAddr ) ;

  /* Remove anything past the first "." 
  gzLocalHost = strtok ( gzLocalHost, "." ) ;
  */

  /* HyperScript Initialization */
  if ( !lHyp_concept_initVariables () ) return FALSE ;

  /* Break up the target */
  gHyp_util_lowerCase ( target, strlen ( target ) ) ;
  gHyp_util_breakTarget ( target, instance, concept, parent, root, host ) ;
  
  /* Initialze current instance name */
  strcpy ( gzInstance, instance ) ;

  /* Save concept name and parent name - globally */
  strcpy ( gzConcept, concept ) ;

  /* Check if we are really the root */
  if ( strcmp ( gzConcept, gzAUTOROUTER ) == 0 ) guRunFlags |= RUN_ROOT ;

  /* Announce we're starting */
  gHyp_util_output ( "================================================================\n" ) ;
  gHyp_util_logInfo ( "HYPERSCRIPT Version %s", VERSION_HYPERSCRIPT ) ;
  /*gHyp_util_logInfo ( "Copyright: (c) 1994 Ab Initio Software" ) ;*/
  gHyp_util_logInfo ( "Copyright:(c) 1994, 2002  Abinition, Inc" ) ;

  /* HyperScript Initialization */
  if ( !lHyp_concept_initEnvironment ( concept ) ) return FALSE ;

  if ( !gHyp_concept_setParent ( pConcept, parent, root ) ) return FALSE ;

  /* Load the keywords table */
  gHyp_load_new() ;

  pConcept->exec.returnToStdIn = TRUE ;
  /*if ( guRunFlags & RUN_COMPILEONLY ) return ;*/

  /* Create the ring of instances */
  pConcept->exec.pInstances = gHyp_data_new ( "Instances" ) ;
  gHyp_data_setHashed ( pConcept->exec.pInstances, TRUE ) ;

  /* If the -t instance#concept specifies a unique instance, then create that */
  if ( strcmp ( instance, concept ) != 0 )
    gHyp_concept_instantiate (  pConcept, 
				NULL,  /* Ok if first argument is false */
				instance,
				gzRoot,
				FALSE, FALSE, TRUE ) ;

  /* Create the parent instance which will be included in the ring of instances.
   * The root.
   */

  pAI = gHyp_concept_instantiate (  pConcept, 
				    NULL,  /* Ok if first argument is false */
				    concept,
				    gzParent,
				    FALSE, FALSE, TRUE ) ;

  /******************
  pFrame = gHyp_instance_frame ( pAI ) ;
  pData = gHyp_frame_createRootVariable ( pFrame, "AUTOROUTER" ) ;
  pValue = gHyp_data_new(NULL);
  gHyp_data_setStr ( pValue, gzAUTOROUTER ) ;
  gHyp_data_append ( pData, pValue ) ;
  pData = gHyp_frame_createRootVariable ( pFrame, "AUTOFIFO" ) ;
  pValue = gHyp_data_new(NULL);
  gHyp_data_setStr ( pValue, gzAUTOFIFO ) ;
  gHyp_data_append ( pData, pValue ) ;
  pData = gHyp_frame_createRootVariable ( pFrame, "HOME" ) ;
  pValue = gHyp_data_new(NULL);
  gHyp_data_setStr ( pValue, gzHOME ) ;
  gHyp_data_append ( pData, pValue ) ;
  pData = gHyp_frame_createRootVariable ( pFrame, "AUTOLOG" ) ;
  pValue = gHyp_data_new(NULL);
  gHyp_data_setStr ( pValue, gzAUTOLOG ) ;
  gHyp_data_append ( pData, pValue ) ;
  pData = gHyp_frame_createRootVariable ( pFrame, "AUTOSPOOL" ) ;
  pValue = gHyp_data_new(NULL);
  gHyp_data_setStr ( pValue, gzAUTOSPOOL ) ;
  gHyp_data_append ( pData, pValue ) ;
  ************************/


  /* Was an instance specified as well? */
  /* Create the token hash table */
  pConcept->exec.tokens = gHyp_hash_new ( TOKEN_HASH_TABLE_SIZE ) ;

  /* Initialize the links area
   *
   *  outbox 
   *    1. If the root, this is undefined (INVALID_HANDLE_VALUE)
   *    2. If not the root, this is the well-known write-only parent fifo
   *
   *  inbox
   *     1. If the root, this is the ROOT fifo /local/fifo/router (mbx_router)
   *     2. If a client, this is the target fifo created by the parent.
   *
   *  pClients
   *     This is the list of write-only child fifo's, but they are opened
   *     read-write so we can read from them to check for stuck-fifos.
   *
   *  listenTCP
   *     This is a listen port if entry exists in /etc/services
   *
   *  pHosts
   *     These are the connections created from the listenTCP port
   *
   *  fd[]
   *      These are the application links
   */

  pConcept->link.outbox = INVALID_HANDLE_VALUE ;
  memset ( &pConcept->link.outboxOverlapped, 0, sizeof (OVERLAPPED) ) ;

  pConcept->link.listenTCP = INVALID_SOCKET ;
  pConcept->link.pSSL = NULL ;
  pConcept->link.inbox = INVALID_HANDLE_VALUE ;
  pConcept->link.pClients = gHyp_data_new ( "Clients" ) ;
  pConcept->link.pHosts = gHyp_data_new ( "Hosts" ) ;
  pConcept->link.pSockets = gHyp_data_new ( "Sockets" ) ;

  gHyp_data_setHashed ( pConcept->link.pClients, TRUE ) ;
  gHyp_data_setHashed ( pConcept->link.pHosts, TRUE ) ;
  gHyp_data_setHashed ( pConcept->link.pSockets, TRUE ) ;
  
  if ( (guRunFlags & RUN_COMPILEONLY) ) return TRUE ;

  giARservicePort = -1 ;
  giListenLoopback_R = INVALID_SOCKET ;
  giListenLoopback_W = INVALID_SOCKET ;
  giInboxLoopback = INVALID_HANDLE_VALUE ;
  giLoopback = INVALID_SOCKET ;
  giLoopbackType = SOCKET_NONE ; 

  giResolveFlag1 = FALSE ;
  giResolveFlag2 = FALSE ;
  strcpy ( gzResolveAddr, "" ) ;

  gHyp_concept_createService ( pConcept, service ) ;

  /* Open the parent (outbox) (if we are ROOT, there is no outbox)  *
  if ( !(guRunFlags & RUN_ROOT) ) gHyp_concept_openWriter ( pConcept ) ;
*/
#ifndef AS_VMS
  /* Open the inbox  */
  /*
  if ( !(guRunFlags & RUN_ROOT) ) if ( !gHyp_concept_openReader ( pConcept ) ) return FALSE ;
  */
#endif

#ifdef AS_DMBX
  if ( (guRunFlags & RUN_ROOT) ) mbxReconnect ( DM_FALSE ) ;
#endif

  /* If the read buffer should be initialized, then do it now 
   * Check to see if the first messages should come from a file
   */
#ifdef AS_VMS
  sprintf ( path, "%s%s.dat", gzAUTOLOG, concept ) ;
#else
  sprintf ( path, "%s/%s.dat", gzAUTOLOG, concept ) ;
#endif
  gHyp_sock_loadBuffer ( path ) ;

  /* Start up a logger HyperScript */
  /*
  gpsLogger = gHyp_sock_findClient ( AUTO_LOGGER, AUTO_LOGGER ) ;
  if ( !gpsLogger )
  gpsLogger = gHyp_sock_findHYP ( AUTO_LOGGER, AUTO_LOGGER ) ;
  if ( !gpsLogger )
    gpsLogger = gHyp_sock_findCOM ( AUTO_LOGGER, AUTO_LOGGER ) ;
  if ( !gpsLogger )
    gpsLogger = gHyp_sock_findEXE ( AUTO_LOGGER, AUTO_LOGGER ) ;
  if ( gpsLogger ) 
    gHyp_util_logInfo ( "Logging routed to %s process", AUTO_LOGGER );
  else
    gHyp_util_logInfo ( "No %s process found for logging.", AUTO_LOGGER ) ;
  */
  gHyp_util_output ( "================================================================\n" ) ;
  gHyp_util_logInfo ( "Initialized new HyperScript concept '%s'", concept ) ;
  
  return TRUE ;
}

sLOGICAL gHyp_concept_setParent ( sConcept *pConcept, char *parent, char *root )
{
  char  
    *pToken,
    path[MAX_PATH_SIZE+1],
    tmp[MAX_PATH_SIZE+1] ;

  /* Parent and Root are used for routing. */
  strcpy ( gzParent, parent ) ;
  strcpy ( gzRoot, root ) ;

  /* Determine the parent and child paths 
   *
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
  if ( (guRunFlags & RUN_ROOT) ) {
      
    /* Root hyperscript */
#ifndef AS_VMS
#ifdef AS_WINDOWS
    /* Windows mailslots */
    sprintf ( gzInboxPath, "%s\\%s", gzAUTOFIFO, gzAUTOROUTER ) ;
    sprintf ( gzConceptPath, "%s\\_%s", gzAUTOFIFO, gzAUTOROUTER ) ;
    strcpy  ( gzOutboxPath, "" ) ;
#else
    /* Unix fifos */
    sprintf ( gzInboxPath, "%s/%s", gzAUTOFIFO, gzAUTOROUTER ) ;
    sprintf ( gzConceptPath, "%s/.%s", gzAUTOFIFO, gzAUTOROUTER ) ;
    strcpy  ( gzOutboxPath, "" ) ;
#endif
#else
    /* VMS mail boxes */
    sprintf ( gzInboxPath, "%s_%s", gzAUTOFIFO, gzAUTOROUTER ) ;
    sprintf ( gzConceptPath, "%s_%s", gzAUTOFIFO, gzAUTOROUTER ) ;
    strcpy  ( gzOutboxPath, "" ) ;
#endif
  }
  else {
    
    /* Not root hyperScript.  Initialize paths */
    sprintf ( gzInboxPath, "%s", gzAUTOFIFO  ) ;
    sprintf ( gzConceptPath, "%s", gzAUTOFIFO ) ;      
    sprintf ( gzOutboxPath, "%s", gzAUTOFIFO  ) ;
    sprintf ( tmp, "%s/%s", gzParent, gzAUTOROUTER ) ;

    while ( tmp[0] ) {
      pToken = strrchr ( tmp, '/' ) ;
      if ( !pToken ) break ;
      *pToken++ = '\0' ;
      if ( *pToken ) {

#ifndef AS_VMS
#ifdef AS_WINDOWS
        sprintf ( path, "\\_%s", pToken ) ; 
#else
        sprintf ( path, "/.%s", pToken ) ; 
#endif
#else
        sprintf ( path, "_%s", pToken ) ;
#endif
        strcat  ( gzInboxPath, path ) ;
        gHyp_sock_mkdir ( gzInboxPath ) ; 
        strcat  ( gzConceptPath, path ) ;

        /* Check if last one - no "." dot */
#ifdef AS_WINDOWS
        if ( pToken == (tmp+1) ) sprintf ( path, "\\%s", pToken ) ; 
#else

#ifdef AS_VMS

        if ( pToken == (tmp+1) ) sprintf ( path, "_%s", pToken ) ; 

#else
        if ( pToken == (tmp+1) ) sprintf ( path, "/%s", pToken ) ; 
#endif
#endif

        strcat  ( gzOutboxPath, path ) ; 
      }
    }
#ifndef AS_VMS
#ifdef AS_WINDOWS
    sprintf ( path, "\\%s", gzConcept ) ; 
    strcat  ( gzInboxPath, path ) ;
    sprintf ( path, "\\_%s", gzConcept ) ; 
    strcat  ( gzConceptPath, path ) ;
#else
    sprintf ( path, "/%s", gzConcept ) ; 
    strcat  ( gzInboxPath, path ) ;
    sprintf ( path, "/.%s", gzConcept ) ; 
    strcat  ( gzConceptPath, path ) ;
#endif
#else
    sprintf ( path, "_%s", gzConcept ) ; 
    strcat  ( gzInboxPath, path ) ;
    sprintf ( path, "_%s", gzConcept ) ; 
    strcat  ( gzConceptPath, path ) ;
#endif
  }
  
#ifdef AS_VMS
  gHyp_util_upperCase ( gzInboxPath, strlen ( gzInboxPath ) ) ;
  gHyp_util_upperCase ( gzOutboxPath, strlen ( gzOutboxPath ) ) ;
  gHyp_util_upperCase ( gzConceptPath, strlen ( gzConceptPath ) ) ;
#endif

   /*
   gHyp_util_debug ( "FIFO INBOX is %s", gzInboxPath ) ;
   gHyp_util_debug ( "CONCEPT PATH is %s", gzConceptPath ) ;
   gHyp_util_debug ( "FIFO OUTBOX is %s", gzOutboxPath ) ; 
   gHyp_util_debug ( "gzParent is %s", gzParent ) ;
   gHyp_util_debug ( "gzConcept is %s", gzConcept ) ;
   gHyp_util_debug ( "gzROOT is %s",gzRoot);
   */

  return TRUE ;
}

sInstance* gHyp_concept_instantiate ( sConcept *pConcept, 
                                      sInstance *pAImain,
                                      char *newInstance,
				      char *root,
                                      sLOGICAL swapFrames,
				      sLOGICAL swapData,
				      sLOGICAL reset)
{
  sInstance
    *pAI ;

  char
    instance[INSTANCE_SIZE+1],
    targetPath[TARGET_SIZE+1],
    targetId[TARGET_SIZE+1] ;

  sData
    *pData ;

  if ( strcmp ( newInstance, gzConcept ) == 0 ) {
    sprintf ( targetPath, "%s%s", gzConcept, root ) ;
    strcpy ( targetId, gzConcept ) ;
    strcpy ( instance, gzConcept ) ;
  }
  else {
    sprintf ( targetPath, "%s#%s%s", newInstance, gzConcept, root ) ;
    sprintf ( targetId, "%s#%s", newInstance, gzConcept ) ;
    strcpy ( instance, newInstance ) ;
  }
  pAI = gHyp_instance_new () ;
  gHyp_instance_init ( pAI, pConcept, targetId, targetPath ) ;

  pData = gHyp_data_new ( instance ) ;
  gHyp_data_setObject ( pData,
                        pAI,
                        DATA_OBJECT_INSTANCE,
                        (void (*)(void*)) gHyp_instance_delete ) ;
  gHyp_data_insert ( pConcept->exec.pInstances, pData ) ;

  if ( swapFrames )
    /* Swap the frames
     * Done for "instantiate()", because we want to have the
     * running frame, which always is the concept instance.
     */
    gHyp_instance_swapFrames ( pAI, pAImain ) ;
  
  if ( swapData )
    /* Swap the data. 
     * Done for "instantiate()", because our new instance must
     * swap back the concept instance data area.
     * Also done for "renameto()", becuase our renamed new
     * concept instance.wants the data area of the old instance.
     */
    gHyp_instance_swapData ( pAI, pAImain ) ;
  

  if ( reset )
    /* Reset the new instance */
    gHyp_instance_reset ( pAI, STATE_IDLE, TRUE ) ;

  return pAI ;
}

void gHyp_concept_delete ( sConcept * pConcept )
{   
  int
    i ;

  sHash
    *pHash,
    *pNext ;

  /* Shutdown the sockets */
  /*gHyp_util_logInfo("shutting down sockets");*/
  gHyp_sock_shutdown ( pConcept->link.listenTCP,
                       pConcept->link.pClients,
                       pConcept->link.pHosts,
                       pConcept->link.pSockets,
                       pConcept ) ;
        
#ifdef AS_SSL
  if ( pConcept->link.pSSL ) gHyp_sock_deleteSSL( pConcept->link.pSSL ) ;
#endif

  /* Delete all the instances */
  /*gHyp_util_logInfo("deleting instances");*/
  gHyp_data_delete ( pConcept->exec.pInstances ) ;

  /*gHyp_util_logInfo("aimsg") ;*/
  /* Remove structures */
  if ( gpsAImsg ) gHyp_aimsg_delete ( gpsAImsg ) ;  
 
  /*gHyp_util_logInfo("tokens");*/
  /* Delete all the tokens from the hash table */
  for ( i=0; i<TOKEN_HASH_TABLE_SIZE; i++ )
    for ( pHash = (*pConcept->exec.tokens)[i]; 
          pHash; 
          pHash=pNext ) {
      pNext = gHyp_hash_next ( pHash ) ;
      gHyp_hash_free ( pHash ) ;
    }

  /* Delete the hash table itself */
  gHyp_hash_delete ( pConcept->exec.tokens ) ;

  /*gHyp_util_logInfo("keywords");*/
  /* Delete the keywords table */
  gHyp_load_delete() ;

  /*gHyp_util_logInfo("concept");*/
  /* Free the concept */
  ReleaseMemory ( pConcept ) ;

  /* Release any data that would otherwise be leaked */
  if ( gpsTempData ) { 
    gHyp_data_delete ( gpsTempData ) ; 
    gpsTempData = NULL ; 
  }
  if ( gpsTempData2 ) { 
    gHyp_data_delete ( gpsTempData2 ) ; 
    gpsTempData2 = NULL ; 
  }

  return ;
}

void gHyp_concept_quit ( sConcept *pConcept )
{
  giJmpEnabled = FALSE ;
  gHyp_util_logInfo ( "Terminating HyperScript" );
  gHyp_concept_disconnect ( pConcept ) ;
  gHyp_concept_delete ( pConcept ) ;
  /*unlink ( gzPIDfileSpec ) ;*/

#ifdef AS_MEMTRACK
  TrackMemory(MEMTRK_REPORT,
              0, NULL, 0, NULL, 0);

  TrackMemory(MEMTRK_DESTROY,
              0, NULL, 0, NULL, 0);
#endif

  /* Close files */
  if ( gzLogName[0] && gsLog && gsLog != stdout ) fclose ( gsLog ) ;

  return ;
}

void gHyp_concept_setReturnToStdIn ( sConcept *pConcept, sLOGICAL returnToStdIn  )
{
  pConcept->exec.returnToStdIn = returnToStdIn ;
  /*
  if ( returnToStdIn )
    gHyp_util_logInfo("Yes return to get stdin" ) ;
  else
    gHyp_util_logInfo("No  return to get stdin" ) ;
  */
  return ;
}

sLOGICAL gHyp_concept_returnToStdIn ( sConcept *pConcept  )
{
  return pConcept->exec.returnToStdIn ;
}

sInstance *gHyp_concept_getNamedInstance ( sConcept *pConcept, char *instance )
{
  sData
    *pData ;

  if ( (pData = gHyp_data_getChildByName ( pConcept->exec.pInstances, instance )) )
    return (sInstance*) gHyp_data_getObject ( pData ) ;
  else
    return NULL ;
}

sData *gHyp_concept_getInstances ( sConcept *pConcept )
{
  return pConcept->exec.pInstances ;
}

int gHyp_concept_setAlarms ( sConcept *pConcept ) 
{
  sLOGICAL 
    f ;

  sData
    *pFirst,
    *pData ;

  sInstance
    *pAImain,
    *pAI ;

  int
    eventTime,
    timeout ;
  
  pAImain = gHyp_concept_getConceptInstance ( pConcept ) ;
  gsCurTime = time ( NULL ) ;
  timeout = HEARTBEAT_INTERVAL ;
  for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pConcept->exec.pInstances ) ;
        (f && pData) || pData != pFirst;
        f=FALSE, pData = gHyp_data_getNext ( pData ) ) {

    pAI = (sInstance*) gHyp_data_getObject ( pData ) ;

    if ( gHyp_instance_getState ( pAI ) == STATE_PARSE &&
         !gHyp_instance_isEND ( pAI ) ) {
     if ( guDebugFlags & DEBUG_DIAGNOSTICS )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
                             "Program code for %s to execute",
                             gHyp_instance_getTargetId(pAI) ); 
      timeout = 0 ;
    }
                      
    /*gHyp_util_debug ( "NEXT EVENT for %s",gHyp_instance_getTargetId(pAI) ); */
    eventTime = gHyp_instance_nextEvent(pAI) ;

    /*if ( eventTime == gsCurTime ) gHyp_instance_signalAlarm ( pAI ) ;*/
    if ( eventTime <= gsCurTime ) gHyp_instance_signalAlarm ( pAI ) ;

    if ( timeout != 0 ) timeout = MIN ( timeout, eventTime-gsCurTime ) ;
    
    if ( gHyp_instance_isSignal ( pAI ) ) {
      /*
      if ( guDebugFlags & DEBUG_DIAGNOSTICS )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
                             "Signal for %s",
                             gHyp_instance_getTargetId(pAI) ) ; 
      */
      timeout = 0 ;
    }

    if ( pAI == pAImain &&
         gHyp_instance_getState ( pAImain ) != STATE_QUERY &&
         gHyp_instance_getState ( pAImain ) != STATE_SLEEP &&
         gHyp_concept_returnToStdIn ( pConcept ) ) {
      /*
      if ( guDebugFlags & DEBUG_DIAGNOSTICS )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
                             "stdin for %s, timeout set to 0",
                             gHyp_instance_getTargetId(pAI) ) ;
      */
      timeout = 0 ; 
    }
    
  }
  return timeout ;
}

sInstance *gHyp_concept_getConceptInstance ( sConcept *pConcept )
{
  return gHyp_concept_getNamedInstance ( pConcept, gzConcept ) ;
}

sInstance *gHyp_concept_nextInstance ( sConcept *pConcept )
{
  sData
    *pAIdata = gHyp_data_getFirst (pConcept->exec.pInstances)  ;

  sInstance
    *pAI = (sInstance*) gHyp_data_getObject ( pAIdata ) ;

  gHyp_data_next ( pConcept->exec.pInstances ) ;

  strcpy ( gzInstance, gHyp_data_getLabel ( pAIdata ) ) ;
  /*
    
  gHyp_util_logInfo("Instance is now %s, state=%d",gHyp_instance_getTargetId(pAI),
                    gHyp_instance_getState(pAI) ) ;
  */
  return pAI ;
}

sData *gHyp_concept_nextInstanceData ( sConcept *pConcept )
{
  sData
    *pAIdata = gHyp_data_getFirst (pConcept->exec.pInstances)  ;

  gHyp_data_next ( pConcept->exec.pInstances ) ;

  strcpy ( gzInstance, gHyp_data_getLabel ( pAIdata ) ) ;
  /*
  gHyp_util_logInfo("Instance is now %s, state=%d",
    gHyp_instance_getTargetId((sInstance*) gHyp_data_getObject (pAIdata) ),
    gHyp_instance_getState((sInstance*) gHyp_data_getObject (pAIdata)) ) ;
  */
  return pAIdata ;
}

sLOGICAL gHyp_concept_openWriter ( sConcept *pConcept )
{
  /* If we are the ROOT, then we have no outbox to connect to */
  if ( (guRunFlags & RUN_ROOT) ) return FALSE ;

  /* Open the write socket if not already open */
  if ( pConcept->link.outbox == INVALID_HANDLE_VALUE ) {
    
    /* Open the well-known and pre-existing outbox channel for "write-only".
     * The parent and creater of the fifo opens it read-write
     */
    pConcept->link.outbox = gHyp_sock_fifo ( gzOutboxPath, 
                                             FALSE, /* Open existing, ie: do no create */
                                             FALSE, /* Not readonly */
                                             TRUE,  /* Open writeonly */
					     FALSE ) ; /* N/A */

    /*gHyp_util_debug("Opening writer %d",pConcept->link.outbox);*/

    if ( pConcept->link.outbox != INVALID_HANDLE_VALUE )
      gHyp_util_logInfo ( "Connected to output message channel '%s' (%d)",
                          gzOutboxPath,
                           pConcept->link.outbox ) ;
  }

  return (sLOGICAL) (pConcept->link.outbox != INVALID_HANDLE_VALUE) ;
}

sLOGICAL gHyp_concept_openReader ( sConcept *pConcept )
{

  sLOGICAL
    tryAgain = ! (pConcept->link.hasRegistered)  ;

  char
    target[OBJECT_SIZE+1] ;

#ifdef AS_VMS
  /* We must CONNECT to the parent, it opens our inbox. */
  if ( !gHyp_concept_connect ( pConcept ) ) return FALSE ;
#endif

  /* Open the read socket if not already open */
  if ( pConcept->link.inbox == INVALID_HANDLE_VALUE ) {

    while ( 1 ) {

      pConcept->link.inbox = gHyp_sock_fifo ( gzInboxPath, 
                                            TRUE,  /* Create */
                                            TRUE, /*  Read-only! */
                                            FALSE, /* Not write-only */
					    TRUE ) ;/* Fail if already exists */

      if ( pConcept->link.inbox != INVALID_HANDLE_VALUE ) {
        gHyp_util_logInfo ( "Connected to input message channel '%s' (%d)", 
                            gzInboxPath,
                            pConcept->link.inbox ) ;

        /* Request the loopback interface */
        giInboxLoopback = INVALID_HANDLE_VALUE ;
        
        giInboxLoopback = gHyp_sock_fifo ( gzInboxPath, 
					   FALSE, /* Do not create */
					   FALSE,  /* Not read */
					   TRUE,  /* Write-only */
					   FALSE ) ;/* N/A */
        if ( giInboxLoopback == INVALID_HANDLE_VALUE )
          return gHyp_util_logError ( "Failed to create loopback fifo handle '%s'", 
                                      gzInboxPath ) ;
        
        
        gHyp_util_logInfo ( "Created loopback fifo handle (%d) at '%s'", 
                            giInboxLoopback, gzInboxPath ) ;
        /* Set the loopback to the inbox channel. If a TCP listen socket is
         * available, then giLoopback will be set to it instead.
         */
        if ( giLoopback == INVALID_SOCKET ) {
	  giLoopback = (SOCKET) giInboxLoopback ;
	  giLoopbackType = SOCKET_FIFO ;
	}

	break ;
      }
      else {
	if ( tryAgain ) {
	  sprintf ( target, "%s_%s/%s", gzConcept, gHyp_util_random8(), gzConcept ) ;
	  gHyp_util_logWarning ("Attempting to reconnect as %s", target ) ;
	  if ( !gHyp_concept_renameto ( pConcept, target ) ) return FALSE ;
	  tryAgain = FALSE ;
	}
	else
	  return FALSE ;
      }
    } /* End while */

  }
  return (sLOGICAL) (pConcept->link.inbox != INVALID_HANDLE_VALUE ) ;
}

void gHyp_concept_closeWriter ( sConcept *pConcept )
{
  if ( pConcept->link.outbox != INVALID_HANDLE_VALUE ) {

    gHyp_sock_close ( (SOCKET) pConcept->link.outbox,
                      SOCKET_FIFO,
                      gzParent,
                      "" ) ;  /* So that unlink won't delete it */

    pConcept->link.outbox = INVALID_HANDLE_VALUE ;
  }
  return ;
}  

void gHyp_concept_closeReader ( sConcept *pConcept )
{
  if ( pConcept->link.inbox != INVALID_HANDLE_VALUE ) {

    gHyp_sock_close ( (SOCKET) pConcept->link.inbox,
                      SOCKET_FIFO,
                      gzConcept,
                      gzInboxPath ) ;
    pConcept->link.inbox = INVALID_HANDLE_VALUE ;
  }

  if ( giInboxLoopback != INVALID_HANDLE_VALUE ) {
    gHyp_sock_close ( (SOCKET) giInboxLoopback,
                      SOCKET_FIFO,
                      gzConcept,
                      gzInboxPath ) ;
    giInboxLoopback = INVALID_HANDLE_VALUE ;
  }
 
  return ;
}  

sLOGICAL gHyp_concept_connect ( sConcept *pConcept )
{

  int
    i ;
  
  char
    target[OBJECT_SIZE+1],
    sender[OBJECT_SIZE+1],
    message[MIN_MESSAGE_SIZE+1] ;
    
  /*gHyp_util_debug("Connected reader %d",pConcept->link.hasRegistered);*/

  if ( pConcept->link.hasRegistered ) return TRUE ;

#ifndef AS_VMS

  /* Try and open the inbox. */
  if ( !gHyp_concept_openReader ( pConcept ) ) return FALSE ;

#endif


  /* If we are the root, then we have no outbox */
  if ( (guRunFlags & RUN_ROOT) ) return TRUE ;

  /* Open the write socket if not already open */
  if ( !gHyp_concept_openWriter ( pConcept ) ) return FALSE ;

  /* Send message to AutoRouter to have it register and create the reader */
  gHyp_util_logInfo ( "...sending a connect message to %s at %s",
                      gzParent, gzOutboxPath );


  if ( gzRoot[0] )   
    sprintf ( target, "%s", gzRoot+1 ) ;
  else
    sprintf ( target, "%s", gzAUTOROUTER ) ;

  sprintf ( sender, "%s%s", gzConcept, gzParent ) ;

  sprintf ( message,
            "|%s|event|CONNECT|%s|%s|%s|||", 
            target,
            sender,
            gHyp_util_random8(),
            gHyp_util_timeStamp ( gsCurTime ) ) ;
    
  /* Poll for the reader to be created. */
  gHyp_sock_logger ( gzOutboxPath, message, NULL ) ;
  if ( gHyp_sock_write ( (SOCKET) pConcept->link.outbox, 
                         message, 
                         strlen(message),
                         SOCKET_FIFO,
                         (OVERLAPPED*) &pConcept->link.outboxOverlapped,
			 NULL ) ) {

    for ( i=0; i < MAX_CONNECT_TRIES && pConcept->link.inbox == INVALID_HANDLE_VALUE; i++ ) {
    
      if ( i>0 ) {
        gHyp_util_logInfo ("...waiting for input message channel '%s'",
				gzInboxPath);
#ifdef AS_WINDOWS
        Sleep ( RETRY_INTERVAL / 1000 ) ;
#else
        sleep ( RETRY_INTERVAL ) ;
#endif
      }
      pConcept->link.inbox = gHyp_sock_fifo ( gzInboxPath, 
                                              FALSE,  /* Open existing. */
                                              TRUE,  /* Read-only */
                                              FALSE, /* No write */
					      FALSE ) ; /* N/A */
    }

    if ( pConcept->link.inbox != INVALID_HANDLE_VALUE ) {
      gHyp_util_logInfo ( "Connected to input message channel '%s' (%d)",
                          gzInboxPath,
                          pConcept->link.inbox ) ;
      pConcept->link.hasRegistered = TRUE ;
    }
    else
      gHyp_util_logWarning ( "Timed-out out waiting for input channel '%s'", gzInboxPath );
  }
  else {
    gHyp_util_logWarning ( "Failed to send connect message to outbox" ) ;
    gHyp_util_logInfo ( "Closing connection to %s",gzParent ) ;
    gHyp_concept_closeReader ( pConcept ) ;
    gHyp_concept_closeWriter ( pConcept ) ;
  }

  return (sLOGICAL) (pConcept->link.inbox != INVALID_HANDLE_VALUE) ;
}

sLOGICAL gHyp_concept_disconnect ( sConcept *pConcept )
{
  char
   target[OBJECT_SIZE+1],
    sender[OBJECT_SIZE+1],
    message[MIN_MESSAGE_SIZE+1] ;
  
  /*if ( !pConcept->link.hasRegistered ) return TRUE ;*/

  if ( pConcept->link.inbox != INVALID_HANDLE_VALUE && 
       pConcept->link.outbox != INVALID_HANDLE_VALUE ) {

    gHyp_util_logInfo ( "...sending a DISCONNECT message to outbox (%d)",pConcept->link.outbox );

    if ( gzRoot[0] )   
      sprintf ( target, "%s", gzRoot+1 ) ;
    else
      sprintf ( target, "%s", gzAUTOROUTER ) ;

    sprintf ( sender, "%s%s", gzConcept, gzParent ) ;

    sprintf ( message,
              "|%s|event|DISCONNECT|%s|%s|%s|||",
              target,
              sender,
              gHyp_util_random8(),
              gHyp_util_timeStamp ( gsCurTime ) ) ;
    gHyp_sock_logger ( gzOutboxPath, message, NULL ) ;
    gHyp_sock_write ( (SOCKET) pConcept->link.outbox, 
                      message, 
                      strlen(message), 
                      SOCKET_FIFO,
                      (OVERLAPPED*) &pConcept->link.outboxOverlapped,
		      NULL ) ;

  }
  return TRUE ;
}  


int gHyp_concept_readSelect ( sConcept *pConcept )
{
  int 
    nBytes = -1 ;

  /* Try reading from the ports */  
  
  /* If we're nested too deep in handlers, then forget it */
  if ( giJmpLevel == MAX_JMP_LEVEL ) {
    gHyp_util_logError ( "Read select jump level overflow at %d",MAX_JMP_LEVEL ) ;
    longjmp ( gsJmpStack[0], COND_FATAL ) ;
  }
  
  giJmpEnabled = FALSE ; /* Disable longjmp from handlers */
  
  /* Get next message. */
  nBytes = gHyp_sock_select ( pConcept,
                              pConcept->link.inbox,
                              pConcept->link.listenTCP,
                              pConcept->link.pClients,
                              pConcept->link.pHosts,
                              pConcept->link.pSockets ) ;
  
  giJmpEnabled = TRUE ; /* Enable longjmp from handlers */
  
  /* The value of nBytes:
   * == 0, nothing received
   * == -1, error, flag will be set.
   * >  0, message received, we exit to process. 
   */
  return nBytes ;
}

sLOGICAL gHyp_concept_moveto ( sConcept *pConcept , char *target )
{

  /* CONCEPT NAME STAYS THE SAME, JUST THE LOCATION (PARENT) CHANGES  */

  sLOGICAL 
    f ;

  sData
    *pFirst,
    *pData ;

  sInstance
    *pAI ;

  char
      newParent[OBJECT_SIZE+1],
      newRoot[OBJECT_SIZE+1],
      instance[INSTANCE_SIZE+1],
      concept[OBJECT_SIZE+1],
      parent[OBJECT_SIZE+1],
      root[OBJECT_SIZE+1],
      host[HOST_SIZE+1],    
      targetPath[OBJECT_SIZE+1] ;
  
  /* Not ok to move if we hav already aconnected */
  if ( pConcept->link.hasRegistered )  return FALSE ;

  gHyp_util_breakTarget ( target, instance, concept, parent, root, host ) ;

  sprintf ( newParent, "/%s%s", concept, parent ) ;

  if ( strcmp ( instance, concept ) == 0 )
    sprintf ( newRoot, "/%s%s", concept, root ) ;
    /*sprintf ( newRoot, "/%s%s", concept, parent ) ;*/
  else
    sprintf ( newRoot, "/%s#%s%s", instance, concept, root ) ;

  if ( !gHyp_concept_setParent ( pConcept, newParent, newRoot ) ) return FALSE ;

  for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pConcept->exec.pInstances ) ;
        (f && pData) || pData != pFirst;
        f=FALSE, pData = gHyp_data_getNext ( pData ) ) {
    
    pAI = (sInstance*) gHyp_data_getObject ( pData ) ;

    sprintf ( targetPath, "%s%s", gHyp_instance_getTargetId(pAI), newRoot ) ;
    gHyp_instance_setTargetPath ( pAI, targetPath ) ;

  }
  return TRUE ;
}


sLOGICAL gHyp_concept_renameto ( sConcept *pConcept, char *target )
{

  /* CONCEPT NAME AND LOCATION CHANGES */

  sInstance
    *pAImain ,
    *pAInew ;

  char
    oldInstance[INSTANCE_SIZE+1],
      instance[INSTANCE_SIZE+1],
      concept[OBJECT_SIZE+1],
      parent[OBJECT_SIZE+1],
      root[OBJECT_SIZE+1],
      host[HOST_SIZE+1] ;
  
  /* Not ok to move if we hav already aconnected */
  if ( pConcept->link.hasRegistered )  return FALSE ;

  pAImain = gHyp_concept_getConceptInstance ( pConcept ) ;
  strcpy ( oldInstance, gzConcept ) ;

  gHyp_util_breakTarget ( target, instance, concept, parent, root, host ) ;

  strcpy ( gzInstance, instance ) ;
  strcpy ( gzConcept, concept ) ;

  if ( !gHyp_concept_setParent ( pConcept, parent, root ) ) return FALSE ;

  pAInew = gHyp_concept_instantiate ( pConcept, 
				      pAImain,
				      instance,
				      root,
				      FALSE, TRUE, TRUE ) ;

  /*gHyp_util_debug("t=%s,c=%s,path=%s,p=%s, r=%s",
		    gHyp_instance_getTargetId ( pAInew ),
		    gzConcept,
		    gHyp_instance_getTargetPath ( pAInew ),
		    gzParent,
		    gzRoot ) ;
  */
		    
  gHyp_instance_swapDevices ( pAInew, pAImain ) ;

  /* Now delete the old instance */
  gHyp_data_deleteChildByName ( pConcept->exec.pInstances, oldInstance ) ;
  return TRUE ;
}


int gHyp_concept_route ( sConcept* pConcept, char *pMsg )
{
  return gHyp_router_message ( pConcept, 
                               pConcept->link.pClients, 
                               pConcept->link.pHosts,
                               pMsg,
			       "" ) ;
}

sLOGICAL gHyp_concept_writeParent ( sConcept * pConcept, 
				    char* pMsg,
				    char* parentObject,
				    char* parentPath,
				    sLOGICAL isConnect,
				    sLOGICAL isEventOrQuery )
{
  /* Description:
   *
   *    Write a message to the parent, so it can route it.
   *
   * Arguments:
   *
   *    pAI                                             [R]
   *    - pointer to instance
   *
   * Return value:
   *
   *    Returns TRUE if the write operation was successfull, FALSE otherwise.
   *
   * Modifications:
   *
   */
  int 
    nBytes,
    maxTries = 2;

  sData
    *pTargetData ;

  sLOGICAL
     isRouted=FALSE ;

  SOCKET
    outbox ;

  /* There must be a reader so that the parent can reply a message back to us */
  if ( !gHyp_concept_openReader ( pConcept ) ) return FALSE ; 

  /* If there is no parent, then we cannot route the message. */
  if ( !gHyp_concept_openWriter ( pConcept ) ) {

    if ( !(guRunFlags & RUN_ROOT) &&
         isConnect && 
	 isEventOrQuery ) {

      pTargetData = gHyp_sock_findHYP ( NULL, 
					parentObject,
					parentPath, 
					pMsg,
					gpsAImsg,
					&isRouted ) ;
      if ( !pTargetData )
	pTargetData = gHyp_sock_findSCR ( NULL, 
					parentObject,
					parentPath, 
					pMsg,
					gpsAImsg,
					&isRouted ) ;

      if ( !pTargetData )
	pTargetData = gHyp_sock_findCOM ( NULL, 
					parentObject,
					parentPath, 
					pMsg,
					gpsAImsg,
					&isRouted ) ;

      if ( !pTargetData ) 
	    pTargetData = gHyp_sock_findEXE ( NULL, 
					parentObject,
					parentPath, 
					pMsg,
					gpsAImsg,
					&isRouted ) ;

      if ( pTargetData ) {

	pConcept->link.hasRegistered = TRUE ;
	pConcept->link.outbox = 
	  (HANDLE) gHyp_channel_socket ( 
	    (sChannel*) gHyp_data_getObject ( pTargetData )
	    ) ;
	gHyp_data_delete ( pTargetData ) ;
      }
    }
  }
  if ( isRouted ) return TRUE ;

  outbox = (SOCKET) pConcept->link.outbox ;
  if ( outbox == INVALID_SOCKET ) return FALSE ;


  /* Do the write operation */  
  gHyp_sock_logger ( gzOutboxPath, pMsg, NULL ) ;

  while ( maxTries-- ) {

    nBytes = gHyp_sock_write (  outbox,
  	                        pMsg,
	                        strlen(pMsg), 
		                SOCKET_FIFO,
			        (OVERLAPPED*) &pConcept->link.outboxOverlapped,
				NULL )  ;

    if ( nBytes > 0 ) return TRUE ;

    gHyp_concept_closeWriter ( pConcept ) ;

    /* The parent (router) could have died and been restarted */
    if ( !gHyp_concept_openWriter( pConcept ) ) break ;

    outbox = (SOCKET) pConcept->link.outbox ;
  }
  
  gHyp_instance_signalHangup ( gHyp_concept_getConceptInstance (pConcept), 
			       outbox, 0, 0 ) ;

  return FALSE ;
}


char * gHyp_concept_createToken ( sConcept *pConcept, char *token )
{
  return (char*) gHyp_hash_create ( pConcept->exec.tokens,
                                    TOKEN_HASH_TABLE_SIZE,
                                    token,
                                    NULL ) ;
}

sInstance *gHyp_concept_getInstForDeviceId ( sConcept *pConcept, sWORD id )
{
  /* Since id is unique, only one instance should have it */

  sLOGICAL 
    f ;

  sData
    *pFirst,
    *pData ;

  sInstance
    *pAI ;

  for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pConcept->exec.pInstances ) ;
        (f && pData) || pData != pFirst;
        f=FALSE, pData = gHyp_data_getNext ( pData ) ) {
    
    pAI = (sInstance*) gHyp_data_getObject ( pData ) ;
    if (  gHyp_instance_getDeviceFd ( pAI, id ) != INVALID_SOCKET ) return pAI ;
  }
  return NULL ;

}

sInstance *gHyp_concept_getInstForFd ( sConcept *pConcept, SOCKET fd )
{

  sLOGICAL 
    f ;

  sData
    *pFirst,
    *pData ;

  sInstance
    *pAI ;

  for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pConcept->exec.pInstances ) ;
        (f && pData) || pData != pFirst;
        f=FALSE, pData = gHyp_data_getNext ( pData ) ) {
    
    pAI = (sInstance*) gHyp_data_getObject ( pData ) ;

    /* The function will only return true if the id is NOT NULL_DEVICEID */ 
    if ( gHyp_instance_hasDeviceId ( pAI, fd ) ) return pAI ;
  }
  return NULL ;

}

sInstance *gHyp_concept_getInstForPlaceHolderFd ( sConcept *pConcept, SOCKET fd )
{

  sLOGICAL 
    f ;

  sData
    *pFirst,
    *pData ;

  sInstance
    *pAI ;

  for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pConcept->exec.pInstances ) ;
        (f && pData) || pData != pFirst;
        f=FALSE, pData = gHyp_data_getNext ( pData ) ) {
    
    pAI = (sInstance*) gHyp_data_getObject ( pData ) ;

    /* The function will only return true if the id is NULL_DEVICEID */ 
    if ( gHyp_instance_hasNullDeviceId ( pAI, fd ) ) return pAI ;
  }
  return NULL ;

}


sBYTE gHyp_concept_getSockObjType ( sConcept *pConcept, SOCKET socket )
{
  sData
    *pData ;

  char
    socketString[32] ;

  sprintf ( socketString, "%x", socket ) ;
  pData = gHyp_data_getChildByName ( pConcept->link.pSockets, socketString ) ;

  if ( pData )
    return gHyp_data_getObjectType ( pData ) ;
  else
    return DATA_OBJECT_NULL ;
}

void *gHyp_concept_getSocketObject ( sConcept *pConcept, SOCKET socket, sBYTE objectType )
{
  sData
    *pData ;

  void
    *pObject ;

  char
    socketString[32] ;

  sprintf ( socketString, "%x", socket ) ;
  pData = gHyp_data_getChildByName ( pConcept->link.pSockets, socketString ) ;

  if ( pData ) {
    pObject = gHyp_data_getObject ( pData ) ;
    if ( objectType == DATA_OBJECT_NULL ||
         gHyp_data_getObjectType ( pData ) == objectType ) 
	  return pObject ;
  }
  
  return NULL ;
}

void gHyp_concept_newSocketObject ( sConcept *pConcept, 
                                    SOCKET socket,
                                    void *pObject,
                                    sBYTE objectType,
                                    void  (*pfDelete)(void*) )
{ 


  /* Application sockets:
   *
   * Protocols and Sockets Supported:
   *
   *     PROTOCOL_SECS1  |  SOCKET_SERIAL
   *     - a serial connection using SECS1 protocol.
   *
   *     PROTOCOL_SECS1  |  SOCKET_TCP
   *     - a tcp connection (via terminal server) using SECS1 protocol.
   *
   *     PROTOCOL_SECS1  | SOCKET_LISTEN:
   *     - a secs1 tcp listen service that creates PROTOCOL_SECS1/SOCKET_TCP channels.
   *
   *     PROTOCOL_HSMS | SOCKET_TCP :
   *    - a tcp connection using the HSMS protocol.
   *
   *     PROTOCOL_HSMS | SOCKET_LISTEN:
   *     - a tcp listen service expecting that creates PROTOCOL_HSMS/SOCKET_TCP channel(s).
   *
   *     PROTOCOL_NONE | SOCKET_SERIAL
   *     PROTOCOL_NONE | SOCKET_TCP
   *     - a general purpose serial or tcp connection using any protocol
   *
   *     PROTOCOL_NONE  | SOCKET_LISTEN:
   *     - a general purpose tcp listen service that creates PROTOCOL_NONE/SOCKET_TCP channels.
   *
   *     PROTOCOL_HTTP | SOCKET_SERIAL
   *     PROTOCOL_HTTP | SOCKET_TCP
   *     - a general purpose serial or tcp connection using any protocol
   *
   *     PROTOCOL_HTTP  | SOCKET_LISTEN:
   *     - a general purpose tcp listen service that creates PROTOCOL_HTTP/SOCKET_TCP channels.

  */
  sData
    *pData ;

  char
    socketString[32] ;

  sprintf ( socketString, "%x", socket ) ;
  pData = gHyp_data_getChildByName ( pConcept->link.pSockets, socketString ) ;

  if ( !pData ) {

    pData = gHyp_data_new ( socketString ) ;
    gHyp_data_setObject ( pData,
                          pObject,
                          objectType,
                          pfDelete ) ;
    gHyp_data_append ( pConcept->link.pSockets, pData ) ;
  }
  else
    gHyp_data_setObject ( pData,
                          pObject,
                          objectType,
                          pfDelete ) ;
  return ;
} 


void gHyp_concept_deleteSocketObject ( sConcept *pConcept, SOCKET socket )
{ 
  sData
    *pData ;

  char
    socketString[32] ;

  sBYTE
    objectType ;

  sHsms
    *pHsms ;

  sSecs1
    *pSecs1,
    *pPort ;

  sSecs2
    *pSecs2 ;

  SOCKET
    parentSocket ;

  sInstance
    *pInstance ;

  sWORD
    id=NULL_DEVICEID ;

  /* Get the instance for the socket */
  pInstance = gHyp_concept_getInstForFd ( pConcept, socket ) ;
  if ( pInstance ) {

    id = gHyp_instance_getDeviceId ( pInstance, socket ) ;
    gHyp_instance_deleteFd ( pInstance, socket ) ;
  }

  sprintf ( socketString, "%x", socket ) ;
  pData = gHyp_data_getChildByName ( pConcept->link.pSockets, socketString ) ;
  if ( pData ) {

    objectType = gHyp_data_getObjectType ( pData ) ;

    switch ( objectType ) {
    case DATA_OBJECT_HSMS :
      pHsms = (sHsms*) gHyp_data_getObject ( pData ) ;
      parentSocket = gHyp_hsms_parentSocket ( pHsms ) ;
      if ( parentSocket != INVALID_SOCKET && pInstance != NULL ) { 
	gHyp_util_logInfo("Reassigning device %d to port %d",id, parentSocket ) ;
	gHyp_instance_updateFd ( pInstance, 
				 parentSocket, 
				 id,
				 NULL,
			         FALSE ) ;
      }
    break ;
    case DATA_OBJECT_SECS1 :
      pSecs1 = (sSecs1*) gHyp_data_getObject ( pData ) ;
      parentSocket = gHyp_secs1_parentSocket ( pSecs1 ) ;
      if ( parentSocket != INVALID_SOCKET && pInstance != NULL ) {
	gHyp_util_logInfo("Reassigning device %d to port %d",id, parentSocket ) ;
	pSecs2 = gHyp_secs2_new () ;
	gHyp_instance_updateFd (  pInstance,
				  parentSocket, 
				  id,
				  pSecs2, 
				  FALSE ) ;
      }
      break ;
    case DATA_OBJECT_PORT :
    case DATA_OBJECT_HTTP :
      pPort = (sSecs1*) gHyp_data_getObject ( pData ) ;
      parentSocket = gHyp_secs1_parentSocket ( pPort ) ;
      if ( parentSocket != INVALID_SOCKET && pInstance != NULL ) {
        gHyp_util_logInfo("Reassigning device %d to port %d",id, parentSocket ) ;
	gHyp_instance_updateFd (  pInstance, 
				    parentSocket, 
				    id,
				    NULL,
				    FALSE ) ;

      }

      break ;
    }
    gHyp_data_detach ( pData ) ;
    gHyp_data_delete ( pData ) ;
   
  }
  return ;
}
    
sChannel * gHyp_concept_findClient ( sConcept *pConcept,  char *object )
{
  sData
    *pData = gHyp_data_getChildByName ( pConcept->link.pClients, object ) ;

  if ( pData )
    return (sChannel *) gHyp_data_getObject ( pData ) ;
  else
    return NULL ;
}

sChannel * gHyp_concept_findNetwork ( sConcept *pConcept,  char *addr )
{
  sData
    *pData = gHyp_data_getChildByName ( pConcept->link.pHosts, addr ) ;

  if ( pData )
    return (sChannel *) gHyp_data_getObject ( pData ) ;
  else
    return NULL ;
}

sData* gHyp_concept_createNetwork ( sConcept *pConcept, char *host, char *addr, SOCKET s )
{
  return gHyp_sock_createNetwork ( pConcept->link.pHosts, host, addr, s ) ;
}

sData* gHyp_concept_getClients ( sConcept *pConcept )
{
  return pConcept->link.pClients ;
}

sChannel * gHyp_concept_findNetworkByFd ( sConcept *pConcept, SOCKET fd )
{

  sLOGICAL 
    f ;

  sData
    *pFirst,
    *pData ;

  sChannel
    *pChannel ;

  for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pConcept->link.pHosts ) ;
        (f && pData) || pData != pFirst;
        f=FALSE, pData = gHyp_data_getNext ( pData ) ) {
    
    pChannel = (sChannel *) gHyp_data_getObject ( pData ) ;

    if ( gHyp_channel_socket ( pChannel ) == fd ) return pChannel ;

  }
  return NULL ;
}
