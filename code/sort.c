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
 *   $Log: sort.c,v $
 *   Revision 1.4  2009-03-07 21:27:32  bergsma
 *   gHyp_data_getAll needs additional handle argument
 *
 *   Revision 1.3  2002-11-14 01:43:57  bergsma
 *   Ditto.
 *
 *   Revision 1.2  2002/11/14 01:40:45  bergsma
 *   Removed ^M occurances and added Modification section.
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

static sLOGICAL lHyp_sort_compExch ( sData *pData, 
				     int i1, 
				     sBYTE oper, 
				     int i2,
				     sLOGICAL compare,
				     sLOGICAL exchange ) {
  
  sData 
    *pValue1,
    *pValue2 ;

  sLOGICAL
    result = TRUE ;

  char
    value1[VALUE_SIZE+1],
    value2[VALUE_SIZE+2],
    *pStr1 = value1,
    *pStr2 = value2 ;
  
  int
    len1,
    len2,
    int1,
    int2 ;
  
  unsigned long
    ulong1,
    ulong2 ;

  double
    double1,
    double2 ;
  
  sLOGICAL
    bool1,
    bool2 ;
  
  sBYTE
    dataType1,
    dataType2,
    tokenType1,
    tokenType2 ;

  void
    *handle1,
    *handle2 ;

  pValue1 = gHyp_data_getAll ( pData, 
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
			       i1,
			       TRUE ) ;
  
  pValue2 = gHyp_data_getAll ( pData, 
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
			       i2,
			       TRUE ) ;

  if ( compare ) {

    if ( dataType1 == TYPE_STRING || dataType2 == TYPE_STRING ||
         dataType1 == TYPE_LIST || dataType2 == TYPE_LIST ) {
      
      switch ( oper ) {
      case TOKEN_EQ:	
	result = (sLOGICAL) ( strcmp(pStr1,pStr2) == 0 ) ;
	break ;
      case TOKEN_NE:	
	result = (sLOGICAL) ( strcmp(pStr1,pStr2) != 0 ) ;
	break ;
      case TOKEN_GT:	
	result = (sLOGICAL) ( strcmp(pStr1,pStr2) > 0 ) ;
	break ;
      case TOKEN_GE:	
	result = (sLOGICAL) ( strcmp(pStr1,pStr2) >= 0 ) ;
	break ;
      case TOKEN_LT:	
	result = (sLOGICAL) ( strcmp(pStr1,pStr2 ) < 0 );
	break ;
      case TOKEN_LE:	
	result = (sLOGICAL) ( strcmp(pStr1,pStr2) <= 0 );
	break ;
      }
    }
    else if ( dataType1 == TYPE_DOUBLE || dataType1 == TYPE_FLOAT ||
	      dataType2 == TYPE_DOUBLE || dataType2 == TYPE_FLOAT ) {
      
      switch ( oper ) {
 	
      case TOKEN_EQ:	
	result = (sLOGICAL) (double1 == double2) ;
	break ;
      case TOKEN_NE:	
	result = (sLOGICAL) (double1 != double2) ;
	break ;
      case TOKEN_GT:	
	result = (sLOGICAL) (double1 > double2) ;
	break ;
      case TOKEN_GE:	
	result = (sLOGICAL) (double1 >= double2) ;
	break ;
      case TOKEN_LT:	
	result = (sLOGICAL) (double1 < double2) ; 
	break ;
      case TOKEN_LE:	
	result = (sLOGICAL) (double1 <= double2) ;
	break ;
      }
      
    }
    else { /* if ( dataType1 == TYPE_INTEGER || dataType2 == TYPE_INTEGER ) */
      
      switch ( oper ) {
      case TOKEN_EQ:	
	result = (sLOGICAL) (int1 == int2) ;
	break ;
      case TOKEN_NE:	
	result = (sLOGICAL) (int1 != int2) ;
	break ;
      case TOKEN_GT:	
	result = (sLOGICAL) (int1 > int2) ;
	break ;
      case TOKEN_GE:	
	result = (sLOGICAL) (int1 >= int2) ;
	break ;
      case TOKEN_LT:	
	result = (sLOGICAL) (int1 < int2) ;
	break ;
      case TOKEN_LE:	
	result = (sLOGICAL) (int1 <= int2) ;
	break ;
      }
    }
  }

  if ( result && exchange ) 
    gHyp_data_exchange ( pValue1, pValue2) ;

  return result ;
}


