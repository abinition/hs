/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: sql.h,v $
 * Revision 1.3  2005/04/13 13:45:55  bergsma
 * HS 3.5.6
 * Added sql_toexternal.
 * Fixed handling of strings ending with bs (odd/even number of backslashes)
 * Better handling of exception condition.
 *
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
extern void		gHyp_sql_toexternal(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
