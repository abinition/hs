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
extern sHTTP*	gHyp_http_new () ;
extern void	gHyp_http_delete ( sHTTP *pHTTP ) ;
extern void	gHyp_http_reset ( sHTTP *pHTTP ) ;
extern void	gHyp_http_setState ( sHTTP* pHTTP, sBYTE ) ;
extern sBYTE	gHyp_http_getState ( sHTTP* ) ;
extern char*	gHyp_http_getArg1 ( sHTTP* ) ;
extern char*	gHyp_http_getArg2 ( sHTTP* ) ;
extern char*	gHyp_http_getArg3 ( sHTTP* ) ;
extern sData*	gHyp_http_getAttributeData( sHTTP* ) ;
extern sData*   gHyp_http_getContentData ( sHTTP* ) ;
extern int	gHyp_http_getActualContentLen( sHTTP* ) ;
extern void	gHyp_http_updateContent ( sHTTP* pHTTP, int contentlength, int actualContentLength ) ;
extern int	gHyp_http_getContentLength( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isChunkedTransferEnc( sHTTP* pHTTP ) ;
extern void	gHyp_http_setChunkedTransferEnc( sHTTP* pHTTP ) ;
extern char*	gHyp_http_getFormDataBoundary( sHTTP* pHTTP ) ;
extern void	gHyp_http_setFormDataBoundary( sHTTP* pHTTP, char* ) ;
extern sLOGICAL	gHyp_http_isXMLEncoded( sHTTP* pHTTP ) ;
extern void	gHyp_http_setXMLEncoded( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isURLEncoded( sHTTP* pHTTP ) ;
extern void	gHyp_http_setURLEncoded( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isZeroLength( sHTTP* pHTTP ) ;
extern void	gHyp_http_setZeroLength( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isPlainText( sHTTP* pHTTP ) ;
extern void	gHyp_http_setPlainText( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isBinary( sHTTP* pHTTP ) ;
extern void	gHyp_http_setBinary( sHTTP* pHTTP, sLOGICAL isBinary ) ;
extern void	gHyp_http_setHeader ( sHTTP*, char* pArg1, char* pArg2, char* pArg3 ) ;
extern sLOGICAL gHyp_http_isReply ( sHTTP *pHTTP ) ;
extern void	gHyp_http_service ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_open ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_close ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_assign ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_enable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_disable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_binary ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_http_eagain ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_http_event ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_query ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_reply ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_handle ( sInstance *, sCode *, sLOGICAL  ) ;
extern void     gHyp_http_asctime ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;

