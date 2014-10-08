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

