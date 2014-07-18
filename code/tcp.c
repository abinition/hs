/******************************************************************************
*                    AUTOROUTER Software Source Code.                         *
*                                                                             *
*          ***** Copyright: (c) 1994 Ab Initio Software                       *
*          ***** All rights reserved - Licensed Material.                     *
*          ***** Program property of Ab Initio Software                       *
*                                                                             *
******************************************************************************/
/* 
 * Modifications:
 *
 *	$Log: tcp.c,v $
 *	Revision 1.13  2004/10/21 04:06:41  bergsma
 *	Use gethostbyaddr instead of gethostbyname for connection requests to
 *	IP addresses.
 *	
 *	Revision 1.12  2004/07/01 02:18:28  bergsma
 *	Functions without stdout to use with JNI interface
 *	
 *	Revision 1.11  2004/04/29 02:13:01  bergsma
 *	Support for 127.*.*.* interfaces.
 *	Do not do gethostbyname or gethostbyaddr unless necessary,
 *	these functions block.
 *	
 *	Revision 1.10  2002/11/20 20:53:02  bergsma
 *	Remove second occurenace of uncommented log statement for Aliases.
 *	
 *	Revision 1.9  2002/11/19 01:59:58  bergsma
 *	Either __WIN32 or WIN32 for Windows.
 *	Remove log statement that tells of Alias's
 *	
 *	Revision 1.8  2002/09/24 17:27:39  bergsma
 *	Added logic when giResolveFlag1 and giResolveFlag2 are true.
 *	
 *	Revision 1.7  2002/09/21 22:23:13  bergsma
 *	Remove _debug statement
 *	
 *	Revision 1.6  2002/09/21 22:14:07  bergsma
 *	Use gethostbyaddr after all.
 *	Lowercase all host entries for use in alias table.
 *	
 *	Revision 1.5  2002/09/20 00:07:43  bergsma
 *	Removed call to gethostbyaddr in gHyp_tcp_resolveHost.  Correct logic for
 *	how the alias table is used.
 *	
 *	Revision 1.4  2002/09/09 20:52:13  bergsma
 *	Support reverse IP lookup  Also, abandon use of inet_aton, use inet_addr instead to distinguish IP number-dot notation from DNS names.
 *	
 *	Revision 1.3  2002/09/05 01:31:40  bergsma
 *	Replaced inet_addr with recommended inet_aton, but WINDOWS and VMS still use
 *	the old format.
 *	
 *	Revision 1.2  2002/09/03 21:35:45  bergsma
 *	Add reverse lookup.  Match IP addresses with the same alias addresses.
 *	
 *
 */

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ***********************/
#if defined ( WIN32 ) || defined ( __WIN32 )
#define AS_WINDOWS 1
#elif defined ( __VMS )
#define AS_VMS 1
#else
#define AS_UNIX 1
#endif

#include <sys/stat.h>

#ifdef AS_VMS
#include <sys/file.h>
#endif

#ifdef AS_SOLARIS
#include <sys/filio.h>		/* To define FIOCLEX */
#endif

#if !defined( AS_VAXC ) && !defined(AS_WINDOWS)
#include <sys/fcntl.h>
#endif

#ifdef AS_WINDOWS
#include <winsock2.h>
#define EWOULDBLOCK WSAEWOULDBLOCK
#else
#include <sys/socket.h>		/* Socket structures and functions */
#include <netdb.h>		/* For network host functions */
#include <netinet/in.h>		/* Internet specific functions */
#include <arpa/inet.h>		/* For inet_addr and inet_ntoa */
#endif

#ifdef AS_UNIX
#include <sys/ioctl.h>		/* Socket structures and functions */
#include <sys/time.h>
#endif

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

static          jmp_buf         gsJmpConnect ;
static		SOCKET		giSocket = INVALID_SOCKET ;

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/* Lookup table to find a host's fully qualified internet host name. */
struct alias_t 
{
  char 		alias[HOST_SIZE+1] ;
  char 		host[HOST_SIZE+1] ;
  char 		addr[HOST_SIZE+1] ;
} ;

static sHash	*(*gpsAliasTable)[MAX_HASH_TABLE_SIZE] ;

