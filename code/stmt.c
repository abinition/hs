/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 2002 Abinition (TM), Inc                      !
!          ***** Program property of Abinition, Inc                           !
!          ***** All rights reserved - Licensed Material.                     !
!
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** Program property of Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!                                                                             !
!*****************************************************************************/

/*
 *  This program is dual-licensed: either;
 *
 *  Under the terms of the GNU General Public License version 3 as 
 *  published by the Free Software Foundation. For the terms of this 
 *  license, see licenses/gplv3.md or <http://www.gnu.org/licenses/>;
 *
 *  Under the terms of the Commercial License as set out in 
 *  licenses/commercial.md
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License or Commerical License for more details.
 */

/*
 * Modifications:
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */
 
/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/**********************	FUNCTION DEFINITIONS ********************************/

void gHyp_stmt_if ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    /* Look next for a '(' */
    gHyp_parse_statement (	pParse, 
				pCode, 
				pAI, 
				G_STMT_IF, 
				G_IF_BEXP ) ;
  else {

    /* Just completed executing the expression in "if ( expression )"
     * Evaluate the truth of the expression.
     * The expression will have left a boolean 1 or 0 on the stack.
     */

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( argCount == 0 )
      gHyp_instance_error ( 	pAI, STATUS_EXPRESSION, 
				"Empty if () expression." ) ;

    /* Evaluate the expression. */
    pData = gHyp_stack_popRdata ( pStack, pAI ) ;    
    if ( gHyp_data_getBool ( pData, gHyp_data_getSubScript(pData), TRUE ) ) 
      gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
    else
      gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
 
    /* If more than one argument, discard arguments 1 through argCount-1 */
    for ( ; argCount > 1 ; argCount-- ) gHyp_stack_popRdata ( pStack, pAI ) ;
    
    return ;
  }
}

void gHyp_stmt_else ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_IF_ELSE :

        /* If the "if (expression)" was false, then turn on execution */
	
	/* Switch to true or false. */ 
        if ( gHyp_frame_isStmtTrue ( pFrame ) ) {

	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  
        }
        else if ( gHyp_frame_isStmtFalse ( pFrame ) )

	  gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	
        /* Change state to expect the start of a new statement */
        gHyp_frame_nextState ( pFrame, G_PROGRAM_STMT ) ;

	break ;

      default :	 
	gHyp_instance_error ( 	pAI, STATUS_PARSE, 
				"Not expecting 'else'." ) ;
	break ;
    }
  }
  return ;
}

