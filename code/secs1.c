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
 *   Revision 1.81  2009-09-28 05:25:09  bergsma
 *   ENQ Contention issues
 *
 *   Revision 1.80  2009-09-21 05:22:52  bergsma
 *   Better handling of ENQ contention
 *
 *   Revision 1.79  2009-08-11 21:19:07  bergsma
 *   Fixed for ANSI C compatibility - some function names were too long....
 *
 *   Revision 1.78  2009-08-04 00:11:06  bergsma
 *   Fix parsing of POST URL-encoded data.
 *
 *   Revision 1.77  2009-07-23 16:16:17  bergsma
 *   Fixed infinite loop caused by truncated HTTP messages.
 *
 *   Revision 1.76  2009-06-12 05:03:02  bergsma
 *   HS 385 Final Checkin and TAG - Append message bug
 *
 *   Revision 1.75  2009-04-09 19:57:21  bergsma
 *   Fixing issues with HTTP and buffer sizing
 *
 *   Revision 1.74  2009-04-07 19:49:57  bergsma
 *   Fix problem when HTTP messages that have incomplete headers
 *   are found at the end of the incoming buffer.
 *
 *   Revision 1.73  2009-04-02 06:35:13  bergsma
 *   Port reads and writes are 4K, Http reads and wites are 5K,
 *
 *   Revision 1.72  2009-03-22 19:21:39  bergsma
 *   no message
 *
 *   Revision 1.71  2009-03-06 18:00:24  bergsma
 *   Comment change
 *
 *   Revision 1.70  2009-03-02 00:34:35  bergsma
 *   Improved handling of zero-length content buffers
 *
 *   Revision 1.69  2009-01-19 23:24:36  bergsma
 *   PFP BUG.  Appended HTTP messages and detection of binary data instead of POST data
 *
 *   Revision 1.68  2009-01-02 17:56:51  bergsma
 *   no message
 *
 *   Revision 1.67  2008-12-30 18:35:39  bergsma
 *   Correct for content beginning with \r\n
 *
 *   Revision 1.66  2008-06-11 18:49:49  bergsma
 *   typo
 *
 *   Revision 1.65  2008-05-06 02:14:58  bergsma
 *   HTTP arguments past first one were not being parsed.
 *   Fixed other occurence of fast ENQ.
 *
 *   Revision 1.64  2008-02-12 23:43:39  bergsma
 *   VS 2008 update
 *
 *   Revision 1.63  2007-10-27 01:56:13  bergsma
 *   Bad % formating in secs S9 messages
 *
 *   Revision 1.62  2007-09-25 17:50:19  bergsma
 *   Integrate SOCK_UNIX_LISTEN with SOCK_LISTEN
 *
 *   Revision 1.61  2007-09-03 06:22:46  bergsma
 *   Knitting bug fixed
 *
 *   Revision 1.60  2007-07-22 02:59:06  bergsma
 *   Add secs1_setHttp
 *
 *   Revision 1.59  2007-06-20 05:18:57  bergsma
 *   When debugging forward functionality, print the entire buffer.
 *
 *   Revision 1.58  2007-05-09 00:43:19  bergsma
 *   Problem with secs1_rawoutgoing - segmentation fault when pData str
 *   is longer than PORT_WRITE_SIZE
 *
 *   Revision 1.57  2007-05-08 01:25:24  bergsma
 *   Fix bug when fetching CGI args.
 *
 *   Revision 1.56  2007-05-02 20:34:01  bergsma
 *   Fix parseurl function.  Improve various print/debug/log statements.
 *   Fix problem with chunked data transfers.
 *
 *   Revision 1.55  2007-04-23 17:15:00  bergsma
 *   Typo
 *
 *   Revision 1.54  2007-04-15 05:09:43  bergsma
 *   Problems when getting chunked data.
 *
 *   Revision 1.53  2007-02-26 02:35:01  bergsma
 *   No longer user NULL_DEVICEID placeholder.  PORT and HTTP
 *   autoallocate device ids, HSMS and SECS I are pre-determined.
 *
 *   Revision 1.52  2007-02-15 03:25:40  bergsma
 *   Removed unecessary logInfo statement.
 *
 *   Revision 1.51  2007-02-13 22:35:13  bergsma
 *   Initialize contentLength=0
 *
 *   Revision 1.50  2006-11-25 03:09:46  bergsma
 *   Incease size of buffer for getStr to accomodate larger strings.
 *
 *   Revision 1.49  2006/10/12 00:32:49  bergsma
 *   Corrected ambiguous debug statement.
 *
 *   Revision 1.48  2006/10/11 16:16:20  bergsma
 *   Make EAGAIN an optional feature that must be turned on.
 *
 *   Revision 1.47  2006/10/02 06:22:32  bergsma
 *   For egain to work properly with SSL, the data that's already been encypted
 *   needs to be saved in a special buffer, pEagainPartial, so that it may be
 *   sent again without having to go through the SSL engine.
 *
 *   Revision 1.46  2006/10/01 16:26:43  bergsma
 *   Support for EAGAIN processing.
 *
 *   Revision 1.45  2006/09/25 05:06:30  bergsma
 *   Cosmetic
 *
 *   Revision 1.44  2006/08/28 21:04:08  bergsma
 *   Removed comment
 *
 *   Revision 1.43  2006/08/22 21:45:13  bergsma
 *   Better PROTOCOL debug message for port traffic.
 *
 *   Revision 1.42  2006/08/22 18:47:13  bergsma
 *   Changed info message to "setting parent socket to %d"
 *
 *   Revision 1.41  2006/01/23 05:12:02  bergsma
 *   Added port_binary() function.
 *
 *   Revision 1.40  2006/01/19 20:30:44  bergsma
 *   Debug statements
 *
 *   Revision 1.39  2006/01/17 19:02:32  bergsma
 *   More debug statements.
 *
 *   Revision 1.38  2006/01/17 16:02:35  bergsma
 *   More debug statements
 *
 *   Revision 1.37  2006/01/16 18:56:36  bergsma
 *   HS 3.6.6
 *   1. Save query timeout events.  Don't let queries repeat indefinitely.
 *   2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 *   Revision 1.36  2006/01/14 20:42:23  bergsma
 *   Imrproved HTTP data buffer knitting.  Only when line does not end with
 *   \n or \n\r
 *
 *   Revision 1.35  2006/01/10 02:05:36  bergsma
 *   Some additional protection against a bad HTTP header.
 *
 *   Revision 1.34  2006/01/06 23:40:39  bergsma
 *   *** empty log message ***
 *
 *   Revision 1.33  2006/01/05 17:37:27  bergsma
 *   *** empty log message ***
 *
 *   Revision 1.32  2005/12/23 15:22:00  bergsma
 *   Change util_debug to util_logInfo for some SSL actions
 *
 *   Revision 1.31  2005/12/21 05:39:04  bergsma
 *   Knitting bug when last line is only 1 character.
 *
 *   Revision 1.30  2005/12/10 00:30:30  bergsma
 *   HS 3.6.5
 *
 *   Revision 1.29  2005/11/29 15:23:31  bergsma
 *   Detect XML data in POST contents.
 *
 *   Revision 1.28  2005/11/28 05:10:19  bergsma
 *   Better error messages and detection of null sockets and device ids
 *
 *   Revision 1.27  2005/11/23 16:57:45  bergsma
 *   no message
 *
 *   Revision 1.26  2005/11/23 15:53:33  bergsma
 *   In secs1_rawIncoming, pAIassigned was not being used for signal hangups or
 *   for the incoming msg target.  The second was also not consistent for
 *   #port and #http data streams.
 *
 *   Revision 1.25  2005/10/25 16:40:21  bergsma
 *   Correct debug statement for forward-ports
 *
 *   Revision 1.24  2005/10/15 21:43:30  bergsma
 *   Debug mode for port forwarding
 *
 *   Revision 1.23  2005/09/23 17:11:17  bergsma
 *   no message
 *
 *   Revision 1.22  2005/09/08 12:54:54  bergsma
 *   WHen port message comes in, use pAIassigned to find target
 *
 *   Revision 1.21  2005/08/17 01:48:18  bergsma
 *   handle problem with toexternal ( string, "|" )
 *
 *   Revision 1.20  2005/08/03 14:00:20  bergsma
 *   no message
 *
 *   Revision 1.19  2005/03/16 23:53:21  bergsma
 *   V 3.5.1 - fixes for use with DECC compiler.
 *
 *   Revision 1.18  2005/02/15 07:08:47  bergsma
 *   Add gHyp_secs1_setForwardPorts().
 *   Make sure the port state is set to MESSAGE_EVENT after being temporarily
 *   set to MESSAGE_REPLY.
 *
 *   Revision 1.17  2005/01/31 06:01:15  bergsma
 *   Add variable _datalen_ to go along with _data_
 *
 *   Revision 1.16  2005/01/25 05:50:28  bergsma
 *   Moved test for nBytes==0 to sock_read
 *
 *   Revision 1.15  2005/01/10 20:21:48  bergsma
 *   Support port_event/port_query when data is a byte array.
 *
 *   Revision 1.14  2005/01/02 01:47:20  bergsma
 *   Port messages must use TRANSACTION ID 00000001
 *
 *   Revision 1.13  2004/11/19 03:51:01  bergsma
 *   Initialize TID on port message to random8 instead of 0000001
 *
 *   Revision 1.12  2004/10/16 05:02:48  bergsma
 *   Bug fixes and added support for HTTP protocol
 *
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

/* Let's keep the I/O buffers permanently allocated.  That's going to make the
 * OS happier, plus we can handle guSIGMSG with it here
 */

static char gzOutBuffer[MAX_INPUT_LENGTH+1] ;
static char gzInputBuffer[OVERFLOW_READ_SIZE+1] ;
static int  giNbytes ;

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
  sBYTE			savebuf[HTTP_READ_SIZE+1];
  int			savebuflen ;
  sLOGICAL		doEagain ;	  /* If eagain processing is enabled - default = no */
  sData*		pEagain ;	  /* Write pending data */  
  sData*		pEagainPartial ;  /* Write partial data */  
  int			nextEagainTime1 ; /* utc until next try */ 
  int			nextEagainTime2 ; /* Plus so many millseconds */
  int			nextEagainInc ;   /* Milliseconds to wait before trying again */
  int			nBytes ;
  OVERLAPPED		overlapped ;
  sBYTE			state ;
  sSSL*			pSSL ;
  sHTTP*		pHTTP ;
  sData*		forwardPorts ;
  sLOGICAL isBinary ;


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
  if ( flags & (SOCKET_LISTEN | SOCKET_UNIX_LISTEN) ) {
    /*gHyp_util_logInfo("Setting parent listent socket to %d",parentSocket ) ;*/
    assert ( parentSocket != INVALID_SOCKET ) ;
  }
  pSecs1->parentSocket = parentSocket ;
  pSecs1->parentAI = parentAI ;

  strcpy ( pSecs1->device, device ) ;

  memset ( &pSecs1->overlapped, 0, sizeof (OVERLAPPED) ) ;
