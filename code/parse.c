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
 *   $Log: parse.c,v $
 *   Revision 1.13  2013-05-15 16:44:08  bergsma
 *   Fix JSON parsing access violation error.
 *
 *   Revision 1.12  2011-01-08 21:43:47  bergsma
 *   JSON handling.
 *   Operator dot function handling.
 *
 *   Revision 1.11  2010-01-08 02:44:57  bergsma
 *   Added ssl_md5(), enhanced ssl_digest.
 *   Fixed urldecode, missing ":"
 *   Enabled object calls, ie:  text.strtok( ) and the like...
 *
 *   Revision 1.10  2004-11-19 03:48:04  bergsma
 *   HS 3.4.0.   Added list indexing with {}.
 *
 *   Revision 1.9  2003/02/06 00:09:22  bergsma
 *   Modified condition for gHyp_parse_isMethodArgs, expDepth must be > 0
 *
 *   Revision 1.8  2003/02/05 23:55:44  bergsma
 *   Added gHyp_parse_isMethodArgs()
 *
 *   Revision 1.7  2003/01/12 06:55:29  bergsma
 *   V3.1.0
 *   1. expDepth now tracks depth of "(" in all expressions, not just for method
 *   definition arguments.
 *   2. isMethodArgs flags added, set TRUE when method arguments are being
 *   defined as in M( methodArgs ) {...}.  Only in method definitions does the
 *   form "<typdef> <variable>" (eg: "list a") become a variable definition
 *   rather than just a <typdef> conversion of an existing <variable>
 *   3. added PARSE_SUBVARIABLE to pParse->flag, and pParse->flag now is
 *   a short rather than just a byte.  The PARSE_SUBVARIABLE bit is set
 *   whenever the expression token is an IDENTIFIER that follows a "." in the
 *   expression stack.  When the expression token is removed from the end
 *   of the expression and executed, a reference to the IDENTIFIER will not
 *   be created if a variable of the same name happens to exist.
 *   4. If a built-in function is used without its () in an expression, then
 *   it will cause an expression error unless the rank of the built-in function
 *   is changed to OPERAND rather than its default UNARY setting.
 *   5. Only allow an expression such as "list a" to define (or re-typecast)
 *   the variable a when the typecast token is:
 *   a) the first token in the expression (or another typcast token)
 *   b) within an expression () where the methodArgs flags is set
 *   c) is within an expression () or [] where the input token is an "="
 *   d) is within a list {}
 *
 *   Revision 1.6  2002/11/14 01:40:45  bergsma
 *   Removed ^M occurances and added Modification section.
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

struct parse_t
{
  int		*expression ;			/* Infix expression */
  sBYTE		*exprFlag ;                     /* Expression argument flag */
  char		exprView[MAX_TRACE_LENGTH+1] ;	/* Visual representation */
  int		exprCount ;			/* Expression count */
  int		maxExprCount ;			/* Water mark */
  int		prevExprRank ;			/* Previous expression rank */
  int		exprRank ;			/* Expression rank */
  int		argCount ;			/* Argument counter */
  int		listDepth ;			/* Depth of {} */
  int		expDepth ;			/* Depth of () */
  sLOGICAL	isMethodArgs ;			/* Definition of method args*/
  int		shortCircuit ;			/* For || and && */
  short         flag ;				/* Expression token flag */
  sHyp*		pMethodHyp ;
  int       	methodIndex ;
  sBYTE		inputPrecedence ;
  char*		inputToken ;
  sBYTE		inputTokenType ;
  int		inputIndex ;
  sBYTE		exprTokenType ;
  sBYTE         saveCurrentState ;
  sCode*	saveInputCode;
  sBYTE		saveInputPrecedence ;
  sBYTE		saveExprTokenType ;
  sLOGICAL	saveMatchedLEFT ;
} ;

/**********************	FUNCTION DEFINITIONS ********************************/

sParse *gHyp_parse_new ( )
{
  sParse * pParse = (sParse*) AllocMemory ( sizeof ( sParse ) ) ;
  assert ( pParse ) ;
  pParse->expression = (int *) AllocMemory ( sizeof ( int ) * giMaxExprSize ) ;
  pParse->exprFlag = (sBYTE *) AllocMemory ( sizeof ( sBYTE ) * giMaxExprSize ) ;
  gHyp_parse_reset ( pParse ) ;
  pParse->saveInputCode = NULL ;
  return pParse ;
}

void gHyp_parse_delete ( sParse * pParse )
{
  ReleaseMemory ( pParse->exprFlag ) ;
  ReleaseMemory ( pParse->expression ) ;
  ReleaseMemory ( pParse ) ;
}

void gHyp_parse_reset ( sParse* pParse )
{  
  pParse->exprCount = 0 ;
  pParse->maxExprCount = 0 ;
  pParse->exprView[0] = '\0' ;
  pParse->exprRank = 0 ;
  pParse->argCount = 0 ;
  pParse->exprFlag[0] = 0 ;
  pParse->listDepth = 0 ;
  pParse->expDepth = 0 ;
  pParse->isMethodArgs = FALSE ;
  pParse->shortCircuit = giMaxExprSize ;
  pParse->flag = 0 ;
  pParse->pMethodHyp = NULL ;
  pParse->methodIndex = -1 ;
  pParse->inputPrecedence = PRECEDENCE_EMPTY ;
  pParse->inputToken = NULL ;
  pParse->inputTokenType = TOKEN_NULL ;
  pParse->inputIndex = 0 ;
}

int gHyp_parse_exprCount ( sParse *pParse ) 
{
  return pParse->exprCount ;
}

int gHyp_parse_exprRank ( sParse *pParse )
{
  return pParse->exprRank ;
}

void gHyp_parse_restoreExprRank ( sParse *pParse )
{
  pParse->exprRank = pParse->prevExprRank ;
}

int gHyp_parse_listDepth ( sParse *pParse ) 
{
  return pParse->listDepth ;
}

int gHyp_parse_expDepth ( sParse *pParse ) 
{
  return pParse->expDepth ;
}

int gHyp_parse_argCount ( sParse *pParse ) 
{
  /* Get counted arguments */
  int 
    argCount = pParse->argCount;

  sBYTE 
    dotArg = 0 ;

  /* If the expression token is itself an argument (eg: an object function), 
   * then add it as an argument.
   */ 
  if ( pParse->exprCount >= 0 ) {
    dotArg = ( pParse->exprFlag[pParse->exprCount] & EXPR_FUNCTION_CALL ) ; 
    /* Clear the flag */
    pParse->exprFlag[pParse->exprCount] = 0 ;
  }

  /* Add the implied extra arg from the .fn() call */
  if ( dotArg ) {
    if ( guDebugFlags & DEBUG_STACK )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL,
			   DEBUG_STACK, 
			   "argcf: +1" ) ;

    argCount++ ;   
  }

  /* Init the argument count for the next call. */
  pParse->argCount = 1 ;

  if ( guDebugFlags & DEBUG_STACK )
    gHyp_util_logDebug ( FRAME_DEPTH_NULL,
			 DEBUG_STACK, 
			 "argc?: %d",
			 argCount ) ;
  return argCount ;
}

sBYTE gHyp_parse_inputPrecedence ( sParse *pParse )
{
  return pParse->inputPrecedence ;
}

int gHyp_parse_inputIndex ( sParse *pParse )
{
  return pParse->inputIndex ;
}

char *gHyp_parse_inputToken ( sParse *pParse )
{
  return pParse->inputToken ;
}

sBYTE gHyp_parse_inputTokenType ( sParse *pParse )
{
  return pParse->inputTokenType ;
}

sBYTE gHyp_parse_exprTokenType ( sParse *pParse ) 
{
  return pParse->exprTokenType ;
}


sLOGICAL gHyp_parse_isSubVariable ( sParse *pParse ) 
{
  if ( pParse->flag & PARSE_SUBVARIABLE ) 
    return TRUE ;
  else
    return FALSE ;
}

sLOGICAL gHyp_parse_isMethodArgs ( sParse *pParse ) 
{
  return ( pParse->isMethodArgs && pParse->expDepth > 0 ) ;
}

sLOGICAL gHyp_parse_isFunctionCall ( sParse *pParse ) 
{
  return (sLOGICAL) (pParse->flag & PARSE_FUNCTION_CALL ) ;
}

