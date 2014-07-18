/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: function.h,v $
 * Revision 1.9  2005/09/06 08:52:05  bergsma
 * Updated vt2html() function.,
 * Added pack() function
 *
 * Revision 1.8  2005/07/23 22:32:19  bergsma
 * Added  vt2html
 *
 * Revision 1.7  2005/05/10 17:38:45  bergsma
 * Names are urlEncode and urlDecode
 *
 * Revision 1.6  2005/05/10 17:32:38  bergsma
 * Added URLencode and URLdecode functions
 *
 * Revision 1.5  2004/04/29 08:30:45  bergsma
 * New functions encode/decode.
 *
 * Revision 1.4  2003/04/04 16:40:27  bergsma
 * Add the strip function.
 *
 * Revision 1.3  2003/01/12 07:16:09  bergsma
 * Removed poll()
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void    	gHyp_function_alarm ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_function_count ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_debug ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_exists ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_exit ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_toupper ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_tolower ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_strip ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_toexternal ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_tointernal ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_idle ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_instantiate ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_lifetime ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_method ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_mode ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_tid ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_sid ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_self ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_sender ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_strlen ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_pack ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_strloc ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_strext ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_strtok ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_time ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_timeout ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_timestamp ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_trim ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_undef ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_encode( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_decode ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_urlEncode( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_urlDecode ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_function_vt2html ( sInstance*, sCode*, sLOGICAL ) ;
