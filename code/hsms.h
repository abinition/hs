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
extern sHsms*		gHyp_hsms_new ( short,
					SOCKET,
					char*,int,
					float,float,float,float,
					sBYTE,
					SOCKET,
					sInstance* ) ;
extern void		gHyp_hsms_delete ( sHsms * ) ;
extern void		gHyp_hsms_close ( sHsms * ) ; 
extern sInstance*	gHyp_hsms_assign ( sHsms*, sConcept*, sWORD ) ;
extern int		gHyp_hsms_nextMessage ( sHsms*, sConcept*, sInstance* ) ;
extern int		gHyp_hsms_incoming ( sHsms*, sConcept*, sInstance*) ;
extern int		gHyp_hsms_outgoing ( sHsms*,
					     sSecs2*,
					     sInstance*,
					     int,int,int,int,int,sBYTE);
extern SOCKET		gHyp_hsms_socket ( sHsms *pHsms ) ;
extern short		gHyp_hsms_flags ( sHsms *pHsms ) ;
extern SOCKET		gHyp_hsms_parentSocket ( sHsms *pHsms ) ;
extern sInstance*	gHyp_hsms_parentAI ( sHsms *pHsms ) ;
extern sBYTE*		gHyp_hsms_buf ( sHsms* ) ;
extern unsigned gHyp_hsms_port ( sHsms *pHsms ) ;
extern float		gHyp_hsms_t5 ( sHsms * ) ;
extern float		gHyp_hsms_t6 ( sHsms * ) ;
extern float		gHyp_hsms_t7 ( sHsms * ) ;
extern float		gHyp_hsms_t8 ( sHsms * ) ;
extern char*		gHyp_hsms_node ( sHsms * ) ;
extern int		gHyp_hsms_TID ( sHsms * ) ;
extern int		gHyp_hsms_SID ( sHsms * ) ;
extern int*		gHyp_hsms_pNbytes ( sHsms *pHsms ) ;
extern LPOVERLAPPED	gHyp_hsms_overlapped ( sHsms * ) ;
extern void gHyp_hsms_setSSL ( sHsms *pHsms, sSSL *pSSL ) ;
extern sSSL *gHyp_hsms_getSSL ( sHsms *pHsms ) ;
