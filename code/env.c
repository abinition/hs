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
 * Revision 1.46  2011-03-14 01:07:54  bergsma
 * Fix node_getnodebyattr
 *
 * Revision 1.45  2011-02-24 23:55:34  bergsma
 * Change some PROTOCOL debugs to DIAGNOSTIC debugs.
 * Get HSDOM working.
 *
 * Revision 1.44  2011-01-08 21:23:41  bergsma
 * Extra pAI arg in frame *Variable* functions
 *
 * Revision 1.43  2010-01-28 04:25:48  bergsma
 * Bug with data_detach, detach first, prevent accidental erasure of value
 *
 * Revision 1.42  2010-01-08 02:44:57  bergsma
 * Added ssl_md5(), enhanced ssl_digest.
 * Fixed urldecode, missing ":"
 * Enabled object calls, ie:  text.strtok( ) and the like...
 *
 * Revision 1.41  2009-12-08 21:00:15  bergsma
 * Comments added
 *
 * Revision 1.40  2009-04-09 19:57:21  bergsma
 * Fixing issues with HTTP and buffer sizing
 *
 * Revision 1.39  2008-09-09 13:48:48  bergsma
 * Make gzStream larger, 4* MAX_INPUT_LENGHT.  Allows bigger XML
 * structures to be parsed (large chrt records from PROMIS).
 *
 * Revision 1.38  2008-08-09 04:45:28  bergsma
 * Initialize tempBuffer in getnodebyattr
 *
 * Revision 1.37  2007-09-03 06:24:08  bergsma
 * No newlines at end of file
 *
 * Revision 1.36  2007-07-11 18:49:21  mhohimer
 * added node_* functions:
gHyp_env_node_name
 * gHyp_env_node_parent
 * gHyp_env_node_root
 * gHyp_env_node_firstchild
 * gHyp_env_node_lastchild
 * gHyp_env_node_nextsibling
 * gHyp_env_node_prevsibling
 * gHyp_env_node_nextfirstcousin
 * gHyp_env_node_prevlastcousin
 * gHyp_env_node_getnodebyattr
 * gHyp_env_node_getnodebyname -- YET TO BE WRITTEN
 *
 * Revision 1.35  2007-05-02 20:34:01  bergsma
 * Fix parseurl function.  Improve various print/debug/log statements.
 * Fix problem with chunked data transfers.
 *
 * Revision 1.34  2007-03-21 17:06:53  bergsma
 * unused variable.
 *
 * Revision 1.33  2007-03-15 01:03:44  bergsma
 * Added smart-join-merge "sjm" function, as an option in mergeData.
 *
 * Revision 1.32  2006-09-25 05:03:17  bergsma
 * Cosmetic.
 *
 * Revision 1.31  2006/09/14 17:12:53  bergsma
 * Typo, "{" was used, should have been "}" in
 * "Failed to load HyperScript segment '}'"
 *
 * Revision 1.30  2006/07/17 16:55:48  bergsma
 * The count function needs and Lvalue as well.
 *
 * Revision 1.29  2006/01/16 18:56:35  bergsma
 * HS 3.6.6
 * 1. Save query timeout events.  Don't let queries repeat indefinitely.
 * 2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 * Revision 1.28  2005/10/15 21:42:09  bergsma
 * Added renameto functionality.
 *
 * Revision 1.27  2005/08/12 01:16:16  bergsma
 * no message
 *
 * Revision 1.26  2005/07/23 22:34:41  bergsma
 * Added insertbefore and insertafter
 *
 * Revision 1.25  2005/05/10 17:31:38  bergsma
 * Added "env" function
 *
 * Revision 1.24  2005/03/29 16:50:45  bergsma
 * V 3.5.2
 * Fix traceback in PROMIS exithandler when HS duplicate process name.
 * Functions chop() and remove() were reversed.
 *
 * Revision 1.23  2005/03/09 04:15:12  bergsma
 * Added appendval and insertval.  Also, disallow append or insert of a variable
 * unto itself.  Needed new gHyp_data_isSilbing function.
 *
 * Revision 1.22  2005/01/31 06:04:43  bergsma
 * Comment change.
 *
 * Revision 1.21  2005/01/10 20:06:32  bergsma
 * In map() function, byte arrays were not being populated correctly.
 *
 * Revision 1.20  2004/11/19 03:45:33  bergsma
 * Comment change
 *
 * Revision 1.19  2004/10/16 04:40:35  bergsma
 * Reworked map() function.  Added unmap() function
 *
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

static char gzStream[(MAX_INPUT_LENGTH*4)+1] ;

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

    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    /*pData = gHyp_stack_popRdata ( pStack, pAI ) ;*/
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

    if ( guDebugFlags & DEBUG_FRAME )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_FRAME,
			   "frame: EXIT (longjmp to 0 from frame %d)",
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

    if ( guDebugFlags & DEBUG_FRAME )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_FRAME,
			   "frame: QUIT (longjmp to 0 from frame %d)",
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

    /* Idle */    
    gHyp_instance_setState ( pAI, STATE_IDLE ) ;
    gHyp_frame_setState ( pFrame, STATE_IDLE ) ;
    gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;
    gHyp_parse_restoreExprRank ( pParse ) ;

    /* If we are the parent, we may turn-off "return to stdIn".*/
    pConcept = gHyp_instance_getConcept(pAI) ;
    if ( gHyp_concept_getConceptInstance ( pConcept ) == pAI ) 
      gHyp_concept_setReturnToStdIn ( pConcept, FALSE ) ;

    if ( guDebugFlags & DEBUG_FRAME )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_FRAME, 
			   "frame: IDLE (longjmp to 1 from frame %d)",
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
				  FALSE,	/* Don't swap the frames */
				  FALSE,	/* Don't swap the data */
				  TRUE ) ;	/* Do reset */
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
					   TRUE,	/* Swap the frames */
					   TRUE,	/* Swap the data */
					   FALSE ) ;	/* Do not reset */

      /* The new instance now has the frame once owned by the concept instance and
       * the concept instance now has a new frame.  However, the _main_ methodData of
       * both frames has been swapped, so that the concept instance still has its
       * global variables and its own _main_. 
       *
       * pAI is still the concept instance, but pFrame, pParse, pStack are owned
       * by pAInew.
       *
       */
      /* Nope * gHyp_instance_swapDevices ( pAInew, pAI ) ; */
      gHyp_instance_setgpAImain ( pAI ) ;
      gHyp_instance_setgpAI ( pAInew ) ;

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
      
      gHyp_frame_deleteVariable ( pAI, pFrame, gHyp_data_getLabel ( pVariable ) ) ;
    
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
	  if ( pRootVariable ) gHyp_frame_deleteRootVariable ( pAI, pFrame, pVarStr ) ;	  
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

