/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: sql.h,v $
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
#ifndef AS_PROC
extern void		gHyp_sql_query ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_sql_open ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_sql_close ( sInstance*, sCode*, sLOGICAL ) ;
#else
extern void		gHyp_sql_query ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_sql_connect ( sInstance*, sCode*, sLOGICAL ) ;
#endif
