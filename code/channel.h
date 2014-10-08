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
extern sChannel*        gHyp_channel_new ( char*, char*, char*, short flags, SOCKET fd  ) ;
extern void		gHyp_channel_close ( sChannel*) ;
extern void		gHyp_channel_delete ( sChannel* ) ;
extern sChannel*        gHyp_channel_next ( sChannel* ) ; 
extern short            gHyp_channel_flags ( sChannel* ) ;
extern void		gHyp_channel_object ( sChannel* ) ;
extern char*            gHyp_channel_target ( sChannel* ) ;
extern char*            gHyp_channel_targetId ( sChannel* ) ;
extern void	        gHyp_channel_setTarget ( sChannel*, char* ) ;
extern char*            gHyp_channel_path ( sChannel* ) ;
extern void		gHyp_channel_setPath ( sChannel *pChannel, char *path ) ;
extern int		gHyp_channel_maxMsgSize ( sChannel* ) ;
extern void             gHyp_channel_setMaxMsgSize ( sChannel*, int ) ;
extern SOCKET           gHyp_channel_socket ( sChannel* ) ;
extern void		gHyp_channel_setSocket ( sChannel *pChannel, SOCKET s ) ;
extern char*            gHyp_channel_buffer ( sChannel* ) ;
extern void		gHyp_channel_setSSL ( sChannel *pChannel, sSSL* ) ;
extern sSSL*		gHyp_channel_getSSL ( sChannel *pChannel ) ;
extern void             gHyp_channel_resetBuffer ( sChannel* ) ;
extern sLOGICAL         gHyp_channel_updateStats ( sChannel*, int, sLOGICAL ) ;
extern time_t           gHyp_channel_updateTime ( sChannel* ) ;
extern void             gHyp_channel_setUpdateTime ( sChannel*, time_t ) ;
extern void             gHyp_channel_clearStats ( sChannel* ) ;
/*extern void             gHyp_channel_initStats ( sChannel*, char*, char* ) ;*/
extern void             gHyp_channel_copyStats ( sChannel*, sChannel* ) ;
extern void             gHyp_channel_displayStats ( sChannel*, sLOGICAL ) ;
extern int*		gHyp_channel_pNbytes ( sChannel * ) ;
extern OVERLAPPED*	gHyp_channel_overlapped ( sChannel *pChannel ) ;
extern int		gHyp_channel_maxMsgSize ( sChannel *pChannel ) ;
extern void		gHyp_channel_setMaxMsgSize ( sChannel *pChannel, int maxMsgSize ) ;
extern int		gHyp_channel_offset ( sChannel *pChannel ) ;
extern void		gHyp_channel_setOffset ( sChannel *pChannel, int offset ) ;
