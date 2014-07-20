/*
 * Modifications:
 *
 * $Log: tcp.h,v $
 * Revision 1.9  2011-01-08 21:34:40  bergsma
 * TRUE64 is really TRU64
 *
 * Revision 1.8  2007-02-17 01:53:13  bergsma
 * Socket handoff does not work with TRUE64
 *
 * Revision 1.7  2006-10-27 17:27:19  bergsma
 * Added port_sendmsg and port_recvmsg
 *
 * Revision 1.6  2005/02/25 04:00:51  bergsma
 * HS 3.4.5
 * Make mailslot read/writes non-blocking.
 *
 * Revision 1.5  2004/07/01 02:18:28  bergsma
 * Functions without stdout to use with JNI interface
 *
 * Revision 1.4  2004/04/29 02:13:01  bergsma
 * Support for 127.*.*.* interfaces.
 * Do not do gethostbyname or gethostbyaddr unless necessary,
 * these functions block.
 *
 * Revision 1.3  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.2  2002/09/20 00:03:46  bergsma
 * gHyp_tcp_createAlias returns sAlias*
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
