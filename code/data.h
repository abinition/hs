/**********************	FUNCTION PROTOTYPES ********************************/

/*
 * Modifications:
 *
 * $Log: data.h,v $
 * Revision 1.5  2007-07-09 05:39:00  bergsma
 * TLOGV3
 *
 * Revision 1.11  2006-09-16 19:40:46  bergsma
 * Added gHyp_data_newVectorSrc
 *
 * Revision 1.10  2005/09/02 04:35:54  bergsma
 * no message
 *
 * Revision 1.9  2005/07/23 22:31:14  bergsma
 * Added insertafter and Insertbefore
 *
 * Revision 1.8  2005/03/09 04:15:12  bergsma
 * Added appendval and insertval.  Also, disallow append or insert of a variable
 * unto itself.  Needed new gHyp_data_isSilbing function.
 *
 * Revision 1.7  2004/10/16 00:51:29  bergsma
 * New function names replace cryptic data functions:
 * gHyp_data_setStr<n>
 * gHyp_data_getStr<n>
 * gHyp_data_newConstant<n>
 *
 * Revision 1.6  2003/04/04 16:42:19  bergsma
 * Added moveValues function, to assist when detaching variables from
 * the method being executed.
 *
 * Revision 1.5  2003/01/12 10:11:46  bergsma
 * Add gHyp_data_getVariable, taken from gHyp_frame_getVariable
 *
 * Revision 1.4  2002/11/14 01:43:57  bergsma
 * Ditto.
 *
 * Revision 1.3  2002/11/14 01:29:28  bergsma
 * Removed unused arguments in gHyp_data_exchange
 *
 * Revision 1.2  2002/09/21 22:11:46  bergsma
 * Added function gHyp_data_newConstant3
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
extern void	       gHyp_data_copyObject ( sData *pDst, sData *pSrc ) ;
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
extern sBYTE		gHyp_data_dataType ( sData* ) ;
extern sBYTE		gHyp_data_getDataType ( sData* ) ;
extern sBYTE		gHyp_data_tokenType ( sData* ) ;
extern sBYTE		gHyp_data_getTokenType ( sData* ) ;
extern sData* 		gHyp_data_getAll ( sData*, sBYTE*, sBYTE*,
					   int*, unsigned long*, double*, sLOGICAL*,
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
