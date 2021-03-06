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

#include <sys/stat.h>
 
/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/**********************	FUNCTION DEFINITIONS ********************************/
void gHyp_secs_xml(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_xml ( )
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
   *	1
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
      isXML = TRUE ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 0 && argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_xml ( [isXML] )");

    if ( argCount == 1 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      isXML = gHyp_data_getBool ( pData,
	  			gHyp_data_getSubScript ( pData ),
			        TRUE ) ;
    }
    
    gHyp_instance_setSecsXML ( pAI, (sLOGICAL) isXML  ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
} 
void gHyp_secs_mlb(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_mlb ( )
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
   *	1
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

    int
      lengthBytes ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_mlb ( int )");

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    lengthBytes = gHyp_data_getInt ( pData,
				     gHyp_data_getSubScript ( pData ),
				     TRUE ) ;
    
    /* Minimum Length Bytes
     * -1 - default - 0 for S1F1, minumum 1 otherwise
     *	0 - minimum of 0 bytes
     *	1 - minimum of 1 bytes
     *	2 - minimum of 2 bytes
     *	3 - minimum of 3 bytes
     */
 
    if ( lengthBytes < -1 || lengthBytes > 3 ) {
      gHyp_instance_error ( pAI,STATUS_BOUNDS, 
			    "Minumum length bytes not in range: -1,0,1,2,3" ) ;
    }
    else {
      gHyp_instance_setSecsMLB ( pAI, (sBYTE) lengthBytes ) ;
    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
} 

void gHyp_secs_mhp(sInstance *pAI, sCode *pCode, sLOGICAL isPARSE) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_mhp ( )
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
   *	1
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
    
    int
      port ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_mhp ( int )");

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    port = gHyp_data_getInt ( pData,
				     gHyp_data_getSubScript ( pData ),
				     TRUE ) ;
 
    if ( port < 1 || port > MAX_PORT_NUMBER ) {
      gHyp_instance_error ( pAI,STATUS_BOUNDS, 
			    "Minumum hsms port number not in range: 1-%d",
			    MAX_PORT_NUMBER );
    }
    else {
      gHyp_instance_setSecsMHP ( pAI, (sWORD) port ) ;
    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
} 

void gHyp_secs_hsms ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_hsms ( )
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
      passiveSocket ;

    sHsms
      *pHsms ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      localAddr[VALUE_SIZE+1],
      service[VALUE_SIZE+1],
      *pService,
      *pLocalAddr=NULL;

    sLOGICAL
      bindAll=FALSE ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 && argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_hsms ( service [,localAddr] )");

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

    /* Get the /etc/service name (or port number) */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
			   service,
			   VALUE_SIZE,
			   gHyp_data_getSubScript ( pData ),
			   TRUE ) ;
    pService = service ;

    passiveSocket = gHyp_tcp_make ( pService, pLocalAddr, bindAll ) ;

    if ( passiveSocket != INVALID_SOCKET ) {
      pHsms = gHyp_hsms_new ( (SOCKET_LISTEN | PROTOCOL_HSMS),
			      passiveSocket,
			      pService,
			      giServicePort,
			      HSMS_DEFAULT_T5,
			      HSMS_DEFAULT_T6,
			      HSMS_DEFAULT_T7,
			      HSMS_DEFAULT_T8,
			      HSMS_EXPECT_RECV_SELECTREQ,
			      passiveSocket,
			      pAI ) ;

      gHyp_concept_newSocketObject ( gHyp_instance_getConcept(pAI),
				     passiveSocket,
				     (void*) pHsms,
				     DATA_OBJECT_HSMS,
				     (void (*)(void*))gHyp_hsms_delete ) ;
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) passiveSocket ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_secs_service ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_service ()
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
      *pSecs1 ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      localAddr[VALUE_SIZE+1],
      service[VALUE_SIZE+1],
      *pService,
      *pLocalAddr=NULL;

    sLOGICAL
      bindAll=FALSE;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 && argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_service ( service [,localAddr) )");

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


    /* Get the /etc/service name */
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
      pSecs1 = gHyp_secs1_new ( (PROTOCOL_SECS1 | SOCKET_LISTEN),
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
				     pSecs1,
				     DATA_OBJECT_SECS1,
				     (void (*)(void*))gHyp_secs1_delete ) ;
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) socket ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_secs_open ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_open ( )
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
      *pArg[6],
      *pResult ;
    
    int
      n,
      argCount = gHyp_parse_argCount ( pParse ),
      argc,
      rty=3,
      speed=9600,
      port=0 ;

    sWORD
      mhp ;

    SOCKET
      secsFd=INVALID_SOCKET ;

    float
      t1=SECS_DEFAULT_T1,
      t5=HSMS_DEFAULT_T5,
      t2=SECS_DEFAULT_T2,
      t6=HSMS_DEFAULT_T6,
      t4=SECS_DEFAULT_T4,
      t7=HSMS_DEFAULT_T7,
      t8=HSMS_DEFAULT_T8 ;

    char
      *pDevice,
      device[HOST_SIZE+1];

    sSecs1
      *pSecs1 ;

    sHsms
      *pHsms ;

    sLOGICAL
      status=TRUE,
      isRS232=TRUE,
      isSECS1=TRUE,
      isHSMS=FALSE ;

    sConcept
      *pConcept =  gHyp_instance_getConcept(pAI) ;

