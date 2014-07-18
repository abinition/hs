/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: mapi.h,v $
 * Revision 1.5  2007-07-09 05:39:00  bergsma
 * TLOGV3
 *
 * Revision 1.1  2005-01-25 05:43:42  bergsma
 * Add MAPI (NSAS Nokia support methods for Outlook forwarding)
 *
 *
 */
#ifdef AS_MAPI
extern void gHyp_mapi_register ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_mapi_getPort ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
#endif