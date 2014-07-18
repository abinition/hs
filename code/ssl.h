/*
 * Modifications:
 *
 * $Log: ssl.h,v $
 * Revision 1.2  2004/05/03 18:03:59  bergsma
 * Removing Windows embedded ^M
 *
 * Revision 1.1  2004/04/29 02:15:02  bergsma
 * Added ssl functions.
 *
 */
extern void gHyp_ssl_new ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_ciphers ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_keyFile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_certFile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_CAfile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_CApath ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_auth ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_authClient ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_assign ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void gHyp_ssl_delete ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_setSession ( sInstance *, sCode *, sLOGICAL  ) ;
extern void gHyp_ssl_getSession ( sInstance *, sCode *, sLOGICAL  ) ;
