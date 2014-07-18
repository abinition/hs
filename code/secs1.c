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
 *   $Log: secs1.c,v $
 *   Revision 1.11  2004/07/23 18:41:02  bergsma
 *   - for secs and hsms connections, an incoming connection request
 *   reassigns ALL device id's into the new socket, not just one.
 *
 *   Revision 1.10  2004/07/01 02:18:48  bergsma
 *   Log message changes
 *
 *   Revision 1.9  2004/06/13 14:11:03  bergsma
 *   For write operations, detect EAGAIN/EWOULDBLOCK, return 0 instead of -1
 *
 *   For PROMIS, in the CONFIG_TEMPLATE.COM, an invalid username sets the
 *   automation logicals to default values - JOB only, not GROUP.
 *
 *   For data coming into a port, set it to REPLY data only if there is
 *   and outstanding query on the same port.
 *
 *   For line-based HTTP data (text/plan, text/xml, text/html), provide
 *   knitting of lines between chunks of data read, also accomodate lines
 *   larger than INTERNAL_VALUE_SIZE if they really would be represented
 *   external in VALUE_SIZE space.
 *
 *   Revision 1.8  2004/05/15 02:08:44  bergsma
 *   Define usleep for VMS as well as windows.
 *
 *   Revision 1.7  2004/04/29 08:35:42  bergsma
 *   Added SSL support.
 *
 *   Revision 1.6  2003/11/16 21:04:23  bergsma
 *   When sending S9 functions, make sure secs2 structure is defined.
 *
 *   Revision 1.5  2003/02/18 02:01:24  bergsma
 *   Enhanced comment regarding invalid checksum.
 *
 *   Revision 1.4  2003/02/17 10:16:00  bergsma
 *   Warning messages starting with "%S9F" was causing core dump on some systems
 *   because %S was treated like a formatting character.
 *   When writing a string with \nnn characters, strlen() will not go past
 *   \0 and thus we need to use memcpy.
 *
 *   Revision 1.3  2003/02/05 23:57:12  bergsma
 *   Added comments
 *
 *   Revision 1.2  2002/11/12 20:19:53  bergsma
 *   When a SECS1 T2 timeout occured after not receiving the length byte,
 *   the retry counter was not being implemented.
 *
 *
 */
/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ***********************/

#include "auto.h"	/* System Interface and Function Prototypes */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef AS_UNIX
#include <sys/termios.h>
#endif

#ifdef AS_UNIX
#include <sys/time.h>
#endif

#ifdef AS_WINDOWS
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

#include "secshead.h"

struct secs1_t
{
  short			flags ;
  SOCKET		fd ;
  SOCKET		parentSocket ;
  sInstance*		parentAI ;
  char			device[VALUE_SIZE+1] ;
  sWORD			deviceId ;
  float			t1 ;
  float			t2 ;
  float			t4 ;
  int			rty ;
  unsigned		modifier ;

  int			SID ;
  int			TID ;

  sBYTE			inbuf[MAX_SECS1_BUFFER*2] ;
  sBYTE			outbuf[MAX_SECS1_BUFFER*2] ;
  int			nBytes ;
  OVERLAPPED		overlapped ;
  sBYTE			state ;
  sSSL*			pSSL ;
  sHTTP*		pHTTP ;
  sData*		forwardPorts ;

#ifdef AS_UNIX
  struct termios	tio ;
  struct termios	oldtio ;
#endif
} ;


/**********************	FUNCTION DEFINITIONS ********************************/
#ifndef AS_WINDOWS
extern int recv(int s, void *buf, int len, unsigned int flags);
extern int send(int  s,  const  void *msg, int len, unsigned int flags);
#endif

#if defined ( AS_VMS ) || defined ( AS_WINDOWS )
void usleep ( unsigned long useconds )
{
  struct timeval timeout ;
  timeout.tv_sec = useconds / 1000 ;
  timeout.tv_usec = useconds % 1000 ;
  select ( 0, NULL, NULL, NULL, &timeout ) ;
}
#endif

sSecs1 *gHyp_secs1_new ( short flags,
			 SOCKET fd,
			 char *device,
			 unsigned modifier,
			 float t1,
			 float t2,
			 float t4,
			 int rty,
			 SOCKET parentSocket,
			 sInstance* parentAI ) 
{
#ifdef AS_WINDOWS
  DCB PortDCB ;
  COMMTIMEOUTS CommTimeouts;  
#endif
  sSecs1 
    *pSecs1 = (sSecs1*) AllocMemory ( sizeof ( sSecs1 ) ) ;

  assert ( pSecs1 ) ;

  pSecs1->flags = flags ;
  pSecs1->fd = fd ;
  /*gHyp_util_debug("Setting parent socket to %d for socket %d",parentSocket,fd ) ;*/
  pSecs1->parentSocket = parentSocket ;
  pSecs1->parentAI = parentAI ;

  strcpy ( pSecs1->device, device ) ;

  memset ( &pSecs1->overlapped, 0, sizeof (OVERLAPPED) ) ;
#ifdef AS_WINDOWS
  pSecs1->overlapped.hEvent = (HANDLE) gHyp_sock_createEvent () ;
  pSecs1->overlapped.Internal = STATUS_READY ;
#endif

  pSecs1->pSSL = NULL ;

  if ( flags & PROTOCOL_HTTP ) 
    pSecs1->pHTTP = gHyp_http_new() ;
  else
    pSecs1->pHTTP = NULL ; 

  pSecs1->forwardPorts = NULL ;

  if ( flags & SOCKET_SERIAL ) {

#ifdef AS_UNIX
    switch ( modifier ) {
      
    case 19200 :
      pSecs1->modifier = B19200 ;
      break ;

    case 9600 :
      pSecs1->modifier = B9600 ;
      break ;
      
    case 4800 :
      pSecs1->modifier = B4800 ;
      break ;
    case 1200 :
      pSecs1->modifier = B1200 ;
      break ;
      
    case 300 :
      pSecs1->modifier = B300 ;
      break ;
    }
#else
    pSecs1->modifier = modifier ;
#endif
  }
  else
    pSecs1->modifier = modifier ;

  pSecs1->t1 = t1 ;
  pSecs1->t2 = t2 ;
  pSecs1->t4 = t4 ;

  pSecs1->TID = 0 ;
  pSecs1->SID = 0 ; /*time ( NULL ) && 0xFF ;*/

  pSecs1->rty = rty ;

  pSecs1->state = SECS_EXPECT_RECV_ENQ ;

  if ( flags & SOCKET_SERIAL ) {

#ifdef AS_UNIX
    memset( (char*) &pSecs1->tio, (char)0, sizeof ( struct termios ) ) ;
    
    tcgetattr ( fd, &pSecs1->oldtio ) ; /* Save current port settings */
    
    /* Setup a raw tty */
    pSecs1->tio.c_cflag = CS8 | CLOCAL | CREAD ;
    pSecs1->tio.c_iflag = IGNBRK | IGNPAR ;
    pSecs1->tio.c_oflag = 0;
    pSecs1->tio.c_lflag = 0;
    
    pSecs1->tio.c_cc[VTIME]    = 1 ;	/* 0.1 second timeout */  
    pSecs1->tio.c_cc[VMIN]     = 0 ;
    
    /* Set line speed */
    cfsetospeed ( &pSecs1->tio, pSecs1->modifier ) ;
    cfsetispeed ( &pSecs1->tio, pSecs1->modifier ) ;
    
    /* Set the tty attributes */
    tcsetattr ( fd, TCSANOW, &pSecs1->tio ) ;
    tcflush ( fd, TCIOFLUSH ) ;
#else
#ifdef AS_WINDOWS

    PortDCB.DCBlength = sizeof (DCB);     /* sizeof(DCB) */

    // Get the default port setting data. 
    GetCommState ( (HANDLE) pSecs1->fd, &PortDCB);
    
    // Change the DCB structure settings. 
    PortDCB.BaudRate = modifier ;         // Current baud rate 
    PortDCB.fBinary = TRUE;               // Binary mode, no EOF check 
    PortDCB.fParity = FALSE;              // Disable parity checking 
    PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
    PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
    PortDCB.fDtrControl = DTR_CONTROL_DISABLE; 
    // DTR flow control type 
    PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
    PortDCB.fTXContinueOnXoff = FALSE;     // XOFF continues Tx 
    PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
    PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
    PortDCB.fErrorChar = FALSE;           // Disable error replacement 
    PortDCB.fNull = FALSE;                // Disable null stripping 
    PortDCB.fRtsControl = RTS_CONTROL_DISABLE; 
    // RTS flow control 
    PortDCB.fAbortOnError = FALSE;        // Do not abort read/write on 
    // error
    PortDCB.ByteSize = 8;                 // Number of bits/byte, 4-8 
    PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
    PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2 
    
   /*
   BuildCommDCB(
  LPCTSTR lpDef,  // pointer to device-control string
  LPDCB lpDCB     // pointer to device-control block
);
*/

    // Configure the port according to the specifications of DCB structure
    if (!SetCommState ( (HANDLE) pSecs1->fd, &PortDCB)) {
      // Could not create the read thread
      gHyp_util_logError ( "Unable to configure the serial port" );
      return NULL ;
    }

    // Retrieve the time-out parameters for all read/write operations
    // on the port. 
    GetCommTimeouts ( (HANDLE) pSecs1->fd, &CommTimeouts);
    
    // Change the COMMTIMEOUTS structure settings.
    CommTimeouts.ReadIntervalTimeout = 100 ;  
    CommTimeouts.ReadTotalTimeoutMultiplier = 0 ;  
    CommTimeouts.ReadTotalTimeoutConstant = 0 ; /* .1 sec */    
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;  
    CommTimeouts.WriteTotalTimeoutConstant = 0;    
    
    // Set the time-out parameters for all read/write operations
    // on the port. 
    if (!SetCommTimeouts ( (HANDLE) pSecs1->fd, &CommTimeouts)) {
      gHyp_util_logError ( "Unable to set the time-out parameters") ;
	return NULL ;
    }
    
    /*SetCommMask (pSecs1->fd, EV_RXCHAR);*/

#endif
#endif
  }
  return pSecs1 ;
}

sSecsHeader *gHyp_secs1_newSecsHeader ()
{
  sSecsHeader 
    *pSecsHeader ;

  pSecsHeader = (sSecsHeader*) AllocMemory ( sizeof ( sSecsHeader ) ) ;  
  assert ( pSecsHeader ) ;
  /*gHyp_util_logInfo("New secsheader");*/
  
  pSecsHeader->deviceId = 0 ;
  pSecsHeader->messageId = 0 ;
  pSecsHeader->blockNum = 0 ;
  pSecsHeader->SID = 0 ;
  pSecsHeader->TID = 0 ;
  pSecsHeader->SType = 0 ;
  pSecsHeader->PType = 0 ;
  pSecsHeader->rBit = FALSE ;
  pSecsHeader->isReplyExpected = FALSE ;
  pSecsHeader->isLastBlock = TRUE ;
  pSecsHeader->isPrimaryMsg = FALSE ;
  pSecsHeader->isFirstBlock = TRUE ; 


  return pSecsHeader ;
}

void gHyp_secs1_delete ( sSecs1 *pSecs1 ) 
{
  gHyp_secs1_close ( pSecs1 ) ;
#ifdef AS_SSL
  if ( pSecs1->pSSL ) gHyp_sock_deleteSSL ( pSecs1->pSSL ) ;
#endif
  if ( pSecs1->pHTTP ) gHyp_http_delete ( pSecs1->pHTTP ) ;
  if ( pSecs1->forwardPorts ) gHyp_data_delete ( pSecs1->forwardPorts ) ;
  if ( pSecs1 ) ReleaseMemory ( pSecs1 ) ;
}

HANDLE gHyp_secs1_open ( char* pDevice ) 
{
  HANDLE
    fd ;

#ifdef AS_WINDOWS
	
  /* Open the serial port. */
  fd = CreateFile ( pDevice,
                    GENERIC_READ | GENERIC_WRITE,
                    0,                /* Share mode */
                    NULL,             /* Pointer to security attribute */
                    OPEN_EXISTING,    /* How to open */
                    FILE_FLAG_OVERLAPPED, /* Port attributes */
                    NULL);            
#else
   fd = open ( pDevice, O_RDWR | O_NOCTTY ) ;
#endif
   gHyp_util_logInfo("Opened %s on port %d", pDevice, fd ) ;

  return fd ;
}

void gHyp_secs1_close ( sSecs1 *pSecs1 ) 
{
  if ( pSecs1->flags & SOCKET_SERIAL ) {
#ifdef AS_UNIX
    tcsetattr ( pSecs1->fd, TCSANOW, &pSecs1->oldtio ) ;
#endif
  }

  gHyp_sock_close ( (SOCKET) pSecs1->fd, (short) (pSecs1->flags&MASK_SOCKET), pSecs1->device, "" ) ;

#ifdef AS_WINDOWS
  CloseHandle ( pSecs1->overlapped.hEvent ) ;
#endif
}

