/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: signl.h,v $
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern Sigfunc * 	gHyp_signal_establish ( int, Sigfunc * ) ;
extern void		gHyp_signal_block ( int ) ;
extern void		gHyp_signal_unblock ( int ) ;
