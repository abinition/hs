/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: instance.h,v $
 * Revision 1.19  2012-01-29 21gsettim:18:20  bergsma
 * Build 3.9.4 - 120202
 *
 * Revision 1.18  2012-01-08 01:25:25  bergsma
 * More work on ENQ contention.
 *
 * Revision 1.17  2011-09-20 05:35:54  bergsma
 * Fixed problem with replyMessage occurring to early, also problem
 * with ENQ contention and resending when the message queue was empty.
 *
 * Revision 1.16  2009-12-08 20:48:20  bergsma
 * Add pushLocalSTATUS method
 *
 * Revision 1.15  2009-11-17 16:05:52  bergsma
 * Fixes ENQ Contention problem
 *
 * Revision 1.14  2008-02-17 02:10:17  bergsma
 * Added secs_xml() function
 *
 * Revision 1.13  2007-02-26 02:35:01  bergsma
 * No longer user NULL_DEVICEID placeholder.  PORT and HTTP
 * autoallocate device ids, HSMS and SECS I are pre-determined.
 *
 * Revision 1.12  2006-12-09 00:06:44  bergsma
 * Move gpsAI and gpsAImain to global external status out of hs.c.
 *
 * Revision 1.11  2006/09/25 05:12:19  bergsma
 * Problem 90% solved for renamed parent concept issues.
 *
 * Revision 1.10  2006/01/19 20:28:01  bergsma
 * Added gHyp_instance_hasNullDeviceId, this allows different instances to
 * do listen socket assignments to the same socket.
 *
 * Revision 1.9  2006/01/16 18:56:36  bergsma
 * HS 3.6.6
 * 1. Save query timeout events.  Don't let queries repeat indefinitely.
 * 2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 * Revision 1.8  2005/11/23 16:57:17  bergsma
 * Added gHyp_instance_swapDevices
 *
 * Revision 1.7  2005/10/15 21:42:09  bergsma
 * Added renameto functionality.
 *
 * Revision 1.6  2005/01/25 05:48:00  bergsma
 * Fix problem where wakeup from SLEEP not always detected, if colliding with
 * HEARTBEAT.
 *
 * Revision 1.5  2004/04/29 08:34:41  bergsma
 * Device Id is unique and is used interchangeably between PORT, HSMS, SECSI, and HTTP sockets.
 * Better handling of fd/id tables.
 *
 * Revision 1.4  2003/01/12 07:01:48  bergsma
 * V3.1.0
 * Added _setTargetPath and _pushSTATUS
 *
 * Revision 1.3  2002/11/20 20:42:59  bergsma
 * Changed gHyp_instance_isSignalMsg to gHyp_instance_isSignal
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern sInstance* gHyp_instance_new () ;
extern void    	gHyp_instance_init ( sInstance*, sConcept*, char*, char* ) ;
extern void    	gHyp_instance_delete ( sInstance * ) ;
extern sConcept *gHyp_instance_getConcept ( sInstance *pAI ) ;
extern void gHyp_instance_swapFrames ( sInstance *pAI, sInstance *pAImain ) ;
extern void gHyp_instance_swapData ( sInstance *pAI, sInstance *pAImain ) ;
extern void gHyp_instance_swapDevices ( sInstance *pAI, sInstance *pAImain ) ;
extern sFrame* 	gHyp_instance_frame ( sInstance *) ;
extern void    	gHyp_instance_setExpectedReply(sInstance*,char*,char*,char*,int);
extern void    	gHyp_instance_requeue ( sInstance* ) ;
extern void  gHyp_instance_signalPipe ( sInstance *pAI, int sigarg, int sigarg2, int sigarg3 ) ;
extern void  gHyp_instance_signalHangup ( sInstance *pAI, int sigarg, int sigarg2, int sigarg3 ) ;
extern void  gHyp_instance_signalConnect ( sInstance *pAI, int sigarg, int sigarg2, int sigarg3 ) ;
extern void  gHyp_instance_signalAlarm ( sInstance *pAI ) ;
extern void  gHyp_instance_signalInterrupt ( sInstance *pAI ) ;
extern void  gHyp_instance_signalTerm ( sInstance *pAI ) ;
extern sLOGICAL gHyp_instance_isSignal ( sInstance *pAI ) ;
extern void  gHyp_instance_signalMsg ( sInstance *pAI ) ;
extern int gHyp_instance_nextEvent ( sInstance *pAI ) ;
extern int gHyp_instance_read ( sInstance*, sLOGICAL ) ;
extern int gHyp_instance_readReply ( sInstance *pAI ) ;
extern int gHyp_instance_readQueue ( sInstance* pAI ) ;
extern int gHyp_instance_readProcess ( sInstance *pAI, sBYTE state ) ;
extern int gHyp_instance_readSignals ( sInstance *pAI ) ;
extern void	gHyp_instance_setSecsMLB ( sInstance*, sBYTE ) ; 
extern sBYTE	gHyp_instance_getSecsMLB ( sInstance* ) ;
extern void	gHyp_instance_setSecsXML ( sInstance*, sLOGICAL ) ; 
extern sLOGICAL	gHyp_instance_getSecsXML ( sInstance* ) ;
extern void	gHyp_instance_setSecsMHP ( sInstance*, sWORD ) ; 
extern sWORD	gHyp_instance_getSecsMHP ( sInstance* ) ;
extern void 	gHyp_instance_setSecsReplyOut (sInstance*,sWORD,int,int,int,int);
extern void 	gHyp_instance_setSecsReplyIn (sInstance*,sWORD,int,int,int,int);
extern void	gHyp_instance_incOutgoingDepth ( sInstance * ) ;
extern void	gHyp_instance_decOutgoingDepth ( sInstance * ) ;
extern void	gHyp_instance_incIncomingDepth ( sInstance * ) ;
extern void	gHyp_instance_decIncomingDepth ( sInstance * ) ;
extern void	gHyp_instance_secsReply ( sInstance *, sData * ) ;
extern void	gHyp_instance_secsEnable(sInstance*,sWORD,int,int,sData*);
extern void	gHyp_instance_secsDisable(sInstance*,sWORD,int,int);
extern sData*	gHyp_instance_secsMethod ( sInstance*,sWORD, int, int ) ;
extern void	gHyp_instance_saveMsgPending ( sInstance* ) ;
extern void	gHyp_instance_secsIncoming ( sInstance*,sWORD,int,int,int,int);
extern sSecs2*  gHyp_instance_getSecs2byId ( sInstance *pAI, sWORD id ) ;
extern sLOGICAL	gHyp_instance_updateFd(sInstance*,SOCKET,sWORD,sSecs2*,sLOGICAL);
extern void gHyp_instance_deleteFd ( sInstance *pAI, SOCKET fd ) ;
extern SOCKET	gHyp_instance_getDeviceFd ( sInstance *, sWORD ) ;
extern sWORD    gHyp_instance_getDeviceId ( sInstance *pAI, SOCKET fd ) ;
extern sLOGICAL gHyp_instance_hasDeviceId ( sInstance *pAI, SOCKET fd ) ;
extern sLOGICAL gHyp_instance_isRbitSet ( sInstance *, sWORD ) ;
extern void	gHyp_instance_portEnable ( sInstance*, sWORD , sData* ) ;
extern void	gHyp_instance_portDisable ( sInstance*, sWORD ) ;
extern sData*	gHyp_instance_portMethod ( sInstance*, sWORD ) ;
extern sLOGICAL	gHyp_instance_replyMessage ( sInstance*, sData* ) ;
extern int gHyp_instance_ENQcontention ( sInstance * pAI, sFrame * pFrame ) ;
extern void    	gHyp_instance_reset ( sInstance*, char, sLOGICAL ) ;
extern sBYTE   	gHyp_instance_getState ( sInstance * ) ;
extern void    	gHyp_instance_setState ( sInstance *, sBYTE ) ;
extern void    	gHyp_instance_newTargetId ( sInstance * ) ;
extern char* gHyp_instance_getTargetId ( sInstance *pAI ) ;
extern char* gHyp_instance_getTargetPath ( sInstance *pAI ) ;
extern void gHyp_instance_setTargetPath ( sInstance *pAI, char* targetPath ) ;
extern sAImsg* 	gHyp_instance_currentMsg ( sInstance * ) ;
extern sAImsg*	gHyp_instance_outgoingMsg ( sInstance * ) ;
extern sAImsg*	gHyp_instance_incomingMsg ( sInstance * ) ;
extern sLOGICAL gHyp_instance_atCorrectDepth ( sInstance *pAI, char *pMethodStr, int depth );
extern sLOGICAL	gHyp_instance_isEND ( sInstance * ) ;
extern void    	gHyp_instance_setAlarm ( sInstance *, int ) ;
extern void    	gHyp_instance_setAlarmHandler ( sInstance *, int, sHyp* ) ;
extern void    	gHyp_instance_setDeath ( sInstance *, int ) ;
extern void    	gHyp_instance_setDeathHandler ( sInstance *, int, sHyp* ) ;
extern void    	gHyp_instance_setErrorHandler ( sInstance *, int, sHyp* ) ; 
extern void    	gHyp_instance_setIntHandler ( sInstance *, int, sHyp* );
extern void    	gHyp_instance_setHangupHandler ( sInstance *, int, sHyp* );
extern void    	gHyp_instance_setConnectHandler ( sInstance *, int, sHyp* );
extern void    	gHyp_instance_setPipeHandler ( sInstance *, int, sHyp* );
extern void    	gHyp_instance_setMessageHandler ( sInstance *, int, sHyp* ) ;
extern void    	gHyp_instance_setMethodCall ( sInstance * ) ;
extern void    	gHyp_instance_setDerefHandler (sInstance *,int, sHyp* );
extern void	gHyp_instance_setBeatTime ( sInstance *, int );
extern void	gHyp_instance_setWakeTime ( sInstance *, int );
extern time_t	gHyp_instance_getWakeTime ( sInstance * );
extern time_t	gHyp_instance_getTimeOutTime ( sInstance *);
extern void	gHyp_instance_cancelTimeOut ( sInstance *, int depth);
extern void    	gHyp_instance_initTimeOut ( sInstance *, int ) ;
extern void    	gHyp_instance_setTimeOut ( sInstance * ) ;
extern void    	gHyp_instance_setTimeOutHandler ( sInstance *, int, sHyp* ) ;
extern sData*  	gHyp_instance_setStatus ( sInstance *, char * ) ;
extern void	gHyp_instance_warning ( sInstance*,char*,char*, ...) ;
extern void    	gHyp_instance_error ( sInstance*, char*, char*, ... ) ;
extern void    	gHyp_instance_clearError ( sInstance* ) ;
extern sLOGICAL	gHyp_instance_handleCondition ( sInstance * pAI ) ;
extern int	gHyp_instance_parse ( sInstance * ) ;
extern int     	gHyp_instance_run ( sInstance * ) ;
extern void	gHyp_instance_pushLocalSTATUS ( sInstance *pAI, sStack *pStack ) ;
extern void     gHyp_instance_pushSTATUS ( sInstance *pAI, sStack *pStack ) ;
extern void     gHyp_instance_setgpAI( sInstance *pAI ) ;
extern void     gHyp_instance_setgpAImain ( sInstance *pAI ) ;
