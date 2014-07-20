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
 *	$Log: util.c,v $
 *	Revision 1.78  2011-02-24 03:12:55  bergsma
 *	Fix bug in gHyp_util_readStream
 *	
 *	Revision 1.77  2011-01-08 21:32:40  bergsma
 *	Added " and ' for URL decoding/encoding
 *	
 *	Revision 1.76  2010-05-05 04:56:19  bergsma
 *	Added CGI form-data support
 *	
 *	Revision 1.75  2010-04-23 05:17:46  bergsma
 *	Added ? and , for URLencoding/decoding.
 *	
 *	Revision 1.74  2010-03-17 08:17:13  bergsma
 *	Trim whitespace leaves \n alone.
 *	
 *	Revision 1.73  2010-02-01 22:27:18  bergsma
 *	Builld 100130, HS 3.9.0
 *	
 *	Revision 1.72  2010-01-16 18:34:11  bergsma
 *	Increase input stream for XML.
 *	
 *	Revision 1.71  2010-01-15 08:30:42  bergsma
 *	Increase XML input buffers for parsing
 *	
 *	Revision 1.70  2010-01-08 02:44:57  bergsma
 *	Added ssl_md5(), enhanced ssl_digest.
 *	Fixed urldecode, missing ":"
 *	Enabled object calls, ie:  text.strtok( ) and the like...
 *	
 *	Revision 1.69  2009-12-24 15:56:55  bergsma
 *	More fixes for handling form data via HTTP
 *	
 *	Revision 1.68  2009-12-19 05:54:21  bergsma
 *	Handle upload of form data
 *	
 *	Revision 1.67  2009-09-28 05:24:00  bergsma
 *	For ORACLE, like SQLSERVER, uses double quotes for escaping a quote.
 *	
 *	Revision 1.66  2009-09-21 05:14:24  bergsma
 *	Fix conversion of XML escape sequences
 *	
 *	Revision 1.65  2009-04-09 19:57:21  bergsma
 *	Fixing issues with HTTP and buffer sizing
 *	
 *	Revision 1.64  2009-03-13 07:48:16  bergsma
 *	GD refinements.
 *	Added BUILD_VERSION
 *	
 *	Revision 1.63  2008-09-09 13:48:48  bergsma
 *	Make gzStream larger, 4* MAX_INPUT_LENGHT.  Allows bigger XML
 *	structures to be parsed (large chrt records from PROMIS).
 *	
 *	Revision 1.62  2008-08-28 14:21:57  bergsma
 *	In breakString, avoid breaking lines ending with backslash
 *	
 *	Revision 1.61  2008-08-21 11:10:32  bergsma
 *	Fix problems where neg value to 'isprint' causes traceback
 *	
 *	Revision 1.60  2008-06-28 19:33:56  bergsma
 *	Mix memory overflow bug with util_parseXML
 *	
 *	Revision 1.59  2008-06-28 00:54:55  bergsma
 *	Make XML buffer larger, don't use strlen in case there's no \0
 *	
 *	Revision 1.58  2008-06-07 02:49:50  bergsma
 *	Added gHyp_util_breakTokenForMsg
 *	
 *	Revision 1.57  2008-05-30 01:22:33  bergsma
 *	Test for negative character value before doing isprint or isspace
 *	
 *	Revision 1.56  2008-05-17 02:32:12  bergsma
 *	In breakString, use a 'work' temp variable to avoid buffers that
 *	have no NULL for an ending.
 *	
 *	Revision 1.55  2008-05-12 17:38:08  bergsma
 *	Error in unparse, buffer overwrite.
 *	
 *	Revision 1.54  2008-02-12 23:13:16  bergsma
 *	Bug dealing with printing &#ooooo numbers
 *	
 *	Revision 1.53  2007-09-03 06:22:27  bergsma
 *	Tickcount sometimes is a neg number?  Fix attempts to correct
 *	
 *	Revision 1.52  2007-06-20 05:18:24  bergsma
 *	util_trim and util_trim_whiitespace need type coversion on return values
 *	
 *	Revision 1.51  2007-06-16 18:20:29  bergsma
 *	Wrong lenght returned for gHyp_util_trim and trimWhitespace
 *	
 *	Revision 1.50  2007-06-16 17:55:51  bergsma
 *	Make util_trim and util_trimWhitespace return length of string
 *	
 *	Revision 1.49  2007-04-07 23:25:00  bergsma
 *	Take modulus 1000 from getclock milliseconds and WIN32 GetTickCount
 *	already returns milliseconds.
 *	
 *	Revision 1.48  2007-04-07 17:49:22  bergsma
 *	When displaying clock milliseconds, used %03d format.
 *	
 *	Revision 1.47  2007-03-26 00:04:03  bergsma
 *	getclock for windows was not returning milliseconds
 *	
 *	Revision 1.46  2007-03-22 04:28:44  bergsma
 *	Make getclock() function more portable
 *	
 *	Revision 1.45  2007-03-19 05:32:08  bergsma
 *	New functions: min, max, pow, hypot, abs, acos, asin, atan,  ceil, cos,
 *	 floor, log10, logN, sin, sqrt, tan, exp printf, sprintf, fprintf
 *	
 *	Revision 1.44  2007-03-15 01:07:50  bergsma
 *	Bug with parsing &#xx; tokens.
 *	Added getclock function, millisecond timer in debug statements.
 *	
 *	Revision 1.43  2006-08-28 21:03:35  bergsma
 *	Added XML &copy;
 *	
 *	Revision 1.42  2006/07/20 17:46:27  bergsma
 *	Added &laquo; and &raquo;
 *	
 *	Revision 1.41  2006/07/17 16:41:59  bergsma
 *	Bug fixed, when decoding BASE 64, last char is not output when input
 *	ends with 2 pad characters
 *	
 *	Revision 1.40  2005/12/10 00:30:30  bergsma
 *	HS 3.6.5
 *	
 *	Revision 1.39  2005/12/08 03:06:15  bergsma
 *	for sql_toexternal, other than PGSQL, replace \nnn with \\ where nnn > 127
 *	
 *	Revision 1.38  2005/09/12 00:49:25  bergsma
 *	Support log() function for DLL
 *	
 *	Revision 1.37  2005/09/08 08:55:54  bergsma
 *	Postgres DEC MCS to Unicode Handling
 *	
 *	Revision 1.36  2005/09/06 12:59:29  bergsma
 *	Postgres UNICODE support
 *	
 *	Revision 1.35  2005/09/06 08:31:01  bergsma
 *	PostgreSQL escape sequence handling
 *	
 *	Revision 1.32  2005/06/12 16:46:22  bergsma
 *	HS 3.6.1
 *	
 *	Revision 1.31  2005/05/10 17:38:45  bergsma
 *	Names are urlEncode and urlDecode
 *	
 *	Revision 1.30  2005/05/10 17:32:38  bergsma
 *	Added URLencode and URLdecode functions
 *	
 *	Revision 1.29  2005/04/13 13:45:55  bergsma
 *	HS 3.5.6
 *	Added sql_toexternal.
 *	Fixed handling of strings ending with bs (odd/even number of backslashes)
 *	Better handling of exception condition.
 *	
 *	Revision 1.28  2005/03/30 04:05:59  bergsma
 *	Backslash should be externalized in unparsestring
 *	
 *	Revision 1.27  2005/02/15 06:58:15  bergsma
 *	No newline at end of file
 *	
 *	Revision 1.26  2005/01/10 18:02:34  bergsma
 *	Comment change only.
 *	
 *	Revision 1.25  2004/12/17 17:41:17  jbergsma
 *	Output to the ListBox window in the ATL (WebPickle) version
 *	
 *	Revision 1.24  2004/12/13 04:52:50  bergsma
 *	Don't null terminate string processes by util_parseString, because in
 *	aimsg.c, strtok will have unpredicatable results.
 *	
 *	Revision 1.23  2004/11/19 03:42:26  bergsma
 *	Don't interpret \u or \x as unicode or hex, digits are required.
 *	
 *	Revision 1.22  2004/11/02 23:00:24  bergsma
 *	(char*) mismatch with (sBYTE*)
 *	
 *	Revision 1.21  2004/10/27 18:24:06  bergsma
 *	HS 3.3.2
 *	1. Fix bug with SQL read when str size is > 512 bytes, use _data_ blobs
 *	2. Fix bug with XML output, forgetting ";" in unicode output.
 *	3. In |TOKEN|VALUE|VALUE| part of message, use unparse on TOKEN
 *	as well as VALUE.
 *	4. Add utility function util_breakStream.
 *	
 *	Revision 1.20  2004/10/16 05:12:22  bergsma
 *	Added gHyp_util_readStream
 *	
 *	Revision 1.19  2004/07/23 18:41:37  bergsma
 *	fixed bug in util_uparseString where '\0' was not be externalized after change to toexternal()
 *	
 *	Revision 1.18  2004/07/01 02:02:34  bergsma
 *	Added 'specialChars' argument to the toexternal() function.
 *	
 *	Revision 1.17  2004/06/13 14:07:23  bergsma
 *	Provide output to JAVA console when hooked through JNI
 *	
 *	Revision 1.16  2004/05/15 02:07:25  bergsma
 *	HTML unicode is &#ddddd; where ddddd is 5 digit decimal
 *	
 *	Revision 1.15  2004/04/29 01:58:47  bergsma
 *	Added encode and decode functions for BASE64
 *	
 *	Revision 1.14  2003/07/27 01:00:09  bergsma
 *	Add support for \u unicode format in hex notation
 *	
 *	Revision 1.13  2003/06/05 21:54:23  bergsma
 *	Added new typedef 'unicode', UTF-16.
 *	Required for new unicode type:
 *	1. New data type in data object
 *	2. Special unicode output for xdescribe function
 *	3. New TYPE_UNICODE in hypdef.h
 *	4. New token 0uNNNNN format, like 0xNNNNN and 0oNNNNN
 *	5. New function unicode()
 *	6. New SECS II data type.
 *	
 *	Revision 1.12  2003/04/30 22:56:38  bergsma
 *	The 'dotMask' variable was not present in lHyp_util_getToken for
 *	the case where an identifier is continued.  This bug fix corrects the
 *	problems whereby a message containing something like ||TOKEN.NAME|VALUE||
 *	was not being interpreted as 'TOKEN.NAME' = "VALUE" ;
 *	
 *	Revision 1.11  2003/04/23 22:55:07  bergsma
 *	1. Needed to separate MAX_OUTPUT_LENGTH from PROMIS restriction of
 *	MAX_TERMINAL_LENGTH, which can only be 256.  This limitation caused
 *	problems in code where MAX_OUTPUT_LENGTH was used.
 *	2. When a character value is above ~, then consider this "unprintable".
 *	
 *	Revision 1.10  2003/04/04 15:59:47  bergsma
 *	Added 'n' count to strip function.
 *	Added getToken3 function, used in env_mergeData, for message arguments.
 *	
 *	Revision 1.9  2003/01/20 18:23:08  bergsma
 *	Undo change for unparsing double quote for messages.
 *	
 *	Revision 1.8  2003/01/09 00:18:29  bergsma
 *	V3.1.0
 *	1. Use gzInstance for logging.
 *	2. When unparsing a string for a message, double quote does not need \"
 *	3. Rework gHyp_util_breakTarget, supporting a more compact form of
 *	a hyperscript instance#concept[/parent]@node address.
 *	
 *	Revision 1.7  2002/11/29 19:49:23  bergsma
 *	In gHyp_util_unparseString, do not expand ' ' and \t into &nbsp; and &tab;
 *	for XML unparsing, it is not required.
 *	
 *	Revision 1.6  2002/11/22 00:04:27  bergsma
 *	In gHyp_util_unparseString, if '|' is encountered, it was being ignored
 *	if the unparsing was not for a message.
 *	
 *	Revision 1.3  2002/09/16 18:02:53  bergsma
 *	In gHyp_util_unparseString, a "|" is converted to "\174", not "174".
 *	
 *	Revision 1.2  2002/09/04 23:20:26  bergsma
 *	In gHyp_util_parseXML, recognition of &#nn; escape sequences is now supported.
 *		
 *
 */

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ***********************/
#include "auto.h"       /* System Interface and Function Prototypes */

