/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!****************************************************************************/
/*
 * Modifications:
 *
 *   $Log: label.c,v $
 *   Revision 1.5  2007-07-09 05:39:00  bergsma
 *   TLOGV3
 *
 *   Revision 1.2  2002-11-14 01:40:45  bergsma
 *   Removed ^M occurances and added Modification section.
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
