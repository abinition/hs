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

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/
struct method_t {

  sData*	pArgs ;	  /* Method arguments. */
  int		index ;	  /* If > -1, the method (sHyp) index. */
  sHyp		*pHyp ;		  /* Method's pHyp  code */
  sLOGICAL	isEnabled ; /* If method is enabled to receive events */

} ;

sMethod *gHyp_method_new () 
{
  sMethod 
    *pMethod ;

  pMethod = (sMethod*) AllocMemory ( sizeof ( sMethod ) ) ;
  assert ( pMethod ) ;

  pMethod->pHyp = NULL ;
  pMethod->pArgs = NULL ;
  pMethod->index = -1 ;
  pMethod->isEnabled = FALSE ;

  return pMethod ;
}

void gHyp_method_delete ( sMethod *pMethod ) 
{
  if ( pMethod->pArgs ) gHyp_data_delete ( pMethod->pArgs ) ;
  if ( pMethod->pHyp ) gHyp_hyp_delete ( pMethod->pHyp ) ;
  ReleaseMemory ( pMethod ) ;
}

sHyp *gHyp_method_getHyp ( sMethod *pMethod ) 
{
  return pMethod->pHyp ;
}

void gHyp_method_setHyp ( sMethod *pMethod, 
			  sHyp *pHyp )
{
  pMethod->pHyp = pHyp ;
  return ;
}

int gHyp_method_getIndex ( sMethod *pMethod ) 
{
  return pMethod->index ;
}

void gHyp_method_setIndex ( sMethod *pMethod, int hypIndex )
{
  pMethod->index = hypIndex ;
  return ;
}

sData* gHyp_method_getArgs ( sMethod *pMethod ) 
{
  return pMethod->pArgs ;
}

void gHyp_method_setArgs ( sMethod *pMethod, sData *pArgs )
{
  pMethod->pArgs = pArgs ;
  return ;
}

void gHyp_method_enable ( sMethod *pMethod )
{
  pMethod->isEnabled = TRUE ;
  return ;
}

void gHyp_method_disable ( sMethod *pMethod )
{
  pMethod->isEnabled = FALSE ;
  return ;
}

sLOGICAL gHyp_method_isEnabled ( sMethod *pMethod )
{
  return pMethod->isEnabled ;
}
