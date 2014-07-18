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
 * $Log: typdef.h,v $
 * Revision 1.3  2004/04/29 02:02:25  bergsma
 * Added structure for SSL.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 */
/**************** Typdefs *************************************************/
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
