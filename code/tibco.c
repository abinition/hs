#ifdef AS_TIBCO
static sLOGICAL lHyp_sock_initTIBCO ()
{
  char  
    *service=NULL,
    *network=NULL,
    *daemon=NULL ;
	
  /* Initialize model elements */
  ptmh=NULL;
  mptr=NULL;

  /* Load exchange model from file (or logical name/environment variable) */
  if ( loadExchangeModel ( &ptmh, "outbound.msg" ) )
    briefWholeModel ( ptmh ) ;
  else {
    briefWholeModel ( ptmh ) ;
    return gHyp_util_logError ( 
      "Failed to load exchange message definition: '%s'",
      "outbound.msg" ) ;
  }

  /* Initialize a rv session to send messages */
  rverr = rv_Init ( &stdSession, service, network, daemon ) ;
  if ( rverr != RV_OK )
    return gHyp_util_logError ( "Failed to initialize session '%s'",
				rv_ErrorText ( NULL, rverr ) ) ;
  return TRUE ;
}
#endif