#include <sys/stat.h>   /* file attributes */

/* For O_CREAT, O_APPEND, ... parameters */
#ifdef AS_VMS
#include <sys/file.h>
#endif

#if !defined( AS_VAXC ) && !defined ( AS_WINDOWS )
#include <sys/fcntl.h>
#endif

#ifdef AS_SOLARIS
#include <sys/systeminfo.h>
#endif

#ifdef AS_VMS
#include <ssdef>
#include <dvidef>
#include <jpidef>
#endif

#ifndef AS_WINDOWS
#include <pwd.h>
#endif

#ifdef AS_ATL
#include <interface.h>
#endif

/********************** EXTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL GLOBAL VARIABLES ****************************/

static  sLOGICAL        guIsTracing = FALSE ;
static  char            gzPrintf[MAX_TERMINAL_LENGTH+1] ;
static  char            gzPreBuf[MAX_TERMINAL_LENGTH+1] ;
static  char            gzBuf[MAX_TERMINAL_LENGTH+1] ;
static  char            *gpEndBuf = gzBuf + MAX_TERMINAL_LENGTH-1 ;

#define PAD '='
#define BS0 (  ibuf[0] >> 2)
#define BS1 ( (ibuf[0] & 3 ) << 4 | ibuf[1] >> 4)
#define BS2 ( (ibuf[1] & 15) << 2 | ibuf[2] >> 6)
#define BS3 (  ibuf[2] & 63)

