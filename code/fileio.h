/**********************	FUNCTION PROTOTYPES ********************************/
/*
 * Modifications:
 *
 * $Log: fileio.h,v $
 * Revision 1.12  2007-03-19 05:32:08  bergsma
 * New functions: min, max, pow, hypot, abs, acos, asin, atan,  ceil, cos,
 *  floor, log10, logN, sin, sqrt, tan, exp printf, sprintf, fprintf
 *
 * Revision 1.11  2007-03-15 01:46:36  bergsma
 * New functions.
 *
 * Revision 1.10  2007-03-15 01:04:39  bergsma
 * Added checksum function
 *
 * Revision 1.9  2006-09-16 19:41:10  bergsma
 * Added gHyp_fileio_dataTypeStr
 *
 * Revision 1.8  2005/12/10 00:30:30  bergsma
 * HS 3.6.5
 *
 * Revision 1.7  2004/04/29 08:32:40  bergsma
 * Added functions frwite,fread,load_binary
 *
 * Revision 1.6  2003/01/12 10:13:15  bergsma
 * Add 'ending' character to describeData.
 *
 * Revision 1.5  2002/11/29 19:58:48  bergsma
 * Removed ^M
 *
 * Revision 1.4  2002/11/29 19:50:00  bergsma
 * Add load() function.
 *
 * Revision 1.3  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 * Revision 1.2  2002/09/16 15:24:18  bergsma
 * Added gHyp_fileio_describeData
 *
 */
extern sBYTE	gHyp_fileio_dataType ( char *dt ) ;
extern char*	gHyp_fileio_dataTypeStr ( int ) ;
extern void	gHyp_fileio_printf ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_sprintf ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_fprintf ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_describe ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_sdescribe ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_fdescribe ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_xdescribe ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_xfdescribe ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_xsdescribe ( sInstance*, sCode*, sLOGICAL ) ;
extern sData*   gHyp_fileio_describeData ( sInstance *pAI, sData *pData, char, sLOGICAL, int * ) ;
extern void	gHyp_fileio_fopen ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_fclose ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_fputs ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_fwrite ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_fread ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_fput ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_getTokens ( char *pBuf, sData *pResult ) ;
extern void    	gHyp_fileio_fgets ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_fget ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_get ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_gets ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_log ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_put ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_puts ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_unlink ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_echo ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_load ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_load_binary ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_checksum ( sInstance*, sCode*, sLOGICAL ) ;
/*
extern void    	gHyp_fileio_sprintf ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_printf ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_fprintf ( sInstance*, sCode*, sLOGICAL ) ;
*/
