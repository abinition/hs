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

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

void gHyp_operand_token ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sData 	
      *pArgs=NULL,
      *pArg,
      *pResult=NULL,
      *pVariable=NULL;
    
    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sBYTE
      tokenType ;

    char
      variable[TOKEN_SIZE+1],
      stream[MAX_INPUT_LENGTH+1],
      *pToken = gHyp_hyp_token ( pCode ) ;

    int
      i,
      n,
      argCount ;

    tokenType = gHyp_hyp_tokenType ( pCode ) ;

    switch ( tokenType ) {
      
    case TOKEN_UNIDENTIFIED : 

      /* Input token was a un-identified (undefined) variable */ 
      pResult = gHyp_data_new ( NULL ) ;
      
      /* If variable exists, retrieve a reference to its value. */
      if ( !gHyp_parse_isSubVariable( pParse ) &&
	   (pVariable = gHyp_frame_findVariable ( pAI, pFrame, pToken )) ) {

	/* Make a reference to the variable and its values */  
	gHyp_data_setReference ( pResult, pToken, pVariable ) ;

      }
      else {
	/* Make a copy of the undentified variable. */   
	gHyp_data_setUnidentified ( pResult, pToken ) ;
      }

      /* Check if its a method call */
      if ( gHyp_hyp_precedence ( pCode ) == PRECEDENCE_UNARY ) {

	argCount = gHyp_parse_argCount ( pParse ) ;
      
	/* Fetch all method arguments - remove from the stack. 
	 * Use gHyp_stack_popRdata2 so that the arguments are not
	 * automatically deleted from the stack.
	 */
	pArgs = gHyp_data_new ( "_parms_" ) ;
	gpsTempData = pArgs ; 
	gpsTempData2 = pResult ; 
	/* Leak of pArgs occurs if popRdata2 fails - must fix with gpsTempData */
	for (; argCount > 0; argCount--) {
	  pArg = gHyp_stack_popRdata2 ( pStack, pAI ) ;
	  gHyp_data_insert ( pArgs, pArg ) ;
	}
	gpsTempData = NULL ;
	gpsTempData2 = NULL ;

	if ( pVariable ) {
	  if ( gHyp_data_getObjectType ( pVariable ) == DATA_OBJECT_METHOD )  
	    gHyp_instance_setMethodCall ( pAI ) ;
	}
	else {
	  gHyp_data_delete ( pResult ) ;
	  gHyp_data_delete ( pArgs ) ;
	  gHyp_instance_error ( pAI, STATUS_UNDEFINED, "No method exists for variable %s", pToken ) ;
	}
      }
      break ;

    case TOKEN_RAW :
      
      /* Input token was a vector of chars */
      sprintf ( variable, "_%s_", gHyp_util_random8() ) ;
      pResult = gHyp_data_new ( variable ) ;
      n = strlen ( pToken ) ;
      gHyp_data_newVector ( pResult, TYPE_RAW, n, FALSE ) ;
      for ( i=0; i<n; i++ ) gHyp_data_setVectorRaw ( pResult, pToken++, i ) ;
      break ;      
      
    case TOKEN_CONSTANT :
      
      /* Input token was a valid constant, see gHyp_util_getToken */
      pResult = gHyp_data_new ( NULL ) ;
      
      /* The tokenType will probably stay a constant, but run the constant
       * through data_setToken to get the internal representations 
       */  
      /*gHyp_util_logInfo("Doing settoken on %s",pToken);*/
      gHyp_data_setToken ( pResult, pToken ) ;
      break ;      
      
    case TOKEN_LITERAL :
      
      /* Input token was a valid literal, enclosed in double quotes */
      pResult = gHyp_data_new ( NULL ) ;
      strcpy ( stream, pToken ) ;
      n = gHyp_util_parseString ( stream ) ;
      gHyp_data_setStr_n ( pResult, stream, n ) ;
      break ;      
      
    default:
      gHyp_instance_error ( pAI, STATUS_EXPRESSION, 
			    "Invalid operand '%s' in expression.",	
			    gHyp_hyp_token ( pCode ) ) ;
    }
    
    /* Push the operand onto the stack */
    gHyp_stack_push ( pStack, pResult ) ;

    if ( pArgs ) gHyp_stack_push ( pStack, pArgs ) ;
	
  }
}