static const char gzBin2base[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/********************** FUNCTION DEFINITIONS ********************************/

#ifdef AS_VMS
#ifdef __cplusplus
extern "C" int sys$getmsg  (unsigned int msgid, unsigned short int
                         *msglen, void *bufadr, unsigned int flags,
                         unsigned char outadr [4]);
extern "C" int sys$getdviw  (unsigned int efn, unsigned short int chan,
                          void *devnam, void *itmlst, short* iosb,
                          void (*astadr)(__unknown_params), int
                          astprm, unsigned __int64 *nullarg);
extern "C" int sys$getjpiw  (unsigned int efn, unsigned int *pidadr, void
                          *prcnam, void *itmlst, short *iosb,
                          void (*astadr)(__unknown_params), int astprm);
extern "C" int sys$setprn  ( void *prcnam);
extern "C" void tut_output ( sDescr* ) ;
#else
extern int sys$getmsg  (unsigned int msgid, unsigned short int
                         *msglen, void *bufadr, unsigned int flags,
                         unsigned char outadr [4]);
extern int sys$getdviw  (unsigned int efn, unsigned short int chan,
                          void *devnam, void *itmlst, short* iosb,
                          void (*astadr)(__unknown_params), int
                          astprm, unsigned __int64 *nullarg);
extern int sys$getjpiw  (unsigned int efn, unsigned int *pidadr, void
                          *prcnam, void *itmlst, short *iosb,
                          void (*astadr)(__unknown_params), int astprm);
extern int sys$setprn  ( void *prcnam);
extern void tut_output ( sDescr* ) ;
#endif

#else

#ifndef AS_WINDOWS
extern uid_t getuid ( void );
extern struct passwd * getpwuid ( uid_t ) ;          
/*extern int gettimeofday(struct timeval *tp, struct timezone *tzp);*/
#endif

#endif

#ifdef AS_JNI
extern void gHyp_hs_output ( char* token ) ;
#endif

int gHyp_util_getopt ( int nargc, char **nargv, char *ostr )
{

  /* Description:
   *
   *    Get option letter from argument vector.
   *    This is a private version of 'getopt' call, because VMS does not 
   *    have it. This code was found from examples returned from an Archie 
   *    search.
   *
   * Arguments:
   *
   *    nargc                                           [R]
   *    - number of arguments
   *
   *    nargv                                           [R]
   *    - pointer to array of pointers to arguments
   *
   *    ostr                                            [R]
   *    - options string to parse
   *
   * Return value:
   *
   *    Returns character of option letter just processed.
   *            gzOptArg
   *    Returns EOF when done.
   *
   */

#define BADCH   (int)'?'
#define EMSG    ""
#define tell(s) fputs(*nargv,stderr);fputs(s,stderr); \
                fputc(giOptopt,stderr);fputc('\n',stderr);return(BADCH);
  static char   *place = EMSG;  /* option letter processing */
  register char *oli;           /* option letter list index */
  
  if( !*place ) {
  
    /* update scanning pointer */
    if( giOptind >= nargc || *(place = nargv[giOptind]) != '-' || !*++place ) 
      return (EOF);

    if (*place == '-') {
      /* found "--" */
      ++giOptind;
      return(EOF);
    }
  }
  
  /* option letter okay? */
  if ((giOptopt = (int)*place++) == (int)':' || !(oli = strchr(ostr,giOptopt))) {
    if (!*place) ++giOptind;
    tell (": illegal option -- ") ;
  }
  
  if (*++oli != ':') {          /* don't need argument */
    gzOptarg = NULL;
    if (!*place) ++giOptind;
  }
  else {        
    /* need an argument */
    if (*place) 
      gzOptarg = place; /* no white space */
    else if (nargc <= ++giOptind) {
      /* no arg */
      place = EMSG;
      tell (": option requires an argument -- ");
    }
    else 
      gzOptarg = nargv[giOptind];       /* white space */
    
    place = EMSG;
    ++giOptind;
  }
  return(giOptopt);                     /* dump back option letter */
}

void gHyp_util_upperCase ( char *pStr, int n )
{
  /* Description:
   *
   *    Converts a string to uppercase.
   *
   * Arguments:
   *
   *    pStr                            [R]
   *    - pointer to null terminated string
   *
   * Return value:
   *
   *    none
   *
   */
  int 
    i ;
  for (i=0; i<n; i++ ) pStr[i] = toupper ( pStr[i] ) ;
}

void gHyp_util_lowerCase ( char *pStr, int n )
{
  /* Description:
   *
   *    Converts a string to lowercase.
   *
   * Arguments:
   *
   *    pStr                            [R]
   *    - pointer to null terminated string
   *
   * Return value:
   *
   *    none
   *
   */
  int 
    i ;
  for (i=0; i<n; i++ ) pStr[i] = tolower ( pStr[i] ) ;
}

char* gHyp_util_timeStamp ( time_t timeStamp )
{
  /* Description:
   *
   *    Generates an ascii formatted timestamp.
   *    Format is YYYYMMDD:HHMMSSCC
   *
   * Arguments:
   *
   *    timeStamp                               [R]
   *    - internal time  
   *
   * Return value:
   *
   *    Formated ascii timestamp.
   *
   */
  static char     gzTimeStamp[DATETIME_SIZE+1];   /* Character timestamp */
  struct tm       *pstm ;               /* Data and Time components */
  
  /*
  struct timeval  tv ;   
  struct timezone tz ;
  gettimeofday ( &tv, &tz ) ;
  */ 
  pstm = localtime ( &timeStamp ) ;

  sprintf (     gzTimeStamp, 
                "%04d%02d%02d:%02d%02d%02d", 
                pstm->tm_year+1900, pstm->tm_mon+1, pstm->tm_mday,
                pstm->tm_hour, pstm->tm_min, pstm->tm_sec  ) ;
  return gzTimeStamp ;
}

char* gHyp_util_random8 ()
{
  /* Description:
   *
   *    Generates an ascii formatted random 8 character hex number.
   *
   * Arguments:
   *
   *
   * Return value:
   *
   *    Random 8 character hex value.
   *
   *
   */
  static int    seed = 0 ;
  static char   gzRandom[8+1];

  if ( seed == 0 ) {
    seed = (int) time ( NULL ) ;
#if defined ( AS_VMS ) || defined ( AS_WINDOWS )
    srand ( seed ) ;
#else
    srandom ( seed ) ;
#endif
  }

#if defined ( AS_VMS ) || defined ( AS_WINDOWS )
  sprintf ( gzRandom, "%08lx", (unsigned long) rand() | ((unsigned long) rand() << 16) ) ;
#else
  sprintf ( gzRandom, "%08lx", (unsigned long) random() ) ;
#endif

  return gzRandom ;
}

static void lHyp_util_out ( char *pStr )
{
  /* Description:
   *
   *    Low-level internal routine to output an ascii string to 'gsLog'
   *
   * Arguments:
   *
   *    pStr                            [R]
   *    - pointer to string to be written to gsLog
   *
   * Return value:
   *
   *    none
   *
   */

#ifdef AS_PROMIS

    if ( gzLogName[0] ) {
      fputs ( pStr, gsLog ) ;
      fflush ( gsLog ) ; 
      fsync ( fileno ( gsLog ) ) ;
    }
    else {
      makeDSCz ( str_d, pStr ) ;
      str_d.dsc_w_length -= 1 ; /* Don't include newline */
      str_d.dsc_w_length = MIN ( MAX_TERMINAL_LENGTH, str_d.dsc_w_length ) ;
      tut_output ( &str_d ) ;
    }
#elif defined ( AS_JNI )

    if ( gzLogName[0] ) {
      fputs ( pStr, gsLog ) ;
      fflush ( gsLog ) ; 
    }
    else
      gHyp_hs_output ( pStr ) ;

#elif defined ( AS_ATL )
    if ( gzLogName[0] ) {
      fputs ( pStr, gsLog ) ;
      fflush ( gsLog ) ; 
    }
    else
      gHyp_hs_output_listbox ( pStr );
#else

    fputs ( pStr, gsLog ) ;
    fflush ( gsLog ) ; 
#ifdef AS_VMS
    if ( gzLogName[0] ) fsync ( fileno ( gsLog ) ) ;
#endif

#endif

    return ;
}

static void lHyp_util_output ( char *pStr )
{
  /* Description:
   *
   *    Low-level externally callable to output an ascii string.
   *    The guIsTracing variable is used by HyperScript to trace
   *    execution of the code - if this routine is called while tracing
   *    is on, then it is turned off by this routine.
   *
   * Arguments:
   *
   *    pStr                            [R]
   *    - pointer to string to be output
   *
   * Return value:
   *
   *    none
   *
   */  
  if ( guIsTracing ) {
    lHyp_util_out ( "\n" ) ;
    /*lHyp_util_out ( "--Trace Off--\n" ) ;*/
    guIsTracing = FALSE ;
  }
  lHyp_util_out ( pStr ) ;
}

void gHyp_util_output ( char *pStr )
{
  if ( guRunFlags & RUN_QUIET ) return ;
  lHyp_util_out ( pStr ) ;
}

void gHyp_util_output2 ( char *pStr )
{
  lHyp_util_out ( pStr ) ;
}

void gHyp_util_trace ( char *pStr )
{
  /* Description:
   *
   *    Implements the HyperScript tracing algorithm, for what its worth.
   *    Based on the token passed, the algorithm may indent the line.
   *
   * Arguments:
   *
   *    pStr                            [R]
   *    - pointer to token string to be "traced"
   *
   * Return value:
   *
   *    none
   *
   */

#define  MAX_INDENT 20

  static sBYTE  indentDepth = 0 ;
  static char   indentStr[MAX_TERMINAL_LENGTH+1] ;
  
  int           i ;
  
  /* Execution tracing */
  
  if ( !guIsTracing ) {
    /* Turn it on */
    guIsTracing = TRUE ;
    /*lHyp_util_out ( "--Trace On---\n" ) ;*/
    indentDepth = 0 ;
    indentStr[0] = '\0' ;
  }
  
  if ( *pStr == ';' ) {

    /* End of statement, start a new line */
    lHyp_util_out ( pStr ) ;
    lHyp_util_out ( "\n" ) ;

    /* Output the indentation string for the next line */
    if ( indentStr[0] ) lHyp_util_out ( indentStr ) ;
  }
  
  else if ( *pStr == '{' ) {

    /* Start of indented statement block. */
    lHyp_util_out ( pStr ) ;
    lHyp_util_out ( "\n" ) ;

    /* Increase indentation by one */
    if ( indentDepth < MAX_INDENT ) indentDepth++ ;
    for ( i=0; i<indentDepth; i++ ) indentStr[i] = ' ' ;
    indentStr[indentDepth] = '\0' ;
    if ( indentStr[0] ) lHyp_util_out ( indentStr ) ;
  }
  
  else if ( *pStr == '}' ) {

    /* End of indented statement block. */
    lHyp_util_out ( "\n" ) ;

    /* Decrease indentation by one */
    if ( indentDepth > 0 ) indentDepth-- ;
    for ( i=0; i<indentDepth; i++ ) indentStr[i] = ' ' ;
    indentStr[indentDepth] = '\0' ;
    if ( indentStr[0] ) lHyp_util_out ( indentStr ) ;
    lHyp_util_out ( pStr ) ;
  }
  
  else
    lHyp_util_out ( pStr ) ;
}

sLOGICAL gHyp_util_log ( char *pStr )
{
  /* Description:
   *
   *    Log a string, with possibly a timestamp prefix.
   *    If the string is longer than MAX_TERMINAL_LENGTH, then
   *    a newline is inserted at that point and the output
   *    is wrapped onto the next line(s).
   *
   * Arguments:
   *
   *    pStr                            [R]
   *    - pointer to null terminated string
   *
   * Return value:
   *
   *    Returns TRUE.
   *
   */

  int   len ;
  int	i ;
  char  *pBuf = gzBuf,
        *pEndStr ;
  
  /* Add timestamp if enabled */
  if ( guTimeStamp ) {
    gsCurTime = time ( NULL ) ;
    if ( gzInstance[0] ) 
      sprintf ( gzBuf, 
                "%s:%s:", 
                gHyp_util_timeStamp( gsCurTime ),
                gzInstance ) ;
    else
      sprintf ( gzBuf,
                "%s:",
                gHyp_util_timeStamp( gsCurTime ) ) ;
    pBuf += strlen ( gzBuf ) ;
  }

  do {

    /* Get remaining length of string. 
     * Determine shorter of string length and buffer size.
     */
    len = strlen ( pStr ) ;
    pEndStr = pStr + len ;
    len = MIN ( len, gpEndBuf-pBuf ) ;

    strncpy ( pBuf, pStr, len ) ;

    /* Replace non-printing other than \n\, \r, \t with just a space */
    for ( i=0; i<len; i++ ) if ( pBuf[i]<=0 || (!isprint (pBuf[i]) && !isspace(pBuf[i])) ) pBuf[i] = ' ' ;

    /* Point to last character in string copied.
     * If its not a newline, add one.
     * Null terminate and output.
     */
    pStr += len-1 ;
    if ( *pStr++ != '\n' ) pBuf[len++] = '\n' ;
    pBuf[len++] = '\0' ;
    lHyp_util_output ( gzBuf ) ;

    /* Reset in case there's more */    
    pBuf = gzBuf ;
  }
  while ( pStr < pEndStr ) ;
  
  return TRUE ;
}

sLOGICAL gHyp_util_logInfo ( const char *fmt, ... )
{
  /* Description:
   *
   *    Log a string with variable formatted argument list.
   *
   * Arguments:
   *
   *    fmt                             [R]
   *    - null terminated format string (see sprintf)
   *
   *    ...                             [R]
   *    - values for format arguments
   *
   * Return value:
   *
   *    Returns TRUE
   *
   */
  va_list
    ap ;

  if ( guRunFlags & RUN_QUIET ) return TRUE ;

  va_start ( ap, fmt ) ;
  vsprintf ( gzPreBuf, fmt, ap ) ;
  va_end ( ap ) ;
  
  gHyp_util_log ( gzPreBuf ) ;

  return TRUE ;
}


char* gHyp_util_sprintf ( const char *fmt, ... )
{
  /* Description:
   */
  va_list
    ap ;

  va_start ( ap, fmt ) ;
  vsprintf ( gzPrintf, fmt, ap ) ;
  va_end ( ap ) ;

  return gzPrintf ;
}

sLOGICAL gHyp_util_debug ( const char *fmt, ... )
{
  /* Description:
   */
  va_list
    ap ;

  va_start ( ap, fmt ) ;
  vsprintf ( gzPreBuf, fmt, ap ) ;
  va_end ( ap ) ;
  
  gHyp_util_log ( gzPreBuf ) ;

  return TRUE ;
}

int gHyp_util_getclock ()
{
  unsigned int
      tickcount ;

#ifndef AS_WINDOWS
#ifdef AS_TRU64
  struct timespec tp ;
#else
  struct timeval tval ;
  struct timezone *tz=NULL ;
#endif
#endif

#ifndef AS_WINDOWS

#ifdef AS_TRU64

  getclock( 1, &tp );
  tickcount = tp.tv_nsec / 1000000 ;        /* convert to milliseconds */

#else

  memset ( (char*) &tval, 0, sizeof ( struct timeval ) ) ;
  /*timerclear(&tval);*/
  gettimeofday(&tval, tz) ; 
  tickcount = tval.tv_usec/1000 ;

#endif

#else
  tickcount = GetTickCount() ;	  /* already in milliseconds */
#endif

  /* Get the remainder from 1000 */
tickcount = tickcount % 1000 ;
return tickcount ; 
}

sLOGICAL gHyp_util_logDebug (   int newFrameDepth, 
                                unsigned short flag,
                                const char *fmt, ... )
{
  /* Description:
   *
   *    Special log for HyperScript debugging.
   *
   * Arguments:
   *
   *    newFrameDepth                   [R]
   *    - new frame depth of HyperScript,
   *      this determines the level of indentation in the log
   *
   *    flag                    [R]
   *    - the debug flag level being display
   *
   *    fmt                             [R]
   *    - null terminated format string (see sprintf)
   *
   *    ...                             [R]
   *    - values for format arguments
   *
   * Return value:
   *
   *    Returns TRUE
   *
   */

  static  int           
    frameDepth = 0 ;

#define         FRAME_DEPTH2    70

  static  char
    frameLevelStr[FRAME_DEPTH2+2] = 
    " | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | " ;

  va_list
    ap ;

  int
    nfd ;

  /* Only change the 'frame depth' prefix string when requested by
   * the newFrameDepth argument.  When positive, the frame depth
   * is in at execution-enabled level.  When negative, execution is
   * currently disabled for that level.
   */
  if ( newFrameDepth != FRAME_DEPTH_NULL ) {

    /* New framedepth. Adjust frameLevelString. */

    nfd = MIN ( ABS ( newFrameDepth*2 ), FRAME_DEPTH2 ) ;

    if ( newFrameDepth < 0 )
      /* Put a star at the new depth. */
      frameLevelStr[nfd] = '*' ;
    else
      /* Put a spave at the new depth. */
      frameLevelStr[nfd] = ' ' ;
         
    /* Erase old position when poping back the frame. */
    if ( nfd < frameDepth ) frameLevelStr[frameDepth] = ' ' ;

    /* Save new frameDepth */
    frameDepth = nfd ;
  }
  
  sprintf ( gzPreBuf, "%%DBG%03d|%03d|%.*s", flag, gHyp_util_getclock(), frameDepth+2, frameLevelStr ) ;

  va_start ( ap, fmt ) ;
  vsprintf ( gzPreBuf+strlen(gzPreBuf), fmt, ap ) ;
  va_end ( ap ) ;
  
  gHyp_util_log ( gzPreBuf ) ;
  
  return FALSE ;
}

sLOGICAL gHyp_util_logError ( const char *fmt, ... )
{
  /* Description:
   *
   *    Log an error.
   *    This routine is just like _logInfo but adds "%ERROR"
   *    to the message and returns FALSE instead of TRUE.
   *
   * Arguments:
   *
   *    fmt                             [R]
   *    - null terminated format string (see sprintf)
   *
   *    ...                             [R]
   *    - values for format arguments
   *
   * Return value:
   *
   *    Returns FALSE.
   */
  va_list       ap ;
  unsigned int i ;
  
  strcpy ( gzPreBuf, "%ERROR: " ) ; 
  
  va_start ( ap, fmt ) ;
  vsprintf ( gzPreBuf+strlen(gzPreBuf), fmt, ap ) ;
  va_end ( ap ) ;
  
  /* Replace non-printing just a space */
  for ( i=0; i<strlen(gzPreBuf); i++ ) 
    if ( gzPreBuf[i]<=0 || (!isprint (gzPreBuf[i]) && !isspace(gzPreBuf[i])) ) gzPreBuf[i] = ' ' ;

  gHyp_util_log ( gzPreBuf ) ;
  
  return FALSE ;
}

sLOGICAL gHyp_util_logWarning ( const char *fmt, ... )
{
  /* Description:
   *
   *    Log an warning.
   *    This routine is just like _logInfo but adds "%WARNING"
   *    to the message and returns FALSE instead of TRUE.
   *
   * Arguments:
   *
   *    fmt                             [R]
   *    - null terminated format string (see sprintf)
   *
   *    ...                             [R]
   *    - values for format arguments
   *
   * Return value:
   *
   *    Returns FALSE.
   *
   */
  va_list       ap ;
  unsigned int i ;
  
  if ( guRunFlags & RUN_QUIET ) return FALSE ;

  strcpy ( gzPreBuf, "%WARNING: " ) ; 
  
  va_start ( ap, fmt ) ;
  vsprintf ( gzPreBuf+strlen(gzPreBuf), fmt, ap ) ;
  va_end ( ap ) ;
  
  /* Replace non-printing just a space */
  for ( i=0; i<strlen(gzPreBuf); i++ )    
    if ( gzPreBuf[i]<=0 || (!isprint (gzPreBuf[i]) && !isspace(gzPreBuf[i])) ) gzPreBuf[i] = ' ' ;

  gHyp_util_log ( gzPreBuf ) ;
    
  return FALSE ;
}

sLOGICAL gHyp_util_sysError ( const char *fmt, ... )
{
  /* Description:
   *
   *    Log an system error.
   *    This routine looks at errno and appends the system error message
   *    to the output string.
   *    With VMS, there is some additional complication...
   *
   * Arguments:
   *
   *    fmt                             [R]
   *    - null terminated format string (see sprintf)
   *
   *    ...                             [R]
   *    - values for format arguments
   *
   * Return value:
   *
   *    Returns FALSE.
   *
   */
  char          *strerr ;  
  va_list       ap ;
  strcpy ( gzPreBuf, "%ERROR: " ) ; 
  
  va_start ( ap, fmt ) ;
  vsprintf ( gzPreBuf+strlen(gzPreBuf), fmt, ap ) ;
  va_end ( ap ) ;

#ifdef AS_VAXC

  if ( errno == EVMSERR ) 
    strerr = strerror (errno,vaxc$errno) ; 
  else
    strerr = strerror (errno) ;

  if ( strerr ) 
    sprintf (   gzPreBuf+strlen(gzPreBuf), ": (%u) %s", 
                errno, 
                strerr ) ; 
  else
    sprintf (   gzPreBuf+strlen(gzPreBuf), ": (%u) Unknown errno", 
                errno ) ;

#else
#ifdef AS_WINDOWS
  errno = GetLastError() ;
#endif

#ifdef AS_SUN

  sprintf (     gzPreBuf+strlen(gzPreBuf), 
                ": (%u) %s", errno, sys_errlist[errno] ) ; 

#else

  strerr = strerror (errno) ;
  if ( strerr )
    sprintf (   gzPreBuf+strlen(gzPreBuf), 
                ": (%u) %s", errno, strerror (errno) ) ; 
  else  
    sprintf (   gzPreBuf+strlen(gzPreBuf), ": (%u) Unknown errno", 
                errno ) ;

#endif

#endif

  gHyp_util_log ( gzPreBuf ) ;
  
  return FALSE ;
}

sLOGICAL gHyp_util_check ( int status, short int ioStatus )
{
  /* Description:
   *
   *    For the benefit of VMS system calls which return a call status
   *    and possibly an IO status, this routine determines if the system
   *    call was successful, and generates an error message if not.
   *
   * Arguments:
   *
   *    status                          [R]
   *    - status from VMS system call
   *
   *    ioStatus                        [R]
   *    - the the system call has an iosb block, then
   *      this is iosb[0]
   *
   * Return value:
   *
   *    Returns TRUE if all was OK, FALSE otherwise.
   *
   */
#ifdef AS_VMS

  unsigned short        msgLen ;
  unsigned char         outadr[4] ;
  char                  msg[MAX_TERMINAL_LENGTH+1] ;
  makeDSCs              ( msg_d, msg );

  if ( !(status & 1) ) {

    /* System call was bad */
    sys$getmsg ( status, &msgLen, &msg_d, 0, &outadr[0] ) ;
    msg[msgLen] = '\0' ;
    return gHyp_util_logError ( msg+1 ) ;

  }
  else if ( !(ioStatus & 1) ) {

    /* System call was ok but the I/O was bad */

    if ( ioStatus & 0x8000  ) {

      /* Wollongong's special psycho conversion to errno when the high bit 
       * of the ioStatus variable is set
       */
      errno = ((0x7ffff & ioStatus ) >> 3 ) ;
      return gHyp_util_sysError ( "Failure" ) ;
    }
    else {    

      /* Bad I/O status */
      status = ioStatus ;
      sys$getmsg ( status, &msgLen, &msg_d, 0, &outadr[0] ) ;
      msg[msgLen] = '\0' ;
      return gHyp_util_logError ( msg+1 ) ;
    }
    
  }
  else
    return TRUE ;
#else
  return TRUE ;
#endif
}

static int lHyp_util_getToken ( char *pStr, sLOGICAL isUnaryOk, sLOGICAL isDashOk, sLOGICAL isDotOk )
{
  /* Description:
   *
   *    Gets the next token in a string, returning the number of characters
   *    in the token.
   *    
   *    Tokens include
   *
   *            Integer values.
   *            Decimal values.
   *            Hexideximal values.
   *            Exponential values.
   *            Identifiers
   *
   *    A state transition method is used to extract a token.
   *
   * Arguments:
   *
   *    pStr                                                    [R]
   *    - null terminated string in which to scan for next token
   *
   *    isUnaryOk                                               [R]
   *    - if TRUE, a unary + or - operator can preceed the token
   *
   *    isDashOk                                               [R]
   *    - if TRUE, an identifier can have a dash, for cgi
   *
   *    isDotOk                                               [R]
   *    - if TRUE, an identifier can have a dot, for recognizing old-style messages
   *
   * Return values:
   *
   *    0   : no valid token found
   *    > 0 : length of valid identifier found  
   *    < 0 : negative length of valid number found
   *
   */

  char
    *pChar = pStr ;

  unsigned long
    expectState = LEX_NONE ;

  int
    scanLen = 0,
    partialLen = 0,
    validLen = 0 ;

  sLOGICAL
    isIdentifier ;

  unsigned long
    colonMask =  LEX_COLON,
    exponentMask   = LEX_EXP,
    dotDecimalMask = LEX_DOT, 
    dashMask=0,
    dotMask=0;

  if ( isDashOk ) dashMask = LEX_DASH ;
  if ( isDotOk ) dotMask = LEX_DOT ;

  /* First character begins the state transition */

  if ( *pChar == '+' || *pChar == '-' ) {
    if ( !isUnaryOk ) return 0 ;
    /* Token is a lone operator or the start of an integer */
    expectState = LEX_DIGIT | dotDecimalMask ;
    isIdentifier = FALSE ;
  }
  else if ( *pChar == '.' ) {
    /* Token is the start of decimal. 
     * Identifiers are not allowed to start with a '.'
     */
    expectState = LEX_DIGIT ;
    /* Only one dot allowed in decimal number. Clear mask. */
    dotDecimalMask = 0 ;
    isIdentifier = FALSE ;
  }
  else if ( isdigit ( *pChar ) ) {
    /* Token is the start of an integer.
     * Identifiers are not allowed to start with a digit.
     */ 
    expectState = LEX_DIGIT | LEX_EXP | dotDecimalMask ;
    /* The integer so far is valid, and is one char long */
    validLen = 1 ;
    isIdentifier = FALSE ;

    /* If the number starts with a 0x or 0o, then it becomes a hex or octal */
    if ( *pChar == '0' ) expectState |= (LEX_HEX | LEX_OCTAL | LEX_UNICODE ) ;
  }
  else if ( isalpha ( *pChar ) || *pChar == '_' || *pChar == '$' ) {
    /* Token is the start of an identifier */
    expectState = LEX_ALPHA | LEX_DIGIT | LEX_UBAR | colonMask | dashMask | dotMask ;
    /* The identifier so far is valid, and is one char long */
    validLen = 1 ;
    isIdentifier = TRUE ;
  }
  else
    /* First character not beginning of an allowed token */
    return 0 ;

  scanLen = 1 ;
  while ( *(++pChar) ) {

    /* Count number of characters scanned */
    scanLen++ ;

    if ( ( expectState & LEX_EXP ) && ( *pChar == 'e' || *pChar == 'E' ) ) {
      /* Token is changing to an exponential number */
      expectState = LEX_EXP_PLUSMINUS | LEX_EXP_DIGIT ;
      /* Turn off exponent mask, only one allowed */    
      exponentMask = 0 ;
    }   

    else if (   ( expectState & LEX_EXP_PLUSMINUS ) && 
                ( *pChar == '+' || *pChar == '-' ) ) {
      /* Continue of exponential number */
      expectState = LEX_EXP_DIGIT ;
    }

    else if (   ( expectState & LEX_EXP_DIGIT ) && 
                ( isdigit ( *pChar ) ) ) {
      /* Completed exponential */
      expectState = LEX_EXP_DIGIT ;
      /* Token is a valid exponential */
      validLen = scanLen ;
    }

    else if ( isdigit ( *pChar ) && 
              ( expectState & LEX_DIGIT ) &&
              isIdentifier ) {

      /* Continue an identifier.
       * Can have more underbars, '$', or maybe a ':'.
       */               
      expectState = LEX_ALPHA | LEX_DIGIT | LEX_UBAR | LEX_DOLLAR | colonMask | dashMask | dotMask ;
      /* If it wasn't valid before, it is now */
      validLen = scanLen ;
 
    }
    else if ( scanLen == 2 && 
              ( expectState & LEX_HEX ) &&
              *pChar == 'x' ) {

      /* Continue a hex digit. */
      expectState = LEX_HEX ;
    }

    else if ( scanLen == 2 && 
              ( expectState & LEX_OCTAL ) &&
              *pChar == 'o' ) {

      /* Continue an octal digit. */
      expectState = LEX_OCTAL ;
    }
    else if ( scanLen == 2 && 
              ( expectState & LEX_UNICODE ) &&
              *pChar == 'u' ) {

      /* Continue a unicode digit. */
      expectState = LEX_UNICODE ;
    }
    else if ( expectState == LEX_HEX && 
              scanLen > 2 &&
              strchr ( "0123456789aAbBcCdDeEfF", *pChar ) != NULL ) {
      /* Continue a hex digit. */
      validLen = scanLen ;
    }

    else if ( expectState == LEX_OCTAL && 
              scanLen > 2 &&
              strchr ( "01234567", *pChar ) != NULL ) {
      /* Continue an octal digit. */
      validLen = scanLen ;
    }


    else if ( expectState == LEX_UNICODE && 
              scanLen > 2 &&
              strchr ( "0123456789", *pChar ) != NULL ) {
      /* Continue an unicode digit. */
      validLen = scanLen ;
    }

    else if ( isdigit ( *pChar ) && 
              ( expectState & LEX_DIGIT ) &&
              !isIdentifier ) {
      /* Continue a decimal or an integer */
      expectState = LEX_DIGIT | exponentMask | dotDecimalMask ;
      validLen = scanLen ;
    }

    else if ( *pChar == ':' && ( expectState & LEX_COLON ) ) {

      /* Namespace in identifier. Cannot have colon again or digit as next character. */
      colonMask = 0 ;
      expectState = LEX_ALPHA | LEX_UBAR | LEX_DOLLAR ;
      /* Its not valid unless we get at least another digit */
      partialLen = scanLen - 1 ;
      validLen = 0 ;
    }
    else if ( *pChar == '-' && ( expectState & LEX_DASH ) ) {

      /* Dash in identifier.  */
      /* Its not valid unless we get at least another digit */
      partialLen = scanLen - 1 ;
      validLen = 0 ;
    }

    else if ( ( isalpha ( *pChar ) && ( expectState & LEX_ALPHA ) ) ||
              ( *pChar == '_' && ( expectState & LEX_UBAR ) ) ||
              ( *pChar == '$' && ( expectState & LEX_DOLLAR ) ) ) {

      /* Continue an identifier.
       * Can have more underbars, '$'.
       */               
      expectState = LEX_ALPHA | LEX_DIGIT | LEX_UBAR | LEX_DOLLAR | colonMask | dashMask | dotMask ;
      /* If it wasn't valid before, it is now */
      validLen = scanLen ;
    }

    else if ( *pChar == '.' && ( expectState & LEX_DOT ) ) {

      if ( expectState & LEX_ALPHA ) {
        /* Continue a valid identifier which allows '.' 
         * Don't allow a digit to start or another dot or a dash !
         */
        expectState = LEX_ALPHA | LEX_UBAR | LEX_DOLLAR ;

        /* Its not valid unless we get at least another digit */
        partialLen = scanLen - 1 ;
        validLen = 0 ;

      }
      else {
        /* Changing integer to decimal. */
        expectState = LEX_DIGIT | exponentMask ;
        dotDecimalMask = 0 ;
        validLen = scanLen ;
      } 
    }

    else {

      /* Next character did not match any expected states */
      break ;   
    }
    
  } 
  if ( isIdentifier ) {
    if ( validLen == 0 ) validLen = partialLen ;
    return validLen ;

  }
  else
    return -(validLen) ;

}


int gHyp_util_getToken ( char *pStr, sLOGICAL isUnaryOk )
{
  /* Description:
   *
   *    Gets the next token in a string, returning the number of characters
   *    in the token.
   *    
   *    Tokens include
   *
   *            Integer values.
   *            Decimal values.
   *            Hexideximal values.
   *            Exponential values.
   *            Identifiers
   *
   *    A state transition method is used to extract a token.
   *
   * Arguments:
   *
   *    pStr                                                    [R]
   *    - null terminated string in which to scan for next token
   *
   *    isUnaryOk                                               [R]
   *    - if TRUE, a unary + or - operator can preceed the token
   *
   * Return values:
   *
   *    0   : no valid token found
   *    > 0 : length of valid identifier found  
   *    < 0 : negative length of valid number found
   *
   */
  return lHyp_util_getToken ( pStr, isUnaryOk, FALSE, FALSE ) ;
}

int gHyp_util_getToken_okDash ( char *pStr )
{
  return lHyp_util_getToken ( pStr, FALSE, TRUE, FALSE ) ;
}

int gHyp_util_getToken_okDot ( char *pStr )
{
  return lHyp_util_getToken ( pStr, FALSE, FALSE, TRUE ) ;
}

sLOGICAL gHyp_util_isIdentifier ( char *pStr )
{
  /* Description:
   *
   *    Determines whether a string is a valid identifier.
   *
   * Arguments:
   *
   *    pStr                            [R]
   *    - string to test
   *
   * Return value:
   *
   *    none
   *
   */

  unsigned tokenLen = gHyp_util_getToken ( pStr, FALSE ) ;

  if ( tokenLen == strlen ( pStr ) )
    return TRUE ;
  else 
    return FALSE ;      
}

sLOGICAL gHyp_util_match ( char *pToken, char *pPattern ) 
{
  /* Description:
   *
   *    Using '*' as a wildcard character, match pToken with pPattern.
   *
   *    Example: 
   *       token 'foobar' matches patterns foobar, *, foo*, *bar, and *oba*
   *
   * Arguments:
   *
   *    pToken                                                  [R]
   *    - null terminated string to match against pattern
   *
   *    pPattern                                                [R]
   *    - null terminated string specifying pattern to match
   *
   * Return value:
   *
   *    Returns TRUE if matched, FALSE otherwise
   *
   */

  sLOGICAL      wildCard = FALSE ;
  int           subStringLen ;
  char          *pMatch,
                *pTokenEnd,
                *pPatternEnd,
                subString[VALUE_SIZE+1] ;
  
  /* Just do strcmp() if there's no wildcard in the pattern */
  if ( strchr ( pPattern, '*' ) == NULL ) 
    return (sLOGICAL) ( strcmp ( pToken, pPattern ) == 0 ) ;
    
  pTokenEnd = pToken + strlen ( pToken ) ;
  pPatternEnd = pPattern + strlen ( pPattern ) ;

  /* Scan from left to right in pToken */
  while ( pPattern < pPatternEnd ) {

    if ( *pPattern == '*' ) {

      /* Encountered a wildcard in pattern. */
      wildCard = TRUE ;
      
      /* Advance to first non-wildcard character in pattern */  
      while ( *pPattern == '*' ) pPattern++ ;
      
      /* If the pattern ended in a wildcard, the rest of the token matches. */  
      if ( pPattern >= pPatternEnd ) return TRUE ;
    }
    
    /* If there's no more token, then there's no match */
    if ( pToken >= pTokenEnd ) return FALSE ;    

    /* Extract the next substring to match from the pattern */
    strcpy ( subString, pPattern ) ;
    
    /* Truncate before the first occurrance of another wildcard. */
    if ( (pMatch = strchr ( subString, '*' ) ) ) *pMatch = '\0' ;
    subStringLen = strlen ( subString ) ;
    
    /* Search for the pattern substring in the token */
    if ( !(pMatch = strstr ( pToken, subString ) ) ) return FALSE ;
      
    /* If the substring was not preceeded by a '*', the search is anchored. */
    if ( !wildCard && pMatch != pToken ) return FALSE ;
    
    /* Advance to the next positions in the token and pattern */
    pToken = pMatch + subStringLen ;
    pPattern += subStringLen ;
    
  }

  /* Pattern has ended */

  /* If the token also ended, then its a match, otherwise its not. */
  if ( pToken >= pTokenEnd ) 
    return TRUE ;
  else
    return FALSE ;
}

void gHyp_util_gethostname ( char *host )
{
  /* Description:
   *
   *    Gets the local host name.
   *
   * Arguments:
   *
   *    host                                            [W]
   *    - pointer to char in which to write host name
   *
   * Return value:
   *
   *    none
   *
   */

#ifdef AS_VMS

  int           status ;
  int           hostLen = 0 ;

  char          *pNL = "NL:";
  makeDSCz      ( NL_d, pNL ) ;

  short int iosb[4] ;

  struct {
    short int   bufLen ;
    short int   itemCode ;
    char*       bufAddr ;
    int*        lenAddr ;
    int         terminator;
  } itmlst = { HOST_SIZE, DVI$_HOST_NAME, host, &hostLen, 0 } ;

  hostLen = 0 ;
  status = sys$getdviw ( 0, 0, &NL_d, &itmlst, &iosb[0], 0, 0, 0 ) ;
  if ( ( status & 1 ) && ( iosb[0] & 1 ) && hostLen <= HOST_SIZE )
    host[hostLen] = '\0' ;
  else
    strcpy ( host, "localhost" ) ;

#else

#ifdef AS_SOLARIS

  sysinfo ( SI_HOSTNAME, host, HOST_SIZE ) ;

#else
  gHyp_tcp_gethostname ( host, HOST_SIZE ) ;
#endif
#endif

  /* Lowercase it */
  gHyp_util_lowerCase ( host, strlen ( host ) ) ;
  
  return ;
}


void gHyp_util_getprocessname ( char *prcnam )
{
  /* Description:
   *
   *    Get process name
   *
   * Arguments:
   *
   *    prcnam                                                  [W]
   *    - pointer to char in which to write process name
   *
   * Return value:
   *
   *    none
   *
   */

#ifdef AS_VMS

  int   status ;
  int   prcnamlen = 0 ;
  short int iosb[4] ;

  struct {
    short int   bufLen ;
    short int   itemCode ;
    char*       bufAddr ;
    int*        lenAddr ;
    int         terminator;
  } itmlst = { TARGET_SIZE, JPI$_PRCNAM, prcnam, &prcnamlen, 0 } ;

  status = sys$getjpiw ( 0, 0, 0, &itmlst, &iosb[0], 0, 0 ) ;
  if ( !gHyp_util_check ( status, iosb[0] ) ) 
    gHyp_util_logWarning ( "Failed to get process name" ) ;
  else {
    /* Truncate if process name is too long */
    if ( prcnamlen <= TARGET_SIZE ) prcnam[prcnamlen] = '\0' ;
  }

#else

#ifdef AS_UNIX
  struct passwd 
    *pw ;

  pw = getpwuid ( getuid() ) ;
  strcpy ( prcnam, pw->pw_name ) ;
  prcnam[TARGET_SIZE] = '\0' ;

#else
  int
    prcnamsz = TARGET_SIZE ;

  /* Windows */
  GetUserName( prcnam, &prcnamsz ) ;
#endif
#endif

  return ;
}

void gHyp_util_setprocessname ( char *prcnam )
{
  /* Description:
   *
   *    Set the process name.  (VMS only)
   *
   * Arguments:
   *
   *    prcnam                                                  [R]
   *    - new process name
   *
   * Return value:
   *
   *    none
   *
   */

#ifdef AS_VMS

  int   status ;
  makeDSCz ( prcnam_d, prcnam ) ;
  status = sys$setprn ( &prcnam_d ) ;
  if ( !gHyp_util_check ( status, 1 ) )
    gHyp_util_logWarning ( "Failed to set process name to %s", prcnam ) ;
#endif

  return ;
}

int gHyp_util_parseXML ( char *pStr )
{
  /* Description:
   *
   *    Parses a string, recognizing the following escape sequences.
   *
   *    &gt;
   *    &lt;
   *    &amp;
   *    &quote
   *    
   *
   * Arguments:
   *
   *    target                          [R/W]
   *    - null terminated string
   *
   * Return value:
   *
   *    length of string
   *
   *
   */
  
  char 
    *pStart,
    *pEnd,
    *pAnchor ;

  int
    i,
    n ;

  unsigned int 
    u ;

  pStart = pAnchor = pStr ;
  pEnd = pStart + strlen ( pStr ) ;
  while ( pStr < pEnd ) {
    
    if ( *pStr == '&' ) {
      
      /* Check next characters */
      if ( strncmp ( pStr, "&lt;", 4 ) == 0 ) {
        /* < */
        *pAnchor++ = '<' ;
        pStr += 3 ;
      }
      else if ( strncmp ( pStr, "&gt;", 4 ) == 0 ) {
        /* < */
        *pAnchor++ = '>' ;
        pStr += 3 ;
      }
      else if ( strncmp ( pStr, "&amp;", 5 ) == 0 ) {
        /* & */
        *pAnchor++ = '&' ;
        pStr += 4 ;
      }
      else if ( strncmp ( pStr, "&quot;", 6 ) == 0 ) {
        /* " */
        *pAnchor++ = '"' ;
        pStr += 5 ;
      }
      else if ( strncmp ( pStr, "&nbsp;", 6 ) == 0 ) {
        *pAnchor++ = ' ' ;
        pStr += 5 ;
      }
      
      else if ( strncmp ( pStr, "&laquo;", 7 ) == 0 ) {
        *pAnchor++ = '\213' ;
        pStr += 6 ;
      }
      else if ( strncmp ( pStr, "&raquo;", 7 ) == 0 ) {
        *pAnchor++ = '\233' ;
        pStr += 6 ;
      }
      else if ( strncmp ( pStr, "&copy;", 6 ) == 0 ) {
        *pAnchor++ = '\251' ;
        pStr += 5 ;
      }
      
      else if ( strncmp ( pStr, "&#x", 3 ) == 0 ) {
         i = sscanf ( pStr+3, "%x%n", (unsigned int*) &u, &n ) ;
	 if ( i == 1 && n > 0 && n < 6 && *(pStr+3+n) == ';' ) {
            *pAnchor++ = (char) u ;
            pStr += (n+3) ;
	 }
      }
      else if ( strncmp ( pStr, "&#", 2 ) == 0 ) {
         i = sscanf ( pStr+2, "%u%n", (unsigned int*) &u, &n ) ;
	 if ( i == 1 && n > 0 && n < 6 && *(pStr+2+n) == ';' ) {
            *pAnchor++ = (char) u ;
            pStr += (n+2) ;
	 }
      }
      else
	*pAnchor++ = *pStr ;
      
    }
    else
      *pAnchor++ = *pStr ;

    pStr++ ;

  }

  /* Null terminate the result */
  *pAnchor = '\0' ;

  /* Return the new length */
  return ( pAnchor - pStart ) ;
}

int gHyp_util_parseString ( char *pStr )
{
  /* Description:
   *
   *    Parses a string into internal form, recognizing the following escape sequences.
   *
   *    \a
   *    \b
   *    \e
   *    \f
   *    \g
   *    \h
   *    \n
   *	\r
   *    \t
   *    \v
   *    \ooo
   *    \xXX
   *    \uXXXX
   *    \\
   *    \"
   *    \'
   *    \`
   *    
   *
   * Arguments:
   *
   *    target                          [R/W]
   *    - null terminated string
   *
   * Return value:
   *
   *    length of string
   *
   */
  
  char 
    *pStr2,
    *pStart,
    *pAnchor ;

  int
    j,
    len,
    n ;

  union {
  	unsigned int u ;
	unsigned char uc ;
  } s ;

  char
    hexString[5],
    octalString[4] ;

  sEndian
    endian ;
  
  pStart = pAnchor = pStr ;
  for ( ;*pStr; pStr++ ) {
    
    if ( *pStr == '\\' ) {
      
      /* Check next character */
      pStr2 = pStr + 1 ;

      switch ( *pStr2 ) {

      case 'a' :
        /* Bell */
        *pAnchor++ = '\a' ;
        pStr = pStr2 ;
        break ;

      case 'b' :
        /* Backspace */
        *pAnchor++ = '\b' ;
        pStr = pStr2 ;
        break ;

      case 'f' :
        /* Formfeed */
        *pAnchor++ = '\f' ;
        pStr = pStr2 ;
        break ;

      case 'n' :
        /* Newline */
        *pAnchor++ = '\n' ;
        pStr = pStr2 ;
        break ;
        
      case 'r' :
        /* Cariage Return */
        *pAnchor++ = '\r' ;
        pStr = pStr2 ;
        break ;

      case 't' :
        /* Tab */
        *pAnchor++ = '\t' ;
        pStr = pStr2 ;
        break ;
        
      case 'v' :
        /* Vertical tab */
        *pAnchor++ = '\v' ;
        pStr = pStr2 ;
        break ;
 
      case '"' :
        /* Double quote */
        *pAnchor++ = '"' ;
        pStr = pStr2 ;
        break ;

      case '\'' :
        /* Singe quote */
        *pAnchor++ = '\'' ;
        pStr = pStr2 ;
        break ;

      case '`' :
        /* Back quote */
        *pAnchor++ = '`' ;
        pStr = pStr2 ;
        break ;
        
      case '\\' :
        /* Back slash */
        *pAnchor++ = '\\' ;
        pStr = pStr2 ;
        break ;
        
      case 'x' :
        /* Hex value */
        pStr2++ ;
        len = strspn (pStr2,"0123456789aAbBcCdDeEfF") ;
        if ( len >= 2 ) {
          /* Valid hex digits - take only the first two digits */
          hexString[0] = *pStr2++ ;
          hexString[1] = *pStr2 ;
          hexString[2] = '\0' ;
          n = sscanf ( hexString, "%x", (unsigned int*) &s.u ) ;
          *pAnchor++ = s.uc ;
          pStr = pStr2 ;
        }
        else
          *pAnchor++ = '\\' ;
        break ;

      case 'u' :
        /* Unicode value */
        pStr2++ ;
        len = strspn (pStr2,"0123456789aAbBcCdDeEfF") ;
        if ( len >= 4 ) {
          /* Valid hex digits. */
          hexString[0] = *pStr2++ ;
          hexString[1] = *pStr2++ ;
          hexString[2] = *pStr2++ ;
          hexString[3] = *pStr2 ;
          hexString[4] = '\0' ;
          n = sscanf ( hexString, "%x", (unsigned int*) (unsigned short*) &endian.x.us ) ;

	  if ( gHyp_util_isLittleEndian() ) 
	    for ( j=1; j>=0; j-- ) *pAnchor++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<2; j++ ) *pAnchor++ = endian.x.b[j] ;

          pStr = pStr2 ;
        }
        else 
          *pAnchor++ = '\\' ;
	
        break ;

      default:
        len = strspn (pStr2,"01234567") ;
        if ( len >= 3 ) {
          /* Valid octal digit */
          octalString[0] = *pStr2 ;
          pStr2++ ;
          octalString[1] = *pStr2 ;
          pStr2++ ;
          octalString[2] = *pStr2 ;
          octalString[3] = '\0' ;
          n = sscanf ( octalString, "%o", (unsigned int*) &s.u ) ;
          *pAnchor++ = s.uc ;
          pStr = pStr2 ;
        }
        else
          *pAnchor++ = '\\' ;

        break ;
      }
      
    }
    else
      *pAnchor++ = *pStr ;
  }
  /* Don't NULL Terminate */
  /*****     *pAnchor = '\0' ;  ****/
  return ( pAnchor - pStart ) ;

}

