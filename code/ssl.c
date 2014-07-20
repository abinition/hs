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
 * Revision 1.30  2013-01-02 19:10:27  bergsma
 * CVS Issues
 *
 * Revision 1.28  2012-08-30 18:04:54  bergsma
 * Changes to ssl setState and getState for V 1.0 of openssl
 *
 * Revision 1.27  2011-01-08 21:35:11  bergsma
 * Add HMAC_SHA256
 *
 * Revision 1.26  2010-06-26 06:34:28  bergsma
 * Typo
 *
 * Revision 1.25  2010-01-08 04:38:29  bergsma
 * Increased ssl_md5 input length
 *
 * Revision 1.24  2010-01-08 02:44:57  bergsma
 * Added ssl_md5(), enhanced ssl_digest.
 * Fixed urldecode, missing ":"
 * Enabled object calls, ie:  text.strtok( ) and the like...
 *
 * Revision 1.23  2009-03-07 21:27:32  bergsma
 * gHyp_data_getAll needs additional handle argument
 *
 * Revision 1.22  2008-12-30 18:36:27  bergsma
 * Add support for conditional SSL
 *
 * Revision 1.21  2008-11-30 22:35:57  bergsma
 * Comments
 *
 * Revision 1.20  2008-05-03 21:44:17  bergsma
 * Add placeholder extra arg (isCond) to ssl_assign
 *
 * Revision 1.19  2008-02-12 23:12:31  bergsma
 * Placeholder for conditional SSL
 *
 * Revision 1.18  2007-09-25 17:50:19  bergsma
 * Integrate SOCK_UNIX_LISTEN with SOCK_LISTEN
 *
 * Revision 1.17  2007-07-11 14:44:59  bergsma
 * Add second argyment to ssl_digest
 *
 * Revision 1.16  2007-07-09 05:36:56  bergsma
 * Add ssl_digest
 *
 * Revision 1.15  2007-05-09 00:43:19  bergsma
 * Problem with secs1_rawoutgoing - segmentation fault when pData str
 * is longer than PORT_WRITE_SIZE
 *
 * Revision 1.14  2007-05-02 20:34:01  bergsma
 * Fix parseurl function.  Improve various print/debug/log statements.
 * Fix problem with chunked data transfers.
 *
 * Revision 1.13  2007-02-13 22:34:37  bergsma
 * SSL Handoff working
 *
 * Revision 1.12  2006/11/25 03:37:39  bergsma
 * SSL set and get state development.
 *
 * Revision 1.11  2006/11/13 02:11:34  bergsma
 * Added functions ssl_setState and ssl_getState
 *
 * Revision 1.10  2005/12/27 02:53:25  bergsma
 * Removed functions ssl_certfile and ssl_keyfile
 *
 * Revision 1.9  2005/12/17 20:38:33  bergsma
 * Added ssl_enableSessions
 *
 * Revision 1.8  2005/12/10 00:30:30  bergsmas
 * HS 3.6.5
 *
 * Revision 1.7  2005/07/23 22:33:06  bergsma
 * ssl fixes
 *
 * Revision 1.6  2005/05/10 17:40:25  bergsma
 * Fix getSession and setSession argument casting
 *
 * Revision 1.5  2005/03/21 18:07:32  bergsma
 * misspelled gHyp_concept_getSockObjType
 *
 * Revision 1.4  2005/03/16 23:53:22  bergsma
 * V 3.5.1 - fixes for use with DECC compiler.
 *
 * Revision 1.3  2004/09/18 21:51:49  bergsma
 * SSL buffer size can be 16K
 * SSL timeout increased from 1 to 1.5 seconds
 * New functions ssl_setSession and ssl_getSession, for Session Id's
 *
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

