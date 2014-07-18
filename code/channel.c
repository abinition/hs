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
 * $Log: channel.c,v $
 * Revision 1.7  2008-02-12 23:03:52  bergsma
 * V3.8.2 (compiled with VS C 2008)
 *
 * Revision 1.6  2006-08-08 20:48:27  bergsma
 * Added setTarget, setPath, setSocket
 *
 * Revision 1.5  2005/01/31 06:05:38  bergsma
 * Added gHyp_channel_offset  and gHyp_channel_setOffset
 *
 * Revision 1.4  2004/06/13 14:05:07  bergsma
 * Cosmetic log statements.
 *
 * Revision 1.3  2004/04/29 02:06:37  bergsma
 * Added pointer to SSL structure.
 *
 * Revision 1.2  2002/11/12 20:08:36  bergsma
 * Removed deprecated tokenValues and sender variables, plus associated functions
 *
 *
 */

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ***********************/
#include "auto.h"	/* System Interface and Function Prototypes */

#include <sys/stat.h>

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/* Channel structure for devices that read and write */
struct channel_t
{
  char		target[TARGET_SIZE+1] ;
  char		path[MAX_PATH_SIZE+1] ;
  short		flags ;
  SOCKET	fd ;
  OVERLAPPED	overlapped ;
  sSSL*		pSSL ;
  char		buf[MAX_BUFFER_SIZE];	/* Read buffer */
  int		offset ;		/* Offset in buffer to start read */
  int		maxMsgSize ;		/* Calculated maximum message size */
  int		nBytes ;		/* Number of bytes read */
  time_t	createTime ;		/* When channel was created */
  time_t	updateTime ;		/* Last time channel was accessed */
  int           inMessageCount ;        /* Total messages written to channel */
  int           outMessageCount ;       /* Total messages read from channel */
  unsigned      inByteCount ;           /* Total bytes written to channel */
  unsigned      outByteCount ;          /* Total bytes read from channel */
  /*
  char          sender[SENDER_SIZE+1] ;  * Current sender id *
  char          tokensValues[MAX_MESSAGE_SIZE+1] ; * CONNECT tokens/values 
  */
} ;

sChannel *gHyp_channel_new ( char *name,
			     char *path,
			     short flags,
			     SOCKET fd )
{
  /* Create and initialize a new channel entry */
  sChannel
    *pChannel ;

  pChannel= (sChannel*) AllocMemory ( sizeof ( sChannel ) ) ;
  strcpy ( pChannel->target, name ) ;
  strcpy ( pChannel->path, path ) ;
  pChannel->flags = flags ;
  pChannel->buf[0] = '\0' ;
  pChannel->offset = 0 ;
  pChannel->maxMsgSize = MIN_MESSAGE_SIZE ;
  pChannel->fd = fd ;
  memset ( &pChannel->overlapped, 0, sizeof (OVERLAPPED) ) ;
#ifdef AS_WINDOWS
  pChannel->overlapped.hEvent = (HANDLE) gHyp_sock_createEvent () ;
  pChannel->overlapped.Internal = STATUS_READY ;
#endif
  pChannel->pSSL = NULL ;
  return pChannel ;
}

void gHyp_channel_close ( sChannel *pChannel ) 
{
  gHyp_util_logInfo ( "Closing connection (%d), flags=%x, target='%s', path=%s", 
		      pChannel->fd, 
		      pChannel->flags, 
		      pChannel->target, 
		      pChannel->path ) ;
  
  gHyp_sock_close ( (SOCKET) pChannel->fd, 
		    (short) (pChannel->flags&MASK_SOCKET), 
		    pChannel->target, 
		    pChannel->path ) ;
  
#ifdef AS_WINDOWS
  CloseHandle ( pChannel->overlapped.hEvent ) ;
#endif
}


void gHyp_channel_delete ( sChannel * pChannel )
{
  gHyp_channel_close ( pChannel ) ;
#ifdef AS_SSL
  if ( pChannel->pSSL ) gHyp_sock_deleteSSL ( pChannel->pSSL ) ;
#endif
  ReleaseMemory ( pChannel ) ;
  return ;
}

char* gHyp_channel_target ( sChannel *pChannel )
{
  return pChannel->target ;
}

void gHyp_channel_setTarget ( sChannel *pChannel, char *target )
{
  strcpy ( pChannel->target, target ) ;
}

