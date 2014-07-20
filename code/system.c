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
 *	$Log: system.c,v $
 *	Revision 1.42  2011-02-24 05:12:27  bergsma
 *	Adjusting values for MAX_OUTPUT_LENGTH and MAX_INPUT_LENGTH
 *	
 *	Revision 1.41  2011-02-20 00:52:41  bergsma
 *	Use MAX_INPUT_LENGTH instead of MAX_OUTPUT_LENGHT for commands
 *	
 *	Revision 1.40  2011-01-08 21:44:35  bergsma
 *	New args to frame Variable functions.
 *	
 *	Revision 1.39  2010-03-05 06:08:46  bergsma
 *	Add -u option and AUTOHOST environment variable.
 *	
 *	Revision 1.38  2010-01-16 18:34:11  bergsma
 *	Increase input stream for XML.
 *	
 *	Revision 1.37  2010-01-15 08:30:42  bergsma
 *	Increase XML input buffers for parsing
 *	
 *	Revision 1.36  2009-09-21 05:19:57  bergsma
 *	Comments
 *	
 *	Revision 1.35  2009-06-17 22:49:02  bergsma
 *	For sql_datetime and datetime, reject erroreous dates, SPECIFICALLY from
 *	PROMIS, a NULL date is -1200798848
 *	
 *	Revision 1.34  2009-06-12 05:01:28  bergsma
 *	HS 385 Final Checkin and TAG - added setheap function.
 *	
 *	Revision 1.33  2008-09-09 13:48:48  bergsma
 *	Make gzStream larger, 4* MAX_INPUT_LENGHT.  Allows bigger XML
 *	structures to be parsed (large chrt records from PROMIS).
 *	
 *	Revision 1.32  2008-08-27 07:22:59  bergsma
 *	AUTOSPOOL was not extern
 *	
 *	Revision 1.31  2008-05-03 21:40:12  bergsma
 *	When doing shell command, capture all output.
 *	Make giLineCount the preferred way to know where the code is.
 *	
 *	Revision 1.30  2008-02-12 23:31:18  bergsma
 *	VS 2008 update
 *	
 *	Revision 1.29  2007-06-20 21:09:24  bergsma
 *	Use popRdata, not popRvalue, when subsequently doing a data_getNext
 *	
 *	Revision 1.28  2007-05-26 22:08:43  bergsma
 *	ADD -w "Ward" flag
 *	
 *	Revision 1.27  2007-05-02 20:34:01  bergsma
 *	Fix parseurl function.  Improve various print/debug/log statements.
 *	Fix problem with chunked data transfers.
 *	
 *	Revision 1.26  2007-03-26 00:19:57  bergsma
 *	Only allow the setting of returnToStdin to TRUE when the instance
 *	executing the sleep() function is itself the parent instance,
 *	
 *	Revision 1.25  2007-03-22 04:28:44  bergsma
 *	Make getclock() function more portable
 *	
 *	Revision 1.24  2007-03-15 01:08:27  bergsma
 *	Added getclock function
 *	
 *	Revision 1.23  2007-02-15 19:27:46  bergsma
 *	Fix for datatype attribute, truncstrlen adjusments (added mayReturnChildLess)
 *	
 *	Revision 1.22  2007-02-13 22:34:19  bergsma
 *	XML end-of-buffer was not being properly detected
 *	
 *	Revision 1.21  2006-10-12 23:09:09  bergsma
 *	Fix problems with STATE_SLEEP state.
 *	
 *	Revision 1.20  2006/10/01 16:25:26  bergsma
 *	Added support for asctime() function and the means to parse it (in dateparse.c)
 *	
 *	Revision 1.19  2006/08/17 05:03:30  bergsma
 *	Detect non-XML (HTML) and assume childless tags won't have /> endings
 *	
 *	Revision 1.18  2006/01/16 18:56:36  bergsma
 *	HS 3.6.6
 *	1. Save query timeout events.  Don't let queries repeat indefinitely.
 *	2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *	
 *	Revision 1.17  2005/12/10 00:30:30  bergsma
 *	HS 3.6.5
 *	
 *	Revision 1.16  2005/10/25 16:39:37  bergsma
 *	Added usleep() function
 *	
 *	Revision 1.15  2005/06/12 16:46:22  bergsma
 *	HS 3.6.1
 *	
 *	Revision 1.14  2005/02/15 07:05:14  bergsma
 *	No end of line at end of file.
 *	
 *	Revision 1.13  2005/01/31 05:59:10  bergsma
 *	The sleep function was changed to be identical to the idle function, except
 *	for the addition of a time limit.
 *	
 *	Revision 1.12  2005/01/25 05:52:36  bergsma
 *	SLEEP uses new WakeTime instead of HEARTBEAT time.
 *	
 *	Revision 1.11  2004/10/16 05:10:55  bergsma
 *	Remove warning in xml() function.
 *	
 *	Revision 1.10  2004/04/29 02:18:11  bergsma
 *	Random number too small from 'random' function when used in VMS and WINDOWS.
 *	
 *	Revision 1.9  2003/01/12 04:54:21  bergsma
 *	For TRUE64, use fork instead of vfork.
 *	
 *	Revision 1.8  2003/01/09 01:20:17  bergsma
 *	Replace call to popRstring to popRvalue
 *	
 *	Revision 1.7  2003/01/09 00:23:24  bergsma
 *	V3.1.0
 *	Incorporate gHyp_instance_pushSTATUS for function return.
 *	
 *	Revision 1.6  2002/11/29 19:46:17  bergsma
 *	Removed isPRE argument from gHyp_cgi_parseXML
 *	
 *	Revision 1.5  2002/11/22 00:01:44  bergsma
 *	Added isSCR argument to gHyp_cgi_parseXML
 *	
 *	Revision 1.4  2002/11/14 01:40:45  bergsma
 *	Removed ^M occurances and added Modification section.
 *	
 *	Revision 1.3  2002/09/05 00:35:02  bergsma
 *	Added pValue argument back into gHyp_cgi_parseXML.  NULL in this case.
 *	
 *	Revision 1.2  2002/09/03 21:37:40  bergsma
 *	Changed arguments for gHyp_cgi_parseXML
 *	
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

#ifdef AS_VMS
#include <processes.h>		/* For vfork and exec */
#endif

#ifndef STDIN_FILENO
#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2
#endif

#ifndef AS_WINDOWS
#include <sys/wait.h>
#endif

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

static char gzStream[STREAM_READ_SIZE*4+1] ;

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

#ifdef __cplusplus

#ifndef AS_WINDOWS
extern "C" pid_t waitpid(pid_t pid, int *status, int options);
#endif
extern "C" int close ( int fd ) ;
extern "C" int dup( int filedes );
extern "C" int dup2( int oldfd, int newfd );                                  

#else

#ifndef AS_WINDOWS
extern pid_t waitpid(pid_t pid, int *status, int options);
#endif
extern int close ( int fd ) ;
extern int dup( int filedes );
extern int dup2( int oldfd, int newfd );   

#endif

void gHyp_system_date ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: date()
   *	Format of date is YYYYMMDD
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
      *pData,
      *pResult ;

    char
      dateStamp[DATE_SIZE+1] ;

    struct tm	*pstm ;

    time_t
      ts ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: date ( [utc] )" ) ;

    if ( argCount == 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      ts = gHyp_data_getRaw ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    }
    else
      ts = gsCurTime = time(NULL) ;

    pstm = localtime ( &ts ) ;
    if ( !pstm || pstm->tm_year > 138 ) {
      gHyp_instance_warning ( pAI,STATUS_BOUNDS, "Invalid UTC value value %d", ts ) ;
      strcpy ( dateStamp, "" ) ;
    }
    else {

      sprintf (	dateStamp, 
		"%04d%02d%02d", 
		pstm->tm_year+1900, pstm->tm_mon+1, pstm->tm_mday ) ;
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, dateStamp ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_system_datetime ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: datetime()
   *	Returns YYYYMMDD:hhmmss
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
      *pData,
      *pResult ;

    struct tm	*pstm ;	

    char
      timeStamp[DATETIME_SIZE+1] ;

    time_t
      ts ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: datetime ( [utc] )" ) ;

    if ( argCount == 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      ts = gHyp_data_getRaw ( pData, gHyp_data_getSubScript(pData), TRUE  ) ;
    }
    else
      ts = gsCurTime = time(NULL) ;

    pstm = localtime ( &ts ) ;
    if ( !pstm || pstm->tm_year > 138 ) {
      gHyp_instance_warning ( pAI,STATUS_BOUNDS,"Invalid UTC value value %d", ts ) ;
      strcpy ( timeStamp, "" ) ;
    }
    else {
      sprintf (	timeStamp, 
		"%04d%02d%02d:%02d%02d%02d", 
		pstm->tm_year+1900, pstm->tm_mon+1, pstm->tm_mday,
		pstm->tm_hour, pstm->tm_min, pstm->tm_sec ) ;
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, timeStamp ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_system_asctime ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: asctime()
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
      *pData,
      *pResult ;

    struct tm	*pstm ;	

    char
      timeStamp[VALUE_SIZE+1] ;

    time_t
      ts ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: asctime ( [utc] )" ) ;

    if ( argCount == 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      ts = gHyp_data_getRaw ( pData, gHyp_data_getSubScript(pData), TRUE  ) ;
    }
    else
      ts = gsCurTime = time(NULL) ;

    pstm = localtime ( &ts ) ;
    if ( !pstm ) gHyp_instance_error ( pAI,STATUS_BOUNDS,
	"Invalid UTC value value %d", ts ) ;


    strcpy ( timeStamp, asctime( pstm ) ) ;
    gHyp_util_trim ( timeStamp ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, timeStamp ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_system_exec ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: exec()
   *	For UNIX systems, exec() is like the UNIX popen() function.
   *	The last line of stdout returned is the return value.
   *	If no output, the return value is 0.
   *	For VMS systems, exec() is like the VMS spawn() utility.
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
   * Modifications:
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
      *pData,
      *pResult ;

    char
      *pStr,
      line[MAX_INPUT_LENGTH+1],
      value[VALUE_SIZE+1],
      command[MAX_COMMAND_LENGTH+1],
      *pCmd,
      *pEndCmd ;
    
    FILE
      *fp = NULL ;
    
    int
      ss,
      valueLen,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;

#ifndef AS_WINDOWS
	int
		pid,
		pfd[2] ;
#endif

#ifdef AS_VMS
    int 
      save_stdout = STDOUT_FILENO ;
#endif
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
	gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: exec ( command )" ) ;
   
    /* Get command to execute */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

    /* Construct the message to be sent to the shell. */
    strcpy ( line, "" );
    pCmd = command ;
    pEndCmd = pCmd + MAX_COMMAND_LENGTH ;    
    pResult = NULL ;
    isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;
    ss = gHyp_data_getSubScript ( pData ) ; 
    context = -1 ;
    while ( (pResult = gHyp_data_nextValue ( pData, 
					     pResult,
					     &context,
					     ss ))) {
      valueLen = gHyp_data_getStr ( pResult, 
				    value, 
				    VALUE_SIZE, 
				    context, 
				    isVector ) ;
      pStr = value ;
      
      if ( (pCmd + valueLen) > pEndCmd ) 
	gHyp_instance_error ( pAI, STATUS_IO,
			      "exec statement longer than %d characters \n",
			      MAX_COMMAND_LENGTH ) ;
      sprintf ( pCmd, "%s", pStr ) ;
      pCmd += valueLen ;   
    }
    if ( context== -2 && ss != -1 ) 
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in exec()",ss) ;
    *pCmd = '\0' ;

    if ( !(guRunFlags & RUN_RESTRICTED) ) {

#ifndef AS_WINDOWS

      pResult = gHyp_data_new ( "_exec_" ) ;

      /* Create a pipe for receiving stdout from the child */
      if ( pipe( pfd ) < 0 ) 
        gHyp_util_sysError ( "Problem with pipe" ) ;
#ifndef AS_TRU64
      else if ( (pid = vfork ()) < 0 )
        gHyp_util_sysError ( "Problem with vfork" ) ;
#else
      else if ( (pid = fork ()) < 0 )
        gHyp_util_sysError ( "Problem with fork" ) ;
#endif
      else if ( pid == 0 ) {
      
        /* Child process */	
#ifdef AS_VMS
        save_stdout = dup ( STDOUT_FILENO ) ;
#endif
        close ( pfd[0] ) ;	/* Close read end of pipe */     
        close ( STDOUT_FILENO ) ;	 
        dup ( pfd[1] ) ;	/* Redirect stdout to write end of pipe */

#ifdef AS_VMS
        system ( command ) ;
#else
        execl ( "/bin/sh", "sh", "-c", command, (char *) 0 ) ;
#endif
        /* Child exits */	
        _exit(127);
      }	
      else {
     
        /* Parent. */	
        close ( pfd[1] ) ;	/* Close write end of pipe */     
      
        /* Read from child until it is done outputing. */
        if ( (fp = fdopen ( pfd[0], "r" ) ) != NULL ) {
  	  while ( fgets ( line, MAX_INPUT_LENGTH, fp ) != NULL ) {
	    pData = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pData, line ) ;
	    gHyp_data_append ( pResult, pData ) ;
            gHyp_util_log ( line ) ;
	  }
        }
        else {
	  /* Some problem */
	  pData = gHyp_data_new ( NULL ) ;
	  strcpy ( line, "0" ) ;
	  gHyp_data_setStr ( pData, line ) ;
	  gHyp_data_append ( pResult, pData ) ;
        }
        /* Wait for child to complete */
        if ( waitpid ( (pid_t) pid, (int *)0, 0 ) != pid ) 
 	  gHyp_util_sysError ( "Failed to waitpid" ) ;
      
        /* Close read end of pipe */
        fclose ( fp ) ;
        close ( pfd[0] ) ;

#ifdef AS_VMS
        dup2 ( save_stdout, STDOUT_FILENO ) ;
#endif
      }
      gHyp_stack_push ( pStack, pResult ) ;

#else
      /* Windows */
      system ( command ) ;
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;

#endif
    }
    else {
      gHyp_instance_warning ( pAI, STATUS_RESTRICTED, 
				"The exec() function is RESTRICTED" ) ;
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
    }
  }
}

void gHyp_system_system ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: system()
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
      command[MAX_COMMAND_LENGTH+1],
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
    	"Invalid arguments. Usage: system ( command )" ) ;
   
    /* Get command to execute */
    pData = gHyp_stack_popRdata ( pStack, pAI ) ;

    /* Construct the message to be sent to the shell. */
    pCmd = command ;
    pEndCmd = pCmd + MAX_COMMAND_LENGTH ;    
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
			      MAX_COMMAND_LENGTH ) ;
      sprintf ( pCmd, "%s", pStr ) ;
      pCmd += valueLen ;   
    }
    if ( context== -2 && ss != -1 ) 
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in exec()",ss) ;
    *pCmd = '\0' ;	
   
  if ( !(guRunFlags & RUN_RESTRICTED) )

    system ( command ) ;
  else
    gHyp_instance_warning ( pAI, STATUS_RESTRICTED, "The system() function is RESTRICTED" ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_system_srandom ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: srandom ( seed )
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
      *pData;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: srandom ( seed )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
#if defined (AS_VMS) || defined ( AS_WINDOWS )
    srand ( gHyp_data_getRaw ( pData, gHyp_data_getSubScript(pData), TRUE ) ) ;
#else
    srandom ( gHyp_data_getRaw ( pData, gHyp_data_getSubScript(pData),TRUE )) ;
#endif
    
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }
}

