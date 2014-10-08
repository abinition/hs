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
/**************** Typdefs *************************************************/
#ifndef __TYPDEF_H_
#define __TYPDEF_H_

typedef unsigned char		sLOGICAL ;
typedef unsigned char		sBYTE ;
typedef unsigned short		sWORD ;
#ifndef AS_WINDOWS
typedef   int		SOCKET ;
typedef   int		HANDLE ;
#endif
#ifndef AS_WINDOWS
typedef struct iosb_t OVERLAPPED, *LPOVERLAPPED;  
#endif	
typedef struct instance_t 	sInstance ;
typedef struct concept_t 	sConcept ;
typedef struct parse_t		sParse ;
typedef struct exec_t		sExec ;
typedef struct hyp_t 		sHyp ;
typedef struct code_t		sCode ;	
typedef struct frame_t 		sFrame ;	
typedef struct field_t 		sField ;	
typedef struct level_t 		sLevel ;	
typedef struct handler_t       	sHandler ;
typedef struct reply_t 		sReply ;	
typedef struct hstack_t		sStack ;	
typedef struct data_t 		sData ;	
typedef struct AImsg_t 		sAImsg ;
typedef struct aip_t		sAIP ;	
typedef struct dsc_t 		sDescr ;
typedef struct endian_t 	sEndian ;
typedef struct sockaddr_un 	sSockUNIX ;
typedef struct sockaddr    	sSockGENERIC ;
typedef struct sockaddr_in 	sSockINET ;
typedef struct sockaddr_dn 	sSockDNET ;
typedef struct sock_t 		sSock ;
typedef struct channel_t       	sChannel ;
typedef struct port_t 		sPort ;
typedef struct hash_t 		sHash ;
typedef struct alias_t 		sAlias ;
typedef struct hsms_t		sHsms ;
typedef struct secs_t		sSecs ;
typedef struct secs1_t 		sSecs1 ;
typedef struct secs1ts_t 	sSecs1ts ;
typedef struct secs2_t 		sSecs2 ;
typedef struct hsmsHeader_t	sHsmsHeader ;
typedef struct secsHeader_t  	sSecsHeader ;
typedef struct keyword_t  	sKeyWord ;
typedef int 			Sigfunc(int) ;
typedef struct cgi_entry_t      sCgi ; 
typedef struct method_t		sMethod ;
typedef struct label_tt		sLabel ;
typedef struct ssl_t		sSSL ;
typedef struct http_t		sHTTP ;

#endif /* __TYPDEF_H_ */
