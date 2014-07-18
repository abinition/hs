/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: stack.h,v $
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern sStack*		gHyp_stack_new () ;
extern void 		gHyp_stack_flush ( sStack * ) ;
extern void 		gHyp_stack_delete ( sStack * ) ;
extern int 		gHyp_stack_depth ( sStack * ) ;
extern void	 	gHyp_stack_push ( sStack *, sData * ) ;
extern sData* 		gHyp_stack_pop ( sStack * ) ;
extern sData* 		gHyp_stack_pop2 ( sStack * ) ;
extern sData*		gHyp_stack_peek ( sStack * ) ;
extern sData* 		gHyp_stack_popRvalue ( sStack *, sInstance * ) ;
extern sData* 		gHyp_stack_popRdata ( sStack *, sInstance * ) ;
extern sData* 		gHyp_stack_popRdata2 ( sStack *, sInstance * ) ;
extern sData* 		gHyp_stack_popLvalue ( sStack *, sInstance *) ;