void gHyp_system_random ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: random ( )
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

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    sData	*pResult ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: random ( )" ) ;

    pResult = gHyp_data_new ( NULL ) ;
#if defined ( AS_VMS ) || defined ( AS_WINDOWS )
    gHyp_data_setInt ( pResult, rand () | (rand() << 16) ) ;
#else
    gHyp_data_setInt ( pResult, random () ) ;
#endif
    gHyp_stack_push ( pStack, pResult ) ;

  }
}

void gHyp_system_sleep ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sleep ( seconds )
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
      *pData;

    sConcept
      *pConcept ;

    int
      seconds,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )  gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: sleep ( seconds )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    seconds = gHyp_data_getInt ( pData, gHyp_data_getSubScript(pData), TRUE) ;

    if ( seconds < 0 || seconds > MAX_SLEEP_SECONDS ) {
      gHyp_instance_warning ( pAI,  STATUS_BOUNDS, "Invalid sleep seconds" ) ;
      gHyp_instance_pushSTATUS ( pAI, pStack) ;
    }
    else {
      gHyp_instance_setState ( pAI, STATE_SLEEP ) ;
      gHyp_frame_setState ( pFrame, STATE_SLEEP ) ;
      gsCurTime = time(NULL);
      gHyp_instance_setWakeTime ( pAI, (int)gsCurTime+seconds ) ;
      gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;

      /*gHyp_instance_pushSTATUS ( pAI, pStack ) ;*/
      
      gHyp_parse_restoreExprRank ( pParse ) ;

      /* If we are the parent, we may turn-off "return to stdIn".*/
      pConcept = gHyp_instance_getConcept(pAI) ;
      if ( gHyp_concept_getConceptInstance ( pConcept ) == pAI ) 
        gHyp_concept_setReturnToStdIn ( pConcept, FALSE ) ;

      if ( guDebugFlags & DEBUG_FRAME )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_FRAME, 
			     "frame: SLEEP (longjmp to 1 from frame %d)",
			     gHyp_frame_depth(pFrame) ) ;
      longjmp ( gsJmpStack[giJmpLevel=1], COND_SILENT ) ;

    }
  }
}

