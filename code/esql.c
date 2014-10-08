#include <sqlhdr.h>
#include <sqliapi.h>
static const char _Cn2[] = "c";
static const char _Cn1[] = "s";
#line 1 "esql.ec"
/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/

/**********************	ENVIRONMENT DEFINITIONS ******************************/

/* 
 * EXEC SQL include sqltypes;
 */
#line 12 "esql.ec"

#line 12 "esql.ec"
#line 1 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/sqltypes.h"
/****************************************************************************
 *
 *                               IBM INC.
 *
 *                           PROPRIETARY DATA
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 2007. All rights reserved.
 *
 *  Title:	sqltypes.h
 *  Description:
 *		type definition
 *
 ***************************************************************************
 */

#ifndef CCHARTYPE
#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

/***********************
 * ++++ CAUTION ++++
 * Any new type to be added to the following lists should not
 * have the following bit pattern (binary short):
 *
 *	xxxxx111xxxxxxxx
 *
 * where x can be either 0 or 1.
 *
 * This is due to the use of the bits as SQLNONULL, SQLHOST and SQLNETFLT
 * (see below).
 *
 * FAILURE TO DO SO WOULD RESULT IN POSSIBLE ERRORS DURING CONVERSIONS.
 *
 ***********************/

 /* C language types */

#define CCHARTYPE	100
#define CSHORTTYPE	101
#define CINTTYPE	102
#define CLONGTYPE	103
#define CFLOATTYPE	104
#define CDOUBLETYPE	105
#define CDECIMALTYPE	107
#define CFIXCHARTYPE	108
#define CSTRINGTYPE	109
#define CDATETYPE	110
#define CMONEYTYPE	111
#define CDTIMETYPE	112
#define CLOCATORTYPE    113
#define CVCHARTYPE	114
#define CINVTYPE	115
#define CFILETYPE	116
#define CINT8TYPE	117
#define CCOLLTYPE       118
#define CLVCHARTYPE     119
#define CFIXBINTYPE     120
#define CVARBINTYPE     121
#define CBOOLTYPE       122
#define CROWTYPE        123
#define CLVCHARPTRTYPE  124
#define CBIGINTTYPE     125
#define CTYPEMAX         26

#define USERCOLL(x)	((x))

#define COLLMASK        0x007F  /* mask out CTYPEDCOLL or CCLIENTCOLL */
                                /* bit set for CCOLLTYPE or CROWTYPE */
#define ISCOLLECTIONVAR(n)  (((n) & COLLMASK) == CCOLLTYPE)
#define ISROWVAR(n)         (((n) & COLLMASK) == CROWTYPE)
#define ISCOLL_OR_ROWVAR(n)   ((ISCOLLECTIONVAR(n)) || (ISROWVAR(n)))
#define CCLIENTCOLL     SQLCLIENTCOLL /* client collection bit */
#define ISCLIENTCOLLECTION(n) (ISCOLLECTIONVAR(n) && ((n) & CCLIENTCOLL))
#define ISCLIENTCOMPLEX(n)    ((ISCLIENTCOLLECTION(n)) || (ISROWVAR(n)))

/*
 * The following are for client side only. They are included here
 * because of the above related definitions.
 */
#define CTYPEDCOLL       0x0080  /* typed collection bit */
#define CTYPEDCOLLUNMASK 0xFF7F  /* unmask typed collection bit */
#define ISTYPEDCOLLECTION(n)  (ISCOLLECTIONVAR(n) && ((n) & CTYPEDCOLL))
#define ISTYPEDROW(n)         (ISROWVAR(n) && ((n) & CTYPEDCOLL))
#define ISTYPEDCOLL_OR_ROW(n)  ( (ISTYPEDCOLLECTION(n)) || (ISTYPEDROW(n)) )

/*
 * Define all possible database types
 *   include C-ISAM types here as well as in isam.h
 */

#define SQLCHAR		0
#define SQLSMINT	1
#define SQLINT		2
#define SQLFLOAT	3
#define SQLSMFLOAT	4
#define SQLDECIMAL	5
#define SQLSERIAL	6
#define SQLDATE		7
#define SQLMONEY	8
#define SQLNULL		9
#define SQLDTIME	10
#define SQLBYTES	11
#define SQLTEXT		12
#define SQLVCHAR	13
#define SQLINTERVAL	14
#define SQLNCHAR	15
#define SQLNVCHAR	16
#define SQLINT8		17
#define SQLSERIAL8	18
#define SQLSET          19
#define SQLMULTISET     20
#define SQLLIST         21
#define SQLROW          22
#define SQLCOLLECTION   23
#define SQLROWREF   	24
/*
 * Note: SQLXXX values from 25 through 39 are reserved to avoid collision
 *       with reserved PTXXX values in that same range. See p_types_t.h
 *
 * REFSER8: create tab with ref: referenced serial 8 rsam counter
 *	this is essentially a SERIAL8, but is an additional rsam counter
 *	this type only lives in the system catalogs and when read from
 *	disk is converted to SQLSERIAL8 with CD_REFSER8 set in ddcol_t
 *      ddc_flags we must distinguish from SERIAL8 to allow both
 *      counters in one tab
 */
#define SQLUDTVAR   	40
#define SQLUDTFIXED   	41
#define SQLREFSER8   	42

/* These types are used by FE, they are not real major types in BE */
#define SQLLVARCHAR     43
#define SQLSENDRECV     44
#define SQLBOOL         45
#define SQLIMPEXP       46
#define SQLIMPEXPBIN    47

/* This type is used by the UDR code to track default parameters,
   it is not a real major type in BE */
#define SQLUDRDEFAULT   48

#define SQLDBSENDRECV   49
#define SQLSRVSENDRECV  50

/* Type used by DESCRIBE INPUT stmt to indicate input parameters whose
   types cannot be determined by the server */
#define SQLUNKNOWN      51

#define SQLINFXBIGINT   52 /* Changing to avoid collision with ODBC */
#define SQLBIGSERIAL    53

#define SQLMAXTYPES     54

#define SQLLABEL        SQLINT

#define SQLTYPE		0xFF	/* type mask		*/

#define SQLNONULL	0x0100	/* disallow nulls	*/
/* a bit to show that the value is from a host variable */
#define SQLHOST		0x0200	/* Value is from host var. */
#define SQLNETFLT	0x0400	/* float-to-decimal for networked backend */
#define SQLDISTINCT	0x0800	/* distinct bit		*/
#define SQLNAMED	0x1000	/* Named row type vs row type */
#define SQLDLVARCHAR    0x2000  /* Distinct of lvarchar */
#define SQLDBOOLEAN     0x4000  /* Distinct of boolean */
#define SQLCLIENTCOLL   0x8000  /* Collection is processed on client */

/* we are overloading SQLDBOOLEAN for use with row types */
#define SQLVARROWTYPE   0x4000  /* varlen row type */

/* We overload SQLNAMED for use with constructor type, this flag
 * distinguish constructor types from other UDT types.
 *
 * Please do not test this bit directly, use macro ISCSTTYPE() instead.
 */
#define SQLCSTTYPE	0x1000	/* constructor type flag */

#define TYPEIDMASK      (SQLTYPE | SQLDISTINCT | SQLNAMED | \
                         SQLDLVARCHAR | SQLDBOOLEAN )

#define SIZCHAR		1
#define SIZSMINT	2
#define SIZINT		4
#define SIZFLOAT	(sizeof(double))
#define SIZSMFLOAT	(sizeof(float))
#define SIZDECIMAL	17	/* decimal(32) */
#define SIZSERIAL	4
#define SIZDATE		4
#define SIZMONEY	17	/* decimal(32) */
#define SIZDTIME	7	/* decimal(12,0) */
#define SIZVCHAR	1
#define SIZINT8		(sizeof(short) + sizeof(muint) * 2)
#define SIZSERIAL8	SIZINT8
#define SIZBIGINT    8
#define SIZBIGSERIAL 8
#define SIZCOLL		sizeof (ifx_collection_t)
#define SIZSET		SIZCOLL
#define SIZMULTISET	SIZCOLL
#define SIZLIST		SIZCOLL
#define SIZROWREF	sizeof (ifx_ref_t)

#define MASKNONULL(t)	((t) & (SQLTYPE))
#define ISSQLTYPE(t)	(MASKNONULL(t) >= SQLCHAR && MASKNONULL(t) < SQLMAXTYPES)

/*
 * SQL types macros
 */
#define ISDECTYPE(t)		(MASKNONULL(t) == SQLDECIMAL || \
				 MASKNONULL(t) == SQLMONEY || \
				 MASKNONULL(t) == SQLDTIME || \
				 MASKNONULL(t) == SQLINTERVAL)
#define ISNUMERICTYPE(t)	(MASKNONULL(t) == SQLSMINT || \
				 MASKNONULL(t) == SQLINT || \
				 MASKNONULL(t) == SQLINT8 || \
				 MASKNONULL(t) == SQLFLOAT || \
				 MASKNONULL(t) == SQLSMFLOAT || \
				 MASKNONULL(t) == SQLMONEY || \
				 MASKNONULL(t) == SQLSERIAL || \
				 MASKNONULL(t) == SQLSERIAL8 || \
  				 MASKNONULL(t) == SQLDECIMAL || \
				 MASKNONULL(t) == SQLINFXBIGINT || \
				 MASKNONULL(t) == SQLBIGSERIAL)
