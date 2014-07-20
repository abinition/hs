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
 * $Log: hsms.c,v $
 * Revision 1.21  2011-02-20 00:58:44  bergsma
 * Use MAX_INPUT instead of MAX_OUTPUT
 *
 * Revision 1.20  2009-11-17 16:07:31  bergsma
 * New call arguments to gHyp_instance_read
 *
 * Revision 1.19  2008-02-12 23:11:05  bergsma
 * VS 2008 Update
 *
 * Revision 1.18  2007-05-02 20:34:01  bergsma
 * Fix parseurl function.  Improve various print/debug/log statements.
 * Fix problem with chunked data transfers.
 *
 * Revision 1.17  2007-02-26 02:35:01  bergsma
 * No longer user NULL_DEVICEID placeholder.  PORT and HTTP
 * autoallocate device ids, HSMS and SECS I are pre-determined.
 *
 * Revision 1.16  2007-02-01 00:29:59  bergsma
 * Increase size of HSMS buffer and max message size.
 *
 * Revision 1.15  2006-01-19 20:37:15  bergsma
 * no message
 *
 * Revision 1.14  2006/01/16 18:56:36  bergsma
 * HS 3.6.6
 * 1. Save query timeout events.  Don't let queries repeat indefinitely.
 * 2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 * Revision 1.13  2005/11/28 05:08:48  bergsma
 * Detect when pParentAI is null
 *
 * Revision 1.12  2005/02/15 07:09:53  bergsma
 * Relax restrictions, object can be DATA_OBJECT_NULL.
 *
 * Revision 1.11  2005/01/31 06:01:39  bergsma
 * Comment change.
 *
 * Revision 1.10  2005/01/25 05:49:59  bergsma
 * 1. Use SLEEP signal rather than HEARTBEAT signal for HSMS pause.
 * 2. Moved test for nBytes==0 to sock_read
 *
 * Revision 1.9  2004/10/16 04:43:49  bergsma
 * Use memmove rather than strncpy for shifting next message.
 *
 * Revision 1.8  2004/07/23 18:41:02  bergsma
 * - for secs and hsms connections, an incoming connection request
 * reassigns ALL device id's into the new socket, not just one.
 *
 * Revision 1.7  2004/04/29 08:31:33  bergsma
 * Added SSL support.  Device id is now sWORD, not int.
 *
 * Revision 1.6  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.5  2002/11/12 20:12:51  bergsma
 * Removed unused return value nBytes from certain calls to hsms_outgoing
 *
 * Revision 1.4  2002/10/27 14:32:58  bergsma
 * No changes, just removed ^M.
 *
 * Revision 1.3  2002/10/23 00:13:15  bergsma
 * changed "Expecting RECV SELECT.REQ" to "Expecting RECV SELECT.RSP"
 *
 * Revision 1.2  2002/09/16 15:27:25  bergsma
 * When a socket error occured, the _deleteSocketObject() and _signalHangup
 * functions were being called twice, resulting in the HANGUP handler not
 * knowing which socket was closed.
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

#include "secshead.h"

struct hsms_t
{
  SOCKET       		socket ;
  SOCKET		parentSocket ;
  sInstance*		parentAI ;
  short			flags ;
  char			node[HOST_SIZE+1] ;
  float			t5 ;
  float			t6 ;
  float			t7 ;
  float			t8 ;

  int			port ;

  sBYTE			state ;
  sBYTE			prevState ;

  int			SID ;
  int			TID ;

  sBYTE		inbuf[MAX_HSMS_MESSAGE_SIZE] ;
  int		nBytes ;
  sBYTE*	pNextMsg ;
  sBYTE*	pEndOfBuf ;
  OVERLAPPED	overlapped ;
  sSSL*		pSSL ;

  unsigned char		outbuf[MAX_HSMS_MESSAGE_SIZE] ;

  sSecs2*	save_pSecs2 ;
  int		save_stream ;
  int		save_function ;
  sBYTE		save_mode ;
  sWORD		save_deviceId ;
  int		save_TID ;
  int		save_SID ;
} ;

/**********************	FUNCTION DEFINITIONS ********************************/

sHsms *gHyp_hsms_new ( short flags,
		       SOCKET socket,
		       char *pNode,
		       int port,
		       float t5,
		       float t6,
		       float t7,
		       float t8,
		       sBYTE state,
		       SOCKET parentSocket,
		       sInstance *parentAI )
{
  sHsms 
    *pHsms ;

  pHsms = (sHsms*) AllocMemory ( sizeof ( sHsms ) ) ;
  assert ( pHsms ) ;

  pHsms->flags = flags ;
  pHsms->socket = socket ;
  pHsms->parentSocket = parentSocket ;
  pHsms->parentAI = parentAI ;

  strcpy ( pHsms->node, pNode ) ;
  pHsms->port = port ;
  pHsms->t5 = t5 ;
  pHsms->t6 = t6 ;
  pHsms->t7 = t7 ;
  pHsms->t8 = t8 ;

  pHsms->TID = 0 ;
  pHsms->SID = 0 ; /*time ( NULL ) && 0xFF ;*/

  pHsms->state = state ;
  pHsms->prevState = state ;

  pHsms->pNextMsg = NULL ;
  pHsms->pEndOfBuf = pHsms->inbuf ;

  memset ( &pHsms->overlapped, 0, sizeof (OVERLAPPED) ) ;
#ifdef AS_WINDOWS
  pHsms->overlapped.hEvent = (HANDLE) gHyp_sock_createEvent () ;
  pHsms->overlapped.Internal = STATUS_READY ;
#endif

  pHsms->pSSL = NULL ;
  pHsms->save_pSecs2 = NULL ;
  pHsms->save_stream = -1 ;
  pHsms->save_function = -1 ;
  pHsms->save_mode = 0 ;
  pHsms->save_deviceId = NULL_DEVICEID ;
  pHsms->save_TID = 0 ;
  pHsms->save_SID = 0 ;
  
  return pHsms ;
}

