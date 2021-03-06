/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/

/**********************	ENVIRONMENT DEFINITIONS ******************************/

EXEC SQL include sqltypes;
EXEC SQL include locator;
EXEC SQL include datetime;
EXEC SQL include decimal;
EXEC SQL include sqlca;
EXEC SQL include sqlda;

typedef struct sqlda SQLDA ;
#define WARNNOTIFY        1
#define NOWARNNOTIFY      0

#define LCASE(c) (isupper(c) ? tolower(c) : (c))
#define BUFFSZ 256

extern char statement[80];

EXEC SQL BEGIN DECLARE SECTION;
    loc_t lcat_descr;
    loc_t lcat_picture; 
EXEC SQL END DECLARE SECTION; 

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

    EXEC SQL WHENEVER SQLERROR GOTO sql_error ;

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
    EXEC SQL BEGIN DECLARE SECTION;
      char sql_statement[MAX_SQL_STMT_LENGTH+1] ;
    EXEC SQL END DECLARE SECTION; 

    /* Process the SQL statement */
    isParse = 1 ;           /* Set a flag for sql_error */
    EXEC SQL PREPARE S FROM :sql_statement ;
    isParse = 0 ;

    EXEC SQL DECLARE C CURSOR FOR S ;



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
      EXEC SQL OPEN C USING DESCRIPTOR bind_dp ;
      
      /* Process the select list, if specified */
      if ( ( strncmp ( sql_statement, "SELECT", 6 ) != 0 ) &&
	   ( strncmp ( sql_statement, "select", 6 ) != 0 ) )
	
	/* Not a 'select' statement. We're done. */
	select_dp->F = 0 ;
      
      else {
	
	/* A 'SELECT' statement.  Obtain info about the SELECT LIST */
	select_dp->N = MAX_SQL_ITEMS ;
	EXEC SQL DESCRIBE S INTO select_dp ;
	
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
	  EXEC SQL WHENEVER NOT FOUND GOTO end_fetch;
	
	  /* Clear the variables to receive the data */
	  for ( i=0; i<select_dp->F; i++ )
	    gHyp_data_deleteValues ( variables[i] ) ;

	  for (;;) {
	  
	    EXEC SQL FETCH C USING DESCRIPTOR select_dp ;
	  
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

    EXEC SQL WHENEVER SQLERROR CONTINUE ;
    EXEC SQL CLOSE C ;
    
    if ( status ) {
      EXEC SQL COMMIT WORK ;
    } 
    else { 
      EXEC SQL ROLLBACK WORK ;
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

    EXEC SQL WHENEVER SQLERROR CONTINUE ;
    EXEC SQL ROLLBACK WORK ;

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

    EXEC SQL WHENEVER SQLERROR GOTO connect_error;
    
    EXEC SQL BEGIN DECLARE SECTION;
      char sql_using[MAX_SQL_NAME_LENGTH+1] ;
    EXEC SQL END DECLARE SECTION; 

    EXEC SQL CONNECT TO :sql_using;

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