#define ISBLOBTYPE(type)	(ISBYTESTYPE (type) || ISTEXTTYPE(type))
#define ISBYTESTYPE(type)	(MASKNONULL(type) == SQLBYTES)
#define ISTEXTTYPE(type)	(MASKNONULL(type) == SQLTEXT)
#define ISSQLHOST(t)            (((t) & SQLHOST) == SQLHOST)

#define ISVCTYPE(t)		(MASKNONULL(t) == SQLVCHAR || \
				 MASKNONULL(t) == SQLNVCHAR)
#define ISCHARTYPE(t)		(MASKNONULL(t) == SQLCHAR || \
				 MASKNONULL(t) == SQLNCHAR)
#define ISNSTRINGTYPE(t)	(MASKNONULL(t) == SQLNCHAR || \
				 MASKNONULL(t) == SQLNVCHAR)

#define ISSTRINGTYPE(t)		(ISVCTYPE(t) || ISCHARTYPE(t))

#define	ISUDTVARTYPE(t)		(MASKNONULL(t) == SQLUDTVAR)
#define	ISUDTFIXEDTYPE(t)	(MASKNONULL(t) == SQLUDTFIXED)
#define	ISUDTTYPE(t)		(ISUDTVARTYPE(t) || ISUDTFIXEDTYPE(t))

#define	ISCOMPLEXTYPE(t)	(ISROWTYPE(t) || ISCOLLTYPE(t))
#define	ISROWTYPE(t)		(MASKNONULL(t) == SQLROW)
#define	ISLISTTYPE(t)		(MASKNONULL(t) == SQLLIST)
#define	ISMULTISETTYPE(t)	(MASKNONULL(t) == SQLMULTISET)
#define	ISREFTYPE(t)		(MASKNONULL(t) == SQLROWREF)
#define  ISSERIAL(t)    (MASKNONULL(t) == SQLSERIAL)
#define  ISSERIAL8(t)      (MASKNONULL(t) == SQLSERIAL8)
#define ISBIGSERIAL(t)          (MASKNONULL(t) == SQLBIGSERIAL)
#define ISBIGINT(t)             (MASKNONULL(t) == SQLINFXBIGINT)
#define	ISSETTYPE(t)		(MASKNONULL(t) == SQLSET)
#define	ISCOLLECTTYPE(t)	(MASKNONULL(t) == SQLCOLLECTION)
#define ISCOLLTYPE(t)		(ISSETTYPE(t) || ISMULTISETTYPE(t) ||\
				 ISLISTTYPE(t) || ISCOLLECTTYPE(t))

#define ISSERIALTYPE(t)		(((t) & SQLTYPE) == SQLSERIAL || \
				 ((t) & SQLTYPE) == SQLSERIAL8 || \
				 ((t) & SQLTYPE) == SQLREFSER8 || \
				 ((t) & SQLTYPE) == SQLBIGSERIAL)

#define ISDISTINCTTYPE(t)	((t) & SQLDISTINCT)
#define ISCSTTYPE(t)		(ISUDTTYPE(t) && ((t) & SQLCSTTYPE))

/* these macros are used to distinguish NLS char types and non-nls (ASCII)
 * char types
 */
#define ISNONNLSCHAR(t)		(MASKNONULL(t) == SQLCHAR || \
				 MASKNONULL(t) == SQLVCHAR)

/* these macros should be used in case statements
 */
#define CHARCASE		SQLCHAR: case SQLNCHAR
#define VCHARCASE		SQLVCHAR: case SQLNVCHAR

#define UDTCASE		 	SQLUDTVAR: case SQLUDTFIXED

/*
 * C types macros
 */
#define ISBLOBCTYPE(type)	(ISLOCTYPE(type) || ISFILETYPE(type))
#define ISLOCTYPE(type)		(MASKNONULL(type) == CLOCATORTYPE)
#define ISFILETYPE(type)	(MASKNONULL(type) == CFILETYPE)
#define ISLVCHARCTYPE(type)     (MASKNONULL(type) == CLVCHARTYPE)
#define ISLVCHARCPTRTYPE(type)  (MASKNONULL(type) == CLVCHARPTRTYPE)
#define ISFIXBINCTYPE(type)     (MASKNONULL(type) == CFIXBINTYPE)
#define ISVARBINCTYPE(type)     (MASKNONULL(type) == CVARBINTYPE)
#define ISBOOLCTYPE(type)       (MASKNONULL(type) == CBOOLTYPE)


#define ISOPTICALCOL(type)	(type == 'O')

#define DEFDECIMAL	9	/* default decimal(16) size */
#define DEFMONEY	9	/* default decimal(16) size */

#define SYSPUBLIC	"public"

/*
 * if an SQL type is specified, convert to default C type
 *  map C int to either short or long
 */


#define TYPEMAX	SQLMAXTYPES

extern int2 sqlctype[];

#define toctype(ctype, type) \
    { \
    if (type == CINTTYPE) \
    { \
      if (sizeof(mint) == sizeof(mlong)) \
          { ctype = type = CLONGTYPE; } \
      else if (sizeof(mint) == sizeof(int2)) \
          { ctype = type = CSHORTTYPE; } \
      else \
          { ctype = CLONGTYPE; type = CINTTYPE; } \
    } \
    else if (type >= 0 && type < TYPEMAX) \
        ctype = sqlctype[type]; \
    else \
        ctype = type; \
    }



/* Extended ID definitions for predefined UDT's */
/* These can not be changed because sqli is using
 * them.  If we change them, the client has to recompile.
 * NOTE: This order must match the definitions in boot90.sql
 */

#define XID_LVARCHAR            1
#define XID_SENDRECV            2
#define XID_IMPEXP              3
#define XID_IMPEXPBIN           4
#define XID_BOOLEAN             5
#define XID_POINTER             6
#define XID_INDEXKEYARRAY       7
#define XID_RTNPARAMTYPES	8
#define XID_SELFUNCARGS         9
#define XID_BLOB                10
#define XID_CLOB                11
#define XID_LOLIST              12
#define XID_STAT                15
#define XID_CLIENTBINVAL        16

/* Max size definitions for the predefined UDT's.
 * Only a few are currently defined.
 */
#define SIZINDEXKEYARRAY	1024
#define SIZLVARCHAR		2048
#define SIZRTNPARAMTYPES	4096
#define SIZSTAT             272

/* Alignment required by predefined UDT's.
 * Only a few are currently defined.  At a minimum,
 * all with alignment not 1 should be defined here
 * and used throughout the code.
 */
#define ALNINDEXKEYARRAY	4
#define ALNSTAT             8


#define USER_XTDTYPEID_START	2048

/* These macros should be used to test lvarchar and distinct of lvarchar */

#define ISLVARCHARXTYPE(t, xid)  (ISUDTTYPE((t)) && (xid) == XID_LVARCHAR)
#define ISDISTINCTLVARCHAR(t)	((t) & SQLDLVARCHAR)
#define LIKELVARCHARXTYPE(t,xid) ((ISLVARCHARXTYPE(t,xid))||\
				   ISDISTINCTLVARCHAR(t))

#define ISSMARTBLOB(type, xid)  (ISUDTTYPE((type)) && \
				    ((xid == XID_CLOB) || (xid == XID_BLOB)))

/* These macros should be used to test boolean and distinct of boolean */
#define ISBOOLEANXTYPE(t, xid)  (ISUDTTYPE((t)) && (xid) == XID_BOOLEAN)
#define ISDISTINCTBOOLEAN(t)	(((t) & SQLDBOOLEAN) && \
				    (ISUDTTYPE(t)))
#define LIKEBOOLEANXTYPE(t,xid) ((ISBOOLEANXTYPE(t,xid))||\
				  ISDISTINCTBOOLEAN(t))

#define ISFIXLENGTHTYPE(t)	(!ISBYTESTYPE(t) && !ISTEXTTYPE(t) \
					&& !ISCOMPLEXTYPE(t) \
					&& !ISUDTVARTYPE(t) \
					&& !ISVCTYPE(t))

#ifdef __cplusplus
}
#endif

#endif /* CCHARTYPE */
#line 400 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/sqltypes.h"
/* 
 * EXEC SQL include locator;
 */
#line 13 "esql.ec"

#line 13 "esql.ec"
#line 1 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/locator.h"
/****************************************************************************
 *
 *                               IBM INC.
 *
 *                           PROPRIETARY DATA
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * Copyright IBM Corporation 2002, 2007. All rights reserved.
 *
 *  Title:	locator.h
 *
 *  Description:
 *              'locator.h' defines 'loc_t' the locator struct.
 *
 ***************************************************************************
 */

#ifndef LOCATOR_INCL		/* avoid multiple includes */
#define LOCATOR_INCL

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

#include "int8.h"