/**********************	FUNCTION DEFINITIONS ********************************/                                 
static char* lHyp_tcp_herror ()
{
  static char *error[6] =
  {
    "Host Lookup Failure",
    "Authoritative Answer Host not found",
    "Non-Authoritive Host not found, or SERVERFAIL",
    "Non recoverable errors, FORMERR, REFUSED, NOTIMP",
    "Valid name, no data record of requested type",
    "No address, look for MX record"
  };
#if !defined( AS_VMS ) && !defined (AS_HPUX)
  int herr = h_errno ;
#else
  int herr = errno ;
#endif
  
  if ( herr < 0 || herr > 5 ) herr = 0 ;
  return error[herr] ;
}

static sLOGICAL lHyp_tcp_setup ( SOCKET s )
{
  /* Description:
   *
   *	Set socket characteristics.
   *	If SIGIO is supported, then set the socket options for:
   *		- non-blocking I/O
   *		- generating a SIGIO signal when there's data to read 
   *	If SIGIO is not supported, the select call must used to poll for
   *	I/O readiness. I personally like the SIGIO polling method over the 
   *	select polling method.
   *
   *	If FIOCLEX is supported, then this causes the socket to be closed
   *	across a (fork and) exec.  If this socket was not closed, then
   *	hyperscript child processes that are fork'ed and exec'ed would
   *	have open all the TCP/IP, fifo, etc sockets.
   *
   * Arguments:
   *
   *	s						[R]
   *	- socket number
   *
   * Return value:
   *
   *	Returns TRUE if the socket setup went ok, otherwise returns FALSE.
   *
   */

  /* Make socket reads non-blocking */
#if defined ( F_SETFL ) && defined ( O_NONBLOCK ) && !defined (AS_VMS)

  if ( fcntl ( s, F_SETFL, O_NONBLOCK ) == -1 )
    return gHyp_util_sysError ( "Failed to F_SETFL (O_NONBLOCK) on socket" ) ;

#elif defined ( FIONBIO )
  int   
    argp = 1 ;

#ifdef AS_WINDOWS
  if ( ioctlsocket ( s, FIONBIO, &argp ) == -1 )
#else
  if ( ioctl ( s, FIONBIO, (char*) &argp ) == -1 )
#endif
    return gHyp_util_sysError ( "Failed to FIONBIO (Non-blocking IO) on socket" ) ;

#endif


#ifndef AS_VMS

#if defined ( F_SETFD )
  if ( fcntl ( s, F_SETFD, 1 ) == -1 )
    return gHyp_util_sysError ( "Failed to F_SETFD (FD_CLOEXEC) on socket" ) ;
#elif defined ( FIOCLEX )
  if ( ioctl ( s, FIOCLEX ) == -1 )
    return gHyp_util_sysError ( "Failed to FIOCLEX on socket" ) ;
#endif

#endif

  return TRUE ;
}

#ifdef SIGALRM
static int lHyp_tcp_alarmHandler ( int signo )
{
  /* Description:
   *
   *    Handler called when SIGALRM is received.
   *
   * Arguments:
   *
   *    none
   *
   * Return value:
   *
   *    none
   *
   */   
  if ( giSocket != INVALID_SOCKET ) {
    gHyp_util_logWarning("Timout waiting for socket connection");
    gHyp_sock_close ( giSocket, SOCKET_TCP, "", "" ) ;
    giSocket = INVALID_SOCKET ;
  }
  longjmp ( gsJmpConnect, 1 ) ;
  return 1 ;
}
#endif

void gHyp_tcp_createAliasTable()
{
  gpsAliasTable = gHyp_hash_new ( ALIAS_HASH_TABLE_SIZE ) ;
}

sAlias *gHyp_tcp_newAlias ( char *alias )
{
  /* Create and initialize a new alias entry */
  sAlias
    *pAlias ;

  pAlias = (sAlias*) AllocMemory ( sizeof ( sAlias ) ) ;
  strcpy ( pAlias->alias, alias ) ;
  pAlias->host[0] = '\0' ;
  pAlias->addr[0] = '\0' ;
  return pAlias ;
}

void gHyp_tcp_deleteAlias ( sAlias * pAlias )
{
  ReleaseMemory ( pAlias ) ;
  return ;
}