#ifdef AS_WINDOWS
  pSecs1->overlapped.hEvent = (HANDLE) gHyp_sock_createEvent () ;
  pSecs1->overlapped.Internal = STATUS_READY ;
#endif

  pSecs1->pSSL = NULL ;
  pSecs1->doEagain = FALSE ;
  pSecs1->pEagain = NULL ;
  pSecs1->pEagainPartial = NULL ;
  pSecs1->nextEagainTime1 = 0 ; ;
  pSecs1->nextEagainTime2 = 0 ; ;
  pSecs1->nextEagainInc = 0 ;
  pSecs1->savebuflen = 0 ;

  if ( flags & PROTOCOL_HTTP ) 
    pSecs1->pHTTP = gHyp_http_new() ;
  else
    pSecs1->pHTTP = NULL ; 

  pSecs1->forwardPorts = NULL ;
  pSecs1->isBinary = FALSE ;

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
  if ( pSecs1->pEagain )  gHyp_data_delete  ( pSecs1->pEagain ) ;
  if ( pSecs1->pEagainPartial )  gHyp_data_delete  ( pSecs1->pEagainPartial ) ;
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
   gHyp_util_logInfo("Opened %s on fd %d", pDevice, fd ) ;

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


sInstance *gHyp_secs1_assign ( sSecs1 *pSecs1, sConcept *pConcept, sWORD id ) 
{ 
  /* The purpose of this function is the call gHyp_instance_updateFd
   * on a id/socket combination.  
   */

  sSecs1
    *pListenSecs1 ;

  sInstance
    *pAIassigned ;

  SOCKET
    fd ;

  if ( id == NULL_DEVICEID ) return NULL ;

  /* First find the instance that is assigned to the id */
  pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, id ) ;

  if ( !pAIassigned ) {

    gHyp_util_logWarning( "No instance has assigned id %d to the socket %d",
      id, pSecs1->fd );
    return NULL ;

  }

  /* Get the port assigned to the id.
   * If it is the parent port, it is a new connection 
   * and we can setup a connect handler 
   */

  fd = gHyp_instance_getDeviceFd ( pAIassigned, id ) ;

  if ( fd == pSecs1->parentSocket ) {

    if ( pSecs1->fd != pSecs1->parentSocket ) {

      /* This is a newly created channel */

      /* This is a newly created channel */
      gHyp_util_logInfo ( "Assigning device id %d to port %d from listen socket %d",
	 		    id, pSecs1->fd, pSecs1->parentSocket );

      /* Take any SSL structures as well */
      pListenSecs1 = (sSecs1*) gHyp_concept_getSocketObject ( pConcept, 
							  (SOCKET) pSecs1->parentSocket, 
							  DATA_OBJECT_NULL ) ;
							  /*DATA_OBJECT_SECS ) ;*/
#ifdef AS_SSL
      if ( pListenSecs1->pSSL != NULL ) {
	pSecs1->pSSL = gHyp_sock_copySSL ( pListenSecs1->pSSL ) ;
      }
#endif

      /* Take this id */
      gHyp_instance_updateFd ( pAIassigned, pSecs1->fd, id, NULL, FALSE ) ;

      gHyp_instance_signalConnect ( pAIassigned, pSecs1->fd, pSecs1->modifier, id ) ;

      pSecs1->deviceId = id ;
    }
   
  }
  return pAIassigned ;

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
   *    - called from gHyp_sock_select<-gHyp_instance_read
   *    - a return value of -1 is required to trigger handlers
   *	- a return value of 0 re-invokes sock_select to read again
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

  sInstance
    *pAIassigned = NULL ;

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
	gHyp_util_logError ( "SECS1 RECV ENQ jump level overflow at %d", MAX_JMP_LEVEL ) ;
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
 
      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
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

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "->EOT" ) ; 

      if ( nBytes <= 0 ) {
	gHyp_util_sysError ( "Failed to write <EOT> to SECS device %s",
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

	  /* We need to ignore block sizes that are not possible.
	   * Or, we need to recognize that they are not block sizes
	   * at all but control characters 
	   */

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
	      if ( guDebugFlags & DEBUG_PROTOCOL )
		gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
				     "<-ENQ(BLK)", blkStr ) ;
	      if ( nBytes > 1 )
		for (i=0; i<nBytes; i++) pSecs1->inbuf[i] = pSecs1->inbuf[i+1];
	      nBytes-- ;

	      if ( nBytes == 0 && !gHyp_instance_isRbitSet ( pAI, id )) {
		/* There was just the ENQ and we are a slave.  
		 * Forget the block we were waiting for.  Send back an EOT 
		 */
		pSecs1->state = SECS_EXPECT_SEND_EOT ;
	        break ;
	      }
	      /* Otherwise, keep processing the characters in the buffer */
	      blkSize = pSecs1->inbuf[0] ;
	    }
	    else if ( pSecs1->inbuf[0] == SECS_CHAR_EOT ) {
	      
	      /* Remove the EOT, shift all bytes down */
	      if ( guDebugFlags & DEBUG_PROTOCOL )
		gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
				     "<-EOT(2)", blkStr ) ;
	      if ( nBytes > 1 )
		for (i=0; i<nBytes; i++) pSecs1->inbuf[i] = pSecs1->inbuf[i+1];
	      nBytes-- ;

	      if ( nBytes == 0 && !gHyp_instance_isRbitSet ( pAI, id ) ) {
		/* There was just the EOT, as if we were supposed to now send
		 * a block.  We are a slave, but we don't have a block to send 
		 * but we can try to reset the protocol by sending a NAK.
		 * So, then forget about the block we were waiting for. 
		 */
		pSecs1->state = SECS_EXPECT_SEND_NAK ;
		break ;
	      }
	      
	      /* Otherwise, keep processing the characters in the buffer,
	       * perhaps we will get a good block of data and can proceed
	       */
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

	if ( guDebugFlags & DEBUG_PROTOCOL ) {
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			       "<-%d received (%d/%d bytes)",
			       nBytes,
			       ix+nBytes,
			       blkSize+3 ) ;
	  pStr = blkStr ;
	  for ( i=ix; i<ix+nBytes; i++ ) {
	    sprintf ( pStr, "%02x ", pSecs1->inbuf[i] ) ;
	    pStr += 3 ;
	  }
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
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
	    gHyp_util_logError ( "SECS-I checksum error, should be 0x%x, sending <NAK>",
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
	if ( guDebugFlags & DEBUG_PROTOCOL )
 	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
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

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "->NAK" ) ;

      if ( nBytes <= 0 ) {
	gHyp_util_sysError ( "Failed to write <NAK> to SECS device %s",
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

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "->ACK" ) ;

      if ( nBytes <= 0 ) {
	gHyp_util_sysError ( "Failed to write <ACK> to SECS device %s",
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

      /* Check to see if we are enabled */
      pAIassigned = gHyp_secs1_assign ( pSecs1, pConcept, header.deviceId ) ;
      if ( pAIassigned ) pAI = pAIassigned ;

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
	gHyp_instance_error ( pAI, STATUS_SECS,
				"SECS II device id %d is not assigned to socket %d", 
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
          gHyp_util_logWarning (
            "SECS-I, first block number not 0 or 1" ) ;
	  pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	  ix = 0 ;
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
	gHyp_util_logWarning("S9F1 not sent to equipment");
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
	gHyp_util_logWarning("S9F3 not sent to equipment");
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
	gHyp_util_logWarning("S9F5 not sent to equipment");
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
	gHyp_util_logWarning("S9F7 not sent to equipment");
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
	gHyp_util_logWarning("S9F9 not sent to equipment");
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
	gHyp_util_logWarning("S9F11 not sent to equipment");
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
	gHyp_util_logWarning("S9F13 not sent to equipment");
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
   * = 0  = no message written, non-fatal case
   * < 0  = fatal error.
   *
   * Called from gHyp_secs_event, gHyp_secs_query, or 
   * gHyp_instance_replyMessage.  A return value of 0 means to resend the
   * message.
   */
  sLOGICAL
    gotENQ,
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

  /* For debugging ENQ contention
   *if ( stream == 6 && function == 5 && mode == MESSAGE_EVENT ) mode = MESSAGE_QUERY ;
   */

  pHeader->isReplyExpected = ( mode == MESSAGE_QUERY ) ;

  pHeader->blockNum = 1 ;
  pHeader->isFirstBlock = TRUE ;
  pHeader->isLastBlock = gHyp_secs2_isEndBlock ( pSecs2, 1, SECS_OUTGOING ) ;
  pHeader->isPrimaryMsg = ( function % 2 ) ;

  /* Restore system bytes */
  if ( guDebugFlags & DEBUG_PROTOCOL )
    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
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
 
  if ( guDebugFlags & DEBUG_PROTOCOL )
    gHyp_util_logDebug ( 
      FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
      "Sending S%dF%d (device %d) (TID=0x%02x) (SID=0x%02x)",
      stream,function,id,TID,SID ) ;

  while ( !lastBlockSent ) {

    /* Process the state and set the next state */    
    switch ( pSecs1->state ) {
      
    case SECS_EXPECT_SEND_ENQ :

      /* Send <ENQ>.  Then wait for <EOT> */
      pSecs1->outbuf[0] = SECS_CHAR_ENQ ;
      nBytes = gHyp_sock_write ( (SOCKET) pSecs1->fd, 
				 (char*) pSecs1->outbuf,
				 1,
				 (short)(pSecs1->flags&MASK_SOCKET),
			         (OVERLAPPED*) &pSecs1->overlapped,
				 pSecs1->pSSL) ;

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "->ENQ" ) ;

      if ( nBytes <= 0 ) {
	gHyp_util_sysError ( "Failed to write <ENQ> to SECS device %s",
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

	  /* Not what we expected */

	  if ( pSecs1->inbuf[0] == SECS_CHAR_ENQ ) {
	    
	    if ( guDebugFlags & DEBUG_PROTOCOL )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
				   "<-ENQ(EOT)" ) ;

	    /* ENQ Contention.  Relinquish if device is not a slave, 
	     * that is, not the host.
	    */
	    if ( !pHeader->rBit ) {
	      
	      /* We are the slave and we are getting an interrupt */
	      if ( guDebugFlags & DEBUG_PROTOCOL )
		gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
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
	    if ( guDebugFlags & DEBUG_PROTOCOL )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
				   "<-NAK", pSecs1->inbuf[0] ) ;
	    /* If we are a slave, exit with an error. */
	    if ( !pHeader->rBit ) {
	      pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	      return COND_ERROR ;
            }
	    /* Otherwise keep waiting for <EOT> */
	    break ;
	  }
	  else {
	    if ( guDebugFlags & DEBUG_PROTOCOL )
	      gHyp_util_logError ( 
		"Expecting <EOT>, got <0x%02x> from SECS device %s",
		pSecs1->inbuf[0], pSecs1->device ) ;

	    /* Keep waiting for <EOT> */
	    break ;

	  }
	}
      }


      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
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

      if ( guDebugFlags & DEBUG_PROTOCOL ) {
	pStr = blkStr ;
	for ( i=0; i<nBytes; i++ ) {
	  sprintf ( pStr, "%02x ", pSecs1->outbuf[i] ) ;
	  pStr += 3 ;
	}
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "->BLK [%s]", blkStr ) ; 
      }

      if ( nBytes <= 0 ) {
	gHyp_util_sysError ( "Failed to write SECS I block to SECS device %s",
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
	    if ( guDebugFlags & DEBUG_PROTOCOL )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
				   "<-NAK" ) ;
	  } 
	  else {
	    if ( guDebugFlags & DEBUG_PROTOCOL )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
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
	    retry = pSecs1->rty ; 
	    break ;
	  }
	}	  
      }

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "<-ACK" ) ;

      if ( pHeader->isLastBlock ) {

	gotENQ = (nBytes > 1 && pSecs1->inbuf[1] == SECS_CHAR_ENQ ) ;

	if ( gotENQ ) {

	  /* ENQ came back FAST from the sender.
	   * In this case, we are reading <ACK><ENQ> in the input buffer.
	   * Which means we have just sent out a message and the other side
	   * is sending us the <ACK>, but also it is sending an <ENQ> to 
	   * tell us it wants to send us a message now.   
	   *
	   * This 2-byte combination often causes a bug that is 
	   * largely undetected in the industry (imo), because the reader
	   * fails to see this <ENQ>, and thus it fails to send an <EOT>, and
	   * thus a T2 timeout occurs.
	   *
	   * HS is fast and even though it does a separate write for
	   * the <ACK> and the subsequent <ENQ>, the TCP stack will
	   * put these characters together and the destination may
	   * read them together.  If the equipment side does not see
	   * our first ENQ, then we will issue a T2 timeout waiting for
	   * the EOT.  This is common in HS logs, and the solution would
	   * be to put a usleep() call before any secs_event or secs_query
	   * call.  Perhaps 100ms would be a good time.
	   * 
	   * In any case, this bug is evidenced by a proliferation of 
	   * T2 errors in secs log files.
	   * 
	   */
	  if ( guDebugFlags & DEBUG_PROTOCOL ) {
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
				 "<-ENQ(ACK)" ) ;

	    if ( !pHeader->rBit )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
				   "ENQ contention. Relinquishing..." ) ;
	  }

	}

	if ( pHeader->isReplyExpected && gotENQ ) {
	  
	  /* We were expecting a reply and now we can get it.
	   * Accept the incoming message.
	   */
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

	  pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	  if ( cond < 0 ) return cond ;
	}
	else {
	  /* Not in a reply state or not got an ENQ */
	  if ( gotENQ )
	    pSecs1->state = SECS_EXPECT_SEND_EOT2 ; 
	  else 
	    pSecs1->state = SECS_EXPECT_RECV_ENQ ;
	  cond = COND_NORMAL ;
	 
	}

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

      if ( guDebugFlags & DEBUG_PROTOCOL )
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
			     "->NAK" ) ;
      
      if ( nBytes <= 0 ) {
	gHyp_util_sysError ( "Failed to write <NAK> to SECS device %s",
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
  return lHyp_secs1_incoming ( pSecs1, 
			       pConcept,
			       gHyp_concept_getInstForFd ( pConcept, pSecs1->fd ),
			       MESSAGE_QUERY, 
			       0, 0, NULL_DEVICEID, 0 ,0 ) ;
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
				     char *pEndBuf, 
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

  while ( pLine < pEndBuf ) {

    /* Advamnce past newline */
    pLine += strspn ( pLine, "\r\n" ) ;

    /* Search for the end of the attribute value section.
     * This is the first  "\r\n" that isn't preceeded by a ','
     */
    pChar = pLine ;
    while ( pChar < pEndBuf ) {
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
    if ( pChar >= pEndBuf ) break ;
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

	while ( pChar < pEndBuf ) {
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
		gHyp_data_setStr_n ( pValue, pLine, n ) ;
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
	  gHyp_data_setStr_n ( pValue, pLine, n ) ;
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
   *    - called from gHyp_sock_select<-gHyp_instance_read
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
    isEndOfMessage=FALSE,
    isEndOfBuffer=FALSE,
    isURLEncoded=FALSE,
    isPlainText=FALSE,
    isBinary=FALSE,
    isPOSTdata=FALSE,
    isXMLEncoded=FALSE,
    isLineBased=TRUE,
    doKnitting=FALSE,
    isZeroLength=FALSE,
    isMessageComplete,
    jmpEnabled ;
  
  sWORD
    forwardId,
    id = NULL_DEVICEID ;

  int
    i,
    n=0,
    ss,
    context,
    lineLen=0,
    width,width2,width3,
    forwardCount,
    actualContentLength=0,
    contentLength=0 ;

  sBYTE
    httpState;

  sSecs1
    *pForwardPort,
    *pListenPort ;

  char
    c,
    *pBuf,
    *pEndBuf,
    *pChar,
    *pTokenName,
    *pTokenValue,
    *pMsg,
    *pContent,*pContent2,*pContent3,
    *pLine,
    *pEndLine,
    *pArg1,*pArg2,*pArg3,
    target[TARGET_SIZE+1],
    sender[SENDER_SIZE+1],
    method[METHOD_SIZE+1],
    mode[MODE_SIZE+1],
    tid[FIELD_SIZE+1],
    value[VALUE_SIZE+2+1] ; /* Room for 2 extra characters.*/

  sData
    *pAttributeData=NULL,
    *pContentData,
    *pSTATUS,
    *pData,
    *pFirst,
    *pLast=NULL,
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


  /* If we're nested too deep in handlers, then forget it */
  if ( giJmpLevel == MAX_JMP_LEVEL ) {
    gHyp_util_logError ( "Port jump level overflow at %d", MAX_JMP_LEVEL ) ;
    longjmp ( gsJmpStack[0], COND_FATAL ) ;
  }

  /******************************************************************
   * WHO GETS THE DATA?
   ******************************************************************/
  
  pAIassigned = gHyp_concept_getInstForFd ( pConcept, pPort->fd ) ;

  if ( pAIassigned ) {

    /* Get id that is assigned to the socket */
    id = gHyp_instance_getDeviceId ( pAIassigned, (SOCKET) pPort->fd ) ;

    if ( guDebugFlags & DEBUG_DIAGNOSTICS )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	"Data on socket %d is assigned to id %d for target %s",
	  pPort->fd, id, gHyp_instance_getTargetId ( pAIassigned ) ) ;
    
  }
  else {

    gHyp_util_logWarning ( "No instance has assigned any id to the socket %d", pPort->fd );

    /* No instance has assigned any id to the port.*/
    if ( pPort->parentSocket != INVALID_SOCKET &&
	 pPort->fd != pPort->parentSocket ) {

      /* This is a newly created channel */
      gHyp_util_logInfo ( "New socket connection %d off of parent socket %d",
	pPort->fd, pPort->parentSocket ) ;

      pAIassigned = gHyp_concept_getInstForFd ( pConcept, pPort->parentSocket ) ;

      if ( pAIassigned ) {

	id = gHyp_instance_getDeviceId ( pAIassigned, pPort->parentSocket ) ;

	if ( id == NULL_DEVICEID ) {
          gHyp_util_logError ( "No device id has been assigned to listen port %d",
			 pPort->parentSocket ) ;
	  /* Close the port */
	  gHyp_instance_signalHangup ( pAIassigned, (int) pPort->fd, pPort->modifier, id ) ;
	  gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pPort->fd ) ;
	  return COND_SILENT ;
	}

  	gHyp_util_logInfo ( "Reassigning device id %d to socket %d from listen socket %d",
	 		    id, pPort->fd, pPort->parentSocket );

	/* Take any SSL structures as well */
	pListenPort = (sSecs1*) gHyp_concept_getSocketObject ( pConcept, 
							      (SOCKET) pPort->parentSocket, 
							      DATA_OBJECT_NULL ) ;
							      /*objectType ) ;*/
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

      }

    }
  }

  /******************************************************************
   * READ THE BUFFER CONTENTS
   ******************************************************************/

  /* Save the previous longjmp return point. */
  jmpEnabled = giJmpEnabled ;
  giJmpEnabled = FALSE ;
  pBuf = gzInputBuffer ;
  if ( guSIGMSG ) {
    guSIGMSG = 0 ;

    /* giNbytes is already set, so is the gzInputBuffer.
     * We are not going to be issuing a read.
     */
    gHyp_util_debug("Processing appended message of %d bytes [%.*s...]",
		    giNbytes,MIN(giNbytes,60),gzInputBuffer );

  }
  else {
    if ( pPort->savebuflen > 0 ) {
      gHyp_util_debug("Short HTTP Message save previously is restored, inserting %d bytes to buffer start",pPort->savebuflen); 
      memmove ( gzInputBuffer, pPort->savebuf, pPort->savebuflen ) ;
      pBuf += pPort->savebuflen ;
    }
    giNbytes =  gHyp_sock_read ( (SOCKET) pPort->fd,
			     (char*) pBuf, 
			     (objectType==DATA_OBJECT_HTTP)?HTTP_READ_SIZE:PORT_READ_SIZE,
			     (short)(pPort->flags&MASK_SOCKET),
			     0, /* 0 means read already completed, just get result */
			     &pPort->nBytes,
  			     &pPort->overlapped,
			     pPort->pSSL) +
			     pPort->savebuflen ;

    /* Watch out for zero byte reads when we have a savebuf condition */
    if ( pPort->savebuflen == giNbytes ) giNbytes = 0 ;
    pPort->savebuflen = 0 ;
  }

  /* Restore the previous longjmp return point. */
  giJmpEnabled = jmpEnabled ;
  
  if ( giNbytes < 0 ) {
    gHyp_util_logError ( "Failed to read data from socket=%d, id=%d, device=%s",
			 pPort->fd,id,pPort->device ) ;
    /* Close the port */
    if ( pAIassigned ) gHyp_instance_signalHangup ( pAIassigned, 
      (int) pPort->fd, pPort->modifier, id ) ;
    gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pPort->fd ) ;
    return COND_SILENT ;
  }
  else if ( giNbytes == 0 ) {
    
    if ( id != NULL_DEVICEID )
      gHyp_util_logWarning ( "Failed to receive data from socket %u, device %d (%s)",
			   pPort->fd, id, pPort->device ) ;

    else
      gHyp_util_logWarning ( "Failed to receive data from socket %d",
			   pPort->fd ) ;

    if ( (pPort->flags & SOCKET_TCP) ) {
      /* Zero bytes from TCP/IP sockets is always fatal */
      if ( pAIassigned ) gHyp_instance_signalHangup ( pAIassigned, (int) pPort->fd, pPort->modifier, id ) ;
      gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pPort->fd ) ;
    }
    else
      gHyp_instance_signalPipe ( pAIassigned, (int) pPort->fd, pPort->modifier, id ) ;

    return COND_SILENT ;
  }
  else {

    if ( !pAIassigned ) {
      gHyp_util_logError ( "No id is assigned to socket %d for device %s",
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
    gzInputBuffer[giNbytes] = '\0' ;
    pBuf = gzInputBuffer ;
    pEndBuf = pBuf + giNbytes ;

    if ( guDebugFlags & DEBUG_PROTOCOL ) {
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
		"Id %u, socket %d, %u bytes",id,pPort->fd,giNbytes) ; ;
      pLine = pBuf ;
      while ( pLine < pEndBuf ) {
	lineLen = MIN ( INTERNAL_VALUE_SIZE, (pEndBuf-pLine) ) ;
        n = gHyp_util_unparseString ( value, pLine, lineLen, VALUE_SIZE, FALSE, FALSE, FALSE,"" ) ;
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,"[%s]",value) ;
	pLine += lineLen ;
      }
    }
    if ( guDebugFlags & DEBUG_DIAGNOSTICS ) {
	gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
		"Received %u bytes on socket %d, id %d",giNbytes,pPort->fd,id) ;
    }

    /******************************************************************
     * PORT-FORWARD IF REQUESTED
     ******************************************************************/
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

	    objectType = gHyp_concept_getSockObjType ( pConcept, fd ) ;
	    if ( objectType == DATA_OBJECT_PORT || objectType == DATA_OBJECT_HTTP ) {

	      pForwardPort = (sSecs1*) gHyp_concept_getSocketObject (
			      pConcept, 
	    		      fd, 
			      objectType  ) ;
	      if ( !(gHyp_secs1_flags(pForwardPort) & (SOCKET_LISTEN | SOCKET_UNIX_LISTEN)) ) {

		
		if ( guDebugFlags & DEBUG_DIAGNOSTICS ) {

#ifdef AS_WINDOWS
		  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
					"%d -> %d (%u) %u bytes",id,forwardId,GetTickCount(),giNbytes);
#else
		  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
					"%d -> %d (%u) %u bytes",id,forwardId,gsCurTime,giNbytes) ;
#endif	        

  
		}
 		n = gHyp_sock_write ( (SOCKET) fd, 
				      gzInputBuffer, 
				      giNbytes,
				      (short) (pForwardPort->flags&MASK_SOCKET), 
				      (OVERLAPPED*) &pForwardPort->overlapped, 
				      pForwardPort->pSSL ) ;

		if ( n <= 0 ) {
		  gHyp_util_sysError ( "Failed to forward data to device id %d",forwardId ) ;
		  gHyp_instance_signalHangup ( pAIassigned, (int) fd, pForwardPort->modifier, forwardId ) ;
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

    /******************************************************************
     * PRE-PROCESS THE DATA - GET HTTP HEADER ELEMENTS
     ******************************************************************/
    if ( objectType == DATA_OBJECT_HTTP ) {
    
      pHTTP = pPort->pHTTP ;

      /* Get the state of the object and its current contents */
      httpState = gHyp_http_getState ( pHTTP ) ;
      pAttributeData = gHyp_http_getAttributeData( pHTTP ) ;
      pContentData = gHyp_http_getContentData ( pHTTP ) ;
      actualContentLength = gHyp_http_getActualContentLen( pHTTP ) ;
      contentLength = gHyp_http_getContentLength( pHTTP ) ;
      isChunkedTransferEncoded = gHyp_http_isChunkedTransferEnc(pHTTP);
      isXMLEncoded = gHyp_http_isXMLEncoded(pHTTP);
      isURLEncoded = gHyp_http_isURLEncoded(pHTTP);
      isPlainText = gHyp_http_isPlainText(pHTTP);
      isBinary = gHyp_http_isBinary(pHTTP);
      isZeroLength = gHyp_http_isZeroLength(pHTTP);
      
      if ( httpState == HTTP_EXPECT_HEADER ) {
      
	/* Look for the double newline. */
	pContent = pBuf + giNbytes ;
	pContent2 = strstr ( pBuf, "\n\n" ) ;

	/* If we find it, set pointer to start of content (set the number
	 * of extra hops to get the start of the content)
	 */
	if ( pContent2 && pContent2 < pContent ) { pContent = pContent2 ; n = 1 ; }

	/* But also check to see if the content is proceeded by double newlines */
	pContent3 = strstr ( pBuf, "\r\n\r\n" ) ;

	/* If it is found, we use that instead, set the number of extra hops to 3 */
	if ( pContent3 && pContent3 < pContent ) { pContent = pContent3 ; n = 3 ; }

	/* If we didnt' get enough of the header, then abandon now */
	if ( !pContent2 && !pContent3 ) {
	  gHyp_util_debug("HTTP Message too short, saving %d bytes.",giNbytes); 
	  memmove ( pPort->savebuf, gzInputBuffer, giNbytes ) ;
	  pPort->savebuflen = giNbytes ;
	  return COND_SILENT ;
	}

	/* Null terminate the header */
	*pContent = '\0' ;

	if ( pContent < (pBuf+giNbytes) ) {
	  /* There appears to be some content */
	  pContent++ ;
	  pContent += n ;  /* Position to the beginning of the data stream */
	  actualContentLength = giNbytes - (pContent-gzInputBuffer) ;
  	  gHyp_http_updateContent ( pHTTP,  contentLength, actualContentLength ) ;
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
		n = sscanf ( pChar, "%d", &contentLength )  ;
		if ( n != 1 || contentLength < 0 ) contentLength = 0 ;
	        if ( guDebugFlags & DEBUG_PROTOCOL )
		  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
		    "Content/Actual Length = %d/%d",contentLength,actualContentLength);
		gHyp_http_updateContent ( pHTTP,  contentLength, actualContentLength ) ;
		/* Keep track of zero length payloads */
		if ( contentLength == 0 ) {
		  isZeroLength = TRUE ;
		  gHyp_http_setZeroLength ( pHTTP ) ;
		}
	      }
	    }
	  }
	}

	/*
	if ( !isZeroLength && actualContentLength == 0 ) {
	  gHyp_util_debug("HTTP Message needs content, appending %d bytes later",giNbytes); 
	  memmove ( pPort->savebuf, gzInputBuffer, giNbytes ) ;
	  pPort->savebuflen = giNbytes ;
	  return COND_SILENT ;
        }
	*/

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
		  gHyp_http_setChunkedTransferEnc ( pHTTP ) ;
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
		else if ( strstr ( pChar, "text/java" ) != NULL ) {
		  isBinary = TRUE ;
		  gHyp_http_setBinary ( pHTTP, TRUE ) ;
		}
		else if ( strstr ( pChar, "text/plain" ) != NULL ) {
		  isPlainText = TRUE ;
		  gHyp_http_setPlainText ( pHTTP ) ;
		}
		else if ( strstr ( pChar, "application/x-www-form-urlencoded" ) != NULL ) {
		  isURLEncoded = TRUE ;
		  gHyp_http_setURLEncoded ( pHTTP ) ;
		}
		else if ( strstr ( pChar, "application/binary" ) != NULL ) {
		  isBinary = TRUE ;
		  gHyp_http_setBinary ( pHTTP, TRUE ) ;
		}
	      }
	    }
	  }
	}

        /******************************************************************
         * SAVE THE HTTP HEADER ELEMENTS
         ******************************************************************/

	/*
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
	  pEndLine = pLine + lineLen ;
	  *pEndLine = '\0' ;

	  /* The first component ends with whitespace */
	  width = strcspn ( pLine, " \t" ) ;
	  if ( width == 0 ) { 
    	    gHyp_util_logError("Invalid HTTP component 1 in header: '%s'",pLine ) ;
	    return COND_SILENT ;
	  }
	  /* Terminate first component */
	  *(pLine+width) = '\0' ;
	  pArg1 = pLine ;

	  /* Advance to start of component 2 */
	  pLine += (width+1) ;
	  pLine += strspn ( pLine, " \t" ) ;	  
	  if ( pLine > pEndLine ) { 
    	    gHyp_util_logError("Invalid HTTP header: '%s'",pBuf ) ;
	    return COND_SILENT ;
	  }

	  /* The second component ends with whitespace */
	  width = strcspn ( pLine, " \t" ) ;
	  if ( width == 0 ) { 
    	    gHyp_util_logError("Invalid HTTP component 2 in header: '%s'",pLine ) ;
	    return COND_SILENT ;
	  }
	  /* Terminate second component */
	  *(pLine+width) = '\0' ;
	  pArg2 = pLine ;

	  /* Advance to start of component 3 */
	  pLine += (width+1) ;
	  pLine += strspn ( pLine, " \t" ) ;	  

	  if ( pLine > pEndLine ) { 
    	    gHyp_util_logError("Invalid HTTP header: '%s'",pBuf ) ;
	    return COND_SILENT ;
	  }

	  /* The third component ends with the line */
	  width = strlen ( pLine ) ;
	  if ( width == 0 ) { 
    	    gHyp_util_logError("Invalid HTTP component 3 in header: '%s'",pLine ) ;
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

	  pBuf = pContent ;
	}
      }
      else {
	/* We've already processed the HTTP header.
	 * The entire buffer is content continued.
	 * Add the length to the content received so far.
	 */
	pContent = gzInputBuffer ;
	actualContentLength += giNbytes ;
	if ( guDebugFlags & DEBUG_PROTOCOL )
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
	    "Actual/content now is %d/%d",actualContentLength,contentLength);
  	gHyp_http_updateContent ( pHTTP,  contentLength, actualContentLength ) ;
        pBuf = pContent ;

      }
    }
    else {

      /* Standard port incoming data.
       * The entire buffer is the result 
       */
      pContent = gzInputBuffer ;
      pBuf = pContent ;
      pContentData = gHyp_data_new ( "_data_" ) ;
      actualContentLength = giNbytes ;
      contentLength = actualContentLength ;
      isLineBased = ! gHyp_secs1_isBinary ( pPort ) ;
    }

    /******************************************************************
     * EXTRACT THE DATA INTO THE pContentData sData object.
     ******************************************************************/

    while ( !isEndOfBuffer ) {

      if ( objectType == DATA_OBJECT_HTTP ) {

	if ( contentLength == 0 ) {

	  /* We don't know how much data there is */

	  if ( isChunkedTransferEncoded ) {

	    /* There may be several chunks in a buffer.
	     * Chunks do not necessarily align with the
	     * start or end of the content.
	     * When contentLength is 0, we are expecting
	     * a chunked length.
	     * If the chunked length is zero, we are done.
	     */

	    /* First skip past any line breaks */
	    pChar = pBuf ;
	    pBuf += strspn ( pChar, "\r\n" ) ;

	    /* pBuf should be pointing to a hex value which is the content length.*/

	    pChar = pBuf ;
	    width = strcspn ( pChar, "\r\n" ) ;
	    pBuf += width;
	    if ( width > 0 && width < VALUE_SIZE ) {

	      strncpy ( value, pChar, width ) ;
	      value[width]='\0' ;
	      pChar = strchr ( value, ';' ) ;
	      if ( pChar ) *pChar = '\0' ;
	      gHyp_util_trim ( value ) ;

	      /*gHyp_util_debug("Chunked value = %s at offset %d",value,pBuf-gzInputBuffer );*/
	      n = sscanf ( value, "%x", &contentLength )  ;
	      if ( n != 1 ) {

		/* Conversion error. */
		gHyp_util_logWarning("Expecting chunked Content-Length value but instead received '%s'",value);
		contentLength = 0 ;
  	        gHyp_http_updateContent ( pHTTP,  contentLength, actualContentLength ) ;
	      }

	      else if ( contentLength == 0 ) {

		/*gHyp_util_debug("Chunked transfer complete");*/
  	        gHyp_http_updateContent ( pHTTP,  contentLength, actualContentLength ) ;
		isEndOfMessage = TRUE ;
		lHyp_secs1_httpHeaders ( pBuf, pEndBuf, pAttributeData ) ;
		isEndOfBuffer = TRUE ;
	      } 
	      else { /* if ( contentLength > 0 ) { */

		/* Got a content length. Get new start of content */
		pChar = pBuf ;
		
		/* Not we skip over a single CRLF */
		if ( *pChar != '\r' || *(pChar+1) != '\n' ) 
		  gHyp_util_logWarning("Did not get expected CRLF after chunked value");
	        else
		  pChar+=2 ;

		/* Update pContent */
		pContent = pChar ;
		pBuf = pContent ; 
		actualContentLength = pEndBuf - pContent ;
  		gHyp_http_updateContent ( pHTTP, 
					  contentLength,
					  actualContentLength ) ;
 		/*gHyp_util_debug("Content/Actual (1) = %d/%d",contentLength,actualContentLength);*/
	      }
		
	    }
	    else {
	      /*gHyp_util_debug("No chunked value");*/
	      /* No chunked value, probably in next read */	    
	      if ( pBuf == pEndBuf ) isEndOfBuffer = TRUE ;
	    }
	  }
	  else {

	    /* Not chunked transfer. If the content is text/html,
	     * then if we see a </html> or </HTML> at the end, we
	     * say we are done. Another way maybe is to see a CR
	     * or LF at the end, but this could be unpredictable.
	     * If 'Content-Length: 0' was seen, then end-of-message
	     * and if actualContentLength is > 0, its an appended msg.
	     */
	    if ( isZeroLength ) {
	      isEndOfMessage = TRUE ;
	    }
	    else if ( actualContentLength > 0 ) { 

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
	} /* end if ( contentLen == 0 ) */

      } /* end if ( objectType == DATA_OBJECT_HTTP ) */
      
      /* How many bytes should we process? */
      n = MIN ( actualContentLength, pEndBuf-pContent ) ;
       if ( isChunkedTransferEncoded ) {
	if ( actualContentLength > contentLength ) {
	  n -= ( actualContentLength - contentLength ) ;
	}
      }
      else {
	if ( isZeroLength || (contentLength > 0 && contentLength < actualContentLength) ) {
	  n -= ( actualContentLength - contentLength );
	}
      }

      if ( n > 0 ) {
	
	doKnitting = FALSE ;
	lineLen = 0 ;

	if ( objectType == DATA_OBJECT_HTTP ) {

          /************************************************************************
	   * KNIT LAST LINE FROM PREVIOUS BUFFER WITH FIRST LINE OF CURRENT BUFFER 
           *************************************************************************/
	  isLineBased =  ( (!isBinary && isPlainText ) || isXMLEncoded ) ;

	  /* Find out if we even need to knit */
	  pLast = NULL ;
	  if ( isLineBased && pContentData ) {

	    /* Get last value of pContent data */
	    pLast = gHyp_data_getLast ( pContentData ) ;
	    if ( pLast ) {
	      lineLen = gHyp_data_getStr ( pLast, value, VALUE_SIZE, 0, TRUE ) ;
	      value[lineLen] = '\0' ;
	      /* So long as the line does not end with \n or \n\r, we can knit it to the
	       * start of the next line
	       * We use the variables pLast,value and lineLen again below..
	       */
	      doKnitting = TRUE ;
	      if ( value[lineLen-1] == '\n' ) 
		doKnitting = FALSE ;
	      else if ( lineLen > 1 && value[lineLen-2] == '\n' && value[lineLen-1] == '\r' ) 
	        doKnitting = FALSE ;
	    }
	  }
	}

	/* Extract the content, adding it section by section to pContentData */
	f = TRUE ;
	pBuf = pContent ;
	while ( n > 0 ) {

	  /* We use INTERVAL_VALUE size to cover the case where each byte
	   * happens to require a 4 character \nnn to represent it.
	   * To display exterally, the would require INTERNAL_VALUE_SIZE*4 bytes,
	   * which is VALUE_SIZE bytes, the HyperScript internal maximum segment
	   * size of a 'str'.
	   */
	  width = MIN ( n, INTERNAL_VALUE_SIZE ) ;

	  if ( isLineBased ) {

	    /* The data should be logically divided by line feeds. */

	    /* Get the length of the next line */
	    width2 = strcspn ( pBuf, "\r\n" ) ;

	    if ( width2 == n )
	      /* Buffer ends without a \r\n".
	       * Never take more than INTERNAL_VALUE_SIZE.
	       */
	      width2 = width ;
	    
	    else
	      
	      /* Add the <lf> and/or the <cr> back into the total width of the line */
	      width2 += strspn ( pBuf+width2, "\r\n" ) ;
	    
	    if ( width2 > 0 ) {
	  
	      /* There is a line. */
	      if ( width2 < width ) {

		/* Shorter is always ok */
		width = width2 ;
	      }
	      else if ( width2 > width ) { 

		/* We'd like to store longer, but will it fit? 
		 *
		 * A non-printing character must be able to be represented 
		 * in an externl \nnn format and also fit into VALUE_SIZE.  
		 * If all the characters were non-printing, we'd have 4x
		 * the required space.
		 * Note below we extract lineLen from VALUE_SIZE just in case
		 * there is a previous line that needs to be knitted with this one,
		 * we want to make sure the whole thing will be <= VALUE_SIZE.
		 */

		width3 = width2 ;
		for ( i=0;i<width2;i++ ) if ( pBuf[i] <= 0 || !isprint(pBuf[i]) ) width3+=3;
		if ( width3 <= (VALUE_SIZE-lineLen) ) 
		  /* Yes, it will fit - adjust size */
		  width = width2 ;

		/* If not, we have no recourse but to take INTERNAL_VALUE_SIZE
		 * of the line.
		 */
	      }

	      /* No recourse to take INTERNAL_VALUE_SIZE??.
	       * Not exactly.
	       */

	      if ( width == INTERNAL_VALUE_SIZE ) {

		/* Let's not cut words in half if we can help
		 * it. Make the string a little shorter if we find a space
		 * between non-spaces.
		 * Start at the end of the string, search backwards over
		 * non-spaces until we find a space.  If so, and its
		 * not the beginning of the string, then we split the
		 * string.
		 */
		pChar = pBuf+width ;

		while ( pChar > pBuf ) {

		  c = *pChar ;

		  /* If we find a space, split the string right here. */
		  if ( c == ' ' ) break ;
		  pChar-- ;
		}

		/* Adjust width if necessary */
		if ( (pChar - pBuf) > 0 ) {
		  width = pChar - pBuf ;
		}
		else {

		  /* Look for comma */
		  pChar = pBuf+width ;

		  while ( pChar > pBuf ) {

		    c = *pChar ;
		    /* If we find a space, split the string right here. */
		    if ( c == ',' ) break ;
		    pChar-- ;
		  }

		  /* Adjust truncStrLen if necessary */
		  if ( pChar - pBuf > 0 ) {
		    width = pChar - pBuf ;
		  }
		}

	      }
	    }
	  }

	  /* Now the width may be undeniably INTERVAL_VALUE_SIZE */

	  if ( f && doKnitting &&  (width + lineLen <= VALUE_SIZE) ) {

	    /* - f means we are the first line of the NEXT segment.
	     * - doKnitting means the last line of the previous segment DID NOT end in \n\r
	     * - linelen+width is the sum of how big the knitted line will be.
	     * - value is the current value of the last line of the previous segment.
	     */
	
	    /* Knit the section to the last value */
	    memcpy ( value+lineLen, pBuf, width ) ;
	    lineLen += width ;
	    /*gHyp_util_debug ( "Knitted line '%.*s'",lineLen, value ) ;*/

	    /* Replace pLast value with new one */
	    gHyp_data_setStr_n ( pLast, value, lineLen ) ;
	    doKnitting = FALSE ;

	  }
	  else {

	    /* Cannot knit the section to the last value.  Make it 
	     * a new value and add it to the content array, 
	     */
	    pValue = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr_n ( pValue, (char*) pBuf, width ) ;
	    gHyp_data_append ( pContentData, pValue ) ;
	    /*if ( isLineBased ) gHyp_util_debug("Added: %.*s",width,pBuf) ;*/
	  }

	  f = FALSE ;
	  pBuf += width ;
	  pContent=pBuf;
	  n -= width ;

	} /* end while ( n > 0 ) */


	/*gHyp_util_debug("Content/Actual (2) = %d/%d",contentLength,actualContentLength);*/
	if ( actualContentLength < contentLength ) {

	  /* We haven't received all of the content. Wait for more. */
	  if ( guDebugFlags & DEBUG_PROTOCOL )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
	      "Incomplete message (%d bytes), %d expected, %d received, %d remaining",
	  giNbytes, contentLength, actualContentLength, (contentLength-actualContentLength) );
	  
	  return COND_SILENT ;
	}

	else {

	  /* ( actualContentLength >= contentLength )
	   * We either received all the data or we've received more that expected 
	   */

	  if ( isChunkedTransferEncoded ) {

	    /* There may be more chunks. */
	      
	    /* Find the segment after the chunk just processed,
	     * the next chunk's size value should be there.
	     */
	    pBuf = pContent ; /*buffer + (contentLength+giNbytes-actualContentLength) ;*/
	    pChar = pBuf ;

	    /* Skip past any line breaks */
	    pBuf += strspn ( pChar, "\r\n" ) ;

	    /* Reset the http to expect a new chunk */
	    actualContentLength = 0 ;
	    contentLength = 0 ;
	    gHyp_http_updateContent ( pHTTP, 
				      contentLength,
				      actualContentLength ) ;

	    /* Check if we are at the end of the buffer */
	    if ( pBuf == pEndBuf ) isEndOfBuffer = TRUE ;

	    /* Otherwise loop around and continue processing */
	  }
	  else {

	    /* Not chunked */

	    /* For the case where we get an HTTP header without
	     * any contentLength specified, and we are not at
	     * the end of the buffer, then expect more data.
	     */
	    if ( contentLength == actualContentLength ) isEndOfBuffer = TRUE ;
	    if ( contentLength > 0 && actualContentLength >= contentLength ) isEndOfBuffer = TRUE ;
	    if ( isZeroLength ) isEndOfBuffer = TRUE ;
	  }
	} 
      } /* end if ( n > 0 ) */
      else
	/* No more data */
	isEndOfBuffer = TRUE ;

    } /* end while ( !isEndOfBuffer ) */

    /******************************************************************
     * BUFFER IS COMPLETE.  IF MESSAGE IS COMPLETE, THEN PROCESS AND RETURN
     ******************************************************************/

    if ( objectType == DATA_OBJECT_HTTP ) {

      if ( guDebugFlags & DEBUG_PROTOCOL )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
	  "HTTP buffer received: content/actual = %d/%d",contentLength,actualContentLength) ;

      if ( !isEndOfMessage ) {

	if ( isChunkedTransferEncoded ) {

	  if ( contentLength > 0 && 
	       contentLength == actualContentLength ) {
	    /* Done the chunk, ready for another */
	    contentLength = 0 ;
	    actualContentLength = 0 ;
	    gHyp_http_updateContent ( pHTTP, 
				    contentLength,
				    actualContentLength ) ;
	  }
	  return COND_SILENT ;
	}
	else {
	  /* Not chunked */
	  if ( contentLength > actualContentLength ) {

	    if ( guDebugFlags & DEBUG_PROTOCOL )
	      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_PROTOCOL,
	      "Incomplete message (%d bytes), %d expected, %d received, %d remaining",
	    giNbytes, contentLength, actualContentLength, (contentLength-actualContentLength) );

	    return COND_SILENT ;
	  }
	}
      }

      /* If haven't got an official content by now, set it to the actual */
      if ( !isZeroLength && contentLength == 0 && actualContentLength > 0 ) {
	contentLength = actualContentLength ;
	gHyp_http_updateContent ( pHTTP, 
			      contentLength,
			      actualContentLength ) ;
      }

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
	  n = MIN ( strlen ( pTokenName), VALUE_SIZE ) ;
	  strncpy ( value,  pTokenName, n  ) ;
	  value[n] = '\0' ;
	  pTokenName = value ;
	  while ( *pTokenName ) {

	    /* Look for '=' */
	    pTokenValue = strchr ( pTokenName, '=' ) ;
	    if ( !pTokenValue ) break ;

	    /* Make sure there is both a token and a value */
	    *pTokenValue = '\0' ;
	    gHyp_util_trim ( pTokenName ) ;
	    if ( strlen ( pTokenName ) == 0 ) break ;

	    /*gHyp_util_debug("Adding tokenname %s",pTokenName);*/
	    /* Get the token value */
	    pTokenValue++ ;
		  
	    pChar = strchr ( pTokenValue, '&' ) ;
	    if ( pChar ) *pChar++ = '\0' ;
	    else pChar = pTokenValue + strlen ( pTokenValue ) ;

	    gHyp_util_trim ( pTokenValue ) ;

	    /*gHyp_util_debug("Adding tokenvalue %s",pTokenValue);*/
	    pVariable = gHyp_data_new ( pTokenName ) ;
 	    pValue = gHyp_data_new ( NULL ) ;
	    if ( strlen ( pTokenValue ) > 0 ) {
    	      gHyp_cgi_plusToSpace( pTokenValue );
	      gHyp_cgi_unescapeUrl( pTokenValue );
	      gHyp_data_setStr ( pValue, pTokenValue ) ;
	    }
	    gHyp_data_append ( pVariable, pValue ) ;
	    gHyp_data_append ( pTV, pVariable ) ;
	    pTokenName = pChar ;
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


      /* Check that the content is XML or URLencoded */
      if ( contentLength > 0 && isPOSTdata ) {

	/* Look to see if the data starts with a <?xml" */
	pData = gHyp_data_getValue ( pContentData, 0, TRUE ) ;
	n = gHyp_data_getStr (	pData, 
				value, 
				VALUE_SIZE, 
				0, 
				TRUE ) ;
	gHyp_util_lowerCase ( value, n ) ;
	if ( strstr ( value, "<?xml" ) != NULL) {
	  isXMLEncoded = TRUE ;
	  gHyp_http_setXMLEncoded ( pHTTP ) ;
	}
      }

      /* Parse the content */
      if ( isXMLEncoded ) {
	gHyp_cgi_xmlData ( pContentData, pAIassigned, gHyp_instance_frame(pAI), pTV ) ;
      }
      else if ( contentLength > 0 && isPOSTdata && !isBinary ) {
	
	/* Process the POST data */
	pVariable = NULL ;
	isVector = FALSE ;
	ss = -1 ; 
	context = -1 ;
	pData = NULL ;
	pChar = gzOutBuffer ;
	/* First put all the contentData in a big buffer */
	while ( (pData = gHyp_data_nextValue ( pContentData, 
  					       pData,
					       &context,
					       ss ))) {

	  n = gHyp_data_getStr ( pData, 
				 pChar, 
				 VALUE_SIZE, 
				 context, 
				 isVector ) ;
	  pChar += n ;
	  if ( pChar-gzOutBuffer > (MAX_INPUT_LENGTH-VALUE_SIZE) ) break ;

	}
	*pChar++ = '\0' ;

	/*gHyp_util_debug("Appended %d bytes of post data in %d buffer",(pChar-gzOutBuffer),MAX_INPUT_LENGTH );*/

	/* Now parse the long string */
	pTokenName = gzOutBuffer ;
	while ( *pTokenName ) {

	  /* Look for '=' */
	  pTokenValue = strchr ( pTokenName, '=' ) ;
	  if ( !pTokenValue ) break ;

	  /* Make sure there is both a token and a value */
	  *pTokenValue = '\0' ;
	  gHyp_util_trim ( pTokenName ) ;
	  if ( strlen ( pTokenName ) == 0 ) break ;
	  pVariable = gHyp_data_new ( pTokenName ) ;
	  gHyp_data_append ( pTV, pVariable ) ;

  	  /*gHyp_util_debug("New token %s",pTokenName);*/

	  /* Get the token value */
	  pTokenValue++ ;

	  /* Look for end of value section */
	  pChar = strchr ( pTokenValue, '&' ) ;
	  if ( !pChar ) {
    	    gHyp_util_debug("Last value %s",pTokenValue);
	    pChar = pTokenValue + strlen ( pTokenValue ) ;
	    *pChar = '\0' ;
	  }
	  else
	    /* Mark end of value */
	    *pChar++ = '\0' ;

	  pTokenName = pChar ; 

 	  pValue = gHyp_data_new ( NULL ) ;
	  if ( strlen ( pTokenValue ) > 0 ) {
	    if ( isURLEncoded ) {
	      gHyp_cgi_plusToSpace( pTokenValue );
	      gHyp_cgi_unescapeUrl( pTokenValue );
      	      gHyp_data_setStr ( pValue, pTokenValue ) ;
  	      /*gHyp_util_debug("New token value %s",pTokenValue);*/
	    }
	  }
	  if ( pVariable ) gHyp_data_append ( pVariable, pValue ) ;
	}
      }
      
      /* Always deliver the raw content */
      pData = gHyp_data_new ( "_http_data_" ) ;
      if ( pContentData )
        gHyp_data_moveValues ( pData, pContentData ) ;
      gHyp_data_append ( pTV, pData ) ;
	  
      /* Add a variable _http_datalen_ */
      pData = gHyp_data_new ( "_http_datalen_" ) ;
      gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
      gHyp_data_setVectorRaw ( pData, &contentLength, 0 ) ;
      gHyp_data_append ( pTV, pData ) ;

      /* Reset the http data area */
      gHyp_http_reset ( pHTTP ) ;

      /* Reset state */
      pPort->state = MESSAGE_EVENT ;

      /* If actualContentLength was greater than contentLength, then this
       * is another message appended to the end.
       * We're going to have to create a signal and save it in the start of the buffer,
       * then come right back and process it too!
       */
      if ( (isZeroLength || contentLength > 0) && actualContentLength > contentLength ) {
        guSIGMSG = 1 ;
	giNbytes = actualContentLength-contentLength ;
        memmove ( gzInputBuffer, pEndBuf-giNbytes, giNbytes );
	gzInputBuffer[giNbytes] = '\0' ;
	gHyp_util_debug("Appended message of %d bytes [%.*s...] ",giNbytes,MIN(giNbytes,60),gzInputBuffer );
      }

    }
    else {
      /* Raw port data. Every message is a complete message. */
      pTV = gHyp_data_new ( "_data_" ) ;
      gHyp_data_append ( pTV, pContentData ) ;
      /* Add a variable _data_len_ */
      pData = gHyp_data_new ( "_datalen_" ) ;
      gHyp_data_newVector ( pData, TYPE_INTEGER, 1, TRUE ) ;
      gHyp_data_setVectorRaw ( pData, &contentLength, 0 ) ;
      gHyp_data_append ( pTV, pData ) ;
      isMessageComplete = TRUE ;
      isReply = ( pPort->state == MESSAGE_QUERY ) ;
      /* Reset state */
      pPort->state = MESSAGE_EVENT ;
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
	  sprintf ( sender, "%u#port%s", id, gzRoot ) ;
	else
	  sprintf ( sender, "%u#http%s", id, gzRoot ) ;
	strcpy ( target, gHyp_instance_getTargetPath ( pAIassigned ) ) ;

	/*gHyp_util_debug("The target is %s",targetId);*/

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
          sprintf ( sender, "%u#port%s", id, gzRoot ) ;
	}

	/*gHyp_util_debug("TP is %s",gHyp_instance_getTargetPath ( pAIassigned ) ) ;*/
	strcpy ( target, gHyp_instance_getTargetPath ( pAIassigned ) ) ;

	/* What method does this function invoke? */
	pVariable = gHyp_instance_portMethod ( pAIassigned, id ) ;
	if ( !pVariable ) {
          gHyp_util_logError ( 
	    "No method enabled for incoming message from socket %d, id %d, device %s",
			 pPort->fd,id,pPort->device ) ;
	  gHyp_data_delete ( pTV ) ;

          gHyp_instance_signalHangup ( pAIassigned, (int) pPort->fd, pPort->modifier, id ) ;
	  gHyp_concept_deleteSocketObject ( pConcept, (SOCKET) pPort->fd ) ;
	  return COND_SILENT ;
	}
 
	/* Set the method of the message */
	strcpy ( method, gHyp_data_getLabel ( pVariable ) ) ;

      }
    
      /*gHyp_util_debug("Received id=%d mode=%s, target=%s, tid = %s",id,mode,target,gHyp_instance_getTargetId ( pAIassigned ) );*/

      strcpy ( tid, "00000001" ) ;
      /*strcpy ( tid, gHyp_util_random8() ) ;*/
      pMsg = gHyp_aimsg_initUnparsePartial ( 
		gHyp_instance_incomingMsg ( pAIassigned ),
		target,
		mode,
		method,
		sender,
		tid,
		gHyp_util_timeStamp ( gsCurTime ),
		pTV ) ;

      if ( pMsg ) return strlen ( pMsg ) ;
    }
    return COND_SILENT ;
  }
}