#ifndef AS_WINDOWS
    struct stat buf ;
#endif

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount < 2 || argCount > 6 ) {
      gHyp_util_logError ( 
         "Invalid args. Usage secs_open( target, modifier, [,options...] )");
    }

    argc = argCount ;
    while ( argc-- ) 
      pArg[argc] = gHyp_stack_popRvalue ( pStack, pAI ) ;
    
    /* Get the device name */
    n = gHyp_data_getStr ( pArg[0], 
			   device, 
			   HOST_SIZE, 
			   gHyp_data_getSubScript(pArg[0]),
			   TRUE ) ;
    pDevice = device ;

#ifdef AS_WINDOWS
    if ( !strrchr ( pDevice, ':' ) )
#else
    if ( stat ( pDevice, &buf ) < 0 )
#endif
    {
      /* Not a comm port.  Assume its a secs1 over a terminal server.  */
      isRS232 = FALSE ;

      /* Get the TCP/IP port number.
       * If the port number is below MHP, its a terminal server port,
       * otherwise its a hsms port.
       */
      mhp = gHyp_instance_getSecsMHP ( pAI ) ;
      port = gHyp_data_getInt ( pArg[1], 
				gHyp_data_getSubScript(pArg[1]),TRUE);
      
      if ( port >= mhp ) { isHSMS = TRUE ; isSECS1 = FALSE ; } 
 
      if ( isHSMS ) {

	if ( argCount > 5 )
	  /* T8: Network Timeout on unresponsive TCP/IP layer */
	  t8 = (float) gHyp_data_getDouble ( pArg[5],
					     gHyp_data_getSubScript(pArg[5]), 
					     TRUE ) ;
	
	if ( argCount > 4 )
	  /* T7: Not Selected Timeout for passive entity to receive SEL.REQ  */
	  t7 = (float) gHyp_data_getDouble ( pArg[4],
					     gHyp_data_getSubScript(pArg[4]),
					     TRUE ) ;
	
	if ( argCount > 3 )	
	  /* T6: Control Transaction Timeout for Select, LinkTest, etc.. */
	  t6 = (float) gHyp_data_getDouble ( pArg[3],
					     gHyp_data_getSubScript(pArg[3]),
					     TRUE ) ;
	
	if ( argCount > 2 )
	  /* T5: Connect Separation Timeout for active entity to recv SEL.RSP*/
	  t5 = (float) gHyp_data_getDouble ( pArg[2],
					     gHyp_data_getSubScript(pArg[2]),
					     TRUE ) ;
	
	if ( t5 < 1 || t5 > 120 ) {
	  status = FALSE ;
	  gHyp_instance_warning ( pAI,
				  STATUS_BOUNDS,
				  "T5 (%g) out of range",
				  t5 ) ;
	}
	
	if ( t6 < 1 || t6 > 120 ) {
	  status = FALSE ;
	  gHyp_instance_warning ( pAI,
				  STATUS_BOUNDS,
				  "T6 (%g) out of range",
				  t6 ) ;
	}
	
	if ( t7 < 1 || t7 > 120 ) {
	  status = FALSE ;
	  gHyp_instance_warning ( pAI,
				  STATUS_BOUNDS, 
				  "T7 (%g) out of range",
				  t7 ) ;
	}
	
	if ( t8 < 1 || t8 > 120 ) {
	  status = FALSE ;
	  gHyp_instance_warning ( pAI,
				  STATUS_BOUNDS, 
				  "T8 (%g) out of range",
				  t8 ) ;
	}
	
	if ( port > 99999 ) {
	  status = FALSE ;
	  gHyp_instance_warning ( pAI,
				  STATUS_BOUNDS,
				  "TCP/IP port (%d) out of range",
				  port ) ;
	}
	gHyp_util_logInfo("T5=%g, T6=%g, T7=%g, T8=%g",
			  t5, t6, t7, t8 ) ;
      }
    }
	else {
#ifndef AS_WINDOWS
	  if ( !S_ISCHR ( buf.st_mode ) ) {
        gHyp_instance_warning ( pAI,
				STATUS_IO,
				"'%s' is not a tty device", pDevice ) ;
        status = FALSE ;
	  }
#endif
	}

    if ( status && isSECS1 ) {
      
      if ( argCount > 5 )
	/* Get the retry value */
	rty = gHyp_data_getInt ( pArg[5], 
				 gHyp_data_getSubScript(pArg[5]),TRUE) ;

      
      if ( argCount > 4 )
	/* Get the t4 timeout value */
	t4 = (float) gHyp_data_getDouble ( pArg[4],
					   gHyp_data_getSubScript(pArg[4]), 
					   TRUE ) ;
      
      if ( argCount > 3 )
	/* Get the t2 timeout value */
	t2 = (float) gHyp_data_getDouble ( pArg[3],
					   gHyp_data_getSubScript(pArg[3]),
					   TRUE ) ;
      
      if ( argCount > 2 )
	/* Get the t1 timeout value */
	t1 = (float) gHyp_data_getDouble ( pArg[2],
					   gHyp_data_getSubScript(pArg[2]),
					   TRUE ) ;


      /* Check the arguments */
      if ( t1 < .1 || t1 > 10 ) {
	status = FALSE ;
	gHyp_instance_warning ( pAI,
				STATUS_BOUNDS,
				"T1 (%g) out of range",
				t1 ) ;
      }
      
      if ( t2 < .2 || t2 > 25 ) {
	status = FALSE ;
	gHyp_instance_warning ( pAI,
				STATUS_BOUNDS,
				"T2 (%g) out of range",
				t2 ) ;
      }
      
      if ( t4 < 1 || t4 > 120 ) {
	status = FALSE ;
	gHyp_instance_warning ( pAI,
				STATUS_BOUNDS, 
				"T4 (%g) out of range",
				t4 ) ;
      }
      
      if ( rty < 0 || rty > 31 ) {
	status = FALSE ;
	gHyp_instance_warning ( pAI,
				STATUS_BOUNDS,
				"RTY (%d) out of range",
				  rty ) ;
      }
      gHyp_util_logInfo("T1=%g, T2=%g, T4=%g, RTY=%d",
			t1, t2, t4, rty ) ;
    }
    
    if ( status ) {

      if ( isRS232 ) {
	      
	/* Get the baud rate for secs1 */
	speed = gHyp_data_getInt ( pArg[1], 
				   gHyp_data_getSubScript(pArg[1]),TRUE);
	
	if ( speed != 300 &&
	     speed != 1200 &&
	     speed != 4800 &&
	     speed != 9600 &&
	     speed != 19200 ) {
	  status = FALSE ;
	  gHyp_instance_warning ( pAI,
				  STATUS_BOUNDS,
				  "Line speed out of range" ) ;
	}
	
	if ( status ) {
	  
	  secsFd = (SOCKET) gHyp_secs1_open ( pDevice ) ;
	  
	  if ( secsFd == INVALID_SOCKET ) {
	    gHyp_instance_warning ( pAI,
				    STATUS_SECS,
				    "Failed to open SECS port '%s'",
				    pDevice ) ;
	    status = FALSE ;
	  }
	  else {
	    
	    /* Create and open the device */ 
	    pSecs1 = gHyp_secs1_new ( (PROTOCOL_SECS1 | SOCKET_SERIAL),
				      secsFd,
				      pDevice,
				      speed,
				      t1,
				      t2,
				      t4,
				      rty,
				      INVALID_SOCKET,
				      pAI ) ;
	    
	    gHyp_concept_newSocketObject ( pConcept, 
					   (SOCKET) secsFd,
					   (void*) pSecs1,
					   DATA_OBJECT_SECS1, 
					   (void (*)(void*))gHyp_secs1_delete ) ;
	  }
	}
      }
      else if ( isHSMS ) {
	
	/* Request a connection */
        giJmpOverride = TRUE ;
	secsFd = gHyp_tcp_request ( pDevice, port ) ;
        giJmpOverride = FALSE ;
	
	if ( secsFd == INVALID_SOCKET ) {
	  gHyp_instance_warning ( pAI,
				  STATUS_SECS,
	    "Failed to connect to hsms service at node '%s' on port %d",
				  pDevice,
				  port ) ;
	  status = FALSE ;
	}
	else {
	  
	  /* Create and open the device */ 
	  pHsms = gHyp_hsms_new ( (SOCKET_TCP|PROTOCOL_HSMS),
				  (SOCKET) secsFd,
				  pDevice,
				  port,
				  t5,
				  t6,
				  t7,
				  t8,
				  HSMS_EXPECT_SEND_SELECTREQ,
				  INVALID_SOCKET,
				  pAI ) ;
	  
	  gHyp_concept_newSocketObject ( pConcept, 
					 (SOCKET) secsFd,
					 (void*) pHsms,
					 DATA_OBJECT_HSMS,
					 (void (*)(void*))gHyp_hsms_delete ) ;

	}
      }
      else {

	/* SECS1 device on terminal server */
	/* Request a connection. */
        giJmpOverride = TRUE ;
	secsFd = gHyp_tcp_request ( pDevice, port ) ;
        giJmpOverride = FALSE ;
	
	if ( secsFd == INVALID_SOCKET ) {
	  gHyp_instance_warning ( pAI,
				  STATUS_SECS,
	    "Failed to connect to terminal server at node '%s' on port %d",
				  pDevice,
				  port ) ;
	  status = FALSE ;
	}
	else {
	  
	  /* Create and open the device */ 
	  pSecs1 = gHyp_secs1_new ( (PROTOCOL_SECS1 | SOCKET_TCP),
				    secsFd,
				    pDevice,
				    port,
				    t1,
				    t2,
				    t4,
				    rty,
				    INVALID_SOCKET,
				    pAI ) ;
	  
	  gHyp_concept_newSocketObject ( pConcept, 
					 (SOCKET) secsFd,
					 (void*) pSecs1,
					 DATA_OBJECT_SECS1,
					 (void (*)(void*))gHyp_secs1_delete ) ;
	}
      }
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) secsFd ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_secs_close ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_close ( )
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
      *pSecs1 ;

    sHsms
      *pHsms ;

    SOCKET
      secsFd ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    sConcept
      *pConcept ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_close ( socket )");

    /* Get the secs port fd */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    secsFd = (SOCKET) gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the SECS I or HSMS structure */
    pConcept =  gHyp_instance_getConcept(pAI) ;
    if ( (pHsms = (sHsms*) gHyp_concept_getSocketObject ( pConcept, 
							  secsFd, 
							  DATA_OBJECT_HSMS ) ) ) { 
      gHyp_concept_deleteSocketObject ( pConcept, secsFd ) ;
				    
    }
    else if ( ( pSecs1 = (sSecs1*) gHyp_concept_getSocketObject ( pConcept, 
								  secsFd, 
								  DATA_OBJECT_SECS1 ) ) ) {
      gHyp_concept_deleteSocketObject ( pConcept, secsFd );
    }
    else {
      gHyp_instance_warning ( pAI,
			      STATUS_SECS,
			      "No such socket %d", secsFd ) ;

    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_secs_assign ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_assign ( )
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
      *pData;
    
    SOCKET
      secsFd ;

    sWORD
      id ;

    short
      flags ;

    int
      ID,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sLOGICAL
      rBit=FALSE,
      status=TRUE ;

    sSecs1
      *pSecs1 ;

    sHsms
      *pHsms ;

    sSecs2
      *pSecs2 ;

    sConcept
      *pConcept ;

    sInstance
      *pAIassigned ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount < 2 || argCount > 3 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_assign ( id, port, [r-bit] )");
   
    if ( argCount > 2  ) {
      /* Is the r-bit set when we send a message down this port?
       * If TRUE, then the destination is a host, up the tree.
       * If FALSE, then the destination is an equipment, down the tree.
       */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      rBit = gHyp_data_getBool ( pData, gHyp_data_getSubScript(pData),TRUE) ;
    } 
    
    /* Get the secs port fd */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    secsFd = (SOCKET) gHyp_data_getHandle( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ID = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( ID < 0 || ID > MAX_SECS_DEVICE_ID )  {
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;
    }
    id = ID ;

    /* Check that secsFd already exists */
    pConcept =  gHyp_instance_getConcept(pAI) ;
    pSecs1 = (sSecs1*) gHyp_concept_getSocketObject ( pConcept, secsFd, DATA_OBJECT_SECS1 ) ;
    pHsms = (sHsms*) gHyp_concept_getSocketObject ( pConcept, secsFd, DATA_OBJECT_HSMS ) ;
    if ( !pSecs1 && !pHsms) {

      status = FALSE ;
      gHyp_instance_warning ( pAI,
			      STATUS_UNDEFINED, 
			      "No secs port '%d'. Use secs_open to open port.",
			      secsFd ) ;
    }

    /* Check to see if the port is already assigned */
    pAIassigned = gHyp_concept_getInstForDeviceId ( pConcept, (sWORD) id ) ;

    if ( pAIassigned && pAIassigned != pAI ) {
      gHyp_util_logWarning ( "Device id %d was previously assigned to port %d by instance %s, deleting old assignment",
			      (sWORD) id,
			      gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ),
			      gHyp_instance_getTargetId ( pAIassigned ) ) ;
      gHyp_instance_deleteFd ( pAIassigned, gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ) ;
    }
        
    if ( status ) {
 

      /*
      gHyp_util_logInfo ( "Device id %d is assigned to port %d for instance %s",
			      (sWORD) id,
			      gHyp_instance_getDeviceFd ( pAI, (sWORD) id ),
			      gHyp_instance_getTargetId ( pAI ) ) ;
      */
      pSecs2 = gHyp_secs2_new ( ) ;
      status = gHyp_instance_updateFd ( pAI, 
					secsFd, 
					id,
					pSecs2, 
					rBit ) ;

      /* In case the pSecs1 device was opened with port_open or http_open */
      if ( pSecs1 ) {
        flags = gHyp_secs1_flags ( pSecs1 ) ;
        flags = (flags & MASK_SOCKET) | PROTOCOL_SECS1 ;
        gHyp_secs1_setFlags ( pSecs1, flags ) ;

      }
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }
}