/*
Locators are used to store TEXT or BYTE fields (blobs) in ESQL
programs.  The "loc_t" structure is described below.  Fields denoted
USER should be set by the user program and will be examined by the DBMS
system.  Those denoted SYSTEM are set by the system and may be examined
by the user program.  Those denoted INTERNAL contain data only the
system manupilates and examines.

If "loc_loctype" is set to LOCMEMORY, then the blob is stored in
primary memory.  The memory buffer is pointed to by the variant
"loc_buffer".  The field "loc_bufsize" gives the size of "loc_buffer".
If the "loc_bufsize" is set to "-1" and "loc_mflags" is set to "0"
and the locator is used for a fetch, memory is obtained using "malloc"
and "loc_buffer" and "loc_bufsize" are set.

If "loc_loctype" is set to LOCFILE, then the blob is stored in a file.
The file descriptor of an open operating system file is specified in
"loc_fd".

If "loc_loctype" is set to LOCFNAME, the the blob is stored in a file
and the name of the file is given.  The DBMS will open or created the
file at the correct time and in the correct mode.

If the "loc_loctype" is set to LOCUSER, "loc_(open/close/read/write)"
are called.  If the blob is an input to a SQL statement, "loc_open" is
called with the parameter "LOC_RONLY".  If the blob is an output target
for an SQL statement, "loc_open" is called with the parameter
"LOC_WONLY".

"loc_size" specifies the maximum number of bytes to use when the
locator is an input target to an SQL statement. It specifies the number
of bytes returned if the locator is an output target.  If "loc_loctype"
is LOCFILE or LOCUSER, it can be set to -1 to indicate transfer until
end-of-file.

"loc_indicator" is set by the user to -1 to indicate a NULL blob.  It
will be  set to -1 if a NULL blob is retrieved.  If the blob to be
retrieved will not fit in the space provided, the indicator contains
the size of the blob.

"loc_status" is the status return of locator operations.

"loc_type" is the "blob" type (SQLTEXT, SQLBYTES, ...).

"loc_user_env" is a pointer for the user's private use. It is neither
set nor examined by the system.  "loc_user_env" as well as the
"loc_union" fieds may be used by user supplied routines to store and
communicate information.
*/

typedef struct tag_loc_t
    {
    int2 loc_loctype;		/* USER: type of locator - see below	*/
    union			/* variant on 'loc'                     */
	{
	struct			/* case LOCMEMORY                       */
	    {
	    int4  lc_bufsize;	/* USER: buffer size */
	    char *lc_buffer;	/* USER: memory buffer to use		*/
	    char *lc_currdata_p;/* INTERNAL: current memory buffer	*/
	    mint   lc_mflags;	/* USER/INTERNAL: memory flags		*/
				/*			(see below)	*/
	    } lc_mem;

	struct			/* cases L0CFNAME & LOCFILE		*/
	    {
	    char *lc_fname;	/* USER: file name			*/
	    mint  lc_mode;	/* USER: perm. bits used if creating	*/
	    mint  lc_fd;	/* USER: os file descriptior		*/
	    int4  lc_position;	/* INTERNAL: seek position		*/
	    } lc_file;
	} lc_union;

    int4  loc_indicator;	/* USER SYSTEM: indicator		*/
    int4  loc_type;		/* USER SYSTEM: type of blob		*/
    int4  loc_size;		/* USER SYSTEM: num bytes in blob or -1	*/
    mint  loc_status;		/* SYSTEM: status return of locator ops	*/
    char *loc_user_env;		/* USER: for the user's PRIVATE use	*/
    int4  loc_xfercount;	/* INTERNAL/SYSTEM: Transfer count	*/

#if defined(__STDC__) || defined(__cplusplus)
    mint (*loc_open)(struct tag_loc_t *loc, mint flag, mint bsize);
    mint (*loc_close)(struct tag_loc_t *loc);
    mint (*loc_read)(struct tag_loc_t *loc, char *buffer, mint buflen);
    mint (*loc_write)(struct tag_loc_t *loc, char *buffer, mint buflen);
#else
    mint (*loc_open)();
    mint (*loc_close)();
    mint (*loc_read)();
    mint (*loc_write)();
#endif /* defined(__STDC__) || defined(__cplusplus) */

    mint   loc_oflags;		/* USER/INTERNAL: see flag definitions below */
    } ifx_loc_t;


#if !defined(IFX_DISABLE_LOC_T)
#if !defined(_AIX) || defined(IFX_COL_T) || !defined(_H_LOCALEDEF)

typedef  ifx_loc_t loc_t;

#if defined(_AIX)
/* ensure that col_t is taken from catalog.h */
#ifndef IFX_LOC_T
#define IFX_LOC_T
#endif /* IFX_LOC_T */

/* prevent the including /usr/include/sys/localedef31.h after locator.h */
#ifndef _H_LOCALEDEF
#define _H_LOCALEDEF
#endif /* _H_LOCALEDEF */
#endif /* _AIX */

#endif /* !_AIX || !_H_LOCALEDEF */
#endif /* !IFX_DISABLE_LOC_T */

#define	loc_fname	lc_union.lc_file.lc_fname
#define loc_fd		lc_union.lc_file.lc_fd
#define loc_position	lc_union.lc_file.lc_position	
#define loc_bufsize	lc_union.lc_mem.lc_bufsize
#define loc_buffer	lc_union.lc_mem.lc_buffer
#define loc_currdata_p	lc_union.lc_mem.lc_currdata_p
#define loc_mflags	lc_union.lc_mem.lc_mflags

/* Enumeration literals for loc_loctype */

#define LOCMEMORY	1		/* memory storage */
#define LOCFNAME	2		/* File storage with file name */
#define LOCFILE		3		/* File storage with fd */
#define	LOCUSER		4		/* User define functions */

/* passed to loc_open and stored in loc_oflags */
#define LOC_RONLY	0x1		/* read only */
#define LOC_WONLY	0x2		/* write only */

/* LOC_APPEND can be set when the locator is created
 * if the file is to be appended to instead of created
 */
#define LOC_APPEND	0x4		/* write with append */
#define LOC_TEMPFILE	0x8		/* 4GL tempfile blob */

/* LOC_USEALL can be set to force the maximum size of the blob to always be
 * used when the blob is an input source.  This is the same as setting the
 * loc_size field to -1.  Good for LOCFILE or LOCFNAME blobs only.
 */
#define LOC_USEALL	0x10		/* ignore loc_size field */
#define LOC_DESCRIPTOR	0x20		/* BLOB is optical descriptor */


/* passed to loc_open and stored in loc_mflags */
#define LOC_ALLOC	0x1		/* free and alloc memory */

/* Flags to indicate if file is on the server or client */
#define LO_CLIENT_FILE  0x00000000
#define LO_SERVER_FILE  0x10000000

/*******************************************************************************
 * File open flags used for operating system file open via
 *	- ifx_lo_copy_to_lo
 *	- ifx_lo_copy_to_file
 *	- ifx_file_to_file
 *
 ******************************************************************************/

#define LO_O_EXCL             0x00000001 /* fail if file exists */
#define LO_O_APPEND           0x00000002 /* append to end of file */
#define LO_O_TRUNC            0x00000004 /* turncate to 0 if file exists */
#define LO_O_RDWR             0x00000008 /* read/write (default) */
#define LO_O_RDONLY           0x00000010 /* read-only (from-flags only) */
#define LO_O_WRONLY           0x00000020 /* write-only (to-flags only) */
#define LO_O_BINARY           0x00000040 /* binary-mode (default) */
#define LO_O_TEXT             0x00000080 /* text-mode (default off)*/


/*******************************************************************************
 *
 * Smartblob Definitions
 *
 ******************************************************************************/

/******************************************************************************
 * Open flags: see ESQL/C documentation for further explanation.
 *
 * LO_APPEND - Positions the seek position to end-of-file + 1. Affects write
 * operations. Reads can still seek anywhere in the LO. Writes always append.
 *
 * LO_SEQUENTIAL - If set overrides optimizer decision. Indicates that
 * reads are sequential in either forward or reverse direction.
 *
 * LO_RANDOM - If set overrides optimizer decision. Indicates that I/O is
 * random and that the system should not read-ahead.
 * LO_FORWARD - Only used for sequential access. Indicates that the sequential
 * access will be in a forward direction, i.e. from low offset to higher offset.
 * LO_REVERSE - Only used for sequential access. Indicates that the sequential
 * access will be in a reverse direction.
 *
 * LO_BUFFER - If set overrides optimizer decision. I/O goes through the
 * buffer pool.
 *
 * LO_NOBUFFER - If set then I/O does not use the buffer pool.
 ******************************************************************************/

#define LO_APPEND	0x1
#define LO_WRONLY 	0x2
#define LO_RDONLY 	0x4	/* default */
#define LO_RDWR   	0x8
#define LO_DIRTY_READ	0x10

#define LO_RANDOM	0x20	/* default is determined by optimizer */
#define LO_SEQUENTIAL	0x40	/* default is determined by optimizer */

#define LO_FORWARD	0x80	/* default */
#define LO_REVERSE	0x100

#define LO_BUFFER	0x200	/* default is determined by optimizer */
#define LO_NOBUFFER	0x400	/* default is determined by optimizer */
#define LO_NODIRTY_READ 0x800

#define LO_LOCKALL      0x1000 /* default */
#define LO_LOCKRANGE    0x2000

#ifndef REMOVE_VECTORIO
#define LO_VECTORIO     0x4000
#endif

/*
 *  Another set of open flags are defined to make the flags more meaningful
*/

#define LO_OPEN_APPEND       LO_APPEND
#define LO_OPEN_WRONLY       LO_WRONLY
#define LO_OPEN_RDONLY       LO_RDONLY     /* default */
#define LO_OPEN_RDWR         LO_RDWR
#define LO_OPEN_DIRTY_READ   LO_DIRTY_READ

#define LO_OPEN_RANDOM       LO_RANDOM      /* default is determined by optimizer */
#define LO_OPEN_SEQUENTIAL   LO_SEQUENTIAL  /* default is determined by optimizer */

#define LO_OPEN_FORWARD      LO_FORWARD    /* default */
#define LO_OPEN_REVERSE      LO_REVERSE

#define LO_OPEN_BUFFER       LO_BUFFER     /* default is determined by optimizer */
#define LO_OPEN_NOBUFFER     LO_NOBUFFER   /* default is determined by optimizer */
#define LO_OPEN_NODIRTY_READ LO_NODIRTY_READ

#define LO_OPEN_LOCKALL      LO_LOCKALL  /* default */
#define LO_OPEN_LOCKRANGE    LO_LOCKRANGE

