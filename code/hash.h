/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: hash.h,v $
 * Revision 1.3  2002/11/14 01:43:57  bergsma
 * Ditto.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */

extern void		gHyp_hash_free ( sHash * ) ;
extern sHash		*(*gHyp_hash_new( int ))[MAX_HASH_TABLE_SIZE] ;
extern void		gHyp_hash_delete ( sHash *(*)[MAX_HASH_TABLE_SIZE] ) ;
extern void*		gHyp_hash_create ( sHash *(*)[MAX_HASH_TABLE_SIZE],
					   int, 
					   char*,
					   void	*(*)(char*) ) ;
extern void*		gHyp_hash_update ( sHash *(*)[MAX_HASH_TABLE_SIZE],
					   int, 
					   char*,
					   void	* ) ;
extern void*		gHyp_hash_remove ( sHash *(*)[MAX_HASH_TABLE_SIZE], 
					   int, 
					   char*,
					   void	*(*)(void*) ) ;
extern void*		gHyp_hash_find ( sHash *(*)[MAX_HASH_TABLE_SIZE], 
					 int, 
					 char* ) ;
extern sHash*		gHyp_hash_next ( sHash* ) ; 
extern void*		gHyp_hash_object ( sHash * ) ;
