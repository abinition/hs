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
 * $Log: data.c,v $
 * Revision 1.39  2009-12-08 20:47:55  bergsma
 * no change
 *
 * Revision 1.38  2009-09-21 05:16:42  bergsma
 * Typo
 *
 * Revision 1.37  2009-06-12 05:05:53  bergsma
 * Comment
 *
 * Revision 1.36  2009-03-13 07:48:16  bergsma
 * GD refinements.
 * Added BUILD_VERSION
 *
 * Revision 1.35  2009-03-07 21:27:32  bergsma
 * gHyp_data_getAll needs additional handle argument
 *
 * Revision 1.34  2008-05-07 17:28:10  bergsma
 * Don't interpret "-" or "+" as a number.
 *
 * Revision 1.33  2008-05-03 21:38:33  bergsma
 * data_exchange did not work when there were 2 elements!!!
 *
 * Revision 1.32  2007-09-03 06:21:31  bergsma
 * Remove const quallifiers
 *
 * Revision 1.31  2007-07-11 18:47:27  mhohimer
 * Added new functions primarily intended to facilitate Node_* functions in env.c:
gHyp_data_getPrev
 * gHyp_data_getTagIndex
 * gHyp_data_getChildNodeByIndex
 * gHyp_data_getFirstNode
 * gHyp_data_getLastNode
 * gHyp_data_getNextNode
 * gHyp_data_getPrevNode
 * gHyp_data_getNodeByAttr
 *
 * Revision 1.30  2007-06-10 05:13:23  bergsma
 * Check for 0x0x in &p
 *
 * Revision 1.29  2006-10-11 16:14:48  bergsma
 * Needed typedef on pSrcData.
 *
 * Revision 1.28  2006/09/16 19:40:46  bergsma
 * Added gHyp_data_newVectorSrc
 *
 * Revision 1.27  2005/12/10 00:30:30  bergsma
 * HS 3.6.5
 *
 * Revision 1.26  2005/09/02 04:35:53  bergsma
 * no message
 *
 * Revision 1.25  2005/08/12 01:16:16  bergsma
 * no message
 *
 * Revision 1.24  2005/07/23 22:31:14  bergsma
 * Added insertafter and Insertbefore
 *
 * Revision 1.23  2005/04/22 19:24:47  bergsma
 * When converting (scanf) integers greater than 0x7fffffff, convert to ulong
 * otherwise convert to int.
 *
 * Revision 1.22  2005/04/13 13:45:53  bergsma
 * HS 3.5.6
 * Added sql_toexternal.
 * Fixed handling of strings ending with bs (odd/even number of backslashes)
 * Better handling of exception condition.
 *
 * Revision 1.21  2005/03/09 04:15:11  bergsma
 * Added appendval and insertval.  Also, disallow append or insert of a variable
 * unto itself.  Needed new gHyp_data_isSilbing function.
 *
 * Revision 1.20  2005/01/10 20:04:04  bergsma
 * Temporary fix for movevalues, do not allow if arguments are not list variables.
 *
 * Revision 1.19  2004/12/13 04:55:35  bergsma
 * In lHyp_data_getStr, be consistent about using pStr variable.
 *
 * Revision 1.18  2004/10/16 04:33:30  bergsma
 * Renamed getStr, setStr, and setConstant functions.
 *
 * Revision 1.17  2004/07/28 00:49:18  bergsma
 * Version 3.3.0
 *
 * Revision 1.16  2004/07/27 15:42:26  bergsma
 * in getStr3, removed nulls altogether, rather than replacing with \001
 *
 * Revision 1.15  2004/07/01 02:02:34  bergsma
 * Added 'specialChars' argument to the toexternal() function.
 *
 * Revision 1.14  2004/05/15 02:07:25  bergsma
 * HTML unicode is &#ddddd; where ddddd is 5 digit decimal
 *
 * Revision 1.13  2004/04/29 08:28:48  bergsma
 * Bug in data_setText, truncation occurring.
 *
 * Revision 1.12  2003/11/16 20:53:24  bergsma
 * Uncomment TOKEN_RAW, literals using backquote.
 *
 * Revision 1.11  2003/07/27 00:51:45  bergsma
 * The unicode \u format needs to be in hex, to be more 'standard'.
 *
 * Revision 1.10  2003/06/05 22:14:42  bergsma
 * Correct formating problem with unicode type.
 *
 * Revision 1.9  2003/06/05 21:54:23  bergsma
 * Added new typedef 'unicode', UTF-16.
 * Required for new unicode type:
 * 1. New data type in data object
 * 2. Special unicode output for xdescribe function
 * 3. New TYPE_UNICODE in hypdef.h
 * 4. New token 0uNNNNN format, like 0xNNNNN and 0oNNNNN
 * 5. New function unicode()
 * 6. New SECS II data type.
 *
 * Revision 1.8  2003/04/27 23:47:55  bergsma
 * In data_moveValues, delete values out of destination variable first.
 *
 * Revision 1.7  2003/04/04 16:42:19  bergsma
 * Added moveValues function, to assist when detaching variables from
 * the method being executed.
 *
 * Revision 1.6  2003/01/12 12:49:04  bergsma
 * V3.1.0
 * - represent handle values as hex number, preceeded by 0x.
 * - gHyp_data_getVariable function
 *
 * Revision 1.5  2002/11/14 01:29:10  bergsma
 * Fixed bug in gHyp_data_exchange, used by sort()
 *
 * Revision 1.4  2002/10/23 00:05:11  bergsma
 * When printing double values, use 12g rather than 7g
 *
 * Revision 1.3  2002/09/21 22:11:36  bergsma
 * Added function gHyp_data_newConstant3 used in sql.c
 *
 * Revision 1.2  2002/09/16 15:21:56  bergsma
 * No changes, just consolidated Modification sections to top of file.
 *
 */

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

static char * NULLstring = "" ;

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/* The data object stores and manages the variables used by a HyperScript
 * program.  The sData objects can be linked together in sibling and child
 * relationships.
 *
 */
struct data_t {
  /* Data object types: 
   *
   * -------------------------------------------------------------------
   * tokenType             dataType    size    ss
   * -------------------------------------------------------------------
   * TOKEN_REFERENCE       STRING       1      array subscript
   * TOKEN_LITERAL         STRING       1      n/a
   * TOKEN_RAW		   STRING       1      n/a
   * TOKEN_CONSTANT        > STRING     1      n/a
   * TOKEN_UNIDENTIFIED    STRING       1      n/a
   * TOKEN_VARIABLE        TYPE_*       0-n    array subscript
   *
   * The first four token types are "leaves", they have no children.
   * The column 'dataType' refers to the data objects themselves.
   * The 'size' column is 1 because there is always just a single token.
   * The column 'ss' has special meaning for TOKEN_REFERENCE, it specifies the
   * element of the variable it reference.  
   *  
   * The last token type VARIABLE is a "node", it can have children.
   * When the data type is TYPE_LIST or TYPE_STRING, then pData is
   * used to hold a list of child data objects.
   * When the data type is TYPE_BYTE or above, then p.value is used to
   * hold a vector of primitive data elements.
   * The 'size' column is the current size of the list or vector.
   * A zero size means the pointers p.value or pData are NULL.
   * Column 'ss' is an array subscript, -1 means undefined.
   * If the 'isDynamic' flag is set for a VARIABLE, then the size can 
   * change dynamically.
   *
   */ 
  sBYTE		tokenType ;
  sBYTE		dataType ;

  /* For VARIABLES */
  int		size ;	/* The current size. */
  sLOGICAL	isDynamic ; /* If size can change, or is fixed. */
  int		ss ;	/* Subscript: -1 = all, 0-N = index */

  /* Malloc'd data.  
   * NULL when TOKEN_LITERAL or can be NULL when 
   * TOKEN_REFERENCE.
   */
  union {
    void*		value ;		/* Catch-all */
    int*		iValue ;	/* TYPE_INTEGER */
    long*		slValue ;	/* TYPE_LONG */
    unsigned long*	ulValue ;	/* TYPE_ULONG */
    short*		ssValue ;	/* TYPE_SHORT */
    unsigned short*	usValue ;	/* TYPE_USHORT */
    char*		sbValue ;	/* TYPE_BYTE */
    unsigned char*     	ubValue ;	/* TYPE_UBYTE */
    unsigned char*     	boValue ;	/* TYPE_BOOLEAN */
    unsigned char*     	biValue ;	/* TYPE_BINARY */
    char*		scValue ;	/* TYPE_CHAR */
    unsigned char*	ucValue ;	/* TYPE_UCHAR */
    float*		fValue ;	/* TYPE_FLOAT */
    double*		dValue ;	/* TYPE_DOUBLE */
    void**		hValue ;	/* TYPE_HANDLE */
    unsigned long*	xValue ;	/* TYPE_HEX */
    unsigned long*	oValue ;	/* TYPE_OCTAL */
    unsigned short*	uValue ;	/* TYPE_UNICODE */
    char*		aValue ;	/* TYPE_ATTR */
   sData**		rValue ;	/* Storage for TOKEN_REFERENCE */
  } p ;

  /* Label when TOKEN_VARIABLE, TOKEN_UNIDENTIFIED, or TOKEN_REFERENCE.
   */  
  char*			pStrVal ;       /* External (string) representation */
  int			strlen ;        /* String length */

  /* Linkages, for TOKEN_VARIABLE (TYPE_LIST or TYPE_STRING) only!!! */
  sData*	pData ; 	/* Child data member */
  sData*	pNext ;		/* Next data member */
  sData*	pPrev ;	/* Previous data member */
  sData*	pParent ;	/* Pointer to parent */

  sHash		*(*pHash)[MAX_HASH_TABLE_SIZE] ;       
  /* Table for non-sequential "hashed" lookup of children. */

  /* HyperScript attachments */

  /* Attached object can be anything, or nothing at all.  It is currently used for:
   * A. Methods, stored in the data element that names the method. A sMethod object contains
   *    the sHyp object.
   * B. sInstance
   * C. sLabel
   * D. Application sockets, DATA_OBJECT_* from hypdef.h
   */
  void*		pObject ;
  sBYTE		objectType ;
  void		(*pfDelete)(void *) ; /* Function to delete pObject */
} ;

/**********************	FUNCTION DEFINITIONS ********************************/

sData* gHyp_data_new ( char *name )
{
  /* Description:
   *
   *	Create and initialize a sData object.
   *
   * Arguments:
   *
   *	name
   *    - name of new data object
   *
   * Return value:
   *
   *	Pointer to new sData object
   *
   */
   
  sData	*pData = (sData*) AllocMemory ( sizeof ( sData ) ) ;
  assert ( pData ) ;
  
  /* Initialize as a NULL string */
  pData->pData = NULL ;
  pData->pNext = pData ;
  pData->pPrev = pData ;
  pData->pParent = pData ;
  pData->pHash = NULL ;
  pData->pObject = NULL ;
  pData->objectType = DATA_OBJECT_NULL ;
  pData->pfDelete = NULL ;
  pData->pStrVal = NULLstring ;
  pData->strlen = 0 ;
  pData->tokenType = TOKEN_LITERAL ;
  pData->dataType = TYPE_STRING ;
  pData->p.value = NULL ;
  pData->size = 1 ;
  pData->isDynamic = TRUE ; 
  pData->ss = -1 ;

  /* If 'name' is specified, then default is a list variable with 0 elements */
  if ( name ) gHyp_data_setVariable ( pData, name, TYPE_LIST ) ;

  /*
   if ( name ) 
    *gHyp_util_debug("Creating %p/%s",pData,pData->pStrVal);
   else
    *gHyp_util_debug("Creating %p/???",pData);
  */

  return pData ;
}

void *gHyp_data_getObject ( sData *pData ) 
{
  /* For references, recursively find the variable */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL ) 
    return gHyp_data_getObject ( *pData->p.rValue ) ;

  return pData->pObject ;
}

sBYTE gHyp_data_getObjectType ( sData *pData ) 
{
  /* For references, recursively find the variable */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL ) 
    return gHyp_data_getObjectType ( *pData->p.rValue ) ;

  return pData->objectType ;
}

void gHyp_data_setObject ( sData *pData, 
			   void *pObject,
			   sBYTE objectType,
			   void  (*pfDelete)(void*) )
{
  /* For references, recursively find the variable */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL ) {
    gHyp_data_setObject ( *pData->p.rValue, pObject, objectType, pfDelete ) ;
    return ;
  }

  if ( pData->pObject != NULL && pData->pObject != pObject ) {
    if ( pData->pfDelete )
      pData->pfDelete ( pData->pObject ) ;
    else
      ReleaseMemory ( pData->pObject ) ;
  }
  pData->pObject = pObject ;
  pData->objectType = objectType ;
  pData->pfDelete = pfDelete ;
  return ;
}

void gHyp_data_copyObject ( sData *pDst, sData *pSrc )
{
  /* For references, recursively find the variable */
  if ( pDst->tokenType == TOKEN_REFERENCE && pDst->p.rValue != NULL ) {
    gHyp_data_copyObject ( *pDst->p.rValue, pSrc ) ;
    return ;
  }

  if ( pSrc->tokenType == TOKEN_REFERENCE && pSrc->p.rValue != NULL ) {
    gHyp_data_copyObject ( pDst, *pSrc->p.rValue ) ;
    return ;
  }

  if ( pSrc != pDst && 
       pSrc->pObject != NULL && 
       pDst->pObject == NULL ) {
    /*gHyp_util_debug("COPY pSrc->tokenType %d",pSrc->tokenType);*/
    /*if ( pSrc->tokenType == TOKEN_VARIABLE ) return ;*/
    pDst->objectType = pSrc->objectType ;
    pDst->pObject = pSrc->pObject ;
    pDst->pfDelete = pSrc->pfDelete ;
    pSrc->pObject = NULL ;
    pSrc->objectType = DATA_OBJECT_NULL ;
    pSrc->pfDelete = NULL ;
  }
  return ;
}
  