void gHyp_ssl_digest ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_digest ( )
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
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char
      digest[VALUE_SIZE+1],
      digestType[VALUE_SIZE+1],
      text1[VALUE_SIZE+1],
      text2[VALUE_SIZE+1] ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 && argCount!= 2 && argCount != 3 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_digest ( text1 [,text2 [,type] ] )");

    if ( argCount == 3 ) {
      /* Get the digest type */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData,
                             digestType,
                             VALUE_SIZE,
                             gHyp_data_getSubScript ( pData ),
                             TRUE ) ;
    }
    else
      strcpy ( digestType, "SHA1" ) ;

    if ( argCount >= 2 ) {
      /* Get the secondary text */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData,
                             text2,
                             VALUE_SIZE,
                             gHyp_data_getSubScript ( pData ),
                             TRUE ) ;
    }
    else
      strcpy ( text2, "" ) ;

    /* Get the primary text */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData,
                           text1,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;

    if ( strcmp ( digestType, "SHA256" ) == 0 )
      gHyp_sock_hmac256 ( text1, text2, digest ) ;
    else
      /* Get the SHA1 (or other) digest */
      gHyp_sock_digest ( text1, text2, digest, digestType ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, digest ) ;
    gHyp_stack_push ( pStack, pResult ) ;

  }
}

void gHyp_ssl_md5 ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_md5 ( )
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
      *pData,
      *pValue;

    int
      ss,
      context,
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sLOGICAL
      isVector=FALSE ;

    char
      digest[VALUE_SIZE+1],
      text[MAX_INPUT_LENGTH+1],
      *pStream = text,
      *pAnchor = pStream,
      *pEndOfStream = pStream ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: ssl_md5 ( text )");

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    ss = gHyp_data_getSubScript ( pData ) ; 
    context=-1;
    isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;
    pValue = NULL ;
    pStream = gHyp_util_readStream (	pStream, pAnchor, &pEndOfStream,
					&n, pData, 
					&pValue, &context, ss, isVector, 
					NULL ) ;

    /* Get the SHA1 (or other) digest */
    gHyp_sock_md5 ( text, n, digest ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setStr ( pResult, digest ) ;
    gHyp_stack_push ( pStack, pResult ) ;

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
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sSSL
      *pSSL=NULL;

    void
      *ctx;

    sLOGICAL
      isClientSpecified,
      isClient=FALSE ;

    char
      condition[VALUE_SIZE+1] ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 && argCount != 3 && argCount != 4 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
      "Invalid args. Usage: hSSL = ssl_assign ( ssl_context_handle, handle [,isClient[,condition]] )");

    if ( argCount == 4 ) {
      /* Get the condition */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData,
                           condition,
                           VALUE_SIZE,
                           gHyp_data_getSubScript ( pData ),
                           TRUE ) ;    
    }
    else
      n = 0 ;

    condition[n] = '\0' ;

    if ( argCount >= 3 ) {
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
    objectType = gHyp_concept_getSockObjType ( pConcept, (SOCKET) fd ) ;

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
          pSSL = gHyp_sock_createSSL ( ctx, isClient, condition ) ;
          if ( pSSL == NULL )
            gHyp_instance_error ( pAI, STATUS_SSL, "No SSL bio created");
          gHyp_channel_setSSL ( pChannel, pSSL ) ;
      }
      else if ( gHyp_concept_serviceHandle ( pConcept ) == fd ) {

          if ( !isClientSpecified ) isClient = FALSE ;
          pSSL = gHyp_sock_createSSL ( ctx, isClient, condition ) ;
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
                                  "No socket '%d'.",
                                  fd ) ;
        }
        else {

          if ( !isClientSpecified )
            isClient = !(gHyp_secs1_flags(pPort) & (SOCKET_LISTEN | SOCKET_UNIX_LISTEN)) ;
          pSSL = gHyp_sock_createSSL ( ctx, isClient, condition ) ;
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
                                  "No hsms socket '%d'.",
                                  fd ) ;
        }
        else {

          if ( !isClientSpecified )
            isClient = !(gHyp_hsms_flags(pHsms) & (SOCKET_LISTEN | SOCKET_UNIX_LISTEN)) ;
          pSSL = gHyp_sock_createSSL ( ctx, isClient, condition ) ;
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
                                  "No secs1 socket '%d'.",
                                  fd ) ;
        }
        else {

          if ( !isClientSpecified )
            isClient = !(gHyp_secs1_flags(pSecs1) & (SOCKET_LISTEN | SOCKET_UNIX_LISTEN)) ;

          pSSL = gHyp_sock_createSSL ( ctx, isClient, condition ) ;
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
      *ctx,
      *session ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: ssl_getSession ( ssl_context_handle )");

    /* Get the SSL context object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the SSL session id */
    session = gHyp_sock_getSession( ctx );

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
      *ctx,
      *session ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: ssl_setSession ( ssl_context_handle, session )");

    /* Get the SESSION object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    session = gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx =  gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );
    
    /* Set the session id */
    gHyp_sock_setSession( ctx, session );

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_ssl_enableSessions ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_enableSessions ( )
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
        "Invalid args. Usage: ssl_enableSessions ( ssl_context_handle )");

    /* Get the SSL object */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    ctx =  gHyp_data_getHandle ( pData, gHyp_data_getSubScript ( pData ), TRUE );
    
    /* Set the session id */
    gHyp_sock_enableSessions( ctx ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_ssl_getState ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_getState ( )
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
      *pResult=NULL,
      *pData ;

    sInstance
      *pAIassigned;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      id ;

    sSecs1
      *pPort ;

    SOCKET
      portFd ;

    sLOGICAL
      status = TRUE ;

    sSSL
      *pSSL ;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: ssl_getState ( deviceId )");


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

      pSSL = gHyp_secs1_getSSL ( pPort ) ;
      if ( pSSL ) {
        pResult = gHyp_sock_getSSLstate ( pSSL ) ;
      }
      else {
	status = FALSE ;
	gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "Socket %d does not have an SSL structure.",
			    portFd ) ;
      }
    }

    if ( status && pResult ) 
	gHyp_stack_push ( pStack, pResult ) ;
    else
      	gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }
}

