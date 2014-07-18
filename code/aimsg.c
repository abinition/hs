/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 AbInitio Software                        !
!          ***** Author: Michael D. Bergsma                                   !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/
/*
 * Modifications:
 *
 *	$Log: aimsg.c,v $
 *	Revision 1.16  2004-12-13 04:56:42  bergsma
 *	In unparse, when processing tokens, must strcpy to token variable
 *	before doing util_parse on it.
 *	
 *	Revision 1.15  2004/11/19 03:43:02  bergsma
 *	Initialize transaction id of new message to random8.
 *	
 *	Revision 1.14  2004/10/27 18:24:07  bergsma
 *	HS 3.3.2
 *	1. Fix bug with SQL read when str size is > 512 bytes, use _data_ blobs
 *	2. Fix bug with XML output, forgetting ";" in unicode output.
 *	3. In |TOKEN|VALUE|VALUE| part of message, use unparse on TOKEN
 *	as well as VALUE.
 *	4. Add utility function util_breakStream.
 *	
 *	Revision 1.13  2004/10/21 04:08:19  bergsma
 *	Also apply util_unparse to TOKEN contents.
 *	
 *	Revision 1.12  2004/10/16 00:53:10  bergsma
 *	New function names replace cryptic data functions:
 *	gHyp_data_setStr<n>
 *	gHyp_data_getStr<n>
 *	gHyp_data_newConstant<n>
 *	
 *	Revision 1.11  2003/04/23 22:40:32  bergsma
 *	Values created from a incoming message data was not retaining
 *	unprintable characters if for example, the string ended in \000.
 *	----------------------------------------------------------------------
 *	
 *	Revision 1.10  2003/04/04 16:38:07  bergsma
 *	Remove special consideration of "TOKEN", whether it looks like a variable
 *	or an expression.  DO that logic in mergeData
 *	
 *	Revision 1.9  2003/03/04 01:41:58  bergsma
 *	Assume old style message format in if the leading substring of the TOKEN
 *	field is a valid identifier, not necessarily the entire field.
 *	
 *	Revision 1.8  2003/02/17 09:53:33  bergsma
 *	Flag temporary _tv_ variables as _tvmsg_ and _tvmsg2_
 *	
 *	Revision 1.7  2003/01/12 10:08:41  bergsma
 *	V3.1.0
 *	When the TOKEN is not an indentifier, then do not set the type as a
 *	VARIABLE, and thus do not append the VALUE(s) to the TOKEN.
 *	
 *	Revision 1.6  2003/01/09 00:06:07  bergsma
 *	V3.1.0
 *	1. Added gHyp_aimsg_setTV function
 *	2. Never specify concept instance as concept#concept.  Eliminates redundancy.
 *	
 *	Revision 1.5  2002/11/21 01:21:02  bergsma
 *	Remove ^M
 *	
 *	Revision 1.4  2002/11/20 20:24:19  bergsma
 *	Added targetRoot, the expanded version of targetParent.
 *	
 *	Revision 1.3  2002/09/24 17:18:10  bergsma
 *	In gHyp_aimsg_initParsePartial - which is called by gHyp_router_message
 *	to partially parse the incoming record - set giResolveFlag2 to TRUE
 *	if giResolveFlag1 is already TRUE, just before resolving the host
 *	name of the sender.  Both flags set tells gHyp_tcp_resolveHost that
 *	the host name does not need a getHostbyName or getHostbyAddr lookup.
 *	
 *	Revision 1.2  2002/09/16 17:59:11  bergsma
 *	In support of "described" arguments, if the token is " ", then only include
 *	the values.  See gHyp_fileio_describeData, setting token value to " "
 *	
 */

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ***********************/

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/* Storage for an "Ab Initio Protocol" message, in both an internal (parsed)
 * and external (unparsed) format.
 */
struct AImsg_t 
{ 
  char
    targetInstance[INSTANCE_SIZE+1],		
    targetConcept[OBJECT_SIZE+1],		
    targetParent[OBJECT_SIZE+1],
    targetHost[HOST_SIZE+1],
    targetRoot[OBJECT_SIZE+1],
    mode[MODE_SIZE+1],
    method[METHOD_SIZE+1],
    senderInstance[INSTANCE_SIZE+1],
    senderConcept[OBJECT_SIZE+1],
    senderParent[OBJECT_SIZE+1],
    senderHost[HOST_SIZE+1],
    senderRoot[OBJECT_SIZE+1],
    transactionId[FIELD_SIZE+1],
    timeStamp[FIELD_SIZE+1],
    tokensValues[MAX_MESSAGE_SIZE+1] ;
  sData		*pTV ;	/* Pointer to the message Tokens and their Values */
} ;

/**********************	FUNCTION DEFINITIONS ********************************/