static int lHyp_secs1_incoming ( sSecs1 *pSecs1, 
				 sConcept *pConcept,
				 sInstance *pAI,
				 sBYTE mode,
				 int TID,
				 int SID,
				 int id,
				 int stream,
				 int function ) 
{
  /* Three return values:
   * > 0  = message read 
   * = 0  = no message, not last block, 
   *	    or alarm, timeout, pipe signal, or other non-fatal case
   * < 0  = fatal error, hangup signal
   *
   * Called from:
   * 1. Waiting for reply message or unsolicited message.
   *    - called from gHyp_msgio_select<-gHyp_instance_read
   *    - a return value of -1 is required to trigger handlers
   *	- a return value of 0 re-invokes msgio_select to read again
   *	- a return value > 0 satisfies the read.
   * 2. ENQ contention.
   *    - called from gHyp_secs1_outgoing, which in turn is called from
   *	  gHyp_secs_event, gHyp_secs_query, or gHyp_instance_replyMessage.
   *	- a return value of -1 aborts the query or aborts the reply Message
   *	- a return value of 0 re-invokes the query or re-sends the reply
   *	- a return value of 1 completes the query or reply message
   * 3. Getting the first ENQ of a reply msg after sending a primary msg.
   *    - called from gHyp_secs1_outgoing
   *    - if ENQ is received, function returns 1 after sending EOT
   *    - if ENQ is not received (timeout), function returns 1
   *
   */
  sLOGICAL
    jmpEnabled,
    blockReceived,
    gotENQ ;
  
  int 
    i,
    ix=0,
    nBytes=0,
    blkSize=0,
    secsIIblkSize,
    mtime=0,
    mt1u,
    mt1,
    mt2,
    mt4,
    timeout,
    retry=0,
    maxFrameError=SECS_MAX_FRAME_ERRORS,
    cond=COND_SILENT ;

  unsigned short
    checksum_read,
    checksum_calc;

  /*
  struct checkSum_t {
    union {
      unsigned short checksum ;
      unsigned char  checksum_b[2] ;
    } x ;
  } cs ;
  */
   
  sBYTE
    *pBuf,
    initialState = pSecs1->state ;

  char
    *pStr,
    blkStr[(MAX_SECS1_BUFFER*3)+1] ;

  sSecsHeader
    header,
    *pHeader ;

  sSecs2
    *pSecs2 = NULL ;


  /* Get SECS1 timeouts in integer milliseconds (+ 2 millisecond bonus) */
  mt1 = (int) (pSecs1->t1 * 1000) + 2 ; 
  mt2 = (int) (pSecs1->t2 * 1000) + 2 ;
  mt4 = (int) (pSecs1->t4 * 1000) + 2 ;
  mt1u = mt1 * 1000 ;

  retry = pSecs1->rty ;

  if ( initialState == SECS_EXPECT_SEND_EOT ||
       initialState == SECS_EXPECT_SEND_EOT2 )

    /* Function was called from gHyp_secs1_outgoing() */
    timeout = mt1 ;  /* +value means read means do read, wait mt1, then get result */

  else
    /* Function was called from gHyp_sock_select_readObjects() */
    timeout = 0 ; /* 0 means read has completed, just get result */

  blockReceived = FALSE ;
  while ( !blockReceived ) {

    /* Process the state and set the next state */    
    switch ( pSecs1->state ) {
      
    case SECS_EXPECT_RECV_ENQ :

      /* If we're nested too deep in handlers, then forget it */
      if ( giJmpLevel == MAX_JMP_LEVEL ) {
	gHyp_util_logError ( "SECS1 RECV NEQ jump level overflow at %d", MAX_JMP_LEVEL ) ;
	longjmp ( gsJmpStack[0], COND_FATAL ) ;
      }

      /* Save the previous longjmp return point. */;
      jmpEnabled = giJmpEnabled ;
      giJmpEnabled = FALSE ;
      nBytes =  gHyp_sock_read ( (SOCKET) pSecs1->fd,
				 (char*) pSecs1->inbuf, 
				 MAX_SECS1_BUFFER,
				 (short) (pSecs1->flags&MASK_SOCKET),
				 timeout,
				 &pSecs1->nBytes,
  				 &pSecs1->overlapped,
				 pSecs1->pSSL ) ;

      /* Restore the previous longjmp return point. */
      giJmpEnabled = jmpEnabled ;

      if ( nBytes < 0 ) {
	gHyp_util_logError ( "Failed (-1) to read <ENQ> from SECS device %s",
			     pSecs1->device ) ;
	/* Close the port */
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {

	/* A read timeout occurred of .1 second or 100 milliseconds. */
	if ( timeout == 0 ) {

	  /* The socket was supposed to contain data, but we didn't get it??.
	   * Maybe the the connection has timed out?
	   * Nonetheless, kill it so it doesn't spin.
	   */	  
#ifdef AS_WINDOWS
          errno = GetLastError() ;
#endif
         if ( errno == EAGAIN || errno == EWOULDBLOCK ) return COND_SILENT ;
  
	  gHyp_util_sysError ( "Failed (0) to read <ENQ> from SECS device %s",
			       pSecs1->device ) ;
	  gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	  gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;

	  return COND_SILENT ;
	} 
	else
	  /* Normal .1 (or 1) second timeout after waiting for ENQ.
	   * RECV_ENQ is normally the state of an idle socket, and in this
	   * state we would generally expect the socket to have data because 
	   * it was triggered in gHyp_sock_select by the select statement.
	   * But, RECV_ENQ can also be the state after:
	   *	- after sending ACK : a successfull receipt of a SECS message
	   *    - after sending a NAK : an unsuccessful receipt
	   *	- 0 byte error while expecting garbage after getting bad block.
	   * Returning 0 at this point is ok w.r.t. a potenital spin loop because 
	   * gHyp_sock_select_readObjects always calls lHyp_secs1_incoming with 
	   * timeout==0.  So, if the socket is not killed now, it will be
	   * the next time.
	   */
	  return COND_SILENT ;
      }
      /* Now, whenever we call sock_read to get data, our maximum timeout is 1,
       * which preferrably is only .1 second.
       */
      timeout = mt1 ;

      i = 0 ;
      gotENQ = FALSE ;
      while ( i<nBytes && !gotENQ ) {
 
	/* First byte should be ENQ. */
	if ( pSecs1->inbuf[i] == SECS_CHAR_ENQ ) 

	  gotENQ = TRUE ;

	/* Check for NAK */
	else if ( pSecs1->inbuf[i] == SECS_CHAR_NAK ) {

	  /* Got */
	  gHyp_util_logError ( 
	    "Expecting <ENQ>, got <NAK> from SECS device %s",
	    pSecs1->device ) ;
	  gHyp_instance_signalPipe ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	  return COND_SILENT ;
	}
	else {
	  
	  /* Did not get ENQ */
	  gHyp_util_logWarning ( 
	    "Expecting <ENQ>, got <0x%02x> from SECS device %s",
	    pSecs1->inbuf[i], pSecs1->device ) ;

	  /* See if we exceeded the number of frame errors */
	  maxFrameError-- ;
	  if ( maxFrameError <= 0 ) {
	    gHyp_util_logError ( "Maximum frame errors exceeded." ) ;
	    gHyp_instance_signalPipe ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	    return COND_SILENT ;
	  }
	}
	/* Look at all bytes received */
	i++ ;
      }

      /* Keep listening for ENQ */
      if ( !gotENQ ) break ;
 
      if ( guDebugFlags & DEBUG_SECS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "<-ENQ" ) ; 

      /* Reply back with <EOT> */
      pSecs1->state = SECS_EXPECT_SEND_EOT ;
      
    case SECS_EXPECT_SEND_EOT :
    case SECS_EXPECT_SEND_EOT2 :

      /* Send <EOT>.  Then wait for the message block. */
      pSecs1->outbuf[0] = SECS_CHAR_EOT ;
      nBytes = gHyp_sock_write ( (SOCKET) pSecs1->fd, 
				 (char*) pSecs1->outbuf,
				 1, 
				 (short) (pSecs1->flags&MASK_SOCKET),
  				&pSecs1->overlapped,
				pSecs1->pSSL ) ;

      if ( guDebugFlags & DEBUG_SECS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "->EOT" ) ; 

      if ( nBytes < 0 ) {
	gHyp_util_sysError ( "Failed to write <EOT> to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;

	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {

	/* Should not happen. */
	gHyp_util_logError ( "Zero bytes written to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }

      /* Initialize elapsed timer */
      mtime = 0 ;

      /* Set next state */
      pSecs1->state = SECS_EXPECT_RECV_BLK ;
      ix = 0 ;
      
    case SECS_EXPECT_RECV_BLK :

      /* We are expecting to receive the message block */
      /* Maximum required buffer size = 
       * 1 length byte + maximum 254 block length + 2 checksum bytes = 257 
       */

      /* If we're nested too deep in handlers, then forget it */
      if ( giJmpLevel == MAX_JMP_LEVEL ) {
	gHyp_util_logError ( "SECS1 RECV BLK jump level overflow at %d", MAX_JMP_LEVEL ) ;
	longjmp ( gsJmpStack[0], COND_FATAL ) ;
      }

      /* Save the previous longjmp return point. */
      jmpEnabled = giJmpEnabled ;
      giJmpEnabled = FALSE ;
      nBytes =  gHyp_sock_read ( (SOCKET) pSecs1->fd,
				 (char*) pSecs1->inbuf+ix, 
				 MAX_SECS1_BUFFER,
				 (short) (pSecs1->flags&MASK_SOCKET),
				 mt1,
				 &pSecs1->nBytes,
  				 &pSecs1->overlapped,
				 pSecs1->pSSL ) ;

      /* Restore the previous longjmp return point. */
      giJmpEnabled = jmpEnabled ;

      if ( nBytes < 0 ) {
	gHyp_util_logError ( "Failed to receive <BLK> from SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes > 0 ) {

	/* If this is the first byte of the block, then it has the length */
	if ( ix == 0 ) {

	  blkSize = pSecs1->inbuf[0] ;

	  while ( blkSize < 10 || blkSize > MAX_SECS1_BLKSIZE ) {

	    /* Check to see if its an ENQ or EOT, 
	     * which could be as a result of the following cases.
	     * [A} When we are a slave (host).
	     *     After sending ENQ, we get ENQ and relinquish control.
	     *     But we get a second ENQ because the equipment may not have
	     *     seen our first ENQ until after it sent a second
	     *     ENQ to get our attention.
	     * [B] When both are slaves, if both sides relinquish control
	     *     after receiving each other's ENQ, then two EOT's are sent.
	     *
	     * If so, then thow it out the character and try the next byte
	     * as the length byte.
	     */
	    if ( pSecs1->inbuf[0] == SECS_CHAR_ENQ ) {
	      
	      /* Remove the ENQ, shift all bytes down */
	      if ( guDebugFlags & DEBUG_SECS )
		gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
				     "<-ENQ(2)", blkStr ) ;
	      if ( nBytes > 1 )
		for (i=0; i<nBytes; i++) pSecs1->inbuf[i] = pSecs1->inbuf[i+1];
	      nBytes-- ;
	      if ( nBytes == 0 ) {
		/* There was just the ENQ.  Send back an EOT */
		pSecs1->state = SECS_EXPECT_SEND_EOT ;
	        break ;
	      }
	      /* Otherwise, keep processing the characters in the buffer */
	      blkSize = pSecs1->inbuf[0] ;
	    }
	    else if ( pSecs1->inbuf[0] == SECS_CHAR_EOT ) {
	      
	      /* Remove the EOT, shift all bytes down */
	      if ( guDebugFlags & DEBUG_SECS )
		gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
				     "<-EOT(2)", blkStr ) ;
	      if ( nBytes > 1 )
		for (i=0; i<nBytes; i++) pSecs1->inbuf[i] = pSecs1->inbuf[i+1];
	      nBytes-- ;
	      if ( nBytes == 0 ) {
		pSecs1->state = SECS_EXPECT_SEND_NAK ;
		break ;
	      }
	      
	      /* Otherwise, keep processing the characters in the buffer */
	      blkSize = pSecs1->inbuf[0] ;
	    }
	    else {
	      /* Bad length byte. The rest of the block is probably garbage.*/
	      gHyp_util_logError ( 
	        "SECS-I length byte out of range (%d)", blkSize );
	      /* Receive bytes until a T1 timeout */
	      gHyp_util_logWarning ( "Assuming rest of bytes are garbage" ) ;
	      pSecs1->state = SECS_EXPECT_RECV_GBG ;
	      nBytes = 0 ;
	      break ;
	    }
	  }
	}
	if ( nBytes == 0 ) break ;

	if ( guDebugFlags & DEBUG_SECS ) {
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			       "<-%d received (%d/%d bytes)",
			       nBytes,
			       ix+nBytes,
			       blkSize+3 ) ;
	  pStr = blkStr ;
	  for ( i=ix; i<ix+nBytes; i++ ) {
	    sprintf ( pStr, "%02x ", pSecs1->inbuf[i] ) ;
	    pStr += 3 ;
	  }
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			       "<-BLK [%s]", blkStr ) ; 
	}

	/* Advance forward in the read buffer */
	ix += nBytes ;

	/* Have all the bytes been received? */
	if ( ix < blkSize + 3 )

	  /* Re-initialize inter-char timeout */
	  mtime = 0 ;
 
	else {

	  /* SECS block is in. Perform the necessary checksum.
	   * Required checksum is in the last 2 bytes of the received data.
	   * Move the checksum bytes to the checksum structure
	   */
	  pBuf = pSecs1->inbuf + blkSize + 1 ;
	  checksum_read = (*pBuf++ & 0x00FF ) ;
	  checksum_read = (checksum_read << 8) | (*pBuf++ & 0x00FF ) ;
	  
	  /* The checksum is the sum of the bytes in the block */
	  for ( checksum_calc=0,i=1; i<blkSize+1; i++ ) 
	    checksum_calc += pSecs1->inbuf[i] ;
	  
	  /* Is the checksum correct? */
	  if ( checksum_read != checksum_calc ) {
	    gHyp_util_logError ( "SECS-I checksum error, should be %x, sending <NAK>",
	      checksum_calc ) ;
	    pSecs1->state = SECS_EXPECT_SEND_NAK ;
	    break ;
	  }
	  else
	    pSecs1->state = SECS_EXPECT_SEND_ACK ;
	}
      }
      else /* ( nBytes == 0 ) */ { 

	/* Timeout occurred of .1 second or 100 milliseconds.
	 * Therefore, add 100 to the time since the last read.
	 */
	mtime += mt1 ;
	
 	if ( ix == 0 ) {
	  /* Still haven't received length byte. */
	  if ( mtime > mt2 ) {
	    /* Too much time waiting for the length byte */
	    gHyp_util_logError ( "SECS1 T2 timeout, sending <NAK>" ) ;

	    retry-- ;
	    if ( retry > 0 ) {
	      pSecs1->state = SECS_EXPECT_SEND_NAK ;
	      break ;
	    }
	    else {
	      gHyp_util_logError ( "Failed to receive length byte after %d tries",
				   pSecs1->rty ) ;
	      pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	      gHyp_instance_signalPipe ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	      return COND_SILENT ;
	    }
	  }
	}
	else {
	  /* We received some of the block but not all of it. */
	  if ( mtime > mt1 ) {
	    /* Too much time waiting between characters */
	    gHyp_util_logError ( "SECS-I T1 timeout, sending <NAK>" ) ;
	    pSecs1->state = SECS_EXPECT_SEND_NAK ;
	    break ;
	  }
	}
	/*
	if ( guDebugFlags & DEBUG_SECS )
 	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			       "<-0 bytes, waiting..." ) ;
	*/
      }
      break ;

    case SECS_EXPECT_RECV_GBG :

      /* We are expecting to receive garbage */

      /* If we're nested too deep in handlers, then forget it */
      if ( giJmpLevel == MAX_JMP_LEVEL ) {
	gHyp_util_logError ( "SECS1 RECV GBG jump level overflow at %d", MAX_JMP_LEVEL ) ;
	longjmp ( gsJmpStack[0], COND_FATAL ) ;
      }

      /* Save the previous longjmp return point. */
      jmpEnabled = giJmpEnabled ;
      giJmpEnabled = FALSE ;
      nBytes =  gHyp_sock_read ( (SOCKET) pSecs1->fd,
				 (char*) pSecs1->inbuf+ix, 
				 MAX_SECS1_BUFFER,
				 (short) (pSecs1->flags&MASK_SOCKET),
				 mt1,
				 &pSecs1->nBytes,
  			         &pSecs1->overlapped,
				 pSecs1->pSSL ) ;

      /* Restore the previous longjmp return point. */
      giJmpEnabled = jmpEnabled ;
      
      if ( nBytes < 0 ) {
	gHyp_util_logError ( "Failed to receive garbage from SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {

	gHyp_util_logWarning ( "Timeout occurred while expecting garbage" ) ;
	pSecs1->state = SECS_EXPECT_RECV_ENQ ;

	/* Leave the socket open.  Its just quiet after the garbage dump. */
	return COND_SILENT ;
      }

      /* The read had a timeout of t1 seconds */
      mtime += mt1 ;

      /* If we are still under T1, then read again */
      if ( mtime <= mt1 ) break ;

      /* Too much time waiting between characters */
      pSecs1->state = SECS_EXPECT_SEND_NAK ;

    case SECS_EXPECT_SEND_NAK :

      /* Send a <NAK> */
      pSecs1->outbuf[0] = SECS_CHAR_NAK ;
      nBytes = gHyp_sock_write ( (SOCKET) pSecs1->fd, 
				 (char*) pSecs1->outbuf,
				 1,
				 (short)(pSecs1->flags&MASK_SOCKET),
  				 &pSecs1->overlapped,
				 pSecs1->pSSL) ;

      if ( guDebugFlags & DEBUG_SECS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "->NAK" ) ;

      if ( nBytes < 0 ) {
	gHyp_util_sysError ( "Failed to write <NAK> to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {

	/* Should not happen */
	gHyp_util_logError ( "Zero bytes written to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      pSecs1->state = SECS_EXPECT_RECV_ENQ ;
      blockReceived = TRUE ;
      ix = 0 ;
      break ;
    
    case SECS_EXPECT_SEND_ACK :

      /* Send an <ACK> */
      pSecs1->outbuf[0] = SECS_CHAR_ACK ;
      nBytes = gHyp_sock_write ( (SOCKET) pSecs1->fd, 
				 (char*) pSecs1->outbuf, 
				 1, 
				 (short)(pSecs1->flags&MASK_SOCKET),
  				&pSecs1->overlapped,
				pSecs1->pSSL ) ;

      if ( guDebugFlags & DEBUG_SECS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "->ACK" ) ;

      if ( nBytes < 0 ) {
	gHyp_util_sysError ( "Failed to write <ACK> to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {

	/* Should not happen */
	gHyp_util_logError ( "Zero bytes written to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }

      /* Set next state. */
      pSecs1->state = SECS_EXPECT_SECSII ;
      
    case SECS_EXPECT_SECSII :

      /* Grab the header bytes 
       *
       *
       *      0      1      2      3      4      5      6      7      8      9
       *  ------------------------------------------------------------------------
       *  ||R|   ||     ||W|   ||     ||B|   ||     ||     ||     ||     ||     || 
       *  ------------------------------------------------------------------------
       *   +-Device Id -++-Str-++-Fnc=++-Block Num--++-System Id--++-Trans Id --+
       *
       *      
       */
      header.deviceId = ((pSecs1->inbuf[SECS_HEADER_UPPERID] & 0x7F) << 8) ;
      header.deviceId |= pSecs1->inbuf[SECS_HEADER_LOWERID] ;
      header.messageId = ((pSecs1->inbuf[SECS_HEADER_STREAM] & 0x7F) << 8) ;
      header.messageId |= pSecs1->inbuf[SECS_HEADER_FUNCTION] ;
      header.blockNum = ((pSecs1->inbuf[SECS_HEADER_UPPERBLK] & 0x7F) << 8);
      header.blockNum |= pSecs1->inbuf[SECS_HEADER_LOWERBLK] ;
      header.SID = (pSecs1->inbuf[SECS_HEADER_UPPERSRC] << 8) ;
      header.SID |= pSecs1->inbuf[SECS_HEADER_LOWERSRC] ;
      header.TID = (pSecs1->inbuf[SECS_HEADER_UPPERTID] << 8);
      header.TID |= pSecs1->inbuf[SECS_HEADER_LOWERTID] ;
      
      header.rBit = (pSecs1->inbuf[SECS_HEADER_UPPERID] & 0x80 ) ;
      header.isReplyExpected = (pSecs1->inbuf[SECS_HEADER_STREAM] & 0x80 ) ;
      header.isLastBlock = (pSecs1->inbuf[SECS_HEADER_UPPERBLK] & 0x80 ) ;
      header.isPrimaryMsg = ((header.messageId & 0xFF) % 2 ) ;
      header.isFirstBlock = ((header.blockNum == 0 && header.isLastBlock) ||
			     (header.blockNum == 1) ) ; 

      pSecs2 = gHyp_instance_getSecs2byId ( pAI, header.deviceId ) ;
	    
      if ( !pSecs2 ) { 
	gHyp_instance_warning ( pAI, STATUS_SECS,
				"SECS II device id %d is not assigned to port %d", 
				header.deviceId, pSecs1->fd ) ;
      } 


      /* Get current header from secsII structure. */
      pHeader = gHyp_secs2_getSecsHeader ( pSecs2, SECS_INCOMING ) ;

      /* Examine message that just arrived */

      if ( header.isFirstBlock ) {

	/* Message appears to be number one of a series...  */

	/* Have previous blocks been received? */
	if ( pHeader ) {

	  /* A header exists, thus a message is already in progress. */
	  if ( header.isPrimaryMsg || !pHeader->isPrimaryMsg ) {

	    /* Just reset secsII buffer. Start over. */
	    gHyp_secs2_resetBuf ( pSecs2, SECS_INCOMING ) ;

	    /* A new primary message can interrupt the one in progress. */
	    pHeader = gHyp_secs2_newSecsHeader ( pSecs2, SECS_INCOMING ) ;
	    *pHeader = header ;	  

	  }
	  else {
	    gHyp_util_logError ( 
	     "SECS-II, non-primary message cannot interrupt current message");
	    pSecs1->state = SECS_EXPECT_SEND_S9F7 ;
	    break ;
	  }
	}
	else {
	  /* Header does not exist. 
	   * First block of message initializes a new Secs1 header 
	   */
	  pHeader = gHyp_secs2_newSecsHeader ( pSecs2, SECS_INCOMING ) ;
	  *pHeader = header ;
	}   
      }
      else {

	/* Not first block */

	if ( pHeader == NULL ) {

	  /* Never got our first block */
	  gHyp_util_logError ( 
	    "SECS-I, first block number not 0 or 1" ) ;
	  pSecs1->state = SECS_EXPECT_SEND_S9F7 ;
	  break ;
	}

	/* Check for duplicate block */
	if ( pHeader->deviceId == header.deviceId &&
	     pHeader->messageId == header.messageId &&
	     pHeader->blockNum == header.blockNum &&
	     pHeader->SID == header.SID &&
	     pHeader->TID == header.TID ) { 
	  gHyp_util_logWarning ( 
	    "SECS-II, duplicate block detected, ignoring." ) ;
	  /* Set next state, expecting an <ENQ> */
	  pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	  ix = 0 ;
	  break ;
	}

	/* Check the stream and function */
	if ( header.messageId != pHeader->messageId ) { 
	  if ( gHyp_secs2_stream ( &header ) !=
	       gHyp_secs2_stream ( pHeader ) ) {
	    gHyp_util_logError ("SECS-II stream mismatch");
	    pSecs1->state = SECS_EXPECT_SEND_S9F3 ;
	  }
	  else {
	    gHyp_util_logError("SECS-II function mismatch");
	    pSecs1->state = SECS_EXPECT_SEND_S9F5 ;
	  }
	  break ;
	}

	/* Make sure block number is in sequence */
	if ( pHeader->blockNum+1 != header.blockNum ) { 
	  gHyp_util_logError ( "SECS-II, block number not in sequence" ) ;
	  pSecs1->state = SECS_EXPECT_SEND_S9F7 ;
	  break ;
	}
	else
	  pHeader->blockNum = header.blockNum ;
	  
	/* Check system bytes */
	if ( header.SID != pHeader->SID ) { 
	  gHyp_util_logError ( 
	    "SECS-II, source Id mismatch on received block" ) ;
	  pSecs1->state = SECS_EXPECT_SEND_S9F7 ;
	  break ;
	}
	if ( header.TID != pHeader->TID ) { 
	  gHyp_util_logError ( 
	    "SECS-II, transaction Id mismatch on received block" ) ;
	  pSecs1->state = SECS_EXPECT_SEND_S9F7 ;
	  break ;
	}
      }

      /* Copy the secsII block into the secsII buffer */
      secsIIblkSize = blkSize - 10 ;

      if ( secsIIblkSize > 0 )
	gHyp_secs2_add2buf ( pSecs2, 
			     pSecs1->inbuf+11,
			     secsIIblkSize,
			     SECS_INCOMING ) ;

      
      /* If this was the last block, then parse the secsII structure */
      if ( header.isLastBlock ) {
	cond = gHyp_secs2_parseSecs ( pSecs2,
				      pHeader,
				      pAI,
				      initialState,
				      mode,
				      TID, SID, stream, function ) ;
	
	if ( cond < 0 ) {
	  if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	    /* We are an equipment, send back F0 abort message to host */
	    gHyp_secs1_outgoing ( pSecs1, pSecs2, pAI,
				  gHyp_secs2_stream ( &header ) ,0,
				  header.deviceId,
				  header.TID,header.SID,
				  MESSAGE_EVENT ) ;
	  }
	  else {
	    gHyp_util_logWarning ( "S%dF0 not sent to equipment",
				   gHyp_secs2_stream ( &header ) ) ;
	  }
	  cond = COND_NORMAL ;
	}
	blockReceived = TRUE ;
      }
      else
	/* Not last block. */
	cond = COND_SILENT ;

      /* Set next state, expecting an <ENQ> */
      pSecs1->state = SECS_EXPECT_RECV_ENQ ;
      ix = 0 ;

      /* When secs1_incoming is called by secs1_outgoing 
       * (because of an ENQ contention or fast arriving ENQ), 
       * then do not return until last block from the master is in, 
       * otherwise the message would be abandoned.
       *
       * If there is not a complete message (cond=SILENT)
       *    AND
       * the calling routine was gHyp_sock_select, 
       * then allow return from this routine.
       */
      if ( cond == COND_SILENT &&
	   initialState != SECS_EXPECT_SEND_EOT &&
	   initialState != SECS_EXPECT_SEND_EOT2 )
	blockReceived = TRUE ;

      break ;

    case SECS_EXPECT_SEND_S9F1 :
      if ( pSecs2 == NULL ) return COND_SILENT ;
      gHyp_secs2_resetBuf ( pSecs2, SECS_OUTGOING ) ;
      if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	gHyp_secs2_add2buf ( pSecs2, pSecs1->inbuf+1,10,SECS_OUTGOING ) ;
	gHyp_secs1_outgoing ( pSecs1, pSecs2, pAI,
			      9,1, header.deviceId,
			      header.TID,header.SID,
			      MESSAGE_EVENT ) ;
      }
      else {
	gHyp_util_logWarning("% S9F1 not sent to equipment");
      }
      return COND_SILENT ;
      
    case SECS_EXPECT_SEND_S9F3 :
      if ( pSecs2 == NULL ) return COND_SILENT ;
      gHyp_secs2_resetBuf ( pSecs2, SECS_OUTGOING ) ;
      if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	gHyp_secs2_add2buf ( pSecs2, pSecs1->inbuf+1,10,SECS_OUTGOING ) ;
	gHyp_secs1_outgoing ( pSecs1, pSecs2, pAI,
			      9,3, header.deviceId,
			      header.TID,header.SID,
			      MESSAGE_EVENT ) ;
      }
      else {
	gHyp_util_logWarning("% S9F3 not sent to equipment");
      }
      return COND_SILENT ;

    case SECS_EXPECT_SEND_S9F5 :
      if ( pSecs2 == NULL ) return COND_SILENT ;
      gHyp_secs2_resetBuf ( pSecs2, SECS_OUTGOING ) ;
      if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	gHyp_secs2_add2buf ( pSecs2, pSecs1->inbuf+1,10,SECS_OUTGOING ) ;
	gHyp_secs1_outgoing ( pSecs1, pSecs2, pAI,
			      9,5, header.deviceId,
			      header.TID,header.SID,
			      MESSAGE_EVENT ) ;
      }
      else {
	gHyp_util_logWarning("% S9F5 not sent to equipment");
      }
      return COND_SILENT ;

    case SECS_EXPECT_SEND_S9F7 :
      if ( pSecs2 == NULL ) return COND_SILENT ;
      gHyp_secs2_resetBuf ( pSecs2, SECS_OUTGOING ) ;
      if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	gHyp_secs2_add2buf ( pSecs2, pSecs1->inbuf+1,10,SECS_OUTGOING ) ;
	gHyp_secs1_outgoing ( pSecs1, pSecs2, pAI,
			      9,7, header.deviceId,
			      header.TID,header.SID,
			      MESSAGE_EVENT ) ;
      }
      else {
	gHyp_util_logWarning("% S9F7 not sent to equipment");
      }
      return COND_SILENT ;

    case SECS_EXPECT_SEND_S9F9 :
      if ( pSecs2 == NULL ) return COND_SILENT ;
      gHyp_secs2_resetBuf ( pSecs2, SECS_OUTGOING ) ;
      if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	gHyp_secs2_add2buf ( pSecs2, pSecs1->inbuf+1,10,SECS_OUTGOING ) ;
	gHyp_secs1_outgoing ( pSecs1, pSecs2, pAI,
			      9,9, header.deviceId,
			      header.TID,header.SID,
			      MESSAGE_EVENT ) ;
      }
      else {
	gHyp_util_logWarning("% S9F9 not sent to equipment");
      }
      return COND_SILENT ;

    case SECS_EXPECT_SEND_S9F11 :
      gHyp_secs2_resetBuf ( pSecs2, SECS_OUTGOING ) ;
      if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	gHyp_secs2_add2buf ( pSecs2, pSecs1->inbuf+1,10,SECS_OUTGOING ) ;
	gHyp_secs1_outgoing ( pSecs1, pSecs2, pAI,
			      9,11, header.deviceId,
			      header.TID,header.SID,
			      MESSAGE_EVENT ) ;  
      }
      else {
	gHyp_util_logWarning("% S9F11 not sent to equipment");
      }
      return COND_SILENT ;

    case SECS_EXPECT_SEND_S9F13 :
      if ( pSecs2 == NULL ) return COND_SILENT ;
      gHyp_secs2_resetBuf ( pSecs2, SECS_OUTGOING ) ;
      if ( gHyp_instance_isRbitSet ( pAI, header.deviceId ) ) {
	gHyp_secs2_add2buf ( pSecs2, pSecs1->inbuf+1,10,SECS_OUTGOING ) ;
	gHyp_secs1_outgoing ( pSecs1, pSecs2, pAI,
			      9,13, header.deviceId,
			      header.TID,header.SID,
			      MESSAGE_EVENT ) ;      
      }
      else {
	gHyp_util_logWarning("% S9F13 not sent to equipment");
      }
      return COND_SILENT ;

    default :
      return COND_SILENT ;
    }
  }
  return cond ;
}

int gHyp_secs1_outgoing ( sSecs1 *pSecs1,
			  sSecs2 *pSecs2,
			  sInstance *pAI,
			  int stream,
			  int function,
			  int id,
			  int TID,
			  int SID,
			  sBYTE mode )
{
  /* Three return values:
   * > 0  = message written 
   * = 0  = no message written, non-fatel case
   * < 0  = fatal error.
   *
   * Called from gHyp_secs_event, gHyp_secs_query, or 
   * gHyp_instance_replyMessage.  A return value of 0 means to resend the
   * message.
   */
  sLOGICAL
    jmpEnabled,
    lastBlockSent = FALSE ;
  
  int
    i,
    nBytes=0,
    blkSize=0,
    mtime=0,
    mt1,
    mt2,
    mt4,
    mt1u,
    retry=0,
    cond=COND_SILENT ;

  unsigned short
    checksum ;

  /*
  struct checkSum_t {
    union {
      unsigned short checksum ;
      unsigned char  checksum_b[2] ;
    } x ;
  } cs ;
  */

  unsigned char
    header[11] ;
       
  char
    *pStr,
    blkStr[MAX_SECS1_BUFFER*3] ;

  sSecsHeader
    *pHeader ;

  /* Initialize a new secs1 header */
  pHeader = gHyp_secs2_newSecsHeader ( pSecs2, SECS_OUTGOING ) ;

  /* Initialize the secs1 header structure */

  /* If we set the R-bit,
   * then we must be the equipment, sending a message to the host.
   * So, when sending to deviceId, we ask if it is a Host w.r.t. us,
   * if so, we set the R-bit.
   */
  pHeader->rBit = gHyp_instance_isRbitSet ( pAI, (sWORD) id );
  pHeader->deviceId = id ;
  pHeader->messageId = (stream << 8) ;
  pHeader->messageId |= function ;
  pHeader->isReplyExpected = ( mode == MESSAGE_QUERY ) ;

  pHeader->blockNum = 1 ;
  pHeader->isFirstBlock = TRUE ;
  pHeader->isLastBlock = gHyp_secs2_isEndBlock ( pSecs2, 1, SECS_OUTGOING ) ;
  pHeader->isPrimaryMsg = ( function % 2 ) ;

  /* Restore system bytes */
  if ( guDebugFlags & DEBUG_SECS )
    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			 "Setting/Restoring TID=0x%02x, SID=0x%02x", 
			 TID, SID ) ;
  pHeader->TID = TID ;
  pHeader->SID = SID ;

  /* Increment transaction id if this is a primary message */
  if ( pHeader->isPrimaryMsg ) {
    pSecs1->TID++ ;
    if ( pSecs1->TID > 0xffff ) pSecs1->TID = 0 ;
  }

  /* Initialize header block */
  header[SECS_HEADER_LENGTH] = 0 ;	/* Don't know length bytes yet */
  header[SECS_HEADER_UPPERID]  = (pHeader->deviceId >> 8) ;
  if ( pHeader->rBit ) header[SECS_HEADER_UPPERID] |= 0x80 ;
  header[SECS_HEADER_LOWERID]  = (sBYTE) pHeader->deviceId ;
  header[SECS_HEADER_STREAM]   = (pHeader->messageId >> 8) ;
  if ( pHeader->isReplyExpected ) header[SECS_HEADER_STREAM] |= 0x80 ;
  header[SECS_HEADER_FUNCTION] = (sBYTE) pHeader->messageId ;
  header[SECS_HEADER_UPPERBLK] = ( pHeader->blockNum >> 8 ) ;
  if ( pHeader->isLastBlock ) header[SECS_HEADER_UPPERBLK] |= 0x80 ;
  header[SECS_HEADER_LOWERBLK] = (sBYTE) pHeader->blockNum ;
  header[SECS_HEADER_UPPERSRC] = (pHeader->SID >> 8) ;
  header[SECS_HEADER_LOWERSRC] = (sBYTE) pHeader->SID ;
  header[SECS_HEADER_UPPERTID] = (pHeader->TID >> 8 ) ;
  header[SECS_HEADER_LOWERTID] = (sBYTE) pHeader->TID ;
    
  /* Set initial state */
  pSecs1->state = SECS_EXPECT_SEND_ENQ ;

  /* Get SECS-I timeouts in integer milliseconds (+ 2 millisecond bonus) */
  mt1 = (int) (pSecs1->t1 * 1000) + 2 ; 
  mt2 = (int) (pSecs1->t2 * 1000) + 2 ;
  mt4 = (int) (pSecs1->t4 * 1000) + 2 ;
  mt1u = mt1 * 1000 ;

  retry = pSecs1->rty ;
 
  if ( guDebugFlags & DEBUG_SECS )
    gHyp_util_logDebug ( 
      FRAME_DEPTH_NULL, DEBUG_SECS,
      "Sending S%dF%d (device %d) (TID=0x%02x) (SID=0x%02x)",
      stream,function,id,TID,SID ) ;

  while ( !lastBlockSent ) {

    /* Process the state and set the next state */    
    switch ( pSecs1->state ) {
      
    case SECS_EXPECT_SEND_ENQ :

      /* Send <EOT>.  Then wait for the message block. */
      pSecs1->outbuf[0] = SECS_CHAR_ENQ ;
      nBytes = gHyp_sock_write ( (SOCKET) pSecs1->fd, 
				 (char*) pSecs1->outbuf,
				 1,
				 (short)(pSecs1->flags&MASK_SOCKET),
			         (OVERLAPPED*) &pSecs1->overlapped,
				 pSecs1->pSSL) ;

      if ( guDebugFlags & DEBUG_SECS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "->ENQ" ) ;

      if ( nBytes < 0 ) {
	gHyp_util_sysError ( "Failed to write <ENQ> to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), 
					  (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {

	/* Should not happen */
	gHyp_util_logError ( "Zero bytes written to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI),
					  (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }

      /* Initialize elapsed timer. */
      mtime = 0 ;

      /* Set next state */
      pSecs1->state = SECS_EXPECT_RECV_EOT ;

    case SECS_EXPECT_RECV_EOT :

      /* If we're nested too deep in handlers, then forget it */
      if ( giJmpLevel == MAX_JMP_LEVEL ) {
	gHyp_util_logError ( "SECS1 RECV EOT jump level overflow at %d", MAX_JMP_LEVEL ) ;
	longjmp ( gsJmpStack[0], COND_FATAL ) ;
      }

      /* Save the previous longjmp return point. */
      jmpEnabled = giJmpEnabled ;
      giJmpEnabled = FALSE ;
      nBytes =  gHyp_sock_read ( (SOCKET) pSecs1->fd,
				 (char*) pSecs1->inbuf, 
				 MAX_SECS1_BUFFER,
				 (short)(pSecs1->flags&MASK_SOCKET),
				 mt1,
				 &pSecs1->nBytes,
  				 &pSecs1->overlapped,
				 pSecs1->pSSL ) ;

      /* Restore the previous longjmp return point. */
      giJmpEnabled = jmpEnabled ;

      if ( nBytes < 0 ) {
	gHyp_util_logError ( "Failed to receive <EOT> from SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), 
					  (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {

	/* A read timeout occurred of .1 second or 100 milliseconds.
	 * Therefore, add 100 to the time since the last read.
	 */
	mtime += mt1 ;
	
	if ( mtime > mt2 ) {

	  /* Too much time waiting for the <EOT> */
	  gHyp_util_logWarning ( "SECS-I T2 timeout waiting for <EOT>" ) ;
	  retry-- ;
	  if ( retry > 0 ) {
	    pSecs1->state = SECS_EXPECT_SEND_ENQ ;
	    break ;
	  }
	  else {
	    gHyp_util_logError ( "Failed to receive <EOT> after %d tries",
				   pSecs1->rty ) ;
	    pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	    gHyp_instance_signalPipe ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	    return COND_SILENT ;
	  }
	}
	break ;

      }
      else {

	/* Got some data. We hope its an <EOT> */
	if ( pSecs1->inbuf[0] != SECS_CHAR_EOT ) {

	  if ( pSecs1->inbuf[0] == SECS_CHAR_ENQ ) {
	    
	    if ( guDebugFlags & DEBUG_SECS )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
				   "<-ENQ" ) ;

	    /* ENQ Contention.  Relinquish if device is not a slave, 
	     * that is, not the host.
	    */
	    if ( !gHyp_instance_isRbitSet ( pAI, (sWORD) id ) ) {
	      
	      /* We are the slave and we are getting an interrupt */
	      if ( guDebugFlags & DEBUG_SECS )
		gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
				   "ENQ contention. Relinquishing..." ) ;
	      pSecs1->state = SECS_EXPECT_SEND_EOT ;
	      cond = lHyp_secs1_incoming ( pSecs1, 
					   gHyp_instance_getConcept(pAI),
					   pAI,
					   mode,
					   TID,
					   SID,
					   pHeader->deviceId,
					   stream,
					   function ) ;
	      pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	      if ( cond < 0 ) return cond ;
	      break ;
	    }
	    else {
	   
	      /* Not a slave but master!. Keep listening for EOT! */
	      gHyp_util_logError ( 
		"Expecting <EOT>, got <ENQ> from SECS device %s",
		  pSecs1->device ) ;
	      break ;
	    }
	  }
	  else if ( pSecs1->inbuf[0] == SECS_CHAR_NAK ) {
	    if ( guDebugFlags & DEBUG_SECS )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
				   "<-NAK", pSecs1->inbuf[0] ) ;
	    /* If we are a slave, exit with an error. */
	    if ( !gHyp_instance_isRbitSet ( pAI, (sWORD) id ) ) {
	      pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	      return COND_ERROR ;
            }
	    /* Otherwise keep waiting for <EOT> */
	    break ;
	  }
	  else {
	    if ( guDebugFlags & DEBUG_SECS )
	      gHyp_util_logError ( 
		"Expecting <EOT>, got <0x%02x> from SECS device %s",
		pSecs1->inbuf[0], pSecs1->device ) ;

	    /* Keep waiting for <EOT> */
	    break ;

	  }
	}
      }


      if ( guDebugFlags & DEBUG_SECS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "<-EOT" ) ;

      /* Reply back with secs block. */
      pSecs1->state = SECS_EXPECT_SEND_BLK ;
      
    case SECS_EXPECT_SEND_BLK :

      /* Fill in the SECS II block */
      blkSize = gHyp_secs2_getBlock ( pSecs2, 
				      pSecs1->outbuf+11, 
				      pHeader->blockNum, 
				      SECS_OUTGOING ) ;

      /* Fill in the length byte */
      blkSize += 10 ;
      header[0] = blkSize ;

      /* Insert the block number */
      header[SECS_HEADER_UPPERBLK] = ( pHeader->blockNum >> 8 ) ;
      header[SECS_HEADER_LOWERBLK] = (sBYTE) pHeader->blockNum ;

      /* If the next block to send is the last block, then set the E-bit */
      pHeader->isLastBlock = gHyp_secs2_isEndBlock ( pSecs2, 
						     pHeader->blockNum,
						     SECS_OUTGOING ) ;
      if ( pHeader->isLastBlock ) header[SECS_HEADER_UPPERBLK] |= 0x80 ;

      /* Fill in the header after incrementing the block count */
      memcpy ( pSecs1->outbuf, header, 11 ) ;

      /* Calculate the checksum */
      for ( checksum=0,i=1; i<blkSize+1; i++ ) 
	checksum += pSecs1->outbuf[i] ;
 
      /* Insert the checksum */
      pSecs1->outbuf[blkSize+1] = (sBYTE) (checksum >> 8);
      pSecs1->outbuf[blkSize+2] = (sBYTE) (checksum >> 0);

      /* Write the SECS data */
      nBytes = 1 + blkSize + 2 ; 
      nBytes = gHyp_sock_write ( (SOCKET) pSecs1->fd, 
				 (char*) pSecs1->outbuf, 
				 nBytes,
				 (short)(pSecs1->flags&MASK_SOCKET),
				 (OVERLAPPED*) &pSecs1->overlapped,
				 pSecs1->pSSL) ;

      if ( guDebugFlags & DEBUG_SECS ) {
	pStr = blkStr ;
	for ( i=0; i<nBytes; i++ ) {
	  sprintf ( pStr, "%02x ", pSecs1->outbuf[i] ) ;
	  pStr += 3 ;
	}
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "->BLK [%s]", blkStr ) ; 
      }

      if ( nBytes < 0 ) {
	gHyp_util_sysError ( "Failed to write SECS I block to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), 
					  (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {
	/* Should not happen */
	gHyp_util_logError ( "Zero bytes written to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI),
					  (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }

      /* Re-initialize elapsed timer. */
      mtime = 0 ;

      /* Set next state */
      pSecs1->state = SECS_EXPECT_RECV_ACK ;
      
    case SECS_EXPECT_RECV_ACK :

      /* If we're nested too deep in handlers, then forget it */
      if ( giJmpLevel == MAX_JMP_LEVEL ) {
	gHyp_util_logError ( "SECS1 RECV ACK jump level overflow at %d", MAX_JMP_LEVEL ) ;
	longjmp ( gsJmpStack[0], COND_FATAL ) ;
      }

      /* Save the previous longjmp return point. */
      jmpEnabled = giJmpEnabled ;
      giJmpEnabled = FALSE ;
      nBytes =  gHyp_sock_read ( (SOCKET) pSecs1->fd,
				 (char*) pSecs1->inbuf, 
				 MAX_SECS1_BUFFER,
				 (short)(pSecs1->flags&MASK_SOCKET),
				 mt1,
				 &pSecs1->nBytes,
  			         &pSecs1->overlapped,
				 pSecs1->pSSL ) ;

      /* Restore the previous longjmp return point. */
      giJmpEnabled = jmpEnabled ;

      if ( nBytes < 0 ) {
	gHyp_util_logError ( "Failed to receive <ACK> from SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), 
					  (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {

	/* Timeout occurred of .1 second or 100 milliseconds.
	 * Therefore, add 100 to the time since the last read.
	 */
	mtime += mt1 ;
	
	if ( mtime > mt2 ) {

	  /* Too much time waiting for the <ACK> */
	  gHyp_util_logWarning ( "SECS-I T2 timeout waiting for <ACK>" ) ;
	  retry-- ;
	  if ( retry > 0 ) {
	    pSecs1->state = SECS_EXPECT_SEND_ENQ ;
	    break ;
	  }
	  else {
	    gHyp_util_logError ( "Failed to receive <ACK> after %d tries",
				   pSecs1->rty ) ;
	    gHyp_instance_signalPipe ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	    pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	    return COND_SILENT ;
	  }
	}
	break ;
      }
      else {

	if ( pSecs1->inbuf[0] != SECS_CHAR_ACK ) {
	    
	  if ( pSecs1->inbuf[0] == SECS_CHAR_NAK ) {
	    if ( guDebugFlags & DEBUG_SECS )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
				   "<-NAK" ) ;
	  } 
	  else {
	    if ( guDebugFlags & DEBUG_SECS )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
				   "<-%02x", pSecs1->inbuf[0] ) ;
	  }

	  /* Reject method.  Send again if the retry count allows it. */
	  retry-- ;
	  if ( retry > 0 ) { 
	    pSecs1->state = SECS_EXPECT_SEND_ENQ ;
	    break ;
	  }
	  else {
	    gHyp_util_logError ( "Failed to send SECS message after %d tries",
				 pSecs1->rty ) ;
	    lastBlockSent = TRUE ;
	    break ;
	  }
	}	  
      }

      if ( guDebugFlags & DEBUG_SECS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "<-ACK" ) ;

      if ( pHeader->isLastBlock ) {

	if ( pHeader->isReplyExpected && 
	     nBytes > 1 && 
	     pSecs1->inbuf[1] == SECS_CHAR_ENQ ) {
	  
	  /* ENQ came back FAST from the sender */
	  if ( guDebugFlags & DEBUG_SECS )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
				 "<-ENQ(*)" ) ;
	  pSecs1->state = SECS_EXPECT_SEND_EOT2 ;
	  cond = lHyp_secs1_incoming ( pSecs1, 
					 gHyp_instance_getConcept(pAI),
					 pAI,
					 mode,
					 TID,
					 SID,
					 id,
					 stream,
					 function ) ;
	}
	else
	  cond = COND_NORMAL ;

	pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	lastBlockSent = TRUE ;
      }
      else {
	/* Increment the block count for the next block to send. */
	pHeader->blockNum++ ;
	/* Set next state, back to sending an <ENQ> for next block. */
	pSecs1->state = SECS_EXPECT_SEND_ENQ ;
	/* Reset retry counter */
	retry = pSecs1->rty ;
      }
      break ;

    case SECS_EXPECT_SEND_NAK :
      
      /* Send a <NAK> */
      pSecs1->outbuf[0] = SECS_CHAR_NAK ;
      nBytes = gHyp_sock_write ( (SOCKET) pSecs1->fd, 
				 (char*) pSecs1->outbuf,
				 1,
				 (short)(pSecs1->flags&MASK_SOCKET),
				 (OVERLAPPED*) &pSecs1->overlapped,
				 pSecs1->pSSL) ;

      if ( guDebugFlags & DEBUG_SECS )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_SECS,
			     "->NAK" ) ;
      
      if ( nBytes < 0 ) {
	gHyp_util_sysError ( "Failed to write <NAK> to SECS device %s",
			     pSecs1->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), 
					  (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      else if ( nBytes == 0 ) {
	
	/* Should not happen */
	gHyp_util_logError ( "Zero bytes written to SECS device %s",
			     pSecs1->device ) ;

	gHyp_instance_signalHangup ( pAI, (int) pSecs1->fd, pSecs1->modifier, id ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), 
					  (SOCKET) pSecs1->fd ) ;
	return COND_SILENT ;
      }
      pSecs1->state = SECS_EXPECT_RECV_EOT ;
      break ;
    default:
      return COND_SILENT ;
    }
  }
  return cond ;
}


int gHyp_secs1_incoming ( sSecs1 *pSecs1, 
			  sConcept *pConcept,
			  sInstance *pParentAI )
{

  sWORD
    id=NULL_DEVICEID ;

  sSecs1
    *pListenSecs1 ;
  
  sInstance
    *pAIassigned ;


  /* The socket pSecs1-socket has data in it/
   * Get the instance for which the data gets assigned to. 
   */
  pAIassigned = gHyp_concept_getInstForFd ( pConcept, pSecs1->fd ) ;

  if ( pAIassigned ) {

    /* Get id that is assigned to the port */
    id = gHyp_instance_getDeviceId ( pAIassigned, (SOCKET) pSecs1->fd ) ;
    /*gHyp_util_debug("found %d for port %d",id,pSecs1->fd);*/

  }
  else {

    /*gHyp_util_debug( "No instance has assigned any id to the port.");*/

    /* No instance has assigned any id to the port.
     * Check if this is a newly created channel.
     */
    if ( pSecs1->fd != pSecs1->parentSocket ) {

      /* This is a newly created channel */
      /*gHyp_util_debug ( "Looking for instance assigned to listen port" );*/
      pAIassigned = gHyp_concept_getInstForFd ( pConcept, pSecs1->parentSocket ) ;

      if ( pAIassigned ) {

	/* Getting device id assigned to the listen port */
	id = gHyp_instance_getDeviceId ( pAIassigned, pSecs1->parentSocket ) ;

	if ( id == NULL_DEVICEID ) {
          gHyp_util_logError ( "No device id has been assigned to listen port %d",
			 pSecs1->parentSocket ) ;
	  /* Close the port */
	  gHyp_instance_signalHangup ( pParentAI, (int) pSecs1->fd, pSecs1->modifier, id) ;
	  gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pSecs1->fd ) ;
	  return COND_SILENT ;
	}
	
	/*gHyp_util_debug ( "Taking device id %d from port %d to port %d",
			    id, pSecs1->parentSocket, pSecs1->fd );
	 
	*/

	/* Take any SSL structures as well */
	pListenSecs1 = (sSecs1*) gHyp_concept_getSocketObject ( pConcept, 
							  (SOCKET) pSecs1->parentSocket, 
							  DATA_OBJECT_SECS1 ) ;
#ifdef AS_SSL
	if ( pListenSecs1->pSSL != NULL ) {
	  pSecs1->pSSL = gHyp_sock_copySSL ( pListenSecs1->pSSL ) ;
	}
#endif	   
	if ( pListenSecs1->forwardPorts != NULL ) {
	  pSecs1->forwardPorts = gHyp_secs1_copyForwardPort ( pListenSecs1, id ) ;
	}

	/* Take this id */
	gHyp_instance_updateFd ( pParentAI, pSecs1->fd, id, NULL, FALSE ) ;

	/* Create a placeholder for the parent so that we can reassign the device id. */
	gHyp_instance_updateFd ( pAIassigned, pSecs1->parentSocket, NULL_DEVICEID, NULL, FALSE ) ;

	/* Take and reassign the rest of the id's as well. */
	while ( 1 ) {
	  id = gHyp_instance_getDeviceId ( pAIassigned, pSecs1->parentSocket ) ;
	  if ( id == NULL_DEVICEID ) break ;
 	  gHyp_instance_updateFd ( pParentAI, pSecs1->fd, id, NULL, FALSE ) ;
	} 

      }
    }
  }

  return lHyp_secs1_incoming ( pSecs1, 
			       pConcept,
			       pAIassigned,
			       MESSAGE_QUERY, 
			       0, 0, id, 0 ,0 ) ;
}

float gHyp_secs1_t1 ( sSecs1 *pSecs1 )
{
  return pSecs1->t1 ;
}

float gHyp_secs1_t2 ( sSecs1 *pSecs1 )
{
  return pSecs1->t2 ;
}

float gHyp_secs1_t4 ( sSecs1 *pSecs1 )
{
  return pSecs1->t4 ;
}

int gHyp_secs1_rty ( sSecs1 *pSecs1 )
{
  return pSecs1->rty ;
}

unsigned gHyp_secs1_speed ( sSecs1 *pSecs1 )
{
  return pSecs1->modifier ;
}

unsigned gHyp_secs1_port ( sSecs1 *pSecs1 )
{
  return pSecs1->modifier ;
}

char* gHyp_secs1_device ( sSecs1 *pSecs1 )
{
  return pSecs1->device ;
}

unsigned gHyp_secs1_TID ( sSecs1 *pSecs1 )
{
  return pSecs1->TID ;
}

unsigned gHyp_secs1_SID ( sSecs1 *pSecs1 )
{
  return pSecs1->SID ;
}

static void lHyp_secs1_httpHeaders ( char *pLine, 
				     char *pContent, 
				     sData* pAttributeData )
{

  int
    n,
    width ;

  char
    *pChar ;

  sData
    *pAttr,
    *pValue ;

  while ( pLine < pContent ) {

    /* Advamnce past newline */
    pLine += strspn ( pLine, "\r\n" ) ;

    /* Search for the end of the attribute value section.
     * This is the first  "\r\n" that isn't preceeded by a ','
     */
    pChar = pLine ;
    while ( pChar < pContent ) {
      width = strcspn ( pChar, "\r\n" ) ;
      if ( width > 2 ) {
	if ( *(pLine+width-1) == ',' && *(pLine+width-2) != '\\' ) {
	  /* Search for next end-of-line */
	  pChar = pLine+width ;
	  pChar += strspn ( pChar, "\r\n" ) ;
	  continue ;
	}
      }
      /* Mark the end of the attribute section */
      pChar += width ;
      *pChar = '\0' ;
      break ;
    }
    if ( pChar >= pContent ) break ;
    /* Where's the colon separating attribute name from value */
    pChar = strchr ( pLine, ':' ) ;
    if ( pChar ) {

      *pChar = '\0' ;

      /* Capture the attribute name */
      gHyp_util_trim ( pLine ) ;

      if ( strlen ( pLine ) > 0 ) {

	/* The attribute has substance */
	pAttr = gHyp_data_new ( pLine ) ;

	/* Get start of attribute value */
	pChar++ ;
	pLine = pChar ;
	pLine += strspn ( pLine, " \t" ) ;

	while ( pChar < pContent ) {
	  /* Search for ',' that separates values */
	  pChar = strchr ( pLine, ',' ) ;
	  if ( pChar ) {
	    if ( *(pChar-1) != '\\' ) {
	      /* Replace ',' with null */
	      *pChar = '\0' ;
	      gHyp_util_trim ( pLine ) ;
	      if ( strlen ( pLine ) > 0 ) {
		pValue = gHyp_data_new ( NULL ) ;
		n = gHyp_util_parseString ( pLine ) ;
		gHyp_data_setStr2 ( pValue, pLine, n ) ;
		gHyp_data_append ( pAttr, pValue ) ;
	      }
	      pLine = pChar+1 ;
	      pLine += strspn ( pLine, " \t\r\n" ) ;
	    }
	    else {
	      /* False ',' */
	      pLine = pChar+1;
	    }
	  }
	  else {
	    /* No more values */ 
	    break ;
	  }
	}
	
	/* Take the remainder */
	gHyp_util_trim ( pLine ) ;
	if ( strlen ( pLine ) ) {
	  pValue = gHyp_data_new ( NULL ) ;
	  n = gHyp_util_parseString ( pLine ) ;
	  gHyp_data_setStr2 ( pValue, pLine, n ) ;
	  gHyp_data_append ( pAttr, pValue ) ;
	}

        gHyp_data_append ( pAttributeData, pAttr ) ;

      }
      else {
	/* Attribute name is blank */
	gHyp_util_logWarning("No attribute name on line '%s'",pLine ) ;
      }
    }
    else if ( strlen ( pLine ) > 0 ) {
      /* No semicolon - bad format */
      gHyp_util_logWarning("No ':' separating attribute name from value on line '%s'",pLine ) ;
    }

    /* Get next line */
    pLine = pLine + strlen ( pLine ) + 1 ;
  }
}

int gHyp_secs1_rawIncoming ( sSecs1 *pPort, sConcept *pConcept, sInstance *pAI, sBYTE objectType ) 
{
  /* Three return values:
   * > 0  = message read 
   * = 0  = no message, alarm, timeout, pipe signal, or other non-fatal case
   * < 0  = fatal error, hangup signal
   *
   * Called from waiting for reply message or unsolicited message.
   *    - called from gHyp_msgio_select<-gHyp_instance_read
   *    - a return value of -1 is required to trigger handlers
   *	- a return value of 0 re-invokes msgio_select to read again
   *	- a return value > 0 satisfies the read.
   */
  sLOGICAL
    f,
    deleteForwardPort,
    isReply,
    isVector,
    isChunkedTransferEncoded=FALSE,
    isChunkedTransferDone=FALSE,
    isEndOfMessage = FALSE,
    isURLEncoded=FALSE,
    isPlainText=FALSE,
    isPOSTdata=FALSE,
    isXMLEncoded=FALSE,
    isLineBased,
    doKnitting=FALSE,
    isMessageComplete,
    jmpEnabled ;
  
  sWORD
    forwardId,
    id = NULL_DEVICEID ;

  int
    i,
    n,
    ss,
    context,
    lineLen=0,
    width,width2,width3,
    forwardCount,
    nBytes=0,
    actualContentLength=0,
    contentLength ;

  sBYTE
    state,
    httpState;

  sSecs1
    *pForwardPort,
    *pListenPort ;

  char
    *pBuf,
    *pEndBuf,
    buffer[PORT_READ_SIZE+1],
    *pChar,
    *pTokenName,
    *pTokenValue,
    *pMsg,
    *pContent,*pContent2,
    *pLine,
    *pArg1,*pArg2,*pArg3,
    prefix[3],
    target[TARGET_SIZE+1],
    sender[SENDER_SIZE+1],
    method[METHOD_SIZE+1],
    mode[MODE_SIZE+1],
    value[VALUE_SIZE+2+1] ; /* Room for 2 extra characters.*/

  sData
    *pAttributeData=NULL,
    *pContentData,
    *pSTATUS,
    *pData,
    *pFirst,
    *pLast,
    *pNext,
    *pValue,
    *pTV,
    *pVariable ;

  sHTTP
    *pHTTP=NULL ;

  sInstance
     *pAIassigned ;

  SOCKET
    fd ;

#define URL_EXPECT_TOKEN 0
#define URL_EXPECT_VALUE 1

  /* If we're nested too deep in handlers, then forget it */
  if ( giJmpLevel == MAX_JMP_LEVEL ) {
    gHyp_util_logError ( "Port jump level overflow at %d", MAX_JMP_LEVEL ) ;
    longjmp ( gsJmpStack[0], COND_FATAL ) ;
  }
  
  /* Who gets the data? */
  pAIassigned = gHyp_concept_getInstForFd ( pConcept, pPort->fd ) ;

  if ( pAIassigned ) {

    /* Get id that is assigned to the port */
    id = gHyp_instance_getDeviceId ( pAIassigned, (SOCKET) pPort->fd ) ;

    /*gHyp_util_debug("found %d for port %d",id,pPort->fd);*/

  }
  else {

    /* No instance has assigned any id to the port.*/
    if ( pPort->fd != pPort->parentSocket ) {

      /* This is a newly created channel */
      /*gHyp_util_debug ( "Looking for device id assigned to listen port" );*/
      pAIassigned = gHyp_concept_getInstForFd ( pConcept, pPort->parentSocket ) ;

      if ( pAIassigned ) {

	id = gHyp_instance_getDeviceId ( pAIassigned, pPort->parentSocket ) ;

	if ( id == NULL_DEVICEID ) {
          gHyp_util_logError ( "No device id has been assigned to listen port %d",
			 pPort->parentSocket ) ;
	  /* Close the port */
	  gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
	  gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pPort->fd ) ;
	  return COND_SILENT ;
	}

  	/*gHyp_util_debug ( "Taking device id %d from port %d to port %d",
	 		    id, pPort->parentSocket, pPort->fd );
	 
	*/

	/* Take any SSL structures as well */
	pListenPort = (sSecs1*) gHyp_concept_getSocketObject ( pConcept, 
							      (SOCKET) pPort->parentSocket, 
							      objectType ) ;
#ifdef AS_SSL
	if ( pListenPort->pSSL != NULL ) {
	  pPort->pSSL = gHyp_sock_copySSL ( pListenPort->pSSL ) ;
	}
#endif

	if ( pListenPort->forwardPorts != NULL ) {
	  /*gHyp_util_debug("Copy to %d from %d for %d",pPort->fd,pListenPort->fd,id);*/
	  pPort->forwardPorts = gHyp_secs1_copyForwardPort ( pListenPort, id ) ;
	} 

	/* Transfer the id. */
	gHyp_instance_updateFd ( pAIassigned, pPort->fd, id, NULL, FALSE ) ;

	/* Create a placeholder for the parent so that we can reassign the device id. */
	gHyp_instance_updateFd ( pAIassigned, pPort->parentSocket, NULL_DEVICEID, NULL, FALSE ) ;

	/* Take and reassign the rest of the id's as well.
	 *
	 * Actually, for ports, don't do this.  Each id must come from a separate
	 * incoming connection request.
	 *
	while ( 1 ) {
	  id = gHyp_instance_getDeviceId ( pAIassigned, pPort->parentSocket ) ;
	  if ( id == NULL_DEVICEID ) break ;
 	  gHyp_instance_updateFd ( pParentAI, pPort->socket, id, NULL, FALSE ) ;
	} 
	********/
      }
    }
  }

  /* Save the previous longjmp return point. */
  jmpEnabled = giJmpEnabled ;
  giJmpEnabled = FALSE ;
  pBuf = buffer ;
  nBytes =  gHyp_sock_read ( (SOCKET) pPort->fd,
			     (char*) pBuf, 
			     PORT_READ_SIZE,
			     (short)(pPort->flags&MASK_SOCKET),
			     0, /* 0 means read already completed, just get result */
			     &pPort->nBytes,
  			     &pPort->overlapped,
			     pPort->pSSL) ;

  /* Restore the previous longjmp return point. */
  giJmpEnabled = jmpEnabled ;
  
  if ( nBytes < 0 ) {
    gHyp_util_logError ( "Failed to read data from device %s",
			 pPort->device ) ;
    /* Close the port */
    gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
    gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pPort->fd ) ;
    return COND_SILENT ;
  }
  else if ( nBytes == 0 ) {
    
    gHyp_util_logWarning ( "Failed to receive data from device %d (%s)",
			   id, pPort->device ) ;

    if ( (pPort->flags & SOCKET_TCP) ) {
      /* Zero bytes from TCP/IP sockets is always fatal */
      gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
      gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pPort->fd ) ;
    }
    else
      gHyp_instance_signalPipe ( pAI, (int) pPort->fd, pPort->modifier, id ) ;

    return COND_SILENT ;
  }
  else {

    if ( !pAIassigned ) {
      gHyp_util_logError ( "No id is assigned to port %d for device %s",
			   pPort->fd,
			   pPort->device);
      return COND_SILENT ;
    }

    if ( id == NULL_DEVICEID ) {
      gHyp_util_logError ( "Device id is unassigned");
      return COND_SILENT ;
    }

    /* If the object type is HTTP, then we need to buffer
     * all the data until we get the complete record.
     */
    buffer[nBytes] = '\0' ;
    /*gHyp_util_debug("%d bytes",nBytes);*/
    /*gHyp_util_debug("---------------------------------------------");*/
    /*gHyp_util_debug("%s",pBuf);*/
    /*gHyp_util_debug("---------------------------------------------");*/
    pEndBuf = pBuf + nBytes ;

    /* Is the port data being forwarded? */
    forwardCount = 0 ;
    if ( pPort->forwardPorts ) {

      for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pPort->forwardPorts ) ;
	    (f && pData) || (pFirst && pData != pFirst );
	    f=FALSE, pData = pNext, pFirst = gHyp_data_getFirst ( pPort->forwardPorts ) ) {
        
	pNext = gHyp_data_getNext ( pData ) ;

	deleteForwardPort = TRUE ;
	forwardId = gHyp_data_getInt ( pData, 1, TRUE ) ;
        pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, forwardId ) ;
	if ( pAIassigned ) {

	  fd = gHyp_instance_getDeviceFd ( pAIassigned, forwardId ) ;
	  if ( fd != INVALID_SOCKET ) {

	    objectType = gHyp_concept_getSocketObjectType ( pConcept, fd ) ;
	    if ( objectType == DATA_OBJECT_PORT || objectType == DATA_OBJECT_HTTP ) {

	      pForwardPort = (sSecs1*) gHyp_concept_getSocketObject (
			      pConcept, 
	    		      fd, 
			      objectType  ) ;
	      if ( !(gHyp_secs1_flags(pForwardPort) & SOCKET_LISTEN) ) {

		/*gHyp_util_debug("%d->%d",id,forwardId);*/

 		n = gHyp_sock_write ( (SOCKET) fd, 
				      buffer, 
				      nBytes,
				      (short) (pForwardPort->flags&MASK_SOCKET), 
				      (OVERLAPPED*) &pForwardPort->overlapped, 
				      pForwardPort->pSSL ) ;

		if ( n < 0 ) {
		  gHyp_util_sysError ( "Failed to forward data to device %d",forwardId ) ;
		  gHyp_instance_signalHangup ( pAIassigned, (int) fd, pForwardPort->modifier, forwardId ) ;
		  gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) fd ) ;
		}
		else if ( nBytes == 0 ) {
	          gHyp_util_logError ( "Zero bytes written to device %d",
			 forwardId  ) ;
		  gHyp_instance_signalHangup ( pAI, (int) fd, pForwardPort->modifier, forwardId ) ;
		  gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) fd ) ;
		}		
		else { 
		  deleteForwardPort = FALSE ;
		  forwardCount++ ;
		}
	      }
	    }
	  }
	}
	if ( deleteForwardPort ) {
          gHyp_data_detach ( pData ) ;
	  gHyp_data_delete ( pData ) ;
	}
      }
      if ( forwardCount > 0 ) return COND_SILENT ;
    }

    if ( objectType == DATA_OBJECT_HTTP ) {

      pHTTP = pPort->pHTTP ;

      /* Get the state of the object and its current contents */
      httpState = gHyp_http_getState ( pHTTP ) ;
      pAttributeData = gHyp_http_getAttributeData( pHTTP ) ;
      pContentData = gHyp_http_getContentData ( pHTTP ) ;
      actualContentLength = gHyp_http_getActualContentLength( pHTTP ) ;
      contentLength = gHyp_http_getContentLength( pHTTP ) ;
      isChunkedTransferEncoded = gHyp_http_isChunkedTransferEncoded(pHTTP);
      isXMLEncoded = gHyp_http_isXMLEncoded(pHTTP);
      isURLEncoded = gHyp_http_isURLEncoded(pHTTP);
      isPlainText = gHyp_http_isPlainText(pHTTP);

      if ( httpState == HTTP_EXPECT_HEADER ) {

	/* Look for the double newline. */
	pContent = pBuf + nBytes ;
	pContent2 = strstr ( pBuf, "\n\n" ) ;
	if ( pContent2 && pContent2 < pContent ) pContent = pContent2 ;
	pContent2 = strstr ( pBuf, "\r\n\r\n" ) ;
	if ( pContent2 && pContent2 < pContent ) pContent = pContent2 ;
	*pContent = '\0' ;

	if ( pContent < (pBuf+nBytes) ) {
	  /* There is content */
	  pContent++ ;
	  pContent += strspn ( pContent, "\r\n" ) ;
	  actualContentLength = nBytes - (pContent-buffer) ;
	}

	/* Now look for "Content-Length" and get value */
	pChar = strstr ( pBuf, "Content-Length" ) ;
	if ( !pChar ) pChar = strstr ( pBuf, "Content-length" ) ;
	if ( pChar ) {
	  /* Scan for end of line */
	  width = strcspn ( pChar, "\r\n" ) ;
	  if ( width > 0 && width < VALUE_SIZE ) {
	    strncpy ( value, pChar, width ) ;
	    value[width]='\0' ;
	    pChar = strchr ( value, ':' ) ;
	    if ( pChar ) {
	      pChar++ ;
	      if ( *pChar ) {
		pChar += strspn ( pChar, " \t" ) ;
		gHyp_util_trim ( pChar ) ;
		sscanf ( pChar, "%d", &contentLength )  ;
		if ( contentLength < 0 ) contentLength = 0 ;
	      }
	    }
	  }
	}

	/* Look for "Transfer-Encoding" and see if it is "chunked" */
	pChar = strstr ( pBuf, "Transfer-Encoding" ) ;
	if ( !pChar ) pChar = strstr ( pBuf, "Transfer-encoding" ) ;
	if ( pChar ) {
	  /* Scan for end of line */
	  width = strcspn ( pChar, "\r\n" ) ;
	  if ( width > 0 && width < VALUE_SIZE ) {
	    strncpy ( value, pChar, width ) ;
	    value[width]='\0' ;
	    pChar = strchr ( value, ':' ) ;
	    if ( pChar ) {
	      pChar++ ;
	      if ( *pChar ) {
		pChar += strspn ( pChar, " \t" ) ;
		gHyp_util_trim ( pChar ) ;
		if ( strcmp ( pChar, "chunked" ) == 0 ) {
		  isChunkedTransferEncoded = TRUE ;
		  gHyp_http_setChunkedTransferEncoded ( pHTTP ) ;
		}
	      }
	    }
	  }
	}

	/* Look for "Content-Type" and see if it is "text/xml or text/html" */
	pChar = strstr ( pBuf, "Content-Type" ) ;
	if ( !pChar ) pChar = strstr ( pBuf, "Content-type" ) ;
	if ( pChar ) {
	  /* Scan for end of line */
	  width = strcspn ( pChar, "\r\n" ) ;
	  if ( width > 0 && width < VALUE_SIZE ) {
	    strncpy ( value, pChar, width ) ;
	    value[width]='\0' ;
	    pChar = strchr ( value, ':' ) ;
	    if ( pChar ) {
	      pChar++ ;
	      if ( *pChar ) {
		pChar += strspn ( pChar, " \t" ) ;
		gHyp_util_trim ( pChar ) ;
		if ( strstr ( pChar, "text/xml" ) != NULL ||
		     strstr ( pChar, "text/html" ) != NULL ||
		     strstr ( pChar, "application/soap" ) != NULL ) {
		  isXMLEncoded = TRUE ;
		  gHyp_http_setXMLEncoded ( pHTTP ) ;
		}
		else if ( strstr ( pChar, "text/plain" ) != NULL ) {
		  isPlainText = TRUE ;
		  gHyp_http_setPlainText ( pHTTP ) ;
		}
		else if ( strstr ( pChar, "application/x-www-form-urlencoded" ) != NULL ) {
		  isURLEncoded = TRUE ;
		  gHyp_http_setURLEncoded ( pHTTP ) ;
		}
	      }
	    }
	  }
	}


	/* Get the header elements.
	 *
	 * Eg: GET /target HTTP/1.1
	 * Eg: HTTP/1.1 200 Ok
	 *
	 * The verb.
	 * The target.
	 * The attributes. 
	 */
	lineLen = strcspn ( pBuf, "\r\n" ) ;
	if ( lineLen < 0 ) {
	  /* Not good.  Something wrong */
	  gHyp_util_logError("Invalid HTTP header '%s'",pBuf ) ;
	  return COND_SILENT ;
	}
	else {

	  /* Search for the 3 components on the first line */
	  pLine = pBuf ;

	  /* Terminate the end of the line */
	  *(pLine+lineLen) = '\0' ;

	  /* The first component ends with whitespace */
	  width = strcspn ( pLine, " \t" ) ;
	  if ( width == 0 ) { 
    	    gHyp_util_logError("Invalid HTTP header '%s'",pLine ) ;
	    return COND_SILENT ;
	  }
	  /* Terminate first component */
	  *(pLine+width) = '\0' ;
	  pArg1 = pLine ;

	  /* Advance to start of component 2 */
	  pLine += (width+1) ;
	  pLine += strspn ( pLine, " \t" ) ;	  

	  /* The second component ends with whitespace */
	  width = strcspn ( pLine, " \t" ) ;
	  if ( width == 0 ) { 
    	    gHyp_util_logError("Invalid HTTP header '%s'",pLine ) ;
	    return COND_SILENT ;
	  }
	  /* Terminate second component */
	  *(pLine+width) = '\0' ;
	  pArg2 = pLine ;

	  /* Advance to start of component 3 */
	  pLine += (width+1) ;
	  pLine += strspn ( pLine, " \t" ) ;	  

	  /* The third component ends with the line */
	  width = strlen ( pLine ) ;
	  if ( width == 0 ) { 
    	    gHyp_util_logError("Invalid HTTP header '%s'",pLine ) ;
	    return COND_SILENT ;
	  }
	  pArg3 = pLine ;

	  gHyp_http_setHeader ( pHTTP, pArg1, pArg2, pArg3 ) ;

	  /* For each subsequent line, up to pContent, extract the attributes */
	  pLine = pBuf + lineLen + 1 ;

	  lHyp_secs1_httpHeaders ( pLine, pContent, pAttributeData ) ;

	  /* Finished attribute section */
	  httpState = HTTP_EXPECT_CONTENT ;
	  gHyp_http_setState ( pHTTP, httpState ) ;

	}
      }
      else {
	/* We've already processed the header.
	 * The entire buffer is content continued.
	 * Add the length to the content received so far.
	 */
	pContent = buffer ;

	actualContentLength += nBytes ;
      }

      /* Set the pointer to the content */
      pBuf = pContent ;

      if ( isChunkedTransferEncoded ) {

	if ( contentLength == 0 ) {

	  /* Content hasn't been specified.
	   * pBuf should be pointing to a hex value which is
	   * the content length.
	   */
	  pChar = pBuf ;
	  width = strcspn ( pChar, "\r\n" ) ;
	  if ( width > 0 && width < VALUE_SIZE ) {
	    strncpy ( value, pChar, width ) ;
	    value[width]='\0' ;
	    pChar = strchr ( value, ';' ) ;
	    if ( pChar ) *pChar = '\0' ;
	    gHyp_util_trim ( value ) ;
	    sscanf ( value, "%x", &contentLength )  ;
	    if ( contentLength < 0 ) 

	      contentLength = 0 ;
	    
	    else if ( contentLength == 0 ) {

	      /*gHyp_util_debug("Chunk transfer complete");*/
	      isChunkedTransferDone = TRUE ;

	    }
	    /*gHyp_util_debug("Chunked size is %d",contentLength);*/

	    if ( contentLength > 0 ) {

	      /* Got a content length. Get new start of content */
	      pChar = pBuf + width ;
	      pChar += strspn ( pChar, "\r\n" ) ;
	      pContent = pChar ;
	      pBuf = pContent ;
	      actualContentLength = pEndBuf - pContent ;
	      /*gHyp_util_debug("Content/Actual = %d/%d",contentLength,actualContentLength);
	       *gHyp_util_debug("Remainder = %s",pEndBuf-(actualContentLength-contentLength));*/

	    }
	  }
	}
      }
      else {

	/* Not chunked transfer. If the content is text/html,
	 * then if we see a </html> or </HTML> at the end, we
	 * say we are done. Another way maybe is to see a CR
	 * or LF at the end, but this could be unpredictable.
	 */
	if ( contentLength == 0 ) {

	  if ( actualContentLength > 0 ) { 

	    if ( !isXMLEncoded ) {
	      pChar = pEndBuf - 1 ;
	      if ( *pChar == '\r' || *pChar == '\n' ) isEndOfMessage = TRUE ;
	    }
	    else if ( actualContentLength > 7 ) {
	      /* Look for ending tag. This really should be changed to
	       * compare the start tag with the end tag.
	       * ...</HTML>"
	       *    ^       
	       *    
	       */
	      pChar = pEndBuf - 7 ;
	      if ( strcmp ( pChar, "</HTML>" ) == 0 ||
		   strcmp ( pChar, "</html>" ) == 0 )
		 isEndOfMessage = TRUE ;

	    }
	  }
	}


      }

      /* How many bytes are left to process */ 
      n = MIN ( actualContentLength, pEndBuf-pContent ) ;

    }
    else {
      /* With standard port incoming data, we assume its
       * reply data only if the state of the socket is
       * query()
       */

      /* The entire buffer is the results */
      pBuf = buffer ;
      pContentData = gHyp_data_new ( "_data_" ) ;
      n = nBytes ;
    }

    isLineBased =  ( objectType == DATA_OBJECT_HTTP &&
		    ( isPlainText || isXMLEncoded ) ) ;

    /* Knit together sections for chunked transfer
       encoded data */
    doKnitting = FALSE ;
    pLast = NULL ;
    if ( isLineBased && pContentData ) {

      /* Get last value of pContent data */
      pLast = gHyp_data_getLast ( pContentData ) ;
      if ( pLast ) {
	lineLen = gHyp_data_getStr ( pLast, value, VALUE_SIZE, 0, TRUE ) ;
	value[lineLen] = '\0' ;
	/* So lone as the line does not end with \n\r, we can knit it to the
	 * start of the next line
	 */
	if ( lineLen > 1 ) {
	  if ( value[lineLen-1] != '\n' && value[lineLen-1] != '\r' ) {
	     doKnitting = TRUE ;
	  }
	}
      }
    }

    /* Add the content to the content data area */
    f = TRUE ;
    while ( n > 0 ) {

      /* We use INTERVAL_VALUE size to cover the case where each byte
       * happens to require a 4 character \nnnn to represent it.
       * To display exterally, the would require INTERNAL_VALUE_SIZE*4 bytes,
       * which is VALUE_SIZE bytes, the HyperScript internal maximum segment
       * size of a 'str'.
       */
      width = MIN ( n, INTERNAL_VALUE_SIZE ) ;

      if ( isLineBased ) {

	/* Ah, but, if the object is HTTP and its plain text or html/xml, then
	 * the data is often determined by line feeds.
	 */

	/* Skip past lf and cr */
        width2 = (int) strspn ( pBuf, "\r\n" ) ;
	if ( width2 > 0 ) {
	  pBuf += width2 ;
	  n -= width2 ;
	  width = MIN ( n, INTERNAL_VALUE_SIZE ) ;
	}

	/* Get the next line */
	width2 = strcspn ( pBuf, "\r\n" ) ;

	if ( width2 > 0 ) {
	  
	  if ( width2 < width ) {

	    /* Shorter is ok */
	    width = width2 ;
	  }
	  else if ( width2 > width ) { 

	    /* We'd like to store longer, but will it fit? */

	    width3 = width2 ;
	    for ( i=0;i<width2;i++ ) if ( !isprint(pBuf[i]) ) width3+=3;
	  
	    if ( width3 <= VALUE_SIZE ) 

	      /* Yes, it will fit - adjust size */
	      width = width2 ;

	  }
	}
      }

      if ( f && doKnitting &&  (width + lineLen <= VALUE_SIZE) ) {
	
	strncat ( value, pBuf, width ) ;
	lineLen+=width ;
	gHyp_data_setStr2 ( pLast, value, lineLen ) ;
	doKnitting = FALSE ;

      }
      else {

        pValue = gHyp_data_new ( NULL ) ;
        gHyp_data_setStr2 ( pValue, (char*) pBuf, width ) ;
	gHyp_data_append ( pContentData, pValue ) ;
      }
      f = FALSE ;
      n -= width ;
      pBuf += width ;
    }

    /* Determine if we are done and can create the message */
    if ( objectType == DATA_OBJECT_HTTP ) {

      if ( contentLength > actualContentLength ) {

	/* We haven't received all of the content. Wait for more. */
	gHyp_http_updateContent ( pHTTP, contentLength, actualContentLength ) ;
	return COND_SILENT ;
      }

      if ( contentLength < actualContentLength ) {

	/* We've got more data than expected. That's ok if we're
	 * receiving chunked data, because headers can follow
	 */

	if ( isChunkedTransferEncoded ) {

	  /* There may be more chunks. */
	      
	  /* Find the segment after the chunk just processed,
	   * the next chunk's size value should be there.
	   */
	  pBuf = pEndBuf - (actualContentLength-contentLength);
	  pChar = pBuf ;

	  /* Skip past any line breaks */
	  pChar += strspn ( pChar, "\r\n" ) ;
	  pLine = pChar ;

	  /* Get the width of the token prior to the next line break */
	  width = strcspn ( pLine, "\r\n" ) ;

	  if ( width == 0 || pChar == pEndBuf ) {

	    /* No new chunked content length values */
	    if ( !isChunkedTransferDone ) {

	      /* Chunk is done.  Reset for next chunk */
  	      actualContentLength = 0 ;
	      contentLength = 0 ;
	      gHyp_http_updateContent ( pHTTP, 
					contentLength, 
					actualContentLength ) ;

	      return COND_SILENT ;
	    }

	  }
	  else if ( width > 0 && width < VALUE_SIZE ) {

	    strncpy ( value, pLine, width ) ;
	    value[width]='\0' ;
	    pLine += width ;
	    pChar = strchr ( value, ';' ) ;
	    if ( pChar ) *pChar = '\0' ;
	    gHyp_util_trim ( value ) ;
	    sscanf ( value, "%x", &contentLength )  ;

	    if ( contentLength < 0 ) contentLength = 0 ;
	    if ( contentLength > 0 ) {

	      /* Got a content length. Get new start of content */
	      pChar = pBuf + width ;
	      pChar += strspn ( pChar, "\r\n" ) ;
	      pContent = pChar ;
	      pBuf = pContent ;
	      actualContentLength = pEndBuf - pContent ;
	      gHyp_http_updateContent ( pHTTP, 
					contentLength, 
					actualContentLength ) ;

	      return COND_SILENT ;
	    }
	    else {
	      /* Total end of chunked data.  Headers could follow. */
	      lHyp_secs1_httpHeaders ( pLine, pEndBuf, pAttributeData ) ;
	      isChunkedTransferDone = TRUE ;
	    }
	  }
	  else {
	    /* Not good - we will take the actual content length */
	    gHyp_util_logWarning("ContentLength too low (%d), should be %d",
	 		          contentLength, actualContentLength ) ;
	  }
	  contentLength = actualContentLength ;

	}
	else {
	  /* Not chunked */
	  if ( contentLength == 0 && !isEndOfMessage ) {
	    /* Could be more to follow */
	    actualContentLength = 0 ;
	    gHyp_http_updateContent ( pHTTP, 
				      contentLength,
				      actualContentLength ) ;

	    return COND_SILENT ;
	  }
	}
      }

      /* We may actually be done - */

      if ( isChunkedTransferEncoded && !isChunkedTransferDone ) {
	/*gHyp_util_debug("MESSAGE IS NOT COMPLETE");*/
	return COND_SILENT ;
      }
      /*gHyp_util_debug("MESSAGE content/actual len = %d/%d",contentLength,actualContentLength) ;*/

      if ( contentLength == 0 && actualContentLength == 0 ) return COND_SILENT ;

      if ( contentLength == 0 ) contentLength = actualContentLength ;

      isReply = gHyp_http_isReply ( pPort->pHTTP ) ;
      isMessageComplete = TRUE ;

      /* Create the top level container */
      pTV = gHyp_data_new ( "_data_" ) ;

      /* Create the attribute data */
      pData = gHyp_data_new ( "_http_attr_" ) ;
      if ( pAttributeData )
        gHyp_data_moveValues ( pData, pAttributeData ) ;

      gHyp_data_append ( pTV, pData ) ;

      if ( isReply ) {

  	pData = gHyp_data_new ( "_http_status_" ) ;
  	pValue = gHyp_data_new ( NULL ) ;
	gHyp_data_setStr ( pValue, gHyp_http_getArg2(pHTTP) ) ;
	gHyp_data_append ( pData, pValue ) ;
  	gHyp_data_append ( pTV, pData ) ;

	/* The status is interpreted as follows: 
	  1xx indicates an informational message only 
	  2xx indicates success of some kind 
	  3xx redirects the client to another URL 
	  4xx indicates an error on the client's part 
	  5xx indicates an error on the server's part 
	*/

  	pData = gHyp_data_new ( "_http_text_" ) ;
  	pValue = gHyp_data_new ( NULL ) ;
	gHyp_data_setStr ( pValue, gHyp_http_getArg3(pHTTP) ) ;
        gHyp_data_append ( pData, pValue ) ;
  	gHyp_data_append ( pTV, pData ) ;

  	pData = gHyp_data_new ( "_http_version_" ) ;
  	pValue = gHyp_data_new ( NULL ) ;
	gHyp_data_setStr ( pValue, gHyp_http_getArg1(pHTTP) ) ;
	gHyp_data_append ( pData, pValue ) ;
  	gHyp_data_append ( pTV, pData ) ;

      }
      else {

	/* The verb: GET, POST, etc */
  	pData = gHyp_data_new ( "_http_verb_" ) ;
	pValue = gHyp_data_new ( NULL ) ;
	pArg1 = gHyp_http_getArg1(pHTTP) ;
	isPOSTdata = ( strcmp ( pArg1, "POST" ) == 0 ) ;
	gHyp_data_setStr ( pValue, pArg1 ) ;
	gHyp_data_append ( pData, pValue ) ;
  	gHyp_data_append ( pTV, pData ) ;

	/* Check the target URL.  It could contain CGI args. */
	pTokenName = strchr ( gHyp_http_getArg2 ( pHTTP ), '?' ) ;
	if ( pTokenName ) {

	  /* Create arguments like "cgiargs" */
	  *pTokenName = '\0' ;
	  pTokenName++ ;
	  strcpy ( value,  pTokenName ) ;
	  pTokenName = value ;
	  while ( *pTokenName ) {

	    /* Look for '=' */
	    pTokenValue = strchr ( pTokenName, '=' ) ;
	    if ( !pTokenValue ) break ;

	    /* Make sure there is both a token and a value */
	    *pTokenValue = '\0' ;
	    gHyp_util_trim ( pTokenName ) ;
	    if ( strlen ( pTokenName ) == 0 ) break ;

	    /* Get the token value */
	    pTokenValue++ ;
		  
	    pChar = strchr ( pTokenValue, '&' ) ;
	    if ( pChar ) *pChar = '\0' ;
	    else pChar = pTokenValue + strlen ( pTokenValue ) ;

	    gHyp_util_trim ( pTokenValue ) ;

	    pVariable = gHyp_data_new ( pTokenName ) ;
 	    pValue = gHyp_data_new ( NULL ) ;
	    if ( strlen ( pTokenValue ) > 0 ) {
    	      gHyp_cgi_plusToSpace( pTokenValue );
	      gHyp_cgi_unescapeUrl( pTokenValue );
	      gHyp_data_setStr ( pValue, pTokenValue ) ;
	    }
	    gHyp_data_append ( pVariable, pValue ) ;
	    gHyp_data_append ( pTV, pVariable ) ;
	    pTokenName = pChar + 1 ;
	  }
	}

	pData = gHyp_data_new ( "_http_target_" ) ;
  	pValue = gHyp_data_new ( NULL ) ;
	gHyp_data_setStr ( pValue, gHyp_http_getArg2(pHTTP) ) ;
	gHyp_data_append ( pData, pValue ) ;
  	gHyp_data_append ( pTV, pData ) ;


  	pData = gHyp_data_new ( "_http_version_" ) ;
  	pValue = gHyp_data_new ( NULL ) ;
	gHyp_data_setStr ( pValue, gHyp_http_getArg3(pHTTP) ) ;
	gHyp_data_append ( pData, pValue ) ;
  	gHyp_data_append ( pTV, pData ) ;
      }

      /*
      if ( isPOSTdata ) {
	*gHyp_util_debug("POST");
      }
      */

      /* Parse the content */
      if ( contentLength > 0 && isXMLEncoded ) {
	gHyp_cgi_xmlData ( pContentData, pAI, gHyp_instance_frame(pAI), pTV ) ;
      }
      else if ( contentLength > 0 && isPOSTdata ) {
	
	/* Process the POST data */
	pVariable = NULL ;
	isVector = FALSE ;
	ss = -1 ; 
	context = -1 ;
	pData = NULL ;
	state = URL_EXPECT_TOKEN ;
	prefix[0] = '\0' ;
	while ( (pData = gHyp_data_nextValue ( pContentData, 
  					       pData,
					       &context,
					       ss ))) {
	  /* Copy any prefix */
	  value[0] = '\0' ;
	  strcpy ( value, prefix ) ;
	  prefix[0] = '\0' ;
	  pChar = value + strlen(value) ;
	  n = gHyp_data_getStr ( pData, 
				 pChar, 
				 VALUE_SIZE, 
				 context, 
				 isVector ) ;

	  pTokenName = value ;
	  while ( *pTokenName ) {

	    if ( state == URL_EXPECT_TOKEN ) {

	      /* Look for '=' */
	      pTokenValue = strchr ( pTokenName, '=' ) ;
	      if ( !pTokenValue ) break ;

	      /* Make sure there is both a token and a value */
	      *pTokenValue = '\0' ;
	      gHyp_util_trim ( pTokenName ) ;
	      if ( strlen ( pTokenName ) == 0 ) break ;
	      pVariable = gHyp_data_new ( pTokenName ) ;
	      gHyp_data_append ( pTV, pVariable ) ;

	      /* Get the token value */
	      pTokenValue++ ;
	      state = URL_EXPECT_VALUE ;
		  
	    }
	    else
	      pTokenValue = value ;

	    /* Look for end of value section */
	    pChar = strchr ( pTokenValue, '&' ) ;
	    if ( pChar ) {
	      /* Mark end of value */
	      *pChar++ = '\0' ;
	      state = URL_EXPECT_TOKEN ;
	      pTokenName = pChar ; 
	    }
	    else {

	      /* The value continues... 
	       * Its length is at least to the end of the value 
	       */
	      width = strlen ( pTokenValue ) ;

	      /* Where is the last occurrence of '%' escape char? */
	      pChar = strrchr ( pTokenValue, '%' ) ;

	      /* If it ends in the last or second to last position,
	       * we need to snip it out and begin it in the next
	       * line.
	       */
	      if ( pChar > ( pTokenValue + width - 3 ) ) {
		strcpy ( prefix, pChar ) ;
		*pChar++ = '\0' ;
		pTokenName = pChar ;
	      }
	      else
		pTokenName = pTokenValue+width ;
	    }

 	    pValue = gHyp_data_new ( NULL ) ;
	    if ( strlen ( pTokenValue ) > 0 ) {
	      if ( isURLEncoded ) {
		gHyp_cgi_plusToSpace( pTokenValue );
		gHyp_cgi_unescapeUrl( pTokenValue );
		gHyp_data_setStr ( pValue, pTokenValue ) ;
	      }
	    }
	    if ( pVariable ) gHyp_data_append ( pVariable, pValue ) ;
	  }
	}
      }
      
      /* Always deliver the raw content */
      pData = gHyp_data_new ( "_http_data_" ) ;
      if ( pContentData )
        gHyp_data_moveValues ( pData, pContentData ) ;
      gHyp_data_append ( pTV, pData ) ;
	  
      /* Reset the http data area */
      gHyp_http_reset ( pHTTP ) ;

    }
    else {
      /* Raw port data. Every message is a complete message. */
      pTV = gHyp_data_new ( "_data_" ) ;
      gHyp_data_append ( pTV, pContentData ) ;
      isMessageComplete = TRUE ;
      isReply = ( pPort->state == MESSAGE_QUERY ) ;
    }

    if ( isMessageComplete ) {

      /* STATUS just gets the first line. Cool */
      pSTATUS = gHyp_data_new ( "STATUS" ) ;
      pData = gHyp_data_getFirst ( pContentData ) ;
      if ( pData ) {
	pValue = gHyp_data_copy ( pData ) ;
      }
      else {
	pValue = gHyp_data_new ( NULL ) ;
	if ( objectType == DATA_OBJECT_PORT )
          gHyp_data_setStr ( pValue, STATUS_NULL ) ;
	else {
	  if ( isReply ) 
    	    gHyp_data_setStr ( pValue, gHyp_http_getArg2(pHTTP) ) ;
	  else
    	    gHyp_data_setStr ( pValue, STATUS_ACKNOWLEDGE ) ;
	}
      }
      gHyp_data_append ( pSTATUS, pValue ) ;

      gHyp_data_append ( pTV, pSTATUS ) ;

      if ( isReply ) {

	strcpy ( mode, "reply" ) ;
	if ( objectType == DATA_OBJECT_PORT )
	  sprintf ( sender, "%u#port", id ) ;
	else
	  sprintf ( sender, "%u#http", id ) ;
	strcpy ( target, gHyp_instance_getTargetId ( pAI ) ) ;

	/* Set the method of the message */
	strcpy ( method, "DATA" ) ;

      }
      else {

        if ( objectType == DATA_OBJECT_HTTP ) {
 	  strcpy ( mode, "event" ) ;
  	  sprintf ( sender, "%u#http", id ) ;
	}
	else {
 	  strcpy ( mode, "event" ) ;
          sprintf ( sender, "%u#port", id ) ;
	}

	strcpy ( target, gHyp_instance_getTargetPath ( pAI ) ) ;

	/* What method does this function invoke? */
	pVariable = gHyp_instance_portMethod ( pAI, id ) ;
	if ( !pVariable ) {
	  gHyp_util_logError (
	   "No method enabled for incoming message from device %d", id ) ;
	  gHyp_data_delete ( pTV ) ;

          gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
	  gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pPort->fd ) ;
	  return COND_SILENT ;
	}
 
	/* Set the method of the message */
	strcpy ( method, gHyp_data_getLabel ( pVariable ) ) ;

      }
    
      /*gHyp_util_debug("Received %d/%s = %s",id,mode,gHyp_data_print(pTV));*/
      pMsg = gHyp_aimsg_initUnparsePartial ( 
		gHyp_instance_incomingMsg ( pAI ),
		target,
		mode,
		method,
		sender,
		"00000001",
		gHyp_util_timeStamp ( gsCurTime ),
		pTV ) ;

      if ( pMsg ) return strlen ( pMsg ) ;
    }
    return COND_SILENT ;
  }
}

int gHyp_secs1_rawOutgoing ( sSecs1 *pPort, sInstance *pAI, sData *pData, int id )
{
  /* Three return values:
   * > 0  = message written 
   * = 0  = no message written, non-fatel case
   * < 0  = fatal error.
   *
   * Called from gHyp_port_event, gHyp_port_query.
   * A return value of 0 means to resend the message.
   */
  sLOGICAL
    isVector ;
  
  sData
    *pResult ;

  int
    count,
    ss,
    cmdLen,
    remainder,
    context,
    valueLen,
    nBytes=0 ;

  char
    command[PORT_READ_SIZE+1],
    *pCmd,
    *pEndCmd,
    *pStr,
    value[VALUE_SIZE+1] ;

  /* Construct the message to be sent to the port. */
  pCmd = command ;
  pEndCmd = pCmd + PORT_READ_SIZE ;    
  pResult = NULL ;

  isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;
  ss = gHyp_data_getSubScript ( pData ) ; 
  context = -1 ;
  count = 0 ;
  while ( (pResult = gHyp_data_nextValue ( pData, 
					   pResult,
					   &context,
					   ss ))) {
    valueLen = gHyp_data_getStr ( pResult, 
				  value, 
				  VALUE_SIZE, 
				  context, 
				  isVector ) ;
    pStr = value ;
    
    if ( (pCmd + valueLen) > pEndCmd ) {

      /* Write out what's there and start another buffer */
      *pCmd = '\0' ;	
      cmdLen = pCmd - command ;
      /*gHyp_util_debug("%s",command);*/

      nBytes = gHyp_sock_write ( (SOCKET) pPort->fd, 
			         command, 
			         cmdLen,
			         (short)(pPort->flags&MASK_SOCKET),
			         (OVERLAPPED*) &pPort->overlapped,
			         pPort->pSSL ) ;

      if ( nBytes < 0 ) {
        gHyp_util_sysError ( "Failed to write data to device %s",
			 pPort->device ) ;
        gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
        gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), (SOCKET) pPort->fd ) ;
        return COND_SILENT ;
      }
      else {

	/* Some or all bytes were written */

	/* Reset start of buffer */
	pCmd = command ;
	pEndCmd = pCmd + PORT_READ_SIZE ;

	if ( nBytes < cmdLen ) {

	  /* Not all was written, add in remainder */
	  remainder = cmdLen - nBytes ;
	  memcpy ( pCmd, pCmd+nBytes, remainder ) ;
	  pCmd+=remainder ;
	  gHyp_util_logInfo("Network pipe full, sleeping for 100 usec");
	  count = 0 ;
	  usleep ( 100 );
    	  if ( (pCmd + valueLen) > pEndCmd ) {
            gHyp_util_logError ( "Failed to write to device %s",
    			 pPort->device ) ;
	    gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
	    gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), (SOCKET) pPort->fd ) ;
	    return COND_SILENT ;
	  }
	}
	else
	  count++ ;

	if ( count > 10 ) { 
	  gHyp_util_logInfo(("Network pipe full, sleeping for 100 usec"));
	  count = 0 ;
	  usleep ( 100 );
	}
      }
    } 
    memcpy ( pCmd, pStr, valueLen ) ;
    pCmd += valueLen ;   

  }
  if ( context== -2 && ss != -1 ) 
    gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			  "Subscript '%d' is out of bounds",ss) ;
  *pCmd = '\0' ;	
  cmdLen = pCmd - command ;

  if ( cmdLen > 0 ) {
    /*gHyp_util_debug("Writing port %d, %d bytes",pPort->fd, cmdLen);*/
    /*gHyp_util_debug("%s",command);*/

    nBytes = gHyp_sock_write ( (SOCKET) pPort->fd, 
			     command, 
			     cmdLen,
			     (short)(pPort->flags&MASK_SOCKET),
			     (OVERLAPPED*) &pPort->overlapped,
			     pPort->pSSL ) ;

    if ( nBytes < 0 ) {
      gHyp_util_sysError ( "Failed to write data to device %s",
			 pPort->device ) ;
      gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
      gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), (SOCKET) pPort->fd ) ;
      return COND_SILENT ;
    }
    else if ( nBytes == 0 ) {
    
      /* Should not happen */
      gHyp_util_logError ( "Zero bytes written to device %s",
			 pPort->device ) ;
      gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
      gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), (SOCKET) pPort->fd ) ;
      return COND_SILENT ;
    }
  }
  return nBytes ;
}