char* gHyp_channel_path ( sChannel *pChannel )
{
  return pChannel->path ;
}

void gHyp_channel_setPath ( sChannel *pChannel, char *path )
{
  strcpy ( pChannel->path, path ) ;
}

short gHyp_channel_flags ( sChannel *pChannel )
{
  return pChannel->flags ;
}

SOCKET gHyp_channel_socket ( sChannel *pChannel )
{
  return pChannel->fd ;
}

void gHyp_channel_setSocket ( sChannel *pChannel, SOCKET s )
{
  pChannel->fd = s ;
}

OVERLAPPED* gHyp_channel_overlapped ( sChannel *pChannel )
{
  return &(pChannel->overlapped) ;
}

void gHyp_channel_setSSL( sChannel *pChannel, sSSL *pSSL )
{
  pChannel->pSSL = pSSL ;
}

sSSL *gHyp_channel_getSSL ( sChannel *pChannel )
{
  return pChannel->pSSL ;
}

char *gHyp_channel_buffer ( sChannel *pChannel )
{
  return pChannel->buf ;
}

extern int *gHyp_channel_pNbytes ( sChannel *pChannel ) 
{
  return &pChannel->nBytes ;
}

void gHyp_channel_resetBuffer ( sChannel *pChannel )
{
  pChannel->buf[0] = '\0' ;
}

int gHyp_channel_offset ( sChannel *pChannel )
{
  return pChannel->offset ;
}

void gHyp_channel_setOffset ( sChannel *pChannel, int offset )
{
  pChannel->offset = offset ;
}

int gHyp_channel_maxMsgSize ( sChannel *pChannel )
{
  return pChannel->maxMsgSize ;
}

void gHyp_channel_setMaxMsgSize ( sChannel *pChannel, int maxMsgSize )
{
  pChannel->maxMsgSize = maxMsgSize ;
}

sLOGICAL gHyp_channel_updateStats ( sChannel *pChannel, int msgLen, sLOGICAL sw )
{
  /* Description:
   *
   *    Keep track of the number of bytes written and read from
   *    the channel plus number of message written and read.
   *
   * Arguments:
   *
   *    pChannel                   [R]
   *    - channel object
   *
   *    msgLen                  [R]
   *    - length of message that was sent
   *
   *    sw                      [R]
   *    - if TRUE, then direction is 'in', otherwise 'out'
   *
   * Return value:
   *
   *    Returns TRUE if msgLen > 0, FALSE otherwise
   *
   */
   
  /* Update channel statistics. */
  if ( msgLen > 0 ) { 
    if ( sw ) {
      pChannel->inMessageCount++ ;
      pChannel->inByteCount += msgLen ;    /* If we exceed 4GB, it overflows. */
    }
    else {
      pChannel->outMessageCount++ ;
      pChannel->outByteCount += msgLen ;   /* If we exceed 4GB, it overflows. */
    }
    gHyp_channel_setUpdateTime ( pChannel, gsCurTime ) ;
    return TRUE ;
  }
  else
    return FALSE ;
}

time_t gHyp_channel_updateTime ( sChannel *pChannel )
{
  /* Description:
   *
   *    Returns the update time for the channel
   *
   * Arguments:
   *
   *    pChannel                           [R]
   *    - pointer to sChannel object
   *
   * Return value:
   *
   *    update time
   *
   *
   */
  return pChannel->updateTime ;
}

void gHyp_channel_setUpdateTime ( sChannel *pChannel, time_t updateTime )
{
  /* Description:
   *
   *    Sets the update time
   *
   * Arguments:
   *
   *    pChannel                           [R]
   *    - pointer to sChannel object
   *
   *    updateTime                          [R]
   *    - new time
   *
   *
   * Return value:
   *
   *    update time
   *
   *
   */

  pChannel->updateTime = updateTime ;
}

void gHyp_channel_clearStats( sChannel *pChannel ) 
{
  pChannel->inMessageCount = 0 ;
  pChannel->outMessageCount = 0 ;
  pChannel->inByteCount = 0 ;
  pChannel->outByteCount = 0 ;
  /*
  strcpy ( pChannel->sender, "NONE" ) ; 
  strcpy ( pChannel->tokensValues, "NONE" ) ;
  */
  return ;
}

