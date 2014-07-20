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
*
* Modified:
*
* $Log: sql.c,v $
* Revision 1.80  2013-01-02 19:10:43  bergsma
* CVS Issues
*
* Revision 1.78  2012-08-11 00:15:04  bergsma
* Add AS_ORACLE_COMMIT_ON_SUCCESS
*
* Revision 1.77  2012-07-23 03:59:33  bergsma
* Enclose ORACLE Failover Callback as AS_ORA_CALLBACK
*
* Revision 1.76  2011-12-24 01:42:12  bergsma
* Handle OCI_NO_DATA condition on UPDATE or INSERT statements.
*
* Revision 1.75  2011-07-03 16:36:56  bergsma
* gHyp_sock_usleep, not gHyp_util_usleep
*
* Revision 1.74  2011-06-09 22:31:40  bergsma
* Integrate ORACLE failover
*
* Revision 1.73  2011-01-08 21:42:56  bergsma
* New args to frame Variable functions.
* Increase in buffer size.
*
* Revision 1.72  2010-05-28 18:08:43  bergsma
* Fix for sql_datetime not entirely correct.
*
* Revision 1.71  2010-04-13 21:07:24  bergsma
* sql_datetime fixed
*
* Revision 1.70  2010-03-17 08:17:37  bergsma
* Fix memory leak for sql_external and meaning of second argument.
*
* Revision 1.69  2010-03-05 06:05:24  bergsma
* SJM Changes
* 1. ORACLE_COMMIT_ON_SUCCESS for non-SELECT statements
* 2, sql_toexternal with DoNotTrim==1 now trims, but never returns NULL
*
* Revision 1.68  2009-09-28 05:28:33  bergsma
* Misplaced bracket
*
* Revision 1.67  2009-09-28 05:23:31  bergsma
* Free ORACLE stmt handle memory leak causing
* ORA-01000: maximum open cursors exceeded
*
* Revision 1.66  2009-09-21 05:20:38  bergsma
* Better detection of NULL sql_datetime values
*
* Revision 1.65  2009-06-17 22:49:02  bergsma
* For sql_datetime and datetime, reject erroreous dates, SPECIFICALLY from
* PROMIS, a NULL date is -1200798848
*
* Revision 1.64  2008-10-16 23:41:31  bergsma
* Bad datetime dbconv in SQLSERVER
*
* Revision 1.63  2008-09-11 00:46:28  bergsma
* Bad conversion of empty values to NULL using sql_toexternal
*
* Revision 1.62  2008-08-25 12:49:07  bergsma
* Typo (in support of PGSQL, missing { in function
*
* Revision 1.61  2008-08-21 11:08:55  bergsma
* For adapting to FreeTDS
*
* Revision 1.60  2008-07-01 23:48:54  bergsma
* When doing 'toexternal', make room for VALUE_SIZE*4
*
* Revision 1.59  2008-06-13 04:22:00  bergsma
* Make CLOB _data_
*
* Revision 1.58  2008-05-27 16:12:25  bergsma
* Make sure _data_ is used for colLens > INTERNAL_VALUE_SIZE
*
* Revision 1.57  2008-05-27 00:25:13  bergsma
* no message
*
* Revision 1.56  2008-05-26 00:32:41  bergsma
* no message
*
* Revision 1.55  2008-05-26 00:08:57  bergsma
* OCIlobRead function problems
*
* Revision 1.54  2008-05-25 02:30:40  bergsma
* OCIlobRead function problems
*
* Revision 1.53  2008-05-23 05:44:51  bergsma
* ORACLE CLOB Reading changes
*
* Revision 1.52  2008-05-19 01:07:40  bergsma
* For ORACLE clob, allow maximum 512K input buffer.
*
* Revision 1.49  2007-12-01 02:00:18  bergsma
* For pgsql, use PQfmod to get varchar value
*
* Revision 1.48  2007-11-29 18:03:03  bergsma
* sql_toexternal, 2nd argument was reversed, trim was broken
*
* Revision 1.47  2007-06-20 22:31:41  bergsma
* no message
*
* Revision 1.46  2007-06-20 21:09:24  bergsma
* Use popRdata, not popRvalue, when subsequently doing a data_getNext
*
* Revision 1.45  2007-06-16 17:57:25  bergsma
* Optional arg for sql_toexternal to notTrim(1) or Trim(0), default = 0.
*
* Revision 1.44  2007-05-26 01:46:40  bergsma
* Make sql_datetime and sql_toexternal return quotes around the
* result, or NULL if invalid.
*
* Revision 1.43  2007-05-03 17:02:33  bergsma
* For sql_datetime, return NULL for errors.
*
* Revision 1.42  2007-03-22 16:45:23  bergsma
* No NL at eof.
*
* Revision 1.41  2007-03-15 01:08:48  bergsma
* Added sql_datetime function
*
* Revision 1.40  2006-10-15 18:52:35  bergsma
* SQLTEXT is _data_
*
* Revision 1.39  2006/10/12 23:08:42  bergsma
* TEXTOID for POSTGRESQL is _data_
*
* Revision 1.38  2006/10/12 00:33:14  bergsma
* Typedef required on setstr_n
*
* Revision 1.37  2006/10/11 16:16:42  bergsma
* Firx compile warnings.
*
* Revision 1.36  2006/10/01 16:27:16  bergsma
* Typo, change C++ comment to C style.
*
* Revision 1.35  2006/09/25 05:02:15  bergsma
* Convert ll results from SQL query to actual data type of column
*
* Revision 1.34  2006/09/16 20:06:28  bergsma
* Datatyping for columns.
* Added _sqlattr_ to further help distinguish datetime types.
*
* Revision 1.33  2006/09/14 17:09:12  bergsma
* Fixes for ORACLE.
* Also, make sure _sql_status_ is defined for each SQL db type.
*
* Revision 1.32  2006/08/17 05:04:40  bergsma
* Addd ORACLE OCI interface
*
* Revision 1.31  2006/02/06 02:52:30  bergsma
* Detect all successful PostgreSQL status valiues.
*
* Revision 1.30  2006/01/29 16:00:57  bergsma
* Postgresl include area is something else
*
* Revision 1.29  2006/01/17 15:59:45  bergsma
* no message
*
* Revision 1.28  2005/09/17 17:10:49  bergsma
* Only PostgreSQL must force uppercase column names.
*
* Revision 1.27  2005/09/08 09:13:30  bergsma
* Postgres DEC MCS to Unicode Handling
*
* Revision 1.23  2005/06/14 23:45:40  bergsma
* no message
*
* Revision 1.22  2005/06/12 16:46:21  bergsma
* HS 3.6.1
*
* Revision 1.21  2005/04/19 16:41:10  bergsma
* Do not use parsestring on VARCHAR fields fetched in select statements.
*
* Revision 1.20  2005/04/18 19:20:57  bergsma
* TEXT fields in SQLSERVER needs _data_.
*
* Revision 1.19  2005/04/13 13:45:54  bergsma
* HS 3.5.6
* Added sql_toexternal.
* Fixed handling of strings ending with bs (odd/even number of backslashes)
* Better handling of exception condition.
*
* Revision 1.18  2005/03/21 18:06:41  bergsma
* For unknown "default" datatypes in SQLSERVER, use buffer instead of value
* to receive data.
*
* Revision 1.17  2004/12/13 04:53:23  bergsma
* Let strings be longer than VALUE_SIZE bytes.
*
* Revision 1.16  2004/11/02 23:00:24  bergsma
* (char*) mismatch with (sBYTE*)
*
* Revision 1.15  2004/10/27 18:24:07  bergsma
* HS 3.3.2
* 1. Fix bug with SQL read when str size is > 512 bytes, use _data_ blobs
* 2. Fix bug with XML output, forgetting ";" in unicode output.
* 3. In |TOKEN|VALUE|VALUE| part of message, use unparse on TOKEN
* as well as VALUE.
* 4. Add utility function util_breakStream.
*
* Revision 1.14  2004/10/16 05:10:06  bergsma
* MySql bug, improper return value from sql_query.
* -1 = error
* 0 = no rows processed or retrieved
* >1 number of rows processed or retrieved
*
*
* Revision 1.12  2004/07/28 00:49:18  bergsma
* Version 3.3.0
*
* Revision 1.12  2004/07/27 15:44:16  bergsma
* Remove ^M
*
* Revision 1.11  2004/07/01 02:01:42  bergsma
* For Mysql, Make Sure -1 Means Sql Error, 0 Means No Rows Affected.
*
* Revision 1.10  2004/05/15 02:08:01  bergsma
* Remove ^M
*
* Revision 1.8  2004/04/29 02:03:55  bergsma
* Added support for sql. Modified by Michael Yockey.
*
* Revision 1.7  2003/02/05 23:58:16  bergsma
* Removed redundant line "pResult = gHyp_data_new()"
*
* Revision 1.6  2003/01/09 01:21:17  bergsma
* V3.1.0
* 1. Change util_logError to instance_warning
* 2. Incorportate instance_pushSTATUS
*
* Revision 1.5  2002/11/28 14:34:50  bergsma
* Typo, || should be &&, optional isSecure arg in sql_open()
*
* Revision 1.4  2002/11/21 23:57:03  bergsma
* Add isSecure argument to sql_open, for SQLSERVER
*
* Revision 1.3  2002/09/21 22:16:53  bergsma
* MySQL returns all results in ascii form, not in binary form as does SQLSERVER.
* Substituted gHyp_data_newConstant2 for gHyp_data_newConstant3.
*
* Revision 1.2  2002/09/20 00:05:17  bergsma
* Cleaned up the logic for the optional 3 last arguments of sql_query.
*
*
*/

/********************** AUTOROUTER INTERFACE ********************************/

#ifdef WIN32
typedef unsigned char BYTE;
typedef BYTE * LPBYTE;
typedef const LPBYTE LPCBYTE ;
#define _LPCBYTE_DEFINED 1
#endif

#include "auto.h"       /* System Interface and Function Prototypes */

/********************** EXTERNAL FUNCTION DECLARATIONS ***********************/

/********************** EXTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/********************** FUNCTION DEFINITIONS ********************************/


#ifdef AS_SQLSERVER_NEW
#define _SQLNCLI_ODBC_
include "sqlncli.h";
#endif

#ifdef AS_SQLSERVER
#define DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>
#ifndef AS_WINDOWS
typedef int INT;
typedef char * LPCSTR;
#define SQLVOID        0x1f
#define SQLTEXT        0x23
#define SQLVARBINARY   0x25
#define SQLINTN        0x26
#define SQLVARCHAR     0x27
#define SQLBINARY      0x2d
#define SQLIMAGE       0x22
#define SQLCHAR        0x2f
#define SQLINT1        0x30
#define SQLBIT         0x32
#define SQLINT2        0x34
#define SQLINT4        0x38
#define SQLMONEY       0x3c
#define SQLDATETIME    0x3d
#define SQLFLT8        0x3e
#define SQLFLTN        0x6d
#define SQLMONEYN      0x6e
#define SQLDATETIMN    0x6f
#define SQLFLT4        0x3b
#define SQLMONEY4      0x7a
#define SQLDATETIM4    0x3a
#define SQLDECIMAL     0x6a
#define SQLNUMERIC     0x6c
#endif
#endif

#ifdef AS_ORACLE
#include <oci.h>

/*#define AS_ORACLE_DO_PREPARE2 */

struct oracle_t {
	OCISvcCtx  *svchp;
	OCISession *authp;
	OCIServer  *srvhp;
	OCIEnv     *envhp;
	OCIError   *errhp;
	char       username[VALUE_SIZE+1];
	char       password[VALUE_SIZE+1];
	char       database[VALUE_SIZE+1];
}  ;

typedef struct oracle_t	sORACLE ;
sORACLE *gsORACLE ;

/*#define ORACLE_AUTO_RECONNECT 1*/


/* for sql_bind */
struct bind_t {
	OCIStmt    *stmthp;
  dvoid      *valuep[MAX_SQL_ITEMS] ;
  ub2        *alenp[MAX_SQL_ITEMS] ;
}  ;

typedef struct bind_t	sBIND ;

#endif

#ifdef AS_MYSQL
#ifdef AS_UNIX
#include <mysql/mysql.h>
#else
#include <mysql.h>
#endif
#endif

#ifdef AS_PGSQL
#ifdef AS_UNIX
#include <postgresql/libpq-fe.h>
#else
#include <libpq-fe.h>
#endif
#endif

#ifdef AS_SQLSERVER

static char errbuf[512];
static int lHyp_sql_errHandler (PDBPROCESS dbproc, INT severity,
INT dberr, INT oserr, LPCSTR dberrstr, LPCSTR oserrstr)
{
	if (oserr != DBNOERR)
	sprintf ( errbuf, "Operating System Error %i: %s\n", oserr, (char*)oserrstr);
	else
	sprintf (errbuf, "DB-Library Error %i: %s\n", dberr, (char*)dberrstr);

	gHyp_util_logError ( errbuf ) ;
	return (INT_CANCEL);
}

static int lHyp_sql_msgHandler (PDBPROCESS dbproc, DBINT msgno, INT msgstate,
INT severity, LPCSTR msgtext, LPCSTR server,
LPCSTR procedure, DBUSMALLINT line)
{
	sprintf ( errbuf, "SQL Server Message %ld: %s\n", (long int) msgno, (char*) msgtext );
	gHyp_util_logWarning ( errbuf );
	return (0);
}
#endif

#ifdef AS_ORACLE

static char errbuf[512];
static sLOGICAL lHyp_sql_checkErr( OCIError *errhp, sword status)
{
	sb4  errcode = 0;
	sLOGICAL isError = 1 ;
	sLOGICAL isWarning = 1 ;

	switch (status) {

	case OCI_SUCCESS:
		strcpy ( errbuf, "SUCCESS" ) ;
		isWarning = 0 ;
		isError = 0 ;
		break;

	case OCI_SUCCESS_WITH_INFO:
		strcpy ( errbuf, "OCI_SUCCESS_WITH_INFO");
		break;

	case OCI_NEED_DATA:
		strcpy ( errbuf, "OCI_NEED_DATA");
		isWarning = 0 ;
		break;

	case OCI_NO_DATA:
		strcpy ( errbuf, "OCI_NO_DATA");
		break;

	case OCI_ERROR:
		(void) OCIErrorGet( (dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
		(text *) errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
		isWarning = 0 ;
		break;

	case OCI_INVALID_HANDLE:
		strcpy ( errbuf, "OCI_INVALID_HANDLE");
		isWarning = 0 ;
		break;

	case OCI_STILL_EXECUTING:
		strcpy ( errbuf, "OCI_STILL_EXECUTE");
		break;

	case OCI_CONTINUE:
		strcpy ( errbuf, "OCI_CONTINUE");
		break;

	default:
		sprintf ( errbuf, "Unknown error %d",status );
		isWarning = 0 ;
		break;
	}

	if ( isError ) {
		if ( isWarning ) 
		gHyp_util_logWarning(errbuf );
		else
		gHyp_util_logError(errbuf );
	}

	return isError ;
}

static ub4 piece_count = 0;
static sb4 lHyp_sql_readLob ( dvoid *ctxp, CONST dvoid *bufxp, ub4 len, ub1 piece )
{
	piece_count++;
	switch (piece) {

	case OCI_LAST_PIECE:
	case OCI_FIRST_PIECE:
	case OCI_NEXT_PIECE:

		/* process buffer bufxp */
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
		"sql : callback read the %d  piece\n", piece_count);
		gHyp_util_breakStream ( (char*) bufxp, len, (sData*) ctxp, TRUE ) ;

		break;

	default:
		gHyp_util_logError("Callback read error: unkown piece = %d.\n", piece);
		return OCI_ERROR;
	}
	return OCI_CONTINUE;
}

#ifdef AS_ORA_CALLBACK
/* Application Failover Callback */
DWORD  gHyp_sql_ApplicationFailoverCallback(  dvoid           *pSvch,
dvoid           *pEnvh,
dvoid           *pFailoverContext,
ub4             FailoverType,
ub4             FailoverEvent )
{


	switch (FailoverEvent) {

	case OCI_FO_BEGIN: {
			sORACLE  *dbproc = gsORACLE ;

#ifndef ORACLE_AUTO_RECONNECT
			OCISvcCtx               *pLocSvch;
			sword                   rc;
#endif

			gHyp_util_logWarning ("Fail Safe Oracle8 database is failing over ... Please stand by\n");
			gHyp_util_logWarning ("Failover type was found to be %s\n",
			((FailoverType == OCI_FO_SESSION) ? "SESSION" :
			(FailoverType == OCI_FO_SELECT) ? "SELECT" : "UNKNOWN!!"));
			gHyp_util_logWarning ("Failover Context is : %s\n", 
			pFailoverContext ? (CHAR *) pFailoverContext : "NULL POINTER!");

			/*** START SECTION A ***
			Earlier Oracle8/8i OCI releases make only one attempt to reconnect after a failure is detected.
			OCI releases 8.0.6 and 8.1.6 or later contain enhancements to allow multiple reconnection attempts.
			When using earlier OCI libraries, the application needs to ensure that sufficient time elapses so 
			that when it returns from the failover callback function, the OCI library will be able to successfully
			reconnect to the virtual server for the Oracle database.  A quick way to do this is to sleep a "reasonable" 
			period of time to allow failover on the cluster to complete. A better solution (used below) is to write a loop 
			in which the application repeatedly  attempts to reconnect to the database at the virtual server address and
			returns either after making (and then disconnecting from) a successful connection or after a specified time 
			interval. For the most recent OCI releases, you can simply add RETRIES and DELAY parameters to the 
			FAILOVER_MODE clause in the TNSNAMES.ORA file and eliminate the need for a callback function.  
			Begin loop that tries to connect to the failover database until successfully connected.
			*/ 

#ifndef ORACLE_AUTO_RECONNECT

			while (1) {
				
				/* Sleep for 10 seconds before we retry to connect to the failover database. */
				gHyp_sock_usleep(10000);
				gHyp_util_logWarning("Retrying to connect to failover database after waiting 10 seconds...\n");

				rc = OCILogon(  pEnvh, 
				pEnvh, 
				&pLocSvch,
				dbproc->username, strlen(dbproc->username),
				dbproc->password, strlen(dbproc->password),
				dbproc->database, strlen(dbproc->database));

				if (rc != OCI_SUCCESS) {
					lHyp_sql_checkErr ( pEnvh, rc);
				}
				else {
					gHyp_util_logWarning("Successfully reconnected to failover database\n");
					lHyp_sql_checkErr ( pEnvh, OCILogoff(pLocSvch, pEnvh));
					break;
				}
			}

			/* If callback returns now immediately after reconnecting, the query that was executing may fail.
			* To avoid possible failure, sleep for an arbitrary time of 10 seconds.
			* This time period will vary from application to application.
			*/
			gHyp_sock_usleep(10000);
			/*** END SECTION A ***/
#endif

			break;
		}


	case OCI_FO_ABORT:
		gHyp_util_logWarning("Failover aborted. Failover will not take place.\n");
		break;

	case OCI_FO_END:
		gHyp_util_logWarning("Failover ended ... Resume services\n");
		break;

	case OCI_FO_REAUTH:
		gHyp_util_logWarning("Failed over user? Resume services\n");
		break;

#ifdef ORACLE_AUTO_RECONNECT

		/*** START SECTION B ***                  
	Starting with  Oracle8 release 8.0.5, the preceding call to OCILogon and the associated logic of section A are no longer needed. 
	Section A above can be commented out and Section B can be used instead. A new return code (currently 25410) tells OCI
	to retry the database connection. Applications can determine what connection retry logic best makes sense (for example,. an 
	exponential backoff in retry frequency, with retry attempts stopped after some number of unsuccessful attempts). The 
				following sample code will retry indefinitely and should be modified appropriately for actual deployment. Also, consult the 
	Oracle8/8i OCI documentation to verify the syntax and return values used by your OCI release.  For OCI releases 8.0.6 and 
	8.1.6 and later, you can, as previously noted, include RETRIES and DELAY parameters in the FAILOVER_MODE clause in 
	the TNSNAMES.ORA file and eliminate the need for a callback function.

			****/

#endif
	case OCI_FO_ERROR:
		gHyp_util_logWarning("Reconnection attempt failed; waiting 10 seconds?");
		gHyp_sock_usleep(10000);
		gHyp_util_logWarning("Retrying to connect to failover database after waiting 10 seconds...\n");
		return (25410);
		break;

		default :
		gHyp_util_logWarning("Bad Failover Event: %d\n", FailoverEvent);
		return -1; /* Error File Oracle Bug report */
	}

	return 0;
}

/* Registration of the Application Callback */ 

DWORD lHyp_sql_RegisterApplicationFailoverCallback( dvoid      *pSrvh, 
OCIError   *pErrh ) 
{ 
	OCIFocbkStruct  FailoverInfo;           /* Information for callback */ 

	/* Allocate Memory for context and fill the information */ 

	if ( !(FailoverInfo.fo_ctx = 
				(dvoid *)malloc(strlen("Application Masking Failover Errors")))) 
	return -1; 
	strcpy((LPSTR)FailoverInfo.fo_ctx, "Application Masking Failover Errors"); 
	FailoverInfo.callback_function = &gHyp_sql_ApplicationFailoverCallback; 
	
	/* Do the registration with OCI */ 
	return OCIAttrSet(  pSrvh, 
	(ub4) OCI_HTYPE_SERVER,  
	(dvoid *) &FailoverInfo, 
	(ub4) 0,  
	(ub4) OCI_ATTR_FOCBK, 
	pErrh); 

} /* end of RegisterApplicationFailoverCallback */ 

#endif
#endif

void gHyp_sql_bind ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
	/* Description:
	*
	*    PARSE or EXECUTE the built-in function: sql_bind ()
	*    Returns TRUE or FALSE
	*
	* Arguments:
	*
	*    pAI                                                     [R]
	*    - pointer to instance object
	*
	*    pCode                                                   [R]
	*    - pointer to code object
	*
	* Return value:
	*
	*    none
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
		  *pStatus,
      *pVariable,
		  *pData,
		  *pData2,
      *pData3,
      *pValue,
		  *pResult ;

		sLOGICAL
		  status,
		  isVector;

		int
		  ss,
		  context,
		  valueLen,
		  argCount = gHyp_parse_argCount ( pParse ) ;

		char
		  value[MAX_INPUT_LENGTH+1],
		  *pSQL,
		  *pEndSQL,
		  sql_stmt[MAX_SQL_STMT_LENGTH+1] ;

#ifdef AS_SQL
		
		sData
		  *pArgs=NULL ;
		
    int
		  i,
      colNameLen,
      colLen,
      dataType,
      precision,
		  numCols,
		  n, 
      msgLen ;

		char
      *pColName,
      colName[TOKEN_SIZE+1],
      msg[MAX_OUTPUT_LENGTH+1] ;

		sLOGICAL
      isFloat,
		  isSelect ;
		
#endif

#ifdef AS_SQL

#ifdef AS_ORACLE

    sBIND *dbbind ;
    dvoid* data_buffer; 
    sb4 data_bufferlen ;
		sORACLE	*dbproc ;
		sword	results ;
		sword	rc ; 
		OCIStmt    *stmthp;
    OCIParam   *mypard;
		char	dataBuffer[MAX_SQL_BUFFER_SIZE] ;
#ifdef AS_ORACLE_DO_PREPARE2
		OraText	*key = "mykey" ;
    ub4	keylen = strlen (key) ;
#endif
		ub2	dataLen[MAX_SQL_ITEMS] ;
		OCIBind*	bindp[MAX_SQL_ITEMS] ;
    OCIDefine*	defnp[MAX_SQL_ITEMS] ;
		OCILobLocator* pLobLocator[MAX_SQL_ITEMS];
		sb2	indicator[MAX_SQL_ITEMS] ;

#endif

#else
		void *dbproc ;
#endif

		/* Assume success */
		gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
		status = TRUE ;

		pStatus = gHyp_frame_createVariable ( pAI, pFrame, "_sql_status_" ) ;
		gHyp_data_deleteValues ( pStatus ) ;

		if ( argCount != 3 )
		gHyp_instance_error ( pAI, STATUS_ARGUMENT,
		"Invalid arguments. Usage: sql_bind ( stmt, handle, bindVariables )" ) ;

		/* Get the Bind list */
		pData3 = gHyp_stack_popRdata ( pStack, pAI ) ;

		/* Get the handle name for database */
		pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
		dbproc =
#ifdef AS_SQLSERVER
		(DBPROCESS*)
#elif AS_MYSQL
		(MYSQL*)
#elif AS_PGSQL
		(PGconn*)
#elif AS_ORACLE
		(sORACLE*)
#endif
		gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE);

		/* Get the SQL statement */
		pData2 = gHyp_stack_popRdata ( pStack, pAI ) ;

		/* Build the SQL statement */
		pSQL = sql_stmt ;
		pEndSQL = pSQL + MAX_SQL_STMT_LENGTH ;

		pResult = NULL ;
		ss = gHyp_data_getSubScript ( pData2 ) ;
		context = -1 ;
		isVector = (gHyp_data_getDataType(pData2) > TYPE_STRING);
		while ( (pResult = gHyp_data_nextValue ( pData2, pResult, &context, ss ) ) ) {

			valueLen = gHyp_data_getStr ( pResult,value,MAX_INPUT_LENGTH,context,isVector ) ;

			if ( (pSQL + valueLen) > pEndSQL )
  			gHyp_instance_error ( pAI, STATUS_SQL,
	  	  	"SQL statement longer than %d characters \n",
		  	  MAX_SQL_STMT_LENGTH ) ;
			
      sprintf ( pSQL, "%s", value ) ;
			pSQL += valueLen ;
			/* printf ("sql_stmt=%s",sql_stmt);  */

		}
		if ( context == -2 )
		  gHyp_instance_error ( pAI, STATUS_BOUNDS,
		    "Subscript is out of bounds in sql_bind()") ;

		pEndSQL = pSQL ;
		*pEndSQL = '\0' ;
		pSQL = sql_stmt ;

#ifdef AS_SQL

		/* See if the first token is a "select" */
		isSelect = FALSE ;
		pSQL += strspn ( pSQL, " \t\n\r" ) ;
		if ( pSQL < pEndSQL ) {
			n = strcspn ( pSQL, " \t\n\r" ) ;
			if ( n > 0 ) {
				strncpy ( value, pSQL, n ) ;
				value[n] = '\0' ;
				gHyp_util_lowerCase ( value, n ) ;
				isSelect = ( strcmp ( value, "select" ) == 0 ) ;
			}
		}

		results = 0 ;

#ifdef AS_ORACLE

		/* AS_ORACLE_DO_PREPARE2 is generally NOT defined,
     * the first section here is what is done
     */
		rc = OCI_SUCCESS ; 

#ifndef AS_ORACLE_DO_PREPARE2

		/* Use OCIHandleAlloc/OCIStmtPrepare/OCIHandleFree  */

		/* Create handle for the statement */ 
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
                         "sql : Creating first stmt handle.");

		rc = OCIHandleAlloc(  (dvoid *) dbproc->envhp, 
		                      (dvoid **) &stmthp, 
		                      OCI_HTYPE_STMT,
		                      (size_t) 0,
		                      (dvoid **) 0);

		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
		
    /* Create the bind handle */
    dbbind = (sBIND*) AllocMemory ( sizeof ( sBIND ) ) ;
    dbbind->stmthp = stmthp ;

		/* Prepare the statement */
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
		"sql : Preparing first stmt.");
		if ( rc == OCI_SUCCESS ) 
		rc = OCIStmtPrepare(  stmthp, 
		                      dbproc->errhp, 
		                      sql_stmt,
		                      (ub4) strlen(sql_stmt),
		                      (ub4) OCI_NTV_SYNTAX, 
		                      (ub4) OCI_DEFAULT);

