/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: util.h,v $
 * Revision 1.7  2004/07/01 02:02:34  bergsma
 * Added 'specialChars' argument to the toexternal() function.
 *
 * Revision 1.6  2004/04/29 01:58:47  bergsma
 * Added encode and decode functions for BASE64
 *
 * Revision 1.5  2003/04/04 15:58:56  bergsma
 * Added 'n' count argument to strip function.
 * Added getToken3 function.
 *
 * Revision 1.4  2003/01/09 00:09:33  bergsma
 * V3.1.0
 * Removed gHyp_util_stripTargetSender
 *
 * Revision 1.3  2002/11/20 20:54:06  bergsma
 * Add root argument to breakTarget, remove second argument to stipTargetSender.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern int 		gHyp_util_getopt ( int, char **, char * ) ;
extern char* 		gHyp_util_timeStamp ( time_t ) ;
extern char* 		gHyp_util_random8 () ;
extern void 		gHyp_util_upperCase ( char *, int ) ;
extern void 		gHyp_util_lowerCase ( char *, int ) ;
extern void 		gHyp_util_output ( char * ) ;
extern void 		gHyp_util_output2 ( char * ) ;
extern void	 	gHyp_util_trace ( char * ) ;
extern sLOGICAL 	gHyp_util_log ( char * ) ;
extern sLOGICAL 	gHyp_util_logInfo ( const char *fmt, ... ) ;
extern sLOGICAL 	gHyp_util_debug ( const char *fmt, ... ) ;
extern sLOGICAL 	gHyp_util_logDebug ( int, unsigned short,
					     const char *fmt, ...);
extern sLOGICAL 	gHyp_util_logError ( const char *fmt, ... ) ;
extern sLOGICAL 	gHyp_util_logWarning ( const char *fmt, ... ) ;
extern sLOGICAL 	gHyp_util_sysError ( const char *fmt, ... ) ;
extern sLOGICAL 	gHyp_util_check ( int, short int ) ;
extern sLOGICAL 	gHyp_util_isIdentifier ( char* ) ;
extern int 		gHyp_util_getToken ( char*, sLOGICAL ) ;
extern int 		gHyp_util_getToken2 ( char* ) ;
extern int 		gHyp_util_getToken3 ( char* ) ;
extern sLOGICAL 	gHyp_util_match ( char *, char * ) ;
extern void		gHyp_util_gethostname ( char * ) ;
extern void 		gHyp_util_getprocessname ( char * ) ;
extern void 		gHyp_util_setprocessname ( char * ) ;
extern int		gHyp_util_parseXML ( char* ) ;
extern int		gHyp_util_parseString ( char* ) ;
extern int		gHyp_util_unparseString ( char*, char*, int, int, sLOGICAL, sLOGICAL, char* ) ;
extern void 		gHyp_util_strip ( char *, int ) ;
extern void 		gHyp_util_trim ( char * ) ;
extern void 		gHyp_util_trimWhiteSpace ( char * ) ;
extern sLOGICAL		gHyp_util_openLog () ;
extern sLOGICAL		gHyp_util_isLittleEndian() ;
extern sLOGICAL		gHyp_util_breakTarget ( char *, char *, char*, char*, char*, char* ) ;
extern int gHyp_util_base64Decode( char *in, int size_in, char *out )  ;
extern int gHyp_util_base64Encode( char *in, int size_in, char *out )  ;

