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
 *	$Log: fileio.c,v $
 *	Revision 1.18  2004/07/01 02:02:34  bergsma
 *	Added 'specialChars' argument to the toexternal() function.
 *	
 *	Revision 1.17  2004/05/15 02:09:59  bergsma
 *	In openvms, fread fails to detect EOF, need stat to pre-get size.
 *	
 *	Revision 1.16  2004/04/29 08:32:40  bergsma
 *	Added functions frwite,fread,load_binary
 *	
 *	Revision 1.15  2003/07/27 00:54:24  bergsma
 *	Remove ^M
 *	
 *	Revision 1.14  2003/07/27 00:53:27  bergsma
 *	In fgets(), remove newline/carriageReturn at end of line only in PROMIS
 *	
 *	Revision 1.13  2003/06/05 21:54:23  bergsma
 *	Added new typedef 'unicode', UTF-16.
 *	Required for new unicode type:
 *	1. New data type in data object
 *	2. Special unicode output for xdescribe function
 *	3. New TYPE_UNICODE in hypdef.h
 *	4. New token 0uNNNNN format, like 0xNNNNN and 0oNNNNN
 *	5. New function unicode()
 *	6. New SECS II data type.
 *	
 *	Revision 1.12  2003/04/23 22:47:42  bergsma
 *	In load(), an extra argument specifies the maximum number of lines to load.
 *	In sdescribe() functions, extra spaces are removed.
 *	
 *	Revision 1.11  2003/04/11 14:20:11  bergsma
 *	When xdescribing PRE or SCRIPT, do not expand double quotes to \".
 *	
 *	Revision 1.10  2003/04/11 01:59:34  bergsma
 *	When describing a literal, the literal must be in external format, and depending on whether
 *	or not the literal is for XML, a message, or normal.
 *	
 *	Revision 1.9  2003/04/04 16:47:26  bergsma
 *	When outputing <b>, <i>, etc... do not start new lines when describing.
 *	
 *	Revision 1.8  2003/01/14 15:53:24  bergsma
 *	Use 'ending' variable in arg list rather than expression.
 *	
 *	Revision 1.7  2003/01/12 13:45:37  bergsma
 *	V3.1.0
 *	1. Incorporate instance_pushSTATUS in functions.
 *	2. Fix error reporting typedef in describe, missing "hex"
 *	3. In describe, compact result if for a message.
 *	4. In describe, allow ending to be ";", ",", or " ".
 *	5. Make unlink() function subject to AS_RESTRICTED compile option
 *	
 *	Revision 1.6  2002/11/29 19:52:10  bergsma
 *	1. Improved handling for <script> <style> <pre> and <textarea> tags when
 *	doing an xdescribe function.
 *	2. Added load() function.
 *	
 *	Revision 1.5  2002/11/21 23:50:57  bergsma
 *	When describing XML with script (javascript) tags, do not interpret data
 *	inside the tag.
 *	
 *	Revision 1.4  2002/09/16 18:01:38  bergsma
 *	Add isMSG argument to lHyp_fileio_describe and lHyp_fileio_describe2.  When
 *	set to true, the gHyp_util_unparseString will convert "|" to "\174".
 *	Currently used in gHyp_fileio_describeData().
 *	
 *	Revision 1.3  2002/09/16 15:23:44  bergsma
 *	Added gHyp_fileio_describeData, used by query() and event() functions,
 *	when the argument style is new. See route.c
 *	
 *	Revision 1.2  2002/09/03 21:21:43  bergsma
 *	Increased size of buffer to handle expansion of characters to XML '&' format
 *	
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include <sys/stat.h>

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/
static char *gzaType[23] = { "null",	/* 0 */
			     "list",	/* 1 */
			     "str",	/* 2 */
			     "byte",	
			     "ubyte",
			     "boolean",
			     "binary",
			     "char",	
			     "uchar",	
			     "attr",
			     "short",	
			     "ushort",
			     "long",
			     "ulong",
			     "int",
			     "float",
			     "double",
			     "hex",
			     "octal",
			     "unicode",
			     "handle",
			     "datetime",
			     "raw"	  /* 22 */
} ;

/**********************	FUNCTION DEFINITIONS ********************************/

#if defined (AS_VMS) && defined ( __cplusplus )
extern "C" int unlink ( const char * ) ;
#else
extern int unlink ( const char * ) ;
#endif



