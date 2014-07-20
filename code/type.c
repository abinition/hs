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
 *	$Log: type.c,v $
 *	Revision 1.32  2013-01-02 19:10:10  bergsma
 *	CVS Issues
 *	
 *	Revision 1.29  2012-05-01 17:51:21  bergsma
 *	Comment
 *	
 *	Revision 1.28  2011-02-24 05:12:27  bergsma
 *	Adjusting values for MAX_OUTPUT_LENGTH and MAX_INPUT_LENGTH
 *	
 *	Revision 1.27  2011-01-08 21:33:33  bergsma
 *	Support for JSON assignment.
 *	Increased buffer size from VALUE_SIZE to MAX_INPUT_LENGTH
 *	
 *	Revision 1.25  2010-04-23 05:16:52  bergsma
 *	Backed out fix whereby global subvariable types could not be changed while in local mode.
 *	
 *	Revision 1.24  2010-03-05 06:16:55  bergsma
 *	Cannot re-type variable if in local scope.
 *	
 *	Revision 1.23  2010-01-08 02:44:57  bergsma
 *	Added ssl_md5(), enhanced ssl_digest.
 *	Fixed urldecode, missing ":"
 *	Enabled object calls, ie:  text.strtok( ) and the like...
 *	
 *	Revision 1.22  2009-12-08 21:00:15  bergsma
 *	Comments added
 *	
 *	Revision 1.21  2009-11-20 19:24:11  bergsma
 *	Must stick to using popLvalue for typeof.  There is workaround.
 *	
 *	Revision 1.19  2009-03-07 21:27:32  bergsma
 *	gHyp_data_getAll needs additional handle argument
 *	
 *	Revision 1.18  2007-06-24 17:49:02  bergsma
 *	Return proper "typeof" for literals.
 *	
 *	Revision 1.17  2006-11-25 03:10:19  bergsma
 *	Incease size of buffer for getStr to accomodate larger strings.
 *	
 *	Revision 1.16  2006/09/16 20:07:10  bergsma
 *	When converting from ATTR, CHAR, UCHAR, or UNICODE to string,
 *	look for line breaks \n.
 *	
 *	Revision 1.15  2006/07/17 16:42:52  bergsma
 *	The 'typeof' function must use an Lvalue, not Rdata.
 *	
 *	Revision 1.14  2006/01/16 18:56:36  bergsma
 *	HS 3.6.6
 *	1. Save query timeout events.  Don't let queries repeat indefinitely.
 *	2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *	
 *	Revision 1.13  2005/04/22 19:26:56  bergsma
 *	Comment
 *	
 *	Revision 1.12  2005/03/09 04:20:28  bergsma
 *	Added scopeof function
 *	
 *	Revision 1.11  2005/01/10 21:01:20  bergsma
 *	Fixed problem with converting bytes to strings.
 *	
 *	Revision 1.10  2005/01/10 18:04:26  bergsma
 *	When converting from large byte array to string, create multiple strings
 *	instead of just one, thereby allowing > VALUE_SIZE bytes.
 *	
 *	Revision 1.9  2004/10/16 05:11:43  bergsma
 *	data_setStr and data_getStr function calls renamed
 *	
 *	Revision 1.8  2004/07/06 00:28:14  bergsma
 *	Added 'valueof()' function.
 *	
 *	Revision 1.7  2004/07/01 02:02:34  bergsma
 *	Added 'specialChars' argument to the toexternal() function.
 *	
 *	Revision 1.6  2004/04/29 02:01:40  bergsma
 *	When converting from str to char/uchar/attr/unicode, do not
 *	stop at null characters in string to convert.
 *	
 *	Revision 1.5  2003/06/05 21:54:23  bergsma
 *	Added new typedef 'unicode', UTF-16.
 *	Required for new unicode type:
 *	1. New data type in data object
 *	2. Special unicode output for xdescribe function
 *	3. New TYPE_UNICODE in hypdef.h
 *	4. New token 0uNNNNN format, like 0xNNNNN and 0oNNNNN
 *	5. New function unicode()
 *	6. New SECS II data type.
 *	
 *	Revision 1.4  2003/02/05 23:56:32  bergsma
 *	When defining method arguments, define as sub-variables so they won't
 *	interfere with regular scope variables.
 *	
 *	Revision 1.3  2003/01/09 00:21:11  bergsma
 *	V3.1.0
 *	Within typecast functions, isSubVariable LOGICAL is dependant on
 *	the list {} Depth, not the expression () depth
 *	
 *	Revision 1.2  2002/11/14 01:40:45  bergsma
 *	Removed ^M occurances and added Modification section.
 *	
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

static sLOGICAL lHyp_type_isVariableDef ( sParse *pParse, sStack *pStack )
{
  sData
    *pData ;

  int
    tokenType ;

  if ( gHyp_parse_isVariableDef ( pParse ) ) {

    if ( gHyp_stack_depth ( pStack ) > 0 ) {

      pData = gHyp_stack_peek ( pStack ) ; 
      tokenType = gHyp_data_tokenType ( pData ) ;
      if ( tokenType == TOKEN_UNIDENTIFIED ||
	   tokenType == TOKEN_REFERENCE ||
	   tokenType == TOKEN_VARIABLE ) return TRUE ;
    }
  }
  return FALSE ;
} 

/*
static sLOGICAL lHyp_type_isVariableDec ( sParse *pParse, sStack *pStack )
{
  sData
    *pData ;

  int
    tokenType ;

  if ( gHyp_parse_isVariableDec ( pParse ) ) {

    if ( gHyp_stack_depth ( pStack ) > 0 ) {

      pData = gHyp_stack_peek ( pStack ) ; 
      tokenType = gHyp_data_tokenType ( pData ) ;
      if ( tokenType == TOKEN_UNIDENTIFIED ||
	   tokenType == TOKEN_REFERENCE ||
	   tokenType == TOKEN_VARIABLE ) return TRUE ;
    }
  }
  return FALSE ;
} 
*/