int gHyp_secs1_rawOutgoing ( sSecs1 *pPort, sInstance *pAI, sData *pData, int id, sBYTE objectType )
{
  /* Three return values:
   * > 0  = message written 
   * = 0  = no message written, non-fatel case
   * 0  = fatal error.
   *
   * Called from gHyp_port_event, gHyp_port_query.
   * A return value of 0 means to resend the message.
   */
  sLOGICAL
    isVector ;
  
  sBYTE
    dataType ;

  sData
    *pResult ;

  int
    count,
    ss,
    write_size,
    cmdLen,
    context,
    valueLen,
    totalBytes=0,
    nBytes=0 ;

  char
    command[HTTP_WRITE_SIZE+1],
    *pCmd,
    *pEndCmd,
    *pStr ;

  write_size = (objectType==DATA_OBJECT_HTTP)?HTTP_WRITE_SIZE:PORT_WRITE_SIZE;

  /* Construct the message to be sent to the port. */
  pCmd = command ;
  pEndCmd = pCmd + write_size ;    
  pResult = NULL ;

  dataType = gHyp_data_getDataType ( pData ) ;
  isVector = ( dataType > TYPE_STRING ) ;
  ss = gHyp_data_getSubScript ( pData ) ; 
  context = -1 ;
  count = 0 ;
  while ( (pResult = gHyp_data_nextValue ( pData, 
					   pResult,
					   &context,
					   ss ))) {

    if ( isVector && dataType <= TYPE_ATTR ) {
      valueLen = gHyp_data_bufferLen( pResult, context ) ;
      valueLen = MIN ( valueLen, VALUE_SIZE ) ;
      pStr = (char*) gHyp_data_buffer ( pResult, context ) ;
      context+=valueLen ;
    }
    else {
      valueLen = gHyp_data_getStr ( pResult, 
				    gzOutBuffer, 
				    MAX_INPUT_LENGTH, 
				    context, 
				    isVector ) ;
      pStr = gzOutBuffer ;
    }

    if ( (pCmd + valueLen) > pEndCmd ) {

      /* Write out what's there and start another buffer */
      *pCmd = '\0' ;	
      cmdLen = pCmd - command ;
      assert ( cmdLen > 0 ) ; 

      if ( guDebugFlags & DEBUG_DIAGNOSTICS )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	  "Writing %d bytes to socket %d",cmdLen, pPort->fd );

      nBytes = gHyp_sock_write ( (SOCKET) pPort->fd, 
			         command, 
			         cmdLen,
			         (short)(pPort->flags&MASK_SOCKET),
			         (OVERLAPPED*) &pPort->overlapped,
			         pPort->pSSL ) ;

      if ( nBytes <= 0 ) {
        gHyp_util_sysError ( "Failed to write data to socket %d, id %d, device %s",
			 pPort->fd,id,pPort->device ) ;
        gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
        gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), (SOCKET) pPort->fd ) ;
        return COND_SILENT ;
      }
      if ( guDebugFlags & DEBUG_DIAGNOSTICS )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	  "Wrote %d bytes to socket %d", nBytes, pPort->fd );
      totalBytes += nBytes ;
      /* Reset buffer */
      pCmd = command ;
      pEndCmd = pCmd + write_size ;
    }

    if ( valueLen > write_size ) {

      /* Break up the buffer into 2 chunks */
      cmdLen = write_size ;
      memcpy ( pCmd, pStr, cmdLen ) ;
      pStr += cmdLen ;
      valueLen -= cmdLen ;;	

      if ( guDebugFlags & DEBUG_DIAGNOSTICS )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	   "Writing %d bytes to socket %d",cmdLen, pPort->fd );

      nBytes = gHyp_sock_write ( (SOCKET) pPort->fd, 
			         command, 
			         cmdLen,
			         (short)(pPort->flags&MASK_SOCKET),
			         (OVERLAPPED*) &pPort->overlapped,
			         pPort->pSSL ) ;

      if ( nBytes <= 0 ) {
        gHyp_util_sysError ( "Failed to write data to socket %d, id %d, device %s",
			 pPort->fd,id,pPort->device ) ;
        gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
        gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), (SOCKET) pPort->fd ) ;
        return COND_SILENT ;
      }
      if ( guDebugFlags & DEBUG_DIAGNOSTICS )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	    "Wrote %d bytes to socket %d", nBytes, pPort->fd );
      totalBytes += nBytes ;
      /* Reset buffer */
      pCmd = command ;
      pEndCmd = pCmd + cmdLen ;
    }

    if ( valueLen > 0 ) {
      /* Add next contents */
      memcpy ( pCmd, pStr, valueLen ) ;
      pCmd += valueLen ;
    }
  }
  if ( context== -2 && ss != -1 ) 
    gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			  "Subscript '%d' is out of bounds",ss) ;
  *pCmd = '\0' ;	
  cmdLen = pCmd - command ;

  if ( cmdLen > 0 ) {

    if ( guDebugFlags & DEBUG_DIAGNOSTICS )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	  "Writing %d bytes to socket %d", cmdLen, pPort->fd );

    nBytes = gHyp_sock_write ( (SOCKET) pPort->fd, 
			     command, 
			     cmdLen,
			     (short)(pPort->flags&MASK_SOCKET),
			     (OVERLAPPED*) &pPort->overlapped,
			     pPort->pSSL ) ;

    if ( nBytes <= 0 ) {
      gHyp_util_sysError ( "Failed to write data to socket %d, id %d, device %s",
			 pPort->fd,id,pPort->device ) ;
      gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, id ) ;
      gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), (SOCKET) pPort->fd ) ;
      return COND_SILENT ;
    }

    if ( guDebugFlags & DEBUG_DIAGNOSTICS )
      gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	  "Wrote %d bytes to socket %d", nBytes, pPort->fd );

    totalBytes += nBytes ;

  }
  if ( guDebugFlags & DEBUG_DIAGNOSTICS )
    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	  "Wrote total of %d bytes to socket %d",totalBytes, pPort->fd );

  return totalBytes ;
}


