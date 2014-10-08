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
extern sHyp*	gHyp_hyp_new ( char * ) ;
extern void 	gHyp_hyp_delete ( sHyp * ) ;
extern void    	gHyp_hyp_traceReport ( int, char* ) ;
extern void    	gHyp_hyp_traceReset ( char * ) ;
extern void gHyp_hyp_describe ( sHyp *pHyp ) ;
extern char* gHyp_hyp_describe2 ( sHyp *pHyp, int hypIndex, char *pBuf, int buflen ) ;
extern void    	gHyp_hyp_setHypCount ( sHyp *, int ) ;
extern int     	gHyp_hyp_getHypCount ( sHyp * ) ;
extern int gHyp_hyp_getHighWaterCount ( sHyp *pHyp ) ;
extern char*    gHyp_hyp_method ( sHyp * ) ;
extern sBYTE   	gHyp_hyp_rank ( sCode * ) ; 
extern void   	gHyp_hyp_setRank ( sCode *, sBYTE rank ) ; 
extern void	gHyp_hyp_setPrecedence ( sCode *, sBYTE precedence ) ;
extern sBYTE   	gHyp_hyp_precedence ( sCode * ) ;
extern void   	gHyp_hyp_setTokenType ( sCode *, sBYTE tokenType ) ;
extern sBYTE   	gHyp_hyp_tokenType ( sCode * ) ;
extern int     	gHyp_hyp_index ( sCode * ) ;
extern int     	gHyp_hyp_line ( sCode * ) ;
extern char*   	gHyp_hyp_token ( sCode * ) ;
extern int	gHyp_hyp_parse ( sInstance*, sCode*, sHyp*, sFrame* );
extern int    	gHyp_hyp_execute ( sInstance*, sCode*);
extern sCode*  	gHyp_hyp_code ( sHyp *, int ) ;
extern void    	gHyp_hyp_error ( sHyp*, int, char*, ... ) ;
extern sLOGICAL gHyp_hyp_okUnaryOperator ( sHyp * ) ;
extern int	 gHyp_hyp_source ( sInstance*,sHyp *, FILE*, char*, sLOGICAL ) ;
extern sLOGICAL gHyp_hyp_load (	sInstance*, sConcept*, sHyp*, char*, sBYTE,
				sBYTE, int, 
				void (*pf)(sInstance*,sCode*,sLOGICAL) ) ;
extern void gHyp_hyp_transfer ( sInstance*, sConcept*, sHyp*, sHyp*, int, int, int, sData* ) ;
