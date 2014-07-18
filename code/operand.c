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
 * $Log: operand.c,v $
 * Revision 1.7  2004/11/19 03:50:04  bergsma
 * New gpsTempData2 variable for memory leak detection on failed method calls.
 *
 * Revision 1.6  2004/10/16 04:49:30  bergsma
 * setStr2 to setStr_n
 *
 * Revision 1.5  2003/02/17 10:05:31  bergsma
 * When the operand token is a literal, it needs to be converted into
 * internal form before it is used.
 *
 * Revision 1.4  2003/01/12 06:59:08  bergsma
 * V3.1.0
 * Only put a reference to a variable on the stack if it is NOT a subvariable.
 *
 * Revision 1.3  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.2  2002/10/23 00:24:19  bergsma
 * Variable _parms_ will leak if popRdata2 fails, store reference in gpsTempData
 * for freeing later.
 *
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

	if ( pVariable &&
	     gHyp_data_getObjectType ( pVariable ) == DATA_OBJECT_METHOD ) {	    
	  gHyp_instance_setMethodCall ( pAI ) ;
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
