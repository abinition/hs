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
 *   $Log: operator.c,v $
 *   Revision 1.9  2004/04/29 02:14:13  bergsma
 *   Fix 'divide by zero' error when using modulus '%'
 *
 *   Revision 1.8  2003/06/05 22:15:29  bergsma
 *   Make sure VMS knows about the pow() function.
 *
 *   Revision 1.7  2003/06/05 21:40:02  bergsma
 *   1. Added TOKEN_POW binary operator **
 *   2. Bug: pValue1 and pValue2 need to be set to NULL after warnings so that
 *   the loop in operator_binary will break out.
 *
 *   Revision 1.6  2003/05/16 04:53:44  bergsma
 *   Fix potential memory leak of pArgs in gHyp_operator_dot
 *
 *   Revision 1.5  2003/04/04 16:23:28  bergsma
 *   Increase the size of the dereference area from VALUE_SIZE to MAX_INPUT_LENGTH
 *
 *   Revision 1.4  2003/02/06 02:11:55  bergsma
 *   Test for isSubVariable must include isMethodArgs
 *
 *   Revision 1.3  2003/01/12 06:58:02  bergsma
 *   V3.1.0
 *   In _assign, isSubVariable only depends on the listDepth, not the exprDepth
 *
 *   Revision 1.2  2002/11/14 01:40:45  bergsma
 *   Removed ^M occurances and added Modification section.
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */
#include "math.h"

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

#if defined ( __cplusplus ) && defined ( AS_VMS )
extern "C" double pow ( double x, double y);
#else
extern double pow ( double x, double y);
#endif

#define BOTH_UNSIGNED 0 
#define SIGNED_UNSIGNED 1
#define UNSIGNED_SIGNED 2 
#define BOTH_SIGNED 3