static void lHyp_aimsg_init ( sAImsg * pAImsg )
{
  /* Description:
   *
   *	Initialize an empty message.
   *
   * Arguments:
   *
   *	pAImsg							[W]
   *	- pointer to message object to initialize
   *
   * Return value:
   *
   *	none
   *

   *
   */
  
  pAImsg->targetInstance[0] = '\0' ;
  pAImsg->targetConcept[0] = '\0' ;
  pAImsg->targetParent[0] = '\0' ;
  pAImsg->targetRoot[0] = '\0' ;
  pAImsg->targetHost[0] = '\0' ;		
  pAImsg->mode[0] = '\0' ;
  pAImsg->method[0] = '\0' ;
  pAImsg->senderInstance[0] = '\0' ;		
  pAImsg->senderConcept[0] = '\0' ;
  pAImsg->senderParent[0] = '\0' ;
  pAImsg->senderRoot[0] = '\0' ;
  pAImsg->senderHost[0] = '\0' ;
  strcpy ( pAImsg->transactionId, gHyp_util_random8() ) ;
  pAImsg->timeStamp[0] = '\0' ;
  pAImsg->tokensValues[0] = '\0' ;

  return ;
}

sAImsg * gHyp_aimsg_new ()
{
  /* Description:
   *
   *	Malloc a new message object,
   *
   * Arguments:
   *
   *	none
   *
   * Return value:
   *
   *	pointer to message object
   *

   */
  sAImsg *pAImsg = (sAImsg*) AllocMemory ( sizeof ( sAImsg ) ) ;
  assert ( pAImsg ) ;
  /*gHyp_util_logInfo("AIMSG %p",pAImsg);*/
  /* Start a new token/value data set */
  pAImsg->pTV = gHyp_data_new ( "_tvmsg_" ) ;
  
  /* Initialize the buffers in the structure. */
  lHyp_aimsg_init ( pAImsg ) ;

  return pAImsg ;
}

void gHyp_aimsg_delete ( sAImsg *pAImsg ) 
{
  /* Description:
   *
   *	Free a message object.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to sAImsg object 
   *
   * Return value:
   *
   *	none
   *

   */
   
  if ( pAImsg->pTV ) {
    gHyp_data_delete ( pAImsg->pTV ) ;
    pAImsg->pTV = NULL ;
  }
  ReleaseMemory ( pAImsg ) ;

  return ;
}

sData *gHyp_aimsg_tv ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to Tokens and Values of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to sAImsg object 
   *
   * Return value:
   *
   *	pointer to data structure of message
   *

   */
  return pAImsg->pTV ;
}


void gHyp_aimsg_setTV ( sAImsg *pAImsg, sData *pTV )
{
  /* Description:
   *
   *	Get pointer to Tokens and Values of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to sAImsg object 
   *
   * Return value:
   *
   *	pointer to data structure of message
   *

   */
  if ( pAImsg->pTV ) gHyp_data_delete ( pAImsg->pTV ) ;
  pAImsg->pTV = pTV ;
}

char *gHyp_aimsg_targetInstance ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to target instance of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to sAImsg object 
   *
   * Return value:
   *
   *	pointer to target instance
   *

   */
  return pAImsg->targetInstance ;
}

char *gHyp_aimsg_targetConcept ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to target object of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to message object 
   *
   * Return value:
   *
   *	pointer to target object
   *

   */
  return pAImsg->targetConcept ;
}

char *gHyp_aimsg_targetParent ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to target parent of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to message object 
   *
   * Return value:
   *
   *	pointer to target parent
   *
   *
   */
  return pAImsg->targetParent ;
}

char *gHyp_aimsg_targetRoot ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to target root of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to message object 
   *
   * Return value:
   *
   *	pointer to target root
   *
   *
   */
  return pAImsg->targetRoot ;
}

char *gHyp_aimsg_targetHost ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to target host of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to message object 
   *
   * Return value:
   *
   *	pointer to target host
   *

   */
  return pAImsg->targetHost ;
}

char *gHyp_aimsg_senderInstance ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to sender instance of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to message object 
   *
   * Return value:
   *
   *	pointer to sender instance
   *

   */
  return pAImsg->senderInstance ;
}

char *gHyp_aimsg_senderConcept ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to sender object of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to message object 
   *
   * Return value:
   *
   *	pointer to sender object
   *

   */
  return pAImsg->senderConcept ;
}


char *gHyp_aimsg_senderParent ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to sender parent of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to message object 
   *
   * Return value:
   *
   *	pointer to sender object
   *

   */
  return pAImsg->senderParent ;
}

char *gHyp_aimsg_senderRoot ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to sender root of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to message object 
   *
   * Return value:
   *
   *	pointer to sender root
   *

   */
  return pAImsg->senderRoot ;
}

char *gHyp_aimsg_senderHost ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get pointer to sender host of a message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to sAImsg object 
   *
   * Return value:
   *
   *	pointer to sender host
   *

   */
  return pAImsg->senderHost ;
}

void gHyp_aimsg_getTargetId ( sAImsg *pAImsg, char *pTargetId )
{

  if ( *(pAImsg->targetInstance) &&
       strcmp ( pAImsg->targetInstance, pAImsg->targetConcept ) != 0 )
    /* Instance name is specified and is not the same as the concept name. */
    sprintf (	pTargetId,
    		"%s#%s",
		pAImsg->targetInstance,
		pAImsg->targetConcept ) ;
  else
    /* Only object name is specified */
    sprintf (	pTargetId,
    		"%s",
		pAImsg->targetConcept ) ;
  return ;
}

