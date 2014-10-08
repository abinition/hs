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
extern void	gHyp_port_service ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_open ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_close ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_assign ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_enable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_disable ( sInstance *, sCode *, sLOGICAL ) ; 
extern void	gHyp_port_binary ( sInstance *, sCode *, sLOGICAL ) ;
extern void	gHyp_port_eagain ( sInstance *, sCode *, sLOGICAL ) ;
extern void	gHyp_port_go ( sInstance *, sCode *, sLOGICAL ) ;
extern void	gHyp_port_stop ( sInstance *, sCode *, sLOGICAL ) ;
extern void	gHyp_port_event ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_query ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_handle ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_recvmsg ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_sendmsg ( sInstance *, sCode *, sLOGICAL  ) ;