int gHyp_secs1_rawOutgoingEagain ( sSecs1 *pPort, sInstance *pAI, int millisecondOffset )
{
  /* 
   * Take the next chunk of data out of pEagain and try to write it out to pPort.
   * Send as much data as possible until its done or we get an eagain, in 
   * which case we set a flag and info to attempt the write again later 
   * and we return to the caller.
   * This function will then be invoked again later so that the next piece of pData
   * can be written out and so on.
   *
   * Three return values:
   * > 0  = message written size in bytes.
   * = 0  = no message written, non-fatal case
   * < 0  = fatal error.
   *
   */
  
  sData
    *pData,
    *pPacket,
    *pValue ;

  sBYTE
    *pEagainBuf ;

  int
    eagainBufLen,
    timeout,
    mTimeout,
    ss,
    context,
    cmdLen,
    totalBytes=0,
    nBytes=0 ;

  char
    *pCmd ;

  /* If there is no data to write, then return quietly */
  if ( pPort->pEagain == NULL && pPort->pEagainPartial == NULL ) return COND_SILENT ;

  /* gsCurTime was when we started to wait.
   * It may actually be 1 second more, but we want to know the milliseconds
   * from when it was last updated in gHyp_sock_select.
   * Ie: millisecondOffset is how many milliseconds past gsCurtime it is now.
  .*/

  timeout = millisecondOffset / 1000 ;
  mTimeout = millisecondOffset - ((millisecondOffset/1000)*1000);

  /*gHyp_util_debug("Should we try to write? %d<=%d? and %d<=%d?",
	  pPort->nextEagainTime1, gsCurTime+timeout,
	  pPort->nextEagainTime2, mTimeout ) ;
   */

  /* If we are not ready to write the data then return quietly */ 
  if ( pPort->nextEagainTime1 > gsCurTime+timeout ) return COND_SILENT ; 
  if ( pPort->nextEagainTime2 > mTimeout ) return COND_SILENT ; 

  /* Send as much as the contents pEagain to the socket as possible */
  while ( pPort->pEagain || pPort->pEagainPartial ) {

    if ( pPort->pEagainPartial )
      pData = pPort->pEagainPartial ;
    else
      pData = pPort->pEagain ;

    pValue = NULL ;
    ss = -1 ; 
    context = -1 ;
    while ( (pValue = gHyp_data_nextValue ( pData, 
    					    pValue,
					    &context,
					    ss ))) {

      cmdLen = gHyp_data_bufferLen( pValue, 0 ) ;
      pCmd = (char*) gHyp_data_buffer ( pValue, 0 ) ;

      if ( guDebugFlags & DEBUG_DIAGNOSTICS ) {
	if ( pData==pPort->pEagain )
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	    "Writing %d bytes to socket %d",cmdLen, pPort->fd );
	else
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	    "Retrying write of %d bytes to socket %d",cmdLen, pPort->fd );
      }

      gHyp_sock_doEagain () ;
      /* Note that when we are taking data from pEagainPartial, any
       * SSL info is set to NULL because the data from pEagainPartial
       * would have already been encrypted and we thus want to bypass
       */
      nBytes = gHyp_sock_write (  (SOCKET) pPort->fd, 
				  pCmd, 
				  cmdLen,
				  (short)(pPort->flags&MASK_SOCKET),
				  (OVERLAPPED*) &pPort->overlapped,
				  (pData==pPort->pEagain)?pPort->pSSL:NULL ) ;

      if ( nBytes <= 0 ) {
	gHyp_util_sysError ( "Failed to write data to socket %d, device %s", 
	  pPort->fd,pPort->device ) ;
	gHyp_instance_signalHangup ( pAI, (int) pPort->fd, pPort->modifier, pPort->deviceId ) ;
	gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), (SOCKET) pPort->fd ) ;
	return COND_SILENT ;
      }

      if ( gHyp_sock_isEagain () ) {

        /* We got an eagain.  We will try again later when the socket is ready */
        pPort->nextEagainTime1 = (int)gsCurTime + (pPort->nextEagainInc/1000) ;
        pPort->nextEagainTime2 = (pPort->nextEagainInc - ((pPort->nextEagainInc/1000)*1000) );
        pPort->nextEagainInc *= 2 ;

        /* Whatever we did NOT write we need to save, and if the socket has SSL, 
	 * it will be already encrypted. 
         */
	pEagainBuf = gHyp_sock_eagainBuf() ;
	eagainBufLen = gHyp_sock_eagainBufLen() ;

	/* Save it to the pEagainPartial data area */
        pPacket = gHyp_data_new ( "_packet_" ) ;

        gHyp_data_newVectorSrc ( pPacket, TYPE_BYTE, eagainBufLen, FALSE, pEagainBuf ) ;

        gHyp_data_detach ( pValue ) ;
        gHyp_data_delete ( pValue ) ;
        pValue = NULL ;

	if ( pPort->pEagainPartial == NULL ) 
	  pPort->pEagainPartial = gHyp_data_new ( "_eagain_" ) ;
        else
 	  gHyp_data_deleteValues ( pPort->pEagainPartial ) ;
       
        gHyp_data_append ( pPort->pEagainPartial, pPacket ) ;

        if ( guDebugFlags & DEBUG_DIAGNOSTICS ) {
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
		"Eagain received. Wrote a total of %d bytes to socket %d",totalBytes, pPort->fd );
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
		"Next attempt at %d seconds + %d milliseconds",
		pPort->nextEagainTime1-gsCurTime, pPort->nextEagainTime2 );
	}


        return totalBytes ;
      } 

      /* If we succeed in writing any bytes at all, then reset  Eagain time */
      pPort->nextEagainInc = 64 ; /* Milliseconds */
      pPort->nextEagainTime1 = (int)gsCurTime ;
      pPort->nextEagainTime2 = 0 ;

      totalBytes += nBytes ;

      gHyp_data_detach ( pValue ) ;
      gHyp_data_delete ( pValue ) ;
      pValue = NULL ;
      context = -1 ;
    }

    if ( gHyp_data_getCount ( pData ) == 0 ) {
	   
      /* No more data in either pEagain or pEagainPartial */
      if ( pData == pPort->pEagain ) {
        gHyp_data_delete ( pPort->pEagain ) ;
        pPort->pEagain = NULL ;
      }
      else {
        gHyp_data_delete ( pPort->pEagainPartial ) ;
        pPort->pEagainPartial = NULL ;
      }
    }

  }

  if ( guDebugFlags & DEBUG_DIAGNOSTICS )
    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	  "Wrote total of %d bytes to socket %d",totalBytes, pPort->fd );

  return totalBytes ;
}