int gHyp_util_unparseString ( char *pDstStr, 
                              char *pSrcStr,
                              int strLen,
                              int maxlen,
			      sLOGICAL isForMSG,
			      sLOGICAL isForXML,
			      sLOGICAL isForSQL,
			      char *specialChars )
{

  /* Parse string into external format. Special case exists if the string is
   * for XML or AbInitio Message.
   * The specialChars are those additional chars that should be preceded by a backslash
   * for with &#000; sequences.
   */
  char
    c,
    *pDst = pDstStr,
    *pEndDst = pDst + maxlen ;

  unsigned char
    uc ;

  int 
    i=0 ;

  sLOGICAL
    WS = TRUE ;;

  if ( isForXML ) {

    for ( ; i<strLen && pDst < pEndDst; pSrcStr++ ) {
    
      switch ( *pSrcStr ) {
      
      case '"' :
        /* Double quote */
 	if ( pDst+6 > pEndDst ) break ;
	pDst += sprintf ( pDst, "%s", "&quot;" ) ;
	WS = FALSE ;
        break ;
      
      case '<' :
	if ( pDst+4 > pEndDst ) break ;
	pDst += sprintf ( pDst, "%s", "&lt;" ) ;
	WS = FALSE ;
        break ;
    
      case '>' :
 	if ( pDst+4 > pEndDst ) break ;
	pDst += sprintf ( pDst, "%s", "&gt;" ) ;	
	WS = FALSE ;
        break ;
      
      case '&' :
 	if ( pDst+5 > pEndDst ) break ;
	pDst += sprintf ( pDst, "%s", "&amp;" ) ;	
	WS = FALSE ;
        break ;

      /*
      case ' ' :
	if ( WS ) {
 	  if ( pDst+6 > pEndDst ) break ;
	  pDst += sprintf ( pDst, "%s", "&nbsp;" ) ;	
        }
	else
          *pDst++ = *pSrcStr ;
	break ;

      case '\t' :
	if ( WS ) {
 	  if ( pDst+5 > pEndDst ) break ;
	  pDst += sprintf ( pDst, "%s", "&tab;" ) ;	
        }
	else
          *pDst++ = *pSrcStr ;
	break ;
      */

      default :
        c = *pSrcStr ;
        if ( c <= 0 || (!isprint ( c ) && !isspace ( c ))  ) {
	  if ( pDst+6 > pEndDst ) break ;
          uc = c ;
	  pDst += sprintf ( pDst, "&#x%02x;", uc ) ;
	  /*
          if ( c > 0 )
            pDst += sprintf ( pDst, "&#%04d;", c ) ;
          else {
            uc = c ;
	    pDst += sprintf ( pDst, "&#%05hu;", uc ) ;
	  }
	  */
	}
        else {
          *pDst++ = *pSrcStr ;
	  WS = FALSE ;
	}
      }
      i++ ;
    }
  }
  else {

    for ( ; i<strLen && pDst < pEndDst; pSrcStr++ ) {

    switch ( *pSrcStr ) {
      
    case '\a' :
      /* Bell */
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = 'a' ;
      break ;
 
    case '\b' :
      /* Backspace */
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = 'b' ;
      break ;
 
    case '\f' :
      /* Formfeed */
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = 'f' ;
      break ;
 
    case '\n' :
      /* Newline */
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = 'n' ;
      break ;

    case '\r' :
      /* Carriage Return */
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = 'r' ;
      break ;
      
    /*
    case '\t' :
      * Tab *
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = 't' ;
      break ;  
    */

    case '\v' :
      /* Vertical tab */
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = 'v' ;
      break ;

    case '"' :
      /* Double quote */
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = '"' ;
      break ;
           
    case '|' :
      if ( isForMSG ) {
        /* Pipe */
        if ( pDst+4 > pEndDst ) break ;
	*pDst++ = '\\' ;
        pDst += sprintf ( pDst, "%03o", 124 ) ;
      }
      else {
         if ( pDst+1 > pEndDst ) break ;
        *pDst++ = *pSrcStr ;	
      }
      break ;

    case '\\' :
      /* Backslash */
      if ( pDst+2 > pEndDst ) break ;
      *pDst++ = '\\' ;
      *pDst++ = '\\' ;
      break ;

    /* Single quote must come before "default" */
    case '\'' :

      /* Single quote */

      if ( isForSQL ) {

#ifdef AS_SQL
        if ( pDst+2 > pEndDst ) break ;
#if defined ( AS_SQLSERVER ) || defined ( AS_ORACLE )
	*pDst++ = '\'' ;
#else /* MYSQL, PGSQL */
	*pDst++ = '\\' ;
#endif
	*pDst++ = '\'' ;
	break ;          
#endif
      }

    default :
      c = *pSrcStr ;

      /* Check for specialChars */
      if ( c != '\0' && strchr ( specialChars, c ) != NULL ) {
	if ( pDst+2 > pEndDst ) break ;
        *pDst++ = '\\' ;
        *pDst++ = c ;
      } 
      else if ( c <= 0 || (!isprint ( c ) && !isspace ( c )) ) {

        if ( isForSQL ) {

#ifdef AS_SQL

#ifdef AS_PGSQL

          /* DEC MCS to Unicode */
          if ( pDst+1 > pEndDst ) break ;
          *pDst++ = '\\' ;

          uc = c ;
          if ( uc >= 128 )
            uc &= ~64 ;
          else
            uc = 191 ;

          if ( pDst+7 > pEndDst ) break ;
          pDst += sprintf ( pDst, "%03o", 195 ) ;
          *pDst++ = '\\' ;
          pDst += sprintf ( pDst, "%03o", uc ) ;

#else     /* ORACLE, MYSQL, SQLSERVER */

	  /* Replace the unprintable with a dot. */
          *pDst++ = '.' ;

#endif

#endif
	}
	else {

	  /* Not for SQL or ordinary control chars */
          if ( pDst+4 > pEndDst ) break ;
          *pDst++ = '\\' ;
	  uc = c ;
	  pDst += sprintf ( pDst, "%03o", uc ) ;

	}
      }
      else
        *pDst++ = *pSrcStr ;
    }
    i++ ;
    }
  }
  *pDst = '\0' ;

  return (pDst-pDstStr); 
}
 