#else

		/* Use OCIStmtPrepare2/OCIStmtRelease */

                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
		"sql : Preparing first stmt.");
		rc = OCIStmtPrepare2 (  dbproc->svchp,
		                        &stmthp,
		                        dbproc->errhp,
		                        sql_stmt,
		                        (ub4) strlen(sql_stmt),
		                        NULL,  /* First prepare, do not use a tag */
		                        (ub4) 0, 
		                        (ub4) OCI_NTV_SYNTAX, 
		                        (ub4) OCI_DEFAULT);
		lHyp_sql_checkErr (	dbproc->errhp, rc ) ;

#endif
		if ( rc == OCI_SUCCESS ) {

      if ( isSelect ) {

			  numCols = 0 ;
			  rc = OCIAttrGet(	stmthp, 
			                    OCI_HTYPE_STMT, 
			                    &numCols, 
			                    0, 
			                    OCI_ATTR_PARAM_COUNT, 
			                    dbproc->errhp);
        lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

			  for ( i=0 ; i<numCols; i++ ) {

				  rc = OCIParamGet(	stmthp, 
				                    OCI_HTYPE_STMT, 
				                    dbproc->errhp, 
				                    &mypard,
				                    (ub4) i+1 ) ;
					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

					/* Retrieve the column name attribute */
					rc = OCIAttrGet(	(dvoid*) mypard, 
						                (ub4) OCI_DTYPE_PARAM, 
						                (dvoid**) &pColName,
						                (ub4 *) &colNameLen, 
						                (ub4) OCI_ATTR_NAME, 
						                (OCIError *) dbproc->errhp );
					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

					rc = OCIAttrGet(	(dvoid*) mypard, 
					                  (ub4) OCI_DTYPE_PARAM, 
					                  (dvoid*) &dataType,
					                  (ub4 *) 0, 
					                  (ub4) OCI_ATTR_DATA_TYPE, 
					                  (OCIError *) dbproc->errhp  );
					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

					rc = OCIAttrGet(	(dvoid*) mypard, 
					                  (ub4) OCI_DTYPE_PARAM, 
					                  (dvoid*) &precision,
					                  (ub4 *) 0, 
					                  (ub4) OCI_ATTR_PRECISION, 
					                  (OCIError *) dbproc->errhp  );
					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

					isFloat = (dataType == SQLT_NUM && precision > 38) ? TRUE : FALSE ;

					/* Retrieve the data width in characters */
					rc = OCIAttrGet(	(dvoid*) mypard, 
					                  (ub4) OCI_DTYPE_PARAM, 
					                  (dvoid*) &colLen, 
					                  (ub4 *) 0, 
					                  (ub4) OCI_ATTR_DATA_SIZE, 
					                  (OCIError *) dbproc->errhp  );
					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

					if ( colLen < VALUE_SIZE ) colLen = VALUE_SIZE ;

					strncpy ( colName, pColName, colNameLen ) ;
					colName[colNameLen] = '\0' ;
					pColName = colName ;
          gHyp_util_upperCase ( colName, colNameLen ) ;
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
					"sql : Column name %d is %s, typ=%d",i,pColName,dataType);

				  pVariable = gHyp_frame_createVariable ( pAI, pFrame, colName ) ;
					gHyp_data_deleteValues ( pVariable ) ;

					switch ( dataType ) {
          case SQLT_CLOB:
          case SQLT_BLOB:
  					gHyp_data_setVariable ( pVariable, colName, TYPE_STRING ) ;
            gHyp_data_setStr_n ( pVariable, dataBuffer, colLen ) ;
            break ;
					case SQLT_CHR :
  					gHyp_data_setVariable ( pVariable, colName, TYPE_STRING ) ;
            gHyp_data_setStr_n ( pVariable, dataBuffer, colLen ) ;
            break ;
					case SQLT_INT :
  					gHyp_data_setVariable ( pVariable, colName, TYPE_INTEGER ) ;
            gHyp_data_setInt ( pVariable, 0 ) ;
            break ;
					case SQLT_FLT:
  					gHyp_data_setVariable ( pVariable, colName, TYPE_FLOAT ) ;
            gHyp_data_setDouble ( pVariable, 0.0 ) ;
            break ;
					case SQLT_NUM :
						/* Numeric.  Must convert */
						if ( isFloat ) {
  					  gHyp_data_setVariable ( pVariable, colName, TYPE_FLOAT ) ;
              dataType = SQLT_FLT ;
              gHyp_data_setDouble ( pVariable, 0.0 ) ;
            }
						else {
  					  gHyp_data_setVariable ( pVariable, colName, TYPE_LONG ) ;
              dataType = SQLT_INT ;
              gHyp_data_setInt ( pVariable, 0 ) ;
            }
						break ;
					case SQLT_DAT :
					case SQLT_DATE :
					case SQLT_STR :
					case SQLT_TIMESTAMP :
					case SQLT_TIMESTAMP_TZ :
					case SQLT_TIMESTAMP_LTZ :
					case SQLT_INTERVAL_YM :
					case SQLT_INTERVAL_DS :
						/* Dates: convert to string */
            gHyp_data_setVariable ( pVariable, colName, TYPE_STRING ) ;
            gHyp_data_setStr_n ( pVariable, dataBuffer, colLen ) ;
						dataType = SQLT_CHR ;
						break ;
					case SQLT_BIN :
						/* Binary */
            gHyp_data_setVariable ( pVariable, colName, TYPE_STRING ) ;
            gHyp_data_setStr_n ( pVariable, dataBuffer, colLen ) ;
            break ;
					default :
            gHyp_data_setVariable ( pVariable, colName, TYPE_STRING ) ;
            gHyp_data_setStr_n ( pVariable, dataBuffer, colLen ) ;
            dataType = SQLT_CHR ;
						break ;
					}
				}

        if ( dataType == SQLT_BLOB || dataType == SQLT_CLOB ) {

					rc = OCIDescriptorAlloc(  dbproc->envhp, 
					                          (void **)&pLobLocator[i],
					                          (ub4)OCI_DTYPE_LOB, 
					                          (size_t)0, (void**)0 ) ;
				  lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

				  rc = OCIDefineByPos(  stmthp, 
				                        &defnp[i], 
				                        dbproc->errhp,
				                        i+1, 
				                        (dvoid *) &pLobLocator[i],
				                        (sb4) 0, 
				                        (ub2) dataType,
				                        (dvoid *) 0 /*&indicator[i]*/, 
				                        (ub2 *) 0 /*&dataLen*/,
				                        (ub2 *)0, 
				                        OCI_DEFAULT);
        }
        else {
          rc = OCIDefineByPos(  stmthp, 
						                    &defnp[i], 
						                    dbproc->errhp,
						                    i+1, 
						                    (dvoid *) gHyp_data_buffer ( pVariable, 0 ),
                                (sb4) gHyp_data_bufferLen ( pVariable,0 ), 
						                    (ub2) dataType,
						                    (dvoid *) &indicator[i], 
						                    (ub2 *)&dataLen[i],
						                    (ub2 *)0, 
						                    OCI_DEFAULT);
        }
				lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
      }

      /* All statement types, INSERT, UPDATE, SELECT */
			numCols = gHyp_data_getCount ( pData3 ) ;

		  ss = gHyp_data_getSubScript ( pData3 ) ;
		  context = -1 ;
		  isVector = (gHyp_data_getDataType(pData3) > TYPE_STRING);
      pVariable = NULL ;
      i = 0 ;
      while ( (pVariable = gHyp_data_nextValue ( pData3, pVariable, &context, ss ) ) ) {

        pColName = gHyp_data_getLabel ( pVariable ) ;
        dataType = gHyp_data_getDataType ( pVariable ) ;
        sprintf ( colName, ":%s", pColName ) ;
        gHyp_util_upperCase ( colName, strlen ( colName ) ) ;

        switch ( dataType ) {
          case TYPE_BOOLEAN:
          case TYPE_BINARY:
          case TYPE_BYTE:
          case TYPE_SHORT:
          case TYPE_LONG:
          case TYPE_INTEGER:
            dataType = SQLT_INT ;
            break ;
          case TYPE_FLOAT:
          case TYPE_DOUBLE:
            dataType = SQLT_FLT ;
            break ;
          case TYPE_LIST:
          case TYPE_STRING:
          default:
            dataType = SQLT_CHR ;
            break ;
        }
  pValue = gHyp_data_getValue(pVariable,0,TRUE) ;

  data_bufferlen = (sb4) gHyp_data_bufferLen ( pValue,0 );
  data_buffer = (dvoid *) gHyp_data_buffer ( pValue, 0 ) ;
  dbbind->valuep[i] = data_buffer ;
  dbbind->alenp[i] = (sb2) gHyp_data_dataLenPtr ( pValue,0 );

                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
        "sql : Binding %s, type %d, buf=%x, len=%d",colName,dataType,data_buffer,data_bufferlen);
				rc = OCIBindByName( stmthp, 
					                  &bindp[i], 
					                  dbproc->errhp,
					                  (text *) colName, 
                            (sb4) strlen ( colName ),
						                (dvoid*) &dbbind->valuep[i]
                            (sb4) data_bufferlen, 
					                  (ub2) dataType,
					                  (dvoid *) 0, /*&indicator[i]*/ 
					                  (ub2 *) &dbbind->alenp[i],
					                  (ub2) 0, 
					                  (ub4) 0, 
					                  (ub4 *) 0, 
					                  OCI_DEFAULT);
				lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

        i++ ;

      }
		}
		if ( context == -2 )
		  gHyp_instance_error ( pAI, STATUS_BOUNDS,
		    "Subscript is out of bounds in sql_bind()") ;

		if ( results != OCI_SUCCESS && results != OCI_NO_DATA ) {

			msgLen = sprintf( msg, "Failed SQL bind, reason is '%s'.",errbuf );
			gHyp_data_setText ( pStatus, msg, msgLen ) ; 
			msg[msgLen] = '\0' ;
			gHyp_instance_warning ( pAI, STATUS_SQL, msg ) ;
			status = FALSE ;

		}
		else
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			"sql : Bound %d columns",i);