void gHyp_ssl_setState ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: ssl_setState ( )
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
      *pSSLdata,
      *pData ;

    sInstance
      *pAIassigned;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      id,
      id2;

    sSecs1
      *pPort,
      *pPort2=NULL;

    SOCKET
      portFd,
      portFd2=INVALID_SOCKET;

    sLOGICAL
      status = TRUE ;

    sSSL
      *pSSL,
      *pSSL2 =NULL,
      *pSSL3 =NULL;

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 3 && argCount != 2 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid args. Usage: ssl_setState ( deviceId, sslState[, sourceDeviceId] )");

    if ( argCount == 3 ) {


      /* Get the source id SSL object */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      id2 = gHyp_data_getInt ( pData, gHyp_data_getSubScript ( pData ), TRUE ) ;

      /* Check id */
      if ( id2 < 0 || id2 > 65535 )
	gHyp_instance_error ( pAI,STATUS_BOUNDS, "Source device ID out of range" ) ;

      /* Get the port structure */
      pAIassigned = gHyp_concept_getInstForDeviceId ( gHyp_instance_getConcept ( pAI ), (sWORD) id2 ) ;

      if ( !pAIassigned ) {
        gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		      "Source device id %d is not assigned",
			      id2 ) ;
        status = FALSE ;
      }

      if ( status ) {

        portFd2 = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id2 ) ;
        if ( portFd2 == INVALID_SOCKET ) {
  	  gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Socket %d does not exist for source ID %d",
			    portFd2, id2 ) ;
	  status = FALSE ;
	}
      }

      if ( status ) {
        pPort2 = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							  (SOCKET) portFd2, 
							  DATA_OBJECT_NULL ) ;
        if ( !pPort2 ) {
	  gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "Socket %d does not exist for source ID %d",
			    portFd2, id2 ) ;
	  status = FALSE ;
	}
      }

      if ( status ) {

        pSSL2 = gHyp_secs1_getSSL ( pPort2 ) ;
    
      }
      if ( !pSSL2 ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "SSL structure does not exist for source ID %d",
			    id2 ) ;
	status = FALSE ;
      }
    
    }

    if ( status ) {

      /* Get the SSL object */
      pSSLdata = gHyp_stack_popRdata ( pStack, pAI ) ;

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
			    "Destination device id %d is not assigned",
			    id ) ;
        status = FALSE ;
      }
    }

    if ( status ) {

      portFd = gHyp_instance_getDeviceFd ( pAIassigned, (sWORD) id ) ;
      if ( portFd == INVALID_SOCKET ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
			    "Socket %d does not exist for destination ID %d.",
			    portFd, id ) ;
	status = FALSE ;
      }
    }

    if ( status ) {
      pPort = (sSecs1*) gHyp_concept_getSocketObject ( gHyp_instance_getConcept(pAI), 
							(SOCKET) portFd, 
							DATA_OBJECT_NULL ) ;
      if ( !pPort ) {
	gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "Socket %d does not exist for destination ID %d.",
			    portFd, id ) ;
	status = FALSE ;
      }
    }

    if ( status ) {

      pSSL = gHyp_secs1_getSSL ( pPort ) ;

      if ( !pSSL && !pSSL2 ) {
	/* There must be a least one SSL structure */
	gHyp_instance_warning ( pAI,STATUS_PORT, 
	  		    "SSL structure does not exist for destination ID %d",
			    id ) ;
	status = FALSE ;
      }

    }
    if ( status ) {
      
      if ( pSSL2 ) {

	/* Take from existing SSL connection */

	if ( pSSL2 == pSSL ) {
	  /* Arg1 ID is the same as Arg3 ID.
	   * We need to create a new SSL structure
	   */
	  gHyp_util_debug("Creating new SSL instance from CTX" ) ;
	  pSSL2 = gHyp_sock_copySSL ( pSSL ) ;
	}

	/* We want to use this new SSL structure */

	/* Assign pSSL2 to pSSL */
	gHyp_secs1_setSSL ( pPort, pSSL2 ) ;

	/* Switch entry with other port */
	if ( pPort2 && pPort2 != pPort ) {
	  gHyp_secs1_setSSL ( pPort2, pSSL ) ;
	}
        else {
	  /* Get rid of old one */
	  gHyp_sock_deleteSSL ( pSSL ) ;
	}
      
        /* Hack the SSL state. GRIM IT IS, BUT WHAT ELSE? . */
        status = gHyp_sock_setSSLstate ( pSSL2, pSSLdata, pSSL ) ;
      }
      else {

        /* Hack the SSL state. GRIM IT IS, BUT WHAT ELSE? . */
        status = gHyp_sock_setSSLstate ( pSSL, pSSLdata, pSSL2 ) ;
      }
    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }
}