static sData* lHyp_env_map ( sData *pDst,
		      char  **ppStream,
		      char  **ppAnchor,
		      char  **ppEndOfStream,
		      sData *pStreamData,
		      sData **ppValue,
		      int   *pContextSrc,
		      int   ss,
		      sLOGICAL  isVectorSrc,
		      sInstance *pAI,
		      sLOGICAL  isLittleEndian,
		      int sss )
{
  sData	
    *pResult,
    *pData,
    *pValue,
    *pDstValue,
    *pNextDstValue,
    *pVariable ;

  char
    *pStream = *ppStream,
    *pAnchor = *ppAnchor,
    *pBuf,
    value[VALUE_SIZE+1] ;

  int
    i,j,
    numElements,
    n,
    streamLen=0,
    dstDataLen,
    contextDst,
    sdv=-1,
    ssd ;

  sLOGICAL
    isVectorDst ;

  sBYTE
    dstDataType,
    dstTokenType ;
  
  sEndian 
    endian ;

  dstTokenType = gHyp_data_getTokenType ( pDst ) ;
  dstDataType =  gHyp_data_getDataType ( pDst ) ;
  dstDataLen = gHyp_data_dataLen ( pDst ) ;
  isVectorDst = ( dstTokenType == TOKEN_VARIABLE && dstDataType > TYPE_STRING ) ;
  ssd = gHyp_data_getSubScript ( pDst ) ; 
  pNextDstValue = pDst ;
  contextDst = -1 ;
  pDstValue = NULL ;

  /* Create a result */
  pResult = gHyp_data_new ( gHyp_data_getLabel ( pDst ) ) ;

  /* Get more stream data if needed */
  pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
					&streamLen, pStreamData, 
					ppValue, pContextSrc, ss, isVectorSrc, 
					NULL ) ;
  *ppStream = pStream ;


  if ( isVectorDst ) {

    numElements = gHyp_data_getCount ( pDst ) ;
    gHyp_data_newVector ( pResult, 
			  dstDataType, 
			  numElements,
			  gHyp_data_isDynamic ( pDst ) ) ;


    for ( j=0; j<numElements; j++ ) {

      if ( !pStream) return pResult ;
      if ( isLittleEndian ) {

	pBuf = pStream ;
        for ( i=dstDataLen-1; i>=0; i-- ) endian.x.b[i] = *pBuf++ ;
        gHyp_data_setVectorRaw ( pResult, 
			       &endian.x.b, 
			       j ) ;
      }
      else
	gHyp_data_setVectorRaw ( pResult, 
  			       pStream, 
			       j ) ;
      pStream += dstDataLen ;
      *ppStream = pStream ;

      /* Get more stream data if needed */
      pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
					&streamLen, pStreamData, 
					ppValue, pContextSrc, ss, isVectorSrc, 
					NULL ) ;
      *ppStream = pStream ;
    }
    return pResult ;
  }

  if ( !pStream ) return pResult ;

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
    /* if ( pDstValue ) gHyp_util_debug ( "Next dst value at %d for %d is %s",contextDst,sdv,gHyp_data_print ( pDstValue )) ;*/

    /* If there is no more source or destination values, we're done. */
    if ( !pDstValue ) break ;

    dstDataType = gHyp_data_dataType ( pDstValue ) ;
    dstTokenType = gHyp_data_tokenType ( pDstValue ) ;
    dstDataLen = gHyp_data_dataLen ( pDstValue ) ;

    if ( dstTokenType != TOKEN_VARIABLE ) {
       
      /* Skip - just keep literals and constants and references */
      pValue = gHyp_data_copy ( pDstValue ) ;
      gHyp_data_append ( pResult, pValue ) ;
      pStream += dstDataLen ;
      *ppStream = pStream ;
    
    }
    else {
      
      if ( dstDataType == TYPE_STRING ) {

	/* Replace destination values with that from the source. */
	pVariable = gHyp_data_new ( gHyp_data_getLabel ( pDstValue ) ) ;
	pValue = gHyp_data_new ( NULL ) ;
	n = MIN ( streamLen, VALUE_SIZE ) ;
	n = MIN ( n, (int) strlen ( pStream ) ) ;

	memcpy ( value, (const char*) pStream, n ) ;
	value[n] = '\0' ;
	gHyp_data_setStr_n ( pValue, value, n ) ;
	gHyp_data_append ( pVariable, pValue ) ;
	gHyp_data_append ( pResult, pVariable ) ;	

	pStream += n ;
	*ppStream = pStream ;

      }
      else {
	
	pData = lHyp_env_map (	pDstValue,
				ppStream,
				ppAnchor,
				ppEndOfStream,
				pStreamData,
		      		ppValue,
				pContextSrc,
				ss,
				isVectorSrc,
		      		pAI,
			        isLittleEndian,
				sdv ) ;
	  	
	pStream = *ppStream ;

	gHyp_data_append ( pResult, pData ) ;
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
      *pValue,
      *pSrc,
      *pSrcData,
      *pDstData,
      *pDst,
      *pResult ;

    char
      *pStream,
      *pAnchor,
      *pEndOfStream ;

    int
      ss,
      contextSrc,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isVectorSrc,
      isLittleEndian = gHyp_util_isLittleEndian() ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: map ( destination, source )" ) ;

    pSrc = gHyp_stack_popRdata ( pStack, pAI ) ;
    pSrcData = gHyp_data_getVariable ( pSrc ) ;
    if ( !pSrcData ) pSrcData = pSrc ;
    
    pDst = gHyp_stack_popRdata ( pStack, pAI ) ;
    pDstData = gHyp_data_getVariable ( pDst ) ;
    if ( !pDstData ) pDstData = pDst ;

    gzStream[0] = '\0' ;
    pStream = gzStream ;
    pAnchor = pStream ;	
    pEndOfStream = pStream ;
    pValue = NULL ;
    contextSrc = -1 ;
    ss = gHyp_data_getSubScript ( pSrcData ) ;
    isVectorSrc = ( gHyp_data_getDataType ( pSrcData ) > TYPE_STRING ) ;
    pResult = lHyp_env_map (  pDstData,
			      &pStream,
			      &pAnchor,
			      &pEndOfStream,
			      pSrcData,
			      &pValue,
			      &contextSrc,
			      ss,
			      isVectorSrc,
			      pAI,
			      isLittleEndian,
			      0) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

static void lHyp_env_unmap ( sData *pResult, 
			     sData *pParent, 
			     int parentContext,
			     sLOGICAL isLittleEndian ) 
{
 
  sData
    *pData,
    *pValue,
    *pValue2 ;

  int
    j,
    n,
    bufferLen,
    maxLen,
    ss,
    context ;

  sLOGICAL
    isParentVector ;

  char
    *pBuf,
    value[VALUE_SIZE+1],
    buffer[INTERNAL_VALUE_SIZE+1],
    *pBuffer ;


  sBYTE
    parentTokenType,
    parentDataType ;

  sEndian
    endian ;

  char	       	
    sb,
    sc ;

  unsigned char
    ub, 
    uc,
    bo,
    bi ;

  pValue = NULL ;
  n = 0 ;
  pBuf = buffer ;

  parentTokenType = gHyp_data_tokenType ( pParent ) ;
  parentDataType = gHyp_data_dataType ( pParent ) ;
  isParentVector = ( parentDataType > TYPE_STRING ) ;

  switch ( parentTokenType ) {

    case TOKEN_REFERENCE :
    case TOKEN_UNIDENTIFIED : 
    
      pBuffer = gHyp_data_getLabel ( pParent ),
      bufferLen = strlen ( pBuffer ) ;

      while ( bufferLen > 0 ) {

	maxLen = MIN ( bufferLen, INTERNAL_VALUE_SIZE ) ;

	if ( n + maxLen > INTERNAL_VALUE_SIZE ) {
	  pValue2 = gHyp_data_new ( NULL ) ;
	  gHyp_data_setStr_n ( pValue2, buffer, n ) ;
	  gHyp_data_append ( pResult, pValue2 ) ;
	  n = 0 ;
	  pBuf = buffer ;
	}

	memcpy ( pBuf, pBuffer, maxLen ) ;
	n += maxLen ;
	pBuf += maxLen ;

	pBuffer += maxLen ;
	bufferLen -= maxLen ;

      }
      break ;
    
    case TOKEN_LITERAL : 

      if ( n > 0 ) {
        pValue2 = gHyp_data_new ( NULL ) ;
        gHyp_data_setStr_n ( pValue2, buffer, n ) ;
        gHyp_data_append ( pResult, pValue2 ) ;
	n = 0 ;
	pBuf = buffer ;
      }
      
      n = gHyp_data_getStr (	pParent, 
	 			value, 
				VALUE_SIZE,
				0,
				FALSE ) ;
      pValue2 = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr_n ( pValue2, value, n  ) ;
      gHyp_data_append ( pResult, pValue2 ) ;
      n = 0 ;
      pBuf = buffer ;

      break ;
    
    case TOKEN_VARIABLE :
    
      if ( parentContext == -1 ) {
 
	/* Non-subscripted, print out all the variable's values */ 
	pData = NULL ;
	context = -1 ;
	ss = -1 ;
	while ( (pData = gHyp_data_nextValue ( pParent, 
					       pData, 
					       &context,
					       ss ))) {

	  if ( isParentVector ) {

	    lHyp_env_unmap ( pResult, pData, context, isLittleEndian ) ;
	  }
	  else {

	    lHyp_env_unmap ( pResult, pData, -1, isLittleEndian ) ;
	  }
	  
	}
	break ;
      }
      /* Fall through to TOKEN_CONSTANT */

    case TOKEN_CONSTANT :

      pBuf = buffer ;
      n = 0 ;
      switch ( parentDataType ) {

	case TYPE_BYTE :
	  sb = (char) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  *pBuf++ = sb ;
	  n = 1 ;
	  break ;
	  
	case TYPE_CHAR :
	case TYPE_ATTR :
	  sc = (char) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  *pBuf++ = sc ;
	  n = 1 ;
	  break ;
	  
	case TYPE_UBYTE :
	  ub = (unsigned char) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  *pBuf++ = ub ;
	  n = 1 ;
	  break ;
	  
	case TYPE_UCHAR :
	  uc = (unsigned char) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  *pBuf++ = uc ;
	  n = 1 ;
	  break ;
	  
	case TYPE_BOOLEAN :
	  bo = (unsigned char) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  *pBuf++ = bo ;
	  n = 1 ;
	  break ;
	  
	case TYPE_BINARY :
	  bi = (unsigned char) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  *pBuf++ = bi ;
	  n = 1 ;
	  break ;
	  
	case TYPE_HEX :
	  endian.x.ul = 
	    (unsigned long) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 4 ;
	  break ;
	  
	case TYPE_OCTAL :
	  endian.x.ul = 
	    (unsigned long) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 4 ;
	  break ;
	  
	case TYPE_UNICODE :
	  endian.x.us = 
	    (unsigned short) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=1; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<2; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 2 ;
	  break ;
	  
	case TYPE_SHORT :
	  endian.x.ss = 
	    (short) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=1; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<2; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 2 ;
	  break ;
	  
	case TYPE_USHORT :
	  endian.x.us = 
	    (unsigned short) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=1; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<2; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 2 ;
	  break ;
	  
	case TYPE_INTEGER :
	case TYPE_LONG :
	  endian.x.sl = 
	    (long) gHyp_data_getInt ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 4 ;
	  break ;
	  
	case TYPE_ULONG :
	  endian.x.ul = 
	    (unsigned long) gHyp_data_getRaw ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 4 ;
	  break ;
	  
	case TYPE_FLOAT :
	  endian.x.f = 
	    (float) gHyp_data_getDouble ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 4 ;

	  break ;
	  
	case TYPE_DOUBLE :
	  endian.x.d = 
	    (double) gHyp_data_getDouble ( pParent, parentContext, isParentVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=7; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<8; j++ ) *pBuf++ = endian.x.b[j] ;
	  n = 8 ;
	  break ;
      }

      pValue2 = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr_n ( pValue2, buffer, n ) ;
      gHyp_data_append ( pResult, pValue2 ) ;
      n = 0 ;
      pBuf = buffer ;

      break ;

  }
  if ( n > 0 ) {
    pValue2 = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr_n ( pValue2, buffer, n ) ;
    gHyp_data_append ( pResult, pValue2 ) ;
    n = 0 ;
    pBuf = buffer ;
  }
  return ;
}

void gHyp_env_unmap ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: unmap ( variable )
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
      *pData,
      *pResult ;

    int
      ss,
      context,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sLOGICAL
      isLittleEndian = gHyp_util_isLittleEndian() ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: unmap ( source )" ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    if ( (pVariable = gHyp_data_getVariable ( pData ) ) )
      pData = pVariable ;

    context = -1 ;
    ss = gHyp_data_getSubScript ( pData ) ;
    pResult = gHyp_data_new ( "_unmap_" ) ;
    lHyp_env_unmap (	pResult,
	  		pData,
			ss,
			isLittleEndian) ;

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
    pResult = gHyp_env_mergeData ( pDstData, pSrcData, pAI, 0, autoIncrement, FALSE, FALSE, NULL ) ;

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

void gHyp_env_sjm ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sjm ( variable, variable )
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
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: sjm ( destination, source )" ) ;

    pSrcData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pDstData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pResult = gHyp_env_mergeData ( pDstData, pSrcData, pAI, 0, FALSE, FALSE, TRUE, NULL ) ;

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
			    sLOGICAL isSmartMerge,
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

    /* Get the next destination value. */
    pDstValue = pNextDstValue ;

    if ( pNextSrcValue ) {
      /* This is either the start of the scan, or there was a previous value.
       * Get the next value from the source data.
       */

      /* If this is a smart merge, then get the source value that
       * has the same name as the destination value
       */
      if ( isSmartMerge && pDstValue ) {
	pNextSrcValue = gHyp_data_getChildByName ( pSrc, gHyp_data_getLabel ( pDstValue ) ) ;
	ssv = 0 ;
      }
      else {
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
    }


    /* Get the next source value. */
    pSrcValue = pNextSrcValue ;
    
    
    /* Done when all values exhausted */
    if ( !pSrcValue && !pDstValue ) break ;

    /* Done if the destination is not dynamic and the count is exceeded */
    if ( !isDynamicResult && count >= countResult ) break ;
    count++ ;

    if ( !pSrcValue ) {

      /*gHyp_util_debug ( "No source value" ) ;*/
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
	 gHyp_util_getToken_okDot ( pLabel ) != labelLen ) {


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
			    "Failed to load HyperScript segment (merge) '{'" ) ;
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
			    "Failed to load HyperScript segment (merge) 'merge ( '" ) ;
	}
	contentLength = 0 ;
        pValue = gHyp_fileio_describeData ( pAI, pDstValue, ' ', FALSE, FALSE, &contentLength ) ;
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
			      "Failed to load HyperScript code segment (merge) '%s'",
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
			    "Failed to load HyperScript segment (merge) ' , '" ) ;
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
			    "Failed to load HyperScript segment label (merge) '%s'", pLabel ) ;
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
			      "Failed to load HyperScript segment (merge) '%s'",
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
			    "Failed to load HyperScript segment (merge) ')'" ) ;
	} 

      }

      /*gHyp_util_debug("## ;");*/
      pStream = gHyp_load_fromStream ( pAI, pHyp, ";", lineCount++ ) ;

      if ( !pStream || *pStream ) {
        gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	if ( pTmp ) gHyp_data_delete ( pTmp ) ;
        gHyp_instance_error ( pAI,
		    STATUS_UNDEFINED,
			    "Failed to load HyperScript segment (merge) ';'" ) ;
      } 

      continue ;
    }    

    
    if ( pDstValue ) {
      
      /*gHyp_util_debug ( "Dst value %s exists",gHyp_data_getLabel(pDstValue) ) ;*/
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

	  /* Destination is a list or string variable and source is a list or string */
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
	      gHyp_data_setStr_n ( pValue2, value, n ) ;
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

      /*gHyp_util_debug ( "No dest value, smart=%d, but still some source",isSmartMerge ) ;*/

      if ( isSmartMerge ) break ;

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
			    "Failed to load HyperScript segment (merge) '}'" ) ;
    }
    gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_MSGARGS ) ;
    /*gHyp_util_debug("Deref from merge");*/
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
      argCount = gHyp_parse_argCount ( pParse ) ;

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

	if ( gHyp_data_isSibling ( pSrcVariable, pVariable ) ) {
          gHyp_data_delete ( pSrc ) ;
	  gHyp_instance_error ( pAI, 
			    STATUS_INVALID, 
			    "Cannot insert a variable into itself" ) ;
	}

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