void gHyp_stmt_bExp ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the '(' character */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE ) {

    sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT: 
      case G_IF_ELSE :
      case G_LIST_END :

        /* Start a new '( expression )' statement. */
	gHyp_frame_newStmt (	pFrame, 
				STATEMENT_PROGRAM,
				G_STMT_EXP,
				G_STMT_EXP ) ;
				
        /* A new statement means a new parse structure */
        pParse = gHyp_frame_parse ( pFrame ) ;
	
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
	break ;

      case G_IF_BEXP :
      
        /* Begin a 'if' statement expression */
        gHyp_frame_nextState ( pFrame, G_IF_EXP ) ;
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
	break ;
      
      case G_WHILE_BEXP :

        /* Begin a 'while' statement expression */
        gHyp_frame_nextState ( pFrame, G_WHILE_EXP ) ;
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
	break ;

      case G_FOR_BEXP :

        /* Begin the 'for' statement expression statement. */
        gHyp_frame_nextState ( pFrame, G_FOR_EXP1 ) ;
	
	/* Turn off execution for the first expression if this is not
	 * the first pass through the 'for' statement loop.
	 */
	if ( gHyp_frame_isStmtTrue ( pFrame ) && 
	     !gHyp_frame_testLoopFlag ( pFrame, FRAME_LOOP_FOR_EXP1 ) ) 
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	
	/* Begin the expression */
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
	
	break ;

      case G_FUNC_BEXP :

        /* Begin a 'function' statement expression */
        gHyp_frame_nextState ( pFrame, gHyp_frame_currentState ( pFrame ) ) ;
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
	break ;

      case G_STMT_EXP :
      case G_IF_EXP :
      case G_WHILE_EXP :
      case G_FOR_EXP1 :
      case G_FOR_EXP2 :
      case G_FOR_EXP3 :

        /* '(' within an '('.  Continue the expression. */
        gHyp_frame_nextState ( pFrame, gHyp_frame_expectedState ( pFrame ) ) ;
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
	break ;

      default :

        gHyp_instance_error ( pAI, STATUS_PARSE, "Not expecting '('." ) ;
	break ;
    }
  }
  else {

    sStack	
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult,
      *pData,
      *pArg,
      *pArgs;    

    char
      *pLabel ;

    int
      argCount ;

    if ( gHyp_parse_isMethodCall ( pParse ) ) {

      argCount = gHyp_parse_argCount ( pParse ) ;
      
      /* Fetch all method arguments - remove from the stack. 
       * Use gHyp_stack_popRdata2 so that the arguments are not
       * automatically deleted from the stack.
       */
      pArgs = gHyp_data_new ( "_parms_" ) ;
      gpsTempData = pArgs ; 
      for (; argCount > 0; argCount--) {
	pArg = gHyp_stack_popRdata2 ( pStack, pAI ) ;
	gHyp_data_insert ( pArgs, pArg ) ;
      }
      gpsTempData = NULL ; 

      if ( gHyp_stack_peek ( pStack ) ) {

	/* See if the method name is a valid identifier. */
	pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

	/* Assume we will be re-using this pointer, 
	 * ie. putting pData back on the stack 
	 */
	pResult = pData ;
	
	/* As long as we aren't defining a new method, check for the enclosed method */
	if ( !gHyp_parse_isMethodDef ( pParse ) &&
	     !gHyp_parse_isMethodDeferred ( pParse ) ) {
	  
	  if ( gHyp_data_getObjectType ( pData ) == DATA_OBJECT_METHOD )

	    /* Object pointed to has an enclosed method - execute it */ 
	    gHyp_instance_setMethodCall ( pAI ) ;

	  else {

	    /* No method!.  Maybe its at a global level */
	    pLabel = gHyp_data_getLabel ( pData ) ;
	    pData = gHyp_frame_findMethodVariable ( pFrame, pLabel, pAI ) ;
	    if ( pData && 
		 gHyp_data_getObjectType ( pData ) == DATA_OBJECT_METHOD ) {
	      /* Found one. */
	      gHyp_instance_setMethodCall ( pAI ) ;
	      /* Create a reference to the method */
	      pResult = gHyp_data_new ( NULL ) ;
	      gHyp_data_setReference ( pResult, 
				       gHyp_data_getLabel(pData),
				       pData ) ;
	    }
	    else
	      gHyp_instance_warning ( pAI, STATUS_METHOD, 
		    "No method for %s", 
		    pLabel ) ;
	  }
	}
	
	gHyp_stack_push ( pStack, pResult ) ;
	
	/* Put the method args back onto the stack */
	gHyp_stack_push ( pStack, pArgs ) ;

      }
      else {
	/* Stack has no data.  Must be a for loop call */
	gHyp_data_delete ( pArgs ) ;
      }
    }    
    else if ( !gHyp_parse_isFunctionCall ( pParse ) ) {

      /* Just an expression. */
      argCount = gHyp_parse_argCount ( pParse ) ;
      if ( argCount == 0 ) 
	gHyp_instance_error ( pAI, STATUS_EXPRESSION, 
			      "Empty expression ()." ) ;
      
      /* Remove the last argument */
      pArg = gHyp_stack_popRdata2 ( pStack, pAI ) ;
      
      /* Ignore all other arguments - remove from the stack. */
      for (; argCount > 1; argCount--) gHyp_stack_popRdata ( pStack, pAI );
      
      /* Put the last argument back onto the stack */
      gHyp_stack_push ( pStack, pArg ) ;
    } 
  }
}

void gHyp_stmt_eExp ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the ')' character */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    /* Continue the expression */
    gHyp_parse_operator ( pParse, pCode, pAI ) ;
}