void gHyp_system_usleep ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: usleep ( microseconds )
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
      *pData;

    unsigned int
      useconds ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )  gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: usleep ( microseconds )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    useconds = (unsigned int) gHyp_data_getInt ( pData, gHyp_data_getSubScript(pData), TRUE) ;

    if ( useconds < 0 ) {
      gHyp_instance_warning ( pAI,  STATUS_BOUNDS, "Invalid usleep seconds" ) ;
    }
    else {
      gHyp_sock_usleep ( useconds ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}



void gHyp_system_time ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: time()
   *	Returns hhmmss format
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
      *pData,
      *pResult ;

    char
      timeStamp[TIME_SIZE+1] ;

    struct tm	*pstm ;

    time_t
      ts ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: time ( [utc] )" ) ;

    if ( argCount == 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      ts = gHyp_data_getRaw ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;
    }
    else
      ts = gsCurTime = time(NULL) ;

    pstm = localtime ( &ts ) ;
    if ( !pstm ) gHyp_instance_error ( pAI,STATUS_BOUNDS,
	"Invalid UTC value value %d", ts ) ;

    sprintf (	timeStamp, 
		"%02d%02d%02d", 
		pstm->tm_hour, pstm->tm_min, pstm->tm_sec ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, timeStamp ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_system_timestamp ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: timestamp ( boolean )
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
      *pData,
      *pResult ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: timestamp ( [boolean] )" ) ;

    if ( argCount == 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      guTimeStamp = gHyp_data_getBool ( pData,
					gHyp_data_getSubScript(pData), 
					TRUE ) ;
    }
    
    pResult = gHyp_data_new ( NULL ) ;
    gsCurTime = time(NULL) ;
    gHyp_data_setInt ( pResult, (int)gsCurTime ) ;
    gHyp_stack_push ( pStack, pResult ) ;

  }
}

void gHyp_system_getclock ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: getclock ()
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
      *pResult ;
    
    int
      tickcount,
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 0 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: getclock ()" ) ;
   
    tickcount = gHyp_util_getclock();

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setInt ( pResult, tickcount ) ;
    gHyp_stack_push ( pStack, pResult ) ;

  }
}


