/*
 * Modifications:
 *
 * $Log: env.h,v $
 * Revision 1.14  2013-05-21 17:46:26  bergsma
 * Add secs_map & secp_unmap.  Deal with 64-bit systems where long and
 * int datatypes are 32 bit.  HS long,ulong,and int are 32 bit.
 *
 * Revision 1.13  2007-09-03 06:24:08  bergsma
 * No newlines at end of file
 *
 * Revision 1.12  2007-07-11 18:49:21  mhohimer
 * added node_* functions:
 * gHyp_env_node_name
 * gHyp_env_node_parent
 * gHyp_env_node_root
 * gHyp_env_node_firstchild
 * gHyp_env_node_lastchild
 * gHyp_env_node_nextsibling
 * gHyp_env_node_prevsibling
 * gHyp_env_node_nextfirstcousin
 * gHyp_env_node_prevlastcousin
 * gHyp_env_node_getnodebyattr
 * gHyp_env_node_getnodebyname -- YET TO BE WRITTEN
 *
 * Revision 1.11  2007-03-15 01:04:07  bergsma
 * Added smart-join-merge "sjm" function, as an option in mergeData.
 *
 * Revision 1.10  2005-07-23 22:34:41  bergsma
 * Added insertbefore and insertafter
 *
 * Revision 1.9  2005/05/10 17:31:38  bergsma
 * Added "env" function
 *
 * Revision 1.8  2005/03/09 04:15:12  bergsma
 * Added appendval and insertval.  Also, disallow append or insert of a variable
 * unto itself.  Needed new gHyp_data_isSilbing function.
 *
 * Revision 1.7  2004/10/16 04:40:45  bergsma
 * Added unmap() function
 *
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
extern void	gHyp_env_sjm ( sInstance*, sCode*, sLOGICAL ) ;
extern sData*	gHyp_env_mergeData ( sData*, sData*, sInstance*, int, sLOGICAL, sLOGICAL, sLOGICAL, sData* ) ;
extern void	gHyp_env_sort ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_reverse ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insert ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insertbefore ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insertafter ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_append ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_remove ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_chop ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_detach ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_quiet ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_verify ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_version ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_env ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_localhost ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_localaddr ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_appendval ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_insertval ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_name ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_parent ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_root ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_firstchild ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_lastchild ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_nextsibling ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_prevsibling ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_nextfirstcousin ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_prevlastcousin ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_getnodebyattr ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_node_getnodebyname ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_secs_map ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_env_secs_unmap ( sInstance*, sCode*, sLOGICAL ) ;