static int lHyp_fileio_describe2 ( sData *pParent, 
				   int parentContext,
				   sLOGICAL firstValue,
				   sLOGICAL lastValue,
				   int indent,
				   char *pOutput,
				   FILE *fp,
				   sLOGICAL isXML,
				   sLOGICAL isPRE,
				   sLOGICAL isTXT,
				   sLOGICAL isMSG,
				   sLOGICAL isSCR,
				   sLOGICAL isMRK,
				   sLOGICAL isAttrOn,
				   char ending,
				   sData *pResult,
				   int *pContentLength ) 
{
  sData
    *pData ;

  int
    n,
    i,
    outputLen,
    outLen,
    newOffset=0,
    tmpOutputLen=0,
    size,
    ss,
    context,
    numNonAttributes=0 ;

  char
    c,
    newOutput[MAX_OUTPUT_LENGTH+1],
    tmpOutput[MAX_OUTPUT_LENGTH+1],
    prefixStr[VALUE_SIZE+1],
    suffixStr[VALUE_SIZE+1],
    spacer[2] = {0,0},
    value[VALUE_SIZE+1],
    value2[VALUE_SIZE+1],
    tag[TOKEN_SIZE+1],
    *pOut,
    *pValue ;

  sBYTE
    dataType,
    tokenType,
    parentDataType,
    parentTokenType ;

  sLOGICAL
    firstVal,
    lastVal,
    noValue,
    isParentDynamic,
    isParentVector = FALSE ;

  sData
    *pLine ;

  outputLen = strlen ( pOutput ) ;

  for ( i=0; i<indent; i++ ) prefixStr[i] = ' ' ;
  /* Calculate indent */
  prefixStr[indent] = '\0' ;

  if ( indent > 0 ) {
    if ( lastValue ) 
      strcpy ( suffixStr, "" ) ;
    else {
      if ( isXML )
	strcpy ( suffixStr, " " ) ;
      else
	strcpy ( suffixStr, "," ) ;
    }
  }
  else {
    if ( lastValue ) {
      suffixStr[0] = ending ; suffixStr[1] = '\0' ;
    }
  }

  /* Print out information on data being described */
  parentTokenType = gHyp_data_tokenType ( pParent ) ;
  parentDataType = gHyp_data_dataType ( pParent ) ;

  if ( parentTokenType != TOKEN_VARIABLE ) {
    /* Output what is there and start a new line */ 
    if ( outputLen > indent ) {
      pOut = ( isPRE || isTXT ) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
      outLen = strlen ( pOut ) ;
      if ( !isMRK ) {
        if ( fp )
	  fprintf ( fp, "%s\n", pOut ) ;
        else if ( pResult ) {
	  pLine = gHyp_data_new ( NULL ) ;
	  gHyp_data_setStr ( pLine, pOut ) ;
	  gHyp_data_append ( pResult, pLine ) ;
	}
        else
	  gHyp_util_log ( pOut ) ;
	*pContentLength += outLen ;
        *pOutput = '\0' ;
        if ( !isMSG && !pResult ) strcat ( pOutput, prefixStr ) ;
        outputLen = strlen ( pOutput ) ;
      }
    }
  }

  switch ( parentTokenType ) {

  case TOKEN_REFERENCE :
    
    if ( isXML )
      newOffset = sprintf ( newOutput,
			    "%s%s",  
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;
    else {

      if ( !isMSG )
        newOffset = sprintf ( newOutput,
			    "&'%s'%s",
			    gHyp_data_getLabel ( pParent ),
    			    suffixStr ) ;
      else {
	strcpy ( value, gHyp_data_getLabel( pParent ) ) ;
	n = gHyp_util_unparseString ( value2, value, strlen(value), VALUE_SIZE, TRUE, FALSE, "'" ) ;
        newOffset = sprintf ( newOutput,
			    "&'%s'%s",
			    value2,
    			    suffixStr ) ;

      }
    }
    break ;
    
  case TOKEN_LITERAL : 
    
    pValue = value ;
    if ( isMSG )
      n = gHyp_data_getStr_msg ( pParent, 
			      value, 
			      VALUE_SIZE,
			      0,
			      FALSE ) ;

    else if ( isXML ) {
      if ( !isSCR && !isPRE && !isTXT ) 
        n = gHyp_data_getStr_xml ( pParent, 
			      value, 
			      VALUE_SIZE,
			      0,
			      FALSE ) ;

      else 
        n = gHyp_data_getStr_nonulls ( pParent, 
			        value, 
			        VALUE_SIZE,
			        0,
			        FALSE ) ;

    }
    else
      n = gHyp_data_getStr_external ( pParent, 
			      value, 
			      VALUE_SIZE,
			      0,
			      FALSE ) ;

    if ( isXML )
      newOffset = sprintf ( newOutput,
			    "%s",
			    pValue ) ;
    else 
      newOffset = sprintf ( newOutput,
			    "\"%s\"%s",
			    pValue,
			    suffixStr ) ;
    break ;
    
  case TOKEN_UNIDENTIFIED : 
    
    if ( isXML )
      newOffset = sprintf ( newOutput,
			    "<%s>%s", 
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;
    else
      if ( !isMSG ) 
        newOffset = sprintf ( newOutput,
			    "'%s'%s",
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;
      else {
	strcpy ( value, gHyp_data_getLabel( pParent ) ) ;
	n = gHyp_util_unparseString ( value2, value, strlen(value), VALUE_SIZE, TRUE, FALSE, "'" ) ;
        newOffset = sprintf ( newOutput,
			    "&'%s'%s",
			    value2,
    			    suffixStr ) ;

      }

    break ;

  case TOKEN_CONSTANT :

    if ( parentDataType == TYPE_CHAR ||
         parentDataType == TYPE_UCHAR ||
	 parentDataType == TYPE_ATTR ) {
      if ( isXML )
        newOffset = sprintf ( newOutput,
			      "%s%s",
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
      else
        newOffset = sprintf ( newOutput,
			      "\"%s\"%s",
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
    }
    else {
      if ( isXML ) {
	if ( parentDataType == TYPE_UNICODE ) {
	  newOffset = sprintf ( newOutput,
			        "&#%05hu%s", 
			        gHyp_data_getInt ( pParent, parentContext, TRUE ),
			        suffixStr ) ;
	}
	else {
	  newOffset = sprintf ( newOutput,
			      "%s%s", 
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
	}
      }
      else
	newOffset = sprintf ( newOutput,
			      "%s%s", 
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
    }
    break ;

  case TOKEN_VARIABLE :

    isParentVector = ( gHyp_data_dataType ( pParent ) > TYPE_STRING ) ;
    
    if ( parentContext != -1 ) {
 
      /* Subscript - print out a single value from the parent variable */
      if ( isXML )
        n = gHyp_data_getStr_xml ( pParent, 
			      value, 
			      VALUE_SIZE,
			      parentContext,
			      isParentVector ) ;
      else
        n = gHyp_data_getStr_external ( pParent, 
			      value, 
			      VALUE_SIZE,
			      parentContext,
			      isParentVector ) ;
      pValue = value ;
      if ( isXML )
        newOffset = sprintf ( newOutput, "%s%s", pValue, suffixStr ) ;
      else
	newOffset = sprintf ( newOutput, "%s%s", pValue, suffixStr ) ;

      break ;
    }
    else {

      /* Non-subscripted, print out all the variable's values */
      size = gHyp_data_getCount ( pParent ) ;
      isParentDynamic = gHyp_data_isDynamic ( pParent ) ;
      
      /* This will be our new primary buffer */
      /*gHyp_util_debug("VARIABLE, '%s'", pOutput ) ;*/

      newOutput[0] = '\0' ;
      newOffset = 0 ;  

      if ( isXML ) {

	if ( parentDataType == TYPE_ATTR ) {

	  if ( !isAttrOn && outputLen > indent ) {
            pOut = (isPRE || isTXT ) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
	    outLen = strlen ( pOut ) ;
	    if ( fp )
	      fprintf ( fp, "%s\n", pOut ) ;
	    else if ( pResult ) {
	      pLine = gHyp_data_new ( NULL ) ;
	      gHyp_data_setStr ( pLine, pOut ) ;
	      gHyp_data_append ( pResult, pLine ) ;
	    }
	    else
	      gHyp_util_log ( pOut ) ;
	    *pContentLength += outLen ;
	    *pOutput = '\0' ;
	    if ( !isMSG && !pResult) strcat ( pOutput, prefixStr ) ;
	    outputLen = strlen ( pOutput ) ;
	  }

	  if ( isAttrOn ) 
	    spacer[0] = ' ' ;
	  else
	    spacer[0] = '\0' ;

	  newOffset = sprintf ( newOutput,
				"%s%s=",
				spacer,
				gHyp_data_getLabel ( pParent ) ) ;
	}
	else {

	  /* Not an attribute - start of an element - could be a list, could be a constant */

	  /* Output what is there and start a new line */ 
	  if ( !isMRK ) {
	    if ( outputLen > indent ) {
	      pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
	      outLen = strlen ( pOut ) ;
	      if ( fp )
	        fprintf ( fp, "%s\n", pOut ) ;
	      else if ( pResult ) {
	        pLine = gHyp_data_new ( NULL ) ;
	        gHyp_data_setStr ( pLine, pOut ) ;
	        gHyp_data_append ( pResult, pLine ) ;
	      }
	      else 
	        gHyp_util_log ( pOut ) ;
	      *pContentLength += outLen ;
	      *pOutput = '\0' ;
	      if ( !isMSG && !pResult) strcat ( pOutput, prefixStr ) ;
	      outputLen = strlen ( pOutput ) ;
	    }
	  }

	  newOffset = sprintf ( newOutput,
				"<%s",
				gHyp_data_getLabel ( pParent ) ) ;
	  isAttrOn = TRUE ;

	  if ( isXML ) {
	    strcpy ( tag, gHyp_data_getLabel(pParent) ) ;
	    gHyp_util_lowerCase( tag, strlen ( tag ) ) ;
	    if ( !isPRE ) isPRE = ( strcmp ("pre",tag)==0) ;
	    if ( !isTXT ) isTXT = ( strcmp ("textarea",tag)==0 ) ;
	    if ( !isSCR ) isSCR = ( strcmp ( "script",tag)==0 || strcmp("style",tag)==0 ) ;
	    isMRK = ( strcmp ("b", tag ) == 0  || 
		strcmp ("big",tag) == 0 ||
		strcmp ("blink",tag) == 0 ||
		strcmp ("i",tag) == 0 ||
		strcmp ("small",tag) == 0 ||
		strcmp ("strike",tag) == 0 ||
		strcmp ("sub",tag) == 0 ||
		strcmp ("sup",tag) == 0 ||
		strcmp ("tt",tag) == 0 ) ;
	  }
	}
      }
      else {

	/* Not XML */

	/* Output what is there and start a new line */
	if ( outputLen > indent ) {
	  pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
 	  outLen = strlen ( pOut ) ;
	  if ( fp )
	    fprintf ( fp, "%s\n", pOut ) ;
	  else if ( pResult ) {
	    pLine = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pLine, pOut ) ;
	    gHyp_data_append ( pResult, pLine ) ;
	  }
	  else 
	    gHyp_util_log ( pOut ) ;
	  *pContentLength += outLen ;
	  *pOutput = '\0' ;
	  if ( !isMSG && !pResult) strcat ( pOutput, prefixStr ) ;
	  outputLen = strlen ( pOutput ) ;
	}

	if ( isMSG ) {
	  strcpy ( value, gHyp_data_getLabel( pParent ) ) ;
	  gHyp_util_unparseString ( value2, value, strlen(value), VALUE_SIZE, TRUE, FALSE, "'" ) ;
	}
	else
	  strcpy ( value2, gHyp_data_getLabel( pParent ) ) ;
	
	if ( isParentDynamic ) {
	  newOffset = sprintf ( newOutput,
			     "%s '%s' = {",
			     gzaType[parentDataType],
			     value2 ) ;
	}
	else {
	  newOffset = sprintf ( newOutput, 
			     "%s '%s'[%u] = {",
			     gzaType[parentDataType],
			     value2,
			     size ) ;
	}
      }

      /* Print (recursively) all the parents values */

      /* Special case, print out CHAR and UCHAR and ATTR values as character strings */
      if ( parentDataType == TYPE_CHAR || 
	   parentDataType == TYPE_UCHAR || 
	   parentDataType == TYPE_ATTR ) {

	if ( isXML ) {
	  if ( parentDataType != TYPE_ATTR ) {
	    numNonAttributes++ ;
	    isAttrOn = FALSE ;
 	    strcat ( newOutput, ">" ) ;
	    newOffset++ ;
	  }
	}

	pData = NULL ;
	context = -1 ;
	ss = -1 ;
	i = 0 ;
	noValue = TRUE ;
	value[0] = '\0' ;

	while ( (pData = gHyp_data_nextValue ( pParent, 
					       pData, 
					       &context,
					       ss ))) {
	  c = (char) gHyp_data_getRaw ( pData,
					context,
					isParentVector ) ;
	  value[i++] = c ;
	  noValue = FALSE ;

	  if ( i  >= INTERNAL_VALUE_SIZE ) {

	    /* Output what is there and start a new line */
	    value[i] = '\0' ;
	    pValue = value ;
	    if ( !isSCR && !isPRE && !isTXT ) {
	      gHyp_util_unparseString ( value2, 
					value, 
					i, 
					VALUE_SIZE, 
					isMSG, 
					isXML,
					"") ;
	      pValue = value2 ;
	    }
	    if ( isXML ) {
	      if ( parentDataType == TYPE_ATTR )
	        tmpOutputLen = sprintf ( tmpOutput, "\"%s\"", pValue ) ;
	      else
	        tmpOutputLen = sprintf ( tmpOutput, "%s",  pValue ) ;
	    }
	    else
	      tmpOutputLen = sprintf ( tmpOutput, "\"%s\",",  pValue ) ;

	    strcat ( newOutput, tmpOutput ) ;
	    newOffset += tmpOutputLen ;
	    tmpOutputLen = 0 ;
	    tmpOutput[0] = '\0' ;
	    value[0] = '\0' ;
	    i = 0 ;
	  }
	}

	if ( noValue && isXML ) {
	  value[0] = ' '; i = 1 ;
	}

	if ( i > 0 ) {

	  /* Add to output */
	  value[i] = '\0' ;
	  pValue = value ;
	  if ( !isSCR && !isPRE && !isTXT ) {	  
	    gHyp_util_unparseString ( value2, 
				      value, 
				      i, 
				      VALUE_SIZE, 
				      isMSG, 
				      isXML,
				      "") ;
	    pValue = value2 ;
	  }
	  if ( isXML ) {
	    if ( parentDataType == TYPE_ATTR ) 
	      tmpOutputLen = sprintf ( tmpOutput, "\"%s\"", pValue ) ;
	    else
	      tmpOutputLen = sprintf ( tmpOutput, "%s", pValue ) ;
	  }
	  else
	    tmpOutputLen = sprintf ( tmpOutput, "\"%s\"", pValue ) ;

	  strcat ( newOutput, tmpOutput ) ;
	  newOffset += tmpOutputLen ;
	  tmpOutputLen = 0 ;
	  tmpOutput[0] = '\0' ;
	}

      }
      else {
      
	/* Not CHAR or UCHAR or ATTR */
	pData = NULL ;
	ss = -1 ; 
	context = -1 ;
	firstVal = TRUE ;
	numNonAttributes = 0 ;
	while ( (pData = gHyp_data_nextValue ( pParent, 
					       pData, 
					       &context,
					       ss ) ) ) {

	  lastVal = ( context == size-1 ) ;
	  tokenType = gHyp_data_tokenType ( pData ) ;
	  dataType = gHyp_data_dataType ( pData ) ;

	  if ( dataType != TYPE_ATTR ) numNonAttributes++ ;
	  if ( isXML ) {

            /* If there are no attributes at all, then close the element with a ">" */ 
	    if ( numNonAttributes == 1 && isAttrOn ) {
	      strcat ( newOutput, ">" ) ;
	      isAttrOn = FALSE ;
	      newOffset = strlen ( newOutput ) ;
	    }

	  }

	  if ( isParentVector ) {

	    /* Print out a single value */
	    strcat ( pOutput, newOutput ) ;
	    newOffset = 0 ;
	    newOutput[0] = '\0' ;

	    newOffset = lHyp_fileio_describe2 ( pData,
						context,
						firstVal,
						lastVal,
						indent+2,
						pOutput,
						fp,
						isXML,
						isPRE,
						isTXT,
						isMSG,
						isSCR,
						isMRK,
						isAttrOn,
						ending,
						pResult,
						pContentLength ) ;
	    firstVal = FALSE ;
	  }
	  else {

	    /* List of values, some could be list variables, some constants. */

	    if ( tokenType == TOKEN_VARIABLE && newOffset > 0 ) {

	      /* New variables (except XML attributes) always starts a new line. 
	       * Flush output 
	       */
	      if ( !(isXML && isAttrOn)) { 
	        /* Pretend first value again so that the indent will occur */
	        firstVal = TRUE ;
	      }
	    }

	    /* Recursively print out the values of the variable. */
	    strcat ( pOutput, newOutput ) ;
	    newOffset = 0 ;
	    newOutput[0] = '\0' ;
	    outputLen = lHyp_fileio_describe2 ( pData,
						     -1,
						     firstVal,
						     lastVal,
						     indent+2,
						     pOutput,
						     fp,
						     isXML,
						     isPRE,
						     isTXT,
						     isMSG,
						     isSCR,
						     isMRK,
						     isAttrOn,
						     ending,
						     pResult,
						     pContentLength ) ;

	    if ( tokenType != TOKEN_VARIABLE )  firstVal = FALSE ;
	  }
	  
	} /* End while loop over elements */

      } /* End not CHAR or UCHAR or ATTR */

      if ( isXML ) { 

	if ( parentDataType != TYPE_ATTR ) { 

	  if ( numNonAttributes == 0 )
	    /* Only a single <element/> is required. */
	    tmpOutputLen = sprintf ( tmpOutput, " %s", "/>" ) ;	    	
	  else {
	    /* Return the trailing "</element>" */
	    if ( !isMRK ) {
	      /* Output what is there and start a new line */ 
	      if ( outputLen > indent ) {
                pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent+2,strspn (pOutput," \t\n")): pOutput ;
	        outLen = strlen ( pOut ) ;
	        if ( fp )
		  fprintf ( fp, "%s\n", pOut ) ;
	        else if ( pResult ) {
		  pLine = gHyp_data_new ( NULL ) ;
		  gHyp_data_setStr ( pLine, pOut ) ;
		  gHyp_data_append ( pResult, pLine ) ;
		}
	        else 
		  gHyp_util_log ( pOut ) ;
		*pContentLength += outLen ;
	        *pOutput = '\0' ;
	        if ( !isMSG && !pResult) strcat ( pOutput, prefixStr ) ;
	        outputLen = strlen ( pOutput ) ;
	      }
	    }
	    tmpOutputLen = sprintf ( tmpOutput, "</%s>",  gHyp_data_getLabel (pParent)) ;
	  }
	  strcat ( newOutput, tmpOutput ) ;
	  newOffset = strlen ( newOutput ) ;
	  tmpOutputLen = 0 ;
	  tmpOutput[0] = '\0' ;
	}

	if ( lastValue ) {
	  strcat ( pOutput, newOutput ) ;
	  newOffset = 0 ;
	  newOutput[0] = '\0' ;
	  if ( numNonAttributes > 0 ) {
	    if ( !isMRK ) {
	      pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")): pOutput ;
	      outLen = strlen ( pOut ) ;
	      if ( fp )
	        fprintf ( fp, "%s\n", pOut ) ;
	      else if ( pResult ) {
	        pLine = gHyp_data_new ( NULL ) ;
	        gHyp_data_setStr ( pLine, pOut ) ;
	        gHyp_data_append ( pResult, pLine ) ;
	      }
	      else 
	        gHyp_util_log ( pOut ) ;

	      *pContentLength += outLen ;
	      *pOutput = '\0' ;
	      if ( !isMSG && !pResult) strcat ( pOutput, prefixStr ) ;
	      outputLen = strlen ( pOutput ) - 2 ;
	      *(pOutput+outputLen) = '\0' ;
	    }
	  }
	}
      }
      else {

	/* Return the trailing "}" */
	if ( lastValue && indent == 0 ) {
	  suffixStr[0] = ending ; suffixStr[1] = '\0' ;
	}
	
	/* Output what is there and start a new line */ 
	if ( outputLen > indent ) {
          pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
	  outLen = strlen ( pOut ) ;
	  if ( fp )
	    fprintf ( fp, "%s\n", pOut ) ;
	  else if ( pResult ) {
	    pLine = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pLine, pOut ) ;
	    gHyp_data_append ( pResult, pLine ) ;
	  }
	  else 
	    gHyp_util_log ( pOut ) ;
	  *pContentLength += outLen ;
	  *pOutput = '\0' ;
	  if ( !isMSG && !pResult) strcat ( pOutput, prefixStr ) ;
	  outputLen = strlen ( pOutput ) ;
	}
	
        tmpOutputLen = sprintf ( tmpOutput, "}%s", suffixStr ) ;

	strcat ( newOutput, tmpOutput ) ;
	newOffset += tmpOutputLen ;
	tmpOutputLen = 0 ;
	tmpOutput[0] = '\0' ;
	if ( lastValue ) {

	  strcat ( pOutput, newOutput ) ;
	  newOffset = 0 ;
	  newOutput[0] = '\0' ;
          pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
	  outLen = strlen ( pOut ) ;
	  if ( fp )
	    fprintf ( fp, "%s\n", pOut ) ;
	  else if ( pResult ) {
	    pLine = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pLine, pOut ) ;
	    gHyp_data_append ( pResult, pLine ) ;
	  }
	  else 
	    gHyp_util_log ( pOut ) ;
	  *pContentLength += outLen ;
	  *pOutput = '\0' ;
	  if ( !isMSG && !pResult) strcat ( pOutput, prefixStr ) ;
	  outputLen = strlen ( pOutput ) - 2 ;
	  *(pOutput+outputLen) = '\0' ;
	}
      }
    } 
    break ;

  } /* End switch */

  /* For REFERENCE, LITERAL, UNIDENTIFIED, CONSTANT, the variable newOutput contains
   * the element+suffix.  If there is no room to add it to the main pOutput string, 
   * then we flush the main output string and replace its contents with the
   * new element.  Otherwise, the new element is concatenated to the main output
   * string and we return.
   *
   * For VARIABLES, if the element was subscripted, then just that element is
   * taken, and the behaviour is as above for REFERENCE, LITERAL, UNIDENTIFIED, CONSTANT.
   *
   * If the VARIABLE is not subscripted, then....
   */

  /* Return pOutput */  
  if ( outputLen + newOffset > MAX_OUTPUT_LENGTH ) {
    pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")): pOutput ;
    outLen = strlen ( pOut ) ;
    if ( fp )
      fprintf ( fp, "%s\n", pOut ) ;
    else if ( pResult ) {
      pLine = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr ( pLine, pOut ) ;
      gHyp_data_append ( pResult, pLine ) ;
    }
    else
      gHyp_util_log ( pOut ) ;
    *pContentLength += outLen ;
    outputLen = 0 ;
    *pOutput = '\0' ;
  }
  
  strcat ( pOutput, newOutput ) ;
  newOffset = 0 ;
  newOutput[0] = '\0' ;
  return strlen ( pOutput ) ;
} 

void lHyp_fileio_describe ( sInstance *pAI, 
			    sCode *pCode, 
			    sLOGICAL isPARSE,
			    sLOGICAL isFILE,
			    sLOGICAL isXML,
			    sLOGICAL isMSG,
			    sLOGICAL isString ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData,
      *pVariable,
      *pResult=NULL ;

    char
      ending,
      output[MAX_OUTPUT_LENGTH+1],
      tag[TOKEN_SIZE+1] ;

    sLOGICAL
      isPRE=FALSE,
      isTXT=FALSE,
      isSCR=FALSE,
      isAttrOn=FALSE,
      isMRK=FALSE,
      firstValue,
      lastValue ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      ss,
      indent,
      contentLength;
    
    FILE
      *fp=NULL ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( isFILE ) {

      if ( argCount != 2 ) {

	if ( isXML )
	  gHyp_instance_error ( 
			       pAI, STATUS_ARGUMENT,
			       "Invalid arguments. Usage: xfdescribe ( variable, handle )" ) ;
	else
	  gHyp_instance_error ( 
			       pAI, STATUS_ARGUMENT,
			       "Invalid arguments. Usage: fdescribe ( variable, handle )" ) ;
      }
	
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

      fp = (FILE*) gHyp_data_getHandle ( pData,
					 gHyp_data_getSubScript(pData),
					 TRUE ) ;
      if ( fp == (FILE*) 0 || fp == (FILE*) 1  )	
	gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;
    }
    else if ( isString ) { 
      if ( argCount != 1 ) {
	if ( isXML )
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: result = xsdescribe ( variable )" ) ;      
	else
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: result = sdescribe ( variable )" ) ;      
      }
    }
    else {
      if ( argCount != 1 ) {
	if ( isXML ) 
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: xdescribe ( variable )" ) ;
	else
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: describe ( variable )" ) ;
      }
    }


    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    ss = gHyp_data_getSubScript ( pData ) ;

    if ( (pVariable = gHyp_data_getVariable ( pData ) ) )
      pData = pVariable ;
    
    if ( isString ) pResult = gHyp_data_new ( "_data_" ) ; ;

    /* Do the describe on pData */

    if ( isXML ) {
      strcpy ( tag, gHyp_data_getLabel(pData) ) ;
      gHyp_util_lowerCase ( tag, strlen ( tag ) ) ;
      isPRE = ( strcmp ("pre", tag ) == 0 ) ;
      isTXT = ( strcmp ( "textarea", tag ) == 0 ) ;
      isSCR = ( strcmp ("script", tag ) == 0 || strcmp ("style",tag) == 0 ) ;
      isMRK = ( strcmp ("b", tag ) == 0  || 
		strcmp ("big",tag) == 0 ||
		strcmp ("blink",tag) == 0 ||
		strcmp ("i",tag) == 0 ||
		strcmp ("small",tag) == 0 ||
		strcmp ("strike",tag) == 0 ||
		strcmp ("sub",tag) == 0 ||
		strcmp ("sup",tag) == 0 ||
		strcmp ("tt",tag) == 0 ) ;
    }

    indent = 0 ;
    firstValue = TRUE ;
    lastValue = TRUE ;
    output[0] = '\0' ;
    ending = isXML ? ' ' : ';' ;
    contentLength = 0 ;
    lHyp_fileio_describe2 ( pData, 
			    ss, 
			    firstValue,
			    lastValue,
			    indent, 
			    output, 
			    fp,
			    isXML,
			    isPRE,
			    isTXT,
			    isMSG,
			    isSCR,
			    isMRK,
			    isAttrOn,
			    ending,
			    pResult,
			    &contentLength ) ;
    if ( output[0] ) {
      if ( fp )
        fprintf ( fp, "%s\n", output ) ;
      else if ( pResult ) {
	pData = gHyp_data_new ( NULL ) ;
	gHyp_data_setStr ( pData, output ) ;
	gHyp_data_append ( pResult, pData ) ;
      }
      else if ( !isString )
        gHyp_util_log  ( output ) ;
    }
    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }	
}

sData *gHyp_fileio_describeData ( sInstance *pAI, sData *pData, char ending, sLOGICAL isXML, int *pContentLength ) 
{
    sData
      *pContainer,
      *pLine ;

    char
      output[MAX_OUTPUT_LENGTH+1] ;

    sLOGICAL
      firstValue,
      lastValue ;

    int
      ss,
      indent ;

    /* Set variable name to blank */
    pContainer = gHyp_data_new ( " " ) ;

    indent = 0 ;
    firstValue = TRUE ;
    lastValue = TRUE ;
    ss = gHyp_data_getSubScript ( pData ) ;
    output[0] = '\0' ;
    lHyp_fileio_describe2 ( pData, 
			    ss, 
			    firstValue,
			    lastValue,
			    indent, 
			    output, 
			    NULL,
			    isXML,  /* isXML */
			    FALSE,  /* isPRE */
			    FALSE,  /* isTXT */
			    TRUE,   /* isMSG */
			    FALSE,  /* isSCR */
			    FALSE,  /* isMRK */
			    FALSE,  /* isAttrOn */
			    ending, /* Ending character */
 			    pContainer,
			    pContentLength ) ;
    if ( output[0] ) {
      pLine = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr ( pLine, output ) ;
      gHyp_data_append ( pContainer, pLine ) ;
    }

    return pContainer ;

}

void gHyp_fileio_sdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sdescribe ( variable )
   *	Returns description of variable.
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
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, FALSE, FALSE, TRUE ) ;
}