void gHyp_stmt_bList ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the '{' character */

  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;
  
  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;
  
  sStack
    *pStack = gHyp_frame_stack ( pFrame ) ;
  
  sCode
    *pMethodCode ;
  
  sMethod
    *pMethod ;

  sData
    *pMethodData,
    *pMethodArgs,
    *pMethodArg,
    *pArgs,
    *pArg,
    *pData,
    *pMethodVariable ;

  sHyp
    *pHyp ;

  sBYTE
    tokenType ;

  int
    n,
    ss,
    context ;

  char
    *pMethodStr,
    currentState ;
	
  if ( isPARSE ) {

    /* Check expected state */
    currentState = gHyp_frame_currentState ( pFrame ) ;
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

    case G_PROGRAM_STMT: 
    case G_IF_ELSE :
    case G_LIST_END :
      
      /* Start a new statement frame. */
      gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_LIST,
				G_LIST_END ) ;
      
      if ( currentState != G_STMT_METHODCALL &&
	   !gHyp_frame_isGlobalScope ( pFrame ) &&
	   !gHyp_frame_testGlobalFlag ( pFrame, FRAME_GLOBAL_MSGARGS ) ) {

	/* Local is set, create local data */
	pMethodData = gHyp_data_new ( "_data_" ) ;
	gHyp_data_setHashed ( pMethodData, TRUE ) ;
	gHyp_frame_setMethodData ( pFrame, pMethodData );
      }
      
      /* If a label definition preceeded the '{', then disable 
       * execution of the block. (A label's block can be executed by a 
       * 'goto <label> ;' from within an external statement block.)
       */
      if ( currentState == G_STMT_LABEL ) 
	gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
      
      break ;
      
    case G_STMT_LIST :
      
      /* A method definition. 
       *
       * 1. The method index (program counter that points to the method name)
       *    has been previously saved in gHyp_parse_expression.
       * 2. The statement block {} following the method definition will have
       *    its execution turned off, so that the contained program 
       *    statements will only be parsed and not executed.
       * 3. A new variable is created with the name of the method.
       */
      
      /* Get the name of the method - saved by gHyp_parse_expression */
      pMethodCode = gHyp_parse_methodCode ( pParse ) ;
      pMethodStr = gHyp_hyp_token ( pMethodCode ) ;
      
      if ( gHyp_frame_isStmtTrue ( pFrame ) ) {
	
	/* Pop the method arguments */
	pArgs = gHyp_stack_popRdata2 ( pStack, pAI ) ;
	
	/* The method name should also be on the stack . Pop it. */
	pMethodVariable = gHyp_stack_popLvalue ( pStack, pAI ) ;
	
	/* Check that the two are the same */
	if ( strcmp ( gHyp_data_getLabel(pMethodVariable), pMethodStr ) != 0 ) {
	  gHyp_data_delete ( pArgs ) ;
	  gHyp_util_logError ( "Method name on stack '%s' != method name in code '%s'",
			       gHyp_data_getLabel ( pMethodVariable ), pMethodStr ) ;
	  longjmp ( gsJmpStack[0], COND_FATAL ) ;
	}
		
	/* Create a new HYP area. Set the line count to zero while we load this method */
	giLineCount = 0 ;
	pHyp = gHyp_hyp_new ( pMethodStr ) ;
	pMethod = gHyp_method_new () ;
	gHyp_method_setHyp ( pMethod, pHyp ) ;
	gHyp_method_setIndex ( pMethod,  gHyp_hyp_index ( pMethodCode ) ) ;
	pMethodVariable = gHyp_data_new ( NULL ) ;
	gHyp_data_setUnidentified ( pMethodVariable, pMethodStr ) ;
	gHyp_data_setObject ( pMethodVariable, 
			      pMethod,
			      DATA_OBJECT_METHOD,
			      (void (*)(void*)) gHyp_method_delete ) ;
	
	/* Check the method arguments.  All must be variable references */
	pArg = NULL ;
	ss = gHyp_data_getSubScript ( pArgs ) ; context = -1 ;
	pMethodArgs = gHyp_data_new ( "args" ) ;

	/* Get next defined method argument */
	n=0;
	while ((pArg = gHyp_data_nextValue ( pArgs, 
					     pArg, 
					     &context,
					     ss ))) {
	  
	  /* The argument should be a "_*_" variable, or its no good. */
	  ss = gHyp_data_getSubScript ( pArg ) ;
	  n++ ;

	  tokenType = gHyp_data_tokenType ( pArg ) ;
	  
	  if ( tokenType == TOKEN_VARIABLE ) {
	    
	    pData = gHyp_data_extractChildBySS ( pArg, ss ) ;
	    if ( !pData ) {
	      gHyp_util_logError (  "No argument to assign!" ) ;
	      gHyp_data_delete ( pArgs ) ;
	      gHyp_data_delete ( pMethodArgs ) ;
	      longjmp ( gsJmpStack[0], COND_FATAL ) ;
	    }
	  
	    /* Make a copy of the argument and store */
	    pMethodArg = pData ; 
	    gHyp_data_append ( pMethodArgs, pMethodArg ) ;
	  }
	  else {
	    gHyp_data_append ( pMethodArgs, gHyp_data_copyAll ( pArg ) ) ;
	  }
	    
	}

	if ( context == -2 ) {
	  gHyp_data_delete ( pArgs ) ;
	  gHyp_data_delete ( pMethodArgs ) ;
	  gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				"Subscript is out of bounds in arg list") ;
	}
	
	/* Set the method arguments */
	gHyp_method_setArgs ( pMethod, pMethodArgs ) ;
	
	/* Delete pArgs, it was popped with gHyp_stack_popRdata2 */
	gHyp_data_delete ( pArgs ) ;

	/* Put the method variable on the stack for use by a typecast */
	gHyp_stack_push ( pStack, pMethodVariable ) ;
      }


      if ( guDebugFlags & DEBUG_FRAME )
	gHyp_util_logDebug (	FRAME_DEPTH_NULL, DEBUG_FRAME,
				"methd: '%s'",
				gHyp_hyp_token ( pMethodCode ) ) ;
      
      	
      gHyp_frame_setStates ( pFrame, G_STMT_METHODDEF, G_STMT_EXP ) ; 
      /*gHyp_frame_setStates ( pFrame, G_STMT_METHODDEF, G_PROGRAM_STMT ) ;*/
      gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_LIST,
				G_LIST_END ) ;
      
      /* Method definitions are not executed. */
      if ( gHyp_frame_isStmtTrue ( pFrame ) ) 
	gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;

      break ;

    case G_STMT_EXP :
    case G_IF_EXP :
    case G_WHILE_EXP :
    case G_FOR_EXP1 :
    case G_FOR_EXP2 :
    case G_FOR_EXP3 :
      
      /* Start of a list in an expression. */
      gHyp_frame_nextState ( pFrame, gHyp_frame_expectedState ( pFrame ) ) ;
      gHyp_parse_expression ( pParse, pCode, pAI ) ;

      break ;
      
    default :

      gHyp_instance_error ( pAI, STATUS_PARSE, "Not expecting '{'." ) ;
      break ;
    }
  }
  else {

    sData
      *pArg,
      *pArgs,
      *pValue,
      *pValue2,
      *pNext,
      *pChild,
      *pList ;

    sLOGICAL
      pushList,
      isVector1,
      isVector2 ;

    sBYTE
      dataType ;

    char
      *pAttr,
      *pLabel,
      newLabel[MAX_INPUT_LENGTH+1],
      value[MAX_INPUT_LENGTH+1],
      value2[MAX_INPUT_LENGTH+1] ;

    int
      os,
      i,
      len,
      ss1,
      ss2,
      context1,
      context2,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Pop all the list elements, putting them in correct order. */
    pArgs = gHyp_data_new ( "_tmp_" ) ;
    gpsTempData = pArgs ;
    while ( argCount-- ) {
      pArg = gHyp_stack_popRdata2 ( pStack, pAI ) ;
      gHyp_data_insert ( pArgs, pArg ) ;	
    }
    gpsTempData = NULL ;

    /* Create a new list of data from the arguments, 
     * splicing all the values from each of the arguments into a new list
     */  
    /*
    sprintf ( variable, "_%s_", gHyp_util_random8() ) ;
    pList = gHyp_data_new ( variable ) ;
    */
    pList = gHyp_data_new ( "_list_" ) ;
    isVector1 = ( gHyp_data_getDataType ( pArgs ) > TYPE_STRING ) ;
    pArg = NULL ;    
    ss1 = -1 ; context1 = -1 ;
    while ( (pArg = gHyp_data_nextValue ( pArgs, 
					  pArg, 
					  &context1,
					  ss1 )) ) {
      pValue = NULL ;
      ss2 = gHyp_data_getSubScript ( pArg ) ;
      context2 = -1 ;
      isVector2 = ( gHyp_data_getDataType (pArg) > TYPE_STRING ) ;
      dataType = gHyp_data_getDataType ( pArg ) ;
      while ((pValue = gHyp_data_nextValue ( pArg, 
					     pValue, 
					     &context2,
					     ss2 ))) {

	if ( isVector2 ) {
      
	  pValue2 = gHyp_data_new ( NULL ) ;
	  gHyp_data_newConstant ( pValue2, 
				  dataType, 
				  pArg, 
				  context2 ) ;
	}
	else {
	  /* Copy first, then append. */
	  pValue2 = gHyp_data_copyAll ( pValue ) ;

          /* We need to also copy over any objects, like a method definition. */
          gHyp_data_copyObject ( pValue2, pValue ) ;
	}
	gHyp_data_append ( pList, pValue2 ) ;
      }
      if ( context2 == -2 ) {
	gHyp_data_delete ( pArgs ) ;
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Subscript is out of bounds in {}") ;
      }
    }

    /* What's on the stack? */    
    pushList = TRUE ;

    pValue = gHyp_stack_peek(pStack) ;
    if ( pValue &&
	 gHyp_data_tokenType (pValue) == TOKEN_REFERENCE &&
	 gHyp_data_getTokenType (pValue) == TOKEN_VARIABLE &&
         /* Support for ?
         (gHyp_data_getDataType (pValue) == TYPE_LIST ||
          gHyp_data_getDataType (pValue) == TYPE_STRING) &&
	 */
	 gHyp_parse_isListCall ( pParse ) ) {

      pValue2 = gHyp_data_getValue ( pList, gHyp_data_getSubScript ( pList ), TRUE ) ;

      if ( pValue2 && gHyp_data_getParent ( pValue2 ) != pValue ) {

        if ( gHyp_data_getDataType (pValue2) == TYPE_ATTR  &&
	     gHyp_data_getTokenType(pValue2) == TOKEN_VARIABLE ) {

	  len = MIN ( MAX_INPUT_LENGTH , gHyp_data_bufferLen ( pValue2, 0 ) ) ;
	  memcpy ( value, gHyp_data_buffer(pValue2,0), len ) ;
	  value[len] = '\0' ;
	  
          pAttr = gHyp_data_getLabel(pValue2) ;

  	  pValue2 = gHyp_data_getReference ( pValue ) ;
  	  pLabel = gHyp_data_getLabel ( pValue2 ) ;

	  pNext = pValue2 ;
	  do {

	    if ( strcmp ( gHyp_data_getLabel ( pNext ), pLabel ) == 0 ) {

	      pChild = gHyp_data_getChildByName ( pNext, pAttr ) ;

	      if ( pChild ) {

    	        len = MIN ( MAX_INPUT_LENGTH , gHyp_data_bufferLen ( pChild, 0 ) ) ;
	        memcpy ( value2, gHyp_data_buffer(pChild,0), len ) ;
  	        value2[len] = '\0' ;

		if ( strcmp ( value, value2 ) == 0 ) {
	       
		  /* Found it */

		  sprintf ( newLabel, "%s{attr '%s'=\"%s\"}",  gHyp_data_getLabel ( pValue ), pAttr, value ) ;
    		  gHyp_data_setReference ( pValue, newLabel, pNext ) ;
		  /*gHyp_util_debug("Setting special reference to %s",gHyp_data_getLabel(pValue));*/
	          pushList = FALSE ;
		  break ;
		}
	      }
	    }
	    pNext = gHyp_data_getNext ( pNext ) ;
	  }
	  while ( pNext != pValue2 ) ;

	}
        else  {

          os = gHyp_data_getInt ( pValue2, -1, FALSE ) ;

  	  pValue2 = gHyp_data_getReference ( pValue ) ;
  	  pLabel = gHyp_data_getLabel ( pValue2 ) ;

	  pNext = pValue2 ;
	  i = 0 ;
	  do {
	    if ( strcmp ( gHyp_data_getLabel ( pNext ), pLabel ) == 0 ) {
	      if ( i == os ) {
		/* Found it */
		sprintf ( newLabel, "%s{%d}",  gHyp_data_getLabel ( pValue ), os ) ;
    		gHyp_data_setReference ( pValue, newLabel, pNext ) ;
		/*gHyp_util_debug("Setting special reference to %s",gHyp_data_getLabel(pValue));*/
		pushList = FALSE ;
		break ;
	      }
	      i++ ;
	    }
	    pNext = gHyp_data_getNext ( pNext ) ;
	  }
	  while ( pNext != pValue2 ) ;
	}
      }
    }

    if ( pushList ) 
      gHyp_stack_push ( pStack, pList ) ;
    else
      gHyp_data_delete ( pList ) ;

    /* Delete the temporary holding variable */
    gHyp_data_delete ( pArgs ) ;
  }
}

