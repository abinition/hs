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
extern sSecs1 *gHyp_secs1_new ( short flags,
				SOCKET fd,
				char *device,
				unsigned modifier,
				float t1,
				float t2,
				float t4,
				int rty,
				SOCKET parentSocket,
				sInstance *parentAI ) ;
extern sSecsHeader*	gHyp_secs1_newSecsHeader () ;
extern void		gHyp_secs1_delete ( sSecs1 * ) ;
extern HANDLE		gHyp_secs1_open ( char * ) ;
extern void		gHyp_secs1_close ( sSecs1 * ) ; 
extern int		gHyp_secs1_incoming ( sSecs1*,sConcept*,sInstance*) ;
extern int		gHyp_secs1_outgoing ( sSecs1*,
					      sSecs2*,
					      sInstance*,
					      int,int,int,int,int,sBYTE );
extern float		gHyp_secs1_t1 ( sSecs1 * ) ;
extern float		gHyp_secs1_t2 ( sSecs1 * ) ;
extern float		gHyp_secs1_t4 ( sSecs1 * ) ;
extern int		gHyp_secs1_rty ( sSecs1 * ) ;
extern unsigned		gHyp_secs1_speed ( sSecs1 * ) ;
extern unsigned		gHyp_secs1_port ( sSecs1 * ) ;
extern char*		gHyp_secs1_device ( sSecs1 * ) ;
extern unsigned		gHyp_secs1_TID ( sSecs1 * ) ;
extern unsigned		gHyp_secs1_SID ( sSecs1 * ) ;
extern void             gHyp_secs1_httpFormData ( sData*, char*, sData* ) ; 
extern int		gHyp_secs1_rawIncoming ( sSecs1*,sConcept*,sInstance*,sBYTE) ;
extern int		gHyp_secs1_rawOutgoing ( sSecs1*,sInstance*,sData*,int,sBYTE ) ;
extern int		gHyp_secs1_rawOutgoingEagain ( sSecs1 *pPort, sInstance *pAI, int ms ) ;
extern int		gHyp_secs1_rawOutEagainInit ( sSecs1 *pPort, sInstance *pAI, sData *pData,sBYTE ) ;
extern SOCKET		gHyp_secs1_fd ( sSecs1* ) ;
extern SOCKET		gHyp_secs1_parentSocket ( sSecs1* ) ;
extern sInstance*	gHyp_secs1_parentAI ( sSecs1 *pSecs1 ) ;
extern sBYTE*		gHyp_secs1_buf ( sSecs1* ) ;
extern int*		gHyp_secs1_pNbytes ( sSecs1 *pSecs1 ) ;
extern short		gHyp_secs1_flags ( sSecs1 *pSecs1 ) ;
extern void		gHyp_secs1_setState ( sSecs1 *pSecs1, sBYTE state ) ;
extern void		gHyp_secs1_setFlags ( sSecs1 *pSecs1, short flags ) ;
extern LPOVERLAPPED	gHyp_secs1_overlapped ( sSecs1 *pSecs1 ) ;
extern void		gHyp_secs1_setSSL ( sSecs1 *pSecs1, sSSL *pSSL ) ;
extern int		gHyp_secs1_eagainTimeout(  sSecs1 *pSecs1, int timeout ) ;
extern sHTTP*		gHyp_secs1_getHttp ( sSecs1 *pSecs1 ) ;
extern void		gHyp_secs1_setHttp ( sSecs1 *pSecs1, sHTTP *pHTTP ) ;
extern void             gHyp_secs1_setHttp ( sSecs1 *pSecs1, sHTTP* pHTTP ) ; 
extern sSSL*		gHyp_secs1_getSSL ( sSecs1 *pSecs1 ) ;
extern sData*		gHyp_secs1_getEagain ( sSecs1 *pSecs1 ) ;
extern sLOGICAL         gHyp_secs1_doEagain ( sSecs1 *pSecs1 ) ;
extern void             gHyp_secs1_setEagain ( sSecs1 *pSecs1, sLOGICAL doEagain ) ;
extern sData*		gHyp_secs1_getForwardPorts ( sSecs1 *pSecs1 ) ;
extern void		gHyp_secs1_setForwardPorts ( sSecs1 *pSecs1, sData* pfp ) ;
extern void             gHyp_secs1_setBinary( sSecs1* pSecs1, sLOGICAL isBinary ) ;
extern sLOGICAL         gHyp_secs1_isBinary( sSecs1* pSecs1 ) ;
extern void		gHyp_secs1_addForwardPort ( sSecs1 *pSecs1, int from_id, int to_id ) ;
extern sData*		gHyp_secs1_copyForwardPort ( sSecs1 *pSecs1, int  ) ;
extern void		gHyp_secs1_removeForwardPort ( sSecs1 *pSecs1, int from_id, int to_id ) ;

