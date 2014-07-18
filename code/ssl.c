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
 * $Log: ssl.c,v $
 * Revision 1.2  2004/07/28 00:49:18  bergsma
 * Version 3.3.0
 *
 * Revision 1.2  2004/07/27 15:44:17  bergsma
 * Remove ^M
 *
 * Revision 1.1  2004/04/29 02:15:02  bergsma
 * Added ssl functions.
 *
 *
 */

/********************** HYPERSCRIPT INTERFACE ********************************/

#include "auto.h"       /* System Interface and Function Prototypes */

/********************** EXTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/

/********************** FUNCTION DEFINITIONS ********************************/

#ifdef AS_SSL
void gHyp_ssl_new ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_new ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData,
      *pResult ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *ctx ;

    char
      *pPassword,
      password[VALUE_SIZE+1],
      *pCertFile,
      certFile[VALUE_SIZE+1],
      *pKeyFile,
      keyFile[VALUE_SIZE+1] ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;


    if ( argCount > 3 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_new ( [certfile [, keyFile [,password]]] )");

    if ( argCount > 2 ) {

      /* Get the password */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData,
                           password,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;
      pPassword = password ;
    }
    else
      pPassword = NULL ;

    if ( argCount > 1 ) {

      /* Get the keyFile */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData,
                           keyFile,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;
      pKeyFile = keyFile ;
    }
    else
      pKeyFile = NULL ;

    if ( argCount > 0 ) {

      /* Get the certfile */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData,
                           certFile,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;

      pCertFile = certFile ;
    }
    else
      pCertFile = NULL ;

    /* Build our SSL context*/
    ctx = gHyp_sock_ctxInit ( pCertFile, pKeyFile, pPassword ) ;

    if ( !ctx )
      gHyp_instance_warning ( pAI, STATUS_SSL, "Failed to create SSL context");

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, ctx ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_ssl_delete ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_delete ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *ctx;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: ssl_delete ( ssl_context_handle )");

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Shutdown the socket */
    gHyp_sock_destroyCTX( ctx );

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_ssl_ciphers ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_ciphers ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *ctx ;

    char
      ciphers[VALUE_SIZE+1] ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_ciphers ( ssl_context_handle, ciphers )");

    /* Get the ciphers */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
                           ciphers,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    if ( ctx == NULL )
      gHyp_instance_error ( pAI, STATUS_SSL, "No SSL context");

    if ( !gHyp_sock_ctxCiphers ( ctx, ciphers ) )
      gHyp_instance_error ( pAI, STATUS_SSL, "Failed to set ciphers");

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_ssl_keyFile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_keyFile ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *ctx ;

    char
      password[VALUE_SIZE+1],
      keyFile[VALUE_SIZE+1] ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 && argCount !=3 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_keyFile ( ssl_context_handle, keyFile [,password] )");

    if ( argCount == 3 ) {

      /* Get the password */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData,
                           password,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;


    }
    else
      password[0] = '\0' ;

    /* Get the keyFile */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
                           keyFile,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    if ( ctx == NULL )
      gHyp_instance_error ( pAI, STATUS_SSL, "No SSL context");

    if ( !gHyp_sock_ctxKey ( ctx, keyFile, password ) )
      gHyp_instance_error ( pAI, STATUS_SSL, "Failed to set keyFile");

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_ssl_certFile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_certFile ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *ctx ;

    char
      certFile[VALUE_SIZE+1] ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_certFile ( ssl_context_handle, certFile )");

    /* Get the certFile */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
                           certFile,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    if ( ctx == NULL )
      gHyp_instance_error ( pAI, STATUS_SSL, "No SSL context");

    if ( !gHyp_sock_ctxCert ( ctx, certFile ) )
      gHyp_instance_error ( pAI, STATUS_SSL, "Failed to set certFile");

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_ssl_CAfile ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_CAfile ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *ctx ;

    char
      CAfile[VALUE_SIZE+1] ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_CAfile ( ssl_context_handle, CAfile )");

    /* Get the CAfile */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
                           CAfile,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    if ( ctx == NULL )
      gHyp_instance_error ( pAI, STATUS_SSL, "No SSL context");

    if ( !gHyp_sock_ctxCAfile ( ctx, CAfile ) )
      gHyp_instance_error ( pAI, STATUS_SSL, "Failed to set CAfile");

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_ssl_CApath ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_CApath ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *ctx ;

    char
      CApath[VALUE_SIZE+1] ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_CApath ( ssl_context_handle, CApath )");

    /* Get the CApath */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
                           CApath,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    if ( ctx == NULL )
      gHyp_instance_error ( pAI, STATUS_SSL, "No SSL context");

    if ( !gHyp_sock_ctxCApath ( ctx, CApath ) )
      gHyp_instance_error ( pAI, STATUS_SSL, "Failed to set CApath");

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


void gHyp_ssl_auth ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_auth ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    void
      *ctx ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_auth ( ssl_context_handle )");

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    if ( ctx == NULL )
      gHyp_instance_error ( pAI, STATUS_SSL, "No SSL context");

    if ( !gHyp_sock_ctxAuth ( ctx ) )
      gHyp_instance_error ( pAI, STATUS_SSL, "Failed to set authentification");

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_ssl_authClient ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_authClient ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *ctx ;


    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_authClient ( ssl_context_handle )");

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    if ( ctx == NULL )
      gHyp_instance_error ( pAI, STATUS_SSL, "No SSL context");

    if ( !gHyp_sock_ctxAuthClient ( ctx ) )
      gHyp_instance_error ( pAI, STATUS_SSL, "Failed to set client authentification");

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}



