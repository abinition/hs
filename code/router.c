/******************************************************************************
*                    AUTOROUTER Software Source Code.                         *
*                                                                             *
*          ***** Copyright: (c) 1994 Ab Initio Software                       *
*          ***** All rights reserved - Licensed Material.                     *
*          ***** Program property of Ab Initio Software                       *
*                                                                             *
******************************************************************************/

/* Modifications: 
 *
 * $Log: router.c,v $
 * Revision 1.18  2004/07/23 18:45:43  bergsma
 * fixed bug where query would not return failure if the target host could not be resolved.
 *
 * Revision 1.17  2003/04/16 01:41:38  bergsma
 * Message Routing
 * ---------------
 * Lift the restriction whereby a hyperscript which specifies a default listen
 * socket with the -n switch does not also have to be the root,
 * ie: specify the -r switch.
 * The restriction has been moved to gHyp_router_message,
 * whereby messages that need to be routed to another host are only sent
 * out on the "-n" socket channel if that hyperscript is also the root.
 * Files affected: hs.c and router.c
 *
 * Revision 1.16  2003/04/04 16:17:10  bergsma
 * When a signal occurred that generated a special ||SIG*||| message on the
 * loopback channel, the special was not being filtered out - it was getting
 * processed as a message and causing a warning error.
 *
 * Revision 1.15  2003/02/17 10:13:44  bergsma
 * If the pTargetChannel was originally found but the write failed, it may
 * be because the socket connect is stale (happens when a client closes
 * its connection but does not notify the parent since it never had a channel
 * open to it in the first place).
 * If it reopens successfully, then the write is retried.
 *
 * Revision 1.14  2003/01/15 13:06:32  bergsma
 * Before sending to client, make sure reader inbox is open.
 *
 * Revision 1.13  2003/01/14 02:33:03  bergsma
 * Initialize targetId and senderId to targetPath and senderPath
 *
 * Revision 1.12  2003/01/09 08:05:26  bergsma
 * V3.1.0
 * Major rework of routing algorithm to handle tree view.
 *
 * Revision 1.11  2002/11/20 20:48:32  bergsma
 * Fix handling of hierarchial message routing using gzRoot.  Call instantiate()
 * if the instance does not exist and the method is CONNECT.
 *
 * Revision 1.10  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.9  2002/11/12 20:17:14  bergsma
 * Removed obsolete calls to gHyp_channel_initStats
 *
 * Revision 1.8  2002/10/27 20:45:27  bergsma
 * Added third argument to gHyp_sock_createClient. This allows the
 * sender client to be accessed even if its already created.
 *
 * Revision 1.7  2002/10/27 14:40:48  bergsma
 * Forward messages to parent only if the sender is local.  This is to
 * prevent a fatal spin condition.
 *
 * Revision 1.6  2002/09/24 17:26:45  bergsma
 * Implemented a new way of resolving the sender address.
 * When a message arrives in which the sender host has not been resolved, then
 * we will create an alias for the sender host without doing a gethostbyname
 * or gethostbyaddr lookup.  In case 1, where gzResolveAddr is not blank,
 * the sender host alias is assigned to gzResolveAddr.  In case 2, where
 * gzResolveAddr is blank, the sender host alias is assigned to itself.
 * See also gHyp_tcp_resolveHost.
 *
 * Revision 1.5  2002/09/21 22:15:17  bergsma
 * If the target is not resolved, don't send back to the parent, just discard.
 * Make sure the message is displayed, routed or not router.
 *
 * Revision 1.4  2002/09/20 00:06:57  bergsma
 * Correct arguments to gHyp_tcp_createAlias for resolving the sender address.
 *
 * Revision 1.2  2002/09/09 20:48:59  bergsma
 * When an IP or HostName is not resolved, try reverse lookup (in TCP.C).
 * Also, when the hyperscript is not the root and the message cannot be
 * routed, then send it to the root.
 *
 *
 */

/**********************	AUTOROUTER INTERFACE ********************************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL FUNCTION DECLARATIONS ***********************/

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

static char gzMessage[MIN_MESSAGE_SIZE+1] ;

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/**********************	FUNCTION DEFINITIONS ********************************/