/* When setting the MI_LO_NOBUFFER flag for write operations, please
 * don't set this flag if the object is small. It usually causes a synchronous
 * flush of the log and a synchronous flush of pages written - this is
 * very slow. Instead use buffered I/O for small writes.
 */
#define LO_NOBUFFER_SIZE_THRESHOLD 8080

/*see Informix internal use file /vobs/tristarp/incl/sblob.h for other flags*/

/*******************************************************************************
 * LO create-time flags:
 *
 * Bitmask - Set/Get via ifx_lo_specset_flags() on ifx_lo_create_spec_t.
 *
 * New applications should use the flags which begin LO_ATTR_
 * The second set of flags are defined for backward compatibility only.
 ******************************************************************************/

#define LO_ATTR_LOG                          0x0001
#define LO_ATTR_NOLOG                        0x0002
#define LO_ATTR_DELAY_LOG                    0x0004
#define LO_ATTR_KEEP_LASTACCESS_TIME         0x0008
#define LO_ATTR_NOKEEP_LASTACCESS_TIME       0x0010
#define LO_ATTR_HIGH_INTEG                   0x0020
#define LO_ATTR_MODERATE_INTEG               0x0040
#define LO_ATTR_TEMP                         0x0080

/* these 7 values are defined for backward compatibility only */
#define LO_LOG                          0x0001
#define LO_NOLOG                        0x0002
#define LO_DELAY_LOG                    0x0004
#define LO_KEEP_LASTACCESS_TIME         0x0008
#define LO_NOKEEP_LASTACCESS_TIME       0x0010
#define LO_HIGH_INTEG                   0x0020
#define LO_MODERATE_INTEG               0x0040
#define LO_TEMP                         0x0080

/* these flags are defined to make the create flags more meaningful */
#define LO_CREATE_LOG                          0x0001
#define LO_CREATE_NOLOG                        0x0002
#define LO_CREATE_DELAY_LOG                    0x0004
#define LO_CREATE_KEEP_LASTACCESS_TIME         0x0008
#define LO_CREATE_NOKEEP_LASTACCESS_TIME       0x0010
#define LO_CREATE_HIGH_INTEG                   0x0020
#define LO_CREATE_MODERATE_INTEG               0x0040
#define LO_CREATE_TEMP                         0x0080

/*******************************************************************************
 * Symbolic constants for the "lseek" routine
 ******************************************************************************/

#define LO_SEEK_SET 0   /* Set curr. pos. to "offset"           */
#define LO_SEEK_CUR 1   /* Set curr. pos. to current + "offset" */
#define LO_SEEK_END 2   /* Set curr. pos. to EOF + "offset"     */

/*******************************************************************************
 * Symbolic constants for lo_lock and lo_unlock routines.
 ******************************************************************************/

#define LO_SHARED_MODE          1       /* ISSLOCK */
#define LO_EXCLUSIVE_MODE       2       /* ISXLOCK */

#define LO_MAX_END              -1
#define LO_CURRENT_END          -2

/*******************************************************************************
 * ifx_lo_create_spec_t:
 *
 * This is an opaque structure used for creating smartblobs. The
 * user may examin and/or set certain fields herein by using
 * ifx_lo_spec[set|get]_* accessor functions. Prototypes for these acessors
 * are in incl/sqlhdr.h
 *
 ******************************************************************************/

typedef struct ifx_lo_create_spec_s ifx_lo_create_spec_t;



/*******************************************************************************
 * ifx_lo_t: A dummy opaque representation of the smartblob structure
 *
 * This can be used for stack or in-line structure declarations.
 *
 ******************************************************************************/
#define SB_LOCSIZE 72 /* length of ifx_lo_t */

typedef struct ifx_lo_ts
{
    char dummy[SB_LOCSIZE];
} ifx_lo_t;

/*******************************************************************************
 * ifx_lo_stat:
 *
 * This is an opaque structure used in querying attribtes of smartblobs. The
 * user may examin fields herein by using ifx_lo_stat_* accessor functions.
 * Prototypes for these acessors are in incl/sqlhdr.h
 *
 * The accessors are defined as follows:
 *     ifx_lo_stat_size: contains the size of the LO in bytes.
 *     ifx_lo_stat_uid: reserved for future use: the user id for the
 *         owner of the LO.
 *     ifx_lo_stat_atime: the time of last access. This is only maintained if
 *         the LO_KEEP_LASTACCESS_TIME flag is set for the LO.
 *         Resolution is seconds.
 *     ifx_lo_stat_mtime: the time of last modification. Resolution is
 *         seconds.
 *     ifx_lo_stat_ctime: the time of the last status change (this includes
 *         updates, changes in ownership, and changes to the number of
 *         references).  Resolution is seconds. See Appendix B2.11,
 *         Future Embedded- language feature issues, Smartblob API
 *         functions using lofd, for enhancements that extend support
 *         for named and external LOs.
 *     ifx_lo_stat_refcnt: the number of pointers to this LO - when 0 the LO is
 *         typically deleted. See deletion criteria.
 *     ifx_lo_stat_cspec: a pointer to the opaque create spec for this object.
 *         (see ifx_lo_spec[get|set]_ accessors for details.)
 *     ifx_lo_stat_type: the 8 byte code for the LO's type
 *
 ******************************************************************************/

typedef struct ifx_lo_stat_s ifx_lo_stat_t;

#ifdef __cplusplus
}
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif	/* LOCATOR_INCL */
#line 422 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/locator.h"
/* 
 * EXEC SQL include datetime;
 */
#line 14 "esql.ec"

#line 14 "esql.ec"
#line 1 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/datetime.h"
/****************************************************************************
 *
 *                               IBM INC.
 *
 *                           PROPRIETARY DATA
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 2002. All rights reserved.
 *
 *  Title:	datetime.h
 *  Description:
 *		include for DATETIME and INTERVAL
 *
 ***************************************************************************
 */

#ifndef _DATETIME_H
#define _DATETIME_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#include "ifxtypes.h"

#include "decimal.h"

typedef struct dtime
	{
	int2 dt_qual;
#ifdef OSF_MLS
        ix_dec_t dt_dec;
#else
	dec_t dt_dec;
#endif
	}	dtime_t;

typedef struct intrvl
	{
	int2 in_qual;
#ifdef OSF_MLS
        ix_dec_t in_dec;
#else
	dec_t in_dec;
#endif
	}	intrvl_t;

/* time units for datetime qualifier */

#define TU_YEAR 0
#define TU_MONTH 2
#define TU_DAY 4
#define TU_HOUR 6
#define TU_MINUTE 8
#define TU_SECOND 10
#define TU_FRAC 12
#define TU_F1 11
#define TU_F2 12
#define TU_F3 13
#define TU_F4 14
#define TU_F5 15

/* TU_END - end time unit in datetime qualifier */
/* TU_START - start time unit in datetime qualifier */
/* TU_LEN - length in  digits of datetime qualifier */

#define TU_END(qual) (qual & 0xf)
#define TU_START(qual) ((qual>>4) & 0xf)
#define TU_LEN(qual) ((qual>>8) & 0xff)

/* TU_ENCODE - encode length, start and end time unit to form qualifier */
/* TU_DTENCODE - encode datetime qual */
/* TU_IENCODE - encode interval qual */

#define TU_ENCODE(len,s,e) (((len)<<8) | ((s)<<4) | (e))
#define TU_DTENCODE(s,e) TU_ENCODE(((e)-(s)+((s)==TU_YEAR?4:2)), s, e)
#define TU_IENCODE(len,s,e) TU_ENCODE(((e)-(s)+(len)),s,e)
#define TU_FLEN(len) (TU_LEN(len)-(TU_END(len)-TU_START(len)))


/* TU_FIELDNO - field number of the given TU_ macro.
   (e.g. year is 0, month is 1, day is 2, TU_F1 is  7, TU_F5 is 11)
 */

#define TU_FIELDNO(f)   (f > TU_SECOND ? (5+(f-TU_SECOND)) : (f/2))


/* TU_CURRQUAL - default qualifier used by current */

#define TU_CURRQUAL TU_ENCODE(17,TU_YEAR,TU_F3)

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DATETIME_H */
#line 113 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/datetime.h"
/* 
 * EXEC SQL include decimal;
 */
#line 15 "esql.ec"

#line 15 "esql.ec"
#line 1 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/decimal.h"
/****************************************************************************
 *
 *                               IBM INC.
 *
 *                           PROPRIETARY DATA
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 2002. All rights reserved.
 *
 *  Title:	decimal.h
 *		(created from isam/decimal.h version 6.1)
 *  Description:
 *		Header file for decimal data type.
 *
 ***************************************************************************
 */

#ifndef _DECIMAL_H
#define _DECIMAL_H

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

/*
 * Unpacked Format (format for program usage)
 *
 *    Signed exponent "dec_exp" ranging from  -64 to +63
 *    Separate sign of mantissa "dec_pos"
 *    Base 100 digits (range 0 - 99) with decimal point
 *	immediately to the left of first digit.
 */

#define DECSIZE 16
#define DECUNKNOWN -2

struct decimal
    {
    int2 dec_exp;		/* exponent base 100		*/
    int2 dec_pos;		/* sign: 1=pos, 0=neg, -1=null	*/
    int2 dec_ndgts;		/* number of significant digits	*/
    char  dec_dgts[DECSIZE];	/* actual digits base 100	*/
    };
#ifdef OSF_MLS
typedef struct decimal ix_dec_t;
#else
typedef struct decimal dec_t;
#endif