void gHyp_hsms_delete ( sHsms *pHsms ) 
{
  gHyp_hsms_close ( pHsms ) ;
#ifdef AS_SSL
  if ( pHsms->pSSL ) gHyp_sock_deleteSSL ( pHsms->pSSL ) ;
#endif
  if ( pHsms ) ReleaseMemory ( pHsms ) ;
}

void gHyp_hsms_close ( sHsms *pHsms ) 
{
  gHyp_sock_close ( pHsms->socket, SOCKET_TCP, pHsms->node, "hsms" ) ;
#ifdef AS_WINDOWS
  CloseHandle ( pHsms->overlapped.hEvent ) ;
#endif
}

short gHyp_hsms_flags ( sHsms *pHsms )
{
  return pHsms->flags ;
}

int gHyp_hsms_nextMessage ( sHsms *pHsms, sConcept *pConcept, sInstance *pAI ) 
{
  int
    i,j,
    stream,
    function,
    msglen,
    nBytes ;

  sBYTE
    *pMsg,
    *pMsgStart,
    *eom ;

  sSecs2
    *pSecs2 ;

  sSecsHeader
    *pHeader,
    header ;

  char
    *pStr,
    blkStr[MAX_LOG_LENGTH+1] ;

  sInstance
    *pAIassigned = NULL ;

  /* Process messages until we get a SECS II data block */
  while ( TRUE ) {

    /* Get start of next message. Return 0 if no message is in 
     * the buffer or if the message is not complete.
     * The first time in this loop, pHsms->pNextMsg ALWAYS
     * points to the start of the pHsms->inbuf buffer.
     */
    pMsg = pHsms->pNextMsg ;
    if ( !pMsg ) return COND_SILENT ;
    
    /* How many bytes are there left to process in the message buffer? */
    nBytes = pHsms->pEndOfBuf - pMsg ;
    if ( nBytes == 0 ) return COND_SILENT ;

    /* At least the 4 length bytes and 10 header bytes must be present
     * for a complete message.
     */
    if ( nBytes < 14 ) {

      /* Shift the truncated message to the start of the buffer */
      if ( pHsms->inbuf < pHsms->pNextMsg )
	memmove ( (char*)pHsms->inbuf, (const char*)pHsms->pNextMsg, nBytes);
	
      /* Set the position where the next read will start from. */
      pHsms->pNextMsg = pHsms->inbuf + nBytes ;
	
      /* Don't process the message, wait until next read gets the rest. */
      return COND_SILENT ;
    }

    /* Get message length, using a byte-order safe method */
    pMsgStart = pMsg ;
    msglen = 0 ;
    msglen = ( msglen << 8 ) | (sBYTE) *pMsg++ ;
    msglen = ( msglen << 8 ) | (sBYTE) *pMsg++ ;
    msglen = ( msglen << 8 ) | (sBYTE) *pMsg++ ;
    msglen = ( msglen << 8 ) | (sBYTE) *pMsg++ ;
    
    /* Determine the virtual "end-of-message" */
    eom = pMsg + msglen ;

    /* Is all of the message in the buffer? */
    if ( eom > pHsms->pEndOfBuf ) {

      /* No. We still need to read more data */

      /* Shift the truncated message to the start of the buffer */
      if ( pHsms->inbuf < pHsms->pNextMsg )
	memmove ( (char*)pHsms->inbuf, (const char*)pHsms->pNextMsg, nBytes ) ;
	
      /* Set the position where the next read will start from. */
      pHsms->pNextMsg = pHsms->inbuf + nBytes ;
	
      /* Don't process the message, wait until next read gets the rest. */
      return COND_SILENT ;
    }
    else if ( eom < pHsms->pEndOfBuf ) {
     
      /* There is another message after this one. Set a pointer to it. */
      pHsms->pNextMsg = eom ;

    }
    else { /* eom == pHsms->pEndOfBuf */

      /* No more messages after this one. */
      pHsms->pNextMsg = NULL ;
    }
    
    if ( guDebugFlags & DEBUG_PROTOCOL ) {

      pStr = blkStr ;
      j = 0 ;

      for ( i=0; i<msglen+4; i++ ) {
	sprintf ( pStr, "%02x ", *(pMsgStart+i) ) ;
	pStr += 3 ;
	j += 3 ;
	if ( j+3 > MAX_LOG_LENGTH ) {
          gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "<- [%s]", blkStr ) ;
	  j = 0 ;
	  pStr = blkStr ;
	  *pStr = '\0';
	}
	
      }
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "<- [%s]", blkStr ) ; 
    }

    /* If the msgLen is less than 10, then its a bad length */
    if ( msglen < 10 ) {
      gHyp_util_logError ( 
	"HSMS Protocol: Invalid number of header bytes from device %d",
	header.deviceId ) ;
      gHyp_instance_signalHangup ( pAI, 
				   (int) pHsms->socket,
				   pHsms->port,
				   header.deviceId ) ;
      gHyp_concept_deleteSocketObject ( pConcept, pHsms->socket ) ;
      return COND_SILENT ;
    }

    /* Grab the header bytes */
    header.deviceId = (*pMsg++ << 8 ) ;
    header.deviceId |= *pMsg++ ;

    /* Check to see if we are enabled */
    pAIassigned = gHyp_hsms_assign ( pHsms, pConcept, header.deviceId ) ;

    /* Overwrite if found */
    if ( pAIassigned ) pAI = pAIassigned ;

    header.isReplyExpected = *pMsg & 0x80 ;
    header.messageId = ((*pMsg++ & 0x7F) << 8 ) ;
    header.messageId |= *pMsg++ ;
    header.PType = *pMsg++ ;
    header.SType = *pMsg++ ;
    header.SID = (*pMsg++ << 8) ;
    header.SID |= *pMsg++ ;
    header.TID = (*pMsg++ << 8 ) ;
    header.TID |= *pMsg++ ;
    header.isPrimaryMsg = (sBYTE) (header.messageId) % 2 ;

    /* The following three fields are not used in HSMS */
    header.isLastBlock = TRUE ;
    header.isFirstBlock = TRUE ; 
    header.rBit = FALSE ;  
       
    switch ( header.SType ) {

    case HSMS_STYPE_DATAMSG :

      stream = (header.messageId >> 8);
      function = (sBYTE) header.messageId ;

      if ( pHsms->state != HSMS_EXPECT_SEND_DATA &&
	   pHsms->state != HSMS_EXPECT_RECV_DATA )

	gHyp_util_logError ( 
	  "HSMS Protocol: Not expecting SECS II DATA from device %d",
	  header.deviceId ) ;

      else {

	pSecs2 = gHyp_instance_getSecs2byId ( pAI, header.deviceId ) ;
	    
	if ( !pSecs2 ) { 
	  gHyp_instance_warning ( pAI, STATUS_SECS,
				  "SECS II device id %d is not assigned to socket %d", 
				  header.deviceId, pHsms->socket ) ;
	} 
	else {

	  /* We have a complete SECS2 message */
	  gHyp_secs2_resetBuf ( pSecs2, SECS_INCOMING ) ;
	  pHeader = gHyp_secs2_newSecsHeader ( pSecs2, SECS_INCOMING ) ;
	  *pHeader = header ;
	  gHyp_secs2_add2buf ( pSecs2, 
			       pMsg,
			       msglen-10,
			       SECS_INCOMING ) ;
	  msglen = gHyp_secs2_parseSecs ( pSecs2,
					  &header,
					  pAI,
					  SECS_EXPECT_RECV_ENQ,
					  MESSAGE_QUERY,
					  0, 0, 0, 0 ) ;
	  if ( msglen < 0 ) {
	    if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	      /* Send back F0 abort message */
	      gHyp_hsms_outgoing ( pHsms, pSecs2, pAI,
				   gHyp_secs2_stream ( &header ) ,0,
				   header.deviceId,
				   header.TID,header.SID,
				   MESSAGE_EVENT ) ;
	    }
	    else {
	      gHyp_util_logWarning ( "S%dF0 not sent to equipment",
				     gHyp_secs2_stream ( &header ) ) ;
	    }
	  }	  
	}
      }
      break ;
      
    case HSMS_STYPE_SELECTREQ :
      
      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received SELECT.REQ" ) ;

      if ( pHsms->state != HSMS_EXPECT_RECV_SELECTREQ )

	gHyp_util_logError ( "HSMS Protocol: Not expecting SELECT.REQ" ) ;

      else {

	pHsms->state = HSMS_EXPECT_SEND_SELECTRSP ;

	gHyp_hsms_outgoing (  pHsms, 
			      NULL,
			      pAI,
			      0,0,
			      NULL_DEVICEID,
			      header.TID,
			      header.SID,
			      MESSAGE_EVENT ) ;
      }
      break ;
      
    case HSMS_STYPE_SELECTRSP :

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received SELECT.RSP");

      if ( pHsms->state != HSMS_EXPECT_RECV_SELECTRSP )

	gHyp_util_logError ( 
           "HSMS Protocol: Not expecting RECV SELECT.RSP" ) ;

      else {

	pHsms->state = HSMS_EXPECT_SEND_DATA ;

	/* We had to save the secs data while we processed the
	 * SELECT.REQ and SELECT.RSP.  Now we can send the actual
	 * SECS message data.
	 */
	if ( pHsms->save_pSecs2 != NULL )
	  gHyp_hsms_outgoing (	pHsms, 
				pHsms->save_pSecs2,
				pAI,
				pHsms->save_stream,
				pHsms->save_function,
				pHsms->save_deviceId,
				pHsms->save_TID,
				pHsms->save_SID,
				pHsms->save_mode ) ;

	/* Check the case where the first HSMS message was an EVENT.
	 * By returning here, we will go back to the call to 
	 * gHyp_instance_read in gHyp_hsms_outgoing, and will then
	 * return back to gHyp_secs_event
	 */
	if ( pHsms->save_mode == MESSAGE_EVENT ) return COND_NORMAL ;
      }
      break ;
      
    case HSMS_STYPE_DESELECTREQ :      
      
      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received DESELECT.REQ" ) ;

      if ( pHsms->state != HSMS_EXPECT_SEND_DATA &&
	   pHsms->state != HSMS_EXPECT_RECV_DATA )

	gHyp_util_logError ( "HSMS Protocol: Not expecting DESELECT.REQ" ) ;

      else {

	pHsms->state = HSMS_EXPECT_SEND_DESELECTRSP ;
	gHyp_hsms_outgoing (  pHsms, 
			      NULL,
			      pAI,
			      0,0,
			      NULL_DEVICEID,
			      header.TID,
			      header.SID,
			      MESSAGE_EVENT ) ;
      }
      break ;

    case HSMS_STYPE_DESELECTRSP :

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received DESELECT.RSP");

      if ( pHsms->state != HSMS_EXPECT_RECV_DESELECTRSP )
	gHyp_util_logError ( 
           "HSMS Protocol: Not expecting RECV DESELECT.RSP" ) ;
      break ;
      
    case HSMS_STYPE_LINKTESTREQ :

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received LINKTEST.REQ" ) ;

      pHsms->prevState = pHsms->state ;
      pHsms->state = HSMS_EXPECT_SEND_LINKTESTRSP ;
      gHyp_hsms_outgoing (  pHsms, 
			      NULL,
			      pAI,
			      0,0,
			      NULL_DEVICEID,
			      header.TID,
			      header.SID,
			      MESSAGE_EVENT ) ;
	   
      break ;
    
    case HSMS_STYPE_LINKTESTRSP :

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received LINKTEST.RSP");

      if ( pHsms->state != HSMS_EXPECT_RECV_LINKTESTRSP )
	gHyp_util_logError ( 
           "HSMS Protocol: Not expecting RECV LINKTEST.RSP" ) ;
      break ;
      
    case HSMS_STYPE_REJECTREQ :

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received REJECT.REQ" ) ;

      pHsms->prevState = pHsms->state ;
      pHsms->state = HSMS_EXPECT_SEND_REJECTRSP ;
      gHyp_hsms_outgoing (  pHsms, 
			      NULL,
			      pAI,
			      0,0,
			      NULL_DEVICEID,
			      header.TID,
			      header.SID,
			      MESSAGE_EVENT ) ;
      break ;

    case HSMS_STYPE_REJECTRSP :

       if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received REJECT.RSP");

      if ( pHsms->state != HSMS_EXPECT_RECV_REJECTRSP )
	gHyp_util_logError ( 
           "HSMS Protocol: Not expecting RECV REJECT.RSP" ) ;

      break ;
      
    case HSMS_STYPE_SEPARATEREQ :

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received SEPARATE.REQ" ) ;
      pHsms->prevState = pHsms->state ;
      pHsms->state = HSMS_EXPECT_SEND_SEPARATERSP ;
	gHyp_hsms_outgoing (  pHsms, 
			      NULL,
			      pAI,
			      0,0,
			      NULL_DEVICEID,
			      header.TID,
			      header.SID,
			      MESSAGE_EVENT ) ;

      break ;

    case HSMS_STYPE_SEPARATERSP :

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "Received SEPARATE.RSP");

      if ( pHsms->state != HSMS_EXPECT_RECV_SEPARATERSP )
	gHyp_util_logError ( 
           "HSMS Protocol: Not expecting RECV SEPARATE.RSP" ) ;

      break ;
    
    default:
      break ;
    }
  }
}

