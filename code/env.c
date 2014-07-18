/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/

/* Modifications:
 *
 * $Log: env.c,v $
 * Revision 1.18  2004/07/28 00:49:18  bergsma
 * Version 3.3.0
 *
 * Revision 1.17  2004/07/27 15:43:26  bergsma
 * In mergeData, if nulls exist in the hyperscript being loaded, then
 * use getStr3 to take them out first
 *
 * Revision 1.16  2004/07/01 01:58:48  bergsma
 * Fixed traceback in detach() error logging.
 *
 * Revision 1.15  2004/06/13 13:56:26  bergsma
 * Added localhost() function.
 *
 * Revision 1.14  2004/04/29 08:26:30  bergsma
 * Fix chop and remove functions.
 *
 * Revision 1.13  2003/06/05 21:33:52  bergsma
 * Problem with merge() and autoincrement feature, was not i
 * auto-incrementing properly.
 *
 * Revision 1.12  2003/05/16 04:52:33  bergsma
 * Functions such as append, insert, global, local, detach, etc, i
 * which use the gHyp_data_detach()
 * function, must never detach a temporary variable, for example:
 *
 * 	  local {} ;
 *   	  detach ( 1+2 ) ;
 *
 * are not valid.
 *
 * Revision 1.11  2003/04/27 23:49:23  bergsma
 * When detaching a variable where new values are moved into an existing
 * variable from a source variable, make sure the source variable is
 * detached before deleting it afterwards.
 *
 * Revision 1.10  2003/04/23 22:44:45  bergsma
 * Use setStr2 in map() function.
 *
 * Revision 1.9  2003/04/04 16:43:19  bergsma
 * MergeData now also handles argument merging from messages.
 *
 * Revision 1.8  2003/02/17 09:57:53  bergsma
 * Fix segmentation fault bug because value[TOKEN_SIZE+1] should have been
 * value[VALUE_SIZE+1].
 *
 * Revision 1.7  2003/01/28 02:01:39  bergsma
 * Added optional condition argument to exit and quit.
 *
 * Revision 1.6  2003/01/12 14:02:00  bergsma
 * V3.1.0
 * 1. Incorportate instance_pushSTATUS in functions
 * 2. Merge instantiate() and instantiation() into lHyp_env_instantiate
 * 3. Rework mergeData and mapData.
 * 4. When merge(), detach(), chop(), remove() are used as lvalues, then
 * the resulting variables are made local/glocal.
 *
 * Revision 1.5  2002/11/20 20:38:12  bergsma
 * Functions env_instance and env_instantiate can now specified arguments
 * that include the full path of the instance.
 *
 * Revision 1.4  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.3  2002/11/12 20:23:25  bergsma
 * In instance() and instantiate(), change exception conditions to match
 * documentation.
 *
 * Revision 1.2  2002/10/23 00:06:28  bergsma
 * Removed some commented-out code.
 *
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

void gHyp_env_count ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: count( variable )
   *	Returns count of elements in variable.
   *	A literal returns 1. A variable can have 0 to MAX_VALUES.
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
   *	07-Apr-96		Mike Bergsma
   *	- Q/A check and code review
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
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: count ( variable )") ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setInt ( pResult, gHyp_data_getCount ( pData ) ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }	
}


void gHyp_env_debug ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: debug ( flags )
   *	Returns 1 if flags are allowed, otherwise 0.
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
   *	07-Apr-96		Mike Bergsma
   *	- Q/A check and code review
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
      prevFlags=guDebugFlags,
      debugFlags,
      frameDepth=0,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    	"Invalid arguments. Usage: debug ( [flags] )" ) ;


    if ( argCount == 1 ) {

      /* Pop the value off of the stack	 */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      debugFlags = gHyp_data_getInt ( pData,
				      gHyp_data_getSubScript ( pData ),
				      TRUE ) ;  
      
      if ( debugFlags  < 0 || debugFlags > MAX_DEBUG_LEVEL ) {
        gHyp_instance_warning ( pAI, STATUS_INVALID, "Invalid debug flags" ) ;
      }
      else {
        guDebugFlags = debugFlags ;

        /* If the FRAME debug flag is set, update the frame depth */
	if ( guDebugFlags & DEBUG_FRAME ) frameDepth  = gHyp_frame_depth (pFrame) ;

        /*gHyp_util_logDebug ( frameDepth, guDebugFlags,"debug: %u", guDebugFlags ) ;*/
      }
    }
    pResult = gHyp_data_new(NULL) ;
    gHyp_data_setInt ( pResult, prevFlags ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }	
}

void gHyp_env_exists ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: exists ( variable )
   *	Returns 0 or 1.
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
   *	07-Apr-96		Mike Bergsma
   *	- Q/A check and code review
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pResult,
      *pData;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: exists ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new (NULL) ;
    /* Locate the variable */

    if ( gHyp_data_getVariable ( pData ) ) {
      gHyp_data_setBool ( pResult, TRUE ) ;
    }
    else {
      gHyp_instance_setStatus ( pAI, STATUS_UNDEFINED ) ;
      gHyp_data_setBool ( pResult, FALSE ) ;
    }

    gHyp_stack_push ( pStack, pResult ) ;
  }	
}

void gHyp_env_exit ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: exit()
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
   *	07-Apr-96		Mike Bergsma
   *	- Q/A check and code review
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    sFrame 	*pFrame = gHyp_instance_frame ( pAI ) ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    sData	*pData ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: exit( [cond] )" ) ;

    if ( argCount == 1 ) {
      /* Pop the condition value off of the stack */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      giCondition = gHyp_data_getInt ( pData,
				       gHyp_data_getSubScript ( pData ),
				       TRUE ) ;
    }

    /* If the method was invoked from a query, then send all replies */
    while ( gHyp_instance_replyMessage (
	       pAI,
	       gHyp_frame_getMethodData(pFrame) ) ) ;

    if ( guDebugFlags & DEBUG_DIAGNOSTICS )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
			   "diag : EXIT (longjmp to 0 from frame %d)",
			   gHyp_frame_depth(pFrame) );
    
    /* Flush the stack and clear the frame */
    gHyp_stack_flush ( pStack ) ;
    gHyp_frame_reset ( pFrame ) ;
    
    /* Exit and Quit */
    longjmp ( gsJmpStack[0], COND_ERROR ) ;
  }
}

void gHyp_env_quit ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: quit()
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

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    sFrame 	*pFrame = gHyp_instance_frame ( pAI ) ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    sData	*pData ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: quit( [cond] )" ) ;

    if ( argCount == 1 ) {
      /* Pop the condition value off of the stack */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      giCondition = gHyp_data_getInt ( pData,
				       gHyp_data_getSubScript ( pData ),
				       TRUE ) ;
    }

    if ( guDebugFlags & DEBUG_DIAGNOSTICS )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
			   "diag : QUIT (longjmp to 0 from frame %d)",
			   gHyp_frame_depth(pFrame) );
    
    /* Flush the stack and clear the frame */
    gHyp_stack_flush ( pStack ) ;
    gHyp_frame_reset ( pFrame ) ;
    
    /* Quit */
    longjmp ( gsJmpStack[0], COND_ERROR ) ;
  }
}