/*
 * Datatypes for DataBlade developers
 */
typedef struct decimal mi_money;
typedef struct decimal mi_decimal;

/*
 *  A decimal null will be represented internally by setting dec_pos
 *  equal to DECPOSNULL
 */

#define DECPOSNULL	(-1)

/*
 * DECLEN calculates minumum number of bytes
 * necessary to hold a decimal(m,n)
 * where m = total # significant digits and
 *	 n = significant digits to right of decimal
 */

#define DECLEN(m,n)	(((m)+((n)&1)+3)/2)
#define DECLENGTH(len)	DECLEN(PRECTOT(len),PRECDEC(len))

/*
 * DECPREC calculates a default precision given
 * number of bytes used to store number
 */

#define DECPREC(size)	(((size-1)<<9)+2)

/* macros to look at and make encoded decimal precision
 *
 *  PRECTOT(x)		return total precision (digits total)
 *  PRECDEC(x) 		return decimal precision (digits to right)
 *  PRECMAKE(x,y)	make precision from total and decimal
 */

#define PRECTOT(x)	(((x)>>8) & 0xff)
#define PRECDEC(x)	((x) & 0xff)
#define PRECMAKE(x,y)	(((x)<<8) + (y))

/*
 * Packed Format  (format in records in files)
 *
 *    First byte =
 *	  top 1 bit = sign 0=neg, 1=pos
 *	  low 7 bits = Exponent in excess 64 format
 *    Rest of bytes = base 100 digits in 100 complement format
 *    Notes --	This format sorts numerically with just a
 *		simple byte by byte unsigned comparison.
 *		Zero is represented as 80,00,00,... (hex).
 *		Negative numbers have the exponent complemented
 *		and the base 100 digits in 100's complement
 */

#ifdef __cplusplus
}
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif /* _DECIMAL_H */
#line 128 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/decimal.h"
/* 
 * EXEC SQL include sqlca;
 */
#line 16 "esql.ec"

#line 16 "esql.ec"
#line 1 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/sqlca.h"
/****************************************************************************
 *
 *                               IBM INC.
 *
 *                           PROPRIETARY DATA
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 2002,2007. All rights reserved.
 *
 *  Title:	sqlca.h
 *  Description:
 *		SQL Control Area
 *
 ***************************************************************************
 */

#ifndef SQLCA_INCL
#define SQLCA_INCL

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef STATIC
#include "ifxdef.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

typedef struct sqlca_s
{
    int4 sqlcode;
#ifdef DB2CLI
    char sqlerrm[600]; /* error message parameters */
#else /* DB2CLI */
    char sqlerrm[72]; /* error message parameters */
#endif /* DB2CLI */
    char sqlerrp[8];
    int4 sqlerrd[6];
		    /* 0 - estimated number of rows returned */
		    /* 1 - serial value after insert or  ISAM error code */
		    /* 2 - number of rows processed */
		    /* 3 - estimated cost */
		    /* 4 - offset of the error into the SQL statement */
		    /* 5 - rowid after insert  */
#ifdef _FGL_
    char sqlawarn[8];
#else
    struct sqlcaw_s
	{
	char sqlwarn0; /* = W if any of sqlwarn[1-7] = W */
	char sqlwarn1; /* = W if any truncation occurred or
				database has transactions or
			        no privileges revoked */
	char sqlwarn2; /* = W if a null value returned or
				ANSI database */
	char sqlwarn3; /* = W if no. in select list != no. in into list or
				turbo backend or no privileges granted */
	char sqlwarn4; /* = W if no where clause on prepared update, delete or
				incompatible float format */
	char sqlwarn5; /* = W if non-ANSI statement */
	char sqlwarn6; /* = W if server is in data replication secondary mode */
	char sqlwarn7; /* = W if database locale is different from proc_locale
			*/
	} sqlwarn;
#endif
    } ifx_sqlca_t;

#define SQLNOTFOUND 100

#ifdef DB2CLI
#define sqlca ifmxsqlca
extern struct sqlca_s sqlca;
#else /* DB2CLI */
extern struct sqlca_s sqlca;
#endif /* DB2CLI */

#ifdef __cplusplus
}
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif /* SQLCA_INCL */
#line 99 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/sqlca.h"
/* 
 * EXEC SQL include sqlda;
 */
#line 17 "esql.ec"

#line 17 "esql.ec"
#line 1 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/sqlda.h"
/****************************************************************************
 *
 *                               IBM INC.
 *
 *                           PROPRIETARY DATA
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 2002. All rights reserved.
 *
 *  Title:	sqlda.h
 *  Description:
 *		SQL Data Description Area
 *
 ***************************************************************************
 */


#ifndef _SQLDA
#define _SQLDA

#ifdef __BORLANDC__
#pragma option -a8
#else
#pragma pack (8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ifxtypes.h"

typedef struct sqlvar_struct
    {
    int2 sqltype;		/* variable type		*/
    int4 sqllen;		/* length in bytes		*/
    char *sqldata;		/* pointer to data		*/
    int2 *sqlind;		/* pointer to indicator	*/
    char  *sqlname;		/* variable name		*/
    char  *sqlformat;	/* reserved for future use 	*/
    int2 sqlitype;		/* ind variable type		*/
    int2 sqlilen;		/* ind length in bytes		*/
    char *sqlidata;		/* ind data pointer		*/
    int4  sqlxid;       /* extended id type             */
    char *sqltypename;  /* extended type name           */
    int2 sqltypelen;   /* length of extended type name */
    int2 sqlownerlen;  /* length of owner name         */
    int2 sqlsourcetype;/* source type for distinct of built-ins */
    char *sqlownername; /* owner name                   */
    int4 sqlsourceid;	/* extended id of source type   */

    /*
     * sqlilongdata is new.  It supports data that exceeds the 32k
     * limit.  sqlilen and sqlidata are for backward compatibility
     * and they have maximum value of <32K.
     */
    char *sqlilongdata;	/* for data field beyond 32K	*/
    int4 sqlflags;		/* for internal use only        */
    void *sqlreserved;	/* reserved for future use      */
    } ifx_sqlvar_t;

typedef struct sqlda
    {
    int2 sqld;
    ifx_sqlvar_t *sqlvar;
    char desc_name[19];	/* descriptor name 			*/
    int2 desc_occ;		/* size of sqlda structure 	*/
    struct sqlda *desc_next;	/* pointer to next sqlda struct */
    void *reserved;		/* reserved for future use	*/
    } ifx_sqlda_t;

#ifdef __cplusplus
}
#endif

#ifdef __BORLANDC__
#pragma option -a-
#else
#pragma pack ()
#endif

#endif /* _SQLDA */
#line 87 "C:/Program Files/IBM/Informix/Client-SDK/incl/esql/sqlda.h"
#line 18 "esql.ec"

typedef struct sqlda SQLDA ;
#define WARNNOTIFY        1
#define NOWARNNOTIFY      0

#define LCASE(c) (isupper(c) ? tolower(c) : (c))
#define BUFFSZ 256

extern char statement[80];

/*
 * EXEC SQL BEGIN DECLARE SECTION;
 */
#line 28 "esql.ec"
#line 29 "esql.ec"
  loc_t lcat_descr;
  loc_t lcat_picture;
/*
 * EXEC SQL END DECLARE SECTION;
 */
#line 31 "esql.ec"


/**********************	HYPERSCRIPT INTERFACE ********************************/

extern unsigned short    guDebugFlags ;  /* Debug flags */
#define NULL ((void *) 0)

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

char   *gpzDmlCommands[] = { "SELECT", "select",
			     "INSERT", "insert",
			     "UPDATE", "update",
			     "DELETE", "delete" } ;

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

#include "typdef.h"
#include "hypdef.h"

void        gHyp_data_append ( sData*, sData* ) ;
void        gHyp_data_deleteValues ( sData* ) ;
sBYTE       gHyp_data_getDataType ( sData* ) ;
double      gHyp_data_getDouble ( sData*, int, sLOGICAL ) ;
int         gHyp_data_getInt ( sData*, int, sLOGICAL ) ;
char*       gHyp_data_getLabel ( sData* ) ;
int         gHyp_data_getStr ( sData*, char*, int, int, sLOGICAL) ;
int         gHyp_data_getSubScript ( sData * ) ;
sData*      gHyp_data_getValue ( sData *, int, sLOGICAL ) ;
sData*      gHyp_data_new ( char * ) ;
sData*      gHyp_data_nextValue ( sData*, sData*, int*, int ) ;
void        gHyp_data_setBool ( sData*, sLOGICAL ) ;
void        gHyp_data_setDouble ( sData*, double ) ;
void        gHyp_data_setInt ( sData*, int ) ;
void        gHyp_data_setNull ( sData* ) ;
void        gHyp_data_setReference ( sData*, char*, sData* ) ;
void        gHyp_data_setStr ( sData*, char* ) ;
sData*      gHyp_frame_findVariable ( sInstance*, sFrame*, char* );
sData*      gHyp_frame_createVariable ( sFrame *,char*);
sParse*     gHyp_frame_parse ( sFrame *pFrame ) ;
sStack*     gHyp_frame_stack ( sFrame *pFrame ) ;
void        gHyp_instance_error ( sInstance*, char*, char*, ... ) ;
sFrame*     gHyp_instance_frame ( sInstance *) ;
sData*      gHyp_instance_setStatus ( sInstance *, char * ) ;
void        gHyp_instance_warning ( sInstance*,char*,char*, ...) ;
int         gHyp_parse_argCount ( sParse * ) ;
void        gHyp_parse_operand ( sParse *, sCode*, sInstance* ) ;
sData*      gHyp_stack_popRvalue ( sStack *, sInstance * ) ;
sData*      gHyp_stack_popRdata ( sStack *, sInstance * ) ;
void        gHyp_stack_push ( sStack *, sData * ) ;
sLOGICAL    gHyp_util_logInfo ( const char *fmt, ... ) ;
sLOGICAL    gHyp_util_logDebug ( int, unsigned short,const char *fmt, ...);
sLOGICAL    gHyp_util_logError ( const char *fmt, ... ) ;
void        gHyp_util_trimWhiteSpace ( char * ) ;

