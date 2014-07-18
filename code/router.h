/* External Global function prototypes */
/*
 * Modifications:
 *
 * $Log: router.h,v $
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
				 int,
				 char* ) ;