void gHyp_aimsg_getTargetPath ( sAImsg *pAImsg, char *pTargetPath )
{

  if ( *(pAImsg->targetInstance) &&
       strcmp ( pAImsg->targetInstance, pAImsg->targetConcept ) != 0 )
    /* Instance name is specified */
    sprintf (	pTargetPath,
    		"%s#%s%s",
		pAImsg->targetInstance,
		pAImsg->targetConcept,
		pAImsg->targetRoot ) ;
  else
    /* Only object name is specified */
    sprintf (	pTargetPath,
    		"%s%s",
		pAImsg->targetConcept,
		pAImsg->targetRoot ) ;
  return ;
}

void gHyp_aimsg_getTargetAddress ( sAImsg *pAImsg, char *pTargetPath )
{
  char host[HOST_SIZE+1];

  if ( strcmp ( pAImsg->targetHost, gzLocalHost ) != 0 ||
       strcmp ( pAImsg->senderHost, gzLocalHost ) != 0)
    sprintf ( host, "@%s", pAImsg->targetHost ) ;
  else
    host[0] = '\0' ;

  if ( *(pAImsg->targetInstance) &&
       strcmp ( pAImsg->targetInstance, pAImsg->targetConcept ) != 0 )
    /* Instance name is specified */
    sprintf (	pTargetPath,
    		"%s#%s%s%s",
		pAImsg->targetInstance,
		pAImsg->targetConcept,
		pAImsg->targetRoot,
		host ) ;
  else
    /* Only object name is specified */
    sprintf (	pTargetPath,
    		"%s%s%s",
		pAImsg->targetConcept,
		pAImsg->targetRoot,
		host) ;


  return ;
}

void gHyp_aimsg_getSenderId ( sAImsg *pAImsg, char *pSenderId )
{
  /* Description:
   *
   *	Get the sender id.
   *	The sender Id is the [instance.]object less the host name.
   *	It is used to uniquely identify a sender on a given host.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- pointer to sAImsg object 
   *
   *	pSenderId	[W]
   *	- pointer to the sender Id
   *
   * Return value:
   *
   *	none
   *

   */
  if ( *(pAImsg->senderInstance) &&
       strcmp ( pAImsg->senderInstance, pAImsg->senderConcept ) != 0 )
    /* Instance name is specified */
    sprintf (	pSenderId,
    		"%s#%s",
		pAImsg->senderInstance,
		pAImsg->senderConcept ) ;
  else
    /* Only object name is specified */
   sprintf (	pSenderId,
    		"%s",
    		pAImsg->senderConcept ) ;
  return ;
}

void gHyp_aimsg_getSenderPath ( sAImsg *pAImsg, char *pSenderPath )
{

  if ( *(pAImsg->senderInstance) &&
       strcmp ( pAImsg->senderInstance, pAImsg->senderConcept ) != 0 )
    /* Instance name is specified */
    sprintf (	pSenderPath,
    		"%s#%s%s",
		pAImsg->senderInstance,
		pAImsg->senderConcept,
		pAImsg->senderRoot ) ;
  else
    /* Only object name is specified */
   sprintf (	pSenderPath,
    		"%s%s",
    		pAImsg->senderConcept,
		pAImsg->senderRoot ) ;
  return ;
}

void gHyp_aimsg_getSenderAddress ( sAImsg *pAImsg, char *pSenderAddr )
{
  char host[HOST_SIZE+1];

  if ( strcmp ( pAImsg->senderHost, gzLocalHost ) != 0 ||
       strcmp ( pAImsg->targetHost, gzLocalHost ) != 0)
    sprintf ( host, "@%s", pAImsg->senderHost ) ;
  else
    host[0] = '\0' ;

  if ( *(pAImsg->senderInstance) &&
       strcmp ( pAImsg->senderInstance, pAImsg->senderConcept ) != 0 )
    /* Instance name is specified */
    sprintf (	pSenderAddr,
    		"%s#%s%s%s",
		pAImsg->senderInstance,
		pAImsg->senderConcept,
		pAImsg->senderRoot,
		host ) ;
  else
    /* Only object name is specified */
   sprintf (	pSenderAddr,
    		"%s%s%s",
    		pAImsg->senderConcept,
		pAImsg->senderRoot,
		host ) ;

  return ;
}

sLOGICAL gHyp_aimsg_setTargetId ( sAImsg *pAImsg, char *pTargetId )
{
  /* Description:
   *
   *	Set a target id in a message.
   *	This only sets the address internally, the message should be
   *	unparsed to get the address in the external format.
   *
   * Arguments:
   *
   *	pAImsg		[R/W]
   *	- message object to set address into
   *
   *	pSenderId	[R]
   *	- sender id to set
   *
   * Return value:
   *
   *	TRUE if id was set correctly, FALSE otherwise
   *
   */
  char	target[OBJECT_SIZE+1] ;

  /* Append the host name */
  sprintf ( target, "%s%s@%s", pTargetId, pAImsg->targetParent, pAImsg->targetHost ) ;

  /* Re-parse the address. */
  return gHyp_util_breakTarget (	target,
  					pAImsg->targetInstance,
  					pAImsg->targetConcept,
  					pAImsg->targetParent,
  					pAImsg->targetRoot,
					pAImsg->targetHost ) ;
}