void gHyp_env_insertbefore ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: insertbefore ( sibling, src )
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
	"Invalid arguments. Usage: insertbefore ( sibling, source )" ) ;

    pSrc = gHyp_stack_popRdata2 ( pStack, pAI ) ;
    pDst = gHyp_stack_popLvalue ( pStack, pAI ) ;
    
    /* Make sure the destination is a list variable */
    if ( (pVariable = gHyp_data_getVariable ( pDst )) &&
	 pVariable != pDst &&
	 gHyp_data_dataType ( pVariable ) < TYPE_BYTE ) {

      if ( (pSrcVariable = gHyp_data_getVariable ( pSrc ) ) ) {

	if ( gHyp_data_isSibling ( pSrcVariable, pVariable ) ) {
          gHyp_data_delete ( pSrc ) ;
	  gHyp_instance_error ( pAI, 
			    STATUS_INVALID, 
			    "Cannot insert a variable into itself" ) ;
	}

	gHyp_data_detach ( pSrcVariable ) ;
	if ( pSrc != pSrcVariable ) gHyp_data_delete ( pSrc ) ;
	pSrc = pSrcVariable ;

      }
      gHyp_data_insertbefore ( pVariable, pSrc ) ;
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

void gHyp_env_insertafter ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: insertafter ( sibling, src )
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
	"Invalid arguments. Usage: insertafter ( sibling, source )" ) ;

    pSrc = gHyp_stack_popRdata2 ( pStack, pAI ) ;
    pDst = gHyp_stack_popLvalue ( pStack, pAI ) ;
    
    /* Make sure the destination is a list variable */
    if ( (pVariable = gHyp_data_getVariable ( pDst )) &&
	 pVariable != pDst &&
	 gHyp_data_dataType ( pVariable ) < TYPE_BYTE ) {

      if ( (pSrcVariable = gHyp_data_getVariable ( pSrc ) ) ) {

	if ( gHyp_data_isSibling ( pSrcVariable, pVariable ) ) {
          gHyp_data_delete ( pSrc ) ;
	  gHyp_instance_error ( pAI, 
			    STATUS_INVALID, 
			    "Cannot insert a variable into itself" ) ;
	}

	gHyp_data_detach ( pSrcVariable ) ;
	if ( pSrc != pSrcVariable ) gHyp_data_delete ( pSrc ) ;
	pSrc = pSrcVariable ;

      }
      gHyp_data_insertafter ( pVariable, pSrc ) ;
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
	if ( gHyp_data_isSibling ( pSrcVariable, pVariable ))  {
          gHyp_data_delete ( pSrc ) ;
	  gHyp_instance_error ( pAI, 
			    STATUS_INVALID, 
			    "Cannot append variable to itself" ) ;
	}

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
	    gHyp_data_detach ( pResult ) ;
 	    gHyp_data_deleteValues ( pVariable ) ;
	    gHyp_data_moveValues ( pVariable, pResult ) ;
	    gHyp_data_delete ( pResult ) ;
	    pResult = NULL ;
	  }
	  else if ( isVariable ) {
	    /* Create new variable */
	    pVariable = gHyp_data_detach ( pResult ) ;
	    gHyp_data_append( gHyp_frame_getMethodData(pFrame),pVariable ) ;
	    pResult = NULL ;
 	  }
	  else {
	    gHyp_instance_warning ( pAI, STATUS_INVALID, 
				   "Cannot create variable from '%s'",
				    gHyp_data_getLabel ( pResult ) ) ;
	    gHyp_data_detach ( pResult ) ;
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
	    gHyp_data_detach ( pResult ) ;
	    gHyp_data_deleteValues ( pVariable ) ;
	    gHyp_data_moveValues ( pVariable, pResult ) ;
	    gHyp_data_delete ( pResult ) ;
	    pResult = NULL ;
	  }
	  else if ( isVariable ) {
	    /* Create new variable */
	    pVariable = gHyp_data_detach ( pResult ) ;
	    gHyp_data_append( gHyp_frame_getMethodData(pFrame),pVariable ) ;
	    pResult = NULL ;
 	  }
	  else {
	    gHyp_data_detach ( pResult ) ;
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
	      gHyp_data_detach ( pResult ) ;
	      gHyp_data_deleteValues ( pVariable ) ;
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
	  gHyp_data_detach ( pResult ) ;
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
    gHyp_data_setStr ( pResult, VERSION_HYPERSCRIPT_BUILD ) ; 
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_env_env ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: env()
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
    			   "Invalid arguments. Usage: env ( )" ) ;

    pResult = gHyp_data_new ( NULL ) ;