sData *gHyp_type_assign ( sInstance *pAI,
			  sFrame *pFrame,
			  sData *pDstData,
			  sData *pSrcData,
			  sBYTE dstDataType,
			  sLOGICAL isSubVariable,
			  sLOGICAL freeSrcDataOnError )
{

  sHyp
    *pHyp ;

  char
    c,
    variable[VALUE_SIZE+1],
    *pStream,
    *pStreamEnd,
    buffer[MAX_BUFFER_SIZE+1],
    value[MAX_INPUT_LENGTH+1] ;

  sData
    *pSourceData=NULL,
    *pResult=NULL,
    *pVariable=NULL,
    *pValue,
    *pValue2 ;

  sBYTE
    tokenType,
    tokenType1,
    dataType1,
    srcDataType ;

  sLOGICAL
    isSrcDynamic=TRUE,
    isDstDynamic=TRUE,
    bool1,
    isTyped = ( dstDataType != TYPE_NULL ),
    isVectorSrc=FALSE,
    isVectorOldDst=FALSE,
    isVectorNewDst=FALSE ;

  int
    i,
    n,
    streamLen,
    int1,
    ssd,
    sss=-1,
    oldSize=-1,
    newSize=-1,
    copySize=1,
    hypIndex,
    context ;

  unsigned long
    ulong1 ;

  double
    double1 ;

  void
    *handle1 ;

  /* This routine handles all assignment operations; top-level variables,
   * sub-variables, and intermediate stack results.  It also handles both list
   * and vectors, which can be initialized from either lists or vectors.
   *
   * Here are the cases:
   *
   * 1. For Top-Level-Variables, the result will be be a REFERENCE to the 
   *    VARIABLE which contains the assigned data.
   * 2. For Sub-Variables, the result will be a temporary variable "_TMP_" 
   *    which contains the SUB-VARIABLE which contains the assigned data.
   * 3. For Stack-Variables, the result will be a temporary variable "_TMP"
   *    which contains the assigned data.
   */

  /* If there is source data, is it a vector source ? */
  if ( pSrcData ) {
    isVectorSrc = ( (gHyp_data_getTokenType ( pSrcData ) == TOKEN_VARIABLE || 
                     gHyp_data_getTokenType ( pSrcData ) == TOKEN_CONSTANT ) &&
		    gHyp_data_getDataType ( pSrcData ) > TYPE_STRING ) ;
    sss = gHyp_data_getSubScript ( pSrcData ) ;
    isSrcDynamic = gHyp_data_isDynamic ( pSrcData ) ;
    context = -1 ;
  }

  if ( pDstData == NULL ) {

    /* Conversion only. Result will be stored a temporary stack variable. */
    if ( dstDataType == TYPE_LIST ) {

      /* List result, Convert all values of source to "list" */
      pResult = gHyp_data_new ( "_tmp_" ) ;
      pValue = NULL ;
      srcDataType = gHyp_data_getDataType (pSrcData ) ;
      while ((pValue = gHyp_data_nextValue ( pSrcData, 
					     pValue, 
					     &context,
					     sss ))) {
	if ( isVectorSrc ) {
      
	  pValue2 = gHyp_data_new ( NULL ) ;
	  gHyp_data_newConstant ( pValue2, 
				  srcDataType, 
				  pSrcData, 
				  context );
	}
	else if ( isTyped && gHyp_data_dataType ( pValue ) == TYPE_STRING ) {
	  pValue2 = gHyp_data_new ( NULL ) ;
          n = gHyp_data_getStr ( pValue,
				 value,
				 MAX_INPUT_LENGTH,
				 context,
				 isVectorSrc ) ;
	  gHyp_data_setToken ( pValue2, value ) ;
	}
	else
	  pValue2 = gHyp_data_copyAll ( pValue ) ;

	gHyp_data_append ( pResult, pValue2 ) ;
     }
     if ( context== -2 && sss != -1 ) {
	gHyp_data_delete ( pResult ) ;
	strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
	if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Subscript '%d' is out of bounds in %s",
			      sss,
			      variable ) ;
      }
    }
    else if ( dstDataType == TYPE_STRING ) {
      
      /* String result. Convert all values of source to "string".
       * Special case when source is TYPE_CHAR or TYPE_UCHAR or TYPE_ATTR or TYPE_UNICODE
       * Convert vector of characters to single string.
       */

      /* Change data type of "_tmp_" to TYPE_STRING */
      pResult = gHyp_data_new ( NULL ) ;
      gHyp_data_setVariable ( pResult, "_str_", TYPE_STRING ) ;
      pValue = NULL ;
      srcDataType = gHyp_data_getDataType (pSrcData ) ;
      if ( srcDataType == TYPE_CHAR || srcDataType == TYPE_UCHAR || 
	   srcDataType == TYPE_ATTR || srcDataType == TYPE_UNICODE ) {
	do {
	  i = 0 ;
	  value[0] = '\0' ;
	  while ( i < VALUE_SIZE &&
		  (pValue = gHyp_data_nextValue ( pSrcData, 
					       pValue, 
					       &context,
					       sss )) ) {

	    c = (char) gHyp_data_getRaw ( pValue,
					context,
					isVectorSrc ) ;
	    value[i++] = c ;
	    /* So that conversion into strings break the string in nice sections,
	     * detect \n in the stream and start a new string segment after each one.
	     */
	    if ( c == '\n' ) break ;
	  }
	  if ( context == -2 && sss != -1 ) {
	    gHyp_data_delete( pResult ) ;
	    pResult = NULL ;
	    strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
    	    if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	    gHyp_instance_error ( pAI, STATUS_BOUNDS, 
	 			  "Subscript '%d' is out of bounds in %s",
				  sss,
				  variable) ;
	  }
	
	  value[i] = '\0' ;
	  if ( i > 0 ) {
	    pValue2 = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr_n ( pValue2, value, i ) ;
	    gHyp_data_append ( pResult, pValue2 ) ;
	  } 
	}
	while ( pValue != NULL ) ;
      }
      else {

	/* Not TYPE_CHAR or TYPE_UCHAR or TYPE_ATTR or TYPE_UNICODE */
	while ((pValue = gHyp_data_nextValue ( pSrcData, 
					       pValue, 
					       &context,
					       sss ))) {
	  n = gHyp_data_getStr ( pValue,
				 buffer,
				 MAX_INPUT_LENGTH,
				 context,
				 isVectorSrc ) ;
	  pValue2 = gHyp_data_new ( NULL ) ;
	  gHyp_data_setStr_n ( pValue2, buffer, n ) ;
	  gHyp_data_append ( pResult, pValue2 ) ;
	}
	if ( context== -2 && sss != -1 ) {
	  gHyp_data_delete ( pResult ) ;
	  pResult = NULL ;
	  strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
	  if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	  gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				"Subscript '%d' is out of bounds in %s",
				sss,
				variable) ;
	}
      }
    }
    else {
      
      /* Vector result. Convert all values of source into 'dstDataType' vector 
       * Special case when source is TYPE_STRING or TYPE_LIST and 
       * destination is TYPE_CHAR or TYPE_UCHAR or TYPE_ATTR or TYPE_UNICODE,
       * convert string to array of characters.
       */
      if ( sss == -1 || isVectorSrc )
	srcDataType = gHyp_data_getDataType ( pSrcData ) ;
      else
	srcDataType = gHyp_data_dataType ( 
			gHyp_data_getValue( pSrcData, sss, TRUE )) ;
	
      pResult = gHyp_data_new ( "_vector_" ) ;
      pValue = NULL ;

      if ( ( srcDataType == TYPE_STRING || srcDataType == TYPE_LIST ) && 
	   ( dstDataType == TYPE_CHAR || dstDataType == TYPE_UCHAR || 
	   dstDataType == TYPE_ATTR || dstDataType == TYPE_UNICODE ) ) {

	/* First append all the strings together */
	streamLen = MAX_INPUT_LENGTH ;
	pStream = (char*) AllocMemory ( sizeof ( char ) * streamLen ) ;
	assert ( pStream ) ;
	pStreamEnd = pStream + streamLen ;
	n = 0 ;
	*pStream = '\0' ;
	while ( (pValue = gHyp_data_nextValue ( pSrcData, 
						pValue,
						&context,
						sss )) ) {

	  gHyp_data_getAll ( pValue, 
			     &tokenType1, 
			     &dataType1, 
			     &int1,
			     &ulong1,
			     &double1, 
			     &bool1, 
			     &handle1,
			     value,
			     &i,
			     MAX_INPUT_LENGTH,
			     context,
			     isVectorSrc ) ;
	  
	    
	  switch ( dataType1 ) {
	  case TYPE_STRING :
	  case TYPE_LIST :
	    /* By how much does pString have to grow. */
	    if ( (pStream + n + i) > pStreamEnd ) { 
	      pStream = (char*) ReAllocMemory ( pStream, streamLen*2 ) ;
	      assert ( pStream ) ;
	      streamLen *= 2 ;
	      pStreamEnd = pStream + streamLen ;
	    }
	    memcpy ( pStream+n, value, i ) ;
	    n += i ;
	    break ;
	    
	  case TYPE_FLOAT :
	  case TYPE_DOUBLE :
	    i = 1 ;
	    if ( (pStream + n + i) > pStreamEnd ) { 
	      pStream = (char*) ReAllocMemory ( pStream, streamLen*2 ) ;
	      assert ( pStream ) ;
	      streamLen *= 2 ;
	      pStreamEnd = pStream + streamLen ;
	    }
	    *(pStream+n) = (sBYTE) (int) double1 ;
	    n++ ;   
	    break ;

	   case TYPE_UNICODE :
	    i = 1 ;
	    if ( (pStream + n + i) > pStreamEnd ) { 
	      pStream = (char*) ReAllocMemory ( pStream, streamLen*2 ) ;
	      assert ( pStream ) ;
	      streamLen *= 2 ;
	      pStreamEnd = pStream + streamLen ;
	    }
	    *(pStream+n) = (sBYTE) (int) ulong1 ;
	    n++ ;   
	    break ;
	  default :
	    i = 1 ;
	    if ( (pStream + n + i) > pStreamEnd ) { 
	      pStream = (char*) ReAllocMemory ( pStream, streamLen*2 ) ;
	      assert ( pStream ) ;
	      streamLen *= 2 ;
	      pStreamEnd = pStream + streamLen ;
	    }
	    *(pStream+n) = (sBYTE) int1 ;
	    n++ ;
	  }
	}
	if ( context== -2 && sss != -1 ) {
	  gHyp_data_delete ( pResult ) ;
	  pResult = NULL ;
	  ReleaseMemory ( pStream ) ;
  	  strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
	  if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	  gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				"Subscript '%d' is out of bounds in %s",
				sss,
				variable) ;
	}
	*(pStream+n) = '\0' ;
	gHyp_data_newVector ( pResult, 
			      dstDataType,
			      n,
			      isSrcDynamic ) ;
	for ( i=0; i<n; i++ ) {
	  c = *(pStream+i) ;
	  gHyp_data_setVectorRaw ( pResult, &c, i ) ;
	} 
	ReleaseMemory ( pStream ) ;

      }
      else {

	/* Standard case */

	if ( sss == -1 ) 
	  n = gHyp_data_getCount ( pSrcData ) ;
	else
	  n = 1 ;

	gHyp_data_newVector ( pResult, 
			      dstDataType,
			      n,
			      isSrcDynamic ) ; 
	i = 0 ;
	while ((pValue = gHyp_data_nextValue ( pSrcData, 
					       pValue, 
					       &context,
					       sss ))) {
	  gHyp_data_setVector ( pResult, pValue, i++, context, isVectorSrc ) ;
	}
	if ( context== -2 && sss != -1 ) {
	  gHyp_data_delete ( pResult ) ;
	  pResult = NULL ;
	  strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
	  if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	  gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				"Subscript '%d' is out of bounds in %s",
				sss,
				variable ) ;
	}
      }
    }
  }
  else {

    /* A new definition or assignment for a variable or sub-variable */
    strcpy ( variable, gHyp_data_getLabel ( pDstData ) ) ;
    ssd = gHyp_data_getSubScript ( pDstData ) ;
    tokenType = gHyp_data_tokenType ( pDstData ) ;

    if ( tokenType == TOKEN_VARIABLE ) {

      /* CASE 1:
       *
       * Sometimes the variable being assigned is a sub-variable, which is
       * contained in the variable called "_sub_".  This occurs just after
       * applying a typecast operator in an expression to initially create
       * the sub-variable.
       * Example:  a = { int b = 0 } ;  
       *    - when 'int' is executed on b, it will create a variable
       *      '_TMP_' with the variable 'b' as its value.
       *    - when '=' is executed to assign 0 to 'b', then '_TMP_'
       *      with the value 'b' will be found on the stack.
       * The next section of code extacts the subvariable from its "_TMP_"
       * parent.  This subvariable becomes the new destination variable.
       * The subvariable will remain on the stack until it is integrated
       * into the parent.
       *
       * CASE 2:
       * 
       * In an expression such as:
       *
       *    *a = value ;
       *
       * the dereference of 'a' may require a "dereference" handler.  The
       * result of the dereference is created as a variable "_xxxxxxxx_"
       * where xxxxxxxx is a randomly generated 8 digit hex number.
       */
      pValue = gHyp_data_extractChildBySS ( pDstData, ssd ) ;
      if ( !pValue ) {
	gHyp_util_logError (  "No variable to assign!" ) ;
	longjmp ( gsJmpStack[0], COND_FATAL ) ;
      }
      else {
	/* The sub-variable is valid.  Re-point the pDstData to the 
	 * temporary sub-variable */
	pDstData = pVariable = pValue ;
	strcpy ( variable, gHyp_data_getLabel ( pVariable ) ) ;
	tokenType = gHyp_data_tokenType ( pVariable ) ;
      }
      /* The following statement should always be false. */
      if ( !isSubVariable ) {
	if ( pSrcData ) 
	  strcpy ( value, gHyp_data_print ( pSrcData ) ) ;
	else
	  strcpy ( value, "" ) ;
	if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;  
	if ( pResult ) gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI, STATUS_EXPRESSION,
			      "Cannot assign '%s' to '%s'",
			      value,
			      gHyp_data_print(pDstData) ) ;
      }
    }
    
    /* Setup the stack result for the sub-variable or the top-level variable */
    if ( isSubVariable ) {

      /* Make sure the tokenType is ok. */
      if ( tokenType == TOKEN_UNIDENTIFIED ||
	   tokenType == TOKEN_VARIABLE ||
	   ( tokenType == TOKEN_REFERENCE &&
	     gHyp_util_isIdentifier ( variable ) ) ||
           ( tokenType == TOKEN_LITERAL &&  /* JSON Variable */
             gHyp_util_isIdentifier ( variable ) )) {
	
	/* Result will be a variable contained in a temp variable */
	pResult = gHyp_data_new ( "_sub_" ) ;
	if ( !pVariable ) pVariable = gHyp_data_new ( variable ) ;
	gHyp_data_append ( pResult, pVariable ) ;
      } 
      else {
	if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	if ( pResult ) gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "'%s' is not a valid sub-variable name",
			      variable ) ;
      }
    }
    else {
      
      /* A top level variable. */
      
      if ( pSrcData == NULL && !gHyp_frame_isGlobalScope ( pFrame ) ) {
      
	/* Variable definition in local scope.
	 * See if the variable already exists in local scope 
	 */
	pVariable = gHyp_frame_findLocalVariable ( pFrame, variable ) ;

        /*
        * When commented out, you cannot change the 'type' of a global sub-variable
        * when running in local mode.  You need to be in global mode.
        * When uncommented, you can not change the 'type' of glocal variables, but it
        * has the undesirable effect of overwriting a global variable when you wish to
        * create a new local variable.
        *
        if ( !pVariable && !pSrcData ) pVariable = gHyp_data_getVariable ( pDstData ) ;
        */
      }
      else {

	/* Either not a new variable definiton or the variable definiton is
	 * in global scope.
	 */
	pVariable = gHyp_data_getVariable ( pDstData ) ;
      }

      /* If not found, but it is an identifier or reference, then create it */
      if ( !pVariable && 
	   ( tokenType == TOKEN_UNIDENTIFIED ||
	     ( tokenType == TOKEN_REFERENCE &&
	       gHyp_util_isIdentifier ( variable ) ) ||
             ( tokenType == TOKEN_LITERAL &&   /* JSON Variable */
               gHyp_util_isIdentifier ( variable ) )
              )) {

	pVariable = gHyp_frame_createVariable ( pAI, pFrame, variable ) ;	
      }
      /* Lastly, try to evaluate pLvalue as an expression.
       * This will be a case like a.b.c = {...}
       * The pDstData must be an unresolved reference.
       */
      if ( !pVariable &&
	   gHyp_data_tokenType ( pDstData ) == TOKEN_REFERENCE ) {
	
	pHyp = gHyp_frame_getHyp ( pFrame ) ;
	hypIndex = gHyp_hyp_getHypCount ( pHyp ) ;
	sprintf ( value, "%s ;", variable ) ;
	pStream = gHyp_load_fromStream ( pAI, pHyp, value, 0 ) ;
	if ( !pStream || *pStream ) {
	  gHyp_hyp_setHypCount ( pHyp, hypIndex ) ;
	  if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	  if ( pResult ) gHyp_data_delete ( pResult ) ;
	  gHyp_instance_error ( pAI,
				STATUS_UNDEFINED,
				"'%s' is not a valid variable name",
				variable ) ;

	}
	
        /*gHyp_util_debug("Cannot resolve variable '%s', attempting dereference ",variable);*/
	gHyp_instance_setDerefHandler ( pAI, hypIndex, pHyp ) ;
	gHyp_frame_setGlobalFlag ( pFrame, FRAME_GLOBAL_DOTCREATE ) ;

	if ( !pSrcData ) {
	  pSrcData = gHyp_data_new ( variable ) ;
	  if ( dstDataType > TYPE_STRING )
	    gHyp_data_newVector ( pSrcData, dstDataType, 1, TRUE ) ;
	}
	if ( pResult ) gHyp_data_delete ( pResult ) ;
	return pSrcData ;
      }
      
      /* Still no variable created?  Then the variable must be wrong. */
      if ( !pVariable ) {
	if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	if ( pResult ) gHyp_data_delete ( pResult ) ;
	gHyp_instance_error ( pAI,
			      STATUS_UNDEFINED,
			      "'%s' is not a valid variable name",
			      variable ) ;
      }

      /* Set a reference to the new variable found/created */
      strcpy ( variable, gHyp_data_getLabel ( pVariable ) ) ;
      pResult = gHyp_data_new ( NULL ) ;
      gHyp_data_setReference ( pResult, variable, pVariable ) ;
    }

    /* Do we need to copy over any objects, like a method definition? */
    if ( gHyp_data_getTokenType ( pDstData ) != TOKEN_VARIABLE )
      gHyp_data_copyObject ( pVariable, pDstData ) ;

    /* Is the current variable a vector? */
    isVectorOldDst = ( gHyp_data_getDataType(pVariable) > TYPE_STRING ) ; 

    /* Will the variable be a vector after this operation? */
    if (dstDataType == TYPE_NULL) 
      dstDataType = gHyp_data_getDataType(pVariable) ;

    isVectorNewDst = ( dstDataType > TYPE_STRING ) ;

    /* Determine what sizing needs to be done to the variable */
    isDstDynamic = gHyp_data_isDynamic ( pVariable ) ;

    /* Get current size value for the variable */
    oldSize = gHyp_data_getCount ( pVariable ) ;

    if ( pSrcData ) {

      /* The source count determines resultant array size. */
      if ( sss == -1 )
	/* Copy all values */
	copySize = gHyp_data_getCount ( pSrcData ) ;
      else
	/* Copy a single value */
	copySize = 1 ;

    }
    else
      /* Copy size == current size of variable */
      copySize = gHyp_data_getCount ( pVariable ) ;

    if ( isTyped ) {

      /* Variable or sub-variable is explicitly typed.
       * 
       * Cases: 
       *	<type> a ;
       *	<type> a = {...} ;
       *	<type> a[n] ;
       *	<type> a[n] = { ... } ;
       */

      if ( ssd == -1 ) {

	/* Subscript was not specified. (No specific size was specified) */

	if ( !isDstDynamic )
	  /* Current vector already has a fixed size. Keep it */
	  newSize = oldSize ;
	else {
	  /* Let size be dynamic */
	  /* If a vector and zero size, make it size = 1 */
	  if ( dstDataType > TYPE_STRING && copySize == 0 && !pSrcData )
	    copySize = 1 ;
	  newSize = copySize ;
	}
      }
      else {
	
	/* Type and size were explicity specified. 
	 * Even if it was previously fixed, this new subscript overrides it.
	 */
	isDstDynamic = FALSE ;
	newSize = ssd ;
      }

      /* Set ssd to one so that all values will be replaced */
      ssd = -1 ;
    } 
    else {

      /* Variable or sub-variable was not explicitly typed.
       * 
       * Cases: 
       *	a ;     - This case does not invoke this subroutine
       *	a = {...} ; This is basic list assignment.
       *	a[n] ;  - This case does not invoke this subroutine
       *	a[n] = {...} ; This is subscript assignment. 
       */
      
      /* The new data type will be determined by the existing datatype */
      if ( ssd == -1 ) { 

	/* Subscript was not specified. Unspecified destination size. */
	if ( !isDstDynamic )
	  /* Current variable has a fixed size */
	  newSize = oldSize ;
	else  
	  newSize = copySize ;
      }
      else {

	/* Subscript assignment.  Size is unchanged */
	newSize = oldSize ;
      }

    }

    if ( pSrcData && isDstDynamic ) 
      /* Make sure we don't copy more than the fixed size */
      copySize = MIN ( copySize, newSize ) ;
    else
      copySize = newSize ;

    if ( ssd == -1 ) {
   
      /* Replace or assign all values */

      if ( dstDataType < TYPE_BYTE ) {	

	/* List or string result. */

	/* If there's no source data and the destination data is typed,
	 * then convert itself to the new type
	 */
	if ( !pSrcData && isTyped ) {
	  pSrcData = pSourceData = gHyp_data_copyAll ( pVariable );
	  isVectorSrc = isVectorOldDst ;
	} 
	
	if ( pSrcData ) {

	  /* Replace all values */
	  gHyp_data_deleteValues ( pVariable ) ;

	  /* Make sure its a list again (in case it was a vector)
	   * and that it has the correct data type
	   */
	  gHyp_data_setVariable ( pVariable, variable, dstDataType ) ;
	  /*
	  gHyp_util_logInfo("%p(%s)setVariableStrFor(%s)",
				  pVariable,variable,
				  gHyp_data_getLabel(pVariable));

	  */

	  /* Set the size, -1 (dynamic) or fixed size list */
	  gHyp_data_setSize ( pVariable, newSize ) ;
	  gHyp_data_setDynamic ( pVariable, isDstDynamic ) ;
	  
	  /* Special case when destination is TYPE_STRING and the
	   * source is TYPE_CHAR or TYPE_UCHAR or TYPE_ATTR or TYPE_UNICODE, convert
	   * vector of characters to single string.
	   */
	  if ( sss == -1 || isVectorSrc )
	    srcDataType = gHyp_data_getDataType ( pSrcData ) ;
	  else {

	    if ( !(pValue = gHyp_data_getValue( pSrcData, sss, TRUE )) ) {
	      if ( pSourceData ) gHyp_data_delete ( pSourceData ) ;
	      strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
              if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	      if ( pResult ) gHyp_data_delete ( pResult ) ;
	      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				    "Subscript is out of bounds in %s",
				    variable ) ;
	    }
	    srcDataType = gHyp_data_dataType ( pValue ) ;
	  }

	  if ( dstDataType == TYPE_STRING &&
	       (srcDataType == TYPE_CHAR || srcDataType == TYPE_UCHAR || 
	        srcDataType == TYPE_ATTR || srcDataType == TYPE_UNICODE ) ) {

	    pValue = NULL ;
	    do {
	      i = 0 ;
	      value[0] = '\0' ;
	      /*copySize = MIN ( copySize, VALUE_SIZE ) ;*/
	      while ( i < VALUE_SIZE &&
		      (pValue = gHyp_data_nextValue( pSrcData,
					      pValue,
					      &context,
					      sss ) ) ) { 

		c = (char) gHyp_data_getRaw ( pValue,
					context,
					isVectorSrc ) ;
		value[i++] = c ;
		/* Detect \n and start a new string segment */
		if ( c == '\n' ) break ;
	      }
	      if ( context== -2 && sss != -1 ) {
		if ( pResult  ) gHyp_data_delete ( pResult  ) ;	      
		if ( pSourceData ) gHyp_data_delete ( pSourceData ) ;	 
		strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
 	  	if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
		gHyp_instance_error ( 
		     pAI, STATUS_BOUNDS, 
		      "Subscript '%d' is out of bounds in %s",
		      sss,
		      variable) ;
	      }

	      value[i] = '\0' ;
	      if ( i > 0 ) {
	        pValue2 = gHyp_data_new ( NULL ) ;
	        gHyp_data_setStr_n ( pValue2, value, i ) ;
	        gHyp_data_append ( pVariable, pValue2 ) ;
	      }
	    }
	    while ( pValue != NULL ) ;
	  }
	  else {

	    /* Copy (or convert) the values */
	    pValue = NULL ;
	    i = 0 ;
	    for ( i=0; i<copySize; i++ ) {

	      if ( i == 0 || pValue ) {
		pValue = gHyp_data_nextValue ( pSrcData,
					       pValue,
					       &context,
					       sss ) ;

		if ( context== -2 && sss != -1 ) {

		  if ( pResult  ) gHyp_data_delete ( pResult  ) ;	      
		  if ( pSourceData ) gHyp_data_delete ( pSourceData ) ;
	          strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
  	  	  if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
		  gHyp_instance_error ( 
		    pAI, STATUS_BOUNDS, 
		    "Subscript '%d' is out of bounds in %s",
		    sss,
		    variable) ;
		}

	      }

	      if ( pValue ) {
		
		if ( dstDataType == TYPE_STRING ) {
		  /* Convert element to string */
		  n = gHyp_data_getStr ( pValue,
					 buffer,
					 MAX_INPUT_LENGTH,
					 context,
					 isVectorSrc ) ;
		  pValue2 = gHyp_data_new ( NULL ) ;
		  gHyp_data_setStr_n ( pValue2, buffer, n ) ;
		}
		/*
		else if ( srcDataType == TYPE_STRING ) {
		  * list result, string source - evaluate *
		  n = gHyp_data_getStr ( pValue,
					 value,
					 VALUE_SIZE,
					 context,
					 isVectorSrc ) ;
		  pValue2 = gHyp_data_new ( NULL ) ;
		  gHyp_data_setToken ( pValue2, value ) ;
		}
	        */
		else if ( isVectorSrc ) {
		  /* vector source. Make new constant */
		  pValue2 = gHyp_data_new ( NULL ) ;
		  gHyp_data_newConstant ( pValue2, 
					  srcDataType,
					  pSrcData,  
					  context ) ;
		}
		else {
		  /* List source. Copy exact */
		  pValue2 = gHyp_data_copyAll ( pValue ) ;
		  /* Do we need to copy over any objects, like a method definition? */
		  gHyp_data_copyObject ( pValue2, pValue ) ;
		}
	      }
	      else {
		pValue2 = gHyp_data_new ( NULL ) ;
		gHyp_data_setInt ( pValue2, 0 ) ;
	      }
	      gHyp_data_append ( pVariable, pValue2 ) ;
	    }
	  }
	}
      }
      else {
	
        /* Vector result. Convert values of source to 'dstDataType' vector */
	if ( pSrcData ) {
	  if ( !isVectorOldDst || (newSize != oldSize) ) 
	    gHyp_data_newVector ( pVariable, 
				  dstDataType,
				  newSize,
				  isDstDynamic );
	}	  
	else {
	  pSrcData = pSourceData = gHyp_data_copyAll ( pVariable ) ;
	  isVectorSrc = isVectorOldDst ;
	  gHyp_data_newVector ( pVariable, 
				dstDataType,
				newSize,
				isDstDynamic );
	  /*
	  gHyp_util_logInfo("%p(%s)new-int-vector(dereference)",
			    pVariable,gHyp_data_getLabel(pVariable));
	  */
	}

	/* Special case when source is TYPE_STRING and destination is
	 * TYPE_CHAR or TYPE_UCHAR or TYPE_ATTR or TYPE_UNICODE:
	 * Convert string to array of characters.
	 */
	if ( sss == -1 || isVectorSrc )
	  srcDataType = gHyp_data_getDataType ( pSrcData ) ;
	else
	  srcDataType = gHyp_data_dataType ( 
			gHyp_data_getValue( pSrcData, sss, TRUE )) ;

	if ( ( srcDataType == TYPE_STRING || srcDataType == TYPE_LIST ) && 
	     ( dstDataType == TYPE_CHAR || dstDataType == TYPE_UCHAR || 
	       dstDataType == TYPE_ATTR || dstDataType == TYPE_UNICODE ) ) {

	  /* First append all the strings together */
	  streamLen = MAX_INPUT_LENGTH ;
	  pStream = (char*) AllocMemory ( sizeof ( char ) * streamLen ) ;
	  assert ( pStream ) ;
	  pStreamEnd = pStream + streamLen ;
	  n = 0 ;
	  *pStream = '\0' ;
	  pValue = NULL ;
	  while ( (pValue = gHyp_data_nextValue ( pSrcData,
						  pValue,
						  &context,
						  sss ))) {
	    
	    gHyp_data_getAll ( pValue, 
			       &tokenType1, 
			       &dataType1, 
			       &int1, 
			       &ulong1,
			       &double1, 
			       &bool1, 
			       &handle1,
			       value, 
			       &i,
			       MAX_INPUT_LENGTH,
			       context,
			       isVectorSrc ) ;
	    
	    switch ( dataType1 ) {
	    case TYPE_STRING :
	    case TYPE_LIST :
	      if ( (pStream + n + i) > pStreamEnd ) { 
		pStream = (char*) ReAllocMemory ( pStream, streamLen*2 ) ;
		assert ( pStream ) ;
		streamLen *= 2 ;
		pStreamEnd = pStream + streamLen ;
	      }
	      memcpy ( pStream+n, value, i ) ;
	      n += i ;
	      break ;
	      
	    case TYPE_FLOAT :
	    case TYPE_DOUBLE :
	      i = 1 ;
	      if ( (pStream + n + i) > pStreamEnd ) { 
		pStream = (char*) ReAllocMemory ( pStream, streamLen*2 ) ;
		assert ( pStream ) ;
		streamLen *= 2 ;
		pStreamEnd = pStream + streamLen ;
	      }
	      *(pStream+n) = (sBYTE) (int) double1 ;
	      n++ ;    
	      break ;
	    case TYPE_UNICODE :
	      i = 1 ;
	      if ( (pStream + n + i) > pStreamEnd ) { 
		pStream = (char*) ReAllocMemory ( pStream, streamLen*2 ) ;
		assert ( pStream ) ;
		streamLen *= 2 ;
		pStreamEnd = pStream + streamLen ;
	      }
	      *(pStream+n) = (sBYTE) (int) ulong1 ;
	      n++ ;    
	      break ;

	    
	    default :
	      i = 1 ;
	      if ( (pStream + n + i) > pStreamEnd ) { 
		pStream = (char*) ReAllocMemory ( pStream, streamLen*2 ) ;
		assert ( pStream ) ;
		streamLen *= 2 ;
		pStreamEnd = pStream + streamLen ;
	      }
	      *(pStream+n) = (sBYTE) int1 ;
	      n++ ;
	    }
	  }
	  if ( context== -2 && sss != -1 ) {
	    if ( pResult  ) gHyp_data_delete ( pResult  ) ;	      
	    if ( pSourceData ) gHyp_data_delete ( pSourceData ) ;
	    strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
      	    if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	    ReleaseMemory ( pStream ) ;
	    gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				  "Subscript '%d' is out of bounds in %s",
				  sss,
				  variable) ;
	  }

	  /* If dynamic, adjust array size */
	  if ( isDstDynamic ) newSize = n ;
	  gHyp_data_newVector ( pVariable, 
				dstDataType,
				newSize,
				isDstDynamic ) ;

	  /* Copy in the values */
	  for ( i=0; i<MIN(newSize,n); i++ ) {
	    c = *(pStream+i) ;
	    gHyp_data_setVectorRaw ( pVariable, &c, i ) ;
	  }
	  ReleaseMemory ( pStream ) ;

	  /* Pad with spaces if necessary */
	  c = ' ' ;
	  for ( i=n; i<newSize; i++ )
	    gHyp_data_setVectorRaw ( pVariable, &c, i ) ;

	}
	else {
	  
	  pValue = NULL ;
	  i = 0 ;
	  while ( (pValue = gHyp_data_nextValue ( pSrcData,
						  pValue,
						  &context,
						  sss ))&&
		  i < copySize ) {
	    gHyp_data_setVector ( pVariable,pValue,i++,context,isVectorSrc);
	  }
	  if ( context== -2 && sss != -1 ) {
	    if ( pResult  ) gHyp_data_delete ( pResult  ) ;	      
	    if ( pSourceData ) gHyp_data_delete ( pSourceData ) ;
	    strcpy ( variable, gHyp_data_getLabel(pSrcData) ) ;
    	    if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	    gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				  "Subscript '%d' is out of bounds in %s",
				  sss,
				  variable) ;
	  }
	}
      }

      /* If source data was made up, then delete it */
      if ( pSourceData ) gHyp_data_delete ( pSourceData ) ;
    
    }
    else {

      /* Replace the subscripted element with a single rValue */

      /* Check to see if it is in range.. */
      if ( !(pValue = gHyp_data_getValue ( pVariable, ssd, TRUE ) ) ) {
        if ( freeSrcDataOnError && pSrcData ) gHyp_data_delete ( pSrcData ) ;
	if ( pResult  ) gHyp_data_delete ( pResult  ) ;
	gHyp_instance_error ( pAI,
			      STATUS_BOUNDS,
			      "Subscript '%d' to '%s' is out of range",
			      ssd,variable ) ;
      }
      
      /* Overwrite the subscripted value in the variable. */
      /* Do the assignment */
      if ( dstDataType == TYPE_LIST ) {
	
	srcDataType = gHyp_data_getDataType (pSrcData) ;
	if ( isVectorSrc ) {
      
	  gHyp_data_newConstant ( pValue,
				  srcDataType,
				  pSrcData,
				  sss ) ;
	}
	else if ( srcDataType == TYPE_STRING ) {

	  n = gHyp_data_getStr ( pSrcData, 
				 buffer, 
				 MAX_INPUT_LENGTH,
				 sss,
				 TRUE ) ;
	  gHyp_data_setStr_n ( pValue, buffer, n ) ;
	}
	else {
	  n = gHyp_data_getStr ( pSrcData, 
				 value, 
				 MAX_INPUT_LENGTH,
				 sss,
				 TRUE ) ;
	  gHyp_data_setToken ( pValue, value ) ;
	}
      }
      else if ( dstDataType == TYPE_STRING ) {
	
	/* String result. Replace single value. */
	n = gHyp_data_getStr ( pSrcData, 
			       buffer, 
			       MAX_INPUT_LENGTH,
			       sss,
			       TRUE ) ;
	gHyp_data_setStr_n ( pValue, buffer, n ) ;
      }
      else {
	
	/* Vector result. Convert value of source to 'dstDataType' vector */
	gHyp_data_setVector ( pVariable, pSrcData, ssd, sss, TRUE ) ;
      }
      
      /* Create the stack result, but only with the subscripted value. */ 
      gHyp_data_setSubScript ( pResult, ssd ) ;
    }
  }
  
  /*gHyp_util_debug ( "assign returning %s", gHyp_data_print ( pResult ) ) ;*/
   
  return pResult ;
}


