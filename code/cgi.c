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

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/* Possible error states: */
enum {
  CGIERR_NONE,
  CGIERR_NOT_INTEGER,
  CGIERR_NOT_DOUBLE,
  CGIERR_NOT_BOOL,
  CGIERR_UNKNOWN_METHOD,
  CGIERR_INCORRECT_TYPE,
  CGIERR_NULL_QUERY_STRING,
  CGIERR_BAD_CONTENT_LENGTH,
  CGIERR_CONTENT_LENGTH_DISCREPANCY,
  CGIERR_CANT_OPEN,
  CGIERR_OUT_OF_MEMORY,
  CGIERR_NUM_ERRS
};

/* Current error state. */

enum {
  CGIREQ_NONE,
  CGIREQ_GET,
  CGIREQ_POST,
  CGIREQ_UNKNOWN
};

/* Globals: */
static int giCgiNumEntries = 0;
static int giCgiErrno = CGIERR_NONE;
static int giCgiRequestMethod = CGIREQ_NONE;
static char *gzCgiQuery = NULL;
static sCgi* cgi_entries ;

/* English error strings: */
static char * cgi_error_strings[CGIERR_NUM_ERRS] = {
  "", 
  "Not an integer",
  "Not a double",
  "Not a boolean",
  "Unknown method",
  "Incorrect Content Type",
  "NULL Query String",
  "Bad Content Length",
  "Content Length Discrepancy"
};

static char gzStream[STREAM_READ_SIZE*4+1] ;

/********************** INTERNAL OBJECT STRUCTURES ***************************/

struct cgi_entry_t
{
  char *name;
  char *val;
} ;

/* Converts hexadecimal to decimal (character): */

static char lHyp_cgi_x2c(char *what)
{
  register char digit;
  
  digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
  digit *= 16;
  digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
  return (digit);
}


/* Unescapes "%"-escaped characters in a query: */

void gHyp_cgi_unescapeUrl(char *url)
{
  register int x,y,len;
  
  len = strlen(url);
  
  for (x=0, y=0; url[y]; ++x, ++y)
    {
      if ((url[x] = url[y]) == '%'
	  && y < len - 2)   /* 2.0.4 - MJ Pomraning (pilcrow@mailbag.com) */
	{
	  url[x] = lHyp_cgi_x2c(&url[y+1]);
	  y+=2;
        }
    }
  url[x] = '\0';
}


/* Converts pluses back to spaces in a query: */

void gHyp_cgi_plusToSpace(char *str)
{
  register int x;
  
  for (x=0; str[x]; x++)
    if (str[x] == '+')
      str[x] = ' ';
}


void gHyp_cgi_urlstring ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: urlstring ( variable, [,variable[,variable[,variable....]]]] )
   *	Returns the urlstring.
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
   *	strin
   *
   * Modifications:
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
      *pResult,
      *pValue,
      *pData;
    
    char
      amp[2],
      decryptedLabel[TOKEN_SIZE+1],
      encryptedLabel[TOKEN_SIZE+1],
      decryptedValue[MAX_INPUT_LENGTH+1],
      encryptedValue[MAX_INPUT_LENGTH+1],
      line[MAX_STREAM_LENGTH+1] ;

    int
      n,
      nl,nv,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount == 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: urlstring ( v1, [,v2[,v3[,v4....]]] )" ) ;

    pResult = gHyp_data_new ("_urlstring_") ;

    amp[0] = '&' ;
    amp[1] = '\0' ;

    while ( argCount-- ) {

      pData = gHyp_stack_popLvalue ( pStack, pAI ) ;

      /* Make label */
      nl = sprintf ( decryptedLabel, "%s", gHyp_data_getLabel ( pData ) ) ;

      nl = gHyp_util_urlEncode ( decryptedLabel, nl, encryptedLabel ) ;

      nv = gHyp_data_getStr ( pData, 
			     decryptedValue, 
			     MAX_INPUT_LENGTH, 
			     gHyp_data_getSubScript ( pData ), 
			     TRUE ) ;
      nv = gHyp_util_urlEncode ( decryptedValue, nv, encryptedValue ) ;
 
      if ( argCount == 0 ) amp[0] = '\0';

      /* Make line */
      n = sprintf ( line, "%s%s=%s", amp, encryptedLabel, encryptedValue ) ;

      pValue = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr_n ( pValue, line, n );
      gHyp_data_insert ( pResult, pValue ) ;
    }

    gHyp_stack_push ( pStack, pResult ) ;
  }	
}

void gHyp_cgi_urlparse ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: urlparse ( string )
   *	Returns the variables in a list
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
   *	strin
   *
   * Modifications:
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
      *pValue2,
      *pVariable,
      *pResult ;
    
    int
      n,
      context,
      ss,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    char
      *pTokenName,
      *pTokenValue,
      *pChar,
      value[MAX_INPUT_LENGTH+1] ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

     if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: urlparse ( queryString )" ) ;


    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_urlparse_", TYPE_STRING ) ;
    isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
					    pValue, 
					    &context,
					    ss ) ) ) {
      n = gHyp_data_getStr ( pValue, 
			     value, 
			     MAX_INPUT_LENGTH, 
			     context, 
			     isVector ) ;
      
      value[n] = '\0' ;
      pTokenName = value ;

      if ( context == 0 ) {

        /* First value, skip past leading '?' */
        if ( *pTokenName == '?' ) {
          n-- ;
          pTokenName++ ;
        }
      }
      
      while ( *pTokenName ) {

	/* Look for '=' */
	pTokenValue = strchr ( pTokenName, '=' ) ;
	if ( !pTokenValue ) break ;

	/* Make sure there is both a token and a value */
	*pTokenValue = '\0' ;
	gHyp_util_trim ( pTokenName ) ;
	if ( strlen ( pTokenName ) == 0 ) break ;

	/*gHyp_util_debug("Adding tokenname %s",pTokenName);*/
        /* Get the token value */
	pTokenValue++ ;
		  
	pChar = strchr ( pTokenValue, '&' ) ;
	if ( pChar ) *pChar++ = '\0' ;
	else pChar = pTokenValue + strlen ( pTokenValue ) ;

	gHyp_util_trim ( pTokenValue ) ;

	/*gHyp_util_debug("Adding tokenvalue %s",pTokenValue);*/
	/*pVariable = gHyp_frame_createVariable ( pFrame, pTokenName ) ;*/
        pVariable = gHyp_data_new ( NULL ) ;
  	gHyp_data_setVariable ( pVariable, pTokenName, TYPE_STRING ) ;
 	pValue2 = gHyp_data_new ( NULL ) ;
	if ( strlen ( pTokenValue ) > 0 ) {
    	  gHyp_cgi_plusToSpace( pTokenValue );
	  gHyp_cgi_unescapeUrl( pTokenValue );
	  gHyp_data_setStr ( pValue2, pTokenValue ) ;
	}
	gHyp_data_append ( pVariable, pValue2 ) ;
        gHyp_data_append ( pResult, pVariable ) ;

	pTokenName = pChar ;
      }
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in decode()",
			    ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }	
}

char *gHyp_cgi_parseXML ( char *pStream,
			  char *pAnchor,
			  char **ppEndOfStream,
			  sData *pStreamData,
			  sData **ppValue,
			  int *pContext,
			  int ss,
			  sLOGICAL isVector,
			  sLOGICAL isSCR,
			  sLOGICAL isPRE,
			  sLOGICAL isTXT,
			  sData *pParentTag, 
			  sData *pGrandParentTag,
			  sLOGICAL *pIsEndTag,
			  sFrame *pFrame,
			  sData *pTV,
			  FILE *pp,
			  sInstance *pAI,
			  sBYTE requestedType,
			  sLOGICAL isPureXML,
			  sLOGICAL mayReturnChildLess ) 
{
  sData
    *pData,
    *pUpperTag,
    *pChildTag,
    *pStrData,
    *pAttrData,
    *pArgs ;

  
  sData
    *pSrc,
    *pValue,
    *pLvalue,
    *pResult;
  
  char
#ifdef AS_TEST_XML_PARSING
    space[20],
#endif
    c,
    quote,
#ifdef _DEBUG 
    *pMemory,
#endif
    *pTag,
    *pExpectedEndTag,
    *pTag2=NULL,
    *pStr,
    *pStr2,
    *pStr3,
    *pSearch,
    buffer[MAX_INPUT_LENGTH+1],
    attr[MAX_INPUT_LENGTH+1],
    tag[TOKEN_SIZE+1],
    tag2[TOKEN_SIZE+1],
    tag_lc[TOKEN_SIZE+1],
    tag2_lc[TOKEN_SIZE+3] ;

  sBYTE
    reqType = TYPE_NULL ;

  int
    dataType,
    context2,
    ss2,
    sss,
    context,
    n,
    i,
    streamLen,
    span,
    wspan,
    tagLen,
    tag2Len,
    attrLen,
    strLen,
    strLen2,
    strLen3,
    truncStrLen ;

  sLOGICAL
    noTAG,
    tagged,
    found,
    inComment,
    inDefinition,
    allowAttributes,
    isEndTag,
    hasNL=FALSE,
    hasWS=FALSE,
    isLiteral=(isPRE||isSCR||isTXT),
    isFirstLine,
    lastBlankLine,
    isLastLine,
    isChildLess,
    hasVariable,
    maybeReturnChildLess=FALSE,
    skipAttr,
    terminated ;

  static char *childLess = 
    "/area/base/basefont/br/col/embed/frame/hr/img/input/isindex/link/meta/p/param/spacer/" ;

  static char *preContent =
    "/tt/b/i/em/strong/code/samp/kbd/var/cite/" ;

 #define SEG_SIZE 20

  /* Get more data if necessary */
#ifdef _DEBUG 
  pMemory = pStream ;
#endif
  pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
					&streamLen, pStreamData, 
					ppValue, pContext, ss, isVector, 
					pp ) ;
