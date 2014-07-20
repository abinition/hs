/* auto.h 
 *
 * Modifications:
 *
 * $Log: auto.h,v $
 * Revision 1.30  2013-05-21 17:50:21  bergsma
 * Windows does not know about stdint include file.
 *
 * Revision 1.29  2013-05-21 17:46:26  bergsma
 * Add secs_map & secp_unmap.  Deal with 64-bit systems where long and
 * int datatypes are 32 bit.  HS long,ulong,and int are 32 bit.
 *
 * Revision 1.28  2010-03-05 06:08:46  bergsma
 * Add -u option and AUTOHOST environment variable.
 *
 * Revision 1.27  2009-06-12 05:00:49  bergsma
 * HS 385 Final Checkin and TAG
 *
 * Revision 1.26  2009-03-08 16:17:45  bergsma
 * First successfull hs/gd compile&link
 *
 * Revision 1.25  2009-01-22 01:34:01  bergsma
 * For guSIGMSG
 *
 * Revision 1.24  2008-08-27 07:22:59  bergsma
 * AUTOSPOOL was not extern
 *
 * Revision 1.23  2007-09-26 00:36:59  bergsma
 * Comment out math.h, use in operator.c only
 *
 * Revision 1.22  2007-05-08 01:27:49  bergsma
 * Make giMaxFrameDepth variable, tied to -x option.
 *
 * Revision 1.21  2007-04-19 00:51:33  bergsma
 * Don't need to hide sql_* functions behind AS_SQL
 *
 * Revision 1.20  2007-03-15 01:46:36  bergsma
 * New functions.
 *
 * Revision 1.19  2006-12-09 00:06:43  bergsma
 * Move gpsAI and gpsAImain to global external status out of hs.c.
 *
 * Revision 1.18  2005/10/15 21:36:00  bergsma
 * Mods for PPC
 *
 * Revision 1.17  2005/02/15 06:59:11  bergsma
 * No newline at end of file
 *
 * Revision 1.16  2005/01/25 05:42:06  bergsma
 * Add MAPI (NSAS Nokia support methods for Outlook forwarding)
 *
 * Revision 1.15  2005/01/10 18:02:00  bergsma
 * Add gsJmpOverride when CTRL./C is pressed during socket connect blocking
 *
 * Revision 1.14  2004/11/19 03:43:25  bergsma
 * Add another gpsTempData2 variable to catch leaks.
 *
 * Revision 1.13  2004/10/16 04:26:05  bergsma
 * HS331
 *
 * Revision 1.12  2004/02/29 14:02:49  bergsma
 * Added http.h and ssl.h
 *
 * Revision 1.11  2003/01/28 01:57:35  bergsma
 * Added gzAUTOBIN for executing hs.exe from cgi
 * Added giCondition for exit( condition ) and quit ( condition )
 *
 * Revision 1.10  2003/01/09 00:08:29  bergsma
 * V3.1.0
 * Added gzInstance global variable. (Needed by util.c for logging)
 *
 * Revision 1.9  2002/11/28 14:30:49  bergsma
 * Make pidFileSpec global (gzPIDfilespec) so it can be unlink'ed when
 * the program exits.
 *
 * Revision 1.8  2002/11/21 01:21:02  bergsma
 * Remove ^M
 *
 * Revision 1.7  2002/11/20 20:25:30  bergsma
 * Added gzRoot, the expanded version of gzParent
 *
 * Revision 1.6  2002/11/19 01:58:29  bergsma
 * Either WIN32 or __WIN32 for windows
 *
 * Revision 1.5  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.4  2002/10/22 23:57:34  bergsma
 * Added gpsTempData.
 *
 * Revision 1.3  2002/09/24 17:32:16  bergsma
 * Added giResolveFlag1, giResolveFlag2, gzResolveAddr
 *
 *
 */

#if defined ( WIN32 ) || defined ( __WIN32 ) || defined ( _WIN32_WCE )
#define AS_WINDOWS 1
#include <windows.h>
#ifdef _WIN32_WCE
#define AS_PPC 1
#endif
#elif defined ( __VMS )
#define AS_VMS 1
#else
#define AS_UNIX 1
#include <unistd.h>	/* UNIX standard functions */
#endif

