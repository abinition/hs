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

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

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
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 && argCount != 2 ) 
      gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: strtok ( string, [separator] )" ) ;

    if ( argCount == 2 ) {
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
    
      if ( *pValStart == '"' ) {
      
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
      else if ( *pValStart == '\'' ) {
	
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
      else  {
	
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
      strVal[INTERNAL_VALUE_SIZE+1],
      strVal2[VALUE_SIZE+1],
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

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
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
			     INTERNAL_VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
        n = gHyp_util_unparseString ( strVal2, pStr, n, VALUE_SIZE, FALSE, FALSE, specialChars ) ;
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

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
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

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
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
      decrypted[INTERNAL_VALUE_SIZE+1],
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
			     INTERNAL_VALUE_SIZE, 
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