#ifdef _DEBUG 
  if ( pMemory != pStream ) 
    gHyp_util_debug ( "Memory shift, %d bytes",streamLen ) ;
#endif
  if ( !pStream ) {
    gHyp_instance_warning ( pAI, STATUS_XML, "No XML stream to parse");
    return NULL ;
  }

  /* Look for start tag, comment, or definition */
  pTag = NULL ;
  isEndTag = FALSE ;
  isChildLess = FALSE ;
  tagged = FALSE ;
  noTAG = FALSE ;
  while ( !pTag ) {

    do {

      /* Look for start of next tag or end of tag. */
      pTag = strchr ( pStream, '<' ) ;
      if ( !pTag ) {
        /*
         gHyp_instance_warning ( pAI, STATUS_XML, "No starting '<' found in XML stream, searching from [%.*s]",
			      SEG_SIZE,pStream );
         return NULL ;
         */
        noTAG = TRUE ;
        pTag = *ppEndOfStream ;
	if ( !pTag ) break ;
      }
      else
        noTAG = FALSE ;

      /* Found something.  Grab the contents preceeding it. (or ignore it if there
       * is no parent, ie: this is the first call to the function and we haven't
       * encountered any root tags yet, or we are in-between root tags)
       */
      if ( pParentTag ) {
        
	/*gHyp_util_debug("process %s",pTag);*/
	isFirstLine = TRUE ;
	lastBlankLine = FALSE ;
	while ( pStream < pTag ) {

	  /* When 'tagged' is TRUE, the pStream pointer is at the token of the next tag,
	   * i.e. <token> or </token>.  We want to back it up to point properly. */
	  if ( tagged ) { pStream-- ; if ( isEndTag ) pStream-- ; tagged = FALSE ; }

	  span = 0 ;
	  wspan = 0 ;
	  if ( !isLiteral ) {
	    /* When not "SCR or PRE or TXT", find the start of the first word in the line,
	     * or the next line, whichever comes first.
	     * (otherwise whitespace is kept LITERALLY).
	     */
	    /* First span over newline  Just ONE!   */
	    if ( *pStream == '\n' )
	      span = ( *(pStream+1) == '\r' ) ? 2 : 1 ;
	    else if ( *pStream == '\r' )
	      span = ( *(pStream+1) == '\n' ) ? 2 : 1 ;
	    else
	      span = 0 ;

	    hasNL = (span > 0 ) ;

	    /* Then over whitespace */
	    wspan = strspn ( pStream+span, " \t" ) ;
	    hasWS = ( wspan > 0 ) ;

	    span += wspan ;

	    /* Stop there */
	    pStream += span ;
	  }

	  /* Find the end of each line or take the entire section up to the '<' */
	  strLen = strcspn ( pStream, "\n\r" ) ;

          /* For literal content (SCRIPT, PRE, TEXT), take everything, including the nl */
	  if ( strLen == 0 && isLiteral ) strLen = strspn ( pStream, "\n\r" ) ;

	  /* But never go past the end of the content, the "<" */
	  if ( strLen > (pTag-pStream) ) strLen = pTag - pStream ; 

          /* Need to know if this is the last line */
          isLastLine = noTAG ? FALSE : ( pTag == pStream );

          /* For zero length lines, we need to know if they count as elements */
	  if ( strLen == 0 ) {

	    /* Seemingly an empty line */
	    if ( isPRE || hasNL || hasWS ) {

#ifdef AS_TEST_XML_PARSING
	      sprintf ( space, "%d %d %d %d",hasNL,hasWS,isFirstLine,isLastLine);
#endif
	      /*
               * Empty content which translates to NULL elements.
               *
	       * 
	       *

	      NL WS F  L  Space Required
              -- -- -- -- --------------
	      0  0  0  0  not possible
	      0  0  0  1  not possible
	      0  0  1  0  not possible
	      0  0  1  1  not possible
	      0  1  0  0  not possible
	      0  1  0  1  not possible
	      0  1  1  0  yes     <tag>__W__nl___</tag>
	      0  1  1  1  yes     <tag>__W__</tag>
	      1  0  0  0  yes     <tag>___nlnl___</tag>
	      1  0  0  1  no      <tag>___nl</tag>
	      1  0  1  0  no      <tag>nlnl___</tag>
	      1  0  1  1  no      <tag>nl</tag>
	      1  1  0  0  yes     <tag>__nl__W__nl___</tag>
	      1  1  1  0  no      <tag>__W__nl___</tag>
	      1  1  0  1  maybe   <tag>__nl__W__</tag>
	      1  1  1  1  no      <tag>nl__W__</tag>

              *
	      */

	      if (   ( !hasNL && hasWS && isFirstLine && isLastLine ) ||
		     ( hasNL && hasWS && !isFirstLine && isLastLine && lastBlankLine ) ||
		     ( !hasNL && hasWS && isFirstLine && !isLastLine ) ||
		     ( hasNL && hasWS && !isFirstLine && !isLastLine ) ||
		     ( hasNL && !hasWS && !isFirstLine && !isLastLine )
		  ) {
		 
	        /* Add a space */
  	        pStrData = gHyp_data_new ( NULL ) ;
#ifdef AS_TEST_XML_PARSING
		gHyp_data_setStr ( pStrData, space ) ;
#else
		gHyp_data_setStr ( pStrData, " " ) ;
#endif
	        gHyp_data_append ( pParentTag, pStrData ) ;
	      }   
	    }
            lastBlankLine = TRUE ;
	  }
          else {
            /* Content with a clearly defined element */
            lastBlankLine = FALSE ;
	    strLen3 = strLen ;
	    pStr3 = pStream ;

	    while ( strLen3 > 0 ) {

	      truncStrLen = MIN ( INTERNAL_VALUE_SIZE, strLen3 ) ;
	      if ( truncStrLen > 0 ) {

		if ( truncStrLen == INTERNAL_VALUE_SIZE && strLen3 > INTERNAL_VALUE_SIZE ) {

		  /* Let's not cut words in half if we can help it.
		   * Make the string a little shorter if we find a space
		   * between non-spaces.
		   * Start at the end of the string, search backwards over
		   * non-spaces until we find a space.  If so, and its
		   * not the beginning of the string, then we split the
		   * string.
		   * 
		   */
		  pStr = pStr3+truncStrLen ;

		  while ( pStr > pStr3 ) {

		    c = *pStr ;

		    /* If we find a space, split the string right there. */
		    if ( c == ' ' ) break ;

		    pStr-- ;
		  }

		  if ( pStr - pStr3 > 0 ) {
		    /* We found a space, split it there */
		    truncStrLen = pStr - pStr3 ;
		  }
		  else {

		    /* Try comma. */
		    pStr = pStr3+truncStrLen ;

		    while ( pStr > pStr3 ) {

		      c = *pStr ;

		      /* If we find a comma, split the string right there. */
		      if ( c == ',' ) break ;

		      pStr-- ;
		    }

		    if ( pStr - pStr3 > 0 ) {
		      /* We found a comma, split it there */
		      truncStrLen = pStr - pStr3 ;
		    }

		  }
		}

		pStr = (char*) AllocMemory ( truncStrLen + 1 ) ;
		assert ( pStr ) ;

		/*strncpy ( pStr, pStr3, truncStrLen ) ;*/
		memcpy ( pStr, pStr3, truncStrLen ) ;
		pStr[truncStrLen] = '\0' ;
                
		/* Parse the string */
		if ( !isSCR && !isPRE && !isTXT ) 
		  strLen2 = gHyp_util_parseXML ( pStr ) ;
		else
		  strLen2 = truncStrLen ;
  
		pStrData = gHyp_data_new ( NULL ) ;
		gHyp_data_setStr_n ( pStrData, pStr, strLen2 ) ;

		ReleaseMemory ( pStr ) ;

		gHyp_data_append ( pParentTag, pStrData ) ;
	      }
	      strLen3 -= truncStrLen ;
	      pStr3 += truncStrLen ;
	    }
	    pStream += strLen ;

	    /* Get more data if necessary *
	     * Make sure we update "pTag", because it can become invalid
	     * if gHyp_util_readStream does a memmove or memcpy
	     */
#ifdef _DEBUG 
  pMemory = pStream ;
#endif
            i = pTag - pStream ;
	    pStream = gHyp_util_readStream (  pStream, pAnchor, ppEndOfStream,
					    &streamLen, pStreamData, 
					    ppValue, pContext, ss, isVector, 
					    pp ) ;
	    pTag = pStream + i ;
#ifdef _DEBUG 
  if ( pMemory != pStream ) 
    gHyp_util_debug ( "Memory shift, %d bytes",streamLen ) ;

#endif
	  }
	  isFirstLine = FALSE ;
	}
      }

      /* Update pointer, then check tag to see if its a comment or definition. */
      pStream = pTag ;

      /* Get more data if necessary */
#ifdef _DEBUG 
  pMemory = pStream ;
#endif
      i = pTag - pStream ;
      pStream = gHyp_util_readStream (  pStream, pAnchor, ppEndOfStream,
					    &streamLen, pStreamData, 
					    ppValue, pContext, ss, isVector, 
					    pp ) ;
      pTag = pStream + i ;
#ifdef _DEBUG 
  if ( pMemory != pStream ) 
    gHyp_util_debug ( "Memory shift, %d bytes",streamLen ) ;
#endif
    }
    while ( noTAG ) ;

    /* Advance past '<' */
    pStream++ ;
    tagged = TRUE ;

    if ( !*pStream ) {
	gHyp_instance_warning ( pAI, STATUS_XML, "Unexpected end of stream encountered at [%.*s]  ",
				SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE));
        return NULL ;
    }

    isEndTag = ( *pStream == '/' ) ;
    if ( isEndTag ) pStream++ ;

    /* Extract the tag name */
    tagLen = gHyp_util_getToken_okDashDot ( pStream ) ;

    /* A tagLen of zero means that there {was not a valid tag name.
     * A tagLen of greater than 0 is the length of the tag name.
     * If an end tag, we must see if what we are expecting..
     * If a comment tag, we must find the end that looks like '-->'
     * If a definition tag, we only need to find the first '>'
     */
    if ( tagLen > 0 ) {

      strncpy ( tag, pStream, tagLen ) ;
      tag[tagLen] = '\0' ;

      /* For script and style and textarea, we must have a matching </script> or </style> 
	 * in order to accept the tag.
	 * For pre tag, we can only accept tags beloging to the preContent list.
	 * Therefore, if we are anything but SCRIPT or STYLE or TEXTAREA or PRE, we 
	 * must process the tag.
	 */
      if ( isPRE ) {
	sprintf ( tag_lc, "/%s/", tag ) ;
	gHyp_util_lowerCase ( tag_lc, tagLen ) ;
	if ( strstr ( preContent, tag_lc ) ) break ;
	if ( strcmp ( "/pre/", tag_lc ) == 0 && isEndTag ) break ;
      }

      if ( !isSCR && !isTXT && !isPRE) break ;

      /* SCRIPT and STYLE and TEXTAREA tags can have no INNER tags 
       * PRE can only have preContent tags
       */
      pTag2 = gHyp_data_getLabel ( pParentTag ) ;

      if ( isEndTag && strcmp ( tag, pTag2 ) == 0 ) {

	pStream += tagLen ;
        if ( *pStream != '>' ) {
	  gHyp_instance_warning ( pAI, STATUS_XML, "Invalid end tag, expecting '>' [%.*s]...[%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);
	  return NULL ;
	}
	pStream++ ;
	*pIsEndTag = TRUE ;

	/* <SCRIPT> <STYLE>  <TEXTAREA> <PRE> tags should never get converted to <tag /> */
	if ( isSCR || isTXT || isPRE ) {
	  pData = gHyp_data_getLast ( pParentTag ) ;
	  if ( pData && gHyp_data_getDataType ( pData ) == TYPE_ATTR ) {
	    pData = gHyp_data_new ( NULL ) ;
	    gHyp_data_append( pParentTag, pData ) ;
	  }
	}
	return pStream ;
      }
      pTag = NULL ;
      continue ;
    }

    /* Check for end tags, comments and XML definitions */
    inComment = ( strstr ( pStream, "!--" ) == pStream ) ;
    inDefinition = ( *pStream == '?' || *pStream == '!' ) ;

    if ( !inComment && !inDefinition ) {
      if ( !isSCR && !isTXT )
	gHyp_instance_warning ( pAI, STATUS_XML, "Unexpected tag, skipping [%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE));
      pTag = NULL ;
    }
    else {

      /* Find end of comment or end of definition */
      if ( inComment ) 
	pTag = strstr ( pStream, "-->" ) ;
      else
	pTag = strchr ( pStream, '>' ) ;

      if ( !pTag ) {
	if ( inComment )
	  gHyp_instance_warning ( pAI, STATUS_XML, "No comment end found in XML stream [%.*s]",
				      SEG_SIZE,pAnchor);
	else
	  gHyp_instance_warning ( pAI, STATUS_XML,"No end of defintion found XML stream [%.*s]",
				   SEG_SIZE,pAnchor);
	return NULL ;
      }
      else {

	/* End of comment of definition. 
	 * For script and style and textarea tags, the comments are included verbatim.
	 */

	/* Step out of comment or definition */  
	pStr = pStream-1; 
	tagged = FALSE ;
	if ( inComment ) 
	  pStream = pTag + 3 ; 
	else
	  pStream = pTag + 1 ;

        if ( isSCR || isTXT ) {

          /* The content must be taken literal */

	  /*gHyp_util_debug("processing %s",pTag);*/
	  while ( pStr < pStream ) {

            /* If at the end of a line, step over it, but only one line */
	    if ( *pStr == '\n' )
	      pStr += ( *(pStr+1) == '\r' ) ? 2 : 1 ;
	    else if ( *pStream == '\r' )
	      pStr += ( *(pStr+1) == '\n' ) ? 2 : 1 ;

	    /* Span the contents of the next line, getting its length */
	    strLen = strcspn ( pStr, "\n\r" ) ;

            /* The total length cannot be past the end of the content */
	    if ( strLen > (pStream-pStr) ) strLen = pStream - pStr ;

            /* If there is content, break it up further if necessary */
            if ( strLen > 0 ) {

	      strLen3 = strLen ;
	      pStr3 = pStr ;
	      while ( strLen3 > 0 ) {

		truncStrLen = MIN ( INTERNAL_VALUE_SIZE, strLen3 ) ;
		if ( truncStrLen > 0 ) {

		  if ( truncStrLen == INTERNAL_VALUE_SIZE && strLen3 > INTERNAL_VALUE_SIZE ) {

		    /* Let's not cut words in half if we can help
		     * it. Make the string a little shorter if we find a space
		     * between non-spaces.
		     * Start at the end of the string, search backwards over
		     * non-spaces until we find a space.  If so, and its
		     * not the beginning of the string, then we split the
		     * string.
		     * 
		     */
		    pStr2 = pStr+truncStrLen ;

		    while ( pStr2 > pStr ) {

		      c = *pStr2 ;
		      /* If we find a space, split the string right here. */
		      if ( c == ' ' ) break ;
		      pStr2-- ;
		    }

		    /* Adjust truncStrLen if necessary */
		    if ( pStr2 - pStr > 0 ) {
		      truncStrLen = pStr2 - pStr ;
		    }
		    else {
		      /* Look for comma */
		      pStr2 = pStr+truncStrLen ;

		      while ( pStr2 > pStr ) {

		        c = *pStr2 ;
		        /* If we find a comma, split the string right here. */
		        if ( c == ',' ) break ;
		        pStr2-- ;
		      }

		      /* Adjust truncStrLen if necessary */
		      if ( pStr2 - pStr > 0 ) {
		        truncStrLen = pStr2 - pStr ;
		      }
		    }
		  }

		  pStr2 = (char*) AllocMemory ( truncStrLen + 1 ) ;
		  assert ( pStr2 ) ;

		  strncpy ( pStr2, pStr3, truncStrLen ) ;
		  pStr2[truncStrLen] = '\0' ;
  
		  pStrData = gHyp_data_new ( NULL ) ;
		  gHyp_data_setStr_n ( pStrData, pStr2, truncStrLen ) ;

		  ReleaseMemory ( pStr2 ) ;
		  gHyp_data_append ( pParentTag, pStrData ) ;
		}
		strLen3 -= truncStrLen ;
		pStr3 += truncStrLen ;
	      }
	      pStr += strLen ;
	    }
	    else {
		pStr += strspn ( pStr, "\n\r" ) ;
	    }
          }
	}


	if ( isEndTag )
	  gHyp_instance_warning ( pAI, STATUS_XML, "Unexpected end tag, skipping [%.*s]...[%.*s]",
		  		  SEG_SIZE,MAX(pAnchor,pTag-SEG_SIZE),SEG_SIZE,pTag);

	pTag = NULL ;

        /* Get more data if necessary */
#ifdef _DEBUG 
	pMemory = pStream ;
#endif
        pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
						&streamLen, pStreamData, 
						ppValue, pContext, ss, isVector, 
						pp ) ;
#ifdef _DEBUG 
	if ( pMemory != pStream ) 
	  gHyp_util_debug ( "Memory shift, %d bytes",streamLen ) ;
#endif

      }
    }
  }

  /* The pStream points to the first character of the tag name after the < or </,
   * and tagLen is greater than zero.
   *
   * This function is called recursively so we may be pointing to
   * a tag at Nth recursize depth.  
   *
   * We need to extract out the <tag> name. 
   */

  /* Create the valid tag name */
  strncpy ( tag, pStream, tagLen ) ;
  tag[tagLen] = '\0' ;
  

  /*gHyp_util_debug("Processing %s",tag);*/


  strcpy ( tag_lc, tag ) ;
  gHyp_util_lowerCase ( tag_lc, tagLen ) ;

  /* If this is <html>, then we can relax the rules about childless tags */
  if ( pParentTag == NULL && strcmp ( tag_lc, "html" ) == 0 ) {
     /*gHyp_util_debug("Not PURE XML");*/

    /* This does not work correctly yet, so I am disabling it 
     isPureXML = FALSE ;
     */
  }

