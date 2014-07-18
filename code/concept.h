/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: concept.h,v $
 * Revision 1.9  2006/01/20 02:55:09  bergsma
 * Fix to reassign port
 *
 * Revision 1.8  2006/01/19 20:37:40  bergsma
 * Added gHyp_concept_getInstForPlaceHolderFd
 *
 * Revision 1.7  2005/10/15 21:42:09  bergsma
 * Added renameto functionality.
 *
 * Revision 1.6  2005/03/16 23:53:21  bergsma
 * V 3.5.1 - fixes for use with DECC compiler.
 *
 * Revision 1.5  2004/04/29 08:30:17  bergsma
 * Device Id is unique and is used interchangeably between PORT, HSMS, SECSI, and HTTP sockets.
 *
 * Revision 1.4  2003/01/12 10:15:39  bergsma
 * Renamed write to writeParent.
 * Added getClients.
 * Added setParent.
 * Added moveto.
 *
 * Revision 1.3  2002/11/20 20:33:38  bergsma
 * Function concept_instantiate has additonal 'root' argument.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
extern sConcept* gHyp_concept_new () ;
extern SOCKET gHyp_concept_serviceHandle ( sConcept *pConcept ) ;
extern SOCKET gHyp_concept_createService( sConcept*, char* ) ;
extern sLOGICAL	gHyp_concept_init ( sConcept*, char*, char*, unsigned, int ) ;
extern sLOGICAL gHyp_concept_setParent ( sConcept *pConcept, char *parent, char *root ) ;
extern sInstance* gHyp_concept_instantiate ( sConcept *, sInstance*,  char*, char*, sLOGICAL, sLOGICAL, sLOGICAL ) ;
extern void    	gHyp_concept_delete ( sConcept * ) ;
extern void    	gHyp_concept_quit ( sConcept *) ;
extern int    gHyp_concept_setAlarms ( sConcept* ) ;
extern void    	gHyp_concept_setReturnToStdIn ( sConcept *, sLOGICAL ) ;
extern sLOGICAL	gHyp_concept_returnToStdIn ( sConcept * ) ;
extern sData *gHyp_concept_getInstances ( sConcept *pConcept ) ;
extern sInstance *gHyp_concept_getNamedInstance ( sConcept *, char *) ;
extern sInstance * gHyp_concept_getConceptInstance ( sConcept* ) ;
extern sInstance * gHyp_concept_nextInstance ( sConcept* ) ;
extern sData * gHyp_concept_nextInstanceData ( sConcept* ) ;
extern char*    gHyp_concept_createToken ( sConcept*, char* ) ;
extern sLOGICAL	gHyp_concept_openWriter ( sConcept * ) ;
extern sLOGICAL	gHyp_concept_openReader ( sConcept * ) ;
extern void    	gHyp_concept_closeWriter ( sConcept * ) ;
extern void    	gHyp_concept_closeReader ( sConcept * ) ;
extern sLOGICAL gHyp_concept_connect ( sConcept *pConcept ) ;
extern sLOGICAL gHyp_concept_moveto ( sConcept*, char * ) ;
extern sLOGICAL gHyp_concept_renameto ( sConcept*, char * ) ;
extern sLOGICAL gHyp_concept_disconnect ( sConcept *pConcept ) ;
extern int gHyp_concept_readSelect ( sConcept *pConcept ) ;
extern int gHyp_concept_route ( sConcept* , char * ) ;
extern sLOGICAL	gHyp_concept_writeParent ( sConcept*, char*, char*, char*, sLOGICAL, sLOGICAL ) ;
extern sInstance * gHyp_concept_getInstForDeviceId ( sConcept*, sWORD ) ;
extern sInstance * gHyp_concept_getInstForFd ( sConcept*, SOCKET ) ;
extern sInstance *gHyp_concept_getInstForPlaceHolderFd ( sConcept *pConcept, SOCKET fd ) ;
extern sBYTE gHyp_concept_getSockObjType ( sConcept *pConcept, SOCKET socket ) ;
extern void *gHyp_concept_getSocketObject ( sConcept *, SOCKET, sBYTE ) ;
extern void gHyp_concept_newSocketObject ( sConcept *pConcept, 
					   SOCKET socket,
					   void *pObject,
					   sBYTE objectType,
					   void  (*pfDelete)(void*) ) ;
extern void gHyp_concept_deleteSocketObject ( sConcept *pConcept, SOCKET s ) ;
extern sChannel *gHyp_concept_findNetwork ( sConcept *pConcept,  char *addr ) ;
extern sData* gHyp_concept_createNetwork ( sConcept *pConcept, char *host, char *addr, SOCKET s ) ;
extern sData* gHyp_concept_getClients ( sConcept *pConcept ) ;
extern sChannel *gHyp_concept_findNetworkByFd ( sConcept *pConcept, SOCKET fd ) ;
extern sSSL* gHyp_concept_getSSL ( sConcept *pConcept ) ;
extern void gHyp_concept_setSSL ( sConcept *pConcept, sSSL *pSSL ) ;
