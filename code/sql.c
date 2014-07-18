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

static int lHyp_sql_errHandler (PDBPROCESS dbproc, INT severity,
    INT dberr, INT oserr, LPCSTR dberrstr, LPCSTR oserrstr)
{
  if (oserr != DBNOERR)
    gHyp_util_logError ("Operating System Error %i: %s\n", oserr, oserrstr);
  else
    gHyp_util_logError ("DB-Library Error %i: %s\n", dberr, dberrstr);

  return (INT_CANCEL);
}

static int lHyp_sql_msgHandler (PDBPROCESS dbproc, DBINT msgno, INT msgstate,
    INT severity, LPCSTR msgtext, LPCSTR server,
    LPCSTR procedure, DBUSMALLINT line)
{
  gHyp_util_logWarning ( "SQL Server Message %ld: %s\n", msgno, msgtext );
  return (0);
}
#endif

#ifdef AS_ORACLE

sword lHyp_sql_checkErr( OCIError *errhp, sword status)
{
  text errbuf[512];
  sb4  errcode = 0;

  switch (status) {

  case OCI_SUCCESS:
    break;

  case OCI_SUCCESS_WITH_INFO:
    gHyp_util_logWarning("OCI_SUCCESS_WITH_INFO");
    break;

  case OCI_NEED_DATA:
    gHyp_util_logError("OCI_NEED_DATA");
    break;

  case OCI_NO_DATA:
    gHyp_util_logWarning("OCI_NODATA");
    break;

  case OCI_ERROR:
    (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                        errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    gHyp_util_logError("%.*s", 512, errbuf);
    break;

  case OCI_INVALID_HANDLE:
    gHyp_util_logError("OCI_INVALID_HANDLE");
    break;

  case OCI_STILL_EXECUTING:
    gHyp_util_logWarning("OCI_STILL_EXECUTE");
    break;

  case OCI_CONTINUE:
    gHyp_util_logError("OCI_CONTINUE");
    break;

  default:
    gHyp_util_logError("Unknown error %d",status );
    break;
  }

  return status ;
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
      *pArgs=NULL,
      *pVariable=NULL,
      *variables[MAX_SQL_ITEMS] ;

    sBYTE
      *pBytes ;

    int
      i,
      colTypes[MAX_SQL_ITEMS],
      colLens[MAX_SQL_ITEMS],
      numCols,
      n ;

    char
      colName[VALUE_SIZE+1],
      *colNames[MAX_SQL_ITEMS],
      *pColName ;

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
    int msgLen ;
    char msg[MAX_OUTPUT_LENGTH+1] ;

#elif AS_PGSQL

    PGconn      *dbproc ;
    PGresult    *results ;
    char       msg[MAX_OUTPUT_LENGTH+1] ;
    int
	msgLen,
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
     ub2	dataLen[MAX_SQL_ITEMS] ;
     OCIDefine*	defnp[MAX_SQL_ITEMS] ;
     sb2	indicator[MAX_SQL_ITEMS] ;	
     int	col,
	        colNameLen ;
     ub2	dataType ;
     ub2	colLen ;


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

#ifdef AS_SQL


#ifdef AS_SQLSERVER

    if ( dbsqlexec (dbproc) == SUCCEED )

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

  if ( rc == OCI_SUCCESS )
    rc = OCIStmtExecute(
			dbproc->svchp, 
			stmthp,
			dbproc->errhp,
			(ub4) 1, 
			(ub4) 0,
			(CONST OCISnapshot *) NULL, 
			(OCISnapshot *) NULL, 
			OCI_DESCRIBE_ONLY /*OCI_DEFAULT*/) ;

  lHyp_sql_checkErr (	dbproc->errhp, rc ) ;

  results = rc ;
  if ( rc == OCI_SUCCESS ) 

#endif
      {
        /* now check the results from the SQL server */
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

#endif
                  //gHyp_util_debug("Column name %d is %s",i,pColName);
                  colNames[i] = pColName ;

                  if ( pTable == NULL ) {

                    /* List variables are created from the column names */
		    strcpy ( colName, pColName ) ;
#ifdef AS_PGSQL
		    /* Postgres is case sensitive, so we have to uppercase the name.
		     * This line should probably be forced for all types 
		     */
		    gHyp_util_upperCase ( colName, strlen ( colName ) ) ;
#endif
                    pVariable = gHyp_frame_createVariable ( pFrame, colName ) ;
                    gHyp_data_deleteValues ( pVariable ) ;

                    /* Store a reference to the variable in the "sqlargs" variable */
                    pData = gHyp_data_new ( NULL ) ;
                    gHyp_data_setReference ( pData, colName, NULL ) ;
                    gHyp_data_append ( pArgs, pData ) ;
                  }
                  else {

                    if ( pColStr != NULL ) {

                      /* Column labels are strings under the pCol label */
                      pVariable = gHyp_data_new ( NULL ) ;
		      strcpy ( colName, pColName ) ;
#ifdef AS_PGSQL
		     /* Postgres is case sensitive, so we have to uppercase the name.
		      * This line should probably be forced for all types 
		      */
		      gHyp_util_upperCase ( colName, strlen ( colName ) ) ;
#endif
                      gHyp_data_setStr ( pVariable, colName ) ;
                      pCol = gHyp_data_new ( pColStr ) ;
                      gHyp_data_append ( pCol, pVariable ) ;
                      gHyp_data_append ( pRow, pCol ) ;
                      gHyp_data_append ( pTable, pRow ) ;
                    }
                    else if ( pRowStr == NULL ) {
                      /* Table contains empty list variables */
		      strcpy ( colName, pColName ) ;
#ifdef AS_PGSQL
		      /* Postgres is case sensitive, so we have to uppercase the name.
		       * This line should probably be forced for all types 
		       */
		      gHyp_util_upperCase ( colName, strlen ( colName ) ) ;
#endif
                      pVariable = gHyp_data_new ( colName ) ;
                      gHyp_data_append ( pTable, pVariable ) ;
                    }
                  }

                  /* Store the variable pointer and column type for later use */
                  variables[i] = pVariable ;
#ifdef AS_SQLSERVER
                  colTypes[i] = dbcoltype ( dbproc, i+1 ) ;
                  colLens[i] = dbcollen ( dbproc, i+1 ) ; ;
#elif AS_MYSQL
                  colTypes[i] = field->type ;
                  colLens[i] = field->length ;
#elif AS_PGSQL
                  colTypes[i] = PQftype ( results, i ) ;
                  colLens[i] = PQfsize( results, i ) ;
                  isDataBinary[i] = PQfformat ( results, i ) ;
#elif AS_ORACLE
		  rc = OCIAttrGet(	(dvoid*) mypard, 
					(ub4) OCI_DTYPE_PARAM, 
					(dvoid*) &dataType,
					(ub4 *) 0, 
					(ub4) OCI_ATTR_DATA_TYPE, 
					(OCIError *) dbproc->errhp  );

		  colTypes[i] = dataType ;

		  /* Retrieve the column width in characters */
		  rc = OCIAttrGet(	(dvoid*) mypard, 
					(ub4) OCI_DTYPE_PARAM, 
					(dvoid*) &colLen, 
					(ub4 *) 0, 
					(ub4) OCI_ATTR_DATA_SIZE, 
					(OCIError *) dbproc->errhp  );
		  colLens[i] = colLen ;

		  /* Allocate a buffer to receive the data for column i+1 */
		  dataBuffer[i] = (void*) AllocMemory ( colLens[i]+1 ) ;
#endif
                }

                /* now process the rows */
                rows = 0 ;
#ifdef AS_SQLSERVER
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

                      case SQLBINARY :
                        if ( n == 1 )
                          gHyp_data_newConstant_raw ( pData, TYPE_BINARY, pBytes ) ;
                        else
                          gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                        break ;

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

                      case SQLCHAR :

			/*
			pBytes[n] = '\0' ;
                        n = gHyp_util_parseString ( (char*) pBytes ) ;
			*/

			/*if ( n <= VALUE_SIZE ) {*/
                          gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                          break ;
			/*} ;*/

                      case SQLIMAGE :
			
                        if ( n == 1 ) {
                          gHyp_data_newConstant_raw ( pData, TYPE_BINARY, pBytes ) ;
                          break ;
			}

                      case SQLTEXT :

			gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
			gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
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
                        else if ( n <= VALUE_SIZE )
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
                      pCol = gHyp_data_new ( pColStr ) ;
                      gHyp_data_append( pCol, pData ) ;
                      gHyp_data_append ( pRow, pCol ) ;
                    }
                    else if ( pRowStr != NULL ) {

                      /* Add the element to the row variable */
		      strcpy ( colName, colNames[i] ) ;
                      pVariable = gHyp_data_new ( colName ) ;
                      gHyp_data_append( pVariable, pData ) ;
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

                        /*n = gHyp_util_parseString ( (char*) pBytes ) ;*/

			/*if ( n <= VALUE_SIZE ) {*/
                          gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                          break ;
			/*}*/

                      case FIELD_TYPE_BLOB :

                        if ( n == 1 ) {
                          gHyp_data_newConstant_raw ( pData, TYPE_BINARY, pBytes ) ;
                          break ;
			}

		      default:

			gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
			gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;

                      }
                    }
                    if ( pColStr != NULL ) {

                      /* Append the column element to the row element */
                      pCol = gHyp_data_new ( pColStr ) ;
                      gHyp_data_append( pCol, pData ) ;
                      gHyp_data_append ( pRow, pCol ) ;

                    }
                    else if ( pRowStr != NULL ) {

                      /* Add the element to the row variable */
		      strcpy ( colName, colNames[i] ) ;
                      pVariable = gHyp_data_new ( colName ) ;
                      gHyp_data_append( pVariable, pData ) ;
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
                /*  Process PG rows  */

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

                      case 16:
                        /* Boolean - true/false */
                        if ( strcmp ( (char*) pBytes, "true" ) == 0 )
                          gHyp_data_setBool ( pData, TRUE ) ;
                        else
                          gHyp_data_setBool ( pData, TRUE ) ;

                        break ;

                      case 18 :
                        /* TYPE_CHAR */
                        gHyp_data_newConstant_raw ( pData, TYPE_CHAR, (char*)pBytes ) ;
                        break ;

                      case 21 :
                        /* TYPE_SHORT */
                        if ( isDataBinary[col] )
                          gHyp_data_newConstant_raw ( pData, TYPE_SHORT, (char*)pBytes ) ;
                        else
                          gHyp_data_newConstant_scanf ( pData, TYPE_SHORT, (char*)pBytes, n ) ;
                        break ;

                      case 23:
                        /* TYPE_LONG */
                        if ( isDataBinary[col] )
                          gHyp_data_newConstant_raw ( pData, TYPE_LONG, (char*)pBytes ) ;
                        else
                          gHyp_data_newConstant_scanf ( pData, TYPE_LONG, (char*)pBytes, n ) ;
                        break ;

                      case 700 :

                        /* 4-byte single-precision */
                        if ( isDataBinary[col] )
                          gHyp_data_newConstant_raw ( pData, TYPE_FLOAT, (char*)pBytes ) ;
                        else
                          gHyp_data_newConstant_scanf ( pData, TYPE_FLOAT, (char*)pBytes, n ) ;
                        break ;

                      case 701 :
                        if ( isDataBinary[col] )
                          gHyp_data_newConstant_raw ( pData, TYPE_DOUBLE, (char*)pBytes ) ;
                        else
                          gHyp_data_newConstant_scanf ( pData, TYPE_DOUBLE, (char*)pBytes, n ) ;
                        break ;

                      case 17 :
		      case 1043:

                        /* Text, use util_parseString to internalize */
                        /* Convert the string to internal form */
                        /*n = gHyp_util_parseString ( (char*) pBytes ) ;*/

			/*if ( n <= VALUE_SIZE ) {*/
                          gHyp_data_setStr_n ( pData, pBytes, n ) ;
                          break ;
			/*}*/

                      case 25 :
                      default :

			gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
			gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
                        break ;

                    }
                    if ( pColStr != NULL ) {

                      /* Append the column element to the row element */
                      pCol = gHyp_data_new ( pColStr ) ;
                      gHyp_data_append( pCol, pData ) ;
                      gHyp_data_append ( pRow, pCol ) ;

                    }
                    else if ( pRowStr != NULL ) {

                      /* Add the element to the row variable */
		      strcpy ( colName, colNames[i] ) ;
		      gHyp_util_upperCase ( colName, strlen ( colName ) ) ;
                      pVariable = gHyp_data_new ( colName ) ;
                      gHyp_data_append( pVariable, pData ) ;
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

		if ( stmthp )
		  OCIHandleFree( (dvoid *)stmthp, (ub4) OCI_HTYPE_STMT);

  /* Create handle for the statement */
  rc = OCIHandleAlloc(	(dvoid *) dbproc->envhp, 
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
		    /* Associate the buffer with a column definition */
		    rc = OCIDefineByPos(stmthp, 
					&defnp[i], 
					dbproc->errhp,
					i+1, 
					(dvoid *) dataBuffer[i],
					(sword) colLens[i], 
					(ub2) colTypes[i],
					(dvoid *) &indicator[i], 
					(ub2 *)&dataLen[i],
					(ub2 *)0, 
					OCI_DEFAULT);
			lHyp_sql_checkErr ( dbproc->errhp, rc ) ;
		  }
		}

                /*  Process ORACLE rows  */
		rc = OCIStmtExecute(
			dbproc->svchp, 
			stmthp,
			dbproc->errhp,
			(ub4) 1, 
			(ub4) 0,
			(CONST OCISnapshot *) NULL, 
			(OCISnapshot *) NULL, 
			OCI_DEFAULT) ;

		lHyp_sql_checkErr ( dbproc->errhp, rc ) ;

                rows = 0 ;
                while ( (results = OCIStmtFetch2(
					stmthp, 
					dbproc->errhp, 
					(ub4) 1,
					(ub2) OCI_FETCH_NEXT,
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

                      case 1 :
                      case 96 :
                        /* VARCHAR2, CHAR */
                        gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                        break ;

		      case 2:
			 /* NUMBER */
                        gHyp_data_newConstant_scanf ( pData, TYPE_LONG, (char*)pBytes, n ) ;
                        break ;

                      case 8:
                        /* LONG */
                        gHyp_data_newConstant_raw ( pData, TYPE_LONG, (char*)pBytes ) ;
                        break ;

		      case 23:
		      case 24 :
			/* RAW and LONG RAW */
                        if ( n == 1 )
                          gHyp_data_newConstant_raw ( pData, TYPE_BINARY, pBytes ) ;
                        else
                          gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                        break ;

                      default :

			gHyp_data_setVariable ( pData, "_data_", TYPE_STRING ) ;
			gHyp_util_breakStream ( (char*) pBytes, n, pData, TRUE ) ;
                        break ;

                    }
                    if ( pColStr != NULL ) {

                      /* Append the column element to the row element */
                      pCol = gHyp_data_new ( pColStr ) ;
                      gHyp_data_append( pCol, pData ) ;
                      gHyp_data_append ( pRow, pCol ) ;

                    }
                    else if ( pRowStr != NULL ) {

                      /* Add the element to the row variable */
		      strcpy ( colName, colNames[i] ) ;
		      gHyp_util_upperCase ( colName, strlen ( colName ) ) ;
                      pVariable = gHyp_data_new ( colName ) ;
                      gHyp_data_append( pVariable, pData ) ;
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

#ifdef AS_PGSQL
                if ( results ) PQclear ( results ) ;
                results = NULL ;
#elif AS_MYSQL
	        if ( results ) mysql_free_result ( results ) ;
	        results = NULL ;
#elif AS_ORACLE

		for ( i=0; i<numCols; i++ )
		  ReleaseMemory ( dataBuffer[i] ) ;

		if ( stmthp )
		  OCIHandleFree( (dvoid *)stmthp, (ub4) OCI_HTYPE_STMT);

#endif

/*
#ifdef AS_MYSQL
	        if ( rows == 0 ) {
	          rows = (int) mysql_affected_rows ( dbproc ) ;
	          *gHyp_util_debug("Affected rows = %d",rows);*
	        }
#endif
*/
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
        msg[msgLen] = '\0' ;
        gHyp_instance_warning ( pAI, STATUS_SQL, msg ) ;
        status = FALSE ;

      }

#elif AS_ORACLE

#else

      /* Probably SQLSERVER, but there is already a message handler */
      /* SO, don't add anything to _sql_status_ 
      gHyp_data_setText ( pStatus, msg, msgLen ) ; 
      */

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

#endif

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 3 && argCount !=4 && argCount != 5 )
      gHyp_instance_error (
               pAI, STATUS_ARGUMENT,
               "Invalid arguments. Usage: sql_open ( user, password, server [,database [,isSecure]])");

    if ( argCount == 5 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      isSecure = gHyp_data_getBool ( pData,
                                     gHyp_data_getSubScript ( pData ),
                                     TRUE )  ;
    }

    if ( argCount >= 4 ) {
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
    }
    else
      strcpy ( sql_database, "" ) ;

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
 
  if ( rc == OCI_SUCCESS )
    /* Attach to the server */
    rc = OCIServerAttach(	dbproc->srvhp, 
				dbproc->errhp,
				(text *)sql_server,
				strlen(sql_server),
				OCI_DEFAULT );
  
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
      strVal2[VALUE_SIZE+1],
      *pStr ;
    
    sLOGICAL
      isVector ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI,STATUS_ARGUMENT,
	"Invalid arguments. Usage: sql_toexternal ( string )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_toexternal_", TYPE_STRING ) ;
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
      }
      else {
        n = gHyp_data_getStr ( pValue, 
			     strVal, 
			     VALUE_SIZE, 
			     context, 
			     isVector ) ;
        pStr = strVal ;
        pValue2 = gHyp_data_new ( NULL ) ;
        n = gHyp_util_unparseString ( strVal2, pStr, n, VALUE_SIZE, FALSE, FALSE, TRUE,"" ) ;
        gHyp_data_setStr_n ( pValue2, strVal2, n ) ;
      }

      gHyp_data_append ( pResult, pValue2 ) ;
    }
    if ( context== -2 && ss != -1 ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%s' is out of bounds in sql_toexternal()",
			    ss);
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

