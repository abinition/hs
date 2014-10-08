/*
 * $Id$
 *
 * $Log$
 * Revision 1.2  2004/12/26 17:36:01  jbergsma
 * Definition of new functions to handle client mailslots
 *
 * Revision 1.1  2004/12/17 17:58:18  jbergsma
 * Interface functions between HyperScript and the WebPickle control
 *
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#define AS_WINDOWS 1

/* Local Includes for HYPERSCRIPT */
#include "typdef.h"	/* Typdef's */
#include "hypdef.h"	/* HyperScript specific */

extern char gzClientInstanceName[INSTANCE_SIZE+1];	// This client instance name
extern char gzClientConcept[TARGET_SIZE+1] ;		// This client concept name
extern char gzClientConceptPath[MAX_PATH_SIZE+1] ;	// This client concept path

char *gHyp_hs_get_concept_name ( ) ;
char *gHyp_hs_get_concept_path ( ) ;
void gHyp_client_set_concept_name ( char * ) ;
void gHyp_client_set_concept_path ( ) ;
char *gHyp_client_get_concept_path ( ) ;
void gHyp_client_jeval ( char * ) ;
char *gHyp_hs_get_mailslot_name ( ) ;
sBYTE gHyp_hs_get_state ( ) ;
void gHyp_notify_client_mailslot ( ) ;
HANDLE	gHyp_client_open_mailslot ( const char *, sLOGICAL, sLOGICAL, sLOGICAL ) ;
UINT gHyp_hs_parse( const char * ) ;
UINT gHyp_hs_eval( const char * ) ;
void gHyp_hs_output_listbox ( const char * ) ;

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif