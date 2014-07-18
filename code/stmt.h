/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: stmt.h,v $
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void 	gHyp_stmt_if ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_else ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_bExp ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_eExp ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_bList ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_list ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_eList ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_bSub ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_eSub ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_while ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_do ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_for ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_eos ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_break ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_return ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_stmt_continue ( sInstance*, sCode*, sLOGICAL ) ;