int  gHyp_secs1_rawOutEagainInit ( sSecs1 *pPort, sInstance *pAI, sData *pData, sBYTE objectType )
{
  sLOGICAL
    isVector ;
  
  sBYTE
    dataType ;

  sData
    *pEagain,
    *pResult,
    *pValue;

  int
    write_size,
    ss,
    cmdLen,
    context,
    valueLen,
    totalBytes=0;

  char
    command[HTTP_WRITE_SIZE+1],
    *pCmd,
    *pEndCmd,
    *pStr,
    value[VALUE_SIZE+1] ;

  write_size = (objectType == DATA_OBJECT_HTTP)? HTTP_WRITE_SIZE : PORT_WRITE_SIZE ;

  if ( pPort->pEagain == NULL ) pPort->pEagain = gHyp_data_new ( "_eagain_" ) ;

  /* Construct the message to be sent to the port. */
  pEagain = pPort->pEagain ;
  pPort->nextEagainInc = 64  ; /* Milliseconds */ 
  pPort->nextEagainTime1 = (int)gsCurTime ;
  pPort->nextEagainTime2 = 0 ;

  pCmd = command ;
  pEndCmd = pCmd + write_size ;    
  pResult = NULL ;
  ss = -1 ; 
  context = -1 ;
  while ( (pResult = gHyp_data_nextValue ( pData, 
					   pResult,
					   &context,
					   ss ))) {

    dataType = gHyp_data_getDataType ( pResult ) ;
    isVector = ( dataType > TYPE_STRING ) ;

    if ( isVector && dataType <= TYPE_ATTR ) {
      valueLen = gHyp_data_bufferLen( pResult, context ) ;
      valueLen = MIN ( valueLen, VALUE_SIZE ) ;
      pStr = (char*) gHyp_data_buffer ( pResult, context ) ;
      context+=valueLen ;
    }
    else {
      valueLen = gHyp_data_getStr ( pResult, 
				  value, 
				  VALUE_SIZE, 
				  context, 
				  isVector ) ;
      pStr = value ;
    }

    if ( (pCmd + valueLen) > pEndCmd ) {

      /* Next piece doesn't fit.  Create buffer of what we already have to write out */
      *pCmd = '\0' ;	
      cmdLen = pCmd - command ;
      pValue = gHyp_data_new ( "_packet_" ) ;
      gHyp_data_newVectorSrc ( pValue, TYPE_BYTE, cmdLen, FALSE, command ) ;
      gHyp_data_append ( pEagain, pValue ) ;

      /* Reset buffer */
      pCmd = command ;
      pEndCmd = pCmd + write_size ;

      totalBytes += cmdLen ;	
    }

    /* Add next contents */
    memcpy ( pCmd, pStr, valueLen ) ;
    pCmd += valueLen ;

  }
  if ( context== -2 && ss != -1 ) 
    gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			  "Subscript '%d' is out of bounds",ss) ;
  *pCmd = '\0' ;	
  cmdLen = pCmd - command ;

  if ( cmdLen > 0 ) {

    /* The remainder, if any */

    totalBytes += cmdLen ;

    pValue = gHyp_data_new ( "_packet_" ) ;
    gHyp_data_newVectorSrc ( pValue, TYPE_BYTE, cmdLen, FALSE, command ) ;
    gHyp_data_append ( pEagain, pValue ) ;

  }

  if ( guDebugFlags & DEBUG_DIAGNOSTICS )
    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	  "Prepared a total of %d bytes for socket %d",totalBytes, pPort->fd );

  return totalBytes ;
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