static sData *lHyp_operator_binaryOp (	sInstance	*pAI,
					sFrame		*pFrame,
					char		oper,
					sData 		*pArg1,
					sData 		*pArg2 )
{
  sData		
    *pResult,
    *pTmp,
    *pNextValue1,
    *pNextValue2,
    *pValue1,
    *pValue2 ;
  
  char
    value1[VALUE_SIZE+1],
    value2[VALUE_SIZE+2],
    *pStr1 = value1,
    *pStr2 = value2,
    *pStr3,
    *pSearchStr,
    *pSubStr ;
  
  int
    ss1,
    ss2,
    int1,
    int2,
    len1,
    len2,
    context1,
    context2 ;
  
  unsigned long
    ulong1,
    ulong2 ;

  double
    double1,
    double2 ;
  
  sLOGICAL
    bool1,
    bool2,
    isEmpty1,
    isEmpty2,
    isVector1,
    isVector2 ;
  
  sBYTE
    dataType1,
    dataType2,
    tokenType1,
    tokenType2,
    sign,
    sign1,
    sign2 ;


  /* Initialize result */
  pResult = gHyp_data_new ( "_op2_" ) ;

  /* For all values of arg1 and arg2, going in parallel. */
  pValue1 = NULL ;
  pValue2 = NULL ;
  pNextValue1 = pArg1 ;
  pNextValue2 = pArg2 ;
  isVector1 = (gHyp_data_getDataType( pArg1 ) > TYPE_STRING ) ;
  isVector2 = (gHyp_data_getDataType( pArg2 ) > TYPE_STRING ) ;
  ss1 = gHyp_data_getSubScript ( pArg1 ) ; context1 = -1 ;
  ss2 = gHyp_data_getSubScript ( pArg2 ) ; context2 = -1 ;
  isEmpty1 = ( gHyp_data_getValue ( pArg1, ss1, TRUE ) == NULL ) ;
  isEmpty2 = ( gHyp_data_getValue ( pArg2, ss2, TRUE ) == NULL ) ;
  pTmp = NULL ;  
  while ( 1 ) {

    if ( pNextValue1 ) {
      /* This is the start of the scan, or the last value was defined,
       * Get the next value.
       */
      pNextValue1 = gHyp_data_nextValue ( pArg1, 
					  pValue1, 
					  &context1,
					  ss1 ) ;
      if ( context1 == -2 && ss1 != -1 ) {
	gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
	    "Subscript '%d' is out of bounds in first argument",ss1) ;
      }
    }
    
    if ( pNextValue2 ) {
      /* This is the start of the scan, or the last value was defined,
       * Get the next value.
       */
      pNextValue2 = gHyp_data_nextValue ( pArg2,
					  pValue2,
					  &context2,
					  ss2 ) ;
      if ( context2 == -2 && ss2 != -1 ) {
	gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
	       "Subscript '%d' is out of bounds in second argument",ss2);
      }
    }

    /* If both lists are exhausted of values, and we have ad least one result,
     * then we're done 
     */
    if ( !pNextValue1 && !pNextValue2 && pTmp ) break ;

    /* Use the next value for list 1 if it was found. */
    if ( pNextValue1 ) {

      pValue1 = pNextValue1 ;

      /* Get single value for arg1 */
      gHyp_data_getAll ( pValue1, 
			 &tokenType1, 
			 &dataType1, 
			 &int1,
			 &ulong1,
			 &double1, 
			 &bool1, 
			 value1,
			 &len1,
			 VALUE_SIZE,
			 context1,
			 isVector1 ) ;
    }

    /* Use the next value for list 2 if it was found. */
    if ( pNextValue2 ) {

      pValue2 = pNextValue2 ;

      /* Get single value for arg2 */
      gHyp_data_getAll ( pValue2, 
			 &tokenType2, 
			 &dataType2, 
			 &int2, 
			 &ulong2,
			 &double2, 
			 &bool2,
			 value2,
			 &len2,
			 VALUE_SIZE,
			 context2,
			 isVector2 ) ;
    }

    /* If either list is empty, then substitute default values */
    if ( isEmpty1 || isEmpty2 ) {

      if ( isEmpty1 && isEmpty2 ) {
	tokenType1 = TOKEN_LITERAL ;
	dataType1 = TYPE_STRING ;
	int1 = 0 ;
	ulong1 = 0 ;
	double1 = 0 ;
	bool1 = 0 ;
	strcpy ( value1, "" ) ;
	len1 = 0 ;
	context1 = -1 ;
	tokenType2 = TOKEN_LITERAL ;
	dataType2 = TYPE_STRING ;
	int2 = 0 ;
	ulong2 = 0 ;
	double2 = 0 ;
	bool2 = 0 ;
	strcpy ( value2, "" ) ;
	context2 = -1 ;
      }
      else {
	if ( isEmpty1 ) {
	  tokenType1 = tokenType2 ;
	  dataType1 = dataType2 ;
	  int1 = 0 ;
	  ulong1 = 0 ;
	  double1 = 0 ;
	  bool1 = 0 ;
	  strcpy ( value1, "" ) ;
	  len1 = 0 ;
	  context1 = context2 ;
	}
	else {
	  tokenType2 = tokenType1 ;
	  dataType2 = dataType1 ;
	  int2 = 0 ;
	  ulong2 = 0 ;
	  double2 = 0 ;
	  bool2 = 0 ;
	  strcpy ( value2, "" ) ;
	  len2 = 0 ;
	  context2 = context1 ;
	}
      }
    }

    /* Result goes here */
    pTmp = gHyp_data_new ( NULL ) ;

    if ( dataType1 == TYPE_STRING || dataType2 == TYPE_STRING ||
	 dataType1 == TYPE_LIST || dataType2 == TYPE_LIST )
      
      switch ( oper ) {
	
      case TOKEN_ADD:	
	pStr3 = (char *) AllocMemory ( len1+len2+1 ) ;
	assert ( pStr3 ) ;
	memcpy ( pStr3, pStr1, len1 ) ;
	memcpy ( pStr3+len1, pStr2, len2 ) ;
	*(pStr3+len1+len2) = '\0' ;
	gHyp_data_setStr_n ( pTmp, pStr3, (len1+len2) ) ;
	ReleaseMemory ( pStr3 ) ;
	break ;
	
      case TOKEN_SUB:
	/* Search for last occurance of str2 in str1 */
	pSubStr = NULL ;
	pSearchStr = pStr1 ;
	while ( *pSearchStr && 
		*pStr2 && 
		(pStr3 = strstr ( pSearchStr, pStr2 ) ) ) {
	  pSubStr = pStr3 ;
	  pSearchStr = pSubStr + len2 ;
	}
	if ( pSubStr ) {
	  pStr3 = (char *) AllocMemory ( len1-len2+1 ) ;
	  assert ( pStr3 ) ;
	  sprintf ( pStr3, 
		    "%.*s%s",
		    (int) (pSubStr-pStr1), value1,
		    pSearchStr ) ;
	  gHyp_data_setStr ( pTmp, pStr3 ) ;
	  ReleaseMemory ( pStr3 ) ;
	}
	else
	  /* Str2 was not found in Str1, so the result is Str1 */
	  gHyp_data_setStr ( pTmp, pStr1 ) ;
	break ;
	
      case TOKEN_MUL:
	gHyp_data_setBool ( pTmp, FALSE ) ;
	gHyp_instance_warning ( pAI, STATUS_UNDEFINED, 
				"String multiplication is undefined" ) ;
	pValue1 = NULL ;
        pValue2 = NULL ;

	break ;
	
      case TOKEN_POW:
	gHyp_data_setBool ( pTmp, FALSE ) ;
	pValue1 = NULL ;
        pValue2 = NULL ;
	gHyp_instance_warning ( pAI, STATUS_UNDEFINED, 
				"String exponentation is undefined" ) ;
	break ;

      case TOKEN_MOD:
	gHyp_data_setBool ( pTmp, FALSE ) ;
	pValue1 = NULL ;
        pValue2 = NULL ;
	gHyp_instance_warning ( pAI, STATUS_UNDEFINED, 
				"String mod is undefined" ) ;
	break ;
	
      case TOKEN_AND:
	gHyp_data_setInt ( pTmp, ( bool1 && bool2 ) ) ;
	break ;
	
      case TOKEN_OR:	
	gHyp_data_setInt ( pTmp, ( bool1 || bool2 ) ) ;
	break ;
	
      case TOKEN_EQ:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) ( strcmp(pStr1,pStr2) == 0 ));
	break ;
	
      case TOKEN_NE:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) ( strcmp(pStr1,pStr2) != 0 ));
	break ;
	
      case TOKEN_GT:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) ( strcmp(pStr1,pStr2) > 0 ));
	break ;
	
      case TOKEN_GE:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) ( strcmp(pStr1,pStr2) >= 0 ));
	break ;
	
      case TOKEN_LT:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) ( strcmp(pStr1,pStr2 ) < 0 ));
	break ;
	
      case TOKEN_LE:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) ( strcmp(pStr1,pStr2) <= 0 ));
	break ;
	
      default :	
	gHyp_data_setBool ( pTmp, FALSE ) ;
	pValue1 = NULL ;
        pValue2 = NULL ;
	gHyp_instance_warning ( pAI, STATUS_UNDEFINED, 
				"Unsupported operation" ) ;
	
      }
    
    else if ( dataType1 == TYPE_DOUBLE || dataType1 == TYPE_FLOAT ||
	      dataType2 == TYPE_DOUBLE || dataType2 == TYPE_FLOAT )
      
      switch ( oper ) {
	
      case TOKEN_ADD:	
	gHyp_data_setDouble ( pTmp, ( double1 + double2 ) ) ;
	break ;
	
      case TOKEN_SUB:	
	gHyp_data_setDouble ( pTmp, ( double1 - double2 ) ) ;
	break ;
	
      case TOKEN_MUL:
	gHyp_data_setDouble ( pTmp, (double1 * double2)) ;
	break ;
		
      case TOKEN_POW:
	gHyp_data_setDouble ( pTmp, pow(double1,double2) ) ;
	break ;

      case TOKEN_DIV:
	if ( double2 == 0.0 ) {
	  gHyp_data_delete ( pResult ) ;
 	  pValue1 = NULL ;
          pValue2 = NULL ;
	  gHyp_instance_error ( pAI, STATUS_BOUNDS, "Divide by zero" ) ;
	}
	else
	  gHyp_data_setDouble ( pTmp, (double1/double2)) ;
	break ;
	
      case TOKEN_MOD:
	gHyp_data_setInt ( pTmp, ( int1 % int2 ) ) ;
	break ;
	
      case TOKEN_AND:
	gHyp_data_setInt ( pTmp, ( bool1 && bool2 ) ) ;
	break ;
	
      case TOKEN_OR:	
	gHyp_data_setInt ( pTmp, ( bool1 || bool2 ) ) ;
	break ;
	
      case TOKEN_EQ:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) (double1 == double2) ) ;
	break ;
	
      case TOKEN_NE:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) (double1 != double2) ) ;
	break ;
	
      case TOKEN_GT:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) (double1 > double2) ) ;
	break ;
	
      case TOKEN_GE:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) (double1 >= double2) ) ;
	break ;
	
      case TOKEN_LT:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) (double1 < double2) ) ;
	break ;
	
      case TOKEN_LE:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) (double1 <= double2) ) ;
	break ;
	
      default :	
	gHyp_data_setBool ( pTmp, FALSE ) ;
	pValue1 = NULL ;
        pValue2 = NULL ;
	gHyp_instance_warning ( pAI, STATUS_UNDEFINED, 
				"Unsupported operation" ) ;
      }
    
    else {
      
      sign1 = ( ( dataType1 == TYPE_BYTE ) ||
		( dataType1 == TYPE_CHAR ) ||
		( dataType1 == TYPE_SHORT ) ||
		( dataType1 == TYPE_LONG ) ||
		( dataType1 == TYPE_INTEGER ) ) ? 1 : 0 ;


      sign2 = (	( dataType2 == TYPE_BYTE ) ||
		( dataType2 == TYPE_CHAR ) ||
		( dataType2 == TYPE_SHORT ) ||
		( dataType2 == TYPE_LONG ) ||
		( dataType2 == TYPE_INTEGER ) ) ? 2 : 0 ;

      sign = sign1 + sign2 ;

      switch ( oper ) {
	
      case TOKEN_ADD:
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 + ulong2 ) ) ;
	    break ;
	  case SIGNED_UNSIGNED:	
	    gHyp_data_setInt ( pTmp, ( ulong1 + int2 ) ) ;
	    break ;
	  case UNSIGNED_SIGNED :	
	    gHyp_data_setInt ( pTmp, ( int1 + ulong2 ) ) ;
	    break ;
	  case BOTH_SIGNED :	
	    gHyp_data_setInt ( pTmp, ( int1 + int2 ) ) ;
	    break ;
	}
	break ;

      case TOKEN_SUB:	
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 - ulong2 ) ) ;
	    break ;
	  case SIGNED_UNSIGNED:	
	    gHyp_data_setInt ( pTmp, ( ulong1 - int2 ) ) ;
	    break ;
	  case UNSIGNED_SIGNED :	
	    gHyp_data_setInt ( pTmp, ( int1 - ulong2 ) ) ;
	    break ;
	  case BOTH_SIGNED :	
	    gHyp_data_setInt ( pTmp, ( int1 - int2 ) ) ;
	    break ;
	}
	break ;
      case TOKEN_MUL:
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 * ulong2 ) ) ;
	    break ;
	  case SIGNED_UNSIGNED:	
	    gHyp_data_setInt ( pTmp, ( ulong1 * int2 ) ) ;
	    break ;
	  case UNSIGNED_SIGNED :	
	    gHyp_data_setInt ( pTmp, ( int1 * ulong2 ) ) ;
	    break ;
	  case BOTH_SIGNED :	
	    gHyp_data_setInt ( pTmp, ( int1 * int2 ) ) ;
	    break ;
	}
	break ;
      case TOKEN_POW:
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, (int) pow ( ulong1, ulong2 ) ) ;
	    break ;
	  case SIGNED_UNSIGNED:	
	    gHyp_data_setInt ( pTmp, (int) pow ( ulong1, int2 ) ) ;
	    break ;
	  case UNSIGNED_SIGNED :	
	    gHyp_data_setInt ( pTmp, (int) pow ( int1, ulong2 ) ) ;
	    break ;
	  case BOTH_SIGNED :	
	    gHyp_data_setInt ( pTmp, (int) pow ( int1, int2 ) ) ;
	    break ;
	}
	break  ;
	case TOKEN_DIV:
	if ( int2 == 0.0 ) {
	  gHyp_data_delete ( pResult ) ;
	  pValue1 = NULL ;
          pValue2 = NULL ;
	  gHyp_instance_error ( pAI, STATUS_BOUNDS, "Divide by zero" ) ;
	}
	else {	
	  switch ( sign ) {
	    case BOTH_UNSIGNED :	
	      gHyp_data_setInt ( pTmp, ( ulong1 / ulong2 ) ) ;
	      break ;
	    case SIGNED_UNSIGNED:	
	      gHyp_data_setInt ( pTmp, ( ulong1 / int2 ) ) ;
	      break ;
	    case UNSIGNED_SIGNED :	
	      gHyp_data_setInt ( pTmp, ( int1 / ulong2 ) ) ;
	      break ;
	    case BOTH_SIGNED :	
	      gHyp_data_setInt ( pTmp, ( int1 / int2 ) ) ;
	      break ;
	  }
	}
	break ;
      case TOKEN_MOD:
	if ( int2 == 0.0 ) {
	  gHyp_data_delete ( pResult ) ;
	  pValue1 = NULL ;
          pValue2 = NULL ;
	  gHyp_instance_error ( pAI, STATUS_BOUNDS, "Divide by zero" ) ;
	}
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 % ulong2 ) ) ;
	    break ;
	  case SIGNED_UNSIGNED:	
	    gHyp_data_setInt ( pTmp, ( ulong1 % int2 ) ) ;
	    break ;
	  case UNSIGNED_SIGNED :	
	    gHyp_data_setInt ( pTmp, ( int1 % ulong2 ) ) ;
	    break ;
	  case BOTH_SIGNED :	
	    gHyp_data_setInt ( pTmp, ( int1 % int2 ) ) ;
	    break ;
	}
	break ;
      case TOKEN_AND:
	gHyp_data_setBool ( pTmp, (sLOGICAL) ( bool1 && bool2 ) ) ;
	break ;
      case TOKEN_OR:	
	gHyp_data_setBool ( pTmp, (sLOGICAL) ( bool1 || bool2 ) ) ;
	break ;
      case TOKEN_BITAND:
	gHyp_data_setInt ( pTmp, ( ulong1 & ulong2 ) ) ;
	break ;
      case TOKEN_BITOR:	
	gHyp_data_setInt ( pTmp, ( ulong1 | ulong2 ) ) ;
	break ;
      case TOKEN_BITXOR:	
	gHyp_data_setInt ( pTmp, ( ulong1 ^ ulong2 ) ) ;
	break ;
      case TOKEN_EQ:	
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 == ulong2 ) ) ;
	    break ;
	  default :	
	    gHyp_data_setInt ( pTmp, ( int1 == int2 ) ) ;
	    break ;
	}
	break ;
      case TOKEN_NE:	
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 != ulong2 ) ) ;
	    break ;
	  default :	
	    gHyp_data_setInt ( pTmp, ( int1 != int2 ) ) ;
	    break ;
	}
	break ;
      case TOKEN_GT:	
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 > ulong2 ) ) ;
	    break ;
	  default :	
	    gHyp_data_setInt ( pTmp, ( int1 > int2 ) ) ;
	    break ;
	}
	break ;
      case TOKEN_GE:	
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 >= ulong2 ) ) ;
	    break ;
	  default :	
	    gHyp_data_setInt ( pTmp, ( int1 >= int2 ) ) ;
	    break ;
	}
	break ;
      case TOKEN_LT:	
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 < ulong2 ) ) ;
	    break ;
	  default :	
	    gHyp_data_setInt ( pTmp, ( int1 < int2 ) ) ;
	    break ;
	}
	break ;
      case TOKEN_LE:	
	switch ( sign ) {
	  case BOTH_UNSIGNED :	
	    gHyp_data_setInt ( pTmp, ( ulong1 <= ulong2 ) ) ;
	    break ;
	  default :	
	    gHyp_data_setInt ( pTmp, ( int1 <= int2 ) ) ;
	    break ;
	}
	break ;
      default :	
	gHyp_data_setBool ( pTmp, FALSE ) ;
	pValue1 = NULL ;
        pValue2 = NULL ;
	gHyp_instance_warning ( pAI, STATUS_UNDEFINED, 
				"Unsupported operation" ) ;
      }
    }

    /* Add pTmp to pResult */
    gHyp_data_append ( pResult, pTmp ) ;	
    
    /* If either argument was empty, then we're done. */
    if ( !pValue1 || !pValue2 ) break ;
  }
  

  return pResult ;
}

