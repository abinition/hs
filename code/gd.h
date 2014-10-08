/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: gd.h,v $
 * Revision 1.2  2009-03-08 16:17:45  bergsma
 * First successfull hs/gd compile&link
 *
 * Revision 1.1  2009-03-01 22:24:51  bergsma
 * no message
 *
 *
 */
#ifdef AS_GD
extern void		gHyp_gd_open ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_gd_close ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_gd_query ( sInstance*, sCode*, sLOGICAL ) ;
#endif
