/*
 * Modifications:
 *
 * $Log: route.h,v $
 * Revision 1.6  2007-03-15 01:09:21  bergsma
 * Added "reply" function.
 *
 * Revision 1.5  2005-10-15 21:42:09  bergsma
 * Added renameto functionality.
 *
 * Revision 1.4  2004/04/29 08:24:33  bergsma
 * New functions service, open, tunnel, forward, unforward
 *
 * Revision 1.3  2003/01/09 08:06:32  bergsma
 * V3.1.0
 * Added _instances, _children, _instance, _concept, _parent, _moveto
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern void 	gHyp_route_enable ( sInstance*, sCode*, sLOGICAL ) ;
extern void 	gHyp_route_disable ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_reply ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_event ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_query ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_requeue ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_instances ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_children ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_instance ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_concept ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_parent ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_moveto ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_route_renameto ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_route_service ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_route_open ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_route_tunnel( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_route_forward ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
extern void	gHyp_route_unforward ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;