sHTTP *gHyp_secs1_getHttp ( sSecs1 *pSecs1 ) 
{
  return pSecs1->pHTTP ;
}

void gHyp_secs1_setHttp ( sSecs1 *pSecs1, sHTTP* pHTTP ) 
{
  pSecs1->pHTTP = pHTTP;
}

void gHyp_secs1_setSSL ( sSecs1 *pSecs1, sSSL *pSSL )
{
  pSecs1->pSSL = pSSL ;
}

sSSL *gHyp_secs1_getSSL ( sSecs1 *pSecs1 )
{
  return pSecs1->pSSL ;
}

sLOGICAL gHyp_secs1_doEagain ( sSecs1 *pSecs1 )
{
  return pSecs1->doEagain ;
}

void gHyp_secs1_setEagain ( sSecs1 *pSecs1, sLOGICAL doEagain )
{
  pSecs1->doEagain = doEagain ;
}

int gHyp_secs1_eagainTimeout ( sSecs1 *pSecs1, int timeout ) 
{
  /* Return the number of milliseconds until we want to write again.
   *
   * The caller is telling us that it currently has a timeout
   * of 'timeout' seconds from the gsCurTime until it will look
   * at events.  
   *
   * If the timeout value of the eagain socket is less, we must
   * return the number of milliseconds until such a time.
   */

  int mTimeout ;
  int maxTimeout = timeout * 1000 ;

  if ( pSecs1->pEagain == NULL ) return maxTimeout ;

  if ( pSecs1->nextEagainTime1 < gsCurTime ) {
    pSecs1->nextEagainTime1 = (int)gsCurTime ;
  }

  /*gHyp_util_debug("Testing, Is %d > %d:%d",gsCurTime,pSecs1->nextEagainTime1,pSecs1->nextEagainTime2 ) ;*/

  if ( pSecs1->nextEagainTime1 > gsCurTime+timeout ) return maxTimeout ;

  mTimeout = (pSecs1->nextEagainTime1-(int)gsCurTime)*1000 + pSecs1->nextEagainTime2 ;
  /*gHyp_util_debug("Yes, returning %d",mTimeout) ;*/

  return mTimeout ;
}


sData *gHyp_secs1_getForwardPorts ( sSecs1 *pSecs1 ) 
{
  return pSecs1->forwardPorts ;
}

void gHyp_secs1_setForwardPorts ( sSecs1 *pSecs1, sData* pfp ) 
{
  pSecs1->forwardPorts = pfp ;
}

void gHyp_secs1_setBinary( sSecs1* pSecs1, sLOGICAL isBinary )
{
  pSecs1->isBinary = isBinary ;
}

sLOGICAL gHyp_secs1_isBinary( sSecs1* pSecs1 )
{
  return pSecs1->isBinary ;
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