sLOGICAL gHyp_parse_isMethodCall ( sParse *pParse ) 
{
  return (sLOGICAL) (pParse->flag & PARSE_METHOD_CALL) ;
}

sLOGICAL gHyp_parse_isIndexCall ( sParse *pParse ) 
{
  return (sLOGICAL) (pParse->flag & PARSE_INDEX_CALL) ;
}

sLOGICAL gHyp_parse_isListCall ( sParse *pParse ) 
{
  return (pParse->flag & PARSE_LIST_CALL) ? TRUE:FALSE ;
}

sLOGICAL gHyp_parse_isObjectCall ( sParse *pParse ) 
{
  return (pParse->flag & PARSE_OBJECT_CALL) ? TRUE:FALSE ;
}


sLOGICAL gHyp_parse_isPointerDeref ( sParse *pParse ) 
{
  return (sLOGICAL) (pParse->flag & PARSE_POINTER_DEREF) ;
}

sLOGICAL gHyp_parse_isMethodDef ( sParse *pParse ) 
{
  return (sLOGICAL) (pParse->flag & PARSE_METHOD_DEF) ;
}

sLOGICAL gHyp_parse_isMethodDeferred ( sParse *pParse ) 
{
  return (sLOGICAL) (pParse->flag & PARSE_METHOD_DEFERRED) ;
}

sLOGICAL gHyp_parse_isConditional ( sParse *pParse , sHyp *pHyp ) 
{
  sCode *pExprCode ; 
  sBYTE exprTokenType ;
  if ( pParse->exprCount < 1 ) return FALSE ;
  pExprCode = gHyp_hyp_code ( pHyp, pParse->expression[pParse->exprCount-1] ); 
  exprTokenType = gHyp_hyp_tokenType ( pExprCode ) ;
  return (sLOGICAL) (exprTokenType == TOKEN_CONDITION ) ;
}


sLOGICAL gHyp_parse_isMethodDeprecated ( sParse *pParse ) 
{
  return (sLOGICAL) (pParse->flag & PARSE_METHOD_DEPRECATED) ;
}

sCode *gHyp_parse_methodCode ( sParse *pParse )
{
  sCode 
    *pCode = gHyp_hyp_code ( pParse->pMethodHyp, pParse->methodIndex ) ;

  pParse->pMethodHyp = NULL ;
  pParse->methodIndex = -1 ;

  return pCode ;
}

sLOGICAL gHyp_parse_isVariableDef ( sParse *pParse ) 
{
  return (sLOGICAL) (pParse->flag & PARSE_VARIABLE_DEF ) ;
}

static char lHyp_parse_specialPrecedence ( 	sBYTE inputTokenType,
						sBYTE inputPrecedence )
{
  /*
   * Change the input precedence of certain tokens
   *
   * The input precedence is set high so that the token is always appended 
   * onto the end of the current expression.
   *
   * The expression precedence is then set lower (to normal) so that it will 
   * behave correctly in the expression evaluation.
   */
  switch ( inputTokenType ) {

  case TOKEN_BLIST:
  case TOKEN_POS:
  case TOKEN_NEG:
  case TOKEN_REF:
  case TOKEN_NOT:
  case TOKEN_COMPLEMENT:
  case TOKEN_PREINC:
  case TOKEN_PREDEC:
  case TOKEN_FUNCTION:
  case TOKEN_TYPECAST:
    inputPrecedence = PRECEDENCE_INPUT_UNARY ;       
    break ;
    
  case TOKEN_BSUB:	
  case TOKEN_BEXP:
    inputPrecedence = PRECEDENCE_INPUT_LEFT ;
    break ;
    
  case TOKEN_LIST:
    inputPrecedence = PRECEDENCE_INPUT_LIST ;
    break ;
    
  case TOKEN_ASSIGN:
    inputPrecedence = PRECEDENCE_INPUT_ASSIGN ;
    break ;

  case TOKEN_POINTER:
    inputPrecedence = PRECEDENCE_INPUT_PTR ;
    break ;

  case TOKEN_DEREF:
    inputPrecedence = PRECEDENCE_INPUT_DEREF ;
    break ;

  case TOKEN_LITERAL:
  case TOKEN_UNIDENTIFIED:
  case TOKEN_CONSTANT:
    inputPrecedence = PRECEDENCE_INPUT_OPERAND ;
    break ;
  }


  return inputPrecedence ;
}

static sLOGICAL lHyp_parse_match (	sInstance *pAI, 
					char currentState,
					sBYTE inputTokenType,
					sBYTE exprTokenType )
{
  /* Check 1.	For input tokens ) ] } and expression tokens  { [ (,
   * 		make sure they are paired correctly 
   */
  sLOGICAL	matchedLEFT = TRUE ;

  switch ( inputTokenType ) {

    case TOKEN_ELIST :	
	if ( exprTokenType != TOKEN_BLIST &&
	     currentState != G_STMT_METHODDEF ) {
	  matchedLEFT = FALSE ;
	  gHyp_instance_error ( 
		pAI, STATUS_PARSE, 
		"Not expecting '}' in expression. No matching '{'." ) ;
	}
	break ;
	
    case TOKEN_ESUB : 
	if ( exprTokenType != TOKEN_BSUB ) {
	  matchedLEFT = FALSE ;
	  gHyp_instance_error ( 
		pAI, STATUS_PARSE, 
		"Not expecting ']' in expression. No matching '['." ) ;
	}
	break ;

    case TOKEN_EEXP : 
	if ( exprTokenType != TOKEN_BEXP ) {
	  matchedLEFT = FALSE ;
	  gHyp_instance_error ( 
		pAI, STATUS_PARSE,
		"Not expecting ')' in expression. No matching '('." ) ;
	}
	break ;
  	
    default:
    
	/* Check 2.	For input tokens that are not ) ] } and
	 *		expression tokens that are { [ (,
	 * 		which of course can only be ";" because its
	 *		the only token with lower precedence.
	 */

	switch ( exprTokenType ) {

	  case TOKEN_BLIST :	
	        matchedLEFT = FALSE ;
		gHyp_instance_error ( 
			pAI, STATUS_EXPRESSION, "No matching '}' for '{'" ) ;
		break ;
	
	  case TOKEN_BSUB : 
	        matchedLEFT = FALSE ;
		gHyp_instance_error ( 
			pAI, STATUS_EXPRESSION, "No matching ']' for '['" ) ;
		break ;

	  case TOKEN_BEXP : 
	        matchedLEFT = FALSE ;
		gHyp_instance_error ( 
			pAI, STATUS_EXPRESSION, "No matching ')' for '('" ) ;
		break ;
        }
  }
  return matchedLEFT ;
}

