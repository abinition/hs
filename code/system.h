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
extern void    	gHyp_system_date ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_datetime ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_asctime ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_exec ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_system ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_random ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_srandom ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_sleep ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_usleep ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_time ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_timestamp ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_getclock ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_parsedate ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_parse ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_xparse ( sInstance*, sCode*, sLOGICAL ) ;
extern time_t	date_parse ( char* ) ;
extern void    	gHyp_system_getenv ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_setenv ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_unsetenv ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_system_setheap ( sInstance*, sCode*, sLOGICAL )  ;