#endif

#endif    /* from AS_SQL way up above  */

		pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) dbbind ) ;
		gHyp_stack_push ( pStack, pResult ) ;
		return ;
	}
}


void gHyp_sql_query ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
	/* Description:
	*
	*    PARSE or EXECUTE the built-in function: sql_query ()
	*    Returns TRUE or FALSE
	*
	* Arguments:
	*
	*    pAI                                                     [R]
	*    - pointer to instance object
	*
	*    pCode                                                   [R]
	*    - pointer to code object
	*
	* Return value:
	*
	*    none
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
		*pStatus,
		*pTable,
		*pRow,
		*pCol,
		*pData,
		*pData2,
		*pResult ;

		sLOGICAL
		status,
		isStmtBound,
		isVector;


		int
		ss,
		context,
		valueLen,
		rows=0,
		argCount = gHyp_parse_argCount ( pParse ) ;

		char
		value[MAX_INPUT_LENGTH+1],
		*pTableStr,
		*pRowStr,
		*pColStr,
		*pSQL,
		*pEndSQL,
		sql_stmt[MAX_SQL_STMT_LENGTH+1] ;

#ifdef AS_SQL
		
		sData
		*pLvalue,
		*pAttr,
		*pColAttr,
		*pArgs=NULL,
		*pVariable=NULL,
		*variables[MAX_SQL_ITEMS] ;

		sBYTE
		hyperscript_datatype[MAX_SQL_ITEMS],
		*pBytes ;

		int
		i,
		colTypes[MAX_SQL_ITEMS],
		colLens[MAX_SQL_ITEMS],
		numCols,
		n ;

		char
		*pAttrStr,
		*colNames[MAX_SQL_ITEMS],
		*pColName ;

		int msgLen ;
		char msg[MAX_OUTPUT_LENGTH+1] ;

		sLOGICAL
		isSelect,
		isCommit ;
#endif

#ifdef AS_SQL
#ifdef AS_SQLSERVER

		DBPROCESS  *dbproc ;
		RETCODE    results ;
		char      buffer[MAX_BUFFER_SIZE+1];

#elif AS_MYSQL

		MYSQL *dbproc ;
		MYSQL_RES *results ;
		MYSQL_FIELD *field ;
		MYSQL_ROW row ;
		unsigned long *length ;

#elif AS_PGSQL

		PGconn      *dbproc ;
		PGresult    *results ;
		int
		numRows,
		isDataBinary[MAX_SQL_ITEMS],
		row,col;

#elif AS_ORACLE

		sORACLE	*dbproc ;
		sword	results ;
		sword	rc ; 
    sBIND* dbbind ;
		OCIStmt    *stmthp;
		OCIParam   *mypard;
		void*	dataBuffer[MAX_SQL_ITEMS] ;
		ub4	dataBufferLen[MAX_SQL_ITEMS] ;
		ub4	amount ;
		ub4	offset ;
    ub4 iters ;
#ifdef AS_ORACLE_DO_PREPARE2
		OraText	*key = "mykey" ;
    ub4	keylen = strlen (key) ;
#endif
		ub2	dataLen[MAX_SQL_ITEMS] ;
		/*dvoid*	dataBufferPtr[MAX_SQL_ITEMS] ;*/
		OCIDefine*	defnp[MAX_SQL_ITEMS] ;
		OCILobLocator* pLobLocator[MAX_SQL_ITEMS];
		sb2	indicator[MAX_SQL_ITEMS],
		precision ;
		sLOGICAL	isFloat[MAX_SQL_ITEMS];
		int	col,
		colNameLen ;
		ub2	dataType ;
		ub2	colLen ;
		ub2	orientation ;

		char colName[VALUE_SIZE+1] ;

#endif

#else
		void *dbproc ;
#endif

		/* Assume success */
		gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
		status = TRUE ;

		pStatus = gHyp_frame_createVariable ( pAI, pFrame, "_sql_status_" ) ;
		gHyp_data_deleteValues ( pStatus ) ;

		if ( argCount < 2 || argCount > 5 )
		gHyp_instance_error ( pAI, STATUS_ARGUMENT,
		"Invalid arguments. Usage: sql_query ( stmt, handle, [table [,row [,col]]] )" ) ;

		pTableStr = NULL ;
		pRowStr = NULL ;
		pColStr = NULL ;
		pTable = NULL ;
		pCol = NULL ;
		pRow = NULL ;

		if ( argCount > 4 )
		pColStr = gHyp_data_getLabel ( gHyp_stack_popLvalue ( pStack, pAI ) ) ;

		if ( argCount > 3 )
		pRowStr = gHyp_data_getLabel ( gHyp_stack_popLvalue ( pStack, pAI ) ) ;

		if ( argCount > 2 )
		pTableStr = gHyp_data_getLabel ( gHyp_stack_popLvalue ( pStack, pAI ) ) ;

		/* Get the handle name for database */
		pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
		dbproc =
#ifdef AS_SQLSERVER
		(DBPROCESS*)
#elif AS_MYSQL
		(MYSQL*)
#elif AS_PGSQL
		(PGconn*)
#elif AS_ORACLE
		(sORACLE*)