void *malloc(int size);
void free(void *ptr);void *realloc(void *ptr, int size);
int sprintf(char *str, const char *format, ...);
int strncmp(const char *s1, const char *s2, int n);
char *strncpy(char *dest, const char *src, int n);

#ifdef _NOT_DEFINED_
static SQLDA *lHyp_esql_setup ( )
{
  SQLDA
    *sqlda ;
  int
    i ;

  /* Allocate the select and bind descriptors */
  sqlda = sqlald ( MAX_SQL_ITEMS, TOKEN_SIZE, TOKEN_SIZE ) ;
  if ( sqlda ) {
    for ( i=0; i<MAX_SQL_ITEMS; i++ ) {
      sqlda->I[i] = (char *) malloc ( sizeof ( char ) ) ;
      sqlda->V[i] = (char *) malloc ( 1 ) ;
    }
  }
  return sqlda ;
}

static void lHyp_esql_cleanup ( SQLDA *sqlda )
{
  int
    i ;

  if ( !sqlda ) return ;

  /* Cleanup */
  for ( i=0; i<MAX_SQL_ITEMS; i++ ) {
    if ( sqlda->V[i] != (char *) 0 ) free ( sqlda->V[i] ) ;
    free ( sqlda->I[i] ) ;
  }

  /* Free descriptor space */
  if ( sqlda ) sqlclu ( sqlda ) ;

  return ;
}

static sLOGICAL lHyp_esql_bind ( sInstance *pAI,
				sFrame *pFrame,
				SQLDA *bind_dp )
{
  int
    i,
    n,
    len ;

  double
    d ;

  sBYTE
    dataType ;

  sLOGICAL
    status = TRUE ;

  char
    bindVariable[TOKEN_SIZE+1],
    value[VALUE_SIZE+1] ;

  sData
    *pVariable,
    *pData ;

  /* Set the maximum number of bind variables in the SQL statement */
  bind_dp->N = bind_dp->F ;

  /* Check that the bind variables are existing HyperScript variables
   * and set the bind variable value to that of the HyperScript variable.
   */
  for ( i=0; i<bind_dp->N; i++ ) {

    /* The name of the bind variable is in bind_dp->S[i] and
     * the length of the bind variable is bind_dp->C[i].
     * Construct a null terminated copy of it.
     */
    len = bind_dp->C[i] ;
    strncpy ( bindVariable, bind_dp->S[i], len ) ;
    bindVariable[len] = '\0' ;

    pVariable = gHyp_frame_findVariable ( pAI, pFrame, bindVariable ) ;

    if ( !pVariable ) {
      gHyp_instance_warning ( pAI,
			      STATUS_SQL,
			      "Bind variable '%s' does not exist",
			      bindVariable ) ;
      status = FALSE ;
      break ;
    }

    /* Get value of HyperScript variable.
     * If its a list, just pull out the first value.
     */
    pData = gHyp_data_getValue ( pVariable, -1, TRUE ) ;
    if ( !pData ) {
      gHyp_instance_warning ( pAI,
			      STATUS_SQL,
			      "Bind variable '%s' has no value.",
			      bindVariable ) ;
      status = FALSE ;
      break ;
    }

    /* Get datatype of variable */
    dataType = gHyp_data_getDataType ( pData ) ;
    switch ( dataType ) {

    case TYPE_INTEGER:
      /* Set the bind datatype to ORACLE EXTERNAL TYPE INTEGER */
      bind_dp->T[i] = 3 ;
      n = gHyp_data_getInt ( pData, -1, TRUE ) ;
      len = sizeof ( int ) ;
      bind_dp->L[i] = len ;
      bind_dp->V[i] = (int *) realloc ( (void *) bind_dp->V[i], len ) ;
      memcpy ( bind_dp->V[i], &n, len ) ;

      /* Indicate that the value is not NULL */
      *bind_dp->I[i] = 0 ;
      break ;

    case TYPE_DOUBLE:
      /* Set the bind datatype to ORACLE EXTERNAL TYPE FLOAT */
      bind_dp->T[i] = 4 ;
      d = gHyp_data_getDouble ( pData, -1, TRUE ) ;
      len = sizeof ( double ) ;
      bind_dp->L[i] = len ;
      bind_dp->V[i] = (float *) realloc ( (void *) bind_dp->V[i], len ) ;
      memcpy ( bind_dp->V[i], &d, len ) ;

      /* Indicate that the value is not NULL */
      *bind_dp->I[i] = 0 ;
      break ;

    case TYPE_STRING:
    default:

      /* Set the bind datatype to ORACLE EXTERNAL TYPE 'STRING' */
      bind_dp->T[i] = 5 ;
      len = gHyp_data_getStr ( pData, value, VALUE_SIZE, -1, TRUE ) ;
      if ( len != -1 ) {
	
	/* Non-null value */
	
	/* Set value into bind structure */
	bind_dp->L[i] = len+1 ;
	bind_dp->V[i] = (char *) realloc ( (void *) bind_dp->V[i], len+1 ) ;
	strcpy ( bind_dp->V[i], value ) ;
	
	/* Indicate that the value is not NULL */
	*bind_dp->I[i] = 0 ;
      }
      else {
	/* Indicate that the value is NULL */
	*bind_dp->I[i] = -1 ;
      }
      break ;
    }
  }
  return status ;
}
#endif

static sLOGICAL lHyp_esql_select ( sInstance *pAI,
				  sFrame *pFrame,
				  SQLDA *select_dp,
				  sData	*(*variables)[] )
{
  int
    i,
    len,
    precision,
    scale,
    nullok ;

  sLOGICAL
    status = TRUE ;

  char
    selectVariable[TOKEN_SIZE+1] ;

  sData
    *pVariable,
    *pArgs,
    *pData ;

  /* Create or retrieve the "sqlargs" variable, and clear it of any values. */
  pArgs = gHyp_frame_createVariable ( pFrame, "sqlargs" ) ;
  gHyp_data_deleteValues ( pArgs ) ;

  /* Set the maximum number of select variables in the SQL statement */
  select_dp->N = select_dp->F ;

  /* For each SELECT variable, allocate the required storage. */
  for ( i=0; i<select_dp->F; i++ ) {
	
    /* The name of the select variable is in select_dp->S[i] and
     * the length of the bind variable is select_dp->C[i].
     * Construct a null terminated copy of it.
     */
    len = select_dp->C[i] ;
    strncpy ( selectVariable, select_dp->S[i], len ) ;
    selectVariable[len] = '\0' ;

    pVariable = gHyp_frame_createVariable ( pFrame, selectVariable ) ;

    /* Store a reference to the variable in the "sqlargs" variable */
    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setReference ( pData, selectVariable, NULL ) ;
    gHyp_data_append ( pArgs, pData ) ;

    /* Store the pointer to the variable for later use */
    (*variables)[i] = pVariable ;

    /* Turn off high order bit of datatype, because we do not care
     * if the column has the characteristic "NOT NULL".
     */
    sqlnul ( &(select_dp->T[i]), &(select_dp->T[i]), &nullok ) ;

    switch ( select_dp->T[i] ) {

    case 1: /* CHAR datatype, no change in length needed,
	     * except possibly for TO_CHAR conversions.
	     */
      /* Coerce to str */
      select_dp->T[i] = 1;
      /* Storage for null terminated string. */
      select_dp->V[i] = (char *) realloc ( (void *) select_dp->V[i],
					   select_dp->L[i]+1 ) ;
      if ( guDebugFlags & DEBUG_SQL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			     "sql  : str %s", selectVariable ) ;
      break ;

    case 2: /* NUMBER datatype, use sqlprc to extract precision,scale */
      sqlprc ( &(select_dp->L[i]), &precision, &scale ) ;
      if ( precision == 0 ) precision = 38 ; /* Allow for max size */
      if ( scale > 0 )
	select_dp->L[i] = sizeof ( float ) ;
      else
	select_dp->L[i] = sizeof ( int ) ;
      /* NUMBER type, storage for int or float. */
      select_dp->V[i] = (char *) realloc ( (void *) select_dp->V[i],
					   select_dp->L[i] ) ;
      /* Coerce the datatypes of NUMBER to float or integer */
      if ( scale > 0 ) {
        if ( guDebugFlags & DEBUG_SQL )
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			       "sql  : float %s", selectVariable ) ;
	select_dp->T[i] = 4 ; /* Float */
      }
      else {
        if ( guDebugFlags & DEBUG_SQL )
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			       "sql  : int %s", selectVariable ) ;
	select_dp->T[i] = 3 ; /* Integer */
      }
      break ;

    case 8: /* LONG datatype */
      select_dp->L[i] = 240 ;
      /* Coerce to str */
      select_dp->T[i] = 1;
      /* Storage for null terminated string. */
      select_dp->V[i] = (char *) realloc ( (void *) select_dp->V[i],
					   select_dp->L[i]+1 ) ;
      if ( guDebugFlags & DEBUG_SQL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			     "sql  : long -> str %s", selectVariable ) ;
      break ;

    case 11: /* ROWID datatype */
      select_dp->L[i] = 18 ;
      /* Coerce to char */
      select_dp->T[i] = 1;
      /* Storage for null terminated string. */
      select_dp->V[i] = (char *) realloc ( (void *) select_dp->V[i],
					   select_dp->L[i]+1 ) ;
      if ( guDebugFlags & DEBUG_SQL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			     "sql  : rowid -> str %s", selectVariable ) ;
      break ;

    case 12: /* DATE datatype */
      select_dp->L[i] = DATETIME_SIZE ;
      /* Coerce to char */
      select_dp->T[i] = 1;
      /* Storage for null terminated string. */
      select_dp->V[i] = (char *) realloc ( (void *) select_dp->V[i],
					   select_dp->L[i]+1 ) ;
      if ( guDebugFlags & DEBUG_SQL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			     "sql  : date -> str %s", selectVariable ) ;
      break ;

    case 23: /* RAW datatype */
      /* Coerce to char */
      select_dp->T[i] = 1;
      /* Storage for null terminated string. */
      select_dp->V[i] = (char *) realloc ( (void *) select_dp->V[i],
					   select_dp->L[i]+1 ) ;
      if ( guDebugFlags & DEBUG_SQL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			     "sql  : raw -> str %s", selectVariable ) ;
      break ;

    case 24: /* LONG RAW datatype */
      /* Coerce to char */
      select_dp->T[i] = 1;
      select_dp->L[i] = 240 ;
      /* Storage for null terminated string. */
      select_dp->V[i] = (char *) realloc ( (void *) select_dp->V[i],
					   select_dp->L[i]+1 ) ;
      if ( guDebugFlags & DEBUG_SQL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			     "sql  : long raw -> str %s", selectVariable ) ;
      break ;

    default:
      if ( guDebugFlags & DEBUG_SQL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			    "sql  : unknown (%d) on %s",
			select_dp->T[i], selectVariable ) ;
      status = FALSE ;

    }
  }
  return status ;

}


