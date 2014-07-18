/*
 * Modifications:
 *
 * $Log: env.h,v $
 * Revision 1.6  2004/07/28 00:49:18  bergsma
 * Version 3.3.0
 *
 * Revision 1.6  2004/07/27 15:43:26  bergsma
 * In mergeData, if nulls exist in the hyperscript being loaded, then
 * use getStr3 to take them out first
 *
 * Revision 1.5  2004/06/13 13:56:26  bergsma
 * Added localhost() function.
 *
 * Revision 1.4  2003/04/04 16:43:19  bergsma
 * MergeData now also handles argument merging from messages.
 *
 * Revision 1.3  2003/01/12 10:12:40  bergsma
 * Replace instance with instantiation.
 * Add autoIncrement flag to mergeData
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void	gHyp_env_count ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_debug ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_exists ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_exit ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_quit ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_idle ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_instantiate ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_instantiation ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_env_undef ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_hashed ( sInstance*, sCode*, sLOGICAL ) ;
extern void     gHyp_env_unhashed ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_local ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_global ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_next ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_prev ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_map ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_unmap ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_merge ( sInstance*, sCode*, sLOGICAL ) ;
extern sData*	gHyp_env_mergeData ( sData*, sData*, sInstance*, int, sLOGICAL, sLOGICAL, sData* ) ;
extern void	gHyp_env_sort ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_reverse ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insert ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_append ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_remove ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_chop ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_detach ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_quiet ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_verify ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_version ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_localhost ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_localaddr ( sInstance*, sCode*, sLOGICAL ) ;
