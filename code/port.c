/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/

/* Modifications:
 * 
 * $Log: port.c,v $
 * Revision 1.41  2009-08-11 21:19:07  bergsma
 * Fixed for ANSI C compatibility - some function names were too long....
 *
 * Revision 1.40  2009-06-23 23:21:12  bergsma
 * HS 3.8.6 PF Milestone
 *
 * Revision 1.39  2009-06-14 13:01:43  bergsma
 * Post HS_385 Fixes - some functions such as port_binary, port_eagain,
 * were not actually working (enabled).
 *
 * Revision 1.38  2009-06-12 05:11:15  bergsma
 * HS 385 TAGGING - Added port_stop and port_go (but disabled in sock.c)
 *
 * Revision 1.37  2009-04-02 06:35:13  bergsma
 * Port reads and writes are 4K, Http reads and wites are 5K,
 *
 * Revision 1.36  2008-03-05 22:21:10  bergsma
 * Try and get recvmsg and sendmsg working for TRU64
 *
 * Revision 1.35  2008-02-12 23:19:07  bergsma
 * VS 2008 update
 *
 * Revision 1.34  2007-09-25 17:50:19  bergsma
 * Integrate SOCK_UNIX_LISTEN with SOCK_LISTEN
 *
 * Revision 1.33  2007-05-08 01:27:12  bergsma
 * Used deleteFd rather than updateFd when id is reassigned to another
 * instance.
 *
 * Revision 1.32  2007-04-07 17:52:06  bergsma
 * When a device id has already been assigned to another port, report
 * the warning and using updateFd instead of deleteFd. (deleteFd removes
 * all device ids assigned to the port).
 *
 * Revision 1.31  2007-02-26 02:35:01  bergsma
 * No longer user NULL_DEVICEID placeholder.  PORT and HTTP
 * autoallocate device ids, HSMS and SECS I are pre-determined.
 *
 * Revision 1.30  2007-02-17 01:53:13  bergsma
 * Socket handoff does not work with TRUE64
 *
 * Revision 1.29  2007-02-15 03:24:15  bergsma
 * Remove the handoff socket after handoff
 *
 * Revision 1.28  2007-02-13 22:35:50  bergsma
 * typo in comment
 *
 * Revision 1.27  2006/10/27 17:27:19  bergsma
 * Added port_sendmsg and port_recvmsg
 *
 * Revision 1.26  2006/10/11 16:16:00  bergsma
 * Make EAGAIN an optional feature that must be turned on.
 *
 * Revision 1.25  2006/10/01 16:25:54  bergsma
 * Changed rawOutgoing to the new Eagain method.
 *
 * Revision 1.24  2006/09/25 05:05:31  bergsma
 * When assigned a device id to a port which was already assigned to
 * another instance, the previous device assignement must be deleted in
 * addition to the new assignment made.
 *
 * Revision 1.23  2006/04/04 14:58:02  bergsma
 * STATUS was not STATUS_PORT, but STATUS_HTTP.  Fixed now.
 *
 * Revision 1.22  2006/01/23 05:12:02  bergsma
 * Added port_binary() function.
 *
 * Revision 1.21  2006/01/20 04:13:17  bergsma
 * Typedef conversion of id to (sWORD)
 *
 * Revision 1.20  2006/01/19 20:49:49  bergsma
 * int id, not sWORD id
 *
 * Revision 1.19  2006/01/19 20:33:33  bergsma
 * no message
 *
 * Revision 1.18  2006/01/19 19:42:10  bergsma
 * Let port_assign and http_assign override previous assignments.
 *
 * Revision 1.17  2006/01/16 18:56:36  bergsma
 * HS 3.6.6
 * 1. Save query timeout events.  Don't let queries repeat indefinitely.
 * 2. Rework DEBUG_DIAGNOSTIC debugging.  Less overhead.
 *
 * Revision 1.16  2005/11/28 05:10:47  bergsma
 * Comment change
 *
 * Revision 1.15  2005/11/23 15:56:26  bergsma
 * %u#ports instead of #d#ports
 *
 * Revision 1.14  2005/01/10 20:09:15  bergsma
 * Enable signal event jmpOverride during tcp connection request
 *
 * Revision 1.13  2004/10/16 04:48:21  bergsma
 * Allow port function to be used on objects intially create for http.
 *
 * Revision 1.12  2004/07/23 18:43:14  bergsma
 * Fixed compile warnings about 'id' being an (sWORD).
 *
 * Revision 1.11  2004/06/13 14:12:52  bergsma
 * Remember port state after each write.
 *
 * Revision 1.10  2004/04/29 08:35:42  bergsma
 * Added SSL support.
 *
 * Revision 1.9  2003/07/27 00:57:32  bergsma
 * Remove ^M
 *
 * Revision 1.8  2003/07/27 00:56:57  bergsma
 * The stat struct is only used if NOT windows
 *
 * Revision 1.7  2003/05/16 05:22:19  bergsma
 * Fix potential memory leak of pPortData in gHyp_port_query
 * Fix potential memory leak of pSecsIIdata in gHyp_secs_query
 * "COM1:" not recognized as TTY in windows in gHyp_port_open.
 *
 * Revision 1.6  2003/03/04 01:47:10  bergsma
 * The pPortData object is popped using popRdata2, thus its cleanup must be
 * done manually.
 *
 * Revision 1.5  2003/01/12 05:08:42  bergsma
 * V3.1.0
 * Incorportate instance_pushSTATUS in all functions
 *
 * Revision 1.4  2002/11/20 20:44:08  bergsma
 * Use gzRoot instead of gzParent.
 *
 * Revision 1.3  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.2  2002/10/23 00:26:51  bergsma
 * Detect zero bytes on write.
 *
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

#include <sys/stat.h>	/* file attributes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/

void gHyp_port_service ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_service ()
   *	Returns 1
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
    
    SOCKET
      socket ;

    sSecs1
      *pPort ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      localAddr[VALUE_SIZE+1],
      service[VALUE_SIZE+1],
      *pService,
      *pLocalAddr=NULL;

    sLOGICAL
      bindAll = FALSE ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 && argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_service ( name [, localAddr ] )");

    if ( argCount == 2 ) {
      /* Get the local address */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData,
			      localAddr,
			    VALUE_SIZE,
			    gHyp_data_getSubScript ( pData ),
			    TRUE ) ;
      pLocalAddr = localAddr ; 

    }
    else
      bindAll = TRUE ;

    /* Get the service name */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
			   service,
			   VALUE_SIZE,
			   gHyp_data_getSubScript ( pData ),
			   TRUE ) ;
    pService = service ;
    socket = gHyp_tcp_make ( pService, pLocalAddr, bindAll ) ;
    if ( socket != INVALID_SOCKET ) {
      /* Create a dummy device */ 
      pPort = gHyp_secs1_new ( (SOCKET_LISTEN | PROTOCOL_NONE),
			       socket,
			       pService,
			       giServicePort, 
			       SECS_DEFAULT_T1,
			       SECS_DEFAULT_T2,
			       SECS_DEFAULT_T4,
			       SECS_DEFAULT_RTY,
			       socket,
			       pAI ) ;

      gHyp_concept_newSocketObject ( gHyp_instance_getConcept(pAI), 
				     socket,
				     pPort,
				     DATA_OBJECT_PORT,
				     (void (*)(void*))gHyp_secs1_delete ) ;
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) socket ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_port_open ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_open ( )
   *	Returns 1
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
      *pResult ;
    
    SOCKET
      portFd=INVALID_SOCKET ;
    int
      n,
      argCount = gHyp_parse_argCount ( pParse ),
      modifier,
      speed=9600,
      port=0 ;

    char
      *pDevice,
      device[HOST_SIZE+1] ;

    sSecs1
      *pPort ;

    sLOGICAL
      status=TRUE,
      isTTY=FALSE,
      isTCP=FALSE ;

