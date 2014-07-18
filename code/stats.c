/* STATS.C 
 * 
 * Modifications:
 *
 *   $Log: stats.c,v $
 *   Revision 1.4  2006/01/16 18:56:36  bergsma
 *   HS 3.6.6
 *   1. Save query timeout events.  Don't let queries repeat indefinitely.
 *   2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 *   Revision 1.3  2005/03/16 23:58:44  bergsma
 *   gzLocalHost and gzLocalAddr are  not static
 *
 *   Revision 1.2  2004/12/17 17:39:41  jbergsma
 *   Fixes and ifdefs for AS_ATL (WebPickle) to compile correctly with cpp in the hsx project
 *
 *   Revision 1.1  2003/04/04 16:01:15  bergsma
 *   The stats.c and stats.com will get statistics from router.log that can
 *   be loaded into excell.
 *
 *
 */

#include "auto.h"	/* System Interface and Function Prototypes */

/* EXTERNAL GLOBALS */
FILE*			gsLog ;
char			gzLogName[MAX_PATH_SIZE+1] ;
char                    gzTLogName[MAX_PATH_SIZE+1] ;
FILE*                   gsTLog ;
int			giTCPchannel ;	/* HyperScript port id */
int			giSocketFIFO  ;
int			giSocketLISTEN ;
int			giSocketLOOPBACK_W ;
int			giSocketLOOPBACK_R ;
char*			gpzLocalHost ;	/* Local host name */
char*			gpzLocalAddr ;	/* Local addr name */
char*			gpzTargetId ;	/* Local target Id */
int			giPID ;		/* PID of process */
time_t			gsCurTime ; 	/* Current time, updated regularily.*/
sLOGICAL		guTimeStamp ;	/* Integer timeStamp. */
unsigned		guSIGIO ;		/* Set when SIGIO */
unsigned		guSIGPIPE ;		/* Set when SIGPIPE */
unsigned		guSIGALRM ;		/* Set when SIGALRM */
unsigned		guSIGINT ;		/* Set when SIGINT */
unsigned		guSIGTERM ;		/* Set when SIGTERM */
unsigned		guSIGHUP ;		/* Set when SIGHUP */
unsigned		guSIGMBX ;
unsigned short		guRunFlags ;	/* General flags */

#ifdef AS_VMS
#include <ssdef.h>		/* SS$_ defines */
#endif

/* INTERNAL (STATIC) GLOBALS */

char		gzLocalHost[MAX_PATH_SIZE+1];	/* Local host name */
char		gzLocalAddr[MAX_PATH_SIZE+1];	/* Local host name */

#ifndef F_OK
#define	F_OK		0	/* does file exist */
#define	X_OK		1	/* is it executable by caller */
#define	W_OK		2	/* writable by caller */
#define	R_OK		4	/* readable by caller */
#endif

#ifndef AS_WINDOWS
#ifdef __cplusplus
extern "C" pid_t getpid(void);
#else
extern pid_t getpid(void);
#endif
#endif

