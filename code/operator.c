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
#include "math.h"

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

static char gzStream[STREAM_READ_SIZE*4+1] ;

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
  
  void
    *handle1,
    *handle2 ;

  sBYTE
    dataType1,
    dataType2,
    tokenType1,
    tokenType2,
    sign,
    sign1,
    sign2 ;

  /* Initialize before each loop */
  tokenType1 = TOKEN_LITERAL ;
  dataType1 = TYPE_STRING ;
  int1 = 0 ;
  ulong1 = 0 ;
  double1 = 0 ;
  bool1 = 0 ;
  value1[0] = '\0' ;
  len1 = 0 ;
  context1 = -1 ;
  tokenType2 = TOKEN_LITERAL ;
  dataType2 = TYPE_STRING ;
  int2 = 0 ;
  ulong2 = 0 ;
  double2 = 0 ;
  bool2 = 0 ;
  value2[0] = '\0' ;
  len2 = 0 ;
  context2 = -1 ;

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

    /* If both lists are exhausted of values, and we have at least one result,
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
			 &handle1,
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
			 &handle2,
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
	value1[0] = '\0' ;
	len1 = 0 ;
	context1 = -1 ;
	tokenType2 = TOKEN_LITERAL ;
	dataType2 = TYPE_STRING ;
	int2 = 0 ;
	ulong2 = 0 ;
	double2 = 0 ;
	bool2 = 0 ;
	value2[0] = '\0' ;
	len2 = 0 ;
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
	  value1[0] = '\0' ;
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
	  value2[0] = '\0' ;
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
      case BINARY_POW:
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
	
      case BINARY_MAX:
	if ( strcmp(pStr1,pStr2) >= 0 )
	  gHyp_data_setStr ( pTmp, pStr1 ) ;
	else
	  gHyp_data_setStr ( pTmp, pStr2 ) ;
	break ;

      case BINARY_MIN:
	if ( strcmp(pStr1,pStr2) < 0 )
	  gHyp_data_setStr ( pTmp, pStr1 ) ;
	else
	  gHyp_data_setStr ( pTmp, pStr2 ) ;
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
      case BINARY_POW:
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
	
      case BINARY_MAX:
	gHyp_data_setDouble ( pTmp, MAX ( double1, double2 ) ) ;
	break ;

      case BINARY_MIN:
	gHyp_data_setDouble ( pTmp, MIN ( double1, double2 ) ) ;
	break ;

      case BINARY_HYPOT:
#ifdef AS_WINDOWS
	gHyp_data_setDouble ( pTmp, _hypot ( double1, double2 ) ) ;
#else
	gHyp_data_setDouble ( pTmp, hypot ( double1, double2 ) ) ;
#endif
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
      case BINARY_POW:
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
	
     case BINARY_MAX:
	gHyp_data_setDouble ( pTmp, MAX ( double1, double2 ) ) ;
	break ;

      case BINARY_MIN:
	gHyp_data_setDouble ( pTmp, MIN ( double1, double2 ) ) ;
	break ;

      case BINARY_HYPOT:
#ifdef AS_WINDOWS
	gHyp_data_setDouble ( pTmp, _hypot ( double1, double2 ) ) ;
#else
	gHyp_data_setDouble ( pTmp, hypot ( double1, double2 ) ) ;
#endif
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

  void
    *handle ;

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
			&handle,
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
    case UNARY_ABS:

      pTmp = gHyp_data_new ( NULL ) ;
      if ( dataType == TYPE_FLOAT || dataType == TYPE_DOUBLE ) {
	if ( doubleVal < 0 ) doubleVal = 0-doubleVal ;
	gHyp_data_setDouble ( pTmp, doubleVal ) ;
      }
      else if ( isSigned ) {
	if ( intVal < 0 ) intVal = 0-intVal ;
	gHyp_data_setInt ( pTmp, intVal ) ;
      }
      else {
        gHyp_data_setInt ( pTmp, ulongVal ) ;
      }
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
    
    case UNARY_ACOS:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, acos ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_ASIN:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, asin ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_ATAN:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, atan ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_DEG2RAD:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp,  (doubleVal*3.14159265358979323846/180.0)  ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_RAD2DEG:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, (doubleVal*180.0/3.14159265358979323846) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_CEIL:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, ceil ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_COS:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, cos ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_FLOOR:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, floor ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_LOG10:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, log10 ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_LOGN:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, log ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_SIN:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, sin ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_SQRT:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, sqrt ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break ;

    case UNARY_TAN:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, tan ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
      break 
	;
    case UNARY_EXP:
      pTmp = gHyp_data_new ( NULL ) ;
      gHyp_data_setDouble ( pTmp, exp ( doubleVal ) ) ;
      gHyp_data_append ( pResult, pTmp ) ;	
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
      *pStream = gzStream,
      *pAnchor = pStream,
      *pEndOfStream = pStream ;

    sBYTE
      tokenType ;

    sLOGICAL
      isVector ;

    int
      lineCount=0,
      streamLen,
      hypIndex=0,
      context=-1,
      ss=-1 ;

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

    /* Stream dereference */
    pValue = NULL ;
    streamLen = 0 ;
    *pStream = '\0' ;
    isVector = (gHyp_data_getDataType( pArg ) > TYPE_STRING ) ;
    while (
     (pStream = gHyp_util_readStream (	pStream, pAnchor, &pEndOfStream,
					&streamLen, pArg, 
					&pValue, &context, ss, isVector, 
					NULL ) ) ) {

      if ( lineCount == 0 ) {
	pHyp = gHyp_frame_getHyp ( pFrame ) ;
	hypIndex = gHyp_hyp_getHypCount ( pHyp ) ;
	pStr = gHyp_load_fromStream ( pAI, pHyp, "{", lineCount++ ) ;
	if ( !pStr || *pStr ) {
	  gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	  gHyp_instance_error ( pAI,
				STATUS_UNDEFINED,
				"Failed to load HyperScript segment (deref) '{'" ) ;
        }
      }

      pStr = pStream ;
      pStr = gHyp_load_fromStream ( pAI, pHyp, pStr, lineCount++ ) ;
      if ( !pStr || *pStr ) {
	gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "Failed to load HyperScript segment (dref) *(%s)",
			      pStream ) ;
      }
 
      pStream += streamLen ;
      if ( pValue == NULL ) break ;
    }

    if ( lineCount > 0 ) {
      pStr = gHyp_load_fromStream ( pAI, pHyp, ";}", lineCount++ ) ;
      if ( !pStr || *pStr ) {
	gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "Failed to load HyperScript segment (deref) ';}'" ) ;
      }
      /*gHyp_util_debug("Deref from operator");*/
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
   *
   * For JSON, the ":" gets treated like the assignment operator "=".
   *
   * { "a" : {1,2,3} }  is equivalent to { list a = {1,2,3} }
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE ) 
    
    gHyp_parse_operator ( pParse, pCode, pAI ) ;

  else {

    /* JSON VARIABLE ASSIGNMENT */
    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult=NULL,
      *pLvalue,
      *pRdata ;

    sLOGICAL
      hasPrevSibling,
      isSubVariable ;

    sBYTE
      exprTokenType ;

    sBYTE
      dataType,
      dataType2 ;

   
    if ( gHyp_parse_isConditional( pParse, gHyp_frame_getHyp ( pFrame ) ) ) return ;

    /* Pop the data to be assigned. Since we are using Rdata2, we
     * must dispose of pRdata ourselves. (Ie: at the end of this routine.)
     * We use stack_popRdata2 because we may need to
     * keep the data that is going to be assigned to the variable
     * if it needs to be created with the dereference/dotcreate method.
     * See lHyp_frame_dereference().
     */

    /*gHyp_util_debug ( "JSON ASSIGNMENT" ) ;*/
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

    /* Change to ATTR iff
     * 1. The data value is not a LIST, ie: it is a literal or constant.
     * 2. All previous data types for the parent's children are also ATTR
     *    (The listCount is either 1 or the top stack element is an ATTR) 
     */
    dataType = gHyp_data_getDataType ( pRdata ) ;
    exprTokenType = gHyp_parse_exprTokenType ( pParse ) ;

    dataType2 = TYPE_LIST ;
    hasPrevSibling = 0 ;
    pResult = gHyp_stack_peek ( pStack ) ;
    if ( pResult ) {
      if ( strcmp ( gHyp_data_getLabel ( pResult ), "_sub_" ) == 0 ) {
        dataType2 = gHyp_data_getDataType ( gHyp_data_getFirst ( pResult ) ) ;
        hasPrevSibling = 1 ;
      }
    }


    /*
    if ( dataType != TYPE_LIST && ( exprTokenType == TOKEN_BLIST || dataType2 == TYPE_ATTR ) ) 
      dataType = TYPE_ATTR ;

    else if ( dataType == TYPE_STRING && 
              exprTokenType == TOKEN_EVAL && 
              (!hasPrevSibling || dataType2 == TYPE_ATTR ) )
      dataType = TYPE_ATTR ;

    else
      dataType = TYPE_LIST ;
    */

    if ( strcmp ( gHyp_data_getLabel ( pRdata ), "_json_" ) == 0 ) 
      pResult = pRdata ;
    else 
      pResult = gHyp_type_assign ( pAI,
				 pFrame,
				 pLvalue,
				 pRdata,
				 dataType,
				 isSubVariable,
				 TRUE ) ;

    /* Don't forget about pRdata if we are done with it */
    if ( pResult != pRdata ) gHyp_data_delete ( pRdata ) ;

    /* Push the result */
    gHyp_stack_push ( pStack, pResult ) ;
    
  }
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
      newVariable[MAX_INPUT_LENGTH+1],
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

    /* Check to see if this is a function being called in an object sense, ie: a.toupper */
    if ( gHyp_parse_isObjectCall( pParse ) ) return ;

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
	  if ( guDebugFlags & DEBUG_HEAP )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_HEAP, 
				 "Creating '%s'.'%s'", 
				 pVariableStr, pSubVariableStr ) ;
	  
	  if ( !pParent ) 
	    pParent = gHyp_frame_createVariable ( pAI, pFrame, pVariableStr ) ;
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