#ifndef AS_WINDOWS
    struct stat
      buf ;
#endif

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) {
      gHyp_util_logError ( 
         "Invalid args. Usage port_open( target, modifier )");
    }
    
    /* Get the modifier */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    modifier = gHyp_data_getInt ( pData, 
				  gHyp_data_getSubScript(pData),
				  TRUE);
      
    /* Get the device name */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   device, 
			   HOST_SIZE, 
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pDevice = device ;

    
#ifdef AS_WINDOWS
    if ( !strrchr ( pDevice, ':' ) )
#else
    if ( stat ( pDevice, &buf ) < 0 )
#endif
    {
       
      /* Device is not a file - probably a TCP port */
      port = modifier ;
      
      if ( port < 1 || port > MAX_PORT_NUMBER )
	gHyp_instance_error ( pAI,
			    STATUS_IO,
			    "Port number '%d' not in range 1-%d", 
			    port,
			    MAX_PORT_NUMBER ) ;
      isTCP = TRUE ;
    }
    else {

      /* Device is a file - it should be a tty device */
#ifndef AS_WINDOWS
		if ( !S_ISCHR ( buf.st_mode ) )
	gHyp_instance_error ( pAI,
			      STATUS_IO,
			      "'%s' is not a tty device", pDevice ) ;
#endif
      isTTY = TRUE ;
      
    }


    if ( isTTY ) {
      
      /* Get the baud rate for the TTy */
      speed = modifier ;
      
      if ( speed != 300 &&
	   speed != 1200 &&
	   speed != 4800 &&
	   speed != 9600 &&
	   speed != 19200 )
	gHyp_instance_error ( pAI,
			      STATUS_BOUNDS,
			      "TTY line speed out of range" ) ;
      
      portFd = (SOCKET) gHyp_secs1_open ( pDevice ) ;
      
      if ( portFd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,
				STATUS_PORT,
				"Failed to open PORT port '%s'",
				pDevice ) ;
	status = FALSE ;
      }
      else {
	
	/* Create and open the device */ 
	pPort = gHyp_secs1_new ( (PROTOCOL_NONE | SOCKET_SERIAL),
				 portFd,
				 pDevice,
				 speed, 
				 SECS_DEFAULT_T1, 
				 SECS_DEFAULT_T2,
				 SECS_DEFAULT_T4,
				 SECS_DEFAULT_RTY,
				 INVALID_SOCKET,
				 pAI ) ;
	
	gHyp_concept_newSocketObject ( gHyp_instance_getConcept ( pAI ),
				       portFd,
				       pPort,
				       DATA_OBJECT_PORT,
				       (void (*)(void*))gHyp_secs1_delete ) ;
      }
    }
    else if ( isTCP ) {
      
      /* Device is on terminal server */
      /* Request a connection. */
      giJmpOverride = TRUE ;
      portFd = gHyp_tcp_request ( pDevice, port ) ;
      giJmpOverride = FALSE ;
      
      if ( portFd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,
				STATUS_PORT,
	    "Failed to connect to TCP service at node '%s' on port %d",
				pDevice,
				port ) ;
	status = FALSE ;
      }
      else {
	
	/* Create and open the service */ 
	pPort = gHyp_secs1_new ( (PROTOCOL_NONE | SOCKET_TCP),
				 portFd,
				 pDevice,
				 port, 
				 SECS_DEFAULT_T1,
				 SECS_DEFAULT_T2,
				 SECS_DEFAULT_T4,
				 SECS_DEFAULT_RTY, 
				 INVALID_SOCKET,
				 pAI ) ;
	
	gHyp_concept_newSocketObject ( gHyp_instance_getConcept ( pAI ),
				       portFd,
				       pPort,
				       DATA_OBJECT_PORT,
				       (void (*)(void*))gHyp_secs1_delete ) ;
      }
    }
    
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) portFd ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_port_close ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_close ( )
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
    
    sSecs1
      *pPort ;

    SOCKET
      portFd ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_close ( port )");

    /* Get the port port fd */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    portFd = (SOCKET) gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the port structure */
    if ( (pPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							   portFd,
							   DATA_OBJECT_NULL ) ) ) { 
      gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), portFd ) ;
    }
    else
      gHyp_instance_warning ( pAI, STATUS_PORT, "No such socket" ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_port_handle ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_handle ( )
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

    sWORD
      id ;

    SOCKET
      portFd=INVALID_SOCKET ;

    sInstance
      *pAIassigned ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_handle ( id )");

    /* Get the port port id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Get the port handle */
    pAIassigned = gHyp_concept_getInstForDeviceId ( 
      gHyp_instance_getConcept ( pAI ), id ) ;

    if ( !pAIassigned )
      gHyp_instance_warning ( pAI, STATUS_PORT, 
	  "Device id %d is not assigned", id ) ;
    else
      portFd = gHyp_instance_getDeviceFd ( pAIassigned, id ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) portFd ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_port_assign ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_assign ( )
   *	Returns 1
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

    SOCKET
      portFd ;

    sConcept
      *pConcept ;

    sSecs1
      *pPort ;

    int
      id ;

    short
      flags ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    sInstance
      *pAIassigned ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_assign ( id, port )");
    
    /* Get the port port fd */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    portFd = (SOCKET) gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( id < 0 || id > 65535 )
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

    /* Check that portFd already exists */
    pConcept =  gHyp_instance_getConcept(pAI) ;
    pPort = (sSecs1*) gHyp_concept_getSocketObject ( pConcept, (SOCKET) portFd, DATA_OBJECT_NULL ) ;
    if ( !pPort ) {
        gHyp_instance_warning ( pAI,
			      STATUS_PORT, 
			      "No port '%d'. Use port_open to open port ",
			      portFd ) ;
    }
    else {
        /* Check to see if the port is already assigned */
        pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, (sWORD) id ) ;

        if ( pAIassigned ) {
          gHyp_util_logWarning ( "Device id %d was previously assigned to port %d by instance %s, deleting old assignment",
			      (sWORD) id,
			      gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ),
			      gHyp_instance_getTargetId ( pAIassigned ) ) ;
	  gHyp_instance_deleteFd ( pAIassigned, gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ) ;
	}
        
        gHyp_instance_updateFd ( pAI, (SOCKET) portFd, (sWORD) id, NULL, FALSE ) ;

	/* Make sure protocol is NONE */
	flags = gHyp_secs1_flags ( pPort ) ;
	flags = (flags & MASK_SOCKET) | PROTOCOL_NONE ;
	gHyp_secs1_setFlags ( pPort, flags ) ;
  
      }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_port_binary ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_binary ( )
   *	Returns 1
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

    sLOGICAL
      status=FALSE,
      isBinary = FALSE ;

    SOCKET
      portFd ;

    sSecs1
      *pPort ;

    int
      id ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    sInstance
      *pAIassigned ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_binary ( id, boolean )");
    
    /* Get the boolean value */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    isBinary = gHyp_data_getBool ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( id < 0 || id > 65535 )
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

    /* Get the port structure */
    pAIassigned = gHyp_concept_getInstForDeviceId ( gHyp_instance_getConcept ( pAI ), (sWORD) id ) ;

    if ( !pAIassigned ) {
      gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Device id %d is not assigned",
			    id ) ;
      status = FALSE ;
    }
    else 
      status = TRUE ;

    if ( status ) {

      portFd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
      if ( portFd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      pPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							(SOCKET) portFd, 
							DATA_OBJECT_NULL ) ;
      if ( !pPort ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      gHyp_secs1_setBinary ( pPort, isBinary ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_port_eagain ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_eagain ( )
   *	Returns 1
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

    sLOGICAL
      status=FALSE,
      doEagain = FALSE ;

    SOCKET
      portFd ;

    sSecs1
      *pPort ;

    int
      id ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    sInstance
      *pAIassigned ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_eagain ( id, boolean )");
    
    /* Get the boolean value */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    doEagain = gHyp_data_getBool ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( id < 0 || id > 65535 )
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

    /* Get the port structure */
    pAIassigned = gHyp_concept_getInstForDeviceId ( gHyp_instance_getConcept ( pAI ), (sWORD) id ) ;

    if ( !pAIassigned ) {
      gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Device id %d is not assigned",
			    id ) ;
      status = FALSE ;
    }
    else 
      status = TRUE ;

    if ( status ) {

      portFd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
      if ( portFd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      pPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							(SOCKET) portFd, 
							DATA_OBJECT_NULL ) ;
      if ( !pPort ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      gHyp_secs1_setEagain ( pPort, doEagain ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_port_enable ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_enable ( )
   *	Returns 1
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
      *pMethodVariable ;
    
    int
      id ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      *pMethodStr ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_enable ( method, id )");

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( id < 0 || id > 65535 )
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;
    
    
      /* Pop the method identifier off of the stack. */
      pData = gHyp_stack_popLvalue ( pStack, pAI ) ;
      pMethodVariable = gHyp_data_getReference ( pData ) ;
      if ( pMethodVariable ) {
        pMethodStr = gHyp_data_getLabel ( pMethodVariable ) ;
        pMethodVariable = gHyp_frame_findMethodVariable ( pFrame, pMethodStr, pAI );
        if ( !pMethodVariable ) { 
	  gHyp_instance_warning ( pAI, STATUS_BRANCH, 
				"Could not find method '%s'", pMethodStr ) ;
	}
        else
	  gHyp_instance_portEnable ( pAI, (sWORD) id, pMethodVariable ) ;
      } 
      else {
        gHyp_instance_warning ( pAI, STATUS_BRANCH, 
			      "Invalid method '%s'",
			       gHyp_data_getLabel ( pData ) ) ;
      }
   
    
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_port_disable ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_disable ( )
   *	Returns 1
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
      *pMethodVariable ;
    
    int
      id ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      *pMethodStr ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_disable ( method, id )");

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( id < 0 || id > 65535 )
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;
    
    
      /* Pop the method identifier off of the stack. */
      pData = gHyp_stack_popLvalue ( pStack, pAI ) ; 
      pMethodVariable = gHyp_data_getReference ( pData ) ;
      if ( pMethodVariable ) {
        pMethodStr = gHyp_data_getLabel ( pMethodVariable ) ;
        pMethodVariable = gHyp_frame_findMethodVariable ( pFrame, pMethodStr, pAI );
        if ( !pMethodVariable ) { 
	  gHyp_instance_warning ( pAI, STATUS_BRANCH, 
				"Could not find method '%s'", pMethodStr ) ;
	}
        else
	  gHyp_instance_portDisable ( pAI, (sWORD) id ) ;
      }
      else {
        gHyp_instance_warning ( pAI, STATUS_BRANCH, 
			      "Invalid method '%s'",
			       gHyp_data_getLabel ( pData ) ) ;
      }
    

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


static void lHyp_port_QE (	sInstance 	*pAI,
				sFrame		*pFrame,
				sStack		*pStack,
				sParse		*pParse,
				sCode		*pCode,
				sBYTE		mode )
{
  /* Description:
   *
   *	Services the port_query() and port_event() functions.
   *	Creates a query or event message and sends it. 
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
  sData
    *pData,
    *pPortData ;

  char
    sender[SENDER_SIZE+1] ;

  sLOGICAL
    status=TRUE;

  int
    id ;

  int
    nBytes ;

  SOCKET
    fd ;
  
  sSecs1
    *pPort=NULL ;

  time_t
    eventTime ;

  sInstance
    *pAIassigned ;

  /* Get argument list */
  pPortData = gHyp_stack_popRdata ( pStack, pAI ) ;
  
  /* Get id */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;
  /* Check id */
  if ( id < 0 || id > 65535 )
    gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

  pAIassigned = gHyp_concept_getInstForDeviceId ( 
    gHyp_instance_getConcept ( pAI ), (sWORD) id ) ;

  if ( !pAIassigned ) {
    gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Device id %d is not assigned",
			    id ) ;
    status = FALSE ;
  }
  else 
    status = TRUE ;

  if ( status ) {

    fd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;

    if ( fd == INVALID_SOCKET ) {
      gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Socket %d does not exist.",
			    fd ) ;
      status = FALSE ;
    }
  }

  if ( status ) {
    pPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
						    (SOCKET) fd, 
						    DATA_OBJECT_NULL ) ;
    if ( !pPort ) {
      gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Socket %d does not exist.",
			    fd ) ;
      status = FALSE ;
    }
  }

  if ( status ) {
    if ( (gHyp_secs1_flags(pPort) & (SOCKET_LISTEN | SOCKET_UNIX_LISTEN)) ) {
      gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Device %d is no longer connected.",
			    id ) ;
      status = FALSE ;
    }
  }

  if ( status ) {  

    if ( gHyp_secs1_doEagain( pPort ) ) {
      /* Non-blocking way */
      nBytes = gHyp_secs1_rawOutEagainInit ( pPort, pAI, pPortData, DATA_OBJECT_PORT ) ;
      nBytes = gHyp_secs1_rawOutgoingEagain ( pPort, pAI, 0 ) ;
    }
    else {
      /* Blocking way */
      nBytes = gHyp_secs1_rawOutgoing( pPort, pAI, pPortData, id, DATA_OBJECT_PORT ) ;
    }

    gHyp_secs1_setState ( pPort, mode ) ;
    if ( nBytes <= 0 ) status = FALSE ;
  }
  
  if ( !status )
    
    gHyp_instance_warning ( pAI, STATUS_PORT, 
			    "Failed to send PORT message" ) ;
  
  else if ( mode == MESSAGE_QUERY ) {
    
    /* Wait for reply message from query */
    gHyp_instance_incIncomingDepth ( pAI ) ;
    gHyp_instance_setTimeOut ( pAI ) ;
    eventTime = gHyp_instance_getTimeOutTime ( pAI ) ;
    sprintf ( sender, "%u#port%s", id, gzRoot ) ;  
    gHyp_instance_setExpectedReply ( pAI, 
				     sender, 
				     "DATA", 
				     "00000001",
				     (int)eventTime ) ;

    gHyp_instance_setState ( pAI, STATE_QUERY ) ;
    gHyp_frame_setState ( pFrame, STATE_QUERY ) ;
    gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;
    gHyp_parse_restoreExprRank ( pParse ) ;
    gHyp_util_logInfo ( "...waiting for reply, timeout in %d seconds", 
			(eventTime-gsCurTime)) ;

    longjmp ( gsJmpStack[giJmpLevel=1], COND_SILENT) ;
  }

  return ;
}

