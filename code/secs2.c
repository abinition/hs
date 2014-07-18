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
 * $Log: secs2.c,v $
 * Revision 1.11  2006/01/16 18:56:36  bergsma
 * HS 3.6.6
 * 1. Save query timeout events.  Don't let queries repeat indefinitely.
 * 2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 * Revision 1.10  2004/10/16 05:04:36  bergsma
 * Improve dubious handling of byte-order sensitive operations.
 *
 * Revision 1.9  2004/04/29 02:21:18  bergsma
 * Device id is sWORD, not int.
 *
 * Revision 1.8  2003/07/27 00:59:04  bergsma
 * Create placeholder for when UNICODE<->JIS8 conversion is needed.
 *
 * Revision 1.7  2003/06/05 21:54:23  bergsma
 * Added new typedef 'unicode', UTF-16.
 * Required for new unicode type:
 * 1. New data type in data object
 * 2. Special unicode output for xdescribe function
 * 3. New TYPE_UNICODE in hypdef.h
 * 4. New token 0uNNNNN format, like 0xNNNNN and 0oNNNNN
 * 5. New function unicode()
 * 6. New SECS II data type.
 *
 * Revision 1.6  2003/02/17 10:17:36  bergsma
 * Cleanup pTV memory if the parseSecs function returns an error.
 *
 * Revision 1.5  2003/01/09 08:01:50  bergsma
 * V3.1.0
 * Convert 8-byte unsigned and signed values to 2 4-byte values.
 *
 * Revision 1.4  2002/11/28 14:32:53  bergsma
 * Always reset the secs mlb after it is used.
 *
 * Revision 1.3  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.2  2002/09/16 18:07:32  bergsma
 * When receiving S9 messages, do a better job of getting the TID and SID values
 * out from the 10 byte message.
 *
 */

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ***********************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

#include "secshead.h"

struct secs2Header_t
{
  int			itemFormat ;
  unsigned long		length ;
} ;

struct secs2_t
{
  struct {
    int			secs2bufSize ;
    sBYTE		*pSecs2buf ;
    sBYTE		*pSecs2bufEnd ;
    sSecsHeader       	*pSecsHeader ;
  } x[2] ;
} ;

/**********************	FUNCTION DEFINITIONS ********************************/

sSecs2 *gHyp_secs2_new ()
{
  sSecs2 
    *pSecs2 ;

  pSecs2 = (sSecs2*) AllocMemory ( sizeof ( sSecs2 ) ) ;
  assert ( pSecs2 ) ;

  pSecs2->x[SECS_INCOMING].pSecs2buf = 
    (sBYTE*) AllocMemory ( sizeof ( sBYTE ) * DEFAULT_SECSII_CODE ) ;
  assert (  pSecs2->x[SECS_INCOMING].pSecs2buf ) ;

  pSecs2->x[SECS_INCOMING].secs2bufSize = DEFAULT_SECSII_CODE ;
  pSecs2->x[SECS_INCOMING].pSecs2bufEnd = pSecs2->x[SECS_INCOMING].pSecs2buf ;
  pSecs2->x[SECS_INCOMING].pSecsHeader = NULL ;

  pSecs2->x[SECS_OUTGOING].pSecs2buf = 
    (sBYTE*) AllocMemory ( sizeof ( sBYTE ) * DEFAULT_SECSII_CODE ) ;
  assert ( pSecs2->x[SECS_OUTGOING].pSecs2buf ) ;

  pSecs2->x[SECS_OUTGOING].secs2bufSize = DEFAULT_SECSII_CODE ;
  pSecs2->x[SECS_OUTGOING].pSecs2bufEnd = pSecs2->x[SECS_OUTGOING].pSecs2buf ;
  pSecs2->x[SECS_OUTGOING].pSecsHeader = NULL ;
  return pSecs2 ;
}

void gHyp_secs2_delete ( sSecs2* pSecs2 )
{
  if ( pSecs2->x[SECS_INCOMING].pSecsHeader ) {
    ReleaseMemory ( pSecs2->x[SECS_INCOMING].pSecsHeader ) ;
  }
  ReleaseMemory ( pSecs2->x[SECS_INCOMING].pSecs2buf ) ;

  if ( pSecs2->x[SECS_OUTGOING].pSecsHeader ) {
    ReleaseMemory ( pSecs2->x[SECS_OUTGOING].pSecsHeader ) ;
  }
  ReleaseMemory ( pSecs2->x[SECS_OUTGOING].pSecs2buf ) ;

  ReleaseMemory ( pSecs2 ) ;
}

sSecsHeader *gHyp_secs2_newSecsHeader ( sSecs2 *pSecs2, sBYTE direction ) 
{
  if ( pSecs2->x[direction].pSecsHeader ) {
    ReleaseMemory ( pSecs2->x[direction].pSecsHeader ) ;
    pSecs2->x[direction].pSecsHeader = NULL ;
  }
  pSecs2->x[direction].pSecsHeader = gHyp_secs1_newSecsHeader() ;
  return  pSecs2->x[direction].pSecsHeader ;
}