#ifdef AS_VMS
    gHyp_data_setStr ( pResult, "VMS" ) ; 
#elif defined ( AS_WINDOWS )
    gHyp_data_setStr ( pResult, "WINDOWS" ) ; 
#elif defined ( AS_UNIX )
    gHyp_data_setStr ( pResult, "UNIX" ) ; 
#else
    gHyp_data_setStr ( pResult, "MAC" ) ; 
#endif

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

void gHyp_env_appendval ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: appendval ( dst, srcval )
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
      *pValue,
      *pValue2 ;
    
    sLOGICAL
      isVector ;

    sBYTE
      dataType ;

    int
      maxCount,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: status = appendval ( destination, sourceval )" ) ;

    pSrc = gHyp_stack_popRdata ( pStack, pAI ) ;
    pDst = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Make sure the destination is a list variable */
    if ( (pVariable = gHyp_data_getVariable ( pDst )) &&
	 pVariable != pDst &&
	 gHyp_data_dataType ( pVariable ) < TYPE_BYTE ) {

      pValue = NULL ;
      ss = gHyp_data_getSubScript ( pSrc ) ;
      context = -1 ;
      isVector = ( gHyp_data_getDataType (pSrc) > TYPE_STRING ) ;
      dataType = gHyp_data_getDataType ( pSrc ) ;
      maxCount = gHyp_data_getCount ( pSrc ) ;
      while ( maxCount-- && 
	      (pValue = gHyp_data_nextValue ( pSrc, 
					     pValue, 
					     &context,
					     ss ))) {

	if ( isVector ) {
      
	  pValue2 = gHyp_data_new ( NULL ) ;
	  gHyp_data_newConstant ( pValue2, 
				  dataType, 
				  pSrc, 
				  context ) ;
	}
	else {
	  /* Copy first, then append. */
	  pValue2 = gHyp_data_copyAll ( pValue ) ;

          /* We need to also copy over any objects, like a method definition. */
          /*gHyp_data_copyObject ( pValue2, pValue ) ;*/
	}
	gHyp_data_append ( pDst, pValue2 ) ;
      }
      if ( context == -2 ) {
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Subscript is out of bounds in value for appendval") ;
      }
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

void gHyp_env_insertval ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: insertval ( dst, srcval )
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
      *pValue,
      *pValue2 ;
    
    sLOGICAL
      isVector ;

    sBYTE
      dataType ;

    int
      maxCount,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: status = insertval ( destination, sourceval )" ) ;

    pSrc = gHyp_stack_popRdata ( pStack, pAI ) ;
    pDst = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Make sure the destination is a list variable */
    if ( (pVariable = gHyp_data_getVariable ( pDst )) &&
	 pVariable != pDst &&
	 gHyp_data_dataType ( pVariable ) < TYPE_BYTE ) {

      pValue = NULL ;
      ss = gHyp_data_getSubScript ( pSrc ) ;
      context = -1 ;
      isVector = ( gHyp_data_getDataType (pSrc) > TYPE_STRING ) ;
      dataType = gHyp_data_getDataType ( pSrc ) ;
      maxCount = gHyp_data_getCount ( pSrc ) ;
      while ( maxCount-- &&
	      (pValue = gHyp_data_nextValue ( pSrc, 
					     pValue, 
					     &context,
					     ss ))) {

	if ( isVector ) {
      
	  pValue2 = gHyp_data_new ( NULL ) ;
	  gHyp_data_newConstant ( pValue2, 
				  dataType, 
				  pSrc, 
				  context ) ;
	}
	else {
	  /* Copy first, then insert. */
	  pValue2 = gHyp_data_copyAll ( pValue ) ;

          /* We need to also copy over any objects, like a method definition. */
          /*gHyp_data_copyObject ( pValue2, pValue ) ;*/
	}
	gHyp_data_insert ( pDst, pValue2 ) ;
      }
      if ( context == -2 ) {
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Subscript is out of bounds in value for insertval") ;
      }
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

void gHyp_env_node_name ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_name ( node )
   *  Returns the name (variable label) of node
   *  
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

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_name ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;    
    
    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s is not a node, node_* functions are intended to be used with arguments of type list",
		gHyp_data_getLabel( pData ) ) ;
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, gHyp_data_getLabel( gHyp_data_getVariable( pData ) ) ) ;
    gHyp_stack_push ( pStack, pResult ) ;   
  }
}

