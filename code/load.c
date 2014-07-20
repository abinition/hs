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
 *   $Log: load.c,v $
 *   Revision 1.63  2013-05-15 16:42:52  bergsma
 *   Comment
 *
 *   Revision 1.62  2011-07-03 16:35:19  bergsma
 *   Added load_csv and csv
 *
 *   Revision 1.61  2011-01-08 21:36:32  bergsma
 *   Added urlparse, indexof, jdescribe, jsdescribe, jfdescribe,
 *
 *   Revision 1.60  2010-07-05 16:03:17  bergsma
 *   Allow input literals larger than 512 characters.
 *
 *   Revision 1.59  2010-05-05 04:55:41  bergsma
 *   Added http_asctime
 *
 *   Revision 1.58  2010-01-08 02:44:57  bergsma
 *   Added ssl_md5(), enhanced ssl_digest.
 *   Fixed urldecode, missing ":"
 *   Enabled object calls, ie:  text.strtok( ) and the like...
 *
 *   Revision 1.57  2009-12-08 20:49:02  bergsma
 *   placeholder for owl constructs
 *
 *   Revision 1.56  2009-06-23 23:21:12  bergsma
 *   HS 3.8.6 PF Milestone
 *
 *   Revision 1.55  2009-06-14 13:01:43  bergsma
 *   Post HS_385 Fixes - some functions such as port_binary, port_eagain,
 *   were not actually working (enabled).
 *
 *   Revision 1.54  2009-06-12 05:07:10  bergsma
 *   HS 385 TAGGING - Added pid() and setheap()
 *
 *   Revision 1.53  2009-03-06 04:13:14  bergsma
 *   Adding GD functionality
 *
 *   Revision 1.52  2008-10-10 14:43:46  bergsma
 *   INFORMIX
 *
 *   Revision 1.51  2008-03-05 22:21:10  bergsma
 *   Try and get recvmsg and sendmsg working for TRU64
 *
 *   Revision 1.50  2008-02-17 02:10:46  bergsma
 *   Added deg2rad and rad2deg
 *
 *   Revision 1.49  2007-10-27 01:55:39  bergsma
 *   newline
 *
 *   Revision 1.48  2007-07-11 18:51:16  mhohimer
 *   Added new functions:
