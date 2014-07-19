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
 *   $Log: function.c,v $
 *   Revision 1.55  2010-04-23 05:18:22  bergsma
 *   Increased buffer size for URLencode.
 *
 *   Revision 1.54  2009-06-12 05:04:22  bergsma
 *   HS 385 Final Checkin and TAG - Added pid() function
 *
 *   Revision 1.53  2008-07-01 23:48:54  bergsma
 *   When doing 'toexternal', make room for VALUE_SIZE*4
 *
 *   Revision 1.52  2008-02-12 23:10:29  bergsma
 *   vt2html numCols and numRows are variables now,.
 *
 *   Revision 1.51  2007-10-27 01:55:12  bergsma
 *   typo in round() function
 *
 *   Revision 1.50  2007-09-03 06:23:34  bergsma
 *   Allow vt2html support different row and col counts
 *
 *   Revision 1.49  2007-06-20 21:09:24  bergsma
 *   Use popRdata, not popRvalue, when subsequently doing a data_getNext
 *
 *   Revision 1.48  2007-05-02 20:34:01  bergsma
 *   Fix parseurl function.  Improve various print/debug/log statements.
 *   Fix problem with chunked data transfers.
 *
 *   Revision 1.47  2007-03-27 00:42:30  bergsma
 *   Newline at eof.
 *
 *   Revision 1.46  2007-03-26 21:16:18  bergsma
 *   Added parseurl function().
 *
 *   Revision 1.45  2007-03-21 02:21:31  bergsma
 *   Added round() function.
 *
 *   Revision 1.44  2007-03-19 05:32:08  bergsma
 *   New functions: min, max, pow, hypot, abs, acos, asin, atan,  ceil, cos,
 *    floor, log10, logN, sin, sqrt, tan, exp printf, sprintf, fprintf
 *
 *   Revision 1.43  2007-03-15 01:46:36  bergsma
 *   New functions.
 *
 *   Revision 1.42  2006-07-17 16:54:58  bergsma
 *   When strtok uses an explicit delimeter, do not consider single and
 *   double quotes from the tokens....
 *
 *   Revision 1.41  2006/01/16 18:56:36  bergsma
 *   HS 3.6.6
 *   1. Save query timeout events.  Don't let queries repeat indefinitely.
 *   2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 *   Revision 1.40  2006/01/06 23:40:39  bergsma
 *   *** empty log message ***
 *
 *   Revision 1.39  2005/12/17 20:39:49  bergsma
 *   no message
 *
 *   Revision 1.38  2005/12/10 00:30:30  bergsma
 *   HS 3.6.5
 *
 *   Revision 1.37  2005/11/06 16:32:29  bergsma
 *   Initialize variable last_character to avoid compiler warnings
 *
 *   Revision 1.36  2005/10/25 16:59:36  bergsma
 *   Compile warnings
 *
 *   Revision 1.35  2005/10/25 16:39:02  bergsma
 *   VT TAB bug.
 *
 *   Revision 1.34  2005/10/20 00:32:37  bergsma
 *   strlen and trim should woirk with empty strings.
 *
 *   Revision 1.33  2005/10/15 21:44:51  bergsma
 *   Problem with 80/132 switching
 *
 *   Revision 1.32  2005/09/25 20:10:40  bergsma
 *   Up to V3.6.3
 *   Add opcert code.
 *   Changes to function VT2HTML
 *   Bug in EAGAIN waiting for UNIX fifo's or network sockets
 *   AUTOROUTER loigical must be uppercase on VMS
 *
 *   Revision 1.31  2005/09/23 17:11:17  bergsma
 *   no message
 *
 *   Revision 1.30  2005/09/17 17:10:10  bergsma
 *   *** empty log message ***
 *
 *   Revision 1.29  2005/09/15 22:01:59  bergsma
 *   *** empty log message ***
 *
 *   Revision 1.28  2005/09/08 12:51:39  bergsma
 *   vthtml fixes
 *
 *   Revision 1.27  2005/09/06 10:18:08  bergsma
 *   Fixed pack() function
 *
 *   Revision 1.26  2005/09/06 08:52:05  bergsma
 *   Updated vt2html() function.,
 *   Added pack() function
 *
 *   Revision 1.25  2005/09/02 04:35:54  bergsma
 *   no message
 *
 *   Revision 1.24  2005/08/31 07:09:50  bergsma
 *   no message
 *
 *   Revision 1.23  2005/08/30 04:23:19  bergsma
 *   no message
 *
 *   Revision 1.22  2005/08/24 15:01:24  bergsma
 *   no message
 *
 *   Revision 1.21  2005/08/24 02:10:55  bergsma
 *   3.2.0 to 3.3.0
 *
 *   Revision 1.20  2005/08/21 17:58:35  bergsma
 *   no message
 *
 *   Revision 1.19  2005/08/17 01:46:58  bergsma
 *   no message
 *
 *   Revision 1.18  2005/08/14 16:22:52  bergsma
 *   no message
 *
 *   Revision 1.17  2005/08/12 01:16:16  bergsma
 *   no message
 *
 *   Revision 1.16  2005/08/03 14:00:19  bergsma
 *   no message
 *
 *   Revision 1.15  2005/07/23 22:32:18  bergsma
 *   Added  vt2html
 *
 *   Revision 1.14  2005/06/12 16:46:21  bergsma
 *   HS 3.6.1
 *
 *   Revision 1.13  2005/05/10 17:42:54  bergsma
 *   urlencode and urldecode
 *
 *   Revision 1.12  2005/05/10 17:38:45  bergsma
 *   Names are urlEncode and urlDecode
 *
 *   Revision 1.11  2005/05/10 17:32:38  bergsma
 *   Added URLencode and URLdecode functions
 *
 *   Revision 1.10  2005/04/13 13:45:54  bergsma
 *   HS 3.5.6
 *   Added sql_toexternal.
 *   Fixed handling of strings ending with bs (odd/even number of backslashes)
 *   Better handling of exception condition.
 *
 *   Revision 1.9  2004/10/16 04:42:19  bergsma
 *   Allow third argument (length) in strext to be optional.
 *   Fix bug in strok which returned extra "" element for whitespace.
 *
 *   Revision 1.8  2004/07/01 02:02:34  bergsma
 *   Added 'specialChars' argument to the toexternal() function.
 *
 *   Revision 1.7  2004/04/29 08:30:45  bergsma
 *   New functions encode/decode.
 *
 *   Revision 1.6  2003/04/23 22:49:56  bergsma
 *   In strtok(), the setToken() was replaced with setStr2
 *
 *   Revision 1.5  2003/04/04 16:40:27  bergsma
 *   Add the strip function.
 *
 *   Revision 1.4  2003/01/13 12:30:39  bergsma
 *   Sender(0) was returing concept@host@host2 when sender was another host
 *
 *   Revision 1.3  2003/01/12 14:05:24  bergsma
 *   V3.1.0
 *   1. Incorportate instance_pushSTATUS in functions
 *   2. strlen(args) function returns same length as puts (args) ;
 *   3. trim, tolower and toupper only affect strings
 *
 *   Revision 1.2  2002/11/14 01:40:45  bergsma
 *   Removed ^M occurances and added Modification section.
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/


