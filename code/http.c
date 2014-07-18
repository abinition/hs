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
 * $Log: http.c,v $
 * Revision 1.4  2004/10/16 04:45:41  bergsma
 * Added http_binary() function.
 *
 * Revision 1.3  2004/07/23 18:43:14  bergsma
 * Fixed compile warnings about 'id' being an (sWORD).
 *
 * Revision 1.2  2004/06/13 14:06:51  bergsma
 * Recognize plain/text as flag.
 *
 * Revision 1.1  2004/04/29 02:33:56  bergsma
 * New http functions.
 *
 *
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

#include <sys/stat.h>	/* file attributes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

struct http_t {

  sBYTE	  state ;
  char	  arg1[VALUE_SIZE+1] ;
  char	  arg2[VALUE_SIZE+1] ;
  char	  arg3[VALUE_SIZE+1] ;
  sData*  pAttributeData ;
  int	  contentLength ;
  int	  actualContentLength ;
  sData*  pContentData ;
  sLOGICAL isChunkTransferEncoded ;
  sLOGICAL isXMLEncoded ;
  sLOGICAL isURLEncoded ;
  sLOGICAL isPlainText ;
  sLOGICAL isBinary ;
} ;

/**********************	FUNCTION DEFINITIONS ********************************/

sHTTP *gHyp_http_new ( )
{
  sHTTP *pHTTP = (sHTTP*) AllocMemory ( sizeof ( sHTTP ) ) ;
  assert ( pHTTP ) ;

  pHTTP->contentLength = 0 ;
  pHTTP->actualContentLength = 0 ;
  pHTTP->pAttributeData = gHyp_data_new("_attr_" );
  pHTTP->pContentData = gHyp_data_new("_content_");
  pHTTP->state = HTTP_EXPECT_HEADER ;
  pHTTP->isChunkTransferEncoded = FALSE ;
  pHTTP->isXMLEncoded = FALSE ;
  pHTTP->isURLEncoded = FALSE ;
  pHTTP->isPlainText = FALSE ;
  pHTTP->isBinary = FALSE ;

  return pHTTP ;
}

void gHyp_http_delete ( sHTTP *pHTTP )
{
  if ( pHTTP->pAttributeData ) gHyp_data_delete ( pHTTP->pAttributeData ) ;
  if ( pHTTP->pContentData ) gHyp_data_delete ( pHTTP->pContentData ) ;
  ReleaseMemory ( pHTTP ) ;
}


void gHyp_http_reset ( sHTTP *pHTTP )
{
  pHTTP->contentLength = 0 ;
  pHTTP->actualContentLength = 0 ;
  pHTTP->state = HTTP_EXPECT_HEADER ;
  pHTTP->isChunkTransferEncoded = FALSE ;
  pHTTP->isXMLEncoded = FALSE ;
  pHTTP->isURLEncoded = FALSE ;
  pHTTP->isPlainText = FALSE ;
  pHTTP->isBinary = FALSE ;
  gHyp_data_deleteValues ( pHTTP->pContentData ) ;
  gHyp_data_deleteValues ( pHTTP->pAttributeData ) ;
}

char *gHyp_http_getArg1 ( sHTTP *pHTTP ) 
{
  return pHTTP->arg1 ;
}

char *gHyp_http_getArg2 ( sHTTP *pHTTP ) 
{
  return pHTTP->arg2 ;
}

char *gHyp_http_getArg3 ( sHTTP *pHTTP ) 
{
  return pHTTP->arg3 ;
}

sBYTE gHyp_http_getState ( sHTTP *pHTTP ) 
{
  return pHTTP->state ;
}

void gHyp_http_setState ( sHTTP *pHTTP, sBYTE state ) 
{
  pHTTP->state = state ;
}

sData* gHyp_http_getAttributeData( sHTTP* pHTTP )
{
  return pHTTP->pAttributeData ;
}

