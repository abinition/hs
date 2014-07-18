/*
 * Modifications:
 *
 * $Log: label.h,v $
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern sLabel *gHyp_label_new ();
extern void gHyp_label_delete ( sLabel *pLabel );
extern int gHyp_label_getIndex ( sLabel *pLabel ) ;
extern int gHyp_label_getDepth ( sLabel *pLabel ) ;
extern void gHyp_label_setLabel ( sLabel *pLabel, int labelIndex, int labelDepth ) ;