SOCKET gHyp_secs1_parentSocket ( sSecs1 *pSecs1 )
{
  return pSecs1->parentSocket ;
}

sInstance* gHyp_secs1_parentAI ( sSecs1 *pSecs1 )
{
  return pSecs1->parentAI ;
}

SOCKET gHyp_secs1_fd ( sSecs1 *pSecs1 )
{
  return pSecs1->fd ;
}

sBYTE*	gHyp_secs1_buf ( sSecs1* pSecs1 ) 
{
  return pSecs1->inbuf ;
}

int *gHyp_secs1_pNbytes ( sSecs1 *pSecs1 )
{
  return &pSecs1->nBytes ;
}

short gHyp_secs1_flags ( sSecs1 *pSecs1 )
{
  return pSecs1->flags ;
}

void gHyp_secs1_setFlags ( sSecs1 *pSecs1, short flags )
{
  pSecs1->flags = flags ;
}

void gHyp_secs1_setState ( sSecs1 *pSecs1, sBYTE state )
{
  pSecs1->state = state ;
}

LPOVERLAPPED gHyp_secs1_overlapped ( sSecs1 *pSecs1 )
{
  return &pSecs1->overlapped ;
}

void gHyp_secs1_setSSL ( sSecs1 *pSecs1, sSSL *pSSL )
{
  pSecs1->pSSL = pSSL ;
}

