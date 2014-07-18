/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!****************************************************************************/

/* 
 * Modifications:
 *
 *	$Log: promis.c,v $
 *	Revision 1.16  2004/07/28 00:49:18  bergsma
 *	Version 3.3.0
 *	
 *	Revision 1.16  2004/07/27 15:44:01  bergsma
 *	Undo fix where nulls were stripped from PROMIS database fields
 *	
 *	Revision 1.15  2004/07/23 18:45:16  bergsma
 *	- Fixed traceback when accessing 'data' from 'tres' in PROMIS, pTEST_DII not defined.
 *	- Added pcapture().
 *	
 *	Revision 1.14  2004/07/01 02:00:20  bergsma
 *	Remove trailing nulls from field 'str' values.
 *	
 *	Revision 1.13  2004/06/13 14:05:07  bergsma
 *	Cosmetic log statements.
 *	
 *	Revision 1.12  2004/05/15 02:09:08  bergsma
 *	Added callback from tut_output when doing pexec()
 *	
 *	Revision 1.11  2003/04/27 13:40:28  bergsma
 *	Typo in VMS PROMIS compiled code: 'rturn' used instead of 'return'
 *	
 *	Revision 1.10  2003/04/23 22:56:43  bergsma
 *	When parsing TRES, the status return value was not being tested properly,
 *	as a result the tres varables would not be exposed.
 *	
 *	Revision 1.9  2003/04/11 02:02:55  bergsma
 *	When PROMIS switches images, hyperscript is forced to exit.  In the exit handler, make
 *	sure any outstanding reply messages are sent.
 *	
 *	Revision 1.8  2003/04/04 16:19:51  bergsma
 *	1. Added plogopen and plogclose functions.
 *	2. When more than one instance exists, doing a pexec() would cause
 *	infinite looping because the state of instance was not getting reset back
 *	to PARSE properly.
 *	
 *	Revision 1.7  2003/01/14 02:32:32  bergsma
 *	Pre-compile typos.
 *	
 *	Revision 1.6  2003/01/12 05:06:57  bergsma
 *	V3.1.0
 *	1. Incorportate instance_pushSTATUS for promis functions
 *	2. psnap returns the 24 lines as a result, the SNAP variable is deprecated
 *	3. To handle multiple hyperscript instances, the call to hyperscript must
 *	be modified so that only the parent instance can come back for more stdin
 *	but that any instance can pexec() (send a result back to PROMIS to execute).
 *	
 *	Revision 1.5  2002/11/20 20:44:54  bergsma
 *	Remove check for superuser in plogin function, which is deprecated anyway.
 *	
 *	Revision 1.4  2002/11/18 23:09:16  bergsma
 *	Modified plogin so that it is only allowed for promis superusr.
 *	
 *	Revision 1.3  2002/09/24 17:20:16  bergsma
 *	Some additional comments in gHyp_promis_addTLOG
 *	
 *	Revision 1.2  2002/09/03 21:24:54  bergsma
 *	Commented out debug statement.
 *	
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

#ifdef AS_VMS
#include <ssdef.h>
#endif

#ifdef AS_PROMIS

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

static sInstance *gpAI = NULL ;	/* HyperScript instance */
static sInstance *gpAImain = NULL ;	/* Main HyperScript instance */
static char		gzResult[MAX_PROMIS_RESULT_SIZE+1] ;
static sData		*gpData ;
static sLOGICAL         giIsHSenabled = FALSE ;
static sLOGICAL         giIsCallbackEnabled = FALSE ;

#define MAX_PROMIS_FILES  200
#define MAX_PROMIS_TABLES 100
#define MAX_PROMIS_ENTRIES (MAX_PROMIS_FILES+MAX_PROMIS_TABLES)
#define MAX_PROMIS_FIELDS 500
#define PROMIS_DATE_OFFSET  946684800
static sField    (*gpsFieldCache[MAX_PROMIS_ENTRIES])[MAX_PROMIS_FIELDS] ;
static int 	 giNumFields[MAX_PROMIS_ENTRIES] ;
static long	 gzTableNames[MAX_PROMIS_TABLES] ;
static long	 giNumTables=MAX_PROMIS_TABLES ;

/********************** INTERNAL OBJECT STRUCTURES ***************************/

#define FIL__DTINTEGER 101          /* integer */
#define FIL__DTREAL    102          /* real number */
#define FIL__DTDATE    103          /* date in Vms internal format */
#define FIL__DTSTRING  104          /* character string */
#define FIL__DTBYTE    105          /* byte */
#define FIL__DTOTHER   106          /* other data type */
#define FIL__DTSTRUCT  107          /* Field is a structure */


#define CMP_EQ		301
#define CMP_GE		302
#define CMP_GT		303

struct field_t {
  char		*(*name)[MAX_PROMIS_FIELDS] ;
  int		nameCount ;
  int		type ;
  int		offset ;
  int		size ;
  int		count ;
  int		id ;
} ;
    
/**********************	FUNCTION DEFINITIONS ********************************/

#if defined ( AS_VMS ) && defined ( __cplusplus )
extern "C" int aeqSsp_autoMan_openFile ( sDescr*, sDescr*, int* ) ;
extern "C" int aeqSsp_autoMan_closeFile ( sDescr*, sDescr*, int* ) ;
extern "C" void aeqSsp_autoMan_closeFiles ( ) ;
extern "C" int aeqSsp_autoMan_getrec ( sDescr*, int*, sDescr*, sDescr*, int* ) ;
extern "C" int aeqSsp_autoMan_getnext ( sDescr*, sDescr*, int* ) ;
extern "C" int aeqSsp_autoMan_snap ( int*, sDescr*, int* );
extern "C" int uaf_user_detLogin ( sDescr* ) ;
extern "C" int Fil_Tbl_FieldCounter ( long*, long*, long* ) ;
extern "C" int Fil_Tbl_FieldName ( long*, long*, sDescr* ) ;
extern "C" int Fil_Tbl_RecFieldInfo ( long*, long*, sDescr*, 
				  long*, long*, long*, long*, long* ) ;
extern "C" int Fil_Tbl_RecordInfo(long*, long*, long*, long*, long*, long*, long*) ;
extern "C" int Fil_Tbl_FileIndex ( long*, long* );
extern "C" int Fil_Tbl_FileInfo ( long*, long*, long*, long*, long*, long*) ;
extern "C" int Fil_Tbl_ListRecordValues ( long*, long*, long*, long* );
extern "C" int aeqSsp_automan_unpack ( long*, long*, long*, sBYTE* ) ;
extern "C" int uaf_info_hyperscript() ;
extern "C" void aeqSsp_automan_initDbFiles() ;
extern "C" void aeqSsp_automan_initTrFiles() ;
extern "C" int  aeqSsp_autoMan_getSymbol ( sDescr*, sDescr*, int*, sDescr*, int*) ;
extern "C" int  aeqSsp_autoMan_getParameter ( sDescr*, sDescr*, int*, sDescr*, int*) ;
extern "C" int  aeqSsp_automan_getTrRecord ( sDescr*, long*, sBYTE** ) ;
extern "C" int  aeqSsp_automan_getDbRecord ( sDescr*, long*, sBYTE** ) ;
extern "C" int  sys$dclexh  (void *desblk);
extern "C" void	gHyp_promis_exitHandler(int) ;
extern "C" sLOGICAL gHyp_promis_hs ( sDescr*, long, long, sLOGICAL*,
				 sDescr*, unsigned long* ) ;
extern "C" void Tut_LogFile_Open ( sDescr* ) ;
extern "C" void aeqSsp_automan_closeLog ( ) ;
extern "C" void aeqSsp_automan_enableCallback() ;
extern "C" void aeqSsp_automan_disableCallback() ;
extern "C" void gHyp_promis_putline ( sDescr*, long ) ;
#else
extern int aeqSsp_autoMan_openFile ( sDescr*, sDescr*, int* ) ;
extern int aeqSsp_autoMan_closeFile ( sDescr*, sDescr*, int* ) ;
extern void aeqSsp_autoMan_closeFiles ( ) ;
extern int aeqSsp_autoMan_getrec ( sDescr*, int*, sDescr*, sDescr*, int* ) ;
extern int aeqSsp_autoMan_getnext ( sDescr*, sDescr*, int* ) ;
extern int aeqSsp_autoMan_snap ( int*, sDescr*, int* );
extern int uaf_user_detLogin ( sDescr* ) ;
extern int Fil_Tbl_FieldCounter ( long*, long*, long* ) ;
extern int Fil_Tbl_FieldName ( long*, long*, sDescr* ) ;
extern int Fil_Tbl_RecFieldInfo ( long*, long*, sDescr*, 
				  long*, long*, long*, long*, long* ) ;
extern int Fil_Tbl_RecordInfo(long*, long*, long*, long*, long*, long*, long*) ;
extern int Fil_Tbl_FileIndex ( long*, long* );
extern int Fil_Tbl_FileInfo ( long*, long*, long*, long*, long*, long*) ;
extern int Fil_Tbl_ListRecordValues ( long*, long*, long*, long* );
extern int aeqSsp_automan_unpack ( long*, long*, long*, sBYTE* ) ;
extern int uaf_info_hyperscript() ;
extern void aeqSsp_automan_initDbFiles() ;
extern void aeqSsp_automan_initTrFiles() ;
extern int  aeqSsp_autoMan_getSymbol ( sDescr*, sDescr*, int*, sDescr*, int*) ;
extern int  aeqSsp_autoMan_getParameter ( sDescr*, sDescr*, int*, sDescr*, int*) ;
extern int  aeqSsp_automan_getTrRecord ( sDescr*, long*, sBYTE** ) ;
extern int  aeqSsp_automan_getDbRecord ( sDescr*, long*, sBYTE** ) ;
extern int  sys$dclexh  (void *desblk);
extern void	gHyp_promis_exitHandler(int) ;
extern sLOGICAL gHyp_promis_hs ( sDescr*, long, long, sLOGICAL*,
				 sDescr*, unsigned long* ) ;
extern void Tut_LogFile_Open ( sDescr* ) ;
extern void aeqSsp_automan_closeLog ( ) ;
extern void aeqSsp_automan_enableCallback() ;
extern void aeqSsp_automan_disableCallback() ;
extern void gHyp_promis_putline ( sDescr*, long ) ;
#endif

#ifdef AS_VMS
static unsigned int lHyp_promis_exceptionHandler ( void *sigarr, void *mecharr )
{
  int signo = 1 ;
  /* Called when VMS exception occurs, re-signals to 'C' handlers */
  gHyp_promis_exitHandler( signo );
  return SS$_RESIGNAL ;
}
#endif

