/*
 * Modifications:
 *
 * $Log: port.h,v $
 * Revision 1.5  2006-10-27 17:27:19  bergsma
 * Added port_sendmsg and port_recvmsg
 *
 * Revision 1.4  2006/10/11 16:16:00  bergsma
 * Make EAGAIN an optional feature that must be turned on.
 *
 * Revision 1.3  2006/01/23 05:12:02  bergsma
 * Added port_binary() function.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void	gHyp_port_service ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_open ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_close ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_assign ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_enable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_disable ( sInstance *, sCode *, sLOGICAL ) ; 
extern void	gHyp_port_binary ( sInstance *, sCode *, sLOGICAL ) ;
extern void	gHyp_port_eagain ( sInstance *, sCode *, sLOGICAL ) ;
extern void	gHyp_port_event ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_query ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_handle ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_recvmsg ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_port_sendmsg ( sInstance *, sCode *, sLOGICAL  ) ;