node_name, node_parent, node_root, node_firstchild, node_lastchild, node_nextsibling,
 *   node_prevsibling, node_nextfirstcousin, node_prevlastcousin, node_getnodebyattr
 *
 *   Revision 1.47  2007-07-09 05:35:30  bergsma
 *   Add urlstring()
 *
 *   Revision 1.46  2007-06-16 17:57:43  bergsma
 *   palceholder for 'when'
 *
 *   Revision 1.45  2007-04-19 00:51:08  bergsma
 *   Don't need to hide sql_* functions behind AS_SQL
 *
 *   Revision 1.44  2007-03-26 21:16:18  bergsma
 *   Added parseurl function().
 *
 *   Revision 1.43  2007-03-21 02:21:31  bergsma
 *   Added round() function.
 *
 *   Revision 1.42  2007-03-19 05:32:08  bergsma
 *   New functions: min, max, pow, hypot, abs, acos, asin, atan,  ceil, cos,
 *    floor, log10, logN, sin, sqrt, tan, exp printf, sprintf, fprintf
 *
 *   Revision 1.41  2007-03-15 01:46:36  bergsma
 *   New functions.
 *
 *   Revision 1.40  2007-03-15 01:14:20  bergsma
 *   New functions.
 *
 *   Revision 1.39  2007-02-24 01:54:21  bergsma
 *   Added secs_*_raw functions.
 *
 *   Revision 1.38  2007-02-17 01:53:13  bergsma
 *   Socket handoff does not work with TRUE64
 *
 *   Revision 1.37  2006-11-13 02:11:34  bergsma
 *   Added functions ssl_setState and ssl_getState
 *
 *   Revision 1.36  2006/10/27 17:27:19  bergsma
 *   Added port_sendmsg and port_recvmsg
 *
 *   Revision 1.35  2006/10/11 16:16:00  bergsma
 *   Make EAGAIN an optional feature that must be turned on.
 *
 *   Revision 1.34  2006/10/01 16:25:26  bergsma
 *   Added support for asctime() function and the means to parse it (in dateparse.c)
 *
 *   Revision 1.33  2006/08/28 21:04:40  bergsma
 *   Placeholder for parseurl() function.
 *
 *   Revision 1.32  2006/05/07 18:32:48  bergsma
 *   From strcpy to memcpy
 *
 *   Revision 1.31  2006/01/23 05:12:02  bergsma
 *   Added port_binary() function.
 *
 *   Revision 1.30  2005/12/27 02:53:24  bergsma
 *   Removed functions ssl_certfile and ssl_keyfile
 *
 *   Revision 1.29  2005/12/17 20:41:14  bergsma
 *   no message
 *
 *   Revision 1.28  2005/10/25 16:39:37  bergsma
 *   Added usleep() function
 *
 *   Revision 1.27  2005/10/15 21:42:09  bergsma
 *   Added renameto functionality.
 *
 *   Revision 1.26  2005/09/06 10:18:08  bergsma
 *   Fixed pack() function
 *
 *   Revision 1.25  2005/07/23 22:31:53  bergsma
 *   Added insertafter and Insertbefore and vt2html
 *
 *   Revision 1.24  2005/05/10 17:42:54  bergsma
 *   urlencode and urldecode
 *
 *   Revision 1.23  2005/05/10 17:39:38  bergsma
 *   Added env, URLencode, URLdecode
 *
 *   Revision 1.22  2005/05/10 17:33:19  bergsma
 *   Added functions env(), URLencode(), URLdecode()
 *
 *   Revision 1.21  2005/04/13 13:45:54  bergsma
 *   HS 3.5.6
 *   Added sql_toexternal.
 *   Fixed handling of strings ending with bs (odd/even number of backslashes)
 *   Better handling of exception condition.
 *
 *   Revision 1.20  2005/03/09 04:15:41  bergsma
 *   Added appendval, insertval, and scopeof
 *
 *   Revision 1.19  2005/01/25 05:45:19  bergsma
 *   Added mapi_register and mapi_getPort
 *
 *   Revision 1.18  2005/01/18 20:42:41  jbergsma
 *   Enable the jeval (JavaScript call) method for the WebPickle ATL project
 *
 *   Revision 1.17  2004/10/16 04:46:20  bergsma
 *   Better truncation warning message/
 *
 *   Revision 1.16  2004/09/18 21:45:20  bergsma
 *   New functions ssl_getSession, ssl_setSession, http_binary, unmap
 *
 *   Revision 1.15  2004/07/23 18:43:29  bergsma
 *   Added pcapture()
 *
 *   Revision 1.14  2004/07/06 00:28:14  bergsma
 *   Added 'valueof()' function.
 *
 *   Revision 1.13  2004/06/13 14:05:41  bergsma
 *   Functions localhost, localaddr, jeval
 *
 *   Revision 1.12  2004/04/29 02:33:10  bergsma
 *   New functions.
 *   encode,decode
 *   ssl_new,ssl_ciphers,ssl_keyFile,ssl_certFile,ssl_CApath,
 *   ssl_CAfile,ssl_auth,ssl_authClient,ssl_assign,ssl_delete
 *   http_open,http_service,http_close,http_event,http_query,
 *   http_reply,http_assign,http_enable
 *   tunnel,forward,unforward,service,open
 *   fread,fwrite,load_binary
 *   on_connect
 *
 *
 *   Revision 1.10  2003/06/05 21:49:40  bergsma
 *   Added new binary operator **, example: 5**3 == 125
 *
 *   Revision 1.9  2003/04/04 16:24:24  bergsma
 *   New functions plogopen, plogclose, strip
 *
 *   Revision 1.8  2003/02/17 10:04:00  bergsma
 *   Literal tokens, enclosed in "", which contain escape sequences \nnn
 *   should NOT be converted to internal form before the token is
 *   hashed in the token lookup table, otherwise values past a \0 character
 *   would be lost.  Instead, convert the string to its internal form only
 *   when it is needed as an operand.  See operand.c
 *
 *   Revision 1.7  2003/01/12 07:07:16  bergsma
 *   Added instantiation, children, concept, parent, instance, instances, moveto.
 *   Removed poll(), same as sleep(0).
 *
 *   Revision 1.6  2002/11/29 19:59:01  bergsma
 *   Removed ^M
 *
 *   Revision 1.5  2002/11/29 19:45:12  bergsma
 *   Added load() function.
 *
 *   Revision 1.4  2002/11/19 02:00:46  bergsma
 *   Removed plogin() again.  It doesn't appear to function correctly on PROMIS.
 *   VS: ----------------------------------------------------------------------
 *
 *   Revision 1.3  2002/11/18 23:08:07  bergsma
 *   Resurrect the plogin() function, but this time make it executable by
 *   PROMIS superuser's only.
 *
 *   Revision 1.2  2002/11/14 01:29:50  bergsma
 *   Added instances() function
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/
#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL FUNCTION DECLARATIONS ***********************/

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

static sLOGICAL	gInComment = FALSE ;	/* Processing a comment */

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/**********************	FUNCTION DEFINITIONS ********************************/

struct keyword_t {
  sBYTE tokenType ;
  sBYTE tokenPrecedence ;
  void	(*function)(sInstance *pAI,sCode *pCode,sLOGICAL isPARSE) ;
} ;

static sHash *(*gpKeyHash)[MAX_HASH_TABLE_SIZE] ;


static void lHyp_load_newKey ( char *kw,
			       void (*fn)(sInstance *pAI,sCode *pCode,sLOGICAL isPARSE), 
			       sBYTE tt,
			       sBYTE tp )
{
  sKeyWord
    *pKeyWord ;

  pKeyWord = (sKeyWord*) AllocMemory ( sizeof ( sKeyWord ) ) ;
  pKeyWord->tokenType = tt ;
  pKeyWord->tokenPrecedence = tp ;
  pKeyWord->function = fn ;
  gHyp_hash_update ( gpKeyHash, KEYWORD_HASH_TABLE_SIZE, kw, pKeyWord ) ;
}

