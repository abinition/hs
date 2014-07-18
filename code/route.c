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
 * $Log: route.c,v $
 * Revision 1.10  2004/10/16 05:00:17  bergsma
 * Fixed memory leak.
 *
 * Revision 1.9  2004/07/23 18:43:14  bergsma
 * Fixed compile warnings about 'id' being an (sWORD).
 *
 * Revision 1.8  2004/04/29 08:24:33  bergsma
 * New functions service, open, tunnel, forward, unforward
 *
 * Revision 1.7  2003/04/04 16:21:38  bergsma
 * Cosmetic only.  Replace endingChar variable with ' '
 *
 * Revision 1.6  2003/01/12 05:01:34  bergsma
 * V3.1.0
 * 1. Incorporate instance_pushSTATUS
 * 2. In query() and event(),  oldstyle is now defined as
 * "when arg3 is non-empty".  New style uses arg4++, arg3 must be empty.
 * 3. Added instances(), parent(), instance(), concept(), children(), moveto()
 *
 * Revision 1.5  2002/11/20 20:45:49  bergsma
 * Use COND_SILENT instead of 0
 *
 * Revision 1.4  2002/11/14 01:30:12  bergsma
 * Added gHyp_route_instances.
 *
 * Revision 1.3  2002/09/24 17:21:07  bergsma
 * Old style of arguments to event and query was not being detected correctly.
 *
 * Revision 1.2  2002/09/16 18:09:35  bergsma
 * Added support for "described" arguments in event and query messages,
 * called the "new style" as compared to the "old style".
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

void gHyp_route_enable ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'enable' statement.
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
      *pMethodVariable,
      *pData,
      *pResult ;
    
    char
      *pMethodStr ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sMethod
      *pMethod = NULL ;

    /* Execute the 'enable' function */
        
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: enable ( method )" ) ;

    /* Pop the method identifier off of the stack. */
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    pMethodVariable = gHyp_data_getReference ( pData ) ;

    if ( !pMethodVariable ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, "Method '%s' is not a variable",
			      gHyp_data_getLabel ( pData ) ) ;
      gHyp_data_setBool ( pResult, FALSE ) ;
    }
    else {

      pMethodStr = gHyp_data_getLabel ( pMethodVariable ) ;

      pMethodVariable = gHyp_frame_findMethodVariable ( pFrame, pMethodStr, pAI );
      if ( pMethodVariable ) pMethod = (sMethod*) gHyp_data_getObject ( pMethodVariable ) ;
      if ( !pMethodVariable || !pMethod ) { 
	gHyp_instance_warning ( pAI, STATUS_BRANCH,
				"Could not find method '%s'",
				pMethodStr ) ;
	gHyp_data_setBool ( pResult, FALSE ) ;
      }
      else {
	
	gHyp_data_setBool ( pResult, TRUE ) ;
	gHyp_method_enable ( pMethod ) ;
      }
    }
    gHyp_stack_push ( pStack, pResult ) ;

    return ;
  }
}

void gHyp_route_disable ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'disable' statement.
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
      *pData,
      *pMethodVariable,
      *pResult ;
    
    char
      *pMethodStr ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
         
    sMethod
      *pMethod = NULL ;

    /* Execute the 'disable' function */
        
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: disable ( method )" ) ;

    /* Pop the method identifier off of the stack. */
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    pMethodVariable = gHyp_data_getReference (  pData ) ;
    if ( pMethodVariable ) pMethod = (sMethod*) gHyp_data_getObject ( pMethodVariable ) ;
    if ( !pMethodVariable || !pMethod ) { 

      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, "Method '%s' is not a variable",
			      gHyp_data_getLabel ( pData ) ) ;
      gHyp_data_setBool ( pResult, FALSE ) ;
    }
    else {

      pMethodStr = gHyp_data_getLabel ( pMethodVariable ) ;

      pMethodVariable = gHyp_frame_findMethodVariable ( pFrame, pMethodStr, pAI );
      if ( !pMethodVariable ) { 
	gHyp_instance_warning ( pAI, STATUS_BRANCH,
				"Could not find method '%s'",
				pMethodStr ) ;
	gHyp_data_setBool ( pResult, FALSE ) ;
      }
      else {
	gHyp_data_setBool ( pResult, TRUE ) ;
	gHyp_method_disable ( pMethod ) ;
      }
    }

    gHyp_stack_push ( pStack, pResult ) ;

    return ;
  }
}