void gHyp_env_node_parent ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_parent ( node )
   *  Returns the label of the parent of node, returns an empy string
   *  if node has no parent
   *  
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
      *pParent,
      *pResult ;
      
    char
      buffer[VALUE_SIZE+1],
      *pBuffer,
      ss[TAG_INDEX_BUFLEN] ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      i ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_parent ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pParent = gHyp_data_getParent( pData ) ;
    pResult = gHyp_data_new ( NULL ) ;
    
    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s is not a node, node_* functions are intended to be used with arguments of type list",
		gHyp_data_getLabel( pData ) ) ;
    }
 
    /* if label of pData matches variable label of pData, then it's a root node with no parent */
    if ( strcmp( gHyp_data_getLabel( pData ), gHyp_data_getLabel( gHyp_data_getVariable( pData ) ) ) == 0 ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s has no parent, it is a root node", gHyp_data_getLabel( pData ) ) ;			
      /* return an empty string */
      gHyp_data_setStr ( pResult, "" ) ;
    }
    else {
	  /* copy label to buffer */
	  pBuffer = buffer;
	  strcpy( pBuffer , gHyp_data_getLabel(pData) );
      pBuffer += strlen( gHyp_data_getLabel(pData) ) ; 
      /*  check for curly bracketed index and remove it*/
	  if( (i = gHyp_data_getTagIndex( pData )) ) {
        sprintf( ss, "{%d}", i ) ;
      	pBuffer -= strlen( ss ) ;
      } 
      /* subtract variable label + 3 character for the 2 "'" and the 1 dot (.) */
      pBuffer -= (3 + strlen( gHyp_data_getLabel( gHyp_data_getVariable(pData) ) ) ) ;
      /* add a null terminator */
      strcpy( pBuffer, "\0" );
      gHyp_data_setStr ( pResult, buffer ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_env_node_root ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_root ( node )
   *  Returns the label of the root of the tree in which node exists
   *  
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
      *pAncestor,
      *pResult ;
      
    char
      buffer[VALUE_SIZE+1],
      *pBuffer,
      ss[TAG_INDEX_BUFLEN] ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      i ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_root ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
	  pResult = gHyp_data_new ( NULL ) ;

    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s is not a node, node_* functions are intended to be used with arguments of type list",
		gHyp_data_getLabel( pData ) ) ;
    }
    /* copy label of node to buffer and set pointer */
    pBuffer = buffer;
    strcpy( pBuffer , gHyp_data_getLabel(pData) );
    pBuffer += strlen( gHyp_data_getLabel(pData) ) ;
    pAncestor = pData;
    /* step up the hierarchy until we reach the root*/
    while ( strcmp( gHyp_data_getLabel( gHyp_data_getParent( pAncestor ) ), "_main_" ) != 0 )
    {
      /*
      gHyp_util_debug("buffer: %s", buffer);
      gHyp_util_debug("node parent: %s", gHyp_data_getLabel( gHyp_data_getVariable( pAncestor )) ) ;
      */
      /*  check for curly bracketed index on the end and remove it*/
      if( (i = gHyp_data_getTagIndex( pAncestor )) ) {
        sprintf( ss, "{%d}", i ) ;
      	pBuffer -= strlen( ss ) ;
      }
	  /* subtract node's variable label + 3 characters for the 2 "'" and the 1 dot (.) */ 
	  pBuffer -= (3 + strlen( gHyp_data_getLabel( gHyp_data_getVariable( pAncestor ) ) ) );
      pAncestor = gHyp_data_getParent( pAncestor );  
    }
    /* add a null  terminator */
    strcpy( pBuffer, "\0" );
    /* push the string to the stack */
    gHyp_data_setStr ( pResult, buffer ) ;
	gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_env_node_firstchild ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_firstchild ( node )
   *  Returns the label of the first child of node or an empty string
   *  if no child is present.
   *  
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
      *pChild,
      *pResult ;

    char
      buffer[VALUE_SIZE+1],
      *pBuffer ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_firstchild ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;

    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s is not a node, node_* functions are intended to be used with arguments of type list",
		gHyp_data_getLabel( pData ) ) ;
    }
    
    if( (pChild = gHyp_data_getFirstNode( pData )) )
    {
      pBuffer = buffer;
      
      /* copy old label to buffer */
      strcpy( pBuffer , gHyp_data_getLabel(pData) );
      pBuffer += strlen( gHyp_data_getLabel(pData) ) ;
      /* stick the child's label  onto the end */
      strcpy( pBuffer, ".'");
      pBuffer += 2;
      strcpy( pBuffer, gHyp_data_getLabel( pChild ) );
      pBuffer += strlen( gHyp_data_getLabel( pChild ) );
      strcpy( pBuffer, "'");
      pBuffer += 1 ;
      /* no need to check if a curly bracketed index is needed cause its the first child */
  
	      /* gHyp_util_debug("New String: %s", buffer ); */
      
      /* push the string to the stack */
      gHyp_data_setStr ( pResult, buffer ) ;
    }
    else {  /* only leaves were found */       
      gHyp_data_setStr ( pResult, "" ) ;
    }

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_env_node_lastchild ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_lastchild ( node )
   *  Returns the label of the last child of node or an empty string
   *  if no child is present.
   *  
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
      *pChild,
      *pResult ;

    char
      buffer[VALUE_SIZE+1],
      *pBuffer ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      i ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_lastchild ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;

    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s is not a node, node_* functions are intended to be used with arguments of type list",
		gHyp_data_getLabel( pData ) ) ;
    }

    if( (pChild = gHyp_data_getLastNode( pData )) ) {

      pBuffer = buffer;
      /* copy old label to buffer */
      strcpy( pBuffer , gHyp_data_getLabel(pData) );
      pBuffer += strlen( gHyp_data_getLabel(pData) ) ;
      /* stick the child's label  onto the end */
      strcpy( pBuffer, ".'");
      pBuffer += 2;
      strcpy( pBuffer, gHyp_data_getLabel( pChild ) );
      pBuffer += strlen( gHyp_data_getLabel( pChild ) );
      strcpy( pBuffer, "'");
      pBuffer += 1 ;
      /* check if a curly bracketed index is needed */
      if ( (i = gHyp_data_getTagIndex( pChild )) ) {
        /* stick the curly index ending onto it */
        sprintf ( pBuffer, "{%d}", i ) ;
      }
      /* gHyp_util_debug("New String: %s", buffer ); */
      gHyp_data_setStr ( pResult, buffer ) ;
    }
    else { /* no child found */
  	  gHyp_data_setStr ( pResult, "" ) ;
	  }
    gHyp_stack_push ( pStack, pResult ) ;  
  }
}