#ifdef __alpha
#define AS_ALPHA 1
#endif

#include <stdarg.h>	/* va* functions */
#include <stdio.h>      /* standard IO functions */
#include <stddef.h>     /* standard types */
#ifdef AS_UNIX
#include <stdint.h>	/* int32_t def */
#endif
#include <stdlib.h>	/* standard library functions */
#include <time.h> 	/* for localtime function */
#include <errno.h>	/* errno */
#include <signal.h>	/* signal processing functions and structures */
#include <assert.h>     /* assert function */
#include <ctype.h>	/* is* macros and functions */
#include <string.h>	/* string functions */
#include <setjmp.h>	/* setjmp and longjmp functions */
/*#include <math.h>	 * math functions */

#ifdef AS_MEMTRACK
#include "memtrack.h"
#else

#define AllocMemory(size) malloc(size)

#define AllocCopyString(s) strdup(s)

#define ReAllocMemory(p, newsize) realloc(p,newsize)

#define ReleaseMemory(p) free(p)
  
#endif

struct dsc_t {
  unsigned short	dsc_w_length ;
  char			dsc_b_dtype ;
  char			dsc_b_class ;
  char			*dsc_a_pointer ;
} ;

struct endian_t 
{
  union {
    double		d ;
    float		f ;
#ifdef AS_UNIX
    int32_t		sl ;
    uint32_t		ul ;
#else
    long		sl ;
    unsigned long	ul ;
#endif
    short		ss ;
    unsigned short	us ;
    unsigned char       b[8] ;		
  } x ;
} ;


struct iosb_t {
    short   ioStatus ;
    short   nBytes ;
    long    buf;
} ;  

#ifndef	FD_SETSIZE
#define	FD_SETSIZE	32
#endif

/* Local Includes for HYPERSCRIPT */
#include "typdef.h"	/* Typdef's */
#include "hypdef.h"	/* HyperScript specific */

/* External Global variables used HYPERSCRIPT */

/* File names and other string variables */
extern char		gzLogName[MAX_PATH_SIZE+1] ;
extern FILE		*gsLog ;	/* Log */

/* Socket channels */
extern int		giServicePort ;	/* Service Channel id */
extern int		giARservicePort ;	/* Service Channel id */
extern SOCKET		giLoopback ;		/* Loopback socket */
extern sBYTE		giLoopbackType ;	/* Loopback socket type */
extern HANDLE		giInboxLoopback ;	/* Loopback socket to inbox */
extern SOCKET		giListenLoopback_R ;	/* Loopback socket (Read) to listen socket */
extern SOCKET		giListenLoopback_W ;	/* Loopback socket (Write) to listen socket */

/* For resolving address */
extern sLOGICAL		giResolveFlag1 ;
extern sLOGICAL		giResolveFlag2 ;
extern char		gzResolveAddr[HOST_SIZE+1];

/* Run-time variables */
extern int		giPID ;		/* PID of process */
extern char		gzPIDfileSpec[MAX_PATH_SIZE+1] ; /* PID file */
extern time_t		gsCurTime ; 	/* Current time, updated regularily. */
extern sLOGICAL 	guTimeStamp ;	/* Integer timeStamp. */
extern unsigned short	guRunFlags ; 	/* General flags */
extern unsigned short	guHeapMultiplier ;

extern sInstance *gpAI ;	/* HyperScript instance */
extern sInstance *gpAImain  ;	/* Main HyperScript instance */

