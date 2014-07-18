/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: parse.h,v $
 * Revision 1.5  2005-01-10 20:12:43  bergsma
 * Added function isListCall
 *
 * Revision 1.3  2003/01/12 05:13:00  bergsma
 * Added gHyp_parse_exprRank(), the depth of "(" in an expression.
 * Added gHyp_parse_isSubVariable(), whether an indentifier follows a "."
 *
 * Revision 1.2  2002/10/27 14:39:46  bergsma
 * Added gHyp_parse_loop and gHyp_parse_completeExpression
 *
 *
 */
extern sParse*		gHyp_parse_new () ;
extern void 		gHyp_parse_delete ( sParse * ) ;
extern void		gHyp_parse_reset ( sParse* ) ;
extern int	 	gHyp_parse_listDepth ( sParse * ) ;
extern int		gHyp_parse_exprRank ( sParse * ) ;
extern void		gHyp_parse_restoreExprRank ( sParse * ) ;
extern int	 	gHyp_parse_expDepth ( sParse * ) ;
extern int	 	gHyp_parse_exprCount ( sParse * ) ;
extern int	 	gHyp_parse_argCount ( sParse * ) ;
extern sCode*		gHyp_parse_methodCode ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isSubVariable ( sParse* ) ; 
extern sLOGICAL		gHyp_parse_isVariableDef ( sParse* ) ; 
extern sLOGICAL		gHyp_parse_isMethodArgs ( sParse* ) ; 
extern sLOGICAL		gHyp_parse_isFunctionCall ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isPointerDeref ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isMethodCall ( sParse * ) ;
extern sLOGICAL gHyp_parse_isListCall ( sParse *pParse ) ;
extern sLOGICAL         gHyp_parse_isMethodDef ( sParse *pParse ) ;
extern sLOGICAL		gHyp_parse_isMethodDeferred ( sParse *pParse ) ;
extern sLOGICAL		gHyp_parse_isMethodDeprecated ( sParse *pParse ) ;
extern sLOGICAL		gHyp_parse_isIndexCall ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isExprBegin ( sParse * ) ;
extern void gHyp_parse_completeExpression ( sParse *pParse,
				     sInstance *pAI,
				     sHyp *pHyp,
				     sFrame *pFrame ) ;
extern void 		gHyp_parse_expression ( sParse*, sCode*, sInstance* ) ;
extern void		gHyp_parse_loop (	sParse *pParse,
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
			  sLOGICAL isNormal ) ;
extern void 		gHyp_parse_statement (	sParse*, sCode*,
						sInstance*,char,char ) ;
extern void 		gHyp_parse_operand ( sParse *, sCode*, sInstance* ) ;
extern void 		gHyp_parse_operator ( sParse *, sCode*, sInstance* ) ;