static int lHyp_parse_exprRank (	unsigned long exprRank, 
					sBYTE rank,
					int argCount,
					sInstance *pAI ) 
{
  /* Rank is used to validite an expression.
   *
   *	Rank is implemented using a bit mask.  In general, an operand sets
   *	a bit and an operator clears a bit.  In a valid expression, the
   *	sequence of operands and operators must alternately set and clear
   *	the bit. In an invalid expression, such as two operands in sequence, 
   *	the second operand would invalidate the expression because the bit
   *	would already be set from the first operand.
   *	When the expression contains the (), {}, and [] token pairs, then
   *	each nested sub-expression uses the next bit in the rank mask.
   *	The rank mask has 32 bits, thus expression nesting is limited to 32
   *	levels.
   *	There are 5 types of tokens that influence the rank:
   *
   *	Token Type		Rank Condition	Rank Action
   *	---------------		------------	--------------
   *	OPERANDS		Clear		Set
   *	UNARY OPERATORS		Clear		none (Clear)
   *	UNARY MODIFIERS		Set		none (Set)
   *	BINARY OPERATORS	Set		Clear
   *	'(', '{', '['		none		Left Shift, then Clear
   *	')', '}', ']'		Set		Right Shift, then Set
   *
   */
  sLOGICAL validExp = FALSE ;
  
  /* If RANK goes out of spec, then the expression is invalid */
  if ( rank == RANK_OPERAND ) {
  
    /* An operand sets the rank bit.  It is allowed only if the rank
     * bit is clear, as would be after an unary or binary operator.
     */
    if ( !(exprRank & 1) ) {
      exprRank |= 1 ;
      validExp = TRUE ;
    }
    else {
      /*gHyp_util_debug("argcount is %d",argCount);*/
      exprRank |= 1 ;
      validExp = TRUE ;
    }
  }
  else if ( rank == RANK_UNARY ) {	
    
    /* A unary operator is allowed only if the rank bit is clear, as would be
     * after a binary operator 
     */
    if ( !(exprRank & 1) ) validExp = TRUE ;
  }
  else if ( rank == RANK_POSTUNARY ) {	
    
    /* A post unary operator sets the rank bit. It is allowed if the 
     * rank bit is set, such as after an operand.
     */
    if ( (exprRank & 1) ) validExp = TRUE ;
  }
  else if ( rank == RANK_OPEN ) {
  	
    /* A '(', '{', or '[' is allowed after a unary, binary, or operand. 
     * Thus, the initial state of the rank bit can be set or clear.
     * By left shifting the rank bit, the state of the bit is saved.
     * The new rank bit is clear, expecting a unary operator or an operand.
     */
    exprRank <<= 1 ;
    exprRank &= ~1 ;
    validExp = TRUE ;
  }
  else if ( rank == RANK_CLOSE ) {	

    /* A ')', '}', or ']'  is allowed after an operand, or when the argument
     * count is zero, as in (), {}, or [].
     * By right shifting the rank bit, the state of the bits prior to the
     * opening '(', '{', or '[' are restored.
     * The old rank bit is set to expect a binary operator.
     */
    if ( (argCount == 0) || (exprRank & 1) ) {
      exprRank >>= 1 ;
      exprRank |= 1 ;
      validExp = TRUE ;
    }
  }
  else if ( rank == RANK_BINARY ) {
  	
    /* A binary operator clears the rank bit. It is allowed if the rank bit
     * is set, such as after an operand, ')', '}', or ']'
     */
    if ( (exprRank & 1) ) {
      exprRank &= ~1 ;
      validExp = TRUE ;
    }
  }
  
  if ( !validExp ) {
    gHyp_instance_error ( pAI, STATUS_EXPRESSION,  "Invalid Expression" ) ;
  }
  return exprRank ;
} 

void gHyp_parse_completeExpression ( sParse *pParse,
				     sInstance *pAI,
				     sHyp *pHyp,
				     sFrame *pFrame )
{
  if ( pParse->saveInputCode != NULL ) {

	  /*gHyp_util_debug("Completing expression, current state %d, token %s ",
		  pParse->saveCurrentState,
		  gHyp_hyp_token ( pParse->saveInputCode ));
	   */

    gHyp_parse_loop(pParse, 
  		      pAI,
		      pHyp,
		      pFrame,
		      gHyp_frame_stack ( pFrame ),
		      pParse->saveCurrentState,
		      pParse->saveInputCode,
		      pParse->saveInputPrecedence,
		      gHyp_hyp_token ( pParse->saveInputCode ),
		      gHyp_hyp_tokenType ( pParse->saveInputCode ),
		      gHyp_hyp_index ( pParse->saveInputCode ),
		      pParse->saveExprTokenType,
		      pParse->saveMatchedLEFT,
		      FALSE ) ;

    if ( pParse->saveCurrentState == G_STMT_EXP &&
	 pParse->saveInputPrecedence == PRECEDENCE_EOS )
      gHyp_frame_endStmt ( pFrame, pAI ) ;
  }

}

