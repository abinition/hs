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
 *	$Log: dateparse.c,v $
 *	Revision 1.9  2009-09-21 05:17:12  bergsma
 *	Comments
 *	
 *	Revision 1.8  2008-02-12 23:07:31  bergsma
 *	V3.8.2 (compiled with VS C 2008)
 *	
 *	Revision 1.7  2006-10-01 16:25:26  bergsma
 *	Added support for asctime() function and the means to parse it (in dateparse.c)
 *	
 *	Revision 1.6  2006/08/22 18:45:19  bergsma
 *	Resolve Win32 problem with qsort
 *	
 *	Revision 1.5  2005/04/13 13:40:18  bergsma
 *	Added YYYY-MM-DD HH:MM:SS format
 *	
 *	Revision 1.4  2004/10/16 04:34:24  bergsma
 *	Removed external variable 'daylight'.  Not used.
 *	
 *	Revision 1.3  2003/07/27 00:55:53  bergsma
 *	For FreeBSD, the timezone variable is already defined for ANSI_SOURCE
 *	
 *	Revision 1.2  2002/11/14 01:40:45  bergsma
 *	Removed ^M occurances and added Modification section.
 *	
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

#if defined ( AS_VMS ) && defined ( __cplusplus )
extern "C" void qsort(void *base, size_t nmemb, size_t size, 
		      int (*compar)(const void *, const void *)) ;
#else
#ifndef AS_WINDOWS
extern void qsort(void *base, size_t nmemb, size_t size, 
		  int (*compar)(const void *, const void *)) ;
#endif
#endif
/* date_parse - parse string dates into internal form
**
** Copyright (C) 1995 by Jef Poskanzer <jef@acme.com>.  All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provded that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/

/* For a while now I've been somewhat unhappy with the various
** date-parsing routines available - yacc-based, lex-based, ad-hoc.  Large
** code size and not very portable are the main complaints.  So I wrote a
** new one that just does a bunch of sscanf's until one matches.  Slow,
** but small and portable.  To figure out what formats to support I did a
** survey of Date: lines in a bunch of Usenet articles.  The following
** two formats accounted for more than 99% of all articles:
**     DD mth YY HH:MM:SS ampm zone
**     wdy, DD mth YY HH:MM:SS ampm zone
** I added Unix ctime() format and a few others:
**     wdy mth DD HH:MM:SS ampm zone YY
**     HH:MM:SS ampm zone DD mth YY
**     DD mth YY
**     HH:MM:SS ampm
**     DD/mth/YYYY:HH:MM:SS zone  (httpd common log format date)
** No-zone, no-seconds, and no-am/pm versions of each are also supported.
** Note that dd/mm/yy and mm/dd/yy are NOT supported - those formats are
** dumb.
*/

#ifdef ANSI_SOURCE
extern long timezone;
#endif

/*extern int daylight ;*/

#ifndef AS_VMS
#define bzero(p, n) memset(p, 0, n)
#endif

#undef DEBUG

#ifdef DEBUG
#define DP(str) (void) fprintf( stderr, "%s\n", str )
#else /* DEBUG */
#define DP(str) (0)
#endif /* DEBUG */

struct strlong {
  char* s;
  long l;
};

static void lHyp_dateparse_poundCase( char* str )
{
  for ( ; *str != '\0'; ++str ) {
    if ( isupper( *str ) )
      *str = tolower( *str );
  }
}

static int lHyp_dateparse_strlongCompare( const void *v1, const void *v2 )
{
  return strcmp( ((struct strlong*) v1)->s, ((struct strlong*) v2)->s );
}


static int lHyp_dateparse_strlongSrch ( char* str, 
					struct strlong* tab, 
					int n, long* lP )
{
  int i, h, l, r;
  
  l = 0;
  h = n - 1;
  for (;;) {
    i = ( h + l ) / 2;
    r = strcmp( str, tab[i].s );
    if ( r < 0 )
      h = i - 1;
    else if ( r > 0 )
      l = i + 1;
    else {
      *lP = tab[i].l;
      return 1;
    }
    if ( h < l )
      return 0;
  }
}


#define AMPM_NONE 0
#define AMPM_AM 1
#define AMPM_PM 2
static int lHyp_dateparse_ampmFix( int hour, int ampm )
{
  switch ( ampm ) {
  case AMPM_NONE:
    break;
  case AMPM_AM:
    if ( hour == 12 )
      hour = 0;
    break;
  case AMPM_PM:
    if ( hour != 12 )
      hour += 12;
    break;
    }
  return hour;
}