#endif
		gHyp_data_getHandle ( pData,
		gHyp_data_getSubScript(pData),
		TRUE);

		/* Get the SQL statement */
		pData2 = gHyp_stack_popRdata ( pStack, pAI ) ;

		if ( gHyp_data_getDataType ( pData2 ) == TYPE_HANDLE ) {
			isStmtBound = TRUE ;
#ifdef AS_ORACLE
			dbbind = (sBIND*) gHyp_data_getHandle ( pData2, 0, TRUE ) ;
      stmthp = dbbind->stmthp ;
#endif
		}
		else {
			dbbind = (sBIND*) NULL ;
			isStmtBound = FALSE ;
		}

		if ( !isStmtBound ) {

			/* Build the SQL statement - Note: SQLSERVER doesn't use sql_stmt */
			pSQL = sql_stmt ;
			pEndSQL = pSQL + MAX_SQL_STMT_LENGTH ;

			pResult = NULL ;
			ss = gHyp_data_getSubScript ( pData2 ) ;
			context = -1 ;
			isVector = (gHyp_data_getDataType(pData2) > TYPE_STRING);
			while ( (pResult = gHyp_data_nextValue ( pData2,
			pResult,
			&context,
			ss ) ) ) {

				valueLen = gHyp_data_getStr ( pResult,
				value,
				MAX_INPUT_LENGTH,
				context,
				isVector ) ;
#ifdef AS_SQLSERVER
				/* For SQLSERVER, feed the sql stmt in one piece at a time.*/
				dbcmd (dbproc, value );
#endif

				if ( (pSQL + valueLen) > pEndSQL )
				gHyp_instance_error ( pAI, STATUS_SQL,
				"SQL statement longer than %d characters \n",
				MAX_SQL_STMT_LENGTH ) ;
				sprintf ( pSQL, "%s", value ) ;
				pSQL += valueLen ;
				/* printf ("sql_stmt=%s",sql_stmt);  */

			}
			if ( context == -2 )
			gHyp_instance_error ( pAI, STATUS_BOUNDS,
			"Subscript is out of bounds in sql_query()") ;

			pEndSQL = pSQL ;
			*pEndSQL = '\0' ;
			pSQL = sql_stmt ;

#ifdef AS_SQL

			/* See if the first token is a "select" */
			isSelect = FALSE ;
			isCommit = FALSE ;
			pSQL += strspn ( pSQL, " \t\n\r" ) ;
			if ( pSQL < pEndSQL ) {
				n = strcspn ( pSQL, " \t\n\r" ) ;
				if ( n > 0 ) {
					strncpy ( value, pSQL, n ) ;
					value[n] = '\0' ;
					gHyp_util_lowerCase ( value, n ) ;
					isSelect = ( strcmp ( value, "select" ) == 0 ) ;
					isCommit = ( strcmp ( value, "commit" ) == 0 ) ;
				}
			}
		} /* !isBound */

		results = 0 ;

#ifdef AS_SQLSERVER

		if ( (results = dbsqlexec (dbproc)) == SUCCEED )

#elif AS_MYSQL

		if ( mysql_query ( dbproc, sql_stmt ) == 0 )

#elif AS_PGSQL

		results = PQexec( dbproc, sql_stmt );
		if ((PQresultStatus(results) == PGRES_TUPLES_OK) )

#elif AS_ORACLE

		/* AS_ORACLE_DO_PREPARE2 is generally NOT defined,
* the first section here is what is done
*/
		rc = OCI_SUCCESS ; 

#ifndef AS_ORACLE_DO_PREPARE2

		/* Use OCIHandleAlloc/OCIStmtPrepare/OCIHandleFree  */

		if ( !isStmtBound ) {

			/* Create handle for the statement */ 
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			"sql : Creating first stmt handle.");
			rc = OCIHandleAlloc(  (dvoid *) dbproc->envhp, 
			(dvoid **) &stmthp, 
			OCI_HTYPE_STMT,
			(size_t) 0,
			(dvoid **) 0);

			lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
			
			/* Prepare the statement */
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			"sql : Preparing first stmt.");
			if ( rc == OCI_SUCCESS ) 
			rc = OCIStmtPrepare(stmthp, 
			dbproc->errhp, 
			sql_stmt,
			(ub4) strlen(sql_stmt),
			(ub4) OCI_NTV_SYNTAX, 
			(ub4) OCI_DEFAULT);

#else

			/* Use OCIStmtPrepare2/OCIStmtRelease */

                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			"sql : Preparing first stmt.");
			rc = OCIStmtPrepare2 ( dbproc->svchp,
			&stmthp,
			dbproc->errhp,
			sql_stmt,
			(ub4) strlen(sql_stmt),
			NULL,  /* First prepare, do not use a tag */
			(ub4) 0, 
			(ub4) OCI_NTV_SYNTAX, 
			(ub4) OCI_DEFAULT);
			lHyp_sql_checkErr (	dbproc->errhp, rc ) ;

#endif

			/* We generally do NOT want AS_ORACLE_COMMIT_ON_SUCCESS as it slows down performance for high volumne transactions. */
#ifdef AS_ORACLE_COMMIT_ON_SUCCESS
			orientation = isSelect ? OCI_DESCRIBE_ONLY : (isCommit ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT ) ;
#else
			orientation = isSelect ? OCI_DESCRIBE_ONLY : OCI_COMMIT_ON_SUCCESS ;
#endif
      iters = 1 ; 
		}
		else {
			/* Bound stmt */
			rc = OCI_SUCCESS ;
			isSelect = FALSE ;
			orientation = OCI_DEFAULT  ;
      iters = 1 ;
    }

		if ( rc == OCI_SUCCESS ) {
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			"sql : Executing first stmt");
			rc = OCIStmtExecute(  dbproc->svchp, 
			                      stmthp,
			                      dbproc->errhp,
			                      (ub4) iters, 
			                      (ub4) 0,
			                      (CONST OCISnapshot *) NULL, 
			                      (OCISnapshot *) NULL, 
			                      orientation ) ;

			lHyp_sql_checkErr (	dbproc->errhp, rc ) ;
		}

		results = rc ;

		if ( rc == OCI_NO_DATA ) {

			/* ORACLE OCI_NO_DATA is a warning */

			/* For SELECT statements, it means SUCCESS, but no data was fetched */
			if ( isSelect ) rc = OCI_SUCCESS ;

			/* For non select statements, it means failure */
			else rc = OCI_ERROR ;

		}

		if ( rc == OCI_SUCCESS && isSelect )