char *gHyp_aimsg_mode ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get the mode of the message (event, query, or reply).
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- message object 
   *
   * Return value:
   *
   *	pointer to mode name (event, query, reply).
   *

   */
  return pAImsg->mode ;
}

char *gHyp_aimsg_method ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Get the name of the method specified in the message.
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- message object 
   *
   * Return value:
   *
   *	pointer to method name
   *

   */
  return pAImsg->method ;
}

char *gHyp_aimsg_getTID ( sAImsg *pAImsg )
{
  return pAImsg->transactionId ;
}

char *gHyp_aimsg_getTS ( sAImsg *pAImsg )
{
  return pAImsg->timeStamp ;
}

void gHyp_aimsg_setTID ( sAImsg *pAImsg, char* tid )
{
  strcpy ( pAImsg->transactionId, tid ) ;
}

void gHyp_aimsg_setTS ( sAImsg *pAImsg, char* ts )
{
  strcpy ( pAImsg->timeStamp, ts ) ;
}

sLOGICAL gHyp_aimsg_isQuery ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Returns true if the message is a query
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- message object 
   *
   * Return value:
   *
   *	TRUE if message mode is query, FALSE otherwise
   *

   */
  return ( strcmp ( pAImsg->mode, "query" ) == 0 ) ;
}

sLOGICAL gHyp_aimsg_isEvent ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Returns true if the message is a event
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- message object 
   *
   * Return value:
   *
   *	TRUE if message mode is event, FALSE otherwise
   *

   */
  return ( strcmp ( pAImsg->mode, "event" ) == 0 ) ;
}

sLOGICAL gHyp_aimsg_isReply ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Returns true if the message is a reply
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- message object 
   *
   * Return value:
   *
   *	TRUE if message mode is reply, FALSE otherwise
   *

   */
  return ( strcmp ( pAImsg->mode, "reply" ) == 0 ) ;
}

char* gHyp_aimsg_unparse ( sAImsg *pAImsg )
{
  /* Description:
   *
   *	Re-create (unparse) the message in external (ascii) format from
   *	internal format (from pTV)
   *
   * Arguments:
   *
   *	pAImsg		[R]
   *	- message object 
   *
   * Return value:
   *
   *	Returns pointer to message if successfully created, NULL otherwise
   *

   */
  
  int
    context1,
    context2,
    msglen,
    fieldlen,
    tokenlen,
    valuelen,
    tvlen,
    ss1,
    ss2 ;

  char
    target[TARGET_SIZE+1],
    sender[SENDER_SIZE+1],
    value1[VALUE_SIZE+1],
    value2[VALUE_SIZE+1],
    *pMsg,
    *pEndBuf,
    *pToken,
    *pValue,
    *pNullValue = " " ;
  
  sData
    *pThisToken,
    *pThisValue,
    *pTV ;
  
  sBYTE
    tokenType ;

  sLOGICAL
    isVector1,
    isVector2 ;

  static char
    message[MAX_MESSAGE_SIZE+1] ;

  /* Initialize */
  pTV = pAImsg->pTV ;				/* Current tokens and values */
  pMsg = message ;				/* Current message pointer */

  /* End of buffer does not include the last two delimeters and the null */
  pEndBuf = pMsg + MAX_MESSAGE_SIZE - 3 ;	

  /* Step 1: Construct the header. */
  gHyp_aimsg_getTargetAddress ( pAImsg, target ) ;
  gHyp_aimsg_getSenderAddress ( pAImsg, sender ) ;
  
  msglen = sprintf ( 	pMsg,
			"|%s|%s|%s|%s|",
			target,
			pAImsg->mode,
			pAImsg->method,
			sender ) ;

  pMsg += msglen ;

  /* Add transaction id and timestamp, if specified */
  if ( *pAImsg->transactionId ) {
    fieldlen = sprintf ( pMsg, 
			 "%s|", 
			 pAImsg->transactionId ) ;
    pMsg += fieldlen ;
    msglen += fieldlen ;
  }
  if ( *pAImsg->timeStamp ) {
    fieldlen = sprintf ( pMsg,
			 "%s|", 
			 pAImsg->timeStamp ) ;
    pMsg += fieldlen ;
    msglen += fieldlen ;
  }

  /* Append the tokens and values */

  /* If there's no allocated storage for tokens and values, then we're done */
  if ( pTV == NULL ) {

    /* Tokens and values do not come from the sData list, they are external */
    tvlen = strlen ( pAImsg->tokensValues ) ;
     
    if ( tvlen > 0 ) {
      *pMsg++ = DEFAULT_DELIMITER ;
      strcpy ( pMsg, pAImsg->tokensValues ) ;
      pMsg += tvlen ;
      *pMsg++ = DEFAULT_DELIMITER ;
    }
  }
  else {

    /* Append the tokens and values */
    isVector1 = ( gHyp_data_getDataType ( pTV ) > TYPE_STRING ) ;
    pThisToken = NULL ;
    ss1 = -1 ; context1 = -1;
    while ( (pThisToken = gHyp_data_nextValue ( pTV, 
						pThisToken, 
						&context1,
						ss1 ))) {

      if ( (tokenlen = gHyp_data_getStr_msg ( pThisToken, 
					  value1, 
					  VALUE_SIZE, 
					  context1,
					  isVector1 ) ) != -1 ) {

        /* Make sure there's enough room for "|token|" */
	gHyp_util_trim ( value1 ) ;
	pToken = value1 ;
	tokenlen = strlen ( pToken ) ;

        if ( ( pMsg + tokenlen + 2 ) > pEndBuf ) {
    	  gHyp_util_logError ( 
		"Message truncated before %d byte limit", MAX_MESSAGE_SIZE ) ;
	  break ;
        }	
		
        /* Append the token */
        *pMsg++ = DEFAULT_DELIMITER ;
	if ( tokenlen > 0 ) {
	  strcpy ( pMsg, pToken ) ;
	  pMsg += tokenlen ;
	  *pMsg++ = DEFAULT_DELIMITER ;
	}
        *pMsg = '\0' ; 
	    
	/* If tokens are not variables, then there are no values */
	tokenType = gHyp_data_getTokenType (pThisToken) ;
	if ( tokenType != TOKEN_VARIABLE ) continue ;

	/* Append the token values */
	isVector2 = ( gHyp_data_getDataType (pThisToken) > TYPE_STRING ) ;
	pThisValue = NULL ;
	ss2 = -1 ; context2 = -1 ;
	while ((pThisValue = gHyp_data_nextValue( pThisToken,
						  pThisValue,
						  &context2,
						  ss2 ) ) ) {
	    
	  /* Get the string, converting to external form */
	  valuelen = gHyp_data_getStr_msg ( pThisValue,
					 value2,
					 VALUE_SIZE,
					 context2,
					 isVector2 ) ;
	  pValue = value2 ;
          if ( valuelen == 0 ) {
            pValue = pNullValue ;
            valuelen = 1 ;
          }

          /* Make sure there's room for "value|" in message */
          if ( ( pMsg + valuelen + 1 ) > pEndBuf ) {
    	    gHyp_util_logError ( 
		"Message truncated before %d byte limit", MAX_MESSAGE_SIZE ) ;
	    break ;
	  }
	
	  /* Append the value */	
          strcpy ( pMsg, pValue ) ;
          pMsg += valuelen ;
          *pMsg++ = DEFAULT_DELIMITER ;
  	  *pMsg = '\0' ;
        } 
      } 
    }
  }  

  /* Add final || and terminating null */
  *pMsg++ = DEFAULT_DELIMITER ;
  *pMsg++ = DEFAULT_DELIMITER ;
  *pMsg = '\0' ;
  
  return message ;
}		  