void gHyp_load_new ()
{
  gpKeyHash = gHyp_hash_new ( KEYWORD_HASH_TABLE_SIZE ) ;

  /* ELSE */
  lHyp_load_newKey ( "else", gHyp_stmt_else, TOKEN_KEYWORD, PRECEDENCE_EMPTY ) ;

  /* CONTROL STATEMENTS */
  lHyp_load_newKey ( "if", gHyp_stmt_if, TOKEN_KEYWORD, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "while", gHyp_stmt_while, TOKEN_KEYWORD, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "do", gHyp_stmt_do, TOKEN_KEYWORD, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "for", gHyp_stmt_for, TOKEN_KEYWORD, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "when", gHyp_stmt_when, TOKEN_KEYWORD, PRECEDENCE_UNARY ) ;

  /* FUNCTION EXPRESSIONS */

  /* Typecast functions */
  lHyp_load_newKey ( "int", gHyp_type_int, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "float" ,  gHyp_type_float, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "str" ,  gHyp_type_str, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "short" ,  gHyp_type_short, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ushort" ,  gHyp_type_ushort, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "long" ,  gHyp_type_long, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ulong" ,  gHyp_type_ulong, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "double" ,  gHyp_type_double, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "byte" ,  gHyp_type_byte, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ubyte" ,  gHyp_type_ubyte, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "char" ,  gHyp_type_char, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "uchar" ,  gHyp_type_uchar, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "list" ,  gHyp_type_list, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "binary" ,  gHyp_type_binary, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "boolean" ,  gHyp_type_boolean, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "hex" ,  gHyp_type_hex, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "octal" ,  gHyp_type_octal, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "unicode" ,  gHyp_type_unicode, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "handle" ,  gHyp_type_handle, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "attr" ,  gHyp_type_attr, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;

  /* Other Functions */
  lHyp_load_newKey ( "goto", gHyp_branch_goto, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "return" ,  gHyp_stmt_return, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "continue" ,  gHyp_stmt_continue, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "break" ,  gHyp_stmt_break, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ; 
    
  /* Message functions */
  lHyp_load_newKey ( "query" ,  gHyp_route_query, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "event" ,  gHyp_route_event, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "reply" ,  gHyp_route_reply, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "requeue" ,  gHyp_route_requeue, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "enable" ,  gHyp_route_enable, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "disable" ,  gHyp_route_disable, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "service" ,  gHyp_route_service, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "open",  gHyp_route_open, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

  lHyp_load_newKey ( "sender" ,  gHyp_function_sender, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "self" ,  gHyp_function_self, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "mode" ,  gHyp_function_mode, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "method" ,  gHyp_function_method, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "tid" ,  gHyp_function_tid, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sid" ,  gHyp_function_sid, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "pid" ,  gHyp_function_pid, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "idle" ,  gHyp_env_idle, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "instantiate",  gHyp_env_instantiate, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "instantiation",  gHyp_env_instantiation, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

  lHyp_load_newKey ( "children",  gHyp_route_children, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "concept",  gHyp_route_concept, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "parent",  gHyp_route_parent, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "instance",  gHyp_route_instance, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "instances",  gHyp_route_instances, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "moveto",  gHyp_route_moveto, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "renameto",  gHyp_route_renameto, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

  /* Handler support functions */
  lHyp_load_newKey ( "on_message" ,  gHyp_branch_on_message, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ; 
  lHyp_load_newKey ( "alarm" ,  gHyp_function_alarm, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "on_alarm" ,  gHyp_branch_on_alarm, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ; 
  lHyp_load_newKey ( "on_error" ,  gHyp_branch_on_error, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "on_interrupt", gHyp_branch_on_interrupt, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "on_pipe", gHyp_branch_on_pipe,  TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "on_hangup", gHyp_branch_on_hangup,  TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "on_connect", gHyp_branch_on_connect,  TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "timeout", gHyp_function_timeout, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "on_timeout",  gHyp_branch_on_timeout,  TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "lifetime" ,  gHyp_function_lifetime, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "on_death" ,  gHyp_branch_on_death, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

  /* System functions */
  lHyp_load_newKey ( "sleep" ,  gHyp_system_sleep, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "usleep" ,  gHyp_system_usleep, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "exec" ,  gHyp_system_exec, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "system" ,  gHyp_system_system, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "parse" ,  gHyp_system_parse, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "srandom" ,  gHyp_system_srandom, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "random" ,  gHyp_system_random, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "datetime" ,  gHyp_system_datetime, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "asctime" ,  gHyp_system_asctime, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_asctime" ,  gHyp_http_asctime, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "time" ,  gHyp_system_time, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "date" ,  gHyp_system_date, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ; 
  lHyp_load_newKey ( "timestamp" ,  gHyp_system_timestamp, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "getclock" ,  gHyp_system_getclock, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "parsedate" ,  gHyp_system_parsedate, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "getenv" ,  gHyp_system_getenv, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "setenv" ,  gHyp_system_setenv, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "unsetenv" ,  gHyp_system_unsetenv, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "setheap" ,  gHyp_system_setheap, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

#ifdef AS_PROMIS
  /* PROMIS functions */
  lHyp_load_newKey ( "pexec" ,  gHyp_promis_pexec, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "pget" ,  gHyp_promis_pget, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "pnext" ,  gHyp_promis_pnext, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "psnap" ,  gHyp_promis_psnap, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "popen" ,  gHyp_promis_popen, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "pclose" ,  gHyp_promis_pclose, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "plogopen" ,  gHyp_promis_plogopen, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "plogclose" ,  gHyp_promis_plogclose, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "pcapture" ,  gHyp_promis_pcapture, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  /*lHyp_load_newKey ( "plogin" ,  gHyp_promis_plogin, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;*/
#endif

#if defined( AS_JNI ) || defined( AS_ATL )
  lHyp_load_newKey ( "jeval" ,  gHyp_hs_jeval, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
#endif

  /* Conversion functions */
  lHyp_load_newKey ( "toupper" ,  gHyp_function_toupper, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "tolower" ,  gHyp_function_tolower, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "toexternal" ,  gHyp_function_toexternal, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "tointernal" ,  gHyp_function_tointernal, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "encode" ,  gHyp_function_encode, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "decode" ,  gHyp_function_decode, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "urlparse" ,  gHyp_cgi_urlparse, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "urlstring" ,  gHyp_cgi_urlstring, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "urlencode" ,  gHyp_function_urlEncode, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "urldecode" ,  gHyp_function_urlDecode, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "vt2html" ,  gHyp_function_vt2html, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "parseurl" ,  gHyp_function_parseurl, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

  /* String functions, some like 'C' */
  lHyp_load_newKey ( "strlen" ,  gHyp_function_strlen, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "strloc" ,  gHyp_function_strloc, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "indexof" ,  gHyp_function_indexof, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "strext" ,  gHyp_function_strext, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "strtok" ,  gHyp_function_strtok, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "pack",	 gHyp_function_pack, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "trim" ,  gHyp_function_trim, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "strip" ,  gHyp_function_strip, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;


  /* File functions */
  lHyp_load_newKey ( "log" ,  gHyp_fileio_log, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fopen" ,  gHyp_fileio_fopen, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fclose" ,  gHyp_fileio_fclose, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fput" ,  gHyp_fileio_fput, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fputs" ,  gHyp_fileio_fputs, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fread" ,  gHyp_fileio_fread, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fwrite" ,  gHyp_fileio_fwrite, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fget" ,  gHyp_fileio_fget, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fgets" ,  gHyp_fileio_fgets, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "fdescribe" ,  gHyp_fileio_fdescribe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "describe",  gHyp_fileio_describe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sdescribe",  gHyp_fileio_sdescribe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "put" ,  gHyp_fileio_put, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "puts" ,  gHyp_fileio_puts, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "get" ,  gHyp_fileio_get, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "gets" ,  gHyp_fileio_gets, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "unlink" ,  gHyp_fileio_unlink, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "echo" ,  gHyp_fileio_echo, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "load" ,  gHyp_fileio_load, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "load_binary" ,  gHyp_fileio_load_binary, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "load_csv" ,  gHyp_fileio_load_csv, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "csv" ,  gHyp_fileio_csv, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "checksum" ,  gHyp_fileio_checksum, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "crc32" ,  gHyp_fileio_crc32, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "round" ,  gHyp_function_round, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;

  /* XML Conversion */
  lHyp_load_newKey ( "xdescribe", gHyp_fileio_xdescribe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "xsdescribe", gHyp_fileio_xsdescribe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "xfdescribe", gHyp_fileio_xfdescribe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "xml", gHyp_cgi_xml, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "xparse" , gHyp_system_xparse, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

  /* JSON */
  lHyp_load_newKey ( "jdescribe", gHyp_fileio_jdescribe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "jsdescribe", gHyp_fileio_jsdescribe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "jfdescribe", gHyp_fileio_jfdescribe, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  /*lHyp_load_newKey ( "json", gHyp_cgi_xml, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;*/
  /*lHyp_load_newKey ( "jparse" , gHyp_system_jparse, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;*/

  /* SQL Interface */
  lHyp_load_newKey ( "sql_toexternal", gHyp_sql_toexternal, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sql_datetime", gHyp_sql_datetime, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
#ifndef AS_INFORMIX
  /* SQL functions */
  lHyp_load_newKey ( "sql_open",  gHyp_sql_open, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sql_close", gHyp_sql_close, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sql_query", gHyp_sql_query, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
#else
  lHyp_load_newKey ( "sql_open",  gHyp_esql_connect, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sql_query", gHyp_esql_query, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
#endif

  /* SECS functions */
  lHyp_load_newKey ( "secs_hsms" ,  gHyp_secs_hsms, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_service", gHyp_secs_service, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_open" ,  gHyp_secs_open, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_assign" ,  gHyp_secs_assign, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_enable" ,  gHyp_secs_enable, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_query" ,  gHyp_secs_query, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_event" ,  gHyp_secs_event, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_query_raw" ,  gHyp_secs_query_raw, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_event_raw" ,  gHyp_secs_event_raw, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_disable" ,  gHyp_secs_disable, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_close" ,  gHyp_secs_close, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_mlb" ,  gHyp_secs_mlb, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_mhp" ,  gHyp_secs_mhp, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_handle" ,  gHyp_secs_handle, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_xml" ,  gHyp_secs_xml, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_map" ,  gHyp_env_secs_map, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "secs_unmap" ,  gHyp_env_secs_unmap, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;


  /* Port functions */
  lHyp_load_newKey ( "port_service" ,  gHyp_port_service, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_open" ,  gHyp_port_open, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_assign" ,  gHyp_port_assign, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_enable" ,  gHyp_port_enable, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_query" ,  gHyp_port_query, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_event" ,  gHyp_port_event, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_disable" ,  gHyp_port_disable, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_close" ,  gHyp_port_close, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_handle" ,  gHyp_port_handle, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_binary" ,  gHyp_port_binary, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_eagain" ,  gHyp_port_eagain, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_stop" ,  gHyp_port_stop, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_go" ,  gHyp_port_go, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

#if defined ( AS_UNIX ) 
  lHyp_load_newKey ( "port_recvmsg" ,  gHyp_port_recvmsg, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "port_sendmsg" ,  gHyp_port_sendmsg, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
#endif

  /* Tunneling functions */
  lHyp_load_newKey ( "tunnel", gHyp_route_tunnel, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "forward" ,  gHyp_route_forward, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "unforward" ,  gHyp_route_unforward, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

  /* HTTP functions */
  lHyp_load_newKey ( "http_service", gHyp_http_service, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_open" ,  gHyp_http_open, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_assign" ,  gHyp_http_assign, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_enable" ,  gHyp_http_enable, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_query" ,  gHyp_http_query, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_event" ,  gHyp_http_event, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_reply" ,  gHyp_http_reply, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_disable" ,  gHyp_http_disable, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_close" ,  gHyp_http_close, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_handle" ,  gHyp_http_handle, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_binary" ,  gHyp_http_binary, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "http_eagain" ,  gHyp_http_eagain, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

#ifdef AS_SSL
  /* SSL functions */
  lHyp_load_newKey ( "ssl_new", gHyp_ssl_new, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_ciphers",  gHyp_ssl_ciphers, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_CApath",  gHyp_ssl_CApath, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_CAfile",  gHyp_ssl_CAfile, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_auth",  gHyp_ssl_auth, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_authClient",  gHyp_ssl_authClient, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_assign",  gHyp_ssl_assign, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_delete", gHyp_ssl_delete, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_getSession", gHyp_ssl_getSession, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_setSession", gHyp_ssl_setSession, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_enableSessions", gHyp_ssl_enableSessions, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_getState", gHyp_ssl_getState, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_setState", gHyp_ssl_setState, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_digest", gHyp_ssl_digest, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ssl_md5", gHyp_ssl_md5, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
#endif

#ifdef AS_GD
  lHyp_load_newKey ( "gd_open",  gHyp_gd_open, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "gd_query", gHyp_gd_query, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "gd_close", gHyp_gd_close, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
#endif

#ifdef AS_MAPI
  lHyp_load_newKey ( "mapi_register", gHyp_mapi_register, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "mapi_getPort", gHyp_mapi_getPort, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  /*
  lHyp_load_newKey ( "mapi_map", gHyp_mapi_getDrive, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "mapi_unmap", gHyp_mapi_getDrive, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  */
#endif

  /* Environment */
  lHyp_load_newKey ( "undef" ,  gHyp_env_undef, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "exists" ,  gHyp_env_exists, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "count" ,  gHyp_env_count, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "hashed" ,  gHyp_env_hashed, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "unhashed" ,  gHyp_env_unhashed, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "local" ,  gHyp_env_local, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "global" ,  gHyp_env_global, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "next" ,  gHyp_env_next, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "prev" ,  gHyp_env_prev, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "insert" ,  gHyp_env_insert, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "append" ,  gHyp_env_append, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "insertbefore" ,  gHyp_env_insertbefore, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "insertafter" ,  gHyp_env_insertafter, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "remove" ,  gHyp_env_remove, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "chop" ,  gHyp_env_chop, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "detach" ,  gHyp_env_detach, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "insertval" ,  gHyp_env_insertval, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "appendval" ,  gHyp_env_appendval, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "map" ,  gHyp_env_map, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "unmap" ,  gHyp_env_unmap, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "merge" ,  gHyp_env_merge, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sjm" ,  gHyp_env_sjm, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sort" ,  gHyp_env_sort, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "reverse" ,  gHyp_env_reverse, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "debug" ,  gHyp_env_debug, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "exit" ,  gHyp_env_exit, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "quit" ,  gHyp_env_quit, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "quiet" ,  gHyp_env_quiet, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "verify" ,  gHyp_env_verify, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "version" ,  gHyp_env_version, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "env" ,    gHyp_env_env, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "typeof" ,  gHyp_type_typeof, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "valueof",  gHyp_type_valueof, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "scopeof",  gHyp_type_scopeof, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "localhost" ,  gHyp_env_localhost, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "localaddr" ,  gHyp_env_localaddr, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "round" ,  gHyp_function_round, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_name" ,  gHyp_env_node_name, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_parent" ,  gHyp_env_node_parent, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_root" ,  gHyp_env_node_root, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_firstchild" ,  gHyp_env_node_firstchild, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_lastchild" ,  gHyp_env_node_lastchild, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_nextsibling" ,  gHyp_env_node_nextsibling, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_prevsibling" ,  gHyp_env_node_prevsibling, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;  
  lHyp_load_newKey ( "node_nextfirstcousin" ,  gHyp_env_node_nextfirstcousin, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_prevlastcousin" ,  gHyp_env_node_prevlastcousin, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_getnodebyattr" ,  gHyp_env_node_getnodebyattr, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "node_getnodebyname" ,  gHyp_env_node_getnodebyname, TOKEN_TYPECAST, PRECEDENCE_UNARY ) ;

  /* Math functions */
  lHyp_load_newKey ( "max" ,  gHyp_operator_max, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "min" ,  gHyp_operator_min, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "hypot", gHyp_operator_hypot, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "pow" ,  gHyp_operator_pow, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "abs" ,  gHyp_operator_abs, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "acos" ,  gHyp_operator_acos, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "asin" ,  gHyp_operator_asin, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "atan" ,  gHyp_operator_atan, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "ceil" ,  gHyp_operator_ceil, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "cos" ,  gHyp_operator_cos, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "floor" ,  gHyp_operator_floor, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "log10" ,  gHyp_operator_log10, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "logN" ,  gHyp_operator_logN, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sin" ,  gHyp_operator_sin, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "sqrt" ,  gHyp_operator_sqrt, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "tan" ,  gHyp_operator_sqrt, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "exp" ,  gHyp_operator_exp, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "deg2rad" ,  gHyp_operator_deg2rad, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;
  lHyp_load_newKey ( "rad2deg" ,  gHyp_operator_rad2deg, TOKEN_FUNCTION, PRECEDENCE_UNARY ) ;

#ifdef AS_OWL
  lHyp_load_newKey ( "and" , gHyp_operator_binary, TOKEN_AND, PRECEDENCE_B ) ;
 !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"*",
				TOKEN_MUL, 
				PRECEDENCE_MULDIVMOD,
				lineNo,
				gHyp_operator_binary ) 
#endif
}

void gHyp_load_delete()
{
  sHash 
    *pNext,
    *pHash ;

  sKeyWord
    *pKeyWord ;

  int
    i ;

  /* Free the secs structures */
  for ( i=0; i<KEYWORD_HASH_TABLE_SIZE; i++ )
    for ( pHash = (*gpKeyHash)[i]; pHash; pHash=pNext ) {
      pNext = gHyp_hash_next ( pHash ) ;
      pKeyWord = (sKeyWord*) gHyp_hash_object ( pHash ) ;
      ReleaseMemory ( pKeyWord ) ;
      gHyp_hash_free ( pHash ) ;
    }
  gHyp_hash_delete ( gpKeyHash ) ;
}

sLOGICAL gHyp_load_isKey ( char *pToken )
{
  if ( gHyp_hash_find ( gpKeyHash,
			KEYWORD_HASH_TABLE_SIZE,
			pToken ) )
    return TRUE ;
  else
    return FALSE ;
}

void (*gHyp_load_fun(char *pToken, char *tt, char *tp))
	 (sInstance* pAI, sCode *pCode, sLOGICAL isPARSE)
{
  /* Description:
   *
   *	Returns a pointer to a function that executes a HyperScript
   *	language element token.
   *
   * Arguments:
   *
   *	pToken						[R]
   *	- pointer to HyperScript token
   *
   *	tt						[W]
   *	- pointer to char to contain token type
   *
   *	tp						[W]
   *	- pointer to char to contain token precedence
   *
   * Return value:
   *
   *	Pointer to HyperScript language element token function.
   *
   */
  /* Check for keyword tokens */

  /* Functions are loosely ordered according to frequency of use. */
  
  sKeyWord
    *pKeyWord  = (sKeyWord*) gHyp_hash_find ( gpKeyHash,
				  KEYWORD_HASH_TABLE_SIZE,
				  pToken ) ;
  if ( pKeyWord ) {
    *tt = pKeyWord->tokenType ;
    *tp = pKeyWord->tokenPrecedence ;
    return pKeyWord->function ;
  }
  else {

    /* Everything else is either an identifier or a constant. */
    
    *tp = PRECEDENCE_OPERAND ;
    if ( gHyp_util_isIdentifier ( pToken ) )
      *tt = TOKEN_UNIDENTIFIED ;
    else
      *tt = TOKEN_CONSTANT ;
    return (void (*)(sInstance*,sCode*,sLOGICAL)) gHyp_operand_token ;
  }
}

char *gHyp_load_fromStream (	sInstance *pAI,
				sHyp 	*pHyp,
				char    *pStream,
				int    	lineNo )
{
  /* Description:
   *
   *	Load a line of HyperScript code to the end of the HyperScript program.
   *	This routine is responsible for recognizing and separating tokens 
   *	from the input stream.  Just like the UNIX utilities 'lex' and yacc,
   *	this routine is equivalent to 'lex': lexical processor.
   *
   * Arguments:
   *
   */
  char		
    *pToken,
    *pStr,
    *pBS,
    *pSearch,
    ch,
    tokenType,
    precedence ;

  int
    bs,
    tokenLen,
    scanLen,
    stringLen ;

  sLOGICAL
    wasLoaded,
    terminated ;

  void
    (*pf)(sInstance*,sCode*,sLOGICAL) ;

  sConcept
    *pConcept = gHyp_instance_getConcept ( pAI ) ;

  /* Extract out identifiers - variables, values, constants, strings, 
   * keywords, and punctuation 
   */
  while ( *pStream ) {

    /* Pull out identifiers; they are either keywords, variables,
     * constants, strings, or labels.
     */
    while ( ( scanLen = abs ( gHyp_util_getToken ( 
			pStream, gHyp_hyp_okUnaryOperator ( pHyp ) ) ) ) ) {

      /* Found a variable, keyword, constant, identifier, or garbage... */
      if ( !gInComment ) {
	pToken = (char*) AllocMemory ( scanLen+1 ) ;
	assert ( pToken ) ;
        memcpy ( pToken, pStream, scanLen ) ;
        pToken[scanLen] = '\0' ;
	/*gHyp_util_debug("TOKEN is %s",pToken);*/
        pf = gHyp_load_fun ( pToken, &tokenType, &precedence ) ;
        wasLoaded = gHyp_hyp_load ( 	pAI,
					pConcept,
					pHyp, 
					pToken,
					tokenType,
					precedence,
					lineNo,
					pf ) ;

	ReleaseMemory ( pToken ) ;
	if ( !wasLoaded ) return NULL ;
      }
      pStream += scanLen ;
      if ( !(*pStream) ) break ;
    }

    /* Quit if we're at the end of the string token */
    if ( !(*pStream) ) break ;

    switch ( *pStream ) {

    case '"' :	/* String */
    case '\'' :	/* Variable */
    case '`' : /* Raw character array */
      
      if ( gInComment ) { pStream++ ; break ; }
      
      /* Start of quoted string. Find end quote */
      ch = *pStream ;
      terminated = FALSE ;
      pSearch = pStream + 1 ;
      while ( !terminated ) {
	
	pStr = strchr ( pSearch, ch ) ;
	
	/* If no ending quote, quit, returning pointer to start */
	if ( pStr == NULL ) return pStream ;
	
	/* If the ending quote is preceeded by a odd number of backslashes, then its not
	 * the ending quote.
	 */
	if ( *(pStr-1) == '\\' ) {
	  bs = 1 ;
	  pBS = pStr-2 ;
	  while ( *pBS-- == '\\' ) bs++ ;
	  if ( bs%2 == 0 )
	    terminated = TRUE ;
	  else
	    /* Not a correct terminator,  keep searching. */
	    pSearch = pStr+1 ;
	}
	else
	  terminated = TRUE ;

      }
      
      /* Replace ending quote with terminating character */
      *pStr = '\0' ;
      
      /* Advance past starting quote */
      pStream++ ;
      
      /* Parse the string */
      stringLen = strlen ( pStream ) ;
      /***stringLen = gHyp_util_parseString ( pStream ) ;***/
      
      /* Extract string - maximum of MAX_INPUT_LENGTH characters. */
      tokenLen = MIN ( MAX_INPUT_LENGTH, stringLen ) ;
      pToken = (char*) AllocMemory ( tokenLen + 1 ) ;
      assert ( pToken ) ;
      if ( tokenLen > 0 ) strncpy ( pToken, pStream, tokenLen ) ;
      pToken[tokenLen] = '\0' ;
      if ( stringLen > MAX_INPUT_LENGTH ) {
	gHyp_util_logError ( "%c%s...%c was truncated at %d characters", 
	  ch, pToken, ch, MAX_INPUT_LENGTH ) ;
	guErrorCount++ ;
      }
      
      /* Advance past quoted string. */
      pStream = pStr + 1 ;
      
      /* Load */
      wasLoaded = gHyp_hyp_load ( pAI,
				  pConcept,
				  pHyp, 
				  pToken,
				  (sBYTE) ( (ch == '"') ? 
					    TOKEN_LITERAL :
					    ((ch == 96) ? 
					     TOKEN_RAW :
					     TOKEN_UNIDENTIFIED )),
				  PRECEDENCE_OPERAND, 
				  lineNo,
				  gHyp_operand_token ) ;
      ReleaseMemory ( pToken ) ;
      if ( !wasLoaded ) return NULL ;
      break ;
      
    case ';' :	/* End of statement */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				";",
				TOKEN_EOS, 
				PRECEDENCE_EOS,
				lineNo,
				gHyp_stmt_eos ) ) return NULL ;
      break ;
      
    case '?' :	/* "exp1 ? exp2 : exp3" condition */

      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"?",
				TOKEN_CONDITION, 
				PRECEDENCE_CONDITION,
				lineNo,
				gHyp_operator_condition ) ) 
	return NULL;
      break ;
      
    case ':' :	/* "exp1 ? exp2 : exp3" condition or label marker or JSON assignment */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				":",
				TOKEN_EVAL,
				PRECEDENCE_EVAL,
				lineNo,
				gHyp_operator_eval ) ) return NULL ;
      break ;
      
      
    case '(' :	/* Begin expression */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"(",
				TOKEN_BEXP, 
				PRECEDENCE_LEFT,
				lineNo,
				gHyp_stmt_bExp ) ) return NULL ;
      break ;
      
    case ')' :	/* End expression */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load ( 	pAI,
				pConcept,
				pHyp, 
				")",
				TOKEN_EEXP, 
				PRECEDENCE_RIGHT,
				lineNo,
				gHyp_stmt_eExp ) ) return NULL ;
      break ;
      
    case '{' :	/* Begin list */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load ( 	pAI,
				pConcept,
				pHyp, 
				"{",
				TOKEN_BLIST, 
				PRECEDENCE_LEFT,
				lineNo,
				gHyp_stmt_bList ) ) return NULL ;
      break ;
      
    case '}' :	/* End list */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load ( 	pAI,
				pConcept,
				pHyp, 
				"}",
				TOKEN_ELIST, 
				PRECEDENCE_RIGHT,
				lineNo,
				gHyp_stmt_eList ) ) return NULL ;
      break ;
      
    case ',' :	/* Next in list */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load ( 	pAI,
				pConcept,
				pHyp, 
				",",
				TOKEN_LIST, 
				PRECEDENCE_LIST,
				lineNo,
				gHyp_stmt_list ) ) return NULL ;
      break ;
      
    case '.' :	/* Structure separator */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load ( 	pAI,
				pConcept,
				pHyp, 
				".",
				TOKEN_DOT, 
				PRECEDENCE_DOT,
				lineNo,
				gHyp_operator_dot ) ) return NULL ;
      break ;
      
    case '[' :	/* Begin subscript */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load ( 	pAI,
				pConcept,
				pHyp, 
				"[",
				TOKEN_BSUB, 
				PRECEDENCE_LEFT,
				lineNo,
				gHyp_stmt_bSub ) ) return NULL ;
      break ;
      
    case ']' :	/* End subscript */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load ( 	pAI,
				pConcept,
				pHyp, 
				"]",
				TOKEN_ESUB, 
				PRECEDENCE_RIGHT,
				lineNo,
				gHyp_stmt_eSub ) ) return NULL ;
      break ;
      
    case '*' :	/* Multiply, de-reference, power, and end comment */
      
      pStream++ ;
      if ( gInComment ) {
	if ( *pStream == '/' ) {
	  pStream++ ;
	  gInComment = FALSE ;
	}
	break ;
      }
      if ( gHyp_hyp_okUnaryOperator ( pHyp ) ) {
	
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"*",
				TOKEN_DEREF, 
				PRECEDENCE_DEREF,
				lineNo,
				gHyp_operator_dereference ) ) return NULL ;
      }
      else {
	

	if ( *pStream == '*' ) {
	  pStream++ ;
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"**",
				TOKEN_POW,
				PRECEDENCE_POW,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
	}
	else {
	  if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"*",
				TOKEN_MUL, 
				PRECEDENCE_MULDIVMOD,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
	}
      }
      break ;
      
    case '/' :	/* Divide and Begin Comment */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( *pStream == '/' ) { pStream += strlen ( pStream ) ; break ; }
      if ( *pStream == '*' ) {
	gInComment = TRUE ;
	pStream++ ;
	break ;
      }
      if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"/",
				TOKEN_DIV, 
				PRECEDENCE_MULDIVMOD,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      break ;
      
    case '+' :	/* Add, PreInc, PostInc, and Positive */
      
      pStream++ ;
      if ( gInComment ) break ;
      
      if ( gHyp_hyp_okUnaryOperator ( pHyp ) ) {
	
	if ( *pStream == '+' ) {
	  pStream++ ;
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"++",
				TOKEN_PREINC,
				PRECEDENCE_UNARY,
				lineNo,
				gHyp_operator_unary ) ) return NULL ;
	}
	else {
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"+",
				TOKEN_POS, 
				PRECEDENCE_UNARY,
				lineNo,
				gHyp_operator_unary ) ) return NULL ;
	}	
      }
      else {
	
	if ( *pStream == '+' ) {
	  pStream++ ;
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"++",
				TOKEN_POSTINC,
				PRECEDENCE_POST_UNARY,
				lineNo,
				gHyp_operator_unary ) ) return NULL ;
	}
	else {
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"+",
				TOKEN_ADD, 
				PRECEDENCE_ADDSUB,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
	}
      }
      break ;
      
    case '-' :	/*  Substract, PreDec, PostDec, and Negation */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( gHyp_hyp_okUnaryOperator ( pHyp ) ) {
	
	if ( *pStream == '-' ) {
	  pStream++ ;
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"--",
				TOKEN_PREDEC,
				PRECEDENCE_UNARY,
				lineNo,
				gHyp_operator_unary ) ) return NULL ;
	}
	else {
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"-",
				TOKEN_NEG, 
				PRECEDENCE_UNARY,
				lineNo,
				gHyp_operator_unary ) ) return NULL ;
	}
      }
      else {
	
	if ( *pStream == '-' ) {
	  pStream++ ;
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"--",
				TOKEN_POSTDEC,
				PRECEDENCE_POST_UNARY,
				lineNo,
				gHyp_operator_unary ) ) return NULL ;
	}
	else if ( *pStream == '>' ) {
	  pStream++ ;
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"->",
				TOKEN_POINTER,
				PRECEDENCE_POINTER,
				lineNo,
				gHyp_operator_pointer ) ) return NULL ;
	}
	else {
	  if ( !gHyp_hyp_load ( pAI,
				pConcept,
				pHyp, 
				"-",
				TOKEN_SUB, 
				PRECEDENCE_ADDSUB,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
	}
      }
      break ;
      
    case '%' :	/* Modulus */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"%",
				TOKEN_MOD, 
				PRECEDENCE_MULDIVMOD,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      break ;
      
    case '<' :	/* < and <= */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( *pStream == '=' ) {
	pStream++ ;
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"<=",
				TOKEN_LE,
				PRECEDENCE_GTLT,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      else {
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"<",
				TOKEN_LT, 
				PRECEDENCE_GTLT,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      break ;
      
    case '>' :	/* > and >= */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( *pStream == '=' ) {
	pStream++ ;
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				">=",
				TOKEN_GE, 
				PRECEDENCE_GTLT,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      else {
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				">",
				TOKEN_GT, 
				PRECEDENCE_GTLT,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      break ;
      
    case '=' :	/* Assignment and Comparision */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( *pStream == '=' ) {
	pStream++ ;
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"==",
				TOKEN_EQ, 
				PRECEDENCE_EQNE,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      else {
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"=",
				TOKEN_ASSIGN, 
				PRECEDENCE_ASSIGN,
				lineNo,
				gHyp_operator_assign ) ) return NULL ;
      }
      break ;
      
    case '&' :	/* AND */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( *pStream == '&' ) {
	pStream++ ;
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"&&",
				TOKEN_AND, 
				PRECEDENCE_ANDOR,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      else if ( gHyp_hyp_okUnaryOperator ( pHyp ) ) {
	
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"&",
				TOKEN_REF, 
				PRECEDENCE_UNARY,
				lineNo,
				gHyp_operator_reference ) ) return NULL ;
      }
      else {
	
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"&",
				TOKEN_BITAND, 
				PRECEDENCE_BITAND,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      break ;
      
    case '|' :	/* OR */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( *pStream == '|' ) {
	pStream++ ;
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"||",
				TOKEN_OR, 
				PRECEDENCE_ANDOR,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      else {
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"|",
				TOKEN_BITOR, 
				PRECEDENCE_BITOR,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      break ;
      
    case '^' :	/* XOR */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"^",
				TOKEN_BITXOR, 
				PRECEDENCE_BITXOR,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      break ;
      
      
    case '!' :	/* NOT and NOT Equal */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( *pStream == '=' ) {
	pStream++ ;
	if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"!=",
				TOKEN_NE, 
				PRECEDENCE_EQNE,
				lineNo,
				gHyp_operator_binary ) ) return NULL ;
      }
      else {
	if ( !gHyp_hyp_load ( pAI,
			      pConcept,
			      pHyp, 
			      "!",
			      TOKEN_NOT, 
			      PRECEDENCE_UNARY,
			      lineNo,
			      gHyp_operator_unary ) ) return NULL ;
      }
      break ;
      
    case '~' :	/* COMPLEMENT */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( !gHyp_hyp_load (	pAI,
				pConcept,
				pHyp, 
				"~",
				TOKEN_COMPLEMENT, 
				PRECEDENCE_UNARY,
				lineNo,
				gHyp_operator_unary ) ) return NULL ;
      break ;
      
    case ' ' :
      pStream++ ;
      break ;
      
    case '#' :	/* #! */
      
      pStream++ ;
      if ( gInComment ) break ;
      if ( *pStream == '!' ) pStream += strlen ( pStream ) ;
      break ;
      
    case '\\' :	/* Reserved */
    case '@' :	/* Reserved */
    default:
      ch = *pStream ;
      pStream++ ;
      if ( gInComment || ch < ' ' ) break ; 
      gHyp_util_logError ( "Ignoring illegal use of character '%c'", ch ) ;
      guErrorCount++ ;
    }
  }
  return pStream ;
}