#endif
		{
			/* now check the results from the SQL server */

			if ( pTableStr ) {

				/* List variables are created inside of pTable */
				pTable = gHyp_frame_createVariable ( pAI, pFrame, pTableStr ) ;
				gHyp_data_deleteValues ( pTable ) ;

			}
			else {

				/* List variables are top-level variables. */

				/* Create or retrieve the "sqlargs" variable, and clear it of any values. */
				pArgs = gHyp_frame_createVariable ( pAI, pFrame, "sqlargs" ) ;
				gHyp_data_deleteValues ( pArgs ) ;
			}

			while

#ifdef AS_SQLSERVER
			( (results = dbresults(dbproc)) != NO_MORE_RESULTS )
#elif AS_MYSQL
			( (results = mysql_use_result(dbproc)) != NULL )
#elif AS_PGSQL
			( results )
#elif AS_ORACLE
			( results == OCI_SUCCESS )
#endif
			{

				if
#ifdef AS_SQLSERVER
				(results == SUCCEED)
#elif AS_MYSQL
				(results != NULL )
#elif AS_PGSQL
				(results != NULL )
#elif AS_ORACLE
				(results == OCI_SUCCESS )
#endif
				{
					if ( pColStr != NULL ) pRow = gHyp_data_new ( pRowStr ) ;

					/* Get info on each column */
#ifdef AS_SQLSERVER
					numCols = dbnumcols ( dbproc );
#elif AS_MYSQL
					numCols = mysql_num_fields ( results ) ;
#elif AS_PGSQL
					numCols = PQnfields ( results );
#elif AS_ORACLE
					numCols = 0 ;
					rc = OCIAttrGet(	stmthp, 
					OCI_HTYPE_STMT, 
					&numCols, 
					0, 
					OCI_ATTR_PARAM_COUNT, 
					dbproc->errhp);

#endif
					for ( i=0 ; i<numCols; i++ ) {

#ifdef AS_SQLSERVER
						pColName = (char*) dbcolname ( dbproc, i+1 ) ;
#elif AS_MYSQL
						field = mysql_fetch_field ( results ) ;
						pColName = field->name ;
#elif AS_PGSQL
						pColName = (char*) PQfname( results, i) ;
#elif AS_ORACLE

						rc = OCIParamGet(	stmthp, 
						OCI_HTYPE_STMT, 
						dbproc->errhp, 
						&mypard,
						(ub4) i+1 ) ;

						/* Retrieve the column name attribute */
						rc = OCIAttrGet(	(dvoid*) mypard, 
						(ub4) OCI_DTYPE_PARAM, 
						(dvoid**) &pColName,
						(ub4 *) &colNameLen, 
						(ub4) OCI_ATTR_NAME, 
						(OCIError *) dbproc->errhp );

						strncpy ( colName, pColName, colNameLen ) ;
						colName[colNameLen] = '\0' ;
						pColName = colName ;

#endif
						colNames[i] = (char*) AllocMemory ( strlen ( pColName ) + 1 ) ;
						strcpy ( colNames[i], pColName ) ;
						gHyp_util_upperCase ( colNames[i], strlen ( colNames[i] ) ) ;

						if ( pTable == NULL ) {

							/* List variables are created from the column names */
							pVariable = gHyp_frame_createVariable ( pAI, pFrame, colNames[i] ) ;
							gHyp_data_deleteValues ( pVariable ) ;
							gHyp_data_setVariable ( pVariable, colNames[i], TYPE_LIST ) ;

							/* Store a reference to the variable in the "sqlargs" variable */
							pData = gHyp_data_new ( NULL ) ;
							gHyp_data_setReference ( pData, colNames[i], NULL ) ;
							gHyp_data_append ( pArgs, pData ) ;
						}
						else {

							if ( pColStr != NULL ) {

								pVariable = gHyp_data_new ( NULL ) ;
								gHyp_data_setStr ( pVariable, colNames[i] ) ;
								pCol = gHyp_data_new ( pColStr ) ;
								gHyp_data_append ( pCol, pVariable ) ;
								gHyp_data_append ( pRow, pCol ) ;
								gHyp_data_append ( pTable, pRow ) ;

							}
							else if ( pRowStr == NULL ) {
								/* Table contains empty list variables */
								pVariable = gHyp_data_new ( colNames[i] ) ;
								gHyp_data_append ( pTable, pVariable ) ;
							}
						}

						/* Store the variable pointer and column type for later use */
						variables[i] = pVariable ;
#ifdef AS_SQLSERVER
						colTypes[i] = dbcoltype ( dbproc, i+1 ) ;
						colLens[i] = dbcollen ( dbproc, i+1 ) ;

#elif AS_MYSQL
						colTypes[i] = field->type ;
						colLens[i] = field->length ;
#elif AS_PGSQL
						colTypes[i] = PQftype ( results, i ) ;
						colLens[i] = PQfsize( results, i ) ;
						if ( colLens[i] == -1 )
						colLens[i] = PQfmod( results, i ) ;
						isDataBinary[i] = PQfformat ( results, i ) ;
#elif AS_ORACLE
						rc = OCIAttrGet(	(dvoid*) mypard, 
						(ub4) OCI_DTYPE_PARAM, 
						(dvoid*) &dataType,
						(ub4 *) 0, 
						(ub4) OCI_ATTR_DATA_TYPE, 
						(OCIError *) dbproc->errhp  );
						lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

						colTypes[i] = dataType ;
						rc = OCIAttrGet(	(dvoid*) mypard, 
						(ub4) OCI_DTYPE_PARAM, 
						(dvoid*) &precision,
						(ub4 *) 0, 
						(ub4) OCI_ATTR_PRECISION, 
						(OCIError *) dbproc->errhp  );

						lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
						isFloat[i] = (dataType == SQLT_NUM && precision > 38) ? TRUE : FALSE ;

						/* Retrieve the data width in characters */
						rc = OCIAttrGet(	(dvoid*) mypard, 
						(ub4) OCI_DTYPE_PARAM, 
						(dvoid*) &colLen, 
						(ub4 *) 0, 
						(ub4) OCI_ATTR_DATA_SIZE, 
						(OCIError *) dbproc->errhp  );
						lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

						colLens[i] = colLen ;
						if ( colLen < VALUE_SIZE ) colLen = VALUE_SIZE ;

						if ( dataType == SQLT_CLOB ||
								dataType == SQLT_BLOB ) {

							dataBufferLen[i] = MAX_SQL_BUFFER_SIZE ;
							/* Allocate a buffer to receive the data for column i+1 */
							dataBuffer[i] = (void*) AllocMemory ( dataBufferLen[i] ) ;
							/*dataBufferPtr[i] = &dataBuffer[i] ;*/
							rc = OCIDescriptorAlloc(  dbproc->envhp, 
							                          (void **)&pLobLocator[i],
							                          (ub4)OCI_DTYPE_LOB, 
							                          (size_t)0, (void**)0 ) ;
							/*(size_t)dataBufferLen[i], 
											(void**)&dataBufferPtr[i] ) ;*/
							lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

							/*
							rc = OCILobEnableBuffering ( dbproc->svchp, 
									dbproc->errhp, 
									pLobLocator[i] ) ;
							lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
							*/
						}
						else {
							dataBufferLen[i] = colLen ;
							/* Allocate a buffer to receive the data for column i+1 */
							dataBuffer[i] = (void*) AllocMemory ( dataBufferLen[i]+1 ) ;
						}
#endif
					} /* cols++ */

					/* now process the rows */
					rows = 0 ;

#ifdef AS_SQLSERVER
					for ( i=0; i<numCols;i++ ) {
						switch ( colTypes[i] ) {
						case SQLINT1 :
							hyperscript_datatype[i] = TYPE_BYTE ;
							break ;
						case SQLINT2 :
							hyperscript_datatype[i] = TYPE_SHORT ;
							break ;
						case SQLINT4 :
							hyperscript_datatype[i] = TYPE_LONG ;
							break ;
						case SQLFLT4 :
							hyperscript_datatype[i] = TYPE_FLOAT ;
							break ;
						case SQLFLT8 :
							hyperscript_datatype[i] = TYPE_DOUBLE ;
							break ;
						case SQLVARCHAR :
						case SQLCHAR :
						case SQLTEXT :
							hyperscript_datatype[i] = TYPE_STRING ;
							break ;
						case SQLIMAGE :
						case SQLBINARY :
						case SQLVARBINARY :
							hyperscript_datatype[i] = TYPE_STRING ;
							reak ;
						case SQLDATETIME:
							hyperscript_datatype[i] = TYPE_DATETIME ;
							break ;
						case SQLDECIMAL:
						case SQLNUMERIC:
							hyperscript_datatype[i] = TYPE_DOUBLE ;
							break ;
							default :
							hyperscript_datatype[i] = TYPE_STRING ;
							break ;
						}
					}

					while ( dbnextrow(dbproc) != NO_MORE_ROWS) {

						rows++ ;

						if ( pRowStr ) {
							pRow = gHyp_data_new ( pRowStr ) ;
							gHyp_data_append ( pTable, pRow ) ;
						}

						for ( i=0 ; i<numCols; i++ ) {

							pData = gHyp_data_new ( NULL ) ;

							n = dbdatlen ( dbproc, i+1 ) ;
							if ( n > 0 ) {

								pBytes = (sBYTE*) dbdata ( dbproc, i+1 ) ;

								switch ( colTypes[i] ) {

								case SQLINT1 :
									gHyp_data_newConstant_raw ( pData, TYPE_BYTE, pBytes ) ;
									break ;

								case SQLINT2 :
									gHyp_data_newConstant_raw ( pData, TYPE_SHORT, pBytes ) ;
									break ;

								case SQLINT4 :
									gHyp_data_newConstant_raw ( pData, TYPE_LONG, pBytes ) ;
									break ;

								case SQLFLT4 :
									gHyp_data_newConstant_raw ( pData, TYPE_FLOAT, pBytes ) ;
									break ;

								case SQLFLT8 :
									gHyp_data_newConstant_raw ( pData, TYPE_DOUBLE, pBytes ) ;
									break ;

								case SQLVARCHAR :
								case SQLCHAR :
									if ( n == 0 ) 
									gHyp_data_setStr_n (pData, "NULL", 4 ) ;

									else if ( colLens[i] <= INTERNAL_VALUE_SIZE )
									gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;

									else {
										gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
										gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
									}
									/*gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;*/
									break ;

								case SQLTEXT :
								case SQLIMAGE :
								case SQLBINARY :
								case SQLVARBINARY :

									gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
									gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
									break ;

								case SQLDATETIME:
									n = dbconvert (	dbproc,
									colTypes[i],
									pBytes,
									n,
									SQLCHAR,
									(unsigned char*) buffer,
									MAX_BUFFER_SIZE);
									gHyp_data_setStr_n ( pData, (char*) buffer, n ) ;
									break ;

								case SQLDECIMAL:
								case SQLNUMERIC:

									n = dbconvert (	dbproc,
									colTypes[i],
									pBytes,
									n,
									SQLCHAR,
									(unsigned char*) buffer,
									MAX_BUFFER_SIZE);
									if ( n < 0 ) gHyp_data_newConstant_scanf ( pData, TYPE_DOUBLE, (char*)buffer, n ) ;
									break ;

									default :

									n = dbconvert (	dbproc,
									colTypes[i],
									pBytes,
									n,
									SQLCHAR,
									(unsigned char*) buffer,
									MAX_BUFFER_SIZE);

									if ( n == 0 ) 
									gHyp_data_setStr_n (pData, "NULL", 4 ) ;

									else if ( colLens[i] <= INTERNAL_VALUE_SIZE )
									gHyp_data_setStr_n ( pData, (char*) buffer, n ) ;

									else {
										gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
										gHyp_util_breakStream ( (char*) buffer, n, pData, TRUE ) ;
									}
									break ;

								}
							}

							if ( pColStr != NULL ) {

								/* Append the column element to the row element */
								pCol = gHyp_data_new ( NULL ) ;
								gHyp_data_setVariable ( pCol, pColStr, TYPE_LIST ) ;
								gHyp_data_append( pCol, pData ) ;

								if ( hyperscript_datatype[i] != TYPE_STRING &&
										hyperscript_datatype[i] != TYPE_DATETIME ) {

									pLvalue = gHyp_data_new ( NULL ) ;
									gHyp_data_setReference ( pLvalue, pColStr, pCol ) ;
									pResult = gHyp_type_assign ( 
									pAI,
									gHyp_instance_frame ( pAI ),
									pLvalue,
									NULL,
									hyperscript_datatype[i], /* Into this datatype */
									FALSE,	
									FALSE  ) ;
									gHyp_data_delete ( pResult ) ;
									gHyp_data_delete ( pLvalue ) ;
								}
								gHyp_data_append ( pRow, pCol ) ;
							}
							else if ( pRowStr != NULL ) {

								/* Add the element to the row variable */
								pVariable = gHyp_data_new ( NULL ) ;
								gHyp_data_setVariable ( pVariable, colNames[i], TYPE_LIST ) ;
								gHyp_data_append( pVariable, pData ) ;

								if ( hyperscript_datatype[i] != TYPE_STRING &&
										hyperscript_datatype[i] != TYPE_DATETIME ) {

									pLvalue = gHyp_data_new ( NULL ) ;
									gHyp_data_setReference ( pLvalue, colNames[i], pVariable ) ;	
									pResult = gHyp_type_assign ( 
									pAI,
									gHyp_instance_frame ( pAI ),
									pLvalue,
									NULL,
									hyperscript_datatype[i], /* Into this datatype */
									FALSE,	
									FALSE  ) ;
									gHyp_data_delete ( pResult ) ;
									gHyp_data_delete ( pLvalue ) ;
								}
								gHyp_data_append ( pRow, pVariable ) ;
							}
							else {
								/* pVariable is already created */
								pVariable = variables[i] ;
								gHyp_data_append ( pVariable, pData ) ;
							}
						}
					}
#elif AS_MYSQL
					for ( i=0; i<numCols;i++ ) {
						switch ( colTypes[i] ) {
						case FIELD_TYPE_TINY :
							hyperscript_datatype[i] = TYPE_BYTE ;
							break ;
						case FIELD_TYPE_SHORT :
							hyperscript_datatype[i] = TYPE_SHORT ;
							break ;
						case FIELD_TYPE_LONG :
							hyperscript_datatype[i] = TYPE_LONG ;
							break ;
						case FIELD_TYPE_FLOAT :
							hyperscript_datatype[i] = TYPE_FLOAT ;
							break ;
						case FIELD_TYPE_DOUBLE :
							hyperscript_datatype[i] = TYPE_DOUBLE ;
							break ;
						case FIELD_TYPE_STRING :
						case FIELD_TYPE_VAR_STRING :
							hyperscript_datatype[i] = TYPE_STRING ;
							break ;
						case FIELD_TYPE_BLOB :
							hyperscript_datatype[i] = TYPE_STRING ;
							break ;
						case FIELD_TYPE_TIMESTAMP :
						case FIELD_TYPE_DATE :
						case FIELD_TYPE_TIME :
						case FIELD_TYPE_DATETIME :
						case FIELD_TYPE_YEAR :
							hyperscript_datatype[i] = TYPE_DATETIME ;
							break ;
						case FIELD_TYPE_SET :
						case FIELD_TYPE_ENUM :
						case FIELD_TYPE_NULL :
						case FIELD_TYPE_LONGLONG :
						default:
							hyperscript_datatype[i] = TYPE_STRING ;
							break ;
						}
					}
					
					while ( (row = mysql_fetch_row (results)) != NULL ) {

						rows++ ;

						if ( pRowStr ) {
							pRow = gHyp_data_new ( pRowStr ) ;
							gHyp_data_append ( pTable, pRow ) ;
						}

						length = mysql_fetch_lengths ( results ) ;

						for ( i=0 ; i<numCols; i++ ) {

							pData = gHyp_data_new ( NULL ) ;

							n = length[i] ;

							if ( n > 0 ) {

								pBytes = (sBYTE*) row[i] ;

								switch ( colTypes[i] ) {

								case FIELD_TYPE_TINY :
									gHyp_data_newConstant_scanf ( pData, TYPE_BYTE, (char*)pBytes, n ) ;
									break ;

								case FIELD_TYPE_SHORT :
									gHyp_data_newConstant_scanf ( pData, TYPE_SHORT, (char*)pBytes, n ) ;
									break ;

								case FIELD_TYPE_LONG :
									gHyp_data_newConstant_scanf ( pData, TYPE_LONG, (char*)pBytes, n ) ;
									break ;

								case FIELD_TYPE_FLOAT :
									gHyp_data_newConstant_scanf ( pData, TYPE_FLOAT, (char*)pBytes, n ) ;
									break ;

								case FIELD_TYPE_DOUBLE :
									gHyp_data_newConstant_scanf ( pData, TYPE_DOUBLE, (char*)pBytes, n ) ;
									break ;

								case FIELD_TYPE_STRING :
								case FIELD_TYPE_VAR_STRING :
									if ( n == 0 ) 
									gHyp_data_setStr_n (pData, "NULL", 4 ) ;

									else if ( colLens[i] <= INTERNAL_VALUE_SIZE )
									gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;

									else {
										gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
										gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
									}
									/*gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;*/
									break ;

								case FIELD_TYPE_BLOB :

									gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
									gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
									break ;

								case FIELD_TYPE_TIMESTAMP :
								case FIELD_TYPE_DATE :
								case FIELD_TYPE_TIME :
								case FIELD_TYPE_DATETIME :
								case FIELD_TYPE_YEAR :
									gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
									break ;

								case FIELD_TYPE_SET :
								case FIELD_TYPE_ENUM :
								case FIELD_TYPE_NULL :
								case FIELD_TYPE_LONGLONG :
								default:

									if ( n == 0 ) 
									gHyp_data_setStr_n (pData, "NULL", 4 ) ;
									else if ( colLens[i] <= INTERNAL_VALUE_SIZE )
									gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
									else {
										gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
										gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
									}
									break ;

								}
							}
							if ( pColStr != NULL ) {

								/* Append the column element to the row element */
								pCol = gHyp_data_new ( NULL ) ;
								gHyp_data_setVariable ( pCol, pColStr, TYPE_LIST ) ;
								gHyp_data_append( pCol, pData ) ;
								if ( hyperscript_datatype[i] != TYPE_STRING &&
										hyperscript_datatype[i] != TYPE_DATETIME ) {

									pLvalue = gHyp_data_new ( NULL ) ;
									gHyp_data_setReference ( pLvalue, pColStr, pCol ) ;
									pResult = gHyp_type_assign ( 
									pAI,
									gHyp_instance_frame ( pAI ),
									pLvalue,
									NULL,
									hyperscript_datatype[i], /* Into this datatype */
									FALSE,	
									FALSE  ) ;
									gHyp_data_delete ( pResult ) ;
									gHyp_data_delete ( pLvalue ) ;
								}
								gHyp_data_append ( pRow, pCol ) ;

							}
							else if ( pRowStr != NULL ) {

								/* Add the element to the row variable */
								pVariable = gHyp_data_new ( NULL ) ;
								gHyp_data_setVariable ( pVariable, colNames[i], TYPE_LIST ) ;
								gHyp_data_append( pVariable, pData ) ;
								if ( hyperscript_datatype[i] != TYPE_STRING &&
										hyperscript_datatype[i] != TYPE_DATETIME ) {

									pLvalue = gHyp_data_new ( NULL ) ;
									gHyp_data_setReference ( pLvalue, colNames[i], pVariable ) ;	
									pResult = gHyp_type_assign ( 
									pAI,
									gHyp_instance_frame ( pAI ),
									pLvalue,
									NULL,
									hyperscript_datatype[i], /* Into this datatype */
									FALSE,	
									FALSE  ) ;
									gHyp_data_delete ( pResult ) ;
									gHyp_data_delete ( pLvalue ) ;
								}
								gHyp_data_append ( pRow, pVariable ) ;

							}
							else {
								/* pVariable is already created */
								pVariable = variables[i] ;
								gHyp_data_append( pVariable, pData ) ;
							}
						}
					}
#elif AS_PGSQL
					for ( col=0 ; col<numCols; col++ ) {
						switch ( colTypes[col] ) {
						case 16 : /*BOOLOID*/
							/* Boolean - true/false */
							hyperscript_datatype[col] = TYPE_BYTE ;
							break ;
						case 18: /*CHAROID*/
							/* TYPE_CHAR */
							hyperscript_datatype[col] = TYPE_CHAR ;
							break ;
						case 21 : /*INT2OID*/
							/* TYPE_SHORT */
							hyperscript_datatype[col] = TYPE_SHORT ;
							break ;
						case 23 : /*INT4OID*/
						case 702: /*ABSTIMEOID*/
							hyperscript_datatype[col] = TYPE_LONG ;
							break ;
						case 700 : /*FLOAT4OID*/
							hyperscript_datatype[col] = TYPE_FLOAT ;
							break ;
						case 701 : /*FLOAT8OID*/
							hyperscript_datatype[col] = TYPE_DOUBLE ;
							break ;
						case 1082: /*DATEOID*/
						case 1083: /*TIMEOID*/
						case 1114: /*TIMESTAMPOID */
							/* Datetime */
							hyperscript_datatype[col] = TYPE_DATETIME ;
							break ;
						case 17: /*BYTEOID*/
						case 25: /*TEXTOID*/
						case 1043: /*VARCHAROID */
							/* Text */
							hyperscript_datatype[col] = TYPE_STRING ;
							break ;
							default :
							hyperscript_datatype[col] = TYPE_STRING ;
							break ;
						}
					}

					numRows = PQntuples( results ) ;
					rows = 0 ;
					for ( row=0; row < numRows; row++ )  {

						rows++ ;

						if ( pRowStr ) {
							pRow = gHyp_data_new ( pRowStr ) ;
							gHyp_data_append ( pTable, pRow ) ;
						}

						for ( col=0 ; col<numCols; col++ ) {

							pData = gHyp_data_new ( NULL ) ;
							n = PQgetlength (results, row, col);
							pBytes = (sBYTE*) PQgetvalue (results, row, col);

							switch ( colTypes[col] ) {

							case 16 : /*BOOLOID*/
								/* Boolean - true/false */
								if ( strcmp ( (char*) pBytes, "true" ) == 0 )
								gHyp_data_setBool ( pData, TRUE ) ;
								else
								gHyp_data_setBool ( pData, TRUE ) ;
								break ;

							case 18: /*CHAROID*/
								/* TYPE_CHAR */
								gHyp_data_newConstant_raw ( pData, TYPE_CHAR, (char*)pBytes ) ;
								break ;

							case 21 : /*INT2OID*/
								/* TYPE_SHORT */
								if ( isDataBinary[col] )
								gHyp_data_newConstant_raw ( pData, TYPE_SHORT, (char*)pBytes ) ;
								else
								gHyp_data_newConstant_scanf ( pData, TYPE_SHORT, (char*)pBytes, n ) ;
								break ;

							case 23 : /*INT4OID*/
							case 702: /*ABSTIMEOID*/

								/* TYPE_LONG */
								if ( isDataBinary[col] )
								gHyp_data_newConstant_raw ( pData, TYPE_LONG, (char*)pBytes ) ;
								else
								gHyp_data_newConstant_scanf ( pData, TYPE_LONG, (char*)pBytes, n ) ;
								break ;

							case 700 : /*FLOAT4OID*/
								/* 4-byte single-precision */
								if ( isDataBinary[col] )
								gHyp_data_newConstant_raw ( pData, TYPE_FLOAT, (char*)pBytes ) ;
								else
								gHyp_data_newConstant_scanf ( pData, TYPE_FLOAT, (char*)pBytes, n ) ;
								break ;

							case 701 : /*FLOAT8OID*/
								if ( isDataBinary[col] )
								gHyp_data_newConstant_raw ( pData, TYPE_DOUBLE, (char*)pBytes ) ;
								else
								gHyp_data_newConstant_scanf ( pData, TYPE_DOUBLE, (char*)pBytes, n ) ;
								break ;

							case 1082: /*DATEOID*/
							case 1083: /*TIMEOID*/
							case 1114: /*TIMESTAMPOID */

								/* Datetime */
								gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
								break ;

							case 17:   /*BYTEOID*/
							case 1043: /*VARCHAROID */

								/* Text */
								if ( n == 0 ) 
								gHyp_data_setStr_n (pData, "NULL", 4 ) ;

								else if ( colLens[col] <= INTERNAL_VALUE_SIZE )
								gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;

								else {
									gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
									gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
								}

								/*gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;*/
								break ;

							case 25: /*TEXTOID*/
								gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
								gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
								break ;

								default :
								if ( n == 0 ) 
								gHyp_data_setStr_n (pData, "NULL", 4 ) ;
								else if ( colLens[col] <= INTERNAL_VALUE_SIZE )
								gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
								else {
									gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
									gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
								}
								break ;


							}
							if ( pColStr != NULL ) {

								/* Append the column element to the row element */
								pCol = gHyp_data_new ( NULL ) ;
								gHyp_data_setVariable ( pCol, pColStr, TYPE_LIST ) ;
								gHyp_data_append( pCol, pData ) ;
								if ( hyperscript_datatype[col] != TYPE_STRING &&
										hyperscript_datatype[col] != TYPE_DATETIME ) {

									pLvalue = gHyp_data_new ( NULL ) ;
									gHyp_data_setReference ( pLvalue, pColStr, pCol ) ;	
									pResult = gHyp_type_assign ( 
									pAI,
									gHyp_instance_frame ( pAI ),
									pLvalue,
									NULL,
									hyperscript_datatype[col], /* Into this datatype */
									FALSE,	
									FALSE  ) ;
									gHyp_data_delete ( pResult ) ;
									gHyp_data_delete ( pLvalue ) ;
								}
								gHyp_data_append ( pRow, pCol ) ;

							}
							else if ( pRowStr != NULL ) {

								/* Append the column element to the row element */
								pVariable = gHyp_data_new ( NULL ) ;
								gHyp_data_setVariable ( pVariable, colNames[col], TYPE_LIST ) ;
								gHyp_data_append( pVariable, pData ) ;

								if ( hyperscript_datatype[col] != TYPE_STRING &&
										hyperscript_datatype[col] != TYPE_DATETIME ) {

									pLvalue = gHyp_data_new ( NULL ) ;
									gHyp_data_setReference ( pLvalue, colNames[col], pVariable ) ;	
									pResult = gHyp_type_assign ( 
									pAI,
									gHyp_instance_frame ( pAI ),
									pLvalue,
									NULL,
									hyperscript_datatype[col], /* Into this datatype */
									FALSE,	
									FALSE  ) ;
									gHyp_data_delete ( pResult ) ;
									gHyp_data_delete ( pLvalue ) ;
								}
								gHyp_data_append ( pRow, pVariable ) ;

							}
							else {
								/* pVariable is already created */
								pVariable = variables[col] ;
								gHyp_data_append( pVariable, pData ) ;
							}
						}
					}
					/*  end of PGSQL row processing */

#elif AS_ORACLE

					for ( col=0 ; col<numCols; col++ ) {
						switch ( colTypes[col] ) {
						case SQLT_CHR :
							/* VARCHAR2 */
							hyperscript_datatype[col] = TYPE_STRING ;
							break ;
						case SQLT_INT :
							/* Integer */
							hyperscript_datatype[col] = TYPE_LONG ;
							break ;
						case SQLT_FLT:
							/* Float */
							hyperscript_datatype[col] = TYPE_FLOAT ;
							break ;
						case SQLT_NUM :
							/* Numeric.  Must convert */
							if ( isFloat[col] ) {
								hyperscript_datatype[col] = TYPE_DOUBLE ;
							}
							else {
								hyperscript_datatype[col] = TYPE_LONG ;
							}
							break ;
						case SQLT_DAT :
						case SQLT_DATE :
						case SQLT_STR :
						case SQLT_TIMESTAMP :
						case SQLT_TIMESTAMP_TZ :
						case SQLT_TIMESTAMP_LTZ :
						case SQLT_INTERVAL_YM :
						case SQLT_INTERVAL_DS :
							/* Dates: convert to string */
							hyperscript_datatype[col] = TYPE_DATETIME ;
							break ;
						case SQLT_BIN :
							/* Binary */
							hyperscript_datatype[col] = TYPE_STRING ;
							break ;
							default :
							hyperscript_datatype[col] = TYPE_STRING ;
							break ;
						}
					}
#ifndef AS_ORACLE_DO_PREPARE2

					/* Use OCIHandleAlloc/OCIStmtPrepare/OCIHandleFree */

                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
					"sql : Freeing first stmt handle");
					if ( stmthp )
					OCIHandleFree( (dvoid *)stmthp, (ub4) OCI_HTYPE_STMT);

                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
					"sql : Creating second stmt handle.");
					rc = OCIHandleAlloc(	
					(dvoid *) dbproc->envhp, 
					(dvoid **) &stmthp, 
					OCI_HTYPE_STMT,
					(size_t) 0,
					(dvoid **) 0);

					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
					"sql : Preparing second stmt.");

					rc = OCIStmtPrepare(
					stmthp, 
					dbproc->errhp, 
					sql_stmt,
					(ub4) strlen(sql_stmt),
					(ub4) OCI_NTV_SYNTAX, 
					(ub4) OCI_DEFAULT);
					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