void gHyp_esql_query ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sql ()
   *	Returns TRUE or FALSE
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   * Modifications:
   *
   */
  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    SQLDA
      *bind_dp,
      *select_dp ;

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData,
      *pData2,
      *pResult,
      *variables[MAX_SQL_ITEMS] ;

    sLOGICAL
      status,
      isVector,	
      isParse = 0 ;

    double
      d ;

    int
      i,
      rows=1,
      n,
      ss,	
      context,
      valueLen,	
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      sql_statement[MAX_SQL_STMT_LENGTH+1],
      sql_handle[MAX_SQL_NAME_LENGTH+1],
      value[VALUE_SIZE+1],
      *pValue,
      *pSQL,
      *pEndSQL ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
    status = TRUE ;

    if ( argCount !=2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: sql ( stmt, handle )" ) ;

    /* Get handle name for remote database */
    pData2 = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData2, value, VALUE_SIZE, 0, TRUE ) ;
    if ( n > MAX_SQL_NAME_LENGTH )
      gHyp_instance_error (pAI, STATUS_SQL,
    	"ORACLE database handle name must be %d characters or less",
	MAX_SQL_NAME_LENGTH ) ;
    strcpy ( sql_handle, value ) ;

    /* Get the SQL statement */
    pData = gHyp_stack_popRdata ( pStack, pAI ) ;

    /* Build the SQL statement */
    pSQL = sql_statement ;
    pEndSQL = pSQL + MAX_SQL_STMT_LENGTH ;
    pResult = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ;
    context = -1 ;
    isVector = (gHyp_data_getDataType(pData) > TYPE_STRING);
    while ( (pResult = gHyp_data_nextValue ( pData,
					     pResult,
					     &context,
					     ss ) ) ) {
	
      valueLen = gHyp_data_getStr ( pResult,
				    value,
				    VALUE_SIZE,
				    context,
				    isVector ) ;

      if ( (pSQL + valueLen) > pEndSQL )
	gHyp_instance_error ( pAI, STATUS_SQL,
			      "SQL statement longer than %d characters \n",
				MAX_SQL_STMT_LENGTH ) ;
      sprintf ( pSQL, "%s", value ) ;
      pSQL += valueLen ;
    }
    if ( context == -2 )
      gHyp_instance_error ( pAI, STATUS_BOUNDS,
			    "Subscript is out of bounds") ;

/*
 *     EXEC SQL WHENEVER SQLERROR GOTO sql_error ;
 */
#line 547 "esql.ec"

    bind_dp = lHyp_esql_setup () ;
    if ( !bind_dp )
      gHyp_instance_error ( pAI,
			    STATUS_SQL,
			    "Could not allocate memory for bind descriptor" ) ;

    select_dp = lHyp_esql_setup () ;
    if ( !select_dp ) {
      lHyp_esql_cleanup ( bind_dp ) ;
      gHyp_instance_error ( pAI,
			    STATUS_SQL,
			    "Could not allocate memory for select descriptor");
    }
    select_dp->N = MAX_SQL_ITEMS ;

    /* Declare the SQL statement */
/*
 *     EXEC SQL BEGIN DECLARE SECTION;
 */
#line 565 "esql.ec"
#line 566 "esql.ec"
  char sql_statement[MAX_SQL_STMT_LENGTH+1];
/*
 *     EXEC SQL END DECLARE SECTION;
 */
#line 567 "esql.ec"


    /* Process the SQL statement */
    isParse = 1 ;           /* Set a flag for sql_error */
/*
 *     EXEC SQL PREPARE S FROM :sql_statement ;
 */
#line 571 "esql.ec"
  {
#line 571 "esql.ec"
  sqli_prep(ESQLINTVERSION, _Cn1, sql_statement,(ifx_literal_t *)0, (ifx_namelist_t *)0, -1, 0, 0 ); 
#line 571 "esql.ec"
  { if (SQLCODE < 0) goto sql_error; }
#line 571 "esql.ec"
}
    isParse = 0 ;

/*
 *     EXEC SQL DECLARE C CURSOR FOR S ;
 */
#line 574 "esql.ec"
{
#line 574 "esql.ec"
sqli_curs_decl_dynm(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 512), _Cn2, sqli_curs_locate(ESQLINTVERSION, _Cn1, 513), 0, 0);
#line 574 "esql.ec"
{ if (SQLCODE < 0) goto sql_error; }
#line 574 "esql.ec"
}



/*** Comment out bind variables ***/

    /* Set the bind variables for any placeholders in the SQL statement */
/*    bind_dp->F = MAX_SQL_ITEMS ;
    EXEC SQL DESCRIBE BIND VARIABLES FOR S INTO bind_dp ; */

    /* Check if SQL statement has more bind variables than MAX_SQL_ITEMS */