void gHyp_type_int ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: int ( value )
   *	Returns the integer representation of value, otherwise returns 0.
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
   *
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: int ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_INTEGER,
				   isSubVariable, FALSE  ) ;
    }
    else {
      
      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_INTEGER,
				   FALSE, FALSE  ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_float ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: float ( value )
   *	Returns the float representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: float ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_FLOAT,
				   isSubVariable, FALSE  ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_FLOAT,
				   FALSE, FALSE  ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_str ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: str ( value )
   *	Returns the string representation of value, otherwise returns 0.
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
 
    sData
      *pRdata,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: str ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_STRING,
				   isSubVariable, FALSE  ) ;
    }
    else {

      /* It is a typecast conversion. */

      /* Why is this popRdata instead of popRvalue?
       * All the other typecast conversions are this way, why not this one?
       *
       * SHOULD BE? -> pRdata = gHyp_stack_popRvalue ( pStack, pAI ) ;
       */
      pRdata = gHyp_stack_popRdata ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRdata,
				   TYPE_STRING,
				   FALSE, FALSE  ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_type_short ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: short ( value )
   *	Returns the short representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: short ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_SHORT,
				   isSubVariable, FALSE  ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_SHORT,
				   FALSE, FALSE  ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_ushort ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: ushort ( value )
   *	Returns the unsigned short representation of value, otherwise 
   *	returns 0.
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
   *
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;			


    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: ushort ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_USHORT,
				   isSubVariable, FALSE  ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_USHORT,
				   FALSE, FALSE  ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_long ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: long ( value )
   *	Returns the long representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: long ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_LONG,
				   isSubVariable, FALSE  ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_LONG,
				   FALSE, FALSE  ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_ulong ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: ulong ( value )
   *	Returns the unsigned long representation of value, otherwise returns 0.
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
   *
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: ulong ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_ULONG,
				   isSubVariable, FALSE  ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_ULONG,
				   FALSE, FALSE  ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_double ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: double ( value )
   *	Returns the double representation of value, otherwise returns 0.
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
   *
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: double ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_DOUBLE,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_DOUBLE,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_byte ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: byte ( value )
   *	Returns the byte representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: byte ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_BYTE,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_BYTE,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_ubyte ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: ubyte ( value )
   *	Returns the unsigned byte representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: ubyte ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_UBYTE,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_UBYTE,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_type_char ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: char ( value )
   *	Returns the char (byte) representation of value, otherwise returns 0.
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
   *
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: char ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_CHAR,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_CHAR,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_type_uchar ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: uchar ( value )
   *	Returns the unsigned char representation of value, otherwise returns 0.
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
   *
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: uchar ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_UCHAR,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_UCHAR,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_type_list ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: list ( value )
   *	Returns the list representation of value, otherwise returns 0.
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
   * Modifications:
   *
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: list ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_LIST,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_LIST,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_type_binary ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: binary ( value )
   *	Returns the unsigned byte representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	    "Invalid arguments. value = binary ( variable/value )" ) ;


    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_BINARY,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_BINARY,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}