void gHyp_parse_expression (	sParse 		*pParse,
				sCode		*pInputCode,
				sInstance 	*pAI )
{
  /* 
   * Evaluate all or part of an expression.
   *
   * When the expression has one or more tokens, the input token will
   * remove all tokens from the end of the expression which have greater 
   * precedence than the input token.  As each token is removed, it is
   * transfered to the RPN stack: operands are pushed on the stack while
   * all other tokens are executed (using the stack for operands and
   * arguments).  
   * 
   * The input token is always appended to the end of the expression after it 
   * has removed one or more input tokens, except when the expression is empty
   * and the input token's input precedence is below PRECEDENCE_EMPTY.  Those
   * tokens are the null token ';', and the '}', ']', ')' match tokens.
   * (An expression is empty if "pX->exprCount == 0").
   * 
   * The '{}', '[]', and '()' tokens form matched pairs. The left element
   * has has the highest possible input precedence and thus can never remove 
   * tokens from the expression.  Once the left element is added to the end of
   * the expression, its input precedence is changed to a precedence lower than
   * the correspond right element.  The right element has the lowest possible
   * precedence and always removes every token up to and including the matching
   * left element.  After the left element is removed by the matching right 
   * element, the right element is discarded. If the token preceeding a
   * matching '(' is a keyword, like 'if', 'while' or 'function', then it is
   * removed and executed as well.
   */
  
  /* Input variables */
  sFrame	
    *pFrame = gHyp_instance_frame ( pAI ) ;
  
  sStack
    *pStack = gHyp_frame_stack ( pFrame ) ;
  
  sHyp
    *pHyp = gHyp_frame_getHyp ( pFrame ) ;
  
  sBYTE
    inputPrecedence,
    inputPrec = gHyp_hyp_precedence ( pInputCode ),
    inputTokenType = gHyp_hyp_tokenType ( pInputCode ),
    inputRank = gHyp_hyp_rank ( pInputCode ) ;
  
  int
    inputIndex = gHyp_hyp_index ( pInputCode ) ;
  
  char
    *inputToken = gHyp_hyp_token ( pInputCode ) ;

  /* Expression variables */
  sCode
    *pExprCode = NULL ;
  
  sBYTE
    exprFlag=0,
    exprPrecedence = PRECEDENCE_EMPTY,
    exprTokenType = TOKEN_NULL ;

  char
    *exprToken = NULL ;
  
  int
    exprIndex = -1 ;

  /* General variables */
  int
    i ;
  
  sData
    *pData ;

  sLOGICAL
    matchedLEFT = FALSE,
    trueValue ;

  /* Set current parse object's token input, index, and type */
    
  pParse->inputIndex = gHyp_hyp_index ( pInputCode ) ;
  pParse->inputToken = gHyp_hyp_token ( pInputCode ) ;
  pParse->inputTokenType = gHyp_hyp_tokenType ( pInputCode ) ;

  if ( pParse->exprCount > 0 ) { 
    /* Get expression information for token on end of expression */
    exprIndex = (int) pParse->expression [ pParse->exprCount - 1 ] ; 
    pExprCode = gHyp_hyp_code ( pHyp, exprIndex ); 
    pParse->exprTokenType = exprTokenType = gHyp_hyp_tokenType ( pExprCode ) ;
    exprPrecedence = gHyp_hyp_precedence ( pExprCode ) ;
    exprToken = gHyp_hyp_token ( pExprCode ) ;
    exprFlag = pParse->exprFlag[ pParse->exprCount - 1 ] ;
  }

  /* Case 1: short circuit handling */
  if ( exprTokenType == TOKEN_CONDITION &&
       pParse->exprCount < pParse->shortCircuit &&
       gHyp_frame_isStmtTrue ( pFrame ) ) {
    
    /* ? is on end of expression and short cicuit is not already set */
    pData = gHyp_stack_peek ( pStack ) ;
    
    /* Determine if what's on the stack is true or false */   
    if ( pData )
      trueValue = gHyp_data_getBool ( pData,
				      gHyp_data_getSubScript(pData),
				      TRUE ) ;
    else
      trueValue = FALSE ;
    
    /* Turn off execution until the ":" is encountered */
    if ( !trueValue )	{
      pParse->shortCircuit = pParse->exprCount  ;
      if ( guDebugFlags & DEBUG_PARSE )
        gHyp_util_logDebug (	
			    FRAME_DEPTH_NULL, DEBUG_PARSE, 
			    "parse: set left short circuit '?:' at index: %u", 
			    pParse->shortCircuit ) ;
    }
  }

  /* Case 2: When encountering a "(", we increment the expression depth.  When encountering
   *         a ")", we decrement the expression depth.
   */
  if ( inputTokenType == TOKEN_BEXP ) { 
       
    pParse->expDepth++ ;
     
    if ( exprTokenType == TOKEN_UNIDENTIFIED && 
	 pParse->expDepth == 1 &&
         !gHyp_frame_findMethodVariable ( pFrame, exprToken, pAI ) ) {
      /* New method definition. */
      pParse->isMethodArgs = TRUE ;
    }
  
  }

  /* Case 3: Change input precedence as required. */
  inputPrecedence = lHyp_parse_specialPrecedence (	inputTokenType,
  							inputPrec ) ; 

  /* Case 3a: Change expression precedence as required */

  /* Case 4: The non-standard format "(function)" is converted to "function".
   *            Example:  (str) "123" ; --> str "123" ;
   */
  if ( inputTokenType == TOKEN_EEXP && pParse->exprCount > 1 ) {

    /* Input token is ')' and the expression has at least two elements. 
     * Check token at the end of the expression.
     */
    if ( exprTokenType == TOKEN_TYPECAST && 
	 exprIndex == inputIndex - 1 ) {
      
      /* Token at the end of the expression is a typecast function that
       * immediately precedes the ')'
       * Get the second last token in the expression.
       */
      exprIndex = (int) pParse->expression [ pParse->exprCount - 2 ] ; 
      pExprCode = gHyp_hyp_code ( pHyp, exprIndex );
      pParse->exprTokenType = exprTokenType = gHyp_hyp_tokenType ( pExprCode ) ;
 
     if ( exprTokenType == TOKEN_BEXP && exprIndex == inputIndex - 2 ) {

	/* The second last token in the expression is a ')' and
	 * it immediately precedes the typecast token.
	 * Remove the '(' and pretend we didn't even get the ')'.
	 */
	pParse->expression [ pParse->exprCount-2 ] = 
	  pParse->expression [ pParse->exprCount-1 ] ;
	pParse->exprFlag [ pParse->exprCount-2 ] = 
	  pParse->exprFlag [ pParse->exprCount-1 ] ;
	pParse->exprCount-- ;
	pParse->maxExprCount = pParse->exprCount ;
	if ( guDebugFlags & DEBUG_PARSE )
	  gHyp_util_logDebug (	FRAME_DEPTH_NULL, DEBUG_PARSE, 
				"parse: reducing '(%s)' to '%s'", 
				exprToken,
				exprToken ) ;
	return ;
      }
    } 
  }
  
  
  /* Case 5: Pointer manipulation */
  if ( inputTokenType == TOKEN_POINTER ) {
    
    /* Convert 'a' + '->' to '->a' + '->' and exec as '*a' + '.'.
     * otherwise '?' + '->' becomes '?->' + '->' and exec as *(?) + '.' 
     *
     * First duplicate the "->" by adding another "->" to the expression end.
     * The input token "->" will have PRECEDENCE_INPUT_PTR and the 
     * expression token will have PRECEDENCE_POINTER.
     * When "->" encounters "->" on the expression, it will execute the
     * function gHyp_operator_pointer.   
     * A special flag (PARSE_POINTER_DEREF) is set and this will make
     * gHyp_operator_pointer call gHyp_operator_dereference. 
     * The input "->" then goes on the end of the expression, with the
     * same precedence as ".".  When it is finally executed, it will also
     * execute gHyp_operator_pointer, but since the special flag will
     * be clear, gHyp_operator_pointer will just call gHyp_operator_dot.
     * 
     */
    if ( pParse->exprCount >= giMaxExprSize ) {
      gHyp_util_logError ( 	"Expression overflow at level %d",
				pParse->exprCount ) ;
      longjmp ( gsJmpStack[0], COND_FATAL ) ;
    }
    /* Insert operator into expression */
    if ( pParse->exprCount > 0 ) { 
      /* Get operator and precedence at end of expression */
      exprIndex = (int) pParse->expression [ pParse->exprCount - 1 ] ; 
      pExprCode = gHyp_hyp_code ( pHyp, exprIndex ) ;
      exprPrecedence = gHyp_hyp_precedence ( pExprCode ) ;
      exprFlag = pParse->exprFlag[ pParse->exprCount - 1 ] ;
    }
    else
      exprPrecedence = PRECEDENCE_EMPTY ;
    
    if ( exprPrecedence > inputPrecedence ) {
      /* An operand preceeds the "->" as expected.  
       * (What about '.' and '*' ?)
       * Converting 'a->' to '->a'.
       * Then we will add the '->' to the end to get '->a->'
       */
      pParse->expression [pParse->exprCount-1] = inputIndex ;
      pParse->exprFlag [pParse->exprCount-1] = 0 ;
      pParse->expression [pParse->exprCount++] = exprIndex ;
    }
    else {
      /* Just add "->" to end of expression */
      pParse->exprFlag [pParse->exprCount] = 0 ;
      pParse->expression [pParse->exprCount++] = inputIndex ;
    }
    if ( guDebugFlags & DEBUG_INFIX_EXPR ) {
      i = pParse->exprCount*7 ;
      if ( i + 7 < MAX_TRACE_LENGTH )
	sprintf ( pParse->exprView + i, "[%-5.5s]", inputToken ) ;
    }
    
    pParse->maxExprCount= pParse->exprCount ;
  } 
  

  /* Case 6: When encountering a new "{", the listdepth should be incremented
   *         to indicate that we are within a list structure at listdepth.
   */
  if ( inputTokenType == TOKEN_BLIST ) pParse->listDepth++ ;

  /* Case 7: RANK ADJUSTMENTS.
   * Always initialize the argument count to zero when encountering a
   * '(', '[', '{', or unary operator.
   */
  if ( inputPrec == PRECEDENCE_LEFT || 
       inputPrec == PRECEDENCE_UNARY  ||
       inputTokenType == TOKEN_FUNCTION ) {
    pParse->argCount = 0 ;
    if ( guDebugFlags & DEBUG_STACK )
      gHyp_util_logDebug (	FRAME_DEPTH_NULL, DEBUG_STACK, 
				"argc0: %d", pParse->argCount ) ;
  }

  /* Case 7b. Two operands in succession - unary unary */
  if ( exprTokenType == TOKEN_UNIDENTIFIED &&
       inputRank == RANK_OPERAND ) {
    /*gHyp_util_debug("Two operands in a row" ) ;*/
    gHyp_hyp_setPrecedence ( pExprCode, PRECEDENCE_UNARY ) ;
    gHyp_hyp_setRank ( pExprCode, RANK_UNARY ) ;
    pParse->exprRank &= ~1 ;
  }

  /* Case 7c. Function precedence.  
     * When used as operand.fn, fn is an operand.
     * When used as fn operand, fn in an unary. */
  if ( exprTokenType == TOKEN_FUNCTION ) {
    /* For consecutive tokens only, not from expression reduction */
    if ( exprIndex == (inputIndex-1) ) {
      /* Some binary operator follows */
      if ( inputRank == RANK_BINARY ) {
        /* gHyp_util_debug ( "Function without () in expression" ) ; */
        gHyp_hyp_setPrecedence ( pExprCode, PRECEDENCE_OPERAND ) ;
        gHyp_hyp_setRank ( pExprCode, RANK_OPERAND ) ;
        /* The operand sets the rank bit */
        pParse->exprRank |= 1 ;
      }
      else if ( inputRank == RANK_OPEN ) {
        /* Some open (), [], or {} */
        /* gHyp_util_debug ( "Function with () in expression" ) ; */
        gHyp_hyp_setPrecedence ( pExprCode, PRECEDENCE_UNARY ) ;
        gHyp_hyp_setRank ( pExprCode, RANK_UNARY ) ;
        /* The unary operator has cleared the rank bit */
        pParse->exprRank &= ~1 ;
      }
    }
    else {

      /* Could be a deduced expression */
      if ( exprFlag&EXPR_FUNCTION_CALL ) {

        /* A fn used with an object, ie: obj.fn(...) */

        if ( inputRank == RANK_BINARY ) {

          /* An input token (binary operator) that must have fn executed, 
           * so we must raise the precedence to ensure it.
           */

          /* gHyp_util_debug ( "Object function call" ) ; */
          gHyp_hyp_setPrecedence ( pExprCode, PRECEDENCE_OPERAND ) ;
          gHyp_hyp_setRank ( pExprCode, RANK_OPERAND ) ;
          /* The operand sets the rank bit */
          pParse->exprRank |= 1 ;
        }
      }
    }
  }

  /* Case 7d.  A dot function. */
  if ( exprTokenType == TOKEN_DOT && inputTokenType == TOKEN_FUNCTION ) {
    /*gHyp_util_debug("dot function" ) ;*/
    gHyp_hyp_setPrecedence ( pInputCode, PRECEDENCE_OPERAND ) ;
    inputRank = RANK_OPERAND ;
    gHyp_hyp_setRank ( pInputCode, inputRank ) ;
  }

  /* Case 8: Check the expression, keeping track of proper expression syntax.*/
  pParse->prevExprRank = pParse->exprRank ;
  pParse->exprRank = lHyp_parse_exprRank ( pParse->prevExprRank,
					   inputRank,
					   pParse->argCount,
					   pAI ) ;

  /* Case 9: The argument count will be at least 1 if an operand is encountered
   *         anywhere in the expression
   */
  if ( inputRank == RANK_OPERAND && inputTokenType != TOKEN_FUNCTION) {
    pParse->argCount = 1 ;
    if ( guDebugFlags & DEBUG_STACK )
      gHyp_util_logDebug (	FRAME_DEPTH_NULL, DEBUG_STACK, 
				"argc1: %d", pParse->argCount ) ;
  }

  /* Within the bowels of the following loop is section 10, where the parser executes
   * a token.  Afterwards, it is necessary to test for handler conditions and call
   * the parser recursively.  Such handlers include the method handler, dereference
   * handler, alarm handler.   Handlers, when they are done, longjmp back to the point
   * which they were invoked, and the loop below continues execution.  
   * However, if the handler calls the sleep(), query(), or idle() functions, then
   * they must longjmp back to the root depth, (usually 1) 
   * (to give other instances a chance to run, etc).
   * Thus, when the handler finally returns, it can no longer longjmp back into this 
   * loop, it must re-enter it instead.  In most cases, this is ok because the input
   * token - which was being parsed when the handler was invoked - had completed 
   * removing all higher priority tokens from the expression lifo.
   * When this is not the case, then we have a problem.  The new input token will
   * collide with what should have been removed by the previous input token.
   * So, in order to fix this problem, the necessary values and pointers are saved
   * just prior to when a handler could be invoked.  If the handler returns to the
   * orginal setjmp point, then the saved values and pointers are cleared.  If however,
   * the handler returns via the next input token, then the saved values will be
   * seen and the original loop condition "completed" before the new input token can
   * begin parsing the expression lifo.
   */

  pParse->saveInputCode = NULL;
  gHyp_parse_loop(    pParse, 
  		      pAI,
		      pHyp,
		      pFrame,
		      pStack,
		      gHyp_frame_currentState(pFrame),
		      pInputCode,
		      inputPrecedence,
		      inputToken,
		      inputTokenType,
		      inputIndex,
		      exprTokenType,
		      matchedLEFT,
		      TRUE ) ;
 }