sLOGICAL gHyp_aimsg_parse ( sAImsg * pAImsg, char *pMsg )
{
  /* Description:
   *
   *	Parse the message from external (ascii) format into
   *	internal format.  If pAImsg->pTV is NULL, then the tokens
   *	and values are not broken up into separate components and
   *	put into the pTV sData linked list
   *
   * Arguments:
   *
   *	pAImsg		[R/W]
   *	- message object 
   *
   *	pMsg 		[R]
   *	- message to parse 
   *
   * Return value:
   *
   *	TRUE if message was successfully parse, FALSE otherwise
   *

   */
  char		
    *pElement,
    *pRemainder,
    *pStr,
    target[TARGET_SIZE+1],
    sender[SENDER_SIZE+1],
    token[TOKEN_SIZE+1],
    message[MAX_MESSAGE_SIZE+1] ;

  int
    n,
    state,
    fieldCount = 0,
    tvlen,
    elementlen ;

  sData
    *pToken = NULL,
    *pValue,
    *pTV = pAImsg->pTV ;

  /* Copy the message into local working storage */
  if ( !pMsg ) return gHyp_util_logError ("No message to parse") ;
  if ( !*pMsg ) return gHyp_util_logError ("Message is zero length") ;
  n = MIN ( strlen ( pMsg ), MAX_MESSAGE_SIZE ) ;
  strncpy ( message, pMsg, n ) ;
  message[n] = '\0' ;
  pMsg = message ;
  target[0] = '\0' ;
  sender[0] = '\0' ;

  if ( *pMsg == DEFAULT_DELIMITER ) 
    /* Delimiter is correct */
    pMsg++ ;
  else
    /* Delimiter is not correct */
    return gHyp_util_logError ( "Message delimiter '|' is missing" ) ;

  state = EXPECT_TARGET ;
  pRemainder = pMsg ;
  while ( (pElement = strtok ( pMsg, "|" )) ) {

    /* The message is parsed using the strtok function:
     *
     *	- the delimiter "|" between the message components are replaced
     *	  by terminating NULLs, making the components ready for string
     *	  functions.
     *	- pRemainder always points to the start of the remainder of the message
     *	  to parse.  Initially, it points to the first character past the
     *	  first delimiter in the message.
     *	- after strtok is called, pElement should point to a zero terminated
     *	  string which will be the next component in the message
     *	- normally, each component is separated by a single delimter, and 
     *    after each call to strtok, pElement == pRemainder
     *	- pElement-pRemainder+1 will always equal the number of delimiters
     *	  found together between components.  In the header of the
     *	  message, multiple delimiters together means that one or more 
     *	  components of the header is missing. In the body of the message,
     *	  two or more delimters together means the end of a token/value list
     *	  and the start of a new token/value list.
     */
     
    /* Check for multiple delimters */
    if ( pElement > pRemainder) {

      /* If parsing the message header, then just skip the missing elements */
      if ( state < EXPECT_TOKEN ) state += (pElement-pRemainder) ;

      /* If curently at the EXPECT_VALUE state, reset back to EXPECT_TOKEN. */
      if ( state > EXPECT_TOKEN ) state = EXPECT_TOKEN ;
    }
    
    /* Prepare subsequent calls to strtok by setting pMsg to NULL */
    if ( pMsg ) pMsg = NULL ;
    
    /* Break if end of message */
    if ( *pElement == '\n' ) break ;
    
    elementlen = strlen ( pElement ) ;
    switch ( state ) {

      case EXPECT_TARGET:

	gHyp_util_lowerCase ( pElement, elementlen ) ;
	if ( elementlen > TARGET_SIZE )
	  return gHyp_util_logError ( 
		"Target '%s' exceeds %d characters", pElement, TARGET_SIZE ) ;
      	strcpy ( target, pElement ) ;
	/* Next state is MODE */
	state++ ;	
	break ;

      case EXPECT_MODE:		

	gHyp_util_lowerCase ( pElement, elementlen ) ;
	if ( 	strcmp ( pElement, "event" ) != 0 && 
	     	strcmp ( pElement, "query" ) != 0 &&
		strcmp ( pElement, "reply" ) != 0 )
	  return gHyp_util_logError ( 
		"Mode is not event or query in message" ) ;
	strcpy ( pAImsg->mode, pElement ) ;
	state++ ;	/* Next state expects METHOD */
	break ;

      case EXPECT_METHOD:	

	if ( elementlen > METHOD_SIZE )
	  return gHyp_util_logError ( 
		"Method '%s' exceeds %d characters", pElement, METHOD_SIZE ) ;
      	strcpy ( pAImsg->method, pElement ) ;
	state++ ;	/* Next state expects SENDER */
	break ;

      case EXPECT_SENDER:	

	gHyp_util_lowerCase ( pElement, elementlen ) ;
	if ( elementlen > TARGET_SIZE )
	  return gHyp_util_logError ( 
		"Sender '%s' exceeds %d characters", pElement, TARGET_SIZE ) ;
      	strcpy ( sender, pElement ) ;
	state++ ;	/* Next state expects a FIELD */
	break ;

      case EXPECT_FIELD:	

	fieldCount++ ;
	if ( fieldCount > MAX_FIELDS ) 
	  return gHyp_util_logError ( 
		"More than %d header fields in message", MAX_FIELDS ) ;
	if ( elementlen > FIELD_SIZE )
	  return gHyp_util_logError ( 
		"Field '%s' exceeds %d characters", pElement, FIELD_SIZE ) ;
 	if ( fieldCount == FIELD_TID ) 
	  strcpy ( pAImsg->transactionId, pElement ) ;
	else if ( fieldCount == FIELD_TIMESTAMP )
	  strcpy ( pAImsg->timeStamp, pElement ) ;

	break ;

      case EXPECT_TOKEN:	

	if ( elementlen > TOKEN_SIZE )
	  return gHyp_util_logError ( 
		"Token '%s' exceeds %d characters", pElement, TOKEN_SIZE ) ;
	state++ ;
	if ( !pTV ) break ;
	pToken = gHyp_data_new ( NULL ) ;
	/* Convert the string to internal form */
	strcpy ( token, pElement ) ;
	n = gHyp_util_parseString ( token ) ;
	token[n] = '\0' ;
	gHyp_data_setVariable ( pToken, token, TYPE_STRING ) ;
	gHyp_data_append ( pTV, pToken ) ;
	break ;

      case EXPECT_VALUE:	
      default:			

	if ( elementlen > VALUE_SIZE )
	  return gHyp_util_logError ( 
		"Value '%s' exceeds %d characters", pElement, VALUE_SIZE ) ;
	if ( !pTV ) break ;
	pValue = gHyp_data_new ( NULL ) ;
	/* Convert the string to internal form */
	n = gHyp_util_parseString ( pElement ) ;
	gHyp_data_setStr_n ( pValue, pElement, n ) ;
	gHyp_data_append ( pToken, pValue ) ;
    }

    /* Advance to the next part of the message to parse */
    pRemainder = pElement + elementlen + 1 ;

    /* Check to see we're at the end of the header.  If so, save
     * the entire message body as a string, prior to hacking it up with
     * the strtok function 
     */
    if ( state == EXPECT_FIELD && *pRemainder == DEFAULT_DELIMITER ) {

      /* Advance past all delimiters */
      pStr = pRemainder ;
      while ( *pStr == DEFAULT_DELIMITER ) pStr++ ;

      /* Copy in what's left - maybe nothing */
      strcpy ( pAImsg->tokensValues, pStr ) ;
      /* Remove any trailing delimiters. */
      tvlen = strlen ( pAImsg->tokensValues ) - 1 ;
      while ( tvlen >= 0 && 
	      pAImsg->tokensValues[tvlen] == DEFAULT_DELIMITER )
	pAImsg->tokensValues[tvlen--] = '\0' ;  
    }
  }

  if ( !target[0] ) 
    return gHyp_util_logError ( "Target not specified in message" ) ;

  if ( !*pAImsg->mode ) 
    return gHyp_util_logError ( "Mode not specified in message" ) ;
				
  if ( !*pAImsg->method ) 
    return gHyp_util_logError ( "Method not specified in message" ) ;
			
  if ( !sender[0] ) 
    return gHyp_util_logError ( "Sender not specified in message" ) ;
								
  /* Parse up target and sender into component parts */
  if ( !gHyp_util_breakTarget ( 	target, 
					pAImsg->targetInstance,
					pAImsg->targetConcept,
					pAImsg->targetParent,
					pAImsg->targetRoot,
					pAImsg->targetHost ) ) return FALSE ;

  /* See gHyp_tcp_resolveHost for explanation of next statement */
  if ( giResolveFlag1 ) giResolveFlag2 = TRUE ;

  if ( !gHyp_util_breakTarget ( 	sender,
					pAImsg->senderInstance,
					pAImsg->senderConcept,
					pAImsg->senderParent,
					pAImsg->senderRoot,
					pAImsg->senderHost ) ) return FALSE ;

  return TRUE ;
}