static char *lHyp_router_rts ( char	*pSenderPath,
			       char     *pSenderConcept,
			       char	*pSenderHost,
			       char 	*pMode,
			       char 	*pMethodSent,
			       char	*pTargetPath,
			       char	*pRouterPath,
			       char	*pTID )
{
  /* Description:
   *
   * 	Return to sender a special message when the orginal message
   *	failed to route to its intended target.
   *
   *	Router returns a message that has TEXT that says:
   *		"%TARGET: Failed to route message to <target>@<targetNode>"
   *
   */

  char
    mode[MODE_SIZE+1],
    extra[80],
    *pStatus = "%TARGET",
    *pMethodDefault = "MESSAGE",
    *pMethod = pMethodDefault,
    *pText = "Failed to route message to " ;
  
   if ( strcmp ( pSenderConcept, "promis" ) == 0 &&
	( strcmp ( pMethod, "abort"      ) == 0 ||
	 strcmp ( pMethod, "ABORT"      ) == 0 ||
	 strcmp ( pMethod, "disconnect" ) == 0 ||
	 strcmp ( pMethod, "DISCONNECT" ) == 0 ) )
    sprintf ( extra, 
	      "|TEXT|%s: %s |%s||PAGEWAIT|YES|", 
	      pStatus,
	      pText,
	      pTargetPath ) ;
   else
     strcpy ( extra, "" ) ;
  
   if ( strcmp ( pMode, "query" ) == 0 ) {
	 /* Failed to route a query message, send back a reply */
     strcpy ( mode, "reply" ) ;
     pMethod = pMethodSent ;
   }
   else {
	 /* Failed to route an event or reply message, 
	  * send back an event. 
	  */
     strcpy ( mode, "event" ) ;
     pMethod = pMethodDefault ;
   }

   /* Construct a reply message */
   sprintf (	gzMessage, 
		"|%s@%s|%s|%s|%s|%s|%s||STATUS|%s: %s |%s|%s||", 
		pSenderPath,
		pSenderHost,
		mode,
		pMethod,
		pRouterPath,
		pTID,
		gHyp_util_timeStamp ( gsCurTime ),
		pStatus,
		pText,
		pTargetPath,
		extra ) ;

   return gzMessage ;
}

