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
