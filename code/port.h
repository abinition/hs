/*
 * Modifications:
 *
 * $Log: port.h,v $
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void		gHyp_port_service ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_port_open ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_port_close ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_port_assign ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_port_enable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_port_disable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_port_event ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_port_query ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_port_handle ( sInstance *, sCode *, sLOGICAL  ) ;
