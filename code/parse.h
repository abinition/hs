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
extern sParse*		gHyp_parse_new () ;
extern void 		gHyp_parse_delete ( sParse * ) ;
extern void		gHyp_parse_reset ( sParse* ) ;
extern int	 	gHyp_parse_listDepth ( sParse * ) ;
extern int		gHyp_parse_exprRank ( sParse * ) ;
extern void		gHyp_parse_restoreExprRank ( sParse * ) ;
extern int	 	gHyp_parse_expDepth ( sParse * ) ;
extern int	 	gHyp_parse_exprCount ( sParse * ) ;
extern int	 	gHyp_parse_argCount ( sParse * ) ;
extern sBYTE		gHyp_parse_inputPrecedence ( sParse *pParse ) ;
extern int		gHyp_parse_inputIndex ( sParse *pParse ) ;
extern char*		gHyp_parse_inputToken ( sParse *pParse ) ;
extern sBYTE		gHyp_parse_inputTokenType ( sParse *pParse ) ;
extern sBYTE		gHyp_parse_exprTokenType ( sParse *pParse ) ;
extern sCode*		gHyp_parse_methodCode ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isSubVariable ( sParse* ) ; 
extern sLOGICAL		gHyp_parse_isVariableDef ( sParse* ) ; 
extern sLOGICAL		gHyp_parse_isMethodArgs ( sParse* ) ; 
extern sLOGICAL		gHyp_parse_isFunctionCall ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isPointerDeref ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isObjectCall ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isMethodCall ( sParse * ) ;
extern sLOGICAL gHyp_parse_isListCall ( sParse *pParse ) ;
extern sLOGICAL         gHyp_parse_isMethodDef ( sParse *pParse ) ;
extern sLOGICAL		gHyp_parse_isMethodDeferred ( sParse *pParse ) ;
extern sLOGICAL		gHyp_parse_isMethodDeprecated ( sParse *pParse ) ;
extern sLOGICAL		gHyp_parse_isIndexCall ( sParse * ) ;
extern sLOGICAL		gHyp_parse_isConditional ( sParse *, sHyp* ) ;
extern int gHyp_parse_handleConditions ( sInstance * pAI ) ;
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
