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
#endif

#ifdef AS_ORACLE
#include <oci.h>

struct oracle_t {
  OCISvcCtx  *svchp;
  OCISession *authp;
  OCIServer  *srvhp;
  OCIEnv     *envhp;
  OCIError   *errhp;
}  ;

typedef struct oracle_t	sORACLE ;

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
    sprintf ( errbuf, "Operating System Error %i: %s\n", oserr, oserrstr);
  else
    sprintf (errbuf, "DB-Library Error %i: %s\n", dberr, dberrstr);

  gHyp_util_logError ( errbuf ) ;
  return (INT_CANCEL);
}

static int lHyp_sql_msgHandler (PDBPROCESS dbproc, DBINT msgno, INT msgstate,
    INT severity, LPCSTR msgtext, LPCSTR server,
    LPCSTR procedure, DBUSMALLINT line)
{
  sprintf ( errbuf, "SQL Server Message %ld: %s\n", msgno, msgtext );
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
      /*gHyp_util_debug("callback read the %d th piece\n", piece_count);*/
      gHyp_util_breakStream ( (char*) bufxp, len, (sData*) ctxp, TRUE ) ;

      break;

    default:
      gHyp_util_logError("Callback read error: unkown piece = %d.\n", piece);
      return OCI_ERROR;
  }
  return OCI_CONTINUE;
}

#endif

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
      isVector;


    int
      ss,
      context,
      valueLen,
      rows=0,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      value[VALUE_SIZE+1],
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
	isSelect ;
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
     OCIStmt    *stmthp;
     OCIParam   *mypard;

     void*	dataBuffer[MAX_SQL_ITEMS] ;

     ub4	dataBufferLen[MAX_SQL_ITEMS] ;
     ub4	amount ;
     ub4	offset ;
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

    pStatus = gHyp_frame_createVariable ( pFrame, "_sql_status_" ) ;
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
                                    VALUE_SIZE,
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

#ifdef AS_SQLSERVER

    if ( (results = dbsqlexec (dbproc)) == SUCCEED )

#elif AS_MYSQL

    if ( mysql_query ( dbproc, sql_stmt ) == 0 )

#elif AS_PGSQL

    results = PQexec( dbproc, sql_stmt );
    if ((PQresultStatus(results) == PGRES_TUPLES_OK) )

#elif AS_ORACLE

  /* Create handle for the statement */
  rc = OCIHandleAlloc(	(dvoid *) dbproc->envhp, 
			(dvoid **) &stmthp, 
			OCI_HTYPE_STMT,
			(size_t) 0,
			(dvoid **) 0);

  lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

  if ( rc == OCI_SUCCESS ) 
    rc = OCIStmtPrepare(stmthp, 
			dbproc->errhp, 
			sql_stmt,
                        (ub4) strlen(sql_stmt),
			(ub4) OCI_NTV_SYNTAX, 
			(ub4) OCI_DEFAULT);
  lHyp_sql_checkErr (	dbproc->errhp, rc ) ;

  orientation = isSelect ? OCI_DESCRIBE_ONLY : OCI_DEFAULT ;
  if ( rc == OCI_SUCCESS )
    rc = OCIStmtExecute(
			dbproc->svchp, 
			stmthp,
			dbproc->errhp,
			(ub4) 1, 
			(ub4) 0,
			(CONST OCISnapshot *) NULL, 
			(OCISnapshot *) NULL, 
			orientation ) ;

  lHyp_sql_checkErr (	dbproc->errhp, rc ) ;

  results = rc ;

  if ( !isSelect ) rc = OCI_NO_DATA ;

  if ( rc == OCI_SUCCESS ) 