static sLOGICAL lHyp_promis_openFile (	sInstance	*pAI,
					sFrame 		*pFrame,
					char 		*pFile )
{
  int		status,
		statusTextLen ;
		
  char		statusText[MAX_OUTPUT_LENGTH+1] ;
  makeDSCs	( statusText_d, statusText );
  makeDSCz	( file_d, pFile ) ;
		
  status = aeqSsp_autoMan_openFile ( 	&file_d, 
     					&statusText_d,
     					&statusTextLen ) ;
  if ( status & 1 ) 
    return TRUE ;
  else {
    statusText[statusTextLen] = '\0' ;
    gHyp_instance_warning ( pAI, STATUS_IO, statusText ) ;
    return FALSE ;
  }
}

static sLOGICAL lHyp_promis_closeFile (	sInstance	*pAI,
					sFrame 		*pFrame,
					char 		*pFile )
{
  int		status,
		statusTextLen ;
		
  char		statusText[MAX_OUTPUT_LENGTH+1] ;
  makeDSCs	( statusText_d, statusText );
  makeDSCz	( file_d, pFile ) ;
		
  status = aeqSsp_autoMan_closeFile ( 	&file_d, 
     					&statusText_d,
     					&statusTextLen ) ;
  if ( status & 1 ) 
    return TRUE ;
  else {
    statusText[statusTextLen] = '\0' ;
    gHyp_instance_warning ( pAI, STATUS_IO, statusText ) ;
    return FALSE ;
  }
}

static sLOGICAL lHyp_promis_getrec (	sInstance	*pAI,
					sFrame 		*pFrame,
					char 		*pKey,
					int		comparison,
					char 		*pValue,
					int		valueLen ) 
{
  int		status,
		statusTextLen ;
		
  char		statusText[MAX_OUTPUT_LENGTH+1] ;
  makeDSCs	( statusText_d, statusText );
  makeDSCz	( key_d, pKey ) ;
  makeDSCn	( value_d, pValue, valueLen ) ;
		
  status = aeqSsp_autoMan_getrec ( 	&key_d, 
  					&comparison,
  					&value_d,
     					&statusText_d,
     					&statusTextLen ) ;
  if ( status & 1 ) 
    return TRUE ;
  else {
    statusText[statusTextLen] = '\0' ;
    gHyp_instance_warning ( pAI, STATUS_IO, statusText ) ;
    return FALSE ;
  }
}

static sLOGICAL lHyp_promis_getnext (	sInstance	*pAI,
					sFrame 		*pFrame, 
					char 		*pKey ) 
{
  int		status,
		statusTextLen ;
		
  makeDSCz	( key_d, pKey ) ;

  char		statusText[MAX_OUTPUT_LENGTH+1] ;
  makeDSCs	( statusText_d, statusText );

  status = aeqSsp_autoMan_getnext (	&key_d, 
     					&statusText_d,
     					&statusTextLen ) ;

  if ( status & 1 ) 
    return TRUE ;
  else {
    statusText[statusTextLen] = '\0' ;
    gHyp_instance_warning ( pAI, STATUS_IO, statusText ) ;
    return FALSE ;
  }
}

static sData* lHyp_promis_snap (	sInstance	*pAI,
					sFrame 		*pFrame )
{
  int		status,
		i,
  		resultLen ;
		
  sData		*pResult,
		*pValue ;

  char		result[VALUE_SIZE+1] ;
  makeDSCs	( result_d, result );

  /* deprecated *
  pVariable = gHyp_frame_createVariable ( pFrame, "SNAP" ) ;
  gHyp_data_deleteValues ( pVariable ) ;
  */

  pResult = gHyp_data_new ( "_psnap_" ) ;
  for ( i=1; i<=24; i++ ) {                            

    aeqSsp_autoMan_snap ( &i, &result_d, &resultLen ) ;
    result[resultLen] = '\0' ;
    pValue = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pValue, result ) ;
    gHyp_data_append ( pResult, pValue ) ;

  }
  return pResult ;
}


static sLOGICAL lHyp_promis_logopen ( char *pFileSpec )
{  
  makeDSCz	( fileSpec_d, pFileSpec ) ;
  
  Tut_LogFile_Open ( &fileSpec_d ) ;
  return TRUE ;
}


static sLOGICAL lHyp_promis_logclose ()
{
  long
    printerr = 0, 
    doprint = 0 ;
  
  /*Tut_LogFile_Close ( &printerr, &doprint ) ;*/
  aeqSsp_automan_closeLog ( ) ;
  
  return TRUE ;
}

static sLOGICAL lHyp_promis_login ( char *pUserId )
{
  int 
    status ;
  
  makeDSCz	( userId_d, pUserId ) ;
  
  status = uaf_user_detLogin ( &userId_d ) ;
  if ( status & 1 ) 
    return TRUE ;
  else
    return FALSE ;
}


static char *lHyp_promis_getResult ( )
{
  if ( gzResult[0] )
    return gzResult ;
  else
    return NULL ;
}

static void lHyp_promis_setResult( char * pStr )
{
  /* Copy pexec() argument into result area */
  sprintf ( gzResult, "%s ", pStr ) ;
}

static sField (*lHyp_promis_getFields ( 
  long fileIndex, long numFields) )[MAX_PROMIS_FIELDS]  
{
  if ( gpsFieldCache[fileIndex] == NULL ) {
    gpsFieldCache[fileIndex] = (sField(*)[MAX_PROMIS_FIELDS]) 
	AllocMemory (sizeof(sField) * numFields ) ;
    assert ( gpsFieldCache[fileIndex] ) ;
    giNumFields[fileIndex] = 0 ;
  }
  return gpsFieldCache[fileIndex] ;
}

void gHyp_promis_exitHandler ( int status ) 
{
  int i ;
  if ( gpsConcept ) {
    /* If the method was invoked from a query, then send all replies */
    lHyp_promis_logclose() ;
    while ( gHyp_instance_replyMessage ( 
            gpAImain,
	    gHyp_frame_getMethodData(gHyp_instance_frame(gpAImain) ) ) ) ;

    guRunFlags &= ~RUN_QUIET ;
    /*gHyp_util_log("PROMIS image is exiting - HyperScript must terminate");*/
    aeqSsp_autoMan_closeFiles ( ) ;
    gHyp_promis_cleanFields ( -1 ) ;
    gHyp_concept_quit ( gpsConcept ) ;
    gpsConcept = NULL ;
    giIsHSenabled = FALSE ;
    giIsCallbackEnabled = FALSE ;
  }    
  return ;
}

