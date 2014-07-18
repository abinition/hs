/*
 * Modifications:
 *
 * $Log: method.h,v $
 * Revision 1.2  2002-11-14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern sMethod *gHyp_method_new () ;
extern void gHyp_method_delete ( sMethod *pMethod ) ; 
extern sHyp *gHyp_method_getHyp ( sMethod *pMethod ) ; 
extern void gHyp_method_setHyp ( sMethod *pMethod, sHyp *pHyp ) ;
extern int gHyp_method_getIndex ( sMethod *pMethpd ) ;
extern void gHyp_method_setIndex ( sMethod *pMethod, int hypIndex ) ;
extern sData* gHyp_method_getArgs ( sMethod *pMethod ) ;
extern void gHyp_method_setArgs ( sMethod *pMethod, sData *pArgs ) ;
extern void gHyp_method_enable ( sMethod *pMethod ) ;
extern void gHyp_method_disable ( sMethod *pMethod ) ;
extern sLOGICAL gHyp_method_isEnabled ( sMethod *pMethod ) ;