sSSL *gHyp_secs1_getSSL ( sSecs1 *pSecs1 )
{
  return pSecs1->pSSL ;
}

sData *gHyp_secs1_getForwardPorts ( sSecs1 *pSecs1 ) 
{
  return pSecs1->forwardPorts ;
}

void gHyp_secs1_addForwardPort ( sSecs1 *pSecs1, int from_id, int to_id ) 
{
  sData
    *pValue,
    *pData ;

  sLOGICAL
    found ;

  int
    context,
    ss,
    id_from,
    id_to ;

  if ( !pSecs1->forwardPorts ) {
    pSecs1->forwardPorts = gHyp_data_new ( "_forward_ports_" ) ;
  }

  pValue = NULL ;
  ss = -1 ; 
  context = -1 ;
  pData = pSecs1->forwardPorts ;
  found = FALSE ;
  while ( !found &&
	  (pValue = gHyp_data_nextValue ( pData, 
					  pValue, 
					  &context,
					  ss ) ) ) {
    id_from = gHyp_data_getInt ( pValue, 0, TRUE ) ;
    id_to = gHyp_data_getInt ( pValue, 1, TRUE ) ;
    if ( id_from == from_id && id_to == to_id ) found = TRUE ;
  }

  if ( !found ) {
    /*gHyp_util_debug("Adding %d->%d for %d",from_id,to_id,pSecs1->fd ) ;*/ 
    pValue = gHyp_data_new ( NULL ) ;
    gHyp_data_newVector ( pValue, TYPE_USHORT, 2, TRUE ) ;
    gHyp_data_setVectorRaw ( pValue, &from_id, 0 ) ;
    gHyp_data_setVectorRaw ( pValue, &to_id, 1 ) ;
    gHyp_data_append ( pData, pValue ) ;
  }

}