sAlias *gHyp_tcp_createAlias ( char *pHost, char *pAddr )
{
  sAlias 
    *pAlias,
    *pAlias2;

  if ( !(pAlias = (sAlias*) gHyp_hash_find ( gpsAliasTable,
					     ALIAS_HASH_TABLE_SIZE,
					     pHost ) ) ) {
    
    /* Create an entry in the table of aliases */
    pAlias = (sAlias*) gHyp_hash_create( gpsAliasTable,
					 ALIAS_HASH_TABLE_SIZE,
					 pHost,
					 (void* (*)(char*)) 
					 gHyp_tcp_newAlias ) ;
    /*gHyp_util_logInfo("Alias %s => %s",pHost,pAddr);*/
    strcpy ( pAlias->host, pHost ) ;
    strcpy ( pAlias->addr, pAddr ) ;
  }

  if ( strcmp ( pAlias->addr, pAddr ) != 0 ) {
    
     /* Create a reverse lookup entry */
    pAlias2 = (sAlias*) gHyp_hash_create( gpsAliasTable,
					 ALIAS_HASH_TABLE_SIZE,
					 pAddr,
					 (void* (*)(char*)) 
					 gHyp_tcp_newAlias ) ;
    /*gHyp_util_debug("Alias %s => %s",pAddr,pAlias->addr);*/
    strcpy ( pAlias2->host, pAddr ) ;
    strcpy ( pAlias2->addr, pAlias->addr ) ;

  }

  return pAlias ;
}

void gHyp_tcp_removeAliases ( char *pAddr ) 
{
  sHash
    *pHash,
    *pNext ;

  sAlias
    *pAlias ;

  int 
    i ;

  for ( i=0; i<ALIAS_HASH_TABLE_SIZE; i++ )
  
    for ( pHash = (*gpsAliasTable)[i];  pHash; pHash=pNext ) {
    
      pNext = gHyp_hash_next ( pHash ) ;
      pAlias = (sAlias*) gHyp_hash_object ( pHash ) ;

      if ( gHyp_util_match ( pAlias->addr, pAddr ) ) {
	/*gHyp_util_logInfo("Removing alias %s",pAlias->alias);*/
	gHyp_hash_remove ( gpsAliasTable,
			   ALIAS_HASH_TABLE_SIZE,			
			   pAlias->alias,
			   (void* (*)(void*)) gHyp_tcp_deleteAlias ) ;
      }
    }
}

void gHyp_tcp_deleteAliasTable()
{
  sHash
    *pHash,
    *pNext ;

  sAlias
    *pAlias ;

  int
    i ;

  for ( i=0; i<ALIAS_HASH_TABLE_SIZE; i++ )
  
    for ( pHash = (*gpsAliasTable)[i];  pHash; pHash=pNext ) {
    
      pNext = gHyp_hash_next ( pHash ) ;
      pAlias = (sAlias*) gHyp_hash_object ( pHash ) ;
      gHyp_hash_remove ( gpsAliasTable,
			 ALIAS_HASH_TABLE_SIZE,			
			 pAlias->alias,
			 (void* (*)(void*)) gHyp_tcp_deleteAlias ) ;
    }

  /* Remove the hash table */
  gHyp_hash_delete ( gpsAliasTable ) ;
}