sData* gHyp_http_getContentData ( sHTTP* pHTTP ) 
{
  return pHTTP->pContentData ;
}

int gHyp_http_getActualContentLength( sHTTP* pHTTP )
{
  return pHTTP->actualContentLength ;
}

int gHyp_http_getContentLength( sHTTP* pHTTP )
{
  return pHTTP->contentLength ;
}

void gHyp_http_updateContent ( sHTTP* pHTTP, 
			       int contentLength,
			       int actualContentLength )
{
  pHTTP->actualContentLength = actualContentLength ;
  pHTTP->contentLength = contentLength ;
}

void gHyp_http_setHeader ( sHTTP* pHTTP, 
			   char* pArg1, 
			   char* pArg2, 
			   char* pArg3 ) 
{
  strcpy ( pHTTP->arg1, pArg1 ) ;
  strcpy ( pHTTP->arg2, pArg2 ) ;
  strcpy ( pHTTP->arg3, pArg3 ) ;
}

sLOGICAL gHyp_http_isReply ( sHTTP *pHTTP ) 
{
  return ( strstr( pHTTP->arg1, "HTTP/1" ) == pHTTP->arg1 ) ;
}

void gHyp_http_setChunkedTransferEncoded( sHTTP* pHTTP )
{
  pHTTP->isChunkTransferEncoded = TRUE ;
}

sLOGICAL gHyp_http_isChunkedTransferEncoded( sHTTP* pHTTP )
{
  return pHTTP->isChunkTransferEncoded ;
}

void gHyp_http_setXMLEncoded( sHTTP* pHTTP )
{
  pHTTP->isXMLEncoded = TRUE ;
}

sLOGICAL gHyp_http_isXMLEncoded( sHTTP* pHTTP )
{
  return pHTTP->isXMLEncoded ;
}

void gHyp_http_setURLEncoded( sHTTP* pHTTP )
{
  pHTTP->isURLEncoded = TRUE ;
}

sLOGICAL gHyp_http_isURLEncoded( sHTTP* pHTTP )
{
  return pHTTP->isURLEncoded ;
}

void gHyp_http_setPlainText( sHTTP* pHTTP )
{
  pHTTP->isPlainText = TRUE ;
}

sLOGICAL gHyp_http_isPlainText( sHTTP* pHTTP )
{
  return pHTTP->isPlainText ;
}

void gHyp_http_setBinary( sHTTP* pHTTP, sLOGICAL isBinary )
{
  pHTTP->isBinary = isBinary ;
}

sLOGICAL gHyp_http_isBinary( sHTTP* pHTTP )
{
  return pHTTP->isBinary ;
}