static sData *lHyp_operator_unaryOp (	sInstance	*pAI,
					sFrame		*pFrame,
					char		oper,
					sData 		*pArg )
{

  sData
    *pValue,
    *pResult,
    *pTmp ;

  char
    value[VALUE_SIZE+1],
    *pArgStr ;

  int	
    valueLen,
    intVal,
    context,
    ss ;

  unsigned long
    ulongVal ;

  double
    doubleVal ;

  sLOGICAL
    isEmpty = TRUE,
    boolVal,
    isVector,
    isSigned;

  sBYTE
    dataType,
    tokenType ;

  /* Initialize result */
  pResult = gHyp_data_new ( "_op1_" ) ;
  pArgStr = gHyp_data_getLabel ( pArg ) ;

  /* For all values of arg */
  pValue = NULL ;
  isVector = (gHyp_data_getDataType( pArg ) > TYPE_STRING ) ;
  ss = gHyp_data_getSubScript ( pArg ) ; context = -1 ;
  while ( ( pValue = gHyp_data_nextValue ( pArg, 
					   pValue, 
					   &context,
					   ss ) ) ) {
    isEmpty = FALSE ;
    gHyp_data_getAll (	pValue, 
			&tokenType, 
			&dataType, 
			&intVal, 
			&ulongVal,
			&doubleVal, 
			&boolVal,
			value,
			&valueLen,
			VALUE_SIZE,
			context,
			isVector ) ;

    isSigned =	( dataType == TYPE_BYTE ) ||
		( dataType == TYPE_CHAR ) ||
		( dataType == TYPE_SHORT ) ||
		( dataType == TYPE_LONG ) ||
		( dataType == TYPE_INTEGER ) ;


    switch ( oper ) {

    case TOKEN_NOT:	
      pTmp = gHyp_data_new ( NULL ) ;
      if ( dataType == TYPE_FLOAT || dataType == TYPE_DOUBLE ) {
        gHyp_data_setBool ( pTmp, (sLOGICAL) (doubleVal == 0 ? 1 : 0) ) ;
      }
      else if ( isSigned )
        gHyp_data_setInt ( pTmp, !intVal ) ;
      else
        gHyp_data_setInt ( pTmp, !ulongVal ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;
      
    case TOKEN_COMPLEMENT:	
      pTmp = gHyp_data_new ( NULL ) ;
      if ( dataType == TYPE_FLOAT || dataType == TYPE_DOUBLE ) {
        gHyp_data_setBool ( pTmp, (sLOGICAL) (doubleVal == 0 ? 1 : 0) ) ;
      }
      else if ( isSigned )
        gHyp_data_setInt ( pTmp, ~intVal ) ;
      else
        gHyp_data_setInt ( pTmp, ~ulongVal ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;
      
    case TOKEN_NEG:	
      pTmp = gHyp_data_new ( NULL ) ;
      if ( dataType == TYPE_FLOAT || dataType == TYPE_DOUBLE )
	gHyp_data_setDouble ( pTmp, -doubleVal ) ;
      else if ( isSigned )
	gHyp_data_setInt ( pTmp, -intVal ) ;
      else
        gHyp_data_setInt ( pTmp, -(int)ulongVal ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;
      
    case TOKEN_POS:	

      pTmp = gHyp_data_new ( NULL ) ;
      if ( dataType == TYPE_DOUBLE )
	gHyp_data_setDouble ( pTmp, doubleVal ) ;
      else if ( isSigned )
	gHyp_data_setInt ( pTmp, intVal ) ;
      else
        gHyp_data_setInt ( pTmp, ulongVal ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;
      

    case TOKEN_PREINC:
      
      if ( pValue != pArg ) {
	pTmp = gHyp_data_new ( NULL ) ;
	if ( dataType == TYPE_FLOAT || dataType == TYPE_DOUBLE ) {
	  gHyp_data_increment ( pValue, context, isVector ) ;
	  doubleVal++ ;
	  gHyp_data_setDouble ( pTmp,   doubleVal ) ;
	}
	else {
	  gHyp_data_increment ( pValue, context, isVector ) ;
	  intVal++ ;
	  gHyp_data_setInt ( pTmp,   intVal ) ;
	}
	gHyp_data_append ( pResult, pTmp ) ;	
      }
      else {
	gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "'%s' is not a valid variable name",
			      pArgStr ) ;
      }
      break ;

    case TOKEN_PREDEC:

      if ( pValue != pArg ) {
	pTmp = gHyp_data_new ( NULL ) ;
	if ( dataType == TYPE_FLOAT || dataType == TYPE_DOUBLE ) {
	  gHyp_data_decrement ( pValue, context, isVector ) ;
	  doubleVal-- ;
	  gHyp_data_setDouble ( pTmp,   doubleVal ) ;
	}
	else {
	  gHyp_data_decrement ( pValue, context, isVector ) ;
	  intVal-- ;
	  gHyp_data_setInt ( pTmp,   intVal ) ;
	}
	gHyp_data_append ( pResult, pTmp ) ;	
      }
      else {
	gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "'%s' is not a valid variable name",
			      pArgStr ) ;
      }
      break ;

    case TOKEN_POSTINC:

      if ( pValue != pArg ) {
	pTmp = gHyp_data_new ( NULL ) ;
	if ( dataType == TYPE_FLOAT || dataType == TYPE_DOUBLE ) {
	  gHyp_data_setDouble ( pTmp,   doubleVal ) ;
	  gHyp_data_increment ( pValue, context, isVector ) ;
	}
	else {
	  gHyp_data_setInt ( pTmp,   intVal ) ;
	  gHyp_data_increment ( pValue, context, isVector ) ;
	}
	gHyp_data_append ( pResult, pTmp ) ;	
      }
      else {
	gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "'%s' is not a valid variable name",
			      pArgStr ) ;
      }
      break ;

    case TOKEN_POSTDEC:

      if ( pValue != pArg ) {
	pTmp = gHyp_data_new ( NULL ) ;
	if ( dataType == TYPE_FLOAT || dataType == TYPE_DOUBLE ) {
	  gHyp_data_setDouble ( pTmp,   doubleVal ) ;
	  gHyp_data_decrement ( pValue, context, isVector ) ;
	}
	else {
	  gHyp_data_setInt ( pTmp,   intVal ) ;
	  gHyp_data_decrement ( pValue, context, isVector ) ;
	}
 	gHyp_data_append ( pResult, pTmp ) ;	
      }
      else {
	gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "'%s' is not a valid variable name",
			      pArgStr ) ;
      }
      break ;
    }
  }
  if ( context== -2 && ss != -1 ) {
    gHyp_data_delete ( pResult ) ;
    gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			  "Subscript '%d' is out of bounds in argument.",ss) ;
  }

  /* Check for operations on empty lists */
  if ( isEmpty ) {
    gHyp_instance_warning ( pAI,
			  STATUS_UNDEFINED,
			  "No values from '%s'",
			  pArgStr ) ;
    pTmp = gHyp_data_new ( NULL ) ;
    gHyp_data_setBool ( pTmp, FALSE ) ;
    gHyp_data_append ( pResult, pTmp ) ;	
  }

  return pResult ;
}

