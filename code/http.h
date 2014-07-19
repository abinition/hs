/*
 * Modifications:
 *
 * $Log: http.h,v $
 * Revision 1.9  2010-05-05 04:55:41  bergsma
 * Added http_asctime
 *
 * Revision 1.8  2009-12-24 15:56:55  bergsma
 * More fixes for handling form data via HTTP
 *
 * Revision 1.7  2009-12-19 05:54:21  bergsma
 * Handle upload of form data
 *
 * Revision 1.6  2009-03-02 00:34:35  bergsma
 * Improved handling of zero-length content buffers
 *
 * Revision 1.5  2006-10-11 16:16:00  bergsma
 * Make EAGAIN an optional feature that must be turned on.
 *
 * Revision 1.4  2005/03/16 23:53:21  bergsma
 * V 3.5.1 - fixes for use with DECC compiler.
 *
 * Revision 1.3  2004/10/16 04:45:41  bergsma
 * Added http_binary() function.
 *
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
extern int	gHyp_http_getActualContentLen( sHTTP* ) ;
extern void	gHyp_http_updateContent ( sHTTP* pHTTP, int contentlength, int actualContentLength ) ;
extern int	gHyp_http_getContentLength( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isChunkedTransferEnc( sHTTP* pHTTP ) ;
extern void	gHyp_http_setChunkedTransferEnc( sHTTP* pHTTP ) ;
extern char*	gHyp_http_getFormDataBoundary( sHTTP* pHTTP ) ;
extern void	gHyp_http_setFormDataBoundary( sHTTP* pHTTP, char* ) ;
extern sLOGICAL	gHyp_http_isXMLEncoded( sHTTP* pHTTP ) ;
extern void	gHyp_http_setXMLEncoded( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isURLEncoded( sHTTP* pHTTP ) ;
extern void	gHyp_http_setURLEncoded( sHTTP* pHTTP ) ;
extern sLOGICAL	gHyp_http_isZeroLength( sHTTP* pHTTP ) ;
extern void	gHyp_http_setZeroLength( sHTTP* pHTTP ) ;
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
extern void	gHyp_http_eagain ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_http_event ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_query ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_reply ( sInstance *, sCode *, sLOGICAL  ) ;
extern void	gHyp_http_handle ( sInstance *, sCode *, sLOGICAL  ) ;
extern void     gHyp_http_asctime ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;