static void lHyp_route_QE (	sInstance 	*pAI,
				sFrame		*pFrame,
				sStack		*pStack,
				sParse		*pParse,
				sCode		*pCode,
				sBYTE		mode,
				int		argCount )
{
  /* Description:
   *
   *	Services the query() and event() functions.
   *	Creates a query or event message and sends it. 
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
   */
  sData
    *pTV,
    *pToken,
    *pValue,
    *pValue2,
    *pVariable,
    *pTokenCopy,
    *pData,
    *pArgList ;

  char
    senderPath[OBJECT_SIZE+1],
    targetPath[OBJECT_SIZE+1],
    method[METHOD_SIZE+1],
    target[TARGET_SIZE+1],
    timeStamp[DATETIME_SIZE+1],
    transactionID[FIELD_SIZE+1],
    value[VALUE_SIZE+1],
    autoEnv[VALUE_SIZE+1],
    *pVal,
    *pMsg,
    *pTarget,
    *pMethod,
    *pMode,
    *pQUERY = "query",
    *pEVENT = "event",
    *pTokenStr;
		
  sBYTE
    tokenType,
    dataType ;

  sLOGICAL
    isOldStyle=FALSE,
    isVector1,
    isVector2,
    isVariable ;

  int
    n,
    ss1,
    ss2,
    context1,
    context2,
    contentLength;

  time_t
    eventTime ;

  /* Get argument list */
  if ( argCount == 3 ) isOldStyle = TRUE ;
  
  /* Get tokens and values */

  pTV = gHyp_data_new ( "_tv_" ) ;
  gpsTempData = pTV ; 

  if ( isOldStyle ) {

    /* Pull out all the token/value pairs from the argument list */
    pArgList = gHyp_stack_popRdata ( pStack, pAI ) ;
    pToken = NULL ;
    isVector1 = (gHyp_data_getDataType(pArgList) > TYPE_STRING ) ;
    ss1 = gHyp_data_getSubScript ( pArgList ) ; context1 = -1 ;
    while ( (pToken = gHyp_data_nextValue ( pArgList, 
					    pToken, 
					    &context1,
					    ss1 ) ) ) {

      pTokenCopy = gHyp_data_new ( NULL ) ;
      n = gHyp_data_getStr ( pToken,
			     value,
			     VALUE_SIZE,
			     context1,
			     isVector1 ) ;
      pTokenStr = value ;
      tokenType = gHyp_data_tokenType ( pToken ) ;
      if ( tokenType == TOKEN_REFERENCE &&
	   (pVariable = gHyp_frame_findVariable ( pAI, pFrame, pTokenStr ))) {
	
	/* Token was referenced (&) and it exists. */
	gHyp_data_setReference ( pToken, pTokenStr, pVariable ) ;
	gHyp_data_setVariable ( pTokenCopy, pTokenStr, TYPE_LIST ) ;
	isVariable = TRUE ;
      }
      else if ( tokenType == TOKEN_VARIABLE ) {
      
	gHyp_data_setVariable ( pTokenCopy, pTokenStr, TYPE_LIST ) ;
	isVariable = TRUE ;
      }
      else
	isVariable = FALSE ;
      
      if ( isVariable ) {
	
	isVector2 = (gHyp_data_getDataType(pToken) > TYPE_STRING ) ;
	pValue = NULL ;
	ss2 = -1 ; 
	context2 = -1 ;
	dataType = gHyp_data_getDataType(pToken) ;
	while ( (pValue = gHyp_data_nextValue ( pToken, 
						pValue, 
						&context2,
						ss2 ) ) ) {
	  if ( isVector2 ) {
      
	    pValue2 = gHyp_data_new ( NULL ) ;
	    gHyp_data_newConstant ( pValue2, 
				    dataType, 
				    pValue, 
				    context2 ) ;
	  }
	  else 
	    pValue2 = gHyp_data_copyAll ( pValue ) ;

	  gHyp_data_append ( pTokenCopy, pValue2 ) ;
	}
      }
      else
	gHyp_data_setStr ( pTokenCopy, pTokenStr ) ;
      
      gHyp_data_append ( pTV, pTokenCopy ) ;		   
    }	
  }
  else if ( argCount > 3 ) {

    /* New Style */
    for ( ; argCount > 3 ; argCount-- ) {

      pToken = gHyp_stack_popRdata ( pStack, pAI ) ;

      /*gHyp_util_debug("%s",gHyp_data_print(pToken));*/
      pVariable = gHyp_data_getVariable ( pToken ) ;
      contentLength = 0 ;
      if ( pVariable ) 
	pValue = gHyp_fileio_describeData ( pAI, pVariable, ' ', FALSE, &contentLength ) ;
      else
	pValue = gHyp_fileio_describeData ( pAI, pToken, ' ', FALSE, &contentLength ) ;			       
      gHyp_data_insert ( pTV, pValue ) ;
    }

    /* Pop the 3rd argument, make sure it is empty */
    pArgList = gHyp_stack_popRdata ( pStack, pAI ) ;
    if ( gHyp_data_getCount ( pArgList ) != 0 ) {
      gHyp_data_delete ( pTV ) ;
      gHyp_instance_error ( 
	pAI, STATUS_ARGUMENT, 
	"The argList (3rd argument) must be an empty if arg1,arg2,...argN are to follow" ) ;
    }
  }

  /* Get method */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  n = gHyp_data_getStr ( pData, 
			 method, 
			 METHOD_SIZE, 
			 gHyp_data_getSubScript(pData),
			 TRUE ) ;
  pMethod = method ;

  /* Get target */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  n = gHyp_data_getStr ( pData, 
			 target,
			 TARGET_SIZE, 
			 gHyp_data_getSubScript(pData),
			 TRUE ) ;
  pTarget = target ;
  gHyp_util_lowerCase ( pTarget, strlen ( pTarget ) ) ;

  if ( ! strchr ( target, '#' ) &&
       ! strchr ( target, '@' ) &&
       ! strchr ( target, '/' ) ) {

    strcpy ( autoEnv, "AUTO_" ) ;
    strcat ( autoEnv, pTarget ) ;
    gHyp_util_upperCase ( autoEnv, strlen ( autoEnv ) ) ;

    if ( ( pVal = getenv ( autoEnv ) ) != NULL ) {
      /* The environment variable has a value */
      gHyp_util_lowerCase ( pVal, strlen ( pVal ) ) ;
      gHyp_util_logInfo("Translating target '%s' to '%s'", pTarget, pVal ) ;
      strcpy ( target, pVal ) ;
    }	
  }

  /* No longer in danger of leaking pTV */
  gpsTempData = NULL ; 


  /* Get Sender and Mode */
  sprintf ( senderPath, gHyp_instance_getTargetPath ( pAI ) ) ;
  pMode = ( mode == MESSAGE_QUERY ) ? pQUERY : pEVENT ;

  /* Create message */
  strcpy ( timeStamp, gHyp_util_timeStamp ( gsCurTime ) ) ;
  strcpy ( transactionID, gHyp_util_random8 () ) ;


  pMsg = gHyp_aimsg_initUnparse (	gHyp_instance_outgoingMsg ( pAI ), 
					pTarget, 
					pMode, 
					pMethod, 
					senderPath, 
					transactionID,
					timeStamp,
					pTV ) ;
  if ( !pMsg ) {
    gHyp_instance_warning (	pAI, STATUS_TARGET,
    				"Failed to create message" ) ;
    return ;
  }

  if ( gHyp_concept_route ( gHyp_instance_getConcept(pAI), pMsg ) < COND_SILENT )

    gHyp_instance_warning (	pAI, STATUS_IO,
    				"Failed to send message" ) ;
  else {

    if ( mode == MESSAGE_QUERY ) {
  
      /* Wait for reply message from query */
      gHyp_instance_incIncomingDepth ( pAI ) ;
      gHyp_instance_setTimeOut ( pAI ) ;
      eventTime = gHyp_instance_getTimeOutTime ( pAI ) ;
      gHyp_aimsg_getTargetPath ( gHyp_instance_outgoingMsg ( pAI ), targetPath ) ;
      gHyp_instance_setExpectedReply ( pAI, 
				       targetPath, 
				       method, 
				       transactionID ) ;

      gHyp_instance_setState ( pAI, STATE_QUERY ) ;
      gHyp_frame_setState ( pFrame, STATE_QUERY ) ;
      gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;
      gHyp_parse_restoreExprRank ( pParse ) ;
      gHyp_util_logInfo ( "...waiting for '%s' reply, timeout in %d seconds", 
			  method,
			  (eventTime-gsCurTime)) ;
      longjmp ( gsJmpStack[giJmpLevel=1], COND_SILENT) ;
    }
  }

  return ;
}