void gHyp_stmt_list ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the ',' character */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
    gHyp_parse_operator ( pParse, pCode, pAI ) ;

}

void gHyp_stmt_eList ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the '}' character */

  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;
  
  sParse	
    *pParse = gHyp_frame_parse ( pFrame ) ;


  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_STMT_EXP :
      case G_IF_EXP :
      case G_WHILE_EXP :
      case G_FOR_EXP1 :
      case G_FOR_EXP2 :
      case G_FOR_EXP3 :

        /* Continue the expression. */
        gHyp_frame_nextState ( pFrame, gHyp_frame_expectedState ( pFrame ) ) ;
	gHyp_parse_expression ( pParse, pCode, pAI ) ;
	break ;
	
      case G_LIST_END :

	/* Complete the statement list */
	gHyp_frame_endStmt ( pFrame, pAI ) ;
	break ;

	
      case G_STMT_EOS :

	/* gHyp_frame_return() changed the expected state to from G_LIST_END to STMT_EOS
	 * so that the frame would unwind back to the last CALL frame.
	 * 
	 */
	gHyp_frame_endStmt ( pFrame, pAI ) ;
	break ;

      default :

        gHyp_instance_error ( pAI, STATUS_PARSE, "Not expecting '}', expected state is %d",
			      gHyp_frame_expectedState(pFrame) ) ;
	break ;
    }
  }
}