static int lHyp_promis_parseRecord ( sData *pTV,
				     long fileId,
				     sBYTE *pRecord,
				     sField (*field)[MAX_PROMIS_FIELDS],
				     int numFields,
				     int offset,
				     sLOGICAL isLittleEndian )
{
  char
    sb,
    *pFieldName,
    *(*name)[MAX_PROMIS_FIELDS],
    counterName[TOKEN_SIZE+1],
    token[TOKEN_SIZE+1],
    value[VALUE_SIZE+1],
    timeStamp[VALUE_SIZE+1] ;

  makeDSCs	( timeStamp_d, timeStamp ) ;
  makeDSCs	( counterName_d, counterName ) ;

  sDescr
    *pDescr ;

  sData
    *pCounter,
    *pStruct,
    *pData,
    *pValue ;

  int
    i,
    j,
    k ;

  time_t ts ;

  long
    stat,
    fieldNameCount,
    fieldOffset,
    fieldType,
    fieldSize,
    fieldCount,
    firstFieldId,
    fieldId,
    structNameCount,
    structOffset,
    structEnd,
    counterId,
    firstStructIndex,
    lastStructIndex,
    componentOffset,
    numStructFields,
    recSize ;

  sBYTE
    *pBuf ;


  sEndian
    endian ;

  sField
    (*pFields)[MAX_PROMIS_FIELDS],
    *pField,
    *pStructField ;

  struct tm 
    *pstm ;

  i = 0 ;
  while ( i < numFields ) {

    pField = &(*field)[i] ;
    name = pField->name ;
    pFieldName = (*name)[0] ;
    fieldNameCount = pField->nameCount ;
    fieldCount = pField->count ;
    fieldSize = pField->size ;
    fieldType = pField->type ;
    fieldOffset = pField->offset ;
    fieldId = pField->id ;
    if ( i == 0 ) firstFieldId = fieldId ;

    pBuf = pRecord + fieldOffset + offset ;
    /*
    gHyp_util_logInfo ( "Field %s (%p),Offset=(%d,%d),Size=%d,Count=%d",
	pFieldName,pBuf,offset,fieldOffset,fieldSize,fieldCount);
    */
    /* Check if fieldCount is overridden */
    if ( fieldCount > 1 ) {

      stat = Fil_Tbl_FieldCounter ( &fileId, &fieldId, &counterId ) ;
      if ( (stat & 1) && counterId > 0 )  {

        stat = Fil_Tbl_FieldName ( &fileId, &counterId, &counterName_d ) ;
        if ( (stat & 1) ) {
          counterName[counterName_d.dsc_w_length] = '\0' ;
          gHyp_util_trim ( counterName ) ;
          gHyp_util_lowerCase ( counterName, strlen ( counterName ) ) ;
	  pCounter = gHyp_data_getChildByName ( pTV, counterName ) ;	   
	  if ( pCounter ) {
	    k = gHyp_data_getInt ( pCounter, -1, TRUE ) ;
	    if ( k > 0 && k < fieldCount ) fieldCount = k ;
            /*
	    gHyp_util_logInfo("Counter =%s,size=%d",counterName,fieldCount);
 	    */
	  }
        }
      }
    }

    switch ( fieldType ) {

    case FIL__DTSTRUCT :

      if ( fieldNameCount < 1 ) {	

        pStruct = gHyp_data_new ( pFieldName ) ;
        gHyp_data_setHashed ( pStruct, TRUE ) ;
        gHyp_data_append ( pTV, pStruct ) ;

      }
      else
        pStruct = pTV ;

      /* Find number of fields in structure. */
      structOffset = offset + fieldOffset ;
      structEnd = structOffset + ( fieldSize * fieldCount ) ;	
      firstStructIndex  = i + 1 ;		

      j = firstStructIndex ;
      while ( j < numFields ) {

        /* Look for the first field past the end of the record */
        pStructField = &(*field)[j] ;
        if ( pStructField->offset + offset >= structEnd ) 
	  break ;
	else
	  j++ ;

      }
      lastStructIndex  = j - 1 ;
      numStructFields = lastStructIndex - firstStructIndex + 1 ;

      for ( j=0; j<fieldCount; j++ ) {	

	if ( fieldNameCount > 0 && j < fieldNameCount )
	  strcpy ( token, (*name)[j] ) ;
	else
	  sprintf ( token, "%s_%d", pFieldName, j ) ;
	
        pData = gHyp_data_new ( token ) ;

        gHyp_data_append ( pStruct, pData ) ;
        /*gHyp_util_logInfo ( "...adding .%s", token ) ;*/
	componentOffset = structOffset + (fieldSize*j) ;
	pFields = (sField (*)[MAX_PROMIS_FIELDS]) &(*field)[firstStructIndex] ;
        stat = lHyp_promis_parseRecord ( pData,
					 fileId,
					 pRecord,
					 pFields,
					 numStructFields,
					 componentOffset,
					 isLittleEndian ) ;
	if ( stat <= 0 ) return -1 ;
      }

      i = lastStructIndex ;
      break ;

    case FIL__DTINTEGER :

      pData = gHyp_data_new ( pFieldName ) ;

      if ( fieldSize == 2 ) 
        gHyp_data_newVector ( pData, TYPE_SHORT, fieldCount, FALSE ) ;
      else
        gHyp_data_newVector ( pData, TYPE_LONG, fieldCount, FALSE ) ;

      for ( k=0; k<fieldCount; k++ ) {
        endian.x.sl = 0 ;
        if ( isLittleEndian ) 
	  for ( j=fieldSize-1; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
        else
	  for ( j=0; j<fieldSize; j++ ) endian.x.b[j] = *pBuf++ ;
	if ( fieldSize == 2 )
          gHyp_data_setVectorRaw ( pData, &endian.x.ss, k ) ;
	else
          gHyp_data_setVectorRaw ( pData, &endian.x.sl, k ) ;
      }
      /*gHyp_util_logInfo ( "(INT).%s", gHyp_data_print ( pData ) ) ;*/
      gHyp_data_append ( pTV, pData ) ;
      break ;

    case FIL__DTREAL    :

      pData = gHyp_data_new ( pFieldName ) ;

      if ( fieldSize == 4 ) 
        gHyp_data_newVector ( pData, TYPE_FLOAT, fieldCount, FALSE ) ;
      else	
        gHyp_data_newVector ( pData, TYPE_DOUBLE, fieldCount, FALSE ) ;

      for ( k=0; k<fieldCount; k++ ) {

        endian.x.f = 0 ;
        if ( isLittleEndian ) 
	  for ( j=fieldSize-1; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
        else
	  for ( j=0; j<fieldSize; j++ ) endian.x.b[j] = *pBuf++ ; 
	if ( fieldSize == 4 )
          gHyp_data_setVectorRaw ( pData, &endian.x.f, k ) ;
	else
          gHyp_data_setVectorRaw ( pData, &endian.x.d, k ) ;
      }
      /*gHyp_util_logInfo ( "(FLOAT).%s", gHyp_data_print ( pData ) ) ;*/
      gHyp_data_append ( pTV, pData ) ;
      break ;

    case FIL__DTDATE    :
      pData = gHyp_data_new ( pFieldName ) ;
      gHyp_data_newVector ( pData, TYPE_LONG, fieldCount, FALSE ) ;

      for ( k=0; k<fieldCount; k++ ) {
        endian.x.sl = 0 ;
        if ( isLittleEndian )
          for ( j=fieldSize-1; j>=0; j-- ) endian.x.b[j] = *pBuf++ ;
        else
          for ( j=0; j<fieldSize; j++ ) endian.x.b[j] = *pBuf++ ;
	
	/* Ansi time for year 2000 == PROMIS "0" time */
	ts = endian.x.sl + PROMIS_DATE_OFFSET ;
	pstm = localtime ( &ts ) ;
	ts = ts - pstm->tm_gmtoff ;
	
	gHyp_data_setVectorRaw ( pData, &ts, k ) ;
	
	/* Convert to string */
        /*
	  aeqSsp_autoutil_dateString2 ( &endian.x.sl, &timeStamp_d ) ;
	  ts = (long) date_parse( timeStamp ) ;
        
          gHyp_data_setVectorRaw ( pData, &ts, k ) ;
	*/
      }
      /*gHyp_util_logInfo ( "DATE: %u %s",endian.x.sl,gHyp_util_timeStamp(ts));*/
      gHyp_data_append ( pTV, pData ) ;
      break ;

    case FIL__DTSTRING :
      pData = gHyp_data_new ( pFieldName ) ;
      for ( k=0; k<fieldCount; k++ ) {
        pValue = gHyp_data_new ( NULL ) ;
	j = MIN ( fieldSize, VALUE_SIZE ) ;
	memcpy ( value, (const char*) pBuf, j ) ;
	value[j] = '\0' ;

	/* Value may have trailing nulls. Take em out! */
	/*j = strlen ( value ) ;
	value[j] = '\0' ;
	*/

	/* Don't want to do trim, we cannot be sure
	 * the trailing spaces are unimportant, one example
	 * is for constructing database key fields.
	 gHyp_util_trim ( value ) ;
	*/

	gHyp_data_setStr2 ( pValue, value, j ) ;
        gHyp_data_append ( pData, pValue ) ;
        pBuf += fieldSize ;
      }
      /*gHyp_util_logInfo ( "(STR).%s", gHyp_data_print ( pData ) ) ;*/
      gHyp_data_append ( pTV, pData ) ;
      break ;

    case FIL__DTBYTE    :
      pData = gHyp_data_new ( pFieldName ) ;
      gHyp_data_newVector ( pData, TYPE_BYTE, fieldCount, FALSE ) ;
      for ( k=0; k<fieldCount; k++ ) {
        sb = *pBuf++ ; 
        gHyp_data_setVectorRaw ( pData, &sb, k ) ;
      }
      /*gHyp_util_logInfo ( "(BYTE).%s", gHyp_data_print ( pData ) ) ;*/
      gHyp_data_append ( pTV, pData ) ;
      break ;

    case FIL__DTOTHER   :
      pData = gHyp_data_new ( pFieldName ) ;
      /*gHyp_util_logInfo ( "(OTHER).%s", gHyp_data_print ( pData ) ) ;*/
      gHyp_data_append ( pTV, pData ) ;
      break ;
    }

    /* Next field */
    i++ ;
  }
  return i ;
}

static sLOGICAL lHyp_promis_fillFields ( long fileIndex, 
					 long fileId, 
					 long recId,
					 long firstFieldIndex,
					 sField (*field)[MAX_PROMIS_FIELDS],
					 int numFields )
{
  char
    *(*name)[MAX_PROMIS_FIELDS] ;

  char 	fieldName[TOKEN_SIZE+1] ;
  makeDSCs	( fieldName_d, fieldName ) ;

  int
    stat,  
    i ;

  sField
    *pField ;

  long
    fieldIndex,
    fileType,
    fieldType,
    fieldOffset,
    fieldSize,
    fieldCount,
    fieldId ;
			     
  /* If already filled, just return TRUE */
  if ( giNumFields[fileIndex] > 0 ) return TRUE ;

  for ( i=0; i<numFields; i++ ) {

    fieldIndex = i + firstFieldIndex ;
    stat = Fil_Tbl_FieldName ( &fileId, &fieldIndex, &fieldName_d ) ;
    if ( !(stat & 1) ) return FALSE ;

    stat = Fil_Tbl_RecFieldInfo (	&fileId, 
					&recId, 
					&fieldName_d, 
					&fieldType,
     					&fieldOffset, 
					&fieldSize,
     					&fieldCount, 
					&fieldId ) ;
    if ( !(stat & 1) ) return FALSE ;

    fieldName[fieldName_d.dsc_w_length] = '\0' ;
    gHyp_util_trim ( fieldName ) ;
    gHyp_util_lowerCase ( fieldName, strlen ( fieldName ) ) ;
    pField = &(*field)[i] ;
    pField->nameCount = 0 ;
    name = pField->name = (char*(*)[MAX_PROMIS_FIELDS]) AllocMemory(sizeof(char*));
    assert ( name ) ;
    (*name)[0] = (char*) AllocMemory ( strlen( fieldName ) + 1 ) ;
    assert ( (*name)[0] ) ;
    strcpy ( (*name)[0], fieldName ) ;
    pField->type = fieldType ;
    pField->offset = fieldOffset - 1 ;
    pField->size = fieldSize ;
    pField->count = fieldCount ;
    pField->id = fieldId ;
    /*
    gHyp_util_logInfo ( "Field '%s' (%d) Offset=(%d,%d),Size=%d,Count=%d",
    			fieldName, i,
			offset,pField->offset,
		         pField->size,
			 pField->count);
    */
  }
  giNumFields[fileIndex] = numFields ;
  return TRUE ;
}

static int lHyp_promis_tresData ( sInstance *pAI,
				  sData *pDATA, 
				  sData *pTRES, 
				  long fileId,
				  sBYTE *pBuf,
				  sLOGICAL isLittleEndian )
{
  char
    *(*name)[MAX_PROMIS_FIELDS],
    token[TOKEN_SIZE+1],
    value[VALUE_SIZE+1],
    dataType,
    *pFieldName,
    *pToken,
    *pValue ;

  sData
    *pResult,*pLvalue,
    *pTRES_NOTESTDI, 
    *pTRES_NUMDIMS,
    *pTRES_DIMNUMVALUES,
    *pTRES_DI,
    *pTRES_DII,
    *pTRES_TYPE,
    *pTRES_BYTELEN,
    *pTRES_TESTOPNO, 
    *pTRES_COMPIDS,
    *pTRES_COMPCOUNT,
    *pTEST,
    *pTEST_DI,
    *pTEST_DII,
    *pTEST_DII_PROMPT,
    *pTEST_MISCNAMES,
    *pTEST_MISCNMTYPES,
    *pTEST_NUMMISCNAMES ;
    
  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;

  int
    i,j,
    n,
    offset,
    stat,
    byteLen,
    miscIndex,
    compCount,
    numMiscNames,
    numBytes,
    numTestDi,
    numDims,
    numFields,
    numComps,
    numItems,
    numSites ;

  sLOGICAL
    found ;

  sField 
    (*pFields)[MAX_PROMIS_FIELDS],
    *pField ;

  /* Read in the test record */
  pTRES_TESTOPNO = gHyp_data_getChildByName ( pTRES, "testopno" ) ;
  n = gHyp_data_getStr ( pTRES_TESTOPNO, value, VALUE_SIZE, -1, TRUE ) ;
  pValue = value ;
  pTEST = NULL ;
  if ( lHyp_promis_getrec (pAI,pFrame,"test.opno",CMP_EQ,pValue,n) )
    pTEST = gHyp_promis_parseRecord ( pAI, pFrame, "test", pTEST ) ;

  pTRES_NOTESTDI     = gHyp_data_getChildByName ( pTRES, "notestdi" ) ;
  pTRES_NUMDIMS      = gHyp_data_getChildByName ( pTRES, "numdims" ) ;
  pTRES_DIMNUMVALUES = gHyp_data_getChildByName ( pTRES, "dimnumvalues" ) ;

  numTestDi = gHyp_data_getInt ( pTRES_NOTESTDI, -1, TRUE ) ;
  numDims   = gHyp_data_getInt ( pTRES_NUMDIMS, -1, TRUE ) ;

  numFields = numDims + numTestDi - 1 ;

  /* numDims Field 0  Field 1  Field 2  Field 3  ...Field (numTestDi+numDims-1)
   * -----------------------------------------------------------------------
   *    3    compId   siteId   item[0]  item[1]  ...item[numTestDi-1]
   *    2    compId   item[0]  item[1]  item[2]  ...item[numTestDi-1]
   *    1    item[0]  item[1]  item[2]  item[3]  ...item[numTestDi-1]
   */

  if ( numDims > 3 )
    gHyp_instance_error ( pAI, STATUS_PROMIS,
	"No support for 4-dimensioned tests. Please contact support" ) ;

  /* Create the structure descriptor.
   * The first argument is 0, this is the location reserved for the virutal 
   * "data" record.
   * The files ACTL -> WAFF are from 1 through n, and the TBLS files 
   * are put in spots n+1 through m.
   */
  pFields = lHyp_promis_getFields ( 0, numFields ) ;
  giNumFields[0] = numFields ;

  if ( pTEST ) {
    pTEST_DI = gHyp_data_getChildByName ( pTEST, "di" ) ;
    pTEST_MISCNAMES = gHyp_data_getChildByName ( pTEST, "miscnames" ) ;
    pTEST_MISCNMTYPES = gHyp_data_getChildByName ( pTEST, "miscnmtypes" ) ;
    pTEST_NUMMISCNAMES = gHyp_data_getChildByName ( pTEST, "nummiscnames" ) ;
    numMiscNames = gHyp_data_getInt ( pTEST_NUMMISCNAMES, 0, TRUE ) ;	

    pLvalue = gHyp_data_new ( NULL ) ;
    gHyp_data_setReference ( pLvalue, 
			     "miscnmtypes",
	  		     pTEST_MISCNMTYPES ) ;	
    pResult = gHyp_type_assign ( pAI,
				 pFrame,
				 pLvalue,
				 NULL,
				 TYPE_CHAR,
				 FALSE, FALSE  ) ;
    gHyp_data_delete ( pResult ) ;
    gHyp_data_delete ( pLvalue ) ;

  }
  pTRES_DI = gHyp_data_getChildByName ( pTRES, "di" ) ;
  pTRES_COMPIDS = gHyp_data_getChildByName ( pTRES, "compids" ) ;
  pTRES_COMPCOUNT = gHyp_data_getChildByName ( pTRES, "compcount" ) ;
  compCount = gHyp_data_getInt ( pTRES_COMPCOUNT, 0, TRUE ) ;	

  numBytes = 0 ;
  j = numDims - 1 ;
  for ( i=0; i<numTestDi; i++ ) {

   sprintf ( token, "di_%d", i ) ;
   pTRES_DII     = gHyp_data_getChildByName ( pTRES_DI, token ) ;
   pTRES_TYPE    = gHyp_data_getChildByName ( pTRES_DII, "type" ) ;
   pTRES_BYTELEN = gHyp_data_getChildByName ( pTRES_DII, "bytelen" ) ;
   byteLen = gHyp_data_getInt ( pTRES_BYTELEN, 0, TRUE ) ;	
   gHyp_data_getStr ( pTRES_TYPE, value, VALUE_SIZE, -1, TRUE ) ;
   dataType = value[0] ;

   pField = &(*pFields)[j] ;
   pField->nameCount = 0 ;
   j++ ;
   name = pField->name = (char*(*)[MAX_PROMIS_FIELDS]) AllocMemory(sizeof(char*)) ;
   assert ( name ) ;
   if ( pTEST ) {
     pTEST_DII = gHyp_data_getChildByName ( pTEST_DI, token ) ;
     if ( pTEST_DII ) {
       pTEST_DII_PROMPT = gHyp_data_getChildByName ( pTEST_DII, "prompt" ) ;
       n = gHyp_data_getStr (pTEST_DII_PROMPT,value,VALUE_SIZE,-1,TRUE);
       gHyp_util_unparseString(token,value,n,TOKEN_SIZE,FALSE,FALSE,"'");
     }
     else
       sprintf ( token, "item_%d", i ) ;
   }
   else
     sprintf ( token, "item_%d", i ) ;

   (*name)[0] = (char*) AllocMemory ( strlen( token ) + 1 ) ;
   assert ( (*name)[0] ) ;
   strcpy ( (*name)[0], token ) ;

   switch ( dataType ) {
   case 'I' :
     /*pField->type = FIL__DTINTEGER ;  REALLY STORED AS FLOAT */
     pField->type = FIL__DTREAL ;
     break ;
   case 'R' :
     pField->type = FIL__DTREAL ;
     break ;
   case 'D' :
     pField->type = FIL__DTDATE ;
     break ;
   case 'A' :
   default:
     pField->type = FIL__DTSTRING ;
     break ;
   }     
   pField->offset = numBytes ;
   pField->size = byteLen ;
   pField->count = 1 ;
   pField->id = 0 ;
   numBytes += byteLen ;
  }

  if ( numDims == 3 ) {

    /* Three dimension test */
    numComps = gHyp_data_getInt ( pTRES_DIMNUMVALUES, 0, TRUE ) ;
    numSites = gHyp_data_getInt ( pTRES_DIMNUMVALUES, 1, TRUE ) ;
    numItems = gHyp_data_getInt ( pTRES_DIMNUMVALUES, 2, TRUE ) ;

    pField = &(*pFields)[0] ;
    pField->nameCount = numComps ;
    name = pField->name = (char*(*)[MAX_PROMIS_FIELDS]) 
      AllocMemory ( sizeof(char*) * pField->nameCount ) ;
    assert ( name ) ;
    miscIndex = 0 ;
    for ( i=0; i<numComps; i++ ) {

      /* Look for label for next component */
      found = FALSE ;
      if ( pTEST ) {
	while ( !found && miscIndex < numMiscNames ) {
          gHyp_data_getStr ( pTEST_MISCNMTYPES, 
			     value,
			     VALUE_SIZE, 
			     miscIndex,
			     TRUE ) ;
	  if ( value[0] == 'W' ) {
            found = TRUE ; 
 	    gHyp_data_getStr ( pTEST_MISCNAMES,
			       token,
			       TOKEN_SIZE, 
			       miscIndex,
			       TRUE ) ;
          }
          miscIndex++ ;
        }  
      }        	  
      if ( !found && i < compCount ) {
        gHyp_data_getStr ( pTRES_COMPIDS,
			   token, 
			   TOKEN_SIZE, 
			   i,
			   TRUE ) ;
	found = TRUE ;
      }	
      if ( !found ) sprintf ( token, "comp_%d", i ) ;

      /* Put the token for the component 'i' in the name list */
      (*name)[i] = (char*) AllocMemory ( strlen( token ) + 1 ) ;
      assert ( (*name)[i] ) ;
      strcpy ( (*name)[i], token ) ;
    }
    pField->type = FIL__DTSTRUCT ;
    pField->offset = 0 ;
    pField->size = numBytes ;
    pField->count = numComps ;
    pField->id = 0 ;

    pField = &(*pFields)[1] ;
    pField->nameCount = numSites ;
    name = pField->name = (char*(*)[MAX_PROMIS_FIELDS]) 
      AllocMemory (sizeof(char*) * pField->nameCount ) ;
    assert ( name ) ;
    miscIndex = 0 ;
    for ( i=0; i<numSites; i++ ) {

      /* Look for label for next component */
      found = FALSE ;
      if ( pTEST ) {
	while ( !found && miscIndex < numMiscNames ) {
          gHyp_data_getStr ( pTEST_MISCNMTYPES, 
			     value,
			     VALUE_SIZE, 
			     miscIndex,
			     TRUE ) ;
	  if ( value[0] == 'S' ) {
            found = TRUE ;
	    gHyp_data_getStr ( pTEST_MISCNAMES,
			       token,
			       TOKEN_SIZE, 
			       miscIndex,
			       TRUE ) ;
	  }
	  miscIndex++ ;
        }
      }        	  
      if ( !found ) sprintf ( token, "site_%d", i ) ;

      /* Put the token for the component 'i' in the name list */
      (*name)[i] = (char*) AllocMemory ( strlen( token ) + 1 ) ;
      assert ( (*name)[i] ) ;
      strcpy ( (*name)[i], token ) ;
    }
    pField->type = FIL__DTSTRUCT ;
    pField->offset = 0 ;
    pField->size = numBytes ;
    pField->count = numSites ;
    pField->id = 0 ;
  }
  else if ( numDims == 2 ) {

    /* Two dimension test */
    numComps = gHyp_data_getInt ( pTRES_DIMNUMVALUES, 0, TRUE ) ;
    numItems = gHyp_data_getInt ( pTRES_DIMNUMVALUES, 1, TRUE ) ;

    pField = &(*pFields)[0] ;
    pField->nameCount = numComps ;
    name = pField->name = (char*(*)[MAX_PROMIS_FIELDS]) 
      AllocMemory (sizeof(char*) * pField->nameCount);
    assert ( name ) ;
    miscIndex = 0 ;
    for ( i=0; i<numComps; i++ ) {

      /* Look for label for next component */
      found = FALSE ;
      if ( pTEST ) {
	while ( !found && miscIndex < numMiscNames ) {
          gHyp_data_getStr ( pTEST_MISCNMTYPES, 
			     value,
			     VALUE_SIZE, 
			     miscIndex,
			     TRUE ) ;
	  if ( value[0] == 'W' ) {
            found = TRUE ; 
	     gHyp_data_getStr ( pTEST_MISCNAMES,
				token,
				TOKEN_SIZE, 
				miscIndex,
				TRUE ) ;
	  }
	  miscIndex++ ;
        }
      }        	  
      if ( !found && i < compCount ) {
        gHyp_data_getStr ( pTRES_COMPIDS,
			   token, 
			   TOKEN_SIZE, 
			   i,
			   TRUE ) ;
	found = TRUE ;
      }
      if ( !found ) sprintf ( token, "comp_%d", i ) ;

      (*name)[i] = (char*) AllocMemory ( strlen( token ) + 1 ) ;
      assert ( (*name)[i] ) ;
      strcpy ( (*name)[i], token ) ;
    }
    pField->type = FIL__DTSTRUCT ;
    pField->offset = 0 ;
    pField->size = numBytes ;
    pField->count = numComps ;
    pField->id = 0 ;

  }
  else {

    /* One dimension test */
    numItems = gHyp_data_getInt ( pTRES_DIMNUMVALUES, 0, TRUE ) ;
  }

  offset = 0 ;
  /*gHyp_util_debug("Parsing TRES");*/
  stat = lHyp_promis_parseRecord (	pDATA,
					fileId,
					pBuf,
					pFields,
					numFields,
					offset,
					isLittleEndian ) ;
  
  /* Since the fields for the virtual DATA file always change, 
   * it cannot be cached 
   */
  gHyp_promis_cleanFields ( 0 ) ;

  return stat ;
}

static sLOGICAL lHyp_promis_addTRES ( 	long fileId, 
					sData *pTRES, 
					char *fieldName,
					sBYTE *pBuf, 
					sInstance *pAI,
					sFrame *pFrame,
					sLOGICAL isLittleEndian,
					sLOGICAL isTRfile )
{
  long
    stat,
    fieldIndex,
    recId,
    fieldType,
    fieldOffset,
    fieldSize,
    fieldCount,
    fieldId ;
  
  sData
    *pRecord,
    *pDATA,
    *pValue ;
          
  makeDSCz	( fieldName_d, fieldName ) ;
   
  /* Get the field 'TRESARR' */
  recId = 0 ;
  stat = Fil_Tbl_RecFieldInfo (	&fileId, 
				&recId, 
				&fieldName_d, 
				&fieldType,
				&fieldOffset, 
				&fieldSize,
				&fieldCount, 
				&fieldId ) ;
  if ( !(stat & 1) ) return FALSE ;

  /* Make a sub-variable 'data' */
  if ( isTRfile )
    pDATA = gHyp_frame_createVariable ( pFrame, "$data" ) ;
  else
    pDATA = gHyp_frame_createVariable ( pFrame, "data" ) ;

  gHyp_data_deleteValues ( pDATA ) ;
  gHyp_data_setHashed ( pDATA, TRUE ) ;

  /* Add the name of it to the end of the pTRES data structure */
  pRecord = gHyp_data_new ( "record" ) ;
  pValue = gHyp_data_new ( NULL ) ;
  if ( isTRfile )
    gHyp_data_setStr ( pValue, "$data" ) ;
  else
    gHyp_data_setStr ( pValue, "data" ) ;
  gHyp_data_append ( pRecord, pValue ) ;
  gHyp_data_append ( pTRES, pRecord ) ;

  /* Add content to pTRES, starting from record buffer pBuf+fieldOffset-1 */
  stat = lHyp_promis_tresData ( pAI, 
				pDATA,
				pTRES,
				fileId,
				pBuf+fieldOffset-1,
					isLittleEndian ) ;
  if ( stat <= 0 ) return FALSE ;

  return TRUE ;
}

static sLOGICAL lHyp_promis_addTBLS ( long fileId, 
				      sData *pTBLS, 
				      char *fieldName,
				      sBYTE *pBuf, 
				      sInstance *pAI,
				      sFrame *pFrame,
				      sLOGICAL isLittleEndian )
     
{
  sData
    *pTABLE,
    *pTBLS_TABLENAME,
    *pRecord,
    *pValue ;

  char
    *pStr,
    tableName[4+1] ;

  sLOGICAL
    found ;
	
  long
    recId,
    numFields,
    firstFieldId,
    parentField,
    idFields[8],
    stat,
    fileType,
    fieldType,
    fieldOffset,
    fieldSize,
    fieldCount,
    fileIndex,
    fieldId ;

  int 
    offset,
    i ;

  union {
    long id ;
    char name[5] ;
  } rec ;

  sField 
    (*pFields)[MAX_PROMIS_FIELDS], /* Pointer to an array of sField(s) */
    *pField ;

  makeDSCz	( fieldName_d, fieldName ) ;

  /* Get the field 'BYTEBUFFER' */
  recId = 0 ;
  stat = Fil_Tbl_RecFieldInfo (	&fileId, 
						&recId, 
						&fieldName_d, 
						&fieldType,
     						&fieldOffset, 
						&fieldSize,
     						&fieldCount, 
						&fieldId ) ;
  if ( !(stat & 1) ) return FALSE ;

  /* What table is it? */
  pTBLS_TABLENAME = gHyp_data_getChildByName ( pTBLS, "tablename" ) ;
  gHyp_data_getStr ( pTBLS_TABLENAME, tableName, 4, -1, TRUE ) ;

  /* Find the table Id value */
  found = FALSE ;
  strcpy ( rec.name, tableName ) ;
  for ( i=0; i<giNumTables; i++ ) {
    if ( rec.id == gzTableNames[i] ) {
      found = TRUE ;
      recId = i+1 ;
      break ;
    }      
  }      
  if ( !found ) return FALSE ;

  /* Initialize the file variable */
  gHyp_util_lowerCase ( tableName, 4 ) ;
  pTABLE = gHyp_frame_createVariable ( pFrame, tableName ) ;
  gHyp_data_deleteValues ( pTABLE ) ;
  gHyp_data_setHashed ( pTABLE, TRUE ) ;

  /* Add the name of it to the end of the pTBLS data structure */
  pRecord = gHyp_data_new ( "record" ) ;
  pValue = gHyp_data_new ( NULL ) ;
  gHyp_data_setStr ( pValue, tableName ) ;
  gHyp_data_append ( pRecord, pValue ) ;
  gHyp_data_append ( pTBLS, pRecord ) ;

  /* Set the fileIndex of tables offset to the main PROMIS files */
  fileIndex = MAX_PROMIS_FILES + recId ;
  stat = Fil_Tbl_RecordInfo(	&fileId,			   
			        &recId,
			       	&numFields,
			       	&firstFieldId,
			       	&parentField,
			       	&idFields[0],
			       	&fileType ) ;
  if ( !(stat & 1) ) return FALSE ;

  /* The fileIndex for the TBLS table is offset above those reserved for ACTL->WAFF */
  pFields = lHyp_promis_getFields ( fileIndex, numFields ) ;
  if ( !lHyp_promis_fillFields (	fileIndex, 
				       	fileId, 
				       	recId,	
				       	firstFieldId,	
				       	pFields, 
				       	numFields ) ) return FALSE ;
  offset = 0 ;
  /*gHyp_util_debug("Parsing TBLS");*/
  numFields = lHyp_promis_parseRecord (	pTABLE, 
							fileId,
					  		pBuf,
					  		pFields,
				  			numFields,
 				  			offset,
				  			isLittleEndian ) ;
  if ( numFields > 0 ) 
    return TRUE ;
  else
    return FALSE ;
}

static sLOGICAL lHyp_promis_addTLOG (	long fileId, 
							sData *pTLOG,
							char *fieldName,
							sBYTE *pBuf,
							sInstance *pAI,
							sFrame *pFrame,
							sLOGICAL isLittleEndian )
{
  sData
    *pFILE,
    *pTLOG_RECID,
    *pTLOG_FLAGS,
    *pTLOG_LENGTH,
    *pRecord,
    *pValue ;

  char
    *pStr,
    recid[1+1],
    tableName[4+1] ;
   
  sBYTE
    flags ;
  
  sLOGICAL
    isTRES=FALSE,
    isTBLS=FALSE ;

  long
    stat,
    recId,
    length,
    bufSize,
    numFields,
    fileType,
    firstFieldId,
    fieldType,
    fieldOffset,
    fieldSize,
    fieldCount,
    fileIndex,
    fieldId ;

  int
    offset ;
  
  sField 
    (*pFields)[MAX_PROMIS_FIELDS], /* Pointer to an array of sField(s) */
    *pField ;

  union {
    long id ;
    char name[5] ;
  } rec ;

  makeDSCz	( fieldName_d, fieldName ) ;

  char fileName[TOKEN_SIZE+1] ;
  makeDSCs ( fileName_d, fileName ) ;

  sBYTE *pNewBuf ;

  pTLOG_RECID = gHyp_data_getChildByName ( pTLOG, "recid" ) ;
  gHyp_data_getStr ( pTLOG_RECID, recid, 1, -1, TRUE ) ;
  pTLOG_FLAGS = gHyp_data_getChildByName ( pTLOG, "flags" ) ;
  flags = gHyp_data_getInt ( pTLOG_FLAGS, 0, TRUE ) ;
  pTLOG_LENGTH = gHyp_data_getChildByName ( pTLOG, "length" ) ;
  length = gHyp_data_getInt ( pTLOG_LENGTH, 0, TRUE ) ;

  /* Process file records in 
   *  "Put",
   *  "Update",
   *  "Journal",
   *  "Delete" records where TLOG_M_JOURNAL = 4 (Delete record contains a before image)
   */
  if ( recid[0] != 'P' && 
       recid[0] != 'U' &&
       recid[0] != 'J' &&
       (recid[0] != 'D' || !(flags & 4) ) ) return TRUE ;

  /* Get the field 'BUFFERA' */
  recId = 0 ;
  stat = Fil_Tbl_RecFieldInfo (	&fileId, 
				&recId, 
				&fieldName_d, 
				&fieldType,
     				&fieldOffset, 
				&fieldSize,
     				&fieldCount, 
				&fieldId ) ;
  if ( !(stat & 1) ) return FALSE ;

  strncpy ( tableName, (const char*) pBuf+fieldOffset-1, 4 ) ; 
  tableName[4] = '\0' ;

  /* Find the PROMIS file name info */
  strcpy ( rec.name, tableName ) ;
  recId = 0 ;
  stat = Fil_Tbl_FileIndex ( &rec.id, &fileIndex ) ; 
  if ( !(stat & 1) ) return FALSE ;
  stat = Fil_Tbl_FileInfo(	&rec.id,	   
			     	0,
			     	&fileType,
			     	&bufSize,
			     	&numFields,
			     	&firstFieldId ) ;
  if ( !(stat & 1) ) return FALSE ;

  /* Save the filename */
  strcpy ( fileName, tableName ) ;
  fileName_d.dsc_w_length = 4 ;

  /* Add the name of the file to the end of the pTLOG data structure */
  pRecord = gHyp_data_new ( "record" ) ;
  pValue = gHyp_data_new ( NULL ) ;
  gHyp_util_lowerCase ( tableName, 4 ) ;
  gHyp_data_setStr ( pValue, tableName ) ;
  gHyp_data_append ( pRecord, pValue ) ;
  gHyp_data_append ( pTLOG, pRecord ) ;

  /* Get the beginning of the record */
  fieldOffset += 4 ;  /* Skip past filename */
  if ( recid[0] != 'P' ) fieldOffset += 92 ; /* Skip past maxkeysize */

  /* Unpack the intername record */
  length = length - fieldOffset + 1; 
  stat = aeqSsp_automan_unpack ( &fileId, &rec.id, &length, 
				 pBuf+fieldOffset-1 ) ;
  if ( !(stat & 1) ) return FALSE ;

  return TRUE ;
}

sLOGICAL gHyp_promis_hs (	sDescr*		token_d,
				long		tokenOffset,
				long		tokenLen,
				sLOGICAL*	pIsHSenabled,
    				sDescr*		result_d,
				unsigned long*	pResultLen )
{
 /* The PROMIS tut_gettoken or tut_inquire routine get tokens and text lines 
   * respectively from the PROMIS input stream; originating from a keyboard, 
   * script file, other other input source. After the token/line is processed
   * by tut_gettoken/tut_inquire, but before the result is returned back to 
   * the calling PROMIS function, the processed result is passed to this
   * routine for further processing.
   *
   * If the token or line is the special "##" sequence, then HyperScript 
   * will be toggled between an enabled and disabled state. While disabled, 
   * all tokens/lines are ignored by this routine.  HyperScript is initially
   * disabled; so when the first "##" is received a new HyperScript program
   * is initialized in dynamic memory.
   *
   * Whenever HyperScript goes from a disabled to enabled state, 
   * the token/line 
   * following the "##" sequence is consumed by this routine as HyperScript 
   * program statements and are appended to the end of the program.  The 
   * token/line is not returned back to tut_gettoken/tut_inquire, which forces 
   * tut_gettoken/tut_inquire to re-prompt for the input.  This re-prompting
   * can be repeated over and over, allowing an entire HyperScript program to
   * be loaded between "##" sequences without moving from the original PROMIS
   * function which called tut_gettoken/tut_inquire.
   *
   * While a HyperScript program is being loaded and parsed, it can either be
   * executed immediately or executed later.  Immediate execution is the 
   * default; as internal HyperScript expressions are formed, they are also
   * executed.  Delayed executing means that HyperScript is loaded and parsed,
   * but not executed. This is a method definition.
   * Immediate mode execution is restored when the method's definition is
   * ended. 
   *
   * When a HyperScript program needs to supply tokens/lines back to the PROMIS
   * application without using a "##" on/off sequence, it can use the 'pexec' 
   * function.  The pexec() function puts its argument string in a special 
   * result buffer, which is returned to PROMIS and inserted into the token
   * stream.  This token stream is subsequently passed back to this routine,
   * but the tokens will be ignored by HyperScript.
   *
   */
  static int		isOPTION_HYPERSCRIPT = 0 ;
  static char		stream[MAX_INPUT_LENGTH+1] ;
  static char		terminator ;
  static char*		pStream ;
  static char*		pEndStream ;
  static int		lineCount = 0 ;
  static int		status ;
  static struct {
    int	link ;
    void	(*handler)(int status) ;
    int	argCount;
    int*	status ;
  } desblk ;

  char
    *pResult ;

  sData
    *pAIdata,
    *pData ;

  char
    result[VALUE_SIZE+1] ;

  int
    i,
    stat,
    resultLen = *pResultLen ;

  sStack
    *pStack ;

  sHyp
    *pHyp ;

  /* Initialize return values. */
  *pResultLen = 0 ;
  if ( !gpsConcept ) {
    giIsHSenabled = FALSE ;
    giIsCallbackEnabled = FALSE ;
  }

  *pIsHSenabled = giIsHSenabled ;

  /* Check for special ## sequence that switches Hyperscript on and off */
  
  if (	( tokenLen == 2 || tokenLen == 3 ) && 
	strncmp (	token_d->dsc_a_pointer+tokenOffset, 
			"## ", 
			tokenLen ) == 0 ) {

    /* '##' or '## 'was the passed token. */

    if ( gpsConcept )

      /* HyperScript instance already exists. 
       * Toggle HyperScript on or off, depending on its current state
       * The logic of this code assumes that isOPTION_HYPERSCRIPT == 1
       */ 	  

      *pIsHSenabled = giIsHSenabled = !giIsHSenabled ;

    else {

      /* HyperScript instance does not exist. 
       * Create new instance (enabled, in idle mode).
       */

      if ( isOPTION_HYPERSCRIPT == 0 ) {

	/* First time running - check OPTION_HYPERSCRIPT */

	/*
        if ( ( pResult = getenv ( "OPTION_HYPERSCRIPT" ) ) != NULL ) {

	  gHyp_util_upperCase ( pResult, strlen ( pResult ) ) ;

          if ( strcmp ( pResult, "YES" ) == 0 )  
	    isOPTION_HYPERSCRIPT = 1 ;
	  else
	    isOPTION_HYPERSCRIPT = - 1 ;
	}
	*/
	if ( uaf_info_hyperscript() )
	  isOPTION_HYPERSCRIPT = 1 ;
	else
	  isOPTION_HYPERSCRIPT = - 1 ;
      }	

      if ( isOPTION_HYPERSCRIPT == 1 ) {	

        char self[TARGET_SIZE+1] ;

        /* Expire on Jan 15, 1995 */
	/*
        if ( gut_curdate() > -125000000 ) {
          isOPTION_HYPERSCRIPT == -1 ;
	  return gHyp_util_logInfo ( "HyperScript demo version has expired" ) ;
	}
	*/

        gHyp_util_getprocessname( self) ;
        gHyp_util_strip ( self,  strlen ( self )  ) ;
        gpsConcept = gHyp_concept_new () ;
	guRunFlags = ( RUN_INTERACTIVE | RUN_PROMIS ) ;
	if ( !gHyp_concept_init ( gpsConcept, 
				  self,
				  "",
				  0,
				  MAX_EXPRESSION )) return 1 ;
        gpAI = gpAImain = gHyp_concept_getConceptInstance ( gpsConcept ) ;
        aeqSsp_automan_initDbFiles () ;
        aeqSsp_automan_initTrFiles () ;
	gHyp_promis_initFields();
        guTimeStamp = FALSE ;
	gsLog = stdout ;
	gzLogName[0] = '\0' ;
        lineCount = 0 ;
        *pIsHSenabled = giIsHSenabled = TRUE ;	
        stream[0] = '\0' ;
        pStream = stream ;
        gzResult[0] = '\0' ;
        pEndStream = stream + MAX_INPUT_LENGTH ;

#ifdef AS_VMS
        /* Because there's not main(), this is required */
        VAXC$CRTL_INIT () ;
	/* VMS says this handler prevents bad behavior from longjmp */
	VAXC$ESTABLISH ( &lHyp_promis_exceptionHandler ) ;
#endif
	/* Setup an exit handler */
        desblk.handler = gHyp_promis_exitHandler ;
        desblk.argCount = 1 ;
        desblk.status = &status ;
        status = sys$dclexh ( &desblk ) ;
        if ( !gHyp_util_check ( status, 1 ) )
	  gHyp_util_logWarning ( 
	    "Failed to declare HyperScript exit handler") ;
      } 
    } 

    if ( isOPTION_HYPERSCRIPT == 1 )
      /* Return to PROMIS. It will not process the "##" */
      return TRUE ;
    else
     /* Return to PROMIS. It will process the "##" like any other token. */
     return FALSE ;
  }

  /* If there's no HyperScript instance, or it's not enabled, then
   * we will not process the token for HyperScript. 
   */
  if ( !gpsConcept || !giIsHSenabled ) return FALSE ;

  if ( tokenOffset == 0 && tokenLen > 0 ) {

    /* The token begins a new line */
    
    /* Check for EXIT or QUIT */	
    if ( ( tokenLen == 4 || tokenLen == 5 ) &&
	   ( ( strncmp ( token_d->dsc_a_pointer, 
			"EXIT ", 
			tokenLen ) == 0 ) || 
	     ( strncmp ( token_d->dsc_a_pointer, 
			"QUIT ", 
			tokenLen ) == 0 ) ) ) {
      aeqSsp_autoMan_closeFiles ( ) ;
      gHyp_promis_cleanFields ( -1 ) ;
      gHyp_concept_quit ( gpsConcept ) ;
      gpsConcept = NULL ; 
      *pIsHSenabled = giIsHSenabled = FALSE ;
      return FALSE ;
    }

    /* Check if the last line still had pending tokens. */
    if ( pStream > stream ) {
      if ( terminator == '"' )
        gHyp_util_logWarning ( 
	"Nonterminated string constant on line %d", lineCount ) ;
      else	
        gHyp_util_logWarning ( 
	"Nonterminated 'identifier' on line %d", lineCount ) ;

      *pStream++ = terminator ;
      *pStream = '\0' ;
    }


    lineCount++ ;
  }

  if ( tokenLen > 0 ) {

    /* Add token to HyperScript's input buffer. */
    strncpy ( pStream, token_d->dsc_a_pointer+tokenOffset, tokenLen ) ;
    pStream += tokenLen ;
    *pStream = '\0' ;
  }

  /* Try loading if there's anything there */
  if ( stream[0] ) {

    /* Load from stream. */
    pHyp = gHyp_frame_getHyp ( gHyp_instance_frame ( gpAI ) ) ;
    pStream = gHyp_load_fromStream ( gpAI, pHyp, stream, lineCount ) ;
 
    /* If load was fatal, (-1) then quit */
    if ( pStream == NULL ) { 

      aeqSsp_autoMan_closeFiles ( ) ;
      gHyp_promis_cleanFields ( -1 ) ;
      gHyp_concept_quit ( gpsConcept ) ;
      gpsConcept = NULL ; 
      *pIsHSenabled = giIsHSenabled = FALSE ;
      return FALSE ;
    }
  }

  if ( *pStream ) {

    /* Some of the stream was not loaded because of an unterminated literal
     * or identifier.
     * Don't process the stream yet. */
    terminator = *pStream ;	
    
    /* Shift the part that wasn't loaded to the front of the buffer */
    strcpy ( stream, pStream ) ;
    pStream = stream + strlen ( stream ) ;
  }
  else {
    /* There is no outstanding stream to load. */
    pStream = stream ;
    stream[0] = '\0' ;
  }
   
  /* Check for pexec() result */
  if ( resultLen != 0 ) {

    strncpy ( result, result_d->dsc_a_pointer, resultLen ) ;
    result[resultLen] = '\0' ;
    pStack = gHyp_frame_stack ( gHyp_instance_frame ( gpAI ) ) ;
    pData = gHyp_stack_pop ( pStack ) ;
    gHyp_data_setStr ( pData, result ) ;
    gHyp_stack_push ( pStack, pData ) ;

  }

  /* Disable the secret TUT callback function */
  if ( giIsCallbackEnabled ) aeqSsp_automan_disableCallback() ;

    /* Execute HyperScript. */
  pAIdata = gHyp_concept_nextInstanceData ( gpsConcept ) ;
  if ( pAIdata ) {

    gpAI = (sInstance*) gHyp_data_getObject ( pAIdata ) ;

    while ( (stat=gHyp_instance_run(gpAI)) > COND_FATAL ) {

      if ( guSIGINT || guSIGTERM ) { stat = COND_FATAL ; break ; }
        
      if ( stat == COND_ERROR ) {

        /* The instance has an error */

        /* Un-caught errors in the main instance begets them all */
        if ( gpAI == gpAImain ) break ;

        /* Otherwise a instance can die and not affect the other instances */
        gHyp_data_detach ( pAIdata ) ;
        gHyp_data_delete ( pAIdata ) ;

      }
      else {

        /* COND_NORMAL or COND_SILENT */
      
        /* Check for a result to send back */
        if ( (pResult = lHyp_promis_getResult () ) ) { 
      
          /* There's a result to send back */
          /*gHyp_util_debug("RESULT is %s",pResult);*/
          *pResultLen = MIN ( result_d->dsc_w_length, strlen ( pResult ) ) ;
          strncpy ( result_d->dsc_a_pointer, pResult, *pResultLen ) ;
          gzResult[0] = '\0' ;
	  gHyp_instance_setState ( gpAI, STATE_PARSE ) ;
          break ;
	}

	/* Check to see if we are going back for more input */
        if ( gpAI == gpAImain &&
	     gHyp_concept_returnToStdIn ( gpsConcept ) &&
	     (gHyp_instance_getState ( gpAImain ) != STATE_QUERY &&
	      gHyp_instance_getState ( gpAImain ) != STATE_SLEEP) ) {
      
          /* HyperScript finished running */
          giJmpLevel = -1 ;	/* Disallow longjmp */
          giJmpEnabled = FALSE ;
	  gHyp_instance_setState ( gpAI, STATE_PARSE ) ;
          break ;
	}
        else {
          /*gHyp_util_debug("Next instance");*/
          pAIdata = gHyp_concept_nextInstanceData ( gpsConcept ) ;
          if ( pAIdata ) {
            gpAI = (sInstance*) gHyp_data_getObject ( pAIdata ) ;    
	  }
	  else {
	    stat = COND_FATAL ;
	    break ;
	  }
	}
      }
    } 	
  }
  else
    stat = COND_FATAL ;

  if ( stat < 0 ) {
    
    /* Death to instance */
    aeqSsp_autoMan_closeFiles ( ) ;
    gHyp_promis_cleanFields ( -1 ) ; 
    gHyp_concept_quit ( gpsConcept ) ;
    gpsConcept = NULL ;
    *pIsHSenabled = giIsHSenabled = FALSE ;
    return FALSE ;
  }  
  return TRUE ;
}

sData* gHyp_promis_getSymbol (	sInstance	*pAI,
				sFrame 		*pFrame, 
				char 		*pSymbol,
				sData		*pVariable ) 
{
  int		
    status,
    symbolLen = strlen ( pSymbol ),
    statusTextLen,
    resultLen ;
		
  sData
    *pResult ;

  makeDSCz	( symbol_d, pSymbol ) ;

  char
    statusText[MAX_OUTPUT_LENGTH+1] ;

  makeDSCs	( statusText_d, statusText );

  char
    result[VALUE_SIZE+1] ;

  makeDSCs	( result_d, result );

  /* Format is '%x' */
  if ( symbolLen < 2 || *pSymbol != '%' ) return NULL ;

  status = aeqSsp_autoMan_getSymbol (	&symbol_d,
     			        	&result_d,
	     				&resultLen,
     					&statusText_d,
     					&statusTextLen ) ;
  if ( !(status & 1) ) return NULL ;

  if ( !pVariable ) pVariable = gHyp_frame_createVariable ( pFrame, pSymbol ) ;
  gHyp_data_deleteValues ( pVariable ) ;
  pResult = gHyp_data_new ( NULL ) ;
  result[resultLen] = '\0' ;
  /*gHyp_data_setToken ( pResult, result ) ;*/
  gHyp_data_setStr ( pResult, result ) ;
  gHyp_data_append ( pVariable, pResult ) ;
  return pVariable ;
}

sData* gHyp_promis_getParm (	sInstance	*pAI,
				sFrame 		*pFrame,
				char 		*pParm,
				sData		*pVariable ) 
{
  int		
    status,
    parmLen = strlen ( pParm ),
    statusTextLen,
    resultLen ;
  
  sData
    *pResult ;

  makeDSCz	( parm_d, pParm ) ;

  char
    statusText[MAX_OUTPUT_LENGTH+1] ;

  makeDSCs	( statusText_d, statusText );

  char
    result[VALUE_SIZE+1] ;

  makeDSCs	( result_d, result );

  /* Format is '$x' */
  if ( parmLen < 2 || *pParm != '$' ) return NULL ;
  
  status = aeqSsp_autoMan_getParameter (	&parm_d,
     			        		&result_d,
	     					&resultLen,
     						&statusText_d,
     						&statusTextLen ) ;
  if ( !(status & 1) ) return NULL ;

  if ( !pVariable ) pVariable = gHyp_frame_createVariable ( pFrame, pParm ) ;
  gHyp_data_deleteValues ( pVariable ) ;
  pResult = gHyp_data_new ( NULL ) ;
  result[resultLen] = '\0' ;
  /*gHyp_data_setToken ( pResult, result ) ;*/
  gHyp_data_setStr ( pResult, result ) ;
  gHyp_data_append ( pVariable, pResult ) ;
  return pVariable ;
}

void gHyp_promis_pget ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: pget(key,comparision,value)
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
      *pData;
    
    char
      key[VALUE_SIZE+1],
      cmp[VALUE_SIZE+1],
      value[VALUE_SIZE+1],
      *pKey,
      *pCmp,
      *pValue ;
		
    int
      n,
      comparison,
      valueLen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( argCount != 3 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: pget ( key, comparison, value )");

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    valueLen = gHyp_data_getStr ( pData, 
				  value, 
				  VALUE_SIZE, 
				  gHyp_data_getSubScript(pData),
				  TRUE ) ;
    pValue = value ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   cmp, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pCmp = cmp ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   key, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ; 
    pKey = key ;

    if ( valueLen == 0 )
      gHyp_instance_error ( 	pAI, STATUS_PROMIS, 
      				"PROMIS key 'value' cannot be null" ) ;
    
    /* HARD-CODIND HERE!! - PROMIS CMP_EQ, CMP_GE, CMP_GT */
    comparison = ( strlen ( pCmp ) != 2 ) ? 0 :
    		 ( strcmp ( pCmp, "eq" ) == 0 ) ? CMP_EQ :
		 ( strcmp ( pCmp, "ge" ) == 0 ) ? CMP_GE :
    	 	 ( strcmp ( pCmp, "gt" ) == 0 ) ? CMP_GT : 0 ;

    if ( comparison == 0 )
      gHyp_instance_error ( 	pAI, STATUS_PROMIS, 
      				"Comparison must be 'eq', 'ge', or 'gt'" ) ;
    
    if ( strlen ( pValue ) == 0 )
      gHyp_instance_error ( 	pAI, STATUS_PROMIS, 
      				"PROMIS file 'key' cannot be null" ) ;
    
    lHyp_promis_getrec (pAI,
			pFrame, 
    			pKey, 
			comparison, 
			pValue,
			valueLen ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }	
}

void gHyp_promis_pnext ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: pnext ( key )
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
      *pResult ;
    
    char
      key[VALUE_SIZE+1],
      *pKey ;
    
    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: pnext ( key )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   key, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ; 
    pKey = key ;
    if ( n == 0 )
      gHyp_instance_error ( 	pAI, STATUS_PROMIS, 
      				"PROMIS key cannot be null" ) ;
        
    pResult = gHyp_data_new ( NULL ) ;
    lHyp_promis_getnext ( pAI, pFrame, pKey ) ;
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }	
}