void gHyp_env_node_nextsibling ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_nextsibling ( node )
   *  Returns the label of the next sibling of node or an empty string
   *  if no next sibling is present.
   *  
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
      *pSibling,
      *pResult ;

    char
      buffer[VALUE_SIZE+1],
      *pBuffer ,
      ss[TAG_INDEX_BUFLEN] ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      i ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_nextsibling ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;

    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID,
	  "%s is not a node, node_* functions are intended to be used with arguments of type list",
	  gHyp_data_getLabel( pData ) ) ;
    }
    if ( (pSibling = gHyp_data_getNextNode( pData )) ) {
      pBuffer = buffer;
      /* copy label to buffer */
      strcpy( pBuffer , gHyp_data_getLabel(pData) );     
      pBuffer += strlen( gHyp_data_getLabel(pData) ) ;
      /*  check for curly bracketed index on the end and remove it*/
      if( (i = gHyp_data_getTagIndex( pData )) ) {
        sprintf( ss, "{%d}", i ) ;
      	pBuffer -= strlen( ss ) ;
      }
      /* subtract variable label + 1 character for the ending "'" */
      pBuffer -= (1 + strlen( gHyp_data_getLabel( gHyp_data_getVariable(pData) ) ) );
      /* stick the new label ending onto it */
      strcpy( pBuffer, gHyp_data_getLabel( pSibling ) );
      pBuffer += strlen( gHyp_data_getLabel( pSibling ) );
      strcpy( pBuffer, "'");
      pBuffer += 1 ;
      /* check if a curly bracketed index is needed */
      if ( (i = gHyp_data_getTagIndex( pSibling )) ) {
        /* stick the curly index ending onto it */
        sprintf ( pBuffer, "{%d}", i ) ;
      }
      /* gHyp_util_debug("Buffer String: %s", buffer ); */
      /* push the string to the stack */
      gHyp_data_setStr ( pResult, buffer ) ;
    } 
    else { 
      /* return an empty string */
      gHyp_data_setStr ( pResult, "" ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;  
  }
}