void gHyp_fileio_fdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fdescribe ( variable )
   *	Returns description of variable.
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
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, TRUE, FALSE, FALSE, FALSE ) ;
}

void gHyp_fileio_describe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: describe ( variable )
   *	Returns description of variable.
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
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, FALSE, FALSE, FALSE ) ;
}

void gHyp_fileio_xfdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: xfdescribe ( variable )
   *	Returns description of variable.
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
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, TRUE, TRUE, FALSE, FALSE ) ;
}

void gHyp_fileio_xdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: xdescribe ( variable )
   *	Returns description of variable.
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
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, TRUE, FALSE, FALSE ) ;
}

void gHyp_fileio_xsdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: xdescribe ( variable )
   *	Returns description of variable.
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
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, TRUE, FALSE, TRUE ) ;
}

static void lHyp_fileio_put (	sInstance 	*pAI, 
				sCode 		*pCode, 	
				sLOGICAL 	isStr,
				sLOGICAL 	isFile,
				sLOGICAL	isBinary,
				sLOGICAL	isPARSE )
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: put(), puts(), fput(), fputs(), fwrite()
   *	The put function puts spaces between the values, the puts function
   *	appends the values together.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   *	isStr							[R]
   *	- puts and fputs
   *
   *	isFile							[R]
   *	- fput and fputs
   *
   *	isBinary						[R]
   *	- use fwrite
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
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
    
    char
      *pValue,
      *pMsg,
      *pEndMsg,
      putMessage[MAX_OUTPUT_LENGTH+1],
      quote,
      value[VALUE_SIZE+1] ;
    
    FILE
      *fp = NULL ; 
    
    sLOGICAL
      isFirstValue = TRUE,
      isVector ;

    sBYTE
      tokenType,
      dataType ;
    
    int
      context,
      ss,
      valueLen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( isFile ) {
      if ( argCount != 2 ) {
        if ( isStr ) {
	  if ( isBinary )
  	    gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: fwrite ( data, handle )" ) ;
	  else
	    gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: fputs ( text, handle )" ) ;
        }
	else {
	  if ( isBinary )
	    gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: fwrite ( data, handle )" ) ;
	  else
	    gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: fput ( variable, handle )" ) ;
	}
      }
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      fp = (FILE*) gHyp_data_getHandle ( pData,
					 gHyp_data_getSubScript(pData),
					 TRUE ) ;
      if ( fp == (FILE*) 0 || fp == (FILE*) 1 )	
	gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;
    }
    else {
      if ( argCount != 1 ) {
        if ( isStr )
	  gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: puts ( text )" ) ;
        else	
	  gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: put ( variable )" ) ;
      }
    }

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pMsg = putMessage ;
    pEndMsg = pMsg + MAX_OUTPUT_LENGTH ;
    pResult = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; 
    context = -1 ;
    quote = '\0'; 
    isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;
    while ( (pResult = gHyp_data_nextValue ( pData, 
					     pResult, 
					     &context,
					     ss ) ) ) {
      if ( isBinary ) {
	valueLen = gHyp_data_getStr ( pResult, 
				     value, 
				     VALUE_SIZE, 
				     context, 
				     isVector ) ;

        if ( valueLen == -1 ) continue ;	
        pValue = value ;

      }
      else {
        tokenType = gHyp_data_tokenType ( pResult ) ;
        dataType = gHyp_data_dataType ( pResult ) ;

        /* If not printing a string, separate all values with spaces. */
	if ( 	!isStr && 
		!isFirstValue && 
		pMsg < pEndMsg ) *pMsg++ = ' ' ;
		
	isFirstValue = FALSE ;

	valueLen = gHyp_data_getStr_nonulls ( pResult, 
				     value, 
				     VALUE_SIZE, 
				     context, 
				     isVector ) ;

        if ( valueLen == -1 ) continue ;	
        pValue = value ;

        quote = '\0' ;
	if ( !isStr ) {
          if ( tokenType == TOKEN_VARIABLE && !isVector )
	    quote = '\'' ;	/* Deliminate with single quotes */
	  else if ( dataType == TYPE_STRING )
	    quote = '"' ;		/* Deliminate with double quotes */
	}
        if ( quote ) valueLen += 2 ;

      }

      if ( ( pMsg + valueLen ) > pEndMsg ) {
      
	/* Cannot fit, output what's there and start a new line */
	if ( isFile ) {
	  if ( isBinary )
	    fwrite ( putMessage, 1, (pMsg-putMessage), fp ) ; 
	  else
	    fprintf ( fp, "%s\n", putMessage ) ;
	}
	else
	  gHyp_util_log ( putMessage ) ;    

	pMsg = putMessage ;
      }

      /* Copy the value */
      if ( isBinary )
	memcpy ( pMsg, pValue, valueLen ) ;
      else {
	if ( quote )
	  sprintf ( pMsg, "%c%s%c", quote, pValue, quote ) ;
	else
	  sprintf ( pMsg, "%s", pValue ) ;
      }

      pMsg += valueLen ;
      *pMsg = '\0' ;
    }	
    if ( context == -2 && ss != -1 )
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds",ss ) ;

    /* Print remainder, if any */
    if ( pMsg > putMessage ) {
      if ( isFile ) {
	if ( isBinary )
	  fwrite ( putMessage, 1, (pMsg-putMessage), fp ) ;
	else
	  fprintf ( fp, "%s\n", putMessage ) ;
      }
      else
        gHyp_util_log ( putMessage ) ;    
    }
    else if ( pMsg == putMessage ) {
      if ( !isBinary ) {
        if ( isFile )
          fprintf ( fp, "\n" ) ;
        else
	  gHyp_util_log ( "" ) ;  
      }
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


static void lHyp_fileio_get ( char *pBuf, sData *pResult )
{

  sData
    *pValue ;
  
  char
    *pEndBuf,
    *pValStart,
    *pValEnd ;

  pEndBuf = pBuf + strlen ( pBuf ) ;

  /* Remove unprintables (newline or carriage return) at end of line */
  while ( pEndBuf > pBuf && *(pEndBuf-1) < ' '  ) *(--pEndBuf) = '\0' ;
      
  /* Extract token values and strings. */
  
  pValStart = pBuf ;      
  do {
    
    /* Scan for first printable character */
    pValStart += strspn ( pValStart, " \t" ) ;
    
    if ( *pValStart == '"' ) {
      
      /* Found start of LITERAL */
      pValStart++ ;
      
      /* Look for end of LITERAL */
      pValEnd = strchr ( pValStart, '"' ) ;
      if ( pValEnd == NULL ) 
	pValEnd = pEndBuf ;
      else
	*pValEnd = '\0' ;
      
      /* Create value element */
      pValue = gHyp_data_new ( NULL ) ;
      if ( pValStart < pValEnd )
	gHyp_data_setStr ( pValue, pValStart ) ;
      else
	gHyp_data_setNull ( pValue ) ;
      
    }
    else if ( *pValStart == '\'' ) {
      
      /* Found start of single quoted variable */
      pValStart++ ;
      
      /* Look for end of variable */
      pValEnd = strchr ( pValStart, '\'' ) ;
      if ( pValEnd == NULL ) 
	pValEnd = pEndBuf ;
      else
	*pValEnd = '\0' ;
      
      /* Create value element */
      pValue = gHyp_data_new ( NULL ) ;
      if ( pValStart < pValEnd )
	gHyp_data_setReference ( pValue, pValStart, NULL ) ;
      else
	gHyp_data_setNull ( pValue ) ;
      
    }
    else  {
      
      /* Found a CONSTANT. Find end. */
      pValEnd = pValStart + strcspn ( pValStart, " \t" ) ;
      *pValEnd = '\0' ;
      
      /* CONSTANT OR IDENTIFIER */
      pValue = gHyp_data_new ( NULL ) ;
      if ( pValStart < pValEnd ) {
	gHyp_data_setToken ( pValue, pValStart ) ;
      }
      else
	gHyp_data_setNull ( pValue ) ;
    }
    
    /* Append value to list. */
    gHyp_data_append ( pResult, pValue ) ;
    
    /* Advance past end of value */
    pValStart = pValEnd + 1 ;
  } 
  while ( pValStart < pEndBuf ) ;

  return ;
}

void gHyp_fileio_fopen ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fopen ( file, mode )
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
    
    char
      fileSpec[MAX_PATH_SIZE+1],
      mode[VALUE_SIZE+1],
      *pFileSpec,
      *pMode ;
    
    FILE
      *pFILE=NULL ;
    
    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;	

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: fopen ( file, mode )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   mode, 
			   VALUE_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pMode = mode ;
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;
        
#ifndef AS_RESTRICTED
    pFILE = fopen ( pFileSpec, pMode ) ;
    if ( !pFILE ) {
      gHyp_instance_warning ( pAI, STATUS_FILE,
			      "Could not open file '%s'", pFileSpec ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

#else
    gHyp_instance_warning ( pAI, STATUS_RESTRICTED, "The fopen() function is RESTRICTED" ) ;
#endif

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) pFILE ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_fclose ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fclose ( handle )
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
   */	

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData ;
    
    FILE
      *pFILE ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: fclose ( handle )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    pFILE = (FILE*) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE);
    if ( fclose ( pFILE ) != 0 )
      gHyp_instance_warning ( pAI, STATUS_FILE, "Failed to close file" ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


void gHyp_fileio_fwrite ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fwrite( data, handle )
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
   */

  lHyp_fileio_put ( pAI, pCode, FALSE, TRUE, TRUE, isPARSE ) ;
}


void gHyp_fileio_fput ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fput ( variable, handle )
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
   */

  lHyp_fileio_put ( pAI, pCode, FALSE, TRUE, FALSE, isPARSE ) ;
}