static void lHyp_data_newVector ( sData *pData, 
				  sBYTE tokenType, 
				  sBYTE dataType, 
				  int size,
				  sLOGICAL isDynamic,
				  void *pSrcData )
{
  /* Description:
   *
   *	Create and initialize a vector or a constant from a pData (variable) 
   *
   * Arguments:
   *
   *	pData
   *    - name of an existing variable
   *
   *	dataType
   *	- dataType of vector to create
   *
   *	size
   *	- size of vector
   *
   * Return value:
   *
   *	none
   *
   *
   */
   
  int    
    i ;
  
  char
    *pa,
    *psc,
    *psb,
    *pa_s,
    *psc_s,
    *psb_s ;
  
  unsigned char
    *puc,
    *pub,
    *pbo,
    *pbi,
    *puc_s,
    *pub_s,
    *pbo_s,
    *pbi_s ;

  short
    *pss,
    *pss_s ;
  
  unsigned short
    *pus,
    *pu,
    *pus_s,
    *pu_s ;
  
  int
    *pi,
    *pi_s ;
  
  long 
    *psl,
    *psl_s ;

  unsigned long
    *pul,
    *px,
    *po,
    *pul_s,
    *px_s,
    *po_s ;
  
  float
    *pf,
    *pf_s ;
  
  double
    *pd,
    *pd_s ;
  
  void	
    **ph,
    **ph_s ;

  /* First delete any existing variables values */
  gHyp_data_deleteValues ( pData ) ;

  /* Set the token type - VARIABLE or CONSTANT */
  pData->tokenType = tokenType ;

  /* Set the datatype */
  pData->dataType = dataType ;

  /* Initialize for VARIABLES */
  if ( tokenType == TOKEN_VARIABLE ) {
    pData->isDynamic = isDynamic ;
    pData->size = size ;
    pData->ss = -1 ;
  }

  if ( size < 1 ) return ;

  /* Create vector based on the data type. */
  switch ( dataType ) {
        
  case TYPE_ATTR :
    pa = pData->p.aValue = (char*) AllocMemory ( sizeof ( char ) * pData->size ) ;
    assert ( pa ) ;
    if ( pSrcData ) {
	pa_s = (char*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pa++ = *pa_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++ ) *pa++ = 32 ;
    break ;

  case TYPE_CHAR :
    psc = pData->p.scValue = (char*) AllocMemory ( sizeof ( char ) * pData->size ) ;
    assert ( psc ) ;
    if ( pSrcData ) {
	psc_s = (char*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *psc++ = *psc_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++ ) *psc++ = 32 ;
    break ;

  case TYPE_BYTE :
    psb = pData->p.sbValue = (char*) AllocMemory ( sizeof ( char ) * pData->size ) ;
    assert ( psb ) ;
    if ( pSrcData ) {
	psb_s = (char*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *psb++ = *psb_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++ ) *psb++ = 0 ;
    break ;
    
  case TYPE_UCHAR :
    puc = pData->p.ucValue = (unsigned char*) 
      AllocMemory ( sizeof (unsigned char) * pData->size ) ;
    assert ( puc ) ;
    if ( pSrcData ) {
	puc_s = (unsigned char*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *puc++ = *puc_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *puc++ = 32 ;
    break ;

  case TYPE_UBYTE :
    pub = pData->p.ubValue = (unsigned char*)
      AllocMemory ( sizeof (unsigned char) * pData->size ) ;
    assert ( pub ) ;
    if ( pSrcData ) {
	pub_s = (unsigned char*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pub++ = *pub_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pub++ = 0 ;
    break ;
    
  case TYPE_BOOLEAN :
    pbo = pData->p.boValue = (unsigned char*)
      AllocMemory ( sizeof (unsigned char) * pData->size ) ;
    assert ( pbo ) ;
    if ( pSrcData ) {
	pbo_s = (unsigned char*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pbo++ = *pbo_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pbo++ = 0 ;
    break ;
    
  case TYPE_BINARY :
    pbi = pData->p.biValue = (unsigned char*)
      AllocMemory ( sizeof (unsigned char) * pData->size ) ;
    assert ( pbi ) ;
    if ( pSrcData ) {
	pbi_s = (unsigned char*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pbi++ = *pbi_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pbi++ = 0 ;
    break ;
    
  case TYPE_HEX :
    px = pData->p.xValue = (unsigned long*)
      AllocMemory ( sizeof (unsigned long) * pData->size ) ;
    assert ( px ) ;
    if ( pSrcData ) {
	px_s = (unsigned long*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *px++ = *px_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *px++ = 0 ;
    break ;
    
  case TYPE_OCTAL :
    po = pData->p.oValue = (unsigned long*)
      AllocMemory ( sizeof (unsigned long) * pData->size ) ;
    assert ( po ) ;
    if ( pSrcData ) {
	po_s = (unsigned long*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *po++ = *po_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *po++ = 0 ;
    break ;
    
  case TYPE_UNICODE :
    pu = pData->p.uValue = (unsigned short*)
      AllocMemory ( sizeof (unsigned short) * pData->size ) ;
    assert ( pu ) ;
    if ( pSrcData ) {
	pu_s = (unsigned short*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pu++ = *pu_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pu++ = 0 ;
    break ;
    
  case TYPE_SHORT :
    pss = pData->p.ssValue = (short*) AllocMemory ( sizeof (short) * pData->size) ;
    assert ( pss ) ;
    if ( pSrcData ) {
	pss_s = (short*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pss++ = *pss_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pss++ = 0 ;
    break ;
    
  case TYPE_USHORT :
    pus = pData->p.usValue = (unsigned short*) AllocMemory ( sizeof(unsigned short) * pData->size);
    assert ( pus ) ;
    if ( pSrcData ) {
	pus_s = (unsigned short*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pus++ = *pus_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pus++ = 0 ;
    break ;
    
  case TYPE_INTEGER :
    pi = pData->p.iValue = (int*) AllocMemory ( sizeof (int) * pData->size);
    assert ( pi ) ;
    if ( pSrcData ) {
	pi_s = (int*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pi++ = *pi_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pi++ = 0 ;
    break ;
    
  case TYPE_LONG :
    psl = pData->p.slValue = (long*) AllocMemory ( sizeof (long) * pData->size);
    assert ( psl ) ;
    if ( pSrcData ) {
	psl_s = (long*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *psl++ = *psl_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *psl++ = 0 ;
    break ;
    
  case TYPE_ULONG :
    pul = pData->p.ulValue = (unsigned long*) AllocMemory ( sizeof (unsigned long) * pData->size);
    assert ( pul ) ;
    if ( pSrcData ) {
	pul_s = (unsigned long*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pul++ = *pul_s++ ;
    }
    else
    for ( i=0; i<pData->size; i++) *pul++ = 0 ;
    break ;
    
  case TYPE_FLOAT :
    pf = pData->p.fValue = (float*) AllocMemory ( sizeof ( float ) * pData->size ) ;
    assert ( pf ) ;
    if ( pSrcData ) {
	pf_s = (float*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pf++ = *pf_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pf++ = 0.0 ;
    break ;
    
  case TYPE_DOUBLE :
    pd = pData->p.dValue = (double*) AllocMemory ( sizeof ( double ) * pData->size ) ;
    assert ( pd ) ;
    if ( pSrcData ) {
	pd_s = (double*) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *pd++ = *pd_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *pd++ = 0.0 ;
    break ;
    
  case TYPE_HANDLE :
    ph = pData->p.hValue = (void**) AllocMemory ( sizeof ( void* ) * pData->size ) ;
    assert ( ph ) ;
    if ( pSrcData ) {
	ph_s = (void**) pSrcData ;
	for ( i=0; i<pData->size; i++ ) *ph++ = *ph_s++ ;
    }
    else
	for ( i=0; i<pData->size; i++) *ph++ = NULL ;
    break ;
  }

}

void gHyp_data_newVector ( sData *pData, 
			   sBYTE dataType, 
			   int size,
			   sLOGICAL isDynamic )
{
  lHyp_data_newVector ( pData, 
			TOKEN_VARIABLE, 
			dataType, 
			size,
			isDynamic,
			NULL ) ;
}


void gHyp_data_newVectorSrc ( sData *pData, 
			   sBYTE dataType, 
			   int size,
			   sLOGICAL isDynamic,
			   void* pSrcData )
{
  lHyp_data_newVector ( pData, 
			TOKEN_VARIABLE, 
			dataType, 
			size,
			isDynamic,
			pSrcData ) ;
}

void gHyp_data_newConstant ( sData *pData, 
			     sBYTE dataType,
			     sData *pSrcData,
			     int ss )
{
  char
    value[VALUE_SIZE+1] ;

  lHyp_data_newVector ( pData, 
			TOKEN_CONSTANT, 
			dataType, 
			1, 
			FALSE,
			NULL ) ;
  if ( pSrcData )
    gHyp_data_setVector ( pData, 
			  pSrcData, 
			  0, 
			  ss,
			  TRUE ) ;
  gHyp_data_getStr ( pData, value, VALUE_SIZE, 0, TRUE ) ;
  gHyp_data_setLabel ( pData, value ) ;
}


void gHyp_data_newConstant_raw ( sData *pData, 
			     sBYTE dataType,
			     void *pSrcData )
{
  char
    value[VALUE_SIZE+1] ;

  lHyp_data_newVector ( pData, 
			TOKEN_CONSTANT, 
			dataType, 
			1, 
			FALSE,
			NULL ) ;
  if ( pSrcData ) gHyp_data_setVectorRaw ( pData, pSrcData, 0 ) ;
  gHyp_data_getStr ( pData, value, VALUE_SIZE, 0, TRUE ) ;
  gHyp_data_setLabel ( pData, value ) ;
}

void gHyp_data_newConstant_scanf (  sData *pData, 
				    sBYTE dataType,
				    char *pSrcData,
				    int n )
{
  short
    s ;
  
  long
    l ;

  float
    f ;

  double 
    d ;

  unsigned char
    b ;

  char
    value[VALUE_SIZE+1] ;

  lHyp_data_newVector ( pData, 
			TOKEN_CONSTANT, 
			dataType, 
			1, 
			FALSE,
			NULL ) ;
 
  strncpy ( value, (char*) pSrcData, n ) ;
  value[n] = '\0' ;

  switch ( dataType ) {

   case TYPE_BYTE :
    sscanf ( value, "%hd", &s ) ;
    b = (unsigned char) s ;
    gHyp_data_setVectorRaw ( pData, &b, 0 ) ;
    break ;
  case TYPE_SHORT:
    sscanf ( value, "%hd", &s ) ;
    gHyp_data_setVectorRaw ( pData, &s, 0 ) ;
    break ;
  case TYPE_LONG :
    sscanf ( value, "%ld", &l ) ;
    gHyp_data_setVectorRaw ( pData, &l, 0 ) ;
    break ;
  case TYPE_FLOAT :
    sscanf ( value, "%f", &f ) ;
    gHyp_data_setVectorRaw ( pData, &f, 0 ) ;
    break ;
  case TYPE_DOUBLE :
    sscanf ( value, "%lf", &d ) ;
    gHyp_data_setVectorRaw ( pData, &d, 0 ) ;
    break ;
  }

  gHyp_data_getStr ( pData, value, VALUE_SIZE, 0, TRUE ) ;
  gHyp_data_setLabel ( pData, value ) ;
}
		      
void gHyp_data_exchange ( sData *pValue1, sData *pValue2 )
{

  sData
      *pPrev1,
      *pNext1,
      *pPrev2,
      *pNext2 ;

  pNext1 = pValue1->pNext ;
  pPrev1 = pValue1->pPrev ;
  pNext2 = pValue2->pNext ;
  pPrev2 = pValue2->pPrev ;

  if ( pNext1 == pValue2 && pPrev1 != pValue2 ) {

    /* P1 - 1 - 2 - N2  ==>  P1 - 2 - 1 - N2 */
    pNext2->pPrev  = pValue1 ;
    pValue1->pNext = pNext2 ;
    pValue1->pPrev = pValue2 ;
    pValue2->pNext = pValue1 ;
    pValue2->pPrev = pPrev1 ;
    pPrev1->pNext  = pValue2 ;
    
  }
  else if ( pPrev1 == pValue2 && pNext1 != pValue2 ) {

    /* P2 - 2 - 1 - N1  ==>  P2 - 1 - 2 - N1  */
    pNext1->pPrev  = pValue2 ;
    pValue2->pNext = pNext1 ;
    pValue2->pPrev = pValue1;
    pValue1->pNext = pValue2 ;
    pValue1->pPrev = pPrev2 ;
    pPrev2->pNext  = pValue1; 
    
  }
  else if ( pPrev1 == pValue2 && pNext1 == pValue2 ) {
    /* Only two values */
    pValue1->pParent->pData = pValue1->pParent->pData->pNext ; 
  }
  else {

    /* P1 - 1 - N1 .... P2 - 2 - N2  ==>   P1 - 2 - N1 .... P2 - 1 - N2 */
    pNext2->pPrev  = pValue1 ;
    pValue1->pNext = pNext2 ;
    pValue1->pPrev = pPrev2 ;
    pPrev2->pNext  = pValue1 ;

    pNext1->pPrev  = pValue2 ;
    pValue2->pNext = pNext1 ;
    pValue2->pPrev = pPrev1 ;
    pPrev1->pNext  = pValue2 ;
  }

  if ( pValue1->pParent->pData == pValue2 ) pValue1->pParent->pData = pValue1 ;
  else if ( pValue1->pParent->pData == pValue1 ) pValue1->pParent->pData = pValue2 ;

  return ;
}

void gHyp_data_append ( sData *pParent, sData *pData )
{
  /* Description:
   *
   *	Append a value to the end of a variable's list.
   *
   * Arguments:
   *
   *	pParent					[R/W]
   *	- pointer to variable
   *
   *	pData					[R]
   *	- pointer to value 	
   *
   * Return value:
   *
   *	none
   *
   */
  sData 
    *pMember ;
  
  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) {
    gHyp_data_append ( *pParent->p.rValue, pData ) ;    
    return ;
  }

  /* Get first member of variable's ring */
  pMember = pParent->pData ;

  if ( pMember ) {

    pData->pPrev = pMember->pPrev ;  /* Attach pData to last element */
    pData->pNext = pMember ;         /* Attach pData to first element */
    pMember->pPrev->pNext = pData ;  /* Last element now points to pData */ 
    pMember->pPrev = pData ;         /* Make pData the last element */

  } 
  else {

    /* Put pData at first position since there are no other Members */
    pParent->pData = pData ;
    pData->pPrev = pData ;
    pData->pNext = pData ;
  }

  pData->pParent = pParent ;  /* Point back to parent */

  /* Add to hash table if defined */
  if ( pParent->pHash )
    gHyp_hash_update ( pParent->pHash,
		       VARIABLE_HASH_TABLE_SIZE,
		       pData->pStrVal,
		       pData ) ;

  return ;
}


void gHyp_data_insert ( sData *pParent, sData *pData )
{
  /* Description:
   *
   *	Insert a value to the front of a variable's list.
   *
   * Arguments:
   *
   *	pParent					[R/W]
   *	- pointer to variable
   *
   *	pData					[R]
   *	- pointer to value 	
   *
   * Return value:
   *
   *	none
   *
   */

  sData
    *pMember ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) {
    gHyp_data_insert ( *pParent->p.rValue, pData ) ;    
    return ;
  }

  /* Get first member of variable's ring */
  pMember = pParent->pData ;

  if ( pMember ) {

    /* Go to last element in list, and append pData to it */
    pData->pPrev = pMember->pPrev ;
    pData->pNext = pMember ;
    pMember->pPrev->pNext = pData ;
    pMember->pPrev = pData ;
  } 
  else {
    /* Adopt pData at first position since there are no other Members */
    pData->pPrev = pData ;
    pData->pNext = pData ;  
  }

  /* Make pData the first member */
  pParent->pData = pData ;
  
  /* Point back to parent */
  pData->pParent = pParent ; 

  /* Add to hash table if defined */
  if ( pParent->pHash )
    gHyp_hash_update ( pParent->pHash,
		       VARIABLE_HASH_TABLE_SIZE,
		       pData->pStrVal,
		       pData ) ;
    
  return ;
}

void gHyp_data_insertbefore ( sData *pSibling, sData *pData )
{
  /* Description:
   *
   *	Insert a value before a sibling.
   *
   * Arguments:
   *
   *	pParent					[R/W]
   *	- pointer to variable
   *
   *	pData					[R]
   *	- pointer to value 	
   *
   * Return value:
   *
   *	none
   *
   */

  sData
    *pParent ;

  /* For references, recursively find the variable */
  if ( pSibling->tokenType == TOKEN_REFERENCE && pSibling->p.rValue != NULL ) {
    gHyp_data_insertbefore ( *pSibling->p.rValue, pData ) ;    
    return ;
  }

  /* Get parent of ring */
  pParent = pSibling->pParent ;

  /* Stitch in pData */

  pData->pParent = pParent ;

  /* Example:

    [pS1]next--><--prev[pS2]next--><--prev[pS3]

    insert pD1 before pS2
  
    [pS1]next--><--prev[pD1]next--><--prev[S2]next--><--prev[S3]

    BUT, REMEMBER, when there is only one sibling, S1==S2==S3

    [pS1]next--><--prev[pS1]next--><--prev[pS1]

    insert pD1 before pS2
  
    [pS1]next--><--prev[pD1]next--><--prev[S1]next--><--prev[pD1]

    THEREFORE, to do it correctly.

    pD1->prev = pS2->prev (but this is S2!!)
    pD1->next = pS2 ;
    pS2->prev->next = pD1
    pS2->prev = pD1

  */

  pData->pPrev = pSibling->pPrev ;
  pData->pNext = pSibling ;
  pSibling->pPrev->pNext = pData ;
  pSibling->pPrev = pData ;

  if ( pParent->pData == pSibling ) pParent->pData = pData ;

  /* Add to hash table if defined */
  if ( pParent->pHash )
    gHyp_hash_update ( pParent->pHash,
		       VARIABLE_HASH_TABLE_SIZE,
		       pData->pStrVal,
		       pData ) ;
    
  return ;
}


void gHyp_data_insertafter ( sData *pSibling, sData *pData )
{
  /* Description:
   *
   *	Insert a value after a sibling.
   *
   * Arguments:
   *
   *	pParent					[R/W]
   *	- pointer to variable
   *
   *	pData					[R]
   *	- pointer to value 	
   *
   * Return value:
   *
   *	none
   *
   */

  sData
    *pParent ;

  /* For references, recursively find the variable */
  if ( pSibling->tokenType == TOKEN_REFERENCE && pSibling->p.rValue != NULL ) {
    gHyp_data_insertafter ( *pSibling->p.rValue, pData ) ;    
    return ;
  }

  /* Get parent of ring */
  pParent = pSibling->pParent ;

  /* Stitch in pData */

  pData->pParent = pParent ;

  pData->pPrev = pSibling        ;  /* Attach pData to last element */
  pData->pNext = pSibling->pNext ;  /* Attach pData to first element */
  pSibling->pNext->pPrev = pData ;  /* Last element now points to pData */ 
  pSibling->pNext = pData ;         /* Make pData the last element */


  /* Add to hash table if defined */
  if ( pParent->pHash )
    gHyp_hash_update ( pParent->pHash,
		       VARIABLE_HASH_TABLE_SIZE,
		       pData->pStrVal,
		       pData ) ;
    
  return ;
}

sLOGICAL gHyp_data_next ( sData *pParent )
{
  /* Description:
   *
   *	Move the pointer from the first child element to the next child.
   *
   * Arguments:
   *
   *	pParent					[R/W]
   *	- pointer to variable
   *
   *	pData					[R]
   *	- pointer to value 	
   *
   * Return value:
   *
   *	TRUE if next value exists
   *
   */

  sData
    *pMember ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL )
    return gHyp_data_next ( *pParent->p.rValue ) ;    

  /* Get first member of variable's ring */
  pMember = pParent->pData ;

  if ( pMember ) {
    /* Go to next element in list, and set parent to point to it. */
    pParent->pData = pMember->pNext ;
    return TRUE ;
  }
  return FALSE ;
}

sLOGICAL gHyp_data_prev ( sData *pParent )
{
  /* Description:
   *
   *	Move the pointer to the previous child element.
   *
   * Arguments:
   *
   *	pParent					[R/W]
   *	- pointer to variable
   *
   *	pData					[R]
   *	- pointer to value 	
   *
   * Return value:
   *
   *	TRUE if previous value exists
   *
   *
   */

  sData
    *pMember ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL )
    return gHyp_data_prev ( *pParent->p.rValue ) ;    

  /* Get first member of variable's ring */
  pMember = pParent->pData ;

  if ( pMember ) {
    /* Go to previous element in list, and set parent to point to it. */
    pParent->pData = pMember->pPrev ;
    return TRUE ;
  }
  return FALSE ;
}

sData *gHyp_data_copy ( sData *pSrc )
{
  /* Description:
   *
   *	Make an exact copy of a sData object, less any children
   *
   * Arguments:
   *
   *	pSrc					[R]
   *	- pointer to sData
   *
   * Return value:
   *
   *	Pointer to sData copy
   *
   *
   */
  int    
    i ;
 
  char
    *pas,
    *pad,
    *pscs,
    *pscd,
    *psbs,
    *psbd ;
  
  unsigned char
    *pucs,
    *pucd,
    *pubs,
    *pubd,
    *pbis,
    *pbid,
    *pbos,
    *pbod ;
  
  short
    *psss,
    *pssd ;
  
  unsigned short
    *puss,
    *pusd, 
    *pus,
    *pud ;
  
  int 
    *pis,
    *pid ;
  
  long 
    *psls,
    *psld ;
  
  unsigned long
    *puls,
    *puld,
    *pod,
    *pos,
    *pxd,
    *pxs ;
  
  float
    *pfs,
    *pfd ;
  
  double
    *pds,
    *pdd ;
  
  void	
    **phs,
    **phd ;
  
  sData	
    **prs,
    **prd ;

  /* Create new data object */
  sData	*pDst = gHyp_data_new ( NULL ) ;

  /* Copy descriptors */
  pDst->tokenType = pSrc->tokenType ;
  pDst->dataType = pSrc->dataType ;
  pDst->size = pSrc->size ;
  pDst->isDynamic = pSrc->isDynamic ;
  pDst->ss = pSrc->ss ;

  /* pDst->pStrVal will be set to NULLstring */
  if ( pSrc->pStrVal != NULLstring ) {

    /* Always copy the string representation */
    pDst->pStrVal = (char*) AllocMemory ( pSrc->strlen + 1 ) ;
    assert ( pDst->pStrVal ) ;
    memcpy ( pDst->pStrVal, pSrc->pStrVal, pSrc->strlen+1 ) ;
    pDst->strlen = pSrc->strlen ;
    /*
    gHyp_util_logInfo("Copying from %s(%p) to %s(%p)",
		      pSrc->pStrVal,pSrc->pStrVal,
		      pDst->pStrVal,pDst->pStrVal) ;
    */
  }
  /*if ( pSrc->pObject ) gHyp_util_debug("Object not copied from %s(%d) to %s(%d)",
		    pSrc->pStrVal,
		    pSrc->tokenType,
		    pDst->pStrVal,
		    pDst->tokenType);
  */

  /* If the source was hashed, then so must be the destination */
  if ( pSrc->pHash ) gHyp_data_setHashed ( pDst, TRUE ) ;

  /* If there's no value to copy, we're done */
  if ( pSrc->p.value == NULL ) return pDst ;

  if ( pSrc->tokenType == TOKEN_REFERENCE ) {
    prd = pDst->p.rValue = (sData**) AllocMemory ( sizeof ( sData* ) ) ;
    assert ( prd ) ;
    prs = pSrc->p.rValue; 
    *prd = *prs ;
    return pDst ;
  } 

  /* Don't AllocMemory for array sizes less than 1 */
  if ( pSrc->size < 1 ) return pDst ;

  /* Copy data based on the data type. */
  switch ( pSrc->dataType ) {
        
  case TYPE_ATTR :
    pad = pDst->p.aValue = (char*) AllocMemory ( sizeof ( char ) * pSrc->size ) ;
    assert ( pad ) ;
    for ( i=0, pas=pSrc->p.aValue; i<pSrc->size; i++ ) 
      *pad++ = *pas++ ;
    break ;

  case TYPE_CHAR :
    pscd = pDst->p.scValue = (char*) AllocMemory ( sizeof ( char ) * pSrc->size ) ;
    assert ( pscd ) ;
    for ( i=0, pscs=pSrc->p.scValue; i<pSrc->size; i++ ) 
      *pscd++ = *pscs++ ;
    break ;

  case TYPE_BYTE :
    psbd = pDst->p.sbValue = (char*) AllocMemory ( sizeof ( char ) * pSrc->size ) ;
    assert ( psbd ) ;
    for ( i=0, psbs=pSrc->p.sbValue; i<pSrc->size; i++ ) 
      *psbd++ = *psbs++ ;
    break ;
    
  case TYPE_UCHAR :
    pucd = pDst->p.ucValue = (unsigned char*) AllocMemory ( sizeof (unsigned char) * pSrc->size ) ;
    assert ( pucd ) ;
    for ( i=0, pucs=pSrc->p.ucValue; i<pSrc->size; i++) 
      *pucd++ = *pucs++ ;
    break ;

  case TYPE_UBYTE :
    pubd = pDst->p.ubValue = (unsigned char*) AllocMemory ( sizeof (unsigned char) * pSrc->size ) ;
    assert ( pubd ) ;
    for ( i=0, pubs=pSrc->p.ubValue; i<pSrc->size; i++) 
      *pubd++ =  *pubs++ ;
    break ;
    
  case TYPE_BINARY :
    pbid = pDst->p.biValue = (unsigned char*) AllocMemory ( sizeof (unsigned char) * pSrc->size ) ;
    assert ( pbid ) ;
    for ( i=0, pbis=pSrc->p.biValue; i<pSrc->size; i++) 
      *pbid++ =  *pbis++ ;
    break ;

  case TYPE_HEX :
    pxd = pDst->p.xValue = (unsigned long*) AllocMemory ( sizeof (unsigned long) * pSrc->size ) ;
    assert ( pxd ) ;
    for ( i=0, pxs=pSrc->p.xValue; i<pSrc->size; i++) 
      *pxd++ =  *pxs++ ;
    break ;

  case TYPE_OCTAL :
    pod = pDst->p.oValue = (unsigned long*) AllocMemory ( sizeof (unsigned long) * pSrc->size ) ;
    assert ( pod ) ;
    for ( i=0, pos=pSrc->p.oValue; i<pSrc->size; i++) 
      *pod++ =  *pos++ ;
    break ;
    
  case TYPE_UNICODE :
    pud = pDst->p.uValue = (unsigned short*) AllocMemory ( sizeof (unsigned short) * pSrc->size ) ;
    assert ( pud ) ;
    for ( i=0, pus=pSrc->p.uValue; i<pSrc->size; i++) 
      *pud++ =  *pus++ ;
    break ;
    
  case TYPE_BOOLEAN :
    pbod = pDst->p.boValue = (unsigned char*) AllocMemory ( sizeof (unsigned char) * pSrc->size ) ;
    assert ( pbod ) ;
    for ( i=0, pbos=pSrc->p.boValue; i<pSrc->size; i++) 
      *pbod++ =  *pbos++ ;
    break ;
    
  case TYPE_SHORT :
    pssd = pDst->p.ssValue = (short*) AllocMemory ( sizeof (short) * pSrc->size) ;
    assert ( pssd ) ;
    for ( i=0,psss=pSrc->p.ssValue; i<pSrc->size; i++) *pssd++ =  *psss++ ;
    break ;
    
  case TYPE_USHORT :
    pusd = pDst->p.usValue = (unsigned short*) AllocMemory ( sizeof(unsigned short) * pSrc->size);
    assert ( pusd ) ;
    for ( i=0,puss=pSrc->p.usValue; i<pSrc->size; i++) *pusd++ =  *puss++ ;
    break ;
    
  case TYPE_INTEGER :
    pid = pDst->p.iValue = (int*) AllocMemory ( sizeof (int) * pSrc->size);
    assert ( pid ) ;
    for ( i=0,pis=pSrc->p.iValue; i<pSrc->size; i++) *pid++ =  *pis++ ;
    break ;

  case TYPE_LONG :
    psld = pDst->p.slValue = (long*) AllocMemory ( sizeof (long) * pSrc->size);
    assert ( psld ) ;
    for ( i=0,psls=pSrc->p.slValue; i<pSrc->size; i++) *psld++ =  *psls++ ;
    break ;
    
  case TYPE_ULONG :
    puld = pDst->p.ulValue = (unsigned long*) AllocMemory ( sizeof (unsigned long) * pSrc->size);
    assert ( puld ) ;
    for ( i=0,puls=pSrc->p.ulValue; i<pSrc->size; i++) *puld++ =  *puls++ ;
    break ;
    
  case TYPE_FLOAT :
    pfd = pDst->p.fValue = (float*) AllocMemory ( sizeof ( float ) * pSrc->size ) ;
    assert ( pfd ) ;
    for ( i=0,pfs=pSrc->p.fValue; i<pSrc->size; i++) *pfd++ =  *pfs++ ;
    break ;
    
  case TYPE_DOUBLE :
    pdd = pDst->p.dValue = (double*) AllocMemory ( sizeof ( double ) * pSrc->size ) ;
    assert ( pdd ) ;
    for ( i=0,pds=pSrc->p.dValue; i<pSrc->size; i++) *pdd++ =  *pds++ ;
    break ;
    
  case TYPE_HANDLE :
    phd = pDst->p.hValue = (void**) AllocMemory ( sizeof ( void* ) * pSrc->size ) ;
    assert ( phd ) ;
    for ( i=0,phs=pSrc->p.hValue; i<pSrc->size; i++) *phd++ =  *phs++ ;
    break ;
  }

  return pDst ;
}

sData *gHyp_data_copyAll ( sData *pSrc )
{
  /* Description:
   *
   *	Make a copy of a variable and its values (recursively).
   *
   * Arguments:
   *
   *	pSrc				[R]
   *	- pointer to variable
   *
   * Return value:
   *
   *	Pointer to copy of variable
   *
   *
   */
  
  sData
    *pThisData,
    *pNextData,
    *pDstChild,
    *pDst ;

  if ( pSrc->tokenType == TOKEN_REFERENCE && pSrc->p.rValue != NULL ) 
    pSrc = *pSrc->p.rValue ;
  
  /* First copy the source */
  pDst = gHyp_data_copy ( pSrc ) ;
  
  /* Second, copy all values of the source list */
  for (	pThisData = pSrc->pData ;
	pThisData ;
	pThisData = pNextData ) {

    pNextData = pThisData->pNext ;	

    /* Copy first, then append. */
    pDstChild = gHyp_data_copyAll ( pThisData ) ;
    gHyp_data_append ( pDst, pDstChild ) ;
 
    /* To handle rings, we're done when we're back at the first element */
    if ( pNextData == pSrc->pData ) break ;
  }

  /* If the source was hashed, then so must be the destination */
  if ( pSrc->pHash ) gHyp_data_setHashed ( pDst, TRUE ) ;

  return pDst ;
}


void gHyp_data_moveValues ( sData *pDst, sData *pSrc )
{
  sData
    *pValue ;

  int
    ss,
    context ;

  if ( gHyp_data_getDataType ( pDst ) > TYPE_STRING ||
       gHyp_data_getDataType ( pSrc ) > TYPE_STRING ) 
    return ;


  /* List to list */
  pValue = NULL ;
  ss = -1 ; context = -1 ;
  while ( (pValue = gHyp_data_nextValue ( pSrc, 
					  pValue, 
					  &context,
					  ss ) ) ) {
    gHyp_data_detach ( pValue ) ;
    gHyp_data_append ( pDst, pValue ) ;
    pValue = NULL ;
    context = -1 ;
  }
  return ;
}


void gHyp_data_moveValuesR ( sData *pDst, sData *pSrc )
{
  sData
    *pValue ;

  int
    ss,
    context ;

  if ( gHyp_data_getDataType ( pDst ) > TYPE_STRING ||
       gHyp_data_getDataType ( pSrc ) > TYPE_STRING ) 
    return ;


  /* List to list */
  pValue = NULL ;
  ss = -1 ; context = -1 ;
  while ( (pValue = gHyp_data_nextValue ( pSrc, 
					  pValue, 
					  &context,
					  ss ) ) ) {
    gHyp_data_detach ( pValue ) ;
    gHyp_data_insert ( pDst, pValue ) ;
    pValue = NULL ;
    context = -1 ;
  }
  return ;
}


void gHyp_data_copyValues ( sData *pDst, sData *pSrc )
{
  /* Description:
   *
   *	Copy from pSrc to pDst, including any children.
   *	Currently, pSrc must be a list variable, called only from
   *	gHyp_env_map and gHyp_env_mergeData
   *
   * Arguments:
   *
   *	pDst					[W]
   *	- pointer to destination of values
   *
   *	pSrc					[R]
   *	- pointer to source of values
   *
   * Return value:
   *
   *	none
   *
   */
  sData	
    *pThisData,
    *pValue ;

  sBYTE
    dataType,
    tokenType ;

  int
    ss,
    context ;

  sLOGICAL
    isSrcVector ;

  /* For references, recursively find the variable */
  if ( pDst->tokenType == TOKEN_REFERENCE && pDst->p.rValue != NULL ) {
    gHyp_data_copyValues ( *pDst->p.rValue, pSrc ) ;    
    return ;
  }

  assert ( gHyp_data_getDataType ( pDst ) <= TYPE_STRING ) ;

  /* First delete any destination values */
  gHyp_data_deleteValues ( pDst ) ;
  
  /* Second, copy all values from the source list */
  pThisData = NULL ;
  ss = pSrc->ss ; 
  context = -1 ;
  dataType = gHyp_data_getDataType ( pSrc ) ; 
  tokenType = gHyp_data_getTokenType ( pSrc ) ;
  isSrcVector = ( tokenType == TOKEN_VARIABLE &&
		  dataType > TYPE_STRING ) ;
   while ( (pThisData = gHyp_data_nextValue ( pSrc, 
					     pThisData,
					     &context,
					     ss )) ) { 
	
    if ( isSrcVector ) {
      
      pValue = gHyp_data_new ( NULL ) ;
      gHyp_data_newConstant ( pValue,
			      dataType, 
			      pSrc, 
			      context ) ;
    }
    else 
      /* Copy first, then append. */
      pValue = gHyp_data_copyAll ( pThisData ) ;

    gHyp_data_append ( pDst, pValue ) ;
    
  }

  return ;

}

static void lHyp_data_delete ( sData *pData )
{
  /* Description:
   *
   *	Delete a data object, but not its values.
   *	(Used only by gHyp_data_delete)
   *
   * Arguments:
   *
   *	pData			[R]
   *	- data object to delete
   *
   * Return value:
   *
   *	none
   *
   */

  if ( pData->pHash ) gHyp_hash_delete ( pData->pHash ) ;
  pData->pHash = NULL ;

  if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
  pData->p.value = NULL ;

  /* If there was an attached object, delete it. */
  if ( pData->pObject ) {
    if ( pData->pfDelete )
      pData->pfDelete ( pData->pObject ) ;
    else
      ReleaseMemory ( pData->pObject ) ;
  }

  /*
  if ( pData->pStrVal )
  *  gHyp_util_debug("%p(%s)free",pData,pData->pStrVal);
  else
  *  gHyp_util_debug("%p(?)free",pData);
  */
  if ( pData->pStrVal != NULLstring ) ReleaseMemory ( pData->pStrVal ) ;
  pData->pStrVal = NULLstring ;
  pData->strlen = 0 ;

  /* Finally, delete the data area itself */
  ReleaseMemory ( pData ) ;
  return ;
}

void gHyp_data_delete ( sData *pData )
{
  /* Description:
   *
   *	Delete (recursively) a data object.
   *
   * Arguments:
   *
   *	pData			[R]
   *	- data object to delete
   *
   * Return value:
   *
   *	none
   *
   *
   */
   
  /* For references, do not recursively find the variable */

  gHyp_data_deleteValues ( pData ) ;
  /*gHyp_util_logInfo("Deleting %p/%s",pData,gHyp_data_print ( pData ) ) ;*/
  lHyp_data_delete ( pData ) ;
  return ;
}   

void gHyp_data_deleteValues ( sData *pParent )
{
  /* Description:
   *
   *	Delete all values of a variable (recusively).
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to variable
   *
   * Return value:
   *
   *	none
   *
   */

  sData	
    *pThisData,
    *pNextData ;

  sHash
    *pHash,
    *pNext ;

  int
    i ;

  if ( pParent->pHash ) {

    /* Delete all variables */
    for ( i=0; i<VARIABLE_HASH_TABLE_SIZE; i++ ) {
      for ( pHash = (*pParent->pHash)[i]; pHash; pHash=pNext ) {
	pNext = gHyp_hash_next ( pHash ) ;
	pThisData = (sData*) gHyp_hash_object ( pHash ) ;
	/*gHyp_util_logInfo("Deleting hash %p/%s",pThisData,gHyp_data_print ( pThisData ));*/
	gHyp_data_delete ( pThisData ) ;
	gHyp_hash_free ( pHash ) ;
      }
      (*pParent->pHash)[i] = NULL ;
    }
    pParent->pData = NULL ;
    pParent->size = 0 ;
    pParent->ss = -1 ;
    pParent->isDynamic = TRUE ;
  }
  else if ( pParent->pData ) {

    for (	pThisData = pParent->pData ;
		pThisData ;
		pThisData = pNextData ) {

      pNextData = pThisData->pNext ;
      /*
      gHyp_util_logInfo(
	"Deleting list %p/%s",pThisData,gHyp_data_print ( pThisData ));
      */
      gHyp_data_delete ( pThisData ) ;

      /* To handle rings, we're done when we're back at the first element */
      if ( pNextData == pParent->pData ) break ;
    }
    pParent->pData = NULL ;
    pParent->size = 0 ;
    pParent->ss = -1 ;
    pParent->isDynamic = TRUE ;
  }
  else if ( pParent->p.value != NULL &&
	    pParent->tokenType == TOKEN_VARIABLE &&
	    pParent->dataType > TYPE_STRING ) {
    /* Delete any vector values */
    ReleaseMemory ( pParent->p.value ) ;
    pParent->p.value = NULL ;
    pParent->size = 0 ;
    pParent->ss = -1 ;
    pParent->isDynamic = TRUE ;
  }
  return ;
}

sData* gHyp_data_getParent ( sData *pData )
{
  /* For references, recursively find the variable */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL ) 
    return gHyp_data_getParent ( *pData->p.rValue ) ;

  return pData->pParent ;
}

sLOGICAL gHyp_data_isSibling ( sData *pData, sData *pChild ) 
{
  sData 
    *pParent = NULL ;

  while ( 1 ) {
    /* Get parent of child */
    if ( pChild == pData ) return TRUE ;

    pParent = gHyp_data_getParent ( pChild ) ;
    if ( pParent == pChild ) return FALSE ;
    pChild = pParent ;
  }

}

sData* gHyp_data_getFirst ( sData *pParent )
{
  /* Description:
   *
   *	Get the first value of a variable list 
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to variable
   *
   * Return value:
   *
   *	Pointer to first value.  If none exists, returns NULL
   *
   */
  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL )
    return gHyp_data_getFirst ( *pParent->p.rValue ) ;    

  return pParent->pData ;
}

sData* gHyp_data_getLast ( sData *pParent )
{
  /* Description:
   *
   *	Get the last value of a variable list 
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to variable
   *
   * Return value:
   *
   *	Pointer to last value.  If none exists, returns NULL
   *
   */

  sData	*pData ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL )
    return gHyp_data_getLast ( *pParent->p.rValue ) ;
  
  pData = pParent->pData ;

  if ( pData ) pData = pData->pPrev ;

  return pData ;
}

sData* gHyp_data_getNext ( sData *pData )
{
  /* Description:
   *
   *	Get the next member of a list.
   *
   * Arguments:
   *
   *	pData						[R]
   *	- pointer to a data member in a list
   *
   * Return value:
   *
   *	Pointer to next member in the list, or NULL
   *
   *
   */
  /* For references, recursively find the variable */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL ) 
    return gHyp_data_getNext ( *pData->p.rValue ) ;
  
  return pData->pNext ;
}

sData* gHyp_data_getPrev ( sData *pData )
{
  /* Description:
   *
   *	Get the previous member of a list.
   *
   * Arguments:
   *
   *	pData						[R]
   *	- pointer to a data member in a list
   *
   * Return value:
   *
   *	Pointer to previous member in the list, or NULL
   *
   *
   */
  /* For references, recursively find the variable */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL ) 
    return gHyp_data_getPrev ( *pData->p.rValue ) ;
  
  return pData->pPrev ;
}

int gHyp_data_getInt ( sData *pData, int ss, sLOGICAL recurse )
{
  /* Description:
   *
   *	Get the integer value of a data object at element pointed
   *	to by the subscript ss
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   *	ss				[R]
   *	- subscriptpToken
   *
   *	recurse			[R]
   *	- if TRUE, get value recursively
   *
   * Return value:
   *
   *    integer value
   *
   *
   */

  unsigned long
    ul ;

  long int
    i = 0 ;

  int
    j ;

  sData
    *pRef,
    *pValue ;

  sBYTE
    dataType ;

  pRef = gHyp_data_getReference ( pData ) ;
  if ( pRef ) pData = pRef ;
  pValue =  gHyp_data_getValue ( pData, ss, recurse ) ;
  if ( !pValue ) return 0 ;

  dataType = pValue->dataType ;
  
  /* If gHyp_data_getValue did a recurse on a list variable and got a value,
   * then do not recurse any more and re-set the subscript.
   */
  if ( recurse && 
       pValue != pData ) {
    recurse = FALSE ;
    ss = -1 ;
  }

  /* If the value is a vector and we do not want to recurse it, then
   * get the label and treat it like a string.
   */
  if ( !recurse && 
       pValue->tokenType == TOKEN_VARIABLE &&
       pValue->dataType > TYPE_STRING ) 
    dataType = TYPE_STRING ;
  
  if ( ss == -1 || pValue->tokenType == TOKEN_CONSTANT ) ss = 0 ;

  switch ( dataType ) { 

  case TYPE_LIST :
  case TYPE_STRING :
 
   /* See if string converts to an integer - otherwise its 0 */
    j = strlen ( pValue->pStrVal ) ;
    
    if ( j > 2 && 
	 strstr ( pValue->pStrVal, "0x" ) == pValue->pStrVal &&
	 strspn ( pValue->pStrVal+2, "0123456789aAbBcCdDeEfF" ) == 
	 (size_t) j-2 ) {
      /* Hex number */
      if ( sscanf ( pValue->pStrVal+2, "%lx", &ul ) != 1 ) 
	i = 0 ;
      else
	i = ul ;
    }		 
    else if ( strspn ( pValue->pStrVal, "0123456789+-" ) == (size_t) j &&
              strspn ( pValue->pStrVal, "+-" ) != (size_t) j ) {
      if ( sscanf ( pValue->pStrVal, "%ld", &ul ) != 1 ) 
	i = 0 ;
      else
	i = (int) ul ;
    }
    else if ( j == 1 )
      /* Single character strings are taken as chars */
      i = *(pValue->pStrVal) ;
    
    else if ( j != 0 && *pValue->pStrVal != '%' ) 
      /* Non-null that do not start with '%' are equal to 1 */
      i = 1 ;
    else
      i = 0 ;

    break ;

  case TYPE_ATTR :
    i = (int) *(pValue->p.aValue+ss) ;
    break ;
    
  case TYPE_CHAR :
    i = (int) *(pValue->p.scValue+ss) ;
    break ;
    
  case TYPE_BYTE :
    i = (int) *(pValue->p.sbValue+ss) ;
    break ;

  case TYPE_UCHAR :
    i = (int) *(pValue->p.ucValue+ss) ;
    break ;

  case TYPE_UBYTE :
    i = (int) *(pValue->p.ubValue+ss) ;
    break ;
    
  case TYPE_BOOLEAN :
    i = (int) *(pValue->p.boValue+ss) ? 1 : 0 ;
    break ;
    
  case TYPE_BINARY :
    i = (int) *(pValue->p.biValue+ss) ;
    break ;
    
  case TYPE_HEX :
    i = (int) *(pValue->p.xValue+ss) ;
    break ;
    
  case TYPE_OCTAL :
    i = (int) *(pValue->p.oValue+ss) ;
    break ;
    
  case TYPE_UNICODE :
    i = (int) *(pValue->p.uValue+ss) ;
    break ;
    
  case TYPE_SHORT :
    i = (int) *(pValue->p.ssValue+ss) ;
    break ;
    
  case TYPE_USHORT :
    i = (int) *(pValue->p.usValue+ss) ;
    break ;
    
  case TYPE_INTEGER :
    i = (int) *(pValue->p.iValue+ss) ;
    break ;
    
  case TYPE_LONG :
    i = (int) *(pValue->p.slValue+ss) ;
    break ;
    
  case TYPE_ULONG :
    i = (int) *(pValue->p.ulValue+ss) ;
    break ;
    
  case TYPE_FLOAT :
    i = (int) *(pValue->p.fValue+ss) ;
    break ;
    
  case TYPE_DOUBLE :
    i = (int) *(pValue->p.dValue+ss) ;
    break ;

  case TYPE_HANDLE :
    i = (int) *(pValue->p.hValue+ss) ;
    break ;

  default:
    i = 0 ;
    break ;
  }
  return i ;
}

unsigned long gHyp_data_getRaw ( sData *pData, int ss, sLOGICAL recurse )
{
  /* Description:
   *
   *	Get the unsigned long value of a data object at element pointed
   *	to by the subscript ss
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   *	ss				[R]
   *	- subscript
   *
   *	recurse			[R]
   *	- if TRUE, get value recursively
   *
   * Return value:
   *
   *    integer value
   *
   */

  unsigned long
    ul ;

  int
    j ;

  sData
    *pRef,
    *pValue ;

  sBYTE
    dataType ;

  pRef = gHyp_data_getReference ( pData ) ;
  if ( pRef ) pData = pRef ;
  pValue =  gHyp_data_getValue ( pData, ss, recurse ) ;
  if ( !pValue ) return 0 ;

  dataType = pValue->dataType ;
  
  /* If gHyp_data_getValue did a recurse on a list variable and got a value,
   * then do not recurse any more and re-set the subscript.
   */
  if ( recurse && 
       pValue != pData ) {
    recurse = FALSE ;
    ss = -1 ;
  }

  /* If the value is a vector and we do not want to recurse it, then
   * treat it like a string.
   */
  if ( !recurse &&  
       pValue->tokenType == TOKEN_VARIABLE &&
       pValue->dataType > TYPE_STRING ) 
    dataType = TYPE_STRING ;

  if ( ss == -1 || pValue->tokenType == TOKEN_CONSTANT ) ss = 0 ;

  switch ( dataType ) { 

  case TYPE_LIST :
  case TYPE_STRING :
 
   /* See if string converts to an integer - otherwise its 0 */
    j = strlen ( pValue->pStrVal ) ;
    
    if ( j > 2 && 
	 strstr ( pValue->pStrVal, "0x" ) == pValue->pStrVal &&
	 strspn ( pValue->pStrVal+2, "0123456789aAbBcCdDeEfF" ) == 
	 (size_t) j-2 ) {
      /* Hex number */
      if ( sscanf ( pValue->pStrVal+2, "%lx", &ul ) != 1 ) ul = 0 ;
      
    }		 
    else if ( strspn ( pValue->pStrVal, "0123456789+-" ) == (size_t) j &&
	      strspn ( pValue->pStrVal, "+-" ) != (size_t) j ) {
      if ( sscanf ( pValue->pStrVal, "%lu", &ul ) != 1 ) ul = 0 ;
    }
    else if ( j == 1 )
      /* Single character strings are taken as chars */
      ul = *(pValue->pStrVal) ;
    
    else if ( j != 0 && *pValue->pStrVal != '%' ) 
      /* Non-null that do not start with '%' are equal to 1 */
      ul = 1 ;
    else
      ul = 0 ;

    break ;

  case TYPE_ATTR :
    ul = (unsigned long) *(pValue->p.aValue+ss) ;
    break ;
    
  case TYPE_CHAR :
    ul = (unsigned long) *(pValue->p.scValue+ss) ;
    break ;
    
  case TYPE_BYTE :
    ul = (unsigned long) *(pValue->p.sbValue+ss) ;
    break ;

  case TYPE_UCHAR :
    ul = (unsigned long) *(pValue->p.ucValue+ss) ;
    break ;

  case TYPE_UBYTE :
    ul = (unsigned long) *(pValue->p.ubValue+ss) ;
    break ;
    
  case TYPE_BOOLEAN :
    ul = (unsigned long) *(pValue->p.boValue+ss) ? 1 : 0 ;
    break ;
    
  case TYPE_BINARY :
    ul = (unsigned long) *(pValue->p.biValue+ss) ;
    break ;
    
  case TYPE_HEX :
    ul = (unsigned long) *(pValue->p.xValue+ss) ;
    break ;
    
  case TYPE_OCTAL :
    ul = (unsigned long) *(pValue->p.oValue+ss) ;
    break ;
    
  case TYPE_UNICODE :
    ul = (unsigned long) *(pValue->p.uValue+ss) ;
    break ;
    
  case TYPE_SHORT :
    ul = (unsigned long) *(pValue->p.ssValue+ss) ;
    break ;
    
  case TYPE_USHORT :
    ul = (unsigned long) *(pValue->p.usValue+ss) ;
    break ;
    
  case TYPE_INTEGER :
    ul = (unsigned long) *(pValue->p.iValue+ss) ;
    break ;
    
  case TYPE_LONG :
    ul = (unsigned long) *(pValue->p.slValue+ss) ;
    break ;
    
  case TYPE_ULONG :
    ul = (unsigned long) *(pValue->p.ulValue+ss) ;
    break ;
    
  case TYPE_FLOAT :
    ul = (unsigned long) *(pValue->p.fValue+ss) ;
    break ;
    
  case TYPE_DOUBLE :
    ul = (unsigned long) *(pValue->p.dValue+ss) ;
    break ;

  case TYPE_HANDLE :
    ul = (unsigned long) *(pValue->p.hValue+ss) ;
    break ;

  default:
    ul = 0 ;
    break ;
  }
  return ul ;
}

double gHyp_data_getDouble ( sData *pData, int ss, sLOGICAL recurse )
{
  /* Description:
   *
   *	Get the double value of a data object at element pointed
   *	to by the subscript ss
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   *	ss				[R]
   *	- subscript
   *
   *	recurse				[R]
   *	- if TRUE, get value recursively
   *
   * Return value:
   *
   *    double value
   *
   */

  int
    j ;

  double
    d = 0.0 ;

  sData
    *pRef,
    *pValue ;

  sBYTE
    dataType ;

  pRef = gHyp_data_getReference ( pData ) ;
  if ( pRef ) pData = pRef ;
  pValue =  gHyp_data_getValue ( pData, ss, recurse ) ;
  if ( !pValue ) return 0.0 ;

  dataType = pValue->dataType ;

  /* If gHyp_data_getValue did a recurse on a list variable and got a value,
   * then do not recurse any more if the value happens to be a vector.
   * Also re-set the subscript since we did the recurse.
   */
  /* If gHyp_data_getValue did a recurse on a list variable and got a value,
   * then do not recurse any more and re-set the subscript.
   */
  if ( recurse && 
       pValue != pData ) {
    recurse = FALSE ;
    ss = -1 ;
  }
 
  /* If the value is a vector and we do not want to recurse it, then
   * treat it like a string.
   */
  if ( !recurse &&  
       pValue->tokenType == TOKEN_VARIABLE &&
       pValue->dataType > TYPE_STRING ) 
    dataType = TYPE_STRING ;

  if ( ss == -1 || pValue->tokenType == TOKEN_CONSTANT ) ss = 0 ;

  switch ( dataType ) { 

  case TYPE_STRING :
  case TYPE_LIST :

    /* All of string - see if it converts to an float - otherwise its 0.0 */
    j = strlen ( pValue->pStrVal ) ;
    if ( strspn ( pValue->pStrVal, "0123456789.eE+-" ) == (size_t) j ) {
      if ( sscanf ( pValue->pStrVal, "%lf", &d ) != 1 ) d = 0.0 ;
    }
    else if ( j == 1 )
      /* Single character strings are taken as chars */
      d = *(pValue->pStrVal) ;
    
    else if ( j != 0 && *pValue->pStrVal != '%' ) 
      /* Non-null that do not start with '%' are equal to 1 */
      d = 1.0 ;
    else
      d = 0.0 ;

    break ;
    
  case TYPE_BYTE :
    d = (double) *(pValue->p.sbValue+ss) ;
    break ;

  case TYPE_ATTR :
    d = (double) *(pValue->p.aValue+ss) ;
    break ;
    
  case TYPE_CHAR :
    d = (double) *(pValue->p.scValue+ss) ;
    break ;
    
  case TYPE_UBYTE :
    d = (double) *(pValue->p.ubValue+ss) ;
    break ;

  case TYPE_UCHAR :
    d = (double) *(pValue->p.ucValue+ss) ;
    break ;
    
  case TYPE_BOOLEAN :
    d = (double) *(pValue->p.boValue+ss) ? 1 : 0 ;
    break ;
    
  case TYPE_BINARY :
    d = (double) *(pValue->p.biValue+ss) ;
    break ;
    
  case TYPE_HEX :
    d = (double) *(pValue->p.xValue+ss) ;
    break ;
    
  case TYPE_OCTAL :
    d = (double) *(pValue->p.oValue+ss) ;
    break ;
    
  case TYPE_UNICODE :
    d = (double) *(pValue->p.uValue+ss) ;
    break ;
    
  case TYPE_SHORT :
    d = (double) *(pValue->p.ssValue+ss) ;
    break ;
    
  case TYPE_USHORT :
    d = (double) *(pValue->p.usValue+ss) ;
    break ;
    
  case TYPE_INTEGER :
    d = (double) *(pValue->p.iValue+ss) ;
    break ;
    
  case TYPE_LONG :
    d = (double) *(pValue->p.slValue+ss) ;
    break ;
    
  case TYPE_ULONG :
    d = (double) *(pValue->p.ulValue+ss) ;
    break ;
    
  case TYPE_FLOAT :
    d = (double) *(pValue->p.fValue+ss) ;
    break ;
    
  case TYPE_DOUBLE :
    d = (double) *(pValue->p.dValue+ss) ;
    break ;
    
  case TYPE_HANDLE :
    d = (double) (int) *(pValue->p.hValue+ss) ;
    break ;

  default:
    d = 0.0 ;
    break ;
  }
  return d ;
}

sLOGICAL gHyp_data_getBool ( sData *pData, int ss, sLOGICAL recurse )
{
  return gHyp_data_getInt ( pData, ss, recurse ) ? 1 : 0 ;
}

void *gHyp_data_getHandle ( sData *pData, int ss, sLOGICAL recurse  )
{
  return (void*) gHyp_data_getRaw ( pData, ss, recurse ) ;
}

static int lHyp_data_getStr ( sData *pData, 
			      char *pStrBuf, 
			      int maxlen,
			      int ss,
			      sLOGICAL recurse,
			      sLOGICAL isExternal,
			      sLOGICAL isForMsg,
			      sLOGICAL isForXML )
{
  /* Description:
   *
   *	Gets the string value of a data object at index ss
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   *	pStr				[W]
   *	- pointer to string buffer
   *
   *	maxlen				[R]
   *	- length of pStr
   *
   *	ss				[R]
   *	- subscript
   *
   *	recurse				[R]
   *	- if TRUE, get value recursively
   *
   * Return value:
   *
   *	Pointer to pStr
   *
   */
  
  char
    *pStr = pStrBuf ;

  sData
    *pRef,
    *pValue ;

  sBYTE
    dataType ;

  int
    strLen ;

  pRef = gHyp_data_getReference ( pData ) ;
  if ( pRef ) pData = pRef ;
  pValue =  gHyp_data_getValue ( pData, ss, recurse ) ;

  if ( !pValue ) return -1 ;
  dataType = pValue->dataType ;
  
  /* If gHyp_data_getValue did a recurse on a list variable and got a value,
   * then do not recurse any more and re-set the subscript.
   */
  if ( recurse && 
       pValue != pData ) {
    recurse = FALSE ;
    ss = -1 ;
  }

  /* If the value is a vector and we do not want to recurse it, then
   * treat it like a string.
   */
  if ( !recurse &&  
       pValue->tokenType == TOKEN_VARIABLE &&
       pValue->dataType > TYPE_STRING ) 
    dataType = TYPE_STRING ;

  if ( ss == -1 || pValue->tokenType == TOKEN_CONSTANT ) ss = 0 ;

  switch ( dataType ) { 

  case TYPE_STRING :
  case TYPE_LIST :
    if ( isExternal ) {
      strLen = gHyp_util_unparseString ( pStr, 
					 pValue->pStrVal, 
					 pValue->strlen, 
					 maxlen,
					 isForMsg,
					 isForXML,
					 FALSE,
					 "") ;
    }
    else {
      strLen = MIN ( pValue->strlen, maxlen ) ;
      memcpy ( pStr, pValue->pStrVal, strLen ) ;
      *(pStr+strLen) = '\0' ;
    }
    break ;

  case TYPE_ATTR :
    strLen = sprintf ( pStr, "%c", *(pValue->p.aValue+ss) ) ;
    break ;

  case TYPE_CHAR :
    strLen = sprintf ( pStr, "%c", (*(pValue->p.scValue+ss) & 0x7F ) );
    break ;

  case TYPE_UCHAR :
    strLen = sprintf ( pStr, "%c", *(pValue->p.ucValue+ss) );
    break ;

  case TYPE_BYTE :
    strLen = sprintf ( pStr, "%d", *(pValue->p.scValue+ss) );
    break ;
      
  case TYPE_UBYTE :
    strLen = sprintf ( pStr, "%u", *(pValue->p.ucValue+ss) ) ;
    break ;
    
  case TYPE_BOOLEAN :
    strLen = sprintf ( pStr, "%u", *(pValue->p.boValue+ss) ? 1 : 0 ) ;
    break ;
    
  case TYPE_BINARY :
    strLen = sprintf ( pStr, "0x%02x", *(pValue->p.biValue+ss) ) ;
    break ;
    
  case TYPE_HEX :
    strLen = sprintf ( pStr, "0x%08lx", *(pValue->p.xValue+ss) ) ;
    break ;
    
  case TYPE_OCTAL :
    strLen = sprintf ( pStr, "0o%010lo", *(pValue->p.oValue+ss) ) ;
    break ;
    
  case TYPE_UNICODE :
    if ( isForXML )
      /*strLen = sprintf ( pStr, "&#%05hu;", *(pValue->p.uValue+ss) ) ;*/
      strLen = sprintf ( pStr, "&#x%04x;", *(pValue->p.uValue+ss) ) ;
    else
      strLen = sprintf ( pStr, "0u%05hx", *(pValue->p.uValue+ss) ) ;
    break ;
    
  case TYPE_SHORT :
    strLen = sprintf ( pStr, "%d", *(pValue->p.ssValue+ss) ) ;
    break ;
    
  case TYPE_USHORT :
    strLen = sprintf ( pStr, "%u", *(pValue->p.usValue+ss) ) ;
    break ;
    
  case TYPE_INTEGER :
    strLen = sprintf ( pStr, "%d", (int) *(pValue->p.iValue+ss) ) ;
    break ;
    
  case TYPE_LONG :
    strLen = sprintf ( pStr, "%d", (int) *(pValue->p.slValue+ss) ) ;
    break ;
    
  case TYPE_ULONG :
    strLen = sprintf ( pStr, "%u", (int) *(pValue->p.ulValue+ss) ) ;
    break ;
    
  case TYPE_FLOAT :
    strLen = sprintf ( pStr, "%.7g", *(pValue->p.fValue+ss) ) ;
    break ;

  case TYPE_DOUBLE :
    strLen = sprintf ( pStr, "%.12g", *(pValue->p.dValue+ss) ) ;
    break ;
    
  case TYPE_HANDLE :
    strLen = sprintf ( pStr, "0x%p", *(pValue->p.hValue+ss) );
    if ( *(pStr+3)=='x' ) 
      strLen = sprintf ( pStr, "%p", *(pValue->p.hValue+ss) );
    break ;

  default:
    strLen = sprintf ( pStr, "%c", '?' );
    break ;
  }
  return strLen ;
}

int gHyp_data_getStr ( sData *pData, 
			 char *pStrBuf, 
			 int maxlen,
			 int ss,
			 sLOGICAL recurse ) 
{
  return lHyp_data_getStr ( pData, 
			    pStrBuf, 
			    maxlen,
			    ss,
			    recurse,
			    FALSE,
			    FALSE,
			    FALSE ) ;
}

int gHyp_data_getStr_external ( sData *pData, 
			char *pStrBuf, 
			int maxlen,
			int ss,
			sLOGICAL recurse ) 
{
  return lHyp_data_getStr ( pData, 
			    pStrBuf, 
			    maxlen,
			    ss,
			    recurse,
			    TRUE,
			    FALSE,
			    FALSE ) ;
}

int gHyp_data_getStr_nonulls ( sData *pData, 
			char *pStrBuf, 
			int maxlen,
			int ss,
			sLOGICAL recurse ) 
{
  int
    i,
    n ;
  
  n = lHyp_data_getStr ( pData, 
			 pStrBuf, 
			 maxlen,
			 ss,
			 recurse,
			 FALSE,
			 FALSE,
			 FALSE ) ;

   /* Replace nulls with spaces */
   for ( i=0; i<n; i++ )
     if ( pStrBuf[i] == '\0' ) pStrBuf[i] = ' ' ;

   return n ;

  /* Remove nulls *
  k = 0 ;
  for ( i=0; i<n; i++ ) {
    if ( pStrBuf[i] != '\0' ) {
      if ( i > k ) pStrBuf[k] = pStrBuf[i] ;
      k++ ;
    }
  }
  pStrBuf[k] = '\0' ;
  return k ;
  */
}

int gHyp_data_getStr_msg ( sData *pData, 
			char *pStrBuf, 
			int maxlen,
			int ss,
			sLOGICAL recurse ) 
{
  return lHyp_data_getStr ( pData, 
			    pStrBuf, 
			    maxlen,
			    ss,
			    recurse,
			    TRUE,
			    TRUE,
			    FALSE ) ;
}


int gHyp_data_getStr_xml ( sData *pData, 
			char *pStrBuf, 
			int maxlen,
			int ss,
			sLOGICAL recurse ) 
{
  return lHyp_data_getStr ( pData, 
			    pStrBuf, 
			    maxlen,
			    ss,
			    recurse,
			    TRUE,
			    FALSE,
			    TRUE ) ;
}

char* gHyp_data_getLabel ( sData *pData )
{
  /* Description:
   *
   *	Get the string value of a data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   * Return value:
   *
   *	Pointer to null terminated string
   *
   */
  return pData->pStrVal ;
}

sBYTE gHyp_data_dataLen ( sData *pData )
{
  /* Description:
   *
   *	Returns the data len of a data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   * Return value:
   *
   *	See HYPDEF.H	
   *
   *
   */
  switch ( pData->dataType ) {

  case TYPE_LIST :
  case TYPE_STRING :
    return pData->strlen ;
  case TYPE_ATTR :
    return sizeof ( char ) ;
  case TYPE_CHAR :
    return sizeof ( char ) ;
  case TYPE_BYTE :
    return sizeof ( char ) ;    
  case TYPE_UCHAR :
    return sizeof ( unsigned char ) ;
  case TYPE_UBYTE :
    return sizeof ( unsigned char ) ;    
  case TYPE_BOOLEAN :
    return  sizeof ( unsigned char ) ;
  case TYPE_BINARY :
    return sizeof ( unsigned char ) ;
  case TYPE_HEX :
    return sizeof ( unsigned long ) ;
  case TYPE_OCTAL :
    return sizeof ( unsigned long ) ;
  case TYPE_UNICODE :
    return sizeof ( unsigned short ) ;
  case TYPE_SHORT :
    return sizeof ( short ) ;
  case TYPE_USHORT :
    return sizeof ( unsigned short ) ;
  case TYPE_INTEGER :
    return sizeof ( int ) ;
  case TYPE_LONG :
    return sizeof ( long ) ;
  case TYPE_ULONG :
    return sizeof ( unsigned long ) ;
  case TYPE_FLOAT :
    return sizeof ( float ) ;    
  case TYPE_DOUBLE :
    return sizeof ( double ) ;    
  case TYPE_HANDLE :
    return sizeof ( void* ) ;
  }
  return 0 ;  
}



sBYTE gHyp_data_dataType ( sData *pData )
{
  /* Description:
   *
   *	Returns the data type of a data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   * Return value:
   *
   *	See HYPDEF.H	
   *
   */
  return pData->dataType ;
}

sBYTE gHyp_data_getDataType ( sData *pData )
{
  /* Description:
   *
   *	Returns the data type of a data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   * Return value:
   *
   *	See HYPDEF.H	
   *
   *
   */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL )
    return gHyp_data_getDataType ( *pData->p.rValue ) ;

  return pData->dataType ;
}

sBYTE gHyp_data_tokenType ( sData *pData )
{
  /* Description:
   *
   *	Returns the token type of a data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   * Return value:
   *
   *	See hypdef.h
   *
   */
  return pData->tokenType ;
}

sBYTE gHyp_data_getTokenType ( sData *pData )
{
  /* Description:
   *
   *	Returns the token type of a data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   * Return value:
   *
   *	See hypdef.h
   *
   */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL )
    return gHyp_data_getTokenType ( *pData->p.rValue ) ;

  return pData->tokenType ;
}

sData* gHyp_data_getAll ( sData 	*pData,
			  sBYTE		*pTokenType,
			  sBYTE		*pDataType,
			  int		*pIntVal,
			  unsigned long	*pUlongVal,
			  double 	*pDoubleVal,
			  sLOGICAL	*pBoolVal,
			  void		**pHandle,
			  char		*pStrBuf,
			  int		*pStrLen,
			  int		maxlen,
			  int		ss,
			  sLOGICAL	recurse )
{
  int 
    strLen=0,
    i=0,
    j ;

  long
    sl = 0 ;

  unsigned long
    u = 0,
    ul = 0,
    x,
    o ;

  short
    sw ;

  unsigned short
    n,
    uw ;

  float
    f = 0.0 ;

  double
    d = 0.0 ;

  sData
    *pRef,
    *pValue ;
  
  char
    a,
    sc,
    sb,
    *pStr = pStrBuf ;

  unsigned char
    uc,
    ub,
    bi,
    bo ;

  void
    *h ;

  sBYTE
    dataType ;

  pRef = gHyp_data_getReference ( pData ) ;
  if ( pRef ) pData = pRef ;
  pValue =  gHyp_data_getValue ( pData, ss, recurse ) ;
  if ( !pValue ) return NULL ;

  dataType = pValue->dataType ;

  /* If gHyp_data_getValue did a recurse on a list variable and got a value,
   * then do not recurse any more and re-set the subscript.
   */
  if ( recurse && 
       pValue != pData ) {
    recurse = FALSE ;
    ss = -1 ;
  }

  /* If the value is a vector and we do not want to recurse it, then
   * treat it like a string.
   */
  if ( !recurse &&  
       pValue->tokenType == TOKEN_VARIABLE &&
       pValue->dataType > TYPE_STRING ) 
    dataType = TYPE_STRING ;
  
  if ( ss == -1 || pValue->tokenType == TOKEN_CONSTANT ) ss = 0 ;

  switch ( dataType ) { 

  case TYPE_LIST :
  case TYPE_STRING :

    j = strlen ( pValue->pStrVal ) ;

    if ( j > 2 && 
	 strstr ( pValue->pStrVal, "0x" ) == pValue->pStrVal &&
	 strspn ( pValue->pStrVal+2, "0123456789aAbBcCdDeEfF" ) == 
	 (size_t) j-2 ) {
      /* Hex number */
      if ( sscanf ( pValue->pStrVal+2, "%lx", &ul ) != 1 ) {
	i = 0 ;
	u = 0 ;
	d = 0.0 ;
	h = NULL ;
      }
      else {
	i = (int) ul ;
	u = ul ;
	d = (double) ul ;
	h = (void*) ul ;
      }
    }		 
    else if ( strspn ( pValue->pStrVal, "0123456789+-" ) == (size_t) j &&
	    strspn ( pValue->pStrVal, "+-" ) != (size_t) j ) {
      if ( sscanf ( pValue->pStrVal, "%ld", (unsigned long*) &sl ) != 1 ) {
	i = 0 ;
	u = 0 ;
	d = 0.0 ;
	h = NULL ;
      }
      else {
	i = sl ;
	u = (unsigned long ) sl ;
	d = (double) sl ;
	h = (void*) sl ;
      }
    }
    else if ( strspn ( pValue->pStrVal, "0123456789.eE+-" ) == (size_t) j ) {
      if ( sscanf ( pValue->pStrVal, "%lf", &d ) != 1 ) d = 0.0 ;
      i = (int) d ;
      u = (unsigned long) d ;
      h = (void*)(int) d ;
    }
    else if ( j == 1 ) {
      /* Single character strings are taken as chars */
      i = *(pValue->pStrVal) ;
      u = i ;
      d = i ;
      h = (void*) i ;
    }
    else if ( j != 0 && *pValue->pStrVal != '%' ) {
      /* Non-null that do not start with '%' are equal to 1 */
      i = 1 ;
      u = 1 ;
      d = 1.0 ;
      h = (void*) 1 ;
    }
    else {
      i = 0 ;
      u = 0 ;
      d = 0.0 ;
      h = NULL ;
    }
    strLen = MIN ( pValue->strlen, maxlen ) ;
    memcpy ( pStr, pValue->pStrVal, strLen ) ;
    *(pStr+strLen) = '\0' ;

    break ;

  case TYPE_BYTE :
    sb = *(pValue->p.sbValue+ss) ;
    d = (double) sb ;
    strLen = sprintf ( pStr, "%d", sb );
    i = (int) sb ;
    u = (unsigned long) sb ;
    h = (void*) (unsigned long) sb ;
    break ;

  case TYPE_ATTR :
    a = *(pValue->p.aValue+ss) ;
    d = (double) a ;
    strLen = sprintf ( pStr, "%c", a ) ;
    i = (int) a ;
    u = (unsigned long) a ;
    h = (void*) (unsigned long) a ;
    break ;

  case TYPE_CHAR :
    sc = (*(pValue->p.scValue+ss) & 0x7F) ;
    d = (double) sc ;
    strLen = sprintf ( pStr, "%c", sc ) ;
    i = (int) sc ;
    u = (unsigned long) sc ;
    h = (void*) (unsigned long) sc ;
    break ;

  case TYPE_UBYTE :
    ub = *(pValue->p.ubValue+ss) ;
    d = (double) ub ;
    strLen = sprintf ( pStr, "%u", ub ) ;
    i = (int) ub ;
    u = ub ;
    h = (void*) (unsigned long) ub ;
    break ;

  case TYPE_UCHAR :
    uc = *(pValue->p.ucValue+ss) ;
    d = (double) uc ;
    strLen = sprintf ( pStr, "%c", uc ) ;
    i = (int) uc ;
    u = uc ;
    h = (void*) (unsigned long) uc ;
    break ;
          
  case TYPE_BOOLEAN :
    bo = *(pValue->p.boValue+ss) ? 1 : 0 ;
    d = (double) bo ;
    strLen = sprintf ( pStr, "%u", bo ) ;
    i = (int) bo ;
    u = (unsigned long) bo ;
    h = (void*) (unsigned long) bo ;
    break ;
          
  case TYPE_BINARY :
    bi = *(pValue->p.biValue+ss) ;
    d = (double) bi ;
    strLen = sprintf ( pStr, "0x%02x", bi ) ;
    i = (int) bi ;
    u = (unsigned long) bi ;
    h = (void*) (unsigned long) bi ;
    break ;
          
  case TYPE_HEX :
    x = *(pValue->p.xValue+ss) ;
    d = (double) x ;
    strLen = sprintf ( pStr, "0x%08lx", x ) ;
    i = (int) x ;
    u = (unsigned long) x ;
    h = (void*) (unsigned long) x ;
    break ;
          
  case TYPE_OCTAL :
    o = *(pValue->p.oValue+ss) ;
    d = (double) o ;
    strLen = sprintf ( pStr, "0o%010lo", o ) ;
    i = (int) o ;
    u = (unsigned long) o ;
    h = (void*) (unsigned long) o ;
    break ;
          
  case TYPE_UNICODE :
    n = *(pValue->p.uValue+ss) ;
    d = (double) n ;
    strLen = sprintf ( pStr, "0u%05hx", n ) ;
    i = (int) n ;
    u = (unsigned long) n ;
    h = (void*) (unsigned long) n ;
    break ;
          
  case TYPE_SHORT :
    sw = *(pValue->p.ssValue+ss) ;
    d = (double) sw ;
    strLen = sprintf ( pStr, "%d", sw ) ;
    i = (int) sw ;
    u = (unsigned long) sw ;
    h = (void*) (unsigned long) sw ;
    break ;
        
  case TYPE_USHORT :
    uw = *(pValue->p.usValue+ss) ;
    d = (double) uw ;
    strLen = sprintf ( pStr, "%u", uw ) ;
    i = (int) uw ;
    u = uw ;
    h = (void*) (unsigned long) uw ;
    break ;
    
  case TYPE_INTEGER :
    i = *(pValue->p.iValue+ss) ;
    u = (unsigned long) i ;
    d = (double) i ;
    strLen = sprintf ( pStr, "%d", (int) i ) ;
    h = (void*) i ;
    break ;
    
  case TYPE_LONG :
    sl = *(pValue->p.slValue+ss) ;
    u = (unsigned long) sl ;
    d = (double) sl ;
    strLen = sprintf ( pStr, "%d", (int) sl ) ;
    i = (int) sl ;
    h = (void*) sl ;
    break ;
    
  case TYPE_ULONG :
    ul = *(pValue->p.ulValue+ss) ;
    u = ul ;
    d = (double) ul ;
    strLen = sprintf ( pStr, "%lu", (unsigned long) ul ) ;
    i = (int) ul ;
    h = (void*) ul ;
    break ;
    
  case TYPE_FLOAT :
    f = *(pValue->p.fValue+ss) ;
    d = (double) f ;
    strLen = sprintf ( pStr, "%.7g", f ) ;
    i = (int) f ;
    u = (unsigned long) f ;
    h = (void*) (unsigned long) f ;
    break ;
    
  case TYPE_DOUBLE :
    d = *(pValue->p.dValue+ss) ;
    strLen = sprintf ( pStr, "%.7g", d ) ;
    i = (int) d ;
    u = (unsigned long) d ;
    h = (void*) (unsigned long) d ;
    break ;
    
  case TYPE_HANDLE :
    u = (unsigned long) *(pValue->p.hValue+ss) ;
    i = (int) u ;
    d = (float) i ;
    strLen = sprintf ( pStr, "0x%p", *(pValue->p.hValue+ss) );
    if ( *(pStr+3)=='x' ) 
      strLen = sprintf ( pStr, "%p", *(pValue->p.hValue+ss) );
    h = (void*) *(pValue->p.hValue+ss) ;
    break ;

  default:
    i = 0 ;
    u = 0 ;
    d = 0.0 ;
    h = NULL ;
    strLen = 0 ;
    break ;
  }

  *pTokenType = pValue->tokenType ;
  *pDataType = pValue->dataType ;
  *pIntVal   = i ;
  *pUlongVal = u ;
  *pDoubleVal = d ;
  *pBoolVal  =  i ? 1 : 0 ;
  *pHandle =  h ;
  *pStrLen = strLen ;

  return pValue ;
}

int gHyp_data_getCount ( sData *pParent )
{
  /* Description:
   *
   *	Returns the number of child data objects in a parent data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to parent data object
   *
   * Return value:
   *
   *	The number of child data object in a parent data object
   */

  sData 
    *pThisData,
    *pNextData ;
  
  int 
    i = 0 ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) 
    return gHyp_data_getCount ( *pParent->p.rValue ) ;    
  
  if ( pParent->tokenType == TOKEN_VARIABLE ) {

    if ( pParent->dataType < TYPE_BYTE ) {

      if ( pParent->pData ) {
	
	for ( pThisData = pParent->pData ;
	      pThisData ;
	    pThisData = pNextData ) {
	  
	  pNextData = pThisData->pNext ;
	  
	  /* Increment count */
	  i++ ;	
	  
	  /* To handle rings, we're done when we're back at the 1rst element */
	  if ( pNextData == pParent->pData ) break ;
	}
      }
      pParent->size = i ;
    }
    else
      /* Vector */
      i = pParent->size ;
  }
  else

    i = 1 ;
  
  return i ;
}


int gHyp_data_check ( sData *pParent )
{
  sData 
    *pThisData,
    *pNextData ;
  
  int 
    i = 0 ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) 
    return gHyp_data_check ( *pParent->p.rValue ) ;    
  
  if ( pParent->tokenType == TOKEN_VARIABLE ) {

    if ( pParent->dataType < TYPE_BYTE ) {

      if ( pParent->pData ) {
	
	assert ( pParent->pData->pPrev->pParent == pParent ) ;

	assert ( pParent->pData->pPrev->pNext == pParent->pData ) ;

	for ( pThisData = pParent->pData ;
	      pThisData ;
	    pThisData = pNextData ) {
	  
	  pNextData = pThisData->pNext ;
	  
	  /* Increment count */
	  i++ ;	

	  
	  /* To handle rings, we're done when we're back at the 1rst element */
	  if ( pNextData == pParent->pData ) break ;
	}
      }
      pParent->size = i ;
    }
    else
      /* Vector */
      i = pParent->size ;
  }
  else

    i = 1 ;
  
  return i ;
}

sLOGICAL gHyp_data_isCountOne ( sData *pParent )
{
  sData 
    *pThisData,
    *pNextData ;
  
  int 
    i = 0 ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) 
    return gHyp_data_isCountOne ( *pParent->p.rValue ) ;    
  
  if ( pParent->tokenType == TOKEN_VARIABLE ) {

    if ( pParent->dataType < TYPE_BYTE ) {

      if ( pParent->pData ) {
	
	for ( pThisData = pParent->pData ;
	      pThisData ;
	    pThisData = pNextData ) {
	  
	  pNextData = pThisData->pNext ;
	  
	  /* Increment count */
	  i++ ;	
	  
	  if ( i > 1 ) return FALSE ;

	  /* To handle rings, we're done when we're back at the 1rst element */
	  if ( pNextData == pParent->pData ) break ;
	}
      }
      pParent->size = i ;
    }
    else
      /* Vector */
      i = pParent->size ;
  }
  else

    i = 1 ;
  
  return ( i == 1 ) ;
}

int gHyp_data_getSubScript ( sData *pData ) 
{
  /* Description:
   *
   *	Returns the subscript of a data object.
   *	If the data object has a list of values, then the subscript
   *	specifies a particular value.  Normally the subscript is set
   *	to -1, which specifies all values.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   * Return value:
   *
   *    subscript value
   *
   */
  return pData->ss ;
}

sLOGICAL gHyp_data_isDynamic ( sData *pData ) 
{
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL )
    return gHyp_data_isDynamic ( *pData->p.rValue ) ;

  return pData->isDynamic ;
}

void gHyp_data_setDynamic ( sData *pData, sLOGICAL isDynamic ) 
{
  pData->isDynamic = isDynamic ;
}

sBYTE *gHyp_data_buffer ( sData *pData, int ss ) 
{

  sBYTE 
    *pBuf ;

  if ( ss < 0 ) ss = 0 ;

  if ( pData->p.value )
    pBuf = pData->p.ubValue+ss ;
  else
    pBuf = (sBYTE*) pData->pStrVal ;

  return pBuf ;
}

int gHyp_data_bufferLen ( sData *pData, int ss ) 
{
  int
    dataLen,
    bufLen ;

  if ( pData->size < 1 ) return 0 ;
  if ( ss < 0 ) ss = 0 ;

  dataLen = gHyp_data_dataLen ( pData ) ;
  if ( pData->p.value )
    bufLen = MAX ( 0, ( pData->size - ss ) * dataLen ) ;
  else
    bufLen = MAX ( 0, dataLen - ss ) ;

  return bufLen ;
}

sData* gHyp_data_getChildBySS ( sData *pParent, int ss )
{
  /* Description:
   *
   *	Returns the i'th child data object in a parent data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to parent data object
   *
   *	ss
   *	- subscript to ss'th child object
   *
   * Return value:
   *
   *	NULL if no ss'th child data object, otherwise pointer to child object
   *
   */
  sData
    *pThisData,
    *pNextData ;

  int
    i = 0 ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) 
    return gHyp_data_getChildBySS ( *pParent->p.rValue, ss ) ;
  
  if ( ss == -1 ) return pParent->pData ;

  for ( pThisData = pParent->pData ;
    	pThisData ;
	pThisData = pNextData ) {
    
    pNextData = pThisData->pNext ;

    /* Found it? */
    if ( i == ss ) return pThisData ;

    /* Increment count */
    i++ ;

    /* To handle rings, we're done when we're back at the first element */
    if ( pNextData == pParent->pData ) break ;
  }
  return NULL ;
}

sData* gHyp_data_getChildByName ( sData *pParent, char *name )
{
  /* Description:
   *
   *	Returns the child data object from the hash table, using key 'name'
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to parent data object
   *
   *	name
   *	- name of child object
   *
   * Return value:
   *
   *	NULL if no child data object, otherwise pointer to child object
   *
   */
  sData
    *pThisData,
    *pNextData ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) 
    return gHyp_data_getChildByName ( *pParent->p.rValue, name ) ;    

  if ( pParent->pHash ) {

    /* Use hash table to lookup name */
    pThisData = (sData*) gHyp_hash_find ( pParent->pHash,
					  VARIABLE_HASH_TABLE_SIZE,
					  name ) ;
  }
  else {

    for ( pThisData = pParent->pData ;
	  pThisData ;
	  pThisData = pNextData ) {
      
      pNextData = pThisData->pNext ;

      /* Found it? */
      if ( strcmp ( name, pThisData->pStrVal ) == 0 ) break ;

      /* To handle rings, we're done when we're back at the first element */
      if ( pNextData == pParent->pData ) {
	pThisData = NULL ;
	break ;
      }
    }
  }
  return pThisData ;
}

sData *gHyp_data_getVariable ( sData *pData )
{
  sData
    *pVariable ;

  if ( gHyp_data_tokenType ( pData ) == TOKEN_VARIABLE ) return pData ;

  /* See if the variable is directly referenced */
  pVariable = gHyp_data_getReference ( pData ) ;

  return pVariable ;
} 

sData* gHyp_data_getReference ( sData *pParent )
{
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) 
    return *pParent->p.rValue ;    
  else
    return NULL ;
}

sLOGICAL gHyp_data_deleteChildByName ( sData *pParent, char *name )
{
  /* Description:
   *
   *	Deletes the child data object from the hash table, using key 'name'
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to parent data object
   *
   *	name
   *	- name of child object
   *
   * Return value:
   *
   *	TRUE if child data object was deleted, FALSE otherwise
   *
   */
  sData
    *pThisData,
    *pNextData ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL )
    return gHyp_data_deleteChildByName ( *pParent->p.rValue, name ) ;    

  if ( pParent->pHash ) {

    /* Use hash table to lookup name */
    pThisData = (sData*) gHyp_hash_remove ( pParent->pHash,
					    VARIABLE_HASH_TABLE_SIZE,
					    name,
					    NULL ) ;
  }
  else {

    for ( pThisData = pParent->pData ;
	  pThisData ;
	  pThisData = pNextData ) {
      
      pNextData = pThisData->pNext ;

      /* Found it? */
      if ( strcmp ( name, pThisData->pStrVal ) == 0 ) break ;

      /* To handle rings, we're done when we're back at the first element */
      if ( pNextData == pParent->pData ) {
	pThisData = NULL ;
	break ;
      }
    }
  }

  if ( pThisData ) {

    if ( pThisData->pPrev != pThisData ) 
      /* Previous sData member exists, join with next sData member */
      pThisData->pPrev->pNext = pThisData->pNext ;
      
    if ( pThisData->pNext != pThisData ) {
      /* A next sData member exists, join with previous sData member */
      pThisData->pNext->pPrev = pThisData->pPrev ;
      if ( pParent->pData == pThisData )
	/* Have parent point to this next member */
	pParent->pData = pThisData->pNext ;
    }
    else {
      /* There was only one value */
      pParent->pData = NULL ;
    }
    
    pThisData->pParent = pThisData ;
    pThisData->pNext = pThisData ;
    pThisData->pPrev = pThisData ;

    /* New delete the data element */
    gHyp_data_delete ( pThisData ) ;

    return TRUE ;
  }
  else
    return FALSE ;
}

sData* gHyp_data_detach ( sData *pData )
{
  sData
    *pParent,
    *pRef ;

  /* For references, recursively find the variable */
  if ( pData->tokenType == TOKEN_REFERENCE && pData->p.rValue != NULL ) {
    /* Delete the reference, because we are detaching the variable */
    pRef = *pData->p.rValue ;
    /*lHyp_data_delete ( pData ) ;*/
    return gHyp_data_detach ( pRef ) ;
  }

  pParent = pData->pParent ;

  if ( pParent != pData ) {

    if ( pParent->pHash ) {

      /* Remove it from the list */
      gHyp_hash_remove ( pParent->pHash,
			 VARIABLE_HASH_TABLE_SIZE,
			 pData->pStrVal,
			 NULL ) ;
    }
      
    if ( pData->pPrev != pData ) 
      /* Previous sData member exists, join with next sData member */
      pData->pPrev->pNext = pData->pNext ;
      
    if ( pData->pNext != pData ) {
      /* A next sData member exists, join with previous sData member */
      pData->pNext->pPrev = pData->pPrev ;
      if ( pParent->pData == pData )
	/* Have parent point to this next member */
	pParent->pData = pData->pNext ;
    }
    else {
      /* There was only one value */
      pParent->pData = NULL ;
    }
    
    pData->pParent = pData ;
    pData->pNext = pData ;
    pData->pPrev = pData ;  
  }
  return pData ;
}

sData* gHyp_data_extractChildByName ( sData *pParent, char *name )
{
  /* Description:
   *
   *	Returns the child data object from the hash table, using key 'name'
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to parent data object
   *
   *	name
   *	- name of child object
   *
   * Return value:
   *
   *	NULL if no child data object, otherwise pointer to child object
   *
   */
  sData
    *pThisData,
    *pNextData ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) 
    return gHyp_data_extractChildByName ( *pParent->p.rValue, name ) ;    

  if ( pParent->pHash ) {

    /* Use hash table to lookup name */
    pThisData = (sData*) gHyp_hash_find ( pParent->pHash,
					  VARIABLE_HASH_TABLE_SIZE,
					  name ) ;
    /* Remove it from the list */
    gHyp_hash_remove ( pParent->pHash,
		       VARIABLE_HASH_TABLE_SIZE,
		       name,
		       NULL ) ;
  }
  else {

    for ( pThisData = pParent->pData ;
	  pThisData ;
	  pThisData = pNextData ) {
      
      pNextData = pThisData->pNext ;

      /* Found it? */
      if ( strcmp ( name, pThisData->pStrVal ) == 0 ) {

	if ( pThisData->pPrev != pThisData ) 
	  /* Previous sData memb1er exists, join with next sData member */
	  pThisData->pPrev->pNext = pThisData->pNext ;
	
	if ( pThisData->pNext != pThisData ) {
	  /* A next sData member exists, join with previous sData member */
	  pThisData->pNext->pPrev = pThisData->pPrev ;
	  if ( pParent->pData == pThisData )
	    /* Have parent point to this next member */
	    pParent->pData = pThisData->pNext ;
	}
	else {
	  /* There was only one value */
	  pParent->pData = NULL ;
	}
	
	pThisData->pParent = pThisData ;
	pThisData->pNext = pThisData ;
	pThisData->pPrev = pThisData ; 
	break ;
      }

      /* To handle rings, we're done when we're back at the first element */
      if ( pNextData == pParent->pData ) {
	pThisData = NULL ;
	break ;
      }
    }
  }
  return pThisData ;
}


sData* gHyp_data_extractChildBySS ( sData *pParent, int ss )
{
  /* Description:
   *
   *	Extract the value of the 'pParent' variable.  Which value depends
   *	on the tokenType of pData and the subscript ss.
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to data object
   *
   *	ss				[R/W]
   *	- subscript
   *
   *
   * Return value:
   *
   *	Pointer to 'index' value, or NULL if it does not exist
   *
   */
  sData
    *pThisData,
    *pNextData ;

  int
    i = 0 ;

  /* For references, recursively find the variable */
  if ( pParent->tokenType == TOKEN_REFERENCE && pParent->p.rValue != NULL ) 
    return gHyp_data_extractChildBySS ( *pParent->p.rValue, ss ) ;

  if ( ss == -1 ) ss = 0 ;

  for ( pThisData = pParent->pData ;
    	pThisData ;
	pThisData = pNextData ) {
    
    pNextData = pThisData->pNext ;

    /* Found it? */
    if ( i == ss ) {

      /* Extract this value from the parent */

      if ( pThisData->pPrev != pThisData ) 
	/* Previous sData memb1er exists, join with next sData member */
	pThisData->pPrev->pNext = pThisData->pNext ;
    
      if ( pThisData->pNext != pThisData ) {
	/* A next sData member exists, join with previous sData member */
	pThisData->pNext->pPrev = pThisData->pPrev ;
	if ( pParent->pData == pThisData )
	  /* Have parent point to this next member */
	  pParent->pData = pThisData->pNext ;
      }
      else {
	/* There was only one value */
	pParent->pData = NULL ;
      }

      pThisData->pParent = pThisData ;
      pThisData->pNext = pThisData ;
      pThisData->pPrev = pThisData ;

      return pThisData ;
    }
    /* Increment count */
    i++ ;

    /* To handle rings, we're done when we're back at the first element */
    if ( pNextData == pParent->pData ) break ;
  }
  return NULL ;
}

sData* gHyp_data_getValue ( sData *pData, int ss, sLOGICAL recurse )
{
  /* Description:
   *
   *	Get the value of the 'pData' variable.  Which value depends
   *	on the tokenType of pData and the subscript ss.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   *	ss				[R/W]
   *	- subscript
   *
   *	recurse				[R]
   *	- if TRUE, get value recursively
   *
   * Return value:
   *
   *	Pointer to 'index' value, or NULL if it does not exist
   *
   */
  switch ( pData->tokenType ) {
  
  case TOKEN_REFERENCE :
    /* If the referenced value is without a pointer (ie: &variable),
     * then return pData as the value, otherwise recursively access the
     * variable pointed to by the reference.
     */
    if ( pData->p.rValue != NULL )
      return gHyp_data_getValue ( *pData->p.rValue, ss, recurse ) ;
    else
      return pData ;
    break ;

  case TOKEN_VARIABLE :

    if ( pData->dataType < TYPE_BYTE ) {
      /* Get the list element, or the first element if index == -1 */
      if ( recurse )
	return gHyp_data_getChildBySS ( pData, ss ) ;
      else
	return pData ;
    }
    else {
      /* Vector, check that the index is within bounds */
      if ( recurse ) {
	if ( ss == -1 ) ss = 0 ;
	if ( ss < pData->size ) return pData ;
      }
      else
	return pData ;
    }
    break ;
      
  case TOKEN_RAW :
    /*gHyp_util_debug("Getting raw value, ss=%d, pData=%s",ss,gHyp_data_print(pData));*/
    if ( recurse ) {
      if ( ss == -1 ) ss = 0 ;
      /*if ( ss < (int) strlen ( pData->pStrVal ) ) return pData ;*/
      if ( ss < pData->strlen ) return pData ;
    }
    else
      return pData ;
    break ;
    
  case TOKEN_LITERAL :
  case TOKEN_UNIDENTIFIED :
  case TOKEN_CONSTANT :
    /* Just return the value */
    return pData ;

  default:
    return NULL ;
  }
  return NULL ;
}

sData *gHyp_data_nextValue ( sData *pParent, 
			     sData *pLastValue, 
			     int* pContext,
			     int ss )
{
  /* Description:
   *
   *	Returns next child data object.
   *	When this routine is first called, pLastValue should be set to NULL.
   *	Subsequent calls to this routine should pass the last child data
   *	object returned by this routine.
   *
   * Arguments:
   *
   *	pParent					[R]
   *	- pointer to parent data object
   *
   *	pLastValue				[R]
   *	- pointer to last value retrieved by this function
   *
   *	pContext				[R/W]
   *	- the orginal value for pParent->ss
   *
   * Return value:
   *
   *	Pointer to next value
   *
   *
   */

  sData
    *pData ;

  switch ( pParent->tokenType ) {
    
  case TOKEN_REFERENCE :
    
    if ( pParent->p.rValue != NULL ) {
      return gHyp_data_nextValue ( *pParent->p.rValue,
				   pLastValue,
				   pContext,
				   ss ) ;
    }
    break ;
 
  case TOKEN_UNIDENTIFIED :
  case TOKEN_LITERAL :
  case TOKEN_CONSTANT :
    
    if ( pLastValue == NULL ) {
      if ( ss == -1 ) 
	*pContext = 0 ;
      else
	*pContext = ss ; 
      
      if ( *pContext == 0 )
	return pParent ;
      else
	*pContext = -2 ;
    }
    break ;
    

  case TOKEN_VARIABLE :
          
    if ( pLastValue == NULL ) {
      
      /* First call to this function. Setup. */
      if ( ss == -1 )
	/* All requested */
	*pContext = 0 ;
      else
	/* One requested */
	*pContext = ss ;

      if ( pParent->dataType < TYPE_BYTE )
	pData = gHyp_data_getChildBySS ( pParent, *pContext ) ;
      else
	pData = (*pContext>=0 && *pContext<pParent->size) ? pParent : NULL ;
  
      /* When asking for a specific element, if none then out of bounds */
      if ( ss != -1 && !pData ) *pContext = -2 ;
      
      /* Otherwise return the next element or MULL at the end of the list */
      return pData ;
      
    }
    else {

      /* Not the first call to this function.
       * Only valid when we ask for all values (ss==-1).
       */
      if ( ss != -1 ) return NULL ;

      /* Advance to next array position */
      *pContext += 1 ;

      if ( pParent->dataType < TYPE_BYTE ) { 
	/* Get next value if requested, do not go past end of ring */
	if ( pLastValue->pNext != pLastValue->pParent->pData )
	  return pLastValue->pNext ;
      }
      else {
      	
	/* Not the first value and all values are requested */
	if ( *pContext < pParent->size ) return pParent ;
      }
    }
    break ;
  }
  return NULL ;
}

void gHyp_data_setLabel ( sData *pData, char *pStrVal )
{
  /* Description:
   *
   *	Set the label value of a data object
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to data object
   *
   * Return value:
   *
   *	Pointer to null terminated string
   * 
   */
  int 
    n ;

  assert ( pStrVal ) ;
  if ( pData->pStrVal != pStrVal &&
       strcmp ( pData->pStrVal, pStrVal ) != 0 ) {
    if ( pData->pStrVal != NULLstring ) ReleaseMemory ( pData->pStrVal ) ;
    n = strlen ( pStrVal ) ;
    pData->pStrVal = (char*) AllocMemory ( n + 1 ) ;
    assert ( pData->pStrVal ) ;
    strcpy ( pData->pStrVal, pStrVal ) ;
    pData->strlen = n ;
  }
  return ;
}

void gHyp_data_setStr ( sData *pData, char *pStr )
{
  /* Description:
   *
   *	Sets a data object to a string value.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	pStr				[R]
   *	- string
   *
   *
   * Return value:
   *
   *	none
   *
   */ 
  int
    n ;
    
  if ( pData->pStrVal != pStr ) {

    if ( pData->pStrVal != NULLstring ) ReleaseMemory ( pData->pStrVal ) ;
    if ( pStr ) {
      n = strlen ( pStr ) ;
      pData->pStrVal = (char *) AllocMemory ( n + 1 ) ;
      assert ( pData->pStrVal ) ;
      strcpy ( pData->pStrVal, pStr ) ;
      pData->strlen = n ;
    }
    else {
      pData->pStrVal = NULLstring ;
      pData->strlen = 0 ;
    }
  }
 
  if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
  pData->p.value = NULL ;

  pData->tokenType = TOKEN_LITERAL ;
  pData->dataType = TYPE_STRING ;

  return ;
}

void gHyp_data_setStr_n ( sData *pData, char *pStr, int n )
{
  /* Description:
   *
   *	Sets a data object to a string value.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	pStr				[R]
   *	- string
   *
   *    n
   *    - string length
   *
   * Return value:
   *
   *	none
   *
   */ 

  if ( pData->pStrVal != pStr ) {

    if ( pData->pStrVal != NULLstring ) ReleaseMemory ( pData->pStrVal ) ;
    if ( pStr ) {
      pData->pStrVal = (char*) AllocMemory ( n + 1 ) ;
      memcpy ( pData->pStrVal, pStr, n ) ;
      assert ( pData->pStrVal ) ;
      pData->pStrVal[n] = '\0' ;
      pData->strlen = n ;
    }
    else {
      pData->pStrVal = NULLstring ;
      pData->strlen = 0 ;
    }
  }
 
  if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
  pData->p.value = NULL ;

  pData->tokenType = TOKEN_LITERAL ;
  pData->dataType = TYPE_STRING ;

  return ;
}

void gHyp_data_setNull ( sData *pData )
{
  /* Description:
   *
   *	Sets a data object to a empty string value.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   * Return value:
   *
   *	none
   *
   *
   */
  gHyp_data_setStr ( pData, "" ) ;
  return ;
}

void gHyp_data_setToken ( sData *pData, char *pStr )
{
  /* Description:
   *
   *	Sets a data object's value from a token string.
   *	The datatype of the token is determined, it may be an integer,
   *	double, or string.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	pStr				[R]
   *	- token string
   *
   * Return value:
   *
   *
   */
  int
    len,
    n ;

  unsigned short
    u ;

  unsigned long 
    ul,
    o,
    x ;

  double
    d ;

  sLOGICAL
    maybeInteger,
    maybeHex,
    maybeOctal,
    maybeDouble,
    maybeUnicode ;

  /* Set to string */
  gHyp_data_setStr ( pData, pStr ) ;

  /*gHyp_util_logInfo("Setting token on %s",pStr);*/
  len = strlen ( pStr ) ;
  if ( len > 0 && 
       (len + gHyp_util_getToken(pStr,TRUE)) == 0 ) {

    /* See if the string can be represented internally */
    maybeHex = (sLOGICAL) ( len > 2 &&
			    strstr ( pStr, "0x" ) == pStr &&
			    strspn ( pStr+2,"0123456789aAbBcCdDeEfF")==
			    (size_t) len-2);
    
    if ( maybeHex ) {
      
      n = sscanf ( pStr+2, "%lx", &x ) ;
      if ( n == 1 ) {
	pData->tokenType = TOKEN_CONSTANT ;
	pData->dataType = TYPE_HEX  ;
	if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
	pData->p.xValue = (unsigned long*) AllocMemory ( sizeof (unsigned long) ) ;
	assert ( pData->p.xValue ) ;
	*(pData->p.xValue) = x ;
	return ;	
      }
    }
    
    maybeOctal = (sLOGICAL) ( len > 2 &&
			    strstr ( pStr, "0o" ) == pStr &&
			    strspn ( pStr+2,"01234567")==(size_t)len-2);
    
    if ( maybeOctal ) {
      
      n = sscanf ( pStr+2, "%lo", &o ) ;
      if ( n == 1 ) {
	pData->tokenType = TOKEN_CONSTANT ;
	pData->dataType = TYPE_OCTAL  ;
	if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
	pData->p.oValue = (unsigned long*) AllocMemory ( sizeof (unsigned long) ) ;
	assert ( pData->p.oValue ) ;
	*(pData->p.oValue) = o ;
	return ;	
      }
    }
    
    maybeUnicode = (sLOGICAL) ( len > 2 &&
			    strstr ( pStr, "0u" ) == pStr &&
			    strspn ( pStr+2,"0123456789aAbBcCdDeEfF")==
			    (size_t)len-2);
    
    if ( maybeUnicode ) {
      
      n = sscanf ( pStr+2, "%hx", &u ) ;
      if ( n == 1 ) {
	pData->tokenType = TOKEN_CONSTANT ;
	pData->dataType = TYPE_UNICODE  ;
	if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
	pData->p.uValue = (unsigned short*) AllocMemory ( sizeof (unsigned short) ) ;
	assert ( pData->p.uValue ) ;
	*(pData->p.uValue) = u ;
	return ;	
      }
    }

    maybeInteger = (sLOGICAL) ( strspn(pStr,"0123456789+-") == (size_t)len &&
      strspn ( pStr, "+-" ) != (size_t) len ) ;
    
    if ( maybeInteger ) {
      
      n = sscanf ( pStr, "%lu", &ul ) ;
      if ( n == 1 ) {
	if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
	pData->tokenType = TOKEN_CONSTANT ;
	if ( strchr ( pStr, '-' ) ) {
	  pData->dataType = TYPE_INTEGER ;
	  pData->p.iValue = (int*) AllocMemory ( sizeof (int) ) ;
	  assert ( pData->p.iValue ) ;
	  *(pData->p.iValue) = (int) ul ;
	}
	else {

	  if ( ul > 0x7fffffff ) {
	  
	    pData->dataType = TYPE_ULONG ;
	    pData->p.ulValue = (unsigned long*) AllocMemory ( sizeof (unsigned long) ) ;
	    assert ( pData->p.ulValue ) ;
	    *(pData->p.ulValue) = ul ;
	  }
	  else {
	    pData->dataType = TYPE_INTEGER ;
	    pData->p.iValue = (int*) AllocMemory ( sizeof (int) ) ;
	    assert ( pData->p.iValue ) ;
	    *(pData->p.iValue) = (int) ul ;
	  }
	}
	return ;	
      }
    }
    
    maybeDouble = ( strspn ( pStr, "0123456789.eE+-" ) == (size_t) len ) ;
    
    if ( maybeDouble ) {
      
      n = sscanf ( pStr, "%lf", &d ) ;
      if ( n == 1 ) {
	pData->tokenType = TOKEN_CONSTANT ;
	pData->dataType = TYPE_DOUBLE ;
	if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
	pData->p.dValue = (double*) AllocMemory ( sizeof ( double ) ) ;
	assert ( pData->p.dValue ) ;
	*(pData->p.dValue) = d ;
	return ;	
      }
    }
  }

  return ;
}


void gHyp_data_setVariable ( sData *pData, char *pStr, sBYTE dataType )
{
  /* Description:
   *
   *	Sets a data object to a TOKEN_VARIABLE, with the specified
   *	dataType
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	pStr				[R]
   *	- token string
   *
   *	dataType
   *
   * Return value:
   *
   *	none
   *
   */
  gHyp_data_setLabel ( pData, pStr ) ;
  if ( dataType < TYPE_BYTE ) {
    pData->tokenType = TOKEN_VARIABLE ;
    pData->dataType = dataType ;
    pData->ss = -1 ;
  }
  else {
    gHyp_data_newVector ( pData, 
			   dataType, 
			   1,
			   TRUE ) ;  
  }
  
  return ;
}

void gHyp_data_setReference ( sData *pData, char *pStr, sData *pRef )
{
  /* Description:
   *
   *	Sets a data object to a reference
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	pRef				[R]
   *    - pointer to sData object to reference, or NULL
   *
   *	pStr				[R]
   *	- token string, or NULL
   *
   * Return value:
   *
   *	none
   *
   *
   */
  if ( pStr ) gHyp_data_setLabel ( pData, pStr ) ;
  pData->tokenType = TOKEN_REFERENCE ;
  pData->dataType = TYPE_STRING ;
  pData->ss = -1 ;

  if ( pRef ) {
    if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
    pData->p.rValue = (sData**) AllocMemory ( sizeof ( sData* ) ) ;
    /*gHyp_util_debug("Allocated %p for %s",pData->p.rValue,pStr);*/
    assert ( pData->p.rValue ) ;
    *(pData->p.rValue) = pRef ;
  }
  else {
    if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
    pData->p.value = NULL ;
  }

  return ;
}

void gHyp_data_setSubScript ( sData *pData, int ss ) 
{
  /* Description:
   *
   *	Sets the subscript of a variable data object.
   *	If the variable has a list of values, then the subscript
   *	specifies a particular value.  Normally the subscript is set
   *	to -1.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	ss				[R]
   *	- subscript value to set
   *
   * Return value:
   *
   *
   */
  pData->ss = ss ;
}

void gHyp_data_setSize ( sData *pData, int size ) 
{
  /* Description:
   *
   *	Sets the maximum size of a variable data object.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	size				[R]
   *	- size value to set
   *
   * Return value:
   *
   *
   *
   */
  pData->size = size ;
}

void gHyp_data_setUnidentified ( sData *pData, char *pStr )
{
  /* Description:
   *
   *	Sets the token type of a data object to unidentified.
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	pStr				[R]
   *	- token string
   *
   *
   * Return value:
   *
   *	none
   *
   *
   */

  gHyp_data_setLabel ( pData, pStr ) ;
  pData->tokenType = TOKEN_UNIDENTIFIED ;
  pData->dataType = TYPE_STRING ;

  return ;
}

void gHyp_data_setVector ( sData *pVariable, 
			   sData *pValue, 
			   int ssd, 
			   int sss,
			   sLOGICAL isValueVector )
{
  /* Sets the 'ssd' element of vector pVariable with the long value
   * extracted from the 'sss' element of the source variable pValue .
   */
  int i ;

  if ( ssd == -1 ) ssd = 0 ;
  if ( sss == -1 ) sss = 0 ;

  switch ( pVariable->dataType ) { 
    
  case TYPE_ATTR :
    *(pVariable->p.aValue+ssd) =
      (char) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;

  case TYPE_CHAR :
    *(pVariable->p.scValue+ssd) =
      ( (char) gHyp_data_getRaw ( pValue, sss, isValueVector ) & 0x7F ) ;
    break ;

  case TYPE_BYTE :
    *(pVariable->p.scValue+ssd) =
      (char) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_UCHAR :
    *(pVariable->p.ucValue+ssd) =
      (unsigned char) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;

  case TYPE_UBYTE :
    *(pVariable->p.ubValue+ssd) =
      (unsigned char) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;

  case TYPE_BOOLEAN :
    i = (unsigned char) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    *(pVariable->p.boValue+ssd) = i ? 1 : 0 ;
    break ;

  case TYPE_BINARY :
    *(pVariable->p.biValue+ssd) =
      (unsigned char) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_HEX :
    *(pVariable->p.xValue+ssd) =
      (unsigned long) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_OCTAL :
    *(pVariable->p.oValue+ssd) =
      (unsigned long) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_UNICODE :
    *(pVariable->p.uValue+ssd) =
      (unsigned short) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_SHORT :
    *(pVariable->p.ssValue+ssd) =
      (short) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_USHORT :
    *(pVariable->p.usValue+ssd) =
      (short unsigned int) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_INTEGER :
    *(pVariable->p.iValue+ssd) =
      (int) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_LONG :
    *(pVariable->p.slValue+ssd) =
      (long) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_ULONG :
    *(pVariable->p.ulValue+ssd) =
      (unsigned long) gHyp_data_getRaw ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_FLOAT :
    *(pVariable->p.fValue+ssd) =
      (float) gHyp_data_getDouble ( pValue, sss, isValueVector ) ;
    break ;
    
  case TYPE_DOUBLE :
    *(pVariable->p.dValue+ssd) =
      (double) gHyp_data_getDouble ( pValue, sss, isValueVector ) ;
    break ;
  
  case TYPE_HANDLE :
    *(pVariable->p.hValue+ssd) = 
      (void*) gHyp_data_getHandle ( pValue, sss, isValueVector ) ;
    break ;
  }
}

void gHyp_data_setVectorRaw ( sData *pVariable, void *pValue, int ssd )
{
  /* Sets the 'ssd' element of vector pVariable with the raw data from
   * the source pointed to by pValue
   */
  switch ( pVariable->dataType ) { 
    
  case TYPE_ATTR :
    *(pVariable->p.aValue+ssd) = *(char*) pValue ;
    break ;

  case TYPE_CHAR :
    *(pVariable->p.scValue+ssd) = (*(char*) pValue & 0x7F ) ;
    break ;

  case TYPE_BYTE :
    *(pVariable->p.scValue+ssd) = *(char*) pValue ;
    break ;
    
  case TYPE_UCHAR :
    *(pVariable->p.ucValue+ssd) = *(unsigned char*) pValue ;
    break ;

  case TYPE_UBYTE :
    *(pVariable->p.ubValue+ssd) = *(unsigned char*) pValue ;
    break ;
    
  case TYPE_BOOLEAN :
    *(pVariable->p.boValue+ssd) = *(unsigned char*) pValue ? 1 : 0 ;
    break ;
    
  case TYPE_BINARY :
    *(pVariable->p.biValue+ssd) = *(unsigned char*) pValue ;
    break ;
    
  case TYPE_HEX :
    *(pVariable->p.xValue+ssd) = *(unsigned long*) pValue ;
    break ;
    
  case TYPE_OCTAL :
    *(pVariable->p.oValue+ssd) = *(unsigned long*) pValue ;
    break ;
    
  case TYPE_UNICODE :
    *(pVariable->p.uValue+ssd) = *(unsigned short*) pValue ;
    break ;
    
  case TYPE_SHORT :
    *(pVariable->p.ssValue+ssd) = *(short*) pValue ;
    break ;
    
  case TYPE_USHORT :
    *(pVariable->p.usValue+ssd) = *(unsigned short*) pValue ;
    break ;
    
  case TYPE_INTEGER :
    *(pVariable->p.iValue+ssd) = *(int*) pValue ;
    break ;
    
  case TYPE_LONG :
    *(pVariable->p.slValue+ssd) = *(long*) pValue ;
    break ;
    
  case TYPE_ULONG :
    *(pVariable->p.ulValue+ssd) = *(unsigned long*) pValue ;
    break ;
    
  case TYPE_FLOAT :
    *(pVariable->p.fValue+ssd) = *(float*) pValue ;
    break ;
    
  case TYPE_DOUBLE :
    *(pVariable->p.dValue+ssd) = *(double*) pValue ;
    break ;
  
  case TYPE_HANDLE :
    *(pVariable->p.hValue+ssd) = *(void**) pValue ;
    break ;
  }

  return ;  
}

void gHyp_data_increment ( sData *pVariable, int ssd, sLOGICAL isVector )
{
  if ( ssd == -1 || !isVector ) ssd = 0 ;

  switch ( pVariable->dataType ) { 
    
  case TYPE_ATTR :
    (*(pVariable->p.aValue+ssd))++ ;
    break ;

  case TYPE_CHAR :
     (*(pVariable->p.scValue+ssd))++ ;
     *(pVariable->p.scValue+ssd) = (*(pVariable->p.scValue+ssd) & 0x7F ) ;
     break ;

  case TYPE_BYTE :
    (*(pVariable->p.scValue+ssd))++ ;
    break ;
    
  case TYPE_UCHAR :
    (*(pVariable->p.ucValue+ssd))++;
    break ;

  case TYPE_UBYTE :
    (*(pVariable->p.ubValue+ssd))++;
    break ;
    
  case TYPE_BOOLEAN :
    *(pVariable->p.boValue+ssd) = !(*(pVariable->p.boValue+ssd) ) ;
    break ;
    
  case TYPE_BINARY :
    (*(pVariable->p.biValue+ssd))++;
    break ;
    
  case TYPE_HEX :
    (*(pVariable->p.xValue+ssd))++;
    break ;
    
  case TYPE_OCTAL :
    (*(pVariable->p.oValue+ssd))++;
    break ;
    
  case TYPE_UNICODE :
    (*(pVariable->p.uValue+ssd))++;
    break ;
    
  case TYPE_SHORT :
    (*(pVariable->p.ssValue+ssd))++;
    break ;
    
  case TYPE_USHORT :
    (*(pVariable->p.usValue+ssd))++ ;
    break ;
    
  case TYPE_INTEGER :
    (*(pVariable->p.iValue+ssd))++ ;
    break ;
    
  case TYPE_LONG :
    (*(pVariable->p.slValue+ssd))++ ;
    break ;
    
  case TYPE_ULONG :
    (*(pVariable->p.ulValue+ssd))++;
    break ;
    
  case TYPE_FLOAT :
    (*(pVariable->p.fValue+ssd))++ ;
    break ;
    
  case TYPE_DOUBLE :
    (*(pVariable->p.dValue+ssd))++ ;
    break ;
  
  default  :
    break ;
  }
}

void gHyp_data_decrement ( sData *pVariable, int ssd, sLOGICAL isVector )
{
  if ( ssd == -1 || !isVector ) ssd = 0 ;

  switch ( pVariable->dataType ) { 
    
  case TYPE_ATTR :
    (*(pVariable->p.aValue+ssd))-- ;
    break ;

  case TYPE_CHAR :
     (*(pVariable->p.scValue+ssd))-- ;
     *(pVariable->p.scValue+ssd) = (*(pVariable->p.scValue+ssd) & 0x7F ) ;
     break ;

  case TYPE_BYTE :
    (*(pVariable->p.scValue+ssd))-- ;
    break ;
    
  case TYPE_UCHAR :
    (*(pVariable->p.ucValue+ssd))--;
    break ;

  case TYPE_UBYTE :
    (*(pVariable->p.ubValue+ssd))--;
    break ;
    
  case TYPE_BOOLEAN :
    *(pVariable->p.boValue+ssd) = !(*(pVariable->p.boValue+ssd) ) ;
    break ;
    
  case TYPE_BINARY :
    (*(pVariable->p.biValue+ssd))--;
    break ;
    
  case TYPE_HEX :
    (*(pVariable->p.xValue+ssd))--;
    break ;
    
  case TYPE_OCTAL :
    (*(pVariable->p.oValue+ssd))--;
    break ;
    
  case TYPE_UNICODE :
    (*(pVariable->p.uValue+ssd))--;
    break ;
    
  case TYPE_SHORT :
    (*(pVariable->p.ssValue+ssd))--;
    break ;
    
  case TYPE_USHORT :
    (*(pVariable->p.usValue+ssd))-- ;
    break ;
    
  case TYPE_INTEGER :
    (*(pVariable->p.iValue+ssd))-- ;
    break ;
    
  case TYPE_LONG :
    (*(pVariable->p.slValue+ssd))-- ;
    break ;
    
  case TYPE_ULONG :
    (*(pVariable->p.ulValue+ssd))--;
    break ;
    
  case TYPE_FLOAT :
    (*(pVariable->p.fValue+ssd))-- ;
    break ;
    
  case TYPE_DOUBLE :
    (*(pVariable->p.dValue+ssd))-- ;
    break ;
  
  default :
    break ;
  }
}

void gHyp_data_setInt ( sData *pData, int val )
{
  /* Description:
   *
   *	Sets a data object to an integer value
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	val				[R]
   *	- integer value
   *
   * Return value:
   *
   *
   *
   */
  char
    strVal[VALUE_SIZE+1] ;

  int
    n ;
    
  if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
  pData->p.iValue = (int*) AllocMemory ( sizeof ( int ) ) ;
  assert ( pData->p.iValue ) ;
  *(pData->p.iValue) = val ;

  if ( pData->pStrVal != NULLstring ) ReleaseMemory ( pData->pStrVal ) ;
  sprintf ( strVal, "%d", val ) ;
  n = strlen ( strVal ) ;
  pData->pStrVal = (char*) AllocMemory ( n + 1 ) ;
  assert ( pData->pStrVal ) ;
  strcpy ( pData->pStrVal, strVal ) ;
  pData->strlen = n;

  /*gHyp_util_logInfo("New int %s (%p)",pData->pStrVal) ;*/

  pData->tokenType = TOKEN_CONSTANT ;
  pData->dataType = TYPE_INTEGER ;
  pData->size = 1 ;

 return ;
}

void gHyp_data_setDouble ( sData *pData, double val )
{
  /* Description:
   *
   *	Sets a data object to an double value
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	val				[R]
   *	- double value
   *
   * Return value:
   *
   *
   */
  char
    strVal[VALUE_SIZE+1] ;  
  
  int
    n ;

  if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
  pData->p.dValue = (double*) AllocMemory ( sizeof ( double ) ) ;
  assert ( pData->p.dValue ) ;
  *(pData->p.dValue) = val ;

  if ( pData->pStrVal != NULLstring ) ReleaseMemory ( pData->pStrVal ) ;
  sprintf ( strVal, "%.7g", val ) ;
  n = strlen ( strVal ) ;
  pData->pStrVal = (char*) AllocMemory ( n + 1 ) ;
  assert ( pData->pStrVal ) ;
  strcpy ( pData->pStrVal, strVal ) ;
  pData->strlen = n ;
  /*gHyp_util_logInfo("New float %s (%p)",pData->pStrVal) ;*/

  pData->tokenType = TOKEN_CONSTANT ;
  pData->dataType = TYPE_DOUBLE ;
  pData->size = 1 ;

   return ;
}

void gHyp_data_setBool ( sData *pData, sLOGICAL boolval )
{
  /* Description:
   *
   *	Sets a data object to an boolean value
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	boolval				[R]
   *	- boolean value
   *
   * Return value:
   *
   *
   *
   */
  char
      strVal[VALUE_SIZE+1] ;  
  
  int 
    n ;

  if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
  pData->p.scValue = (char*) AllocMemory ( sizeof ( char ) ) ;
  assert ( pData->p.scValue ) ;
  *(pData->p.scValue ) = boolval ;

  if ( pData->pStrVal != NULLstring ) ReleaseMemory ( pData->pStrVal ) ;
  sprintf ( strVal, "%d", boolval ? 1 : 0 ) ;
  n = strlen ( strVal ) ;
  pData->pStrVal = (char*) AllocMemory ( n + 1 ) ;
  assert ( pData->pStrVal ) ;
  strcpy ( pData->pStrVal, strVal ) ;
  pData->strlen = n ;
  /*gHyp_util_logInfo("New boolean %s (%p)",pData->pStrVal) ;*/

  pData->tokenType = TOKEN_CONSTANT ;
  pData->dataType = TYPE_BOOLEAN ;
  pData->size = 1 ;

  return ;
}

void gHyp_data_setHandle ( sData *pData, void *handle )
{
  /* Description:
   *
   *	Sets a data object to an handle value
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	handle 				[R]
   *	- handle value
   *
   * Return value:
   *
   *
   *
   */  
  char
    strVal[VALUE_SIZE+1] ;  
  
  int
    n ;

  if ( pData->p.value ) ReleaseMemory ( pData->p.value ) ;
  pData->p.hValue = (void**) AllocMemory ( sizeof ( void* ) ) ;
  assert ( pData->p.hValue ) ;
  *(pData->p.hValue) = handle ;

  if ( pData->pStrVal != NULLstring ) ReleaseMemory ( pData->pStrVal ) ;
  sprintf ( strVal, "0x%p", handle ) ;
  if ( strVal[3]=='x' ) sprintf ( strVal, "%p", handle ) ;

  n = strlen ( strVal ) ;

  pData->pStrVal = (char*) AllocMemory ( n + 1 ) ;
  assert ( pData->pStrVal ) ;
  strcpy ( pData->pStrVal, strVal ) ;
  pData->strlen = n ;
  /*gHyp_util_logInfo("New handle %s (%p)",pData->pStrVal) ;*/

  pData->tokenType = TOKEN_CONSTANT ;
  pData->dataType = TYPE_HANDLE ;
  pData->size = 1 ;
  
  return ;
}

void gHyp_data_setHashed ( sData *pParent, sLOGICAL makeHashed ) 
{
  sData 
    *pThisData,
    *pNextData ;

  int
    i ;
  
  sHash
    *pHash,
    *pNext ;

  if ( makeHashed ) {

    /* If there is already a hash table then exit */
    if ( !pParent->pHash )  
      /* Create hash table */
      pParent->pHash = gHyp_hash_new ( VARIABLE_HASH_TABLE_SIZE ) ;
    
    /* Add or update all the elements to the hash table. */
    for ( pThisData = pParent->pData ;
	  pThisData ;
	  pThisData = pNextData ) {
      
      pNextData = pThisData->pNext ;
      
      /* Hash it */
      gHyp_hash_update ( pParent->pHash,
			 VARIABLE_HASH_TABLE_SIZE,
			 pThisData->pStrVal,
			 pThisData ) ;
      
      /* To handle rings, we're done when we're back at the first element */
      if ( pNextData == pParent->pData ) break ;
    }
  }
  else {

    /* Remove the hash table */
    /* Delete all the tokens from the hash table */
    for ( i=0; i<VARIABLE_HASH_TABLE_SIZE; i++ )
      for ( pHash = (*pParent->pHash)[i]; pHash; pHash=pNext ) {
	pNext = gHyp_hash_next ( pHash ) ;
	gHyp_hash_free ( pHash ) ;
      }
    /* Delete the hash table */
    gHyp_hash_delete ( pParent->pHash ) ;
    pParent->pHash = NULL ;
  }
  return ;
}


char* gHyp_data_print ( sData *pData )
{
  /* Description:
   *
   *	Print out a variable's contents.
   *
   * Arguments:
   *
   *	pData	[R/W]
   *	- pointer to variable
   *
   * Return value:
   *
   *	Pointer to created sData variable.
   *
   */
  static
    char string[MAX_LOG_LENGTH+1] ;
  
  char
    *pStr = string,
    *pEndStr = pStr + MAX_LOG_LENGTH - 2,
    value[VALUE_SIZE+1] ;
  
  sData
    *pResult,
    *pVariable=NULL ;

  sLOGICAL
    hasValues,
    isVector ;
  
  sBYTE
    parentTokenType,
    valueTokenType;

  int
    context,
    ss,
    resultLen ;
  
  parentTokenType = pData->tokenType ;

  switch ( parentTokenType ) {

  case TOKEN_REFERENCE : 
    sprintf ( pStr, "&%s", gHyp_data_getLabel ( pData ) ) ;
    pVariable = gHyp_data_getReference ( pData ) ;
    if ( pVariable ) {
      parentTokenType = pVariable->tokenType ;
      pData = pVariable ;
    }
    break ;

  case TOKEN_LITERAL : 
    sprintf ( pStr, "\"%s\"", gHyp_data_getLabel ( pData ) ) ;
    break ;
    
  case TOKEN_RAW : 
    sprintf ( pStr, "`%s`", gHyp_data_getLabel ( pData ) ) ;
    break ;
    
  case TOKEN_UNIDENTIFIED : 
    sprintf ( pStr, "'%s'", gHyp_data_getLabel ( pData ) ) ;
    break ;

  case TOKEN_VARIABLE : 
    pVariable = pData ;
  case TOKEN_CONSTANT : 
  default:
    strcpy ( pStr, gHyp_data_getLabel ( pData ) ) ;
    break ;
  }

  pStr += strlen ( string ) ;

  if ( parentTokenType == TOKEN_CONSTANT || 
       parentTokenType == TOKEN_LITERAL ) 
    return string ;  

  isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;

  if ( pVariable ) {

    if ( pVariable->isDynamic ) {
      if ( isVector )
	strcpy ( pStr, "<> " ) ;
      else
	strcpy ( pStr, "{} " ) ;
      pStr += 3 ;
    }
    else {
      if ( isVector )
	pStr += sprintf ( pStr, "<%d>", pVariable->size ) ;
      else
	pStr += sprintf ( pStr, "{%d}", pVariable->size ) ;
    }
  }

  if ( pData->ss >= 0 ) pStr += sprintf ( pStr, "[%d]", pData->ss ) ;
  
  if ( pData->pObject )
      pStr += sprintf ( pStr, "(%d)", pData->objectType ) ;

  pResult = NULL ;
  hasValues = FALSE ;
  ss = pData->ss ; context = -1 ;
  if ( ss == -1 ) 
  while ( (pResult = gHyp_data_nextValue ( pData, 
					   pResult, 
					   &context, 
					   ss ) ) ) {

    resultLen = gHyp_data_getStr ( pResult, 
				   value, 
				   VALUE_SIZE,
				   context,
				   isVector ) ;

    if ( resultLen == -1 ) break ;

    if ( !hasValues ) {
      if ( isVector )
	strcpy ( pStr, " = < " ) ;
      else
	strcpy ( pStr, " = { " ) ;
      pStr += 5 ;
      hasValues = TRUE ;
    }
    else {
      if ( ( pStr + resultLen + 1 ) > pEndStr ) break ;
      strcpy ( pStr, " , " ) ;
      pStr += 3 ;
    }

    valueTokenType = pResult->tokenType ;
    if ( isVector ) valueTokenType = TOKEN_CONSTANT ;
    switch ( valueTokenType ) {
    case TOKEN_REFERENCE : 
      if ( ( pStr + resultLen + 1 ) > pEndStr ) break ;      
      pStr += sprintf ( pStr, "&%s", value ) ;
      break ;
    case TOKEN_LITERAL : 
      if ( ( pStr + resultLen + 2 ) > pEndStr ) break ;      
      pStr += sprintf ( pStr, "\"%s\"", value ) ;
      break ;
      
    case TOKEN_RAW : 
      if ( ( pStr + resultLen + 2 ) > pEndStr ) break ;      
      pStr += sprintf ( pStr, "`%s`", value ) ;
      break ;
      
    case TOKEN_UNIDENTIFIED : 
      if ( ( pStr + resultLen + 2 ) > pEndStr ) break ;      
      pStr += sprintf ( pStr, "'%s'", value ) ;
      break ;
    case TOKEN_VARIABLE : 
      if ( pResult->dataType > TYPE_STRING ) {
	if ( ( pStr + resultLen + 2 ) > pEndStr ) break ;      
	pStr += sprintf ( pStr, "<%s>", value ) ;
	break ;
      }
    case TOKEN_CONSTANT : 
    default:
      if ( ( pStr + resultLen ) > pEndStr ) break ;      
      strcpy ( pStr, value ) ;
      pStr += resultLen ;
      break ;
    }
    *pStr = '\0' ;    
  }
  
  if ( hasValues ) {
    if ( (pStr + 2) <= pEndStr ) {
      if ( isVector ) 
	strcpy ( pStr, " >" ) ;
      else
	strcpy ( pStr, " }" ) ;
    }
  }
  return string ;
}

void gHyp_data_setText ( sData *pData, char *pString, int n )
{
  /* Description:
   *
   *	Adds lines of text to a list variable
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	pString				[R]
   *	- string
   *
   *    n
   *    - string length
   *
   * Return value:
   *
   *	none
   */
  char
    *pStr,
    *pSearch = pString,
    *pEndStr = pString + n  ;

  sData
    *pValue ;

  /* Delete all values for pData */
  /*gHyp_data_deleteValues ( pData ) ;*/

  /* Cut the string up, max 256 (VALUE_SIZE) characters per lines, but look for
   * cr/lf sequence as natual line break.
   */
  while ( pSearch < pEndStr ) {

    pStr = strchr ( pSearch, '\n' ) ;

    /* If no linefeed, take min of VALUE_SIZE or end of buffer */
    if ( pStr == NULL ) {
      n = MIN ( (pEndStr-pSearch), VALUE_SIZE ) ;
    }
    else {
      n = MIN ( (pStr-pSearch), VALUE_SIZE ) ;
    }
    pValue = gHyp_data_new ( NULL ) ;
    pStr = pSearch+n ; *pStr = '\0' ;
    gHyp_data_setStr_n ( pValue, pSearch, n ) ;
    gHyp_data_append ( pData, pValue ) ;
    pSearch += n+1 ;
  }

  return ;
}

int gHyp_data_getTagIndex ( sData *pData ) {
  /* Description:
   *
   *	Returns the index of a token with respect to
   *    sibling elements that share the same label
   *
   * Arguments:
   *
   *	pData				[R]
   *	- pointer to sData object
   *
   *	pString				[R]
   *	- string
   *
   *    n
   *    - string length
   *
   * Return value:
   *
   *	none
   */
  sData
    *pPrev,
    *pLast ;
  int
    i = 0 ;
    
  pPrev = gHyp_data_getPrev( pData ) ;
  pLast = gHyp_data_getLast( gHyp_data_getParent( pData ) ) ;
  pData = gHyp_data_getVariable( pData ) ;
  while ( pPrev !=  pLast ) {
  	if ( strcmp( gHyp_data_getLabel( pData ), gHyp_data_getLabel( pPrev ) ) == 0 )
  	  i += 1 ;
    pPrev =  gHyp_data_getPrev( pPrev ) ; 
  }
  return i ;
}

sData *gHyp_data_getChildNodeByIndex( sData *pParent, int index ) {
  /* Description:
   *
   *	Returns the pointer of a child node at index or NULL
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to sData object
   *
   *	index				[R]
   *	- integer
   *
   * Return value:
   *
   *	pointer to child node or NULL
   */
  sData
    *pChild ;
  int
  	i,
  	nodeNum = 0,  
    count = gHyp_data_getCount( pParent ) ;
    
  if( (pChild = gHyp_data_getFirst( pParent )) ) {
    for ( i = 0; i < count; i += 1 ) {
      if ( gHyp_data_getDataType ( pChild ) == TYPE_LIST ) {
      	if ( nodeNum == index ) return pChild ;
        nodeNum += 1 ;
      }  
      pChild = gHyp_data_getNext( pChild ) ;
    }
  }
  return NULL ;
}

sData *gHyp_data_getFirstNode( sData *pParent ) {
  /* Description:
   *
   *	Returns the pointer of the first child node or NULL
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to sData object
   *
   * Return value:
   *
   *	pointer to first child node or NULL
   */
  sData
    *pChild,
    *pFirst = gHyp_data_getFirst( pParent ) ;

  if( (pChild = pFirst) ) {
    do {
      if ( gHyp_data_getDataType ( pChild ) == TYPE_LIST ) return pChild ;
      pChild = gHyp_data_getNext( pChild ) ;
    } while ( pChild != pFirst ) ;
  }
  return NULL ;
}

sData *gHyp_data_getLastNode( sData *pParent ) {
  /* Description:
   *
   *	Returns the pointer of the last child node or NULL
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to sData object
   *
   * Return value:
   *
   *	pointer to last child node or NULL
   */
  sData
    *pChild,
    *pLast = gHyp_data_getLast( pParent ) ;
    
  if( (pChild = pLast) ) {
    do {
      if ( gHyp_data_getDataType ( pChild ) == TYPE_LIST ) return pChild ;
      pChild = gHyp_data_getPrev( pChild ) ;
    } while ( pChild != pLast ) ;
  }
  return NULL ;
}

sData *gHyp_data_getNextNode( sData *pData ) {    	
  /* Description:
   *
   *	Returns the pointer to the next node (of type list) or NULL
   *    getNextNode does not wrap around
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to sData object
   *
   * Return value:
   *
   *	pointer to next node or NULL
   */
  sData
    *pSibling,
    *pFirst =  gHyp_data_getFirst( gHyp_data_getParent( pData ) ) ;
          	
  pSibling = pData ;
  do {
    pSibling = gHyp_data_getNext ( pSibling ) ;
	if ( pSibling == pFirst ) return NULL ;
  }
  while ( gHyp_data_getDataType ( pSibling ) != TYPE_LIST ) ;
  return pSibling ;
}

sData *gHyp_data_getPrevNode( sData *pData ) {    	
  /* Description:
   *
   *	Returns the pointer to the previous node (of type list) or NULL
   *    getPrevNode does not wrap around
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to sData object
   *
   * Return value:
   *
   *	pointer to previous node or NULL
   */
  sData
    *pSibling,
    *pLast =  gHyp_data_getLast( gHyp_data_getParent( pData ) ) ;
          	
  pSibling = pData ;
  do {
    pSibling = gHyp_data_getPrev ( pSibling ) ;
	if ( pSibling == pLast ) return NULL ;
  }
  while ( gHyp_data_getDataType ( pSibling ) != TYPE_LIST ) ;
  return pSibling ;
}

sData *gHyp_data_getNodeByAttr( sData *pData, char *attrName, char *attrValue, char *pValueBuffer ) 
{
  /* Description: 
   *
   *	Traverse a datastructure via depth-first recursion and
   *    returns the pointer to the first child node (of type list) of pData
   *    that has an attribute variable matching attrName and attrValue
   *    If no suitable node is found, returns NULL
   *
   * Arguments:
   *
   *	pParent				[R]
   *	- pointer to sData object
   * 
   *    attrName
   *    - string cooresponding to the attribute's variable label to search for
   * 
   *    attrValue
   *    - string cooresponding to the attribute's value to search for
   * 
   *    pValueBuffer
   *    - pointer to a character buffer of size VALUE_SIZE + 1
   *      reserved for storing the attribute value. The function
   *      is recursive and can use a gob more memory than needed if
   *      this variable is not included.
   *
   * Return value:
   *
   *	pointer to node or NULL
   */
   
  sData
    *pAttr,
    *pValue,
    *pDescendent,
    *pTest ;
    
  int
    len ;
    
  pDescendent = gHyp_data_getFirstNode( pData ) ;
  while ( pDescendent ) {
    /* BEGIN ATTRIBUTE COMPARISON
     * the function uses getChildByName, which only returns the first name;
     * therefore, the function assumes that there is only one node with this label.
     * If there are more than one and if "correct" one is not younger than
     * the "incorrect one, we'll miss it.
     */
    if( (pAttr = gHyp_data_getChildByName( pDescendent, attrName )) &&
         gHyp_data_getDataType( pAttr ) == TYPE_ATTR &&
         gHyp_data_getTokenType( pAttr ) == TOKEN_VARIABLE )
    {	 
      /* gHyp_util_debug("attribute variable matches") ; */
      /* now check attribute's value */
	  if( (pValue = gHyp_data_getValue( pAttr, 0, 0 )) ) {
	    len = MIN ( VALUE_SIZE , gHyp_data_bufferLen ( pValue, 0 ) ) ;
	    memcpy ( pValueBuffer, gHyp_data_buffer(pValue,0), len ) ;
	    *(pValueBuffer + len) = '\0' ;
        /* gHyp_util_debug("attribute value found: %s", pValueBuffer); */
        if( strcmp( pValueBuffer, attrValue ) == 0 ){
          /* gHyp_util_debug("attribute value matches") ; */
          return pDescendent ;
		}
      }
    }
    /* END ATTRIBUTE COMPARISON */
    /* recursionion through all the offspring of the current pDescendent */
    if( (pTest = gHyp_data_getNodeByAttr( pDescendent, attrName, attrValue, pValueBuffer )) ) return pTest ;
    pDescendent = gHyp_data_getNextNode( pDescendent ) ;
  }
  return NULL ;
}