void gHyp_env_idle ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: idle()
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
   *	07-Apr-96		Mike Bergsma
   *	- Q/A check and code review
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    int	
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sConcept
      *pConcept ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: idle ( )" ) ;

    /* If the method was invoked from a query, then send all replies ********
    while ( gHyp_instance_replyMessage (
	       pAI,
	       gHyp_frame_getMethodData(pFrame) ) ) ;
    *************/

    /* Exit and Idle */    
    gHyp_instance_setState ( pAI, STATE_IDLE ) ;
    gHyp_frame_setState ( pFrame, STATE_IDLE ) ;
    gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;
    gHyp_parse_restoreExprRank ( pParse ) ;

    /* If we are the parent, we may turn-off "return to stdIn".*/
    pConcept = gHyp_instance_getConcept(pAI) ;
    if ( gHyp_concept_getConceptInstance ( pConcept ) == pAI ) 
      gHyp_concept_setReturnToStdIn ( pConcept, FALSE ) ;

    if ( guDebugFlags & DEBUG_DIAGNOSTICS )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS, 
			   "diag : IDLE (longjmp to 1 from frame %d)",
			   gHyp_frame_depth(pFrame) ) ;

    longjmp ( gsJmpStack[giJmpLevel=1], COND_SILENT) ;

  }
}

void lHyp_env_instantiate ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE, sLOGICAL fork ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    char
      value[VALUE_SIZE+1],
      newInstance[VALUE_SIZE+1],
      targetPath[TARGET_SIZE+1],
      instance[INSTANCE_SIZE+1],
      concept[OBJECT_SIZE+1],
      parent[OBJECT_SIZE+1],
      root[OBJECT_SIZE+1],
      host[HOST_SIZE+1] ;
    
    sData
      *pData;

    sConcept
      *pConcept ;

    sInstance
      *pAInew ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( argCount > 1 ) {
      if ( fork )
	gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: instantiate ( [name] )" ) ;
        else
	  gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	    "Invalid arguments. Usage: instantiation ( [name] )" ) ;
    }

    if ( argCount == 1 ) {
  
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData, 
			     value, 
			     VALUE_SIZE,
			     gHyp_data_getSubScript(pData),
			     TRUE ) ;
      gHyp_util_lowerCase ( value, n ) ;

      if ( !strchr ( value, '/' ) ) 
	sprintf ( newInstance, "%s#%s%s", value, gzConcept, gzRoot ) ;
      else 
	strcpy ( newInstance, value ) ;
    }	
    else
      /* No argument. Generate random instance name */
      sprintf ( newInstance, "%s#%s%s", gHyp_util_random8(), gzConcept, gzRoot ) ;
      
    gHyp_util_breakTarget ( newInstance, instance, concept, parent, root, host ) ;
    sprintf ( targetPath, "%s#%s%s", instance, concept, root ) ;
    pConcept = gHyp_instance_getConcept(pAI) ;

    /* Make sure we're the parent instance */
    if ( pAI != gHyp_concept_getConceptInstance ( pConcept ) ) {

      gHyp_instance_warning ( 
	  pAI, STATUS_REJECTED, 
	  "Instantiate to %s not allowed from %s since this is not the parent concept",
	  targetPath, gHyp_instance_getTargetId(pAI) ) ;

    }
    else if ( gHyp_concept_getNamedInstance ( gHyp_instance_getConcept(pAI),
					      instance ) ) {
      gHyp_instance_warning ( pAI,
			      STATUS_TARGET, 
			      "Instance %s already created",
			      instance ) ;

    }
    else if ( !fork ) {

      /* Just create the new instance */
      gHyp_concept_instantiate (  gHyp_instance_getConcept(pAI), 
				  pAI,
				  instance,
				  root,
				  FALSE ) ;
    }
    else {    
      
      /* Create the new instance name from the parent */

      /* If the method was invoked from a query, then send all replies */
      while ( gHyp_instance_replyMessage (
	       pAI,
	       gHyp_frame_getMethodData(pFrame) ) ) ;

      pAInew = gHyp_concept_instantiate (  pConcept, 
					   pAI,
					   instance,
					   root,
					   TRUE ) ;

      /* The new instance now has the frame once owned by the concept instance and
       * the concept instance now has a new frame.  However, the _main_ methodData of
       * both frames has been swapped, so that the concept instance still has its
       * global variables and its own _main_. 
       *
       * pAI is still the concept instance, but pFrame, pParse, pStack are owned
       * by pAInew.
       *
       */

      gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;
      gHyp_parse_restoreExprRank ( pParse ) ;
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
      	
      if ( !gHyp_instance_isEND ( pAI ) && gHyp_frame_depth (pFrame) > 1 )
	gHyp_concept_setReturnToStdIn ( pConcept, FALSE ) ;

      else if ( gHyp_concept_returnToStdIn ( pConcept ) )
	gHyp_util_logWarning( "Parent concept is not at eof" ) ;

      gHyp_instance_reset ( pAI, STATE_IDLE, TRUE ) ;

      longjmp ( gsJmpStack[giJmpLevel=1], COND_NORMAL ) ;

    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}
void gHyp_env_instantiate ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  lHyp_env_instantiate ( pAI,pCode,isPARSE,TRUE); 
}

void gHyp_env_instantiation ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  lHyp_env_instantiate ( pAI,pCode,isPARSE,FALSE); 
}


void gHyp_env_undef ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: undef ( variable )
   *	Returns 0 or 1.
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
   *	07-Apr-96		Mike Bergsma
   *	- Q/A check and code review
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pVariable,
      *pData ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
   	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: undef ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    if ( (pVariable = gHyp_data_getVariable ( pData ) ) ) 
      
      gHyp_frame_deleteVariable ( pFrame, gHyp_data_getLabel ( pVariable ) ) ;
    
    else
      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, "Cannot undef '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;

    return ;
  }
}


