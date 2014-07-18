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
 *  4/13/2004:Yockey:Add PostgreSQL support,work in process
 *
 * $Log: sql.c,v $
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

#ifdef AS_MYSQL
#ifdef AS_UNIX
#include <mysql/mysql.h>
#else
#include <mysql.h>
#endif
#endif

#ifdef AS_PGSQL
#include <pgsql/libpq-fe.h>
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
      gHyp_util_logInfo ("SQL Server Message %ld: %s\n", msgno, msgtext);
  return (0);
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
      *pColStr ;

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
      *colNames[MAX_SQL_ITEMS],
      *pColName ;

#endif

#ifdef AS_SQL
#ifdef AS_SQLSERVER
    DBPROCESS  *dbproc ;
    RETCODE    results ;
#elif AS_MYSQL
    MYSQL *dbproc ;
    MYSQL_RES *results ;
    MYSQL_FIELD *field ;
    MYSQL_ROW row ;
    unsigned long *length ;
    char
      *pSQL,
      *pEndSQL,
      sql_stmt[MAX_SQL_STMT_LENGTH+1] ;
#elif AS_PGSQL
    PGconn      *dbproc ;
    PGresult    *results ;
    int
        numRows,
        isDataBinary[MAX_SQL_ITEMS],
        row,col;
    char
      *pSQL,
      *pEndSQL,
      sql_stmt[MAX_SQL_STMT_LENGTH+1] ;
#endif
#else
    void *dbproc ;
#endif

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
    status = TRUE ;

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
#endif
      gHyp_data_getHandle ( pData,
                            gHyp_data_getSubScript(pData),
                            TRUE);

    /* Get the SQL statement */
    pData2 = gHyp_stack_popRdata ( pStack, pAI ) ;

    /* Build the SQL statement */
#ifdef AS_MYSQL
    pSQL = sql_stmt ;
    pEndSQL = pSQL + MAX_SQL_STMT_LENGTH ;
#elif AS_PGSQL
    pSQL = sql_stmt ;
    pEndSQL = pSQL + MAX_SQL_STMT_LENGTH ;
#endif
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
      dbcmd (dbproc, value );
#elif defined ( AS_MYSQL ) || defined ( AS_PGSQL )
/* for AS_MYSQL and AS_PGSQL.  */
      if ( (pSQL + valueLen) > pEndSQL )
        gHyp_instance_error ( pAI, STATUS_SQL,
                              "SQL statement longer than %d characters \n",
                              MAX_SQL_STMT_LENGTH ) ;
      sprintf ( pSQL, "%s", value ) ;
      pSQL += valueLen ;
      /* printf ("sql_stmt=%s",sql_stmt);  */

#endif
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
        /*gHyp_util_debug("status=%s",PQresStatus(PQresultStatus(results)));*/
        if ((PQresultStatus(results) == PGRES_TUPLES_OK) )