#ifdef _DEBUG 
  pMemory = pStream ;
#endif
  /* Check if more of the stream is needed */
  pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
					&streamLen, pStreamData, 
					ppValue, pContext, ss, isVector, 
					pp ) ;
#ifdef _DEBUG 
  if ( pMemory != pStream ) 
    gHyp_util_debug ( "Memory shift, %d bytes",streamLen ) ;
#endif

  if ( isEndTag ) {

    /* If we encountered an end tag, then one of the following cases will apply.
     * 
     * 1. It will match pParentTag, we will set *pEndTag = TRUE return.
     * 2. It will not match and we will need to deal with that condition.
     */

    if ( pParentTag == NULL ) {
      strcpy ( tag2_lc, tag_lc ) ;
      pExpectedEndTag = tag2_lc ;
    }
    else {
      
      pExpectedEndTag = gHyp_data_getLabel ( pParentTag ) ;
      strcpy ( tag2_lc, pExpectedEndTag ) ;
      gHyp_util_lowerCase ( tag2_lc, strlen ( tag2_lc ) ) ;
    }

    if ( strcmp ( tag_lc, tag2_lc ) == 0 ) {

      /* Matched ending tag */
      pStream += tagLen ;

      /* Check for '>' */
      if ( *pStream != '>' ) {
	gHyp_instance_warning ( pAI, STATUS_XML, "Invalid end tag, expecting '>' [%.*s]...[%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);
        return NULL ;
      }

      pStream++ ;

      if ( requestedType != TYPE_NULL ) {

	skipAttr = TRUE ;

	/* Examples: 
	 *	<x datatype="list">String 99 1.23e45 0x3f 0o037</x>
	 *	<y datatype="float">1 2 3</y> 
	 *	<z datatype="str">A list of strings\nseparated by newline</z>
	 */
  
	/* If any of the children are variables, then don't use gettoken */
	pValue = NULL ;
	sss = -1; context = -1 ;
	hasVariable = FALSE ;
	while ( (pValue = gHyp_data_nextValue ( pParentTag, 
						  pValue, 
						  &context,
						  sss ) ) ) {
	  /* Skip over the attr */
	  if ( skipAttr && gHyp_data_getDataType ( pValue ) == TYPE_ATTR ) continue ;
	  skipAttr = FALSE ;
	  if ( gHyp_data_getTokenType ( pValue ) == TOKEN_VARIABLE ) {
		hasVariable = TRUE ;
		break ;
	  }
	}

	if ( !hasVariable ) {

		/* Extract the contents, first as tokens */
		pSrc = gHyp_data_new ( "_vector_" ) ;

		skipAttr = TRUE ;
		pValue = NULL ;
		sss = -1; context = -1 ;
		while ( (pValue = gHyp_data_nextValue ( pParentTag, 
						  pValue, 
						  &context,
						  sss ) ) ) {

		  /* Skip over the attr */
		  if ( skipAttr && gHyp_data_getDataType ( pValue ) == TYPE_ATTR ) continue ;
		  skipAttr = FALSE ;

		  /* Get the buffer contents */
		  n = gHyp_data_getStr (	pValue, 
						buffer, 
						MAX_INPUT_LENGTH, 
						context, 
						FALSE  ) ;

		  if ( requestedType != TYPE_STRING )
		    /* Parse each element into separate tokens */
		    gHyp_fileio_getTokens ( buffer, pSrc ) ;
		  else
		    gHyp_data_append ( pSrc, gHyp_data_copy ( pValue ) );
		    
		}

		/* Now assign pSrc back to pParentTag */
		pLvalue = gHyp_data_new ( NULL ) ;
		gHyp_data_setReference (  pLvalue, 
					    gHyp_data_getLabel ( pParentTag ),
		  			    pParentTag ) ;
		pResult = gHyp_type_assign( pAI,
					      gHyp_instance_frame ( pAI ),
					      pLvalue,
					      pSrc,
					      requestedType,
					      FALSE,  
					      FALSE  ) ; 
		gHyp_data_delete ( pResult ) ;

		gHyp_data_delete ( pLvalue ) ;
		gHyp_data_delete ( pSrc ) ;

	}
      }

      *pIsEndTag = TRUE ;
      return pStream ;

    }
    else {

      if ( mayReturnChildLess ) {

	/* Set from the level above, the tag above can be childless,
	 * so we can accept this ending tag as valid and continue.
	 */
        pStream += tagLen ;

        /* Check for '>' */
        if ( *pStream != '>' ) {
	  gHyp_instance_warning ( pAI, STATUS_XML, "Invalid end tag, expecting '>' [%.*s]...[%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);
          return NULL ;
	}
        pStream++ ;
	*pIsEndTag = TRUE  ;
	return pStream ;
      }

      /* Ending tag does not match. */
      gHyp_instance_warning ( pAI, STATUS_XML,"Unexpected end tag </%s>, expecting </%s> [%.*s]...[%.*s]",
			      tag,pExpectedEndTag,
			      SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream) ;

      /* Go back and try to fix any improperly nested tags.
       *
        * This example contains 4 problems.  The 2 <INPUT>
       * tags do not have ends, the </FORM> end tag is
       * reversed with the </TBODY> tag, and the </CENTER>
       * tag has no start tag to match.
       *
       *    <TABLE>
       *      <FORM>
       *        <TBODY>
       *          <INPUT type="hidden" value="a" name=b">
       *          <INPUT type="hidden" value="m" name=n">
       *        </FORM>
       *       </TBODY>
       *      </CENTER>
       *     </TABLE>
       *
       * When the </FORM> is first encountered, the tree looks like:
       *
       *    <TABLE>
       *      <FORM>
       *        <TBODY>
       *          <INPUT type="hidden" value="a" name=b">
       *            <INPUT type="hidden" value="m" name=n">
       *
       *            </FORM>   <---- where we first encounter a problem
       *
       *        </TBODY>
       *     </TABLE>
       *
       * Here is our state:
       * 1. The current pParentTag node is at the 1st <INPUT> tag.
       * 2. The current pChildTag node is at the 2nd <INPUT> tag.
       * 3. The current unexpected tag is </FORM>
       *
       * We fix the HTML as follows:
       * ITERATION 1, Unexpected </FORM>
       * a) Take the 2nd INPUT pChildtag and look for grandchildren.  
       * b) There are none.
       * c) Assume the <INPUT> tag has ended.
       * d) pParentTag becomes <TBODY>, pChildTag becomes the 1st <INPUT> tag.
       *
       * ITERATION 2, Unexpected </FORM>
       * a) Take the 1st INPUT pChildTag and look for grandchildren.  
       * b) The grandchild is the 2nd INPUT tag.
       * c) Append this grandchild to the parent tag. Ie: Append the
       *    2nd INPUT tag to the TBODY tag.
       * d) End the <INPUT> tag.
       * e) pParentTag becomes <FORM> and pChildTag becomes <TBODY>
       *
       * ITERATION 3, Unexpected </FORM>
       * a) The <FORM> tag matches the pParentTag, and we detect it is reversed with 
       *    the <TBODY> tag.
       * b) Switch the tags. 
       * c) pParentTag becomes <FORM>
       *
       */

      if ( !pParentTag ) {

	/* We're at the top.  Reject the tag and continue the parse */
	gHyp_instance_warning ( pAI, STATUS_XML,"Discarding end tag </%s>. Continuing...[%.*s]",
				tag,
				SEG_SIZE,pStream) ;
	/* Ending tag */
	pStream += tagLen ;

	/* Check for '>' */
	if ( *pStream != '>' ) {
	  gHyp_instance_warning ( pAI, STATUS_XML, "Invalid end tag [%.*s]...[%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);
          return NULL ;
	}

	pStream++ ;
	/* Continue processing */

	*pIsEndTag = FALSE ;
	return pStream ;

      }
      else {
	   
	/* There is a parent.  Figure out why the tag is wrong */
	      	    
	/***********************************************************************************
	 * Look for next ending tag past the one we are having trouble matching.
	 * If encountering errors, don't report them, because we are just 
	 * looking ahead.
	 */
	pStr = pStream + tagLen ;
	tag2Len = 0 ;

	/* Check for '>' */
	if ( *pStr == '>' ) {

	  pTag2 = NULL ;
	  tag2Len = 0 ;
	  while ( !pTag2 ) {

	    pTag2 = strchr ( pStr, '<' ) ;
	    if ( !pTag2 ) break ;

	    /* Found something.  Check if its a comment or definition. */
	    pStr = pTag2 ;
	    pStr++ ;
	    if ( !*pStr ) { pTag2 = NULL; break ; }

	    /* Check for end tags, comments and XML definitions */
	    isEndTag = ( *pStr == '/' ) ;
	    inComment = ( strstr ( pStr, "!--" ) == pStr ) ;
	    inDefinition = ( *pStr == '?' || *pStr == '!' ) ;

	    if ( isEndTag ) {
	      /* It's what we want, hoping for the reversed tag */
	      pStr++ ;
	      tag2Len = gHyp_util_getToken_okDashDot ( pStr ) ;
	      if ( tag2Len > 0 ) break ;
	      pTag2 = NULL ;
	      break ;

	    }
	    else if ( !inComment && !inDefinition ) {
	      /* No end tag found, we didn't find it */
	      pTag2 = NULL ;
	      break ;
	    }
	    else {
	      /* Skip pase comment or end of definition */
	      if ( inComment ) 
	        pTag2 = strstr ( pStr, "-->" ) ;
	      else
	        pTag2 = strchr ( pStr, '>' ) ;
 	      if ( !pTag2 ) break ;

    	      /* Step out of comment or definition or end tag and keep searching for a start tag */
	      if ( inComment ) 
	        pStr = pTag2 + 3 ; 
	      else
	        pStr = pTag2 + 1 ;

	      pTag2 = NULL ;
	    }
	  }
	}

	/***********************************************************************************
	 * Look at the next ending tag, if we found it above. 
	 */
	if ( tag2Len > 0 ) {
	      
	  strncpy ( tag2, pStr, tag2Len ) ;
	  tag2[tag2Len] = '\0' ;

	  /* Is the unexpected tag found at the next higher level? 
	   * If so, then the expected tag may be reversed with the unexpected tag. 
	   * We have to find out.
	   */
	  if (	pGrandParentTag &&
		strcmp ( gHyp_data_getLabel(pGrandParentTag), tag ) == 0 ) {

	    if ( strcmp ( tag2, pExpectedEndTag ) == 0 ) {
	      gHyp_instance_warning ( pAI, STATUS_XML,"Ending tag </%s> is reversed with </%s>",
				  pExpectedEndTag,tag) ;
	      /* Switch tags, and exit at the location of the next tag. */
	      pTag = pStr = pTag2 + tag2Len - tagLen ;
	      *pStr++ = '<' ;
	      *pStr++ = '/' ;
	      strncpy ( pStr, tag, tagLen ) ;
	      *pIsEndTag = TRUE ;
	      return pTag ;
	    }
	  }
	}
	     
	/***********************************************************************************
	 * Not reversed. Try looking for the missing tag at a higher level. 
	 */
	pData = pParentTag ;
	found = FALSE ;

	while ( !found ) {
	  pUpperTag = gHyp_data_getParent ( pData ) ;
	  strcpy ( tag2_lc, gHyp_data_getLabel(pUpperTag) ) ;
	  gHyp_util_lowerCase ( tag2_lc, strlen ( tag2_lc ) ) ;
	  if ( strcmp ( tag_lc, tag2_lc ) == 0 )
	    found = TRUE ;
	  else {
	    pData = pUpperTag ;
 	    pUpperTag = gHyp_data_getParent ( pData ) ;
	    if ( pData == pUpperTag ) break ;
	  }
	}
	if ( !found ) {
	  gHyp_instance_warning ( pAI, STATUS_XML,"Inserting tag <%s> after <%s>",
				tag,gHyp_data_getLabel(pParentTag) ) ;
	  pData = gHyp_data_new ( tag ) ;
	  gHyp_data_moveValues ( pData, pParentTag ) ;
	  gHyp_data_append ( pParentTag, pData ) ;
	  *pIsEndTag = FALSE ;
	  return pTag2 ; 
	}

	/***********************************************************************************
	 * The unexpected tag is not reversed and not found at a higher level.
	 * First just assume that the current tag value should really just
	 * have ended with a "/>".  This is confirmed with the tag being contained
	 * in the childLess list.
	 */
	if ( pGrandParentTag ) {
	  
	  sprintf ( tag2_lc, "/%s/", gHyp_data_getLabel(pParentTag) ) ;
	  gHyp_util_lowerCase ( tag2_lc, strlen ( tag2_lc ) ) ;

	  if ( strstr ( childLess, tag2_lc ) ) {

	    // Move the children of the tag we are ending up to the grandparent.
	    gHyp_instance_warning ( pAI, STATUS_XML,"Assuming childless tag <%s />",pExpectedEndTag);
	    pChildTag = NULL ; 
	    ss2 = -1 ; 
	    context2 = -1 ;
	    while ( (pChildTag = gHyp_data_nextValue ( pParentTag,
							  pChildTag,
							  &context2,
							  ss2 ) ) ) {

	      dataType = gHyp_data_getDataType(pChildTag) ;

	      /* Skip past the attributes */
	      if ( dataType == TYPE_ATTR ) continue ;
		
	      gHyp_data_detach ( pChildTag ) ;
	      
	      /*
	      gHyp_instance_warning ( pAI, STATUS_XML, 
		     "Appending element <%s> from <%s> to <%s> [%.*s]...[%.*s]",
		      gHyp_data_getLabel(pChildTag),
		      gHyp_data_getLabel(pParentTag),
		      gHyp_data_getLabel(pGrandParentTag),
		      SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),
		      SEG_SIZE,pStream) ;
	      */
	      gHyp_data_append ( pGrandParentTag, pChildTag ) ;

	      /* Start over from the beginning */
	      pChildTag = NULL ; 
	    }
	  }
	}
	/***********************************************************************************
	 * Go back to previous position at start of misplaced tag 
	 */
	*pIsEndTag = TRUE ; 
	return pTag ;
      }
    }
    /*****************************************************************************/
  }

  /* A null argument indicates the start of an new XML stream.
   * If the stream is <tag1></tag1><tag2></tag2>, then two XML
   * streams will be found, once for tag1 and once for tag2.
   * If the stream is <tag1><tag2></tag2></tag1>, then only one
   * XML stream will be found, just for tag1.
   */

  if ( pParentTag == NULL ) {

    /* Another tag at root level. Create the variable. */
    /*gHyp_util_debug("Creating root tag <%s>",tag);*/
    if ( pTV ) {
      pChildTag = gHyp_data_new ( tag ) ;
      gHyp_data_append ( pTV, pChildTag ) ;
    }
    else {
      pChildTag = gHyp_frame_createVariable ( pAI, pFrame, tag ) ;
      gHyp_data_deleteValues ( pChildTag ) ;
      /* If its a vector, convert it back to a string */
      gHyp_data_setVariable ( pChildTag, tag, TYPE_LIST ) ;

    }
    /* Create or retrieve the "xmlargs" variable, adding the new tag name. */
    pArgs = gHyp_frame_createVariable ( pAI, pFrame, "xmlargs" ) ;
    if ( pArgs ) {
      pData = gHyp_data_new ( NULL ) ;
      gHyp_data_setReference ( pData, tag, NULL ) ;
      gHyp_data_append ( pArgs, pData ) ;
    }
  }
  else {
    /* Create new tagData, appending to existing tag variable */
    pChildTag = gHyp_data_new ( tag ) ;
    gHyp_data_append ( pParentTag, pChildTag ) ;
  }

#ifdef _DEBUG 
  pMemory = pStream ;
#endif
  /* Check if more of the stream is needed */
  pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
					&streamLen, pStreamData, 
					ppValue, pContext, ss, isVector, 
					pp ) ;
#ifdef _DEBUG 
  if ( pMemory != pStream ) 
    gHyp_util_debug ( "Memory shift, %d bytes",streamLen ) ;
#endif

   /* Attributes are allowed until the end of the tag */
  allowAttributes = TRUE ;

  sprintf ( tag2_lc, "/%s/", tag ) ;
  gHyp_util_lowerCase ( tag2_lc, strlen ( tag2_lc ) ) ;
  isChildLess = ( strstr ( childLess, tag2_lc ) != NULL ) ;

  /* Advance past tag name */
  pStream += tagLen ;
  while ( *pStream ) {
    
    if ( allowAttributes ) {
      /* This is always the case at the start of this loop.
       * Span whitespace after tag name 
       */
      span = strspn ( pStream, " \t\n\r" ) ;
      pStream += span ;
    }
    else
      span = 0 ;
     
    if ( (*pStream == '/' && *(pStream+1) == '>') ) {

      /* A tag with no data, ending with />, with zero or more attributes */
      pStream++ ;
      if ( *pStream == '>' ) return pStream ;

    }
    else if ( *pStream == '>' ) {

      /* End of current tag start.  Get contents of tag */
      pStream++ ;

      allowAttributes = FALSE ;

      /* For HTML specific childless tags, we can assume they have no children.
       * In pure XML, this would be a mistake
       */
      if ( isChildLess && !isPureXML ) {

	/* The tag, for example a <P> may or may not have an ending tag </P>
	 * In less than pure HTML, we won't complain if the ending tag is not
	 * there. So, here we set a flag to pass down to the level below,
	 * so that if we end up not finding the </p>, we can return 
	 * success, allowing the childless tag condition.
	 */
	maybeReturnChildLess = TRUE ;
      }
    }
    else {

      /* The stuff inside the tag, ie: <TAG> ...stuff...</TAG> */

      /* Collect attributes so long as we can (and we are not in a script or style or textarea */
      if ( allowAttributes && !isSCR && !isTXT ) {

        /* Pull out attributes value */
        attrLen = gHyp_util_getToken_okDashDot ( pStream ) ;
        if ( attrLen <= 0 ) {
          gHyp_instance_warning ( pAI, STATUS_XML, "Invalid attribute name [%.*s]...[%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);

	  /* Span non-whitespace to find the end of the invalid value */
	  attrLen = strcspn ( pStream, " \t>" ) ;
	  if ( attrLen <= 0 ) return NULL ;
	}
      
        /* Found an attribute */
	if ( attrLen > MAX_INPUT_LENGTH ) {
          gHyp_instance_warning ( pAI, STATUS_XML,"Attribute name greater than %d characters, it has been truncated [%.*s]...[%.*s]",
				 MAX_INPUT_LENGTH, SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);

	}

	attrLen = MIN ( MAX_INPUT_LENGTH, attrLen ) ;
        strncpy ( attr, pStream, attrLen ) ;
        attr[attrLen] = '\0' ;

        /*gHyp_util_debug("Got attr %s",attr);*/

        /* Create the attribute variable */
        pAttrData = gHyp_data_new ( attr ) ;

        pStream += attrLen ;

        /* Span whitespace after attribute name */
        span = strspn ( pStream, " \t\n\r" ) ;
        pStream += span ;

        /* The "=" sign should be next */
        if ( *pStream != '=' ) {

	  gHyp_instance_warning ( pAI, STATUS_XML, "No ending '\"' on prev attr or new attr '%s' has no '=' [%.*s]...[%.*s]",
				 attr, SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);

	  /* Create the attribute space */
	  gHyp_data_newVector ( pAttrData, TYPE_ATTR, 1, TRUE ) ;
	  c = '1' ;
	  gHyp_data_setVectorRaw ( pAttrData, &c, 0 ) ;
	  gHyp_data_append ( pChildTag, pAttrData ) ;

	}
        else { 

	  /* Get the attribute value */
	  pStream++ ;

#ifdef _DEBUG 
  pMemory = pStream ;
#endif
	  /* Get more data if necessary */
	  pStream = gHyp_util_readStream (  pStream, pAnchor, ppEndOfStream,
					    &streamLen, pStreamData, 
					    ppValue, pContext, ss, isVector, 
					    pp ) ;
#ifdef _DEBUG 
  if ( pMemory != pStream ) 
    gHyp_util_debug ( "Memory shift, %d bytes",streamLen ) ;
#endif
	  /* Span whitespace after "=" */
	  span = strspn ( pStream, " \t\n\r" ) ;
          pStream += span ;

	  if ( *pStream != '"' && *pStream != '\'' ) {

	    gHyp_instance_warning ( pAI, STATUS_XML, "No starting \" on attribute [%.*s]...[%.*s]",
				   SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);

	    /* Span non-whitespace (and ">") to find the end of the attribute value */
	    strLen = strcspn ( pStream, " \t>" ) ;
	    
	    /* Set end of attribute value */
	    pStr = pStream + strLen ;

	    if ( strLen > 0 && *pStr == '>' ) pStr-- ;

	  }
	  else {

	    quote = *pStream ;

	    if ( quote == '\'' )
	      gHyp_instance_warning ( pAI, STATUS_XML, "Attribute using single quote \'[%.*s]...[%.*s]",
				   SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);
	    
	    /* Found starting quote, extract attribute value */
	    pStream++ ;

	    terminated = FALSE ;
	    pSearch = NULL ;
	    while ( !terminated ) {

#ifdef _DEBUG 
  pMemory = pStream ;
#endif
              /* Get more data if necessary */
	      pStream = gHyp_util_readStream (  pStream, pAnchor, ppEndOfStream,
					    &streamLen, pStreamData, 
					    ppValue, pContext, ss, isVector, 
					    pp ) ;
#ifdef _DEBUG 
  if ( pMemory != pStream ) 
    gHyp_util_debug ( "Memory shift, %d bytes",streamLen ) ;
#endif
	      if ( pSearch == NULL ) pSearch = pStream ;

	      pStr = strchr ( pSearch, quote ) ;
	      
	      /* If no ending quote, quit, returning pointer to start */
	      if ( !pStr ) {
		gHyp_instance_warning ( pAI, STATUS_XML,"No ending quote [%.*s]...[%.*s]",
				       SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);

		/* Span across non-whitespace */
		strLen = strcspn ( pSearch, " \t" ) ;

 		/* Set end of attribute value */
		pStr = pSearch + strLen ;
		terminated = TRUE ;
	      }
	      else {
		/* If the ending quote is preceeded by a backslash, then its not
		 * the ending quote.
		 */
		if ( *(pStr-1) == '\\' ) {
		  /* Not a correct terminator,  keep searching. */
		  pSearch = pStr+1 ;
		}
		else
		  terminated = TRUE ;

		strLen = pStr - pStream ;
	      }

	    }
	  }
	  /*gHyp_util_debug("Got value %.*s",strLen, pStream );*/

	  /* Extract string - maximum of MAX_INPUT_LENGTH characters. */
	  truncStrLen = MIN ( MAX_INPUT_LENGTH, strLen ) ;

	  /* Create the attribute space */
	  gHyp_data_newVector ( pAttrData, TYPE_ATTR, truncStrLen, TRUE ) ; 

	  if ( truncStrLen > 0 ) {

	    pStr2 = (char*) AllocMemory ( truncStrLen + 1 ) ;
	    assert ( pStr2 ) ;
	  
	    strncpy ( pStr2, pStream, truncStrLen ) ;
	    pStr2[truncStrLen] = '\0' ;

	    /* Check for type attribute. */
	    gHyp_util_lowerCase ( attr, strlen ( attr ) ) ;
	    if ( strcmp ( attr, "datatype" ) == 0 ) {
	      reqType = gHyp_fileio_dataType ( pStr2 )  ;
	    }
	  
	    for ( i=0; i<truncStrLen; i++ ) {
	      c = *(pStr2+i) ;
	      gHyp_data_setVectorRaw ( pAttrData, &c, i ) ;
	    }
	    ReleaseMemory ( pStr2 ) ;	    
	  }
	  gHyp_data_append ( pChildTag, pAttrData ) ;

	  /* Advance past end of attribute value. */
	  pStream = pStr + 1 ;
	}
      }
      else {

	/* No longer in attributes section.
	 * This is where we either go deeper into the XML structure,
	 * calling this same function recursively, or we find an ending
	 * tag and return from this function.
	 */

	/* Determine if we are subject to being inside a SCRIPT/STYLE tag or a
	 * TEXTAREA/PRE tag.  The flag determines how will be process whitespace.
	 * For SCRIPT/STYLE, we must find an ending tag, everything without exception
	 * between the start/end tags be taken "as-is".
	 * For TEXTAREA/PRE, we do process internal tags, but otherwise text is taken
	 * "as-is".
	 */
	if ( !isSCR ) isSCR = ( strcmp (tag_lc,"script") == 0 || strcmp(tag_lc,"style")==0 ) ;
	if ( !isPRE ) isPRE = ( strcmp (tag_lc,"pre")==0 ) ;
	if ( !isTXT ) isTXT = ( strcmp (tag_lc,"textarea") == 0 ) ;

	/* Call this function recursively 
	 *
	 */
	isEndTag = FALSE ;
	pStream = gHyp_cgi_parseXML ( pStream,
				      pAnchor,
				      ppEndOfStream,
				      pStreamData,
				      ppValue,
				      pContext,
				      ss,
				      isVector,
				      isSCR,
				      isPRE,
				      isTXT,
				      pChildTag,
				      pParentTag,
				      &isEndTag,
				      pFrame,
				      pTV,
				      pp,
				      pAI,
				      reqType,
				      isPureXML,
				      maybeReturnChildLess ) ;

	if ( !pStream ) return NULL ;
	if ( isEndTag ) return pStream ;
      }
    }
  }
  return pStream ;
}

void gHyp_cgi_xmlData ( sData *pData, sInstance *pAI, sFrame *pFrame, sData *pTV ) 
{    
  int
    ss,
    context ;
    
  char
    *pStream,
    *pAnchor,
    *pEndOfStream ;

  sLOGICAL
    isPureXML = TRUE,
    mayReturnChildLess = FALSE,
    isEndTag,
    isVector ;

  sData
    *pValue ;

  isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;
  ss = gHyp_data_getSubScript ( pData ) ; 
  context = -1 ;
  gzStream[0] = '\0' ;
  pStream = gzStream ;
  pAnchor = pStream ;
  pEndOfStream = pStream ;
  pValue = NULL ;
  gHyp_data_deleteValues ( gHyp_frame_createVariable ( pAI, pFrame, "xmlargs" ) ) ;

  /*gHyp_util_debug("Content-type: text/html\n\n");*/
  while ( TRUE ) {
    pStream = gHyp_cgi_parseXML ( pStream,
				  pAnchor,
				  &pEndOfStream,
				  pData,
				  &pValue,
				  &context,
			 	  ss,
				  isVector,
				  FALSE,
				  FALSE,
				  FALSE,
				  NULL,
				  NULL,
				  &isEndTag,
				  pFrame,
				  pTV,
				  NULL,
				  pAI,
				  TYPE_LIST,
				  isPureXML,
				  mayReturnChildLess ) ;
    if ( !pStream ) break ;
    if ( !*pStream || !pEndOfStream ) break ;
    pAnchor=pStream ; 
  }
}

void gHyp_cgi_xml ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: xml ( string )
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
      *pData;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: xml ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

    /* Parse the xml */
    gHyp_cgi_xmlData ( pData, pAI, pFrame, NULL) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

static int lHyp_cgi_init( sInstance *pAI, sFrame *pFrame )
{
  int
    n,
    cl, 
    i;
 
  char
    contentType[VALUE_SIZE+1];

  sBYTE
    isIMG=FALSE,
    isCGI=FALSE,
    isXML=FALSE,
    isHYP=FALSE ;

  sData
     *pParentTag,
     *pContentData;

  char
    *pEnv,
    boundary[MAX_INPUT_LENGTH+1] ;

  /* Default, no errors, no name/value pairs ("entries"): */
  giCgiErrno = CGIERR_NONE;
  giCgiNumEntries = 0;

  /* Check for REQUEST_METHOD (set by HTTP server): */
  
  if ( (pEnv = getenv ( "REQUEST_METHOD" )) == NULL ) {

    /* None set?  Assume the user is invoking the CGI from a shell prompt
     * (for debugging): 
     */
    giCgiRequestMethod = CGIREQ_NONE;
  }
  else {
    /* Determine the exact request method, and grab the data (if any)
       in the appropriate manner: */
    
    if ( strcmp ( pEnv, "POST") == 0 ) {

      /* Post method (data is sent to us via "stdin"): */
      
      giCgiRequestMethod = CGIREQ_POST;

      isCGI = TRUE ;

      pEnv = getenv( "CONTENT_TYPE" ) ;
      contentType[0] = '\0' ;
      if ( pEnv ) {
        strncpy ( contentType, pEnv, VALUE_SIZE ) ;
        contentType[VALUE_SIZE] = '\0' ;
      }
      n = gHyp_util_trim ( contentType ) ;
      if ( n > 0 ) {
        isIMG = ( strstr(contentType,"multipart/form-data")!=NULL) ;
        isHYP = ( strstr(contentType,"application/hyperscript")!=NULL) ;
        isCGI = ( strstr(contentType,"application/x-www-form-urlencoded")!=NULL) ;
        isXML = ( strstr ( contentType, "text/xml" ) != NULL ||
		strstr ( contentType, "text/html" ) ||
		strstr ( contentType, "application/soap" ) != NULL ) ;
      }

      if ( !isCGI && !isXML && !isHYP && !isIMG ) {
	/* Is the content type incorrect (or not set at all?) */	  
	giCgiErrno = CGIERR_INCORRECT_TYPE;
	return(giCgiErrno);
      }
      else {
		
	/* How much data do we expect? */
	
	if ( (pEnv = getenv ( "CONTENT_LENGTH" )) == NULL ||
	     sscanf ( pEnv, "%d", &cl) != 1) {
	  
	  giCgiErrno = CGIERR_BAD_CONTENT_LENGTH;
	  return(giCgiErrno);
	}
	
	if ( isHYP ) {

          /* For DOM usage.  The program code is not interpreted */

	  /* List out the program contents
	  pBuf = buf ;
	  while ( pBuf ) {
	    pBuf = fgets ( buf, MAX_INPUT_LENGTH, stdin ); 
	    if ( feof ( stdin ) ) break ;
	    gHyp_util_output2 ( pBuf ) ;
	  }
	  */

          /* Ignore the code */
	  return(CGIERR_NONE);
	}

	/* Create space for it: */	  
	gzCgiQuery = (char*) AllocMemory(cl + 1);
	
	if (gzCgiQuery == NULL) {
	  giCgiErrno = CGIERR_OUT_OF_MEMORY;
	  return(giCgiErrno);
	}
	
	/* Read it in: */
        n = fread ( gzCgiQuery, 1, cl, stdin ); 

	/*fgets(gzCgiQuery, cl + 1, stdin);*/
	
	/* Verify that we got as much data as we expected: */
	if (n != (unsigned) cl)
	  giCgiErrno = CGIERR_CONTENT_LENGTH_DISCREPANCY;
	else
          gzCgiQuery[cl] = '\0' ;

      }
    }
    else if ( strcmp ( pEnv, "GET") == 0 ) {
      
      isCGI = TRUE ;
      contentType[0] = '\0' ;
      pEnv = getenv( "CONTENT_TYPE" ) ;
      if ( pEnv ) {
        strncpy ( contentType, pEnv, VALUE_SIZE ) ;
        contentType[VALUE_SIZE] = '\0' ;
      }
      n = gHyp_util_trim ( contentType ) ;

      if ( n > 0 ) {
        /* GET method (data sent via "QUERY_STRING" env. variable): */	
        isIMG = ( strstr(contentType,"multipart/form-data")!=NULL) ;
        isHYP = ( strstr(contentType,"application/hyperscript")!=NULL) ;
        isCGI = ( strstr(contentType,"application/x-www-form-urlencoded")!=NULL) ;
        isXML = ( strstr ( contentType, "text/xml" ) != NULL ||
		strstr ( contentType, "text/html" ) ||
		strstr ( contentType, "application/soap" ) != NULL ) ;
      }
      else
        isCGI = TRUE ;

      giCgiRequestMethod = CGIREQ_GET;
      
      /* Get a pointer to the data: */
      
      gzCgiQuery = getenv("QUERY_STRING");
      
      if (gzCgiQuery == NULL)  {
	/* Does the "QUERY_STRING" env. variable not exist!? */
	
	giCgiErrno = CGIERR_NULL_QUERY_STRING;
	
	return(giCgiErrno);
      }
      else {
	
	/* Determine the content length by seeing how big the	string is: */
	cl = strlen(gzCgiQuery);
      }
    }
    else {
      
      /* Something else? We can't handle it! */	
      giCgiRequestMethod = CGIREQ_UNKNOWN;
      giCgiErrno = CGIERR_UNKNOWN_METHOD;
      giCgiNumEntries = 0;
      return(giCgiErrno);
    }

    if ( isXML ) {

      pParentTag = gHyp_data_new( NULL ) ;
      /* Although we *can* use setStr for setting a really large string,
       * it is better to use gHyp_util_breakStream and then gHyp_util_readStream
       *
      gHyp_data_setStr ( pParentTag, gzCgiQuery ) ;
      gHyp_cgi_xmlData ( pParentTag, pAI, pFrame, NULL ) ;
      */
      gHyp_data_setVariable ( pParentTag, "_xmldata_", TYPE_STRING ) ;
      gHyp_util_breakStream ( gzCgiQuery, cl, pParentTag, TRUE ) ;
      gHyp_cgi_xmlData ( pParentTag, pAI, pFrame, NULL ) ;

    }
    else if ( isIMG ) {

      /*
      pEndBuf = gzCgiQuery + cl ;
      pLine = gzCgiQuery ;
      while ( pLine < pEndBuf ) {
	lineLen = MIN ( INTERNAL_VALUE_SIZE, (pEndBuf-pLine) ) ;
        n = gHyp_util_unparseString ( value, pLine, lineLen, VALUE_SIZE, FALSE, FALSE, FALSE,"" ) ;
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,"[%s]",value) ;
	pLine += lineLen ;
      }
      */
      pContentData = gHyp_frame_createVariable ( pAI, gHyp_instance_frame(pAI), "_form_data_" ) ;
      gHyp_data_deleteValues ( pContentData ) ;	  
      gHyp_util_breakStream ( gzCgiQuery, cl, pContentData, TRUE ) ;

      pParentTag = gHyp_frame_createVariable ( pAI, pFrame, "_http_form_" ) ;
      gHyp_data_deleteValues ( pParentTag ) ;

      /* The first element will be the boundary */
      n = gHyp_data_getStr ( pContentData,
			     boundary,
			     MAX_INPUT_LENGTH, 
			     0,
			     TRUE ) ;
       if ( n > 0 ) {
        gHyp_util_trim ( boundary ) ;
	gHyp_secs1_httpFormData ( pContentData,
				  boundary+2,
				  pParentTag ) ;
      }
    }
    else if ( isCGI ) {

      /* How many entries (name/value pairs) do we need to
       *   allocate space for? (They should be separated by "&"'s) 
       */
    
      giCgiNumEntries = gzCgiQuery[0] == '\0' ? 0 : 1 ;
      for (i = 1; i < cl; i++)
        if (gzCgiQuery[i] == '&' || gzCgiQuery[i] == '\0')
	  giCgiNumEntries++;
    
      /* Allocate the space for that many structures: */
    
      cgi_entries = (sCgi*) AllocMemory(sizeof(sCgi) * giCgiNumEntries);
    
      if (cgi_entries == NULL) {
        giCgiErrno = CGIERR_OUT_OF_MEMORY;
        return(giCgiErrno);
      }
    
      /* Grab each name/value pair: */
      n = 0;
    
      /* (Begin with the first half of the first pair): */
      if ( gzCgiQuery[0] != '\0' && gzCgiQuery[0] != '&' )
        cgi_entries[0].name = gzCgiQuery;
    
      /* Go through the entire string of characters: */
      for (i = 0; i < cl; i++) {
      
        if (gzCgiQuery[i] == '&') {
	
	  /* "&" represents the end of a name/value pair: */	  
	  cgi_entries[n].name = gzCgiQuery + i + 1;
	  gzCgiQuery[i] = '\0';
	}
        else if (gzCgiQuery[i] == '=' && n < giCgiNumEntries ) {
	
	  /* "=" is the end of the name half of a name/value pair: */	  
	  cgi_entries[n].val = gzCgiQuery + i + 1;
	
	  /*  lHyp_cgi_plusToSpace(cgi_entries[giCgiNumEntries].val);
	   *   lHyp_cgi_unescapeUrl(cgi_entries[giCgiNumEntries].val); 
	   */
	  n++;
	
	  gzCgiQuery[i] = '\0';
	}
      }
    
      for (i = 0; i < giCgiNumEntries; i++) {
        gHyp_cgi_plusToSpace(cgi_entries[i].val);
        gHyp_cgi_unescapeUrl(cgi_entries[i].val);
      }
    } 
  
  
    /* Fix any NULL strings to be empty strings */  
    for (i = 0; i < giCgiNumEntries; i++) {
      if (cgi_entries[i].name == NULL ) cgi_entries[i].name = "";
      if (cgi_entries[i].val == NULL) cgi_entries[i].val = "";
    }
  }
  return(CGIERR_NONE);
}


/* Free up memory that was allocated when we called "lHyp_cgi_init()": */

static void lHyp_cgi_quit(void)
{
  if (giCgiRequestMethod == CGIREQ_NONE ||
      giCgiRequestMethod == CGIREQ_UNKNOWN) {
    /* Nothing to do! */
  }
  else {

    if ( giCgiRequestMethod == CGIREQ_POST ) {
      /* Was it POST method?  Free the data we had read from "stdin" */
      ReleaseMemory(gzCgiQuery);
    }     
    /* Free the entry structures themselves: */
    ReleaseMemory(cgi_entries);
  }
  
  cgi_entries = NULL;
  giCgiNumEntries = 0;
  giCgiErrno = CGIERR_NONE;
  giCgiRequestMethod = CGIREQ_NONE;
  gzCgiQuery = NULL;
}

/* Returns whether or not an e-mail address appears to be in the correct
   syntax ("username@host.domain"): */

/*
static int lHyp_cgi_goodemailaddress(const char * addr)
{
  int 
    i;
   
   No "@".. what? 
  
  if (strchr(addr, '@') == NULL)
    return 0;
  
  
   "@" or "." at the end or beginning? 
  
  if (addr[strlen(addr - 1)] == '@' ||
      addr[strlen(addr - 1)] == '.' ||
      addr[0] == '@' || addr[0] == '.')
    return 0;
  
   No "." after the "@"?  More than one "@"? 
  
  if (strchr(strchr(addr, '@'), '.') == NULL ||
      strchr(strchr(addr, '@') + 1, '@') != NULL)
    return 0;
  
  
   Any illegal characters within the string? 
  
  for (i = 0; i < (int)strlen(addr); i++) {
    if (isalnum(addr[i]) == 0 &&
	addr[i] != '.' && addr[i] != '@' && addr[i] != '_' &&
	addr[i] != '-')
      return(0);
  }
  
  
   Must be ok... 
  return 1;
}
*/

/* Returns the English string description for a particular cgi error
   value: */

static const char * lHyp_cgi_strerror(int err)
{
  if (err < 0 || err > CGIERR_NUM_ERRS)
    return("");
  else
    return(cgi_error_strings[err]);
}

void gHyp_cgi_parse ( sInstance *pAI, sFrame *pFrame  ) 
{
  sData
    *pArgs,
    *pVariable,
    *pData ;
  
  char
    *variable,
    *value ;
  
  sLOGICAL
    found ;

  int
    i,j,
    stat ;
       
  /* Initialize the CGI and send out an HTTP header: */
  stat = lHyp_cgi_init ( pAI, pFrame );
  
  /* Was there an error initializing the CGI??? */    
  if ( stat != CGIERR_NONE ) {
    gHyp_util_logWarning ( "%s", lHyp_cgi_strerror(stat) ) ;
    return ;
  }  
  /* Create or retrieve the "cgiargs" variable, clear it of any values. */
  pArgs = gHyp_frame_createVariable ( pAI,  pFrame, "cgiargs" ) ;
  gHyp_data_deleteValues ( pArgs ) ;
  
  for (i = 0; i < giCgiNumEntries; i++) {
    
    variable = cgi_entries[i].name ;
    value = cgi_entries[i].val ;
    /*gHyp_util_debug("%s = '%s'", variable, value ) ;*/
    
    pVariable = gHyp_frame_createVariable ( pAI, pFrame, variable ) ;
    
    /* Store a reference to the variable in the "cgiargs" variable,
     * but only if that reference does not already exist 
     */
    found = FALSE ;
    for (j=0;j<i;j++) if (strcmp(variable,cgi_entries[j].name)==0) found=TRUE;
    if ( !found ) {
      pData = gHyp_data_new ( NULL ) ;
      gHyp_data_setReference ( pData, variable, NULL ) ;
      gHyp_data_append ( pArgs, pData ) ;
    }
    gHyp_data_setText ( pVariable, value, strlen ( value ) ) ;
    /*gHyp_util_debug("var = %s", gHyp_data_print ( pVariable) ) ;*/
  }
  
  lHyp_cgi_quit();
}
