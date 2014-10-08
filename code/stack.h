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
extern sStack*		gHyp_stack_new () ;
extern void 		gHyp_stack_flush ( sStack * ) ;
extern void 		gHyp_stack_delete ( sStack * ) ;
extern int 		gHyp_stack_depth ( sStack * ) ;
extern void	 	gHyp_stack_push ( sStack *, sData * ) ;
extern sData* 		gHyp_stack_pop ( sStack * ) ;
extern sData* 		gHyp_stack_pop2 ( sStack * ) ;
extern sData*		gHyp_stack_peek ( sStack * ) ;
extern sData* 		gHyp_stack_popRvalue ( sStack *, sInstance * ) ;
extern sData* 		gHyp_stack_popRdata ( sStack *, sInstance * ) ;
extern sData* 		gHyp_stack_popRdata2 ( sStack *, sInstance * ) ;
extern sData*		gHyp_stack_popRdata_nocheck ( sStack * pStack, sInstance * pAI ) ;
extern sData* 		gHyp_stack_popRdata2_nocheck ( sStack *, sInstance * ) ;
extern sData* 		gHyp_stack_popLvalue ( sStack *, sInstance *) ;
