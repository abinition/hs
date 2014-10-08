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
#ifndef AS_INFORMIX
extern void		gHyp_sql_bind ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_sql_open ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_sql_close ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_sql_query ( sInstance*, sCode*, sLOGICAL ) ;
#else
extern void		gHyp_esql_connect ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_esql_query ( sInstance*, sCode*, sLOGICAL ) ;
#endif
extern void		gHyp_sql_datetime ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_sql_toexternal(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
