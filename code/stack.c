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
 *   $Log: stack.c,v $
 *   Revision 1.4  2004/04/29 02:19:32  bergsma
 *   Added comment in gHyp_stack_popRdata.
 *
 *   Revision 1.3  2003/01/09 01:19:35  bergsma
 *   V3.1.0
 *   Changed error from popLvalue from STATUS_UNDEFINED to STATUS_IDENTIFIER
 *
 *   Revision 1.2  2002/11/14 01:40:45  bergsma
 *   Removed ^M occurances and added Modification section.
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

struct hstack_t
{
  int		depth ;				/* Current stack depth */
  sData*        *data ;				/* Execution Stack */
} ;

/**********************	FUNCTION DEFINITIONS ********************************/

sStack *gHyp_stack_new ( )
{
  int	i ;
  sStack 
    *pStack ;

  pStack = (sStack*) AllocMemory ( sizeof ( sStack ) ) ;
  assert ( pStack ) ;
  pStack->depth = 0 ;
  pStack->data = (sData **) AllocMemory ( sizeof ( sData* ) * giMaxStackDepth ) ;
  assert ( pStack->data ) ;
  for ( i=0; i<giMaxStackDepth; i++ ) {
    pStack->data[i] = NULL ;
  }
  return pStack ;
}

void gHyp_stack_flush ( sStack * pStack )
{
  int 	i ;
  for ( i=0; i<giMaxStackDepth; i++ ) 
    if ( pStack->data[i] != NULL ) {
      gHyp_data_delete ( pStack->data[i] ) ;
      pStack->data[i] = NULL ;
    }
  pStack->depth = 0 ;
  return ;  
}

void gHyp_stack_delete ( sStack * pStack )
{
  gHyp_stack_flush ( pStack ) ;
  ReleaseMemory ( pStack->data ) ;
  ReleaseMemory ( pStack ) ;
}

int gHyp_stack_depth ( sStack * pStack ) 
{
  return pStack->depth ;
}

void gHyp_stack_push ( sStack * pStack, sData *pData )
{
  /* Stack description:
   *
   * There can be up to giMaxStackDepth elements on the stack.
   *
   * An element is pushed onto the stack before the stack pointer is 
   * incremented to point to the next higher position.  Since the
   * stack depth is initialized to 0, the first stack element will go
   * at stack[0] and the stackDepth will be 1.  The stackDepth equals
   * the number of elements on the stack.
   *
   * An element is poped from the stack after the stack pointer is
   * decremented to point to the next lower position.  
   */
  sData	*pOldData ;
  if ( pStack->depth < giMaxStackDepth ) {

    pOldData = pStack->data[pStack->depth] ;
    if ( pOldData && ( pOldData != pData ) ) {
      gHyp_data_delete ( pOldData ) ;
    }
    pStack->data[pStack->depth++] = pData ;
    
    if ( guDebugFlags & DEBUG_STACK )
      gHyp_util_logDebug (	FRAME_DEPTH_NULL, DEBUG_STACK,
				"push : %s", 
      				gHyp_data_print ( pData ) ) ;
  }
  else {
    gHyp_data_delete ( pData ) ;
    gHyp_util_logError ( "Stack overflow error at level %d", pStack->depth ) ;
    longjmp ( gsJmpStack[0], COND_FATAL ) ;
  }
  
  return ;
}

sData * gHyp_stack_pop ( sStack * pStack )
{
  /* Stack description:
   *
   * There can be up to giMaxStackDepth elements on the stack.
   *
   * An element is poped from the stack after the stack pointer is
   * decremented to point to the next lower position.  Since the
   * stack depth is initialized to 0, there are initially no elements
   * on the stack to pop.
   *
   * An element is pushed onto the stack before the stack pointer is 
   * incremented to point to the next higher position.  
   */
  
  if ( pStack->depth == 0 )
    return NULL ;
  else {
  
    pStack->depth-- ;

    if ( guDebugFlags & DEBUG_STACK )
      gHyp_util_logDebug (
      	FRAME_DEPTH_NULL, DEBUG_STACK,
	"pop  : %s", 
      	gHyp_data_print ( pStack->data[pStack->depth] ) ) ;
  
    return pStack->data[pStack->depth] ;
  }
}

sData * gHyp_stack_pop2 ( sStack * pStack  )
{
  /* Identical to gHyp_stack_pop except that the pointer to the
   * sData object taken off the stack is cleared on the stack, making
   * the calling routine responsible for its disposal, rather than when
   * a new value is pushed on the stack 
   */
  sData
    *pData = gHyp_stack_pop ( pStack ) ;
  pStack->data[pStack->depth] = NULL ;
  return pData ;
}

sData* gHyp_stack_peek ( sStack *pStack )
{
  if ( pStack->depth == 0 ) 
    return NULL ;
  else {
    if ( guDebugFlags & DEBUG_STACK )
      gHyp_util_logDebug (
      	FRAME_DEPTH_NULL, DEBUG_STACK,
	"peek : %s", 
      	gHyp_data_print ( pStack->data[pStack->depth-1] ) ) ;

    return pStack->data[pStack->depth-1] ;
  }
}