sInstance *gHyp_hsms_assign ( sHsms *pHsms, sConcept *pConcept, sWORD id ) 
{ 
  /* The purpose of this function is the call gHyp_instance_updateFd
   * on a id/socket combination.  
   */

  sHsms
    *pListenHsms ;

  sInstance
    *pAIassigned ;

  SOCKET
    fd ;

  if ( id == NULL_DEVICEID ) return NULL ;

  /* First find the instance that is assigned to the id */
  pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, id ) ;

  if ( !pAIassigned ) {

    gHyp_util_logWarning( "No instance has assigned id %d to the socket %d",
      id, pHsms->socket );
    return NULL ;

  }

  /* Get the port assigned to the id.
   * If it is the parent port, it is a new connection 
   * and we can setup a connect handler 
   */

  fd = gHyp_instance_getDeviceFd ( pAIassigned, id ) ;

  if ( fd == pHsms->parentSocket ) {

    if ( pHsms->socket != pHsms->parentSocket ) {

      /* This is a newly created channel */

      /* This is a newly created channel */
      gHyp_util_logInfo ( "Assigning device id %d to socket %d from listen port %d",
	 		    id, pHsms->socket, pHsms->parentSocket );

      /* Take any SSL structures as well */
      pListenHsms = (sHsms*) gHyp_concept_getSocketObject ( pConcept, 
							  (SOCKET) pHsms->parentSocket, 
							  DATA_OBJECT_NULL ) ;
							  /*DATA_OBJECT_HSMS ) ;*/

#ifdef AS_SSL
      if ( pListenHsms->pSSL != NULL ) {
	pHsms->pSSL = gHyp_sock_copySSL ( pListenHsms->pSSL ) ;
      }
#endif

      /* Take this id */
      gHyp_instance_updateFd ( pAIassigned, pHsms->socket, id, NULL, FALSE ) ;

      gHyp_instance_signalConnect ( pAIassigned, pHsms->socket, pHsms->port, id ) ;

    }
   
  }
  return pAIassigned ;

}
  