void gHyp_port_event ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: event (sender,method,arglist)
   *	Sets the value of STATUS.
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
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
		
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
			    "Invalid arguments. Usage: port_event ( id, data )");

    lHyp_port_QE ( pAI, pFrame, pStack, pParse, pCode, MESSAGE_EVENT ) ;
    
    /* Result is in status variable */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }
  return ;
}

void gHyp_port_query ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_query()
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
 
    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	   "Invalid arguments. Usage: port_query ( id, data )");
    
    lHyp_port_QE ( pAI, pFrame, pStack, pParse, pCode, MESSAGE_QUERY ) ;
    
    /* Result from query is in STATUS variable. */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
  return ;
}

void gHyp_port_go ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_go ( )
   *	Returns 1
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

    sLOGICAL
      status=FALSE ;

    SOCKET
      portFd ;

    sSecs1
      *pPort ;

    int
      id ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    sInstance
      *pAIassigned ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_go ( id )");

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( id < 0 || id > 65535 )
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

    /* Get the port structure */
    pAIassigned = gHyp_concept_getInstForDeviceId ( gHyp_instance_getConcept ( pAI ), (sWORD) id ) ;

    if ( !pAIassigned ) {
      gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Device id %d is not assigned",
			    id ) ;
      status = FALSE ;
    }
    else 
      status = TRUE ;

    if ( status ) {

      portFd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
      if ( portFd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      pPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							(SOCKET) portFd, 
							DATA_OBJECT_NULL ) ;
      if ( !pPort ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      /*gHyp_util_debug("Setting port GO for socket %d",gHyp_secs1_fd(pPort));*/
      gHyp_concept_setPortStopGo ( gHyp_instance_getConcept(pAI), gHyp_secs1_fd(pPort), TRUE ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


void gHyp_port_stop ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_stop ( )
   *	Returns 1
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

    sLOGICAL
      status=FALSE ;

    SOCKET
      portFd ;

    sSecs1
      *pPort ;

    int
      id ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    sInstance
      *pAIassigned ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_stop ( id )");

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( id < 0 || id > 65535 )
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

    /* Get the port structure */
    pAIassigned = gHyp_concept_getInstForDeviceId ( gHyp_instance_getConcept ( pAI ), (sWORD) id ) ;

    if ( !pAIassigned ) {
      gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Device id %d is not assigned",
			    id ) ;
      status = FALSE ;
    }
    else 
      status = TRUE ;

    if ( status ) {

      portFd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
      if ( portFd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      pPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							(SOCKET) portFd, 
							DATA_OBJECT_NULL ) ;
      if ( !pPort ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      /*gHyp_util_debug("Setting port STOP for socket %d",gHyp_secs1_fd(pPort));*/
      gHyp_concept_setPortStopGo ( gHyp_instance_getConcept(pAI), gHyp_secs1_fd(pPort), FALSE ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


#if defined ( AS_UNIX ) 

void gHyp_port_recvmsg ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_recvmsg ()
   *	Returns 1
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
    
    SOCKET
      socket ;

    sSecs1
      *pPort ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      service[VALUE_SIZE+1],
      *pService ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_recvmsg ( name )");

    /* Get the service name */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
			   service,
			   VALUE_SIZE,
			   gHyp_data_getSubScript ( pData ),
			   TRUE ) ;
    pService = service ;
    socket = gHyp_tcp_makeUNIX ( pService ) ;

    if ( socket != INVALID_SOCKET ) {

      /* Create a dummy device */ 
      pPort = gHyp_secs1_new ( (SOCKET_UNIX_LISTEN | PROTOCOL_NONE ),
			       socket,
			       pService,
			       giServicePort, 
			       SECS_DEFAULT_T1,
			       SECS_DEFAULT_T2,
			       SECS_DEFAULT_T4,
			       SECS_DEFAULT_RTY,
			       socket,
			       pAI ) ;

      gHyp_concept_newSocketObject ( gHyp_instance_getConcept(pAI), 
				     socket,
				     pPort,
				     DATA_OBJECT_PORT,
				     (void (*)(void*))gHyp_secs1_delete ) ;
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) socket ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_port_sendmsg ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: port_sendmsg ()
   *	Returns 1
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
    
    SOCKET
      socket ;

    int
      port,
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sLOGICAL
      status ;

    char
      client[VALUE_SIZE+1],
      *pClient,
      service[VALUE_SIZE+1],
      *pService ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 4 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: port_sendmsg ( client, server, handle, port )");

    /* Get the port number */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    port =  gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the port port fd */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    socket = (SOCKET) gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the service name */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
			   service,
			   VALUE_SIZE,
			   gHyp_data_getSubScript ( pData ),
			   TRUE ) ;
    pService = service ;

    /* Get the client name */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
			   client,
			   VALUE_SIZE,
			   gHyp_data_getSubScript ( pData ),
			   TRUE ) ;
    pClient = client ;

    status = gHyp_tcp_sendmsg ( pClient, pService, socket, port ) ;

    if ( status ) {

      /* Remove the socket */
      gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept ( pAI ), socket ) ;

    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setBool ( pResult, status ) ;
    gHyp_stack_push ( pStack, pResult ) ;

  }
}
#endif
