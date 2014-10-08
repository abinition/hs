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

extern sData* 		gHyp_data_new ( char * ) ;
extern void 		gHyp_data_newVector ( sData*, sBYTE, int, sLOGICAL ) ;
extern void 		gHyp_data_newVectorSrc ( sData*, sBYTE, int, sLOGICAL, void* ) ;
extern void 		gHyp_data_newConstant ( sData*, sBYTE, sData*, int ) ;
extern void 		gHyp_data_newConstant_raw ( sData*, sBYTE, void* ) ;
extern void 		gHyp_data_newConstant_scanf ( sData*, sBYTE, char*, int ) ;
extern void*		gHyp_data_getObject ( sData *pData ) ;
extern sBYTE		gHyp_data_getObjectType ( sData *pData ) ;
extern void		gHyp_data_setObject ( sData *, void *, sBYTE, void (*pf)(void*) ) ;
extern void	        gHyp_data_copyObject ( sData *pDst, sData *pSrc ) ;
extern void		gHyp_data_exchange ( sData*, sData* ) ;
extern void 		gHyp_data_append ( sData*, sData* ) ;
extern void 		gHyp_data_insert ( sData*, sData* ) ;
extern void		gHyp_data_insertbefore ( sData *pSibling, sData *pData ) ;
extern void		gHyp_data_insertafter ( sData *pSibling, sData *pData ) ;
extern sLOGICAL        	gHyp_data_next ( sData* ) ;
extern sLOGICAL		gHyp_data_prev ( sData* ) ;
extern sData*		gHyp_data_copy ( sData* ) ;
extern sData*		gHyp_data_copyAll ( sData* ) ;
extern void		gHyp_data_moveValues ( sData*, sData* ) ;
extern void		gHyp_data_moveValuesR ( sData*, sData* ) ;
extern void		gHyp_data_copyValues ( sData*, sData* ) ;
extern void 		gHyp_data_delete ( sData* ) ;
extern void 		gHyp_data_deleteValues ( sData* ) ;
extern sData* 		gHyp_data_getParent ( sData* ) ;
extern sLOGICAL		gHyp_data_isSibling ( sData *pData, sData *pChild ) ; 
extern sData* 		gHyp_data_getFirst ( sData* ) ;
extern sData* 		gHyp_data_getLast ( sData* ) ;
extern sData* 		gHyp_data_getNext ( sData* ) ;
extern sData* 		gHyp_data_getPrev ( sData* ) ;
extern int 		gHyp_data_getInt ( sData*, int, sLOGICAL ) ;
extern unsigned long  	gHyp_data_getRaw ( sData*, int, sLOGICAL ) ;
extern double 		gHyp_data_getDouble ( sData*, int, sLOGICAL ) ;
extern sLOGICAL 	gHyp_data_getBool ( sData*, int, sLOGICAL ) ;
extern void*		gHyp_data_getHandle ( sData*, int, sLOGICAL ) ;
extern char* 		gHyp_data_getLabel ( sData* ) ;
extern sBYTE*		gHyp_data_buffer ( sData*, int ) ;
extern int		gHyp_data_bufferLen ( sData*, int ) ;
extern int 		gHyp_data_getStr ( sData*, char*, int, int, sLOGICAL) ;
extern int 		gHyp_data_getStr_external ( sData*, char*, int, int, sLOGICAL );
extern int 		gHyp_data_getStr_nonulls ( sData*, char*, int, int, sLOGICAL);
extern int 		gHyp_data_getStr_msg ( sData*, char*, int, int, sLOGICAL);
extern int 		gHyp_data_getStr_xml ( sData*, char*, int, int, sLOGICAL);
extern sBYTE		gHyp_data_dataLen ( sData* ) ;
extern short *gHyp_data_dataLenPtr( sData *pData ) ;
extern sBYTE		gHyp_data_dataType ( sData* ) ;
extern sBYTE		gHyp_data_getDataType ( sData* ) ;
extern sBYTE		gHyp_data_tokenType ( sData* ) ;
extern sBYTE		gHyp_data_getTokenType ( sData* ) ;
extern sData* 		gHyp_data_getAll ( sData*, sBYTE*, sBYTE*,
					   int*, unsigned long*, double*, sLOGICAL*, void**,
					   char*, int*, int, int, sLOGICAL ) ;