/*    if ( bind_dp->F < 0 ) {
      n = -(bind_dp->F) ;
      lHyp_esql_cleanup ( bind_dp ) ;
      lHyp_esql_cleanup ( select_dp ) ;
      gHyp_instance_error ( pAI, STATUS_SQL,
			    "Too many bind variables (%d), maximum is %d",
			    n, MAX_SQL_ITEMS ) ;
    }

    status = lHyp_esql_bind ( pAI, pFrame, bind_dp ) ;
*/
/*** end of comment out bind variables ***/



    if ( status ) {

      /* Open the cursor and execute the statement */
/*
 *       EXEC SQL OPEN C USING DESCRIPTOR bind_dp ;
 */
#line 603 "esql.ec"
{
#line 603 "esql.ec"
sqli_curs_open(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 768), bind_dp, (char *)0, (struct value *)0, 1, 0);
#line 603 "esql.ec"
{ if (SQLCODE < 0) goto sql_error; }
#line 603 "esql.ec"
}

      /* Process the select list, if specified */
      if ( ( strncmp ( sql_statement, "SELECT", 6 ) != 0 ) &&
	   ( strncmp ( sql_statement, "select", 6 ) != 0 ) )
	
	/* Not a 'select' statement. We're done. */
	select_dp->F = 0 ;

      else {
	
	/* A 'SELECT' statement.  Obtain info about the SELECT LIST */
	select_dp->N = MAX_SQL_ITEMS ;
/*
 * 	EXEC SQL DESCRIBE S INTO select_dp ;
 */
#line 616 "esql.ec"
{
#line 616 "esql.ec"
sqli_describe_stmt(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn1, 769), &select_dp, 0);
#line 616 "esql.ec"
{ if (SQLCODE < 0) goto sql_error; }
#line 616 "esql.ec"
}
	
	/* Check if SQL statement has more select variables
	 * than MAX_SQL_ITEMS
	 */
	if ( select_dp->F < 0 ) {
	  n = -(select_dp->F) ;
	  lHyp_esql_cleanup ( bind_dp ) ;
	  lHyp_esql_cleanup ( select_dp ) ;
	  gHyp_instance_error (pAI, STATUS_SQL,
			       "Too many select variables (%d), maximum is %d",
			       n, MAX_SQL_ITEMS ) ;
	}

	status = lHyp_esql_select ( pAI, pFrame,
				   select_dp,
				   (sData*(*)[]) variables ) ;

	if ( status ) {

	  /* Do the FETCH */
/*
 * 	  EXEC SQL WHENEVER NOT FOUND GOTO end_fetch;
 */
#line 637 "esql.ec"
	
	  /* Clear the variables to receive the data */
	  for ( i=0; i<select_dp->F; i++ )
	    gHyp_data_deleteValues ( variables[i] ) ;

	  for (;;) {
	
/*
 * 	    EXEC SQL FETCH C USING DESCRIPTOR select_dp ;
 */
#line 645 "esql.ec"
{
#line 645 "esql.ec"
static _FetchSpec _FS0 = { 0, 1, 0 };
sqli_curs_fetch(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 768), (ifx_sqlda_t *)0, select_dp, (char *)0, &_FS0);
#line 645 "esql.ec"
{ if (SQLCODE < 0) goto sql_error; }
#line 645 "esql.ec"
{ if (SQLCODE == SQLNOTFOUND) goto end_fetch; }
#line 645 "esql.ec"
}
	
	    for ( i=0; i<select_dp->F; i++ ) {
	
	      pData = gHyp_data_new ( NULL ) ;

	      if ( *select_dp->I[i] < 0 ) {
		/* NULL value */
		if ( guDebugFlags & DEBUG_SQL )
                  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
                  	"sql  : list %s[%d] = {}",
			gHyp_data_getLabel ( variables[i] ),
			i ) ;
		gHyp_data_setNull ( pData ) ;
	      }
	      else {
		/* Non-NULL value */
		if ( select_dp->T[i] == 3 ) {
		  /* Integer result */
		  n = *(int *)select_dp->V[i] ;
		  if ( guDebugFlags & DEBUG_SQL )
                    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
                  	"sql  : int %s[%d] = %d",
			gHyp_data_getLabel ( variables[i] ),
			i,
			n );
		  gHyp_data_setInt ( pData, n ) ;
                }
		else if ( select_dp->T[i] == 4 ) {
		  /* Float result */
                  d = (double) *(float *)select_dp->V[i] ;
		  if ( guDebugFlags & DEBUG_SQL )
                    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
                  	"sql  : double %s[%d] = %g",
			gHyp_data_getLabel ( variables[i] ),
			i,
			d );
		  gHyp_data_setDouble ( pData, d ) ;		
                }
		else  {
		  /* String result */
                  pValue = select_dp->V[i] ;
                  n = MIN ( select_dp->L[i], VALUE_SIZE ) ;
		  strncpy ( value, pValue, n ) ;
		  value[n] = '\0' ;
		  gHyp_util_trimWhiteSpace ( value ) ;
		  if ( guDebugFlags & DEBUG_SQL )
                    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
                  	"sql  : str %s[%d] = '%s'",
			gHyp_data_getLabel ( variables[i] ),
			i,
			value );
		  gHyp_data_setStr ( pData, value ) ;
                }
	      }
	      /* Append the value */
	      gHyp_data_append ( variables[i], pData ) ;
	    }
	  }
        }
      }
    }
    end_fetch:

    if ( status ) {

      /* Stats */
      for ( i=0; i<8; i++ ) {
	if ( strncmp ( sql_statement, gpzDmlCommands[i], 6 ) == 0 ) {
	  rows = sqlca.sqlerrd[2] ;
	  if ( rows == 0 )
	    gHyp_instance_warning ( pAI, STATUS_SQL,
				    "0 rows processed" ) ;
	  else
      	    if ( guDebugFlags & DEBUG_SQL )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
	   			   "sql  : %d row%c processed",
			       	   rows,
			            rows == 1 ? '\0' : 's' ) ;
	  break ;
	}
      }

    }

/*
 *     EXEC SQL WHENEVER SQLERROR CONTINUE ;
 */
#line 730 "esql.ec"
/*
 *     EXEC SQL CLOSE C ;
 */
#line 731 "esql.ec"
{
#line 731 "esql.ec"
sqli_curs_close(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 768));
#line 731 "esql.ec"
{ if (SQLCODE == SQLNOTFOUND) goto end_fetch; }
#line 731 "esql.ec"
}

    if ( status ) {
/*
 *       EXEC SQL COMMIT WORK ;
 */
#line 734 "esql.ec"
{
#line 734 "esql.ec"
sqli_trans_commit();
#line 734 "esql.ec"
{ if (SQLCODE == SQLNOTFOUND) goto end_fetch; }
#line 734 "esql.ec"
}
    }
    else {
/*
 *       EXEC SQL ROLLBACK WORK ;
 */
#line 737 "esql.ec"
{
#line 737 "esql.ec"
sqli_trans_rollback();
#line 737 "esql.ec"
{ if (SQLCODE == SQLNOTFOUND) goto end_fetch; }
#line 737 "esql.ec"
}
    }
    lHyp_esql_cleanup ( bind_dp ) ;
    lHyp_esql_cleanup ( select_dp ) ;

    pResult = gHyp_data_new ( NULL ) ;
    if ( status ) 	
      gHyp_data_setInt ( pResult, rows ) ;
    else
      gHyp_data_setInt ( pResult, 0 ) ;
    gHyp_stack_push ( pStack, pResult ) ;
    return ;

  sql_error:

    status = FALSE ;
    gHyp_util_logError (  "%.70s", sqlca.sqlerrm.sqlerrmc ) ;
    if ( isParse )
      gHyp_util_logError ( "Parse error at character offset %d",
			   sqlca.sqlerrd[4] ) ;

/*
 *     EXEC SQL WHENEVER SQLERROR CONTINUE ;
 */
#line 758 "esql.ec"
/*
 *     EXEC SQL ROLLBACK WORK ;
 */
#line 759 "esql.ec"
{
#line 759 "esql.ec"
sqli_trans_rollback();
#line 759 "esql.ec"
{ if (SQLCODE == SQLNOTFOUND) goto end_fetch; }
#line 759 "esql.ec"
}

    lHyp_esql_cleanup ( bind_dp ) ;
    lHyp_esql_cleanup ( select_dp ) ;

    gHyp_instance_warning ( pAI, STATUS_SQL,
			    "Could not execute SQL statement %s",
			    sql_statement ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setBool ( pResult, status ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_esql_connect ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sql_connect ()
   *	Returns TRUE or FALSE
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   * Modifications:
   *
   */
  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      value[VALUE_SIZE+1],
      sql_user[MAX_SQL_USER_LENGTH+1],
      sql_pass[MAX_SQL_NAME_LENGTH+1],
      sql_using[MAX_SQL_NAME_LENGTH+1],
      sql_at[MAX_SQL_NAME_LENGTH+1] ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount !=4 )
      gHyp_instance_error (
	 pAI, STATUS_ARGUMENT,
       "Invalid arguments. Usage: sql_connect ( user, pass, name, handle )");

    /* Get handle name for remote database */	
    n = gHyp_data_getStr (	gHyp_stack_popRvalue ( pStack, pAI ),
				value,
				VALUE_SIZE,
				0,
				TRUE ) ;
    if ( n > MAX_SQL_NAME_LENGTH )
      gHyp_instance_error (pAI, STATUS_SQL,
    	"ORACLE database handle name must be %d characters or less",
	MAX_SQL_NAME_LENGTH ) ;

    strcpy ( sql_at, value ) ;
	
    /* Get specification for database */
    n = gHyp_data_getStr (	gHyp_stack_popRvalue ( pStack, pAI ),
				value,
				VALUE_SIZE,
				0,
				TRUE ) ;
    if ( n > MAX_SQL_NAME_LENGTH )
      gHyp_instance_error (pAI, STATUS_SQL,
    	"ORACLE database specification must be %d characters or less",
	MAX_SQL_NAME_LENGTH ) ;
    strcpy ( sql_using, value ) ;	

    n = gHyp_data_getStr ( gHyp_stack_popRvalue ( pStack, pAI ),
			   value,
			   VALUE_SIZE,
			   0,
			   TRUE ) ;
    if ( n > MAX_SQL_NAME_LENGTH )
      gHyp_instance_error ( pAI, STATUS_SQL,
    	"ORACLE password must be %d characters or less",
	MAX_SQL_NAME_LENGTH ) ;
    strcpy ( sql_pass, value ) ;

    n = gHyp_data_getStr ( gHyp_stack_popRvalue ( pStack, pAI ),
			   value,
			   VALUE_SIZE,
			   0,
			   TRUE ) ;
    if ( n > MAX_SQL_USER_LENGTH )
      gHyp_instance_error ( pAI, STATUS_SQL,
    	"ORACLE username must be %d characters or less",
	MAX_SQL_USER_LENGTH ) ;
    strcpy ( sql_user, value ) ;

/*
 *     EXEC SQL WHENEVER SQLERROR GOTO connect_error;
 */
#line 879 "esql.ec"

/*
 *     EXEC SQL BEGIN DECLARE SECTION;
 */
#line 881 "esql.ec"
#line 882 "esql.ec"
  char sql_using[MAX_SQL_NAME_LENGTH+1];
/*
 *     EXEC SQL END DECLARE SECTION;
 */
#line 883 "esql.ec"


/*
 *     EXEC SQL CONNECT TO :sql_using;
 */
#line 885 "esql.ec"
{
#line 885 "esql.ec"
sqli_connect_open(ESQLINTVERSION, 0, sql_using, (char *)0, (ifx_conn_t *)0, 0);
#line 885 "esql.ec"
{ if (SQLCODE < 0) goto connect_error; }
#line 885 "esql.ec"
{ if (SQLCODE == SQLNOTFOUND) goto end_fetch; }
#line 885 "esql.ec"
}

    gHyp_util_logInfo ( "Connected to ORACLE as user %s", sql_user ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setBool ( pResult, TRUE ) ;
    gHyp_stack_push ( pStack, pResult ) ;
    return ;

  connect_error:
    gHyp_instance_warning ( pAI, STATUS_SQL,
			    "Cannot connect to ORACLE as user %s\n",
			    sql_user ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setBool ( pResult, FALSE ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

#line 902 "esql.ec"