void gHyp_operator_binary ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operator ( pParse, pCode, pAI ) ;

  else {

    /* STATE_EXECUTE */

    /* Infix expression is 'arg1 op arg2'. */
    /* Postfix expression is 'arg1 arg2 op' */

    sStack	
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pArg1,
      *pArg2,
      *pResult ;
    
    /* Get arg2 off the stack */
    pArg2 = gHyp_stack_popRdata ( pStack, pAI ) ; 
    
    /* Get arg1 off the stack */
    pArg1 = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_binaryOp (	pAI, pFrame,
					gHyp_hyp_tokenType ( pCode ),
					pArg1, pArg2 ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_unary ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    /* Unary operators are parsed like operands */
    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    /* STATE_EXECUTE */

    /* Infix expression is 'op arg1' */
    /* Postfix expression is 'arg1 op' */

    sStack	
      *pStack = gHyp_frame_stack ( pFrame ) ;
  
    sData
      *pArg,
      *pResult ;
    
    /* Remove the argument */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;

    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					gHyp_hyp_tokenType ( pCode ),
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_assign ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Encountered the '=' operator */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operator ( pParse, pCode, pAI ) ;

  else {

    /* STATE_EXECUTE */
    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult=NULL,
      *pLvalue,
      *pRdata ;

    sLOGICAL
      isSubVariable ;

    /* Pop the data to be assigned. Since we are using Rdata2, we
     * must dispose of pRdata ourselves. (Ie: at the end of this routine.)
     * We use stack_popRdata2 because we may need to
     * keep the data that is going to be assigned to the variable
     * if it needs to be created with the dereference/dotcreate method.
     * See lHyp_frame_dereference().
     */
    pRdata = gHyp_stack_popRdata2 ( pStack, pAI ) ;

    /* Pop the variable that will get the data */
    gpsTempData = pRdata ; 
    pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
    gpsTempData = NULL ; 

    /* The different types of Lvalues for 'a' are given by these examples:
     *
     * Expression          Lvalue 
     * a = 1 ;             &a        - reference to a
     * a[3] = 1 ;          &a[3]     - reference to a[3]   
     * b = { a = 1 } ;     'a'       - identifier
     * a.b = 1 ;       	   &a.b      - unresolved reference.
     */

    isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
		      gHyp_parse_isMethodArgs ( pParse ) ) ;
    pResult = gHyp_type_assign ( pAI,
				 pFrame,
				 pLvalue,
				 pRdata,
				 TYPE_NULL,
				 isSubVariable,
				 TRUE ) ;
    
    /* Don't forget about pRdata if we are done with it */
    if ( pResult != pRdata ) gHyp_data_delete ( pRdata ) ;

    /* Push the result */
    gHyp_stack_push ( pStack, pResult ) ;
    
  }
}

