/* FUNCTION PROTOTYPES */

/*
 * Modifications:
 *
 * $Log: aimsg.h,v $
 * Revision 1.5  2003-01-09 00:06:58  bergsma
 * V3.1.0
 * 1. Added gHyp_aimsg_setTV
 *
 * Revision 1.4  2002/11/21 01:21:02  bergsma
 * Remove ^M
 *
 * Revision 1.3  2002/11/20 20:24:57  bergsma
 * Added functions targetRoot and senderRoot
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */

extern sAImsg * 	gHyp_aimsg_new ( ) ;
extern void		gHyp_aimsg_delete ( sAImsg* ) ;
extern sData*		gHyp_aimsg_tv ( sAImsg* ) ;
extern void		gHyp_aimsg_setTV ( sAImsg *pAImsg, sData *pTV ) ;
extern char*		gHyp_aimsg_targetInstance ( sAImsg* ) ;
extern char*		gHyp_aimsg_targetConcept ( sAImsg* ) ;
extern char*		gHyp_aimsg_targetParent ( sAImsg* ) ;
extern char*		gHyp_aimsg_targetRoot ( sAImsg* ) ;
extern char*		gHyp_aimsg_targetHost ( sAImsg* ) ;
extern char*		gHyp_aimsg_senderInstance ( sAImsg* ) ;
extern char*		gHyp_aimsg_senderConcept ( sAImsg* ) ;
extern char*		gHyp_aimsg_senderParent ( sAImsg* ) ;
extern char*		gHyp_aimsg_senderRoot ( sAImsg* ) ;
extern char*		gHyp_aimsg_senderHost ( sAImsg* ) ;
extern void 		gHyp_aimsg_getTargetId ( sAImsg*, char* ) ;
extern void 		gHyp_aimsg_getSenderId ( sAImsg*, char* ) ;
extern void 		gHyp_aimsg_getTargetPath ( sAImsg*, char* ) ;
extern void 		gHyp_aimsg_getSenderPath ( sAImsg*, char* ) ;
extern sLOGICAL		gHyp_aimsg_setTargetId ( sAImsg*, char* ) ;
extern void 		gHyp_aimsg_getTargetAddress ( sAImsg*, char* ) ;
extern void 		gHyp_aimsg_getSenderAddress ( sAImsg*, char* ) ;
extern char*		gHyp_aimsg_mode ( sAImsg* ) ;
extern char*		gHyp_aimsg_method ( sAImsg* ) ;
extern char*		gHyp_aimsg_getTID ( sAImsg* ) ;
extern char*		gHyp_aimsg_getTS ( sAImsg* ) ;
extern void		gHyp_aimsg_setTID ( sAImsg*, char* ) ;
extern void		gHyp_aimsg_setTS ( sAImsg*, char* ) ;
extern sLOGICAL 	gHyp_aimsg_isQuery ( sAImsg * ) ;
extern sLOGICAL 	gHyp_aimsg_isEvent ( sAImsg * ) ;
extern sLOGICAL 	gHyp_aimsg_isReply ( sAImsg * ) ;
extern char*		gHyp_aimsg_unparse ( sAImsg* ) ;
extern sLOGICAL 	gHyp_aimsg_parse ( sAImsg*, char* ) ;
extern sLOGICAL 	gHyp_aimsg_initParse ( sAImsg*, char* ) ;
extern sLOGICAL 	gHyp_aimsg_initParsePartial ( sAImsg*, char* ) ;
extern char*		gHyp_aimsg_initUnparse ( sAImsg	*, 
						 char *, char*, char*, char*,
						 char*, char*, sData* ) ;
extern char*		gHyp_aimsg_initUnparsePartial ( sAImsg	*, 
						 char *, char*, char*, char*,
						 char*, char*, sData* ) ;
extern char*		gHyp_aimsg_tokensValues ( sAImsg* ) ;