void gHyp_promis_popen ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: popen ( file )
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
      *pData ;
    
    char
      file[VALUE_SIZE+1],
      *pFile ;
    
    int
      n,
      i,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: popen ( filename )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   file, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ; 
    pFile = file ;
    if ( n != 4 )
      gHyp_instance_error ( 	pAI, STATUS_PROMIS, 
      				"PROMIS file name must be 4 characters" ) ;
        
    gHyp_util_upperCase ( file, 4 ) ;
    lHyp_promis_openFile ( pAI, pFrame, pFile ) ;
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }	
}

void gHyp_promis_pclose ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: pclose ( file )
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
      *pData ;
    
    char
      file[VALUE_SIZE+1],
      *pFile ;
    
    int
      n,
      i,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: pclose ( filename )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   file, 
			   VALUE_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ; 
    pFile = file ;
    if ( n != 4 )
      gHyp_instance_error ( 	pAI, STATUS_PROMIS, 
      				"PROMIS file name must be 4 characters" ) ;
        
    gHyp_util_upperCase ( file, 4 ) ;
    lHyp_promis_closeFile ( pAI, pFrame, pFile );
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }	
}

void gHyp_promis_pexec ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: pexec ( commandString )
   *	The last line of stdout returned is the return value.
   *	If no output, the return value is 0.
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
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData,
      *pResult;
    
    char
      *pStr,
      *pMsg,
      *pEndMsg,
      value[VALUE_SIZE+1],
      /* Symbuffer in PROMIS is 80 characters */
      message[MAX_PROMIS_RESULT_SIZE+1] ; 
    
    int
      ss,
      context,
      strLen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sLOGICAL
      isVector ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: pexec ( text )" ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    
    /* Construct the message to be sent to PROMIS */
    pMsg = message ;
    pEndMsg = pMsg + MAX_PROMIS_RESULT_SIZE ;
    pResult = NULL ;
    isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;
    ss = gHyp_data_getSubScript ( pData ) ; context = -1 ;
    while ( (pResult = gHyp_data_nextValue ( pData, 
					     pResult,
					     &context,
					     ss ))) {

      strLen = gHyp_data_getStr ( pResult, 
				  value, 
				  VALUE_SIZE, 
				  context, 
				  isVector ) ;
      pStr = value ;
      if ( ( pMsg + strLen ) > pEndMsg-1 ) {
	gHyp_instance_warning ( pAI, STATUS_PROMIS,
			      "pexec statement longer than %d characters \n",
			      MAX_PROMIS_RESULT_SIZE ) ;
	break ;
      }
      strcpy ( pMsg, pStr ) ;
      pMsg += strLen ;
    }
    if ( context== -2 && ss != -1 ) 
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds in pexec()",ss) ;
    
    *pMsg = '\0' ;
    pResult = gHyp_data_new ( NULL ) ;
    
    /* Initialize return result */
    gHyp_data_setToken ( pResult, "0" ) ;
    gHyp_stack_push ( pStack, pResult ) ;

    /* Initialize output area */
    gpData = gHyp_frame_createVariable ( pFrame, "_promis_data_" ) ;
    gHyp_data_deleteValues ( gpData ) ;
  
    /* Call the secret TUT callback function */
    if ( giIsCallbackEnabled ) aeqSsp_automan_enableCallback() ;

    /* Invalidate the tracking buffers */
    aeqSsp_automan_initTrFiles() ;

    /* To execute PROMIS, we store the command string in a special result
     * area, then longjmp out of the HyperScript program, back to PROMIS
     * to execute the command string. When PROMIS returns to HyperScript,
     * it will resume execution where it left off before the longjmp.
     * In other words, we have to set the hypIndex to one less than what
     * it is now.
     */
    lHyp_promis_setResult ( message ) ;
    gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;
    longjmp ( gsJmpStack[giJmpLevel=1], COND_NORMAL ) ;
  }
}

