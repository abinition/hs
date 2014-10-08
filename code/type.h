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
extern sData*		gHyp_type_assign ( sInstance*,sFrame *,sData*,sData*,
					   sBYTE, sLOGICAL, sLOGICAL );
extern void		gHyp_type_int ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_float ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_str ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_short ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_ushort ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_long ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_ulong ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_double ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_byte ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_ubyte ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_char ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_uchar ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_list ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_binary ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_boolean ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_hex ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_handle ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_octal ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_unicode ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_attr ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_typeof ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_valueof ( sInstance*, sCode*, sLOGICAL ) ;
extern void		gHyp_type_scopeof ( sInstance*, sCode*, sLOGICAL ) ;
