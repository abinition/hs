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
extern SOCKET		gHyp_tcp_request ( char *, int ) ;
extern SOCKET		gHyp_tcp_requestJNI ( char* pHost, int port ) ;
extern SOCKET  		gHyp_tcp_make ( char *, char *, sLOGICAL ) ;
extern SOCKET		gHyp_tcp_checkInbound ( SOCKET, char*, char*, sLOGICAL ) ;
extern sLOGICAL		gHyp_tcp_resolveHost ( char *, char * ) ;
extern void		gHyp_tcp_createAliasTable() ;
extern void		gHyp_tcp_deleteAliasTable() ;
extern sAlias*		gHyp_tcp_newAlias ( char * ) ;
extern void		gHyp_tcp_deleteAlias ( sAlias* ) ;
extern sAlias*		gHyp_tcp_createAlias ( char *pHost, char *pAddr ) ;
extern void		gHyp_tcp_removeAliases ( char* ) ;
extern void		gHyp_tcp_gethostname ( char *, int  ) ;
extern sLOGICAL		gHyp_tcp_setup ( SOCKET s ) ;
#if defined (AS_UNIX)
extern sLOGICAL		gHyp_tcp_sendmsg( char *pClient, char *pService, SOCKET sendfd, int port ) ;
extern SOCKET		gHyp_tcp_recvmsg ( int s, int *pport ) ;
extern SOCKET		gHyp_tcp_makeUNIX ( char *pService ) ;
extern SOCKET		gHyp_tcp_checkInboundUNIX ( SOCKET s ) ;
#endif