sSecsHeader *gHyp_secs2_getSecsHeader ( sSecs2 *pSecs2, sBYTE direction ) 
{
  return pSecs2->x[direction].pSecsHeader ;
}

void gHyp_secs2_resetBuf ( sSecs2 *pSecs2, sBYTE direction )
{
  if ( pSecs2->x[direction].pSecsHeader ) {
    ReleaseMemory ( pSecs2->x[direction].pSecsHeader ) ;
    pSecs2->x[direction].pSecsHeader = NULL ;
  }
  pSecs2->x[direction].pSecs2bufEnd = pSecs2->x[direction].pSecs2buf ;
  return ;
}

void gHyp_secs2_add2buf ( sSecs2 *pSecs2, 
			  sBYTE* pBuf,
			  int nBytes,
			  sBYTE d )
{
  int 
    offset ;

  /* First check if we are out of room */
  while ( ((pSecs2->x[d].pSecs2bufEnd - pSecs2->x[d].pSecs2buf) + nBytes) > 
	  pSecs2->x[d].secs2bufSize ) {
	
    /* Save offset since pSecs->pSecs2bufEnd is invalid after realloc */
    offset = pSecs2->x[d].pSecs2bufEnd - pSecs2->x[d].pSecs2buf ;
	  
    /* Out of room. Allocate twice as much as before */
    pSecs2->x[d].secs2bufSize <<= 1 ;
    if ( pSecs2->x[d].secs2bufSize > MAX_SECSII_CODE ) {
      gHyp_util_logError ( 
        "SecsII code size of %d exceeds maximum size of %d",
	pSecs2->x[d].secs2bufSize, MAX_SECSII_CODE ) ;
      longjmp ( gsJmpStack[0], COND_FATAL ) ;
    }
	
    pSecs2->x[d].pSecs2buf = 
      (sBYTE*) ReAllocMemory ( pSecs2->x[d].pSecs2buf, pSecs2->x[d].secs2bufSize ) ;
    assert ( pSecs2->x[d].pSecs2buf ) ;
    pSecs2->x[d].pSecs2bufEnd = pSecs2->x[d].pSecs2buf + offset ;
  }
      
  /* Copy and advance the pointer */
  if ( pBuf ) {
    memcpy ( pSecs2->x[d].pSecs2bufEnd, pBuf, nBytes ) ;
    pSecs2->x[d].pSecs2bufEnd += nBytes ;
  }
  /*
  gHyp_util_logInfo("BUF SIZE IS NOW %u out of %u, asking %u",
		    (pSecs2->x[d].pSecs2bufEnd - pSecs2->x[d].pSecs2buf),
		    pSecs2->x[d].secs2bufSize,
		    nBytes ) ;
  */
  return ;
}

sLOGICAL gHyp_secs2_isEndBlock ( sSecs2 *pSecs2, int blkNum, sBYTE d ) 
{
  return ( (pSecs2->x[d].pSecs2buf + (blkNum*MAX_SECS2_BLKSIZE)) >= 
	   pSecs2->x[d].pSecs2bufEnd ) ; 
}

int gHyp_secs2_getBlock ( sSecs2 *pSecs2, 
			  sBYTE* pBuf, 
			  int blkNum, 
			  sBYTE d )
{
  sBYTE 
    *pBufStart,
    *pBufEnd ;
  
  int
    nBytes ;
  
  pBufStart = pSecs2->x[d].pSecs2buf + ((blkNum-1)*MAX_SECS2_BLKSIZE) ;
  if ( pBufStart > pSecs2->x[d].pSecs2bufEnd ) return 0 ;
  pBufEnd = MIN ( pBufStart+MAX_SECS2_BLKSIZE, pSecs2->x[d].pSecs2bufEnd ) ;
  nBytes = pBufEnd - pBufStart ;
  memcpy ( pBuf, pBufStart, nBytes ) ;
  return nBytes ;
}

int gHyp_secs2_getAllBlocks ( sSecs2 *pSecs2, 
			      sBYTE *pBuf, 
			      sBYTE d )
{
  int
    nBytes ;

  nBytes = pSecs2->x[d].pSecs2bufEnd - pSecs2->x[d].pSecs2buf ;
  memcpy ( pBuf, pSecs2->x[d].pSecs2buf, nBytes ) ;
  return nBytes ;
}