int gHyp_util_trim ( char *target )
{
  /* Description:
   *
   *    Trim all trailing blanks and tabs from string
   *
   * Arguments:
   *
   *    target                          [R/W]
   *    - null terminated string
   *
   * Return value:
   *
   *    length of string
   *
   */

  char *pStr = target + strlen ( target ) - 1 ;

  /* Remove unprintables (newline or carriage return) at end of line */
  while ( pStr >= target && (*pStr < ' ' ) )
    *pStr-- = '\0' ;

  /* Remove whitespace at end of line */
  while ( pStr >= target && (*pStr == ' ' || *pStr == '\t' ) )
    *pStr-- = '\0' ;

  return (int)((pStr - target) + 1);
}

int gHyp_util_trimWhiteSpace ( char *target )
{
  /* Description:
   *
   *    Trim all trailing blanks and tabs from string, but not \n or other
   *
   * Arguments:
   *
   *    target                          [R/W]
   *    - null terminated string
   *
   * Return value:
   *
   *    length of trimmed string
   *
   */

  char *pStr = target + strlen ( target ) - 1 ;
  while ( pStr >= target && (*pStr == ' ' || *pStr == '\t' ) )
    *pStr-- = '\0' ;
  return (int)((pStr-target)+1);
}

void gHyp_util_strip ( char *target, int count )
{
  /* Description:
   *
   *    Strip all characters from string except alphanumerics, '.', '_','-'
   *
   * Arguments:
   *
   *    target                          [R/W]
   *    - null terminated string
   *
   * Return value:
   *
   *    none
   *
   */

  char *newTarget ;
  int i ;

  newTarget = target ;

  for ( i=0; i<count; i++ ) {

    if ( isalnum ( *target ) || 
         *target == '_' || 
         *target == '.' ||
         *target == '-' ) {
      if ( target > newTarget ) *newTarget = *target ;
      newTarget++ ;
    }
    target++ ;
  }
  *newTarget = '\0' ;
}

