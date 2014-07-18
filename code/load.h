/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: load.h,v $
 * Revision 1.5  2007-07-09 05:39:00  bergsma
 * TLOGV3
 *
 * Revision 1.4  2005-03-16 23:54:37  bergsma
 * V 3.5.1 - fixes for use with DECC compiler.
 *
 * Revision 1.3  2004/06/13 14:05:41  bergsma
 * Functions localhost, localaddr, jeval
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 *
 */
extern void   (*gHyp_load_fun(char*,char*,char*))(sInstance*,sCode*,sLOGICAL);
extern void gHyp_load_new() ;
extern void gHyp_load_delete() ;
extern sLOGICAL gHyp_load_isKey( char *) ;
extern char*  gHyp_load_fromStream ( sInstance*, sHyp*, char *, int ) ;
extern void gHyp_hs_jeval ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) ;