int gHyp_router_message ( sConcept *pConcept,
			  sData *pClients, 
			  sData *pHosts,
			  char *pMsg,
			  char *pMsgAddr )
{
  /* Description:
   *
   *	Handles the routing of Ab Initio Protocol messages between local and
   *	remote processes.
   *
   */
   
  sData
    *pTargetData,
    *pSenderData ;
   
  sChannel
    *pTargetChannel,
    *pSenderChannel ;

  char
    *pMethod,
    *pMode,
    *pStr,
    *pChar,
    targetId[OBJECT_SIZE+1],
    senderId[OBJECT_SIZE+1],
    targetPath[OBJECT_SIZE+1],
    targetParent[OBJECT_SIZE+1],
    targetRoot[OBJECT_SIZE+1],
    targetRoot2[OBJECT_SIZE+1],
    senderPath[OBJECT_SIZE+1],
    senderParent[OBJECT_SIZE+1],
    senderRoot[OBJECT_SIZE+1],
    conceptPath[OBJECT_SIZE+1],
    senderInstance[INSTANCE_SIZE+1],
    targetInstance[INSTANCE_SIZE+1],
    targetClient[OBJECT_SIZE+1],
    senderClient[OBJECT_SIZE+1],
    targetAddr[MAX_PATH_SIZE+1],
    senderAddr[MAX_PATH_SIZE+1],
    parentConcept[TOKEN_SIZE+1],
    parentPath[OBJECT_SIZE+1],
    reason[VALUE_SIZE+1],
    *pTargetInstance,
    *pSenderInstance,
    *pTargetConcept,
    *pSenderConcept,
    *pTargetHost,
    *pSenderHost,
    *pTID ;

  sLOGICAL
    isLocalTarget,
    isLocalSender,
    isWildCardSender,
    isWildCardTarget,
    isTargetResolved,
    isSenderResolved,
    isEventOrQuery,
    isConnect,
    isDisconnect,
    isRouted,
    isMsgToUs,
    isMsgFromUs,
    msgDisplayed ;
  
  int
    cond=COND_SILENT,
    n,
    msgLen,
    socket;

  sInstance
    *pAI ;

  isRouted = FALSE ;
  strcpy ( reason, "No reason supplied" );
  msgDisplayed = FALSE ;

  strcpy ( gzResolveAddr, pMsgAddr ) ;
  giResolveFlag1 = TRUE ;
  
  /* Parse the external message into its separate components */
  if ( !gHyp_aimsg_initParsePartial ( gpsAImsg, pMsg ) ) {
    gHyp_util_output ( pMsg ) ;
    gHyp_util_output ( "\n" ) ;
    gHyp_util_logWarning ( "Message was not parsed, was not routed" ) ;
    gHyp_util_output ( "-------------\n" ) ;
    msgDisplayed = TRUE ;
    return COND_SILENT ;
  }
  giResolveFlag1 = FALSE ;

  /* Construct the target Id and sender Id, needed for the lookup tables. */
  gHyp_aimsg_getTargetPath ( gpsAImsg, targetPath ) ;
  gHyp_aimsg_getSenderPath ( gpsAImsg, senderPath ) ;
  strcpy ( targetId, targetPath ) ;
  strcpy ( senderId, senderPath ) ;

  /* Get some target and sender characteristics.
   * - Extract the concept name from the [instance.]concept format.
   * - Get the host name and resolve it completely by adding the complete
   *   domain name.  DNS is used the to resolve the name the first time,
   *   thereafter it is gotten from an internal table.
   * - Determine if the host name is local or remote.
   */
  pTargetData = NULL ;
  pSenderData = NULL ;
  pTargetChannel = NULL ;
  pSenderChannel = NULL ;
  pTargetHost = gHyp_aimsg_targetHost ( gpsAImsg ) ;
  pSenderHost = gHyp_aimsg_senderHost ( gpsAImsg ) ;
  pTargetConcept = gHyp_aimsg_targetConcept ( gpsAImsg ) ;
  pSenderConcept = gHyp_aimsg_senderConcept ( gpsAImsg ) ;
  pTargetInstance = gHyp_aimsg_targetInstance ( gpsAImsg ) ;
  pSenderInstance = gHyp_aimsg_senderInstance ( gpsAImsg ) ;
  strcpy ( targetParent, gHyp_aimsg_targetParent ( gpsAImsg ) ) ;
  strcpy ( senderParent, gHyp_aimsg_senderParent ( gpsAImsg ) ) ;
  strcpy ( targetRoot, gHyp_aimsg_targetRoot ( gpsAImsg ) ) ;
  strcpy ( senderRoot, gHyp_aimsg_senderRoot ( gpsAImsg ) ) ;

  isTargetResolved = gHyp_tcp_resolveHost ( pTargetHost, targetAddr ) ;
  isSenderResolved = gHyp_tcp_resolveHost ( pSenderHost,senderAddr ) ;
  
  isLocalTarget = ( isTargetResolved && strcmp ( targetAddr, gzLocalAddr ) == 0 ) ;
  isLocalSender = ( isSenderResolved && strcmp ( senderAddr, gzLocalAddr ) == 0 ) ;

  isMsgToUs = ( strcmp ( gzConcept, pTargetConcept ) == 0 &&
		strcmp ( gzParent, targetParent ) == 0 ) ;
  isMsgFromUs = ( strcmp ( gzConcept, pSenderConcept ) == 0 &&
		  strcmp ( gzParent, senderParent ) == 0 ) ;


  /* Construct our current path.
   * If we are root, gzParent is null and path will be, eg: /router
   * If we are not root, path will be /concept/router or /child/concept/router
   */
  if ( strcmp ( gzConcept, gzAUTOROUTER ) == 0 )
    sprintf ( conceptPath, "/%s%s", gzConcept, gzParent ) ;
  else
    sprintf ( conceptPath, "/%s%s/%s", gzConcept, gzParent, gzAUTOROUTER ) ;
  
  /* The message is either for us, or must be routed up, down, to another node, or rejected.
   * To be routed down, our complete path must be a trailing suffix to the
   * complete path of the target.
   */
  strcpy ( targetClient, "" ) ;
  strcpy ( senderClient, "" ) ;

  if ( isLocalTarget && strcmp ( pTargetConcept, gzAUTOROUTER ) != 0 ) {
    /* Fix targetParent (for algorithm below) if the target is not the root */
    strcat ( targetParent, "/") ;
    strcat ( targetParent,  gzAUTOROUTER ) ;
    strcpy ( targetRoot2, targetRoot ) ; /* Save a copy */
    strcat ( targetRoot, "/" ) ;
    strcat ( targetRoot, gzAUTOROUTER ) ;

    
  }
  if ( isLocalSender && strcmp ( pSenderConcept, gzAUTOROUTER ) != 0 ) { 
    /* Fix senderParent (for algorithm below) if the sender is not the root */
    strcat ( senderParent, "/") ;
    strcat ( senderParent,  gzAUTOROUTER ) ;
    strcat ( senderRoot, "/" ) ;
    strcat ( senderRoot, gzAUTOROUTER ) ;
  }

  /* If we have to route to the parent and need to start the parent, we need
   * the concept name and the path
   */
  if ( gzRoot[0] )
    strcpy ( parentPath, gzRoot+1 ) ;
  else
    strcpy ( parentPath, gzAUTOROUTER ) ;

  n = strcspn ( parentPath,"/" ) ;
  strncpy ( parentConcept, parentPath, n ) ;
  parentConcept[n] = '\0' ;
  pChar = strchr ( parentConcept, '#' ) ;
  if ( pChar ) strcpy ( parentConcept, pChar+1 ) ; 
  
  if ( !isMsgToUs && isLocalTarget ) {
    
    /* The message is either routed up or down. */	

    /* Does our 'path' exist at the end of 'targetParent' ? */
    pStr = strstr ( targetParent, conceptPath ) ;

    if ( pStr && ( strlen ( pStr ) == strlen ( conceptPath )) ) {

      /* Yes it does. Is target at our level or deeper? */

      /* Get the instance that is named in the message route */
      n = strspn ( targetRoot+1,"#/") ;
      if ( n > 0 ) {
        strncpy ( targetInstance, targetRoot+1, n );
        targetInstance[n] = '\0' ;
        if ( strcmp ( targetInstance, gzConcept ) != 0 ) {
          /* Does the instance exist? */
          pAI = gHyp_concept_getNamedInstance ( pConcept, targetInstance ) ;
          if ( !pAI ) 
            gHyp_util_logWarning( "Target instance %s does not exist in concept %s",
		                targetInstance, gzConcept ) ;
	}
      }
      
      if ( pStr == targetParent )

	/* Directly below us - eg: /concept/router  */
	strcpy ( targetClient, pTargetConcept ) ;
      
      else {
	
	/* Further below us, eg: /child/concept/router.
	 * Extract just the child token previous to the path.
	 * NOTE: This invalidates targetParent! Do not use after this.
	 */

	/* Change path /child/concept/router to /child/concept*router (*=NULL) */
	*pStr = '\0' ;

	/* Look for 'concept' by finding last occurence of '/' */
	pStr = strrchr ( targetParent, '/' ) ;
	*pStr = '\0' ;  /* Now targetParent is /child*concept*router */
	pStr++ ;
	strcpy ( targetClient, pStr ) ;  /* concept */

	/* Count the number of slashes left in targetParent */
	n = 0 ;
	pChar = targetParent ;
	while ( ( pChar = strchr ( pChar, '/') ) ) {
	  n++ ;
	  pChar++ ;
	}
	/* Advance the same number of slashes through targetRoot */
	pChar = targetRoot+1 ;
	while ( n-- ) {
	  pChar = strchr ( pChar, '/' ) ;
	  pChar++ ;
	}
	/* Now locate the concept inside */
	strcpy ( targetId, pChar ) ;
	
      }    
    }
  }
  
  if ( !isMsgFromUs && isLocalSender ) {	 
    
    /* Does our 'path' exist at the end of 'senderParent' ? */
    pStr = strstr ( senderParent, conceptPath ) ;
    if ( pStr && (strlen ( pStr ) == strlen ( conceptPath )) ) {
      
      /* Get the instance that is named in the message route */
      n = strspn ( senderRoot+1,"#/") ;
      if ( n > 0 ) {
        strncpy ( senderInstance, senderRoot+1, n );
        senderInstance[n] = '\0' ;
        if ( strcmp ( senderInstance, gzConcept ) != 0 ) {
          /* Does the instance exist? */
          pAI = gHyp_concept_getNamedInstance ( pConcept, senderInstance ) ;
          if ( !pAI ) 
            gHyp_util_logWarning( "sender instance %s does not exist in concept %s",
		                senderInstance, gzConcept ) ;
	}
      }
      
 
      /* Down - get the client concept. */
      if ( pStr == senderParent )

	/* Directly below us - our child */
	strcpy ( senderClient, pSenderConcept ) ;

      else {

	/* Further below us. Extract just the child token previous to the path.
	 * NOTE: This invalidates senderParent! Do not use after this.
	 */

	/* Change path /child/concept/router to /child/concept*router (*=NULL) */
	*pStr = '\0' ;

	/* Look for 'concept' by finding last occurence of '/' */
	pStr = strrchr ( senderParent, '/' ) ;
	*pStr = '\0' ;  /* Now senderParent is /child*concept*router */
	pStr++ ;
	strcpy ( senderClient, pStr ) ;  /* concept */

	/* Count the number of slashes left in senderParent */
	n = 0 ;
	pChar = senderParent ;
	while ( ( pChar = strchr ( pChar, '/') ) ) {
	  n++ ;
	  pChar++ ;
	}
	/* Advance the same number of slashes through senderRoot */
	pChar = senderRoot+1 ;
	while ( n-- ) {
	  pChar = strchr ( pChar, '/' ) ;
	  pChar++ ;
	}
	/* Now locate the concept inside */
	strcpy ( senderId, pChar ) ;
      }
    }
  }

  isWildCardTarget = ( strchr ( targetClient, '*' ) != NULL ) ;
  isWildCardSender = ( strchr ( senderClient, '*' ) != NULL ) ;

  /* gHyp_util_debug("sender client is '%s'", senderClient ) ;
   * gHyp_util_debug("target client is '%s'", targetClient ) ;
   */

  /* Get transaction id of message */
  pTID = gHyp_aimsg_getTID ( gpsAImsg ) ;
  
  /* Point to some other components from the message */
  pMethod = gHyp_aimsg_method ( gpsAImsg ) ;
  pMode = gHyp_aimsg_mode ( gpsAImsg ) ;

  isEventOrQuery = ( strcmp ( pMode, "event" ) == 0 || 
		     strcmp ( pMode, "query" ) == 0 ) ;

  isConnect = ( strcmp ( pMethod, "connect" ) == 0 ||
		strcmp ( pMethod, "CONNECT" ) == 0 ) ;

  isDisconnect = ( strcmp ( pMethod, "disconnect" ) == 0 ||
		   strcmp ( pMethod, "DISCONNECT" ) == 0 ||
		   strcmp ( pMethod, "abort"      ) == 0 ||
		   strcmp ( pMethod, "ABORT"      ) == 0 ) ;

  /* Look for a pre-existing return socket for the sender */
  if ( isLocalSender && !isMsgFromUs && senderClient[0] && !isWildCardSender ) {
    pSenderData = gHyp_sock_findClient ( pClients, senderClient, senderId ) ;
  }
  else if ( !isLocalSender && isSenderResolved ) 

    pSenderData = gHyp_sock_findNetwork ( pHosts, senderAddr )  ;

  if ( pSenderData )
    pSenderChannel = (sChannel*) gHyp_data_getObject ( pSenderData ) ;

  /* Re-format the message to send. */
  if ( isLocalTarget ) {
    
    /******
     * Routing logic
     *
     * Example:
     *
     * We are:
     *
     *	instance = A-1
     *	concept = semy
     *	parent = diff/fab
     *  
     *   Incoming messages:
     *
     *		target = F12345.1#lotid/A-1#semy/diff2#diff/fab3#fab
     *	        instance = F12345.1
     *		concept = lotid
     *		parent = semy/diff/fab
     *		1. Message is destined for one of our clients.
     *		2. Our parent "diff/fab" is suffix to the msg parent, so it's below us 
     *		3. Lookup "lotid" in pClients, if found then send message 
     *		4. If 'lotid' is not found then startup a lotid hyperscript and
     *		   first put a CONNECT message in the buffer followed by the
     *		   client message.
     *
     *		target = 1#wafer/F12345.1#lotid/A-1#semy/diff2#diff/fab3#fab
     *	        instance = 1
     *		concept = wafer
     *		parent = lotid/semy/diff/fab
     *		1. Message is destined for one of our client's clients.
     *		2. Our parent "diff/fab" is suffix to the msg parent, so it's below us 
     *	        3. Lookup "lotid" in pClients, if found then send message.
     *		4. If 'lotid' is not found then startup a lotid hyperscript and
     *		   first put a CONNECT message in the buffer followed by the
     *		   client message.
     *
     *         Message to concept contained in diff2
     *		target = A-1#semy/diff2#diff/fab3#fab
     *	        instance = A-1 
     *		concept = semy
     *		parent = diff/fab
     *		1. Same parent, same concept, its a message for us.
     *		2. Lookup instance, if found then put message in pAI queue.
     *		3. If instance is not found, then send CONNECT message.
     *
     *         Message to concept contained in fab3
     *		target = diff2#diff/fab3#fab
     *	        instance = diff2 
     *		concept = diff
     *		parent = fab
     *		1. No parent match, so send it to our parent.  If we are the root,
     *		   then we have no parent, then reject message.
     */
    
    /* A local client target */    
    if ( isMsgToUs ) {

      /* The message is for us */
      gHyp_sock_logger ( gzInboxPath, pMsg, gpsAImsg ) ;
      isRouted = TRUE ;

    }
    else {
      
      /* The message is not addressed to us - it must be routed */

      if ( isWildCardTarget && targetClient[0] ) {

        /* The message is addressed to multiple targets. If the message
         * is sent to any one target, then 'isRouted' is TRUE, and the
	 * message is printed out down further in the code.
	 * If we didn't route, print the message now.
         */
        isRouted = gHyp_sock_putWildCardClients ( pClients, targetClient, gpsAImsg ) ;
        if ( !isRouted ) {
	  gHyp_util_output ( pMsg ) ; 
	  gHyp_util_output ( "\n" ) ;
	  sprintf ( reason, "Failed to put message to wild card clients '%s'",targetClient);
	  msgDisplayed = TRUE ;
	}
      }
      else if ( targetClient[0] ) {
      
        /* The message is for a specific target. */
    
	/* Make sure the reader is open, if not already */
	gHyp_concept_openReader ( pConcept ) ; 

        /* Look for a socket already created for the specified target */
        pTargetData = gHyp_sock_findClient ( pClients, targetClient, targetId ) ;

	/* Try to execute a script file if the message is CONNECT and
	 * the socket does not exist.
	 */
        if ( isConnect && isEventOrQuery && pTargetData == NULL ) {

	  /* If a target file exits, execute it. */
	  if ( !pTargetData ) 
 	    pTargetData = gHyp_sock_findHYP ( pClients, targetClient, targetId, 
					      pMsg, gpsAImsg, &isRouted) ;

	  if ( !pTargetData )
	    pTargetData = gHyp_sock_findSCR ( pClients, targetClient, targetId, 
					      pMsg, gpsAImsg, &isRouted ) ;

	  if ( !pTargetData ) 
	    pTargetData = gHyp_sock_findCOM ( pClients, targetClient, targetId, 
					      pMsg, gpsAImsg, &isRouted )  ;

	  if ( !pTargetData ) 
	    pTargetData = gHyp_sock_findEXE ( pClients, targetClient, targetId, 
					      pMsg, gpsAImsg, &isRouted ) ;

	  if ( !isRouted ) 
	    sprintf ( reason, "Failed to route message to new target '%s'",targetId ) ;

	}
	
	if ( pTargetData ) {
	  pTargetChannel = (sChannel*) gHyp_data_getObject ( pTargetData ) ;
 	  assert ( pTargetChannel ) ;
	}

        /* If a target socket was found or created, then route the message. */
	if ( pTargetChannel ) {

	  /**** For CONNECT messages, update the transaction data. *
	  if ( isConnect && isEventOrQuery ) {
	  
	    * Set the tokens/values associated with the CONNECT message. *
	    gHyp_channel_initStats ( pTargetChannel, 
				     senderId, 
				     gHyp_aimsg_tokensValues ( gpsAImsg ) ) ;	    
	  }
	  *****/ 

	  /* Send message. */
          if ( !isRouted ) isRouted = gHyp_sock_putChannel ( pTargetChannel,
							     pMsg,
							     gpsAImsg ) ;

	  if ( !isRouted ) {
	    /* If there was a problem, then delete the client socket. */
	    gHyp_data_detach ( pTargetData ) ;
	    gHyp_data_delete ( pTargetData ) ;

	    /* Try one more time */
	    pTargetData =  gHyp_sock_findClient ( pClients, targetClient, targetId ) ;
	    if ( pTargetData ) {
	      pTargetChannel = (sChannel*) gHyp_data_getObject ( pTargetData ) ;
	      assert ( pTargetChannel ) ;
              isRouted = gHyp_sock_putChannel ( pTargetChannel,
						pMsg,
						gpsAImsg ) ;
	      
	      
	    }
	    if ( !isRouted ) {
	      sprintf ( reason, "Failed to write message to target client '%s'",targetClient);
	      pTargetData = NULL ;
	      pTargetChannel = NULL ;
	    }
	  }
	}
	else {
	  /* No target socket. Print the message */
	  sprintf ( reason, "No target socket exists for '%s'", targetClient ) ;
	  gHyp_util_output ( pMsg ) ;
	  gHyp_util_output ( "\n" ) ;
	  msgDisplayed = TRUE ;
	}
      }
      else {
	/* Route the message to the parent */
        isRouted = gHyp_concept_writeParent ( pConcept, 
					      pMsg, 
					      parentConcept, 
					      parentPath,
					      isConnect,
					      isEventOrQuery ) ;
	if ( !isRouted ) {
	  sprintf ( reason, "Failed to write message to parent (%s) '%s'",
	    parentConcept,
	    parentPath);
	  cond = COND_ERROR ;
	}
      }
    }
  }  
  else if ( isTargetResolved ) {

    /* Network target */
    
    /* Connection might already exist, look for it. */
    if ( giARservicePort != -1 && (guRunFlags & RUN_ROOT) )  {

      pTargetData = gHyp_sock_findNetwork ( pHosts, targetAddr )  ;
      if ( pTargetData == NULL ) gHyp_util_logError ( 
		 "Network connection does not exist to '%s'", pTargetHost ) ;

      /* If connection does not exist attempt TCP/IP */
      
      /* Try TCP/IP */
      if ( pTargetData == NULL ) {
		socket = gHyp_tcp_request ( pTargetHost, giARservicePort ) ;
		if ( socket != INVALID_SOCKET )
		  pTargetData = gHyp_sock_createNetwork ( pHosts,
						  pTargetHost,
						  targetAddr,
						  socket ) ;

      }
	
      /* If the network host exists, attempt to route the message. */
      if ( pTargetData )
		pTargetChannel = (sChannel*) gHyp_data_getObject( pTargetData ) ;
      else
		pTargetChannel = NULL ;
	
      if ( pTargetChannel ) {
		/* Send message. */
		isRouted = gHyp_sock_putChannel ( pTargetChannel, pMsg, gpsAImsg ) ;
		if ( !isRouted ) {
		  gHyp_data_detach ( pTargetData ) ;
		  gHyp_data_delete ( pTargetData ) ;
		  pTargetData = NULL ;
		  pTargetChannel = NULL ;
		  sprintf ( reason, "Failed to write message to network host '%s'",pTargetHost);
		} 
      }
      else {
		/* The remote host was not resolved. Print the message. */
		sprintf ( reason, "Failed to resolve host '%s'",pTargetHost);
		cond = COND_ERROR ;
		gHyp_util_output ( pMsg ) ;
		gHyp_util_output ( "\n" ) ;
		msgDisplayed = TRUE ;
      }
    }
    else if ( isLocalSender ) {
      /* Route the message to the parent */
      isRouted = gHyp_concept_writeParent ( pConcept, 
					    pMsg, 
					    parentConcept, 
					    parentPath,
					    isConnect,
					    isEventOrQuery ) ;
      if ( !isRouted ) {
		sprintf ( reason, "Failed to write message to parent '%s'",parentPath);
		cond = COND_ERROR ;
      }
    }
    else {
      sprintf ( reason, "Failed to resolve network target '%s'", pTargetHost ) ;
      isRouted = FALSE ;
	  cond = COND_ERROR ;
    }
  }
  else {
    sprintf ( reason, "Failed to resolve network target '%s'", pTargetHost ) ;
	cond = COND_ERROR ;
    isRouted = FALSE ;
  }

  /* Post-processing */
  
  /* Process messages from local senders */
  
  if ( isLocalSender && !isMsgFromUs && 
       !isWildCardSender && pSenderChannel == NULL ) {
    
    if ( senderClient[0] ) {
      
      /* Look for (and create) a client socket on behalf of the sender.*/
      if ( !isDisconnect ) {
		pSenderData = gHyp_sock_createClient ( pClients, senderClient, senderId, FALSE ) ;
      
	/* Set the tokens/values associated if the message is CONNECT. */
	if ( pSenderData && isConnect && isEventOrQuery ) {
	  pSenderChannel = (sChannel*) gHyp_data_getObject ( pSenderData ) ;
	  /****
	  gHyp_channel_initStats ( pSenderChannel,
				   targetId,
				   gHyp_aimsg_tokensValues ( gpsAImsg ) ) ;
	  ****/
	}
      }
    }  
  }

  if ( isRouted ) {

    /* Update the transaction data */
    msgLen = strlen ( pMsg ) ;
    if ( !isMsgFromUs && !isWildCardSender && pSenderChannel )
      gHyp_channel_updateStats ( pSenderChannel, msgLen, FALSE ) ;

  }
  
  /* Process DISCONNECT or ABORT messages to router */
  if (	isLocalTarget ) {

    if ( isMsgToUs ) {

      if (  isDisconnect && pSenderChannel && isLocalSender && 
	    !isMsgFromUs && !isWildCardSender ) {
	
	/* Delete the socket. */
	gHyp_data_detach ( pSenderData ) ;
	gHyp_data_delete ( pSenderData ) ;
	pSenderData = NULL ;
	pSenderChannel = NULL ;
      }

      /* Which instance */
      pAI = gHyp_concept_getNamedInstance ( pConcept, pTargetInstance ) ;
      if ( !pAI )  {
        /* Instance was not found - create new one */
	pAI = gHyp_concept_instantiate (  pConcept, 
					  NULL, /* ok if 4th arg is FALSE */
				          pTargetInstance,
					  targetRoot2,
				          FALSE ) ;
      }
      if ( pAI ) {
	if ( guDebugFlags & DEBUG_DIAGNOSTICS )
	  gHyp_util_logDebug ( FRAME_DEPTH_NULL, DEBUG_DIAGNOSTICS,
			       "Found instance %s#%s",pTargetInstance,pTargetConcept) ;
	gHyp_aimsg_initParse ( gHyp_instance_incomingMsg(pAI), (char*) pMsg ) ;
      }
    }
  }
  
  /* Process messages that were not routed. */
  if ( !isRouted ) {

    if ( !msgDisplayed ) {
      gHyp_util_output ( pMsg ) ;
      gHyp_util_output ( "\n" ) ;
      msgDisplayed = TRUE ;
    }
    gHyp_util_logWarning ( "Message was not routed, reason was '%s'",reason ) ;
    
    if ( pSenderChannel ) {

      /* The message was not properly routed. Return to sender */
      if ( (pMsg = lHyp_router_rts(	senderPath,
					pSenderConcept,
					pSenderHost,
					pMode, 
					pMethod, 
					targetPath,
					gzConcept,
					pTID ))) {

        /* Send message and if it fails, close the sender socket. */
        if ( isLocalSender ) {
	  if ( !gHyp_sock_putChannel ( pSenderChannel, pMsg, gpsAImsg ) ) {
	    gHyp_data_detach ( pSenderData ) ;
	    gHyp_data_delete ( pSenderData ) ;
	    pSenderData = NULL ;
	    pSenderChannel = NULL ;
          }
        }
        else {
  	  if ( !gHyp_sock_putChannel ( pSenderChannel, pMsg, gpsAImsg ) ) {
            gHyp_data_detach ( pSenderData ) ;
            gHyp_data_delete ( pSenderData ) ;
	    pSenderData = NULL ;
	    pSenderChannel = NULL ;
	  }
        }
      }
    }       
  }       

  /* Separate all messages processed */
  gHyp_util_output ( "-------------\n" ) ;
  
  return cond ;
}

