/*
 * Modifications:
 *
 * $Log: secs1.h,v $
 * Revision 1.6  2004/10/16 05:03:22  bergsma
 * Added gHyp_secs1_getHttp function.
 *
 * Revision 1.5  2004/06/13 14:11:03  bergsma
 * For write operations, detect EAGAIN/EWOULDBLOCK, return 0 instead of -1
 *
 * For PROMIS, in the CONFIG_TEMPLATE.COM, an invalid username sets the
 * automation logicals to default values - JOB only, not GROUP.
 *
 * For data coming into a port, set it to REPLY data only if there is
 * and outstanding query on the same port.
 *
 * For line-based HTTP data (text/plan, text/xml, text/html), provide
 * knitting of lines between chunks of data read, also accomodate lines
 * larger than INTERNAL_VALUE_SIZE if they really would be represented
 * external in VALUE_SIZE space.
 *
 * Revision 1.4  2004/04/29 08:35:42  bergsma
 * Added SSL support.
 *
 * Revision 1.3  2002/11/18 23:09:31  bergsma
 * Removed ^M
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
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
extern int		gHyp_secs1_rawIncoming ( sSecs1*,sConcept*,sInstance*,sBYTE) ;
extern int		gHyp_secs1_rawOutgoing ( sSecs1*,sInstance*,sData*, int id ) ;
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
extern sHTTP*		gHyp_secs1_getHttp ( sSecs1 *pSecs1 ) ;
extern sSSL*		gHyp_secs1_getSSL ( sSecs1 *pSecs1 ) ;
extern sData*		gHyp_secs1_getForwardPorts ( sSecs1 *pSecs1 ) ;
extern void		gHyp_secs1_addForwardPort ( sSecs1 *pSecs1, int from_id, int to_id ) ;
extern sData*		gHyp_secs1_copyForwardPort ( sSecs1 *pSecs1, int  ) ;
extern void		gHyp_secs1_removeForwardPort ( sSecs1 *pSecs1, int from_id, int to_id ) ;

