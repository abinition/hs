/* External Global function prototypes */
/*
 * Modifications:
 *
 * $Log: router.h,v $
 * Revision 1.5  2007-07-09 05:39:00  bergsma
 * TLOGV3
 *
 * Revision 1.3  2005-01-31 06:06:01  bergsma
 * Arguments to gHyp_router_process changed.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern int gHyp_router_message ( sConcept *,
				 sData *p, 
				 sData *,
				 char *,
				 char *) ; 
extern int gHyp_router_process ( sConcept*,
				 sData *,
				 sData *,
				 char *, 
				 char*,  
				 int, 
				 int*,
				 char* ) ;
