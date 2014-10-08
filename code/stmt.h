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
extern void 	gHyp_stmt_if ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_else ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_bExp ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_eExp ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_bList ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_list ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_eList ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_bSub ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_eSub ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_while ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_do ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_for ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_eos ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_break ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_return ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_continue ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_stmt_when ( sInstance*, sCode*, sLOGICAL ) ;
