/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 2002 Abinition (TM), Inc                      !
!          ***** Program property of Abinition, Inc                           !
!          ***** All rights reserved - Licensed Material.                     !
!
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** Program property of Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!                                                                             !
!*****************************************************************************/

/*
 *  This program is dual-licensed: either;
 *
 *  Under the terms of the GNU General Public License version 3 as 
 *  published by the Free Software Foundation. For the terms of this 
 *  license, see licenses/gplv3.md or <http://www.gnu.org/licenses/>;
 *
 *  Under the terms of the Commercial License as set out in 
 *  licenses/commercial.md
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License or Commerical License for more details.
 */

/*
 * Modifications:
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

        if ( pAIassigned && pAIassigned != pAI ) {
          gHyp_util_logWarning ( "Device id %d was previously assigned to port %d by instance %s, deleting old assignment",
			      (sWORD) id,
			      gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ),
			      gHyp_instance_getTargetId ( pAIassigned ) ) ;
	  gHyp_instance_deleteFd ( pAIassigned, gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ) ;
	}
        /*
        gHyp_util_logInfo ( "Device id %d is assigned to port %d for instance %s",
			      (sWORD) id,
			      gHyp_instance_getDeviceFd ( pAI, (sWORD) id ),
			      gHyp_instance_getTargetId ( pAI ) ) ;
        */
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
  else if ( mode == MESSAGE_QUERY && pAI != pAIassigned ) {
    gHyp_instance_warning ( pAI,STATUS_HTTP, 
			    "Device id %d is assigned to %s, not %s",
			    id,
                            gHyp_instance_getTargetId(pAIassigned), 
                            gHyp_instance_getTargetId(pAI)) ;
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
    /* Set index back one to be at the right spot for gHyp_parse_completeExpression */ 
    gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;
    gHyp_parse_restoreExprRank ( pParse ) ;
    gHyp_util_logInfo ( "...waiting for reply, timeout in %d seconds", 
			(eventTime-gsCurTime)) ;

    if ( guDebugFlags & DEBUG_FRAME )
        gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_FRAME, 
			   "frame: QUERY PORT (longjmp to %d from frame %d)",
			   giJmpRootLevel,gHyp_frame_depth(pFrame) ) ;
    longjmp ( gsJmpStack[giJmpLevel=giJmpRootLevel], COND_SILENT) ;
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