static void lHyp_sort_quick ( sData *pData, int m, int n ) {

  /* Called with m = 0 and n = length of array to sort. 
   *
   * 
   *
      void quicksort(int m, int n) {
	int i,j,pivot;
	if (n<=m+1) return;
	if (n-m==2) {
	  if (get(n-1)<get(m)) {
	    exchange(m,n-1);
	  }
	  return;
	}
	i=m+1;
	j=n-1;
	if (get(i)>get(m)) {
	  exchange(i,m);
	}
	if (get(j)<get(m)) {
	  exchange(m,j);
	}
	if (get(i)>get(m)) {
	  exchange(i,m);
	}
	
	pivot=get(m);
	setColor(m,Color.red);
	while(true) {
	  j--;
	  while (pivot<get(j)) j--;
	  i++;
	  while (pivot>get(i)) i++;
	  if (j<=i) break;
	  exchange(i,j);
	}
	exchange(m,j);
	if (j-m < n-j) {
	  quicksort(m,j);
	  quicksort(j+1,n);
	} else {
	  quicksort(j+1,n);
	  quicksort(m,j);
	}
      }
      
      *
      */

  int 
    i,
    j,
    pivot ;

  if ( n <= m+1 ) return ;

  if ( n-m == 2 ) {
    lHyp_sort_compExch ( pData, n-1, TOKEN_LT, m, TRUE, TRUE ) ;
    return;
  }
  

  i = m+1;
  j = n-1;

  lHyp_sort_compExch ( pData, i, TOKEN_GT, m, TRUE, TRUE ) ;
  lHyp_sort_compExch ( pData, j, TOKEN_LT, m, TRUE, TRUE ) ;
  lHyp_sort_compExch ( pData, i, TOKEN_GT, m, TRUE, TRUE ) ;

  
  pivot = m ;
  while ( 1 ) {
    j-- ;
    while ( lHyp_sort_compExch ( pData, 
				 pivot, 
				 TOKEN_LT, 
				 j, 
				 TRUE, 
				 FALSE )) j--;
    i++ ;
    while ( lHyp_sort_compExch ( pData, 
				 pivot, 
				 TOKEN_GT, 
				 i, 
				 TRUE, 
				 FALSE )) i++;
    if ( j <= i ) break;
    lHyp_sort_compExch ( pData, i, TOKEN_NULL, j, FALSE, TRUE ) ;
  }
  lHyp_sort_compExch ( pData, m, TOKEN_NULL, j, FALSE, TRUE ) ;
  if ( j-m < n-j ) {
    lHyp_sort_quick ( pData, m, j ) ;
    lHyp_sort_quick ( pData, j+1, n ) ;
  } 
  else {
    lHyp_sort_quick ( pData, j+1, n ) ;
    lHyp_sort_quick ( pData, m, j ) ;
  }
}

static void lHyp_sort_isort ( sData *pData, 
			      int m, 
			      int k, 
			      int n ) 
{
  /*
  void isort(int m,int k) {
    int i,j;
    setColor(m,series[m%series.length]);
    for (j=m+k;j<N;j+=k) {
      setColor(j,series[m%series.length]);
      for (i=j;i>=k && get(i)<get(i-k);i-=k) {
	exchange(i,i-k);
      }
    }
  }
  */
  int
    i,
    j ;

  for ( j=m+k; j<n; j+=k )
    for ( i=j; 
	  i>=k && lHyp_sort_compExch( pData,i,TOKEN_LT,i-k,TRUE,FALSE);
	  i-=k )
      lHyp_sort_compExch ( pData,i,TOKEN_NULL,i-k,FALSE,TRUE) ;
}

static void lHyp_sort_shell ( sData *pData, int m, int n )
{
  /*
  k=N/5;
  for(m=0;m<k;m++) {
    isort(m,k);
  }
  k=N/7;
  for(m=0;m<k;m++) {
    isort(m,k);
  }
  for (k=7;k>0;k-=2) {
    for(m=0;m<k;m++) {
      isort(m,k);
    }
  }
  */

  int k ;

  k = n / 5 ;
  for ( m=0; m<k; m++ ) lHyp_sort_isort ( pData, m, k, n ) ;

  k = n / 7 ;
  for ( m=0; m<k; m++ ) lHyp_sort_isort ( pData, m, k, n ) ;

  for ( k=7; k>0; k-=2 )
    for ( m=0; m<k; m++ ) lHyp_sort_isort ( pData, m, k, n );
}

void gHyp_sort_sort ( sData *pData ) {

  int 
    m,
    n ;

  sBYTE
    tokenType ;

  tokenType = gHyp_data_getTokenType ( pData ) ;

  if ( tokenType != TOKEN_VARIABLE ) return ;  

  n = gHyp_data_getCount ( pData ) ;
  m = 0 ;
  if ( n > 20 )
    lHyp_sort_quick ( pData, m, n ) ;
  else
    lHyp_sort_shell ( pData, m, n ) ;
}

void gHyp_sort_reverse ( sData *pData )
{
  sData
    *pValue,
    *pTmpData ;

  int
    ss,
    context ;

  /* First sort it ascending */
  gHyp_sort_sort ( pData ) ;

  /* Reverse the data in another variable */
  pTmpData = gHyp_data_new ( "_sort_" ) ;
  pValue = NULL ;
  ss = -1 ; context = -1 ;
  while ( (pValue = gHyp_data_nextValue ( pData, 
					  pValue, 
					  &context,
					  ss ) ) ) {
    gHyp_data_detach ( pValue ) ;
    gHyp_data_insert ( pTmpData, pValue ) ;
    pValue = NULL ;
  }
  /* Copy the values back to the original variable */
  ss = -1 ; context = -1 ;
  pValue = NULL ;
  while ( (pValue = gHyp_data_nextValue ( pTmpData, 
					  pValue, 
					  &context,
					  ss ) ) ) {
    gHyp_data_detach ( pValue ) ;
    gHyp_data_append ( pData, pValue ) ;
    pValue = NULL ;
  }
  gHyp_data_delete ( pTmpData ) ;
  return ;
}
