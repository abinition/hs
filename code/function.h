/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: function.h,v $
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