#else
					/* Use OCIStmtPrepare2/OCIStmtRelease */
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
					"sql : First stmt released, key = %s",key );
					rc = OCIStmtRelease ( 
					stmthp,  
					dbproc->errhp,
					key,  /* Release into cache the stmt tagged as key */
					(ub4) keylen,
					(ub4) OCI_DEFAULT );  
					lHyp_sql_checkErr (	dbproc->errhp, rc ) ;

                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
					"sql : Preparing second stmt");
					if ( rc == OCI_SUCCESS ) 
					rc = OCIStmtPrepare2 ( 
					dbproc->svchp,
					&stmthp,
					dbproc->errhp,
					NULL,	 /* Second stmt, sql_stmt is NULL */
					(ub4) 0,  
					key,	 /* But fetch first stmt from cache */
					(ub4) keylen,
					(ub4) OCI_NTV_SYNTAX, 
					(ub4) OCI_PREP2_CACHE_SEARCHONLY );  
					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
					if ( stmthp == NULL ) break ;
#endif
					if ( rc == OCI_SUCCESS ) {

						for ( i=0; i<numCols; i++ ) {

							/* Associate the buffer with a column definition.
							* Convert everything to CHAR
							*/
							if ( colTypes[i] == SQLT_CLOB || colTypes[i] == SQLT_BLOB ) {

								rc = OCIDefineByPos(
								stmthp, 
								&defnp[i], 
								dbproc->errhp,
								i+1, 
								(dvoid *) &pLobLocator[i],
								(sb4) 0, 
								(ub2) colTypes[i],
								(dvoid *) 0 /*&indicator[i]*/, 
								(ub2 *) 0 /*&dataLen[i]*/,
								(ub2 *)0, 
								OCI_DEFAULT);
							}
							else {
								rc = OCIDefineByPos(
								stmthp, 
								&defnp[i], 
								dbproc->errhp,
								i+1, 
								(dvoid *) dataBuffer[i],
								(sb4) dataBufferLen[i], 
								(ub2) SQLT_CHR, /*colTypes[i],*/
								(dvoid *) &indicator[i], 
								(ub2 *)&dataLen[i],
								(ub2 *)0, 
								OCI_DEFAULT);
							}

							lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
						}
					}
					/*  Process ORACLE rows  */
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
					"sql:  Executing second stmt.");
					rc = OCIStmtExecute(
					dbproc->svchp, 
					stmthp,
					dbproc->errhp,
					(ub4) 0, 
					(ub4) 0,
					(CONST OCISnapshot *) NULL, 
					(OCISnapshot *) NULL, 
					OCI_DEFAULT) ;
					/*OCI_STMT_SCROLLABLE_READONLY) ;*/

					lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

					rows = 0 ;

					/*orientation = OCI_FETCH_NEXT ;*/
					orientation = OCI_DEFAULT ;
					
					while ( (results = OCIStmtFetch2( stmthp, 
					                                  dbproc->errhp, 
					                                  (ub4) 1,
					                                  orientation,
					                                  (sb4) 0,
					                                  OCI_DEFAULT)) == OCI_SUCCESS) {

						rows++ ;

						if ( pRowStr ) {
							pRow = gHyp_data_new ( pRowStr ) ;
							gHyp_data_append ( pTable, pRow ) ;
						} 

						for ( col=0 ; col<numCols; col++ ) {

							pData = gHyp_data_new ( NULL ) ;

							pBytes = (sBYTE*) dataBuffer[col] ;
							n = dataLen[col] ;

							switch ( colTypes[col] ) {

							case SQLT_CHR :

								/* VARCHAR2 */
								if ( n == 0 ) 
								gHyp_data_setStr_n (pData, "NULL", 4 ) ;

								else if ( colLens[col] <= INTERNAL_VALUE_SIZE )
								gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;

								else {
									gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
									gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
								}
								/*gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;*/
								break ;

							case SQLT_INT :

								/* Integer */
								gHyp_data_newConstant_scanf ( pData, TYPE_LONG, (char*)pBytes, n ) ;
								break ;

							case SQLT_FLT:

								/* Float */
								gHyp_data_newConstant_scanf ( pData, TYPE_FLOAT, (char*)pBytes, n ) ;
								break ;

							case SQLT_NUM :
								/* Numeric.  Must convert */
								if ( isFloat[col] ) {
									gHyp_data_newConstant_scanf ( pData, TYPE_DOUBLE, (char*)pBytes, n ) ;
								}
								else {
									gHyp_data_newConstant_scanf ( pData, TYPE_LONG, (char*)pBytes, n ) ;
								}
								break ;

							case SQLT_DAT :
							case SQLT_DATE :
							case SQLT_STR :
							case SQLT_TIMESTAMP :
							case SQLT_TIMESTAMP_TZ :
							case SQLT_TIMESTAMP_LTZ :
							case SQLT_INTERVAL_YM :
							case SQLT_INTERVAL_DS :
								/* Dates: convert to string */
								gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
								break ;

							case SQLT_BIN :

								/* Binary */
								gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
								gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
								break ;

							case SQLT_BLOB :
							case SQLT_CLOB :

								gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;

								OCILobGetLength( dbproc->svchp, dbproc->errhp, pLobLocator[col], &amount);
								/*amount =  MAX_SQL_BUFFER_SIZE ; 4294967295 = 4 gigabytes minus 1 */
								offset = 1 ;
								rc = OCILobRead(
								dbproc->svchp, 
								dbproc->errhp, 
								pLobLocator[col],
								(ub4*) &amount,
								offset, 
								(dvoid *) &dataBuffer[col],
								(ub4) dataBufferLen[col], 
								(dvoid *) pData, 
								lHyp_sql_readLob,
								(ub2) 0, 
								(ub1) SQLCS_IMPLICIT ) ;
								lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
								
								break ;

								default :
								
								if ( n == 0 ) 
								gHyp_data_setStr_n (pData, "NULL", 4 ) ;
								else if ( colLens[col] <= INTERNAL_VALUE_SIZE )
								gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
								else {
									gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
									gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
								}
								break ;
							}
							
							if ( pColStr != NULL ) {

								/* Append the column element to the row element */
								pCol = gHyp_data_new ( NULL ) ;
								gHyp_data_setVariable ( pCol, pColStr, TYPE_LIST ) ;
								gHyp_data_append( pCol, pData ) ;
								if ( hyperscript_datatype[col] != TYPE_STRING &&
										hyperscript_datatype[col] != TYPE_DATETIME ) {
									pLvalue = gHyp_data_new ( NULL ) ;
									gHyp_data_setReference ( pLvalue, pColStr, pCol ) ;
									pResult = gHyp_type_assign ( 
									pAI,
									gHyp_instance_frame ( pAI ),
									pLvalue,
									NULL,
									hyperscript_datatype[col],
									FALSE,	
									FALSE  ) ;
									gHyp_data_delete ( pResult ) ;
									gHyp_data_delete ( pLvalue ) ;
								}
								gHyp_data_append ( pRow, pCol ) ;

							}
							else if ( pRowStr != NULL ) {

								/* Append the column element to the row element */
								pVariable = gHyp_data_new ( NULL ) ;
								gHyp_data_setVariable ( pVariable, colNames[col], TYPE_LIST ) ;
								gHyp_data_append( pVariable, pData ) ;
								if ( hyperscript_datatype[col] != TYPE_STRING &&
										hyperscript_datatype[col] != TYPE_DATETIME ) {
									pLvalue = gHyp_data_new ( NULL ) ;
									gHyp_data_setReference ( pLvalue, colNames[col], pVariable ) ;
									pResult = gHyp_type_assign ( 
									pAI,
									gHyp_instance_frame ( pAI ),
									pLvalue,
									NULL,
									hyperscript_datatype[col], /* Into this datatype */
									FALSE,	
									FALSE  ) ;
									gHyp_data_delete ( pResult ) ;
									gHyp_data_delete ( pLvalue ) ;
								}
								gHyp_data_append ( pRow, pVariable ) ;

							}
							else {
								/* pVariable is already created */
								pVariable = variables[col] ;
								gHyp_data_append( pVariable, pData ) ;
							}
						}
					}
#endif
					/* Post processing, results were found */
					if ( pColStr == NULL && pRowStr == NULL ) {

						for ( i=0;i<numCols;i++) {
							
							if ( hyperscript_datatype[i] != TYPE_STRING &&
									hyperscript_datatype[i] != TYPE_DATETIME ) {

								pVariable = variables[i] ;
								pLvalue = gHyp_data_new ( NULL ) ;
								gHyp_data_setReference ( pLvalue, colNames[i], pVariable ) ;	
								pResult = gHyp_type_assign ( 
								pAI,
								gHyp_instance_frame ( pAI ),
								pLvalue,
								NULL,	
								hyperscript_datatype[i], /* Into this datatype */
								FALSE,	
								FALSE  ) ;
								gHyp_data_delete ( pResult ) ;
								gHyp_data_delete ( pLvalue ) ;
							}
						}
					}

					pAttr = gHyp_frame_createVariable ( pAI, pFrame, "_sqlattr_" ) ;
					gHyp_data_deleteValues ( pAttr ) ;
					for ( i=0; i<numCols; i++ ) {

						pColAttr = gHyp_data_new ( colNames[i] ) ;
						gHyp_data_append ( pAttr, pColAttr ) ;

						pData = gHyp_data_new ( "type" ) ;
						pAttrStr = gHyp_fileio_dataTypeStr ( hyperscript_datatype[i] ) ;
						gHyp_data_newVectorSrc ( pData, TYPE_ATTR, strlen (pAttrStr), FALSE, pAttrStr ) ;
						
						gHyp_data_append ( pColAttr, pData ) ;

						pData = gHyp_data_new ( "width" ) ;
						n = sprintf ( value, "%d", colLens[i] ) ; ;
						gHyp_data_newVectorSrc ( pData, TYPE_ATTR, n, FALSE, value ) ;
						
						gHyp_data_append ( pColAttr, pData ) ;\
						
					}