void gHyp_route_event ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: event (sender,method,arglist)
   *	Sets the value of STATUS.
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
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
		
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount < 2  )
    	gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: event ( target, method[,argList [,arg1[, arg2[, ...]]]] )" ) ;

    lHyp_route_QE ( pAI, pFrame, pStack, pParse, pCode, MESSAGE_EVENT, argCount ) ;
  
    /* Result is in status variable */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
  return ;
}

void gHyp_route_query ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: query(sender,method,arglist)
   *	Sets the value of STATUS.
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
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount < 2  )
     gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: query ( target, method[,argList [,arg1[, arg2[, ...]]]] )" ) ;

    lHyp_route_QE ( pAI, pFrame, pStack, pParse, pCode, MESSAGE_QUERY, argCount ) ;
  
    /* Result from query is in STATUS variable. */
   gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
  return ;
}

void gHyp_route_instances ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: instances()
   *	Sets the value of STATUS.
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

    int
      n,
      ss,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pValue,
      *pValue2,
      *pResult ;
    
    char
      instance[OBJECT_SIZE+1] ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0  )
     gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: instances ()" ) ;

    pData = gHyp_concept_getInstances ( gHyp_instance_getConcept(pAI )) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_instances_", TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; 
    context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
      n = gHyp_data_getStr ( pValue, 
			     instance, 
			     OBJECT_SIZE, 
			     context, 
			     FALSE ) ;
      pValue2 = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr ( pValue2, instance ) ;
      gHyp_data_append ( pResult, pValue2 ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
 
  }
  return ;
}