/* ====== NEW HS 3. 8. 0   FUNCTIONS ***************/

void gHyp_operator_max ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: max ( value1, value2 )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg1,
      *pArg2,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: max ( value1, value2 )" ) ;

    /* Get arg2 off the stack */
    pArg2 = gHyp_stack_popRdata ( pStack, pAI ) ; 
    
    /* Get arg1 off the stack */
    pArg1 = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_binaryOp (	pAI, pFrame,
					(char) BINARY_MAX,
					pArg1, pArg2 ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_min ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: min ( value1, value2 )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg1,
      *pArg2,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: min ( value1, value2 )" ) ;

    /* Get arg2 off the stack */
    pArg2 = gHyp_stack_popRdata ( pStack, pAI ) ; 
    
    /* Get arg1 off the stack */
    pArg1 = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_binaryOp (	pAI, pFrame,
					(char) BINARY_MIN,
					pArg1, pArg2 ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_hypot ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: hypot ( value1, value2 )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg1,
      *pArg2,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: hypot ( value1, value2 )" ) ;

    /* Get arg2 off the stack */
    pArg2 = gHyp_stack_popRdata ( pStack, pAI ) ; 
    
    /* Get arg1 off the stack */
    pArg1 = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_binaryOp (	pAI, pFrame,
					(char) BINARY_HYPOT,
					pArg1, pArg2 ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_pow ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: pow ( value1, value2 )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg1,
      *pArg2,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: pow ( value1, value2 )" ) ;

    /* Get arg2 off the stack */
    pArg2 = gHyp_stack_popRdata ( pStack, pAI ) ; 
    
    /* Get arg1 off the stack */
    pArg1 = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_binaryOp (	pAI, pFrame,
					(char) BINARY_POW,
					pArg1, pArg2 ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_operator_abs ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: abs ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: abs ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_ABS,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_operator_acos ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: acos ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: acos ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_ACOS,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_asin ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: asin ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: asin ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_ASIN,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_atan ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: atan ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: atan ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_ATAN,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_ceil ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: ceil ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: ceil ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_CEIL,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_cos ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: cos ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: cos ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_COS,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_floor ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: floor ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: floor ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_FLOOR,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_log10 ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: log10 ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: log10 ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_LOG10,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_logN ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: logN ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: logN ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_LOGN,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_sin ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sin ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: sin ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_SIN,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_sqrt ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sqrt ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: sqrt ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_SQRT,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_tan ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: tan ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: tan ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_TAN,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}



void gHyp_operator_exp ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: exp ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: exp ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_EXP,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_deg2rad ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: deg2rad ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: deg2rad ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_DEG2RAD,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_operator_rad2deg ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: rad2deg ( value )
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
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pArg,
      *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	  "Invalid arguments. Usage: rad2deg ( value )" ) ;
    
    /* Get the arg off the stack */
    pArg = gHyp_stack_popRdata ( pStack, pAI ) ;
	
    pResult = lHyp_operator_unaryOp (	pAI, pFrame,
					(char) UNARY_RAD2DEG,
					pArg ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

