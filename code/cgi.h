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
extern void	gHyp_cgi_parse ( sInstance *pAI, sFrame* ) ;
extern void	gHyp_cgi_urlstring( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_cgi_urlparse( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
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
				      sInstance *pAI,
				      sBYTE requestedType,
				      sLOGICAL isPureXML,
				      sLOGICAL mayReturnChildLess) ; 
