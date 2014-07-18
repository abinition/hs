/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: branch.h,v $
 * Revision 1.3  2004-04-29 01:55:53  bergsma
 * Added 'on_connect' function.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void 	gHyp_branch_goto ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_message ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_error ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_timeout ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_alarm ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_interrupt ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_hangup ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_connect ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_pipe ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_branch_on_death ( sInstance*, sCode*, sLOGICAL ) ;