void gHyp_ssl_assign ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_assign ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult,
      *pData ;

    SOCKET
      fd = INVALID_SOCKET;

    sConcept
      *pConcept = gHyp_instance_getConcept ( pAI ) ;

    sSecs1
      *pPort,
      *pSecs1;

    sHsms
      *pHsms ;

    sChannel
      *pChannel ;

    sBYTE
      objectType ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    sSSL
      *pSSL=NULL;

    void
      *ctx;

    sLOGICAL
      isClientSpecified,
      isClient=FALSE ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 && argCount != 3 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: hSSL = ssl_assign ( ssl_context_handle, handle [, isClient] )");

    if ( argCount == 3 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      isClient = gHyp_data_getBool ( pData, gHyp_data_getSubScript ( pData ), TRUE );
      isClientSpecified = TRUE ;
    }
    else
      isClientSpecified = FALSE ;

    /* Get the host name or port fd */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

    /* Get the socket handle */
    fd = (SOCKET) gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );
    objectType = gHyp_concept_getSocketObjectType ( pConcept, (SOCKET) fd ) ;

    if ( objectType == DATA_OBJECT_NULL ) objectType = DATA_OBJECT_CHANNEL ;

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    if ( ctx == NULL )
      gHyp_instance_error ( pAI, STATUS_SSL, "No SSL context");

    pConcept =  gHyp_instance_getConcept(pAI) ;

    switch ( objectType ) {

    case DATA_OBJECT_CHANNEL :

      pChannel = gHyp_concept_findNetworkByFd ( pConcept, (SOCKET) fd ) ;
      if ( pChannel ) {

          if ( !isClientSpecified ) isClient = TRUE ;
          pSSL = gHyp_sock_createSSL ( ctx, isClient ) ;
          if ( pSSL == NULL )
            gHyp_instance_error ( pAI, STATUS_SSL, "No SSL bio created");
          gHyp_channel_setSSL ( pChannel, pSSL ) ;
      }
      else if ( gHyp_concept_serviceHandle ( pConcept ) == fd ) {

          if ( !isClientSpecified ) isClient = FALSE ;
          pSSL = gHyp_sock_createSSL ( ctx, isClient ) ;
          if ( pSSL == NULL )
            gHyp_instance_error ( pAI, STATUS_SSL, "No SSL bio created");
          gHyp_concept_setSSL ( pConcept, pSSL ) ;
      }
      else
        gHyp_instance_warning ( pAI,
                                  STATUS_CHANNEL,
                                  "No channel with socket '%d'.",
                                  fd ) ;

      break ;

    case DATA_OBJECT_PORT:
    case DATA_OBJECT_HTTP:

        pPort = (sSecs1*) gHyp_concept_getSocketObject ( pConcept,
                                                     (SOCKET) fd,
                                                     objectType ) ;

        if ( !pPort ) {
          gHyp_instance_warning ( pAI,
                                  STATUS_PORT,
                                  "No port '%d'.",
                                  fd ) ;
        }
        else {

          if ( !isClientSpecified )
            isClient = !(gHyp_secs1_flags(pPort) & SOCKET_LISTEN) ;
          pSSL = gHyp_sock_createSSL ( ctx, isClient ) ;
          if ( pSSL == NULL )
            gHyp_instance_error ( pAI, STATUS_SSL, "No SSL bio created");
          gHyp_secs1_setSSL ( pPort, pSSL ) ;

        }
        break ;

      case DATA_OBJECT_HSMS:

        pHsms = (sHsms*) gHyp_concept_getSocketObject ( pConcept,
                                                      (SOCKET) fd,
                                                      DATA_OBJECT_HSMS ) ;

        if ( !pHsms ) {
          gHyp_instance_warning ( pAI,
                                  STATUS_HSMS,
                                  "No hsms port '%d'.",
                                  fd ) ;
        }
        else {

          if ( !isClientSpecified )
            isClient = !(gHyp_hsms_flags(pHsms) & SOCKET_LISTEN) ;
          pSSL = gHyp_sock_createSSL ( ctx, isClient ) ;
          if ( pSSL == NULL )
            gHyp_instance_error ( pAI, STATUS_SSL, "No SSL bio created");
          gHyp_hsms_setSSL ( pHsms, pSSL ) ;

        }
        break ;

      case DATA_OBJECT_SECS1:

        pSecs1 = (sSecs1*) gHyp_concept_getSocketObject ( pConcept,
                                                        (SOCKET) fd,
                                                        DATA_OBJECT_SECS1 ) ;

        if ( !pSecs1 ) {
          gHyp_instance_warning ( pAI,
                                  STATUS_SECS1,
                                  "No secs1 port '%d'.",
                                  fd ) ;
        }
        else {

          if ( !isClientSpecified )
            isClient = !(gHyp_secs1_flags(pSecs1) & SOCKET_LISTEN) ;

          pSSL = gHyp_sock_createSSL ( ctx, isClient ) ;
          if ( pSSL == NULL )
            gHyp_instance_error ( pAI, STATUS_SSL, "No SSL bio created");
          gHyp_secs1_setSSL ( pSecs1, pSSL ) ;

        }
        break ;

      case DATA_OBJECT_NULL:
      default:
        gHyp_instance_warning( pAI, STATUS_SSL, "Cannot find Port, Hsms, or Secs1 object" ) ;

    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, pSSL ) ;
    gHyp_stack_push ( pStack, pResult ) ;


  }
}

void gHyp_ssl_getSession ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_getSession ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult,
      *pData ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *pSSL,
      *session ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: ssl_getSession ( hSSL )");

    /* Get the socket handle */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pSSL = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the SSL session id */
    session = gHyp_sock_getSession( pSSL );

    if ( !session )
      gHyp_instance_warning ( pAI, STATUS_SSL, "Failed to get SSL session object");

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, session ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


void gHyp_ssl_setSession ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_setSession ( )
   *    Returns 1
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame        *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse        *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    void
      *pSSL,
      *session ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: ssl_setSession ( hSSL, session )");

    /* Get the SESSION object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    session = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    pSSL =  gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );
    
    /* Set the session id */
    gHyp_sock_setSession( pSSL, session );

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

#endif