void gHyp_env_hashed ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: hashed ( variable )
   *	Returns 1 if hashed, 0 otherwise.
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
  sFrame	
    *pFrame = gHyp_instance_frame ( pAI ) ;

  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pVariable,
      *pData ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: hashed ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Locate the variable */
    if ( (pVariable = gHyp_data_getVariable ( pData )) )
      gHyp_data_setHashed ( pVariable, TRUE ) ;
    
    else
      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, "Cannot hash '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_unhashed ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: unhashed ( variable )
   *	Returns 1 if unhashed, 0 otherwise.
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
      *pVariable,
      *pData ;

    int	
      argCount = gHyp_parse_argCount ( pParse ) ;
    	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: unhashed ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Locate the variable */
    if ( (pVariable = gHyp_data_getVariable ( pData )) )
      gHyp_data_setHashed ( pVariable, FALSE ) ;
    else
      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, "Cannot unhash '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_local ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: local ( variable )
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
   * Modifications:
   *
   */
  sFrame	
    *pFrame = gHyp_instance_frame ( pAI ) ;

  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData,
      *pVariable,
      *pLocalVariable ;

    char
      *pVarStr ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;	

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: local ( [variable] )" ) ;

    if ( argCount == 1 ) {
      
      /* Pop the variable off of the stack */
      pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
      if ( (pVariable = gHyp_data_getVariable ( pData ) ) && pVariable != pData ) {
	
	pVarStr = gHyp_data_getLabel ( pVariable ) ;
	pLocalVariable = gHyp_frame_findLocalVariable ( pFrame, pVarStr ) ;
	if ( pVariable != pLocalVariable ) {
	  /* Variable does not exist in local scope */
	  if ( pLocalVariable ) gHyp_frame_deleteLocalVariable ( pFrame, pVarStr ) ;	  
	  gHyp_data_detach ( pVariable ) ;
	  gHyp_data_append( gHyp_frame_getMethodData(pFrame),pVariable);
	}
      }
      else
	gHyp_instance_error ( pAI, 
			      STATUS_UNDEFINED,
			      "Variable '%s' does not exist or is a temporary variable",
			      gHyp_data_getLabel ( pData ) ) ;
    }
    else
      gHyp_frame_setGlobalScope ( pFrame, FALSE ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_global ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: global ( variable )
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
      *pVariable,
      *pRootVariable ;

    char
      *pVarStr ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: global ( [variable] )" ) ;

    if ( argCount == 1 ) {

      /* Pop the variable off of the stack */
      pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
      if ( (pVariable = gHyp_data_getVariable ( pData ) ) && pVariable != pData ) { 
	pVarStr = gHyp_data_getLabel ( pVariable ) ;
	pRootVariable = gHyp_frame_findRootVariable ( pFrame, pVarStr ) ;
	if ( pVariable != pRootVariable ) {
	  /* Variable does not exist in global scope  */
	  if ( pRootVariable ) gHyp_frame_deleteRootVariable ( pFrame, pVarStr ) ;	  
	  gHyp_data_detach ( pVariable ) ;
	  gHyp_data_append( gHyp_frame_getRootMethodData(pFrame),pVariable);
	}
      }
      else
	gHyp_instance_error ( pAI, 
			      STATUS_UNDEFINED,
			      "Variable '%s' does not exist or is a temporary variable",
			      gHyp_data_getLabel ( pData ) ) ; 
    }
    else
      gHyp_frame_setGlobalScope ( pFrame, TRUE ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_next ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: next ( variable )
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
      *pVariable ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: next ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    if ( (pVariable = gHyp_data_getVariable ( pData ) ) ) 
      gHyp_data_next ( pVariable ) ;
    else
      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, "Cannot do next on '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_prev ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: prev ( variable )
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
      *pVariable ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: prev ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    if ( (pVariable = gHyp_data_getVariable ( pData ) ) )
      gHyp_data_prev ( pVariable ) ;
    else
      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, "Cannot do prev on '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

sData* lHyp_env_map ( sData *pDst,
		      sData *pSrc,
		      sData *pSrcValue,
		      int *pContextSrc,
		      sInstance *pAI,
		      sLOGICAL isLittleEndian )
{
  sData	
    *pResult,
    *pValue,
    *pDstValue,
    *pNextSrcValue,
    *pNextDstValue,
    *pVariable ;

  char
    value[VALUE_SIZE+1] ;

  int
    i,
    n,
    byteBufferLen=0,
    dstDataLen,
    srcDataLen,
    contextSrc,
    contextDst,
    ssv=-1,
    sdv=-1,
    sss,
    ssd ;

  sLOGICAL
    isVectorDst,
    isVectorSrc ;

  sBYTE
    *pBuf,
    *pByteBuffer=NULL,
    dstDataType,
    srcDataType,
    srcTokenType,
    dstTokenType ;
  
  sEndian 
    endian ;

  srcTokenType = gHyp_data_getTokenType ( pSrc ) ;
  dstTokenType = gHyp_data_getTokenType ( pDst ) ;
  srcDataType = gHyp_data_getDataType ( pSrc ) ;
  dstDataType = gHyp_data_getDataType ( pDst ) ;

  isVectorSrc = ( srcTokenType == TOKEN_VARIABLE &&
		  srcDataType > TYPE_STRING ) ;
  isVectorDst = ( dstTokenType == TOKEN_VARIABLE &&
		  dstDataType > TYPE_STRING ) ;

  /* Create the result */
  pResult = gHyp_data_new ( gHyp_data_getLabel ( pDst ) ) ;
  if ( isVectorDst )
    gHyp_data_newVector ( pResult, 
			  dstDataType, 
			  gHyp_data_getCount ( pDst ),
			  gHyp_data_isDynamic ( pDst ) ) ;
  pNextSrcValue = pSrc ;
  pNextDstValue = pDst ;
  sss = gHyp_data_getSubScript ( pSrc ) ; 
  ssd = gHyp_data_getSubScript ( pDst ) ; 
  contextDst = -1 ;
  pDstValue = NULL ;
  while ( 1 ) {

    if ( pNextDstValue ) {
      /* This is either the start of the scan, or there was a previous value.
       * Get the next value of the destination.
       */
      pNextDstValue = gHyp_data_nextValue ( pDst, 
					    pDstValue, 
					    &contextDst,
					    ssd ) ;
      if ( contextDst == -2 ) {
	gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( 
          pAI, STATUS_BOUNDS, 
	  "Destination subscript is out of bounds in map()");
      }
      if ( isVectorDst ) 
	sdv = contextDst ;
      else
	sdv = 0 ;
    }

    /* Get the next destination value. */
    pDstValue = pNextDstValue ;
    
    /* If there is no more source or destination values, we're done. */
    if ( !pDstValue ) break ;

    if ( pNextSrcValue ) {
      /* This is either the start of the scan, or there was a previous value.
       * Get the next value from the source data.
       */
      pNextSrcValue = gHyp_data_nextValue ( pSrc, 
					    pSrcValue,
					    pContextSrc,
					    sss ) ;
      if ( *pContextSrc == -2 ) {
	gHyp_data_delete ( pResult ) ;

	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Source subscript is out of bounds in map()");
      }
      if ( isVectorSrc ) 
	ssv = *pContextSrc ;
      else
	ssv = 0 ;
    }
    /* Get the next source value. */
    pSrcValue = pNextSrcValue ;
        
    /* If there is no more source or destination values, we're done. */
    if ( !pSrcValue ) break ;

    /* Initialize the byte buffer */
    pByteBuffer = gHyp_data_buffer ( pSrcValue, ssv ) ;
    byteBufferLen = gHyp_data_bufferLen ( pSrcValue, ssv ) ;
    /*
    gHyp_util_logInfo("Next byte starts with %x at %d",
		      *pByteBuffer,
		      ssv );
		      */
    srcDataType = gHyp_data_dataType ( pSrcValue ) ;
    srcTokenType = gHyp_data_tokenType ( pSrcValue ) ;
    srcDataLen = gHyp_data_dataLen ( pSrcValue ) ;

    dstDataType = gHyp_data_dataType ( pDstValue ) ;
    dstTokenType = gHyp_data_tokenType ( pDstValue ) ;
    dstDataLen = gHyp_data_dataLen ( pDstValue ) ;
    
    if ( isVectorDst ) {

      /* Replace destination vector element with that from source.
       * Make sure there is enough buffer to read from.
       * Advance the context of the destination if required.
       */
      if ( byteBufferLen < dstDataLen ) {
	
	/* Have to cast */
	gHyp_data_setVector ( pResult, 
			      pSrcValue, 
			      sdv,
			      ssv,
			      isVectorSrc ) ;
	  
      }
      else { 
	
	/* How many elements are skipped over ? */
	n = dstDataLen / srcDataLen ;
	if ( n > 1 ) {
	  *pContextSrc += (n-1) ;
	  if ( isVectorSrc ) 
	    ssv = *pContextSrc ;
	  else
	    ssv = 0 ;
	}
	/*
	gHyp_util_logInfo("RAW TRANSFER to %s of %d bytes at [%d]",
			  gHyp_data_getLabel(pResult),
			  dstDataLen,ssv ) ;
	*/
	if ( isLittleEndian ) {
	  pBuf = pByteBuffer ;
	  for ( i=dstDataLen-1; i>=0; i-- ) endian.x.b[i] = *pBuf++ ;
	  gHyp_data_setVectorRaw ( pResult, 
				   &endian.x.b, 
				   sdv ) ;
	}
	else
	  gHyp_data_setVectorRaw ( pResult, 
				   pByteBuffer, 
				   sdv ) ;
      }

    }
    else if ( dstTokenType == TOKEN_CONSTANT || 
	      dstTokenType == TOKEN_LITERAL ) {
      
      /* Skip - just keep literals and constants. */
      pValue = gHyp_data_copy ( pDstValue ) ;
      gHyp_data_append ( pResult, pValue ) ;
    }
    
    else if ( dstTokenType == TOKEN_REFERENCE ) {
      
      if ( srcTokenType == TOKEN_VARIABLE ) {
	
	/* Make copy of source data, then rename it. */
	pVariable = gHyp_data_copyAll ( pSrcValue ) ;
	gHyp_data_setLabel ( pVariable, gHyp_data_getLabel ( pDstValue ) ) ;
	gHyp_data_append ( pResult, pVariable ) ;
      }
      else {

	/* Create new variable, then insert contents of source */
	pVariable = gHyp_data_new ( NULL ) ;
	gHyp_data_setVariable (pVariable, gHyp_data_getLabel(pDstValue), srcDataType);
	if ( srcDataType > TYPE_STRING )
	   gHyp_data_setVector ( pVariable, 
				 pSrcValue, 
				 0,
				 ssv,
				 isVectorSrc ) ;
	else
	  gHyp_data_copyValues ( pVariable, pSrcValue ) ;

	gHyp_data_append ( pResult, pVariable ) ;
      }
    }
    else {
      
      /* Destination must be a VARIABLE */

      if ( srcTokenType != TOKEN_VARIABLE ) {
	
	/* Replace destination values with that from the source. */
	pVariable = gHyp_data_new ( gHyp_data_getLabel ( pDstValue ) ) ;
	pValue = gHyp_data_new ( NULL ) ;
	n = MIN ( byteBufferLen, VALUE_SIZE ) ;
	memcpy ( value, (const char*) pByteBuffer, n ) ;
	value[n] = '\0' ;
	if ( srcDataType == TYPE_STRING )
	  gHyp_data_setStr2 ( pValue, value, n ) ;
	else
	  gHyp_data_setToken ( pValue, value ) ;
	/*
	gHyp_util_logInfo("NEW VALUE %s = %s",
			  gHyp_data_getLabel ( pDstValue ),
			  value) ;
	*/
	gHyp_data_append ( pVariable, pValue ) ;
	gHyp_data_append ( pResult, pVariable ) ;	

	/* How many elements are skipped over ? */
	n = n / srcDataLen ;
	if ( n > 1 ) {
	  *pContextSrc += (n-1) ;
	  if ( isVectorSrc ) 
	    ssv = *pContextSrc ;
	  else
	    ssv = 0 ;
	}	
      }
      else {

	/* Source is also a VARIABLE  */
	if ( pSrc == pSrcValue ) {

	  /* Source is a vector */
	  /*gHyp_util_logInfo("Source vector (%d)",ssv ) ;*/

	  *pContextSrc -= 1 ;
	  gHyp_data_append ( pResult, 
			     lHyp_env_map ( pDstValue,
					    pSrcValue, 
					    pSrcValue,
					    pContextSrc,
					    pAI,
					    isLittleEndian ));
	  /*gHyp_util_logInfo("End source vector");*/
	}
	else {

	  contextSrc = -1 ;

	  /*gHyp_util_logInfo("Source list (%d)",contextSrc ) ;*/
	  gHyp_data_append ( pResult, 
			     lHyp_env_map ( pDstValue,
					    pSrcValue, 
					    NULL,
					    &contextSrc,
					    pAI,
					    isLittleEndian ));	
	}
      }
    }
  }
  return pResult ;
}

void gHyp_env_map ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: map ( variable, variable )
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
      *pSrcData,
      *pDstData,
      *pSrc,
      *pDst,
      *pResult ;

    int
      contextSrc = -1,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isLittleEndian = gHyp_util_isLittleEndian() ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: map ( destination, source )" ) ;

    pSrcData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pDstData = gHyp_stack_popRdata ( pStack, pAI ) ;

    pSrc = gHyp_data_getVariable ( pSrcData ) ;
    pDst = gHyp_data_getVariable ( pDstData ) ;
    if ( !pSrc ) pSrc = pSrcData ;
    if ( !pDst ) pDst = pDstData ;
    pResult = lHyp_env_map ( pDst, 
			     pSrc,
			     NULL,
			     &contextSrc,
			     pAI,
			     isLittleEndian ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_env_merge ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: merge ( variable, variable )
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
      *pSrcData,
      *pDstData,
      *pVariable,
      *pData,
      *pResult ;

    sLOGICAL
      autoIncrement=FALSE ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 && argCount != 3 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: merge ( destination, source[, autoIncrement] )" ) ;

    if ( argCount == 3 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      autoIncrement = gHyp_data_getBool ( pData,
					  gHyp_data_getSubScript ( pData  ),
					  TRUE ) ;
    }
    pSrcData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pDstData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pResult = gHyp_env_mergeData ( pDstData, pSrcData, pAI, 0, autoIncrement, FALSE, NULL ) ;

    if ( gHyp_parse_exprCount ( pParse ) == 0 ) {

      if ( gHyp_data_tokenType ( pResult ) == TOKEN_VARIABLE ) {

        pVariable = gHyp_frame_findVariable ( pAI, pFrame, gHyp_data_getLabel ( pResult ) ) ;
	if ( pVariable ) {
	  /* Variable already exists. Just move the new values into place */
	  gHyp_data_deleteValues ( pVariable ) ;
	  gHyp_data_moveValues ( pVariable, pResult ) ;
	  gHyp_data_delete ( pResult ) ;
	  pResult = NULL ;
	}
	else 
	  gHyp_data_append( gHyp_frame_getMethodData(pFrame),pResult ) ;

	pResult = NULL ;
      }
      else {
	gHyp_instance_warning ( pAI, STATUS_INVALID, "Cannot create variable from '%s'",
			      gHyp_data_getLabel ( pResult ) ) ;
      }
    }
    if ( pResult ) 
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

sData* gHyp_env_mergeData ( sData *pDst, 
			    sData *pSrc,
			    sInstance *pAI,
			    int counter,
			    sLOGICAL isDataMerge,
			    sLOGICAL isMsgMerge,
			    sData *pTmp ) {

  sData	
    *pResult,
    *pToken,
    *pValue,
    *pValue2,
    *pData,
    *pData2,
    *pSrcValue,
    *pDstValue,
    *pNextSrcValue,
    *pNextDstValue,
    *pLastDstValue,
    *pVariable ;

  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;

  int
    context,
    contextSrc,
    contextDst,
    n,
    ssv=-1,
    sdv=-1,
    ss,
    sss,
    ssd,
    count,
    countDst,
    countSrc,
    countResult,
    counter2,
    labelLen,
    hypIndex,
    lineCount=0,
    contentLength ;

  sLOGICAL
    isVector,
    isVectorDst,
    isVectorSrc,
    isDynamicDst,
    isDynamicSrc,
    isDynamicResult,
    doDereference = FALSE,
    firstPass=TRUE;

  sBYTE
    dstDataType,
    srcDataType,
    srcTokenType,
    dstTokenType ;

  char
    *pStr,
    *pStream,
    *pLabel,
    value[VALUE_SIZE+1] ;
  
  sHyp
    *pHyp = gHyp_frame_getHyp ( pFrame ) ;


  hypIndex = gHyp_hyp_getHypCount ( pHyp ) ;
  srcTokenType = gHyp_data_getTokenType ( pSrc ) ;
  dstTokenType = gHyp_data_getTokenType ( pDst ) ;
  srcDataType = gHyp_data_getDataType ( pSrc ) ;
  dstDataType = gHyp_data_getDataType ( pDst ) ;

  isVectorSrc = ( srcTokenType == TOKEN_VARIABLE && 
		  srcDataType > TYPE_STRING ) ;
  isVectorDst = ( dstTokenType ==  TOKEN_VARIABLE &&
		  dstDataType > TYPE_STRING ) ;

  isDynamicSrc = gHyp_data_isDynamic ( pSrc ) ;
  isDynamicDst = gHyp_data_isDynamic ( pDst ) ;

  countSrc = gHyp_data_getCount ( pSrc ) ;
  countDst = gHyp_data_getCount ( pDst ) ;

  if ( !isDynamicDst ) {
    countResult = countDst ;
    isDynamicResult = FALSE ;
  }
  else {
    countResult = countSrc ;
    isDynamicResult = isDynamicSrc ;
  }

  /* Create the result */
  if ( isDataMerge && counter > 0 ) {
    sprintf ( value, "%s%d", 
	      gHyp_data_getLabel ( pDst ),
	      counter++ ) ;
  }
  else
    strcpy ( value,  gHyp_data_getLabel ( pDst ) );
  
   
  if ( isVectorDst ) {
    pResult = gHyp_data_new ( value ) ;
    gHyp_data_newVector ( pResult, 
			  dstDataType, 
			  countResult,
			  isDynamicResult ) ;
  }
  else {
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, value, dstDataType );
  }

  /* Merge values of pSrc into pDst, going in parallel. */
  pSrcValue = NULL ;
  pDstValue = NULL ;
  pNextSrcValue = pSrc ;
  pNextDstValue = pDst ;
  pLastDstValue = NULL ;
  sss = gHyp_data_getSubScript ( pSrc ) ; 
  contextSrc = -1 ;
  ssd = gHyp_data_getSubScript ( pDst ) ; 
  contextDst = -1 ;
  count = 0 ;

  while ( 1 ) {
    
    /* The logic for merging is as follows:
     *
     * 1. The source values are merged into destination values, in parallel.
     *    
     *    If there are more source values than destination values, then 
     *    if the destination has a fixed size, the values are truncated,
     *    otherwise the values are appended.  If there
     *    are more destination values than source values, then the extra
     *    destination values are not changed.
     *    If the destination is a vector, then all source values will have
     *    to merge into the vector slots. 
     *    If the destination is a list, then the source values will have
     *    to merge into the list locations.
     *	  If the count of a list destination variable is one,
     * 2. If the destination element is a element in a vector, then
     *    the destination element is replaced by the value in the source.
     * 3. If the destination element is a constant or a literal,
     *    then the destination element is replaced entirely by the source
     *    value at that point.
     * 4. If the destination element is an undefined variable, i.e. an
     *    UNIDENTIFIED variable, then a new destination variable is 
     *    created with the contents of the source, whatever that may be.
     * 5. If the destination element is a variable, then the merge procedure
     *    is done recursively on the values of the destination element and 
     *    the values of the source element.   
     */

    if ( pNextSrcValue ) {
      /* This is either the start of the scan, or there was a previous value.
       * Get the next value from the source data.
       */
      pNextSrcValue = gHyp_data_nextValue ( pSrc, 
					    pSrcValue,
					    &contextSrc,
					    sss ) ;
      if ( contextSrc == -2 ) {
	gHyp_data_delete ( pResult ) ;

	if ( pTmp ) gHyp_data_delete ( pTmp ) ;
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Source subscript is out of bounds in merge()");
      }
      if ( isVectorSrc ) 
	ssv = contextSrc ;
      else
	ssv = 0 ;
    }

    if ( pNextDstValue ) {
      /* This is either the start of the scan, or there was a previous value.
       * Get the next value of the destination.
       */
      pLastDstValue = pNextDstValue ;
      pNextDstValue = gHyp_data_nextValue ( pDst, 
					    pDstValue, 
					    &contextDst,
					    ssd ) ;
      if ( contextDst == -2 ) {
	gHyp_data_delete ( pResult ) ;
	if ( pTmp ) gHyp_data_delete ( pTmp ) ;
	gHyp_instance_error ( 
          pAI, STATUS_BOUNDS, 
	  "Destination subscript is out of bounds in merge()");
      }
      if ( isVectorDst ) 
        sdv = contextDst ;
      else
	sdv = 0 ;
    }

    /* Get the next source value. */
    pSrcValue = pNextSrcValue ;
    
    /* Get the next destination value. */
    pDstValue = pNextDstValue ;
    
    /* Done when all values exhausted */
    if ( !pSrcValue && !pDstValue ) break ;

    /* Done if the destination is not dynamic and the count is exceeded */
    if ( !isDynamicResult && count >= countResult ) break ;
    count++ ;

    if ( !pSrcValue ) {

      if ( gHyp_data_getTokenType ( pDstValue )  == TOKEN_REFERENCE ) {
              
	/* Create the result */
	if ( countDst == 1 && countSrc > 1 && isDataMerge ) {
	  if ( counter == 0 ) counter = 1 ;
	  sprintf ( value, "%s%d", gHyp_data_getLabel ( pDstValue ), counter++ ) ;
	}
	else
	  strcpy ( value, gHyp_data_getLabel ( pDstValue ) ) ;

	/* Create new empty list variable */
	pVariable = gHyp_data_new ( value ) ;
      }

      else
        /* No more source values, keep remaining destination values*/
        pVariable = gHyp_data_copyAll ( pDstValue ) ;
      
      gHyp_data_append ( pResult, pVariable ) ;
      continue ;
    }
    
    srcDataType = gHyp_data_getDataType ( pSrcValue ) ;
    srcTokenType = gHyp_data_getTokenType ( pSrcValue ) ;
    pLabel = gHyp_data_getLabel ( pSrcValue ) ;
    labelLen = strlen ( pLabel ) ;

    if ( isMsgMerge &&
         !gHyp_load_isKey ( pLabel ) &&  
	 gHyp_util_getToken3 ( pLabel ) != labelLen ) {


      /* SPECIAL CASE:  Merge called from instance_consumeMessage. */
      doDereference = TRUE ;	

      if ( firstPass ) {
	/*gHyp_util_debug("## { ");*/
        pStream = gHyp_load_fromStream ( pAI, pHyp, "{", lineCount++ ) ;
        if ( !pStream || *pStream ) {
          gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
  	  if ( pTmp ) gHyp_data_delete ( pTmp ) ;
          gHyp_instance_error ( pAI,
			    STATUS_UNDEFINED,
			    "Failed to load HyperScript segment '{'" ) ;
	}
        firstPass = FALSE ;
      }
      
      if ( pDstValue ) {

	/*gHyp_util_debug("## merge ( ");*/
        pStream = gHyp_load_fromStream ( pAI, pHyp, "merge ( ", lineCount++ ) ;
        if ( !pStream || *pStream ) {
          gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	  if ( pTmp ) gHyp_data_delete ( pTmp ) ;
          gHyp_instance_error ( pAI,
			    STATUS_UNDEFINED,
			    "Failed to load HyperScript segment 'merge ( '" ) ;
	}
	contentLength = 0 ;
        pValue = gHyp_fileio_describeData ( pAI, pDstValue, ' ', FALSE, &contentLength ) ;
        pToken = NULL ;
        ss = -1 ;
        context = -1 ;
        while ( (pToken = gHyp_data_nextValue ( pValue, 
					        pToken, 
					        &context,
					        ss ) ) ) {
      
          /* Get the value, as a string */
          n = gHyp_data_getStr_nonulls ( pToken, 
			     value, 
			     VALUE_SIZE, 
			     context, 
			     FALSE ) ;
          pStr = value ;

          /* Add the code segment to the stream */
	  /*gHyp_util_debug("## %s",pStr);*/
          pStream = gHyp_load_fromStream ( pAI, pHyp, pStr, lineCount++ ) ;
          if ( !pStream || *pStream ) {
	    gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
  	    if ( pTmp ) gHyp_data_delete ( pTmp ) ;
	    gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "Failed to load HyperScript segment '%s'",
			      pStr ) ;
	  }
	}
        gHyp_data_delete ( pValue ) ;

	/*gHyp_util_debug("## , ");*/
        pStream = gHyp_load_fromStream ( pAI, pHyp, ",", lineCount++ ) ;
        if ( !pStream || *pStream ) {
          gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
  	  if ( pTmp ) gHyp_data_delete ( pTmp ) ;
          gHyp_instance_error ( pAI,
			    STATUS_UNDEFINED,
			    "Failed to load HyperScript segment ' , '" ) ;
	}

      }

      /* Insert the source value(s), its already been described. */
      /*gHyp_util_debug("## %s ",pLabel );*/
      pStream = gHyp_load_fromStream ( pAI, pHyp, pLabel, lineCount++ ) ;
      if ( !pStream || *pStream ) {
        gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	if ( pTmp ) gHyp_data_delete ( pTmp ) ;
        gHyp_instance_error ( pAI,
			    STATUS_UNDEFINED,
			    "Failed to load HyperScript segment '%s'", pLabel ) ;
      }

      pToken = NULL ;
      ss = -1 ;
      context = -1 ;
      while ( (pToken = gHyp_data_nextValue ( pSrcValue, 
					      pToken, 
					      &context,
					      ss ) ) ) {
      
        /* Get the value, as a string */
        n = gHyp_data_getStr_nonulls ( pToken, 
				value, 
				VALUE_SIZE, 
				context, 
				FALSE ) ;
        pStr = value ;

        /* Add the code segment to the stream */
	/*gHyp_util_debug("## %s",pStr);*/
        pStream = gHyp_load_fromStream ( pAI, pHyp, pStr, lineCount++ ) ;
        if ( !pStream || *pStream ) {
	  gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
  	  if ( pTmp ) gHyp_data_delete ( pTmp ) ;
	  gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "Failed to load HyperScript segment '%s'",
			      pStr ) ;
	}
      }

      if ( pDstValue ) {

	/*gHyp_util_debug("## )");*/
        pStream = gHyp_load_fromStream ( pAI, pHyp, ")", lineCount++ ) ;

        if ( !pStream || *pStream ) {
          gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	  if ( pTmp ) gHyp_data_delete ( pTmp ) ;
          gHyp_instance_error ( pAI,
			    STATUS_UNDEFINED,
			    "Failed to load HyperScript segment ')'" ) ;
	} 

      }

      /*gHyp_util_debug("## ;");*/
      pStream = gHyp_load_fromStream ( pAI, pHyp, ";", lineCount++ ) ;

      if ( !pStream || *pStream ) {
        gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	if ( pTmp ) gHyp_data_delete ( pTmp ) ;
        gHyp_instance_error ( pAI,
		    STATUS_UNDEFINED,
			    "Failed to load HyperScript segment ';'" ) ;
      } 

      continue ;
    }    

    
    if ( pDstValue ) {
      
      /* Destination and source values exist */
      dstDataType = gHyp_data_getDataType ( pDstValue ) ;
      dstTokenType = gHyp_data_getTokenType ( pDstValue ) ;

      if ( isVectorDst ) {

	/* Replace destination vector element with that from source. */
	gHyp_data_setVector ( pResult, 
			      pSrcValue, 
			      sdv,
			      ssv,
			      isVectorSrc ) ;

      }
      else if ( dstTokenType == TOKEN_CONSTANT || 
		dstTokenType == TOKEN_LITERAL ) {
 
	/* Replace destination element constant/literal with that from source*/
	if ( isVectorSrc ) {
      
	  pValue = gHyp_data_new ( NULL ) ;
	  gHyp_data_newConstant ( pValue, 
				  srcDataType, 
				  pSrcValue, 
				  ssv ) ;
	}
	else 
	  pValue = gHyp_data_copyAll ( pSrcValue ) ;

	gHyp_data_append ( pResult, pValue ) ;
      }

      else if ( dstTokenType == TOKEN_REFERENCE ) {

        /* Create the result */
	if ( countDst == 1 && countSrc > 1 && isDataMerge ) {
	  if ( counter == 0 ) counter = 1 ;
	  sprintf ( value, "%s%d", gHyp_data_getLabel ( pDstValue ), counter++ ) ;
	}
	else
	  strcpy ( value, gHyp_data_getLabel ( pDstValue ) ) ;

	/* Destination is just am empty label */
	if ( srcTokenType == TOKEN_VARIABLE || srcTokenType == TOKEN_REFERENCE ) {

	  if ( srcTokenType == TOKEN_REFERENCE ) {
	    /*
	    pValue = gHyp_frame_getVariable ( gHyp_instance_frame(pAI), pAI, pSrcValue ) ;
	    */
	    pVariable = gHyp_data_new ( value ) ;
	    pValue = gHyp_data_new ( NULL ) ;
	    gHyp_data_setReference (  pValue, 
				      gHyp_data_getLabel(pSrcValue), 
				      NULL ) ;
	    gHyp_data_append ( pVariable, pValue ) ;
	  }
	  else {
	    /* Make copy of source data, then rename it. */
	    pVariable = gHyp_data_copyAll ( pSrcValue ) ;
	  }
	  gHyp_data_setLabel ( pVariable, value ) ;
	  gHyp_data_append ( pResult, pVariable ) ;
	}
	else {

	  /* Create new variable, then insert contents of source */
	  pVariable = gHyp_data_new ( NULL ) ;
	  gHyp_data_setVariable (pVariable, value, srcDataType);
	  if ( srcDataType > TYPE_STRING )
	     gHyp_data_setVector ( pVariable, 
				   pSrcValue, 
				   0,
				   ssv,
				   isVectorSrc ) ;
	  else
	    gHyp_data_copyValues ( pVariable, pSrcValue ) ;
	  
	  gHyp_data_append ( pResult, pVariable ) ;
	}
      }
      else {

	/* Destination must be VARIABLE LIST or STRING */
	
	/*
	 *gHyp_util_debug(  "Destination %s, count=%d",
			    gHyp_data_print(pDstValue),
			    gHyp_data_getCount ( pDstValue )) ;
	 *gHyp_util_debug(  "Source %s, count=%d",
			    gHyp_data_print(pSrcValue),
			    gHyp_data_getCount ( pSrcValue )) ;
	 */

	/* If source is empty but the destination variable 
	 * has a default value, then don't change the destination.
	 */
	
	if ( gHyp_data_getCount ( pSrcValue ) == 0 &&
	     gHyp_data_getCount ( pDstValue ) > 0 ) {
	  
	  /* Keep destination values. */
	  pVariable = gHyp_data_new ( gHyp_data_getLabel ( pDstValue ) ) ;
	  gHyp_data_copyValues ( pVariable, pDstValue ) ;
	  gHyp_data_append ( pResult, pVariable ) ;
	}
	else if ( isVectorSrc ) {
	    
	  pValue = gHyp_data_new ( NULL ) ;
	  gHyp_data_newConstant ( pValue, 
				  srcDataType, 
				  pSrcValue, 
				  ssv ) ;
	  gHyp_data_append ( pResult, pValue ) ;
	}
	else {

	  /* Destination is a list or string variable and destination is a list or string */
	  if ( countDst == 1 && countSrc > 1 && isDataMerge ) 
	    counter2 = 1 ;
	  else
	    counter2 = 0 ;

	  pData = gHyp_env_mergeData (	pDstValue,
					pSrcValue,
					pAI,
					counter2,
					isDataMerge,
					FALSE,
					NULL ) ;

	  if ( dstDataType == TYPE_STRING ) {

	    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
	    pValue = NULL ;
	    ss = -1 ; 
	    context = -1 ;

	    /* Make a copy of just the top level variable */
	    pData2 = gHyp_data_copy ( pData ) ;

	    /* Add all pData's contents in str format */
	    while ( (pValue = gHyp_data_nextValue ( pData, 
						    pValue, 
						    &context,
						    ss ) ) ) {
	      n = gHyp_data_getStr (  pValue,
	      			      value, 
				      VALUE_SIZE, 
				      context, 
				      isVector ) ;
	      pValue2 = gHyp_data_new ( NULL ) ;
	      gHyp_data_setStr2 ( pValue2, value, n ) ;
	      gHyp_data_append ( pData2, pValue2 ) ;
	    }
	    
	    gHyp_data_delete ( pData ) ;
	    pData = pData2 ;
	  }

	  gHyp_data_append ( pResult, pData ) ;
	  
	}
      }
    }
    else {

      /* No more destination values, but still some source values */
      if ( isVectorDst ) {

	/* Replace destination vector element with that from source. */
	gHyp_data_setVector ( pResult, 
			      pSrcValue, 
			      sdv++,
			      ssv,
			      isVectorSrc ) ;
      }
      else {

	/* List or constant or literal or reference destination.*/

	if ( countDst == 1 && pLastDstValue &&
	     dstTokenType == TOKEN_VARIABLE &&
	     dstDataType <= TYPE_STRING ) {

	  /* If the destination count is 1 and the destination is a list variable,
	   * then append the source values to the destination values. 
	   */
	  if ( isDataMerge ) 
	    counter2 = contextSrc+1 ;
	  else
	    counter2 = 0 ;

	  gHyp_data_append ( pResult, 
			     gHyp_env_mergeData (  pLastDstValue,
						   pSrcValue,
						   pAI,
						   counter2,
						   isDataMerge,
						   FALSE,
						   NULL ));
	}
	else {

	  /* Source is a list/vector and destination is either a CONSTANT,LITERAL,or REFERENCE*/
	  if ( isVectorSrc ) {
	    
	    pValue = gHyp_data_new ( NULL ) ;
	    gHyp_data_newConstant ( pValue, 
				    srcDataType, 
				    pSrcValue, 
				    ssv ) ;
	  }
	  else {

	    if ( dstTokenType == TOKEN_REFERENCE ) {

	      /* Create the result */
	      if ( countDst == 1 && countSrc > 1 && isDataMerge ) {
	        if ( counter == 0 ) counter = 1 ;
		sprintf ( value, "%s%d", gHyp_data_getLabel ( pLastDstValue ), counter++ ) ;
	      }
	      else
		strcpy ( value, gHyp_data_getLabel ( pLastDstValue ) ) ;

	      /* Destination is just am empty label */
	      if ( srcTokenType == TOKEN_VARIABLE ) {

		/* Make copy of source data, then rename it. */
		pValue = gHyp_data_copyAll ( pSrcValue ) ;
		gHyp_data_setLabel ( pValue, value ) ;
		gHyp_data_append ( pResult, pValue ) ;
	      }
	      else {

		/* Create new variable, then insert contents of source */
		pValue = gHyp_data_new ( NULL ) ;
		gHyp_data_setVariable (pValue, value, srcDataType);
		if ( srcDataType > TYPE_STRING )
		  gHyp_data_setVector ( pValue, 
		  			pSrcValue, 
					sdv,
					ssv,
					isVectorSrc ) ;
		else
		  gHyp_data_copyValues ( pValue, pSrcValue ) ;
	      }
	    }
	    else {
	      /* Copy first, then append. */
	      pValue = gHyp_data_copyAll ( pSrcValue ) ;
	    }
	  }
	  gHyp_data_append ( pResult, pValue ) ;
	}
      }
    }
  }

  if ( doDereference ) {

    /*gHyp_util_debug("## } ");*/
    pStream = gHyp_load_fromStream ( pAI, pHyp, "}", lineCount++ ) ;
    if ( !pStream || *pStream ) {
      gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
      if ( pTmp ) gHyp_data_delete ( pTmp ) ;
      gHyp_instance_error ( pAI,
			    STATUS_UNDEFINED,
			    "Failed to load HyperScript segment '{'" ) ;
    }
    gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_MSGARGS ) ;
    gHyp_instance_setDerefHandler ( pAI, 
				    hypIndex, 
				    pHyp ) ; 
  }

  return pResult ;
}

