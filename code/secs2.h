/*
 * Modifications:
 *
 * $Log: secs2.h,v $
 * Revision 1.4  2007-02-24 01:54:08  bergsma
 * Added secs_*_raw functions.
 *
 * Revision 1.3  2004-04-29 02:21:18  bergsma
 * Device id is sWORD, not int.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern sSecs2 *		gHyp_secs2_new () ;
extern void 		gHyp_secs2_delete ( sSecs2* ) ;
extern sSecsHeader*	gHyp_secs2_newSecsHeader ( sSecs2*, sBYTE  ) ;
extern sSecsHeader*	gHyp_secs2_getSecsHeader ( sSecs2*, sBYTE ) ;
extern void		gHyp_secs2_resetBuf ( sSecs2*, sBYTE ) ;
extern void		gHyp_secs2_add2buf(sSecs2*,sBYTE*,int,sBYTE) ;
extern sLOGICAL		gHyp_secs2_isEndBlock ( sSecs2 *pSecs2, int,sBYTE d ) ;
extern int		gHyp_secs2_getBlock(sSecs2*,sBYTE*,int,sBYTE) ;
extern int		gHyp_secs2_getAllBlocks(sSecs2*,sBYTE*,sBYTE) ;
extern int		gHyp_secs2_parseSecs(sSecs2*,sSecsHeader*,
					     sInstance*,sBYTE,sBYTE,
					     int,int,int,int);
extern void		gHyp_secs2_unParseSecs( sSecs2*,sData*,
						sInstance*,int,int) ;
extern void		gHyp_secs2_unParseSecsRaw( sSecs2*,sData*) ;
extern int		gHyp_secs2_stream ( sSecsHeader* ) ;
extern int		gHyp_secs2_function ( sSecsHeader* ) ;
extern sWORD		gHyp_secs2_deviceId ( sSecsHeader* ) ;
extern int		gHyp_secs2_TID ( sSecsHeader * ) ;
extern int		gHyp_secs2_SID ( sSecsHeader * ) ;
extern sLOGICAL		gHyp_secs2_isRbitSet ( sSecsHeader * ) ; 
extern sLOGICAL		gHyp_secs2_isReplyExpected ( sSecsHeader * ) ; 
extern sLOGICAL		gHyp_secs2_isFirstBlock ( sSecsHeader * ) ;
extern sLOGICAL		gHyp_secs2_isLastBlock ( sSecsHeader * ) ;
extern sLOGICAL		gHyp_secs2_isPrimaryMsg ( sSecsHeader * ) ;
