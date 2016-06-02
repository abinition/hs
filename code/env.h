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
extern void	gHyp_env_count ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_debug ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_exists ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_exit ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_quit ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_idle ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_instantiate ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_instantiation ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_undef ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_hashed ( sInstance*, sCode*, sLOGICAL ) ;
extern void     gHyp_env_unhashed ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_local ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_global ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_next ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_prev ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_map ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_unmap ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_merge ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_sjm ( sInstance*, sCode*, sLOGICAL ) ;
extern sData*	gHyp_env_mergeData ( sData*, sData*, sInstance*, int, sLOGICAL, sLOGICAL, sLOGICAL, sLOGICAL, sData*, int ) ;
extern void	gHyp_env_sort ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_reverse ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insert ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insertbefore ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insertafter ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_append ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_remove ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_chop ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_detach ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_quiet ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_verify ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_version ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_env ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_localhost ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_localaddr ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_appendval ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insertval ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_name ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_parent ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_root ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_firstchild ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_lastchild ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_nextsibling ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_prevsibling ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_nextfirstcousin ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_prevlastcousin ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_getnodebyattr ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_getnodebyname ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_secs_map ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_secs_unmap ( sInstance*, sCode*, sLOGICAL ) ;