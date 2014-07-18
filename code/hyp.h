/**********************	FUNCTION PROTOTYPES ********************************/
/* Modifications:
 *
 * $Log: hyp.h,v $
 * Revision 1.5  2007-07-09 05:39:00  bergsma
 * TLOGV3
 *
 * Revision 1.3  2006-01-16 18:56:36  bergsma
 * HS 3.6.6
 * 1. Save query timeout events.  Don't let queries repeat indefinitely.
 * 2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 * Revision 1.2  2002/10/23 00:18:16  bergsma
 * New argument to gHyp_hyp_traceReset
 *
 *
 */
extern sHyp*	gHyp_hyp_new ( char * ) ;
extern void 	gHyp_hyp_delete ( sHyp * ) ;
extern void    	gHyp_hyp_traceReport ( int ) ;
extern void    	gHyp_hyp_traceReset ( char * ) ;
extern void gHyp_hyp_describe ( sHyp *pHyp ) ;
extern char* gHyp_hyp_describe2 ( sHyp *pHyp, int hypIndex, char *pBuf, int buflen ) ;
extern void    	gHyp_hyp_setHypCount ( sHyp *, int ) ;
extern int     	gHyp_hyp_getHypCount ( sHyp * ) ;
extern int gHyp_hyp_getHighWaterCount ( sHyp *pHyp ) ;
extern char*    gHyp_hyp_method ( sHyp * ) ;
extern sBYTE   	gHyp_hyp_rank ( sCode * ) ; 
extern void   	gHyp_hyp_setRank ( sCode *, sBYTE rank ) ; 
extern void	gHyp_hyp_setPrecedence ( sCode *, sBYTE precedence ) ;
extern sBYTE   	gHyp_hyp_precedence ( sCode * ) ;
extern void   	gHyp_hyp_setTokenType ( sCode *, sBYTE tokenType ) ;
extern sBYTE   	gHyp_hyp_tokenType ( sCode * ) ;
extern int     	gHyp_hyp_index ( sCode * ) ;
extern int     	gHyp_hyp_line ( sCode * ) ;
extern char*   	gHyp_hyp_token ( sCode * ) ;
extern int	gHyp_hyp_parse ( sInstance*, sCode*, sHyp*, sFrame* );
extern int    	gHyp_hyp_execute ( sInstance*, sCode*);
extern sCode*  	gHyp_hyp_code ( sHyp *, int ) ;
extern void    	gHyp_hyp_error ( sHyp*, int, char*, ... ) ;
extern sLOGICAL gHyp_hyp_okUnaryOperator ( sHyp * ) ;
extern int	 gHyp_hyp_source ( sInstance*,sHyp *, FILE*, sLOGICAL ) ;
extern sLOGICAL gHyp_hyp_load (	sInstance*, sConcept*, sHyp*, char*, sBYTE,
				sBYTE, int, 
				void (*pf)(sInstance*,sCode*,sLOGICAL) ) ;
extern void gHyp_hyp_transfer ( sInstance*, sConcept*, sHyp*, sHyp*, int, int, int, sData* ) ;