void gHyp_secs_enable ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_enable ( )
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
    
    sWORD
      id ;

    int
      ID,
      stream,
      function,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      *pMethodStr ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 4 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_enable ( method, id, stream, function )");

    /* Get the secs function */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    function = gHyp_data_getInt ( pData, gHyp_data_getSubScript (pData), TRUE);

    /* Get the secs stream */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    stream = gHyp_data_getInt ( pData, gHyp_data_getSubScript (pData), TRUE ) ;

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ID = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( ID < 0 || ID > MAX_SECS_DEVICE_ID )  {
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;
    }
    id = ID ;

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
	gHyp_instance_secsEnable ( pAI, id, stream, function, pMethodVariable ) ;
    }
    else {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
			      "Invalid method '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
    } 
    
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_secs_disable ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_disable ( )
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
    
    sWORD
      id ;

    int
      ID,
      stream,
      function,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      *pMethodStr ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 4 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_disable ( method, id, stream, function )");

    /* Get the secs function */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    function = gHyp_data_getInt ( pData, gHyp_data_getSubScript (pData), TRUE);

    /* Get the secs stream */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    stream = gHyp_data_getInt ( pData, gHyp_data_getSubScript (pData), TRUE ) ;

    /* Get the device id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ID = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Check id */
    if ( ID < 0 || ID > MAX_SECS_DEVICE_ID )  {
      gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;
    }
    id = ID ;

    /* Pop the method identifier off of the stack. */
    pData = gHyp_stack_popLvalue ( pStack, pAI ) ; 
    pMethodVariable = gHyp_data_getReference ( pData ) ;
    if ( pMethodVariable ) {
      pMethodStr = gHyp_data_getLabel ( pMethodVariable ) ;
      pMethodVariable = gHyp_frame_findMethodVariable ( pFrame, pMethodStr, pAI );
      if ( !pMethodVariable )
	gHyp_instance_warning ( pAI, STATUS_BRANCH, 
				"Could not find method '%s'", pMethodStr ) ;
      else
	gHyp_instance_secsDisable ( pAI, id, stream, function ) ;
    }
    else {
      gHyp_instance_warning ( pAI, STATUS_INVALID, 
			      "Invalid method '%s'",
			      gHyp_data_getLabel ( pData ) ) ;
    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


static void lHyp_secs_QE (	sInstance 	*pAI,
				sFrame		*pFrame,
				sStack		*pStack,
				sParse		*pParse,
				sCode		*pCode,
				sBYTE		mode,
				int		argCount,
				sLOGICAL	isRaw )
{
  /* Description:
   *
   *	Services the secs_query() and secs_event() functions.
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
   */
  sData
    *pData,
    *pData2,
    *pSecsIIdata ;

  sLevel
    *pLevel ;

  char
    method[METHOD_SIZE+1],
    sender[SENDER_SIZE+1],
    transactionId[FIELD_SIZE+1] ;

  sLOGICAL
    resend,
    status=TRUE;

  SOCKET
    fd=INVALID_SOCKET ;

  sWORD 
    id=NULL_DEVICEID ;

  int
    ID,
    nBytes,
    maxTries,
    TID=0,
    SID=0,
    stream,
    function,
    saveJmpRootLevel=giJmpRootLevel,
    hypIndex,
    cond ;
  
  sSecs1
    *pSecs1=NULL ;

  sHsms
    *pHsms=NULL ;

  sSecs2
    *pSecs2 ;

  time_t
    eventTime ;

  sInstance
    *pAIassigned ;

  /* Get argument list */
  if ( argCount == 4 ) 
    pSecsIIdata = gHyp_stack_popRdata2 ( pStack, pAI ) ;
  else
    pSecsIIdata = NULL ;

  /* Get function */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  function = gHyp_data_getInt ( pData, gHyp_data_getSubScript (pData), TRUE ) ;

  /* Get stream */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  stream = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

  /* Get id */
  pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
  ID = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

  /* Check id */
  if ( ID < 0 || ID > MAX_SECS_DEVICE_ID )  {
    if ( pSecsIIdata ) gHyp_data_delete ( pSecsIIdata ) ;
    gHyp_instance_error ( pAI,STATUS_BOUNDS, "Device ID out of range" ) ;
  }
  id = ID ;

  /* Get the SECS II structure to use when sending and receiving data
   * from the specified id 
   */

  pAIassigned = gHyp_concept_getInstForDeviceId ( 
    gHyp_instance_getConcept ( pAI ), id ) ;

  if ( !pAIassigned ) {
    gHyp_instance_warning ( pAI,STATUS_SECS, 
			    "Device id %d is not assigned",
			    id ) ;
    status = FALSE ;
  }
  else if ( pAI != pAIassigned ) {
    gHyp_instance_warning ( pAI,STATUS_SECS, 
			    "Device id %d is assigned to %s, not %s",
			    id,
                            gHyp_instance_getTargetId(pAIassigned), 
                            gHyp_instance_getTargetId(pAI)) ;
    status = FALSE ;
  }
  else
    status = TRUE ;

  if ( status ) {

    fd = gHyp_instance_getDeviceFd ( pAIassigned, id ) ;

    if ( fd == INVALID_SOCKET ) {
      gHyp_instance_warning ( pAI,STATUS_SECS, 
			    "Socket %d does not exist.",
			    fd ) ;
      status = FALSE ;
    }
  }

  if ( status ) {

    /* Get the SECS I or HSMS structure */
    pHsms = (sHsms*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
						  fd, 
						  DATA_OBJECT_HSMS ) ;

    pSecs1 = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
						    fd, 
						    DATA_OBJECT_SECS1 ) ;
    if ( pHsms == NULL && pSecs1 == NULL ) {
      status = FALSE ;
      gHyp_instance_warning ( pAI,STATUS_SECS, 
			    "No SECS connection (through port %d) exists for device %d. ",
			    fd, id ) ;
    }

  }

  if ( status ) {

    if ( pHsms && (gHyp_hsms_flags(pHsms) & (SOCKET_LISTEN | SOCKET_UNIX_LISTEN))) {
      status = FALSE ;
      gHyp_instance_warning ( pAI,STATUS_SECS, 
			    "No HSMS connection (through port %d) exists for device %d. ",
			    fd, id ) ;
    }

    if ( pSecs1 && (gHyp_secs1_flags(pSecs1) & (SOCKET_LISTEN | SOCKET_UNIX_LISTEN))) {
      status = FALSE ;
      gHyp_instance_warning ( pAI,STATUS_SECS, 
			    "No SECS 1 connection (through port %d) exists for device %d. ",
			    fd, id ) ;
    }
  }

  if ( status ) {
	  pSecs2 = gHyp_instance_getSecs2byId ( pAIassigned, id ) ;

	/* Check function */
	if ( ( mode == MESSAGE_QUERY ) && !(function % 2) )  {
		status = FALSE ;
		gHyp_instance_warning ( pAI,
			    STATUS_SECS, 
			    "Cannot query with non-primary function" ) ;
	}
    
	/* See if the secsII structure exists */
	if ( pSecs2 == NULL ) {
		status = FALSE ;
		gHyp_instance_warning ( pAI,STATUS_SECS, 
			    "Device id %d is not assigned. Use secs_assign(id,fd)",
			    id ) ;
	}
  }

  if ( status ) {

    /* Just in case we don't return, pSecsIIdata needs to be deleted */
    gpsTempData = pSecsIIdata ; 

    /* Initialize the secs structures. */
    if ( isRaw ) {
      /* To do:  get the variable of the reference to pSecsIIdata */
      pData2 = gHyp_data_getVariable ( pSecsIIdata ) ;
      if ( pData2 ) pSecsIIdata = pData2 ;
      gHyp_secs2_unParseSecsRaw ( pSecs2, pSecsIIdata ) ;
    }
    else
      gHyp_secs2_unParseSecs ( pSecs2, pSecsIIdata, pAI, stream, function ) ;

    if ( pSecs1 ) {

      TID = gHyp_secs1_TID ( pSecs1 ) ;
      SID = gHyp_secs1_SID ( pSecs1 ) ;
    }
    else if ( pHsms ) {

      TID = gHyp_hsms_TID ( pHsms ) ;
      SID = gHyp_hsms_SID ( pHsms ) ;
    }

    if ( mode == MESSAGE_QUERY ) {
      pLevel = gHyp_frame_level ( pFrame ) ;
      hypIndex = gHyp_frame_getHypIndex(pFrame) - 1 ;
      gHyp_instance_incIncomingDepth ( pAI ) ;
      gHyp_instance_setTimeOut ( pAI ) ;
      eventTime = gHyp_instance_getTimeOutTime ( pAI ) ;
      sprintf ( sender, "%u#secs%s", id, gzRoot ) ;  
      sprintf ( method, "S%dF%d", stream, function+1 ) ;
      sprintf ( transactionId, "%08x", TID ) ;
      gHyp_instance_setExpectedReply ( pAI, 
				       sender, 
				       method, 
				       transactionId,
				       (int)eventTime ) ;
      gHyp_instance_setSecsReplyIn ( pAI, 
				     id, 
				     stream, 
				     function+1,
				     TID,
				     SID ) ;
      gHyp_instance_setState ( pAI, STATE_QUERY ) ;
      gHyp_frame_setState2 ( pLevel, STATE_QUERY ) ;
    }

    /* Send message. */
    if ( pSecs1 ) {

      /* If gHyp_secs1_outgoing returns 0, then the message has been
       * interrupted (an ENQ/ENQ contention).  We should service the
       * incoming message. 
       */
      nBytes = 0 ;
      maxTries = MAX_RESEND ;
      gHyp_util_logInfo ( "Sending S%dF%d, TID=%08x", stream, function,TID );
	    
      while ( nBytes == 0 && maxTries > 0 ) {

	nBytes = gHyp_secs1_outgoing ( pSecs1, 
				       pSecs2,
				       pAI,
				       stream,
				       function,
				       id,
				       TID,
				       SID,
				       mode ) ;
	
	if ( nBytes < 0 ) 

	  /* Something that we have to abort from, but not fatal */
	  status = FALSE ;

	else if ( nBytes == 0 ) {

          /* ENQ contention likely occurred, execute all pending conditions. */

          if ( guDebugFlags & DEBUG_DIAGNOSTICS )
	    gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
	    "ENQ Contention on Query. Adjusting jmpRootLevel to %d",giJmpLevel+1);

	  if ( giJmpLevel == MAX_JMP_LEVEL ) {
            gHyp_util_logError ( "Parse jump level overflow at %d", MAX_JMP_LEVEL ) ;
            longjmp ( gsJmpStack[0], COND_FATAL ) ;
          }

	  giJmpRootLevel = giJmpLevel+1 ;
            cond = gHyp_instance_ENQcontention ( pAI, pFrame) ;
	  giJmpRootLevel = saveJmpRootLevel ; 

	  resend = TRUE ;

	  /* Incase the port was closed */
	  pSecs1 = NULL ;
          
	  fd = gHyp_instance_getDeviceFd ( pAIassigned, id ) ;
          if ( fd != INVALID_SOCKET ) {
	    pSecs1 = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							    fd, 
							    DATA_OBJECT_SECS1 ) ;
	  }
	  
	  if ( !pSecs1 ) resend = FALSE ;
          
	  if ( resend ) {

	    /* Print a helpful message */
	    gHyp_util_logInfo ( "...re-sending interrupted S%dF%d, TID=%08x",
				stream, function,TID );
	    
	    gHyp_secs2_unParseSecs ( pSecs2, 
				     pSecsIIdata, 
				     pAI, 
				     stream, 
				     function ) ;
	    

	  }
	  else {
	    gHyp_instance_warning ( pAI, STATUS_SECS, 
				   "Failed to re-send S%dF%d",
				   stream, function ) ;
	    break ;
	  }
	  maxTries-- ;
	}
      }
    }
    else if ( pHsms ) {

      nBytes = gHyp_hsms_outgoing ( pHsms, 
				    pSecs2,
				    pAI,
				    stream,
				    function,
				    id,
				    TID,
				    SID,				   
				    mode ) ;

      if ( nBytes < 0 ) status = FALSE ;
    }
    else
      status = FALSE ;

    /* pSecsIIdata can be cleared */
    gpsTempData = NULL ; 

    if ( !status ) {

      gHyp_instance_warning ( pAI, STATUS_SECS, 
			      "Failed to send S%dF%d",
				   stream, function ) ;

    }
    else if ( mode == MESSAGE_QUERY ) {
  
      /* Wait for reply message from query */

      gHyp_util_logInfo ( "...waiting for S%dF%d reply from device '%d', timeout in %d seconds", 
			  stream,function+1,
			  id,
			  (eventTime-gsCurTime)) ;
      if ( pSecsIIdata ) gHyp_data_delete ( pSecsIIdata ) ;
      pSecsIIdata = NULL ; /* Consumed */

      while ( (cond = gHyp_instance_readQueue ( pAI )) == COND_NORMAL ) {
	/* Got a message, an event, query, or reply */
        cond = gHyp_instance_readProcess ( pAI, STATE_QUERY ) ;
	/* Reply messages return COND_SILENT, event and query COND_NORMAL */
        if ( cond == COND_NORMAL ) {
          /* Got an interrupting message. Service it. */
	  cond = COND_SILENT ;
	  break ;
        }
        else if ( cond == COND_SILENT ) {
          /* Got a reply message */
	  cond = gHyp_instance_readReply ( pAI ) ;
	  /* If it satisfies this query, break and PARSE */
	  if ( cond == COND_NORMAL ) break ;
	}
	/* Otherwise keep looking */
      }

      if ( cond == COND_SILENT ) {
        /* A message is interrupting OR there is simply nothing to do but wait. */
        if ( guDebugFlags & DEBUG_FRAME )
          gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_FRAME, 
			   "frame: QUERY SECS (longjmp to %d from frame %d)",
			   giJmpRootLevel,gHyp_frame_depth(pFrame) ) ;
        /* Set index back one to be at the right spot for gHyp_parse_completeExpression */ 
        gHyp_frame_setHypIndex2 ( pLevel, hypIndex ) ;
        gHyp_parse_restoreExprRank ( pParse ) ;

        longjmp ( gsJmpStack[giJmpLevel=giJmpRootLevel], COND_SILENT) ;
      }
      else {
        /* Make sure the frame state is no longer STATE_QUERY */
        gHyp_instance_setState ( pAI, STATE_PARSE ) ;
        gHyp_frame_setState ( pFrame, STATE_PARSE ) ;
	/*gHyp_util_debug("Continuing from successful SECS query");*/
      }
    }
  }

  if ( pSecsIIdata ) gHyp_data_delete ( pSecsIIdata ) ;

  /* The STATUS variable will have either $ACKNOWLEDGE or %<error>*/
  return ;
}