static int lHyp_dateparse_scanAMPM( char* str_ampm, long* ampmP )
{
  static struct strlong ampm_tab[] = {
    { "am", AMPM_AM }, { "pm", AMPM_PM },
  };
  static int sorted = 0;
  
  if ( ! sorted ) {
    qsort( ampm_tab, sizeof(ampm_tab)/sizeof(struct strlong),
	   sizeof(struct strlong), lHyp_dateparse_strlongCompare );
    sorted = 1;
  }
  lHyp_dateparse_poundCase( str_ampm );
  return lHyp_dateparse_strlongSrch( str_ampm, 
				     ampm_tab, 
				     sizeof(ampm_tab)/sizeof(struct strlong),
				     ampmP );
}


static int lHyp_dateparse_scanWday( char* str_wday, long* tm_wdayP )
{
  static struct strlong wday_tab[] = {
    { "sun", 0 }, { "sunday", 0 },
    { "mon", 1 }, { "monday", 1 },
    { "tue", 2 }, { "tuesday", 2 },
    { "wed", 3 }, { "wednesday", 3 },
    { "thu", 4 }, { "thursday", 4 },
    { "fri", 5 }, { "friday", 5 },
    { "sat", 6 }, { "saturday", 6 },
  };
  static int sorted = 0;
  
  if ( ! sorted ) {
    qsort( wday_tab, sizeof(wday_tab)/sizeof(struct strlong),
	   sizeof(struct strlong), lHyp_dateparse_strlongCompare );
    sorted = 1;
  }
  lHyp_dateparse_poundCase( str_wday );
  return lHyp_dateparse_strlongSrch( str_wday,
				     wday_tab, 
				     sizeof(wday_tab)/sizeof(struct strlong),
				     tm_wdayP );
}


static int lHyp_dateparse_scanMon( char* str_mon, long* tm_monP )
{
  static struct strlong mon_tab[] = {
    { "jan", 0 }, { "january", 0 },
    { "feb", 1 }, { "february", 1 },
    { "mar", 2 }, { "march", 2 },
    { "apr", 3 }, { "april", 3 },
    { "may", 4 },
    { "jun", 5 }, { "june", 5 },
    { "jul", 6 }, { "july", 6 },
    { "aug", 7 }, { "august", 7 },
    { "sep", 8 }, { "september", 8 },
    { "oct", 9 }, { "october", 9 },
    { "nov", 10 }, { "november", 10 },
    { "dec", 11 }, { "december", 11 },
  };
  static int sorted = 0;
  
  if ( ! sorted ) {
    qsort( mon_tab, sizeof(mon_tab)/sizeof(struct strlong),
	   sizeof(struct strlong), lHyp_dateparse_strlongCompare );
    sorted = 1;
  }
  lHyp_dateparse_poundCase( str_mon );
  return lHyp_dateparse_strlongSrch (str_mon, 
				     mon_tab, 
				     sizeof(mon_tab)/sizeof(struct strlong),
				     tm_monP );
}


static int lHyp_dateparse_scanGMToff( char* str_gmtoff, long* gmtoffP )
{
  static struct strlong gmtoff_tab[] = {
    { "gmt", 0L }, { "utc", 0L }, { "ut", 0L },
    { "0000", 0L }, { "+0000", 0L }, { "-0000", 0L },
    { "0100", 3600L }, { "+0100", 3600L }, { "-0100", -3600L },
    { "0200", 7200L }, { "+0200", 7200L }, { "-0200", -7200L },
    { "0300", 10800L }, { "+0300", 10800L }, { "-0300", -10800L },
    { "0400", 14400L }, { "+0400", 14400L }, { "-0400", -14400L },
    { "0500", 18000L }, { "+0500", 18000L }, { "-0500", -18000L },
    { "0600", 21600L }, { "+0600", 21600L }, { "-0600", -21600L },
    { "0700", 25200L }, { "+0700", 25200L }, { "-0700", -25200L },
    { "0800", 28800L }, { "+0800", 28800L }, { "-0800", -28800L },
    { "0900", 32400L }, { "+0900", 32400L }, { "-0900", -32400L },
    { "1000", 36000L }, { "+1000", 36000L }, { "-1000", -36000L },
    { "1100", 39600L }, { "+1100", 39600L }, { "-1100", -39600L },
    { "1200", 43200L }, { "+1200", 43200L }, { "-1200", -43200L },
    { "cet", 3600L }, { "ced", 7200L },	/* Central European time */
    { "mez", 3600L }, { "mesz", 7200L },	/* Mitteleuropdische Zeit */
    { "jst", 32400L }, { "jdt", 36000L },	/* Japan time */
    { "bst", -3600L },
    { "nst", -12600L },
    { "ast", -14400L }, { "adt", -10800L },
    { "est", -18000L }, { "edt", -14400L },
    { "cst", -21600L }, { "cdt", -18000L },
    { "mst", -25200L }, { "mdt", -21600L },
    { "pst", -28800L }, { "pdt", -25200L },
    { "yst", -32400L }, { "ydt", -28800L },
    { "hst", -36000L }, { "hdt", -32400L },
    { "a", -3600L }, { "b", -7200L }, { "c", -10800L }, { "d", -14400L },
    { "e", -18000L }, { "f", -21600L }, { "g", -25200L }, { "h", -28800L },
    { "i", -32400L }, { "k", -36000L }, { "l", -39600L }, { "m", -43200L },
    { "n", 3600L }, { "o", 7200L }, { "p", 10800L }, { "q", 14400L },
    { "r", 18000L }, { "s", 21600L }, { "t", 25200L }, { "u", 28800L },
    { "v", 32400L }, { "w", 36000L }, { "x", 39600L }, { "y", 43200L },
    { "z", 0L },
  };
  static int sorted = 0;
  
  if ( ! sorted ) {
    qsort( gmtoff_tab, sizeof(gmtoff_tab)/sizeof(struct strlong),
	   sizeof(struct strlong), lHyp_dateparse_strlongCompare );
    sorted = 1;
  }
  lHyp_dateparse_poundCase( str_gmtoff );
  return lHyp_dateparse_strlongSrch(str_gmtoff, 
				    gmtoff_tab,
				    sizeof(gmtoff_tab)/sizeof(struct strlong), 
				    gmtoffP );
}

