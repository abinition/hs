/*
 * Modifications:
 *
 * $Log: secs.h,v $
 * Revision 1.4  2008-02-17 02:10:17  bergsma
 * Added secs_xml() function
 *
 * Revision 1.3  2007-02-24 01:54:52  bergsma
 * Added secs_*_raw functions.
 *
 * Revision 1.2  2002-11-14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void		gHyp_secs_mlb ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_xml ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_mhp ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_hsms ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_service ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_open ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_close ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_assign ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_enable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_disable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_event ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_query ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_event_raw ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_query_raw ( sInstance *, sCode *, sLOGICAL  ) ;
extern void		gHyp_secs_handle ( sInstance *, sCode *, sLOGICAL  ) ;
