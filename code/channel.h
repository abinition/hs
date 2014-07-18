/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: channel.h,v $
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
extern char*            gHyp_channel_path ( sChannel* ) ;
extern int		gHyp_channel_maxMsgSize ( sChannel* ) ;
extern void             gHyp_channel_setMaxMsgSize ( sChannel*, int ) ;
extern SOCKET           gHyp_channel_socket ( sChannel* ) ;
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