static int gHyp_dateparse_isLeap( int year )
{
  if ( year < 70 )
    year += 2000;
  else if ( year < 1900 )
    year += 1900;
  return year % 400? ( year % 100 ? ( year % 4 ? 0 : 1 ) : 0 ) : 1;
}

static time_t lHyp_dateparse_tmToTime( struct tm* tmP,  int gmtoff )
{
  /* Basically the same as mktime(). */

  time_t
    t ;
  

  /* Got this definition, but doesn't produce the same result as the enclosed
   * algorithm.
   * 
  t =  tmP->tm_sec +
       tmP->tm_min*60 +
       tmP->tm_yday*86400 +
       (tmP->tm_year-70)*31536000 + 
       ((tmP->tm_year-69)/4)*86400 ;
  */

  static int monthtab[12] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
  
  /* Years since epoch, converted to days. */
  t = ( tmP->tm_year - 70 ) * 365;

  /* Leap days for previous years. */
  t += ( tmP->tm_year - 69 ) / 4;
  
  /* Days for the beginning of this month. */
  t += monthtab[tmP->tm_mon];
  
  /* Leap day for this year. */
  if ( tmP->tm_mon >= 2 && gHyp_dateparse_isLeap( tmP->tm_year ) )
    ++t;
  
  /* Days since the beginning of this month. */
  t += tmP->tm_mday - 1;
  
  /* Hours, minutes, and seconds. */
  t = t * 24 + tmP->tm_hour;
  t = t * 60 + tmP->tm_min;
  t = t * 60 + tmP->tm_sec;
  
  /* Adjust for zone */
  t -= gmtoff;

  if ( t < 0 ) t = 2147483647 ;
  return t;
}


