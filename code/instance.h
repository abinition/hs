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
extern void    	gHyp_instance_restoreTimeOut ( sInstance * ) ;
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