void gHyp_env_node_prevsibling ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_prevsibling ( node )
   *  Returns the label of the previous sibling of node or an empty string
   *  if no next sibling is present.
   *  
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
      *pSibling,
      *pResult ;

    char
      buffer[VALUE_SIZE+1],
      *pBuffer,
      ss[TAG_INDEX_BUFLEN] ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      i ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_prevsibling ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;

    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
        gHyp_instance_warning ( pAI, STATUS_INVALID, 
	    "%s is not a node, node_* functions are intended to be used with arguments of type list",
	    gHyp_data_getLabel( pData ) ) ;
    }
    if ( (pSibling = gHyp_data_getPrevNode ( pData )) ) { 
      pBuffer = buffer;
      /* copy old label to buffer */
      strcpy( pBuffer , gHyp_data_getLabel(pData) );
      pBuffer += strlen( gHyp_data_getLabel(pData) ) ;
      /*  check for curly bracketed index and remove it*/
      if( (i = gHyp_data_getTagIndex( pData )) ) {
        sprintf( ss, "{%d}", i ) ;
      	pBuffer -= strlen( ss ) ;
      }
      /* subtract variable label + 1 character for the ending "'" */
      pBuffer -= (1 + strlen( gHyp_data_getLabel( gHyp_data_getVariable(pData) ) ) );
      /* stick the new label ending onto it */
      strcpy( pBuffer, gHyp_data_getLabel( pSibling ) );
      pBuffer += strlen( gHyp_data_getLabel( pSibling ) );
      strcpy( pBuffer, "'");
      pBuffer += 1 ;
      /* check if a curly bracketed index is needed */
      if ( (i = gHyp_data_getTagIndex( pSibling )) ) {
        /* stick the curly index ending onto it */
        sprintf ( pBuffer, "{%d}", i ) ;
      }
      /* gHyp_util_debug("Buffer String: %s", buffer ); */
      /* push the string to the stack */
      gHyp_data_setStr ( pResult, buffer ) ;
    }
    else { /* return an empty string */
      gHyp_data_setStr ( pResult, "" ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;   
  }
}

void gHyp_env_node_nextfirstcousin ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_nextfirstcousin ( node )
   *  Returns the label of the next first cousin of node
   *  (the first child of the node's parent's next sibling)
   *  or an empty string if no next first cousin is present.
   *  
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
      *pParent,
      *pUncle,
      *pCousin,
      *pResult ;

    char
      buffer[VALUE_SIZE+1],
      *pBuffer,
      ss[TAG_INDEX_BUFLEN] ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      i ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_nextfirstcousin ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
	  pResult = gHyp_data_new ( NULL ) ;

    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s is not a node, node_* functions are intended to be used with arguments of type list",
		gHyp_data_getLabel( pData ) ) ;
    }

    pParent = gHyp_data_getParent( pData );
    
    if ( strcmp( gHyp_data_getLabel( pParent ), "_main_" ) )
    {  
	  /* make sure pParent has a next sibling and that it is not the first of the list (wrap around) */
      if ( (pUncle = gHyp_data_getNextNode( pParent )) )
      {
      	/* gHyp_util_debug("Next uncle found: %s", gHyp_data_getLabel( pUncle ) ); */
        if ( (pCousin = gHyp_data_getFirstNode( pUncle )) ) { 
  	      /* gHyp_util_debug("Next cousin Found: %s", gHyp_data_getLabel( pCousin ) ); */	  	
		  pBuffer = buffer;
	      /* copy node's label to buffer */
	      strcpy( pBuffer , gHyp_data_getLabel(pData) );
	      pBuffer += strlen( gHyp_data_getLabel(pData) ) ;
	      /*  check for curly bracketed index and remove it*/
	      if( (i = gHyp_data_getTagIndex( pData )) ) {
            sprintf( ss, "{%d}", i ) ;
      	    pBuffer -= strlen( ss ) ;
          } 
	      /* subtract child label (and the quotes and dot) */
	      pBuffer -= (3 + strlen( gHyp_data_getLabel( gHyp_data_getVariable(pData) ) ) );
	      /*  check for parent's curly bracketed index and remove it*/
	      if( (i = gHyp_data_getTagIndex( pParent )) ) {
            sprintf( ss, "{%d}", i ) ;
      	    pBuffer -= strlen( ss ) ;
          }
		  /* subtract parent label (and the ending quote mark) */
		  pBuffer -= (1 + strlen( gHyp_data_getLabel( pParent ) ) );
	      /* add uncle's label */
	      strcpy( pBuffer, gHyp_data_getLabel( pUncle ) );
	      pBuffer += strlen( gHyp_data_getLabel( pUncle ) );
	      strcpy( pBuffer, "'.'");
	      pBuffer += 3;
	      /* add cousin's label */
	      strcpy( pBuffer, gHyp_data_getLabel( pCousin ) );
	      pBuffer += strlen( gHyp_data_getLabel( pCousin ) );
	      strcpy( pBuffer, "'");
		  pBuffer += 1 ;
	      /* check if a curly bracketed index is needed */
	      if ( (i = gHyp_data_getTagIndex( pCousin )) ) {
	        /* stick the curly index ending onto it */
	        sprintf ( pBuffer, "{%d}", i ) ;
	      }
	      /* gHyp_util_debug("Buffer String: %s", buffer ); */
	      gHyp_data_setStr ( pResult, buffer ) ;
		}
        else { /* no cousin node found */
          gHyp_data_setStr ( pResult, "" ) ;
          /* gHyp_util_debug("No next cousin node found"); */
        }
      }
      else { /* no uncle found */
      	gHyp_data_setStr ( pResult, "" ) ;
      	/* gHyp_util_debug("No next uncle found"); */
      }
    }
    else { /* no parent found */
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
					"%s has no cousin, it is a root node", gHyp_data_getLabel( pData ) ) ;
    	gHyp_data_setStr ( pResult, "" ) ;
    	/* gHyp_util_debug("No parent found"); */
    }
    gHyp_stack_push ( pStack, pResult ) ;           
  }
}

