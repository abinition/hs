/*
 * Modifications:
 *
 * $Log: type.h,v $
 * Revision 1.5  2005-03-09 04:20:28  bergsma
 * Added scopeof function
 *
 * Revision 1.4  2004/07/06 00:28:14  bergsma
 * Added 'valueof()' function.
 *
 * Revision 1.3  2003/06/05 21:54:23  bergsma
 * Added new typedef 'unicode', UTF-16.
 * Required for new unicode type:
 * 1. New data type in data object
 * 2. Special unicode output for xdescribe function
 * 3. New TYPE_UNICODE in hypdef.h
 * 4. New token 0uNNNNN format, like 0xNNNNN and 0oNNNNN
 * 5. New function unicode()
 * 6. New SECS II data type.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
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
extern void		gHyp_type_var ( sInstance*, sCode*, sLOGICAL ) ;
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