void gHyp_promis_putline (  sDescr* output_d,
			    long outputLen )  
{
  sData
    *pValue = gHyp_data_new ( NULL ) ;

  if ( gpData ) {
    gHyp_data_setStr2 ( pValue, output_d->dsc_a_pointer, outputLen ) ;
    gHyp_data_append ( gpData, pValue ) ;
  }

}

void gHyp_promis_psnap ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: psnap ( )
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

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    sData	*pResult ;
    int	  	argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: psnap()" ) ;

    pResult = lHyp_promis_snap ( pAI, pFrame ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }	
}


void gHyp_promis_pcapture ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    sData	*pData ;
    int	  	argCount = gHyp_parse_argCount ( pParse ) ;
    sLOGICAL isCallbackEnabled = TRUE ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: pcapture( boolean )" ) ;

    if ( argCount == 1 ) {
      /* Pop the value off of the stack	 */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      if ( gHyp_data_getBool ( pData,
			       gHyp_data_getSubScript ( pData ),
			       TRUE )  )
	isCallbackEnabled = TRUE ;
      else
	isCallbackEnabled = FALSE ;
    }
    else
      isCallbackEnabled = TRUE ;

    giIsCallbackEnabled = isCallbackEnabled ;

    if ( giIsCallbackEnabled ) aeqSsp_automan_disableCallback() ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }	
}

