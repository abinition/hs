/*
 * Modifications:
 *
 * $Log: cgi.h,v $
 * Revision 1.9  2004/02/29 14:04:46  bergsma
 * Major rewrite of xml parse routines.
 *
 * Revision 1.8  2003/04/23 22:44:07  bergsma
 * The xml  parse function was broke.  i
 * The argument pValue in gHyp_cgi_parseXML needed to
 * be a pointer so that it can be updated within the recursion of the parsing.
 * This bug affected only parsing of XML strings, xsdescribe(), xml()
 *
 * Revision 1.7  2003/01/12 10:09:57  bergsma
 * Add isPRE back into gHyp_cgi_parseXML
 * Add sFrame argument to gHyp_cgi_parse
 *
 * Revision 1.6  2002/11/29 19:42:56  bergsma
 * Removed isPRE argument from gHyp_cgi_parseXML
 *
 * Revision 1.5  2002/11/21 23:49:03  bergsma
 * Added isSCR (is javascript) flag to gHyp_cgi_parseXML
 *
 * Revision 1.4  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.3  2002/09/05 01:32:31  bergsma
 * Put back pValue argument to gHyp_cgi_parseXML
 *
 * Revision 1.2  2002/09/03 21:19:36  bergsma
 * Removed pValue argument in gHyp_cgi_parseXML, and added pAnchor argument
 *
 */
extern void	gHyp_cgi_parse ( sInstance *pAI, sFrame* ) ;
extern void	gHyp_cgi_unescapeUrl(char *url) ;
extern void     gHyp_cgi_plusToSpace( char *url) ;
extern void	gHyp_cgi_xml ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ; 
extern void	gHyp_cgi_xmlData( sData *pData, sInstance *pAI, sFrame *pFrame, sData *pTV ) ; 
extern char     *gHyp_cgi_parseXML (  char *pStream, 
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
				      sData *pXML,
				      sData *pParentXML,
				      sLOGICAL *pIsEndTag,
				      sFrame *pFrame,
				      sData *pTV,
				      FILE *pp,
				      sInstance *pAI ) ; 
