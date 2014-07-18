/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/
/*
 * Modifications:
 *
 *   $Log: hash.c,v $
 *   Revision 1.2  2002/11/14 01:40:45  bergsma
 *   Removed ^M occurances and added Modification section.
 *
 */

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ***********************/
#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/* Hash structure for devices that read and write */

struct hash_t
{
  char*		name ;
  void*		object ;

  /* Linkages */
  sHash*	pNext ;
  sHash*	pPrev ;
} ;

void gHyp_hash_free ( sHash *pHash ) 
{
  ReleaseMemory ( pHash->name ) ;
  ReleaseMemory ( pHash ) ;
}
 
static void *lHyp_hash_hash (	sHash	*(*hashes)[MAX_HASH_TABLE_SIZE],
				int	hashSize,
				char	*name,
				sBYTE	action,
				void	*(*pfNew)(char*),
				void	*(*pfDelete)(void*),
				void	*pUpdate ) 
      
{
  /* Description:
   *
   *	Maintain a hash table for sHash objects.
   *
   * Arguments:
   *
   *	hashes						[R/W]
   *	- pointer to an array of pointers to sHash objects
   *
   *    hashSize
   *    - size of hash table				[R]
   *
   *	name						[R]
   *	- target "key" to hash in table
   *
   *	action						[R]
   *	- HASH_FIND, HASH_CREATE, HASH_DELETE, HASH_UPDATE
   *
   *    pfNew
   *    - constructor function if HASH_CREATE
   *	- if NULL, then only name is malloc'd and stored.
   *
   *    pfDelete
   *	- destructor function if HASH_DELETE
   *	- if NULL, then only name is free'd.
   *
   *	pUpdate
   *	- object to add if HASH_UPDATE
   *
   * Return value:
   *
   *	- pointer to sHash container object found, created, updated.
   *	- or NULL if not found or if deleted
   *
   *
   */

  int		
    ii = 0 ;
  
  char
    *pp = name ;

  sHash
    *pThisHash,
    *pHash ;

  void
    *pObject = NULL ;

  /* Calculate HASH position (ii) */
  while ( *pp ) ii = ii<<1 ^ *pp++ ;
  if ( ii < 0 ) ii = -ii ;
  ii %= hashSize ;

  /* Search for specified hash name. If not found, pHash will be NULL */
  for ( pThisHash = (*hashes)[ii], pHash = NULL ;
	pThisHash ; 
	pThisHash = pThisHash->pNext )
    if ( strcmp ( name, pThisHash->name ) == 0 &&
	 ( pUpdate == NULL || pUpdate == pThisHash->object ) ) {
      pHash = pThisHash ;
      pObject = pHash->object ;
      break ;
    }

  switch ( action ) {
  
  case HASH_FIND:
    break ;
    
  case HASH_CREATE:
  case HASH_UPDATE:
    
    /* If entry already exists, don't re-create */
    if ( pHash ) break ;
    
    /* Create a new hash entry */
    pHash = (sHash*) AllocMemory ( sizeof ( sHash ) ) ;
    assert ( pHash ) ;
    pHash->name = (char*) AllocMemory ( strlen ( name ) + 1 ) ;
    assert ( pHash->name ) ;
    strcpy ( pHash->name, name ) ;
    
    /* Initialize the new hash entry */
    if ( pfNew ) 
      pObject = pHash->object = pfNew ( name ) ;
    else if ( pUpdate ) {
      pObject = pHash->object = pUpdate ;
    }
    else {
      pObject = pHash->object = pHash->name ;
    }
    
    /* New sHash is inserted at the head of the linked list */
    if ( ( pHash->pNext = (*hashes)[ii]) ) pHash->pNext->pPrev = pHash ;
    
    /* Head of list has no previous sHash */
    pHash->pPrev = NULL ;

    /* Hash position ii stores the pointer to the head of the list */
    (*hashes)[ii] = pHash ;
    
    break ;
    
  case HASH_DELETE:
    
    /* If entry does not exist, don't delete */
    if ( !pHash ) break ;
    
    if ( pHash->pPrev ) 
      /* Previous sHash member exists, join with next sHash member */
      pHash->pPrev->pNext = pHash->pNext ;
    else
      /* Previous sHash does not exist, join next member to head of list */
      (*hashes)[ii] = pHash->pNext ;
    
    /* If next sHash member exists, join with previous sHash member */
    if ( pHash->pNext ) pHash->pNext->pPrev = pHash->pPrev ;
    
    /* Now its safe to delete it */
    if ( pfDelete )
      pfDelete ( pHash->object ) ;
    else
      pObject = pHash->object ;
    
    gHyp_hash_free ( pHash ) ;
    
    break ;
  }
  return pObject ;
}

