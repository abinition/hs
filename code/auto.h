/* auto.h 
 *
 * Modifications:
 *
 * $Log: auto.h,v $
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

#if defined ( WIN32 ) || defined ( __WIN32 )
#define AS_WINDOWS 1
#include <windows.h>
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
#include <stdlib.h>	/* standard library functions */
#include <time.h> 	/* for localtime function */
#include <ctype.h>	/* is* macros and functions */
#include <string.h>	/* string functions */
#include <errno.h>	/* errno */
#include <assert.h>     /* assert function */
#include <signal.h>	/* signal processing functions and structures */
#include <setjmp.h>	/* setjmp and longjmp functions */

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
    long		sl ;
    unsigned long	ul ;
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

/* Environment variables */
extern char    		gzAUTOROUTER[OBJECT_SIZE+1];	/* AUTOROUTER root name "router" */
extern char    		gzAUTOFIFO[MAX_PATH_SIZE+1];	/* AUTOFIFO path */
extern char    		gzAUTOBIN[MAX_PATH_SIZE+1];	/* AUTOBIN path */
extern char    		gzAUTORUN[MAX_PATH_SIZE+1];	/* AUTORUN path */
extern char    		gzAUTOLOG[MAX_PATH_SIZE+1];	/* AUTOLOG path */
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
#include "ssl.h"
#include "stack.h"
#include "stmt.h"
#include "system.h"
#ifdef AS_SQL
#include "sql.h"
#endif
#include "type.h"
