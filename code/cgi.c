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
 * $Log: cgi.c,v $
 * Revision 1.14  2004/07/01 01:57:29  bergsma
 * Minor change to some logError statements.
 *
 * Revision 1.13  2004/02/29 14:04:46  bergsma
 * Major rewrite of xml parse routines.
 *
 * Revision 1.12  2003/11/16 21:00:35  bergsma
 * 1. Try and detect and correct reversed tags.
 * 2. Make warning log comments smaller in size.
 *
 * Revision 1.11  2003/07/27 00:50:26  bergsma
 * Span whitespace after = name in attribute
 *
 * Revision 1.10  2003/04/27 13:39:51  bergsma
 * Dereference ppValue only if not NULL.
 *
 * Revision 1.8  2003/01/12 13:48:32  bergsma
 * V3.1.0
 * 1. Add isPRE condition back in describe for XML output
 * 2. Use instance_error and instance_warning in describe rather than util_*
 * 3. Fix spin problem when parsing bad-XML
 *
 * Revision 1.7  2002/11/29 19:41:57  bergsma
 * Support of <pre> tag not necessary here, only needed for xdescribe().
 * Support of <script> and <style> tags are needed here, as we must not
 * interpret and tags inside of the <script> or <style> tags.
 *
 * Revision 1.6  2002/11/21 23:48:16  bergsma
 * To support JAVASCRIPT (tag=SCRIPT), do not interpret any data inside
 * the tag body. Introduced the 'isSCR' flag.
 *
 * Revision 1.5  2002/10/22 23:59:13  bergsma
 * Do not use gHyp_data_setToken, rather gHyp_data_setStr for XML data
 * elements.  Let the application do any implicit conversion.
 *
 * Revision 1.4  2002/09/29 20:30:20  bergsma
 * Increased size of input stream for cgi xml messages.
 * Recognize application/soap as well as text/xml for xml format
 *
 * Revision 1.3  2002/09/05 01:34:16  bergsma
 * 1. Restored pValue argument in gHyp_cgi_parseXML.  This pValue argument
 * is the incremetor for the pStreamData argument and its context must be
 * saved.
 * 2. Skip tags beginning with <! and <?
 *
 * Revision 1.2  2002/09/03 21:18:04  bergsma
 * Increase size of XML input stream to MAX_INPUT_LENGTH for handling of large XML streams.
 *
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