time_t gHyp_dateparse_parse( char* str )
{
  time_t
    t,
    check_t,
    now;

  struct tm
    *check_tmP,
    *now_tmP,
    tm;

  char
    *cp,
    str_mon[500],
    str_wday[500],
    str_gmtoff[500],
    str_ampm[500];
  
  int 
    got_zone,
    delta,
    tm_sec,
    tm_min,
    tm_hour,
    tm_mday,
    tm_year;
  
  long 
    tm_mon,
    tm_wday, 
    ampm,
    gmtoff;
  
  /* Initialize tm with relevant parts of current local time. */
  now = time( (time_t*) 0 );
  now_tmP = localtime( &now );
  
  bzero( (char*) &tm, sizeof(struct tm) );
  tm.tm_sec = now_tmP->tm_sec;
  tm.tm_min = now_tmP->tm_min;
  tm.tm_hour = now_tmP->tm_hour;
  tm.tm_mday = now_tmP->tm_mday;
  tm.tm_mon = now_tmP->tm_mon;
  tm.tm_year = now_tmP->tm_year;
  tm.tm_isdst = 0 ;
  ampm = AMPM_NONE;
  got_zone = 0;

  /* Find local zone offset.  This is the only real area of
  ** non-portability, and it's only used for local times that don't
  ** specify a zone - those don't occur in email and netnews.
  */
#if defined (AS_SOLARIS) || defined (AS_WINDOWS) || defined (_ANSI_C_SOURCE) || defined (_DECC_V4_SOURCE)
#ifdef AS_WINDOWS
  _tzset();
#else
  tzset();
#endif
  gmtoff = -timezone;
#else /* SYSV */
  gmtoff = now_tmP->tm_gmtoff;
#endif 
  /*gHyp_util_debug("2. gmtoff = %d",gmtoff);*/

  /* Skip initial whitespace ourselves - sscanf is clumsy at this. */
  for ( cp = str; *cp == ' ' || *cp == '\t'; ++cp ) ;
  
  /* And do the sscanfs.  WARNING: you can add more formats here,
  ** but be careful!  You can easily screw up the parsing of existing
  ** formats when you add new ones.  The order is important.
  */
  
  if ( sscanf( cp, "%4d%2ld%2d:%2d%2d%2d %[^: 	\n]",
	       &tm_year, &tm_mon, &tm_mday, 
	       &tm_hour, &tm_min, &tm_sec, str_gmtoff ) == 7 &&
       lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "YYYYMMDD:HHMMSS zone" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon - 1;
      tm.tm_year = tm_year;
      tm.tm_hour = tm_hour ;
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      got_zone = 1;
    }

  else if ( sscanf( cp, "%4d%2ld%2d:%2d%2d%2d",
	       &tm_year, &tm_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec ) == 6 )
    {
      /*DP( "YYYYMMDD:HHMMSS" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon - 1;
      tm.tm_year = tm_year;
      tm.tm_hour = tm_hour ;
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }

  else if ( sscanf( cp, "%4d-%2ld-%2d %2d:%2d:%2d %[^: 	\n]",
	       &tm_year, &tm_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec, str_gmtoff ) == 6 )
    {
      /*DP( "YYYY-MM-DD HH:MM:SS zone" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon - 1;
      tm.tm_year = tm_year;
      tm.tm_hour = tm_hour ;
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      got_zone = 1;
    }
  else if ( sscanf( cp, "%4d-%2ld-%2d %2d:%2d:%2d",
	       &tm_year, &tm_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec ) == 6 )
    {
      /*DP( "YYYY-MM-DD HH:MM:SS" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon - 1;
      tm.tm_year = tm_year;
      tm.tm_hour = tm_hour ;
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }
  /* DD/mth/YY:HH:MM:SS zone */
  else if ( sscanf( cp, "%d/%[a-zA-Z]/%d:%d:%d:%d %[^: 	\n]",
	       &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
	       str_gmtoff ) == 7 &&
       lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
       lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "DD/mth/YY:HH:MM:SS zone" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      got_zone = 1;
    }
  
  /* DD-mth-YY HH:MM:SS ampm zone */
  else if ( ( ( sscanf( cp, "%d-%[a-zA-Z]-%d %d:%d:%d %[apmAPM] %[^: 	\n]",
			&tm_mday, str_mon, 
			&tm_year, &tm_hour, &tm_min, &tm_sec,
			str_ampm, str_gmtoff ) == 8 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d-%[a-zA-Z]-%d %d:%d:%d %[^: 	\n]",
		      &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
		      str_gmtoff ) == 7 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "DD-mth-YY HH:MM:SS ampm zone" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      got_zone = 1;
    }
  /* DD-mth-YY HH:MM ampm zone */
  else if ( ( ( sscanf( cp, "%d-%[a-zA-Z]-%d %d:%d %[apmAPM] %[^: 	\n]",
			&tm_mday, str_mon, &tm_year, &tm_hour, 
			&tm_min, str_ampm,
			str_gmtoff ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d-%[a-zA-Z]-%d %d:%d %[^: 	\n]",
		      &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		      str_gmtoff ) == 6 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "DD-mth-YY HH:MM ampm zone" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      got_zone = 1;
    }
  /* DD-mth-YY HH:MM:SS ampm */
  else if ( ( ( sscanf( cp, "%d-%[a-zA-Z]-%d %d:%d:%d %[apmAPM]",
			&tm_mday, str_mon, &tm_year, 
			&tm_hour, &tm_min, &tm_sec,
			str_ampm ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d-%[a-zA-Z]-%d %d:%d:%d",
		      &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		      &tm_sec ) == 6 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "DD-mth-YY HH:MM:SS ampm" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }
  /* DD-mth-YY HH:MM ampm */
  else if ( ( ( sscanf( cp, "%d-%[a-zA-Z]-%d %d:%d %[apmAPM]",
			&tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
			str_ampm ) == 6 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d-%[a-zA-Z]-%d %d:%d",
		      &tm_mday, str_mon, 
		      &tm_year, &tm_hour, &tm_min ) == 5 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "DD-mth-YY HH:MM ampm" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
    }
  
  /* DD mth YY HH:MM:SS ampm zone */
  else if ( ( ( sscanf( cp, "%d %[a-zA-Z] %d %d:%d:%d %[apmAPM] %[^: 	\n]",
			&tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
			str_ampm, str_gmtoff ) == 8 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d %[a-zA-Z] %d %d:%d:%d %[^: 	\n]",
		      &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec,
		      str_gmtoff ) == 7 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "DD mth YY HH:MM:SS ampm zone" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      got_zone = 1;
    }
  /* DD mth YY HH:MM ampm zone */
  else if ( ( ( sscanf( cp, "%d %[a-zA-Z] %d %d:%d %[apmAPM] %[^: 	\n]",
			&tm_mday, str_mon, &tm_year, 
			&tm_hour, &tm_min, str_ampm,
			str_gmtoff ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
            sscanf( cp, "%d %[a-zA-Z] %d %d:%d %[^: 	\n]",
		    &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    str_gmtoff ) == 6 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "DD mth YY HH:MM ampm zone" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      got_zone = 1;
    }
  /* DD mth YY HH:MM:SS ampm */
  else if ( ( ( sscanf( cp, "%d %[a-zA-Z] %d %d:%d:%d %[apmAPM]",
			&tm_mday, str_mon, &tm_year, 
			&tm_hour, &tm_min, &tm_sec,
			str_ampm ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d %[a-zA-Z] %d %d:%d:%d",
		      &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		      &tm_sec ) == 6 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "DD mth YY HH:MM:SS ampm" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }
  /* DD mth YY HH:MM ampm */
  else if ( ( ( sscanf( cp, "%d %[a-zA-Z] %d %d:%d %[apmAPM]",
			&tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
			str_ampm ) == 6 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d %[a-zA-Z] %d %d:%d",
		      &tm_mday, str_mon, 
		      &tm_year, &tm_hour, &tm_min ) == 5 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "DD mth YY HH:MM ampm" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
    }
  
  /* DD mth HH:MM:SS ampm */
  else if ( ( ( sscanf( cp, "%d %[a-zA-Z] %d:%d:%d %[apmAPM]",
			&tm_mday, str_mon, &tm_hour, &tm_min, &tm_sec,
			str_ampm ) == 6 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d %[a-zA-Z] %d:%d:%d",
		      &tm_mday, str_mon, &tm_hour, &tm_min,
		      &tm_sec ) == 5 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "DD mth HH:MM:SS ampm" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }
  /* DD mth HH:MM ampm */
  else if ( ( ( sscanf( cp, "%d %[a-zA-Z] %d:%d %[apmAPM]",
			&tm_mday, str_mon, &tm_hour, &tm_min,
			str_ampm ) == 5 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d %[a-zA-Z] %d:%d",
		      &tm_mday, str_mon, &tm_hour, &tm_min ) == 4 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "DD mth HH:MM ampm" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
    }
  
  /* HH:MM:SS ampm zone DD-mth-YY */
  else if ( ( ( sscanf( cp, "%d:%d:%d %[apmAPM] %[^: 	\n] %d-%[a-zA-Z]-%d",
			&tm_hour, &tm_min, &tm_sec, 
			str_ampm, str_gmtoff, &tm_mday,
			str_mon, &tm_year ) == 8 &&
	    lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d:%d %[^: 	\n] %d-%[a-zA-Z]-%d",
		      &tm_hour, &tm_min, &tm_sec, 
		      str_gmtoff, &tm_mday, str_mon,
		      &tm_year ) == 7 ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "HH:MM:SS ampm zone DD-mth-YY" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
    }
  /* HH:MM ampm zone DD-mth-YY */
  else if ( ( ( sscanf( cp, "%d:%d %[apmAPM] %[^: 	\n] %d-%[a-zA-Z]-%d",
			&tm_hour, &tm_min, str_ampm, 
			str_gmtoff, &tm_mday, str_mon,
			&tm_year ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d %[^: 	\n] %d-%[a-zA-Z]-%d",
		      &tm_hour, &tm_min, str_gmtoff, &tm_mday, str_mon,
		      &tm_year ) == 6 ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "HH:MM ampm DD-mth-YY" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
    }
  /* HH:MM:SS ampm DD-mth-YY */
  else if ( ( ( sscanf( cp, "%d:%d:%d %[apmAPM] %d-%[a-zA-Z]-%d",
			&tm_hour, &tm_min, &tm_sec, 
			str_ampm, &tm_mday, str_mon,
			&tm_year ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d:%d %d-%[a-zA-Z]-%d",
		      &tm_hour, &tm_min, &tm_sec, &tm_mday, str_mon,
		      &tm_year ) == 6 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "HH:MM:SS ampm DD-mth-YY" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
    }
  /* HH:MM ampm DD-mth-YY */
  else if ( ( ( sscanf( cp, "%d:%d %[apmAPM] %d-%[a-zA-Z]-%d",
			&tm_hour, &tm_min, str_ampm, &tm_mday, str_mon,
			&tm_year ) == 6 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d %d-%[a-zA-Z]-%d",
		      &tm_hour, &tm_min, &tm_mday, 
		      str_mon, &tm_year ) == 5 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "HH:MM ampm DD-mth-YY" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
    }
  
  /* HH:MM:SS ampm zone DD mth YY */
  else if ( ( ( sscanf( cp, "%d:%d:%d %[apmAPM] %[^: 	\n] %d %[a-zA-Z] %d",
			&tm_hour, &tm_min, &tm_sec, 
			str_ampm, str_gmtoff, &tm_mday,
			str_mon, &tm_year ) == 8 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d:%d %[^: 	\n] %d %[a-zA-Z] %d",
		      &tm_hour, &tm_min, &tm_sec, 
		      str_gmtoff, &tm_mday, str_mon,
		      &tm_year ) == 7 ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "HH:MM:SS ampm zone DD mth YY" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
    }
  /* HH:MM ampm zone DD mth YY */
  else if ( ( ( sscanf( cp, "%d:%d %[apmAPM] %[^: 	\n] %d %[a-zA-Z] %d",
			&tm_hour, &tm_min, str_ampm, 
			str_gmtoff, &tm_mday, str_mon,
			&tm_year ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d %[^: 	\n] %d %[a-zA-Z] %d",
		      &tm_hour, &tm_min, str_gmtoff, &tm_mday, str_mon,
		      &tm_year ) == 6 ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "HH:MM ampm DD mth YY" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
    }
  /* HH:MM:SS ampm DD mth YY */
  else if ( ( ( sscanf( cp, "%d:%d:%d %[apmAPM] %d %[a-zA-Z] %d",
			&tm_hour, &tm_min, &tm_sec, 
			str_ampm, &tm_mday, str_mon,
			&tm_year ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d:%d %d %[a-zA-Z] %d",
		      &tm_hour, &tm_min, &tm_sec, &tm_mday, str_mon,
		      &tm_year ) == 6 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "HH:MM:SS ampm DD mth YY" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
    }
  /* HH:MM ampm DD mth YY */
  else if ( ( ( sscanf( cp, "%d:%d %[apmAPM] %d %[a-zA-Z] %d",
			&tm_hour, &tm_min, str_ampm, &tm_mday, str_mon,
			&tm_year ) == 6 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%d:%d %d %[a-zA-Z] %d",
		      &tm_hour, &tm_min, &tm_mday, 
		      str_mon, &tm_year ) == 5 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "HH:MM ampm DD mth YY" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
    }
  
  /* wdy, DD-mth-YY HH:MM:SS ampm zone */
  else if ((( sscanf( cp, 
		  "%[a-zA-Z], %d-%[a-zA-Z]-%d %d:%d:%d %[apmAPM] %[^: 	\n]",
		      str_wday, &tm_mday, str_mon, 
		      &tm_year, &tm_hour, &tm_min,
		      &tm_sec, str_ampm, str_gmtoff ) == 9 &&
	      lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	    sscanf( cp, "%[a-zA-Z], %d-%[a-zA-Z]-%d %d:%d:%d %[^: 	\n]",
		    str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		    &tm_sec, str_gmtoff ) == 8 ) &&
	   lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	   lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	   lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "wdy, DD-mth-YY HH:MM:SS ampm zone" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      got_zone = 1;
    }
  /* wdy, DD-mth-YY HH:MM ampm zone */
  else if ( ( ( sscanf( cp, 
		 "%[a-zA-Z], %d-%[a-zA-Z]-%d %d:%d %[apmAPM] %[^: 	\n]",
			str_wday, &tm_mday, str_mon, 
			&tm_year, &tm_hour, &tm_min,
			str_ampm, str_gmtoff ) == 8 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z], %d-%[a-zA-Z]-%d %d:%d %[^: 	\n]",
		      str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		      str_gmtoff ) == 7 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "wdy, DD-mth-YY HH:MM ampm zone" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      got_zone = 1;
    }
  /* wdy, DD-mth-YY HH:MM:SS ampm */
  else if ( ( ( sscanf( cp, "%[a-zA-Z], %d-%[a-zA-Z]-%d %d:%d:%d %[apmAPM]",
			str_wday, &tm_mday, str_mon, 
			&tm_year, &tm_hour, &tm_min,
			&tm_sec, str_ampm ) == 8 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z], %d-%[a-zA-Z]-%d %d:%d:%d",
		      str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		      &tm_sec ) == 7 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "wdy, DD-mth-YY HH:MM:SS ampm" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }
  /* wdy, DD-mth-YY HH:MM ampm */
  else if ( ( ( sscanf( cp, "%[a-zA-Z], %d-%[a-zA-Z]-%d %d:%d %[apmAPM]",
			str_wday, &tm_mday, str_mon, 
			&tm_year, &tm_hour, &tm_min,
			str_ampm ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z], %d-%[a-zA-Z]-%d %d:%d",
		      str_wday, &tm_mday, str_mon, &tm_year, &tm_hour,
		      &tm_min ) == 6 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "wdy, DD-mth-YY HH:MM ampm" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
    }
  
  /* wdy, DD mth YY HH:MM:SS ampm zone */
  else if ( ( ( sscanf( cp, 
		   "%[a-zA-Z], %d %[a-zA-Z] %d %d:%d:%d %[apmAPM] %[^: 	\n]",
			str_wday, &tm_mday, str_mon, 
			&tm_year, &tm_hour, &tm_min,
			&tm_sec, str_ampm, str_gmtoff ) == 9 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z], %d %[a-zA-Z] %d %d:%d:%d %[^: 	\n]",
		      str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		      &tm_sec, str_gmtoff ) == 8 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "wdy, DD mth YY HH:MM:SS ampm zone" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      got_zone = 1;
    }
  /* wdy, DD mth YY HH:MM ampm zone */
  else if ( ( ( sscanf( cp, 
			"%[a-zA-Z], %d %[a-zA-Z] %d %d:%d %[apmAPM] %[^: 	\n]",
			str_wday, &tm_mday, str_mon, 
			&tm_year, &tm_hour, &tm_min,
			str_ampm, str_gmtoff ) == 8 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z], %d %[a-zA-Z] %d %d:%d %[^: 	\n]",
		      str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		      str_gmtoff ) == 7 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "wdy, DD mth YY HH:MM ampm zone" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      got_zone = 1;
    }
  /* wdy, DD mth YY HH:MM:SS ampm */
  else if ( ( ( sscanf( cp, "%[a-zA-Z], %d %[a-zA-Z] %d %d:%d:%d %[apmAPM]",
			str_wday, &tm_mday, str_mon, 
			&tm_year, &tm_hour, &tm_min,
			&tm_sec, str_ampm ) == 8 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z], %d %[a-zA-Z] %d %d:%d:%d",
		      str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min,
		      &tm_sec ) == 7 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "wdy, DD mth YY HH:MM:SS ampm" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }
  /* wdy, DD mth YY HH:MM ampm */
  else if ( ( ( sscanf( cp, "%[a-zA-Z], %d %[a-zA-Z] %d %d:%d %[apmAPM]",
			str_wday, &tm_mday, str_mon, 
			&tm_year, &tm_hour, &tm_min,
			str_ampm ) == 7 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z], %d %[a-zA-Z] %d %d:%d",
		      str_wday, &tm_mday, str_mon, &tm_year, &tm_hour,
		      &tm_min ) == 6 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "wdy, DD mth YY HH:MM ampm" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
    }
  
  /* wdy mth DD HH:MM:SS ampm zone YY */
  else if ( ( ( sscanf( cp, 
	       "%[a-zA-Z] %[a-zA-Z] %d %d:%d:%d %[apmAPM] %[^: 	\n] %d",
			str_wday, str_mon, &tm_mday, 
			&tm_hour, &tm_min, &tm_sec,
			str_ampm, str_gmtoff, &tm_year ) == 9 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, 
		      "%[a-zA-Z] %[a-zA-Z] %d %d:%d:%d %[^: 	\n] %d",
		      str_wday, str_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec,
		      str_gmtoff, &tm_year ) == 8 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "wdy mth DD HH:MM:SS ampm zone YY" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mon = tm_mon;
      tm.tm_mday = tm_mday;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
      got_zone = 1;
      tm.tm_year = tm_year;
    }
  /* wdy mth DD HH:MM ampm zone YY */
  else if ( ( ( sscanf( cp, "%[a-zA-Z] %[a-zA-Z] %d %d:%d %[apmAPM] %[^: 	\n] %d",
			str_wday, str_mon, &tm_mday, &tm_hour, &tm_min,
			str_ampm, str_gmtoff, &tm_year ) == 8 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z] %[a-zA-Z] %d %d:%d %[^: 	\n] %d",
		      str_wday, str_mon, &tm_mday, &tm_hour, &tm_min,
		      str_gmtoff, &tm_year ) == 7 ) &&
	    lHyp_dateparse_scanWday( str_wday, &tm_wday ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) &&
	    lHyp_dateparse_scanGMToff( str_gmtoff, &gmtoff ) )
    {
      /*DP( "wdy mth DD HH:MM ampm zone YY" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mon = tm_mon;
      tm.tm_mday = tm_mday;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
      got_zone = 1;
      tm.tm_year = tm_year;
    }
  /* ANSI asctime: Sun Nov 6 08:49:37 1994  
     wdy mth DD HH:MM   YY */
  else if ( ( sscanf( cp, "%[a-zA-Z] %[a-zA-Z] %d %d:%d:%d %d",
			str_wday, str_mon, &tm_mday, &tm_hour, &tm_min,
			&tm_sec, &tm_year ) == 7 ) &&
	    ( lHyp_dateparse_scanWday( str_wday, &tm_wday ) ) &&
	    ( lHyp_dateparse_scanMon( str_mon, &tm_mon ) ) )
    {
      /*DP( "wdy mth DD HH:MM:SS zone YY" );*/
      tm.tm_wday = tm_wday;
      tm.tm_mon = tm_mon;
      tm.tm_mday = tm_mday;
      tm.tm_hour = tm_hour ;
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec ;
      tm.tm_year = tm_year;
    }

  /* mth DD HH:MM:SS ampm */
  else if ( ( ( sscanf( cp, "%[a-zA-Z] %d %d:%d:%d %[apmAPM]",
			str_mon, &tm_mday, &tm_hour, &tm_min, &tm_sec,
			str_ampm ) == 6 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z] %d %d:%d:%d",
		      str_mon, &tm_mday, &tm_hour, &tm_min,
		      &tm_sec ) == 5 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "mth DD HH:MM:SS ampm" );*/
      tm.tm_mon = tm_mon;
      tm.tm_mday = tm_mday;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }
  /* mth DD HH:MM ampm */
  else if ( ( ( sscanf( cp, "%[a-zA-Z] %d %d:%d %[apmAPM]",
			str_mon, &tm_mday, &tm_hour, &tm_min,
			str_ampm ) == 5 &&
		lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	      sscanf( cp, "%[a-zA-Z] %d %d:%d",
		      str_mon, &tm_mday, &tm_hour, &tm_min ) == 4 ) &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "mth DD HH:MM ampm" );*/
      tm.tm_mon = tm_mon;
      tm.tm_mday = tm_mday;
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
    }
  
  /* DD-mth-YY */
  else if ( sscanf( cp, "%d-%[a-zA-Z]-%d",
		    &tm_mday, str_mon, &tm_year ) == 3 &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "DD-mth-YY" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = 0;
      tm.tm_min = 0;
      tm.tm_sec = 0;
    }
  /* DD mth YY */
  else if ( sscanf( cp, "%d %[a-zA-Z] %d",
		    &tm_mday, str_mon, &tm_year ) == 3 &&
	    lHyp_dateparse_scanMon( str_mon, &tm_mon ) )
    {
      /*DP( "DD mth YY" );*/
      tm.tm_mday = tm_mday;
      tm.tm_mon = tm_mon;
      tm.tm_year = tm_year;
      tm.tm_hour = 0;
      tm.tm_min = 0;
      tm.tm_sec = 0;
    }
  
  /* HH:MM:SS ampm */
  else if ( ( sscanf( cp, "%d:%d:%d %[apmAPM]",
		      &tm_hour, &tm_min, &tm_sec, str_ampm ) == 4 &&
	      lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	    sscanf( cp, "%d:%d:%d", &tm_hour, &tm_min, &tm_sec ) == 3 )
    {
      /*DP( "HH:MM:SS ampm" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = tm_sec;
    }
  /* HH:MM ampm */
  else if ( ( sscanf( cp, "%d:%d %[apmAPM]", &tm_hour, &tm_min,
		      str_ampm ) == 3 &&
	      lHyp_dateparse_scanAMPM( str_ampm, &ampm ) ) ||
	    sscanf( cp, "%d:%d", &tm_hour, &tm_min ) == 2 )
    {
      /*DP( "HH:MM" );*/
      tm.tm_hour = lHyp_dateparse_ampmFix( tm_hour, ampm );
      tm.tm_min = tm_min;
      tm.tm_sec = 0;
    }
  else
    return (time_t) -1;
  
  if ( tm.tm_year > 1900 )
    tm.tm_year -= 1900;
  else if ( tm.tm_year < 70 )
    tm.tm_year += 100;
  
  /* Determine the UTC value from our input. */
  t = lHyp_dateparse_tmToTime( &tm, gmtoff );

  /* If a zone was not specified (as is often the case), then it is implied that the zone is the
   * same as the localtime.   If daylight savings was in effect during the time just parsed, then
   * it will change the value of 't', the true UTC value.  
   *
   * We assume that 't' is correct.  We check for a DST adjustment by calling localtime( &t ) and 
   * then checking the 'tm struct' against what we just used to determine 't'.  If the check amount 
   * is less, we adjust 't' up by one hour.  If more, we adjust 't' down by one hour.
   */
  if ( !got_zone ) {

    /* Check the time. */

    check_tmP = localtime( &t ) ;

    /* Determine the UTC value from our input. */
    check_t = lHyp_dateparse_tmToTime( check_tmP, gmtoff ) ;

    /*gHyp_util_debug( "(%d) %d %d %d %d %d %d %d %d %d",
		     t,
		     tm.tm_sec,
                     tm.tm_min,
                     tm.tm_hour,
                     tm.tm_mday,
                     tm.tm_mon, 
                     tm.tm_year,
                     tm.tm_wday,
                     tm.tm_yday,
                     tm.tm_isdst ) ;
    */
    /*gHyp_util_debug( "(%d) %d %d %d %d %d %d %d %d %d",
		     check_t,
		     check_tmP->tm_sec,
                     check_tmP->tm_min,
                     check_tmP->tm_hour,
                     check_tmP->tm_mday,
                     check_tmP->tm_mon, 
                     check_tmP->tm_year,
                     check_tmP->tm_wday,
                     check_tmP->tm_yday,
                     check_tmP->tm_isdst ) ;
    */
    /*gHyp_util_debug("%d",t-check_t);*/

    delta = (int) ( t - check_t ) ;
    t += delta ;
  }

  return t ;
}