void gHyp_fileio_fputs ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fputs ( string, handle )
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
   */

  lHyp_fileio_put ( pAI, pCode, TRUE, TRUE, FALSE, isPARSE ) ;
}

void gHyp_fileio_fget ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fget ( handle )
   *	Returns list of values input.
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
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pResult ;
    
    char
      *pBuf ;
    
    char
      buf[MAX_INPUT_LENGTH+1] ;
    
    FILE
      *fp ;    

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: fget ( handle )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    fp = (FILE*) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE ) ;

    if ( fp == (FILE*) 0 || fp == (FILE*) 1 )	
      gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;

    pBuf = fgets ( buf, MAX_INPUT_LENGTH, fp ) ;
    if ( !pBuf && !feof(fp)  )
      gHyp_instance_error ( pAI, 
			    STATUS_FILE, 
			    "Failed to read from file" ) ;

    pResult = gHyp_data_new ( "_fget_" ) ;
    if ( pBuf ) {
      lHyp_fileio_get ( pBuf, pResult ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;

    /* Push the result on the stack */
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_fgets ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fgets ( handle )
   *	Returns the string that was input.
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
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      buflen,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sData	
      *pData,
      *pResult ;

    char
      *pBuf ;

    char
      buf[MAX_INPUT_LENGTH+1] ;

    FILE
      *fp ;    

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: fgets ( handle )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    fp = (FILE*) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    if ( fp == (FILE*) 0 || fp == (FILE*) 1 )	
      gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;
    pBuf = fgets ( buf, MAX_INPUT_LENGTH, fp ); 
    if ( !pBuf && !feof ( fp ) )
      gHyp_instance_error ( pAI,
			    STATUS_FILE, 
			    "Failed to read from file" ) ;
    
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_fgets_", TYPE_STRING ) ;
    if ( pBuf ) {
      buflen = strlen ( pBuf ) ;
#ifdef AS_PROMIS
      /* Remove unprintables (newline or carriage return) at end of line */
      while ( buflen > 0 && *(pBuf+buflen-1) < ' ' ) { 
	buflen-- ;
	*(pBuf+buflen) = '\0' ;
      }
#endif
      if ( buflen == 0 ) 
	gHyp_data_setNull ( pResult ) ;
      else
	gHyp_data_setText ( pResult, pBuf, buflen ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_fread ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fread ( handle )
   *	Returns the binary data that was input.
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
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      bufLen,
      width,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sData	
      *pData,
      *pResult ;

    char
      *pBuf ;

    char
      buf[MAX_INPUT_LENGTH+1] ;

    FILE
      *fp ;    

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: fread ( handle )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    fp = (FILE*) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    if ( fp == (FILE*) 0 || fp == (FILE*) 1 )	
      gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;

    bufLen = fread ( buf, 1, MAX_INPUT_LENGTH, fp ); 
    if ( bufLen < 0 && !feof ( fp ) )
      gHyp_instance_error ( pAI,
			    STATUS_FILE, 
			    "Failed to read from file" ) ;
    
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_fread_", TYPE_STRING ) ;

    if ( bufLen == 0 )
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;
    else {

      /* Add the content to the content data area */
      pBuf = buf ;
      while ( bufLen > 0 ) {

	pData = gHyp_data_new ( NULL ) ;

	width = MIN ( bufLen, INTERNAL_VALUE_SIZE ) ;
	gHyp_data_setStr_n ( pData, (char*) pBuf, width ) ;
	gHyp_data_append ( pResult, pData ) ;

	bufLen -= width ;
	pBuf += width ;

      }
    }

    gHyp_stack_push ( pStack, pResult ) ;
  }
}
void gHyp_fileio_get ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: get ( prompt )
   *	Returns list of values input.
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
   *	- version 2.1-0
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    int	   	
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sData
      *pData,
      *pResult ;

    char
      *pBuf,
      prompt[VALUE_SIZE+1],
      *pPrompt ;

    char
      buf[MAX_INPUT_LENGTH+1] ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: get ( prompt )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr_nonulls ( pData, 
			    prompt, 
			    VALUE_SIZE, 
			    gHyp_data_getSubScript(pData),
			    TRUE ) ;
    pPrompt = prompt ;
    gHyp_util_output2 ( pPrompt ) ;
    pBuf = fgets ( buf, MAX_INPUT_LENGTH, stdin ); 
    if ( !pBuf && !feof ( stdin ) )
      gHyp_instance_error ( pAI, 
			    STATUS_FILE, 
			    "Failed to read from input stream" ) ;

    pResult = gHyp_data_new ( "_get_" ) ;
    if ( pBuf ) {
      lHyp_fileio_get ( pBuf, pResult ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;
      
    /* Push the result on the stack */
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_gets ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: gets ( prompt )
   *	Returns the string that was input.
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
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      n,
      buflen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pResult ;
    
    char
      *pBuf,
      prompt[VALUE_SIZE+1],
      *pPrompt,
      buf[MAX_INPUT_LENGTH+1] ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: gets ( prompt )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr_nonulls ( pData, 
			    prompt, 
			    VALUE_SIZE, 
			    gHyp_data_getSubScript(pData),
			    TRUE ) ;
    pPrompt = prompt ;
    gHyp_util_output2 ( pPrompt ) ;
    pBuf = fgets ( buf, MAX_INPUT_LENGTH, stdin ); 
    if ( !pBuf && !feof ( stdin ) )
        gHyp_instance_error ( pAI,
			      STATUS_FILE,
			      "Failed to read from input stream" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_gets_", TYPE_STRING ) ;
    if ( pBuf ) {
      buflen = strlen ( pBuf ) ;
      /* Remove unprintables (newline or carriage return) at end of line */
      while ( buflen > 0 && *(pBuf+buflen-1) < ' ' ) { 
	buflen-- ;
	*(pBuf+buflen) = '\0' ;
      }
      if ( buflen == 0 ) 
	gHyp_data_setNull ( pResult ) ;
      else
	gHyp_data_setText ( pResult, pBuf, buflen ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_log ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: log ( file, text )
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
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pFile,
      *pValue,
      *pData=NULL ;

    char
      *pStr,
      *pFileSpec=NULL,
      *pMsg,
      *pEndMsg,
      fileSpec[MAX_PATH_SIZE+1],
      logMessage[MAX_LOG_LENGTH+1],
      value[VALUE_SIZE+1] ;
    
    sBYTE
      lastTokenType,
      nextTokenType ;
    
    sLOGICAL
      isVector ;
    
    FILE
      *pFILElog = NULL ;
    
    int 
      n,
      context,
      ss,
      valueLen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( argCount > 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: log ( [file [,text]] )" ) ;

    if ( argCount == 2 )
      pData = gHyp_stack_popRdata ( pStack, pAI ) ;

    if ( argCount > 0 ) {
      pFile = gHyp_stack_popRvalue ( pStack, pAI ) ; 
      n = gHyp_data_getStr ( pFile, 
			     fileSpec, 
			     MAX_PATH_SIZE,
			     gHyp_data_getSubScript(pFile),
			     TRUE) ;
      pFileSpec = fileSpec ;
        
#ifndef AS_RESTRICTED

      pFILElog = fopen ( pFileSpec, "a+"
#ifdef AS_VMS
			 , "shr=get,put"
#endif
			 ) ;
#else
    gHyp_instance_warning ( pAI, STATUS_RESTRICTED,"The log() function is RESTRICTED" ) ;
#endif

    }

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount == 0 ) {

      gsLog = stdout ;
      gzLogName[0] = '\0' ;
    }
    else if ( !pFILElog ) {
    
      gHyp_instance_warning ( pAI, STATUS_FILE, 
			      "Could not open file '%s'", pFileSpec ) ;
    }
    else if ( argCount == 1 ) {
    
      if ( gzLogName[0] ) {
	/* Close previous one */
	fclose ( gsLog ) ;
      }
      /* Set new log file */
      gsLog = pFILElog ; 
      strcpy ( gzLogName, pFileSpec ) ; 

    }
    else {

      pMsg = logMessage ;
      pEndMsg = pMsg + MAX_LOG_LENGTH ;
      gsCurTime = time ( NULL ) ;
      sprintf (	pMsg, "%s:", gHyp_util_timeStamp ( gsCurTime ) ) ;
      pMsg += (DATETIME_SIZE+1) ;

      isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING );
      lastTokenType = TOKEN_NULL ;
      pValue = NULL ;
      ss = gHyp_data_getSubScript ( pData ) ; 
      context = -1 ;
      while ( (pValue = gHyp_data_nextValue ( pData, 
					       pValue, 
					       &context,
					       ss ) ) ) {
	
	/* Put spaces between tokens that are not literals. */
	nextTokenType = gHyp_data_tokenType ( pValue ) ;
	if (	lastTokenType   != TOKEN_NULL &&
		( lastTokenType != TOKEN_LITERAL || 
		  nextTokenType != TOKEN_LITERAL) &&
		pMsg < pEndMsg ) *pMsg++ = ' ' ;
	  
	valueLen = gHyp_data_getStr_nonulls ( pValue, 
				       value, 
				       VALUE_SIZE,
				       context,
				       isVector ) ;
	pStr = value ;
	
        if ( (pMsg + valueLen) > pEndMsg ) {
	
	  /* Cannot fit, wrap onto next line */
	  int truncLen = pEndMsg - pMsg ;
	  strncpy ( pMsg, pStr, truncLen ) ;
	  pMsg += truncLen ;
	  *pMsg = '\0' ;
	  pStr += truncLen ;
	  valueLen -= truncLen ;
          fprintf ( pFILElog, "%s\n", logMessage ) ;
	  pMsg = logMessage ;
	}
	  
	/* Copy the value (or what's left after truncation) */
	strncpy ( pMsg, pStr, valueLen ) ;
        pMsg += valueLen ;
	
	/* Remember last token type */
	lastTokenType = nextTokenType ;

	/* Cap it  */
	*pMsg = '\0' ;
	
      }
      if ( context == -2 && ss != -1 )
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Subscript '%d' is out of bounds", ss ) ;

      /* Print remainder, if any */
      if ( pMsg > logMessage+DATETIME_SIZE+1 ) 
	fprintf ( pFILElog, "%s\n", logMessage ) ;
      
      fclose ( pFILElog ) ;
    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_fileio_put ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: put ( variable )
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
   */
  lHyp_fileio_put ( pAI, pCode, FALSE, FALSE, FALSE, isPARSE ) ;
}


void gHyp_fileio_puts ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: puts ( string )
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
   */
  lHyp_fileio_put ( pAI, pCode, TRUE, FALSE, FALSE, isPARSE ) ;
}

void gHyp_fileio_unlink ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: unlink ( file )
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
      *pData ;
    
    char
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1] ;
    
    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: unlink ( filespec )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;
#ifndef AS_RESTRICTED
    if ( unlink ( pFileSpec ) == -1 )
      gHyp_instance_warning ( pAI, STATUS_FILE, "Failed to unlink file %s",pFileSpec ) ;
#else
    gHyp_instance_warning ( pAI, STATUS_RESTRICTED, "The fopen() function is RESTRICTED" ) ;
#endif

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_fileio_echo ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: echo ( "text" )
   *	Returns the string that was input.
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
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      n,
      buflen,
      eosIndex,
      hypIndex,
      hypCount,
      tokenType,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pValue,
      *pData,
      *pResult ;
    
    sHyp
      *pHyp ;

    sLOGICAL
      isEOF = FALSE,
      isEOT = FALSE ;

    char
      *pBuf,
      endOfText[VALUE_SIZE+1],
      *pEOT,
      buf[MAX_INPUT_LENGTH+1] ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: echo ( endOfText )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr_nonulls ( pData, 
			    endOfText, 
			    VALUE_SIZE, 
			    gHyp_data_getSubScript(pData),
			    TRUE ) ;
    pEOT = endOfText ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_echo_", TYPE_STRING ) ;
    
    pHyp = gHyp_frame_getHyp ( pFrame ) ;
    hypCount = gHyp_hyp_getHypCount ( pHyp ) ;
    eosIndex = hypIndex = gHyp_frame_getHypIndex ( pFrame ) ;
    pBuf = buf ;
 
    if ( hypCount == eosIndex ) {

      while ( !isEOT && !isEOF && pBuf ) {

        pBuf = fgets ( buf, MAX_INPUT_LENGTH, gsPP );
        isEOF = feof ( gsPP ) ;      
        if ( !pBuf && !isEOF )
          gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
	
        if ( pBuf ) {
	  buflen = strlen ( pBuf ) ;
	  isEOT = ( strncmp ( pBuf, pEOT, n ) == 0 ) ;
	  if ( !isEOT ) {
	    pValue = gHyp_data_new ( NULL ) ;
	    if ( buflen == 0 ) 
	      gHyp_data_setNull ( pValue ) ;
	    else {
              /*
	      gHyp_util_trim ( pBuf ) ;
	      buflen = strlen ( pBuf ) ;
	      */
              buflen = gHyp_util_parseString ( pBuf ) ;
	      gHyp_data_setStr_n ( pValue, pBuf, buflen ) ;
	    }
	    gHyp_data_append ( pResult, pValue ) ;
	  }
	}
      }
    }
    else {

      /* Probably in dereference or in a method */
      while ( !isEOT && eosIndex < hypCount && pBuf ) {
	        
	pCode = gHyp_hyp_code ( pHyp, eosIndex ) ;
	tokenType = gHyp_hyp_tokenType ( pCode ) ;
	pBuf = gHyp_hyp_token ( pCode ) ;
	/*gHyp_util_debug("TOKEN %s",pBuf);*/
	buflen = strlen ( pBuf ) ;
	isEOT = ( strncmp ( pBuf, pEOT, n ) == 0 ) ;
	if ( !isEOT ) {
	  pValue = gHyp_data_new ( NULL ) ;
	  if ( buflen > 0 ) {
	    if ( tokenType == TOKEN_LITERAL ) {
	      buflen = sprintf ( buf, "\"%s\"", pBuf ) ;
	      pBuf = buf ; ;
	    }
	    else if ( tokenType == TOKEN_UNIDENTIFIED ) {
	      buflen = sprintf ( buf, "`%s`", pBuf ) ;
	      pBuf = buf ;
  	    }
	    gHyp_data_setStr_n ( pValue, pBuf, buflen ) ;
	    gHyp_data_append ( pResult, pValue ) ;
	  }
	}
	eosIndex++ ;
      }
      if ( eosIndex > hypIndex ) {
	 gHyp_frame_setHypIndex ( pFrame, eosIndex - 1 ) ;
      }
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_load( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: load ( fileSpec )
   *	Returns the string that was input.
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
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      maxLines,
      line,
      n,
      buflen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pValue,
      *pData,
      *pResult ;

    sLOGICAL
      isEOF = FALSE ;

    char
      *pBuf,
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1],
      buf[MAX_INPUT_LENGTH+1] ;

    FILE
      *pFILE ;

    if ( argCount != 1 && argCount != 2) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: load ( fileSpec [,maxlines] )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount == 2 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      maxLines = gHyp_data_getInt ( pData, gHyp_data_getSubScript (pData), TRUE ) ;
    }
    else
      maxLines = -1 ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;

    pFILE = fopen ( pFileSpec, "r" ) ;
    if ( !pFILE ) {
      gHyp_instance_warning ( pAI, STATUS_FILE,
			      "Could not open file '%s'", pFileSpec ) ;
      pResult = NULL ;
    }
    else {

      pResult = gHyp_data_new ( NULL ) ;
      gHyp_data_setVariable ( pResult, "_load_", TYPE_STRING ) ;
    
      line = 0 ;
      pBuf = buf ;
      while ( !isEOF && pBuf ) {

	        pBuf = fgets ( buf, VALUE_SIZE, pFILE );
        isEOF = feof ( pFILE ) ;      
        if ( !pBuf && !isEOF ) {
	  gHyp_data_delete ( pResult ) ;
          gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
	}

        if ( pBuf ) {
	  buflen = strlen ( pBuf ) ;
	  pValue = gHyp_data_new ( NULL ) ;
	  if ( buflen == 0 ) 
	    gHyp_data_setNull ( pValue ) ;
	  else {
	    gHyp_util_trim ( pBuf ) ;
	    gHyp_data_setStr_n ( pValue, pBuf, strlen(pBuf) ) ;
	  }
	  gHyp_data_append ( pResult, pValue ) ;
	  line++ ;
	  if ( maxLines != -1 && line >= maxLines ) break ;
	}
      }
 
      fclose ( pFILE ) ;
    }

    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_fileio_load_binary( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: load_binary ( fileSpec )
   *	Returns the string that was input.
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
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      n,
      buflen,
      fileSize,
      filePtr,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pValue,
      *pData,
      *pResult ;

    char
      *pBuf,
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1],
      buf[MAX_INPUT_LENGTH+1] ;

    FILE
      *pFILE ;


  struct stat
    stats ;

  if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: load_binary ( fileSpec  )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;

    pFILE = fopen ( pFileSpec, "rb" ) ;
    if ( !pFILE ) {
      gHyp_instance_warning ( pAI, STATUS_FILE,
			      "Could not open file '%s'", pFileSpec ) ;
      pResult = NULL ;
    }
    else {

      if ( stat ( pFileSpec, &stats ) < 0 )
	gHyp_instance_error ( pAI, STATUS_FILE,
			      "Could not stat file '%s'", pFileSpec ) ;
      fileSize = stats.st_size ;
      filePtr = 0 ;

      pResult = gHyp_data_new ( NULL ) ;
      gHyp_data_setVariable ( pResult, "_load_", TYPE_STRING ) ;
    
      pBuf = buf ;
      while ( 1 ) {

        buflen = fread ( buf, 1, INTERNAL_VALUE_SIZE, pFILE );
        if ( buflen < 0 ) {
	  gHyp_data_delete ( pResult ) ;
          gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
	}
	else if ( buflen == 0 || filePtr >= fileSize ) {
	  break ;
	}
        else  {

	  if ( filePtr+buflen > fileSize ) {
            /*gHyp_util_debug ( "Would exceed file size!!!" ) ;*/
	    buflen = fileSize - filePtr ;
	  }
	  filePtr += buflen ;

	  pValue = gHyp_data_new ( NULL ) ;
	  gHyp_data_setStr_n ( pValue, pBuf, buflen ) ;
	  gHyp_data_append ( pResult, pValue ) ;
	}
      }
 
      fclose ( pFILE ) ;
    }

    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }

}