sLOGICAL gHyp_util_openLog ()
{
  /* Check for general logging */
  if ( gzLogName[0] ) {
    if ( (gsLog = fopen ( gzLogName, "a+"
#if defined(AS_VMS) && !defined(_ANSI_C_SOURCE)
                          , "shr=get,put"
#endif
                          ) ) == NULL ) {
      gzLogName[0] = '\0' ;
      gsLog = stdout ;
      return gHyp_util_sysError ( "Failed to open '%s'", gzLogName ) ;
    }
#ifndef AS_VMS
#if defined ( F_SETFD )
    if ( fcntl ( fileno(gsLog), F_SETFD, 1 ) == -1 )
      return gHyp_util_sysError ( "Failed to F_SETFD (FD_CLOEXEC) on '%s'",
                                  gzLogName ) ;
#elif defined ( FIOCLEX )
    if ( ioctl ( fileno(gsLog), FIOCLEX ) == -1 )
      return gHyp_util_sysError ( "Failed to FIOCLEX on '%s'",
                                  gzLogName) ;
#endif
#endif
  }
  return TRUE ;
}

sLOGICAL gHyp_util_isLittleEndian()
{
  short int word = 0x0001;
  char *byte = (char *) &word;
  return (sLOGICAL) byte[0];
}

sLOGICAL gHyp_util_breakTarget ( char *target, 
                             char *instance, 
                             char *concept, 
                             char *parent,
                             char *root,
			     char *node ) 
{
  char
    *pRoot,
    *pSep,
    *pLastToken,
    addr[MAX_PATH_SIZE+1],
    lastInstance[INSTANCE_SIZE+1],
    tmp[MAX_PATH_SIZE+1] ;

  sLOGICAL
    pastInstance = FALSE,
    pastConcept = FALSE,
    pastParent = FALSE,
    pastNode = FALSE ;

  int
    pn=0,
    n,
    n1,
    n2,
    n3;;

  /* Break up target into parent and child components */
  *parent = '\0' ;
  *concept = '\0' ;
  *instance = '\0' ;
  *root = '\0' ;
  *node = '\0' ;

  /* Extract all tokens. */
  
  pLastToken = target ;
  while ( *pLastToken ) {

    n = strcspn ( pLastToken, "#/@" ) ;
 
    pSep = pLastToken+n ;
    switch ( *pSep ) {
    case '#' :
      if ( !pastInstance ) {
        strncpy ( instance, pLastToken, n ) ;
        *(instance+n) = '\0' ;
        pastInstance = TRUE ;
      }
      else {
	strncpy ( lastInstance, pLastToken, n ) ;
        *(lastInstance+n) = '\0' ;
      }
      pSep++ ;
      pLastToken = pSep ;
      break ;
    case '@' :
      if ( !pastConcept ) {
        strncpy ( concept, pLastToken, n ) ;
        *(concept+n) = '\0' ;
        pastConcept = TRUE ;
      }
      pastInstance = TRUE ;;
      pastParent = TRUE ;
      /* Extract node if specified */
      if ( !pastNode ) {
        pSep++ ;
        if ( *pSep ) strcpy ( node, pSep ) ;
        pastNode = TRUE ;
      }
      else
        pSep++ ;
      pLastToken = pSep ;      
      break ;
    case '/' :
    case '\0' :
      pastInstance = TRUE ;
      if ( !pastConcept ) {
        strncpy ( concept, pLastToken, n ) ;
        *(concept+n) = '\0' ;
        pastConcept = TRUE ;
	lastInstance[0] = '\0' ;
      }
      else if ( !pastParent ) {

	/* Append to the parent */
        strcat ( parent, "/" ) ; 
        pn++ ; 
        strncat ( parent, pLastToken, n ) ;
        pn += n ;
        *(parent+pn) = '\0' ;

	/* Append to the root */
        strcat ( root, "/" ) ;
        pn++ ; 
	if ( lastInstance[0] && strcmp ( lastInstance, pLastToken ) != 0 ) {
	  strcat ( root, lastInstance ) ;
	  pn += strlen ( lastInstance ) ;
	  lastInstance[0] ='\0' ;
	  strcat ( root, "#" ) ;
          pn++ ;
	}
        strncat ( root, pLastToken, n ) ;
        pn += n ;
        *(root+pn) = '\0' ;
      }
      if ( *pSep ) pSep++ ;
      pLastToken = pSep ;
      break ;
    }
  }

  /* If we didn't set the node, assume it is the localhost */
  if ( !*node ) strcpy ( node, gzLocalHost ) ;

  /* If the mesage is to another host, don't assume any more, just return */
  if ( gHyp_tcp_resolveHost ( node, addr ) &&
       strcmp ( addr, gzLocalAddr ) != 0 ) return TRUE ;

  /* If we didn't set the instance, then assume its name is the same as the concept */
  if (  !*instance && *concept ) strcpy ( instance, concept ) ;

  if ( ! (!*parent && *concept && strcmp ( concept, gzAUTOROUTER ) == 0 ) ) {

    /* Not the ROOT - remove the implied ROOT name if already there. */

    sprintf ( tmp, "/%s", gzAUTOROUTER ) ;
    n1 = strlen ( tmp ) ;
    pRoot = root ;
    n2 = strlen ( pRoot ) ;
    n3 = n2-n1 ;
    if ( strstr ( pRoot, tmp ) == pRoot+n3 ) root[n3] = '\0' ;

    sprintf ( tmp, "/%s", gzAUTOROUTER ) ;
    n1 = strlen ( tmp ) ;
    pRoot = parent ;
    n2 = strlen ( pRoot ) ;
    n3 = n2-n1 ;
    if ( strstr ( pRoot, tmp ) == pRoot+n3 ) parent[n3] = '\0' ;

  }

  return TRUE ;
}