void gHyp_http_service ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_service ()
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
      *pHttpPort ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      localAddr[VALUE_SIZE+1],
      service[VALUE_SIZE+1],
      *pService,
      *pLocalAddr = NULL ;

    sLOGICAL
      bindAll = FALSE;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 && argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: http_service ( name [,localAddr] )");

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
      bindAll = TRUE ; ;


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
      /* Create a device */ 
      pHttpPort = gHyp_secs1_new ( (SOCKET_LISTEN | PROTOCOL_HTTP),
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
				     pHttpPort,
				     DATA_OBJECT_HTTP,
				     (void (*)(void*))gHyp_secs1_delete ) ;
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) socket ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_http_open ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_open ( )
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
      *pHttpPort ;

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
         "Invalid args. Usage http_open( target, modifier )");
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
			    "HTTP port number '%d' not in range 1-%d",
			    port, MAX_PORT_NUMBER ) ;
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
				"Failed to open HTTP port '%s'",
				pDevice ) ;
	status = FALSE ;
      }
      else {
	
	/* Create and open the device */ 
	pHttpPort = gHyp_secs1_new ( (PROTOCOL_HTTP | SOCKET_SERIAL),
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
				       pHttpPort,
				       DATA_OBJECT_HTTP,
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
				STATUS_HTTP,
	    "Failed to connect to TCP service at node '%s' on port %d",
				pDevice,
				port ) ;
	status = FALSE ;
      }
      else {
	
	/* Create and open the service */ 
	pHttpPort = gHyp_secs1_new ( (PROTOCOL_HTTP | SOCKET_TCP),
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
				       pHttpPort,
				       DATA_OBJECT_HTTP,
				       (void (*)(void*))gHyp_secs1_delete ) ;
      }
    }
    
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) portFd ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_http_close ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_close ( )
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
      *pHttpPort ;

    SOCKET
      portFd ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: http_close ( port )");

    /* Get the HTTP port fd */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    portFd = (SOCKET) gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the port structure */
    if ( (pHttpPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							   portFd,
							   DATA_OBJECT_NULL ) ) ) { 
      gHyp_concept_deleteSocketObject ( gHyp_instance_getConcept(pAI), portFd ) ;
    }
    else
      gHyp_instance_warning ( pAI, STATUS_HTTP, "No such socket" ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_http_handle ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_handle ( )
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
      portFd = INVALID_SOCKET ;

    sInstance
      *pAIassigned ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: http_handle ( id )");

    /* Get the http port id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Get the port handle */
    pAIassigned = gHyp_concept_getInstForDeviceId ( 
      gHyp_instance_getConcept ( pAI ), id ) ;

    if ( !pAIassigned )
      gHyp_instance_warning ( pAI, STATUS_HTTP, 
	  "Device id %d is not assigned", id ) ;
    else
      portFd = gHyp_instance_getDeviceFd ( pAIassigned, id ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) portFd ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_http_assign ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_assign ( )
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

    short
      flags ;

    sSecs1
      *pHttp ;

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
      "Invalid args. Usage: http_assign ( id, port )");
    
    /* Get the http port fd */
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
    pHttp = (sSecs1*) gHyp_concept_getSocketObject ( pConcept, (SOCKET) portFd, DATA_OBJECT_NULL ) ;
    if ( !pHttp ) {
      gHyp_instance_warning ( pAI,
			      STATUS_HTTP, 
			      "No port '%d'. Use http_open to open port ",
			      portFd ) ;
    }
    else {
      /* Check to see if the port is already assigned */
      pAIassigned = gHyp_concept_getInstForFd ( pConcept, portFd ) ;
      if ( pAIassigned && pAI != pAIassigned ) {
        gHyp_instance_warning ( pAI,
			      STATUS_HTTP, 
			      "Http %d is already assigned to another device %d by instance %s",
			      portFd,
			      gHyp_instance_getDeviceId ( pAIassigned, portFd ),
			      gHyp_instance_getTargetId ( pAIassigned ) ) ;
      }
      else {
	gHyp_instance_updateFd ( pAI, (SOCKET) portFd, (sWORD) id, NULL, FALSE ) ;
	flags = gHyp_secs1_flags ( pHttp ) ;
	flags = (flags & MASK_SOCKET) | PROTOCOL_HTTP ;
	gHyp_secs1_setFlags ( pHttp, flags ) ;
      }

    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_http_binary ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_binary ( )
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
      *pHttpPort ;

    sHTTP
      *pHttp ;


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
      "Invalid args. Usage: http_binary ( id, boolean )");
    
    /* Get the boolean value */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    isBinary = gHyp_data_getBool ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( id < 0 || id > 65535 )
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

    /* Get the http structure */
    pAIassigned = gHyp_concept_getInstForDeviceId ( gHyp_instance_getConcept ( pAI ), (sWORD) id ) ;

    if ( !pAIassigned ) {
      gHyp_instance_warning ( pAI,STATUS_HTTP, 
			    "Device id %d is not assigned",
			    id ) ;
      status = FALSE ;
    }

    if ( status ) {

      portFd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
      if ( portFd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,STATUS_HTTP, 
			    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      pHttpPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							(SOCKET) portFd, 
							DATA_OBJECT_NULL ) ;
      if ( !pHttpPort ) {
	gHyp_instance_warning ( pAI,STATUS_HTTP, 
	  		    "Socket %d does not exist.",
			    portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      pHttp = gHyp_secs1_getHttp ( pHttpPort ) ; 
      if ( !pHttp ) {
        gHyp_instance_warning ( pAI,
			      STATUS_HTTP, 
			      "No port '%d'. Use http_open to open port ", portFd ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      gHyp_http_setBinary ( pHttp, isBinary ) ;
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_http_enable ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_enable ( )
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
      "Invalid args. Usage: http_enable ( method, id )");

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

void gHyp_http_disable ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_disable ( )
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
      id,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      *pMethodStr ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: http_disable ( method, id )");

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


static void lHyp_http_QE (	sInstance 	*pAI,
				sFrame		*pFrame,
				sStack		*pStack,
				sParse		*pParse,
				sCode		*pCode,
				int		argCount,
				sBYTE		mode )
{
  /* Description:
   *
   *	Services the http_query() and http_event() functions.
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
    *pHttpList,
    *pData,
    *pValue,
    *pAttrData,
    *pAttr,
    *pHttpData ;

  char
    sender[SENDER_SIZE+1] ;

  sLOGICAL
    firstValue,
    hasContentLength,
    hasUserAgent,
    hasHost,
    hasDate,
    hasContentType,
    isVector,
    isVector1,
    isVector2,
    status=TRUE;

  int
    id,
    n,
    lineLen,
    code=0,
    contentLength=0,
    context,
    context1,
    context2,
    ss,
    ss1,
    ss2,
    nBytes ;

  sBYTE
    tokenType,
    dataType ;

  SOCKET
    fd ;
  
  sSecs1
    *pHttpPort =NULL ;

  time_t
    eventTime ;

  sInstance
    *pAIassigned ;


  char
    line[VALUE_SIZE+3],	    /* Extra 2 chars for \r\n */
    value[VALUE_SIZE+1],
    value2[VALUE_SIZE+3],  /* Extra 2 chars for \r\n */
    arg3[VALUE_SIZE+1],
    arg2[VALUE_SIZE+1],
    *pAttribute,
    *pArg3=NULL,
    *pArg2=NULL ;

  if ( argCount == 5 )
    /* Get http data */
    pHttpData = gHyp_stack_popRdata ( pStack, pAI ) ;
  else
    pHttpData = NULL ;

  if ( argCount >= 4 ) 
    /* Get attribute data */
    pAttrData = gHyp_stack_popRdata ( pStack, pAI ) ;
  else
    pAttrData = NULL ;
  
  /* Get the target directory (or text string for http_reply) */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  n = gHyp_data_getStr ( pData,
			 arg3,
			 VALUE_SIZE,
			 gHyp_data_getSubScript ( pData ),
			 TRUE ) ;
  pArg3 = arg3  ; 

  /* Get the http verb (or status value for http_reply) */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  if ( mode == MESSAGE_REPLY )
    code = gHyp_data_getInt ( pData, 
			      gHyp_data_getSubScript ( pData ), 
			      TRUE ) ;
  else {
    n = gHyp_data_getStr (  pData,
			    arg2,
 			    VALUE_SIZE,
			    gHyp_data_getSubScript ( pData ),
			    TRUE ) ;
    pArg2 = arg2  ; 
  }

  /* Get id */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;
  /* Check id */
  if ( id < 0 || id > 65535 )
    gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;

  /* Get the http structure */
  pAIassigned = gHyp_concept_getInstForDeviceId ( gHyp_instance_getConcept ( pAI ), (sWORD) id ) ;

  if ( !pAIassigned ) {
    gHyp_instance_warning ( pAI,STATUS_HTTP, 
			    "Device id %d is not assigned",
			    id ) ;
    status = FALSE ;
  }

  if ( status ) {

    fd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
    if ( fd == INVALID_SOCKET ) {
      gHyp_instance_warning ( pAI,STATUS_HTTP, 
			    "Socket %d does not exist.",
			    fd ) ;
      status = FALSE ;
    }
  }

  if ( status ) {
    pHttpPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							(SOCKET) fd, 
							DATA_OBJECT_NULL ) ;
    if ( !pHttpPort ) {
      gHyp_instance_warning ( pAI,STATUS_HTTP, 
			    "Socket %d does not exist.",
			    fd ) ;
      status = FALSE ;
    }
  }

  if ( status ) {
    if ( (gHyp_secs1_flags(pHttpPort) & SOCKET_LISTEN) ) {
      gHyp_instance_warning ( pAI,STATUS_HTTP, 
			    "Device %d has not been connected.",
			    id ) ;
      status = FALSE ;
    }
  }

  if ( status ) {

    /* Create the HTTP message */
    pHttpList = gHyp_data_new ( "_http_" ) ;

    /*
    pHttp = gHyp_secs1_getHttp ( pHttpPort ) ;
    gHyp_http_reset ( pHTTP, HTTP_EXPECT_HEADER ) ;
    */

    hasContentLength = FALSE ;
    hasContentType = FALSE ;
    hasUserAgent = FALSE ;
    hasHost = FALSE ;
    hasDate = FALSE ;

    /* Loop through attribute list */
    if ( pAttrData ) {

      pAttr = NULL ;
      isVector1 = (gHyp_data_getDataType(pAttrData) > TYPE_STRING ) ;
      ss1 = gHyp_data_getSubScript ( pAttrData ) ; 
      context1 = -1 ;

      while ( (pAttr = gHyp_data_nextValue( pAttrData, 
		 			    pAttr, 
					    &context1,
					    ss1 ) ) ) {

        tokenType = gHyp_data_getTokenType ( pAttr ) ;
        dataType = gHyp_data_getDataType ( pAttr ) ;

        if ( tokenType != TOKEN_VARIABLE  )
	  gHyp_instance_warning ( pAI, STATUS_HTTP, "'%s' is not a variable",
				gHyp_data_getLabel(pAttr) ) ;
        else {

	  pAttribute = gHyp_data_getLabel ( pAttr ) ;

	  if ( !hasContentLength ) 
	    hasContentLength = ( strcmp ( pAttribute, "Content-Length" ) == 0 ) ;
	  if ( !hasContentType ) 
	    hasContentType = ( strcmp ( pAttribute, "Content-Type" ) == 0 ) ;
	  if ( !hasUserAgent ) 
	    hasUserAgent = ( strcmp ( pAttribute, "User-Agent" ) == 0 ) ;
	  if ( !hasHost ) 
	    hasHost = ( strcmp ( pAttribute, "Host" ) == 0 ) ;
	  if ( !hasDate ) 
	    hasDate = ( strcmp ( pAttribute, "Date" ) == 0 ) ;

	  lineLen = sprintf ( line, "%s: ", pAttribute ) ;
	  pValue = NULL ;
	  isVector2 = ( gHyp_data_getDataType( pAttr ) > TYPE_STRING ) ;
	  context2 = 0 ;
	  ss2 = -1 ;
	  firstValue = TRUE ;
	  value[0] = '\0' ;

          while ( (pValue = gHyp_data_nextValue ( pAttr, 
					          pValue, 
					          &context2,
					          ss2 ))) {

	    n = gHyp_data_getStr ( pValue, 
				   value, 
				   INTERNAL_VALUE_SIZE, 
				   context2, 
				   isVector2 ) ;
	    if ( n > 0 ) {

	      if ( !firstValue ) {
		line[lineLen++] = ',' ;
		line[lineLen++] = '\0' ;
	      }
	      firstValue = FALSE ;

	      /*
 	      n = gHyp_util_unparseString ( value2, 
	     				    value, 
					    n, 
					    VALUE_SIZE, 
					    FALSE, 
					    FALSE ) ;
	      */
	      strncpy ( value2, value, n ) ;
	      value2[n++] = '\0' ;

	      if ( lineLen + n > VALUE_SIZE ) {

		/* Write out what's there and start a new line */
		line[lineLen++] = '\r' ;
		line[lineLen++] = '\n' ;
		line[lineLen] = '\0' ;
		pValue = gHyp_data_new ( NULL ) ;
		gHyp_data_setStr_n ( pValue, line, lineLen ) ;
		gHyp_data_append ( pHttpList, pValue ) ;

		line[0] = '\0' ;
		lineLen = 0 ;
	      }

	      /* Add new value to line */
	      strcat ( line, value2 ) ;	
	      lineLen = strlen ( line ) ;
	    }
	  }

	  /* Add the remainder */
	  line[lineLen++] = '\r' ;
	  line[lineLen++] = '\n' ;
	  line[lineLen] = '\0' ;

          pValue = gHyp_data_new ( NULL ) ;
	  gHyp_data_setStr_n ( pValue, line, lineLen ) ;
          gHyp_data_append ( pHttpList, pValue ) ;
	}
      }
      if ( context1 == -2 && ss1 != -1 ) 
        gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			  "   Subscript '%d' is out of bounds",ss1) ;

      /* End of attributes.  Should there be a content length? */
    }

    if ( pHttpData ) {

      /*
      pVariable = gHyp_data_getVariable ( pHttpData ) ;
      contentLength = 0 ;

      if ( pVariable ) 
	pData = gHyp_fileio_describeData ( pAI, pVariable, ' ', TRUE, &contentLength ) ;
      else
	pData = gHyp_fileio_describeData ( pAI, pHttpData, ' ', TRUE, &contentLength ) ;			       
      */

      /* Determine the content length of the data */
      pValue = NULL ;
      isVector = ( gHyp_data_getDataType ( pHttpData ) > TYPE_STRING ) ;
      ss = gHyp_data_getSubScript ( pHttpData ) ; 
      context = -1 ;
      contentLength=0;
      while ( (pValue = gHyp_data_nextValue ( pHttpData, 
  					      pValue,
					      &context,
					      ss ))) {
	n = gHyp_data_getStr ( pValue, 
			       value, 
			       VALUE_SIZE, 
			       context, 
			       isVector ) ;
        contentLength += n ;
      }

    }
    /* Add required headers.*/
    if ( !hasUserAgent) {
      pValue = gHyp_data_new ( NULL ) ;
      n = sprintf ( line, "User-Agent: HyperScript/%s\r\n", 
		      VERSION_HYPERSCRIPT ) ;
      gHyp_data_setStr_n ( pValue, line, n ) ;
      gHyp_data_insert ( pHttpList, pValue ) ;
    }

    if ( !hasHost) {
      pValue = gHyp_data_new ( NULL ) ;
      n = sprintf ( line, "Host: %s:%d\r\n",
		      gHyp_secs1_device(pHttpPort),
		      gHyp_secs1_port(pHttpPort));
      gHyp_data_setStr_n ( pValue, line, n ) ;
      gHyp_data_insert ( pHttpList, pValue ) ;
    }

    if ( !hasDate) {
      strcpy ( value, asctime(localtime(&gsCurTime)) );
      gHyp_util_trim ( value ) ;
      pValue = gHyp_data_new ( NULL ) ;
      n = sprintf ( line, "Date: %s\r\n", value );
      gHyp_data_setStr_n ( pValue, line, n ) ;
      gHyp_data_insert ( pHttpList, pValue ) ;
    }

    if ( !hasContentLength && contentLength > 0) {
      pValue = gHyp_data_new ( NULL ) ;
      n = sprintf ( line, "Content-Length: %d\r\n", contentLength ) ;
      gHyp_data_setStr_n ( pValue, line, n ) ;
      gHyp_data_append ( pHttpList, pValue ) ;
    }

    if ( contentLength > 0 ) {
      /* Add double line */
      pValue = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr_n ( pValue, "\r\n", 2 ) ;
      gHyp_data_append ( pHttpList, pValue ) ;

      /* Add HTTP data */
      gHyp_data_moveValues ( pHttpList, pHttpData ) ;
    }
    else {
      /* Add double line */
      pValue = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr_n ( pValue, "\r\n", 2 ) ;
      gHyp_data_append ( pHttpList, pValue ) ;
    }

    /* Add first line */
    pValue = gHyp_data_new ( NULL ) ;
    if ( mode == MESSAGE_REPLY )
      n = sprintf ( line, "HTTP/1.1 %d %s\r\n", code, pArg3 ) ;
    else
      n = sprintf ( line, "%s %s HTTP/1.1\r\n", pArg2, pArg3 ) ;

    gHyp_data_setStr_n ( pValue, line, n ) ;
    gHyp_data_insert ( pHttpList, pValue ) ;


    nBytes = gHyp_secs1_rawOutgoing ( pHttpPort, pAI, pHttpList, id ) ;
    gHyp_secs1_setState ( pHttpPort, mode ) ;
    gHyp_data_delete ( pHttpList ) ;
    if ( nBytes <= 0 ) status = FALSE ;
  }
  
  if ( !status )
    
    gHyp_instance_warning ( pAI, STATUS_HTTP, 
			    "Failed to send HTTP message" ) ;
  
  else if ( mode == MESSAGE_QUERY ) {
    
    /* Wait for reply message from query */
    gHyp_instance_incIncomingDepth ( pAI ) ;
    gHyp_instance_setTimeOut ( pAI ) ;
    eventTime = gHyp_instance_getTimeOutTime ( pAI ) ;
    sprintf ( sender, "%d#http%s", id, gzRoot ) ;  
    gHyp_instance_setExpectedReply ( pAI, 
				     sender, 
				     "DATA", 
				     "00000001" ) ;

    gHyp_instance_setState ( pAI, STATE_QUERY ) ;
    gHyp_frame_setState ( pFrame, STATE_QUERY ) ;
    gHyp_frame_setHypIndex ( pFrame, gHyp_frame_getHypIndex(pFrame) - 1 ) ;
    gHyp_parse_restoreExprRank ( pParse ) ;
    gHyp_util_logInfo ( "...waiting for  reply, timeout in %d seconds", 
			(eventTime-gsCurTime)) ;

    longjmp ( gsJmpStack[giJmpLevel=1], COND_SILENT) ;
  }

  return ;
}

void gHyp_http_event ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
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

    if ( argCount != 5 && argCount != 4 && argCount != 3 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: http_event ( id, cmd, verb [,attributes [,data]] )");

    lHyp_http_QE ( pAI, pFrame, pStack, pParse, pCode, argCount, MESSAGE_EVENT ) ;
    
    /* Result is in status variable */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }
  return ;
}

void gHyp_http_query ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_query()
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

    if ( argCount != 5 && argCount != 4 && argCount != 3 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	   "Invalid arguments. Usage: http_query ( id, cmd, target [,attributes [,data]] )");
    
    lHyp_http_QE ( pAI, pFrame, pStack, pParse, pCode, argCount, MESSAGE_QUERY ) ;
    
    /* Result from query is in STATUS variable. */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
  return ;
}

void gHyp_http_reply ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: http_reply()
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

    if ( argCount != 5 && argCount != 4 && argCount != 3 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	   "Invalid arguments. Usage: http_reply ( id, status, text, [,attributes [,data]] )");
    
    lHyp_http_QE ( pAI, pFrame, pStack, pParse, pCode, argCount, MESSAGE_REPLY ) ;
    
    /* Result from query is in STATUS variable. */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
  return ;
}