void gHyp_route_children ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: children()
   *	Sets the value of STATUS.
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

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pFirst,
      *pClients,
      *pData,
      *pValue,
      *pResult ;
    
    sConcept
      *pConcept ;

    sChannel
      *pChannel ;

    sLOGICAL
      f ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 )
     gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: children ()" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_children_", TYPE_STRING ) ;
    pConcept = gHyp_instance_getConcept ( pAI ) ;
    pClients = gHyp_concept_getClients ( pConcept ) ;
    for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pClients ) ;
          (f && pData) || pData != pFirst;
          f=FALSE, pData = gHyp_data_getNext ( pData ) ) {

      pChannel = (sChannel*) gHyp_data_getObject ( pData ) ;
 ;
      pValue = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr ( pValue, gHyp_channel_target ( pChannel ) ) ;
      gHyp_data_append ( pResult, pValue ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
 
  }
  return ;
}


void gHyp_route_moveto ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: moveto ( parent )
   *	Sets the value of STATUS.
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

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData ;

    char   
      target[OBJECT_SIZE+1];

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  )
     gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: moveto ( [parent] )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   target, 
			   OBJECT_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    gHyp_util_lowerCase ( target, n ) ;
    gHyp_concept_moveto ( gHyp_instance_getConcept(pAI), target ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
 
  }
  return ;
}


