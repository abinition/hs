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
 *	$Log: branch.c,v $
 *	Revision 1.5  2007-07-09 05:39:00  bergsma
 *	TLOGV3
 *	
 *	Revision 1.3  2004-04-29 01:55:22  bergsma
 *	Added 'on_connect' function.
 *	
 *	Revision 1.2  2002/11/14 01:40:45  bergsma
 *	Removed ^M occurances and added Modification section.
 *	
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/**********************	FUNCTION DEFINITIONS ********************************/

void gHyp_branch_goto ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'goto' statement.
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

    gHyp_parse_statement (	pParse, 
				pCode, 
				pAI, 
				G_STMT_GOTO, 
				G_GOTO_LABEL ) ;

  else {

    sStack	*pStack = gHyp_frame_stack ( pFrame ) ;
    sData 	*pData ;
    char	*pLabel ;
    int		argCount = gHyp_parse_argCount ( pParse ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: goto label" ) ;

    /* Execute the 'goto' function */
        
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ; 
    pLabel = gHyp_data_getLabel ( pData ) ;
    
    /* Branch to label if defined on local or lower frame levels */
    if ( !gHyp_frame_goto ( pFrame, pLabel ) ) { 
      gHyp_instance_error ( 
      	pAI, STATUS_BRANCH, "Could not find label '%s'", pLabel ) ;
    }

    return ;
  }
}

void gHyp_branch_on_message ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE  ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_message' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
	/* If executing, define the handler */
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setMessageHandler ( pAI, 
					    gHyp_hyp_index (pCode) + 1,
					    gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;
    }
  }
}

void gHyp_branch_on_error ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE  ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_error' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	  /* If executing, define the handler */
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setErrorHandler ( pAI, 
					  gHyp_hyp_index (pCode) + 1,
					  gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;
    }
  }
}

void gHyp_branch_on_timeout ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE  ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_timeout' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	  /* If executing, define the handler */
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setTimeOutHandler ( pAI, 
					    gHyp_hyp_index (pCode) + 1,
					    gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;

    }
  }
}

void gHyp_branch_on_alarm ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_alarm' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	  /* If executing, define the handler */
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setAlarmHandler ( pAI, 
					  gHyp_hyp_index (pCode) + 1,
					  gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;

    }
  }
}

void gHyp_branch_on_interrupt ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_interrupt' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	  /* If executing, define the handler */
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setIntHandler ( pAI, 
					gHyp_hyp_index (pCode)+1,
					gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;

    }
  }
}
void gHyp_branch_on_hangup ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_hangup' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	  /* If executing, define the handler */
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setHangupHandler ( pAI, 
					gHyp_hyp_index (pCode)+1,
					gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;

    }
  }
}

void gHyp_branch_on_connect ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_connect' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	  /* If executing, define the handler */
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setConnectHandler ( pAI, 
					gHyp_hyp_index (pCode)+1,
					gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;

    }
  }
}


void gHyp_branch_on_pipe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_pipe' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	  /* If executing, define the handler */
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setPipeHandler ( pAI, 
					 gHyp_hyp_index (pCode)+1,
					 gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;

    }
  }
}

void gHyp_branch_on_death ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	Parses/Executes the 'on_death' statement.
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

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
				
  	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_ON_CONDITION, 
				G_PROGRAM_STMT ) ;
  	
  	
	if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	  /* If executing, define the handler */
  	  if ( guDebugFlags & DEBUG_TRACE ) gHyp_util_trace ( "{}" ) ;
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  gHyp_instance_setDeathHandler ( pAI, 
					  gHyp_hyp_index (pCode) + 1,
					  gHyp_frame_getHyp ( pFrame ) ) ;
	}
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement '%s'.",
		gHyp_hyp_token ( pCode ) ) ;

    }
  }
}