sLOGICAL gHyp_aimsg_initParse ( sAImsg *pAImsg, char *pMsg )
{
  /* Description:
   *
   *	Parse a message from an external ascii format into an internal
   *	representation.
   *
   * Arguments:
   *
   *	pAImsg				[R/W]
   *	- message object to initialize 
   *
   *	pMsg				[R]
   *	- external ascii message to parse
   *
   * Return value:
   *
   *	TRUE if succuessfully initialized, FALSE otherwise
   *

   *
   *	25-Jan-95		Mike Bergsma
   *	- make more memory efficient
   */
   
  /* Delete any tokens and values */
  if ( pAImsg->pTV ) gHyp_data_delete ( pAImsg->pTV ) ;

  /* Start a new token/value data set */
  pAImsg->pTV = gHyp_data_new ( "_tvmsg2_" ) ;

  /* Initialize a new AImsg structure */
  lHyp_aimsg_init ( pAImsg ) ;
  
  /* Create the internal message. */
  return gHyp_aimsg_parse ( pAImsg, pMsg ) ;
}

sLOGICAL gHyp_aimsg_initParsePartial ( sAImsg *pAImsg, char *pMsg )
{
  /* Description:
   *
   *	Partially parse a message (header only) from from an external ascii 
   *	format.
   *
   * Arguments:
   *
   *	pAImsg				[R/W]
   *	- message object to initialize 
   *
   *	pMsg				[R]
   *	- external ascii message to parse
   *
   * Return value:
   *
   *	TRUE if succuessfully initialized, FALSE otherwise
   *

   *
   */
   
  /* Delete any tokens and values */
  if ( pAImsg->pTV ) gHyp_data_delete ( pAImsg->pTV ) ;
  pAImsg->pTV = NULL ;

  /* Initialize a new AImsg structure */
  lHyp_aimsg_init ( pAImsg ) ;
  
  /* Parse the message. */
  return gHyp_aimsg_parse ( pAImsg, pMsg ) ;
}