void gHyp_stmt_bSub ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the '[' character */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operator ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pValue,
      *pList,
      *pArg,
      *pArgs,
      *pData,
      *pSubScript = NULL ;

    char
      *pLabel ;

    int
      isVector,
      ss,
      context,
      argCount;

    argCount = gHyp_parse_argCount ( pParse ) ;

    /* Make sure the subscript value was specified. */
    if ( argCount > 0 ) {

      /* If the subscript is a string, then pretend
       * we were using the dot syntax:
       *
       *  a["b"] is equivalent to a.b
       */
      if ( argCount == 1 && gHyp_data_getDataType (gHyp_stack_peek(pStack)) == TYPE_STRING ) {
	gHyp_operator_dot(pAI, pCode, isPARSE) ;
	return ;
      }

      if ( gHyp_parse_isIndexCall ( pParse ) ) {

        /* Get the subscript */
        pSubScript = gHyp_stack_popRvalue ( pStack, pAI );

        /* Get the subscript value */	
        ss = gHyp_data_getInt ( pSubScript, 
			        gHyp_data_getSubScript ( pSubScript ),
			        TRUE ) ;
        if ( ss < 0 )
          gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				"Subscript is not a positive integer" ) ;

        /* If more than one argument, discard arguments 1 through argCount-1 */
        for ( ; argCount > 1 ; argCount-- ) gHyp_stack_popRdata ( pStack, pAI ) ;

        /* Pop the data to subscript */
        pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

        /* Apply the subscripts */
        gHyp_data_setSubScript ( pData, ss ) ;

        /* Put the data back on the stack with the subscript set */
        gHyp_stack_push ( pStack, pData ) ;

      }
      else {

        /* JSON subscripting */
        /*
        gHyp_instance_error ( 	pAI, STATUS_EXPRESSION, 
				"No operand to subscript." ) ;
        */

        /* Pop all the subscript elements, putting them in correct order. */
        pArgs = gHyp_data_new ( "_tmp_" ) ;
        gpsTempData = pArgs ;
        while ( argCount-- ) {
          pArg = gHyp_stack_popRdata2 ( pStack, pAI ) ;
          gHyp_data_insert ( pArgs, pArg ) ;	
        }
        gpsTempData = NULL ;

        /* Look at the operand */
        pData = gHyp_stack_peek ( pStack ) ;

        /* Create a new list of data from the arguments, 
         * splicing all the values from each of the arguments into a new list
         */  
        pLabel = gHyp_data_getLabel ( pData ) ;
        pList = gHyp_data_new ( "_json_" ) ;
        isVector = ( gHyp_data_getDataType ( pArgs ) > TYPE_STRING ) ;
        pArg = NULL ;    
        ss = -1 ; context = -1 ;
        while ( (pArg = gHyp_data_nextValue ( pArgs, pArg, &context, ss )) ) {
          pValue = gHyp_data_copy ( pData ) ;
          gHyp_data_setVariable ( pValue, pLabel, TYPE_LIST ) ;
          /*
          pValue2 = gHyp_data_copyAll ( pArg ) ;
          gHyp_data_append ( pValue, pValue2 ) ;
          */
          gHyp_data_copyValues ( pValue, pArg ) ;
          gHyp_data_append ( pList, pValue ) ;
        }
        gHyp_stack_push ( pStack, pList ) ;
        gHyp_data_deleteValues ( pArgs ) ;
        gHyp_data_delete ( pArgs ) ;
      }
    }
    else {
      /* Empty array [] */
      if ( !gHyp_parse_isIndexCall ( pParse ) ) {
        /* JSON subscripting - empty array [] */
        pList = gHyp_data_new ( "_json_" ) ;
        gHyp_stack_push ( pStack, pList ) ;
      }
    }
  }
}