void gHyp_route_parent ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: parent()
   *	Sets the value of STATUS.
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

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pResult ;

    char
      instance[INSTANCE_SIZE+1],
      concept[OBJECT_SIZE+1],
      parent[OBJECT_SIZE+1],
      root[OBJECT_SIZE+1],
      host[HOST_SIZE+1],    
      target[OBJECT_SIZE+1] ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1  )
     gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: parent ( [target] )" ) ;

    if ( argCount == 1 ) {
  
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData, 
			     target, 
			     OBJECT_SIZE,
			     gHyp_data_getSubScript(pData),
			     TRUE ) ;
      gHyp_util_lowerCase ( target, n ) ;

    }
    else
      strcpy ( target, gHyp_instance_getTargetPath ( pAI ) ) ;

    gHyp_util_breakTarget ( target, instance, concept, parent, root, host ) ;

    if ( strcmp ( instance, concept ) == 0 ) 
      strcpy ( target, parent ) ;
    else
      strcpy ( target, root+1 ) ;	

    pResult = gHyp_data_new ( "NULL" ) ;
    gHyp_data_setStr ( pResult, target ) ;
    gHyp_stack_push ( pStack, pResult ) ;
 
  }
  return ;
}


void gHyp_route_instance ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: instance()
   *	Sets the value of STATUS.
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

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pResult ;

    char
      instance[INSTANCE_SIZE+1],
      concept[OBJECT_SIZE+1],
      parent[OBJECT_SIZE+1],
      root[OBJECT_SIZE+1],
      host[HOST_SIZE+1],    
      target[OBJECT_SIZE+1] ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1  )
     gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: instance ( [target] )" ) ;

    if ( argCount == 1 ) {
  
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData, 
			     target, 
			     OBJECT_SIZE,
			     gHyp_data_getSubScript(pData),
			     TRUE ) ;
      gHyp_util_lowerCase ( target, n ) ;
    }
    else
      strcpy ( target, gHyp_instance_getTargetPath(pAI) ) ;
    
    gHyp_util_breakTarget ( target, instance, concept, parent, root, host ) ;

    pResult = gHyp_data_new ( "NULL" ) ;
    gHyp_data_setStr ( pResult, instance ) ;
    gHyp_stack_push ( pStack, pResult ) ;
 
  }
  return ;
}

void gHyp_route_concept ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: concept()
   *	Sets the value of STATUS.
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

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pResult ;

    char
      instance[INSTANCE_SIZE+1],
      concept[OBJECT_SIZE+1],
      parent[OBJECT_SIZE+1],
      root[OBJECT_SIZE+1],
      host[HOST_SIZE+1],    
      target[OBJECT_SIZE+1] ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1  )
     gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: concept ( [target] )" ) ;

    if ( argCount == 1 ) {
  
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData, 
			     target, 
			     OBJECT_SIZE,
			     gHyp_data_getSubScript(pData),
			     TRUE ) ;
      gHyp_util_lowerCase ( target, n ) ;
      gHyp_util_breakTarget ( target, instance, concept, parent, root, host ) ;
      strcpy ( target, concept ) ;
    }	
    else
      strcpy ( target, gzConcept ) ;

    pResult = gHyp_data_new ( "NULL" ) ;
    gHyp_data_setStr ( pResult, target ) ;
    gHyp_stack_push ( pStack, pResult ) ;
 
  }
  return ;
}


