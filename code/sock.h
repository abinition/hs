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
extern sLOGICAL	gHyp_sock_init () ;
extern sLOGICAL	gHyp_sock_isEagain () ;
extern void	gHyp_sock_doEagain () ;
extern sBYTE* gHyp_sock_eagainBuf() ;
extern int gHyp_sock_eagainBufLen() ;
extern void	gHyp_sock_usleep ( unsigned int ) ;
extern void gHyp_sock_cancelIO ( SOCKET ) ;
extern HANDLE	gHyp_sock_fifo ( char*, sLOGICAL, sLOGICAL, sLOGICAL, sLOGICAL ) ;
extern sLOGICAL	gHyp_sock_mkdir ( char* ) ;
#ifdef AS_SSL
extern void *gHyp_sock_ctxInit(char*,char*,char*);
extern int gHyp_sock_password_cb(char *buf, int num, int rwflag, void *userdata ) ;
extern void gHyp_sock_hmac256 ( char *text, char *text2, char *digest ) ;
extern void gHyp_sock_digest ( char *text, char *text2, char *digest, char *type ) ;
extern void gHyp_sock_md5 ( char *text, int len, char *digest ) ;
extern sLOGICAL gHyp_sock_ctxCiphers ( void * ctx, char *ciphers ) ;
extern sLOGICAL gHyp_sock_ctxCApath ( void * ctx, char *CApath ) ;
extern sLOGICAL gHyp_sock_ctxCAfile ( void * ctx, char *CAfile ) ;
extern sLOGICAL gHyp_sock_ctxAuth ( void *ctx ) ;
extern sLOGICAL gHyp_sock_ctxAuthClient ( void *ctx ) ;
extern void   gHyp_sock_destroyCTX ( void *ctx ) ;
extern void   gHyp_sock_deleteSSL ( sSSL *pSSL ) ;
extern void gHyp_sock_setSession ( void *ctx, void *session  ) ;
extern void *gHyp_sock_getSession ( void *ctx ) ;
extern void gHyp_sock_enableSessions ( void *ctx ) ;
extern sSSL* gHyp_sock_copySSL ( sSSL *pSSL ) ;
extern sSSL* gHyp_sock_createSSL ( void *ctx, sLOGICAL isClient, char *condition ) ;
extern sData *gHyp_sock_getSSLstate(  sSSL *pSSL  ) ;
extern sLOGICAL gHyp_sock_setSSLstate ( sSSL *pSSL, sData *pSSLdata, sSSL *pSSLORIG ) ;
#endif
extern int gHyp_sock_readJNI ( SOCKET s, 
			char *pMsgOff,
			int maxBytes,
			int timeout,
			int *pNbytes ) ;
extern int gHyp_sock_writeJNI (  SOCKET s, 
			  char *pMsg,  
			  int msgLen ) ;
extern int		gHyp_sock_read ( SOCKET s, char*, int, short, int, int*,
			OVERLAPPED* pOverlapped, sSSL* pSSL ) ;
extern void		gHyp_sock_clearReadFDS () ;
extern int		gHyp_sock_write ( SOCKET s, char*, int, short ,
			OVERLAPPED* pOverlapped, sSSL* pSSL ) ;
extern void		gHyp_sock_ticks ( int, int ) ;
extern sLOGICAL		gHyp_sock_initialize ( char* ) ;
extern void 		gHyp_sock_list () ;
extern void		gHyp_sock_shutdown ( SOCKET ,
					     sData *,
					     sData *,
					     sData *,
					     sConcept *) ;
extern sData*		gHyp_sock_createClient ( sData*, char*, char*, sLOGICAL ) ;
extern sData*		gHyp_sock_createNetwork ( sData*, char*, char*, SOCKET ) ; 
extern sData*		gHyp_sock_findClient ( sData*, char *, char* ) ;
extern sData*	        gHyp_sock_findParent ( char *object, char *targetId ) ;
extern sData*	 	gHyp_sock_findNetwork ( sData *, char * ) ;
extern void 		gHyp_sock_closeJNI ( SOCKET ) ;
extern void 		gHyp_sock_close ( SOCKET, short, char*, char* ) ;
extern sLOGICAL		gHyp_sock_isChannelDMBX ( sChannel* ) ;
extern int		gHyp_sock_messageCount ( sChannel* ) ;
extern sData*		gHyp_sock_findSCR ( sData*, char*, char*, char*, sAImsg*, sLOGICAL* ) ;
extern sData*		gHyp_sock_findHYP ( sData*, char*, char*, char*, sAImsg*, sLOGICAL* ) ;
extern sData*		gHyp_sock_findEXE ( sData*, char*, char*, char*, sAImsg*, sLOGICAL* ) ;
extern sData*		gHyp_sock_findCOM ( sData*, char*, char*, char*, sAImsg*, sLOGICAL* ) ;
extern void		gHyp_sock_cleanClient ( sData* ) ;
extern sLOGICAL		gHyp_sock_flushBuffer ( char* ) ;
extern sLOGICAL		gHyp_sock_loadBuffer ( char* ) ;
extern sLOGICAL		gHyp_sock_process () ;
extern sLOGICAL		gHyp_sock_putChannel ( sChannel*, char*, sAImsg* ) ;
extern sLOGICAL		gHyp_sock_putWildCardClients ( sData*, char *, sAImsg* ) ;
extern void		gHyp_sock_logger ( char*, char*, sAImsg* ) ;
extern HANDLE		gHyp_sock_createEvent ();
extern int		gHyp_sock_select ( sConcept*, 
					   HANDLE, 
					   SOCKET, 
					   sData*, 
					   sData*,
					   sData* );