sLOGICAL gHyp_tcp_resolveHost ( char* pHost, char *pAddr )
{
  /* Description:
   *
   * The goal is to resolve the network
   * address given by the first argument into the known IP dot notation address
   * that is the key to getting the target information, ie.
   *
   *     pTargetData = gHyp_sock_findNetwork ( pHosts, targetAddr )  ;
   *
   * Lookup of pHost always starts with a hashed search through the alias table.
   *
   * The alias table contains keys in both IP and DNS notation.  
   * If pHost is found in the alias table, then the address has been previously resolved
   * and the IP dot notation address is returned.
   *
   * If the pHost name is not in the alias table, we do a 
   * gethostbyname or gethostbyaddr lookup for the IP addresss or host name.
   *
   * If the lookup succeeds, we create two entries in the alias table: 1) the DNS name key value
   * which points to the resolved IP address and 2) the resolved IP address key which points
   * to itself.
   *
   * Arguments:
   *
   *	pHost						[R]
   *	- host alias name to be resolved
   * 
   *	pAddr						[W]
   *	- ip address of resolved host
   *
   * Return value:
   *
   *	TRUE if resolved, FALSE otherwise

   *
   */
  struct hostent
    h,*hp ;

  unsigned long
      i ;

  sAlias
    *pAlias ;

  char
    host[HOST_SIZE+1],
    addr[HOST_SIZE+1] ;

  sLOGICAL
    inAddrForm,
    doAlias = FALSE ;
 
  /* Initialize result */
  strcpy ( pAddr, pHost ) ;

  inAddrForm = ( (i = inet_addr ( pHost ) ) != INADDR_NONE ) ; 
  
  /* Get the fully resolved IP name for the requested host */  
  pAlias = (sAlias*) gHyp_hash_find ( gpsAliasTable,
				     ALIAS_HASH_TABLE_SIZE,
				     pHost ) ;

  /* This next section is kludgy, wish I had a better idea */
  if ( giResolveFlag1 && giResolveFlag2 ) {

    if ( !pAlias ) {

      /* Speical case where we do not want to lookup the address
       * but simply assign the alias to .  This occurs only
       * in gHyp_router_message just prior to calling 
       * gHyp_aimsg_initParsePartial. The first flag is set in
       * gHyp_router_message, the second flag in gHyp_aimsg_parse.
       * Both flags must be on.
       */
      
      if ( strlen ( gzResolveAddr ) == 0 ) {
	pAlias = gHyp_tcp_createAlias ( pHost, pHost ) ;
      }
      else {
	pAlias = (sAlias*) gHyp_hash_find ( gpsAliasTable,
					    ALIAS_HASH_TABLE_SIZE,
					    gzResolveAddr ) ;
	if ( pAlias ) 
	  pAlias = gHyp_tcp_createAlias ( pHost, pAlias->addr ) ;
      }
    }
    giResolveFlag1 = 0 ;
    giResolveFlag2 = 0 ;
    gzResolveAddr[0] = '\0' ;
  }

  if ( !pAlias ) {

    /* Alias 'pHost' is not in alias tables. */
	  
    /* Look it up using DNS */

    if ( inAddrForm ) {

      /* Don't do it if its a local address */
      if ( strncmp ( pHost, "127.", 4 ) == 0 )

	doAlias = TRUE ;

      else {
	
        hp = gethostbyaddr(  	(char*)&i,
				sizeof(i),
				AF_INET ) ;

        if ( !hp )
  	  gHyp_util_logWarning(	"(%s) for host '%s'",
      				lHyp_tcp_herror(), 
				pHost ) ;
	else {
	  h = *hp ;
	  strcpy ( host, h.h_name ) ;
	  gHyp_util_lowerCase ( host, strlen(host) ) ;
	  strcpy ( addr, inet_ntoa ( *(struct in_addr*) h.h_addr ) ) ;
	  doAlias = TRUE ;
	  /* gHyp_util_debug("IP %s RESOLVED to %s",host,addr) ;*/
	}
      }

    }
    else {

      hp = gethostbyname ( pHost ) ;
      if ( !hp )
	gHyp_util_logWarning(	"(%s) for host '%s'",
      				lHyp_tcp_herror(), 
				pHost ) ;
      else { 

	h = *hp ;
	strcpy ( host, h.h_name ) ;
	gHyp_util_lowerCase ( host, strlen(host) ) ;
	strcpy ( addr, inet_ntoa ( *(struct in_addr*) h.h_addr ) ) ;
	/*gHyp_util_debug("DNS %s RESOLVED to %s",host,addr);*/
	doAlias = TRUE ;
      }
    }
    if ( doAlias ) {

      /* Create or update the alias entries so everything points to 
       * the resolved ip address 
       * */
      pAlias = gHyp_tcp_createAlias ( host, addr ) ;

      if ( strcmp ( pHost, addr ) != 0 ) gHyp_tcp_createAlias ( pHost, addr ) ;
      if ( strcmp ( host, addr  ) != 0 ) gHyp_tcp_createAlias ( addr, addr ) ;
    }

  }

  if ( !pAlias )  return FALSE ; 
  
  /* Copy result */
  strcpy ( pAddr, pAlias->addr ) ;

  return TRUE ;
}