sHash *(*gHyp_hash_new( int hashTableSize ))[MAX_HASH_TABLE_SIZE]
{
  sHash 
    *(*pHash)[MAX_HASH_TABLE_SIZE] ;

  int
    i ;

  pHash = (sHash*(*)[MAX_HASH_TABLE_SIZE]) 
  AllocMemory ( sizeof ( sHash ) * hashTableSize ) ;
  assert ( pHash ) ;

  for ( i=0; i<hashTableSize; i++ ) (*pHash)[i] = NULL ;

  return pHash ;
}

void gHyp_hash_delete ( sHash *(*pHash)[MAX_HASH_TABLE_SIZE] )
{
  ReleaseMemory ( pHash ) ;
  return ;
}

void *gHyp_hash_create (	sHash	*(*hashes)[MAX_HASH_TABLE_SIZE],
				int	hashSize,
				char	*name,
				void	*(*pfNew)(char*) ) 
      
{
  /* Description:
   *
   *	Create a new hash table entry.
   *
   * Arguments:
   *
   *	hashes						[R/W]
   *	- pointer to an array of pointers to sHash objects
   *
   *    hashSize
   *    - size of hash table				[R]
   *
   *	name						[R]
   *	- target "key" to hash in table
   *
   *    pfNew
   *    - constructor function
   *
   * Return value:
   *
   *	pointer to sHash containter object created
   *
   *
   */

  return lHyp_hash_hash ( hashes,
			  hashSize,
			  name,
			  HASH_CREATE,
			  pfNew,
			  NULL, NULL ) ;
}

void *gHyp_hash_update (	sHash	*(*hashes)[MAX_HASH_TABLE_SIZE],
				int	hashSize,
				char	*name,
				void	*pUpdate )  
      
{
  /* Description:
   *
   *	Update a hash table entry.
   *
   * Arguments:
   *
   *	hashes						[R/W]
   *	- pointer to an array of pointers to sHash objects
   *
   *    hashSize
   *    - size of hash table				[R]
   *
   *	name						[R]
   *	- target "key" to hash in table
   *
   *    pUpdate
   *    - object to add/update in hash table
   *
   * Return value:
   *
   *	pointer to sHash container object created
   *
   */

  return lHyp_hash_hash ( hashes,
			  hashSize,
			  name,
			  HASH_UPDATE,
			  NULL, NULL,
			  pUpdate ) ;
}

void* gHyp_hash_remove (  sHash	*(*hashes)[MAX_HASH_TABLE_SIZE],
			  int	hashSize,
			  char	*name,
			  void	*(*pfDelete)(void*) ) 
      
{
  /* Description:
   *
   *	Delete a hash table entry.
   *
   * Arguments:
   *
   *	hashes						[R/W]
   *	- pointer to an array of pointers to sHash objects
   *
   *    hashSize
   *    - size of hash table				[R]
   *
   *	name						[R]
   *	- target "key" to hash in table
   *
   *    pfDelete
   *	- destructor function if HASH_DELETE
   *
   * Return value:
   *
   *	Pointer to object removed.
   *	If the object was deleted, then the pointer will be null.
   *
   *
   */
   return lHyp_hash_hash ( hashes,
			   hashSize,
			   name,
			   HASH_DELETE,
			   NULL,
			   pfDelete,
			   NULL ) ;
}     

void *gHyp_hash_find (	sHash	*(*hashes)[MAX_HASH_TABLE_SIZE],
			int	hashSize,
			char	*name )
      
{
  /* Description:
   *
   *	Find a hash table entry for name.
   *
   * Arguments:
   *
   *	hashes						[R/W]
   *	- pointer to an array of pointers to sHash objects
   *
   *    hashSize
   *    - size of hash table				[R]
   *
   *	name						[R]
   *	- target "key" to hash in table
   *
   * Return value:
   *
   *	pointer to sHash containted object found.
   *
   *
   */

  return lHyp_hash_hash ( hashes,
			  hashSize,
			  name,
			  HASH_FIND,
			  NULL, NULL, NULL ) ;
}

sHash* gHyp_hash_next ( sHash *pHash ) 
{
  return pHash->pNext ;
}

void *gHyp_hash_object ( sHash *pHash )
{
  return pHash->object ;
}