sData * gHyp_stack_popRdata ( sStack * pStack, sInstance * pAI )
{
  /* A Rdata is needed when executing an expression.
   * It is called by gHyp_stack_popRvalue.  It is also called
   * by routines that do their own accessing of the contents of
   * the Rdata,which could be a CONSTANT, LITERAL, or VARIABLE/REFERENCE.
   * A Rdata cannot be an UNIDENTIFIED (undefined) variable.
   * A Rdata cannot be an undefined REFERENCE.
   */
  sData
    *pData = gHyp_stack_pop ( pStack ) ;

  sBYTE
    tokenType ;

  if ( pData == NULL ) {
    gHyp_util_logError ( "Stack was empty when rValue was expected" ) ;
    longjmp ( gsJmpStack[0], COND_FATAL ) ;
  }
  
  tokenType = gHyp_data_tokenType ( pData ) ;
  if ( tokenType == TOKEN_UNIDENTIFIED )
    gHyp_instance_error ( 
       pAI, STATUS_UNDEFINED, "'%s' is an undefined variable",
       gHyp_data_getLabel ( pData ) ) ;

  /*
   * Don't uncomment this as tempting as it may be.
   *
  if ( (tokenType == TOKEN_REFERENCE && !gHyp_data_getReference(pData)) )
    gHyp_instance_error ( 
       pAI, STATUS_UNDEFINED, "'%s' is an undefined reference",
       gHyp_data_getLabel ( pData ) ) ;
  */

  return pData ;
}

sData * gHyp_stack_popRdata2 ( sStack * pStack, sInstance * pAI )
{
  /* Identical to gHyp_stack_popRdata except that the pointer to the
   * sData object taken off the stack is cleared on the stack, making
   * the calling routine responsible for its disposal, rather than when
   * a new value is pushed on the stack 
   */
  sData
    *pData = gHyp_stack_popRdata ( pStack, pAI ) ;
  pStack->data[pStack->depth] = NULL ;
  return pData ;
}

sData * gHyp_stack_popRvalue ( sStack * pStack, sInstance * pAI )
{
  /* A Rvalue is an Rdata that must have at least one value.
   * An empty list would return an error when calling popRvalue.
   * A subscript out of range in a vector or list would also return an error.
   */
  sData
    *pValue,
    *pData = gHyp_stack_popRdata ( pStack, pAI ) ;
  
  int
    ss = gHyp_data_getSubScript ( pData ) ;

  if ( !(pValue = gHyp_data_getValue ( pData, ss, TRUE ) ) )
    gHyp_instance_error ( 
	pAI, STATUS_EXPRESSION, 
	"Identifier '%s' has no values or subscript '%d' is out of range.", 
	gHyp_data_getLabel ( pData ),
	ss  ) ;
     
  return pData ;
}

sData * gHyp_stack_popLvalue ( sStack * pStack, sInstance *pAI )
{
  /* A Lvalue is needed for:
   * 1. A label in a goto statement.
   * 2. A method name for enable() and disable() functions.
   * 3. A variable name for exists() and undef() functions.
   * 4. A method name when calling a method.
   * 5. Defining variables, eg: int i ;
   * 6. The left part of an assignment statement, eg: i = 1 ;
   * 7. A reference, eg: &foobar, &"foobar"
   * 8. A variable sub-structure, ie: a.b
   * 9. A subscripted variable, identifier, list, ie: a[0]
   *
   * The tokenType of the stack data determines whether or not it is
   * a Lvalue.  The tokenType must be one of:
   * 1. An unidentified variable: TOKEN_UNIDENTIFIED
   * 2. An identified variable: TOKEN_VARIABLE or TOKEN_REFERENCE
   * 4. A literal, which is treated like an unidentified variable.   
   */
  sBYTE	
    tokenType ;

  sData 
    *pData = gHyp_stack_pop ( pStack ),
    *pValue ;
    
  char
    *pLeft,
    *pRight,
    variable[TOKEN_SIZE+1] ;
    
  int
    ss ;

  if ( pData == NULL ) {
    gHyp_util_logError ( "Stack was empty when variable was expected" ) ;
    longjmp ( gsJmpStack[0], COND_FATAL ) ;
  }

  tokenType = gHyp_data_tokenType ( pData ) ;
  if ( tokenType != TOKEN_UNIDENTIFIED &&
       tokenType != TOKEN_KEYWORD &&
       tokenType != TOKEN_LITERAL &&
       tokenType != TOKEN_VARIABLE &&
       tokenType != TOKEN_REFERENCE )
    gHyp_instance_error ( 
      pAI,
      STATUS_IDENTIFIER,
      "'%s' is not a valid identifier, literal, or reference", 
      gHyp_data_getLabel ( pData ) ) ;

  /* If the lValue is subscripted, like a[i], and the subscript value
   * is a VARIABLE, then resolve a[i] to a.x, where x is the name of
   * the variable at a[i].
   */
  ss = gHyp_data_getSubScript ( pData ) ;
  if ( ss >= 0 &&
       tokenType == TOKEN_REFERENCE &&
       (pValue = gHyp_data_getValue ( pData, ss, TRUE )) != NULL && 
       gHyp_data_getReference ( pData ) != pValue &&
       gHyp_data_tokenType ( pValue ) == TOKEN_VARIABLE ) {
    pLeft = gHyp_data_getLabel ( pData ) ;
    pRight = gHyp_data_getLabel ( pValue ) ;
    if ( strlen ( pLeft ) + strlen ( pRight ) > TOKEN_SIZE ) 
     gHyp_instance_error ( 
        pAI,
        STATUS_IDENTIFIER,
	"New variable %s.%s exceeds maximum length TOKEN_SIZE=%d",
	pLeft, pRight, TOKEN_SIZE ) ;

    sprintf ( variable,
	      "%s.%s",
	      pLeft,
	      pRight ) ;
    gHyp_data_setReference ( pData, variable, pValue ) ;
    gHyp_data_setSubScript ( pData, -1 ) ;
    if ( guDebugFlags & DEBUG_STACK )
      gHyp_util_logDebug (
      	FRAME_DEPTH_NULL, DEBUG_STACK,
	"dot  : %s", 
      	gHyp_data_print ( pData ) ) ;
  }
  
  return pData ;
}