char * gHyp_aimsg_initUnparse ( 	sAImsg	*pAImsg,
					char	*pTarget,
					char	*pMode,
					char	*pMethod,
					char	*pSender,
					char    *pTID,
					char    *pTS,
					sData	*pTV )
{
  /* Description:
   *
   *	Unparse a message from an internal representation to an external
   *	ascii message string.
   *
   * Arguments:
   *
   *	pAImsg				[R/W]
   *	- message object to initialize
   *
   *	pTarget				[R]
   *	- target name of message
   *
   *	pMode				[R]
   *	- mode name of message
   *
   *	pMethod				[R]
   *	- method name of message
   *
   *	pSender				[R]
   *	- sender name of message
      *
   *	pTID				[R]
   *	- transaction id of message
      *
   *	pTS				[R]
   *	- timestamp of message
   *
   *	pTV				[R]
   *	- pointer to tokens and values
   *
   * Return value:
   *
   *	Returns pointer to message if successfully created, NULL otherwise
   *
   */
   
  /* Initialize the message object */
  lHyp_aimsg_init ( pAImsg ) ;
  
  /* Initialize the header of the message */

  /* Check target and sender */
  if ( !gHyp_util_breakTarget ( pTarget,
  				pAImsg->targetInstance,
  				pAImsg->targetConcept,
				pAImsg->targetParent,
				pAImsg->targetRoot,
  				pAImsg->targetHost ) ) return NULL ;

  if ( !gHyp_util_breakTarget ( pSender,
  				pAImsg->senderInstance,
  				pAImsg->senderConcept,
				pAImsg->senderParent,
				pAImsg->senderRoot,
  				pAImsg->senderHost ) ) return NULL ;

  /* Check mode */
  if ( strlen ( pMode ) > MODE_SIZE ) {
    gHyp_util_logError ( 
      "Mode '%s' exceeds %d characters", pMode, MODE_SIZE ) ;
    return NULL ;
  }
  strcpy ( pAImsg->mode, pMode ) ;

  /* Check method */
  if ( strlen ( pMethod ) > METHOD_SIZE ) {
    gHyp_util_logError ( 
      "Method '%s' exceeds %d characters", pMethod, METHOD_SIZE ) ;
    return NULL ;
  }
  strcpy ( pAImsg->method, pMethod ) ;

  /* Add transaction id and timestamp, if specified */
  if ( pTID ) {
    if ( strlen ( pTID ) > FIELD_SIZE ) {
      gHyp_util_logError ( 
        "Transaction Id '%s' exceeds %d characters", pTID, MODE_SIZE ) ;
      return NULL ;
    }
    strcpy ( pAImsg->transactionId, pTID ) ;
  }
  if ( pTS ) {
    if ( strlen ( pTS ) > FIELD_SIZE ) {
      gHyp_util_logError ( 
        "TimeStamp '%s' exceeds %d characters", pTS, MODE_SIZE ) ;
      return NULL ;
    }
    strcpy ( pAImsg->timeStamp, pTS ) ;
  }


  /* Delete any previous tokens and values */
  if ( pAImsg->pTV ) gHyp_data_delete ( pAImsg->pTV ) ;

  /* Add the data */
  pAImsg->pTV = pTV ;

  /* Create the external message. */
  return gHyp_aimsg_unparse ( pAImsg ) ;
}