SOCKET gHyp_tcp_request ( char* pHost, int port )
{
  /* Description:
   *
   *	Request a TCP/IP connection to host 'pHost'.
   *	An autorouter process must be running on the remote host,
   *	waiting on port # 7001.  See the /etc/services file.
   *
   * Arguments:
   *
   *	pHost					[R]
   *	- name of host in which to make connection
   *
   * Return value:
   *
   *	Returns the socket number, or INVALID_SOCKET if request fails
   *
   */

  struct hostent
    h,*hp ;
  
  sSockINET
    sock ;

  SOCKET
	s ;

  sLOGICAL
    inAddrForm ;

  unsigned long
    i ;

  int
    sockLen ;

#ifdef SIGALRM
  int
	jmpVal ;
#endif

  /* Close any outstanding network requests */
  if ( giSocket != INVALID_SOCKET ) {
    gHyp_util_logWarning ( "Closing connection request to socket '%d'", 
			   giSocket ) ;
    gHyp_sock_close ( giSocket, SOCKET_TCP, "", "" ) ;
    giSocket = INVALID_SOCKET ;
  }

  /* Get the address of the requested host */ 
  /* Request a tcp/ip socket connection on a remote host */
			
  inAddrForm = ( (i = inet_addr ( pHost ) ) != INADDR_NONE ) ; 

  if ( inAddrForm ) {

    gHyp_util_logInfo("GetHostByAddr: %s",pHost) ;
    hp = gethostbyaddr(  	(char*)&i,
				sizeof(i),
				AF_INET ) ;

    if ( !hp ) {
       gHyp_util_logError (	"(%s) for address '%s'",
      				lHyp_tcp_herror(), 
				pHost ) ;
       return INVALID_SOCKET ;
    }
  }
  else {

    gHyp_util_logInfo ( "GetHostByName: %s",pHost ) ;
    if ( (hp = gethostbyname ( pHost ) ) == NULL ) {
      gHyp_util_logError(	"(%s) for host '%s'", lHyp_tcp_herror(), pHost ) ;
      return INVALID_SOCKET ;
    }
  }

  /* Copy hostent structure to safe place */
  h = *hp ;

  /* Initialize the socket structure */
  memset( (char*)&sock, (char)0, sizeof ( sSockINET ) ) ;
  sock.sin_family 	= AF_INET ;
  sock.sin_port 	= htons ( (unsigned short) port ) ; 
  /*memmove((char*) &sock.sin_addr, h.h_addr, h.h_length ) ;*/
  sock.sin_addr = * ((struct in_addr*) h.h_addr ) ;
  sockLen = sizeof ( sSockINET ) ;
  
  /* Create the socket */
  gHyp_util_logInfo("Socket: host = %s, port = %d",
    pHost,port);

  if (( giSocket = socket ( AF_INET, SOCK_STREAM, 0 )) == SOCKET_ERROR ) {
    giSocket = INVALID_SOCKET ;
    gHyp_util_sysError ( "Could not create AF_INET socket for host '%s'", pHost ) ;
    return INVALID_SOCKET ;
  }

#ifdef SIGALRM
  gHyp_signal_establish ( SIGALRM, lHyp_tcp_alarmHandler ) ;
  gHyp_signal_unblock ( SIGALRM ) ;
  alarm ( CONNECT_TIMEOUT ) ;
  if ( (jmpVal = setjmp ( gsJmpConnect )) ) return INVALID_SOCKET ;
#endif

  /* Connect to the INET host */
  if ( connect ( giSocket, (sSockGENERIC*)&sock, sockLen ) == INVALID_SOCKET ) {
    gHyp_util_sysError ( "Failed to connect to internet host '%s'", pHost ) ;
    gHyp_sock_close ( giSocket, SOCKET_TCP, pHost, "" ) ;
    giSocket = INVALID_SOCKET ;
  }	
  
#ifdef SIGALRM
  /* Cancel alarms */
  gHyp_signal_block ( SIGALRM ) ;
  alarm ( 0 ) ;
#endif
  
  /* Log success */
  if ( giSocket != INVALID_SOCKET ) {

    /* Setup the socket */
    if ( !lHyp_tcp_setup ( giSocket ) ) {
      gHyp_util_logError (
       "Failed to initialize internet socket for host '%s'", pHost ) ;
      gHyp_sock_close ( giSocket, SOCKET_TCP, pHost, "" ) ;
      giSocket = INVALID_SOCKET ;
      return INVALID_SOCKET ;		
    }
    
    gHyp_util_logInfo(
          "Connected: socket = %d, host = %s, port = %d",
	  giSocket,
	  pHost,
	  port );
  }

  /* Set giSocket to INVALID_SOCKET to show we have no outstanding connection requests. */
  s = giSocket ;
  giSocket = INVALID_SOCKET ;

  return s ;
}

