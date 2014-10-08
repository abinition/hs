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
extern void 	gHyp_operator_binary ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_unary ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_assign ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_condition ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_reference ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_dereference ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_eval ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_dot ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_pointer ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_max ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_min ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_hypot ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_pow ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_abs ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_acos ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_asin ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_atan ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_ceil ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_cos ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_floor ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_log10 ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_logN ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_sin ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_sqrt ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_tan ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_exp ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_deg2rad ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_rad2deg ( sInstance*, sCode*, sLOGICAL ) ;