int gHyp_hsms_incoming ( sHsms *pHsms, 
			 sConcept *pConcept,
			 sInstance *pAI ) 
{ 

  int
    nBytes ;

  sBYTE
    *pMsg ;

  sLOGICAL
    jmpEnabled ;  

  sInstance
    *pAIassigned = gHyp_concept_getInstForFd ( pConcept, pHsms->socket ) ;

  /* What offset should the read start from? */
  pMsg = pHsms->pNextMsg ? pHsms->pNextMsg : pHsms->inbuf ;

  /* Read from socket */
  jmpEnabled = giJmpEnabled ;
  giJmpEnabled = FALSE ;
  nBytes = gHyp_sock_read ( pHsms->socket, 
			    (char*) pMsg, 
			    MAX_HSMS_BUFFER_SIZE,
			    SOCKET_TCP,
			    0,  /* 0 means read has completed, just get result */
			    &pHsms->nBytes,
  			    &pHsms->overlapped,
			    pHsms->pSSL ) ;
  
  /* Restore the previous longjmp return point. */
  giJmpEnabled = jmpEnabled ;

  if ( nBytes < 0 ) {

    if ( pAIassigned ) 
      gHyp_instance_signalHangup ( 
        pAIassigned, 
        (int) pHsms->socket, 
	pHsms->port,
        gHyp_instance_getDeviceId ( pAIassigned, pHsms->port) ) ;

    gHyp_concept_deleteSocketObject ( pConcept, pHsms->socket ) ;
    return COND_SILENT ;
  }
  else if ( nBytes == 0 ) {
    
    /* This can happen when the link as been closed at the other end. */
    gHyp_util_logError ( "Zero bytes read from SECS node %s, socket %d",
			 pHsms->node, pHsms->socket ) ;

    if ( pAIassigned ) 
      gHyp_instance_signalHangup ( 
	pAIassigned, 
        (int) pHsms->socket, 
        pHsms->port,
        gHyp_instance_getDeviceId ( pAIassigned, pHsms->port) ) ;
        
    gHyp_concept_deleteSocketObject ( pConcept, pHsms->socket ) ;


    return COND_SILENT ;
  }
  else {

    /* Received some bytes */
    pHsms->pEndOfBuf = pMsg + nBytes ;
    pHsms->pNextMsg = pHsms->inbuf ;
    return gHyp_hsms_nextMessage ( pHsms, pConcept, pAIassigned ) ;

  }
}