int gHyp_util_urlEncode( char *in, int size_in, char *out )  
{

  char  
    *ptr,
    c;

  ptr = out ;
  *ptr = '\0' ;

  while ( size_in-- ) {

    c = *in++ ;

    switch ( c ) {
    case '"' :
      *ptr++ = '%' ;
      *ptr++ = '2' ;
      *ptr++ = '2' ;
    case '\'' :
      *ptr++ = '%' ;
      *ptr++ = '2' ;
      *ptr++ = '7' ;
      break ; 
    case ',' :
      *ptr++ = '%' ;
      *ptr++ = '2' ;
      *ptr++ = 'C' ;
      break ; 
    case '%' :
      *ptr++ = '%' ;
      *ptr++ = '2' ;
      *ptr++ = '5' ;
      break ;
    case ' ' :
      *ptr++ = '%' ;
      *ptr++ = '2' ;
      *ptr++ = '0' ;
      break ;
    case '~' :
      *ptr++ = '%' ;
      *ptr++ = '7' ;
      *ptr++ = 'E' ;
      break ;
    case '/' :
      *ptr++ = '%' ;
      *ptr++ = '2' ;
      *ptr++ = 'F' ;
      break ;
    case '=' :
      *ptr++ = '%' ;
      *ptr++ = '3' ;
      *ptr++ = 'D' ;
      break ;
    case '&' :
      *ptr++ = '%' ;
      *ptr++ = '2' ;
      *ptr++ = '6' ;
      break ;
    case '+' :
      *ptr++ = '%' ;
      *ptr++ = '2' ;
      *ptr++ = 'B' ;
      break ;
    case ':' :
      *ptr++ = '%' ;
      *ptr++ = '3' ;
      *ptr++ = 'A' ;
      break ;
    case '?' :
      *ptr++ = '%' ;
      *ptr++ = '3' ;
      *ptr++ = 'F' ;
      break ; 
    default :
      if ( isprint ( c ) )
         *ptr++ = c ;
      else {
	 *ptr++ = '%' ;
         sprintf ( ptr, "%02lx", (unsigned long) c ) ;	 
         ptr += 2 ;
      }
       break ;
    }
  }
  *ptr = 0;
  return (ptr-out) ;
  
}

#define SPC_BASE16_TO_10(x) (((x) >= '0' && (x) <= '9') ? ((x) - '0') : \
                             (toupper((x)) - 'A' + 10))


int gHyp_util_urlDecode( char *in, int size_in, char *out )  
{
   
  char  
    *ptr;

  const char 
    *c;
   
  ptr = out ;

  for (c = in;  *c;  c++) {
    if (*c != '%' || !isxdigit(c[1]) || !isxdigit(c[2])) *ptr++ = *c;
    else {
      *ptr++ = (SPC_BASE16_TO_10(c[1]) * 16) + (SPC_BASE16_TO_10(c[2]));
      c += 2;
    }
  }
  *ptr = 0;
  return (ptr-out) ;
}

int gHyp_util_base64Encode( char *in, int size_in, char *out) {

  sBYTE
    ibuf[3];

  int 
    i_in,
    i_out,
    n_in,
    n ;

  i_in = 0 ;
  i_out = 0 ;
  n_in = size_in ;
 
  for( ;; ) {

    /* Clear internal buffer */
    memset(ibuf,0,3);

    /* Fill with next 3 characters to encode */
    for( n = 0; n < 3; n++ ) {

      if ( i_in == n_in ) break ;
      ibuf[n] = in[i_in++] ;
    }

    /* If we ran out of characters, we're done */
    if( n == 0 ) break;

    /* Create the first 2 encoded characters */
    out[i_out++] = gzBin2base[BS0] ;
    out[i_out++] = gzBin2base[BS1] ;

    /* If our segment was only 1 character, add 2 PAD characters */
    if( n == 1 ) {
      out[i_out++] = PAD ;
      out[i_out++] = PAD ;
      break;
    }

    /* Output the third encoded character */
    out[i_out++] = gzBin2base[BS2] ;

    /* If our segment was only 2 characters, add 1 PAD characters */
    if( n == 2 ) {
      out[i_out++] = PAD ;
      break;
    }

    /* Output the fourth encoded character */
    out[i_out++] = gzBin2base[BS3] ;
  }

  return i_out ;

}


static int lHyp_util_getBase64Char ( char ch ) 
{
  /* return index of next base64 char
   * or -1 if space
   * or -2 if PAD
   * or -3 if EOF
   * or -4 if illegal char
   */
  char 
    *bsoff;

  if( isspace(ch) ) return -1 ;
  if( ch == PAD ) return -2 ;
  if ( !ch ) return -3 ;
  bsoff = strchr(gzBin2base,ch)  ;
  if ( !bsoff ) return -4 ;
  return (int)(bsoff - gzBin2base);
}