void gHyp_promis_plogopen ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    sData	*pData ;
    int	  	argCount = gHyp_parse_argCount ( pParse ) ;
    char        filename[MAX_PATH_SIZE+1] ;    

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: plogopen( filename )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    gHyp_data_getStr ( pData, 
		       filename, 
		       MAX_PATH_SIZE,
		       gHyp_data_getSubScript(pData),
		       TRUE ) ;
    lHyp_promis_logopen ( filename ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }	
}

void gHyp_promis_plogclose ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {

    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    int	  	argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: plogclose()" ) ;

    lHyp_promis_logclose () ;
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }	
}


void gHyp_promis_plogin ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: plogin ( )
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
     *pResult ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      userId[VALUE_SIZE+1] ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT,
	"Invalid arguments. Usage: plogin ( username )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_getStr ( pData, 
		       userId, 
		       VALUE_SIZE,
		       gHyp_data_getSubScript(pData),
		       TRUE ) ;
    gHyp_data_setBool ( pResult, lHyp_promis_login ( userId ) ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }	
}

sData* gHyp_promis_getPROMIS (	sInstance	*pAI,
				sFrame 		*pFrame, 
				char 		*pStr,
				sData		*pVariable ) 
{
  sData 
    *pPROMISvariable = NULL ;

  /* Could the variable be a PROMIS $parameter ? */
  pPROMISvariable = gHyp_promis_getParm ( pAI, pFrame, pStr, pVariable ) ;

  if ( pPROMISvariable == NULL ) {

    /* No? Then is the variable a PROMIS %symbol ? */

    pPROMISvariable = gHyp_promis_getSymbol ( pAI, pFrame, pStr, pVariable ) ;

    if ( pPROMISvariable == NULL )

      /* No? Then is the variable a PROMIS database record $file or file ? */
      pPROMISvariable = gHyp_promis_parseRecord (pAI,pFrame,pStr,pVariable);

  }
  if ( pPROMISvariable ) pVariable = pPROMISvariable ;

  return pVariable ;
}