void gHyp_stmt_eSub ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the ']' character */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operator ( pParse, pCode, pAI ) ;

}

void gHyp_stmt_while ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :
      
        gHyp_frame_newStmt (	pFrame, 
				STATEMENT_LOOP,
				G_STMT_WHILE,
				G_WHILE_BEXP ) ;
				
	gHyp_frame_setLoopFlag ( pFrame, FRAME_LOOP_WHILE ) ;
	
        /* A new statement means a new parse structure */
        pParse = gHyp_frame_parse ( pFrame ) ;
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
        break ;	

      case G_STMT_WHILE :

        gHyp_frame_nextState (	pFrame, G_WHILE_BEXP ) ;
	gHyp_parse_reset ( pParse ) ;
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
        break ;	
      
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting token 'while'.",
		gHyp_hyp_token ( pCode ) ) ;
    }
  }
  else {

    /* Just completed executing the expression in "while ( expression )"
     * Evaluate the truth of the expression.
     * The expression will have left a boolean 1 or 0 on the stack.
     */
    sStack	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData 
      *pData ;
    
    sLOGICAL
      isTrue ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( argCount == 0 ) 
      gHyp_instance_error ( 	pAI, STATUS_EXPRESSION, 
				"Empty while () expression." ) ;
    
    /* Evaluate the expression. */
    pData = gHyp_stack_popRdata ( pStack, pAI ) ; 

    /* If more than one argument, discard arguments 1 through argCount-1 */
    for ( ; argCount > 1 ; argCount-- ) gHyp_stack_popRdata ( pStack, pAI ) ;
    
    isTrue =  gHyp_data_getBool ( pData, gHyp_data_getSubScript(pData), TRUE );

    /* If the 'while(expression)' was false, stop loop execution */
    if ( !isTrue ) 
      gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
    
    if ( !isTrue && gHyp_frame_branch ( pFrame ) )
    
      /* This loop has a branch defined. Take the fast route */
      gHyp_frame_endStmt ( pFrame, pAI ) ;
      
    else {
    
      /* Take the slow road */
      if ( gHyp_frame_testLoopFlag ( pFrame, FRAME_LOOP_WHILE ) )
    
        /* After 'while(exp)' comes a statement */
        gHyp_frame_setStates ( pFrame, G_STMT_WHILE, G_PROGRAM_STMT ) ;
      
      else {
    
        /* After 'do stmt while(expression)' comes a ';' */
        gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_EOS,
				G_STMT_EOS ) ;
      }  
    }
  }
}