int gHyp_util_base64Decode( char *in, int size_in, char *out )  
{
  sBYTE 
    obuf[3];

  int 
    ch,
    i_in,
    i_out,
    n_in,
    n_out;
  
  i_in = 0 ;
  i_out = 0 ;
  n_out = 0 ;
  n_in = size_in ;

  for( ;; ) {

    /* Get character from base 64 array */
    while ( (ch = lHyp_util_getBase64Char(in[i_in++])) == -1 ) ;

    /* Exit if end of string or error or PAD character */
    if( ch == -3 || ch == -4 || ch == -2 ) break;

    /* Decode it into position 0. */
    obuf[0] = ch << 2;

    /* Get next character from base 64 array */
    while ( (ch = lHyp_util_getBase64Char(in[i_in++])) == -1 ) ;

    /* Exit if end of string or error or PAD character */
    if( ch == -3 || ch == -4 || ch == -2 ) break;

    /* Decode into positions 0.and 1 */
    obuf[0] |= ch >> 4;
    obuf[1] = ch << 4;

    /* Get next character from base 64 array */
    while ( (ch = lHyp_util_getBase64Char(in[i_in++])) == -1 ) ;

    /* Exit if end of string or error */
    if( ch == -3 || ch == -4 ) break;

    /* Look out for PAD character */
    if( ch == -2 ) {

      /* Get next character from base 64 array */
      while ( (ch = lHyp_util_getBase64Char(in[i_in++])) == -1 ) ;

      /* Exit if end of string or error */
      if( ch == -3 || ch == -4 ) break;

      /* Decode a single character */
      out[i_out++] = obuf[0] ;

      /* Exit if PAD character */
      if( ch == -2 ) break;


      break;
    }

    /* Decode into positions 1.and 2 */
    obuf[1] |= ch >> 2;
    obuf[2] = ch << 6;

    /* Get next character from base 64 array */
    while ( (ch = lHyp_util_getBase64Char(in[i_in++])) == -1 ) ;

    /* Exit if end of string or error */
    if( ch == -3 || ch == -4 ) break;

    /* Look out for PAD character */
    if( ch == -2 ) {
      /* Decode two characters */
      out[i_out++] = obuf[0] ;
      out[i_out++] = obuf[1] ;
      break;
    }

    /* Decode into position 2 */
    obuf[2] |= ch;


    /* Decode three characters */
    out[i_out++] = obuf[0] ;
    out[i_out++] = obuf[1] ;
    out[i_out++] = obuf[2] ;
  }

  if( ch == -4 ) {
    gHyp_util_logError("Base64 decode error on '%s'",in);
  }

  return i_out ;

}

char *gHyp_util_readStream (  char *pStream,
			      char *pAnchor,
			      char **ppEndOfStream,
			      int  *pStreamLen,
			      sData *pStreamData,
			      sData **ppValue,
			      int *pContext,
			      int ss,
			      sLOGICAL isVector,
			      FILE *pp )  
{
  /* The function of this routine is to keep the buffer filled.
   *
   * The data can either come from a FILE* (pp) or a sData* (pStreamData).   
   * In either case, the buffer is 4 times the size 
   * of what can be read from pp or pStreamData ( STREAM_READ_SIZE ).
   *
   * The buffer is filled until it exceeds STREAM_READ_SIZE*3.
   * When the current processing pointer exceeds STREAM_READ_SIZE, then
   * the contents are shifted to the beginning of the buffer, and the
   * buffer is filled back to a point that exceeds STREAM_READ_SIZE*3.
   *
   *  pStream - the current processing position
   *  pAnchor - the start of the buffer
   *  pEndOfStream - the end of the buffer
   *  pStreamData - one possible source of data
   *  (ppValue,pContext,ss,isVector) - needed for pStreamData
   *  pp - another possible source of data 
   *
   * The return value is the current stream position.
   * When the data is exhausted, the value in ppEndOfStream is set to null,
   * so that subsequent calls to this routine will do nothing except just
   * return the pointer to the position of the start of the buffer
   * last read.
   */

  sData *pValue ;
  int n ;
  int dataLen ;
  char *pStr ;
  char *pBuf ;
  char *pEndOfStream = *ppEndOfStream ;

  /* If there's no more data to be read, then just return */
  if ( pEndOfStream == NULL ) return pStream ;

  /* If the current processing pointer has not moved passed 
   * STREAM_READ_SIZE, then we don't need to read more data in yet.
   */

  /* Get the current length of the stream left to process */
  *pStreamLen = (pEndOfStream - pStream) ;

  /* As long as there is enough data, return that position */
  if ( pStream < pEndOfStream &&
       pStream < (pAnchor + STREAM_READ_SIZE) ) return pStream ;

  /* Need to read some more data */
  if ( pStream > pAnchor && *pStreamLen > 0 ) {

    /* Shift the remaining stream back to the anchor position */
    memmove ( pAnchor, pStream, *pStreamLen ) ;

    /* Calculate new end of stream, where we will append data. */
    pEndOfStream = pAnchor + *pStreamLen ;

    *ppEndOfStream = pEndOfStream ;
    pStream = pAnchor ;

    /*gHyp_util_debug("[[%.*s]]",*pStreamLen, pStream);*/

  }

  /* We now read data until the FILE or sData source is exhausted or
   * until the pEndOfStream value exceeds MAX_INPUT_LENGTH*3
   */

  while ( pEndOfStream && (pEndOfStream < (pAnchor+(STREAM_READ_SIZE*3))) ) {

    if ( pStreamData ) {
   
      /* Data comes from a sData source */
      pValue = ppValue ? *ppValue : NULL ;

      /* Get next line */
      pValue = gHyp_data_nextValue ( pStreamData, 
				     pValue,
				     pContext,
				     ss ) ;
      *ppValue = pValue ;

      if ( !pValue ) { 
	pEndOfStream = NULL ;
	*ppEndOfStream = pEndOfStream ;
      }
      else {

	if ( gHyp_data_dataType ( pValue ) > TYPE_STRING ) {

	  pBuf = (char*) gHyp_data_buffer ( pValue, *pContext ) ;
	  n = gHyp_data_bufferLen ( pValue, *pContext ) ;

	  memcpy ( pEndOfStream, pBuf, n ) ;

	  dataLen = gHyp_data_dataLen ( pValue ) ;
	  *pContext += n/dataLen ;
	}
	else {
          n = gHyp_data_getStr (pValue, 
	 			pEndOfStream, 
				STREAM_READ_SIZE, 
				*pContext, 
				isVector ) ;
	}

        pStream = pAnchor ;
	pEndOfStream += n ;
	*ppEndOfStream = pEndOfStream ;
        *pStreamLen = (pEndOfStream - pStream) ;
      }
    }
    else if ( pp ) {

      /* Data comes from a file */
      pStr = fgets ( pEndOfStream, STREAM_READ_SIZE, pp ) ;
      if (!pStr) {
        *pStreamLen = (pEndOfStream - pStream) ;
	pEndOfStream = NULL ;
      }
      else {
	pEndOfStream += strlen ( pStr ) ;
        *pStreamLen = (pEndOfStream - pStream) ;
      }
      *ppEndOfStream = pEndOfStream ; 

    }
  }
  /*gHyp_util_debug("{{%.*s}}",*pStreamLen, pStream);*/

  return pStream ;
}

void gHyp_util_breakStream ( char *buffer, int bufLen, sData *pParent, sLOGICAL isLineBased )
{
  /* Basically, we divide a large buffer up into segments, we don't want any
   * segment to exceed INTERNAL_VALUE_SIZE, so that's where we snip it.
   * But, if isLineBase is true, we try to snip at the linefeed character.
   * However, if the line we are snipping is longer than INTERNAL_VALUE_SIZE,
   * then providing it would externalize to a string that is less that VALUE_SIZE,
   * we take the longer length.  
   */
  char 
    *pBuf ;

  sData
    *pValue ;

  int 
    i,
    width,
    width2,
    width3;

  char work[VALUE_SIZE+1];

  pBuf = buffer ;
  while ( bufLen > 0 ) {

    width = MIN ( bufLen, INTERNAL_VALUE_SIZE ) ;

    if ( isLineBased ) {

      /* The data should be logically divided by line feeds. */

      memcpy ( work, pBuf, width ) ;
      work[width] = '\0' ;

      /* Avoid ending in backslash */
      width2 = width ;
      while ( width2 > 0 && work[width2-1] == '\\' ) width2-- ;
      if ( width2 > 0 && width2 < width ) { 
	width = width2 ;
        work[width] = '\0' ;
      }

      /* Get the length of the next line */
      width2 = strcspn ( work, "\r\n" ) ;

      /* Keep the lf and cr */
      width2 += strspn ( work+width2, "\r\n" ) ;
	    
      if ( width2 > 0 ) {
	  
	if ( width2 < width ) {

	  /* Shorter is ok */
	  width = width2 ;
	}
	else if ( width2 > width ) { 

	  /* We'd like to store longer, but will it fit? */

	  width3 = width2 ;
	  for ( i=0;i<width2;i++ ) if ( work[i] <= 0 || !isprint(work[i]) ) width3+=3;
	  if ( width3 <= VALUE_SIZE ) 
	     /* Yes, it will fit - adjust size */
	     width = width2 ;
	}
      }
      pValue = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr_n ( pValue, (char*) work, width ) ;
    }
    else {
      pValue = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr_n ( pValue, (char*) pBuf, width ) ;
    }

    gHyp_data_append ( pParent, pValue ) ;

    bufLen -= width ;
    pBuf += width ;
  }
}

int gHyp_util_breakStringForMsg( char *buffer, int bufLen, int maxBufLen ) 
{

  /* Not used anywhere....  */

  char 
    *pBuf,
    *pBuf2,
    *pEndBuf,
    *pMaxEndBuf ;

  pBuf = buffer ;
  pBuf2 = pBuf ;
  pEndBuf = pBuf + bufLen ;
  pMaxEndBuf = pBuf + maxBufLen ;

  while ( pBuf < pEndBuf && pEndBuf < pMaxEndBuf ) {

    if ( *pBuf == '"' && 
	 pBuf < pEndBuf &&
	 *(pBuf+1) == ',' ) {

      /* Push everything ahead */
      pBuf2 = pEndBuf ;  /* Pointing to the NULL at the end */
      pEndBuf++ ;
      if ( pEndBuf >= pMaxEndBuf ) break ;
      *pEndBuf = '\0' ;

      /* Advance pBuf to the comma */
      pBuf++ ;

      /* Advance pBuf past the comma */
      pBuf++ ;

      /* Overwrite with the characters before */
      while ( pBuf2 > pBuf ) { *pBuf2 = *(pBuf2-1) ; pBuf2-- ; }

      /* Add the pipe */
      *pBuf = '|' ;
    }
    pBuf++ ;
  }

  /* Null terminate */
  *pBuf = '\0' ;

  /* Return new length */
  return (pBuf2-buffer);

}

char *gHyp_util_memmem(const void *buf, const void *pattern, size_t buflen, size_t len)
{
   char 
     *bf = (char *)buf,
     *pt = (char *)pattern,
     *p = bf;

      while (len <= (buflen - (p - bf)))
      {
            if (NULL != (p = (char*) memchr(p, (int)(*pt), buflen - (p - bf))))
            {
                  if (memcmp(p, pattern, len) == 0)
                        return p;
                  else  ++p;
            }
            else  break;
      }
      return NULL;
}


  