int gHyp_hsms_outgoing ( sHsms *pHsms, 
			 sSecs2 *pSecs2,
			 sInstance *pAI,
			 int stream,
			 int function,
			 int deviceId,
			 int TID,
			 int SID,
			 sBYTE mode )
{
  sSecsHeader
    header,
    *pHeader ;

  int
    i,j,
    msglen=0,
    nBytes ;

  char
    *pStr,
    blkStr[MAX_LOG_LENGTH+1] ;

  /* Initialize a new secs header */
  if ( pSecs2 ) 
    pHeader = gHyp_secs2_newSecsHeader ( pSecs2, SECS_OUTGOING ) ;
  else
    pHeader = &header ;

  switch ( pHsms->state ) {

   case	HSMS_EXPECT_SEND_LINKTESTRSP :

    /* We must have received the LINKTEST.REQ from the active node.
     * Now we must send back the LINKTEST.RSP within the T6 timeout period.
     */
     if ( guDebugFlags & DEBUG_PROTOCOL )
       gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			    "Sending LINKTEST.RSP to host '%s', socket %d",
			    pHsms->node, pHsms->socket ) ;

    /* Initialize the secs header structure */
    pHeader->deviceId = NULL_DEVICEID ;
    pHeader->messageId = 0 ;
    pHeader->PType = 0 ;
    pHeader->SType = HSMS_STYPE_LINKTESTRSP ;
    pHeader->TID = TID ;
    pHeader->SID = SID ;
    pHeader->isReplyExpected = FALSE ;

    /* Restore previous state */
    pHsms->state = pHsms->prevState ;
    pHsms->prevState = HSMS_EXPECT_SEND_LINKTESTRSP ;
    msglen = 10 ;

    break ;

   case	HSMS_EXPECT_SEND_DESELECTRSP :
    /* We must have received the DESELECT.REQ from the active node.
     * Now we must send back the DESELECT.RSP within the T6 timeout period.
     */
     if ( guDebugFlags & DEBUG_PROTOCOL )
       gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			    "Sending DESELECT.RSP to host '%s', socket %d",
			    pHsms->node, pHsms->socket ) ;

    /* Initialize the secs header structure */
    pHeader->deviceId = NULL_DEVICEID ;
    pHeader->messageId = 0 ;
    pHeader->PType = 0 ;
    pHeader->SType = HSMS_STYPE_DESELECTRSP ;
    pHeader->TID = TID ;
    pHeader->SID = SID ;
    pHeader->isReplyExpected = FALSE ;

    /* Set next state */
    pHsms->state = HSMS_EXPECT_SEND_SELECTREQ ;
    pHsms->prevState = HSMS_EXPECT_SEND_DESELECTRSP ;

    msglen = 10 ;

    break ;


   case	HSMS_EXPECT_SEND_REJECTRSP :
    /* We must have received the REJECT.REQ from the active node.
     * Now we must send back the REJECT.RSP within the T6 timeout period.
     */
     if ( guDebugFlags & DEBUG_PROTOCOL )
       gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			    "Sending REJECT.RSP to host '%s', socket %d",
			    pHsms->node, pHsms->socket ) ;
     
    /* Initialize the secs header structure */
    pHeader->deviceId = NULL_DEVICEID ;
    pHeader->messageId = 0 ;
    pHeader->PType = 0 ;
    pHeader->SType = HSMS_STYPE_REJECTRSP ;
    pHeader->TID = TID ;
    pHeader->SID = SID ;
    pHeader->isReplyExpected = FALSE ;

    /* Set next state */
    pHsms->state = pHsms->prevState ;
    pHsms->prevState = HSMS_EXPECT_SEND_REJECTRSP ;
    msglen = 10 ;

    break ;


   case	HSMS_EXPECT_SEND_SEPARATERSP :

    /* We must have received the SEPARATE.REQ from the active node.
     * Now we must send back the SEPARATE.RSP within the T6 timeout period.
     */
     if ( guDebugFlags & DEBUG_PROTOCOL )
       gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			    "Sending SEPARATE.RSP to host '%s', socket %d",
			    pHsms->node, pHsms->socket ) ;

    /* Initialize the secs header structure */
    pHeader->deviceId = NULL_DEVICEID ;
    pHeader->messageId = 0 ;
    pHeader->PType = 0 ;
    pHeader->SType = HSMS_STYPE_SEPARATERSP ;
    pHeader->TID = TID ;
    pHeader->SID = SID ;
    pHeader->isReplyExpected = FALSE ;

    /* Set next state */
    pHsms->state = HSMS_EXPECT_SEND_CONNECT ;
    pHsms->prevState = HSMS_EXPECT_SEND_SEPARATERSP ;
    msglen = 10 ;

    break ;

  case	HSMS_EXPECT_RECV_CONNECT :

    /* RECV CONNECT is a transient state.  */
    gHyp_util_logError ( 
      "Bad HSMS state 'RECV CONNECT' on node '%s', socket %d",
      pHsms->node, pHsms->socket ) ;
    return COND_SILENT ;

  case	HSMS_EXPECT_SEND_ACCEPT	:
    /* SEND ACCEPT is a transient state. */
    gHyp_util_logError ( 
      "Bad HSMS state 'SEND ACCEPT' on node '%s', socket %d",
      pHsms->node, pHsms->socket ) ;
    return COND_SILENT ;

  case	HSMS_EXPECT_RECV_SELECTREQ :
    /* We have not yet received a SELECT.REQ yet from an active node.
     * We are still in the state initialized after we had accepted
     * an incoming connection request.
     */
    gHyp_util_logError ( 
      "Expecting RECV SELECT.REQ from node '%s', socket %d",
      pHsms->node, pHsms->socket ) ;
    return COND_SILENT ;

  case	HSMS_EXPECT_SEND_SELECTRSP :
    /* We must have received the SELECT.REQ from the active node.
     * Now we must send back the SELECT.RSP within the T6 timeout period.
     */
    if ( guDebugFlags & DEBUG_PROTOCOL )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "Sending SELECT.RSP to host '%s', socket %d",
			   pHsms->node, pHsms->socket ) ;

    /* Initialize the secs header structure */
    pHeader->deviceId = NULL_DEVICEID ;
    pHeader->messageId = 0 ;
    pHeader->PType = 0 ;
    pHeader->SType = HSMS_STYPE_SELECTRSP ;
    pHeader->TID = TID ;
    pHeader->SID = SID ;
    pHeader->isReplyExpected = TRUE ;

    /* Set next state */
    pHsms->prevState = HSMS_EXPECT_SEND_SELECTRSP ;
    pHsms->state = HSMS_EXPECT_RECV_DATA ;
    if ( guDebugFlags & DEBUG_PROTOCOL )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "Now expecting RECV DATA" ) ;
    msglen = 10 ;

    break ;

  case	HSMS_EXPECT_SEND_CONNECT :
    /* SEND ACCEPT is a transient state.  See secs.c */
    gHyp_util_logError ( 
      "Bad HSMS state 'SEND CONNECT' on node '%s', socket %d",
      pHsms->node, pHsms->socket ) ;
    return COND_SILENT ;

  case	HSMS_EXPECT_RECV_ACCEPT	:
    /* RECV ACCEPT is a transient state.  See sock.c */
    gHyp_util_logError ( 
      "Bad HSMS state 'RECV ACCEPT' on node '%s', socket %d",
      pHsms->node, pHsms->socket ) ;
    return COND_SILENT ;

  case	HSMS_EXPECT_SEND_SELECTREQ :
    if ( guDebugFlags & DEBUG_PROTOCOL )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "Sending SELECT.REQ to host '%s', socket %d",
			   pHsms->node, pHsms->socket ) ;

    /* We must save the SECS message so that we can send it
     * after we receive the SELECT.RSP message.
     */
    pHsms->save_pSecs2 = pSecs2 ;
    pHsms->save_stream = stream ;
    pHsms->save_function = function ;
    pHsms->save_mode = mode ;
    pHsms->save_deviceId = deviceId ;
    pHsms->save_TID = TID ;
    pHsms->save_SID = SID ;
 
    /* Initialize the secs header structure */
    pHeader->deviceId = NULL_DEVICEID  ;
    pHeader->messageId = 0 ; 
    pHeader->PType = 0 ;
    pHeader->SType = HSMS_STYPE_SELECTREQ ;
    pHeader->TID = TID ;
    pHeader->SID = SID ;
    
    /* Set next state */
    pHsms->prevState = HSMS_EXPECT_SEND_SELECTREQ ;
    pHsms->state = HSMS_EXPECT_RECV_SELECTRSP ;
    if ( guDebugFlags & DEBUG_PROTOCOL )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "Now expecting RECV SELECT.RSP" ) ;
    msglen = 10 ;

    break ;

  case	HSMS_EXPECT_RECV_SELECTRSP :
    /* We have not yet received a SELECT.RSP from a passive node.
     * We are still in the state after connecting to a passive node and
     * sending a SELECT.REQ message.  See sock.c.
     */
    gHyp_util_logWarning ( 
      "Expecting RECV SELECT.RSP from node '%s', socket %d",
      pHsms->node, pHsms->socket ) ;
    return COND_SILENT ;

  case	HSMS_EXPECT_RECV_DATA :
    /* This is ok.  Just change state to SEND_DATA */
    pHsms->prevState = HSMS_EXPECT_RECV_DATA ;
    pHsms->state = HSMS_EXPECT_SEND_DATA ;
    if ( guDebugFlags & DEBUG_PROTOCOL )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "Now expecting SEND DATA" ) ;
    
  case	HSMS_EXPECT_SEND_DATA :
    
    if ( guDebugFlags & DEBUG_PROTOCOL )
      gHyp_util_logDebug ( 
        FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
	"Sending S%dF%d (device %d) (TID=0x%02x) (SID=0x%02x)",
			   stream,function,deviceId,TID,SID ) ;

    /* Initialize the secs header structure */
    pHeader->deviceId = deviceId ;
    pHeader->messageId = (stream << 8 );
    pHeader->messageId |= function ;
    pHeader->PType = 0 ;
    pHeader->SType = HSMS_STYPE_DATAMSG ;
    pHeader->isReplyExpected = ( mode == MESSAGE_QUERY ) ;
    pHeader->isPrimaryMsg = ( function % 2 ) ;
    if ( guDebugFlags & DEBUG_PROTOCOL )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "Restoring TID=0x%02x, SID=0x%02x", TID , SID ) ;
    pHeader->TID = TID ;
    pHeader->SID = SID ;

    /* Increment transaction id if this is a primary message */
    if ( pHeader->isPrimaryMsg ) {
      pHsms->TID++ ;
      if ( pHsms->TID > 0xffff ) pHsms->TID = 0 ;
    }

    /* Fill in the SECS II message */
    msglen = gHyp_secs2_getAllBlocks ( pSecs2, 
				       pHsms->outbuf+14, 
				       SECS_OUTGOING ) ;
    msglen += 10 ;
    break ;

  }
  if ( msglen == 0 ) return COND_SILENT ;

  /* Fill in the length bytes */
  pHsms->outbuf[0] = (sBYTE) (msglen >> 24) ;
  pHsms->outbuf[1] = (sBYTE) (msglen >> 16) ;
  pHsms->outbuf[2] = (sBYTE) (msglen >> 8) ;
  pHsms->outbuf[3] = (sBYTE) (msglen >> 0) ;
  
  pHsms->outbuf[HSMS_HEADER_UPPERID] = (pHeader->deviceId >> 8) ;
  pHsms->outbuf[HSMS_HEADER_LOWERID]  = (sBYTE) pHeader->deviceId ;
  pHsms->outbuf[HSMS_HEADER_STREAM]   = (pHeader->messageId >> 8);

  if ( pHsms->state == HSMS_EXPECT_SEND_DATA && 
    pHeader->isReplyExpected ) {
    pHsms->outbuf[HSMS_HEADER_STREAM] |= 0x80 ;
  }
  pHsms->outbuf[HSMS_HEADER_FUNCTION] = (sBYTE) pHeader->messageId ;
  pHsms->outbuf[HSMS_HEADER_PTYPE] = pHeader->PType ;
  pHsms->outbuf[HSMS_HEADER_STYPE] = pHeader->SType ;
  pHsms->outbuf[HSMS_HEADER_UPPERSRC] = (pHeader->SID >> 8) ;
  pHsms->outbuf[HSMS_HEADER_LOWERSRC] = (sBYTE) pHeader->SID ;
  pHsms->outbuf[HSMS_HEADER_UPPERTID] = (pHeader->TID >> 8) ;
  pHsms->outbuf[HSMS_HEADER_LOWERTID] = (sBYTE) pHeader->TID ;

  if ( guDebugFlags & DEBUG_PROTOCOL ) {
    pStr = blkStr ;
    j = 0 ;
    for ( i=0; i<msglen+4; i++ ) {
      sprintf ( pStr, "%02x ", pHsms->outbuf[i] ) ;
      pStr += 3 ;
      j += 3 ;
      if ( j+3 > MAX_LOG_LENGTH ) {
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			   "-> [%s]", blkStr ) ;
	j = 0 ;
	pStr = blkStr ;
	*pStr = '\0';
      }	
    }
    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			 "-> [%s]", blkStr ) ; 
  }
  /* Write it out */

  nBytes = gHyp_sock_write ( pHsms->socket, 
			     (char*) pHsms->outbuf, 
			     msglen+4, 
			     SOCKET_TCP,
			     &pHsms->overlapped,
			     pHsms->pSSL ) ;

  if ( nBytes <= 0 ) {
    gHyp_util_sysError ( "Failed to send to HSMS node %s",
			 pHsms->node ) ;
    gHyp_instance_signalHangup ( 
      pAI, (int) pHsms->socket, pHsms->port, deviceId ) ;
    gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), pHsms->socket ) ;
    return COND_SILENT ;
  }

  if ( pHsms->state == HSMS_EXPECT_RECV_SELECTRSP && 
       mode == MESSAGE_EVENT ) {
    
    /* Case where the first HSMS message is an EVENT, but we need
     * to still get SELECT.RSP after sending SELECT.REQ.
     * Wait T6 seconds for the SELECT.RSP.
     */ 
    gHyp_util_logInfo("Waiting for SELECT.RSP before sending message");

    /*gHyp_instance_setBeatTime ( pAI, gsCurTime+(int)pHsms->t6) ;*/

    gHyp_instance_setWakeTime ( pAI, (int)gsCurTime+(int)pHsms->t6) ;

    return gHyp_instance_read ( pAI, FALSE ) ;
  }
  return COND_SILENT ;
}