void gHyp_stmt_do ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :

	gHyp_frame_newStmt (	pFrame, 
				STATEMENT_LOOP,
				G_STMT_DO,
				G_PROGRAM_STMT ) ;
	
	gHyp_frame_setLoopFlag ( pFrame, FRAME_LOOP_DO ) ;
	
	break ;
	
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting statement 'do'." ) ;
    }
  }
}

void gHyp_stmt_for ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE ) {

    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :

        gHyp_frame_newStmt (	pFrame, 
				STATEMENT_LOOP,
				G_STMT_FOR,
				G_FOR_BEXP ) ;
				
	gHyp_frame_setLoopFlag ( pFrame,	
				 FRAME_LOOP_FOR |
				 FRAME_LOOP_FOR_EXP1 |
				 FRAME_LOOP_FOR_EXP2 ) ;
	
        /* A new statement means a new parse structure */
        pParse = gHyp_frame_parse ( pFrame ) ;
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
        break ;	
      
      case G_STMT_FOR :

        gHyp_frame_nextState ( pFrame, G_FOR_BEXP ) ;
	gHyp_parse_reset ( pParse ) ;
        gHyp_parse_expression ( pParse, pCode, pAI ) ;
        break ;	
      
      default :	 

        gHyp_instance_error ( 
      		pAI, STATUS_PARSE, "Not expecting token 'for'.",
		gHyp_hyp_token ( pCode ) ) ;
    }
  }
  else {

    /* Just completed executing one or more of the expressions in 
     * a "for ( exp1 ; exp2 ; exp3 )" statement.
     * The expression will have left a boolean 1 or 0 on the stack.
     * If exp3 was executed, then do nothing because its value is irrelevent. 
     * If exp1 was executed, then so also was exp2 executed.  The value for
     * exp1 will be lower on the stack. Its value is also irrelevent.
     * If exp2 was evaluated, then its value determines whether the loop
     * statement is executed.
     * Always disable execution of expression 1 after it has been done the
     * first time.
     */
     
    /* Check state of for expression */
    if ( gHyp_frame_testLoopFlag ( pFrame, FRAME_LOOP_FOR_EXP3 ) ) {
    
      /* Done executing expression3. */
      gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_EOS,
				G_STMT_EOS ) ;
      gHyp_frame_endStmt ( pFrame, pAI ) ;	

    }
    else {
    
      /* Conditional expression2 */
      
      sLOGICAL
	isTrue ;
      
      sData
	*pData ;
      
      sStack
	*pStack = gHyp_frame_stack ( pFrame ) ;
      
      int
	argCount = gHyp_parse_argCount ( pParse ) ;
    
      /* Disable expression 1 */
      gHyp_frame_clearLoopFlag ( pFrame, FRAME_LOOP_FOR_EXP1 ) ;
      
      if ( argCount == 0 ) 

        isTrue = TRUE ;

      else {
      
	/* Evaluate the expression. */
        pData = gHyp_stack_popRdata ( pStack, pAI ) ;    
        isTrue = gHyp_data_getBool ( pData,gHyp_data_getSubScript(pData),TRUE);

	/* Flush any thing left on the stack */
	gHyp_stack_flush ( pStack ) ;

	/* If more than one argument, discard arguments 1 through argCount-1 */
	/*for (;argCount > 1;argCount--) gHyp_stack_popRdata ( pStack,pAI) ;*/
      }

      /* If the 'for(expr2)' was false, stop loop execution */
      if ( !isTrue ) 
	gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
    
      if ( !isTrue && gHyp_frame_branch ( pFrame ) )
    
        /* This loop has a branch defined. Take the fast route */
        gHyp_frame_endStmt ( pFrame, pAI ) ;
      
      else
        /* Take the slow route. */
        gHyp_frame_setStates ( pFrame, G_STMT_FOR, G_PROGRAM_STMT ) ;
    }
  }
}

