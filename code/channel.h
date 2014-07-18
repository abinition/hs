/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: channel.h,v $
 * Revision 1.5  2007-07-09 05:39:00  bergsma
 * TLOGV3
 *
 * Revision 1.5  2006-08-08 20:48:27  bergsma
 * Added setTarget, setPath, setSocket
 *
 * Revision 1.4  2005/01/31 06:05:38  bergsma
 * Added gHyp_channel_offset  and gHyp_channel_setOffset
 *
 * Revision 1.3  2004/04/29 02:06:37  bergsma
 * Added pointer to SSL structure.
 *
 * Revision 1.2  2002/11/12 20:09:07  bergsma
 * Removed gHyp_channel_initStats
 *
 *
 */
extern sChannel*        gHyp_channel_new ( char*, char*, short flags, SOCKET fd  ) ;
extern void		gHyp_channel_close ( sChannel*) ;
extern void		gHyp_channel_delete ( sChannel* ) ;
extern sChannel*        gHyp_channel_next ( sChannel* ) ; 
extern short            gHyp_channel_flags ( sChannel* ) ;
extern void		gHyp_channel_object ( sChannel* ) ;
extern char*            gHyp_channel_target ( sChannel* ) ;
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