/*
void gHyp_channel_initStats( sChannel *pChannel, char *sender, char *tokensValues ) 
{
  if ( strcmp ( pChannel->sender, "NONE" ) == 0 ) 
    strcpy ( pChannel->sender, sender ) ;
  if ( strcmp ( pChannel->tokensValues, "NONE" ) == 0 )
    strcpy ( pChannel->tokensValues, tokensValues ) ;
  return ;
}
*/

void gHyp_channel_copyStats( sChannel *pDstChannel, sChannel *pSrcChannel ) 
{
  pDstChannel->inMessageCount = pSrcChannel->inMessageCount ;
  pDstChannel->inMessageCount = pSrcChannel->inMessageCount ;
  pDstChannel->inByteCount = pSrcChannel->inByteCount ;
  pDstChannel->inByteCount = pSrcChannel->inByteCount ;
  /*
  strcpy ( pDstChannel->sender, pSrcChannel->sender ) ;
  strcpy ( pDstChannel->tokensValues, pSrcChannel->tokensValues ) ;
  */
  return ;
}

void gHyp_channel_displayStats ( sChannel *pChannel, sLOGICAL log )
{
  /* Description:
   *
   *    Log some statistics about a channel.
   *
   * Arguments:
   *
   *    pChannel                                           [R] 
   *    - pointer to client or server sChannel object
   *
   *    log
   *    - if TRUE, also log to transaction log
   *
   * Return value:
   *
   *    none
   *
   *
   */

  static char
    *gzaTime[3] = { "seconds", "minutes", "hours" } ;
  
  static char
    *gzaQuantity[3] = { "bytes", "Kbytes", "MBytes" } ;
  
  int   
    t,i,o,
    elapsedSeconds = (int) (pChannel->updateTime - pChannel->createTime),
    elapsedTime = elapsedSeconds,
    totalOut = pChannel->outByteCount,
    totalIn = pChannel->inByteCount ;

  char
    startTime[DATETIME_SIZE+1],
    endTime[DATETIME_SIZE+1]; 

  strcpy ( startTime, gHyp_util_timeStamp ( pChannel->createTime ) ) ;
  strcpy ( endTime,   gHyp_util_timeStamp ( pChannel->updateTime ) ) ;

  if ( elapsedTime < 600 ) {

    /* Less than 10 minutes: Display in seconds */
    t = 0 ;
  }
  else if ( elapsedTime < 36000 ) {

    /* Less than 10 hours: Display in minutes */
    t = 1 ;
    elapsedTime /= 60 ;
  }
  else {

    /* Greater than 10 hours: Display in hours */
    t = 2 ;
    elapsedTime /= 3600 ;
  }

  if ( totalIn < 10240 ) {

    /* Less than 10KB: Display in bytes */  
    i = 0 ;
  }
  else if ( totalIn < 10485760 ) {

    /* Less than 10MB, Display in Kbytes */  
    i = 1 ;
    totalIn /= 1024 ;
  }
  else {
    
    /* Greater than 10MB, Display in Mbytes */  
    i = 2 ;
    totalIn /= 1048576 ;
  }
  
  if ( totalOut < 10240 ) {
    
    /* Less than 10KB: Display in bytes */  
    o = 0 ;
  }
  else if ( totalOut < 10485760 ) {
    
    /* Less than 10MB, Display in Kbytes */  
    o = 1 ;
    totalOut /= 1024 ;
  }
  else {
    
    /* Greater than 10MB, Display in Mbytes */  
    o = 2 ;
    totalOut /= 1048576 ;
  }
  
  gHyp_util_logInfo ( "   %s (%d)",
                      pChannel->target,
                      pChannel->fd ) ;

  gHyp_util_logInfo ( "      Start Time: %s, End Time: %s",
                      startTime,
                      endTime ) ;
  
  gHyp_util_logInfo ( "      Elapsed Time: %d %s",
                      elapsedTime,
                      gzaTime[t] ) ;
  
  gHyp_util_logInfo ( "      Write %d msgs (%d %s)",
                      pChannel->inMessageCount,
                      totalIn,
                      gzaQuantity[i] ) ;
  
  gHyp_util_logInfo ( "      Read %d msgs (%d %s)",
                      pChannel->outMessageCount,
                      totalOut,    
		      gzaQuantity[o] ) ;
  
  if ( !log ) return ;

  return ;
}

