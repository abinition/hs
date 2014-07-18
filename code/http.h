/*
 * Modifications:
 *
 * $Log: http.h,v $
 * Revision 1.2  2004/06/13 14:06:51  bergsma
 * Recognize plain/text as flag.
 *
 * Revision 1.1  2004/04/29 02:33:56  bergsma
 * New http functions.
 *
 *
 */
extern sHTTP*	gHyp_http_new () ;
extern void	gHyp_http_delete ( sHTTP *pHTTP ) ;
extern void	gHyp_http_reset ( sHTTP *pHTTP ) ;
extern void	gHyp_http_setState ( sHTTP* pHTTP, sBYTE ) ;
extern sBYTE	gHyp_http_getState ( sHTTP* ) ;
extern char*	gHyp_http_getArg1 ( sHTTP* ) ;
extern char*	gHyp_http_getArg2 ( sHTTP* ) ;
extern char*	gHyp_http_getArg3 ( sHTTP* ) ;
extern sData*	gHyp_http_getAttributeData( sHTTP* ) ;
extern sData*   gHyp_http_getContentData ( sHTTP* ) ;
extern int	gHyp_http_getActualContentLength( sHTTP* ) ;
extern void	gHyp_http_updateContent ( sHTTP* pHTTP, int contentlength, int actualContentLength ) ;
extern int	gHyp_http_getContentLength( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isChunkedTransferEncoded( sHTTP* pHTTP ) ;
extern void	gHyp_http_setChunkedTransferEncoded( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isXMLEncoded( sHTTP* pHTTP ) ;
extern void	gHyp_http_setXMLEncoded( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isURLEncoded( sHTTP* pHTTP ) ;
extern void	gHyp_http_setURLEncoded( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isPlainText( sHTTP* pHTTP ) ;
extern void	gHyp_http_setPlainText( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isBinary( sHTTP* pHTTP ) ;
extern void	gHyp_http_setBinary( sHTTP* pHTTP, sLOGICAL isBinary ) ;
extern void	gHyp_http_setHeader ( sHTTP*, char* pArg1, char* pArg2, char* pArg3 ) ;
extern sLOGICAL gHyp_http_isReply ( sHTTP *pHTTP ) ;
extern void	gHyp_http_service ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_open ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_close ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_assign ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_enable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_disable ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_binary ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_http_event ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_query ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_reply ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_handle ( sInstance *, sCode *, sLOGICAL  ) ;
