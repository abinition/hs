/*
 * Modifications:
 *
 * $Log: hsms.h,v $
 * Revision 1.3  2004/04/29 08:31:33  bergsma
 * Added SSL support.  Device id is now sWORD, not int.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern sHsms*		gHyp_hsms_new ( short,
					SOCKET,
					char*,int,
					float,float,float,float,
					sBYTE,
					SOCKET,
					sInstance* ) ;
extern void		gHyp_hsms_delete ( sHsms * ) ;
extern void		gHyp_hsms_close ( sHsms * ) ; 
extern int		gHyp_hsms_nextMessage ( sHsms *, sConcept*, sInstance* ) ;
extern int		gHyp_hsms_incoming ( sHsms*, sConcept*, sInstance*) ;
extern int		gHyp_hsms_outgoing ( sHsms*,
					     sSecs2*,
					     sInstance*,
					     int,int,int,int,int,sBYTE);
extern SOCKET		gHyp_hsms_socket ( sHsms *pHsms ) ;
extern short		gHyp_hsms_flags ( sHsms *pHsms ) ;
extern SOCKET		gHyp_hsms_parentSocket ( sHsms *pHsms ) ;
extern sInstance*	gHyp_hsms_parentAI ( sHsms *pHsms ) ;
extern sBYTE*		gHyp_hsms_buf ( sHsms* ) ;
extern unsigned gHyp_hsms_port ( sHsms *pHsms ) ;
extern float		gHyp_hsms_t5 ( sHsms * ) ;
extern float		gHyp_hsms_t6 ( sHsms * ) ;
extern float		gHyp_hsms_t7 ( sHsms * ) ;
extern float		gHyp_hsms_t8 ( sHsms * ) ;
extern char*		gHyp_hsms_node ( sHsms * ) ;
extern int		gHyp_hsms_TID ( sHsms * ) ;
extern int		gHyp_hsms_SID ( sHsms * ) ;
extern int*		gHyp_hsms_pNbytes ( sHsms *pHsms ) ;
extern LPOVERLAPPED	gHyp_hsms_overlapped ( sHsms * ) ;
extern void gHyp_hsms_setSSL ( sHsms *pHsms, sSSL *pSSL ) ;
extern sSSL *gHyp_hsms_getSSL ( sHsms *pHsms ) ;