void gHyp_type_boolean ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: boolean ( value )
   *	Returns the unsigned byte representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: boolean ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
       isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_BOOLEAN,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_BOOLEAN,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_hex ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: hex ( value )
   *	Returns the unsigned byte representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: hex ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_HEX,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_HEX,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_handle ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: handle ( value )
   *	Returns the void* representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: handle ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_HANDLE,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_HANDLE,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_octal ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: octal ( value )
   *	Returns the unsigned byte representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: octal ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_OCTAL,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_OCTAL,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_unicode ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: unicode ( value )
   *	Returns the unsigned byte representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: unicode ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_UNICODE,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_UNICODE,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_attr ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: attr ( value )
   *	Returns the unsigned byte representation of value, otherwise returns 0.
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
 
    sData
      *pRvalue,
      *pLvalue,
      *pResult ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isSubVariable ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: attr ( value )" ) ;
     

    /* Assume just a temporary result */

    if ( lHyp_type_isVariableDef ( pParse, pStack ) ) {

      /* It is a variable definition and initialization. */
      isSubVariable = ( gHyp_parse_listDepth ( pParse ) > 0 ||
			gHyp_parse_isMethodArgs  ( pParse ) ) ;
      pLvalue = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   pLvalue,
				   NULL,	/* No assignment data */
				   TYPE_ATTR,
				   isSubVariable, FALSE ) ;
    }
    else {

      /* It is a typecast conversion. */
      pRvalue = gHyp_stack_popRvalue ( pStack, pAI ) ;
      pResult = gHyp_type_assign ( pAI,
				   pFrame,
				   NULL,	/* No assignment variable */
				   pRvalue,
				   TYPE_ATTR,
				   FALSE, FALSE ) ;
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_type_typeof ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: typeof ( variable )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
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
      *pData,
      *pValue,
      *pResult ;

    sBYTE
      tokenType,
      dataType ;

    char
      dataTypeString[10] ;

    int
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: typeof ( variable )" ) ;

    /* Don't use popRdata !!!!
     * i.e.  pData = gHyp_stack_popRdata ( pStack, pAI ) ;
     * if 
     *	list A = { "string", 1, 1.2, 
     *              list a = { "nested literal", 1, 1.2 }
     * If we use Rdata, then  
     *	  typeof ( (*node)[ii] ) 
     * will not work - it will not do sub-scripting...
     * it will return 
     *	  "list".
     * but...
     *	   typeof 1.2 
     * would return "float" as expected, but when we
     * use popLvalue, we get an error!!
     *	   %IDENTIFIER:  '1.2' is not a variable....
     * So, we use Lvalue, then
     *	  typeof valueof "1.2" will create an ERROR.
     * To get around Lvalue error, do this:
     *	  typeof ( ( list tmp={valueof "1.2"} )[0] )
     *	
     */
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    /* Although we expect an lValue, if we get a LITERAL
     * instead, then report the correct data type
     */

    ss = gHyp_data_getSubScript ( pData ) ;
    tokenType = gHyp_data_tokenType ( pData ) ;

    if ( ss >= 0 &&
	 tokenType == TOKEN_REFERENCE &&
	(pValue = gHyp_data_getValue ( pData, ss, TRUE )) != NULL && 
       gHyp_data_getReference ( pData ) != pValue &&
       gHyp_data_tokenType ( pValue ) != TOKEN_VARIABLE ) 
       dataType = gHyp_data_getDataType ( pValue ) ;

    else
      dataType = gHyp_data_getDataType ( pData ) ;


    switch ( dataType ) {

    case		TYPE_NULL :
      strcpy ( dataTypeString, "null" ) ;
      break ;
    case		TYPE_LIST :
      strcpy ( dataTypeString, "list" ) ;
      break ;
    case		TYPE_STRING :
      strcpy ( dataTypeString, "str" ) ;
      break ;
    case		TYPE_BYTE :
      strcpy ( dataTypeString, "byte" ) ;
      break ;
    case		TYPE_UBYTE :		
      strcpy ( dataTypeString, "ubyte" ) ;
      break ;
    case		TYPE_BOOLEAN :            
      strcpy ( dataTypeString, "boolean" ) ;
      break ;
    case		TYPE_BINARY :          
      strcpy ( dataTypeString, "binary" ) ;
      break ;
    case		TYPE_ATTR :		
      strcpy ( dataTypeString, "attr" ) ;
      break ;
    case		TYPE_CHAR :		
      strcpy ( dataTypeString, "char" ) ;
      break ;
    case		TYPE_UCHAR :		
      strcpy ( dataTypeString, "uchar" ) ;
      break ;
    case		TYPE_SHORT :     	
      strcpy ( dataTypeString, "short" ) ;
      break ;
    case		TYPE_USHORT :		
      strcpy ( dataTypeString, "ushort" ) ;
      break ;
    case		TYPE_LONG :	        		
      strcpy ( dataTypeString, "long" ) ;
      break ;
    case		TYPE_ULONG :	        		
      strcpy ( dataTypeString, "ulong" ) ;
      break ;
    case		TYPE_INTEGER :            
      strcpy ( dataTypeString, "int" ) ;
      break ;
    case		TYPE_FLOAT :		
      strcpy ( dataTypeString, "float" ) ;
      break ;
    case		TYPE_DOUBLE :		
      strcpy ( dataTypeString, "double" ) ;
      break ;
    case		TYPE_HEX :               
      strcpy ( dataTypeString, "hex" ) ;
      break ;
    case		TYPE_OCTAL :              
      strcpy ( dataTypeString, "octal" ) ;
      break ;
    case		TYPE_UNICODE :              
      strcpy ( dataTypeString, "unicode" ) ;
      break ;
    case		TYPE_HANDLE :		
      strcpy ( dataTypeString, "handle" ) ;
      break ;
    case		TYPE_DATETIME :		
      strcpy ( dataTypeString, "datetime" ) ;
      break ;
    case		TYPE_RAW :		
      strcpy ( dataTypeString, "raw" ) ;
      break ;
    default:
      strcpy ( dataTypeString, "unknown" ) ;
      break ;
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, dataTypeString ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_valueof(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: valueof()
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
      *pData,
      *pResult ;
    
    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      strVal[MAX_INPUT_LENGTH+1] ;
    

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: valueof ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   strVal, 
			   MAX_INPUT_LENGTH, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pResult = gHyp_data_new ( "_valueof_" ) ;
    gHyp_data_setToken ( pResult, strVal ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_type_scopeof(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: scopeof()
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
      *pVariable,
      *pLocalVariable,
      *pRootVariable,
      *pData,
      *pResult ;
    
    /* local / global */
    char
      *pVarStr,
      scopeString[7] ;

    sLOGICAL
      isGlobalScope ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: scopeof ( variable )" ) ;

    /* Pop the variable off of the stack */
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

    if ( (pVariable = gHyp_data_getVariable ( pData ) ) && pVariable != pData ) { 

      pVarStr = gHyp_data_getLabel ( pVariable ) ;

      pRootVariable = gHyp_frame_findRootVariable ( pFrame, pVarStr ) ;
      pLocalVariable = gHyp_frame_findLocalVariable ( pFrame, pVarStr ) ;
      isGlobalScope = gHyp_frame_isGlobalScope ( pFrame ) ;

      /*gHyp_util_debug( "G=%p L=%p GS=%u",pRootVariable,pLocalVariable,isGlobalScope);*/
      if ( pLocalVariable != NULL ) {
	/* Local variable is defined - must be local */
	strcpy ( scopeString, "local" ) ;	
      }
      else {
	strcpy ( scopeString, "global" ) ;
      }
    }
    else {
      gHyp_instance_error ( pAI, 
			      STATUS_UNDEFINED,
			      "Variable '%s' does not exist",
			      gHyp_data_getLabel ( pData ) ) ; 
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, scopeString ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}
