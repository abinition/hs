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
extern void	gHyp_fileio_jdescribe ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_jfdescribe ( sInstance*, sCode*, sLOGICAL ) ;
extern void	gHyp_fileio_jsdescribe ( sInstance*, sCode*, sLOGICAL ) ;
extern sData*   gHyp_fileio_describeData ( sInstance *pAI, sData *pData, char, sLOGICAL, sLOGICAL, int * ) ;
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
extern void    	gHyp_fileio_load_csv ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_csv ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_checksum ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_crc32 ( sInstance*, sCode*, sLOGICAL ) ;
/*
extern void    	gHyp_fileio_sprintf ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_printf ( sInstance*, sCode*, sLOGICAL ) ;
extern void    	gHyp_fileio_fprintf ( sInstance*, sCode*, sLOGICAL ) ;
*/