void gHyp_parse_loop (	sParse *pParse,
		        sInstance *pAI,
			sHyp *pHyp,
			sFrame *pFrame,
			sStack *pStack,
			char currentState,
			sCode *pInputCode,
			sBYTE inputPrecedence,
			char *inputToken,
			sBYTE inputTokenType,
			int inputIndex,
			sBYTE exprTokenType,
			sLOGICAL matchedLEFT,
			sLOGICAL isNormal )

{
  sCode
    *pMethodCode,
    *pCode ;

  int
    i,
    cond,
    saveJmpRootLevel=giJmpRootLevel,
    depth ;

  sLabel
    *pLabel ;

  sBYTE
    exprFlag=0,
    tokenType ;

  sData
    *pData ;
  
  sLOGICAL
    trueValue ;

  int exprIndex = -1 ;
  sCode *pExprCode = NULL ;
  sBYTE exprPrecedence = PRECEDENCE_EMPTY ;
  char *exprToken = NULL ;

  /* Do the execution loop */
  while ( TRUE ) {
    
    if ( isNormal ) {

    /* GET NEXT TOKEN FROM THE END OF THE EXPRESSION. */
    if ( pParse->exprCount == 0 ) {
      
      /* Empty expression */
      exprPrecedence = PRECEDENCE_EMPTY ;
      pParse->exprTokenType = exprTokenType = TOKEN_NULL ;
      exprToken = NULL ;
    }
    else {
      
      /* Get next token from end of expression */
      exprIndex = pParse->expression [ pParse->exprCount - 1 ] ; 
      pExprCode = gHyp_hyp_code ( pHyp, exprIndex );
      exprPrecedence = gHyp_hyp_precedence ( pExprCode ) ; 
      pParse->exprTokenType = exprTokenType = gHyp_hyp_tokenType ( pExprCode ) ;
      exprToken = gHyp_hyp_token ( pExprCode ) ;
      exprFlag = pParse->exprFlag [ pParse->exprCount - 1 ] ; 
    }

    /* Check for label definitions. */
    if (	pParse->exprCount == 1 && 
		inputTokenType == TOKEN_EVAL &&
		exprTokenType == TOKEN_UNIDENTIFIED ) {
	
      /* Found "LABEL :". Define the label. */
      pData = gHyp_frame_createVariable ( pAI, pFrame, exprToken ) ;
      depth = gHyp_frame_depth( pFrame ) - 1 ;
      pLabel = gHyp_label_new() ;
      gHyp_label_setLabel ( pLabel, inputIndex, depth ) ;
      gHyp_data_setObject ( pData, 
			    (void*) pLabel, 
			    DATA_OBJECT_LABEL, 
			    (void (*)(void*)) gHyp_label_delete ) ;
        
      if ( guDebugFlags & DEBUG_PARSE )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
			     "parse: label '%s' at index: %d, depth %d", 
			     exprToken,
			     inputIndex,
			     depth ) ;
	
      gHyp_frame_setStates ( pFrame, G_STMT_LABEL, G_PROGRAM_STMT ) ;
      return ;
    }
    }

    /* EVALUATE PRECEDENCE */
    if ( guDebugFlags & DEBUG_INFIX_EXPR ) 
        gHyp_util_logDebug ( 	FRAME_DEPTH_NULL,  DEBUG_INFIX_EXPR,
				"expr : [Input '%s'(%d) > Expr '%s'(%d)] ?",
				inputToken, inputPrecedence, 
				exprToken, exprPrecedence ) ;
       
    if ( inputPrecedence > exprPrecedence && isNormal ) {

      /* Input token has higher precedence than end expression token
       * Add input token to end of expression. 
       */
       
      if ( pParse->exprCount >= giMaxExprSize ) {
	gHyp_util_logError ( 	"Expression overflow at level %d",
				pParse->exprCount ) ;
 	longjmp ( gsJmpStack[0], COND_FATAL ) ;
      }
      

      if ( guDebugFlags & DEBUG_INFIX_EXPR ) {

        i = pParse->exprCount*7 ;
	if ( i + 7 < MAX_TRACE_LENGTH )
          sprintf ( pParse->exprView + i, "[%-5.5s]", inputToken ) ;

        gHyp_util_logDebug ( 	FRAME_DEPTH_NULL,  DEBUG_INFIX_EXPR,
				"expr : %s",
				pParse->exprView ) ;
      }	

      if ( pParse->flag & PARSE_OBJECT_CALL && 
          inputTokenType == TOKEN_FUNCTION ) {
	pParse->exprFlag[pParse->exprCount] |= EXPR_FUNCTION_CALL ;
        exprFlag = pParse->exprFlag[pParse->exprCount] ;
        /* IMPORTANT TO RESET the ARG COUNT HERE back to zero */
        /*gHyp_util_debug("Resetting to 0");*/
        pParse->argCount = 0 ; 

      }
      else 
        exprFlag = pParse->exprFlag [ pParse->exprCount ] = 0 ;
        

      pParse->expression [ pParse->exprCount++ ] = inputIndex ;
      pParse->maxExprCount = pParse->exprCount ;
 
      /* Cancel or turn on the short circuit when the ":" meets the "?" */
      if ( inputTokenType == TOKEN_EVAL &&
	   exprTokenType == TOKEN_CONDITION ) {
	if ( pParse->shortCircuit == pParse->exprCount - 1 ) {
	  pParse->shortCircuit = giMaxExprSize ;
	  if ( guDebugFlags & DEBUG_PARSE )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
				 "parse: cancel left short circuit '?:'" ) ;
	}
	else if (  pParse->shortCircuit > pParse->exprCount &&
		   gHyp_frame_isStmtTrue ( pFrame ) ) {
	  pParse->shortCircuit = pParse->exprCount  ;
	  if ( guDebugFlags & DEBUG_PARSE )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
				 "parse: set right short circuit '?:' at %d ", 
				 pParse->shortCircuit ) ;
	}
      }
      
      /* Short circuit the rest of the expression (do not execute further),
       * when the input token is && and the value on the stack
       * is false, or when the input token is || and the value on the stack
       * is true.
       */	

      if ( pParse->exprCount < pParse->shortCircuit &&
	   gHyp_frame_isStmtTrue ( pFrame ) &&
	   ( inputTokenType == TOKEN_AND || inputTokenType == TOKEN_OR ) ) {
	   
	/* Determine if what's on the stack is true or false */
	if ( guDebugFlags & DEBUG_PARSE )
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
			       "parse: test for short circuit '&&' at %d ", 
			       pParse->shortCircuit ) ;
	pData = gHyp_stack_peek ( pStack ) ;
	if ( pData )
	  trueValue = gHyp_data_getBool ( pData,
					  gHyp_data_getSubScript(pData),
					  TRUE ) ;
	else
	  trueValue = FALSE ;

	if ( inputTokenType == TOKEN_AND && !trueValue ) {
	  /* Short circuit the expression after the && */
	  pParse->shortCircuit = pParse->exprCount - 1 ;
	  if ( guDebugFlags & DEBUG_PARSE )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
				 "parse: set short circuit '&&' at %d ", 
				 pParse->shortCircuit ) ;
	}
	else if ( inputTokenType == TOKEN_OR && trueValue ) {
	  /* Short circuit the expression after the || */
	  pParse->shortCircuit = pParse->exprCount - 1 ;
	  if ( guDebugFlags & DEBUG_PARSE )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
				 "parse: set short circuit '||' at %d ", 
				 pParse->shortCircuit ) ;

	}
      }	

      /* Exit the expression. */
      break ;
    }
    else {

      /* Input token has an equal or lower precedence than the ending 
       * expression token.
       * Remove the expression token and execute it.
       */

      if ( pParse->exprCount == 0 && isNormal ) {	

        /* There were no more tokens in the expression.  
	 * The input token MUST be either a ';' ')', '}' or a ']' (because only
	 * those token's precedences are lower than PRECEDENCE_EMPTY).
	 */
	
	if ( inputPrecedence == PRECEDENCE_RIGHT && !matchedLEFT ) 
	   /* A left token never matched a right token, its a parse error. */
           lHyp_parse_match ( pAI, currentState, inputTokenType, TOKEN_NULL ) ;

	/* The expression is complete. */
	break ;
      }	
      else {
	
	if ( isNormal ) {

        /* Remove the expression token and execute it.*/

	/* 1.	Reset the flag that indicates special properities of the
	 *	expression token that is about to be removed 
	 */ 
	pParse->flag = 0 ;

	/* 2.	If the input token is a ')', '}', or ']' and has already
 	 *	removed the matching '(', '{', or '[', then further
	 *	expression tokens can only be removed if they are 
	 *	keyword tokens, ie: if, for, do, while. 
	 */
	if (	inputPrecedence == PRECEDENCE_RIGHT &&
		matchedLEFT &&
		exprTokenType != TOKEN_KEYWORD ) break ;

	/* 3.	If the expression token is a ',', then increment the
	 *	argument count.
	 */
	if (	exprPrecedence == PRECEDENCE_LIST ) {
		pParse->argCount++;
		if ( guDebugFlags & DEBUG_STACK )
		  gHyp_util_logDebug (	FRAME_DEPTH_NULL, DEBUG_STACK, 
					"argc+: %d", pParse->argCount ) ;
   	}			 
	
	/* 4.	If the expression token is an operand, then it could
	 *	be a method call or a method definition providing
	 *	the input token is a '('.
	 *	IF SO, WE MUST SAVE THE POSITION OF THE METHOD OPERAND
	 *	SO THAT WE CAN DEFINE WHERE THE METHOD STARTS.
         *
         *      If the operand is preceeded by a ".", then it is a sub-variable.
	 */
	if (	exprTokenType  == TOKEN_UNIDENTIFIED ) {

		if ( inputTokenType == TOKEN_BEXP ) {
		  pParse->pMethodHyp = pHyp ;
		  pParse->methodIndex = exprIndex ;
		}

		if ( pParse->exprCount > 1 ) {

		  pCode = gHyp_hyp_code ( 
		      pHyp, 
		      pParse->expression[pParse->exprCount-2] ) ;
		  tokenType = gHyp_hyp_tokenType ( pCode ) ;
		  if ( tokenType == TOKEN_DOT ) 
		    pParse->flag |= PARSE_SUBVARIABLE ;

		}
	}

        /* 5.	If the expression token is a '(', '[', or '{', then the input 
	 * 	token has to be a matching ')', ']', or '}', otherwise its
	 * 	a parse error.
	 *
	 *	Also determine whether the expression has an operand to act
	 *	upon, example: a[0], m(), or is preceded by a unary operator
	 *	such as: int(a), *(a), or is an expression that stands
	 *	alone, such as in "a = {1,2,3}".
	 *
	 *	An operand would be indicated by the previous value of the rank
	 *	bit being 1.  In this case, the operand will be on the stack.
	 *	If the rank was 0, a unary operator will still be in the
	 *	expression.
	 */        
	if (	exprPrecedence == PRECEDENCE_LEFT ) {

	  if ( !(matchedLEFT = lHyp_parse_match ( pAI,
						  currentState,
						  inputTokenType, 
						  exprTokenType))) break;
	  if ( ((pParse->prevExprRank >>= 1) & 1) ) {

	    /* Must be an operand to act upon */

	    if ( exprTokenType == TOKEN_BEXP ) {

	      /* Got to be a method call "m()" */
	      pParse->flag |= PARSE_METHOD_CALL ;
	      if ( guDebugFlags & DEBUG_PARSE )
		gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
				     "parse: method call");
	      
	      /* Check for '.' */
	      if ( pParse->exprCount > 1 ) {
	        pCode = gHyp_hyp_code ( pHyp, 
					pParse->expression[pParse->exprCount-2] ) ;
	        if ( gHyp_hyp_tokenType ( pCode ) == TOKEN_DOT ) {
		  /* A method call in a sub-variable */
		  pParse->flag |= PARSE_METHOD_DEFERRED ;
		  if ( guDebugFlags & DEBUG_PARSE )
		    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
					 "parse: deferred method call");
		}
	      }
	      
	      /* Check to see if we are expecting a method definition, i.e. "m() {"
	       * We cannot do a token lookahead, so we cannot rely on the next token 
	       * being a "{".
	       * Here are the conditions:
	       * 
	       * 1. The method position has previously been saved.
	       * 2. The mode of execution is ON (or compile only) 
	       * 3. The method is not yet defined.  This is not so easy to figure out. 
	       * For example, does
	       *
	       *      list a = { int b() 
	       *
	       *    expect a "{" to define a new method 'a.b()' or did we really mean:
	       *
	       *      list a = { int ( b() )
	       *
	       *    where we clearly want the integer result of executing the method named 'b()'.
	       *
	       * We say the method needs to be defined, .i.e. expects a "{" when there is a
	       * typecast token.
	       */
	      if ( pParse->pMethodHyp != NULL ) {

		pMethodCode = gHyp_hyp_code ( pParse->pMethodHyp,
					      pParse->methodIndex ) ;
		
		/*	
		if ( gHyp_frame_isStmtTrue ( pFrame ) || 
		     (guRunFlags & RUN_COMPILEONLY) ) {	     
		
		*/
		  if ( pParse->exprCount > 1 ) {

		    pCode = gHyp_hyp_code ( pHyp, 
		      pParse->expression[pParse->exprCount-2] ) ;
		    tokenType = gHyp_hyp_tokenType ( pCode ) ;

		    if ( tokenType == TOKEN_TYPECAST ) {

		      /* Its a method definition.  The next token expected
		       * will be a "{", as in M() { ... }
		       */
		      gHyp_frame_nextState ( pFrame, G_STMT_LIST ) ;

		      pParse->flag |= PARSE_METHOD_DEF ;

		      if ( guDebugFlags & DEBUG_PARSE )
		        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
			 		     "parse: method definition '%s'", 
					     gHyp_hyp_token ( pMethodCode ));
		    }
		  }
		  else {

		    /* May be a deprecated method definition */
		    if ( pParse->listDepth == 0 &&
			 gHyp_frame_isStmtTrue ( pFrame ) &&
		         !gHyp_frame_findMethodVariable ( pFrame, 
							  gHyp_hyp_token ( pMethodCode ), 
							  pAI ) )  {

		      /*
		      gHyp_util_logWarning (
			"Deprecated method definition, add ';' to end, format is: list %s () {...} ;",
			gHyp_hyp_token ( pMethodCode ) ) ;
		      */
		      gHyp_frame_nextState ( pFrame, G_STMT_LIST ) ;

		      pParse->flag |= PARSE_METHOD_DEF ;
		      pParse->flag |= PARSE_METHOD_DEPRECATED ;

		      if ( guDebugFlags & DEBUG_PARSE )
		        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
			 		     "parse: method definition '%s'", 
					     gHyp_hyp_token ( pMethodCode ));
		    }
		  }
		  /*}*/
	      }
	    }
	    else if ( exprTokenType == TOKEN_BSUB ) {

	      /* Got to be an index "call", ie:  a[i] */
	      pParse->flag |= PARSE_INDEX_CALL ;
	      if ( guDebugFlags & DEBUG_PARSE )
		 gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
				      "parse: subscript []");
	    }
	    else if ( exprTokenType == TOKEN_BLIST ) {

	      /* Got to be an list "call", ie:  a{i} */
	      pParse->flag |= PARSE_LIST_CALL ;
	      if ( guDebugFlags & DEBUG_PARSE )
		 gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
				      "parse: list subscript {}");
	    }
	  }
	  else if ( pParse->exprCount > 1 ) {

	    /* A unary operand precedes the operand.
	     * Get the tokenType of the unary operand.
	     */ 
	    pCode = gHyp_hyp_code ( pHyp, 
				    pParse->expression[pParse->exprCount-2] ) ;
	    tokenType = gHyp_hyp_tokenType ( pCode ) ;

	    if ( exprTokenType == TOKEN_BEXP &&
		 (tokenType==TOKEN_TYPECAST || tokenType==TOKEN_FUNCTION )) {

	      /* Its a built-in function call */
	      pParse->flag |= PARSE_FUNCTION_CALL ;
	    }
	  }
	}

	/* 6.	If the expression token is a typecast function, and it is the
	 *	first (or series of typecast) tokens in an expression or 
	 *      immediately following a ",", "(", "[", or "{", 
	 *	then flag it as being capable of either
	 *	defining a new variable or modifying an existing variable.
	 */
	if (	exprTokenType == TOKEN_TYPECAST ) {

	  /* Starting at pParse->exprCount-2, look backwards for the first typecast token
	   * in a series of one or more typecast tokens, then pick the token just
	   * before it.
	   */
	  i = pParse->exprCount - 2 ;
	  tokenType = TOKEN_TYPECAST ;
	  while ( i >= 0 ) {
	    pCode = gHyp_hyp_code ( pHyp, pParse->expression[i] ) ;
	    tokenType = gHyp_hyp_tokenType ( pCode ) ;
	    if ( tokenType != TOKEN_TYPECAST ) break ;
	    i-- ;
	  }

	  /* Now span across all occurences of "," */
	  while ( tokenType == TOKEN_LIST && i >= 0 ) {
	    pCode = gHyp_hyp_code ( pHyp, pParse->expression[i] ) ;
	    tokenType = gHyp_hyp_tokenType ( pCode ) ;
	    if ( tokenType != TOKEN_LIST ) break ;
	    i-- ;
	  }

	  /* We now want to look at just what we want to typecast.
	   * First, there must be expression token(s) that were taken off before the
	   * current typecast token, which is indicated by the high water mark (maxExprCount).
	   *
	   * Secondly, a new variable definition is allowed if the typecast token:
	   *
	   * 1. is the first token in the expression (or another typcast token)
	   * 2. is within an expression () where the methodArgs flags is set
	   * 3. is within an expression () or [] where the input token is an "=" 
	   * 4. is within a list {}
	   *
	   */
	  if ( pParse->exprCount < pParse->maxExprCount &&
	       ( tokenType == TOKEN_TYPECAST ||
		 (tokenType == TOKEN_BEXP  && pParse->isMethodArgs ) ||
		 (tokenType == TOKEN_BEXP  && inputTokenType == TOKEN_ASSIGN) ||
		 (tokenType == TOKEN_BSUB  && inputTokenType == TOKEN_ASSIGN) ||
		 (tokenType == TOKEN_BLIST) 
	       ) 
	     ) {
	    
	    /* Get the first token past the end of the string
	     * of typecast tokens.
	     */
	    i = pParse->exprCount ;
	    tokenType = TOKEN_TYPECAST ;
	    while ( i < pParse->maxExprCount ) {
	      pCode = gHyp_hyp_code ( pHyp, pParse->expression[i] ) ;
	      tokenType = gHyp_hyp_tokenType ( pCode ) ;
	      if ( tokenType != TOKEN_TYPECAST ) break ;
	      i++ ;
	    }

	    /* Now check that the first high-water mark token in the current 
	     * expression list is either
	     *	- a dereference
	     *	- OR  an unidentified variable,
	     *	- OR  a "[".
	     *  - OR  a "."
	     *  - OR  In the case of m() {...}, the (){} has been snipped out and
	     *        assigned as a pHyp to the variable 'm', therfore the token referred
	     *        to by pParse->expression[i] above will be the input token type.
	     */
	    if ( tokenType == TOKEN_DEREF ||
		 tokenType == TOKEN_UNIDENTIFIED ||
		 tokenType == TOKEN_BSUB ||
		 tokenType == TOKEN_DOT ||
		 tokenType == inputTokenType ) {

	      pParse->flag |= PARSE_VARIABLE_DEF ;
	      if ( guDebugFlags & DEBUG_PARSE )
		gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
				     "parse: type declaration '%s'",
				     exprToken ) ;
	    }
	  }
	}
	
	/* 7.	If the expression token is the 'if', 'while', or 'for'
	 *	keywords, the next state of the parser must be changed 
	 *	to expect a program statement.
	 *      Also, for 'for' statements where execution was disabled for 
	 *	one or more of the expressions, turn execution back on. 
	 */
	if (	exprTokenType == TOKEN_KEYWORD && 
		inputTokenType == TOKEN_EEXP ) {
		
          gHyp_frame_nextState ( pFrame, G_PROGRAM_STMT ) ;
          
	  if ( gHyp_frame_isStmtFalse ( pFrame ) &&
	       strcmp ( exprToken, "for" ) == 0  )
	    gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_TRUE ) ;
        }
	
	/* 8.	If the input token is a "->", then the expression token
	 *	will also be a "->" (see above).  If so, set a flag
	 *	thar will be detected in gHyp_operator_pointer.
	 */
	if (	inputTokenType == TOKEN_POINTER )
		pParse->flag |= PARSE_POINTER_DEREF ;

	/* 9.	If the input token is a function and the expression token
	 *	is a "dot", then set a flag
	 *	that will be detected when the function is executed.
	 */
	if (	inputTokenType == TOKEN_FUNCTION &&
		exprTokenType == TOKEN_DOT ) {
		if ( pParse->exprCount > 0 )  {
          /* Remember this flag at begining of the next iteration of the
           * loop where the function is put on the end of the expression.
           */
	  pParse->flag |= PARSE_OBJECT_CALL ;
          gHyp_hyp_setPrecedence ( pInputCode, PRECEDENCE_OPERAND ) ;
          gHyp_hyp_setRank ( pInputCode, RANK_OPERAND ) ;
		}
	}

        /* 10.  If debug is set, then print out the expression */
	if ( guDebugFlags & DEBUG_INFIX_EXPR ) {

	  if ( pParse->exprCount < pParse->maxExprCount ) {
	    /* Remove the previous null character. */
	    i = pParse->exprCount*7 ;
	    if ( i < MAX_TRACE_LENGTH ) pParse->exprView[i] = '[' ;
	  }

	  /* Put a null character to divide the unspent expression tokens
	   * from the spent ones.
	   */
          i = (pParse->exprCount-1)*7 ;
	  if ( i < MAX_TRACE_LENGTH ) pParse->exprView[i] = '\0' ;

	  if ( i > 0 )
            gHyp_util_logDebug ( 	FRAME_DEPTH_NULL, DEBUG_INFIX_EXPR,
					"expr : %s<%-5.5s][%s ",
					pParse->exprView,
					inputToken,
					pParse->exprView+i+1 ) ;
 	  else
            gHyp_util_logDebug ( 	FRAME_DEPTH_NULL, DEBUG_INFIX_EXPR,
					"expr : <%-5.5s][%s ",
					inputToken,
					pParse->exprView+1 ) ;
        }	


        /* 11.	If the statement frame is currently truthful, then execute
	 *	the expression token.
	 *	- If the token is a variable, constant, or literal 
	 *	  (an operand), its value will be pushed on the stack.
         * 	- If the token is an operator or a function, then it will 
	 *	  be executed, and will use the values already on the stack. 
         */	
	/*pParse->flag = pParse->exprFlag[pParse->exprCount];*/
        pParse->exprCount-- ;

        if ( gHyp_frame_isStmtTrue ( pFrame ) && 
	     pParse->exprCount < pParse->shortCircuit ) {
    
	  
	  gHyp_instance_setState ( pAI, STATE_EXECUTE ) ;

	  if ( giJmpLevel == MAX_JMP_LEVEL ) {
	    gHyp_util_logError ( "Execute jump level overflow at %d", MAX_JMP_LEVEL ) ;
            longjmp ( gsJmpStack[0], COND_FATAL ) ; 
	  }

	  giJmpLevel++ ;
          cond = gHyp_hyp_execute (	pAI,
					pExprCode ) ;
    	  giJmpLevel-- ;

 	  /* Save the necessary values */ 
	  pParse->saveCurrentState = currentState ;
	  pParse->saveInputCode = pInputCode ;
	  pParse->saveInputPrecedence = inputPrecedence ;
	  pParse->saveExprTokenType = exprTokenType ;
	  pParse->saveMatchedLEFT = matchedLEFT ; 

	  /*
	   *giJmpLevel++ ;
	   *giJmpRootLevel = giJmpLevel ;
           */
          cond = gHyp_parse_handleConditions( pAI ) ;
	  /*
	   *giJmpRootLevel = saveJmpRootLevel ; 
           *giJmpLevel-- ;
	   */
	 
        }

	} /* This ends the section 'if ( isNormal ) {' */

	isNormal = TRUE ;
	pParse->saveInputCode = NULL ;

	/* 12.	If the expression token was a ||, a &&, or a ":", then cancel
	 *	the short circuit marker that may have been previously set when
	 *	the || or && was the input token.
	 */
	if ( ( ( exprTokenType == TOKEN_AND || 
	  	 exprTokenType == TOKEN_OR ) &&
	       pParse->exprCount == pParse->shortCircuit ) ||
	     ( exprTokenType == TOKEN_EVAL &&
	       pParse->exprCount == pParse->shortCircuit - 1 ) ) {

	  pParse->shortCircuit = giMaxExprSize ;
	  
	  if ( guDebugFlags & DEBUG_PARSE )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PARSE, 
 				 "parse: cancel short circuit ||/&&" ) ;
	  if ( pParse->argCount > 1 ) pParse->argCount-- ;
	}

	/* 13.  If the expr token is a "{" then decrement the list depth 
	 */
	if ( exprTokenType == TOKEN_BLIST && pParse->listDepth > 0 ) 
	  pParse->listDepth-- ;

	/* 14.  If the expr token is a "(" then decrement the expression depth 
	 */
	if ( exprTokenType == TOKEN_BEXP && pParse->expDepth > 0 ) 
	  pParse->expDepth-- ;
      }
    }
  }
  
  return ;
}