sData *gHyp_ssl_getData ( sData *pData, char *a, char *b, char *c )
{

  sData 
     *pChild ;

  pChild = gHyp_data_getChildByName ( pData, a ) ;

  if ( pChild ) {

    if ( strlen ( b ) > 0 ) {

      pChild = gHyp_data_getChildByName ( pChild, b ) ;

      if ( pChild ) {

	if ( strlen ( c ) > 0 ) {

          pChild = gHyp_data_getChildByName ( pChild, c ) ;

	}
      }
    }
  }

  /*gHyp_util_debug("SSL structure %s %s %s",a,b,c);*/

  return pChild ;

}




/**********************

  S A V E  T H I S  F O R  L A T E R  U S E  I N  E C R Y P T I N G / D E C R Y P T I N G  A  F I L E 

#include <openssl/blowfish.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define IP_SIZE 1024
#define OP_SIZE 1032

unsigned char key[16];
unsigned char iv[8];

int
generate_key ()
{
	int i, j, fd;
	if ((fd = open ("/dev/random", O_RDONLY)) == -1)
		perror ("open error");

	if ((read (fd, key, 16)) == -1)
		perror ("read key error");

	if ((read (fd, iv, 8)) == -1)
		perror ("read iv error");
	
	printf("128 bit key:\n");
	for (i = 0; i < 16; i++)
		printf ("%d \t", key[i]);
	printf ("\n ------ \n");

	printf("Initialization vector\n");
	for (i = 0; i < 8; i++)
		printf ("%d \t", iv[i]);

	printf ("\n ------ \n");
	close (fd);
	return 0;
}

int
decrypt (int infd, int outfd)
{
	unsigned char outbuf[IP_SIZE];
	int olen, tlen, n;
	char inbuff[OP_SIZE];
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init (&ctx);
	EVP_DecryptInit (&ctx, EVP_bf_cbc (), key, iv);

	for (;;)
	  {
		  bzero (&inbuff, OP_SIZE);
		  if ((n = read (infd, inbuff, OP_SIZE)) == -1)
		    {
			    perror ("read error");
			    break;
		    }
		  else if (n == 0)
			  break;

		  bzero (&outbuf, IP_SIZE);

		  if (EVP_DecryptUpdate (&ctx, outbuf, &olen, inbuff, n) != 1)
		    {
			    printf ("error in decrypt update\n");
			    return 0;
		    }

		  if (EVP_DecryptFinal (&ctx, outbuf + olen, &tlen) != 1)
		    {
			    printf ("error in decrypt final\n");
			    return 0;
		    }
		  olen += tlen;
		  if ((n = write (outfd, outbuf, olen)) == -1)
			  perror ("write error");
	  }

	EVP_CIPHER_CTX_cleanup (&ctx);
	return 1;
}

int
encrypt (int infd, int outfd)
{
	unsigned char outbuf[OP_SIZE];
	int olen, tlen, n;
	char inbuff[IP_SIZE];
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init (&ctx);
	EVP_EncryptInit (&ctx, EVP_bf_cbc (), key, iv);

	for (;;)
	  {
		  bzero (&inbuff, IP_SIZE);

		  if ((n = read (infd, inbuff, IP_SIZE)) == -1)
		    {
			    perror ("read error");
			    break;
		    }
		  else if (n == 0)
			  break;

		  if (EVP_EncryptUpdate (&ctx, outbuf, &olen, inbuff, n) != 1)
		    {
			    printf ("error in encrypt update\n");
			    return 0;
		    }

		  if (EVP_EncryptFinal (&ctx, outbuf + olen, &tlen) != 1)
		    {
			    printf ("error in encrypt final\n");
			    return 0;
		    }
		  olen += tlen;
		  if ((n = write (outfd, outbuf, olen)) == -1)
			  perror ("write error");
	  }
	EVP_CIPHER_CTX_cleanup (&ctx);
	return 1;
}

int
main (int argc, char *argv[])
{
	int flags1 = 0, flags2 = 0, outfd, infd, decfd;
	mode_t mode;
	char choice, temp;
	int done = 0, n, olen;

	bzero (&key, 16);
	bzero (&iv, 8);
	bzero (&mode, sizeof (mode));

	flags1 = flags1 | O_RDONLY;
	flags2 = flags2 | O_RDONLY;
	flags2 = flags2 | O_WRONLY;
	flags2 = flags2 | O_CREAT;

	mode = mode | S_IRUSR;
	mode = mode | S_IWUSR;

	while (!done)
	  {
		  printf ("E - Encrypt a file\n");
		  printf ("D - Decrypt a file\n");
		  printf ("G - Generate a key\n");
		  printf ("Q - Quit\n");

		  choice = getchar ();
		  temp = getchar ();

		  switch (choice)
		    {
		    case 'e':
		    case 'E':

			    if ((infd = open (argv[1], flags1, mode)) == -1)
				    perror ("open input file error");

			    if ((outfd = open (argv[2], flags2, mode)) == -1)
				    perror ("open output file error");

			    encrypt (infd, outfd);

			    close (infd);
			    close (outfd);
			    break;

		    case 'd':
		    case 'D':

			    if ((outfd = open (argv[2], flags1, mode)) == -1)
				    perror ("open output file error");

			    if ((decfd = open (argv[3], flags2, mode)) == -1)
				    perror ("open output file error");

			    decrypt (outfd, decfd);

			    close (outfd);
			    fsync (decfd);
			    close (decfd);
			    break;

		    case 'g':
		    case 'G':
			    generate_key ();
			    break;

		    case 'Q':
		    case 'q':
			    done = 1;
			    break;

		    default:
			    printf ("ERROR: Unrecognized command.  Try again.\n");
			    break;
		    }
	  }
	return 0;
}
*******************************************/

#endif
