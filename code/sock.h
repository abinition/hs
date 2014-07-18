/* External Global function prototypes 
 *
 * Modifications:
 *
 * $Log: sock.h,v $
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
extern void gHyp_sock_cancelIO ( SOCKET ) ;
extern HANDLE	gHyp_sock_fifo ( char*, sLOGICAL, sLOGICAL, sLOGICAL, sLOGICAL ) ;
extern sLOGICAL	gHyp_sock_mkdir ( char* ) ;
#ifdef AS_SSL
extern void *gHyp_sock_ctxInit(char*,char*,char*);
extern int gHyp_sock_password_cb(char *buf, int num, int rwflag, void *userdata ) ;
extern sLOGICAL gHyp_sock_ctxCiphers ( void * ctx, char *ciphers ) ;
extern sLOGICAL gHyp_sock_ctxKey ( void * ctx, char *key, char *pass ) ;
extern sLOGICAL gHyp_sock_ctxCert ( void * ctx, char *cert ) ;
extern sLOGICAL gHyp_sock_ctxCApath ( void * ctx, char *CApath ) ;
extern sLOGICAL gHyp_sock_ctxCAfile ( void * ctx, char *CAfile ) ;
extern sLOGICAL gHyp_sock_ctxAuth ( void *ctx ) ;
extern sLOGICAL gHyp_sock_ctxAuthClient ( void *ctx ) ;
extern void   gHyp_sock_destroyCTX ( void *ctx ) ;
extern void   gHyp_sock_deleteSSL ( sSSL *pSSL ) ;
extern void gHyp_sock_setSession ( sSSL *pSSL, void *session  ) ;
extern void *gHyp_sock_getSession ( sSSL *pSSL ) ;
extern sSSL* gHyp_sock_copySSL ( sSSL *pSSL ) ;
extern sSSL* gHyp_sock_createSSL ( void *ctx, sLOGICAL isClient ) ;
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
extern sData*		gHyp_sock_createNetwork ( sData*, char*, char*, int ) ; 
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