int gHyp_parse_handleConditions ( sInstance * pAI )
{
  int cond ;


  /*
  int jmpVal ;
  if ( (jmpVal = setjmp ( gsJmpStack[giJmpLevel] )) ) return jmpVal ;
  */

  /* Look for exception conditions, executing any and all handlers. */
  do {
    gHyp_instance_setState ( pAI, STATE_EXECUTE ) ;
  }
  while ( (cond = gHyp_instance_parse ( pAI )) == COND_NORMAL ) ;
	  
  gHyp_instance_setState ( pAI, STATE_PARSE ) ;

  return cond ;
}

void gHyp_parse_statement (	sParse		*pParse, 
				sCode		*pCode,
				sInstance 	*pAI,
				char 		currentState, 
				char 		expectedState ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;

  /* Check expected state */
  switch ( gHyp_frame_expectedState ( pFrame ) ) {

    case G_PROGRAM_STMT :
    case G_IF_ELSE :
    case G_LIST_END :

      gHyp_frame_newStmt (	pFrame, 
				STATEMENT_PROGRAM,
				currentState,
				expectedState ) ;
				
      /* A new statement means a new parse structure */
      pParse = gHyp_frame_parse ( pFrame ) ;
      
      gHyp_parse_expression ( pParse, pCode, pAI ) ;
      
      break ;	

    default :	 

      gHyp_instance_error ( 
      	pAI, STATUS_PARSE, "Not expecting token '%s'.",
	gHyp_hyp_token ( pCode ) ) ;

  } 
}