SOCKET gHyp_hsms_socket ( sHsms *pHsms )
{
  return pHsms->socket ;
}

SOCKET gHyp_hsms_parentSocket ( sHsms *pHsms )
{
  return pHsms->parentSocket ;
}

sInstance* gHyp_hsms_parentAI ( sHsms *pHsms )
{
  return pHsms->parentAI ;
}

sBYTE* gHyp_hsms_buf ( sHsms* pHsms)
{
  return pHsms->inbuf ;
}

unsigned gHyp_hsms_port ( sHsms *pHsms )
{
  return pHsms->port ;
}

float gHyp_hsms_t5 ( sHsms *pHsms )
{
  return pHsms->t5 ;
}

float gHyp_hsms_t6 ( sHsms *pHsms )
{
  return pHsms->t6 ;
}

float gHyp_hsms_t7 ( sHsms *pHsms )
{
  return pHsms->t7 ;
}

float gHyp_hsms_t8 ( sHsms *pHsms )
{
  return pHsms->t8 ;
}

char* gHyp_hsms_node ( sHsms *pHsms )
{
  return pHsms->node ;
}

int gHyp_hsms_TID ( sHsms *pHsms )
{
  return pHsms->TID ;
}

int gHyp_hsms_SID ( sHsms *pHsms )
{
  return pHsms->SID ;
}

int *gHyp_hsms_pNbytes ( sHsms *pHsms )
{
  return &pHsms->nBytes ;
}

LPOVERLAPPED gHyp_hsms_overlapped ( sHsms *pHsms )
{
  return &pHsms->overlapped ;
}

void gHyp_hsms_setSSL ( sHsms *pHsms, sSSL *pSSL )
{
  pHsms->pSSL = pSSL ;
}

sSSL *gHyp_hsms_getSSL ( sHsms *pHsms )
{
  return pHsms->pSSL ;
}