sData* gHyp_secs1_copyForwardPort ( sSecs1 *pSecs1, int from_id ) 
{
  sData
    *pFirst,
    *pNext,
    *pData,
    *pValue,
    *pForwardPorts ;

  sLOGICAL
    f ;

  int 
    id_from,
    id_to ;

  if ( !pSecs1->forwardPorts ) return NULL ;

  pForwardPorts = NULL ;

  for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pSecs1->forwardPorts ) ;
	(f && pData) || (pFirst && pData != pFirst );
	f=FALSE, pData = pNext, pFirst = gHyp_data_getFirst ( pSecs1->forwardPorts ) ) {
        
    pNext = gHyp_data_getNext ( pData ) ;

    id_from = gHyp_data_getInt ( pData, 0, TRUE ) ;
    id_to = gHyp_data_getInt ( pData, 1, TRUE ) ;
    if ( id_from == from_id ) {

      /*gHyp_util_debug("Taking %d->%d",id_from,id_to ) ;*/ 
      /*gHyp_data_detach ( pData ) ;*/
      pValue = gHyp_data_copy ( pData ) ;
      pForwardPorts = gHyp_data_new ( "_forward_ports_" ) ;
      gHyp_data_append( pForwardPorts, pValue ) ;
      break ;
    }
  }

  /*
  if ( gHyp_data_getCount ( pSecs1->forwardPorts ) == 0 ) {
    gHyp_data_delete ( pSecs1->forwardPorts ) ;
    pSecs1->forwardPorts = NULL ;
  }
  */

  return pForwardPorts ;
}

void gHyp_secs1_removeForwardPort ( sSecs1 *pSecs1, int from_id, int to_id ) 
{
  sData
    *pFirst,
    *pNext,
    *pData ;

  sLOGICAL
    f ;

  int 
    id_to,
    id_from ;

  if ( !pSecs1->forwardPorts ) return ;

  for ( f=TRUE, pData = pFirst = gHyp_data_getFirst ( pSecs1->forwardPorts ) ;
	(f && pData) || (pFirst && pData != pFirst );
	f=FALSE, pData = pNext, pFirst = gHyp_data_getFirst ( pSecs1->forwardPorts ) ) {
        
    pNext = gHyp_data_getNext ( pData ) ;

    id_from = gHyp_data_getInt ( pData, 0, TRUE ) ;
    id_to = gHyp_data_getInt ( pData, 1, TRUE ) ;
    if ( id_from == from_id && id_to == to_id ) {

      gHyp_data_detach ( pData ) ;
      gHyp_data_delete ( pData ) ;
    }
  }

  if ( gHyp_data_getCount ( pSecs1->forwardPorts ) == 0 ) {
    gHyp_data_delete ( pSecs1->forwardPorts ) ;
    pSecs1->forwardPorts = NULL ;
  }
}