SOCKET gHyp_tcp_requestJNI ( char* pHost, int port )
{
  /* Description:
   *
   *	Scaled down version of tcp_request, but with no logging
   */

  struct hostent
    h,*hp ;
  
  sSockINET
    sock ;

  SOCKET
    s ;

  int
    sockLen ;
			
  if ( (hp = gethostbyname ( pHost ) ) == NULL ) {
    return INVALID_SOCKET ;
  }

  /* Copy hostent structure to safe place */
  h = *hp ;

  /* Initialize the socket structure */
  memset( (char*)&sock, (char)0, sizeof ( sSockINET ) ) ;
  sock.sin_family 	= AF_INET ;
  sock.sin_port 	= htons ( (unsigned short) port ) ; 
  sock.sin_addr = * ((struct in_addr*) h.h_addr ) ;
  sockLen = sizeof ( sSockINET ) ;
  
  if (( s = socket ( AF_INET, SOCK_STREAM, 0 )) == SOCKET_ERROR )
    return INVALID_SOCKET ;

  if ( connect ( s, (sSockGENERIC*)&sock, sockLen ) == INVALID_SOCKET ) 
    return INVALID_SOCKET ;
  
  return s ;
}

SOCKET gHyp_tcp_make ( char *pService, char *pLocalAddr, sLOGICAL bindAll )
{
  /* Description:
   *
   *	Make the TCP/IP listen socket, which listens for incoming TCP/IP
   *	connection requests over port specified in /etc/services.
   *
   * Arguments:
   *
   *	
   *
   * Return value:
   *
   *	Socket number of TCP/IP listen socket.
   *
   */

  struct servent
    *sp ;

  struct hostent
    h,*hp ;

  unsigned long
      i ;
  
  sSockINET
    sock ;

  sLOGICAL
    isLocalAddr,
    inAddrForm ;

  SOCKET
    s ;

  int
    scanLen,
    optval,
    sockLen ;

  sLOGICAL
    isInteger ;

  /* Make a socket and listen for incoming internet connection requests. */

  /* If the service name is an integer, then we don't need to look up
   * a name in /etc/services, rather just use that integer as the port number.
   */
  scanLen = gHyp_util_getToken(pService,FALSE) ;
  if ( scanLen + strlen ( pService ) == 0 ) {
    isInteger = TRUE ;
    sscanf ( pService, "%d", &giServicePort ) ;
  }
  else {
    isInteger = FALSE ;
    /* Look up the service entry (in /etc/services) */  
    if ( (sp = getservbyname ( pService, "tcp" ) ) == NULL ) {
      gHyp_util_logError ( "Could not find %s in /etc/services", pService ) ;
      return INVALID_SOCKET ;
    }
    giServicePort = ntohs ( sp->s_port ) ; 
  }

  hp = NULL ;
  isLocalAddr = FALSE ;
  inAddrForm = FALSE ;

  if ( pLocalAddr ) inAddrForm = ( (i = inet_addr ( pLocalAddr ) ) != INADDR_NONE ) ; 

  if ( inAddrForm ) {

    if ( strncmp ( pLocalAddr, "127.", 4 ) == 0 ) {
      isLocalAddr = TRUE ;
    }
    else {
      gHyp_util_logInfo("GetHostByAddr: %s",pLocalAddr) ;
      hp = gethostbyaddr(  	(char*)&i,
				sizeof(i),
				AF_INET ) ;

      if ( !hp )
	gHyp_util_logWarning(	"(%s) for local address '%s'",
      				lHyp_tcp_herror(), 
				pLocalAddr ) ;
      else 
        h = *hp ;
    }

  }
  else if ( pLocalAddr ) {

    gHyp_util_logInfo("GetHostByName: %s",pLocalAddr) ;
    hp = gethostbyname ( pLocalAddr ) ;
    if ( !hp )
      gHyp_util_logWarning(	"(%s) for local host '%s'",
      				lHyp_tcp_herror(), 
				pLocalAddr ) ;
    else
      h = *hp ;
  }
  else
    hp = NULL ;

  /* Initialize the socket structure */
  memset( (char*) &sock, (char)0, sizeof ( sock ) ) ;
  sock.sin_family 	 = AF_INET ;

  if ( bindAll ) 
    sock.sin_addr.s_addr = htonl ( INADDR_ANY ) ;
  else if ( hp )
    sock.sin_addr = * ((struct in_addr*) h.h_addr ) ;
  else if ( isLocalAddr ) 
    sock.sin_addr.s_addr = i ;
  else
    sock.sin_addr.s_addr = htonl ( INADDR_ANY ) ;


  sock.sin_port 	 = htons ( (unsigned short) giServicePort ) ; 
  sockLen 		 = sizeof ( sSockINET ) ; 
  
  /* Create the socket */
  if ( pLocalAddr ) 
    gHyp_util_logInfo("Socket: host = %s, port = %d", pLocalAddr,giServicePort);
  else
    gHyp_util_logInfo("Socket: port = %d", giServicePort);

  if ( ( s = socket ( AF_INET, SOCK_STREAM, 0 )) == SOCKET_ERROR ) {
    gHyp_util_sysError (
    	"Could not create AF_INET socket" );
     return INVALID_SOCKET ;
  }

  /* Set some socket options. The REUSE ADDRESS feature allows the
   * autorouter to start back up right away after a crash, without waiting
   * for the inetd daemon to time-out and release the internet port #6001.
   * The socket option FIOCLEX (see lHyp_tcp_setup) is also important,
   * as it prevents fork'd/exec'd hyperscript processes from locking up the
   * internet port and preventing its reuse.
   */
  optval = 1 ;
  if ( setsockopt (	s,
			SOL_SOCKET,
			SO_REUSEADDR, 
			(char*)&optval, 
			sizeof ( optval ) ) == SOCKET_ERROR) { 
    gHyp_util_sysError ( "Failed to set socket option to re-use for address") ;
    gHyp_sock_close ( s, SOCKET_TCP,gzLocalHost, pLocalAddr ) ;
    return INVALID_SOCKET ;		
  }

  /* Setup the socket for connection requests */
  if ( !lHyp_tcp_setup ( s ) ) {
    gHyp_util_logError (
  	"Failed to initialize internet socket" ) ;
    gHyp_sock_close ( s, SOCKET_TCP, gzLocalHost, pLocalAddr ) ;
    return INVALID_SOCKET ;		
  }                                                 
 
  /* Bind the socket to the INET domain */
  if ( bind ( s, (sSockGENERIC*)&sock, sockLen ) == SOCKET_ERROR ) {
    gHyp_util_sysError ( 
  	"Failed to bind internet socket" ) ;
    gHyp_sock_close ( s, SOCKET_TCP, gzLocalHost, pLocalAddr ) ;
    return INVALID_SOCKET ;		
  }
  
  if ( ( listen ( s, 5 ) ) == SOCKET_ERROR ) {
    gHyp_util_sysError ( "Failed to listen on internet socket" ) ;
    gHyp_sock_close ( s, SOCKET_TCP, gzLocalHost, pLocalAddr ) ;
    return INVALID_SOCKET ;
  }

  if ( pLocalAddr ) {
    if ( isInteger )
      gHyp_util_logInfo(  "Listening: addr = %s, port = %u, socket = %d",
			  pLocalAddr, giServicePort, s ) ;
    else
      gHyp_util_logInfo(  "Listening: addr = %s, port = %u, socket = %d, service = %s",
			  pLocalAddr, giServicePort, s, pService ) ;
  }
  else {
    if ( isInteger )
      gHyp_util_logInfo( "Listening: port = %u, socket = %d",giServicePort, s ) ;
    else
      gHyp_util_logInfo( "Listening: port = %u, socket = %d, service = %s", giServicePort, s, pService ) ;
  }

  return s ;
}


