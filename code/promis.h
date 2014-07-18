/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: promis.h,v $
 * Revision 1.6  2004/07/28 00:49:18  bergsma
 * Version 3.3.0
 *
 * Revision 1.6  2004/07/27 15:44:01  bergsma
 * Undo fix where nulls were stripped from PROMIS database fields
 *
 * Revision 1.5  2004/07/23 18:45:16  bergsma
 * - Fixed traceback when accessing 'data' from 'tres' in PROMIS, pTEST_DII not defined.
 * - Added pcapture().
 *
 * Revision 1.4  2004/05/15 02:09:08  bergsma
 * Added callback from tut_output when doing pexec()
 *
 * Revision 1.3  2003/04/04 16:17:34  bergsma
 * Added plogopen and plogclose
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern sData*	gHyp_promis_getTRfield ( sInstance*, sFrame*, char*, sData* ) ;
extern sData*	gHyp_promis_getDBfield ( sInstance*, sFrame*, char*, sData* ) ;
extern sData*   gHyp_promis_getSymbol (	sInstance*, sFrame*, char*, sData* ) ;
extern sData*   gHyp_promis_getParm (	sInstance*, sFrame*, char*, sData* ) ;
extern sData*   gHyp_promis_parseRecord ( sInstance*,sFrame*,char*,sData*);
extern void	gHyp_promis_pexec ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_promis_popen ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_promis_pclose ( sInstance*, sCode*, sLOGICAL ) ;
extern void     gHyp_promis_pget ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_promis_pnext ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_promis_psnap ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_promis_plogin ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_promis_plogopen ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_promis_plogclose ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_promis_pcapture ( sInstance*, sCode*, sLOGICAL ) ; 
extern sData*   gHyp_promis_getPROMIS ( sInstance*, sFrame*, char*, sData* ) ;
extern void     gHyp_promis_initFields () ;
extern void     gHyp_promis_cleanFields ( long ) ;