static char* lHyp_secs2_parse ( char *pBuf,
				char *pBufEnd,
				sData *pTV,
				int sequence,
				sLOGICAL isLittleEndian,
				char *primary ) 
{
  char
    name[TOKEN_SIZE+1] ;

  int
    i,j,
    numElements,
    numLengthBytes ;

  sData
    *pData ;

  struct secs2Header_t 
    s2h ;

  sEndian 
    endian ;

  char	       	
    sb ;

  unsigned char
    ub,   
    bo,
    bi ;

  /* The item format must always be the first byte in pBuf */
  s2h.itemFormat = *pBuf & 0xFC ;
  numLengthBytes = *pBuf & 0x03 ;
  pBuf++ ;

  /* Calculate # of bytes in item */
  s2h.length = 0 ;
  for ( i=0; i<numLengthBytes; i++ ) 
    s2h.length = (s2h.length << 8) | (*pBuf++ & 0x00FF ) ;
  numElements = s2h.length ;

  /* Pull out the data */
  switch ( s2h.itemFormat ) {

  case SECS_ITEM_LIST :

    if ( sequence == -1 ) 
      /* Signifies the root variable. Use supplied name */ 
      sprintf ( name, primary ) ;
    else
      /* Generate variable based on sequence */
      sprintf ( name, "L_%u", sequence ) ;

    /* Start all 'list' sequences at 1 */
    sequence = 0 ;

    pData = gHyp_data_new ( name ) ;
    for ( i=0; i<numElements; i++ )
      pBuf = lHyp_secs2_parse ( pBuf, 
				pBufEnd, 
				pData, 
				sequence++, 
				isLittleEndian,
				primary ) ; 
    gHyp_data_append ( pTV, pData ) ;
    break ;
  
  case SECS_ITEM_JIS8 :
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "U_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_UNICODE, numElements/2, FALSE ) ;
    for ( i=0; i<numElements/2; i++ ) {
      endian.x.us = 0 ;
      if ( isLittleEndian ) 
	for ( j=1; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<2; j++ ) endian.x.b[j] = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &endian.x.us, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;


  case SECS_ITEM_ASCII :
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "A_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_CHAR, numElements, FALSE ) ;
    for ( i=0; i<numElements; i++ ) {
      sb = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &sb, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_SQUAD :
    /* Not supported, default to SLONG array of 2 elements */
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "SL_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_LONG, numElements/4, FALSE ) ;
    for ( i=0; i<numElements/4; i++ ) {
      endian.x.sl = 0 ;
      if ( isLittleEndian ) 
	for ( j=3; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<4; j++ ) endian.x.b[j] = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &endian.x.sl, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_SLONG :
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "SL_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_LONG, numElements/4, FALSE ) ;
    for ( i=0; i<numElements/4; i++ ) {
      endian.x.sl = 0 ;
      if ( isLittleEndian ) 
	for ( j=3; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<4; j++ ) endian.x.b[j] = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &endian.x.sl, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
    
  case SECS_ITEM_SWORD :
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "SW_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_SHORT, numElements/2, FALSE ) ;
    for ( i=0; i<numElements/2; i++ ) {
      endian.x.ss = 0 ;
      if ( isLittleEndian ) 
	for ( j=1; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<2; j++ ) endian.x.b[j] = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &endian.x.ss, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_SBYTE :
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "SB_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_BYTE, numElements, FALSE ) ;
    for ( i=0; i<numElements; i++ ) {
      sb = *pBuf++ ; 
      gHyp_data_setVectorRaw ( pData, &sb, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_DOUBLE :
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "D_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_DOUBLE, numElements/8, FALSE ) ;
    for ( i=0; i<numElements/8; i++ ) {
      endian.x.d = 0 ;
      if ( isLittleEndian )
	for ( j=7; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<8; j++ ) endian.x.b[j] = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &endian.x.d, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_FLOAT :
     if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "F_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_FLOAT, numElements/4, FALSE ) ;
    for ( i=0; i<numElements/4; i++ ) {
      endian.x.f = 0 ;
      if ( isLittleEndian ) 
	for ( j=3; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<4; j++ ) endian.x.b[j] = *pBuf++ ; 
      gHyp_data_setVectorRaw ( pData, &endian.x.f, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_UQUAD :
    /* Not supported, default to ULONG 2-element array */
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "UL_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_ULONG, numElements/4, FALSE ) ;
    for ( i=0; i<numElements/4; i++ ) {
      endian.x.ul = 0 ;
      if ( isLittleEndian ) 
	for ( j=3; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<4; j++ ) endian.x.b[j] = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &endian.x.ul, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_ULONG :
     if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "UL_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_ULONG, numElements/4, FALSE ) ;
    for ( i=0; i<numElements/4; i++ ) {
      endian.x.ul = 0 ;
      if ( isLittleEndian ) 
	for ( j=3; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<4; j++ ) endian.x.b[j] = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &endian.x.ul, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_UWORD :
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "UW_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_USHORT, numElements/2, FALSE ) ;
    for ( i=0; i<numElements/2; i++ ) {
      endian.x.us = 0 ;
      if ( isLittleEndian ) 
	for ( j=1; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
      else
	for ( j=0; j<2; j++ ) endian.x.b[j] = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &endian.x.us, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;

  case SECS_ITEM_BOOLEAN :
     if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "BL_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_BOOLEAN, numElements, FALSE ) ;
    for ( i=0; i<numElements; i++ ) {
      bo = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &bo, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_BINARY :
     if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "BN_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_BINARY, numElements, FALSE ) ;
    for ( i=0; i<numElements; i++ ) {
      bi = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &bi, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
    
  case SECS_ITEM_UBYTE :
    if ( sequence == -1 ) 
      sprintf ( name, primary ) ;
    else
      sprintf ( name, "UB_%u", sequence ) ;
    pData = gHyp_data_new ( name ) ;
    gHyp_data_newVector ( pData, TYPE_UBYTE, numElements, FALSE ) ;
    for ( i=0; i<numElements; i++ ) {
      ub = *pBuf++ ;
      gHyp_data_setVectorRaw ( pData, &ub, i ) ;
    }
    gHyp_data_append ( pTV, pData ) ;
    break ;
  }

  return pBuf ;
}

int gHyp_secs2_parseSecs ( sSecs2 *pSecs2,
			   sSecsHeader *pSecsHeader,
			   sInstance *pAI,
			   sBYTE initialState,
			   sBYTE modeState,
			   int requestTID,
			   int requestSID,
			   int requestStream,
			   int requestFunction ) 
{

  /* The secsII structure is parsed into sData objects, then a sAImsg object
   * is created from that, which in turn is returned to the HyperScript 
   * instance for consumption into its program
   */
  
  char
    primary[TOKEN_SIZE+1],
    target[TARGET_SIZE+1],
    sender[SENDER_SIZE+1],
    method[METHOD_SIZE+1],
    mode[MODE_SIZE+1],
    transactionId[8+1],
    systemId[8+1],
    timeStamp[DATETIME_SIZE+1],
    error[VALUE_SIZE+1],
    *pEvent = "event",
    *pQuery = "query",
    *pReply = "reply",
    *pMsg,
    *pBuf,
    *pEndBuf ;

  sData
    *pTV,
    *pData,
    *pMethodVariable,
    *pSTATUS,
    *pVALUE;

  int
    i,j,
    numLengthBytes,
    numElements,
    sequence,
    stream,
    function,
    TID,
    SID ;

  sWORD
    deviceId ;

  unsigned long
    length ;

  sLOGICAL
    isBinary,
    isReply=FALSE,
    isLittleEndian = gHyp_util_isLittleEndian() ;

  sEndian
    endian ;

  /* Initialize the method variable */
  pTV = gHyp_data_new ( "_tvsecs_" ) ;
  
  /* Create a variable name for the message. */
  stream = gHyp_secs2_stream ( pSecsHeader ) ; 
  function = gHyp_secs2_function ( pSecsHeader ) ;
  sprintf ( primary, "S%dF%d", stream, function ) ;
  TID = gHyp_secs2_TID ( pSecsHeader ) ;
  SID = gHyp_secs2_SID ( pSecsHeader ) ;

  /* For stream 9 messages, take the information from the message
   * body, which is the 10 bytes secs header from the message which
   * was not accepted. 
   */
  if ( stream == 9 ) {
    
     /* The item format must always be the first byte in pBuf */
    pBuf = (char*) (pSecs2->x[SECS_INCOMING].pSecs2buf) ;
    isBinary = ((*pBuf & 0xFC) == SECS_ITEM_BINARY ) ;

    if ( isBinary ) {

      numLengthBytes = *pBuf & 0x03 ;
      pBuf++ ;
      length = 0 ;
      for ( i=0; i<numLengthBytes; i++ ) length = (length << 8) | (*pBuf++ & 0x00FF ) ;
      numElements = length ;

      if ( numElements == 10 ) {
 
        /* Stream 9 message contains 10 byte header of message it is
         * complaining about.  Change the TID to that in the header,
         * so that the stream 9 message can match (as a reply) to any 
         * primary message that has been sent out previously.  The matching
         * occurs in lHyp_instance_readProcess.
         */
        pBuf += 6  ;
	endian.x.ss = 0 ;
	if ( isLittleEndian ) 
	  for ( j=1; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
	else
	  for ( j=0; j<2; j++ ) endian.x.b[j] = *pBuf++ ;
	SID = endian.x.ss ;

        endian.x.ss = 0 ;
        if ( isLittleEndian ) 
          for ( j=1; j>=0; j-- ) endian.x.b[j] = *pBuf++ ; 
        else
          for ( j=0; j<2; j++ ) endian.x.b[j] = *pBuf++ ;
        TID = endian.x.ss ;
      }
    }
  }
  
  sprintf ( transactionId, "%08x", TID ) ;
  sprintf ( systemId, "%08x", SID ) ;
  
  /* Set the sender name as <deviceId>.SECS */
  deviceId = gHyp_secs2_deviceId ( pSecsHeader ) ;
  sprintf ( sender, "%u#secs", deviceId ) ;  
  
  if ( guDebugFlags & DEBUG_PROTOCOL )
    gHyp_util_logDebug ( 
      FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
      "Received S%dF%d (Device %d) (TID=0x%02x) (SID=0x%02x) ",
      stream,function,deviceId,TID,SID ) ;

  if ( gHyp_secs2_isPrimaryMsg ( pSecsHeader ) ) {

    /* Its a primary message */
    if ( gHyp_secs2_isReplyExpected ( pSecsHeader ) ) {
      
      /* A reply is expected */
      strcpy ( mode, pQuery ) ;

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Saving TID=0x%02x, SID=0x%02x",
			     TID , SID ) ;

    }
    else {
      
      /* Its an event message */
      strcpy ( mode, pEvent ) ;
    }

    /* What method does this function invoke? */
    pMethodVariable = gHyp_instance_secsMethod ( pAI, 
						 deviceId, 
						 stream, 
						 function ) ;    
    if ( !pMethodVariable ) {
      gHyp_util_logError (
	"No method enabled for incoming S%dF%d SECS message from device %u",
	stream, function, deviceId ) ;
      gHyp_secs2_resetBuf ( pSecs2, SECS_INCOMING ) ;
      gHyp_data_delete ( pTV ) ;
      /* Send back F0 */
      return COND_ERROR ;
    }
    
    /* Set the method of the message */
    strcpy ( method, gHyp_data_getLabel ( pMethodVariable ) ) ;
  }
  else {

    /* Its a secondary reply message. Set the method variable
     * to the SxFx value, and it will be matched with the 
     * primary message that was sent out.
     */
    strcpy ( mode, pReply ) ;
    sprintf ( method, "S%dF%d", stream, function ) ;
    isReply = TRUE ;
    
  }

  strcpy ( target, gHyp_instance_getTargetPath ( pAI ) ) ;

  /* Calculate timestamp */
  strcpy ( timeStamp, gHyp_util_timeStamp ( gsCurTime ) ) ;

  /* Parse the SECS data */
  sequence = -1 ;
  pBuf = (char*) pSecs2->x[SECS_INCOMING].pSecs2buf ;
  pEndBuf = (char*) pSecs2->x[SECS_INCOMING].pSecs2bufEnd ; 
  if ( pEndBuf > pBuf ) {
    while ( pBuf < pEndBuf ) 
      pBuf = lHyp_secs2_parse (  pBuf , 
				 pEndBuf, 
				 pTV, 
				 sequence,
				 isLittleEndian,
				 primary ) ;
  }
  else {
    /* Message with no contents */
    pData = gHyp_data_new ( primary ) ;
    gHyp_data_append ( pTV, pData ) ;
  }

  gHyp_secs2_resetBuf ( pSecs2, SECS_INCOMING ) ;

  /* Add the STATUS variable */
  pSTATUS = gHyp_data_new ( "STATUS" ) ;
  pVALUE = gHyp_data_new ( NULL ) ;

  if ( function == 0 ) {
    sprintf ( error, "%%%s", primary ) ;
    gHyp_data_setStr ( pVALUE, error ) ;
  }
  else if ( stream == 9 ) {

    switch ( function ) {

    case 1 :
      gHyp_data_setStr ( pVALUE, "%S9F1" ) ;
      break ;

    case 3 :
      gHyp_data_setStr ( pVALUE, "%S9F3" ) ;
      break ;

    case 5 :
      gHyp_data_setStr ( pVALUE, "%S9F5" ) ;
      break ;

    case 7 :
      gHyp_data_setStr ( pVALUE, "%S9F7" ) ;
      break ;

    case 9 :
      gHyp_data_setStr ( pVALUE, "%S9F9" ) ;
      break ;

    case 11 :
      gHyp_data_setStr ( pVALUE, "%S9F11" ) ;
      break ;

    case 13 :
    default:
      gHyp_data_setStr ( pVALUE, "%S9F13" ) ;
      break ;
    }
  }
  else
    gHyp_data_setStr ( pVALUE, "$ACKNOWLEDGE" ) ;

  gHyp_data_append ( pSTATUS, pVALUE ) ;
  gHyp_data_append ( pTV, pSTATUS ) ;


  /* pAI is the instance that created the secs port (fd) with secs_open or secs_hsms,
   * but any instance can get the secs message if they did a secs_assign to the fd with
   * a designated device id.  For secs_hsms, 
   */

  /* Build a new incoming message */
  pMsg = gHyp_aimsg_initUnparsePartial ( gHyp_instance_incomingMsg ( pAI ),
					 target,
					 mode,
					 method,
					 sender,
					 transactionId,
					 systemId,
					 pTV ) ;
  if ( pMsg ) {

    /* Good message - set secs parameters in message */
    gHyp_instance_secsIncoming ( pAI,
				 deviceId,
				 stream,
				 function,
				 TID,
				 SID ) ;
    
    /*
     *
     * CASE: initialState == SECS_EXPECT_SEND_EOT2
     *
     *   We had just sent a message when another message another came in, 
     *   and ENQ was in the same buffer as the final ACK. (ENQ*)
     *
     *   1. mode == MESSAGE_REPLY
     *
     *	    We're done sending the reply message. 
     *
     *   2. mode == MESSAGE_QUERY, MESSAGE_EVENT
     *
     *      If the message is not a reply message.
     *      
     *	    It could be a S9 or other alarm.
     *	    SERVICE IT IMMEDIATELY.
     *
     *	  else if the message is a reply message.
     *
     *      QUEUE IT.
     *
     * CASE: initialState == SECS_EXPECT_SEND_EOT:
     *
     *   We were about to send a SECS message when a ENQ slave contention
     *   occurred and an unsolicted message arrived.
     *
     *   1. mode == MESSAGE_REPLY
     *      If sending a reply message, we must always re-send it, so
     *      we queue up the incoming messages. 
     *      (gHyp_secs1_outgoing called from gHyp_instance_replyMessage).
     *      QUEUE IT.
     *
     *   2. mode == MESSAGE_QUERY, MESSAGE_EVENT 
     * 
     *      If the message is not a reply message.
     *      
     *	    We must not re-send the query / reply until we first service 
     *        the incoming primary message. 
     *        (gHyp_secs1_outgoing called from lHyp_secs_QE)
     *        SERVICE IT IMMEDIATELY.
     *
     *	  else if the message is a reply message.
     *
     *	    QUEUE IT.
     */

    /* If the mssage has already been sent, complete the function */
    if ( initialState == SECS_EXPECT_SEND_EOT2 ) return COND_NORMAL ;

    /* If the ENQ contention incoming message was a reply, 
     * then return COND_SILENT so that the message will be re-sent.
     * (initialState == SECS_EXPECT_SEND_EOT)
     */
    if ( isReply ) return COND_SILENT ;
 
    /* If the ENQ contention event or query message is a S9 or F0 and 
     * has the same TID as the reply message we are trying to re-send,
     * then do not continue resending.
     */
    if ( TID == requestTID &&
	 ( stream == 9 || 
	   ( stream == requestStream && function == 0 ) ) )
      gHyp_util_logInfo ( "Aborting re-send of S%dF%d message",
			  requestStream, requestFunction ) ;

    return COND_NORMAL ;
  }

  /* All other cases return COND_SILENT */
  return COND_SILENT ;
}

static void lHyp_secs2_fill ( sInstance *pAI,
			      sData *pData, 
			      sSecs2* pSecs2,
			      sLOGICAL isLittleEndian,
			      int mlb ) 
{
  sData
    *pValue ;

  char
    *pStr ;

  sBYTE
    *pBuf ;

  int
    j,
    lBytes=1,
    context,
    ss ;

  sBYTE
    tokenType,
    dataType ;

  sLOGICAL
    isVector = FALSE ;

  struct secs2Header_t 
    s2h ;

  sEndian
    endian ;

  char	       	
    sb,
    sc ;

  unsigned char
    ub, 
    uc,
    bo,
    bi ;

  /*
  unsigned long
    x,
    o ;
  */

  unsigned short
    u ;

#define LBYTES(s) ( s <= 255 ? 0x1 : s < 65535 ? 0x2 : 0x3 )

  ss = gHyp_data_getSubScript ( pData ) ;
  context = -1 ;
  tokenType = gHyp_data_getTokenType ( pData ) ;
  dataType = gHyp_data_getDataType ( pData ) ;

  switch ( tokenType ) {

  case TOKEN_UNIDENTIFIED : 
  case TOKEN_REFERENCE : 
  case TOKEN_LITERAL : 
    pStr = gHyp_data_getLabel ( pData ) ;
    s2h.length = strlen ( pStr ) ;
    
    lBytes = LBYTES ( s2h.length ) ;
    if ( mlb > lBytes ) lBytes = mlb ;
    s2h.itemFormat = SECS_ITEM_ASCII | lBytes ;

    gHyp_secs2_add2buf ( pSecs2, NULL, s2h.length+lBytes+2, SECS_OUTGOING ) ;

    pBuf = pSecs2->x[SECS_OUTGOING].pSecs2bufEnd ;
    *pBuf++ = s2h.itemFormat ;
    if ( lBytes == 3 ) *pBuf++ = (sBYTE) (s2h.length >> 16) ;
    if ( lBytes >= 2 ) *pBuf++ = (sBYTE) (s2h.length >> 8);
    if ( lBytes >= 1 ) *pBuf++ = (sBYTE) (s2h.length >> 0) ;

    /* Fill in the ascii characters, subtract the null character at the end */
    while ( (*pBuf++ = *pStr++) ) ;
    pBuf-- ;
    pSecs2->x[SECS_OUTGOING].pSecs2bufEnd = pBuf ;

    break ;

  case TOKEN_CONSTANT :
  case TOKEN_VARIABLE :

    if ( ss == -1 ) 
      s2h.length = gHyp_data_getCount ( pData ) ;
    else
      s2h.length = 1 ;
    
    if ( dataType < TYPE_BYTE ) {

      if ( mlb == 0 && s2h.length == 0 ) return ;

      lBytes = LBYTES ( s2h.length ) ;
      if ( mlb > lBytes ) lBytes = mlb ;
      s2h.itemFormat = SECS_ITEM_LIST | lBytes ;
      
      gHyp_secs2_add2buf ( pSecs2, NULL, lBytes+1, SECS_OUTGOING ) ;
      
      pBuf = pSecs2->x[SECS_OUTGOING].pSecs2bufEnd ;
      *pBuf++ = s2h.itemFormat ;
      if ( lBytes == 3 ) *pBuf++ = (sBYTE) (s2h.length >> 16) ;
      if ( lBytes >= 2 ) *pBuf++ = (sBYTE) (s2h.length >> 8);
      if ( lBytes >= 1 ) *pBuf++ = (sBYTE) (s2h.length >> 0) ;
      pSecs2->x[SECS_OUTGOING].pSecs2bufEnd = pBuf ;
      
      pValue = NULL ;
      while ( (pValue = gHyp_data_nextValue ( pData, 
					      pValue, 
					      &context,
					      ss ) ) ) {  
	lHyp_secs2_fill ( pAI, pValue, pSecs2, isLittleEndian, 1 ) ;
      }
      if ( context == -2 && ss != -1 ) 
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
		 "Subscript '%d' is out of bounds in SECS message", ss ) ;
      
      
      break ;
    }
    else {

      if ( tokenType == TOKEN_VARIABLE ) isVector = TRUE ;

      if ( mlb == 0 && s2h.length == 0 ) return ;

      switch ( dataType ) {
	
      case TYPE_BYTE :
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_SBYTE | lBytes ;
	break ;
	
      case TYPE_UBYTE :
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_UBYTE | lBytes ;
	break ;
	
      case TYPE_BOOLEAN :
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_BOOLEAN | lBytes ;
	break ;
	
      case TYPE_BINARY :
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_BINARY | lBytes ;
	break ;
	
      case TYPE_HEX :
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_ULONG | lBytes ;
	break ;
	
      case TYPE_OCTAL :
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_ULONG | lBytes ;
	break ;

      case TYPE_UNICODE :
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_JIS8 | lBytes ;
	break ;
	
      case TYPE_CHAR :
      case TYPE_ATTR :
      case TYPE_UCHAR :
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_ASCII | lBytes ;
	break ;
		
      case TYPE_SHORT :
	s2h.length <<= 1 ;
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_SWORD | lBytes ;
	break ;
	
      case TYPE_USHORT :
	s2h.length <<= 1 ;
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_UWORD | lBytes ;
	break ;
	
      case TYPE_LONG :
      case TYPE_INTEGER :
	s2h.length <<= 2 ; 
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_SLONG | lBytes ;
	break ;
	
      case TYPE_ULONG :
	s2h.length <<= 2 ;
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_ULONG | lBytes ;
	break ;
	
      case TYPE_FLOAT :
	s2h.length <<= 2 ;
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_FLOAT | lBytes ;
	break ;
	
      case TYPE_DOUBLE :
	s2h.length <<= 3 ;
	lBytes = LBYTES ( s2h.length ) ;
	if ( mlb > lBytes ) lBytes = mlb ;
	s2h.itemFormat = SECS_ITEM_DOUBLE | lBytes ;
	break ;
      }
      
      gHyp_secs2_add2buf ( pSecs2, NULL, s2h.length+lBytes+1, SECS_OUTGOING ) ;
      
      pBuf = pSecs2->x[SECS_OUTGOING].pSecs2bufEnd ;
      *pBuf++ = s2h.itemFormat ;
      if ( lBytes == 3 ) *pBuf++ = (sBYTE) (s2h.length >> 16) ;
      if ( lBytes >= 2 ) *pBuf++ = (sBYTE) (s2h.length >> 8);
      if ( lBytes >= 1 ) *pBuf++ = (sBYTE) (s2h.length >> 0) ;
      
      pValue = NULL ;
      while ((pValue = gHyp_data_nextValue ( pData, 
					     pValue, 
					     &context,
					     ss ))) {
	
	switch ( dataType ) {
	case TYPE_BYTE :
	  sb = (char) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = sb ;
	  break ;
	  
	case TYPE_CHAR :
	case TYPE_ATTR :
	  sc = (char) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = sc ;
	  break ;
	  
	case TYPE_UBYTE :
	  ub = (unsigned char) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = ub ;
	  break ;
	  
	case TYPE_UCHAR :
	  uc = (unsigned char) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = uc ;
	  break ;
	  
	case TYPE_BOOLEAN :
	  bo = (unsigned char) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = bo ;
	  break ;
	  
	case TYPE_BINARY :
	  bi = (unsigned char) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = bi ;
	  break ;
	  
	case TYPE_HEX :
	  endian.x.ul = 
	    (unsigned long) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  /*
	  x = (unsigned long) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = (sBYTE) x ;
	  */
	  break ;
	  
	case TYPE_OCTAL :
	  endian.x.ul = 
	    (unsigned long) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  /*
	  o = (unsigned long) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = (sBYTE) o ;
	  */
	  break ;
	  
	case TYPE_UNICODE :
	  /* This is not right.  We should be applying the algorithm
	   * to convert UNICODE 2 byte sequences to UTF-8
	   */
	  u = (unsigned short) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  *pBuf++ = (sBYTE) u ;
	  break ;
	  
	case TYPE_SHORT :
	  endian.x.ss = 
	    (short) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=1; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<2; j++ ) *pBuf++ = endian.x.b[j] ;
	  /*
	  *pBuf++ = (sBYTE) (endian.x.ss >> 8);
	  *pBuf++ = (sBYTE) endian.x.ss ;
	  */
	  break ;
	  
	case TYPE_USHORT :
	  endian.x.us = 
	    (unsigned short) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=1; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<2; j++ ) *pBuf++ = endian.x.b[j] ;
	  /*
	  *pBuf++ = (sBYTE) (endian.x.us >> 8) ;
	  *pBuf++ = (sBYTE) endian.x.us ;
	  */
	  break ;
	  
	case TYPE_INTEGER :
	case TYPE_LONG :
	  endian.x.sl = 
	    (long) gHyp_data_getInt ( pValue, context, isVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  /*
	  *pBuf++ = (sBYTE) (endian.x.sl >> 24) ;
	  *pBuf++ = (sBYTE) (endian.x.sl >> 16) ;
	  *pBuf++ = (sBYTE) (endian.x.sl >> 8) ;
	  *pBuf++ = (sBYTE) endian.x.sl ;
	  */
	  break ;
	  
	case TYPE_ULONG :
	  endian.x.ul = 
	    (unsigned long) gHyp_data_getRaw ( pValue, context, isVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  /*
	  *pBuf++ = (sBYTE) (endian.x.ul >> 24) ;
	  *pBuf++ = (sBYTE) (endian.x.ul >> 16) ;
	  *pBuf++ = (sBYTE) (endian.x.ul >> 8) ;
	  *pBuf++ = (sBYTE) endian.x.ul ;
	  */
	  break ;
	  
	case TYPE_FLOAT :
	  endian.x.f = 
	    (float) gHyp_data_getDouble ( pValue, context, isVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=3; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<4; j++ ) *pBuf++ = endian.x.b[j] ;
	  break ;
	  
	case TYPE_DOUBLE :
	  endian.x.d = 
	    (double) gHyp_data_getDouble ( pValue, context, isVector ) ;
	  if ( isLittleEndian ) 
	    for ( j=7; j>=0; j-- ) *pBuf++ = endian.x.b[j] ; 
	  else
	    for ( j=0; j<8; j++ ) *pBuf++ = endian.x.b[j] ;
	  break ;
	}
      }
      if ( context == -2 && ss != -1 ) 
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
	       "Subscript '%d' is out of bounds in SECS message",ss ) ;
    }
    pSecs2->x[SECS_OUTGOING].pSecs2bufEnd = pBuf ;
  }
  return ;
}

void gHyp_secs2_unParseSecs ( sSecs2 *pSecs2, 
			      sData *pData,
			      sInstance *pAI,
			      int stream,
			      int function )
{
  sLOGICAL
    isS1F1 = ( stream == 1 && function == 1 ) ? TRUE : FALSE ; 

  sBYTE 
    mlb = gHyp_instance_getSecsMLB ( pAI ) ;

   /* The pData object is unparsed into a SECS II stream. */
  sLOGICAL
    isLittleEndian = gHyp_util_isLittleEndian() ;

  /* Always reset the mlb after each use */
  gHyp_instance_setSecsMLB ( pAI, -1 ) ;

  if ( mlb == 0xFF ) mlb = isS1F1 ? 0 : 1 ;

  /* Reset the buffer */
  gHyp_secs2_resetBuf ( pSecs2, SECS_OUTGOING ) ;

  /* Parse the data and fill the secs II structure */
  if ( pData ) {
    /*gHyp_util_logInfo("Unparsing SECS data %s",gHyp_data_print(pData));*/
    lHyp_secs2_fill ( pAI, pData, pSecs2, isLittleEndian, mlb ) ;
  }

  return ;
}

sWORD gHyp_secs2_deviceId ( sSecsHeader *pSecsHeader )
{
  return pSecsHeader->deviceId ;
}

int gHyp_secs2_stream ( sSecsHeader *pSecsHeader )
{
  return ( pSecsHeader->messageId >> 8 ) ;
}

int gHyp_secs2_function ( sSecsHeader *pSecsHeader )
{
  return (int) (sBYTE) pSecsHeader->messageId ;
}

int gHyp_secs2_TID ( sSecsHeader *pSecsHeader )
{
  return pSecsHeader->TID ;
}

int gHyp_secs2_SID ( sSecsHeader *pSecsHeader )
{
  return pSecsHeader->SID ;
}

sLOGICAL gHyp_secs2_isRbitSet ( sSecsHeader *pSecsHeader )
{
  return pSecsHeader->rBit ;
}

sLOGICAL gHyp_secs2_isReplyExpected ( sSecsHeader *pSecsHeader )
{
  return pSecsHeader->isReplyExpected ;
}

sLOGICAL gHyp_secs2_isFirstBlock ( sSecsHeader *pSecsHeader )
{
  return pSecsHeader->isFirstBlock ;
}

sLOGICAL gHyp_secs2_isLastBlock ( sSecsHeader *pSecsHeader )
{
  return pSecsHeader->isLastBlock ;
}

sLOGICAL gHyp_secs2_isPrimaryMsg ( sSecsHeader *pSecsHeader )
{
  return pSecsHeader->isPrimaryMsg ;
}