SOCKET gHyp_tcp_checkInbound ( SOCKET s,
			       char *pHost,
			       char *pAddr,
			       sLOGICAL okLocalHost ) 
{
  /* Description:
   *
   *	Look for an incoming TCP/IP connection request over the
   *	TCP/IP listen port.  
   */

  struct hostent
    h,*hp ;

  sSockINET
    sock ;

#ifdef AS_VMS
  unsigned int
#else
  int
#endif
    sockLen = sizeof ( sSockINET ) ;

  char
    resolvedAddr[HOST_SIZE+1] ;

  SOCKET
    ns=INVALID_SOCKET ;

  /* Accept inbound connection requests over the InterNet */
  
  if ( s == INVALID_SOCKET ) return INVALID_SOCKET ;
     
  if ( ( ns = accept ( s, (sSockGENERIC*)&sock, &sockLen ) ) == SOCKET_ERROR ) {

    /* The socket has been made non-blocking, so EAGAIN and EWOULDBLOCK are 
     * expected error codes, but all other errors are bad news.
     */
    if ( errno != EAGAIN && errno != EWOULDBLOCK ) {
      gHyp_util_sysError ( 
      	"Failed to accept internet connection request" ) ;
      return INVALID_SOCKET ;
    }
  }
  else {
  
    /* Received inbound connection request. */   

    /* Capture address. */
    strcpy ( pAddr, inet_ntoa (sock.sin_addr) ) ;

    if ( strncmp ( pAddr, "127.", 4 ) == 0 ) {

      /* Make IP address point to IP address */
      strcpy ( pHost, pAddr ) ; 
      gHyp_util_lowerCase ( pHost, strlen ( pHost ) ) ;

      /* Create an alias to itself. */
      gHyp_tcp_createAlias ( pHost, pAddr ) ;

      /* Pretend the address was resolved */
      strcpy ( resolvedAddr, pAddr ) ;
    }
    else /* Do a DNS lookup */ 
    if ( 
      !(hp = gethostbyaddr(	(char*)&sock.sin_addr,
				sizeof (sock.sin_addr),
				AF_INET) ) ) {
      /* No DNS name - no problem. */
      gHyp_util_logWarning(	"(%s) for address '%s'" ,
				lHyp_tcp_herror(),
				pAddr ) ;
      /* Make IP address point to IP address */
      strcpy ( pHost, pAddr ) ; 
      gHyp_util_lowerCase ( pHost, strlen ( pHost ) ) ;

      /* Create an alias to itself. */
      gHyp_tcp_createAlias ( pHost, pAddr ) ;

      /* Pretend the address was resolved */
      strcpy ( resolvedAddr, pAddr ) ; 
    }
    else {

      /* DNS lookup good. Capture host and address names */
      h = *hp ;
      strcpy ( pHost, h.h_name ) ;
      gHyp_util_lowerCase ( pHost, strlen ( pHost ) ) ;
      strcpy ( resolvedAddr, inet_ntoa ( *(struct in_addr*) h.h_addr ) ) ;

      /* Create or update the alias entry for the host name and its resolved address . */
      gHyp_tcp_createAlias ( pHost, resolvedAddr ) ;

      if ( strcmp ( pAddr, resolvedAddr ) != 0 ) {
	/* Resolved address and incoming address are not the same. 
	 * Create a reverse lookup for the incoming address 
	 */
	gHyp_tcp_createAlias ( pAddr, resolvedAddr ) ;
      }

      /* Create an entry that points to itself */
      gHyp_tcp_createAlias ( resolvedAddr, resolvedAddr ) ;

    }

    if ( !lHyp_tcp_setup ( ns ) ) {
      gHyp_util_sysError ( "Failed to setup socket for host '%s'", pHost ) ;
      gHyp_sock_close ( ns, SOCKET_TCP, pHost, resolvedAddr ) ;
      return INVALID_SOCKET ;
    }
    
    if ( !okLocalHost && strcmp ( resolvedAddr, gzLocalAddr ) == 0 ) {  
      if ( giListenLoopback_R == INVALID_SOCKET ) {
        gHyp_util_logInfo ( 
	"Loopback: addr = %s, socket = %d", resolvedAddr, ns ) ;
        giListenLoopback_R = ns ;
        giLoopback = giListenLoopback_W ;
        giLoopbackType = SOCKET_TCP ;
      }
      else {
	gHyp_util_logError ( "Loopback connection already exists. Rejecting") ;
        gHyp_sock_close ( ns, SOCKET_TCP, pHost, resolvedAddr ) ;
        return INVALID_SOCKET ;
      }
    }
  }
  return ns;
}

void gHyp_tcp_gethostname ( char *host, int size )
{
 gethostname ( host, size ) ;
}