extern int 		gHyp_data_getCount ( sData * ) ;
extern int 		gHyp_data_check ( sData * ) ;
extern sLOGICAL		gHyp_data_isCountOne ( sData * ) ;
extern int 		gHyp_data_getSubScript ( sData * ) ;
extern sLOGICAL		gHyp_data_isDynamic ( sData *pData ) ;
extern void		gHyp_data_setDynamic ( sData *pData, sLOGICAL ) ;
extern void		gHyp_data_setHypLabel ( sData*, int, int ) ;
extern sData* 		gHyp_data_getChildBySS ( sData *, int ) ;
extern sData* 		gHyp_data_getChildByName ( sData *, char * ) ;
extern sData*		gHyp_data_getReference ( sData * ) ;
extern sData*		gHyp_data_getVariable ( sData *pData ) ;
extern sLOGICAL 	gHyp_data_deleteChildByName ( sData *, char * ) ;
extern sData*		gHyp_data_detach ( sData * ) ;
extern sData*		gHyp_data_extractChildByName ( sData *, char* ) ;
extern sData*		gHyp_data_extractChildBySS ( sData *, int ) ;
extern sData*		gHyp_data_getValue ( sData *, int, sLOGICAL ) ;
extern sData*           gHyp_data_nextValue ( sData*, sData*, int*, int ) ;
extern void 		gHyp_data_setNull ( sData* ) ;
extern void 		gHyp_data_setSubScript ( sData *, int ) ;
extern void		gHyp_data_setSize ( sData *, int ) ; 
extern void 		gHyp_data_setToken ( sData*, char* ) ;
extern void 		gHyp_data_setLabel ( sData*, char* ) ;
extern void 		gHyp_data_setStr ( sData*, char* ) ;
extern void 		gHyp_data_setStr_n ( sData*, char*, int ) ;
extern void 		gHyp_data_setVariable ( sData*, char*, sBYTE ) ;
extern void 		gHyp_data_setReference ( sData*, char*, sData* ) ;
extern void 		gHyp_data_setUnidentified ( sData*, char* ) ;
extern void 		gHyp_data_setVector ( sData*, sData*, 
					      int, int, sLOGICAL ) ;
extern void		gHyp_data_setVectorRaw ( sData *, void *, int  ) ;
extern void		gHyp_data_increment ( sData *, int, sLOGICAL ) ;
extern void		gHyp_data_decrement ( sData *, int, sLOGICAL ) ;
extern void 		gHyp_data_setInt ( sData*, int ) ;
extern void 		gHyp_data_setDouble ( sData*, double ) ;
extern void 		gHyp_data_setBool ( sData*, sLOGICAL ) ;
extern void 		gHyp_data_setHandle ( sData*, void* ) ;
extern void 		gHyp_data_setHashed ( sData*, sLOGICAL ) ;
extern char* 		gHyp_data_print ( sData* ) ;
extern void 		gHyp_data_setText ( sData*, char*, int ) ;
extern int          gHyp_data_getTagIndex ( sData* ) ;
extern sData*       gHyp_data_getChildNodeByIndex( sData*, int ) ;
extern sData*       gHyp_data_getFirstNode( sData* ) ;
extern sData*       gHyp_data_getLastNode( sData* ) ;
extern sData*       gHyp_data_getNextNode( sData* ) ;
extern sData*       gHyp_data_getPrevNode( sData* ) ;
extern sData*       gHyp_data_getNodeByAttr( sData*, char*, char*, char* ) ;