void gHyp_route_requeue( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: requeue ()
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: requeue ( )" ) ;
 
    gHyp_instance_requeue ( pAI ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_route_open ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: open ( )
   *	Returns 1
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
      *pResult ;
    
    SOCKET
      fd=INVALID_SOCKET ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ),
      port ;

    char
      *pHost,
      host[HOST_SIZE+1],
      targetAddr[MAX_PATH_SIZE+1] ;

    sLOGICAL
      status=TRUE;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) {
      gHyp_util_logError ( 
         "Invalid args. Usage open( host, port )");
    }
    
    /* Get the port */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    port = gHyp_data_getInt ( pData, 
			      gHyp_data_getSubScript(pData),
			      TRUE);
      
    /* Get the host name */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   host, 
			   HOST_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pHost = host ;

    if ( port < 1 || port > MAX_PORT_NUMBER )
	gHyp_instance_warning ( pAI,
				STATUS_IO,
				"Port number '%d' not in range 1-%d",
				port,
				MAX_PORT_NUMBER ) ;

    if ( gHyp_tcp_resolveHost ( pHost, targetAddr ) ) {
 
      giJmpOverride = TRUE ;      
      fd = gHyp_tcp_request ( pHost, port ) ;
      giJmpOverride = FALSE ;
      
      if ( fd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,
				STATUS_PORT,
	    "Failed to connect to TCP service at node '%s' on port %d",
				pHost,
				port ) ;
	status = FALSE ;
      }
      else {
	
	/* Create and open the service */ 
	pData = gHyp_concept_createNetwork ( gHyp_instance_getConcept(pAI),
					     pHost,
					     targetAddr,
					     fd ) ;

      }
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) fd ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_route_service ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: service ()
   *	Returns 1
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
    
    SOCKET
      socket ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      service[VALUE_SIZE+1],
      *pService;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: service ( name )");

    /* Get the service name */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
			   service,
			   VALUE_SIZE,
			   gHyp_data_getSubScript ( pData ),
			   TRUE ) ;

    pService = service ;
    socket = gHyp_concept_createService( gHyp_instance_getConcept (pAI), pService ) ;

    if ( socket == INVALID_SOCKET ) {
      gHyp_instance_warning ( pAI, STATUS_SOCKET, "Failed to create listen service" ) ; 
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) socket ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_route_tunnel ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: tunnel ()
   *	Returns 1
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
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 3 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: tunnel ( sourceId, relay, destinationId )");

    /* Get the service name */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_route_forward ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: forward ()
   *	Returns 1
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
      *pForwardList,
      *pResult,
      *pValue,
      *pData ;

    sSecs1
      *pForwardPort;

    sLOGICAL
      status=TRUE,
      isVector ;

    sBYTE
      objectType ;

    int
      context,
      ss,
      id,
      forwardId,
      argCount = gHyp_parse_argCount ( pParse ) ;

    SOCKET
      fd ;

    sConcept
      *pConcept ;

    sInstance
      *pAIassigned ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: forward ( device, deviceList )");

    /* Get device list */
    pForwardList = gHyp_stack_popRvalue ( pStack, pAI ) ;

    /* Get the device id to forward */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;
    /* Check id */
    if ( id < 0 || id > 65535 ) 
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

    pConcept = gHyp_instance_getConcept(pAI) ;
    pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, (sWORD) id ) ;
    if ( !pAIassigned ) {
      status = FALSE ;
      gHyp_instance_warning ( pAI, STATUS_PORT, "No instance is assigned to device id '%d'",id);
    }

    if ( status ) {
      fd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
      if ( fd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI, STATUS_SOCKET, "Invalid socket for device id '%d'",id ) ;
	status = FALSE ;
      }

    }

    if ( status ) {
      objectType = gHyp_concept_getSocketObjectType ( pConcept, fd ) ;
      if ( objectType != DATA_OBJECT_PORT && objectType != DATA_OBJECT_HTTP ) {
	gHyp_instance_warning ( pAI, STATUS_SOCKET, "Must be PORT or HTTP type for device id '%d'",id ) ;
	status = FALSE ;
      }
    }

    if ( status ) {

      pForwardPort = (sSecs1*) gHyp_concept_getSocketObject (
  		      pConcept, 
	    	      fd, 
		      objectType  ) ;

      if ( !pForwardPort ) {
	gHyp_instance_warning ( pAI, STATUS_SOCKET, "No socket object found for device id '%d'",id ) ;
	status = FALSE ;
      }
    }

    if ( status ) {

      pConcept = gHyp_instance_getConcept ( pAI ) ;
      isVector = (gHyp_data_getDataType( pForwardList ) > TYPE_STRING ) ;
      pValue = NULL ;
      ss = gHyp_data_getSubScript ( pForwardList ) ; 
      context = -1 ;
      while ( (pValue = gHyp_data_nextValue ( pForwardList, 
					      pValue, 
					      &context,
					      ss ) ) ) {

	/* Get the integer device id value */
	forwardId = gHyp_data_getInt ( pValue, ss, TRUE ) ;

	/* See if the device exists */
	if ( forwardId < 0 || forwardId > 65535 )  {
	  gHyp_instance_warning ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;
	}
	else {
	  /* If the device exists, then keep it. */
	  pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, (sWORD) forwardId ) ;
	  if ( !pAIassigned ) {
	    gHyp_instance_warning ( pAI,STATUS_BOUNDS, 
	      "Device ID %d is not assigned.",forwardId ) ;
	  }
	  else {
	    gHyp_util_logInfo ( 
	      "Forwarding device %d to device %d.",id, forwardId ) ;
	    gHyp_secs1_addForwardPort ( pForwardPort, id, forwardId ) ;
	  }
	}
      }
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setBool ( pResult, status ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_route_unforward ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: unforward ()
   *	Returns 1
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
      *pForwardList,
      *pResult,
      *pValue,
      *pData ;

    sSecs1
      *pForwardPort;

    sLOGICAL
      status=TRUE,
      isVector ;

    sBYTE
      objectType ;

    int
      context,
      ss,
      id,
      forwardId,
      argCount = gHyp_parse_argCount ( pParse ) ;

    SOCKET
      fd ;

    sConcept
      *pConcept ;

    sInstance
      *pAIassigned ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: unforward ( device, deviceList )");

    /* Get device list */
    pForwardList = gHyp_stack_popRvalue ( pStack, pAI ) ;

    /* Get the device id to forward */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;
    /* Check id */
    if ( id < 0 || id > 65535 ) 
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

    pConcept = gHyp_instance_getConcept(pAI) ;
    pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, (sWORD) id ) ;
    if ( !pAIassigned ) {
      status = FALSE ;
      gHyp_instance_warning ( pAI, STATUS_PORT, "No instance is assigned to device id '%d'",id);
    }

    if ( status ) {
      fd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
      if ( fd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI, STATUS_SOCKET, "Invalid socket for device id '%d'",id ) ;
	status = FALSE ;
      }

    }

    if ( status ) {
      objectType = gHyp_concept_getSocketObjectType ( pConcept, fd ) ;
      if ( objectType != DATA_OBJECT_PORT && objectType != DATA_OBJECT_HTTP ) {
	gHyp_instance_warning ( pAI, STATUS_SOCKET, "Must be PORT or HTTP type for device id '%d'",id ) ;
	status = FALSE ;
      }
    }

    if ( status ) {

      pForwardPort = (sSecs1*) gHyp_concept_getSocketObject (
  		      pConcept, 
	    	      fd, 
		      objectType  ) ;

      if ( !pForwardPort ) {
	gHyp_instance_warning ( pAI, STATUS_SOCKET, "No socket object found for device id '%d'",id ) ;
	status = FALSE ;
      }
    }

    if ( status ) {

      pConcept = gHyp_instance_getConcept ( pAI ) ;
      isVector = (gHyp_data_getDataType( pForwardList ) > TYPE_STRING ) ;
      pValue = NULL ;
      ss = gHyp_data_getSubScript ( pForwardList ) ; 
      context = -1 ;
      while ( (pValue = gHyp_data_nextValue ( pForwardList, 
					      pValue, 
					      &context,
					      ss ) ) ) {
	/* Get the integer device id value */
	forwardId = gHyp_data_getInt ( pValue, ss, TRUE ) ;

	/* See if the device exists */
	if ( forwardId < 0 || forwardId > 65535 )  {
	  gHyp_instance_warning ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;
	}
	else {
	  /* If the device exists, then keep it. */
	  pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, (sWORD) forwardId ) ;
	  if ( !pAIassigned ) {
	    gHyp_instance_warning ( pAI,STATUS_BOUNDS, "Device ID %d is not assigned." ) ;
	  }
	  else {
	    gHyp_util_logInfo (  
	      "Unforwarding device %d to device %d.",id, forwardId ) ;
	    gHyp_secs1_removeForwardPort ( pForwardPort, id, forwardId ) ;
	  }
	}
      }
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setBool ( pResult, status ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