void gHyp_operator_dereference ( sInstance *pAI,sCode *pCode,sLOGICAL isPARSE )
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
 
    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    /* STATE_EXECUTE */

    sStack	
      *pStack = gHyp_frame_stack ( pFrame ) ;
  
    sData
      *pArg,
      *pValue,
      *pVariable=NULL,
      *pResult=NULL ;
    
    sHyp
      *pHyp=NULL ;

    char
      *pStr,
      *pStream,
      value[MAX_INPUT_LENGTH+1] ;

    sBYTE
      tokenType ;

    sLOGICAL
      isVector ;

    int
      n,
      lineCount=0,
      hypIndex=0,
      context,
      ss ;

    /* Remove the argument */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;

    ss = gHyp_data_getSubScript ( pArg ) ; 
    if ( ss != -1 || gHyp_data_isCountOne(pArg) ) {

      /* Dereference a single list or string variable with a single element */

      pValue = gHyp_data_getValue ( pArg, ss, TRUE ) ;
      if ( pValue ) {
        tokenType = gHyp_data_getTokenType(pValue);
        pStr = gHyp_data_getLabel ( pValue ) ; 

        if ( tokenType == TOKEN_VARIABLE ) {
	
	  /*gHyp_util_debug("Deref value %s",pStr) ;*/
	  pResult = gHyp_data_new ( NULL ) ;
	  gHyp_data_setReference ( pResult, pStr, pValue ) ;
	  gHyp_stack_push ( pStack, pResult ) ;
	  return ;
	}
        else if ((pVariable = gHyp_frame_findVariable ( pAI, pFrame, pStr ))) {

	  /*gHyp_util_debug("Deref variable %s",gHyp_data_print(pVariable)) ;*/
	  pResult = gHyp_data_new ( NULL ) ;
 	  gHyp_data_setReference ( pResult, pStr, pVariable ) ;
	  gHyp_stack_push ( pStack, pResult ) ;
	  return ;

	}
      }
    }
    /* For all values of arg */
    pValue = NULL ;
    isVector = (gHyp_data_getDataType( pArg ) > TYPE_STRING ) ;
    context = -1 ;
    while ( ( pValue = gHyp_data_nextValue ( pArg, 
 				             pValue, 
					     &context,
					     ss ) ) ) {
      n = gHyp_data_getStr ( pValue, 
	 		     value, 
			     MAX_INPUT_LENGTH, 
			     context, 
			     isVector ) ;
      pStr = value ;

      /* For first token of variable to dereference. Save position where
       * code will be loaded.
       */
      if ( lineCount == 0 ) {
	pHyp = gHyp_frame_getHyp ( pFrame ) ;
	hypIndex = gHyp_hyp_getHypCount ( pHyp ) ;
	pStream = gHyp_load_fromStream ( pAI, pHyp, "{", lineCount++ ) ;
	if ( !pStream || *pStream ) {
	  gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	  gHyp_instance_error ( pAI,
				STATUS_UNDEFINED,
				"Failed to load HyperScript segment '{'" ) ;
	}
      }

      pStream = gHyp_load_fromStream ( pAI, pHyp, pStr, lineCount++ ) ;
      if ( !pStream || *pStream ) {
	gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "Failed to load HyperScript segment '%s'",
			      pStr ) ;
      }

    }
    if ( context== -2 && ss != -1 ) {
      if ( pResult ) gHyp_data_delete ( pResult ) ; 
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
	        "Subscript '%d' is out of bounds in dereference",ss) ;
    }
    if ( lineCount > 0 ) {
      pStream = gHyp_load_fromStream ( pAI, pHyp, ";}", lineCount++ ) ;
      if ( !pStream || *pStream ) {
	gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "Failed to load HyperScript segment ';}'" ) ;
      }
      /*gHyp_util_debug("Setting deref");*/
      gHyp_instance_setDerefHandler ( pAI, 
				      hypIndex, 
				      pHyp ) ;
    }
    return ;
  }
}