int main ( int argc, char * argv[] )
{

#define NUM_OPTIONS 3

  char *usage[NUM_OPTIONS] =
  {
    "usage: -f file                (input from 'file' instead of stdin)\n",
    "       -l log                 (output to 'log' instead of stdout)\n",
    "       -h                     (print this help)\n"
  };
 
  int 
    n,
    i,
    ss1,
    ss2,
    context1,
    context2 ;
  
  FILE
    *pp ;
  
  sLOGICAL
    errflg = FALSE ;

  sAImsg
    *pAImsg ;

  sData
    *pTV,
    *pValue,
    *pData ;

  char
    c,
    token[TOKEN_SIZE+1],
    value[VALUE_SIZE+1],
    *pValueStr,
    *pTokenStr,
    tlog[MAX_PATH_SIZE+1],
    output[MAX_OUTPUT_LENGTH+1],
    stream[MAX_INPUT_LENGTH+1],		
    *pMessage,
    *pStream,
    LOTID[VALUE_SIZE+1],
    EQPID[VALUE_SIZE+1],
    LOTTYPE[VALUE_SIZE+1],
    NUMWAFERS[VALUE_SIZE+1],
    NUMWAFS[VALUE_SIZE+1],
    PRIORITY[VALUE_SIZE+1],
    EQPTYPE[VALUE_SIZE+1],
    PARTID[VALUE_SIZE+1],
    PROCID[VALUE_SIZE+1],
    RECPID[VALUE_SIZE+1],
    DCOP[VALUE_SIZE+1],
    STEP[VALUE_SIZE+1],
    ROUTE[VALUE_SIZE+1],
    STAGE[VALUE_SIZE+1],
    LOCATION[VALUE_SIZE+1],
    USERID[VALUE_SIZE+1],
    AUTOTYPE[VALUE_SIZE+1],
    TIMESTAMP[VALUE_SIZE+1] ;


#ifdef AS_PROMIS
  /* All PROMIS images require cfc_initimage */
  cfc_initimage() ;
#endif

  /* Initialize */
  gsLog = stdout ;
  tlog[0] = '\0' ;
  gzLogName[0] = '\0' ;

  /*
  gHyp_util_logInfo("Starting ...." );
  for ( i=0; i<argc; i++ ) gHyp_util_logInfo ( "argv[%d] = %s"[VALUE_SIZE+1],
     i, argv[i] ) ;
  */

  /* Get command line arguments and set flags, options */
  while ( (c = gHyp_util_getopt ( argc, argv, "hf:l:" )) != EOF ) {
  
    switch ( c ) {
      
    case 'f' : 
      n = MIN ( strlen ( gzOptarg ), MAX_PATH_SIZE ) ;
      strncpy ( tlog, gzOptarg, n ) ;
      tlog[n] = '\0' ;
      break ;
      
    case 'l' :
      n = MIN ( strlen ( gzOptarg ), MAX_PATH_SIZE ) ;
      strncpy ( gzLogName, gzOptarg, n ) ;
      gzLogName[n] = '\0' ;
      break ;	
      
    case 'h' :
    case '?':
    default:
      errflg = TRUE ;
      break;
    }
  }
   
  /* Check for command line errors */
  if ( giOptind < argc )
      gHyp_util_logWarning ( "Ignoring argument '%s'", argv[giOptind] ) ;
  
  if ( errflg ) {
    for (i = 0; i < NUM_OPTIONS; i++)
      fprintf (stderr, usage[i] );
   exit (2) ;
  }
  
  /*guRunFlags = ( RUN_QUIET + RUN_INTERACTIVE ) ;*/

  /* Get current host name */
  gHyp_util_gethostname ( gzLocalHost ) ;

  /* Resolve it */
  gHyp_tcp_createAliasTable() ;
  gHyp_tcp_resolveHost ( gzLocalHost, gzLocalAddr ) ;

  /* Keep global pointer */
  gpzLocalHost = gzLocalHost ;
  gpzLocalAddr = gzLocalAddr ;

  /* The PID is needed for various functions and applications */
#ifndef AS_WINDOWS
  /* Get the PID of the process */
  giPID = getpid() ;
#else
  giPID = GetCurrentProcessId();
; 
#endif

  /* Set default log name if not already set */
  if ( gzLogName[0] ) {
    if ( (gsLog = fopen ( gzLogName, "a+"
#if defined( AS_VMS ) && !defined(_ANSI_C_SOURCE)
			  , "shr=get,put"
#endif
			  ) ) == NULL ) {
      gHyp_util_sysError ( "Failed to open log '%s'", gzLogName ) ;
      exit(2) ;
    }
  }

  /* Set default program source if not already set */
  if ( tlog[0] ) {
    if ( (pp = fopen ( tlog, "r" )) == NULL ) {
      gHyp_util_sysError ( "Failed to open tlog '%s'", tlog ) ;
      exit(2) ;
    }
  }
  else
    pp = stdin ;

  pAImsg = gHyp_aimsg_new () ;
  while ( (pStream = fgets ( stream, MAX_INPUT_LENGTH, pp )) != NULL ) {

    /* Remove any "|" at the beginning */
    pMessage = strchr ( pStream, '|' ) ;
    if ( !pMessage ) continue ;

    /* Parse the message */
    gHyp_aimsg_initParse ( pAImsg, pMessage );
    pTV = gHyp_aimsg_tv ( pAImsg ) ;

    
    ss1 = -1 ; 
    context1 = -1 ;
    pData = NULL ;
    output[0] = '\0' ;

    LOTID[0] = '\0' ;
    EQPID[0] = '\0' ;
    LOTTYPE[0] = '\0' ;
    NUMWAFERS[0] = '\0' ;
    NUMWAFS[0] = '\0' ;
    PRIORITY[0] = '\0' ;
    EQPTYPE[0] = '\0' ;
    PARTID[0] = '\0' ;
    PROCID[0] = '\0' ;
    RECPID[0] = '\0' ;
    DCOP[0] = '\0' ;
    STEP[0] = '\0' ;
    ROUTE[0] = '\0' ;
    STAGE[0] = '\0' ;
    LOCATION[0] = '\0' ;
    USERID[0] = '\0' ;
    AUTOTYPE[0] = '\0' ;
    strcpy ( TIMESTAMP, gHyp_aimsg_getTS ( pAImsg ) ) ;

    while ( (pData = gHyp_data_nextValue ( pTV,
					   pData, 
					   &context1,
					   ss1 ) ) ) {

      n = gHyp_data_getStr ( pData, 
			     token, 
			     TOKEN_SIZE, 
			     context1, 
			     FALSE ) ;
      pTokenStr = token ;
      ss2 = -1 ; 
      context2 = -1 ;
      pValue = NULL ;
      while ( (pValue = gHyp_data_nextValue ( pData, 
					      pValue, 
					      &context2,
					      ss2 ) ) ) {

        n = gHyp_data_getStr ( pValue, 
			       value, 
			       VALUE_SIZE, 
			       context2, 
			       FALSE ) ;
	pValueStr = value ;

	if ( strcmp ( pTokenStr, "LOTID" ) == 0 )

	  strcpy ( LOTID, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "LOTIDS" ) == 0 )

	  strcpy ( LOTID, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "EQPID" ) == 0 )

	  strcpy ( EQPID, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "LOTTYPE" ) == 0 )

	  strcpy ( LOTTYPE, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "NUMWAFERS" ) == 0 )

	  strcpy ( NUMWAFERS, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "NUMWAFS" ) == 0 )

	  strcpy ( NUMWAFS, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "PRIORITY" ) == 0 )

	  strcpy ( PRIORITY, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "EQPTYPE" ) == 0 )

	  strcpy ( EQPTYPE, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "PARTID" ) == 0 )

	  strcpy ( PARTID, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "PROCID" ) == 0 )

	  strcpy ( PROCID, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "RECPID" ) == 0 )

	  strcpy ( RECPID, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "RECIPES" ) == 0 )

	  strcpy ( RECPID, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "DCOP" ) == 0 )

	  strcpy ( DCOP, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "STEP" ) == 0 )

	  strcpy ( STEP, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "ROUTE" ) == 0 )

	  strcpy ( ROUTE, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "STAGE" ) == 0 )

	  strcpy ( STAGE, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "LOCATION" ) == 0 )

	  strcpy ( LOCATION, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "USERID" ) == 0 )

	  strcpy ( USERID, pValueStr ) ;

	else if ( strcmp ( pTokenStr, "AUTOTYPE" ) == 0 )

	  strcpy ( AUTOTYPE, pValueStr ) ;

	/* Print out only the first value */
	break ;
      }
    }

    /* Discard when SENDER is NONE */
    if ( USERID[0] == '\0' ) continue ;
    
    sprintf ( output,
    "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
    TIMESTAMP,LOTID,EQPID,LOTTYPE,NUMWAFERS,NUMWAFS,PRIORITY,EQPTYPE,PARTID,
    PROCID,RECPID,DCOP,STEP,ROUTE,STAGE,LOCATION,USERID,AUTOTYPE ) ;

    gHyp_util_log ( output ) ;
  }


#ifdef PROFILE
    exit(2) ;
#endif

  return 0 ;
}