void gHyp_secs_event ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
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

    if ( argCount < 3 || argCount > 4 )
    	gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
     "Invalid arguments. Usage: secs_event ( id, stream, function, data )");

    lHyp_secs_QE ( pAI, pFrame, pStack, pParse, pCode, MESSAGE_EVENT, argCount, FALSE ) ;
  
    /* Result is in status variable */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
  return ;
}

void gHyp_secs_query ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_query()
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

    if ( argCount < 3 || argCount > 4 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
     "Invalid arguments. Usage: secs_query ( id, stream, function, data )");

    lHyp_secs_QE ( pAI, pFrame, pStack, pParse, pCode, MESSAGE_QUERY, argCount, FALSE ) ;

  }
  return ;
}


void gHyp_secs_event_raw ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
		
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount < 3 || argCount > 4 )
    	gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
     "Invalid arguments. Usage: secs_event_raw ( id, stream, function, data )");

    lHyp_secs_QE ( pAI, pFrame, pStack, pParse, pCode, MESSAGE_EVENT, argCount, TRUE ) ;
  
    /* Result is in status variable */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
  return ;
}

void gHyp_secs_query_raw ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )
  
    gHyp_parse_operand ( pParse, pCode, pAI ) ;
    
  else {
 
    sStack 	*pStack = gHyp_frame_stack ( pFrame ) ;
    int	   	argCount = gHyp_parse_argCount ( pParse ) ;
    
    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount < 3 || argCount > 4 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
     "Invalid arguments. Usage: secs_query_raw ( id, stream, function, data )");

    lHyp_secs_QE ( pAI, pFrame, pStack, pParse, pCode, MESSAGE_QUERY, argCount, TRUE ) ;
  
    /* Result from query is in STATUS variable. */
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
  return ;
}


void gHyp_secs_handle ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: secs_handle ( )
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
      secsFd = INVALID_SOCKET ;

    sInstance
      *pAIassigned ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    /* Assume success */	
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid args. Usage: secs_handle ( id )");

    /* Get the secs secs id */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    id = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

    /* Get the port handle */
    pAIassigned = gHyp_concept_getInstForDeviceId ( 
      gHyp_instance_getConcept ( pAI ), id ) ;

    if ( !pAIassigned )
      gHyp_instance_warning ( pAI, STATUS_SECS, 
	  "Device id %d is not assigned", id ) ;
    else
      secsFd = gHyp_instance_getDeviceFd ( pAIassigned, id ) ;

    if ( secsFd == INVALID_SOCKET )
      gHyp_instance_warning ( pAI, STATUS_SECS, 
			      "No socket handle exists for device id %d",
			      id ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) secsFd ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}
