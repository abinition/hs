/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: operator.h,v $
 * Revision 1.5  2007-07-09 05:39:00  bergsma
 * TLOGV3
 *
 * Revision 1.3  2007-03-19 05:32:08  bergsma
 * New functions: min, max, pow, hypot, abs, acos, asin, atan,  ceil, cos,
 *  floor, log10, logN, sin, sqrt, tan, exp printf, sprintf, fprintf
 *
 * Revision 1.2  2002-11-14 01:40:45  bergsma
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
extern void    	gHyp_operator_max ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_min ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_hypot ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_pow ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_abs ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_acos ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_asin ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_atan ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_ceil ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_cos ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_floor ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_log10 ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_logN ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_sin ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_sqrt ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_tan ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_operator_exp ( sInstance*, sCode*, sLOGICAL ) ;

