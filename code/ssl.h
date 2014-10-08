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
extern void gHyp_ssl_new ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_ciphers ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_keyFile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_certFile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_CAfile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_CApath ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_digest ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_md5 ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_ssl ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_auth ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_authClient ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_assign ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_delete ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_setSession ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_getSession ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_getState ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_setState ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_enableSessions ( sInstance *, sCode *, sLOGICAL  ) ;
extern sData *gHyp_ssl_getData ( sData *pData, char *a, char *b, char *c ) ;