void gHyp_parse_operand ( sParse *pParse, sCode *pCode, sInstance *pAI ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sBYTE		tokenType = gHyp_hyp_tokenType ( pCode ) ; 

  /* Check expected state */
  switch ( gHyp_frame_expectedState ( pFrame ) ) {

  case G_PROGRAM_STMT :
  case G_IF_ELSE :
  case G_LIST_END :

    /* A statement expression can start with a variable, string, constant,
     * or an identifier.  Whether the expression will be valid or not
     * remains to be determined.
     */
    gHyp_frame_newStmt (	pFrame, 
				STATEMENT_PROGRAM,
				G_STMT_EXP,
				G_STMT_EXP ) ;
    
    /* A new statement means a new parse structures */
    pParse = gHyp_frame_parse ( pFrame ) ;
    
  case G_STMT_EXP:
  case G_IF_EXP :
  case G_WHILE_EXP :
  case G_FOR_EXP1 :
  case G_FOR_EXP2 :
  case G_FOR_EXP3 :
    
    /* Set next state to the expected state */
    gHyp_frame_nextState ( pFrame, gHyp_frame_expectedState ( pFrame ) ) ;
     
    /* Continuing an expression */
    gHyp_parse_expression ( pParse, pCode, pAI ) ;
    break ;

  case G_GOTO_LABEL :
      
    /* Only an identifier is valid for labels. */
    if ( tokenType != TOKEN_UNIDENTIFIED ) 
      gHyp_instance_error ( pAI, STATUS_IDENTIFIER, 
			    "'%s' is an invalid label identifier", 
			    gHyp_hyp_token ( pCode ) ) ;
    
    gHyp_frame_nextState ( pFrame, G_STMT_EOS ) ;
    
    /* Add the identifier to the expression. */
    gHyp_parse_expression ( pParse, pCode, pAI ) ;
    break ;
	

  default :	 
    
    gHyp_instance_error ( pAI, STATUS_PARSE, "Not expecting operand '%s'.",
			  gHyp_hyp_token ( pCode ) ) ;
  } 
}

void gHyp_parse_operator ( sParse *pParse, sCode *pCode, sInstance *pAI ) 
{
  /* Check expected state */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;

  switch ( gHyp_frame_expectedState ( pFrame ) ) {

  case G_STMT_EXP:
  case G_IF_EXP:
  case G_WHILE_EXP:
  case G_FOR_EXP1:
  case G_FOR_EXP2:
  case G_FOR_EXP3:
    
    gHyp_frame_nextState ( pFrame, gHyp_frame_expectedState ( pFrame ) ) ;
    gHyp_parse_expression ( pParse, pCode, pAI ) ;
    break ;
    
  default :	 
    
    gHyp_instance_error ( pAI, STATUS_PARSE, "Not expecting operator '%s'.", 
			  gHyp_hyp_token ( pCode ) ) ;
  } 
}