void gHyp_function_alarm ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: alarm( seconds )
   *	Returns 1 if alarm seconds is valid, otherwise returns 0.
   *	Does not actually perform the 'C' alarm() function, but 
   *	sets the alarm event in the instance object.
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
      *pData ;
	
    
    int
      seconds,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: status = alarm ( seconds )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    seconds = gHyp_data_getInt ( pData,
				 gHyp_data_getSubScript ( pData),
				 TRUE ) ;

    if ( seconds < 0 || seconds > MAX_ALARM_SECONDS ) {
      gHyp_instance_warning ( pAI,  STATUS_BOUNDS, 
			      "Invalid alarm seconds, not between %d and %d",
			      0, MAX_ALARM_SECONDS ) ;
    }
    else {
      gHyp_instance_setAlarm ( pAI, seconds ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_function_lifetime ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: lifetime ( seconds )
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
      *pData ;
    
    int
      seconds,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: lifetime ( seconds )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    seconds = gHyp_data_getInt ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    
    if ( seconds < 0 || seconds > MAX_LIFETIME_SECONDS ) {
      gHyp_instance_warning ( pAI,STATUS_BOUNDS, "Invalid lifetime seconds" ) ;
    }
    else {
      gHyp_instance_setDeath ( pAI, seconds ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_function_method ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: method()
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

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: method()" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( 	pResult, 
    			gHyp_aimsg_method ( 
			gHyp_instance_currentMsg ( pAI ) ) ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_mode ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: mode()
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
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: mode()" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr (	pResult, 
    			gHyp_aimsg_mode ( 
			gHyp_instance_currentMsg ( pAI ) ) ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_function_tid ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: tid()
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
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: tid()" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr (	pResult, 
    			gHyp_aimsg_getTID ( 
			gHyp_instance_currentMsg ( pAI ) ) ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_function_sid ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sid()
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
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: sid()" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr (	pResult, 
    			gHyp_aimsg_getTS ( 
			gHyp_instance_currentMsg ( pAI ) ) ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_pid ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: pid()
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
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: pid()" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setInt (	pResult, giPID ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_function_self ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: self ( [component] ) ;
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
      argValue,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      target[OBJECT_SIZE+1],
      instance[INSTANCE_SIZE+1],
      concept[OBJECT_SIZE+1],
      parent[OBJECT_SIZE+1],
      root[OBJECT_SIZE+1],
      host[HOST_SIZE+1] ;
		
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: self ( [component] )" ) ;

    if ( argCount == 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      argValue = gHyp_data_getInt ( pData,gHyp_data_getSubScript(pData),TRUE) ;
    }	
    else
      argValue = 0 ;

    pResult = gHyp_data_new ( NULL ) ;
    
    strcpy ( target, gHyp_instance_getTargetPath(pAI) ) ;

    gHyp_util_breakTarget ( target, instance, concept, parent, root, host ) ;

    switch ( argValue ) {
      case 3:
	gHyp_data_setStr ( pResult, gzLocalHost ) ;
	break ;
      case 2:
	sprintf ( target, "%s%s",  concept, root ) ;
	gHyp_data_setStr ( pResult, target ) ;
	break ;
      case 1:
	gHyp_data_setStr ( pResult, instance ) ;
	break ;
      case 0:
	strcat ( target, "@" ) ;
	strcat ( target, gzLocalHost ) ;
	gHyp_data_setStr ( pResult, target ) ;
	break ;
      default:
	gHyp_data_setNull (	pResult ) ;
        gHyp_instance_warning ( pAI, STATUS_BOUNDS, 
				"Invalid self component index" ) ;
	break ;
    }	
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_sender ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sender( [component] )
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
      argValue,
      argCount = gHyp_parse_argCount ( pParse ) ;   

    char
      target[OBJECT_SIZE+1],
      instance[INSTANCE_SIZE+1],
      concept[OBJECT_SIZE+1],
      parent[OBJECT_SIZE+1],
      root[OBJECT_SIZE+1],
      host[HOST_SIZE+1] ;
		
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: sender ( [component] )" ) ;

    if ( argCount == 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      argValue = gHyp_data_getInt ( pData,gHyp_data_getSubScript(pData),TRUE) ;
    }	
    else
      argValue = 0 ;

    pResult = gHyp_data_new ( NULL ) ;
    
    gHyp_aimsg_getSenderAddress ( gHyp_instance_currentMsg ( pAI ), target ) ;

    gHyp_util_breakTarget ( target, instance, concept, parent, root, host ) ;

   switch ( argValue ) {
      case 3:
	gHyp_data_setStr ( pResult, host ) ;
	break ;
      case 2:
	sprintf ( target, "%s%s",  concept, root ) ;
	gHyp_data_setStr ( pResult, target ) ;
	break ;
      case 1:
	gHyp_data_setStr ( pResult, instance ) ;
	break ;
      case 0:
	gHyp_data_setStr ( pResult, target ) ;
	break ;
      default:
	gHyp_data_setNull (	pResult ) ;
        gHyp_instance_warning ( pAI, STATUS_BOUNDS, 
				"Invalid sender component index" ) ;
	break ;
    }	
    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_function_strext ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: strext ( str, off, len )
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
      strVal[VALUE_SIZE+1],
      *pStr,
      *pSubStr ;
    
    int
      stringLen,
      offset,
      length=0,
      argCount = gHyp_parse_argCount ( pParse ) ;
		
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
    
    if ( argCount != 3 && argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: strext ( string, offset [, length] )" ) ;

    if ( argCount == 3 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      length = gHyp_data_getInt ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    }

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    offset = gHyp_data_getInt ( pData,gHyp_data_getSubScript(pData), TRUE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    stringLen = gHyp_data_getStr ( pData, 
				   strVal, 
				   VALUE_SIZE, 
				   gHyp_data_getSubScript(pData),
				   TRUE ) ;
    pStr = strVal ;

    if ( argCount == 2 ) length = stringLen - offset ;

    /* Be forgiving about going out of bounds in the length. */
    pResult = gHyp_data_new ( NULL ) ;
    
    if (	offset >= 0 &&
    		offset < stringLen &&
		length > 0 &&
		offset + length <= stringLen ) {
		
      pSubStr = (char*) AllocMemory ( length + 1 ) ;
      pStr += offset ;
      memcpy ( pSubStr, pStr, length ) ;
      pSubStr[length] = '\0' ;
      gHyp_data_setStr_n ( pResult, pSubStr, length ) ;
      ReleaseMemory ( pSubStr ) ;
    }
    else {
      if ( offset < 0 || offset >= stringLen ) 
	gHyp_instance_warning ( pAI, STATUS_BOUNDS, 
				"Invalid offset (%d) specified in strext().",
				offset);
      else
	gHyp_instance_warning ( pAI, STATUS_BOUNDS, 
				"Invalid length (%d) specified in strext().",
				length);
      gHyp_data_setNull ( pResult ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_strlen ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: strlen ( string )
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
      *pValue,
      *pResult ;
    
    int
      n,
      N,
      ss,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      strVal[VALUE_SIZE+1] ;

    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: strlen ( string )" ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;

    isVector = (gHyp_data_getDataType(pData) > TYPE_STRING ) ;
    pValue = NULL ;
    N = 0 ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {

      n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
      N += n ;
    }
    if ( context== -2 && ss != -1 )
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in strlen()",ss) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setInt ( pResult, N ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_pack ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: pack ( string )
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      N,
      ss,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      *pStr,
      strVal[VALUE_SIZE+1],
      strVal2[VALUE_SIZE+1];

    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: pack ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

    pResult = gHyp_data_new ( "_pack_" ) ;

    isVector = (gHyp_data_getDataType(pData) > TYPE_STRING ) ;
    pValue = NULL ;
    N = 0 ;
    pStr = strVal2 ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {

      n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;

      if ( N+n < VALUE_SIZE ) {

	/* Pack it */
	memcpy ( (void*) pStr , strVal, n ) ;
	pStr += n ;
	N+=n ;
	n = 0 ;
      }
      else {

	/* Pack string is full. Flush and start a new string */
	pValue2 = gHyp_data_new ( NULL ) ;
	gHyp_data_setStr_n ( pValue2, strVal2, N ) ;
	gHyp_data_append ( pResult, pValue2 ) ;
	N = 0 ;
	pStr = strVal2 ;
	memcpy ( (void*) pStr , strVal, n ) ;
	pStr += n ;
	N+=n ;
	n = 0 ;
      }
      
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in pack()",ss) ;
    }

    if ( N > 0 ) {
      pValue2 = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr_n ( pValue2, strVal2, N ) ;
      gHyp_data_append ( pResult, pValue2 ) ;
    }


    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_function_strloc ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: strloc ( string, substring )
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
      *pData1,
      *pData2,
      *pResult ;
    
    char
      strVal1[VALUE_SIZE+1],
      strVal2[VALUE_SIZE+1],
      *pStr,
      *pStr1,
      *pStr2 ;

    int
      n,
      loc,
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: strloc ( string, substring )" ) ;

    pData2 = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData2, 
			   strVal2, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData2),
			   TRUE ) ;
    pStr2 = strVal2 ;

    pData1 = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData1, 
			   strVal1, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData1),
			   TRUE ) ;
    pStr1 = strVal1 ;

    pStr = strstr ( pStr1, pStr2 ) ;
    loc = pStr ? pStr - pStr1 : n ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setInt ( pResult , loc ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_function_strtok ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: strtok ( string )
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
      *pValue,
      *pResult ;
    
    char
      value[VALUE_SIZE+1],
      separator[VALUE_SIZE+1],
      strBuf[MAX_INPUT_LENGTH+1],
      *pStr,
      *pBuf,
      *pValStart,
      *pValEnd,
      *pBufEnd ;    

    int
      n,
      valueLen,
      ss,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      doLiterally=FALSE,
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount < 1 || argCount > 3 ) 
      gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: strtok ( string, [separator,[doLitterally]])" ) ;

    if ( argCount > 2 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      doLiterally = gHyp_data_getBool ( pData,
					  gHyp_data_getSubScript ( pData  ),
					  TRUE ) ;
    }

    if ( argCount > 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData, 
			     separator, 
			     VALUE_SIZE, 
			     gHyp_data_getSubScript(pData),
			     TRUE ) ;
    }
    else
      strcpy ( separator, " \t" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

    /* Construct the string from the elements of the list */
    pBuf = strBuf ;
    pBufEnd = pBuf + MAX_INPUT_LENGTH ;    
    pResult = NULL ;
    isVector = (gHyp_data_getDataType(pData) > TYPE_STRING ) ;
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
      
      if ( (pBuf + valueLen) > pBufEnd ) 
	gHyp_instance_error ( pAI, STATUS_IO,
			      "Input string longer than %d characters \n",
			      MAX_INPUT_LENGTH ) ;
      sprintf ( pBuf, "%s", pStr ) ;
      pBuf += valueLen ;   
    }
    if ( context== -2 && ss != -1 ) 
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in strtok()",ss);
    *pBuf = '\0' ;

    /* Extract token values and strings. */
    pResult = gHyp_data_new ( "_strtok_" ) ;
    pBuf = strBuf ;
    pBufEnd = pBuf + strlen ( pBuf ) ;
    pValStart = pBuf ;      
    do {
    
      /* Scan for first printable character */
      pValStart += strspn ( pValStart, separator ) ;
    
      if ( !doLiterally && *pValStart == '"' ) {
      
	/* Found start of LITERAL */
	pValStart++ ;
      
	/* Look for end of LITERAL */
	pValEnd = strchr ( pValStart, '"' ) ;
	if ( pValEnd == NULL ) 
	  pValEnd = pBufEnd ;
	else
	  *pValEnd = '\0' ;
      
	/* Create value element */
	pValue = gHyp_data_new ( NULL ) ;
	if ( pValStart < pValEnd )
	  gHyp_data_setStr ( pValue, pValStart ) ;
	else
	  gHyp_data_setNull ( pValue ) ;
	
      }
      else if ( !doLiterally && *pValStart == '\'' ) {
	
	/* Found start of single quoted variable */
	pValStart++ ;
	
	/* Look for end of variable */
	pValEnd = strchr ( pValStart, '\'' ) ;
	if ( pValEnd == NULL ) 
	  pValEnd = pBufEnd ;
	else
	  *pValEnd = '\0' ;
	
	/* Create value element */
	pValue = gHyp_data_new ( NULL ) ;
	if ( pValStart < pValEnd )
	  gHyp_data_setReference ( pValue, pValStart, NULL ) ;
	else
	  gHyp_data_setNull ( pValue ) ;
	
      }
      else if ( pValStart < pBufEnd ) {
	
	/* Found a CONSTANT. Find end. */
	pValEnd = pValStart + strcspn ( pValStart, separator ) ;
	*pValEnd = '\0' ;
	
	/* CONSTANT OR IDENTIFIER */
	pValue = gHyp_data_new ( NULL ) ;
	if ( pValStart < pValEnd )
	  gHyp_data_setStr_n ( pValue, pValStart, pValEnd-pValStart ) ;
	else
	  gHyp_data_setNull ( pValue ) ;
      }
      else
	break ;
    
      /* Append value to list. */
      gHyp_data_append ( pResult, pValue ) ;
      
      /* Advance past end of value */
      pValStart = pValEnd + 1 ;
    } 
    while ( pValStart < pBufEnd ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_timeout ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: timeout ( seconds )
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
      *pData ;
    
    int
      seconds,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: timeout ( seconds )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    seconds = gHyp_data_getInt ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    
    if ( seconds < 0 || seconds > MAX_TIMEOUT_SECONDS ) 
      gHyp_instance_warning ( pAI, STATUS_BOUNDS, "Invalid timeout seconds" ) ;
    else
      gHyp_instance_initTimeOut ( pAI, seconds ) ;
    
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_function_tolower ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: tolower()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      strVal[VALUE_SIZE+1],
      strVal2[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: tolower ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_tolower_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {

      if ( isVector ) {
      
	pValue2 = gHyp_data_new ( NULL ) ;
	gHyp_data_newConstant ( pValue2, 
				gHyp_data_dataType ( pValue ), 
				pValue, 
				context ) ;
      }
      else {
        n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
        memcpy ( strVal2, pStr, n ) ;
        strVal2[n] = '\0' ;
        gHyp_util_lowerCase ( strVal2, n ) ;
        gHyp_data_setStr_n ( pValue2, strVal2, n ) ;
      }
      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Subscript '%s' is out of bounds in tolower()",ss);
    }

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_toupper ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: toupper()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      ss,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      strVal[VALUE_SIZE+1],
      strVal2[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: toupper ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_toupper_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
      if ( isVector ) {
      
	pValue2 = gHyp_data_new ( NULL ) ;
	gHyp_data_newConstant ( pValue2, 
				gHyp_data_dataType ( pValue ), 
				pValue, 
				context ) ;
      }
      else {
        n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
        memcpy ( strVal2, pStr, n ) ;
        strVal2[n] = '\0' ;
        gHyp_util_upperCase ( strVal2, n ) ;
        gHyp_data_setStr_n ( pValue2, strVal2, n ) ;
      }

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ; 
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in toupper()",ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_strip ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: strip()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      ss,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      strVal[VALUE_SIZE+1],
      strVal2[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: strip ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_toalpha_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
      if ( isVector ) {
      
	pValue2 = gHyp_data_new ( NULL ) ;
	gHyp_data_newConstant ( pValue2, 
				gHyp_data_dataType ( pValue ), 
				pValue, 
				context ) ;
      }
      else {
        n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
        memcpy ( strVal2, pStr, n ) ;
        strVal2[n] = '\0' ;
        gHyp_util_strip ( strVal2, n ) ;
        gHyp_data_setStr ( pValue2, strVal2 ) ;
      }

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ; 
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in strip()",ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_function_toexternal(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: toexternal()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      specialChars[VALUE_SIZE+1],
      strVal[VALUE_SIZE+1],
      strVal2[VALUE_SIZE*4+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 && argCount != 2 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: toexternal ( string [,specialChars] )" ) ;

    if ( argCount == 2 ) {

      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      gHyp_data_getStr ( pData, 
			 specialChars, 
			 VALUE_SIZE, 
			 gHyp_data_getSubScript(pData),
			 TRUE ) ;
    }
    else
      specialChars[0] = '\0' ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_toexternal_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
      if ( isVector ) {
      
	pValue2 = gHyp_data_new ( NULL ) ;
	gHyp_data_newConstant ( pValue2, 
				gHyp_data_dataType ( pValue ), 
				pValue, 
				context ) ;
      }
      else {
        n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
        n = gHyp_util_unparseString ( strVal2, pStr, n, VALUE_SIZE*4, FALSE, FALSE, FALSE,specialChars ) ;
        gHyp_data_setStr_n ( pValue2, strVal2, n ) ;
      }

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in toexternal()",
			    ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_tointernal(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: tointernal()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      strVal[VALUE_SIZE+1],
      strVal2[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: tointernal ( string )" ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_tointernal_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
      if ( isVector ) {
      
	pValue2 = gHyp_data_new ( NULL ) ;
	gHyp_data_newConstant ( pValue2, 
				gHyp_data_dataType ( pValue ), 
				pValue, 
				context ) ;
      }
      else {
        n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
        memcpy ( strVal2, pStr, n ) ;
        strVal2[n] = '\0' ;
        n = gHyp_util_parseString ( strVal2 ) ;
        gHyp_data_setStr_n ( pValue2, strVal2, n ) ;
      }

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in tointernal()",
			    ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_trim ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: trim ()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      context,
      n,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      strVal[VALUE_SIZE+1],
      strVal2[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: trim ( string )" ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_trim_", TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
       if ( isVector ) {
      
	pValue2 = gHyp_data_new ( NULL ) ;
	gHyp_data_newConstant ( pValue2, 
				gHyp_data_dataType ( pValue ), 
				pValue, 
				context ) ;
      }
      else {

        n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
        memcpy ( strVal2, pStr, n ) ;
        strVal2[n] = '\0' ;
        gHyp_util_trim ( strVal2 ) ;
        gHyp_data_setStr ( pValue2, strVal2 ) ;
      }

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript is out of bounds in trim()",ss) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_function_decode(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: decode()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      encrypted[VALUE_SIZE+1],
      decrypted[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: decode ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_decode_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
        n = gHyp_data_getStr ( pValue, 
			     encrypted, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = encrypted ;
        pValue2 = gHyp_data_new ( NULL ) ;
	n = gHyp_util_base64Decode ( encrypted, n, decrypted ) ;
        gHyp_data_setStr_n ( pValue2, decrypted, n ) ;

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in decode()",
			    ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_function_encode(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: encode()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      encrypted[VALUE_SIZE+1],
      decrypted[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: encode ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_encode_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
        n = gHyp_data_getStr ( pValue, 
			     decrypted, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = encrypted ;
        pValue2 = gHyp_data_new ( NULL ) ;
	n = gHyp_util_base64Encode ( decrypted, n, encrypted ) ;
        gHyp_data_setStr_n ( pValue2, encrypted, n ) ;

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in encode()",
			    ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_function_urlDecode(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: urldecode()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      encrypted[VALUE_SIZE+1],
      decrypted[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: urldecode ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_urldecode_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
        n = gHyp_data_getStr ( pValue, 
			     encrypted, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = encrypted ;
        pValue2 = gHyp_data_new ( NULL ) ;
	n = gHyp_util_urlDecode ( encrypted, n, decrypted ) ;
        gHyp_data_setStr_n ( pValue2, decrypted, n ) ;

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in urldecode()",
			    ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_function_urlEncode(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: urlencode()
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      n,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      encrypted[VALUE_SIZE+1],
      decrypted[INTERNAL_VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: urlencode ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_urlencode_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
        n = gHyp_data_getStr ( pValue, 
			     decrypted, 
			     INTERNAL_VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = encrypted ;
        pValue2 = gHyp_data_new ( NULL ) ;
	n = gHyp_util_urlEncode ( decrypted, n, encrypted ) ;
        gHyp_data_setStr_n ( pValue2, encrypted, n ) ;

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in urlencode()",
			    ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

#define XPECT_CHARACTER 1
#define XPECT_ESCAPE_CODE 2
#define XPECT_CONTROL_CODE 3
#define XPECT_FINAL_G0 4
#define XPECT_FINAL_G1 5
#define XPECT_FINAL_G2 6
#define XPECT_FINAL_G3 7
#define XPECT_LINE_ATTR 8
#define XPECT_END_DCS 9


#define ACTION_NONE 0
#define ACTION_BELL 1
#define ACTION_BACKSPACE 2
#define ACTION_TAB 3
#define ACTION_LINEFEED 4
#define ACTION_CARRIAGE_RETURN 5
#define ACTION_GL_G0 6
#define ACTION_GL_G1 7
#define ACTION_GL_G2 8
#define ACTION_GL_G3 9
#define ACTION_GR_G0 10
#define ACTION_GR_G1 11
#define ACTION_GR_G2 12
#define ACTION_GR_G3 13
#define ACTION_GL_G2_SHIFT 14
#define ACTION_GL_G3_SHIFT 15
#define ACTION_ASCII_TO_G0 16
#define ACTION_ASCII_TO_G1 17
#define ACTION_ASCII_TO_G2 18
#define ACTION_ASCII_TO_G3 19
#define ACTION_LINEDRAW_TO_G0 20 
#define ACTION_LINEDRAW_TO_G1 21
#define ACTION_LINEDRAW_TO_G2 22
#define ACTION_LINEDRAW_TO_G3 23
#define ACTION_QUICKCHART_TO_G0 24 
#define ACTION_QUICKCHART_TO_G1 25
#define ACTION_QUICKCHART_TO_G2 26 
#define ACTION_QUICKCHART_TO_G3 27
#define ACTION_DOUBLE_TOP 28 
#define ACTION_DOUBLE_BOTTOM 29 
#define ACTION_SINGLE_WIDTH 30
#define ACTION_DOUBLE_WIDTH 31
#define ACTION_SAVE_STATE 32 
#define ACTION_RESTORE_STATE 33
#define ACTION_CURSOR_UP 34
#define ACTION_CURSOR_DOWN 35 
#define ACTION_CURSOR_RIGHT 36 
#define ACTION_CURSOR_LEFT 37 
#define ACTION_CURSOR_MOVETO 38 
#define ACTION_HORIZ_TAB_SET 39 
#define ACTION_CLEAR_TAB 40
#define ACTION_INDEX 41
#define ACTION_REVERSE_INDEX 42
#define ACTION_NEXT_LINE 43
#define ACTION_END_DCS 44
#define ACTION_TERMINAL_SET_MODE 45 
#define ACTION_TERMINAL_RESET_MODE 46
#define ACTION_RENDITION 47
#define ACTION_SCROLL_REGION 48
#define ACTION_CHARACTER 49
#define ACTION_ERASE_IN_CHARACTER 50
#define ACTION_ERASE_IN_LINE 51
#define ACTION_ERASE_IN_DISPLAY 52
#define ACTION_ENTER_ANSI_MODE 53
#define ACTION_EXIT_ALT_KEYPAD 54
#define ACTION_DEVICE_REQUEST 55
#define ACTION_DEVICE_REQUEST_VT52 56
#define ACTION_EEEE 57
#define ACTION_EMIT 58
#define ACTION_CHARACTER_WRAP 59

#define VT_NUL	'\0'
#define VT_SOL	'\1'
#define VT_STX	'\2'
#define VT_ETX	'\3'
#define VT_EOT	'\4'
#define VT_ENQ	'\5'
#define VT_ACK	'\6'
#define VT_BEL	'\7'
#define VT_BS	'\10'
#define VT_HT	'\11'
#define VT_LF	'\12'
#define VT_VT	'\13'
#define VT_FF	'\14'
#define VT_CR	'\15'
#define VT_SO	'\16'
#define VT_SI	'\17'
#define VT_DLE	'\20'
#define VT_DC1	'\21'
#define VT_DC2	'\22'
#define VT_DC3	'\23'
#define VT_DC4	'\24'
#define VT_NAK	'\25'
#define VT_SYN	'\26'
#define VT_ETB	'\27'
#define VT_CAN	'\30'
#define VT_EM	'\31'
#define VT_SUB	'\32'
#define VT_ESC	'\33'
#define VT_FS	'\34'
#define VT_GS	'\35'
#define VT_RS	'\36'
#define VT_US	'\37'
#define VT_DEL	'\177'
#define VT_CSI	'\233'

#define VT_TN_IAC '\377'
#define VT_TN_DONT '\376'
#define VT_TN_DO '\375'
#define VT_TN_WONT '\374'
#define VT_TN_WILL '\373'
#define VT_TN_SUBNEGOTIATE '\372'
#define VT_TN_GOAHEAD '\371'
#define VT_TN_ERASE_LINE '\370'
#define VT_TN_ERASE_CHAR '\367'
#define VT_TN_AREYOUTHERE '\366'
#define VT_TN_ABORT '\365'
#define VT_TN_INTERRUPT '\364'
#define VT_TN_BREAK '\363'
#define VT_TN_DATAMARK '\362'
#define VT_TN_NOP '\361'
#define VT_TN_ENDSUBNEGOTIATE '\360'

#define VT_TN_OPTION_ECHO '\001'
#define VT_TN_OPTION_SUPPRESS_GOAHEAD '\003'
#define VT_TN_OPTION_STATUS '\005'
#define VT_TN_OPTION_TIMING_MARK '\006'
#define VT_TN_OPTION_TERMINAL_TYPE '\030'
#define VT_TN_OPTION_WINDOW_SIZE '\037'
#define VT_TN_OPTION_TERMINAL_SPEED '\040'
#define VT_TN_OPTION_REMOTE_FLOW '\041'
#define VT_TN_OPTION_LINEMODE '\042'
#define VT_TN_OPTION_ENV '\044'

#define RENDITION_STANDARD 0
#define RENDITION_BOLD 1
#define RENDITION_REVERSE 2
#define RENDITION_UNDERSCORE 4
#define RENDITION_BLINKING 8
#define RENDITION_NOBOLD 16
#define RENDITION_NOREVERSE 32
#define RENDITION_NOUNDERSCORE 64
#define RENDITION_NOBLINKING 128

#define VT_GRAPHICS_ASCII 0
#define VT_GRAPHICS_LINEDRAW 1
#define VT_GRAPHICS_SUPPLEMENTAL 2
#define VT_GRAPHICS_QUICKCHART 3
#define VT_GRAPHICS_G0_ASCII 0
#define VT_GRAPHICS_G0_LINEDRAW 1
#define VT_GRAPHICS_G0_SUPPLEMENTAL 2
#define VT_GRAPHICS_G0_QUICKCHART 3

#define VT_GRAPHICS_G1_ASCII 0
#define VT_GRAPHICS_G1_LINEDRAW (1<<2)
#define VT_GRAPHICS_G1_SUPPLEMENTAL (2<<2)
#define VT_GRAPHICS_G1_QUICKCHART (3<<2)

#define VT_GRAPHICS_G2_ASCII 0
#define VT_GRAPHICS_G2_LINEDRAW (1<<4)
#define VT_GRAPHICS_G2_SUPPLEMENTAL (2<<4)
#define VT_GRAPHICS_G2_QUICKCHART (3<<4)

#define VT_GRAPHICS_G3_ASCII 0
#define VT_GRAPHICS_G3_LINEDRAW (1<<6)
#define VT_GRAPHICS_G3_SUPPLEMENTAL (2<<6)
#define VT_GRAPHICS_G3_QUICKCHART (3<<6)

#define VT_GRAPHICS_G0_MASK   (3)
#define VT_GRAPHICS_G1_MASK   (3<<2)
#define VT_GRAPHICS_G2_MASK   (3<<4)
#define VT_GRAPHICS_G3_MASK   (3<<6)

#define VT_GRAPHICS_DEFAULT (VT_GRAPHICS_G0_ASCII|VT_GRAPHICS_G1_LINEDRAW|VT_GRAPHICS_G2_SUPPLEMENTAL|VT_GRAPHICS_G3_QUICKCHART)

#define VT_FLAGS_GL_ASCII 0
#define VT_FLAGS_GL_LINEDRAW 1
#define VT_FLAGS_GL_SUPPLEMENTAL 2
#define VT_FLAGS_GL_QUICKCHART 3
#define VT_FLAGS_GL_MASK 3

#define VT_FLAGS_GR_ASCII 0
#define VT_FLAGS_GR_LINEDRAW 4
#define VT_FLAGS_GR_SUPPLEMENTAL 8
#define VT_FLAGS_GR_QUICKCHART 12
#define VT_FLAGS_GR_MASK 12

#define VT_FLAGS_G_MASK (VT_FLAGS_GL_MASK+VT_FLAGS_GR_MASK)

#define VT_FLAGS_TAB_STOP  16
#define VT_FLAGS_MODIFIED 32
#define VT_FLAGS_PAGED 64

#define VT_FLAGS_MASK (VT_FLAGS_TAB_STOP+VT_FLAGS_MODIFIED+VT_FLAGS_PAGED)

#define NUMERIC_ARG_COUNT 5
#define NUMERIC_ARG_WIDTH 12

#define VT_MAX_ROWS 24
#define VT_80_COL 80
#define VT_132_COL 132
#define VT_MAX_COLS VT_132_COL
#define VT_MAX_OFFSCROLL_ROWS VT_MAX_ROWS

#define VT_FLAGS2_ORIGIN 1    /* When set, terminal is in orgin mode */
#define VT_FLAGS2_WRAP 2      /* When set, characters are wrapped */
#define VT_FLAGS2_WRAP_PENDING 4   /* When set, wrap is pending. */

#define VT_FLAGS3_LOCKED  1	/* When set, keyboard is locked */
#define VT_FLAGS3_INSERT  2 /* When set, terminal inserts characters, otherwise replaces */
#define VT_FLAGS3_ECHO    4 /* When set, terminal echos characters locally */
#define VT_FLAGS3_NEWLINE 8 /* When set, [Return] sends CR/LF, otherwise just CR */
#define VT_FLAGS3_132_COL 16 /* When set, terminal is in 132 column mode, otherwise 80 */
#define VT_FLAGS3_SMOOTH_SCROLL 32  /* When set, smooth scroll, otherwise jump scroll */ 
#define VT_FLAGS3_REVERSE 64  /* When set, video is in reverse mode (white background!!) */

static void lHyp_function_emitAction ( sData *pResult,
				       int row,
				       int col,
				       int colspan,
				       sBYTE rendition,
				       sBYTE graphics,
				       sBYTE flags3,
				       char *token,
				       sLOGICAL doDebug )
{
      
  sData
    *pData,
    *pValue,
    *pAction ;

  /*
  char
    encrypted[VALUE_SIZE+1];

  int
      n ;
  */

  if (doDebug) gHyp_util_debug("Update cell (%u,%u)[%u] = '%s'", row,col,colspan,token);
  pAction = gHyp_data_new ( "updatecell" ) ;
  
  pData = gHyp_data_new ( "row") ;
  gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
  gHyp_data_setVectorRaw ( pData, &row, 0 ) ;
  gHyp_data_append ( pAction, pData ) ;

  pData = gHyp_data_new ( "col") ;
  gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
  gHyp_data_setVectorRaw ( pData, &col, 0 ) ;
  gHyp_data_append ( pAction, pData ) ;

  pData = gHyp_data_new ( "colspan") ;
  gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
  gHyp_data_setVectorRaw ( pData, &colspan, 0 ) ;
  gHyp_data_append ( pAction, pData ) ;

  pData = gHyp_data_new ( "rendition") ;
  gHyp_data_newVector ( pData, TYPE_UBYTE, 1, TRUE ) ;
  gHyp_data_setVectorRaw ( pData, &rendition, 0 ) ;
  gHyp_data_append ( pAction, pData ) ;

  pData = gHyp_data_new ( "flags3") ;
  gHyp_data_newVector ( pData, TYPE_UBYTE, 1, TRUE ) ;
  gHyp_data_setVectorRaw ( pData, &flags3, 0 ) ;
  gHyp_data_append ( pAction, pData ) ;

  pData = gHyp_data_new ( "graphics") ;
  gHyp_data_newVector ( pData, TYPE_UBYTE, 1, TRUE ) ;
  gHyp_data_setVectorRaw ( pData, &graphics, 0 ) ;
  gHyp_data_append ( pAction, pData ) ;

  /* Add the characters */
  pData = gHyp_data_new ( NULL ) ;
  gHyp_data_setVariable ( pData, "text", TYPE_STRING ) ;
  pValue = gHyp_data_new ( NULL ) ;
  /*
  n = gHyp_util_base64Encode ( token, strlen (token), encrypted ) ;
  gHyp_data_setStr_n ( pValue, encrypted, n ) ;
  */
  gHyp_data_setStr ( pValue, token ) ;
  gHyp_data_append ( pData, pValue ) ;

  gHyp_data_append ( pAction, pData ) ;
  
  gHyp_data_append ( pResult, pAction ) ;

}


static void lHyp_function_flushActions (   sData *pResult,
				   int numRows,
				   sBYTE  rowMap[VT_MAX_ROWS],
				   sBYTE  flags3,
				   int  numCols,
				   char  *pScreenCharacter[VT_MAX_ROWS],
				   sBYTE *pScreenRendition[VT_MAX_ROWS],
				   sBYTE *pScreenFlags[VT_MAX_ROWS],
				   sLOGICAL doDebug )
{

  int j,
      k,
      ir,
      ic,
      last_row=1,
      last_col=1,
      colspan ;
      
  sBYTE
    last_graphics=0,
    last_flags=0,
    last_rendition=0 ;

  char
    this_c,
    last_c,
    token[VALUE_SIZE+1],
    last_character=' ' ;

  sLOGICAL
    isModified,
    emit,
    allEqual,
    last_isGL=FALSE;

  { /* Start block */


      /* Generate the td elements of the row */

      /* 
       *  
       * The action list we create looks like this:
       *
       * list action = {
       *
       *   // Scroll down
       *   list scrolldown = {
       *    int startrow=1,
       *    int endrow=24
       *   },
       *   // Scroll up
       *   list scrollup = {
       *    int startrow=5,
       *    int endrow=6
       *   },
       *   // Update a single cell
       *   list updatecell {
       *     int row=5,
       *     int col=7,
       *     str bgcolor="#FF0080",
       *     str style="color:white",
       *     str text="X"
       *   },
       *   // Update a set of 10 cells in a row
       *   list updatecell {
       *     int row=24,
       *     int col=5,
       *     str style="color:black",
       *     int colspan=10,
       *     str text="Hello"
       *   },
       *   // DEC special graphics (line-draw), 10 cells in a row
       *   // creates TD attribute src="images/dsg109.jpg"
       *   list updatecell {
       *     int row=10,
       *     int col=1,
       *     int colspan=10,
       *     int src="dsg",
       *     str text="mmmmmmmmmm"
       *   },
       *   // Downloadable characters, 3 cells in a row.
       *   // creates TD attributes
       *   //  src="images/dc4t70.jpg"
       *   //  src="images/dc4t117.jpg"
       *   //  src="images/dc4t50.jpg"
       *   list updatecell {
       *     int row=8,
       *     int col=55,
       *     int src="dcs",
       *     int colspan=3,
       *     str text="Fu2"
       *   }
       *
       */

    /* Process the screen and produce output */
    emit = FALSE ;
    colspan = 0 ;
    allEqual = TRUE ;

    if ( doDebug ) gHyp_util_debug("Scanning %d rows, %d cols",numRows,numCols);
    for ( k=1;k<=numRows;k++ ) {

      ir = rowMap [ k-1 ] ;

      for ( j=1;j<=numCols;j++ ) {

	ic = j-1 ;

      	isModified = ( pScreenFlags[ir][ic] & VT_FLAGS_MODIFIED ) ;

	if ( isModified ) {

	  /* No longer modified,*/
	  pScreenFlags[ir][ic] &= ~VT_FLAGS_MODIFIED ;

	  /*if ( doDebug ) gHyp_util_debug("MC (%u,%u)='%c'",k,j,pScreenCharacter[ir][ic]);*/

	  if ( colspan > 0 ) {

	    /* Can the next character be bundled with the previous one? */

	    /* If its the start of a new column, then no */
	    /*if ( j == 1 && last_character != ' ' ) emit = TRUE ;*/
	    if ( j == 1 ) emit = TRUE ;

	    if ( !emit ) {

	      /* If the graphics set has changed then no */

	      if ( last_isGL ) {
	        if ( (last_flags&VT_FLAGS_GL_MASK) != (pScreenFlags[ir][ic]&VT_FLAGS_GL_MASK) )
		  emit = TRUE ;
	      }
	      else {
	        if ( (last_flags&VT_FLAGS_GR_MASK) != (pScreenFlags[ir][ic]&VT_FLAGS_GR_MASK) )
		  emit = TRUE ;
	      }
	    }

	    if ( !emit ) {

	      /* Same graphics set, but do they have the same rendition?
	       * Rendition only matters if the graphics set is ASCII.
	       */

	      if ( last_rendition == pScreenRendition[ir][ic] ) {
		
		/* If switching between whitespace and non-whitespace, emit */
		
		this_c = pScreenCharacter[ir][ic] ;
		last_c = token[colspan-1] ;

		/*if ( ( isspace ( this_c ) && !isspace ( last_c ) ) || 
		     ( !isspace ( this_c ) && isspace ( last_c ) ) )
		  emit = TRUE ;
		else {
		*/
		  token[colspan++] = this_c ;
		  token[colspan] = '\0' ;
		  if ( pScreenCharacter[ir][ic] != last_character ) allEqual = FALSE ;
		/*}*/
	      }
	      else {
		/* Emit action */
		emit = TRUE ;
	      }
	    }
	  }
	  else
	    /* Colspan == 0 */
	    emit = FALSE ;
	}
	else {
	  /* Next screen element not modified */
	  /*gHyp_util_debug("Cell (%u,%u) %c was NOT modified",k,j,pScreenCharacter[ir][ic]);*/
	  if ( colspan > 0 ) emit = TRUE ;
	}

	if ( emit ) {

	  if ( allEqual ) token[1] = '\0' ;
	  lHyp_function_emitAction ( pResult, 
			      last_row,
			      last_col, 
			      colspan, 
			      last_rendition,
			      last_graphics,
			      flags3,
			      token,
			      doDebug ) ;
	   emit = FALSE ;
	   colspan = 0 ;

	}
	  
	if ( colspan == 0 && isModified ) {

	  last_rendition = pScreenRendition[ir][ic] ;
	  last_character = pScreenCharacter[ir][ic] ;
	  last_flags     = pScreenFlags[ir][ic] & VT_FLAGS_G_MASK ;
	  last_isGL      = (last_character >= 0 ) ;
	  last_row = k ;
	  last_col = j ;

	  last_graphics = last_isGL ? 
		(pScreenFlags[ir][ic]&VT_FLAGS_GL_MASK) : 
		((pScreenFlags[ir][ic]&VT_FLAGS_GR_MASK)<<2) ; 
	    
	  token[0] = last_character ;
	  token[1] = '\0' ;

	  colspan = 1 ;
	  emit = FALSE ;
	  allEqual = TRUE ;
	}

      } /* End of columns */

    } /* End of rows */

    if ( colspan > 0 ) {
      if ( allEqual ) token[1] = '\0' ;
      lHyp_function_emitAction ( pResult, 
				last_row,
				last_col, 
				colspan, 
				last_rendition,
				last_graphics,
			    flags3,
				token,
				doDebug ) ;
    }
  }
}



static sData* lHyp_function_vt2html ( sInstance *pAI, 
				   sData *pStream, 
				   sData* pVT, 
				   sLOGICAL doReset,
				   sLOGICAL doDebug ) {


  int numeric_args[NUMERIC_ARG_COUNT] ;
  char numeric_arg_buffer[NUMERIC_ARG_WIDTH+1] ;
  int numeric_arg_count ;

  sBYTE rowMap[VT_MAX_ROWS] ;
  sBYTE rowId[VT_MAX_ROWS];

  char 
    chr,
    last_character,
    *pScreenCharacter[VT_MAX_ROWS] ;

  sBYTE
    *pScreenRendition[VT_MAX_ROWS],
    *pScreenFlags[VT_MAX_ROWS] ;

  sBYTE
    expectState,
    graphics,
    save_graphics,
    rendition,
    last_rendition,
    flags,
    last_flags,
    flags2,
    flags3,
    save_flags,
    save_flags2,
    save_rendition,
    action ;

  sData 
    *pData,
    *pValue,
    *pResult,
    *pLine,
    *pCol,
    *pNumCols,
    *pSaveCol,
    *pRow,
    *pNumRows,
    *pSaveRow,
    *pRendition,
    *pChr,
    *pSaveRendition,
    *pGraphics,
    *pSaveGraphics,
    *pFlags,
    *pFlags2,
    *pFlags3,
    *pSaveFlags,
    *pSaveFlags2,
    *pExpectState,
    *pVector,
    *pNextRow,
    *pStartRow,
    *pEndRow,
    *pStartScroll,
    *pEndScroll,
    *pScreen,
    *pPage,
    *pOffScroll,
    *pOffRow,
    *pArgc,
    *pArgb,
    *pArgs,
    *pAction ;

  int
    i,
    k,
    m,
    n,
    ix,
    ir,
    ic,
    ss,
    row,
    col,
    numCols,
    numRows,
    save_col,
    save_row,
    context,
    startRow=1,
    endRow=VT_MAX_ROWS,
    originStart,
    originEnd,
    arg=1,
    negotiateLen ;

  unsigned int
    r ;

  sLOGICAL
    actionModifier,
    isVector,
    isShift,
    foundTabStop,
    isModified=FALSE,
    hasPaged=FALSE,
    flushOutput=FALSE,
    doErase=TRUE ;

  sBYTE
    *pBuffer ;

  char
    *pLabel,
    c,c2,c3,c4,c5,t2,
    answerback[VALUE_SIZE+1],
    negotiate[VALUE_SIZE+1],
    id[VALUE_SIZE+1],
    line[VALUE_SIZE+1] ;


   /* Input structures:

   The arguments are
   
      list actions = vt2html ( pStream, pVT ) ;

   where:

   Output actions, list of id's to update.
     
      str actions = {
	str r1c1 = list td = { ....},
	str r24 = list tr = { list td = {...
      }

   Input pStream, raw VT input, eg:

      str stream = { ".....VT 200 input stream", .... } ;
   
   Input/Output VT screen state.


   list VT = {
   
     list state = {
	int row,    // Cursor row position
	int col,    // Cursor col position
	int numCols,  // Now a variable, for smaller than 80x24
	int numRows,  // Now a variable, for smaller than 80x24
	byte rendition,  // Current rendition setting
	binary graphics,	  // Current graphics mapping (G0,G1,G2,G3 mapping)
	binary flags,	  // Current flags state (GL/GR mapping)
	binary flags2,	  // Current flags2 (flags that can be saved/restored)
	binary flags3,	  // Current flags3 (terminal modes)
	int startRow,	// Start scroll line #, default = 1
	int endRow	// End scroll line #, default = 24
	byte expect   // Current expect state (internal use only)
	int argc      // Current argument count
	int argb      // Current argument TEXT
	int args[5]   // Current argument list
     },

     list save_state = {
	int col,      // Last saved cursor row position
	int row,      // Last saved cursor col positon
	byte rendition,  // Last saved rendition
	binary graphics	// Last saved graphics mapping
	binary flags	// Last saved flags state
	binary flags2	// Last saved flags2 state
     },

     list screen = {

	// <MAX_COL> cols <MAX_ROW> rows, labelled according to the id's of the rows
	// in the html <TABLE> node that holds the VT screen
	// The original order of the rows is r1, r, r3, r4,...r24,...
	// however the combination of both scrolling and the setting 
	// of scrolling regions at arbitrary start and end rows
	// can make the order mixed up.  If the HTML table gets out
	// of sync, its always re-ordered during the case when a
	// entire page gets refreshed, ie: all 24 rows.

	list r2 = {
	    char character[MAX_COLS], // ASCII value in cell
	    byte rendition[MAX_COLS], // Rendition at cell
	    byte flags[MAX_COLS],     // Flags at cell
	},
	list r5 = {
	    char character[MAX_COLS],
	    byte rendition[MAX_COLS],
	    byte flags[MAX_COLS],  
	},
	list r6 = {
	.
	.
	,
	list row24 = {
	    char character[MAX_COLS],
	    byte rendition[MAX_COLS],
	    byte flags[MAX_COLS],  
	}
      },

      // This is the offscroll area, It is output only,
      // its what is pushed off the scrolling
      //.area. It could be 0,1,or more lines. 
      list offscroll = {
	list row = {
	    char character[MAX_COLS],
	    byte rendition[MAX_COLS],
	    byte flags[MAX_COLS],  
	},
	list row = {
	    char character[MAX_COLS],
	    byte rendition[MAX_COLS],
	    byte flags[MAX_COLS],  
	},
	.
	.
	.
	,
	list row = {
	    char character[MAX_COLS],
	    byte rendition[MAX_COLS],
	    byte flags[MAX_COLS],  
	}
      } ,

      // This is the paging area, It is the copy of
      // the last complete page and the offscroll contents
      //.. 
     list page = {

	list screen = {
	  //Copy of screen area 
	},
	list offscroll = {
	  // Copy of offscroll area 
	}
      },

   }
  } ;

  */

  /*****************************************************************************
   *
   *
   * L O A D  V T  S T A T E :  R O W , C O L , G R A P H I C S , R E N D I T I O N
   *
   *
   ******************************************************************************/

  pData = gHyp_data_getChildByName ( pVT, "state" ) ;
  if ( !pData ) {

    pData = gHyp_data_new ( "state" ) ;
    gHyp_data_append ( pVT, pData ) ;

    row = 1 ;
    pRow = gHyp_data_new ( "row" ) ;
    gHyp_data_newVector ( pRow, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
    gHyp_data_append ( pData, pRow ) ;

    col = 1 ;
    pCol = gHyp_data_new ( "col" ) ;
    gHyp_data_newVector ( pCol, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
    gHyp_data_append ( pData, pCol ) ;

    numCols = VT_MAX_COLS ;
    pNumCols = gHyp_data_new ( "numCols" ) ;
    gHyp_data_newVector ( pNumCols, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pNumCols, &numCols, 0 ) ;
    gHyp_data_append ( pData, pNumCols ) ;

    numRows = VT_MAX_ROWS ;
    pNumRows = gHyp_data_new ( "numRows" ) ;
    gHyp_data_newVector ( pNumRows, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pNumRows, &numRows, 0 ) ;
    gHyp_data_append ( pData, pNumRows ) ;

    chr =  ' ' ;
    pChr = gHyp_data_new ( "chr" ) ;
    gHyp_data_newVector ( pChr, TYPE_UCHAR, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
    gHyp_data_append ( pData, pChr ) ;

    startRow = 1 ;
    pStartRow = gHyp_data_new ( "startRow" ) ;
    gHyp_data_newVector ( pStartRow, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pStartRow, &startRow, 0 ) ;
    gHyp_data_append ( pData, pStartRow ) ;

    endRow = numRows ;
    pEndRow = gHyp_data_new ( "endRow" ) ;
    gHyp_data_newVector ( pEndRow, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pEndRow, &endRow, 0 ) ;
    gHyp_data_append ( pData, pEndRow ) ;
    
    rendition = RENDITION_STANDARD ;
    pRendition = gHyp_data_new ( "rendition" ) ;
    gHyp_data_newVector ( pRendition, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pRendition, &rendition, 0 ) ;
    gHyp_data_append ( pData, pRendition ) ;

    graphics = (unsigned char) VT_GRAPHICS_DEFAULT ;
    pGraphics = gHyp_data_new ( "graphics" ) ;
    gHyp_data_newVector ( pGraphics, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
    gHyp_data_append ( pData, pGraphics ) ;

    flags = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL ;
    pFlags = gHyp_data_new ( "flags" ) ;
    gHyp_data_newVector ( pFlags, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
    gHyp_data_append ( pData, pFlags ) ;

    flags2 = 0 ;
    pFlags2 = gHyp_data_new ( "flags2" ) ;
    gHyp_data_newVector ( pFlags2, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
    gHyp_data_append ( pData, pFlags2 ) ;

    flags3 = 0 ;
    pFlags3 = gHyp_data_new ( "flags3" ) ;
    gHyp_data_newVector ( pFlags3, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pFlags3, &flags3, 0 ) ;
    gHyp_data_append ( pData, pFlags3 ) ;

    /* Number of columns defaults to 80 */
    numCols = VT_80_COL ; ;

    expectState = XPECT_CHARACTER ;
    pExpectState = gHyp_data_new ( "expect" ) ;
    gHyp_data_newVector ( pExpectState, TYPE_BYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pExpectState, &expectState, 0 ) ;
    gHyp_data_append ( pData, pExpectState ) ;

    numeric_arg_count = 0 ;
    pArgc = gHyp_data_new ( "argc" ) ;
    gHyp_data_newVector ( pArgc, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
    gHyp_data_append ( pData, pArgc ) ;

    numeric_arg_buffer[0] = '\0' ;
    pValue = gHyp_data_new ( "argb" ) ;
    pArgb = gHyp_data_new(NULL) ;
    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
    gHyp_data_append ( pValue, pArgb ) ;
    gHyp_data_append ( pData, pValue ) ;

    pArgs = gHyp_data_new ( "args" ) ;
    gHyp_data_newVector ( pArgs, TYPE_INTEGER, 5, TRUE ) ;
    for ( i=0;i<NUMERIC_ARG_COUNT;i++) {
      numeric_args[i] = 0 ;
      gHyp_data_setVectorRaw ( pArgs, &numeric_args[i], i ) ;
    }
    gHyp_data_append ( pData, pArgs ) ;

  }

  pNumRows = gHyp_data_getChildByName ( pData, "numRows" ) ;
  if ( !pNumRows ) {
    pNumRows = gHyp_data_new ( "numRows" ) ;
    gHyp_data_newVector ( pNumRows, TYPE_INTEGER, 1, TRUE ) ;
    numRows = VT_MAX_ROWS ;
    gHyp_data_setVectorRaw ( pNumRows, &numRows, 0 ) ;
    gHyp_data_append ( pData, pNumRows ) ;
  }

  numRows = gHyp_data_getInt ( pNumRows, 0, TRUE ) ;
  if ( numRows < 3 || numRows > VT_MAX_ROWS ) {
    numRows = VT_MAX_ROWS ;
    gHyp_data_setVectorRaw ( pNumRows, &numRows, 0 ) ;
  }


  pRow = gHyp_data_getChildByName ( pData, "row" ) ;
  if ( !pRow ) {
    pRow = gHyp_data_new ( "row" ) ;
    gHyp_data_newVector ( pRow, TYPE_INTEGER, 1, TRUE ) ;
    row = 1 ;
    gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
    gHyp_data_append ( pData, pRow ) ;
  }

  row = gHyp_data_getInt ( pRow, 0, TRUE ) ;
  if ( row < 1 || row > numRows ) {
    row = 1 ;
    gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
  }


  pChr = gHyp_data_getChildByName ( pData, "chr" ) ;
  if ( !pChr ) {
    pChr = gHyp_data_new ( "chr" ) ;
    gHyp_data_newVector ( pChr, TYPE_UCHAR, 1, TRUE ) ;
    chr = ' ' ;
    gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
    gHyp_data_append ( pData, pChr ) ;
  }
  chr = (char) gHyp_data_getInt ( pChr, 0, TRUE ) ;

  pFlags = gHyp_data_getChildByName ( pData, "flags" ) ;
  if ( !pFlags ) {
    pFlags = gHyp_data_new ( "flags" ) ;
    gHyp_data_newVector ( pFlags, TYPE_UBYTE, 1, TRUE ) ;
    flags = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL ;
    gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
    gHyp_data_append ( pData, pFlags ) ;
  }
  flags = gHyp_data_getInt ( pFlags, 0, TRUE ) ;

  pFlags2 = gHyp_data_getChildByName ( pData, "flags2" ) ;
  if ( !pFlags2 ) {
    pFlags2 = gHyp_data_new ( "flags2" ) ;
    gHyp_data_newVector ( pFlags2, TYPE_UBYTE, 1, TRUE ) ;
    flags2 = 0 ;
    gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
    gHyp_data_append ( pData, pFlags2 ) ;
  }
  flags2 = gHyp_data_getInt ( pFlags2, 0, TRUE ) ;

  pFlags3 = gHyp_data_getChildByName ( pData, "flags3" ) ;
  if ( !pFlags3 ) {
    pFlags3 = gHyp_data_new ( "flags3" ) ;
    gHyp_data_newVector ( pFlags3, TYPE_UBYTE, 1, TRUE ) ;
    flags3 = 0 ;
    gHyp_data_setVectorRaw ( pFlags3, &flags3, 0 ) ;
    gHyp_data_append ( pData, pFlags3 ) ;
  }
  flags3 = gHyp_data_getInt ( pFlags3, 0, TRUE ) ;

  pNumCols = gHyp_data_getChildByName ( pData, "numCols" ) ;
  if ( !pNumCols ) {
    pNumCols = gHyp_data_new ( "numCols" ) ;
    gHyp_data_newVector ( pNumCols, TYPE_INTEGER, 1, TRUE ) ;
    numCols = ( flags3 & VT_FLAGS3_132_COL ) ? VT_132_COL : VT_80_COL  ;
    gHyp_data_setVectorRaw ( pNumCols, &numCols, 0 ) ;
    gHyp_data_append ( pData, pNumCols ) ;
  }

  numCols = gHyp_data_getInt ( pNumCols, 0, TRUE ) ;
  if ( numCols < 5 || numCols > VT_MAX_COLS ) {
    /* Set number of columns */
    numCols = ( flags3 & VT_FLAGS3_132_COL ) ? VT_132_COL : VT_80_COL  ;
    gHyp_data_setVectorRaw ( pNumCols, &numCols, 0 ) ;
  }

  pCol = gHyp_data_getChildByName ( pData, "col" ) ;
  if ( !pCol ) {
    pCol = gHyp_data_new ( "col" ) ;
    gHyp_data_newVector ( pCol, TYPE_INTEGER, 1, TRUE ) ;
    col = 1 ;
    gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
    gHyp_data_append ( pData, pCol ) ;
  }

  col = gHyp_data_getInt ( pCol, 0, TRUE ) ;
  if ( col < 1 || col > numCols ) {
    col = 1 ;
    gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
  }

  pStartRow = gHyp_data_getChildByName ( pData, "startRow" ) ;
  if ( !pStartRow ) {
    startRow = 1 ;
    pStartRow = gHyp_data_new ( "startRow" ) ;
    gHyp_data_newVector ( pStartRow, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pStartRow, &startRow, 0 ) ;
    gHyp_data_append ( pData, pStartRow ) ;
  }

  startRow = gHyp_data_getInt ( pStartRow, 0, TRUE ) ;
  if ( startRow < 1 || startRow > numRows ) {
    startRow = 1 ;
    gHyp_data_setVectorRaw ( pStartRow, &startRow, 0 ) ;
  }

  pEndRow = gHyp_data_getChildByName ( pData, "endRow" ) ;
  if ( !pEndRow ) {
    endRow = numRows ;
    pEndRow = gHyp_data_new ( "endRow" ) ;
    gHyp_data_newVector ( pEndRow, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pEndRow, &endRow, 0 ) ;
    gHyp_data_append ( pData, pEndRow ) ;
  }

  endRow = gHyp_data_getInt ( pEndRow, 0, TRUE ) ;
  if ( endRow < startRow || endRow > numRows ) {
    endRow = numRows ;
    gHyp_data_setVectorRaw ( pEndRow, &endRow, 0 ) ;
  }

  pRendition = gHyp_data_getChildByName ( pData, "rendition" ) ;
  if ( !pRendition ) {
    pRendition = gHyp_data_new ( "rendition" ) ;
    gHyp_data_newVector ( pRendition, TYPE_UBYTE, 1, TRUE ) ;
    rendition = RENDITION_STANDARD ;
    gHyp_data_setVectorRaw ( pRendition, &rendition, RENDITION_STANDARD ) ;
    gHyp_data_append ( pData, pRendition ) ;
  }
  rendition = gHyp_data_getInt ( pRendition, 0, TRUE ) ;

  pGraphics = gHyp_data_getChildByName ( pData, "graphics" ) ;
  if ( !pGraphics ) {
    pGraphics = gHyp_data_new ( "graphics" ) ;
    gHyp_data_newVector ( pGraphics, TYPE_UBYTE, 1, TRUE ) ;
    graphics = (unsigned char) VT_GRAPHICS_DEFAULT ;
    gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
    gHyp_data_append ( pData, pGraphics ) ;
  }
  graphics = gHyp_data_getInt ( pGraphics, 0, TRUE ) ;

  pExpectState = gHyp_data_getChildByName ( pData, "expect" ) ;
  if ( !pExpectState ) {
    pExpectState = gHyp_data_new ( "expect" ) ;
    gHyp_data_newVector ( pExpectState, TYPE_BYTE, 1, TRUE ) ;
    expectState = XPECT_CHARACTER ;
    gHyp_data_setVectorRaw ( pExpectState, &expectState, 0 ) ;
    gHyp_data_append ( pData, pExpectState ) ;
  }
  expectState = gHyp_data_getInt ( pExpectState, 0, TRUE ) ;
  if ( expectState < XPECT_CHARACTER || expectState > XPECT_END_DCS ) {
    expectState = XPECT_CHARACTER ;
    gHyp_data_setVectorRaw ( pExpectState, &expectState, 0 ) ;
  }
  
  pArgc = gHyp_data_getChildByName ( pData, "argc" ) ;
  if ( !pArgc ) {
    pArgc = gHyp_data_new ( "argc" ) ;
    gHyp_data_newVector ( pArgc, TYPE_INTEGER, 1, TRUE ) ;
    numeric_arg_count = 0 ;
    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
    gHyp_data_append ( pData, pArgc ) ;
  }
  numeric_arg_count = gHyp_data_getInt ( pArgc, 0, TRUE ) ;
  if ( numeric_arg_count < 0 || numeric_arg_count > NUMERIC_ARG_COUNT ) {
    numeric_arg_count = 0 ;
    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
  }

  pValue = gHyp_data_getChildByName ( pData, "argb" ) ;
  if ( !pValue ) {
    pValue = gHyp_data_new ( "argb" ) ;
    numeric_arg_buffer[0] = '\0' ;
    pArgb = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
    gHyp_data_append ( pValue, pArgb ) ;
    gHyp_data_append ( pData, pValue ) ;
  }
  pArgb = gHyp_data_getValue ( pValue, 0, TRUE ) ;
  gHyp_data_getStr ( pArgb, numeric_arg_buffer, NUMERIC_ARG_WIDTH, 0, TRUE ) ;
   
  numeric_arg_count = gHyp_data_getInt ( pArgc, 0, TRUE ) ;
  if ( numeric_arg_count < 0 || numeric_arg_count > NUMERIC_ARG_COUNT ) {
    numeric_arg_count = 0 ;
    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
  }

  
  pArgs = gHyp_data_getChildByName ( pData, "args" ) ;
  if ( !pArgs ) {
    pArgs = gHyp_data_new ( "args" ) ;
    gHyp_data_newVector ( pArgs, TYPE_INTEGER, NUMERIC_ARG_COUNT, TRUE ) ;
    for ( i=0;i<NUMERIC_ARG_COUNT; i++ ) {
      numeric_args[i] = 0 ;
      gHyp_data_setVectorRaw ( pArgs, &numeric_args[i], i ) ;
    }
    gHyp_data_append ( pData, pArgs ) ;
  }
  for ( i=0; i<numeric_arg_count;i++ ) {
    numeric_args[i] = gHyp_data_getInt ( pArgs, i, TRUE ) ;
  }


  /*****************************************************************************
   *
   *
   * L O A D  V T  S A V E  S T A T E
   *
   *
   ******************************************************************************/

  pData = gHyp_data_getChildByName ( pVT, "save_state" ) ;
  if ( !pData ) {

    pData = gHyp_data_new ( "save_state" ) ;
    gHyp_data_append ( pVT, pData ) ;

    save_row = 1 ;
    pSaveRow = gHyp_data_new ( "row" ) ;
    gHyp_data_newVector ( pSaveRow, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pSaveRow, &save_row, 0 ) ;
    gHyp_data_append ( pData, pSaveRow ) ;

    save_col = 1 ;
    pSaveCol = gHyp_data_new ( "col" ) ;
    gHyp_data_newVector ( pSaveCol, TYPE_INTEGER, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pSaveCol, &save_col, 0 ) ;
    gHyp_data_append ( pData, pSaveCol ) ;
        
    save_rendition = RENDITION_STANDARD ;
    pSaveRendition = gHyp_data_new ( "rendition" ) ;
    gHyp_data_newVector ( pSaveRendition, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pSaveRendition, &save_rendition, 0 ) ;
    gHyp_data_append ( pData, pSaveRendition ) ;

    save_graphics = (unsigned char) VT_GRAPHICS_DEFAULT ;

    pSaveGraphics = gHyp_data_new ( "graphics" ) ;
    gHyp_data_newVector ( pSaveGraphics, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pSaveGraphics, &save_graphics, 0 ) ;
    gHyp_data_append ( pData, pSaveGraphics ) ;

    save_flags = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL ;
    pSaveFlags = gHyp_data_new ( "flags" ) ;
    gHyp_data_newVector ( pSaveFlags, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pSaveFlags, &save_flags, 0 ) ;
    gHyp_data_append ( pData, pSaveFlags ) ;

    save_flags2 = 0 ;
    pSaveFlags2 = gHyp_data_new ( "flags2" ) ;
    gHyp_data_newVector ( pSaveFlags2, TYPE_UBYTE, 1, TRUE ) ;
    gHyp_data_setVectorRaw ( pSaveFlags2, &save_flags2, 0 ) ;
    gHyp_data_append ( pData, pSaveFlags2 ) ;

  }

  pSaveRow = gHyp_data_getChildByName ( pData, "row" ) ;
  if ( !pSaveRow ) {
    pSaveRow = gHyp_data_new ( "row" ) ;
    gHyp_data_newVector ( pSaveRow, TYPE_INTEGER, 1, TRUE ) ;
    save_row = 1 ;
    gHyp_data_setVectorRaw ( pSaveRow, &save_row, 0 ) ;
    gHyp_data_append ( pData, pSaveRow ) ;
  }

  save_row = gHyp_data_getInt ( pSaveRow, 0, TRUE ) ;
  if ( save_row < 1 || save_row > numRows ) {
    save_row = 1 ;
    gHyp_data_setVectorRaw ( pSaveRow, &save_row, 0 ) ;
  }

  pSaveCol = gHyp_data_getChildByName ( pData, "col" ) ;
  if ( !pSaveCol ) {
    pSaveCol = gHyp_data_new ( "col" ) ;
    save_col = 1 ;
    gHyp_data_setVectorRaw ( pSaveCol, &save_col, 0 ) ;
    gHyp_data_append ( pData, pSaveCol ) ;
  }

  save_col = gHyp_data_getInt ( pSaveCol, 0, TRUE ) ;
  if ( save_col < 1 || save_col > VT_MAX_COLS ) {
    save_col = 1 ;
    gHyp_data_setVectorRaw ( pSaveCol, &save_col, 0 ) ;
  }

  pSaveRendition = gHyp_data_getChildByName ( pData, "rendition" ) ;
  if ( !pSaveRendition ) {
    pSaveRendition = gHyp_data_new ( "rendition" ) ;
    gHyp_data_newVector ( pSaveRendition, TYPE_UBYTE, 1, TRUE ) ;
    save_rendition = RENDITION_STANDARD ;
    gHyp_data_setVectorRaw ( pSaveRendition, &save_rendition, 0 ) ;
    gHyp_data_append ( pData, pSaveRendition ) ;
  }
  save_rendition = gHyp_data_getInt ( pSaveRendition, 0, TRUE ) ;

  pSaveGraphics = gHyp_data_getChildByName ( pData, "graphics" ) ;
  if ( !pSaveGraphics ) {
    pSaveGraphics = gHyp_data_new ( "graphics" ) ;
    gHyp_data_newVector ( pSaveGraphics, TYPE_UBYTE, 1, TRUE ) ;
    save_graphics = (unsigned char) VT_GRAPHICS_DEFAULT ;
    gHyp_data_setVectorRaw ( pSaveGraphics, &save_graphics, 0 ) ;
    gHyp_data_append ( pData, pSaveGraphics ) ;
  }
  save_graphics = gHyp_data_getInt ( pSaveGraphics, 0, TRUE ) ;

  pSaveFlags = gHyp_data_getChildByName ( pData, "flags" ) ;
  if ( !pSaveFlags ) {
    pSaveFlags = gHyp_data_new ( "flags" ) ;
    gHyp_data_newVector ( pSaveFlags, TYPE_UBYTE, 1, TRUE ) ;
    save_flags = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL ;
    gHyp_data_setVectorRaw ( pSaveFlags, &save_flags, 0 ) ;
    gHyp_data_append ( pData, pSaveFlags ) ;
  }
  save_flags = gHyp_data_getInt ( pSaveFlags, 0, TRUE ) ;

  pSaveFlags2 = gHyp_data_getChildByName ( pData, "flags2" ) ;
  if ( !pSaveFlags2 ) {
    pSaveFlags2 = gHyp_data_new ( "flags2" ) ;
    gHyp_data_newVector ( pSaveFlags2, TYPE_UBYTE, 1, TRUE ) ;
    save_flags2 = 0 ;
    gHyp_data_setVectorRaw ( pSaveFlags2, &save_flags2, 0 ) ;
    gHyp_data_append ( pData, pSaveFlags2 ) ;
  }
  save_flags2 = gHyp_data_getInt ( pSaveFlags2, 0, TRUE ) ;


  /*****************************************************************************
   *
   *
   * L O A D  V T  S C R E E N  D A T A
   *
   *
   ******************************************************************************/

  /* The rowMap and Id's always starts off 1 to 1 */
  for ( ir=0;ir<numRows;ir++ ) {
    rowMap[ir] = ir ;
    rowId[ir] = ir+1 ;
  }

  pScreen = gHyp_data_getChildByName ( pVT, "screen" ) ;
  if ( !pScreen ) {

    pScreen = gHyp_data_new ( "screen" ) ;
    gHyp_data_append ( pVT, pScreen ) ;
  }

  n = gHyp_data_getCount ( pScreen ) ;

  ir = 0 ;
  while ( ir < n ) {

    pNextRow = gHyp_data_getChildBySS ( pScreen, ir ) ; 

    if ( ir >= numRows ) {
      /* Rid ourselves of any extra */
      assert ( FALSE ) ;
      gHyp_data_detach ( pNextRow ) ;
      gHyp_data_delete ( pNextRow ) ;
      n-- ;
      continue ;
    }

    /* Get row map information */
    pLabel = gHyp_data_getLabel ( pNextRow ) ;
    strcpy ( id, pLabel+1 ) ;

    i = sscanf ( id, "%u", (unsigned int*) &r ) ;
    if ( i != 1 ) r = ir+1 ;
    rowId[ir] = r ;

    /*if (doDebug) gHyp_util_debug ( "rowId[%u] = %u, rowMap[%u] = %u", ir, rowId[ir], ir, rowMap[ir] );*/

    pVector = gHyp_data_getChildByName ( pNextRow, "character" ) ;
    if ( !pVector ) {
      pVector = gHyp_data_new ( "character" ) ;
      gHyp_data_newVector ( pVector, TYPE_CHAR, VT_MAX_COLS, FALSE ) ;
      pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
      for ( ic=0; ic<VT_MAX_COLS; ic++ ) *pBuffer++ = ' ' ;
      gHyp_data_append ( pNextRow, pVector ) ;
    }
    pScreenCharacter[ir] = (char*) gHyp_data_buffer ( pVector, 0 ) ;

    pVector = gHyp_data_getChildByName ( pNextRow, "rendition" ) ;
    if ( !pVector ) {
      pVector = gHyp_data_new ( "rendition" ) ;
      gHyp_data_newVector ( pVector, TYPE_UBYTE, VT_MAX_COLS, FALSE ) ;
      pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
      for ( ic=0; ic<VT_MAX_COLS; ic++ )  *pBuffer++ =  RENDITION_STANDARD ;
      gHyp_data_append ( pNextRow, pVector ) ;
    }
    pScreenRendition[ir]  = gHyp_data_buffer ( pVector, 0 ) ;

    pVector = gHyp_data_getChildByName ( pNextRow, "flags" ) ;
    if ( !pVector ) {
      pVector = gHyp_data_new ( "flags" ) ;
      gHyp_data_newVector ( pVector, TYPE_UBYTE, VT_MAX_COLS, FALSE ) ;
      pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
      for ( ic=0; ic<VT_MAX_COLS; ic++ )  {
	if ( ic%8 == 0 )
	  *pBuffer++ = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED + 
	    VT_FLAGS_TAB_STOP ;
	else
	  *pBuffer++ = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED ;
      }
      gHyp_data_append ( pNextRow, pVector ) ;
    }
    pScreenFlags[ir] = gHyp_data_buffer ( pVector, 0 ) ;

    ir++ ;

  }

  /* Fill in anything missing */

  for ( ir=n;ir<numRows; ir++ ) {

    sprintf ( id, "r%u", ir+1 ) ;
    pNextRow = gHyp_data_new ( id ) ;

    rowId[ir] = ir+1 ;

    gHyp_data_append ( pScreen, pNextRow ) ;

    pVector = gHyp_data_new ( "character" ) ;
    gHyp_data_newVector ( pVector, TYPE_CHAR, VT_MAX_COLS, FALSE ) ;
    pScreenCharacter[ir] = (char*) gHyp_data_buffer ( pVector, 0 ) ;
    for ( ic=0; ic<VT_MAX_COLS; ic++ ) pScreenCharacter[ir][ic] = ' ' ;
    gHyp_data_append ( pNextRow, pVector ) ;

    pVector = gHyp_data_new ( "rendition" ) ;
    gHyp_data_newVector ( pVector, TYPE_UBYTE, VT_MAX_COLS, FALSE ) ;
    pScreenRendition[ir] = gHyp_data_buffer ( pVector, 0 ) ;
    for ( ic=0; ic<VT_MAX_COLS; ic++ ) pScreenRendition[ir][ic] = RENDITION_STANDARD ;
    gHyp_data_append ( pNextRow, pVector ) ;

    pVector = gHyp_data_new ( "flags" ) ;
    gHyp_data_newVector ( pVector, TYPE_UBYTE, VT_MAX_COLS, FALSE ) ;
    pScreenFlags[ir] = gHyp_data_buffer ( pVector, 0 ) ;

    for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
      if ( ic%8 == 0 )
	pScreenFlags[ir][ic] =
	  VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED + 
	  VT_FLAGS_TAB_STOP;
      else
	pScreenFlags[ir][ic] = 
	  VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED ;
    }
    isModified = TRUE ;

    gHyp_data_append ( pNextRow, pVector ) ;

  }


  /*****************************************************************************
   *
   *
   * I N I T I A L I A L I Z E   V T  O F F  S C R E E N
   *
   *
   ******************************************************************************/

  pPage = gHyp_data_getChildByName ( pVT, "page" ) ;
  if ( !pPage ) {
    pPage = gHyp_data_new ( "page" ) ;
    gHyp_data_append ( pVT, pPage ) ;
  }
;


  /*****************************************************************************
   *
   *
   * I N I T I A L I A L I Z E   V T  O F F  S C R O L L
   *
   *
   ******************************************************************************/

  pOffScroll = gHyp_data_getChildByName ( pVT, "offscroll" ) ;
  if ( !pOffScroll ) {
    pOffScroll = gHyp_data_new ( "offscroll" ) ;
    gHyp_data_append ( pVT, pOffScroll ) ;
  }
;

  /*****************************************************************************
   *
   *
   * P R O C E S S   B U F F E R
   *
   *
   ******************************************************************************/

  /* Setup for reading pStream */
  flushOutput = FALSE ;
  pResult = gHyp_data_new ( "_vt2html_" ) ;
  isVector = (gHyp_data_getDataType(pStream) > TYPE_STRING ) ;
  pLine = NULL ;
  ss = gHyp_data_getSubScript ( pStream ) ; 
  context = -1 ;

  /* RESET PAINTS THE ENTIRE SCREEN */
  if ( doReset ) {
    if ( doDebug ) gHyp_util_debug("INTERNAL RESET");
    for ( ir=0;ir<numRows;ir++ ) {
      for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	/* Set as modified */
        pScreenFlags[ir][ic] |=  VT_FLAGS_MODIFIED ;
	/* And clear the page. */
        pScreenFlags[ir][ic] &= ~VT_FLAGS_PAGED ;
      }
    }
    isModified = TRUE ;
  }

  action = ACTION_NONE ;
  actionModifier = FALSE ;
  isShift = FALSE ;
  c = ' ' ;
  t2 = ' ' ;

  while ( (pLine = gHyp_data_nextValue ( pStream, 
					 pLine, 
					 &context,
					 ss ) ) ) {
    n = gHyp_data_getStr ( pLine, 
			   line, 
			   VALUE_SIZE, 
			   context, 
			   isVector ) ;

    /* Start of next block on n characters, index is ix */
    ix = 0 ;
    while ( ix < n ) {
    
      /*****************************************************************************
       *
       *
       * G E T  A C T I O N  
       *
       *
       ******************************************************************************/

      /*
       *
       * ACTIONS:
       *
       * A. A Character
       * B. An escape sequence
       * C. A control sequence
       *
       */

      while ( action == ACTION_NONE  ) {

	/* Get next character */

	if ( ix >= n ) break ;
	c = line[ix++] ;

	/*if (doDebug) gHyp_util_debug("CHAR %c [%u] at (%u,%u)", c, c, row, col );*/
	
	switch ( c ) {

	/***********************************************************************/
	case VT_TN_IAC :

	  /* Start of telnet sequence 
	  
	  VT_TN_OPTION_ECHO '\001'
	  VT_TN_OPTION_SUPPRESS_GOAHEAD '003'
	  VT_TN_OPTION_STATUS '\005'
	  VT_TN_OPTION_TIMING_MARK '\006'
	  VT_TN_OPTION_TERMINAL_TYPE '\030'
	  VT_TN_OPTION_WINDOW_SIZE '\037'
	  VT_TN_OPTION_TERMINAL_SPEED '\040'
	  VT_TN_OPTION_REMOTE_FLOW '\041'
	  VT_TN_OPTION_LINEMODE '\042'
	  VT_TN_OPTION_ENV '\044'

	  VT_TN_DONT '\376'
	  VT_TN_DO '\375'
	  VT_TN_WONT '\374'
	  VT_TN_WILL '\373'
	  VT_TN_SUBNEGOTIATE '\372'

	  A real example:
	  ASK   ---> from telent server
	  REPLY <--- from telnet client
	  
	  Ask:  DO TERM TYPE?, DO TERM SPEED?, DO 043?,      DO 047)?
	  ->    \377\375\030   \377\375\040    \377\375\043  \377\375\047

	  Reply:WILL TERM TYPE, WONT TERM SPEED, WONT 043,     WONT 047
	  <-    \377\373\030    \377\374\040     \377\374\043  \377\374\047

	  Ask:  NEGOTIATE TERM TYPE, REQUEST 1 ARG
	  ->    \377\372\030\001\377\360

	  Reply NEGOTIATE TERM TYPE: SUPPLY "vt100"
	  <-    \377\372\030\000vt100\377\360

	  Ask:  WILL SUPPRESS GOAHEAD,  DO ECHO?,     DO WINDOW SIZE?   WILL STATUS?     DO REMOTE FLOW?  
	  ->    \377\373\003            \377\375\001  \377\375\037        \377\373\005     \377\375\041

	  Reply:DO SUPPRESS GOHEAD, WONT ECHO,    WILL WINDOW SIZE, NEGOTIATE WINDOW SIZE (80x24),        DONT STATUS,  WONT REMOTE FLOW
	  <-    \377\375\003        \377\374\001  \377\373\037      \377\372\037\000\120\000\030\377\360  \377\376\005  \377\374\041

	  Ask:  WILL ECHO?
	  ->    \377\373\001

	  Reply:DO ECHO
	  <-    \377\375\001	  
	  
	  */

	  if ( ix >= n ) {
	    pLine = gHyp_data_nextValue ( pStream, pLine, &context,ss ) ;
	    if ( !pLine ) { action = ACTION_NONE ; break ; }
	    n = gHyp_data_getStr ( pLine, line,  VALUE_SIZE, context, isVector ) ;
	    ix = 0 ;
	  }

	  if ( line[ix] == VT_TN_IAC ) {
	    c = line[ix++] ;
	    action = ACTION_CHARACTER ;
	    break ;
	  }
	  
	  c2 = line[ix++] ;
	  if ( ix >= n ) {
	    pLine = gHyp_data_nextValue ( pStream, pLine, &context,ss ) ;
	    if ( !pLine ) { action = ACTION_NONE ; break ; }
	    n = gHyp_data_getStr ( pLine, line,  VALUE_SIZE, context, isVector ) ;
	    ix = 0 ;
	  }
	  c3 = line[ix++] ;

	  negotiate[0] = '\0' ;
	  negotiateLen = 0 ;
	  action = ACTION_EMIT ;
	    
	  switch ( c2 ) {

	    case VT_TN_DONT :
	    case VT_TN_WONT :
	      /* If we receive DONT or WONT, we should do something, but not reply */
	      action = ACTION_NONE ;
	      break ;

	    case VT_TN_DO :

	      /* If we receive DO, we should reply with a WILL or a WONT */

	      switch ( c3 ) {

	      /* Options we WILL support */
	      case VT_TN_OPTION_ECHO :
	      case VT_TN_OPTION_TERMINAL_TYPE :
	      case VT_TN_OPTION_SUPPRESS_GOAHEAD :
		t2 = VT_TN_WILL ;
		break ;

	      case VT_TN_OPTION_WINDOW_SIZE :
		t2 = VT_TN_WILL ;

	        /* Emit response */
		pAction = gHyp_data_new ( "emit" ) ;
		answerback[0] = c ;
		answerback[1] = t2 ;
		answerback[2] = c3 ;
		answerback[3] = '\0' ;
		pData = gHyp_data_new ( NULL ) ;
		gHyp_data_setStr_n ( pData, answerback, 3 ) ;
		gHyp_data_append ( pAction, pData ) ;
		gHyp_data_append ( pResult, pAction ) ;

		t2 = VT_TN_SUBNEGOTIATE ;
		negotiateLen = 6 ;
		memcpy ( negotiate, "\000\120\000\030\377\360", negotiateLen ) ;

		break ;

	      /* Options we WONT support */
	      case VT_TN_OPTION_STATUS :
	      case VT_TN_OPTION_TIMING_MARK :
	      case VT_TN_OPTION_REMOTE_FLOW :
	      case VT_TN_OPTION_LINEMODE :
	      case VT_TN_OPTION_ENV :
	      default:
		t2 = VT_TN_WONT ;
		break ;

	      }
	      break ;

	    case VT_TN_WILL :

	      /* If we receive WILL, we should reply DO or DONT */

	      switch ( c3 ) {

	      /* Options we DO support */
	      case VT_TN_OPTION_ECHO :
	      case VT_TN_OPTION_SUPPRESS_GOAHEAD :
	      case VT_TN_OPTION_WINDOW_SIZE :
		t2 = VT_TN_DO ;
		break ;

	      /* Optiona qw DONT support */
	      case VT_TN_OPTION_TERMINAL_TYPE :
	      case VT_TN_OPTION_STATUS :
	      case VT_TN_OPTION_TIMING_MARK :
	      case VT_TN_OPTION_REMOTE_FLOW :
	      case VT_TN_OPTION_LINEMODE :
	      case VT_TN_OPTION_ENV :
	      default:

		t2 = VT_TN_DONT ;
		break ;

	      }
	      break ;

	    case VT_TN_SUBNEGOTIATE :

	      /* If we receive SUBNEGOTIATE, we should reply SUBNEGOTIATE or DONT */

	      if ( ix >= n ) {
	        pLine = gHyp_data_nextValue ( pStream, pLine, &context,ss ) ;
	        if ( !pLine ) { action = ACTION_NONE ; break ; }
	        n = gHyp_data_getStr ( pLine, line,  VALUE_SIZE, context, isVector ) ;
	        ix = 0 ;
	      }
	      arg = line[ix++] ;

	      if ( ix >= n ) {
	        pLine = gHyp_data_nextValue ( pStream, pLine, &context,ss ) ;
	        if ( !pLine ) { action = ACTION_NONE ; break ; }
	        n = gHyp_data_getStr ( pLine, line,  VALUE_SIZE, context, isVector ) ;
	        ix = 0 ;
	      }
	      c4 = line[ix++] ;

	      if ( c4 != VT_TN_IAC ) {
	        /* Was expecting \377 */
	        action = ACTION_NONE ;
	        break ;
	      }

	      if ( ix >= n ) {
	        pLine = gHyp_data_nextValue ( pStream, pLine, &context,ss ) ;
	        if ( !pLine ) { action = ACTION_NONE ; break ; }
	        n = gHyp_data_getStr ( pLine, line,  VALUE_SIZE, context, isVector ) ;
	        ix = 0 ;
	      }
	      c5 = line[ix++] ;

	      if ( c5 != VT_TN_ENDSUBNEGOTIATE ) {
	        /* Was expecting \360 */
	        action = ACTION_NONE ;
	        break ;
	      }

	      switch ( c3 ) {

	      case VT_TN_OPTION_TERMINAL_TYPE :
		t2 = VT_TN_SUBNEGOTIATE ;
		negotiateLen = 8 ;
		memcpy ( negotiate, "\000vt100\377\360", negotiateLen ) ;
		break ;

	      case VT_TN_OPTION_WINDOW_SIZE :
		t2 = VT_TN_SUBNEGOTIATE ;
		negotiateLen = 6 ;
		memcpy ( negotiate, "\000\120\000\030\377\360", negotiateLen ) ;
		break ;

	      case VT_TN_OPTION_STATUS :
	      case VT_TN_OPTION_TIMING_MARK :
	      case VT_TN_OPTION_REMOTE_FLOW :
	      case VT_TN_OPTION_LINEMODE :
	      case VT_TN_OPTION_ENV :
	      case VT_TN_OPTION_ECHO :
	      case VT_TN_OPTION_SUPPRESS_GOAHEAD :
	      default:
		t2 = VT_TN_SUBNEGOTIATE ;
		negotiateLen = 2 ;
		memcpy ( negotiate, "\377\360", negotiateLen ) ;
		break ;

	      }
	      break ;

	    case VT_TN_GOAHEAD :
	    case VT_TN_ERASE_LINE :
	    case VT_TN_ERASE_CHAR :
	    case VT_TN_AREYOUTHERE :
	    case VT_TN_INTERRUPT :
	    case VT_TN_BREAK :
	    case VT_TN_DATAMARK :
	    case VT_TN_NOP :
	    case VT_TN_ENDSUBNEGOTIATE :

	    default :
	      t2 = VT_TN_WONT ;
	      break ;
	  }

	  if ( action == ACTION_EMIT ) {
	    /* Emit response */
	    pAction = gHyp_data_new ( "emit" ) ;
	    answerback[0] = c ;
	    answerback[1] = t2 ;
	    answerback[2] = c3 ;
	    answerback[3] = '\0' ;
	    for (i=0;i<negotiateLen;i++ ) answerback[i+3] = negotiate[i] ;
	    pData = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr_n ( pData, answerback, 3+negotiateLen ) ;
	    gHyp_data_append ( pAction, pData ) ;
	    gHyp_data_append ( pResult, pAction ) ;
	  }
	  action = ACTION_NONE ;
	  
	  break ;
	  
	/***********************************************************************/
	case VT_NUL :

	  /* Ignore */
	  action = ACTION_NONE ;
	  break ;

	/***********************************************************************/
	case VT_EOT :

	  /* Generate answerback message: */
	  action = ACTION_CARRIAGE_RETURN ;
	  break ;


	/***********************************************************************/
	case VT_ENQ :

	  /* Generate answerback message: */
	  action = ACTION_NONE ;
	  break ;

	/***********************************************************************/
	case VT_BEL :
	  
	  /* Ring a bell */
	  action = ACTION_BELL ;
	  break ;
	  
	/***********************************************************************/
	case VT_BS :

	  /* Back up one line */
	  action = ACTION_BACKSPACE ;
	  break ;

	/***********************************************************************/
	case VT_HT :

	  /* Next tab stop */
	  action = ACTION_TAB ;
	  break ;

	/***********************************************************************/
	case VT_LF :

	  /* If its the last linefeed in the stream, ignore it */
	  /*if ( ix == n ) break ;*/

	case VT_VT :
	case VT_FF :

	  /* Line feed and vertical tab */
	  if ( expectState == XPECT_END_DCS ) break ;
	  action = ACTION_LINEFEED ;
	  break ;

	/***********************************************************************/
	case VT_CR :

	  action = ACTION_CARRIAGE_RETURN ;
	  break ;

	/***********************************************************************/
	case VT_SO :

	  /* Invoke GL <-- G1 */
	  action = ACTION_GL_G1 ;
	  break ;


	/***********************************************************************/
	case VT_SI :

	  /* Invoke GL <-- G0 */
	  action = ACTION_GL_G0 ;
	  break ;

	/***********************************************************************/
	case VT_DC1 :
	  /* XON */
	  break ;

	/***********************************************************************/
	case VT_DC3 :

	  /* XOFF */
	  break ;

	/***********************************************************************/
	case VT_CAN :

	  /* Canel */
	  expectState = XPECT_CHARACTER ;
	  break ;

	/***********************************************************************/
	case VT_SUB :

	  /* Canel */
	  action = ACTION_CARRIAGE_RETURN ;
	  expectState = XPECT_CHARACTER ;
	  break ;

	/***********************************************************************/
	case VT_ESC :

	  /* Escape is never escaped, at least not in this algorithm  */
	  expectState = XPECT_ESCAPE_CODE ;
	  numeric_arg_count = 0 ;
	  gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	  numeric_arg_buffer[0] = '\0' ;
	  gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	  break ;


	/***********************************************************************/
	case VT_DEL :

	  /* Delete */
	  break ;


	/***********************************************************************/

	case VT_CSI :

	  /* Escape is never escaped, at least not in this algorithm  */
	  numeric_arg_count = 0 ;
	  gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	  numeric_arg_buffer[0] = '\0' ;
	  gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	  expectState = XPECT_CONTROL_CODE ;
	  break ;

	case '[' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    expectState = XPECT_CONTROL_CODE ;
	    numeric_arg_count = 0 ;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[[" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
        case ';' :

	  switch  ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>;" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE:

	    /* End of numeric argument */

	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;
	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	      numeric_arg_buffer[0] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;

	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case '(' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    expectState = XPECT_FINAL_G0 ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[(" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case ')' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    expectState = XPECT_FINAL_G1 ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[)" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break;


	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case '*' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    expectState = XPECT_FINAL_G2 ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[*" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '+' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    expectState = XPECT_FINAL_G3 ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[+" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break;


	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '<' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_ENTER_ANSI_MODE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[<" ) ;
	    action = ACTION_CHARACTER ;
	    break;

	  case XPECT_FINAL_G0 :
	    action = ACTION_ASCII_TO_G0 ;
	    break ;

	  case XPECT_FINAL_G1 :
	    action = ACTION_ASCII_TO_G1 ;
	    break ;

	  case XPECT_FINAL_G2 :
	    action = ACTION_ASCII_TO_G2 ;
	    break ;

	  case XPECT_FINAL_G3 :
	    action = ACTION_ASCII_TO_G3 ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '0' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>0" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }
	    break ;

	  case XPECT_FINAL_G0 :
	    action = ACTION_LINEDRAW_TO_G0 ;
	    break ;

	  case XPECT_FINAL_G1 :
	    action = ACTION_LINEDRAW_TO_G1 ;
	    break ;

	  case XPECT_FINAL_G2 :
	    action = ACTION_LINEDRAW_TO_G2 ;
	    break ;

	  case XPECT_FINAL_G3 :
	    action = ACTION_LINEDRAW_TO_G3 ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '1' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>1" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }	    
	    break ;

	  case XPECT_FINAL_G0 :
	    action = ACTION_QUICKCHART_TO_G0 ;
	    break ;

	  case XPECT_FINAL_G1 :
	    action = ACTION_QUICKCHART_TO_G1 ;
	    break ;

	  case XPECT_FINAL_G2 :
	    action = ACTION_QUICKCHART_TO_G2 ;
	    break ;

	  case XPECT_FINAL_G3 :
	    action = ACTION_QUICKCHART_TO_G3 ;
	    break ;


	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }
	  break ;


	/***********************************************************************/
	case '2' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>2" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }
	    break ;

	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }
	  break ;


	/***********************************************************************/
	case '3' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>3" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }	    
	    break ;

	  case XPECT_LINE_ATTR :
	    action = ACTION_DOUBLE_TOP ;
	    break ;

	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '4' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>4" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }	    
	    break ;

	  case XPECT_LINE_ATTR :
	    action = ACTION_DOUBLE_BOTTOM ;
	    break ;

	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '5' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>5" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }	    
	    break ;

	  case XPECT_LINE_ATTR :
	    action = ACTION_SINGLE_WIDTH ;
	    break ;

	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '6' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>6" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }	    
	    break ;

	  case XPECT_LINE_ATTR :
	    action = ACTION_DOUBLE_WIDTH ;
	    break ;

	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }
	  
	  break ;

	/***********************************************************************/
	case '7' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_SAVE_STATE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }	    
	    break ;

	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '8' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_RESTORE_STATE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }	    
	    break ;

	  case XPECT_LINE_ATTR :
	    action = ACTION_EEEE ;
	    break ;

	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case '9' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>9" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    /* Save the number */
	    i = strlen ( numeric_arg_buffer ) ;
	    if ( i < NUMERIC_ARG_WIDTH ) {
	      numeric_arg_buffer[i] = c ;
	      numeric_arg_buffer[i+1] = '\0' ;
	      gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    }	    
	    break ;

	  default:
	    // Part of a token value
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case 'A' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_CURSOR_UP ;
	    numeric_arg_count = 0 ;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 1 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 1 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    action = ACTION_CURSOR_UP ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'B' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    numeric_arg_count = 0 ;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    action = ACTION_CURSOR_DOWN ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :

	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 1 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 1 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    action = ACTION_CURSOR_DOWN ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_FINAL_G0 :
	    action = ACTION_ASCII_TO_G0 ;
	    break ;

	  case XPECT_FINAL_G1 :
	    action = ACTION_ASCII_TO_G1 ;
	    break ;

	  case XPECT_FINAL_G2 :
	    action = ACTION_ASCII_TO_G2 ;
	    break ;

	  case XPECT_FINAL_G3 :
	    action = ACTION_ASCII_TO_G3 ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'C' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    numeric_arg_count = 0 ;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    action = ACTION_CURSOR_RIGHT ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 1 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 1 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;

	    action = ACTION_CURSOR_RIGHT ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'D' :

	  switch ( expectState ) {;

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    numeric_arg_count = 0 ;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    action = ACTION_INDEX ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 1 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 1 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;

	    action = ACTION_CURSOR_LEFT ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'H' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_HORIZ_TAB_SET ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 1 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 1 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    
	    action = ACTION_CURSOR_MOVETO ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case 'J' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_ERASE_IN_DISPLAY ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;

	    action = ACTION_ERASE_IN_DISPLAY ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case 'K' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_ERASE_IN_LINE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    
	    action = ACTION_ERASE_IN_LINE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case 'X' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>X" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;

	    action = ACTION_ERASE_IN_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'f' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>f" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 1 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 1 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;


	    action = ACTION_CURSOR_MOVETO ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '~' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    /* Invoke GR <-- G1 */
	    action = ACTION_GR_G1 ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[~" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'c' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>c" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;
	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;

	    action = ACTION_DEVICE_REQUEST ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case 'n' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    /* Invoke GL <-- G2 */
	    action = ACTION_GL_G2 ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[n" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '}' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    /* Invoke GR <-- G2 */
	    action = ACTION_GR_G2 ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[}" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'o' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    /* Invoke GL <-- G3 */
	    action = ACTION_GL_G3 ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[o" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '|' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    /* Invoke GR <-- G3 */
	    action = ACTION_GR_G3 ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[|" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'N' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    /* Invoke GL <-- G2 */
	    action = ACTION_GL_G2_SHIFT ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[N" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'O' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    /* Invoke GL <-- G3 */
	    action = ACTION_GL_G3_SHIFT ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[O" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'E' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_NEXT_LINE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[E" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'M' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_REVERSE_INDEX ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[M" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'P' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Downloading DCS" ) ;
	    expectState = XPECT_END_DCS ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[P" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'Z' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    numeric_args[0] = 0 ;
	    numeric_arg_count = 1 ;
	    action = ACTION_DEVICE_REQUEST_VT52 ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[Z" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '>' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_EXIT_ALT_KEYPAD ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    actionModifier = TRUE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '/' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    action = ACTION_END_DCS ;
	    gHyp_util_logWarning ( "END DCS" ) ;
	    /*gHyp_data_check ( pScreen ) ;*/
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[/" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '\\' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "END DCS" ) ;
	    action = ACTION_END_DCS ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[\\" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'h' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>h" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    
	    action = ACTION_TERMINAL_SET_MODE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'l' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>l" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    
	    action = ACTION_TERMINAL_RESET_MODE ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '?' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>?" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    actionModifier = TRUE ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	case 'g' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>g" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;


	    action = ACTION_CLEAR_TAB ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'm' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>m" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) numeric_args[numeric_arg_count] = 0 ;

	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else
	      numeric_args[numeric_arg_count] = 0 ;
	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;
	    
	    action = ACTION_RENDITION ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case '#' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    expectState = XPECT_LINE_ATTR ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[#" ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/
	case 'r' :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>r" ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    if ( strlen ( numeric_arg_buffer ) > 0 ) {
	      i = sscanf ( numeric_arg_buffer,
			"%u", 
			(unsigned int*) &numeric_args[numeric_arg_count] ) ;
	      if ( i != 1 ) {
		if ( numeric_arg_count == 0 )
		  numeric_args[numeric_arg_count] = 1 ;
		else
		  numeric_args[numeric_arg_count] = numRows ;
	      }
	      gHyp_data_setVectorRaw ( pArgs, &numeric_args[numeric_arg_count], numeric_arg_count ) ;
	    }
	    else {
	      if ( numeric_arg_count == 0 )
		numeric_args[numeric_arg_count] = 1 ;
	      else
		numeric_args[numeric_arg_count] = numRows ;
	    }

	    numeric_arg_count++;
	    gHyp_data_setVectorRaw ( pArgc, &numeric_arg_count, 0 ) ;
	    numeric_arg_buffer[0] = '\0' ;
	    gHyp_data_setStr ( pArgb, numeric_arg_buffer ) ;


	    action = ACTION_SCROLL_REGION ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:
	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;


	/***********************************************************************/
	default :

	  switch ( expectState ) {

	  case XPECT_END_DCS:
	    break ;

	  case XPECT_ESCAPE_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>%c",c ) ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  case XPECT_CONTROL_CODE :
	    gHyp_util_logWarning ( "Unsupported <ESC>[%c",c ) ;
	    action = ACTION_CHARACTER ;
	    expectState = XPECT_CHARACTER ;
	    break ;

	  default:

	    expectState = XPECT_CHARACTER ;
	    action = ACTION_CHARACTER ;
	    break;
	  }

	  break ;

	/***********************************************************************/

	} /* End switch ( c ) */

      }  /* end  while ( action == ACTION_NONE  ) */

      gHyp_data_setVectorRaw ( pExpectState, &expectState, 0 ) ;


      if ( action == ACTION_NONE ) continue ;
      if ( action == ACTION_CHARACTER && (flags2 & VT_FLAGS2_WRAP_PENDING ))
	action = ACTION_CHARACTER_WRAP ;

      /*****************************************************************************
       *
       *
       * P E R F O R M   A C T I O N  
       *
       *
       ******************************************************************************/

      ir = rowMap [ row-1 ] ;
      ic = col-1 ;
 
      switch ( action ) {

      case ACTION_NONE :
	break ;

      case ACTION_SAVE_STATE :

	/* Save state - cursor, rendition, graphics */
	if (doDebug) gHyp_util_debug("SAVE STATE, (%u,%u)",row,col);   
	save_row = row ;
	save_col = col ;
	save_rendition = rendition ;
	save_graphics = graphics ;
	save_flags = flags ;
	gHyp_data_setVectorRaw ( pSaveCol, &save_col, 0 ) ;
	gHyp_data_setVectorRaw ( pSaveRow, &save_row, 0 ) ;
	gHyp_data_setVectorRaw ( pSaveRendition, &save_rendition, 0 ) ;
	gHyp_data_setVectorRaw ( pSaveGraphics, &save_graphics, 0 ) ;
	gHyp_data_setVectorRaw ( pSaveFlags, &save_flags, 0 ) ;
	gHyp_data_setVectorRaw ( pSaveFlags2, &save_flags2, 0 ) ;

	//save_shift_state = shift_state ;
	//save_selective_erase = selective_erase ;

	break ;

      case ACTION_RESTORE_STATE :

	/* Save state - cursor, rendition, graphics */
	row = save_row ;
	col = save_col ;
	rendition = save_rendition ;
	graphics = save_graphics ;
	flags = save_flags ;
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
	gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
	gHyp_data_setVectorRaw ( pRendition, &rendition, 0 ) ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;

	if (doDebug) gHyp_util_debug("RESTORE STATE, (%u,%u)",row,col);

	/* Set save state to defaults, so a restore without a
	 * previous save gets the defaults.
	 */
        save_row = 1 ;
	gHyp_data_setVectorRaw ( pSaveRow, &save_row, 0 ) ;
        save_col = 1 ;
	gHyp_data_setVectorRaw ( pSaveCol, &save_col, 0 ) ;
	save_rendition = RENDITION_STANDARD ;
	gHyp_data_setVectorRaw ( pSaveRendition, &save_rendition, 0 ) ;
        save_graphics = (unsigned char) VT_GRAPHICS_DEFAULT ;
        gHyp_data_setVectorRaw ( pSaveGraphics, &save_graphics, 0 ) ;
	save_flags = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL ;
        gHyp_data_setVectorRaw ( pSaveFlags, &save_flags, 0 ) ;
        save_flags2 = 0 ; 
        gHyp_data_setVectorRaw ( pSaveFlags2, &save_flags2, 0 ) ;

	//shift_state = save_shift_state ;
	//selective_erase = save_selective_erase ;

	break ;

      case ACTION_DEVICE_REQUEST_VT52 :
      case ACTION_DEVICE_REQUEST :

	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 0 ;
	}

	arg = numeric_args[0] ;

	if ( actionModifier ) {

	  /* Was a "CSI > c", secondary device request */
	  if ( arg == 0 ) {
	  
	    /* What we are?
	     * 2 - vt200 series
	     * 10 - support 132 column
	     * 0 - no options
	     * 
	     */

	    if ( doDebug ) gHyp_util_debug("DEVICE REQUEST REPORT" ) ; 
	    pAction = gHyp_data_new ( "emit" ) ;
	    strcpy ( answerback, "\033[>2;10;0c" ) ;

	    pData = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pData, answerback ) ;
	    gHyp_data_append ( pAction, pData ) ;
	    gHyp_data_append ( pResult, pAction ) ;
	  }

	}
	else {

	  /* Just a plain CSI c */
	  if ( arg == 0 ) {

	    /* What we support.
	     * 62  Service class 2 (VT200 family) 
	     * 1   132 col support
	     * 6   Selective erase
	     */

	    if ( doDebug ) gHyp_util_debug("DEVICE REQUEST SUPPORT" ) ; 
	    pAction = gHyp_data_new ( "emit" ) ;
	    strcpy ( answerback, "\033[?62;1;6c" ) ;

	    pData = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pData, answerback ) ;
	    gHyp_data_append ( pAction, pData ) ;
	    gHyp_data_append ( pResult, pAction ) ;

	  }
	  
	}
	break ;

      case ACTION_BELL :
	break ;

      case ACTION_TAB :
	/* Move to the next tab position */
	foundTabStop = FALSE ;
	for ( ic=col; ic<numCols; ic++ ) {
	  if ( pScreenFlags[ir][ic] & VT_FLAGS_TAB_STOP ) {
	    col = ic+1 ;
	    if ( doDebug ) gHyp_util_debug("TAB OVER TO COLUMN %d",col ) ; 
	    gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	    foundTabStop = TRUE ;
	    break ;
	  }
	}
	if ( !foundTabStop ) {
	  col = numCols ;
	  if ( doDebug ) gHyp_util_debug("TAB OVER TO COLUMN %d",col ) ; 
	  gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	  break ;
	}

	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
	break ;

      case ACTION_BACKSPACE :

	if ( col > 1 ) { 
	  col-- ;
	  gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	}

	/* Turn off wrap pending if set */
	if ( flags2 & VT_FLAGS2_WRAP_PENDING ) {
	  flags2 &= ~VT_FLAGS2_WRAP_PENDING ;
	  gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	}

	if (doDebug) gHyp_util_debug("BACKSPACE to (%u,%u)",row, col);

	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	break ;


      case ACTION_CURSOR_DOWN :

	/* Moves the cursor down, but NO SCROLLING */
	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 1 ;
	}

	arg = numeric_args[0] ;
	if ( arg == 0 ) arg = 1 ;

	if ( flags2 & VT_FLAGS2_ORIGIN ) {
	  originStart = startRow ;
	  originEnd = endRow ;
	}
	else {
	  originStart = 1 ;
	  originEnd = numRows ;
	}

	for ( k=0; k<arg;k++ ) {
	  if ( row < originEnd ) {
	    row++ ;
	    gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
   	    if (doDebug) gHyp_util_debug("CURSOR DOWN to (%u,%u)",row,col);
	  }
	}
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
	break ;

      case ACTION_CARRIAGE_RETURN :
	col = 1 ;
	if (doDebug) gHyp_util_debug("CARRIAGE RETURN to (%u,%u)",row,col);
	gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
        gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	/* Turn off wrap pending if set */
	if ( flags2 & VT_FLAGS2_WRAP_PENDING ) {
	  flags2 &= ~VT_FLAGS2_WRAP_PENDING ;
	  gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	}
	break ;

      case ACTION_CHARACTER :

	last_character = pScreenCharacter[ir][ic] ;
	last_rendition = pScreenRendition[ir][ic] ;
	last_flags     = pScreenFlags[ir][ic]  ;

	if ( !(last_flags & VT_FLAGS_MODIFIED) ) {

	  /* The cell has not been marked as modified. */

	  if ( c != ' ' ||
	       last_character != c ||
	       last_rendition != rendition ||
	       (last_flags&VT_FLAGS_G_MASK) != (flags&VT_FLAGS_G_MASK) ) {

	    /* Any non-blank character, or a rendition change, or graphics change */

	    last_flags |= VT_FLAGS_MODIFIED ;

  	    if ( doDebug )
	      gHyp_util_debug("CELL(%u,%u)(%u,%u) <== '%c'",row,col,ir,ic,c);

	    isModified = TRUE ;

	    /* Do we need to page? */

	    if ( !hasPaged ) {
	      
	      /* We have not paged yet.  Only one paging is allowed per input */

	      if ((last_flags & VT_FLAGS_PAGED) ) {

		/* We have not paged since the last time this cell was modified 
		 * Capture the page before we lose the information.
		 */

		if ( doDebug ) gHyp_util_debug("PAGING(%u,%u) (%u,%u) <== '%c'", row, col,ir, ic, c ); 
		gHyp_data_deleteValues ( pPage ) ;
		gHyp_data_append ( pPage, gHyp_data_copyAll ( pScreen ) ) ;
		gHyp_data_append ( pPage, gHyp_data_copyAll ( pOffScroll ) ) ;
		gHyp_data_deleteValues ( pOffScroll ) ;
		hasPaged = TRUE ;
	      }
	      else {
		/* Mark the cell as not-yet paged. */
		if ( doDebug ) gHyp_util_debug("MARKING(%u,%u) (%u,%u) <== '%c'", row, col,ir, ic, c ); 
		last_flags |= VT_FLAGS_PAGED ;
	      }
	    }
	  }
	}

	pScreenCharacter[ir][ic] = c ;
	pScreenRendition[ir][ic] = rendition ;
	pScreenFlags[ir][ic] = flags + (last_flags&VT_FLAGS_MASK);

	/* Advance to next col */
	if ( col < numCols ) {
	  col++ ;
	  gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	  chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	  gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	  /* Turn off wrap pending if set */
	  if ( flags2 & VT_FLAGS2_WRAP_PENDING ) {
	    flags2 &= ~VT_FLAGS2_WRAP_PENDING ;
	    gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	  }
	}
	else if ( (flags2 & VT_FLAGS2_WRAP) ) {

	  /* Wrap is set.  The first time we are at column 80/132, we
	   * don't wrap, its the next character that comes along that
	   * does it.
	   */
	  flags2 |= VT_FLAGS2_WRAP_PENDING ;
	  if ( doDebug ) gHyp_util_debug("WRAP PENDING");
	  gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	}

	/* Update character at new row,col */
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	break ;

      case ACTION_LINEFEED :
      case ACTION_NEXT_LINE :
      case ACTION_CHARACTER_WRAP :

	/* Goes to the first column on the next line. Scroll if necessary */
   	if (doDebug && action == ACTION_LINEFEED ) gHyp_util_debug("NEXT LINE");
   	if (doDebug && action == ACTION_NEXT_LINE ) gHyp_util_debug("NEXT LINE");
   	if (doDebug && action == ACTION_CHARACTER_WRAP ) gHyp_util_debug("CHARACTER WRAP");

	if ( action != ACTION_LINEFEED || (flags3 & VT_FLAGS3_NEWLINE) ) { 

   	  if (doDebug ) gHyp_util_debug("COL <== 1");
	  col = 1 ;
	  gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	  chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	  gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	  /* Turn off wrap pending if set */
	  if ( flags2 & VT_FLAGS2_WRAP_PENDING ) {
	    flags2 &= ~VT_FLAGS2_WRAP_PENDING ;
	    gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	  }
	}

	/* FALL THROUGH */
	numeric_arg_count = 0 ;

      case ACTION_INDEX :

	/* Moves cursor down one line in the same column, and scrolls if necessary */
	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 1 ;
	}
	arg = numeric_args[0] ;
	if ( arg == 0 ) arg = 1 ;

	if ( flags2 & VT_FLAGS2_ORIGIN ) {
	  originStart = startRow ;
	  originEnd = endRow ;
	}
	else {
	  originStart = 1 ;
	  originEnd = numRows ;
	}

	for ( k=0; k<arg;k++ ) {

	  if ( row != endRow && row < originEnd ) {
	    row++ ;
  	    if (doDebug) gHyp_util_debug("INDEX to (%u,%u)",row,col);
	    gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
	    chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	    gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
	  }

	  else {

          /*****************************************************************************
           *
           *
           * S C R O L L  U P 
           *
           *
           ******************************************************************************/

 	  if ( doDebug ) gHyp_util_debug("SCROLL up at (%u,%u)",row,col);

	  /* Step 1. Locate the first line of the scroll region, ready to roll it */
	  pStartScroll = gHyp_data_getChildBySS ( pScreen, startRow-1 ) ;

	  /*
	  pVector = gHyp_data_getChildByName ( pStartScroll,  "flags" ) ;
	  pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
	  for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	    pBuffer[ic] &= ~VT_FLAGS_PAGED ;
	  }
	  */

	  /* Step 2. Locate the last line of the scroll region */
	  pEndScroll = gHyp_data_getChildBySS ( pScreen, endRow-1 ) ;

	  /* Step 3. The offscroll row will have the same name as the one scrolled off */
	  pOffRow = gHyp_data_new ( gHyp_data_getLabel ( pStartScroll ) ) ;

 	  /* Step 4. Create the offscroll data */
	  ir = rowMap[startRow-1] ;

	  pVector = gHyp_data_new ( "character" ) ;
	  gHyp_data_newVector ( pVector, TYPE_CHAR, VT_MAX_COLS, FALSE ) ;
	  pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
	  for ( ic=0; ic<VT_MAX_COLS; ic++ ) *pBuffer++ = pScreenCharacter[ir][ic] ;
	  gHyp_data_append ( pOffRow, pVector ) ;

	  pVector = gHyp_data_new ( "rendition" ) ;
	  gHyp_data_newVector ( pVector, TYPE_UBYTE, VT_MAX_COLS, FALSE ) ;
	  pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
	  for ( ic=0; ic<VT_MAX_COLS; ic++ ) *pBuffer++ = pScreenRendition[ir][ic] ;
	  gHyp_data_append ( pOffRow, pVector ) ;

	  pVector = gHyp_data_new ( "flags" ) ;
	  gHyp_data_newVector ( pVector, TYPE_UBYTE, VT_MAX_COLS, FALSE ) ;
	  pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
	  for ( ic=0; ic<VT_MAX_COLS; ic++ ) *pBuffer++ = pScreenFlags[ir][ic] ;
	  gHyp_data_append ( pOffRow, pVector ) ;

	  /* Insert old row into off scroll area */
	  gHyp_data_insert ( pOffScroll, pOffRow ) ;


	  /* Step 5. Do the internal scrolling 
	   *
	   * Use the space occupied by the row we are scrolling off, which is the startRow
	   *
	   * Example:
	   *
	   * Scroll region StartRow=3, EndRow=7
	   *
	   * The rowMap tells us the internal array position
	   *
	   * rowMap  0   1   2   3   4   5   6   7   8    9  ....
	   * ----------------------------------------------------
	   * Row #   1   2   3   4   5   6   7   8   9   10    
	   *.----------------------------------------------------
	   * Internal
	   * -Before         4   5   6   7   8
	   * -After          5   6   7   8   4
	   *
	   * 1. Save value at rowMap[startRow-1]  = 4
	   * 2. Move values down from startRow+1 to endRow  = 5, 6, 7, 8
	   * 3. Restore value to rowMap[endRow-1] = 4
	   */

	  /* Erase the top row of the scrolling region before we move
	   * it to the bottom row.  Preserve the tab stops and the paged info
	   */
	  for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	    if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
		 pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		 (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {
	      pScreenCharacter[ir][ic] = ' ' ;
	      pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	      pScreenFlags[ir][ic] =
	        VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED + 
	        (pScreenFlags[ir][ic] & (VT_FLAGS_TAB_STOP+VT_FLAGS_PAGED));
	      isModified = TRUE ;
	    }
	  }
	  if ( isModified /*&& !doReset*/ ) {
	    lHyp_function_flushActions ( pResult,
				   numRows,
				   rowMap,
				   flags3,
				   numCols,
				   pScreenCharacter,
				   pScreenRendition,
				   pScreenFlags,
				   doDebug ) ;
	    isModified = FALSE ;
	  }

	  /* Take value from first row */
	  /* ir = rowMap[startRow-1]    (was moved higher up) */

	  /* Shift values down one */
	  for ( i=startRow; i<endRow; i++ ) {
	    rowMap [i-1] = rowMap[i] ;
	  }

	  /* Update last row */
	  rowMap[endRow-1] = ir ;

	  /* Step 6. Detach the first row */
	  gHyp_data_detach ( pStartScroll ) ;

	  /* Step 7. Re-attach it after the end row. */
	  gHyp_data_insertafter ( pEndScroll, pStartScroll ) ;

	  /*if ( !doReset ) {*/
	    /* Step 8: Create the action for javascript to roll the screen */
	    pAction = gHyp_data_new ( "scrollup" ) ;

	    pData = gHyp_data_new ( "startrow") ;
	    gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
	    gHyp_data_setVectorRaw ( pData, &startRow, 0 ) ;
	    gHyp_data_append ( pAction, pData ) ;

	    pData = gHyp_data_new ( "endrow") ;
	    gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
	    gHyp_data_setVectorRaw ( pData, &endRow, 0 ) ;
	    gHyp_data_append ( pAction, pData ) ;

	    gHyp_data_append ( pResult, pAction ) ;
	  /*}*/
	}
	}

	if ( action == ACTION_CHARACTER_WRAP ) {

	  ir = rowMap [ row-1 ] ;
	  ic = col-1 ;

	  last_character = pScreenCharacter[ir][ic] ;
	  last_rendition = pScreenRendition[ir][ic] ;
	  last_flags     = pScreenFlags[ir][ic]  ;


	  if ( !(last_flags & VT_FLAGS_MODIFIED) ) {
	    if ( c != ' ' ||
		 last_character != c ||
	         last_rendition != rendition ||
	         (last_flags&VT_FLAGS_G_MASK) != (flags&VT_FLAGS_G_MASK) ) {
	      last_flags |= VT_FLAGS_MODIFIED ;
  	      if ( doDebug ) gHyp_util_debug("CELLW(%u,%u) <== '%c'",row,col,c);
	      isModified = TRUE ;

	      /* Do we need to page? */
	      if ( !hasPaged ) {
		
		if ( ( last_flags & VT_FLAGS_PAGED ) ) {
		  /* We have not paged since the last time this cell was modified */
		  if ( doDebug ) gHyp_util_debug("PAGING(%u,%u) (%u,%u) <== '%c'", row, col,ir, ic, c ); 
		  gHyp_data_deleteValues ( pPage ) ;
		  gHyp_data_append ( pPage, gHyp_data_copyAll ( pScreen ) ) ;
		  gHyp_data_append ( pPage, gHyp_data_copyAll ( pOffScroll ) ) ;
		  gHyp_data_deleteValues ( pOffScroll ) ;
		  hasPaged = TRUE ;
		}
		else {
		  /* Mark the cell as not-yet paged. */
		  if ( doDebug ) gHyp_util_debug("MARKING(%u,%u) (%u,%u) <== '%c'", row, col,ir, ic, c ); 
		  last_flags |= VT_FLAGS_PAGED ;
		}
	      }	
	    }
	  }

 	  pScreenCharacter[ir][ic] = c ;
	  pScreenRendition[ir][ic] = rendition ;
	  pScreenFlags[ir][ic] = flags + (last_flags&VT_FLAGS_MASK);

	  col = 2 ;
	  gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;

	  /* Update character at new row, col */
	  chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	  gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	  /* Turn off wrap pending */
	  flags2 &= ~VT_FLAGS2_WRAP_PENDING ;
	  gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	}

	break ;
	
      case ACTION_REVERSE_INDEX :

	/* Moves cursor up one line in the same column, and scrolls if necessary */

	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 1 ;
	}

	arg = numeric_args[0] ;
	if ( arg == 0 ) arg = 1 ;


	if ( flags2 & VT_FLAGS2_ORIGIN ) {
	  originStart = startRow ;
	  originEnd = endRow ;
	}
	else {
	  originStart = 1 ;
	  originEnd = numRows ;
	}

	for ( k=0; k<arg;k++ ) {

	  if ( row != startRow && row > originStart ) {
	    row-- ;
	    gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
	    chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	    gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
   	    if (doDebug) gHyp_util_debug("CURSOR UP to (%u,%u)",row,col);
	  }
	  else {


          /*****************************************************************************
           *
           *
           * S C R O L L  D O W N
           *
           *
           ******************************************************************************/
 	  if (doDebug) gHyp_util_debug("SCROLL down at (%u,%u)",row,col);

	  /* Step 1. Locate the last line of the off-scroll region, ready to throw it away */
	  pEndScroll = gHyp_data_getChildBySS ( pScreen, endRow-1 ) ;

	  /* Step 2. Locate the first line of the scroll region */
	  pStartScroll = gHyp_data_getChildBySS ( pScreen, startRow-1 ) ;

	  /* Step 3. Scroll internally
	   *. 
	   * Use the space occupied by the row we are scrolling off, which is endRow. 
	   *
	   * Example:
	   *
	   * Scroll region startRow=3, EndRow=7
	   *
	   * rowMap  0   1   2   3   4   5   6   7   8    9  ....
	   * ----------------------------------------------------
	   * Row #   1   2   3   4   5   6   7   8   9   10    
	   *.
	   *
	   * Before          4   5   6   7   8
	   * After           8   4   5   6   7   
	   *
	   * 1. Save value at rowMap[endRow-1]  = 8
	   * 2. Move values from startRow+1 to endRow  = 5, 6, 7, 8
	   * 3. Restore value to rowMap[startRow-1] = 4
	   */

	  /* The last row will become the first row, */

	  /* Take value from last row */
	  ir = rowMap[endRow-1] ;

	   /* Erase the top row of the scrolling region before we move
	   * it to the bottom row.  Preserve the tab stops and PAGED information.
	   */
	  for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	    if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
		 pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		 (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {
	      pScreenCharacter[ir][ic] = ' ' ;
	      pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	      pScreenFlags[ir][ic] =
	        VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED + 
	        (pScreenFlags[ir][ic] & (VT_FLAGS_TAB_STOP+VT_FLAGS_PAGED));
	  
	      isModified = TRUE ;
	    }
	  }
	  if ( isModified /*&& !doReset */ ) {
	    lHyp_function_flushActions ( pResult,
				   numRows,
				   rowMap,
				   flags3,
				   numCols,
				   pScreenCharacter,
				   pScreenRendition,
				   pScreenFlags,
				   doDebug ) ;
	    isModified = FALSE ;
	  }

	  /* Shift values up one */
	  for ( i=endRow; i>startRow; i-- ) {
	    rowMap [i-1] = rowMap[i-2] ;
	  }

	  /* Update start row */
	  rowMap[startRow-1] = ir ;	  

	  /*if ( !doReset ) {*/

	    /* Create the action for javascript to roll the screen */
	    pAction = gHyp_data_new ( "scrolldown" ) ;

	    pData = gHyp_data_new ( "startrow") ;
	    gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
	    gHyp_data_setVectorRaw ( pData, &startRow, 0 ) ;
	    gHyp_data_append ( pAction, pData ) ;

	    pData = gHyp_data_new ( "endrow") ;
	    gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
	    gHyp_data_setVectorRaw ( pData, &endRow, 0 ) ;
	    gHyp_data_append ( pAction, pData ) ;

	    gHyp_data_append ( pResult, pAction ) ;

	  /*}*/

	  /* If there is off-screen area, take it .*/
	  pOffRow = gHyp_data_getLast ( pOffScroll ) ;
	  if ( pOffRow ) {

	    if ( doDebug ) gHyp_util_debug("OFFSCROLL TO ROW %d",startRow);

	    /* Update the top row with the offscroll data */
	    pVector = gHyp_data_getChildByName ( pOffRow, "character" ) ;
	    pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
	    for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	      if ( pScreenCharacter[ir][ic] != pBuffer[ic] ) { 
		pScreenCharacter[ir][ic] = pBuffer[ic] ;
		pScreenFlags[ir][ic] |= VT_FLAGS_MODIFIED ; ;
		isModified = TRUE ;
	      }
	    }

	    pVector = gHyp_data_getChildByName ( pOffRow, "rendition" ) ;
	    pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
	    for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	      if ( pScreenRendition[ir][ic] != pBuffer[ic] ) {
	        pScreenRendition[ir][ic] = pBuffer[ic] ; ;
	        pScreenFlags[ir][ic] |= VT_FLAGS_MODIFIED ; ;
	        isModified = TRUE ;
	      }
	    }

	    pVector = gHyp_data_getChildByName ( pOffRow,  "flags" ) ;
	    pBuffer = gHyp_data_buffer ( pVector, 0 ) ;
	    for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	      if ( (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (pBuffer[ic]&VT_FLAGS_G_MASK) ) {
	        pScreenFlags[ir][ic] = pBuffer[ic] ; ;
	        pScreenFlags[ir][ic] |= VT_FLAGS_MODIFIED ;
 	        isModified = TRUE ;
	      }
	    }

	    /* Delete the offscroll row */
	    gHyp_data_detach ( pOffRow ) ;
	    gHyp_data_delete ( pOffRow ) ;

	  }
	}
	}
	break ;


      case ACTION_CURSOR_UP :

	/* Moves the cursor up, but NO SCROLLING */

	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 1 ;
	}

	if ( flags2 & VT_FLAGS2_ORIGIN ) {
	  originStart = startRow ;
	  originEnd = endRow ;
	}
	else {
	  originStart = 1 ;
	  originEnd = numRows ;
	}


	arg = numeric_args[0] ;
	if ( arg == 0 ) arg = 1 ;

	for ( k=0; k<arg;k++ ) {
	  if ( row > originStart ) {
	    row-- ;
	    gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
   	    if (doDebug) gHyp_util_debug("CURSOR UP to (%u,%u)",row,col);
	  }
	}
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
	break ;

      case ACTION_CURSOR_RIGHT :
	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 1 ;
	}

	arg = numeric_args[0] ;
	if ( arg == 0 ) arg = 1 ;
	for ( k=0; k<arg;k++ ) {
	  if ( col < numCols ) {
 	    col++ ;
	    gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;

	    /* Turn off wrap pending if set */
	    if ( flags2 & VT_FLAGS2_WRAP_PENDING ) {
	      flags2 &= ~VT_FLAGS2_WRAP_PENDING ;
	      gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	    }
	  }
	}
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

  	if (doDebug) gHyp_util_debug("CURSOR RIGHT to (%u,%u)",row,col);	
	break ;

      case ACTION_CURSOR_LEFT :

	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 1 ;
	}

	arg = numeric_args[0] ;
	if ( arg == 0 ) arg = 1 ;
	for ( k=0; k<arg;k++ ) {
 	  if ( col > 1 ) {
	    col-- ;
	    gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	    /* Turn off wrap pending if set */
	    if ( flags2 & VT_FLAGS2_WRAP_PENDING ) {
	      flags2 &= ~VT_FLAGS2_WRAP_PENDING ;
	      gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	    }
	  }
	}
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
  	if (doDebug) gHyp_util_debug("CURSOR LEFT to (%u,%u)",row,col);
	break ;

      case ACTION_CURSOR_MOVETO :

	if ( flags2 & VT_FLAGS2_ORIGIN ) {
	  originStart = startRow ;
	  originEnd = endRow ;
	}
	else {
	  originStart = 1 ;
	  originEnd = numRows ;
	}
	numRows = originEnd - originStart + 1 ;

	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 2 ;
	  numeric_args[0] = 1 ;
	  numeric_args[1] = 1 ;
	}
	else if ( numeric_arg_count == 1 ) {
	  numeric_arg_count = 2 ;
	  numeric_args[1] = 1 ;
	}
	else if ( numeric_arg_count != 2 ) {
	  /*if (doDebug) gHyp_util_debug ( "moveto args not 2" ) */	;
	  numeric_arg_count = 2 ;
	  numeric_args[0] = 1 ;
	  numeric_args[1] = 1 ;
	}

	/* If in origin mode, and the scroll region is, for example, 3,21, then
	 * a cursor moveto ( 2,1 ) would be row 4. (orginStart + row - 1 );
	 */
	numeric_args[0] += (originStart - 1) ;

	/* Check args */
	if ( numeric_args[0] < originStart ) numeric_args[0] = originStart ;
	if ( numeric_args[0] > originEnd ) numeric_args[0] = originEnd ;
	if ( numeric_args[1] < 1 ) numeric_args[1] = 1 ;
	if ( numeric_args[1] > numCols ) numeric_args[1] = numCols ;

	/* Turn off wrap pending if set */
	if ( flags2 & VT_FLAGS2_WRAP_PENDING ) {
	  flags2 &= ~VT_FLAGS2_WRAP_PENDING ;
	  gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	}

	/* Set the new row and column */
	row = numeric_args[0] ;
	col = numeric_args[1] ;
	gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
	gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

  	if (doDebug) gHyp_util_debug("CURSOR MOVETO (%u,%u)",row,col);
	break ;

      case ACTION_HORIZ_TAB_SET :

	ic = col-1 ; 
	if ( doDebug ) gHyp_util_debug("TAB SET COLUMN %d",col ) ; 
	for ( i=1;i<=numRows;i++ ) {
	  ir = rowMap[i-1];
	  pScreenFlags[ir][ic] |= VT_FLAGS_TAB_STOP;
	}
	break ;

      case ACTION_CLEAR_TAB :

	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 0 ;
	}
	arg = numeric_args[0] ;

	if ( arg == 0 ) {

	  if ( doDebug ) gHyp_util_debug("CLEAR TAB AT COLUMN %d",col ) ; 

	  ic = col-1 ;
	  for ( i=1;i<=numRows;i++ ) {
	    ir = rowMap[i-1];
	    pScreenFlags[ir][ic] &= ~VT_FLAGS_TAB_STOP ;
	  }

	}
	else if ( arg == 3 ) {

	  if ( doDebug ) gHyp_util_debug("CLEAR ALL TABS" ) ; 
	  for ( i=1;i<=numRows;i++ ) {
	    ir = rowMap[i-1];
	    for ( ic=0;ic<VT_MAX_COLS;ic++ ) pScreenFlags[ir][ic] &= ~VT_FLAGS_TAB_STOP ;
	  }

	}
	break ;

      case ACTION_END_DCS :
	break ;

      case ACTION_TERMINAL_SET_MODE :

	/* Flags 

	    VT_FLAGS2_WRAP 1  When set, characters are wrapped 
	    VT_FLAGS2_ORIGIN 2  When set, terminal is in origin mode 

	    VT_FLAGS3_LOCKED  1	 When set, keyboard is locked 
	    VT_FLAGS3_INSERT  2 When set, terminal inserts characters, otherwise replaces 
	    VT_FLAGS3_ECHO    4 When set, terminal echos characters locally 
	    VT_FLAGS3_NEWLINE 8  When set, [Return] sends CR/LF and LF does a NEW_LINE 
	    VT_FLAGS3_132_COL 16  When set, terminal is in 132 column mode, otherwise 80 
	    VT_FLAGS3_SMOOTH_SCROLL 32   When set, smooth scroll, otherwise jump scroll 
	    VT_FLAGS3_REVERSE 64   When set, video is in reverse mode (white backgound)
	*/

	for ( i=0; i<numeric_arg_count; i++ ) {

	  k = numeric_args[i] ;

	  switch ( k ) {

	  case 2 :
	    if ( !actionModifier ) {
	      /*  Keyboard action */
	      if ( doDebug ) gHyp_util_debug("SET: KEYBOARD LOCKED");
	      flags3 |= VT_FLAGS3_LOCKED ;
	    }
	    break ;

	  case 3 :
	    if ( actionModifier ) {

	      /* Set cursor to HOME position */
	      col = 1 ;
  	      gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	      if ( (flags2 & VT_FLAGS2_ORIGIN) )
		row = startRow ;
	      else
		row = 1 ;
  	      gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;		
	      chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	      gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	      /* Clear the screen area whenever we scale up to 132 column */
	      for ( i=1;i<=numRows;i++ ) {

		ir = rowMap[i-1];
		for ( ic=0; ic<VT_MAX_COLS; ic++ ) {

		if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
		     pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		     (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {

		    pScreenCharacter[ir][ic] = ' ' ;
		    pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
		    pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
		      (pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
		    isModified = TRUE ;
		  }
		}
	      }
	      
	      if ( isModified ) {
		lHyp_function_flushActions ( pResult,
				   numRows,
				   rowMap,
				   flags3,
				   numCols,
				   pScreenCharacter,
				   pScreenRendition,
				   pScreenFlags,
				   doDebug ) ;
		isModified = FALSE ;
	      }
	      

	      /*  Screen width */
	      flags3 |= VT_FLAGS3_132_COL ;
	      numCols = VT_132_COL ;
	      if ( doDebug ) gHyp_util_debug("SET: 132 COL");
	    }
	    break ;

	  case 4 :
	    if ( actionModifier ) {
	      /* Smooth/jump */
	      flags3 |= VT_FLAGS3_SMOOTH_SCROLL ;
	      if ( doDebug ) gHyp_util_debug("SET: SMOOTH SCROLL");
	    }
	    else {
	      /* Insert/replace */
	      if ( doDebug ) gHyp_util_debug("INSERT MODE");
	      flags3 |= VT_FLAGS3_INSERT ;
	    }
	    break ;

	  case 5 :
	    if ( actionModifier ) {
	      /* Reverse/normal */
	      if ( doDebug ) gHyp_util_debug("SET: REVERSE VIDEO");

	      if ( !(flags3&VT_FLAGS3_REVERSE )) {
		for ( ir=0;ir<numRows;ir++ )
		  for ( ic=0; ic<VT_MAX_COLS; ic++ ) 
		    pScreenFlags[ir][ic] |= VT_FLAGS_MODIFIED ;
		isModified = TRUE ;
	      }
	      flags3 |= VT_FLAGS3_REVERSE ;
	    }
	    break ;

	  case 6 :
	    if ( actionModifier ) {
	      /* Origin/Absolute */
	      if ( doDebug ) gHyp_util_debug("SET: ORIGIN MODE");
	      flags2 |= VT_FLAGS2_ORIGIN ;
	      /* Contain row */
	      row = startRow ;
	      gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;
	      col = 1 ;
	      gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	      chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	      gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;
	    }
	    break ;

	  case 7 :
	    if ( actionModifier ) {
	      /* Wrap/nowrap */
	      if ( doDebug ) gHyp_util_debug("SET: WRAP");
	      flags2 |= VT_FLAGS2_WRAP ;
	    }
	    break ;

	  case 12:
	    if ( !actionModifier ) {
	      /* Send/recieve  (echo/noecho) */
	      if ( doDebug ) gHyp_util_debug("SET: NOECHO");
	      flags3 &= ~VT_FLAGS3_ECHO ;
	    }
	    break ;

	  case 20 :
	    if ( !actionModifier ) {
	      /* linefeed / newline */
	      if ( doDebug ) gHyp_util_debug("NEWLINE MODE");
	      flags3 |= VT_FLAGS3_NEWLINE ;
	    }
	    break ;
	  }

	}
	gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	gHyp_data_setVectorRaw ( pFlags3, &flags3, 0 ) ;

	break ;

      case ACTION_TERMINAL_RESET_MODE :

	for ( i=0; i<numeric_arg_count; i++ ) {

	  k = numeric_args[i] ;

	  switch ( k ) {

	  case 2 :
	    if ( !actionModifier ) {
	      /*  Keyboard action */
	      flags3 &= ~VT_FLAGS3_LOCKED ;
	      if ( doDebug ) gHyp_util_debug("RESET: KEYBOARD UNLOCKED");
	    }
	    break ;

	  case 3 :

	    if ( actionModifier ) {

	      /* Set cursor to HOME position */
	      col = 1 ;
  	      gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	      if ( (flags2 & VT_FLAGS2_ORIGIN) )
	        row = startRow ;
	      else
		row = 1 ;
  	      gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;		
	      chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	      gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	      /* Clear the screen area whenever we scale down to 80 column */
	      for ( i=1;i<=numRows;i++ ) {

		ir = rowMap[i-1];

		for ( ic=0; ic<VT_MAX_COLS; ic++ ) {

	          if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
		       pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		       (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {

		    pScreenCharacter[ir][ic] = ' ' ;
		    pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
		    pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
		      (pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
		    isModified = TRUE ;
		  }
		}
	      }
	      
	      if ( isModified ) {
		lHyp_function_flushActions ( pResult,
				   numRows,
				   rowMap,
				   flags3,
				   numCols,
				   pScreenCharacter,
				   pScreenRendition,
				   pScreenFlags,
				   doDebug ) ;
		isModified = FALSE ;
	      }
	      

	      /*  Screen width */
	      flags3 &= ~VT_FLAGS3_132_COL ;
	      if ( doDebug ) gHyp_util_debug("RESET: 80 COL");
	      numCols = VT_80_COL ;

	    }
	    break ;

	  case 4 :
	    if ( actionModifier ) {
	      /* Smooth/jump */
	      flags3 &= ~VT_FLAGS3_SMOOTH_SCROLL ;
	      if ( doDebug ) gHyp_util_debug("RESET: JUMP SCROLL");
	    }
	    else {
	      /* Insert/replace */
	      if ( doDebug ) gHyp_util_debug("RESET: REPLACE MODE");
	      flags3 &= ~VT_FLAGS3_INSERT ;
	    }
	    break ;

	  case 5 :
	    if ( actionModifier ) {

	      /* Reverse/normal - default is BLACK screen, white characters -
	       * this is flags3=0 for us.   flags3=64 means REVERSE video, which is
	       * white background, black characters.   
	       * For us, the FLAGS_REVERSE_SET is a white background), and this is not the norm, 
	       * because we want to have flags3=0 for a terminal reset to its default state.
	       */
	      if ( doDebug ) gHyp_util_debug("RESET: NORMAL VIDEO");
	      if ( (flags3&VT_FLAGS3_REVERSE )) {
		for ( ir=0;ir<numRows;ir++ )
		  for ( ic=0; ic<VT_MAX_COLS; ic++ ) 
		    pScreenFlags[ir][ic] |= VT_FLAGS_MODIFIED ;
		isModified = TRUE ;
	      }
	      flags3 &= ~VT_FLAGS3_REVERSE ;
	    }
	    break ;

	  case 6 :
	    if ( actionModifier ) {
	      /* Origin/Absolute */
	      if ( doDebug ) gHyp_util_debug("RESET: ABSOLUTE MODE");
	      flags2 &= ~VT_FLAGS2_ORIGIN ;
	    }
	    break ;

	  case 7 :
	    if ( actionModifier ) {
	      /* Wrap/nowrap */
	      if ( doDebug ) gHyp_util_debug("RESET: NOWRAP");
	      flags2 &= ~VT_FLAGS2_WRAP ;
	    }
	    break ;

	  case 12:
	    if ( !actionModifier ) {
	      /* Send/recieve  (echo/noecho) */
	      if ( doDebug ) gHyp_util_debug("RESET: ECHO MODE");
	      flags3 |= VT_FLAGS3_ECHO ;
	    }
	    break ;

	  case 20 :
	    if ( !actionModifier ) {
	      /* linefeed / newline */
	      if ( doDebug ) gHyp_util_debug("RESET: LINEFEED MODE");
	      flags3 &= ~VT_FLAGS3_NEWLINE ;
	    }
	    break ;
	  }
	}
	gHyp_data_setVectorRaw ( pFlags2, &flags2, 0 ) ;
	gHyp_data_setVectorRaw ( pFlags3, &flags3, 0 ) ;

	break ;


      case ACTION_GL_G0 :

	flags &= (~VT_FLAGS_GL_MASK) ;
	flags |= ( graphics & VT_GRAPHICS_G0_MASK ) ;

	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	/*if (doDebug) gHyp_util_debug("Graphics GL<-G0");*/
	break ;

      case ACTION_GL_G1 :

	flags &= ~VT_FLAGS_GL_MASK ;
	flags |= ( ( graphics & VT_GRAPHICS_G1_MASK ) >> 2 ) ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	/*if (doDebug) gHyp_util_debug("Graphics GL<-G1");*/
	break ;

      case ACTION_GL_G2 :
	flags &= ~VT_FLAGS_GL_MASK ;
	flags |= ( ( graphics & VT_GRAPHICS_G2_MASK ) >> 4 ) ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	/*if (doDebug) gHyp_util_debug("Graphics GL<-G2");*/
	break ;

      case ACTION_GL_G3 :
	flags &= ~VT_FLAGS_GL_MASK ;
	flags |= ( ( graphics & VT_GRAPHICS_G3_MASK ) >> 6 ) ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	/*if (doDebug) gHyp_util_debug("Graphics GL<-G3");*/
	break ;

      case ACTION_GR_G0 :
	flags &= ~VT_FLAGS_GR_MASK ;
	flags |= ( ( graphics & VT_GRAPHICS_G0_MASK ) << 2 ) ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	break ;

      case ACTION_GR_G1 :
	flags &= ~VT_FLAGS_GR_MASK ;
	flags |= ( graphics & VT_GRAPHICS_G1_MASK ) ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	break ;

      case ACTION_GR_G2 :
	flags &= ~VT_FLAGS_GR_MASK ;
	flags |= ( ( graphics & VT_GRAPHICS_G2_MASK ) >> 2 ) ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	break ;

      case ACTION_GR_G3 :
	flags &= ~VT_FLAGS_GR_MASK ;
	flags |= ( ( graphics & VT_GRAPHICS_G2_MASK ) >> 4 ) ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	break ;

      case ACTION_GL_G2_SHIFT :
	flags &= ~VT_FLAGS_GL_MASK ;
	flags |= ( ( graphics & VT_GRAPHICS_G2_MASK ) >> 4 ) ;
	isShift = TRUE ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	break ;

      case ACTION_GL_G3_SHIFT :
	flags &= ~VT_FLAGS_GL_MASK ;
	flags |= ( ( graphics & VT_GRAPHICS_G3_MASK ) >> 6 ) ;
	isShift = TRUE ;
	gHyp_data_setVectorRaw ( pFlags, &flags, 0 ) ;
	break ;

      case ACTION_ASCII_TO_G0 :
	graphics &= ~VT_GRAPHICS_G0_MASK ;
	graphics |= VT_GRAPHICS_ASCII ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_ASCII_TO_G1 :
	graphics &= ~VT_GRAPHICS_G1_MASK ;
	graphics |= ( VT_GRAPHICS_ASCII << 2 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_ASCII_TO_G2 :
	graphics &= ~VT_GRAPHICS_G2_MASK ;
	graphics |= ( VT_GRAPHICS_ASCII << 4 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_ASCII_TO_G3 :
	graphics &= ~VT_GRAPHICS_G3_MASK ;
	graphics |= ( VT_GRAPHICS_ASCII << 6 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_LINEDRAW_TO_G0 :
	graphics &= ~VT_GRAPHICS_G0_MASK ;
	graphics |= VT_GRAPHICS_LINEDRAW ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_LINEDRAW_TO_G1 :
	graphics &= ~VT_GRAPHICS_G1_MASK ;
	graphics |= ( VT_GRAPHICS_LINEDRAW << 2 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_LINEDRAW_TO_G2 :
	graphics &= ~VT_GRAPHICS_G2_MASK ;
	graphics |= ( VT_GRAPHICS_LINEDRAW << 4 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_LINEDRAW_TO_G3 :
	graphics &= ~VT_GRAPHICS_G3_MASK ;
	graphics |= ( VT_GRAPHICS_LINEDRAW << 6 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_QUICKCHART_TO_G0 :
	graphics &= ~VT_GRAPHICS_G0_MASK ;
	graphics |= VT_GRAPHICS_QUICKCHART ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_QUICKCHART_TO_G1 :
	graphics &= ~VT_GRAPHICS_G1_MASK ;
	graphics |= ( VT_GRAPHICS_QUICKCHART << 2 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_QUICKCHART_TO_G2 :
	graphics &= ~VT_GRAPHICS_G2_MASK ;
	graphics |= ( VT_GRAPHICS_QUICKCHART << 4 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_QUICKCHART_TO_G3 :
	graphics &= ~VT_GRAPHICS_G3_MASK ;
	graphics |= ( VT_GRAPHICS_QUICKCHART << 6 ) ;
	gHyp_data_setVectorRaw ( pGraphics, &graphics, 0 ) ;
	break ;

      case ACTION_DOUBLE_TOP :
	break ;

      case ACTION_DOUBLE_BOTTOM :
	break ;

      case ACTION_SINGLE_WIDTH :
	break ;

      case ACTION_DOUBLE_WIDTH :
	break ;

      case ACTION_EEEE :

	for ( i=1;i<=numRows;i++ ) {
	  ir = rowMap[i-1];
	  for ( ic=0; ic<numCols; ic++ ) {
	    pScreenCharacter[ir][ic] = 'E' ;
	    pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	    if ( ic%8 == 0 )
	      pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + 
		VT_FLAGS_MODIFIED + VT_FLAGS_TAB_STOP;
	    else
	      pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + 
		VT_FLAGS_MODIFIED ;
	    isModified = TRUE ;
	  }
	}

	/* Set cursor to HOME position */
	col = 1 ;
  	gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	if ( (flags2 & VT_FLAGS2_ORIGIN) )
	  row = startRow ;
	 else
	  row = 1 ;
  	 gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;		
	 chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	 gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	/* Clear rendition */
	rendition = 0 ;
	gHyp_data_setVectorRaw ( pRendition, &rendition, 0 );

	break ;

      case ACTION_RENDITION :

        /*****************************************************************************
         *
         *
         * A P P L Y  R E N D I T I O N 
         *
         *
         ******************************************************************************/
	/*
             RENDITION_STANDARD 0
             RENDITION_BOLD 1
             RENDITION_REVERSE 2
             RENDITION_UNDERSCORE 4
             RENDITION_BLINKING 8
             RENDITION_NOBOLD 16
             RENDITION_NOREVERSE 32
             RENDITION_NOUNDERSCORE 64
             RENDITION_NOBLINKING 128
	*/

	/* What's the current rendition? Initialize to STANDARD */
	ir = rowMap[row-1] ;
	ic = col-1 ;

	last_rendition = pScreenRendition[ir][ic] ;

	/* Calculate a new rendition */

	/* If no arguments, default is 0 */
	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = RENDITION_STANDARD ;
	}

	rendition = RENDITION_STANDARD ;

	for ( i=0; i<numeric_arg_count; i++ ) {

	  k = numeric_args[i] ;

	  switch ( k ) {

	  case 0 :
	    rendition = RENDITION_STANDARD ;
	    break ;

	  case 1 :
	    rendition &= ~RENDITION_NOBOLD ;
	    rendition |= RENDITION_BOLD ;
	    break ;

	  case 4 :
	    rendition &= ~RENDITION_NOUNDERSCORE ;
	    rendition |= RENDITION_UNDERSCORE ;
	    break ;

	  case 5 :
	    rendition &= ~RENDITION_NOBLINKING ;
	    rendition |= RENDITION_BLINKING ;
	    break ;

	  case 7 :
	    rendition &= ~RENDITION_NOREVERSE ;
	    rendition |= RENDITION_REVERSE ;
	    break ;

	  case 22 :
	    rendition &= ~RENDITION_BOLD ;
	    rendition |= RENDITION_NOBOLD ;
	    break ;

	  case 24 :
	    rendition &= ~RENDITION_UNDERSCORE ;
	    rendition |= RENDITION_NOUNDERSCORE ;
	    break ;

	  case 25 :
	    rendition &= ~RENDITION_BLINKING ;
	    rendition |= RENDITION_NOBLINKING ;
	    break ;

	  case 27 :
	    rendition &= ~RENDITION_REVERSE ;
	    rendition |= RENDITION_NOREVERSE ;
	    break ;

	  }
  	  /*if (doDebug) gHyp_util_debug("RENDITION SET to %x",rendition );*/
	}
	break ;

      case ACTION_SCROLL_REGION :

	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 2 ;
	  numeric_args[0] = 1 ;
	  numeric_args[1] = numRows ;
	}
	else if ( numeric_arg_count == 1 ) {
	  numeric_arg_count = 2 ;
	  numeric_args[1] = numRows ;
	}

	startRow = numeric_args[0] ;
	endRow = numeric_args[1] ;

	if ( startRow < 1 ) startRow = 1 ;
	if ( startRow > numRows ) startRow = numRows ;
	if ( endRow < 1 ) endRow = 1 ;
	if ( endRow > numRows ) endRow = numRows ;
	if ( endRow < startRow ) endRow = startRow ;
	gHyp_data_setVectorRaw ( pStartRow, &startRow, 0 ) ;
	gHyp_data_setVectorRaw ( pEndRow, &endRow, 0 ) ;

  	if (doDebug) gHyp_util_debug("SCROLL REGION [%u...%u]",startRow,endRow );

	/* Set cursor to HOME position */
	col = 1 ;
  	gHyp_data_setVectorRaw ( pCol, &col, 0 ) ;
	if ( (flags2 & VT_FLAGS2_ORIGIN) )
	  row = startRow ;
        else
	  row = 1 ;
  	gHyp_data_setVectorRaw ( pRow, &row, 0 ) ;		
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	break ;

      case ACTION_ERASE_IN_CHARACTER :

	/* If no arguments, default is 1 */
	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 1 ;
	}

	m = numeric_args[0] ;
  	if (doDebug) gHyp_util_debug("ERASE IN CHARACTER from (%u,%u), arg=%u",row,col,m );

	k = col-1 ;
	for ( i=row;i<=numRows;i++ ) {

	  ir = rowMap[i-1];

	  for ( ic=k; ic<VT_MAX_COLS; ic++ ) {
	    k = 0 ;
	    if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
		 pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		 (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {
	      pScreenCharacter[ir][ic] = ' ' ;
	      pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	      pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
	        (pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
	      isModified = TRUE ;
	    }
	    m-- ;
	    if ( m==0 ) break ;
	  }
	  if ( m==0 ) break ;
	}
	/*
	if ( isModified ) {
	  lHyp_function_flushActions ( pResult,
				   numRows,
				   rowMap,
				   flags3,
				   numCols,
				   pScreenCharacter,
				   pScreenRendition,
				   pScreenFlags,
	  isModified = FALSE ;
	}
	*/

	/* Clear rendition */
	rendition = 0 ;
	gHyp_data_setVectorRaw ( pRendition, &rendition, 0 );
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	break ;

      case ACTION_ERASE_IN_LINE :

	/* If no arguments, default is 0 */
	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 0 ;
	}

	m = numeric_args[0] ;
  	if (doDebug) gHyp_util_debug("ERASE IN LINE from (%u,%u), arg=%u",row,col,m );

	if ( m == 0 ) {

	  ir = rowMap[row-1] ;
	  for ( ic=col-1;ic<VT_MAX_COLS;ic++ ) {
	    if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
		 pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		 (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {
	      pScreenCharacter[ir][ic] = ' ' ;
	      pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	      pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
		(pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
	      isModified = TRUE ;
	    }
	  }
	}
	else if ( m == 1 ) {

	  ir = rowMap[row-1] ;
	  for ( ic=0;ic<col;ic++ ) {
	    if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
		 pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		 (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {

	      pScreenCharacter[ir][ic] = ' ' ;
	      pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	      pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
	        (pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
	      isModified = TRUE ;
	    }
	  }
	}
	else if ( m == 2 ) {

	  ir = rowMap[row-1] ;
	  for ( ic=0;ic<VT_MAX_COLS;ic++ ) {
    	    if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
		 pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		 (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {

	      pScreenCharacter[ir][ic] = ' ' ;
	      pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	      pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
	        (pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
	      isModified = TRUE ;
	    }
	  }
	}
	/*
	if ( isModified ) {
	  lHyp_function_flushActions ( pResult,
				   numRows,
				   rowMap,
				   flags3,
				   numCols,
				   pScreenCharacter,
				   pScreenRendition,
				   pScreenFlags) 
	  isModified = FALSE ;
	}
	*/
	/* Clear rendition */
	rendition = 0 ;
	gHyp_data_setVectorRaw ( pRendition, &rendition, 0 );
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	break ;

      case ACTION_ERASE_IN_DISPLAY :

	/* If no arguments, default is 0 */
	if ( numeric_arg_count == 0 ) {
	  numeric_arg_count = 1 ;
	  numeric_args[0] = 0 ;
	}

	m = numeric_args[0] ;
  	if (doDebug) gHyp_util_debug("ERASE IN DISPLAY from (%u,%u), arg=%u",row,col,m );

	if ( m == 0 ) {

	  k = col-1 ;
	  for ( i=row;i<=numRows;i++ ) {
	    ir = rowMap[i-1];
	    for ( ic=k; ic<VT_MAX_COLS; ic++ ) {
	      k = 0 ;
	      if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
	 	   pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		   (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {
	        pScreenCharacter[ir][ic] = ' ' ;
	        pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	        pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
		    (pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
	        isModified = TRUE ;
	      } 
	    }
	  }
	}
	else if ( m == 1 ) {

	  k = VT_MAX_COLS ; ;
	  for ( i=1;i<=row;i++ ) {
	    ir = rowMap[i-1];
	    if ( i==row ) k = col ;
	    for ( ic=0; ic<k; ic++ ) {
	      if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
	 	   pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		   (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {
	        pScreenCharacter[ir][ic] = ' ' ;
	        pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	        pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
		  (pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
	        isModified = TRUE ;
	      }
	    }
	  }
	}
	else if ( m == 2 ) {
 
	  for ( i=1;i<=numRows;i++ ) {
	    ir = rowMap[i-1];
	    for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	      if ( doErase || pScreenCharacter[ir][ic] != ' ' ||
	 	   pScreenRendition[ir][ic] != RENDITION_STANDARD ||
		   (pScreenFlags[ir][ic]&VT_FLAGS_G_MASK) != (VT_FLAGS_GL_ASCII+VT_FLAGS_GR_SUPPLEMENTAL)) {
	        pScreenCharacter[ir][ic] = ' ' ;
	        pScreenRendition[ir][ic] = RENDITION_STANDARD ; ;
	        pScreenFlags[ir][ic] = VT_FLAGS_GL_ASCII + VT_FLAGS_GR_SUPPLEMENTAL + VT_FLAGS_MODIFIED +
		  (pScreenFlags[ir][ic]&VT_FLAGS_TAB_STOP);
	        isModified = TRUE ;
	      }
	    }
	  }
	}
	/*
	if ( isModified ) {
	  lHyp_function_flushActions ( pResult,
				   numRows,
				   rowMap,
				   flags3,
				   numCols,
				   pScreenCharacter,
				   pScreenRendition,
				   pScreenFlags ) ;
	  isModified = FALSE ;
	}
	*/
	/* Clear rendition */
	rendition = 0 ;
	gHyp_data_setVectorRaw ( pRendition, &rendition, 0 );
	chr = pScreenCharacter[rowMap[row-1]][col-1] ;
	gHyp_data_setVectorRaw ( pChr, &chr, 0 ) ;

	break ;


      case ACTION_ENTER_ANSI_MODE :
	break ;

      case ACTION_EXIT_ALT_KEYPAD :
	break ;

      default:
	gHyp_util_debug ( "No action??" ) ;
      
      } /* End switch ( action ) { */

      numeric_arg_count = 0 ;
      action = ACTION_NONE ;
      actionModifier = FALSE ;

    } /* end loop over pStream */

    if ( context== -2 && ss != -1 )
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in vt2html()",ss) ;

    if ( pLine == NULL ) break ;

  } /* End block */

  if ( hasPaged ) {
    for ( ir=0;ir<numRows;ir++ ) {
      for ( ic=0; ic<VT_MAX_COLS; ic++ ) {
	/* Clear the page. */
        pScreenFlags[ir][ic] &= ~VT_FLAGS_PAGED ;
      }
    }
  }

  if ( isModified ) {

    if ( doDebug ) gHyp_util_debug("Flush %d rows, %d cols",numRows,numCols);
    lHyp_function_flushActions ( pResult,
  			 numRows,
			 rowMap,
			 flags3,
			 numCols,
			 pScreenCharacter,
			 pScreenRendition,
			 pScreenFlags,
			 doDebug ) ;
  }


  return pResult ;
}

void gHyp_function_vt2html ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: vt2html ( string, doReset, doDebug )
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
      *pValue,
      *pVTstate,
      *pResult ;
    
    sLOGICAL
      doReset,
      doDebug ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 4 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: vt2html ( input, state, doReset, doDebug )" ) ;

    pValue = gHyp_stack_popRvalue ( pStack, pAI ) ;
    doDebug = gHyp_data_getInt ( pValue, gHyp_data_getSubScript ( pValue), TRUE ) ;

    pValue = gHyp_stack_popRvalue ( pStack, pAI ) ;
    doReset = gHyp_data_getInt ( pValue, gHyp_data_getSubScript ( pValue), TRUE ) ;

    pVTstate = gHyp_stack_popRdata ( pStack, pAI ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;

    pResult = lHyp_function_vt2html ( pAI, pData, pVTstate, doReset, doDebug ) ;
    
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_round ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: round ( value, precision )
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
      *pValue,
      *pValue2,
      *pResult ;
    
    int
      precision,
      ss,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      strVal[VALUE_SIZE+1] ;

    sLOGICAL
      isVector ;

    double
      doubleValue ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: round ( value, precision )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    precision = gHyp_data_getInt ( pData,
				 gHyp_data_getSubScript ( pData),
				 TRUE ) ;

    pResult = gHyp_data_new ( "_round_" ) ;

    if ( precision < 0 || precision > 12 ) {
      gHyp_instance_warning ( pAI, "STATUS_BOUNDS", "Precision not between 1 and 12, it is %d ", precision ) ;
      precision = 7 ;
    }

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
 
    isVector = (gHyp_data_getDataType(pData) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {

      doubleValue = gHyp_data_getDouble ( pValue, ss, FALSE ) ;
      sprintf ( strVal, "%.*g", precision, doubleValue ) ;
      pValue2 = gHyp_data_new ( NULL ) ;
      gHyp_data_setToken ( pValue2, strVal ) ; 
      gHyp_data_append ( pResult, pValue2 ) ;
      
    }
    if ( context== -2 && ss != -1 )
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in round()",ss) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_function_parseurl ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: parseurl ( string )
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
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      name[VALUE_SIZE+1],
      *p,
      *after_access,
      *orig,
      *dest,
      *pAccess=NULL,
      *pHost=NULL,
      *pAbsolute=NULL,
      *pRelative=NULL,
      *pArgs=NULL,
      *pFragment=NULL,
      *pPort=NULL ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: parseurl ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   name, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;

    pResult = gHyp_data_new ( "_url_" ) ;

    p = after_access = name ;

    /* Look for fragment identifier */
    if ((p = strchr(name, '#')) != NULL) {
      *p++ = '\0';
      pFragment = p;
    }
    
    /* Look for args identifier */
    if ((p = strchr(name, '?')) != NULL) {
      *p++ = '\0';
      pArgs = p;
    }
    
    if ((p = strchr(name, ' ')) != NULL) *p++ = '\0';    
    
    for (p=name; *p; p++) {

      /* 
       * Look for any whitespace. This is very bad for pipelining as it
       * makes the request invalid
       */
      if ( isspace ( (int) *p) ) {

	orig = p ;
	dest = p+1 ;
	while ( (*orig++ = *dest++) ) ;
	p = p-1;
      }

      if (*p=='/' || *p=='#' || *p=='?') break ;

      if (*p==':') {

	*p = 0;
	pAccess = after_access; /* Scheme has been specified */

	after_access = p+1;

	gHyp_util_lowerCase ( pAccess, strlen ( pAccess ) ) ;
	if (0==strcmp("url", pAccess)) {

	  pAccess = NULL;  /* Ignore IETF's URL: pre-prefix */
	} 
	else 
	  break;
      }
    }
    
    p = after_access;
    if ( *p == '/' ) {

      if ( p[1] == '/' ) {

	pHost = p+2;		  /* host has been specified 	*/
	*p = 0;			  /* Terminate access 		*/
	p = strchr( pHost,'/');	  /* look for end of host name if any */

	if ( p ) {

	  *p=0;			/* Terminate host */
	  pAbsolute = p+1;		/* Root has been found */
	}

      } 
      else {
	pAbsolute = p+1;		/* Root found but no host */
      }	    
    } 
    else {
      pRelative = (*after_access) ? after_access : 0; /* zero for "" */
    }

    /* Look for port identifier */
    if ( pHost ) {
      if ((p = strchr(pHost, ':')) != NULL) {
        *p++ = '\0';
        pPort = p;
      }
    }

    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pData, pAccess ) ;
    gHyp_data_append ( pResult, pData ) ;

    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pData, pHost ) ;
    gHyp_data_append ( pResult, pData ) ;

    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pData, pPort ) ;
    gHyp_data_append ( pResult, pData ) ;

    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pData, pAbsolute ? pAbsolute : pRelative ) ;
    gHyp_data_append ( pResult, pData ) ;

    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pData, pArgs ) ;
    gHyp_data_append ( pResult, pData ) ;

    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pData, pFragment ) ;
    gHyp_data_append ( pResult, pData ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}