char * gHyp_aimsg_initUnparsePartial ( 	sAImsg	*pAImsg,
					char	*pTarget,
					char	*pMode,
					char	*pMethod,
					char	*pSender,
					char    *pTID,
					char    *pTS,
					sData	*pTV )
{
  /* Description:
   *
   *	Unparse a message from an internal representation to an external
   *	ascii message string - HEADER ONLY!
   *
   * Arguments:
   *
   *	pAImsg				[R/W]
   *	- message object to initialize
   *
   *	pTarget				[R]
   *	- target name of message
   *
   *	pMode				[R]
   *	- mode name of message
   *
   *	pMethod				[R]
   *	- method name of message
   *
   *	pSender				[R]
   *	- sender name of message
      *
   *	pTID				[R]
   *	- transaction id of message
      *
   *	pTS				[R]
   *	- timestamp of message
   *
   *	pTV				[R]
   *	- pointer to tokens and values
   *
   * Return value:
   *
   *	Returns pointer to message if successfully created, NULL otherwise
   *
   */
   
  /* Initialize the message object */
  lHyp_aimsg_init ( pAImsg ) ;
  
  /* Initialize the header of the message */

  if ( !gHyp_util_breakTarget ( pTarget,
  				pAImsg->targetInstance,
  				pAImsg->targetConcept,
				pAImsg->targetParent,
				pAImsg->targetRoot,
  				pAImsg->targetHost ) ) return NULL ;

  if ( !gHyp_util_breakTarget ( pSender,
  				pAImsg->senderInstance,
  				pAImsg->senderConcept,
				pAImsg->senderParent,
				pAImsg->senderRoot,
 				pAImsg->senderHost ) ) return NULL ;
  
  /* Check mode */
  if ( strlen ( pMode ) > MODE_SIZE ) {
    gHyp_util_logError ( 
      "Mode '%s' exceeds %d characters", pMode, MODE_SIZE ) ;
    return NULL ;
  }
  strcpy ( pAImsg->mode, pMode ) ;

  /* Check method */
  if ( strlen ( pMethod ) > METHOD_SIZE ) {
    gHyp_util_logError ( 
      "Method '%s' exceeds %d characters", pMethod, METHOD_SIZE ) ;
    return NULL ;
  }
  strcpy ( pAImsg->method, pMethod ) ;

  /* Add transaction id and timestamp, if specified */
  if ( pTID ) {
    if ( strlen ( pTID ) > FIELD_SIZE ) {
      gHyp_util_logError ( 
        "Transaction Id '%s' exceeds %d characters", pTID, MODE_SIZE ) ;
      return NULL ;
    }
    strcpy ( pAImsg->transactionId, pTID ) ;
  }
  if ( pTS ) {
    if ( strlen ( pTS ) > FIELD_SIZE ) {
      gHyp_util_logError ( 
        "TimeStamp '%s' exceeds %d characters", pTS, MODE_SIZE ) ;
      return NULL ;
    }
    strcpy ( pAImsg->timeStamp, pTS ) ;
  }


  /* Delete any previous tokens and values */
  if ( pAImsg->pTV ) gHyp_data_delete ( pAImsg->pTV ) ;

  /* Add the data */
  pAImsg->pTV = pTV ;

  return pAImsg->transactionId ;
}

char *gHyp_aimsg_tokensValues ( sAImsg *pAImsg )
{
  /* Description:
   *
   *    Get pointer to Tokens and Values of a message.
   *
   * Arguments:
   *
   *    pAImsg          [R]
   *    - pointer to sAImsg object 
   *
   * Return value:
   *
   *    pointer to ascii data structure of message
   *
   */
  return pAImsg->tokensValues ;
}
