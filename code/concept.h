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
extern sConcept* gHyp_concept_new () ;
extern SOCKET gHyp_concept_serviceHandle ( sConcept *pConcept ) ;
extern SOCKET gHyp_concept_createService( sConcept*, char* ) ;
extern sLOGICAL	gHyp_concept_init ( sConcept*, char*, char*, char*, unsigned, int ) ;
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
extern void gHyp_concept_setPortStopGo ( sConcept *pConcept, SOCKET fd, sLOGICAL go ) ;
extern sLOGICAL gHyp_concept_getPortStopGo ( sConcept *pConcept, SOCKET fd ) ;