void gHyp_env_node_prevlastcousin ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
   *
   *	PARSE or EXECUTE the built-in function: node_prevsibling ( node )
   *  Returns the label of the previous last cousin of node
   *  (the last child of the node's parent's previous sibling )
   *  or an empty string if no previous last cousin is present.  
   *  
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
      *pParent,
      *pUncle,
      *pCousin,
      *pResult ;

    char
      buffer[VALUE_SIZE+1],
      *pBuffer,
      ss[TAG_INDEX_BUFLEN] ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      i ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_prevlastcousin ( node )" ) ;
    
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
	  pResult = gHyp_data_new ( NULL ) ;

    /* warn if argument is not a node, it still works though */
    if( gHyp_data_getDataType ( pData ) != TYPE_LIST ) {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s is not a node, node_* functions are intended to be used with arguments of type list",
		gHyp_data_getLabel( pData ) ) ;
    }

    pParent = gHyp_data_getParent( pData );
    
    if ( strcmp( gHyp_data_getLabel( pParent ), "_main_" ) )
    { 
	  /* make sure pParent has a previous sibling and that it is not the first of the list (wrap around) */
      if ( (pUncle = gHyp_data_getPrevNode( pParent )) )
      {
      	/* gHyp_util_debug("Previous uncle found: %s", gHyp_data_getLabel( pUncle ) ); */
        if ( (pCousin = gHyp_data_getLastNode( pUncle )) ) {
  		  /* gHyp_util_debug("Next cousin Found: %s", gHyp_data_getLabel( pCousin ) ); */	  	
		  pBuffer = buffer;
	      /* copy node's label to buffer */
	      strcpy( pBuffer , gHyp_data_getLabel(pData) );
	      pBuffer += strlen( gHyp_data_getLabel(pData) ) ;
	      /*  check for curly bracketed index and remove it*/
	      if( (i = gHyp_data_getTagIndex( pData )) ) {
            sprintf( ss, "{%d}", i ) ;
      	    pBuffer -= strlen( ss ) ;
          }
	      /* subtract child label (and the quotes and dot) */
	      pBuffer -= (3 + strlen( gHyp_data_getLabel( gHyp_data_getVariable(pData) ) ) );
	      /*  check for parent's curly bracketed index and remove it*/
	      if( (i = gHyp_data_getTagIndex( pParent )) ) {
            sprintf( ss, "{%d}", i ) ;
      	    pBuffer -= strlen( ss ) ;
          }
		  /* subtract parent label (and the ending quote mark) */
		  pBuffer -= (1 + strlen( gHyp_data_getLabel( pParent ) ) );
	      /* add uncle's label */
	      strcpy( pBuffer, gHyp_data_getLabel( pUncle ) );
	      pBuffer += strlen( gHyp_data_getLabel( pUncle ) );
	      strcpy( pBuffer, "'.'");
	      pBuffer += 3;
	      /* add cousin's label */
	      strcpy( pBuffer, gHyp_data_getLabel( pCousin ) );
	      pBuffer += strlen( gHyp_data_getLabel( pCousin ) );
	      strcpy( pBuffer, "'");
	      pBuffer += 1 ;
	      /* check if a curly bracketed index is needed */
	      if ( (i = gHyp_data_getTagIndex( pCousin )) ) {
	        /* stick the curly index ending onto it */
	        sprintf ( pBuffer, "{%d}", i ) ;
	      }
	      /* gHyp_util_debug("Buffer String: %s", buffer ); */
	      
	      gHyp_data_setStr ( pResult, buffer ) ;
	    }
		else { /* no cousin found at all */
          gHyp_data_setStr ( pResult, "" ) ;
	      /* gHyp_util_debug("No previous cousin nodes found"); */
		}
      }
      else { /* no uncle found */
      	gHyp_data_setStr ( pResult, "" ) ;
      	/* gHyp_util_debug("No previous uncle found"); */
      }
    }
    else { /* no parent found */
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
		"%s has no cousin, it is a root node", gHyp_data_getLabel( pData ) ) ;
    	gHyp_data_setStr ( pResult, "" ) ;
    	/* gHyp_util_debug("No parent found"); */
    }
    gHyp_stack_push ( pStack, pResult ) ;           
  }
}

void gHyp_env_node_getnodebyattr ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{

  /* Description:
   *
   *  PARSE or EXECUTE the built-in function: node_getnodebyattr ( node, name, value )
   *  Returns the label of the first child of node that contains an attribute
   *  of specified name and value or an empty string if no child is present.
   *  
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
      *pAttrVal,
      *pAttrVar,
      *pChild,
      *pResult ;

    char
      buffer[VALUE_SIZE+1],
      *pBuffer,
      tempBuffer[VALUE_SIZE+1],
      valueBuffer[VALUE_SIZE+1] ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ),
      i ;
          
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 3 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: node_getnodebyattr ( node, attr_name, attr_value )" ) ;
  
    pAttrVal = gHyp_stack_popRvalue( pStack, pAI ) ;
    pAttrVar = gHyp_stack_popRvalue( pStack, pAI ) ;
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;

    /* error if argument are of wrong type */
    if( gHyp_data_getDataType( pData ) != TYPE_LIST ||
        gHyp_data_getDataType( pAttrVar ) != TYPE_STRING ||
        gHyp_data_getDataType( pAttrVal ) != TYPE_STRING ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid argument types. Usage: node_getnodebyattr ( node, attr_name, attr_value )" ) ;

    /* Get the value of the third argument */
    n = gHyp_data_getStr (  pAttrVal, 
			    buffer, 
			    VALUE_SIZE,
			    gHyp_data_getSubScript(pAttrVal),
			    TRUE ) ;
    buffer[n] = '\0' ;

    if( (pChild = gHyp_data_getNodeByAttr( pData,
                                           gHyp_data_getLabel(pAttrVar),
                                           buffer,
                                           valueBuffer )) ) {                               	     
      /*generate the ancestory chain back up to pData*/  
      tempBuffer[0] = '\0' ;				      
      while( pChild != gHyp_data_getVariable( pData ) ) {
      	pBuffer = buffer; 
      	/* gHyp_util_debug("current ancestor: %s\n", gHyp_data_getLabel( pChild ) ); */
      	strcpy( pBuffer, ".'");
        pBuffer += 2;
        strcpy( pBuffer, gHyp_data_getLabel( pChild ) );
        pBuffer += strlen( gHyp_data_getLabel( pChild ) );
        strcpy( pBuffer, "'");
        pBuffer += 1 ;
        /* check if a curly bracketed index is needed */
        if ( (i = gHyp_data_getTagIndex( pChild )) ) {
          sprintf ( pBuffer, "{%d}", i ) ;
        }
      	/* gHyp_util_debug("current ancestor: %s\n", buffer ) ; */
        strcat( buffer, tempBuffer) ;
        strcpy( tempBuffer, buffer) ;
        /* gHyp_util_debug("current ancestry chain: %s\n", tempBuffer ) ; */
      	pChild = gHyp_data_getParent( pChild );
      }                           	
      /* prepend root name to the ancestry chain */
      strcpy( buffer , gHyp_data_getLabel(pData) );
      strcat( buffer, tempBuffer );
      /* gHyp_util_debug("New String: %s", buffer ); */
      /* push the string to the stack */
      gHyp_data_setStr ( pResult, buffer ) ;
    }
    else {  /* no match found */       
      gHyp_data_setStr ( pResult, "" ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_env_node_getnodebyname ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* YET TO BE COMPLETED */
}