sData *gHyp_promis_parseRecord ( sInstance *pAI, 
				 sFrame *pFrame, 
				 char *recordName, 
				 sData *pTV ) 
{
  char
    *pChar,
    *(*name)[MAX_PROMIS_FIELDS],
    *pStr,
    recid[1+1],
    tableName[4+1] ;

  char fileName[TOKEN_SIZE+1] ;
  makeDSCs	( fileName_d, fileName ) ;

  int
    i,
    j,
    k,
    recordNameLen,
    lastStructIndex,
    recSize,
    stat ;

  sBYTE
    *pRecord ;

  sData
    *pTRES,
    *pTBLS,
    *pTLOG,
    *pValue,
    *pTLOG_RECID,
    *pTLOG_FLAGS,
    *pTLOG_LENGTH,
    *pTLOG_BUFFERA,
    *pTBLS_TABLENAME ;

  sLOGICAL
    found,
    isTRfile,
    isTRES=FALSE,
    isTBLS=FALSE,
    isTLOG=FALSE,
    isLittleEndian = FALSE ; /*gHyp_util_isLittleEndian() ;*/

  long
    recId,
    flags,
    length,
    fileId,
    fileIndex,
    fileType,
    fileChars[2],
    fieldId,
    fieldIndex,
    fieldType,
    fieldOffset,
    fieldSize,
    fieldCount,
    firstFieldId,
    parentField,
    idFields[8],
    bufSize,
    offset,
    numFields ;

  union {
    long id ;
    char name[5] ;
  } file ;

  sBYTE 
    *pBuf ;

  sField 
    (*pFields)[MAX_PROMIS_FIELDS], /* Pointer to an array of sField(s) */
    *pField ;
	
  /* Format should be '$file' or 'file' */
  recordNameLen = strlen ( recordName ) ;
  if ( recordNameLen != 4 && recordNameLen != 5 ) return NULL ;

  pChar = recordName ;
  isTRfile = FALSE ;
  if ( recordNameLen == 5 ) {
    if ( *pChar != '$' ) return NULL ;
    pChar++ ;
    isTRfile = TRUE ;
  }

  for ( i=0; i<4; i++ ) {
    if ( !isalpha ( *pChar ) || !islower ( *pChar ) ) return NULL ;
    pChar++ ;
  }

  /* Is the filename valid? */
  if ( isTRfile ) 
    strcpy ( file.name, recordName+1 ) ;
  else
    strcpy ( file.name, recordName ) ;

  gHyp_util_upperCase ( file.name, 4 ) ;

  /* Get info on the file */
  fileId = file.id ;  
  stat = Fil_Tbl_FileIndex ( &fileId, &fileIndex ) ; 
  if ( !(stat & 1) ) return NULL ;

  /* If the record is "fresh", it needs to be re-parsed. */
  strcpy ( fileName, file.name ) ;

  stat = Fil_Tbl_FileInfo( &fileId,			   
			   0,
			   &fileType,
			   &bufSize,
			   &numFields,
			   &firstFieldId ) ;
  if ( !(stat & 1) ) return NULL ;

  fileName_d.dsc_w_length = 4 ;

  if ( isTRfile ) {
    stat = aeqSsp_automan_getTrRecord ( &fileName_d, 
	 			        &bufSize,
				        &pBuf ) ;
    if ( !(stat & 1) ) return NULL ;

  }
  else {
    stat = aeqSsp_automan_getDbRecord ( &fileName_d, 
				        &bufSize,
				        &pBuf ) ;
    if ( !(stat & 1) ) return NULL ;
  }

  /* Parse the record */

  /* Initialize the file variable */
  if ( !pTV ) pTV = gHyp_frame_createVariable (pFrame,recordName);
  gHyp_data_deleteValues ( pTV ) ;
  gHyp_data_setHashed ( pTV, TRUE ) ;

  recId = 0 ;
  /* 
  gHyp_util_logInfo ( "File %s", file.name ) ;
  gHyp_util_logInfo ( "Type = %u", fileType ) ;
  gHyp_util_logInfo( "recsize=%d,numfields=%d,firstFieldId=%d",
		     bufSize,numFields,firstFieldId);
  */

  if ( strcmp ( file.name, "TRES" ) == 0 ) {
    /* Do not process TRESARR, that's dealt with as a special case below */      
    isTRES = TRUE ;
    numFields = numFields - 1 ;
  }
  else if ( strcmp ( file.name, "TLOG" ) == 0 ) {
    /* Do not process BUFFER, that's dealt with as a special case below */
    isTLOG = TRUE ;
    numFields = numFields - 1 ;
  }
  else if ( strcmp ( file.name, "TBLS" ) == 0 ) {
    /* Do not process BYTEBUFFER, that's dealt with as a special case below */
    isTBLS = TRUE ;
    numFields = numFields - 1 ;
  }
  /* The fileIndex will be between ACTL->WAFF */
  pFields = lHyp_promis_getFields ( fileIndex, numFields ) ;
  if ( !lHyp_promis_fillFields ( fileIndex, 
				   fileId, 
    				   recId,
				   firstFieldId,
				   pFields, 
				   numFields )) return NULL;

  offset = 0 ;
  /*gHyp_util_debug("Parsing %s", recordName);*/
  numFields = lHyp_promis_parseRecord (	pTV, 
					fileId,
					pBuf,
					pFields,
					numFields,
					offset,
					isLittleEndian ) ;
  if ( numFields <= 0 ) return NULL ;

  if ( isTRES ) {
    /* Special TRES "test results" case.*/
    if ( !lHyp_promis_addTRES( fileId,
			       pTV,
			       "TRESARR",
			       pBuf,
			       pAI,
			       pFrame,
			       isLittleEndian,
			       isTRfile))
      return NULL ; 
  }
  else if ( isTBLS ) {
    /* Special TBLS case.*/
    if ( !lHyp_promis_addTBLS( fileId,
			       pTV,
			       "BYTEBUFFER",
			       pBuf,
			       pAI,
			       pFrame,
			       isLittleEndian))
      return NULL ; 
  }
  else if ( isTLOG ) {
    /* Special TLOG case.*/
    if ( !lHyp_promis_addTLOG( fileId,
			       pTV,
			       "BUFFERA",
			       pBuf,
			       pAI,
			       pFrame,
			       isLittleEndian))
      return NULL ; 
  }
  return pTV ;
}