#endif
      {
        /* now check the results from the SQL server */
        while

#ifdef AS_SQLSERVER
         ( (results = dbresults(dbproc)) != NO_MORE_RESULTS )
#elif AS_MYSQL
         ( (results = mysql_use_result(dbproc)) != NULL )
#elif AS_PGSQL
         (  results  )
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
#endif
                for ( i=0 ; i<numCols; i++ ) {

#ifdef AS_SQLSERVER
                  pColName = (char*) dbcolname ( dbproc, i+1 ) ;
#elif AS_MYSQL
                  field = mysql_fetch_field ( results ) ;
                  pColName = field->name ;
#elif AS_PGSQL
                  pColName = (char*) PQfname( results, i) ;
#endif
                  //gHyp_util_debug("Column name %d is %s",i,pColName);
                  colNames[i] = pColName ;

                  if ( pTable == NULL ) {

                    /* List variables are created from the column names */
                    pVariable = gHyp_frame_createVariable ( pFrame, pColName ) ;
                    gHyp_data_deleteValues ( pVariable ) ;

                    /* Store a reference to the variable in the "sqlargs" variable */
                    pData = gHyp_data_new ( NULL ) ;
                    gHyp_data_setReference ( pData, pColName, NULL ) ;
                    gHyp_data_append ( pArgs, pData ) ;
                  }
                  else {

                    if ( pColStr != NULL ) {

                      /* Column labels are strings under the pCol label */
                      pVariable = gHyp_data_new ( NULL ) ;
                      gHyp_data_setStr ( pVariable, pColName ) ;
                      pCol = gHyp_data_new ( pColStr ) ;
                      gHyp_data_append ( pCol, pVariable ) ;
                      gHyp_data_append ( pRow, pCol ) ;
                      gHyp_data_append ( pTable, pRow ) ;
                    }
                    else if ( pRowStr == NULL ) {
                      /* Table contains empty list variables */
                      pVariable = gHyp_data_new ( pColName ) ;
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

                      case SQLCHAR :
                      case SQLTEXT :
                      case SQLIMAGE :
                        gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                        break ;

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

                      default :
                        n = dbconvert ( dbproc,colTypes[i],pBytes,n,SQLCHAR,value,VALUE_SIZE);
                        if ( n > 0 )
                          gHyp_data_setStr_n ( pData, (char*) value, n ) ;
                        else
                          gHyp_data_setStr_n (pData, "NULL", 4 ) ;
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
                      pVariable = gHyp_data_new ( colNames[i] ) ;
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

                      case FIELD_TYPE_STRING :
                      case FIELD_TYPE_VAR_STRING :
                        gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                        break ;

                      case FIELD_TYPE_BLOB :
                        if ( n == 1 )
                          gHyp_data_newConstant_raw ( pData, TYPE_BINARY, pBytes ) ;
                        else
                          gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
                        break ;

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

                      default :
                        gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
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
                      pVariable = gHyp_data_new ( colNames[i] ) ;
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
                gHyp_util_debug("Tuples = %d",numRows);
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

                      case 17 :
                        /* Text, use util_parseString to internalize */
                        /* Convert the string to internal form */
                        n = gHyp_util_parseString ( pBytes ) ;
                        gHyp_data_setStr_n ( pData, pBytes, n ) ;
                        break ;

                      case 18 :
                        /* TYPE_CHAR */
                        gHyp_data_newConstant_raw ( pData, TYPE_BYTE, (char*)pBytes ) ;
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

                      case 25 :
                      default :
                        gHyp_data_setStr_n ( pData, (char*) pBytes, n ) ;
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
                      pVariable = gHyp_data_new ( colNames[col] ) ;
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
                PQclear ( results ) ;
                results = NULL ;
/*  end of PGSQL row processing */
#endif

	   /* Post processing, results were found */

            #ifdef AS_PGSQL
              if ( results ) PQclear ( results ) ;
              results = NULL ;
	    #elif AS_MYSQL
	      if ( results ) mysql_free_result ( results ) ;
	      results = NULL ;
            #endif

/*
#ifdef AS_MYSQL
	    if ( rows == 0 ) {
	      rows = (int) mysql_affected_rows ( dbproc ) ;
	      gHyp_util_debug("Affected rows = %d",rows);
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
      /* Query failed */
#ifdef AS_MYSQL
        gHyp_instance_warning ( pAI, STATUS_SQL, 
	    "Failed SQL query, reason is '%s'.",mysql_error(dbproc) );
#else
        gHyp_instance_warning ( pAI, STATUS_SQL, 
	    "Failed SQL query" );
#endif
        status = FALSE ;
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
#endif

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount !=4 && argCount != 5 )
      gHyp_instance_error (
               pAI, STATUS_ARGUMENT,
               "Invalid arguments. Usage: sql_open ( user, password, server, database [,isSecure])");

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

      if ( dbuse ( dbproc, sql_database ) == SUCCEED )
        gHyp_data_setHandle ( pResult, (void*) dbproc ) ;
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
#endif
      gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE);

#ifdef AS_SQLSERVER
    dbclose ( dbproc ) ;
#elif AS_MYSQL
    mysql_close ( dbproc ) ;
#elif AS_PGSQL
    PQfinish( dbproc );
#endif
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
    return ;
  }
}
