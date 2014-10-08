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