#endif
      {
        /* now check the results from the SQL server */

	if ( pTableStr ) {

          /* List variables are created inside of pTable */
          pTable = gHyp_frame_createVariable ( pFrame, pTableStr ) ;
          gHyp_data_deleteValues ( pTable ) ;

        }
        else {

          /* List variables are top-level variables. */

          /* Create or retrieve the "sqlargs" variable, and clear it of any values. */
          pArgs = gHyp_frame_createVariable ( pFrame, "sqlargs" ) ;
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

	    /*gHyp_util_debug("GOT RESULTS");*/
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
                  /*gHyp_util_debug("Column name %d is %s",i,pColName);*/
                  colNames[i] = (char*) AllocMemory ( strlen ( pColName ) + 1 ) ;
		  strcpy ( colNames[i], pColName ) ;
		  gHyp_util_upperCase ( colNames[i], strlen ( colNames[i] ) ) ;

                  if ( pTable == NULL ) {

                    /* List variables are created from the column names */
                    pVariable = gHyp_frame_createVariable ( pFrame, colNames[i] ) ;
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
		  isFloat[i] = (dataType == SQLT_NUM && precision > 0) ? TRUE : FALSE ;

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
		}

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
			break ;
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
					buffer,
					MAX_BUFFER_SIZE);
                        gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                        break ;

		      case SQLDECIMAL:
		      case SQLNUMERIC:

                        n = dbconvert (	dbproc,
					colTypes[i],
					pBytes,
					n,
					SQLCHAR,
					buffer,
					MAX_BUFFER_SIZE);
                        if ( n < 0 ) gHyp_data_newConstant_scanf ( pData, TYPE_DOUBLE, (char*)buffer, n ) ;
                        break ;

                      default :

                        n = dbconvert (	dbproc,
					colTypes[i],
					pBytes,
					n,
					SQLCHAR,
					buffer,
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

                if ( stmthp )
		  OCIHandleFree( (dvoid *)stmthp, (ub4) OCI_HTYPE_STMT);

		rc = OCIHandleAlloc(	
			(dvoid *) dbproc->envhp, 
			(dvoid **) &stmthp, 
			OCI_HTYPE_STMT,
			(size_t) 0,
			(dvoid **) 0);

		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		rc = OCIStmtPrepare(
			stmthp, 
			dbproc->errhp, 
			sql_stmt,
                        (ub4) strlen(sql_stmt),
			(ub4) OCI_NTV_SYNTAX, 
			(ub4) OCI_DEFAULT);
		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

		if ( rc == OCI_SUCCESS ) {

		  for ( i=0; i<numCols; i++ ) {

		    /* Associate the buffer with a column definition.
		     * Convert everything to CHAR
		     */
		    if ( colTypes[i] == SQLT_CLOB || colTypes[i] == SQLT_BLOB ) {

		      rc = OCIDefineByPos(stmthp, 
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
		      rc = OCIDefineByPos(stmthp, 
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

                while ( (results = OCIStmtFetch2(
					stmthp, 
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

	        pAttr = gHyp_frame_createVariable ( pFrame, "_sqlattr_" ) ;
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
			  
		  gHyp_data_append ( pColAttr, pData ) ;
		
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

		if ( stmthp )
		  OCIHandleFree( (dvoid *)stmthp, (ub4) OCI_HTYPE_STMT);
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

    if ( isSecure ) {
      DBSETLSECURE (loginrec);
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


        if ( dbsetopt( dbproc, DBTEXTSIZE, "64512" ) != SUCCEED ) { 
          gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to dbsetopt DBTEXTSIZE");	 
	}
        else if ( dbsetopt( dbproc, DBTEXTLIMIT, "64512" ) != SUCCEED ) { 
          gHyp_instance_warning ( pAI, STATUS_SQL, "Failed to dbsetopt DBTEXTLIMIT");	 
	}
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
    rc = OCISessionBegin (	dbproc->svchp,  
				dbproc->errhp, 
				dbproc->authp, 
				OCI_CRED_RDBMS,
				(ub4) OCI_DEFAULT );
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


  if ( rc != OCI_SUCCESS ) {
    gHyp_instance_warning ( pAI, STATUS_SQL, "SQL connect to ORACLE failed");
    gHyp_data_setHandle ( pResult, NULL ) ;
  }
  else {
     gHyp_util_logInfo ( "Connected to ORACLE as user %s", sql_username ) ;
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
      doTrim=TRUE,
      isEmpty,
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 2 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: sql_toexternal ( string [, notrim] )" ) ;

    if ( argCount == 2 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      doTrim  = ! ( gHyp_data_getBool ( pData,
				    gHyp_data_getSubScript ( pData  ),
				    TRUE ) ) ;
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
      }
      else {
        n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
	if ( doTrim ) n = gHyp_util_trim ( strVal ) ;
        n = gHyp_util_unparseString ( strVal2, pStr, n, VALUE_SIZE*4, FALSE, FALSE, TRUE,"" ) ;
        gHyp_data_setStr_n ( pValue2, strVal2, n ) ;
	if ( n > 0 ) isEmpty = FALSE ;
      }

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in sql_toexternal()",
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
	"Invalid arguments. Usage: sql_datetime ( [ansitime] )" ) ;

    if ( argCount == 0 ) {

      ts = gsCurTime = time(NULL) ;
      pstm = localtime ( &ts ) ;
      if ( !pstm ) 
	strcpy ( timeStamp, "NULL" ) ;
      else
        sprintf (timeStamp, 
		"'%04d-%02d-%02d %02d:%02d:%02d'", 
		pstm->tm_year+1900, pstm->tm_mon+1, pstm->tm_mday,
		pstm->tm_hour, pstm->tm_min, pstm->tm_sec ) ;
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
	if ( !pstm )
  	  strcpy ( timeStamp, "NULL" ) ;
	else
          sprintf ( timeStamp, 
	  	  "'%04d-%02d-%02d %02d:%02d:%02d'", 
		  pstm->tm_year+1900, pstm->tm_mon+1, pstm->tm_mday,
		  pstm->tm_hour, pstm->tm_min, pstm->tm_sec ) ;
        pValue2 = gHyp_data_new ( NULL ) ;
        gHyp_data_setStr ( pValue2, timeStamp ) ;
        gHyp_data_append ( pResult, pValue2 ) ;
      }
      if ( context== -2 && ss != -1 ) {
        gHyp_data_delete ( pResult ) ;
        gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in sql_datetime()",
			    ss);
      }
      gHyp_stack_push ( pStack, pResult ) ;
    }
  }
}