/* Environment variables */
extern char    		gzAUTOROUTER[OBJECT_SIZE+1];	/* AUTOROUTER root name "router" */
extern char    		gzAUTOHOST[OBJECT_SIZE+1];	/* AUTOHOST root name "hostname" */
extern char    		gzAUTOFIFO[MAX_PATH_SIZE+1];	/* AUTOFIFO path */
extern char    		gzAUTOBIN[MAX_PATH_SIZE+1];	/* AUTOBIN path */
extern char    		gzAUTORUN[MAX_PATH_SIZE+1];	/* AUTORUN path */
extern char    		gzAUTOLOG[MAX_PATH_SIZE+1];	/* AUTOLOG path */
extern char    		gzAUTOSPOOL[MAX_PATH_SIZE+1];	/* AUTOLOG path */
extern char		gzInstance[INSTANCE_SIZE+1] ;
extern char		gzConcept[TARGET_SIZE+1] ;
extern char		gzConceptPath[MAX_PATH_SIZE+1] ;
extern char		gzParent[MAX_PATH_SIZE+1] ;
extern char		gzRoot[MAX_PATH_SIZE+1] ;
extern char		gzInboxPath[MAX_PATH_SIZE+1] ;
extern char		gzOutboxPath[MAX_PATH_SIZE+1] ;
extern char		gzLocalHost[MAX_PATH_SIZE+1] ;
extern char		gzLocalAddr[MAX_PATH_SIZE+1] ;

/* HyperScript code variables */
extern sData*		gpsTempData ;	/* If defined, its a leak */
extern sData*		gpsTempData2 ;	/* If defined, its a leak */
extern int		giMaxExprSize ; /* Heap allocation for expression and stack */
extern int		giMaxStackDepth ; /* Heap allocation for stack */
extern int		giMaxFrameDepth ; /* Heap allocation for frame */
extern sConcept*       	gpsConcept ;    /* Pointer to concept */
extern unsigned short	guDebugFlags ; 	/* Debug flags */
extern unsigned		guErrorCount ;	/* HyperScript error count */
extern int		giLineCount ;
extern int		giProgramCount ;
extern int		giCondition ;   /* Return condition */
extern sLOGICAL		giJmpEnabled ;  /* If longjmp is enabled from handlers*/
extern SOCKET           gsSocketToCancel ;/* Should we want to cancel I/O */
extern int		giJmpLevel ;    /* Current depth of saved jmp env */
extern jmp_buf		gsJmpStack[MAX_JMP_LEVEL+1] ; /* For setjmp,longjmp */ 
extern jmp_buf		gsJmpOverride ;  /* Jump Override */
extern sLOGICAL		giJmpOverride ;  /* If longjmp override should be done */
extern FILE*		gsPP ;		/* Hyperscript input stream */
extern sAImsg*		gpsAImsg  ;	/* Working message structure */

/* Variables for the gHyp_util_getopt function call - in util.c */
extern int		giOpterr ;	/* Useless, never set or used */
extern int		giOptind ;	/* Index into parent argv vector */
extern int		giOptopt ;	/* Character checked for validity */
extern char		*gzOptarg ;	/* Argument associated with option */

/* Global signal flags */
extern unsigned		guSIGIO ;      	/* Set when SIGIO */
extern unsigned		guSIGPIPE ;    	/* Set when SIGPIPE */
extern unsigned		guSIGALRM ;    	/* Set when SIGALRM */
extern unsigned		guSIGINT ;     	/* Set when SIGINT */
extern unsigned		guSIGTERM ;    	/* Set when SIGTERM */
extern unsigned		guSIGHUP ;     	/* Set when SIGHUP */
extern unsigned		guSIGMBX ;     	/* Set when SIGMBX */
extern unsigned		guSIGMSG ;	/* Set when SIGMSG */

/* Function prototypes */
#include "aimsg.h"
#include "data.h"
#include "channel.h"
#include "hash.h"
#include "util.h"
#include "router.h"
#include "sock.h"
#include "signl.h"
#include "tcp.h"
#include "branch.h"
#include "concept.h" 
#include "cgi.h"
#include "dateparse.h"
#include "env.h"
#include "fileio.h"
#include "frame.h"
#include "function.h"
#include "hyp.h"
#include "gd.h"
#include "instance.h"
#include "label.h"
#include "load.h"
#include "method.h"
#include "operand.h"
#include "operator.h"
#include "promis.h"
#include "parse.h"
#include "hsms.h"
#include "secs.h"
#include "secs1.h"
#include "secs2.h"
#include "route.h"
#include "port.h"
#include "http.h"
#include "sort.h"

#ifdef AS_SSL
#include "ssl.h"
#endif

#include "stack.h"
#include "stmt.h"
#include "system.h"

/* Don't make sql functions an option anymore */
#include "sql.h"

#ifdef AS_MAPI
#include "mapi.h"
#endif

#include "type.h"

