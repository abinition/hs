/**********************	FUNCTION PROTOTYPES ********************************/

/* Modifications:
 * 
 * $Log: frame.h,v $
 * Revision 1.4  2003/01/12 10:14:12  bergsma
 * Removed getVariable (moved to data.h).
 * Added testLocalFlag, setLocalFlag, clearLocalFlag
 *
 * Revision 1.3  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.2  2002/10/23 00:11:27  bergsma
 * Add gHyp_frame_dumpLevel, for debugging purposes only.
 *
 *
 */

extern void 		gHyp_frame_resetLevel ( sLevel * ) ;
extern void 		gHyp_frame_dumpLevel ( sFrame * ) ;
extern void 		gHyp_frame_reset ( sFrame * ) ;
extern void 		gHyp_frame_reset ( sFrame * ) ;
extern sFrame*		gHyp_frame_new () ;
extern void 		gHyp_frame_delete ( sFrame * ) ;
extern sLevel*		gHyp_frame_level ( sFrame * ) ;
extern int 		gHyp_frame_depth ( sFrame * ) ;
extern void		gHyp_frame_setGlobalScope ( sFrame *, sLOGICAL ) ;
extern sLOGICAL		gHyp_frame_isGlobalScope ( sFrame *) ;
extern sData*		gHyp_frame_findRootVariable ( sFrame*, char* );
extern sData*		gHyp_frame_findLocalVariable ( sFrame*, char* );
extern sData*		gHyp_frame_findVariable ( sInstance*, sFrame*, char* );
extern sData*		gHyp_frame_createRootVariable ( sFrame *, char* ) ;
extern sData*		gHyp_frame_createLocalVariable ( sFrame *, char* ) ;
extern sData*		gHyp_frame_createVariable ( sFrame *,char*);
extern sLOGICAL		gHyp_frame_deleteLocalVariable (sFrame *,char*);
extern sLOGICAL		gHyp_frame_deleteRootVariable (sFrame *,char*);
extern sLOGICAL		gHyp_frame_deleteVariable (sFrame *,char*);
extern void 		gHyp_frame_setMethodData ( sFrame *, sData* ) ;
extern sData*		gHyp_frame_getMethodData ( sFrame * ) ;
extern void		gHyp_frame_swapRootMethodData ( sFrame *pFrame1, sFrame *pFrame2 ) ;
extern sData*		gHyp_frame_getRootMethodData ( sFrame * ) ;
extern void 		gHyp_frame_setMethodVariable ( sFrame *, sData* ) ;
extern sData*		gHyp_frame_getMethodVariable ( sFrame * ) ;
extern sData*		gHyp_frame_findRootMethodVar ( sFrame *pFrame, char *pStr ) ;
extern sData*		gHyp_frame_findMethodVariable ( sFrame *, char*, sInstance* ) ; 
extern sParse*		gHyp_frame_parse ( sFrame *pFrame ) ;
extern sStack*		gHyp_frame_stack ( sFrame *pFrame ) ;
extern void 		gHyp_frame_setHyp ( sFrame *, sHyp* ) ;
extern void 		gHyp_frame_setHypIndex ( sFrame *, int ) ;
extern void 		gHyp_frame_incHypIndex ( sFrame * ) ;
extern sHyp* 		gHyp_frame_getHyp ( sFrame * ) ;
extern int 		gHyp_frame_getHypIndex ( sFrame * ) ;
extern sLOGICAL		gHyp_frame_branch ( sFrame * ) ;
extern sLOGICAL 	gHyp_frame_testLoopFlag ( sFrame *, sBYTE ) ;
extern void 		gHyp_frame_setLoopFlag ( sFrame *,  sBYTE ) ;
extern void	 	gHyp_frame_clearLoopFlag ( sFrame *, sBYTE ) ;
extern sLOGICAL 	gHyp_frame_testGlobalFlag ( sFrame *, sBYTE ) ;
extern void 		gHyp_frame_setGlobalFlag ( sFrame *,  sBYTE ) ;
extern void	 	gHyp_frame_clearGlobalFlag ( sFrame *, sBYTE ) ;
extern sLOGICAL 	gHyp_frame_testLocalFlag ( sFrame *, sBYTE ) ;
extern void 		gHyp_frame_setLocalFlag ( sFrame *,  sBYTE ) ;
extern void	 	gHyp_frame_clearLocalFlag ( sFrame *, sBYTE ) ;
extern void 		gHyp_frame_setState ( sFrame *, sBYTE ) ;
extern sBYTE 		gHyp_frame_getState ( sFrame * ) ;
extern sBYTE 		gHyp_frame_statementType ( sFrame * ) ;
extern int 		gHyp_frame_getStatementIndex ( sFrame * ) ;
extern void 		gHyp_frame_setStatementIndex ( sFrame *, int ) ;
extern sBYTE 		gHyp_frame_expectedState ( sFrame * ) ;
extern sBYTE 		gHyp_frame_currentState ( sFrame * ) ;
extern char*		gHyp_frame_expectedStateStr ( sFrame* ) ;
extern char*		gHyp_frame_currentStateStr ( sFrame* ) ;
extern void		gHyp_frame_setStates ( sFrame *, char, char ) ;
extern void		gHyp_frame_nextState ( sFrame *, char ) ;
extern sLOGICAL 	gHyp_frame_isStmtTrue ( sFrame * ) ;
extern sLOGICAL 	gHyp_frame_isStmtFalse ( sFrame * ) ;
extern void	 	gHyp_frame_endStmt ( sFrame*, sInstance* ) ;
extern void	 	gHyp_frame_newStmt ( sFrame *, char,char,char ) ;
extern sLOGICAL	 	gHyp_frame_return ( sFrame * ) ;
extern sLOGICAL	 	gHyp_frame_break ( sFrame * ) ;
extern sLOGICAL	 	gHyp_frame_continue ( sFrame * ) ;
extern sLOGICAL	 	gHyp_frame_goto ( sFrame *, char* ) ;