#ifdef AS_PGSQL
					if ( results ) PQclear ( results ) ;
					results = NULL ;
#elif AS_MYSQL
					if ( results ) mysql_free_result ( results ) ;
					results = NULL ;
#elif AS_ORACLE

					if ( results != OCI_NO_DATA ) 
					lHyp_sql_checkErr ( dbproc->errhp, results ) ;

					for ( i=0; i<numCols; i++ ) {
						ReleaseMemory ( dataBuffer[i] ) ;
						if ( colTypes[i] == SQLT_CLOB || colTypes[i] == SQLT_BLOB ) {
							OCIDescriptorFree(pLobLocator[i], (ub4) OCI_DTYPE_LOB);
						}
					}

					if ( stmthp ) {
#ifndef AS_ORACLE_DO_PREPARE2
						
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
						"sql : Releasing second stmt handle.");
						OCIHandleFree( (dvoid *)stmthp, (ub4) OCI_HTYPE_STMT);

#else
						/* Use OCIStmtPrepare2/OCIStmtRelease */
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
						"sql : Releasing second stmt.");
						rc = OCIStmtRelease ( 
						stmthp,  
						dbproc->errhp,
						key,
						(ub4) keylen,
						(ub4) OCI_STRLS_CACHE_DELETE /*OCI_STMTCACHE_DELETE*/ ); 
						lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
#endif
					}
#endif
					for ( i=0; i<numCols; i++ )
					ReleaseMemory ( colNames[i] ) ;

				}
				else {

					gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to get results");
					status = FALSE ;
					break;

				} /* End if ( results ) */

			} /* end while ( get (results ) ) */

		} /* if ( query != NULL ) */ 
		else {

			/* Query MAY have failed */

#ifdef AS_MYSQL

			/* Definitely */
			msgLen = sprintf( msg, "Failed SQL query, reason is '%s'.",mysql_error(dbproc) );
			gHyp_data_setText ( pStatus, msg, msgLen ) ; 
			msg[msgLen] = '\0' ;
			gHyp_instance_warning ( pAI, STATUS_SQL, msg ) ;
			status = FALSE ;

#elif AS_PGSQL

			/* Test further */
			if (  (PQresultStatus(results) != PGRES_EMPTY_QUERY) &&
					(PQresultStatus(results) != PGRES_COMMAND_OK) ) {
				msgLen = sprintf( msg, "Failed SQL query, reason is '%s'.",PQresStatus(PQresultStatus(results)) );
				gHyp_data_setText ( pStatus, msg, msgLen ) ; 
				msg[msgLen] = '\0' ;
				gHyp_instance_warning ( pAI, STATUS_SQL, msg ) ;
				status = FALSE ;

			}

#elif AS_ORACLE

			if ( results != OCI_SUCCESS && results != OCI_NO_DATA ) {

				msgLen = sprintf( msg, "Failed SQL query, reason is '%s'.",errbuf );
				gHyp_data_setText ( pStatus, msg, msgLen ) ; 
				msg[msgLen] = '\0' ;
				gHyp_instance_warning ( pAI, STATUS_SQL, msg ) ;
				status = FALSE ;

			}

#else

			/* SQLSERVER */
			if ( results != SUCCEED ) {
				msgLen = sprintf( msg, "Failed SQL query, reason is '%s'.",errbuf );
				gHyp_data_setText ( pStatus, msg, msgLen ) ; 
				msg[msgLen] = '\0' ;
				gHyp_instance_warning ( pAI, STATUS_SQL, msg ) ;
				status = FALSE ;
			}

#endif

		}

#ifdef AS_ORACLE
		if ( stmthp && !isStmtBound ) { 

#ifndef AS_ORACLE_DO_PREPARE2

                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			"sql : No Data: Releasing first stmt handle.");
			OCIHandleFree( (dvoid *)stmthp, (ub4) OCI_HTYPE_STMT);
			
#else
                if ( guDebugFlags & DEBUG_SQL) gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SQL,
			"sql : No Data: first stmt released");
			rc = OCIStmtRelease ( 
			stmthp,  
			dbproc->errhp,
			NULL,
			(ub4) 0,
			(ub4) OCI_STRLS_CACHE_DELETE ); 
			lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
#endif
		}
#endif

#endif    /* from AS_SQL way up above  */

		pResult = gHyp_data_new ( NULL ) ;
		if ( status )
		gHyp_data_setInt ( pResult, rows ) ;
		else
		gHyp_data_setInt ( pResult, -1) ;

		gHyp_stack_push ( pStack, pResult ) ;
		return ;
	}
}

void gHyp_sql_open ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
	/* Description:
	*
	*    PARSE or EXECUTE the built-in function: sql_open ()
	*    Returns TRUE or FALSE
	*
	* Arguments:
	*
	*    pAI                                                     [R]
	*    - pointer to instance object
	*
	*    pCode                                                   [R]
	*    - pointer to code object
	*
	* Return value:
	*
	*    none
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
		*pData,
		*pResult ;

		int
		n,
		argCount = gHyp_parse_argCount ( pParse ) ;

		char
		value[VALUE_SIZE+1],
		sql_username[MAX_SQL_USERNAME_LENGTH+1],
		sql_password[MAX_SQL_PASSWORD_LENGTH+1],
		sql_server[MAX_SQL_SERVER_LENGTH+1],
		sql_database[MAX_SQL_DATABASE_LENGTH+1] ;

		sLOGICAL
		isSecure=FALSE ;

#ifdef AS_SQLSERVER

		DBPROCESS   *dbproc;
		LOGINREC    *loginrec;

#elif AS_MYSQL

		MYSQL       *dbproc ;

#elif AS_PGSQL

		/* "dbname="    is 7 chars                          */
		/* " user="     is 6 chars including leading space  */
		/* " password=" is 10 chars including leading space */
		/* " host="     is 6 chars including leading space  */
		/* +1 for null at end = 30 chars                    */
		char        HSPGconninfo[MAX_SQL_USERNAME_LENGTH +
		MAX_SQL_PASSWORD_LENGTH +
		MAX_SQL_SERVER_LENGTH +
		MAX_SQL_DATABASE_LENGTH + 30];
		PGconn      *dbproc;

#elif AS_ORACLE

		sORACLE
		*dbproc ;

		sword	rc ;

		char
		connectString[VALUE_SIZE+1] ;

#endif

		/* Assume success */
		gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

		if ( argCount !=4 && argCount != 5 )
		gHyp_instance_error (
		pAI, STATUS_ARGUMENT,
		"Invalid arguments. Usage: sql_open ( user, password, server ,database [,isSecure])");

		if ( argCount == 5 ) {
			pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
			isSecure = gHyp_data_getBool ( pData,
			gHyp_data_getSubScript ( pData ),
			TRUE )  ;
		}


		/* Get name for database */
		n = gHyp_data_getStr (      gHyp_stack_popRvalue ( pStack, pAI ),
		value,
		VALUE_SIZE,
		0,
		TRUE ) ;
		if ( n > MAX_SQL_DATABASE_LENGTH )
		gHyp_instance_error (pAI, STATUS_SQL,
		"Database name must be %d characters or less",
		MAX_SQL_DATABASE_LENGTH ) ;

		strcpy ( sql_database, value ) ;


		/* Get name for server */
		n = gHyp_data_getStr (      gHyp_stack_popRvalue ( pStack, pAI ),
		value,
		VALUE_SIZE,
		0,
		TRUE ) ;
		if ( n > MAX_SQL_SERVER_LENGTH )
		gHyp_instance_error (pAI, STATUS_SQL,
		"Server name must be %d characters or less",
		MAX_SQL_SERVER_LENGTH ) ;

		strcpy ( sql_server, value ) ;

		n = gHyp_data_getStr ( gHyp_stack_popRvalue ( pStack, pAI ),
		value,
		VALUE_SIZE,
		0,
		TRUE ) ;
		if ( n > MAX_SQL_PASSWORD_LENGTH )
		gHyp_instance_error ( pAI, STATUS_SQL,
		"Password must be %d characters or less",
		MAX_SQL_PASSWORD_LENGTH ) ;
		strcpy ( sql_password, value ) ;

		n = gHyp_data_getStr ( gHyp_stack_popRvalue ( pStack, pAI ),
		value,
		VALUE_SIZE,
		0,
		TRUE ) ;
		if ( n > MAX_SQL_USERNAME_LENGTH )
		gHyp_instance_error ( pAI, STATUS_SQL,
		"Username must be %d characters or less",
		MAX_SQL_USERNAME_LENGTH ) ;
		strcpy ( sql_username, value ) ;

		pResult = gHyp_data_new ( NULL ) ;

#ifdef AS_SQLSERVER
		/* Initialize */
		dbinit() ;

		/* Install user-supplied error- and message-handling functions.*/
		dberrhandle (lHyp_sql_errHandler);
		dbmsghandle (lHyp_sql_msgHandler);

		loginrec = dblogin();
		DBSETLUSER (loginrec, sql_username );
		DBSETLPWD (loginrec, sql_password);
		DBSETLAPP (loginrec, "hyperscript");
		DBSETLVERSION (loginrec,DBVER60);
		/*    DBSETLVERSION (loginrec,DBVERSION70);*/

		if ( isSecure ) {
#ifdef AS_WINDOWS
			DBSETLSECURE (loginrec);
#endif
		}

		/* Now attempt to create and initialize a DBPROCESS structure */

		if ((dbproc = dbopen (loginrec, sql_server)) == NULL) {

			gHyp_instance_warning ( pAI, STATUS_SQL, "SQL dbopen failed");
			gHyp_data_setHandle ( pResult, NULL ) ;      

		}
		else {

			gHyp_util_logInfo ( "Connected to SQLSERVER as user %s", sql_username ) ;

			if ( dbuse ( dbproc, sql_database ) == SUCCEED ) {

				gHyp_data_setHandle ( pResult, (void*) dbproc ) ;

#ifdef AS_UNIX
				if ( dbsetopt( dbproc, DBTEXTSIZE, "64512", 0 ) != SUCCEED ) { 
					gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to dbsetopt DBTEXTSIZE");	 
				}
				else if ( dbsetopt( dbproc, DBTEXTLIMIT, "64512", 0 ) != SUCCEED ) { 
					gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to dbsetopt DBTEXTLIMIT");	 
				}
#else
				if ( dbsetopt( dbproc, DBTEXTSIZE, "64512" ) != SUCCEED ) { 
					gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to dbsetopt DBTEXTSIZE");	 
				}
				else if ( dbsetopt( dbproc, DBTEXTLIMIT, "64512" ) != SUCCEED ) { 
					gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to dbsetopt DBTEXTLIMIT");	 
				}
#endif
			}
			else {
				gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to dbuse()");
				gHyp_data_setHandle ( pResult, NULL ) ;
			}
		}

#elif AS_MYSQL

		if ( (dbproc = mysql_init ( NULL ) ) == NULL) {
			gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to mysql_init()");
			gHyp_data_setHandle ( pResult, NULL ) ;
		}
		else {
			if ( (dbproc = mysql_real_connect ( dbproc,
							sql_server,
							sql_username,
							sql_password,
							sql_database,
							0,
							NULL,
							0 ) ) == NULL) {
				gHyp_instance_warning ( pAI, STATUS_SQL, "SQL mysql_real_connect failed");
				gHyp_data_setHandle ( pResult, NULL ) ;
			}
			else {
				gHyp_util_logInfo ( "Connected to MYSQL as user %s", sql_username ) ;
				gHyp_data_setHandle ( pResult, (void*) dbproc ) ;
			}
		}

#elif AS_PGSQL

		/* do PG connect   */
		strcpy ( HSPGconninfo, "dbname=" ) ;
		strcat ( HSPGconninfo, sql_database ) ;
		strcat ( HSPGconninfo, " user=" ) ;
		strcat ( HSPGconninfo, sql_username ) ;
		strcat ( HSPGconninfo, " password=" ) ;
		strcat ( HSPGconninfo, sql_password ) ;
		strcat ( HSPGconninfo, " host=" ) ;
		strcat ( HSPGconninfo, sql_server ) ;
		dbproc = PQconnectdb( (const char*) HSPGconninfo ) ;
		if (PQstatus(dbproc) != CONNECTION_OK) {
			gHyp_instance_warning ( pAI, STATUS_SQL, "SQL PQconnectdb failed");
			gHyp_data_setHandle ( pResult, NULL ) ;
		}
		else {
			gHyp_util_logInfo ( "Connected to PostgreSQL as user %s", sql_username ) ;
			gHyp_data_setHandle ( pResult, (void*) dbproc ) ;
		}