int gHyp_router_process ( sConcept *pConcept,
			  sData *pClients,
			  sData *pHosts,
			  char *pMsgBuf, 
			  char *pMsgOff,
			  int nBytes,
			  int maxMessageSize,
			  char *pMsgAddr )
{
  /* Description:
   *
   *	Process one or more messages contained in the message buffer.
   *
   *	Messages should end with three delimiters.
   *
   *	If there are multiple messages in the buffer and the last message
   *	was truncated, then before this routine exits, the truncated message 
   *	is moved up to the start of the buffer so that the next read will 
   *    append the rest of the message to the truncated start.
   *
   * Arguments:
   *
   *	pMsgBuf			[R]
   *	- buffer containing one or more appended messages, NULL terminated
   *
   *	pMsgOff			[R]
   *	- offset in buffer where last read was anchored
   *	  (will equal pMsgBuf unless previous read had truncated message)
   *
   *	nBytes			[R]
   *	- number of bytes from pMsgOff to end of message
   *
   *	maxMessageSize		[R]
   *	- maximum message size supported by fifo, tcp/ip, etc..
   *
   * Return value:
   *
   *	COND_*
   *
   * Modifications:
   *
   *	14-Jan-96		Mike Bergsma
   *	- Q/A check and code review
   */
   
  char
    saveChar,
    *d3 = "|||",       			/* Delimiter string */
    *eob,				/* End Of Buffer */
    *eom,				/* End of Message */
    *pMsg,
    *pNextMsg = pMsgBuf ;
  
  int
    cond=COND_SILENT,
    stat,
    msgLen ;

  /* Process all messages in the buffer */
  eob = pMsgOff + nBytes ;
  do {
  
    /* Point to start of next message */
    
    pMsg = pNextMsg ;
    /* Divide the messages in buffer where ever there is more than |||
     * Ie:Messages begin with a defining | or |||| and end with |||. 
     */
    
    /* Find the first occurance of "|" */
    pMsg = strchr ( pMsg, '|' ) ;
    if ( !pMsg ) {
      /*gHyp_util_debug("Bad message '%s'",pNextMsg);*/
      break ;
    }

    /* Skip across multiple | characters */
    while ( *(pMsg+1) == '|' ) pMsg++ ; 
    
    /* Find the end of the message in the buffer */
    eom = strstr ( pMsg, d3 ) ;
    if ( eom ) {
      /* Found it. Null terminate the message, replacing the last delimiter.*/
      eom += 3 ;
    }
  
    /* Find the next message, if it exists. */
    if ( eom ) {
    
      /* The previous message ended with three delimiters. */

      /* How many bytes total in message? */	
      msgLen = eom - pMsg ;

      /* See if another message follows. (is appended). */
        
      /* Advance past newlines, if they exist */
      while ( (eom < eob) && !isprint(*eom) ) eom++ ;
      
      /* If there remains a printable character, assume its a new message, */
      if ( (eom < eob) && isprint ( *eom ) )
        pNextMsg = eom ;
      else
        pNextMsg = NULL ;

    }
    else {
    
      /* Message did not end with |||. */

      /* The message length is to the end of the buffer. */
      msgLen = eob - pMsg ;

      /* Not all of the message was received.
       * (The read filled the buffer and another read is necessary 
       *  to get the remainder of of the message).
       */
      
      if ( msgLen <= MAX_MESSAGE_SIZE ) {
	
	/* Shift the truncated message to the start of the buffer */
	strncpy ( pMsgBuf, pMsg, msgLen ) ;
	
	/* Remember where the next read will start from. */
	pMsgBuf += msgLen ;
	
	/* Don't process the message, wait until next read gets the rest. */
	pMsg = NULL ;
      }
      else {
        gHyp_util_logError ( "Message did not end with |||" ) ;
	gHyp_util_logError ( "Ignoring message beginning with '%.*s '", 48, 
			     pMsg ) ;
	gHyp_util_output   ( "-------------\n" ) ;
	pMsg = NULL ;
      }

      /* End of buffer, no more messages */
      pNextMsg = NULL ;
    }
        
    /* Check message length */
    if ( msgLen > MAX_MESSAGE_SIZE ) {
      gHyp_util_logError ( "Message exceeds %u byte limit", MAX_MESSAGE_SIZE );
      gHyp_util_logError ( "Ignoring message beginning with '%.*s'", 40, 
			   pMsg ) ;
      gHyp_util_output   ( "-------------\n" ) ;
    }	
    else if ( pMsg ) {

      saveChar = *eom ;
      *eom = '\0' ;

      /* Process the message */
      if ( strncmp ( pMsg, "|SIG", 4 ) == 0  &&
	   ( strncmp ( pMsg, "|SIGMBX||", 9) == 0  ||
	     strncmp ( pMsg, "|SIGALRM||", 10) == 0 ||
	     strncmp ( pMsg, "|SIGPIPE||", 10 ) == 0 ||
	     strncmp ( pMsg, "|SIGHUP||", 9) == 0 ||
	     strncmp ( pMsg, "|SIGINT||", 9) == 0 ||
	     strncmp ( pMsg, "|SIGTERM||", 10) == 0 ) ) {
	/*gHyp_util_debug("SIGNAL MESSAGE");*/
	stat = COND_NORMAL ;
      }
      else
	stat = gHyp_router_message ( pConcept, pClients, pHosts, pMsg, pMsgAddr ) ;

      if ( stat == COND_NORMAL ) cond = COND_NORMAL ;
      *eom = saveChar ;
    }

  } while ( pNextMsg ) ;
  
  /* Null terminate the buffer at the point of the next read.
   * (The 'pMsgBuf' pointer will always be the start of the buffer unless 
   *  a partial message was read, in which case it spans the partial message.)
   */
  *pMsgBuf = '\0' ;

  return cond ;
}