void gHyp_env_sort ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sort ( variable )
   *	Returns 1 if sorted, 0 otherwise
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
  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;
  
  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pVariable,
      *pData ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: sort ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
 
    /* Locate the variable */
    if ( (pVariable = gHyp_data_getVariable ( pData )) &&
         gHyp_data_getDataType ( pVariable ) < TYPE_BYTE )
      gHyp_sort_sort ( pVariable ) ;
    else
      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, "Cannot sort non-list variable '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
      
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_reverse ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: reverse ( variable )
   *	Returns 1 if sorted, 0 otherwise
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
  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;
  
  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pVariable,
      *pData ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: reverse ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Locate the variable */
    if ( (pVariable = gHyp_data_getVariable ( pData )) &&
         gHyp_data_getDataType ( pVariable ) < TYPE_BYTE )
      gHyp_sort_reverse ( pVariable ) ;
    else
      gHyp_instance_warning ( pAI, STATUS_UNDEFINED, "Cannot reverse non-list variable '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
      
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_insert ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: insert ( dst, src )
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
      *pDst,
      *pSrc,
      *pVariable,
      *pSrcVariable;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: insert ( destination, source )" ) ;

    pSrc = gHyp_stack_popRdata2 ( pStack, pAI ) ;
    pDst = gHyp_stack_popLvalue ( pStack, pAI ) ;
    
    /* Make sure the destination is a list variable */
    if ( (pVariable = gHyp_data_getVariable ( pDst )) &&
	 pVariable != pDst &&
	 gHyp_data_dataType ( pVariable ) < TYPE_BYTE ) {

      if ( (pSrcVariable = gHyp_data_getVariable ( pSrc ) ) ) {
	gHyp_data_detach ( pSrcVariable ) ;
	if ( pSrc != pSrcVariable ) gHyp_data_delete ( pSrc ) ;
	pSrc = pSrcVariable ;

      }
      gHyp_data_insert ( pVariable, pSrc ) ;
    }
    else {
      gHyp_data_delete ( pSrc ) ;
      gHyp_instance_error ( pAI,
			    STATUS_INVALID, 
			    "'%s' is not a list variable, or is a temporary variable",
			    gHyp_data_getLabel ( pDst ) ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_append ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: append ( dst, src )
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
      *pDst,
      *pSrc,
      *pVariable,
      *pSrcVariable ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: status = append ( destination, source )" ) ;

    pSrc = gHyp_stack_popRdata2 ( pStack, pAI ) ;
    pDst = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Make sure the destination is a list variable */
    if ( (pVariable = gHyp_data_getVariable ( pDst )) &&
	 pVariable != pDst &&
	 gHyp_data_dataType ( pVariable ) < TYPE_BYTE ) {

      if ( (pSrcVariable = gHyp_data_getVariable ( pSrc )) ) {

	/* Variable exists */
	gHyp_data_detach ( pSrcVariable ) ;
	if ( pSrc != pSrcVariable ) gHyp_data_delete ( pSrc ) ;
	pSrc = pSrcVariable ;
      }

      gHyp_data_append ( pVariable, pSrc ) ;
    }
    else {
      gHyp_data_delete ( pSrc ) ;
      gHyp_instance_error ( pAI, 
			    STATUS_INVALID, 
			    "'%s' is not a list or str variable, or is a temporary variable",
			    gHyp_data_getLabel ( pDst ) ) ;
     }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_remove ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: remove ( variable )
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
   *	A reference to the variable removed.
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
      *pVariable,
      *pResult=NULL ;

    sLOGICAL
      isVariable ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: value = remove ( variable )" ) ;

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Make sure the argument is a list variable */
    if ( (pVariable = gHyp_data_getVariable ( pData )) &&
	  gHyp_data_dataType ( pVariable ) < TYPE_BYTE ) {

      pResult = gHyp_data_getLast ( pVariable ) ;

      if ( pResult ) {

        isVariable = ( gHyp_data_tokenType ( pResult ) == TOKEN_VARIABLE ) ;
	if ( isVariable )
          pVariable = gHyp_frame_findVariable ( pAI, pFrame, 
					    gHyp_data_getLabel ( pResult ) ) ;
        else
	  pVariable = NULL ;

 	if ( gHyp_parse_exprCount ( pParse ) == 0 ) {

 	  if ( pVariable ) {

	    /* Variable already exists. */
 	    gHyp_data_deleteValues ( pVariable ) ;
	    gHyp_data_detach ( pResult ) ;
	    gHyp_data_moveValues ( pVariable, pResult ) ;
	    gHyp_data_delete ( pResult ) ;
	    pResult = NULL ;
	  }
	  else if ( isVariable ) {
	    /* Create new variable */
	    pVariable = gHyp_data_detach ( pResult ) ;
	    gHyp_data_append( gHyp_frame_getMethodData(pFrame),pResult ) ;
	    pResult = NULL ;
 	  }
	  else {
	    gHyp_instance_warning ( pAI, STATUS_INVALID, 
				   "Cannot create variable from '%s'",
				    gHyp_data_getLabel ( pResult ) ) ;
	  }
	}
        else {
	  gHyp_data_detach ( pResult ) ;
	}
      }
      else {
	gHyp_instance_warning ( pAI, 
			      STATUS_BOUNDS, 
			      "No value to remove from '%s'",
			      gHyp_data_getLabel ( pVariable ) ) ;
      }
    }
    else {
      gHyp_instance_error ( pAI, 
			      STATUS_INVALID, 
			      "'%s' is not a list or str variable",
			      gHyp_data_getLabel ( pData ) ) ;    
    }

    if ( pResult ) 
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_chop ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: chop ( variable )
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
      *pVariable,
      *pResult=NULL ;

    sLOGICAL
      isVariable ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: value = chop ( variable )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

     pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Make sure the argument is a list variable */
    if ( (pVariable = gHyp_data_getVariable ( pData )) &&
	  gHyp_data_dataType ( pVariable ) < TYPE_BYTE ) {


      pResult = gHyp_data_getFirst ( pVariable ) ;

      if ( pResult ) {

        isVariable = ( gHyp_data_tokenType ( pResult ) == TOKEN_VARIABLE ) ;
	if ( isVariable )
          pVariable = gHyp_frame_findVariable ( pAI, pFrame, 
					    gHyp_data_getLabel ( pResult ) ) ;
        else
	  pVariable = NULL ;

 	if ( gHyp_parse_exprCount ( pParse ) == 0 ) {

 	  if ( pVariable ) {

	    /* Variable already exists. */
	    gHyp_data_deleteValues ( pVariable ) ;
	    gHyp_data_detach ( pResult ) ;
	    gHyp_data_moveValues ( pVariable, pResult ) ;
	    gHyp_data_delete ( pResult ) ;
	    pResult = NULL ;
	  }
	  else if ( isVariable ) {
	    /* Create new variable */
	    pVariable = gHyp_data_detach ( pResult ) ;
	    gHyp_data_append( gHyp_frame_getMethodData(pFrame),pResult ) ;
	    pResult = NULL ;
 	  }
	  else {
	    gHyp_instance_warning ( pAI, STATUS_INVALID, 
				   "Cannot create variable from '%s'",
				    gHyp_data_getLabel ( pResult ) ) ;
	  }
	}
        else {
	  gHyp_data_detach ( pResult ) ;
	}
      }
      else {
	gHyp_instance_warning ( pAI, 
			      STATUS_BOUNDS, 
			      "No value to remove from '%s'",
			      gHyp_data_getLabel ( pVariable ) ) ;
      }
    }
    else {
      gHyp_instance_error ( pAI, 
			      STATUS_INVALID, 
			      "'%s' is not a list or str variable",
			      gHyp_data_getLabel ( pData ) ) ;    
    }

    if ( pResult ) 
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_detach ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: detach ( variable )
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
   *	TRUE/FALSE.
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
      *pVariable,
      *pResult=NULL ;
    
    sLOGICAL
      isVariable ;

    int
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: detach ( variable )" ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    ss = gHyp_data_getSubScript ( pData ) ;
    if ( ss == -1 )
      pResult = gHyp_data_getVariable ( pData ) ;
    else
      pResult = gHyp_data_getValue ( pData, ss, TRUE) ;

    /* If the detach function is used without a return argument, the variable detached
     * is created in the current methodData scope.
     */
    if ( pResult ) {

      isVariable = ( gHyp_data_tokenType ( pResult ) == TOKEN_VARIABLE ) ;

      if ( isVariable )
        pVariable = gHyp_frame_findVariable ( pAI, pFrame, 
					    gHyp_data_getLabel ( pResult ) ) ;
      else
	pVariable = NULL ;
      
      if ( gHyp_parse_exprCount ( pParse ) == 0 ) {

        /* Detach as variable */

        if ( isVariable && pData != pResult ) {
	  
	  /* We are detaching a variable */
	  if ( pVariable ) {

	    /* Variable of the same name already exists */
	    if ( pVariable == pResult ) {
	      /* Variable detaching itself - do not destroy variable!  */
	      pResult = gHyp_data_new ( gHyp_data_getLabel ( pResult ) ) ;
	      gHyp_data_moveValues ( pResult, pVariable ) ;
	    }
	    else {
	      /* Move new values into existing variable */
	      gHyp_data_deleteValues ( pVariable ) ;
	      gHyp_data_detach ( pResult ) ;
	      gHyp_data_moveValues ( pVariable, pResult ) ;
	      gHyp_data_delete ( pResult ) ;
	      pResult = NULL ;
	    }
	  }
	  else {
	    /* Create new variable */
	    pVariable = gHyp_data_detach ( pResult ) ;
	    gHyp_data_append( gHyp_frame_getMethodData(pFrame),pVariable ) ;
	    pResult = NULL ;
	  }
	}
      	else {
	  gHyp_instance_warning ( pAI, STATUS_INVALID, "Cannot create variable from '%s'",
			      gHyp_data_getLabel ( pResult ) ) ;
	}
      }
      else {

      	/* Detach for expression */
        if ( isVariable && pVariable == pResult ) {
	  /* Variable detaching itself - do not destroy variable!  */
	  pResult = gHyp_data_new ( gHyp_data_getLabel ( pResult ) ) ;
	  gHyp_data_moveValues ( pResult, pVariable ) ;
	}
	else {
	  gHyp_data_detach ( pResult ) ;
	}
      }
    }
    else {
      gHyp_instance_warning ( pAI, 
			      STATUS_BOUNDS, 
			      "No value to detach from '%s'",
			      gHyp_data_getLabel ( pData ) ) ;    
    }

    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


void gHyp_env_quiet ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: quiet()
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
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    sData
      *pData ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: quiet ( boolean )" ) ;


    if ( argCount == 1 ) {
      /* Pop the value off of the stack	 */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      if ( gHyp_data_getBool ( pData,
			       gHyp_data_getSubScript ( pData ),
			       TRUE )  )
	guRunFlags |= RUN_QUIET ;
      else
	guRunFlags &= ~RUN_QUIET ;
    }
    else
      guRunFlags |= RUN_QUIET ;
    
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_env_verify ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: verify()
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
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: verify ( boolean )" ) ;


    if ( argCount == 1 ) {
      /* Pop the value off of the stack	 */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      if ( gHyp_data_getBool ( pData,
			       gHyp_data_getSubScript ( pData ),
			       TRUE )  )
	guRunFlags |= RUN_VERIFY ;
      else
	guRunFlags &= ~RUN_VERIFY ;
    }
    else
      guRunFlags |= RUN_VERIFY ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}



void gHyp_env_version ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: version()
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

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    sData	*pResult ;
   
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    			   "Invalid arguments. Usage: version ( )" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, VERSION_HYPERSCRIPT ) ; 
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_env_localhost ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: localhost()
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

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    sData	*pResult ;
   
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    			   "Invalid arguments. Usage: localhost ( )" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, gzLocalHost ) ; 
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_env_localaddr ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: localaddr()
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

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    sData	*pResult ;
   
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    			   "Invalid arguments. Usage: localaddr ( )" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, gzLocalAddr ) ; 
    gHyp_stack_push ( pStack, pResult ) ;
  }
}