#elif AS_ORACLE

		/* Allocate memory for our special ORACLE structure */
		dbproc = (sORACLE*) AllocMemory ( sizeof ( sORACLE ) ) ;

		/* Get ready to use ORACLE */
		(void) OCIInitialize(	(ub4) OCI_DEFAULT, (dvoid *)0,
		(dvoid * (*)(dvoid *, size_t)) 0,
		(dvoid * (*)(dvoid *, dvoid *, size_t))0,
		(void (*)(dvoid *, dvoid *)) 0 );

		/* Initialize the environment */
		rc = OCIEnvInit(	(OCIEnv **) &dbproc->envhp, 
		OCI_DEFAULT,
		(size_t) 0,
		(dvoid **) 0 );

		if ( rc == OCI_SUCCESS )
		/* Create handle for the errhp */
		rc = OCIHandleAlloc(	(dvoid *) dbproc->envhp, 
		(dvoid **) &dbproc->errhp, 
		OCI_HTYPE_ERROR,
		(size_t) 0,
		(dvoid **) 0);

		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		if ( rc == OCI_SUCCESS )
		/* Create handle for the srvhp */
		rc = OCIHandleAlloc(	(dvoid *) dbproc->envhp,
		(dvoid **) &dbproc->srvhp,
		OCI_HTYPE_SERVER,
		(size_t) 0,
		(dvoid **) 0);

		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		if ( rc == OCI_SUCCESS ) {
			/* Attach to the server */

			if ( strlen ( sql_database ) == 0 )
			strcpy ( connectString, sql_server ) ;
			else
			sprintf ( connectString, "//%s/%s", sql_server, sql_database ) ;

			rc = OCIServerAttach(	dbproc->srvhp, 
			dbproc->errhp,
			(text *)connectString,
			strlen(connectString),
			OCI_DEFAULT );
		}
		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;


		if ( rc == OCI_SUCCESS )
		/* Create handle for the svchp */
		rc = OCIHandleAlloc(	(dvoid *) dbproc->envhp,
		(dvoid **) &dbproc->svchp,
		OCI_HTYPE_SVCCTX,
		(size_t) 0,
		(dvoid **) 0);

		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;


		if ( rc == OCI_SUCCESS )
		/* Set attribute server context in the service context */
		rc = OCIAttrSet(		(dvoid *) dbproc->svchp,
		OCI_HTYPE_SVCCTX, 
		(dvoid *)dbproc->srvhp,
		(ub4) 0,
		OCI_ATTR_SERVER,
		(OCIError *) dbproc->errhp);
		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;


		/* Get handle for the authp */
		if ( rc == OCI_SUCCESS )
		rc = OCIHandleAlloc(	(dvoid *) dbproc->envhp,
		(dvoid **)&dbproc->authp,
		(ub4) OCI_HTYPE_SESSION, 
		(size_t) 0, 
		(dvoid **) 0);
		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		/* Attach username and password */
		if ( rc == OCI_SUCCESS )
		rc = OCIAttrSet(		(dvoid *) dbproc->authp, 
		(ub4) OCI_HTYPE_SESSION,
		(dvoid *) sql_username, 
		(ub4) strlen((char *)sql_username),
		(ub4) OCI_ATTR_USERNAME, 
		dbproc->errhp);
		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		if ( rc == OCI_SUCCESS )
		rc = OCIAttrSet(		(dvoid *) dbproc->authp, 
		(ub4) OCI_HTYPE_SESSION,
		(dvoid *) sql_password, 
		(ub4) strlen((char *)sql_password),
		(ub4) OCI_ATTR_PASSWORD, 
		dbproc->errhp);
		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		if ( rc == OCI_SUCCESS )
		rc = OCISessionBegin  (	dbproc->svchp,  
		dbproc->errhp, 
		dbproc->authp, 
		OCI_CRED_RDBMS,
		(ub4) /*OCI_DEFAULT*/ OCI_STMT_CACHE );
		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		/* set the user session attribute in the service context handle*/
		if ( rc == OCI_SUCCESS )
		rc = OCIAttrSet(		(dvoid *) dbproc->svchp, 
		(ub4) OCI_HTYPE_SVCCTX,
		(dvoid *) dbproc->authp,
		(ub4) 0,
		(ub4) OCI_ATTR_SESSION,
		dbproc->errhp);
		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		/* Now register the Application Callback for this User Session *

	if ( rc == OCI_SUCCESS )
		rc = lHyp_sql_RegisterApplicationFailoverCallback(dbproc->svchp, dbproc->errhp );

	lHyp_sql_checkErr ( dbproc->errhp, rc ) ; 
	*/

		if ( rc != OCI_SUCCESS ) {
			gHyp_instance_warning ( pAI, STATUS_SQL, "SQL connect to ORACLE failed");
			gHyp_data_setHandle ( pResult, NULL ) ;
		}
		else {
			gHyp_util_logInfo ( "Connected to ORACLE as user %s", sql_username ) ;
			/* Save the latest pointer for use with failover */
			strcpy ( dbproc->username, sql_username ) ;
			strcpy ( dbproc->password, sql_password ) ;
			strcpy ( dbproc->database, sql_database ) ;
			gsORACLE = dbproc ;

			gHyp_data_setHandle ( pResult, (void*) dbproc ) ;
		}


		/****
	AN OLDER ALTERNATE WAY?
	****
	rc = OCIInitialize((ub4) OCI_DEFAULT, (dvoid *)0,
					(dvoid * (*)(dvoid *, size_t)) 0,
					(dvoid * (*)(dvoid *, dvoid *, size_t))0,
					(void (*)(dvoid *, dvoid *)) 0 );

	rc = OCIEnvInit( (OCIEnv **) &p_env, OCI_DEFAULT, (size_t) 0, (dvoid **) 0 );

	rc = OCIHandleAlloc( (dvoid *) p_env, (dvoid **) &p_err, OCI_HTYPE_ERROR,
					(size_t) 0, (dvoid **) 0);
	rc = OCIHandleAlloc( (dvoid *) p_env, (dvoid **) &p_svc, OCI_HTYPE_SVCCTX,
					(size_t) 0, (dvoid **) 0);

	rc = OCILogon(p_env, p_err, &p_svc, "scott", 5, "tiger", 5, "d458_nat", 8);
	if (rc != 0) {
			OCIErrorGet((dvoid *)p_err, (ub4) 1, (text *) NULL, &errcode, errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
			printf("Error - %.*s\n", 512, errbuf);
			exit(8);
	}
	****/


#endif

		gHyp_stack_push ( pStack, pResult ) ;
		return ;
	}
}

void gHyp_sql_close ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
	/* Description:
	*
	*    PARSE or EXECUTE the built-in function: sql_close ( handle )
	*    Returns TRUE or FALSE
	*
	* Arguments:
	*
	*    pAI                                                     [R]
	*    - pointer to instance object
	*
	*    pCode                                                   [R]
	*    - pointer to code object
	*
	* Return value:
	*
	*    none
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
		*pData ;

		int
		argCount = gHyp_parse_argCount ( pParse ) ;

#ifdef AS_SQL
#ifdef AS_SQLSERVER
		DBPROCESS   *dbproc;
#elif AS_MYSQL
		MYSQL *dbproc ;
#elif AS_PGSQL
		PGconn *dbproc ;
#elif AS_ORACLE
		sORACLE *dbproc ;
#endif
#else
		void *dbproc ;
#endif

		/* Assume success */
		gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

		if ( argCount != 1 )
		gHyp_instance_error ( pAI, STATUS_ARGUMENT,
		"Invalid arguments. Usage: sql_close ( handle )" ) ;

		/* Get the handle name for database */
		pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

		dbproc =

#ifdef AS_SQLSERVER
		(DBPROCESS*)
#elif AS_MYSQL
		(MYSQL*)
#elif AS_PGSQL
		(PGconn*)
#elif AS_ORACLE
		(sORACLE*)
#endif
		gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE);

#ifdef AS_SQLSERVER
		dbclose ( dbproc ) ;

#elif AS_MYSQL
		mysql_close ( dbproc ) ;

#elif AS_PGSQL

		PQfinish( dbproc );

#elif AS_ORACLE

		/* End a session */
		lHyp_sql_checkErr(	dbproc->errhp, 
		OCISessionEnd((OCISvcCtx *)dbproc->svchp, 
		(OCIError *)dbproc->errhp,
		(OCISession *)dbproc->authp, 
		(ub4) OCI_DEFAULT));

		/* Detach from the server */
		lHyp_sql_checkErr(	dbproc->errhp, 
		OCIServerDetach((OCIServer *)dbproc->srvhp,
		(OCIError *)dbproc->errhp,
		(ub4)OCI_DEFAULT));

		/* Free the handles */
		if (dbproc->authp) {

			OCIHandleFree(	(dvoid *)dbproc->authp, 
			(ub4) OCI_HTYPE_SESSION);
		}

		if ( dbproc->svchp ) {

			OCIHandleFree((	dvoid *)dbproc->svchp, 
			(ub4) OCI_HTYPE_SVCCTX);
		}

		if ( dbproc->srvhp ) {

			OCIHandleFree(	(dvoid *)dbproc->srvhp, 
			(ub4) OCI_HTYPE_SERVER);
		}

		if ( dbproc->errhp ) {

			OCIHandleFree((	dvoid *)dbproc->errhp, 
			(ub4) OCI_HTYPE_ERROR);
		}

		if ( dbproc->envhp ) {

			OCIHandleFree(	(dvoid *)dbproc->envhp, 
			(ub4) OCI_HTYPE_ENV);
		}

		ReleaseMemory ( dbproc ) ;

#endif
		gHyp_instance_pushSTATUS ( pAI, pStack ) ;
		return ;
	}
}


void gHyp_sql_toexternal(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
	*
	*	PARSE or EXECUTE the built-in function: sql_toexternal()
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
	*
	*/
	sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
	sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

	if ( isPARSE )

	gHyp_parse_operand ( pParse, pCode, pAI ) ;

	else {

		sStack 	
		*pStack = gHyp_frame_stack ( pFrame ) ;
		
		sData
		*pData,
		*pValue,
		*pValue2,
		*pResult ;
		
		int
		n,
		context,
		ss,
		argCount = gHyp_parse_argCount ( pParse ) ;
		
		char
		strVal[VALUE_SIZE+1],
		strVal2[VALUE_SIZE*4+1],
		*pStr ;
		
		sLOGICAL
		noNULL=FALSE,
		doTrim=TRUE,
		isEmpty,
		isVector ;

		/* Assume success */	
		gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

		if ( argCount > 2 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
		"Invalid arguments. Usage: sql_toexternal ( string [, noNULL] )" ) ;

		if ( argCount == 2 ) {
			pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
			noNULL  = gHyp_data_getBool ( pData,
			gHyp_data_getSubScript ( pData  ),
			TRUE ) ;
			doTrim = !noNULL ;
		}

		pData = gHyp_stack_popRdata ( pStack, pAI ) ;
		pResult = gHyp_data_new ( NULL ) ;
		gHyp_data_setVariable ( pResult, "_toexternal_", TYPE_STRING ) ;
		isEmpty = TRUE ;
		isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
		pValue = NULL ;
		ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
		while ( (pValue = gHyp_data_nextValue ( pData, 
		pValue, 
		&context,
		ss ) ) ) {
			if ( isVector ) {
				
				pValue2 = gHyp_data_new ( NULL ) ;
				gHyp_data_newConstant ( pValue2, 
				gHyp_data_dataType ( pValue ), 
				pValue, 
				context ) ;
				isEmpty = FALSE ;
				gHyp_data_append ( pResult, pValue2 ) ;
			}
			else {
				n = gHyp_data_getStr ( pValue, 
				strVal, 
				VALUE_SIZE, 
				context, 
				isVector ) ;
				pStr = strVal ;
				n = gHyp_util_trimWhiteSpace ( strVal ) ;
				if ( noNULL && n == 0 ) n = sprintf ( strVal, "%s", " " ) ;
				if ( n > 0 ) {
					n = gHyp_util_unparseString ( strVal2, pStr, n, VALUE_SIZE*4, FALSE, FALSE, TRUE,"" ) ;
					pValue2 = gHyp_data_new ( NULL ) ;
					gHyp_data_setStr_n ( pValue2, strVal2, n ) ;
					isEmpty = FALSE ;
					gHyp_data_append ( pResult, pValue2 ) ;
				}
			}
		}
		if ( context== -2 && ss != -1 ) {
			gHyp_data_delete ( pResult ) ;
			gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			"Subscript '%d' is out of bounds in sql_toexternal()",
			ss);
		}
		if ( isEmpty ) {
			strcpy ( strVal, "NULL" ) ;
			pValue = gHyp_data_new ( NULL ) ;
			gHyp_data_setStr ( pValue, strVal ) ;
			gHyp_data_append ( pResult, pValue ) ;
		}
		else {
			strcpy ( strVal, "'" ) ;
			pValue = gHyp_data_new ( NULL ) ;
			gHyp_data_setStr ( pValue, strVal ) ;
			gHyp_data_insert ( pResult, pValue ) ;
			pValue = gHyp_data_new ( NULL ) ;
			gHyp_data_setStr ( pValue, strVal ) ;
			gHyp_data_append ( pResult, pValue ) ;
		}

		gHyp_stack_push ( pStack, pResult ) ;
	}
}

void gHyp_sql_datetime ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
	/* Description:
	*
	*	PARSE or EXECUTE the built-in function: sql_datetime()
	*	Returns YYYYMMDD:hhmmss
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
	*/
	sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
	sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

	if ( isPARSE )

	gHyp_parse_operand ( pParse, pCode, pAI ) ;

	else {

		sStack 	
		*pStack = gHyp_frame_stack ( pFrame ) ;

		sData
		*pData,
		*pValue,
		*pValue2,
		*pResult ;

		struct tm	*pstm ;	

		char
		timeStamp[SQL_DATETIME_SIZE+1] ;

		time_t
		ts ;

		sLOGICAL
		isVector ;

		int
		ss,
		context,
		argCount = gHyp_parse_argCount ( pParse ) ;
		
		/* Assume success */	
		gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

		if ( argCount > 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
		"Invalid arguments. Usage: sql_datetime ( [utc] )" ) ;

		if ( argCount == 0 ) {

			ts = gsCurTime = time(NULL) ;
			pstm = localtime ( &ts ) ;
			if ( !pstm || pstm->tm_year > 138 ) {
				strcpy ( timeStamp, " " ) ;
			}
			else {
				sprintf (timeStamp, 
				"%04d-%02d-%02d %02d:%02d:%02d", 
				pstm->tm_year+1900, pstm->tm_mon+1, pstm->tm_mday,
				pstm->tm_hour, pstm->tm_min, pstm->tm_sec ) ;
			}
			pResult = gHyp_data_new ( NULL ) ;
			gHyp_data_setStr ( pResult, timeStamp ) ;
			gHyp_stack_push ( pStack, pResult ) ;
		}
		else {

			pResult = gHyp_data_new ( NULL ) ;
			gHyp_data_setVariable ( pResult, "_sql_datetime_", TYPE_STRING ) ;

			pData = gHyp_stack_popRdata ( pStack, pAI ) ;
			isVector = (gHyp_data_getDataType( pData ) > TYPE_STRING ) ;
			pValue = NULL ;
			ss = gHyp_data_getSubScript ( pData ) ; 
			context = -1 ;
			while ( (pValue = gHyp_data_nextValue ( pData, 
			                                        pValue, 
			                                        &context,
			                                        ss ) ) ) {
				
				ts = gHyp_data_getRaw ( pValue, context, TRUE  ) ;
				pstm = localtime ( &ts ) ;
				if ( !pstm || pstm->tm_year > 138 ) {
					strcpy ( timeStamp, " " ) ;
				}
				else {
					sprintf ( timeStamp, 
					"%04d-%02d-%02d %02d:%02d:%02d", 
					pstm->tm_year+1900, pstm->tm_mon+1, pstm->tm_mday,
					pstm->tm_hour, pstm->tm_min, pstm->tm_sec ) ;
				}
				pValue2 = gHyp_data_new ( NULL ) ;
				gHyp_data_setStr ( pValue2, timeStamp ) ;
				gHyp_data_append ( pResult, pValue2 ) ;
			}
			if ( context== -2 && ss != -1 ) {
				gHyp_data_delete ( pResult ) ;
				gHyp_instance_error ( pAI, STATUS_BOUNDS, 
				"Subscript '%d' is out of bounds in sql_datetime()",
				ss);
			}
			gHyp_stack_push ( pStack, pResult ) ;
		}
	}
}