void gHyp_system_parsedate ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: parsedate ( string )
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
      *pData,
      *pResult ;
    
    char
      *pStr,
      value[VALUE_SIZE+1] ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    time_t
      timeStamp ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: parsedate ( [string] )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   value, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pStr = value ;
    timeStamp = gHyp_dateparse_parse ( pStr ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setInt ( pResult, (int)timeStamp ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_system_parse ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: parse ( file )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pFile ;

    char
      *pFileSpec=NULL,
      fileSpec[MAX_PATH_SIZE+1] ;
    
    FILE
      *pp = NULL ;
    
    sLOGICAL
      isDereference;

    sHyp
      *pHyp ;

    char
      *pStream ;

    int
      n,
      hypIndex,
      eosIndex,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sInstance
      *pAImain ;

   gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

   if ( argCount > 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: parse ( [file] )" ) ;
    

   if ( argCount == 0 ) {

      /* Go back to PARSE mode - must not allow return to IDLE */
     pAImain = gHyp_concept_getConceptInstance ( gHyp_instance_getConcept ( pAI ) ) ;
     if ( pAI == pAImain && !gHyp_instance_isEND ( pAI ) )
       gHyp_concept_setReturnToStdIn ( gHyp_instance_getConcept(pAI), TRUE ) ;
     gHyp_instance_setState ( pAI, STATE_PARSE ) ;
      strcpy ( fileSpec, "stdin" ) ;
      pFileSpec = fileSpec ;
    }
    else {
      pFile = gHyp_stack_popRvalue ( pStack, pAI ) ; 
      n = gHyp_data_getStr ( pFile, 
			     fileSpec, 
			     MAX_PATH_SIZE,
			     gHyp_data_getSubScript(pFile),
			     TRUE) ;
      pFileSpec = fileSpec ;
      pp = fopen ( pFileSpec, "r"
#ifdef AS_VMS
		   , "shr=get,put"
#endif
		   ) ;
      
      if ( !pp ) {
	gHyp_instance_warning ( pAI, STATUS_FILE, 
				"Could not open file '%s'", pFileSpec ) ;
      }
      else {

	/* Reset the line count */
	giLineCount = 0 ;

	/* Get the current EOS position */
	pHyp = gHyp_frame_getHyp ( pFrame ) ;
	eosIndex = gHyp_hyp_getHypCount ( pHyp ) ;

	/* Get the current execution position */
	hypIndex = gHyp_frame_getHypIndex ( pFrame ) ;

	/* For parse that occurs at the end-of-stream, no need to
	 * dereference.  Make it inline code.
	 * Otherwise, we surround the code with { } and call it like a method.
	 */
	isDereference = ( hypIndex < eosIndex ) ;

	if ( isDereference ) {
	  /* Load the new code */
	  giProgramCount++ ;
	  pStream = gHyp_load_fromStream ( pAI, pHyp, "{", giLineCount++ ) ;
	  if ( !pStream || *pStream ) {
	    gHyp_hyp_setHypCount ( pHyp, eosIndex ) ;
	    gHyp_instance_error ( pAI,
				  STATUS_UNDEFINED,
				  "Failed to load HyperScript segment (parse) '{'" ) ;
	  }
	}

 	while ( gHyp_hyp_source ( pAI, pHyp, pp, pFileSpec, FALSE ) >= 0 ) ;
	fclose ( pp ) ;

	if ( isDereference ) {
	  giProgramCount++ ;
	  pStream = gHyp_load_fromStream ( pAI, pHyp, ";}", giLineCount++);
	  if ( !pStream || *pStream ) {
	    gHyp_hyp_setHypCount ( pHyp, eosIndex ) ;
	    gHyp_instance_error ( pAI,
				  STATUS_UNDEFINED,
				  "Failed to load HyperScript segment (parse) ';}'" ) ;
	  }
	  
	  /*gHyp_hyp_describe ( pHyp ) ;*/

	  /*gHyp_util_debug("DEREF parse");*/
	  /* Set a dereference handler */
          /*gHyp_util_debug("Deref from parse");*/
	  gHyp_instance_setDerefHandler ( pAI, 
					  eosIndex, 
					  pHyp ) ;
	}

      }
    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


void gHyp_system_xparse ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: xparse ( file )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
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
      *pFile;

    char
      *pFileSpec=NULL,
      fileSpec[MAX_PATH_SIZE+1],
      *pStream,
      *pAnchor,
      *pEndOfStream ;
    
    FILE
      *pp = NULL ;

    sLOGICAL
      isPureXML = TRUE,
      mayReturnChildLess = FALSE,
      isEndTag ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

   if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: xparse ( [file] )" ) ;
    
    pFile = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pFile, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pFile),
			   TRUE) ;
    pFileSpec = fileSpec ;
    pp = fopen ( pFileSpec, "r"
#ifdef AS_VMS
		 , "shr=get,put"
#endif
		 ) ;
    
    if ( !pp )
      gHyp_instance_warning ( pAI, STATUS_FILE, 
			      "Could not open file '%s'", pFileSpec ) ;
    else {
      gHyp_data_deleteValues ( gHyp_frame_createVariable ( pAI, pFrame, "xmlargs" ) ) ;
      gzStream[0] = '\0' ;
      pStream = gzStream ;
      pAnchor = pStream ;
      pEndOfStream = pStream ;
      while ( TRUE ) {
	pStream = gHyp_cgi_parseXML ( pStream,/* stream buffer */
				      pAnchor,/*  stream anchor */
				      &pEndOfStream, /* stream buffer end */
				      NULL, /* no stream data */
				      NULL, /* no value */
	  	  		      0,    /* no context */
				      0,    /* no ss */
				      FALSE,/* no vector */
				      FALSE,/* not SCR */
				      FALSE,/* not PRE */
				      FALSE,/* not TXT */
				      NULL, /* no pXML */
				      NULL, /* no pParentXML */
				      &isEndTag,
				      pFrame,
				      NULL, /* No pTV */
				      pp,
				      pAI,
				      TYPE_LIST,
				      isPureXML,
				      mayReturnChildLess) ;
	if ( pStream == NULL ) {
	  /*
	  gHyp_instance_warning ( pAI, STATUS_XML, 
			         "Failed to parse XML from file '%s'", pFileSpec ) ;
	  */
	  break ;
	}
	else if ( !*pStream || !pEndOfStream ) 
	  break ;

	pAnchor = pStream ;
      }

      fclose ( pp ) ;

    }
    
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_system_getenv ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: getenv ( string )
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
   *	string
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
      *pData,
      *pResult ;
    
    char
      *pValue,
      token[VALUE_SIZE+1],
      value[MAX_INPUT_LENGTH+1];

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: getenv ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   token, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    if ( ( pValue = getenv ( token ) ) != NULL ) {
      n = MIN ( strlen ( pValue ), MAX_INPUT_LENGTH ) ;
      strncpy ( value, pValue, n ) ;
      value[n] = '\0' ;
    }
    else {

      if (      strcmp ( token, "AUTOROUTER" ) == 0 )
	strcpy ( value, gzAUTOROUTER ) ;
      else if ( strcmp ( token, "AUTOHOST"   ) == 0 )
	strcpy ( value, gzAUTOHOST ) ;
      else if ( strcmp ( token, "AUTOFIFO"   ) == 0 )
	strcpy ( value, gzAUTOFIFO ) ;
      else if ( strcmp ( token, "AUTOBIN"    ) == 0 )
	strcpy ( value, gzAUTOBIN ) ;
      else if ( strcmp ( token, "AUTORUN"    ) == 0 )
	strcpy ( value, gzAUTORUN ) ;
      else if ( strcmp ( token, "AUTOSPOOL"    ) == 0 )
	strcpy ( value, gzAUTOSPOOL ) ;
      else if ( strcmp ( token, "AUTOLOG"    ) == 0 )
	strcpy ( value, gzAUTOLOG ) ;
      else {
        gHyp_instance_warning(pAI, STATUS_UNDEFINED, 
	    "No value for environment variable %s found",token);
        strcpy ( value, "" ) ;
      }

    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, value ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_system_setenv ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: setenv ( string )
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
   *	TRUE or FALSE
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData ;
    
    char
      *pToken,
      *pValue,
#if defined ( AS_SOLARIS ) || defined ( AS_HPUX ) || ( AS_WINDOWS )
      tokenValue[VALUE_SIZE*2+1],
#endif
      token[VALUE_SIZE+1],
      value[VALUE_SIZE+1];

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: setenv ( string, string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   value, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pValue = value ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   token, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pToken = token ;

#if defined ( AS_SOLARIS ) || defined ( AS_HPUX ) || defined ( AS_WINDOWS )
    sprintf ( tokenValue, "%s=%s", pToken, pValue ) ;
#ifdef AS_WINDOWS
    if ( !_putenv ( tokenValue ) != -1 )
#else
      if ( !putenv ( tokenValue ) != -1 ) 
#endif
#else
    if ( !setenv ( pToken, pValue, 1 ) != -1 )
#endif
      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, 
			      "Failed to set environment variable '%s'",
			      token ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_system_unsetenv ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: unsetenv ( string )
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
   *	TRUE or FALSE
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
      *pData ;
    
    char
      *pToken,
#if defined ( AS_SOLARIS ) || defined ( AS_HPUX ) || defined ( AS_WINDOWS )
      tokenValue[VALUE_SIZE*2+1],
#endif
      token[VALUE_SIZE+1] ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: unsetenv ( string " ) ;


    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   token, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pToken = token ;

#if defined ( AS_SOLARIS ) || defined ( AS_HPUX ) || defined ( AS_WINDOWS )
    sprintf ( tokenValue, "%s=", pToken ) ;
#ifdef AS_WINDOWS
    _putenv ( tokenValue ) ; 
#else
    putenv ( tokenValue ) ; 
#endif
#else
    unsetenv ( token ) ;
#endif
    gHyp_instance_pushSTATUS ( pAI, pStack  ) ;
  }
}


void gHyp_system_setheap ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: setheap ( multiplier )
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
      *pData;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: setheap ( multiplier )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    guHeapMultiplier = gHyp_data_getInt ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    
    if ( guHeapMultiplier > 4 ) {

      guHeapMultiplier = 4 ;
      gHyp_instance_warning ( pAI,STATUS_ARGUMENT,
	"Set heap multiplier cannot exceed 4" ) ;
    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }
}