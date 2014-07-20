/* External Global function prototypes 
 *
 * Modifications:
 *
 * $Log: sock.h,v $
 * Revision 1.22  2011-01-08 21:40:09  bergsma
 * Added sock_hmac256
 *
 * Revision 1.21  2010-01-08 02:44:57  bergsma
 * Added ssl_md5(), enhanced ssl_digest.
 * Fixed urldecode, missing ":"
 * Enabled object calls, ie:  text.strtok( ) and the like...
 *
 * Revision 1.20  2008-12-30 18:36:27  bergsma
 * Add support for conditional SSL
 *
 * Revision 1.19  2007-07-11 14:44:59  bergsma
 * Add second argyment to ssl_digest
 *
 * Revision 1.18  2007-07-09 05:37:32  bergsma
 * Add ssl_digest
 *
 * Revision 1.17  2006-11-13 02:11:34  bergsma
 * Added functions ssl_setState and ssl_getState
 *
 * Revision 1.16  2006/10/02 06:22:32  bergsma
 * For egain to work properly with SSL, the data that's already been encypted
 * needs to be saved in a special buffer, pEagainPartial, so that it may be
 * sent again without having to go through the SSL engine.
 *
 * Revision 1.15  2006/10/01 16:26:43  bergsma
 * Support for EAGAIN processing.
 *
 * Revision 1.14  2006/08/09 00:51:22  bergsma
 * Undo last change to createNetwork
 *
 * Revision 1.13  2006/08/08 20:50:59  bergsma
 * In createNetwork, prevent closing of valid socket on another IP channel.
 *
 * Revision 1.12  2005/12/27 02:53:25  bergsma
 * Removed functions ssl_certfile and ssl_keyfile
 *
 * Revision 1.11  2005/12/17 20:42:11  bergsma
 * no message
 *
 * Revision 1.10  2005/11/29 23:57:04  bergsma
 * Argument missing in gHyp_sock_usleep
 *
 * Revision 1.9  2005/11/06 16:45:06  bergsma
 * Added setKetFile and getKeyFile
 *
 * Revision 1.8  2005/10/25 16:39:49  bergsma
 * Added usleep() function
 *
 * Revision 1.7  2005/07/23 22:33:06  bergsma
 * ssl fixes
 *
 * Revision 1.6  2004/10/16 05:06:06  bergsma
 * Added getSessionand setSession functions
 *
 * Revision 1.5  2004/07/01 02:18:28  bergsma
 * Functions without stdout to use with JNI interface
 *
 * Revision 1.4  2004/04/29 02:20:39  bergsma
 * Added SSL support, HTTP support.
 *
 * Revision 1.3  2003/01/09 08:00:36  bergsma
 * V3.1.0
 * New argument 'alreadyExists' added to gHyp_sock_createClient
 * Added function gHyp_sock_findParent
 * Renamed gHyp_sock_putClient and gHyp_sock_putNetwork to gHyp_sock_putChannel
 *
 * Revision 1.2  2002/10/27 20:50:05  bergsma
 * Added 4th argument to gHyp_sock_fifo
 *
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