void gHyp_stmt_eos ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the ';' symbol. 
   *
   * The ';' is always expected at the end of a statement.
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE ) {
    
    /* Check expected state */
    switch ( gHyp_frame_expectedState ( pFrame ) ) {

      case G_PROGRAM_STMT :
      case G_IF_ELSE :
      case G_LIST_END :

  	/* Encountered the ';' as a statement all by itself */
        /* Start a new frame */
	gHyp_frame_newStmt (	pFrame,
				STATEMENT_PROGRAM,
				G_STMT_EXP,
				G_STMT_EOS ) ;
    
        /* A new statement means a new parse structures */
        pParse = gHyp_frame_parse ( pFrame ) ;
	
      case G_STMT_EXP :
      case G_STMT_EOS :

        /* Complete the expression */
        gHyp_parse_expression ( pParse, pCode, pAI ) ;

    	/* Pop the frame stack back through all completed statements*/
	gHyp_frame_endStmt ( pFrame, pAI ) ;   	
	break ; 
      
      case G_FOR_EXP1 :
        
	/* Complete the first expression in the 'for' statement */
        gHyp_parse_expression ( pParse, pCode, pAI ) ;

	/* Set the next state - the second expression. */
        gHyp_frame_nextState ( pFrame, G_FOR_EXP2 ) ;
	
	/* If execution was turned off for expression 1, turn it back on. */
	if ( gHyp_frame_isStmtFalse ( pFrame ) )
	  gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
 	
	/* Turn off execution of expression 2 if the loop was just restored.
	 * (expression 3 is always executed after the loop is restored).
	 */
	if ( gHyp_frame_isStmtTrue ( pFrame ) && 
	     gHyp_frame_testLoopFlag ( pFrame, FRAME_LOOP_FOR_EXP3 ) ) 
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	
	break ;
	
      case G_FOR_EXP2 :
	
	/* Completed the second expression in the 'for' statement */
        gHyp_parse_expression ( pParse, pCode, pAI ) ;

	/* Set the next state - the third expression. */
        gHyp_frame_nextState ( pFrame, G_FOR_EXP3 ) ;
	
	/* If execution was turned off for expression 2, turn it back on. */
	if ( gHyp_frame_isStmtFalse ( pFrame ) )
	  gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	  	
	/* Turn off execute expression 3 if the frame was not just restored. */
	if ( gHyp_frame_isStmtTrue ( pFrame ) && 
	     !gHyp_frame_testLoopFlag ( pFrame, FRAME_LOOP_FOR_EXP3 ) ) 
	  gHyp_frame_clearGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
	
	break ;
	
      default :

        gHyp_instance_error ( pAI, STATUS_PARSE, "Not expecting ';'." ) ;
    }
  }
}

void gHyp_stmt_break ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the 'break' statement */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_statement (	pParse, 
				pCode, 
				pAI, 
				G_STMT_BREAK, 
				G_STMT_EOS ) ;

  else {
  
    /* Break out of the current "loop".  
     *	
     * This means that we need to know where the end of the loop is.
     * Which means we still have to parse forwards, without executing.
     * Therefore, just set all the frames to false down to the loop
     * frame, thereby turning off execution until we get to the loop end.
     */
    gHyp_frame_break ( pFrame ) ;	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

  }
}

void gHyp_stmt_return ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the 'return' statement */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE ) {

    gHyp_parse_statement (	pParse, 
				pCode, 
				pAI, 
				G_STMT_RETURN, 
				G_STMT_EXP ) ;
  }
  else {

    /* Executing the return statement.
     * Check to see if the stack contains a value, which should be
     * assigned to the method variable name or the STATUS variable.
     */
    
    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult,
      *pMethodVariable,
      *pLvalue,
      *pStackData ;

    char
      *pMethodStr ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    if ( argCount > 0 ) {

      /* Get argument */
      pStackData = gHyp_stack_popRdata ( pStack, pAI ) ;
    
      /* If more than one argument, discard arguments 1 through argCount-1 */
      for ( ; argCount > 1 ; argCount-- ) gHyp_stack_popRdata ( pStack, pAI ) ;

      /* Assign the method variable the return value */
      pMethodVariable = gHyp_frame_getMethodVariable ( pFrame ) ;
      pMethodStr = gHyp_data_getLabel ( pMethodVariable ) ;
      if ( strcmp ( pMethodStr, "_main_" ) == 0 ) {
	pMethodVariable = gHyp_frame_findRootVariable ( pFrame, "STATUS" ) ;
        pMethodStr = gHyp_data_getLabel ( pMethodVariable ) ;
      }

      if ( pMethodVariable != gHyp_data_getVariable ( pStackData ) ) {

	pLvalue = gHyp_data_new ( NULL ) ;
	gHyp_data_setReference ( pLvalue, 
			       pMethodStr,
			       pMethodVariable ) ;

	pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   pStackData,
				   TYPE_NULL,
				   FALSE, FALSE  ) ;
	gHyp_data_delete ( pResult ) ;
	gHyp_data_delete ( pLvalue ) ;
      }

      if ( guDebugFlags & DEBUG_DIAGNOSTICS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS, 	
			     "diag : return %s",
			     gHyp_data_print ( pMethodVariable ) ) ;
    }
    
    /* Return from the current method.
     * This means that we need to know where the call originated.
     * Which means we just have to unwind the frame stack.
     */    
    gHyp_frame_return ( pFrame ) ;
  }
}

void gHyp_stmt_continue ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the 'continue' statement */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_statement (	pParse, 
				pCode, 
				pAI, 
				G_STMT_CONTINUE, 
				G_STMT_EOS ) ;

  else {

    /* Restart the current 'loop'
     * This means that we need to know where the call originated.
     * Which means we just have to unwind the frame stack.
     */
    gHyp_frame_continue ( pFrame ) ;
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;


  }
}

void gHyp_stmt_when ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: when ( condition ) stmt ;
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
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    sData
      *pData ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: when ( condition " ) ;
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

    gHyp_instance_warning ( pAI,STATUS_UNDEFINED,
	"Keyword 'when' is reserved, currently not implemented. " ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack  ) ;
  }
}
