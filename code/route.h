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
extern void 	gHyp_route_enable ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_route_disable ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_reply ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_event ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_query ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_requeue ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_instances ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_children ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_instance ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_concept ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_parent ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_moveto ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_renameto ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_route_service ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_route_open ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_route_tunnel( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_route_forward ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_route_unforward ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