void gHyp_promis_initFields ()
{
  int
    stat,
    i ;

  union {
    long id ;
    char name[5] ;
  } file ;

  long 
    recId ;

  for ( i=0; i<MAX_PROMIS_ENTRIES; i++ ) {
    gpsFieldCache[i] = NULL ;
    giNumFields[i] = 0 ;
  }

  /* Get a list of all the TBLS record values */
  strcpy ( file.name, "TBLS" ) ;
  recId = 0 ;
  stat = Fil_Tbl_ListRecordValues ( &file.id, 
				    &recId, 
				    &gzTableNames[0], 
				    &giNumTables);
}

void gHyp_promis_cleanFields ( long fileIndex )
{
  int
    numFields,
    i,
    j,
    k,
    startIndex,
    endIndex ;

  sField
    (*pFields)[MAX_PROMIS_FIELDS],
    *pField ; 

  char
   *(*name)[MAX_PROMIS_FIELDS]; 
  /* name is a pointer to an array of pointers to char */

  if ( fileIndex == -1 ) {
    startIndex = 0 ;
    endIndex = MAX_PROMIS_ENTRIES ;
  }
  else {
    startIndex = fileIndex ;
    endIndex = startIndex+1 ;
  }

  for (i=startIndex; i<endIndex; i++ ) {

    pFields = gpsFieldCache[i] ;
    numFields = giNumFields[i] ;

    if ( pFields ) {     

      /*gHyp_util_logInfo("Cleaning %d fields from file %d",numFields,i);*/

      /* Free memory allocated by field */
      for ( j=0; j<numFields; j++ ) {

        pField = &(*pFields)[j] ;
        if ( pField->nameCount < 1 ) pField->nameCount = 1 ;
        name = pField->name ;
	/*
        gHyp_util_logInfo("Cleaning %d names for field %d",pField->nameCount,j);
	*/
        for ( k=0; k<pField->nameCount; k++ ) {
          /*gHyp_util_logInfo("Freeing %s",(*name)[k]);*/
 	  ReleaseMemory ( (*name)[k] ) ;
	}
        ReleaseMemory ( pField->name ) ;
      }
      ReleaseMemory ( pFields ) ;
      gpsFieldCache[i] = NULL ;
      giNumFields[i] = 0 ;
    }
  }   
  return ;
}

#endif
