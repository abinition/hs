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
/* Neds to be label_tt rather than label_t for VMS*/
struct label_tt {

  /* Label */
  int		index ;	  /* If > -1, the label (sHyp) index. */
  int		depth ;	  /* If > -1, the label frame depth. */
} ;

sLabel *gHyp_label_new ()
{
  sLabel
    *pLabel ;

  pLabel = (sLabel*) AllocMemory ( sizeof ( sLabel ) ) ;
  assert ( pLabel ) ;

  pLabel->index = -1 ;
  pLabel->depth = -1 ;

  return pLabel ;
}

void gHyp_label_delete ( sLabel *pLabel )
{
  ReleaseMemory ( pLabel ) ;
}

int gHyp_label_getIndex ( sLabel *pLabel ) 
{
  return pLabel->index ;
}

int gHyp_label_getDepth ( sLabel *pLabel ) 
{
  return pLabel->depth ;
}

void gHyp_label_setLabel ( sLabel *pLabel, int labelIndex, int labelDepth )
{
  pLabel->index = labelIndex ;
  pLabel->depth = labelDepth ;
  return ;
}
