/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: operator.h,v $
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void 	gHyp_operator_binary ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_unary ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_assign ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_condition ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_reference ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_dereference ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_eval ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_dot ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_operator_pointer ( sInstance*, sCode*, sLOGICAL ) ;

