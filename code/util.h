/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 2002 Abinition (TM), Inc                      !
!          ***** Program property of Abinition, Inc                           !
!          ***** All rights reserved - Licensed Material.                     !
!
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** Program property of Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!                                                                             !
!*****************************************************************************/

/*
 *  This program is dual-licensed: either;
 *
 *  Under the terms of the GNU General Public License version 3 as 
 *  published by the Free Software Foundation. For the terms of this 
 *  license, see licenses/gplv3.md or <http://www.gnu.org/licenses/>;
 *
 *  Under the terms of the Commercial License as set out in 
 *  licenses/commercial.md
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License or Commerical License for more details.
 */

/*
 * Modifications:
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
extern char*	 	gHyp_util_sprintf ( const char *fmt, ... ) ;
extern int		gHyp_util_getclock();
extern sLOGICAL 	gHyp_util_logDebug ( int, unsigned short,
					     const char *fmt, ...);
extern sLOGICAL 	gHyp_util_logError ( const char *fmt, ... ) ;
extern sLOGICAL 	gHyp_util_logWarning ( const char *fmt, ... ) ;
extern sLOGICAL 	gHyp_util_sysError ( const char *fmt, ... ) ;
extern sLOGICAL 	gHyp_util_check ( int, short int ) ;
extern sLOGICAL 	gHyp_util_isIdentifier ( char* ) ;
extern int 		gHyp_util_getToken ( char*, sLOGICAL ) ;
extern int 		gHyp_util_getToken_okDash ( char* ) ;
extern int 		gHyp_util_getToken_okDot ( char* ) ;
extern int 		gHyp_util_getToken_okDashDot ( char* ) ;
extern sLOGICAL 	gHyp_util_match ( char *, char * ) ;
extern void		gHyp_util_gethostname ( char * ) ;
extern void 		gHyp_util_getprocessname ( char * ) ;
extern void 		gHyp_util_setprocessname ( char * ) ;
extern int		gHyp_util_parseXML ( char* ) ;
extern int		gHyp_util_parseString ( char* ) ;
extern int		gHyp_util_unparseString ( char*, char*, int, int, sLOGICAL, sLOGICAL, sLOGICAL,char* ) ;
extern void 		gHyp_util_strip ( char *, int ) ;
extern int 		gHyp_util_trim ( char * ) ;
extern int 		gHyp_util_trimWhiteSpace ( char * ) ;
extern sLOGICAL		gHyp_util_openLog () ;
extern sLOGICAL		gHyp_util_isLittleEndian() ;
extern sLOGICAL		gHyp_util_breakTarget ( char *, char *, char*, char*, char*, char* ) ;
extern int gHyp_util_base64Decode( char *in, int size_in, char *out )  ;
extern int gHyp_util_base64Encode( char *in, int size_in, char *out )  ;
extern int gHyp_util_urlDecode( char *in, int size_in, char *out )  ;
extern int gHyp_util_urlEncode( char *in, int size_in, char *out )  ;
extern char *gHyp_util_readStream (  char *pStream,
				    char *pAnchor,
				    char **ppEndOfStream,
				    int *pStreamLen,
				    sData *pStreamData,
				    sData **ppValue,
				    int *pContext,
				    int ss,
				    sLOGICAL isVector,
				    FILE *pp )  ;

extern void gHyp_util_breakStream ( char *buffer, int bufLen, sData *pParent, sLOGICAL isLineBased ) ;
extern int gHyp_util_breakStringForMsg( char *buffer, int bufLen, int maxBufLen ) ;
extern char *gHyp_util_memmem(const void *buf, const void *pattern, size_t buflen, size_t len) ;
extern sLOGICAL gHyp_util_maybeDouble ( char *token ) ;