static char gzStream[MAX_INPUT_LENGTH*3+1] ;

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
			  sInstance *pAI ) 
{
  sData
    *pData,
    *pUpperTag,
    *pChildTag,
    *pStrData,
    *pAttrData,
    *pArgs ;

  char
    c,
    quote,
    *pTag,
    *pExpectedEndTag,
    *pTag2=NULL,
    *pStr,
    *pStr2,
    *pStr3,
    *pSearch,
    attr[VALUE_SIZE+1],
    tag[TOKEN_SIZE+1],
    tag2[TOKEN_SIZE+1],
    tag_lc[TOKEN_SIZE+1],
    tag2_lc[TOKEN_SIZE+3] ;

  int
    dataType,
    context2,
    ss2,
    i,
    streamLen,
    span,
    tagLen,
    tag2Len,
    attrLen,
    strLen,
    strLen2,
    strLen3,
    truncStrLen ;

  sLOGICAL
    tagged,
    found,
    inComment,
    inDefinition,
    allowAttributes,
    isEndTag,
    terminated ;

  static char *childLess = 
    "/area/base/basefont/br/col/embed/frame/hr/img/input/isindex/link/meta/p/param/spacer/" ;

  static char *preContent =
    "/tt/b/i/em/strong/code/samp/kbd/var/cite/" ;

 #define SEG_SIZE 10 

  /* Get more data if necessary */
  pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
					&streamLen, pStreamData, 
					ppValue, pContext, ss, isVector, 
					pp ) ;


  /* Look for start tag, comment, or definition */
  pTag = NULL ;
  isEndTag = FALSE ;
  tagged = FALSE ;
  while ( !pTag ) {

    /* Look for start of next tag or end of tag. */
    pTag = strchr ( pStream, '<' ) ;
    if ( !pTag ) {
      /*
       gHyp_util_logWarning ( "No starting '<' found in XML stream, searching from [%.*s]",
			      SEG_SIZE,pStream );
       */
      return NULL ;
    }
    else {

      /* Found something.  Grab the contents before it (or ignore it if there
       * is no parent, ie: this is the first call to the function and we haven't
       * encountered any root tags yet (or we are between root tags)
       */
      if ( pParentTag ) {

	while ( pStream < pTag ) {

	  if ( tagged ) { pStream-- ; if ( isEndTag ) pStream-- ; tagged = FALSE ; }

	  if ( !isSCR && !isPRE && !isTXT ) {
	    /* When not "SCR or PRE or TXT", find the start of the first word in the line,
	     * otherwise whitespace and punctuation is kept.
	     */
	    span = strspn ( pStream, " \t\n\r" ) ;
	    pStream += span ;
	  }

	  /* Find the end of each line or take the entire section up to the '<' */
	  strLen = strcspn ( pStream, "\n\r" ) ;
	  if ( strLen == 0 ) strLen = pTag - pStream ;
	  if ( strLen > (pTag-pStream) ) strLen = pTag - pStream ;

	  if ( strLen == 0 ) {
	    /* Empty line */
	    if ( isPRE ) {
	      /* Add a space */
	      pStrData = gHyp_data_new ( NULL ) ;
	      gHyp_data_setStr ( pStrData, " " ) ;
	      gHyp_data_append ( pParentTag, pStrData ) ;
	    }
	  }
	  else {
	    strLen3 = strLen ;
	    pStr3 = pStream ;

	    while ( strLen3 > 0 ) {

	      truncStrLen = MIN ( INTERNAL_VALUE_SIZE, strLen3 ) ;
	      if ( truncStrLen > 0 ) {

		pStr = (char*) AllocMemory ( truncStrLen + 1 ) ;
		assert ( pStr ) ;

		strncpy ( pStr, pStr3, truncStrLen ) ;
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
	    while ( *pStream == '\n' || *pStream == '\r' ) pStream++ ; 
	  }
	}
      }

      /* Update pointer, then check tag to see if its a comment or definition. */
      pStream = pTag ;

      /* Get more data if necessary */
      pStream = gHyp_util_readStream (  pStream, pAnchor, ppEndOfStream,
					    &streamLen, pStreamData, 
					    ppValue, pContext, ss, isVector, 
					    pp ) ;
      /* Advance past '<' */
      pStream++ ;
      tagged = TRUE ;

      if ( !*pStream ) {
	gHyp_util_logWarning ( "Unexpected end of stream encountered at [%.*s]  ",
				SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE));
        return NULL ;
      }

      isEndTag = ( *pStream == '/' ) ;
      if ( isEndTag ) pStream++ ;

      /* Extract the tag name */
      tagLen = gHyp_util_getToken_okDash ( pStream ) ;

      /* A tagLen of zero means that there was not a valid tag name.
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
	    gHyp_util_logError ( "Invalid end tag, expecting '>' [%.*s]...[%.*s]",
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
	  gHyp_util_logError ( "Unexpected tag, skipping [%.*s]",
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
	    gHyp_util_logError ( "No comment end found in XML stream [%.*s]",
				      SEG_SIZE,pAnchor);
	  else
	    gHyp_util_logError ( "No end of defintion found XML stream [%.*s]",
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

	    while ( pStr < pStream ) {

	      /* Find the end of each line or take the entire section. */
	      strLen = strcspn ( pStr, "\n\r" ) ;
	      if ( strLen == 0 ) strLen = pStream - pStr ;
	      if ( strLen > (pStream-pStr) ) strLen = pStream - pStr ;
	 
	      strLen3 = strLen ;
	      pStr3 = pStr ;
	      while ( strLen3 > 0 ) {

		truncStrLen = MIN ( INTERNAL_VALUE_SIZE, strLen3 ) ;
		if ( truncStrLen > 0 ) {
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
	      if ( *pStr == '\n' || *pStr == '\r' ) pStr++ ; 
	    }
	  }

	  if ( isEndTag )
	    gHyp_util_logError ( "Unexpected end tag, skipping [%.*s]...[%.*s]",
		  		  SEG_SIZE,MAX(pAnchor,pTag-SEG_SIZE),SEG_SIZE,pTag);

	  pTag = NULL ;

          /* Get more data if necessary */
	  pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
						&streamLen, pStreamData, 
						ppValue, pContext, ss, isVector, 
						pp ) ;

	}
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

   /* Check if more of the stream is needed */
  pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
					&streamLen, pStreamData, 
					ppValue, pContext, ss, isVector, 
					pp ) ;


  if ( isEndTag ) {

    /* If we encountered an end tag, then one of the following cases will apply.
     * 
     * 1. It will match pParentTag, we will set *pEndTag = TRUE return.
     * 2. It will not match and we will need to deal with that condition.
     */

    pExpectedEndTag = gHyp_data_getLabel ( pParentTag ) ;
    strcpy ( tag2_lc, pExpectedEndTag ) ;
    gHyp_util_lowerCase ( tag2_lc, strlen ( tag2_lc ) ) ;

    if ( strcmp ( tag_lc, tag2_lc ) == 0 ) {

      /* Matched ending tag */
      pStream += tagLen ;

      /* Check for '>' */
      if ( *pStream != '>' ) {
	gHyp_util_logError ( "Invalid end tag, expecting '>' [%.*s]...[%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);
        return NULL ;
      }

      pStream++ ;

      *pIsEndTag = TRUE ;
      return pStream ;

    }
    else {

      /* Ending tag does not match. */
      gHyp_util_logWarning ( "Unexpected end tag </%s>, expecting </%s> [%.*s]...[%.*s]",
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
	gHyp_util_logError ( "Discarding end tag </%s>. Continuing...[%.*s]",
				tag,
				SEG_SIZE,pStream) ;
	/* Ending tag */
	pStream += tagLen ;

	/* Check for '>' */
	if ( *pStream != '>' ) {
	  gHyp_util_logError ( "Invalid end tag [%.*s]...[%.*s]",
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
	      tag2Len = gHyp_util_getToken_okDash ( pStr ) ;
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
	      gHyp_util_logError("Ending tag </%s> is reversed with </%s>",
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
	  gHyp_util_logWarning( "Inserting tag <%s> after <%s>",
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
	    gHyp_util_logWarning ( "Assuming childless tag <%s />",pExpectedEndTag);
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
	      gHyp_util_logWarning ( 
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
      pChildTag = gHyp_frame_createVariable ( pFrame, tag ) ;
      gHyp_data_deleteValues ( pChildTag ) ;
      /* Create or retrieve the "xmlargs" variable, adding the new tag name. */
      pArgs = gHyp_frame_createVariable ( pFrame, "xmlargs" ) ;
      if ( pArgs ) {
	pData = gHyp_data_new ( NULL ) ;
        gHyp_data_setReference ( pData, tag, NULL ) ;
        gHyp_data_append ( pArgs, pData ) ;
     }
    }
  }
  else {
    /* Create new tagData, appending to existing tag variable */
    pChildTag = gHyp_data_new ( tag ) ;
    gHyp_data_append ( pParentTag, pChildTag ) ;
  }

  /* Check if more of the stream is needed */
  pStream = gHyp_util_readStream (	pStream, pAnchor, ppEndOfStream,
					&streamLen, pStreamData, 
					ppValue, pContext, ss, isVector, 
					pp ) ;


   /* Attributes are allowed until the end of the tag */
  allowAttributes = TRUE ;

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
     
    if ( *pStream == '/'  ) {

      /* A tag with no data, ending with />, with zero or more attributes */
      pStream++ ;
      if ( *pStream == '>' ) return pStream ;

    }
    else if ( *pStream == '>' ) {

      /* End of current tag start.  Get contents of tag */
      pStream++ ;

      allowAttributes = FALSE ;

    }
    else {

      /* The stuff inside the tag, ie: <TAG ...stuff...> */

      /* Collect attributes so long as we can (and we are not in a script or style or textarea */
      if ( allowAttributes && !isSCR && !isTXT ) {

        /* Pull out attributes value */
        attrLen = gHyp_util_getToken_okDash ( pStream ) ;
        if ( attrLen <= 0 ) {
          gHyp_util_logWarning ( "Invalid Attribute Value [%.*s]...[%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);

	  /* Span non-whitespace to find the end of the invalid value */
	  attrLen = strcspn ( pStream, " \t>" ) ;
	  if ( attrLen <= 0 ) return NULL ;
	}
      
        /* Found an attribute */
	attrLen = MIN ( VALUE_SIZE, attrLen ) ;
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

	  gHyp_util_logWarning ( "No = after attribute [%.*s]...[%.*s]",
				 SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);

	  /* Create the attribute space */
	  gHyp_data_newVector ( pAttrData, TYPE_ATTR, 1, TRUE ) ;
	  c = '1' ;
	  gHyp_data_setVectorRaw ( pAttrData, &c, 0 ) ;
	  gHyp_data_append ( pChildTag, pAttrData ) ;

	}
        else { 

	  /* Get the attribute value */
	  pStream++ ;

	  /* Span whitespace after "=" */
	  span = strspn ( pStream, " \t\n\r" ) ;
          pStream += span ;

	  if ( *pStream != '"' && *pStream != '\'' ) {

	    gHyp_util_logWarning ( "No starting \" on attribute [%.*s]...[%.*s]",
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
	      gHyp_util_logWarning ( "Attribute using single quote \'[%.*s]...[%.*s]",
				   SEG_SIZE,MAX(pAnchor,pStream-SEG_SIZE),SEG_SIZE,pStream);
	    
	    /* Found starting quote, extract attribute value */
	    pStream++ ;

	    terminated = FALSE ;
	    pSearch = pStream ;
	    while ( !terminated ) {
	      
	      pStr = strchr ( pSearch, quote ) ;
	      
	      /* If no ending quote, quit, returning pointer to start */
	      if ( !pStr ) {
		gHyp_util_logWarning ( "No ending quote [%.*s]...[%.*s]",
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

	  /* Create the attribute space */
	  gHyp_data_newVector ( pAttrData, TYPE_ATTR, strLen, TRUE ) ; 

	  /* Extract string - maximum of VALUE_SIZE characters. */
	  truncStrLen = MIN ( VALUE_SIZE, strLen ) ;

	  if ( truncStrLen > 0 ) {

	    pStr2 = (char*) AllocMemory ( truncStrLen + 1 ) ;
	    assert ( pStr2 ) ;
	  
	    strncpy ( pStr2, pStream, truncStrLen ) ;
	    pStr2[truncStrLen] = '\0' ;
	  
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
	 * If the 
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
				      pAI ) ;

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
  gHyp_data_deleteValues ( gHyp_frame_createVariable ( pFrame, "xmlargs" ) ) ;

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
				  pAI ) ;
    if ( !pStream ) break ;
    if ( !*pStream ) break ;
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

    sLOGICAL
      status ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: xml ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    status = TRUE ;

    /* Parse the xml */
    gHyp_cgi_xmlData ( pData, pAI, pFrame, NULL) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

static int lHyp_cgi_init( sInstance *pAI, sFrame *pFrame )
{
  int
    cl, 
    i;
 
  char
    contentType[VALUE_SIZE+1] ;

  sBYTE
    isCGI=FALSE,
    isXML=FALSE ;

  sData
     *pParentTag ;

  /* Default, no errors, no name/value pairs ("entries"): */
  giCgiErrno = CGIERR_NONE;
  giCgiNumEntries = 0;

  /* Check for REQUEST_METHOD (set by HTTP server): */
  
  if ( getenv ( "REQUEST_METHOD" ) == NULL ) {

    /* None set?  Assume the user is invoking the CGI from a shell prompt
     * (for debugging): 
     */
    giCgiRequestMethod = CGIREQ_NONE;
  }
  else {
    /* Determine the exact request method, and grab the data (if any)
       in the appropriate manner: */
    
    if ( strcmp ( getenv("REQUEST_METHOD"),"POST") == 0 ) {
      /* Post method (data is sent to us via "stdin"): */
      
      giCgiRequestMethod = CGIREQ_POST;

      
      strncpy ( contentType, getenv( "CONTENT_TYPE" ), VALUE_SIZE ) ;
      contentType[VALUE_SIZE] = '\0' ;
      gHyp_util_trim ( contentType ) ;
      
      isCGI = (strstr(contentType,"application/x-www-form-urlencoded")!=NULL) ;
      isXML = ( strstr ( contentType, "text/xml" ) != NULL ||
		strstr ( contentType, "text/html" ) ||
		strstr ( contentType, "application/soap" ) != NULL ) ;


      if ( !isCGI && !isXML ) {
	/* Is the content type incorrect (or not set at all?) */	  
	giCgiErrno = CGIERR_INCORRECT_TYPE;
	return(giCgiErrno);
      }
      else {
		
	/* How much data do we expect? */
	
	if ( getenv ( "CONTENT_LENGTH" ) == NULL ||
	     sscanf ( getenv( "CONTENT_LENGTH" ), "%d", &cl) != 1) {
	  
	  giCgiErrno = CGIERR_BAD_CONTENT_LENGTH;
	  return(giCgiErrno);
	}
	
	/* Create space for it: */	  
	gzCgiQuery = (char*) AllocMemory(cl + 1);
	
	if (gzCgiQuery == NULL) {
	  giCgiErrno = CGIERR_OUT_OF_MEMORY;
	  return(giCgiErrno);
	}
	
	/* Read it in: */
	fgets(gzCgiQuery, cl + 1, stdin);
	
	/* Verify that we got as much data as we expected: */
	if (strlen(gzCgiQuery) != (unsigned) cl)
	  giCgiErrno = CGIERR_CONTENT_LENGTH_DISCREPANCY;
      }
    }
    else if ( strcmp ( getenv("REQUEST_METHOD"),"GET") == 0 ) {
      
      /* GET method (data sent via "QUERY_STRING" env. variable): */	
      isCGI = 1 ;
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
      gHyp_data_setStr ( pParentTag, gzCgiQuery ) ;
      gHyp_cgi_xmlData ( pParentTag, pAI, pFrame, NULL ) ;

    }
    else if ( isCGI ) {

      /* How many entries (name/value pairs) do we need to
       *   allocate space for? (They should be separated by "&"'s) 
       */
    
      giCgiNumEntries = 0;
    
      for (i = 0; i <= cl; i++)
        if (gzCgiQuery[i] == '&' || gzCgiQuery[i] == '\0')
	  giCgiNumEntries++;
    
    
      /* Allocate the space for that many structures: */
    
      cgi_entries = (sCgi*) AllocMemory(sizeof(sCgi) * giCgiNumEntries);
    
      if (cgi_entries == NULL) {
        giCgiErrno = CGIERR_OUT_OF_MEMORY;
        return(giCgiErrno);
      }
    
      /* Grab each name/value pair: */
      giCgiNumEntries = 0;
    
      /* (Begin with the first half of the first pair): */
      if ( gzCgiQuery[0] != '\0' && gzCgiQuery[0] != '&' )
        cgi_entries[0].name = gzCgiQuery;
    
      /* Go through the entire string of characters: */
      for (i = 0; i <= cl; i++) {
      
        if (gzCgiQuery[i] == '&') {
	
	  /* "&" represents the end of a name/value pair: */	  
	  cgi_entries[giCgiNumEntries].name = gzCgiQuery + i + 1;
	  gzCgiQuery[i] = '\0';
	}
        else if (gzCgiQuery[i] == '=') {
	
	  /* "=" is the end of the name half of a name/value pair: */	  
	  cgi_entries[giCgiNumEntries].val = gzCgiQuery + i + 1;
	
	  /*  lHyp_cgi_plusToSpace(cgi_entries[giCgiNumEntries].val);
	   *   lHyp_cgi_unescapeUrl(cgi_entries[giCgiNumEntries].val); 
	   */
	  giCgiNumEntries++;
	
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
  pArgs = gHyp_frame_createVariable ( pFrame, "cgiargs" ) ;
  gHyp_data_deleteValues ( pArgs ) ;
  
  for (i = 0; i < giCgiNumEntries; i++) {
    
    variable = cgi_entries[i].name ;
    value = cgi_entries[i].val ;
    /*gHyp_util_debug("%s = '%s'", variable, value ) ;*/
    
    pVariable = gHyp_frame_createVariable ( pFrame, variable ) ;
    
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