void gHyp_operator_reference ( sInstance 
			       *pAI, 
			       sCode *pCode, 
			       sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operator ( pParse, pCode, pAI ) ;

  else {

    /* STATE_EXECUTE */

    sStack	*pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pVariable,
      *pRef,
      *pResult ;
    
    char
      *pVariableStr,
      variable[VALUE_SIZE+1] ;
  
    int 
      ss ;

    /* Remove the argument */
    pVariable = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pVariableStr = gHyp_data_getLabel ( pVariable ) ;
    ss = gHyp_data_getSubScript ( pVariable ) ;

    if ( ss >= 0 ) {

      sprintf ( variable,
		"%s[%d]", 
		pVariableStr,
		ss ) ;

      pVariableStr = variable ;
    }

    pResult = gHyp_data_new ( "_ref_" ) ;
    pRef = gHyp_data_new ( NULL ) ;
    gHyp_data_setReference ( pRef, pVariableStr, 
			     NULL /* gHyp_data_getReference(pVariable) */ ) ;
    gHyp_data_append ( pResult, pRef ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_condition ( sInstance 
			       *pAI, 
			       sCode *pCode, 
			       sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operator ( pParse, pCode, pAI ) ;

  else {

    /* STATE_EXECUTE */

    sStack	
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult,
      *pData ;

    /* Execute the 'x ? a : b' expression. */

    pResult = gHyp_stack_popRdata2 ( pStack, pAI ) ;     
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_eval ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Encountered the ':' symbol. 
   *
   * It is not known yet whether or not the ':' will be a label marker or
   * part of a "x ? a : b" expression.  When the expression is evaluated,
   * it will be determined.
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE ) gHyp_parse_operator ( pParse, pCode, pAI ) ;
}


void gHyp_operator_dot ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;
  
  if ( isPARSE )
    
    gHyp_parse_operator ( pParse, pCode, pAI ) ;

  else {

    /* STATE_EXECUTE */

    /* Infix expression is 'variable.subvariable'. */

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pParent,
      *pChild=NULL,
      *pResult,
      *pSubVariable,
      *pVariable,
      *pData,
      *pArgs;
 
    char
      newVariable[MAX_OUTPUT_LENGTH+1],
      variable[VALUE_SIZE+1],
      subVariable[VALUE_SIZE+1],
      *pVariableStr,
      *pSubVariableStr ;
    
    sBYTE
      variableTT,
      subVariableTT ;

    int
      ssSubVariable,
      ssVariable ;

    /* Check to see if this is a method call as well */
    pData = gHyp_stack_peek ( pStack ) ;
    if ( strcmp ( gHyp_data_getLabel ( pData ), "_parms_" ) == 0 ) {
      pArgs = gHyp_stack_popRdata2 ( pStack, pAI ) ;
      // Because we used Rdata2, and we have popLvalue's to follow
      // that may fail, we need to remember to collect the garbage.
      gpsTempData = pArgs ;
    }
    else
      pArgs = NULL ;

    /* Get the sub-variable off the stack */
    pSubVariable = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pSubVariableStr = gHyp_data_getLabel ( pSubVariable ) ;
    ssSubVariable = gHyp_data_getSubScript ( pSubVariable ) ;
    subVariableTT = gHyp_data_getTokenType ( pSubVariable ) ;

    /* Get parent variable off the stack */
    pVariable = gHyp_stack_popLvalue ( pStack, pAI ) ;
    pVariableStr = gHyp_data_getLabel ( pVariable ) ;
    ssVariable = gHyp_data_getSubScript ( pVariable ) ;
    variableTT = gHyp_data_getTokenType ( pVariable ) ;

    gpsTempData = NULL ;

    if ( ssVariable == -1 ) {

      /* No subscript on parent. */

      /* See if the parent exists */
      pParent = gHyp_data_getVariable ( pVariable ) ;
      

      /* If the parent is real, look for the sub-variable */
      if ( pParent )
	pChild = gHyp_data_getChildByName ( pParent, pSubVariableStr ) ;
 
      if ( !pChild &&
	   gHyp_frame_testGlobalFlag ( pFrame, FRAME_GLOBAL_DOTCREATE ) ) {
	
	/* This condition occurs when an assignment statement is specified
	 * that has more than one level of subVariable.  For example:
	 *
	 * a.b.c = 1 ;
	 *
	 * In the above example, the variable a, its subvariable b, and b.c
	 * will be created.
	 *
	 * The condition for "DOTCREATE" occurs with the context of a
	 * "dereference" handler. This works as follows:
	 *
	 * 1. First a.b.c is parsed, and the stack will contain an
	 *    unresolved reference to a.b.c.
	 * 2. The assignment operation, if it sees that the variable to
	 *    assign is an unresolveable reference, sets up a special
	 *    dereference handler to evaluate the expression "a.b.c"
	 * 3. In the context of a dereference handler, the components
	 *    of a.b.c are created.
	 *
	 * Note that after creating "a.b.c", we could also write:
	 *
	 * a[0].d = 1
	 *
	 * This routine does not get it as a[0].d, however, it gets it
	 * as a.b.d, since the routine gHyp_stack_popLvalue will first 
	 * resolve a[0] into a.b.
	 */
	/*
	  gHyp_util_logInfo("In op_dot with %s.%s", 
			  pVariableStr, pSubVariableStr ) ;
	*/
	if ( ( pParent ||
	       variableTT == TOKEN_UNIDENTIFIED ||
	       gHyp_util_isIdentifier ( pVariableStr ) ) 
	       &&
	     ( subVariableTT == TOKEN_UNIDENTIFIED ||
	       gHyp_util_isIdentifier ( pSubVariableStr ) ) ) {

	  /* Two identifiers id1.id2 makes a good variable */	  
	  if ( guDebugFlags & DEBUG_DIAGNOSTICS )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS, 
				 "Creating '%s'.'%s'", 
				 pVariableStr, pSubVariableStr ) ;
	  
	  if ( !pParent ) 
	    pParent = gHyp_frame_createVariable ( pFrame, pVariableStr ) ;
	  pChild = gHyp_data_new ( pSubVariableStr ) ;
	  /*gHyp_util_debug("%p(%s)new-childof(%s)",pChild,pSubVariableStr,
			    pVariableStr ) ;
          */
	  gHyp_data_append ( pParent, pChild ) ;
	}
	else {
          if ( pArgs ) gHyp_data_delete ( pArgs ) ;
	  gHyp_instance_error ( pAI,
				STATUS_UNDEFINED,
				"'%s'.'%s' is not a valid variable name",
				pVariableStr, 
				pSubVariableStr ) ;
	}
      }
    }
    
    /* Create the result */
    pResult = gHyp_data_new ( NULL ) ;
    
    if ( *pVariableStr != '\'' ) 
      sprintf ( variable, "'%s'", pVariableStr ) ;
    else 
      strcpy ( variable, pVariableStr ) ;
    
    if ( *pSubVariableStr != '\'' ) 
      sprintf ( subVariable, "'%s'", pSubVariableStr ) ;
    else 
      strcpy ( subVariable, pSubVariableStr ) ;

    if ( pChild ) {
      
      /* Make the referenced variable more descriptive.  Include any
       * parent variable subscript that was specified.  Do not, however,
       * include the subVariable subscript, because it is still specified
       * with the data_setSubScript call.
       */
      if ( ssVariable >= 0 ) 
	/* Subscripted */
	sprintf ( newVariable,
		  "%s[%d].%s", 
		  variable,
		  ssVariable,
		  subVariable ) ;
      else
	/* Not subscripted */
	sprintf ( newVariable,
		  "%s.%s", 
		  variable,
		  subVariable ) ;
      
      gHyp_data_setReference ( pResult, newVariable, pChild ) ;
      
      if ( ssSubVariable >= 0 ) 
	gHyp_data_setSubScript ( pResult, ssSubVariable ) ;
    }
    else {
      
      /* The variable was neither created nor found.  
       * It becomes or remains an unresolved reference. 
       * Later when a function needs its value, an error will be
       * generated by gHyp_stack_popRdata.
       */
      if ( ssVariable >= 0 && ssSubVariable >= 0 ) 
	sprintf ( newVariable,
		  "%s[%d].%s[%d]", 
		  variable,
		  ssVariable,
		  subVariable,
		  ssSubVariable ) ;
      
      else if ( ssVariable >= 0 && ssSubVariable == -1 ) 
	sprintf ( newVariable,
		  "%s[%d].%s", 
		  variable,
		  ssVariable,
		  subVariable ) ;
      
      else if ( ssVariable == -1 && ssSubVariable >= 0 ) 
	sprintf ( newVariable,
		  "%s.%s[%d]", 
		  variable,
		  subVariable,
		  ssSubVariable ) ;
      
      else 
	sprintf ( newVariable,
		  "%s.%s", 
		  variable,
		  subVariable ) ;
      
      gHyp_data_setReference ( pResult, newVariable, pChild ) ;
    }
  
    gHyp_stack_push ( pStack, pResult ) ;

    if ( pArgs ) { 
      gHyp_stack_push ( pStack, pArgs ) ;
      gHyp_instance_setMethodCall ( pAI ) ;
    }
  }
}

void gHyp_operator_pointer ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operator ( pParse, pCode, pAI ) ;
 
  else {

    /* STATE_EXECUTE */

    if ( gHyp_parse_isPointerDeref ( pParse ) )

      gHyp_operator_dereference ( pAI, pCode, FALSE ) ;

    else
      
      gHyp_operator_dot ( pAI, pCode, FALSE ) ;
  }
}
