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
 *	$Log: fileio.c,v $
 *	Revision 1.71  2012-02-09 22:53:23  bergsma
 *	Unintialized vars.
 *	
 *	Revision 1.70  2011-07-03 16:35:19  bergsma
 *	Added load_csv and csv
 *	
 *	Revision 1.69  2011-05-30 17:05:56  bergsma
 *	When doing fputs, and line already ends with \n, then output line.
 *	
 *	Revision 1.68  2011-02-24 05:12:27  bergsma
 *	Adjusting values for MAX_OUTPUT_LENGTH and MAX_INPUT_LENGTH
 *	
 *	Revision 1.67  2011-02-24 03:13:32  bergsma
 *	Adjusting values for MAX_OUTPUT_LENGTH and MAX_INPUT_LENGTH
 *	
 *	Revision 1.66  2011-02-20 00:58:21  bergsma
 *	Use MAX_INPUT instead of MAX_OUTPUT
 *	
 *	Revision 1.64  2011-01-08 21:27:00  bergsma
 *	Add JSON functionality - jdescribe.
 *	
 *	Revision 1.63  2010-04-23 05:15:20  bergsma
 *	Error expanding attr values in XML when they are > 256 characters long.
 *	
 *	Revision 1.62  2010-03-17 08:18:15  bergsma
 *	Fixed logic for outputing XML for tags with multiple elements
 *	
 *	Revision 1.61  2010-02-01 22:27:18  bergsma
 *	Builld 100130, HS 3.9.0
 *	
 *	Revision 1.60  2010-01-28 04:26:34  bergsma
 *	Build 3.9 1/25/2010
 *	
 *	Revision 1.59  2010-01-08 02:44:57  bergsma
 *	Added ssl_md5(), enhanced ssl_digest.
 *	Fixed urldecode, missing ":"
 *	Enabled object calls, ie:  text.strtok( ) and the like...
 *	
 *	Revision 1.58  2009-12-13 04:00:16  bergsma
 *	Back to correcting XML parsing and unparsing.
 *	With AS_XML_NON_STANDARD not set, elements could be missed
 *	when parsing XML.  So, AS_XML_NON_STANDARD was made standard.
 *	
 *	Revision 1.57  2009-12-08 21:00:15  bergsma
 *	Comments added
 *	
 *	Revision 1.56  2009-11-17 15:57:58  bergsma
 *	Added AS_XML_NON_STANDARD to preserve an old feature
 *	that does not put \n after the last value in an XML tag's values.
 *	It is better to keep the \n as this helps in conversion to and from HS data
 *	structures
 *	
 *	Revision 1.55  2009-09-21 05:18:03  bergsma
 *	sdescribe and xsdescribe have a "doIndent" second argument
 *	
 *	Revision 1.54  2009-03-13 07:48:16  bergsma
 *	GD refinements.
 *	Added BUILD_VERSION
 *	
 *	Revision 1.53  2008-08-09 04:46:52  bergsma
 *	Allow attribute values to exceed INTERNAL_VALUE_SIZE
 *	
 *	Revision 1.52  2008-07-27 15:09:59  bergsma
 *	When outputing \n is xsdescribe, don't do it for the last value of a tag's elements.
 *	
 *	Revision 1.51  2008-07-02 01:01:46  bergsma
 *	Add \n after every end tag for xsdescribe
 *	
 *	Revision 1.50  2008-05-30 01:21:43  bergsma
 *	puts() function overflows if too many quotes.
 *	
 *	Revision 1.49  2008-05-29 04:28:39  bergsma
 *	xsdescribe fix
 *	
 *	Revision 1.48  2008-05-27 16:13:29  bergsma
 *	(typedef) needed
 *	
 *	Revision 1.47  2008-05-12 17:37:43  bergsma
 *	checksum function can read data as well as files.
 *	
 *	Revision 1.46  2008-02-17 02:09:24  bergsma
 *	Was using back-quote instead of regular quote for TOKEN_IDENTIFIED,
 *	causing echo to fail because backquotes are for TOKEN_RAW.
 *	
 *	Revision 1.45  2008-02-12 23:09:29  bergsma
 *	V3.8.2 (compiled with VS C 2008)
 *	
 *	Revision 1.44  2007-06-20 05:17:34  bergsma
 *	Fixes for RUN_RESTRICTED, fixes for describe.
 *	
 *	Revision 1.43  2007-06-10 05:14:50  bergsma
 *	When doing sfdescribe, make output lengths shorter
 *	
 *	Revision 1.42  2007-05-30 06:24:22  bergsma
 *	no message
 *	
 *	Revision 1.41  2007-05-26 22:08:43  bergsma
 *	ADD -w "Ward" flag
 *	
 *	Revision 1.40  2007-03-26 00:03:19  bergsma
 *	Was not closing file handle in checksum function
 *	
 *	Revision 1.39  2007-03-22 16:33:43  bergsma
 *	Checksum returns a hex value.
 *	
 *	Revision 1.38  2007-03-22 04:45:38  bergsma
 *	Checksum returns a hex "handle" like value.
 *	
 *	Revision 1.37  2007-03-22 04:28:17  bergsma
 *	Initialize variable insep in checksum
 *	
 *	Revision 1.36  2007-03-21 16:01:23  bergsma
 *	min/max functions use macro MIN/MAX.
 *	Correct buf/ubuf pointer discrepency in checksum().
 *	
 *	Revision 1.35  2007-03-21 02:21:31  bergsma
 *	Added round() function.
 *	
 *	Revision 1.34  2007-03-19 05:32:07  bergsma
 *	New functions: min, max, pow, hypot, abs, acos, asin, atan,  ceil, cos,
 *	 floor, log10, logN, sin, sqrt, tan, exp printf, sprintf, fprintf
 *	
 *	Revision 1.33  2007-03-15 01:04:39  bergsma
 *	Added checksum function
 *	
 *	Revision 1.32  2006-11-25 03:08:55  bergsma
 *	Incease size of buffer for getStr to accomodate larger strings.
 *	
 *	Revision 1.31  2006/10/01 21:47:58  bergsma
 *	Dont' call the XML datatype attribute 'type', call it 'datatype' instead, otherwise
 *	it will conflict with the well known input tag attribute called 'type'
 *	
 *	Revision 1.30  2006/09/25 05:08:35  bergsma
 *	Hideous bug, was subtracting 2 from pOutput and inserting a null. OMG!
 *	
 *	Revision 1.29  2006/09/17 21:22:43  bergsma
 *	Implement data typing for XML conversions
 *	
 *	Revision 1.28  2006/09/16 19:41:10  bergsma
 *	Added gHyp_fileio_dataTypeStr
 *	
 *	Revision 1.27  2006/08/22 18:45:32  bergsma
 *	Resolve Win32 problem with unlink
 *	
 *	Revision 1.26  2006/04/04 14:57:30  bergsma
 *	When converting to XML, an empty attr should come up as "", not " "
 *	
 *	Revision 1.25  2006/02/09 05:09:02  bergsma
 *	Buffer overflow when describing CHAR or ATTR arrays exceeding 5000 elements.
 *	
 *	Revision 1.24  2005/12/10 00:30:30  bergsma
 *	HS 3.6.5
 *	
 *	Revision 1.23  2005/04/13 13:45:54  bergsma
 *	HS 3.5.6
 *	Added sql_toexternal.
 *	Fixed handling of strings ending with bs (odd/even number of backslashes)
 *	Better handling of exception condition.
 *	
 *	Revision 1.22  2005/01/25 05:44:47  bergsma
 *	Don't trim newline from end of lines when using load() function
 *	
 *	Revision 1.21  2004/11/02 22:57:58  bergsma
 *	When displaying &#nnnn; unicode characters, do no add trailing space.
 *	
 *	Revision 1.20  2004/10/27 18:24:07  bergsma
 *	HS 3.3.2
 *	1. Fix bug with SQL read when str size is > 512 bytes, use _data_ blobs
 *	2. Fix bug with XML output, forgetting ";" in unicode output.
 *	3. In |TOKEN|VALUE|VALUE| part of message, use unparse on TOKEN
 *	as well as VALUE.
 *	4. Add utility function util_breakStream.
 *	
 *	Revision 1.19  2004/10/16 04:38:52  bergsma
 *	Fixes to describe() for data that is for transported in a message.
 *	
 *	Revision 1.18  2004/07/01 02:02:34  bergsma
 *	Added 'specialChars' argument to the toexternal() function.
 *	
 *	Revision 1.17  2004/05/15 02:09:59  bergsma
 *	In openvms, fread fails to detect EOF, need stat to pre-get size.
 *	
 *	Revision 1.16  2004/04/29 08:32:40  bergsma
 *	Added functions frwite,fread,load_binary
 *	
 *	Revision 1.15  2003/07/27 00:54:24  bergsma
 *	Remove ^M
 *	
 *	Revision 1.14  2003/07/27 00:53:27  bergsma
 *	In fgets(), remove newline/carriageReturn at end of line only in PROMIS
 *	
 *	Revision 1.13  2003/06/05 21:54:23  bergsma
 *	Added new typedef 'unicode', UTF-16.
 *	Required for new unicode type:
 *	1. New data type in data object
 *	2. Special unicode output for xdescribe function
 *	3. New TYPE_UNICODE in hypdef.h
 *	4. New token 0uNNNNN format, like 0xNNNNN and 0oNNNNN
 *	5. New function unicode()
 *	6. New SECS II data type.
 *	
 *	Revision 1.12  2003/04/23 22:47:42  bergsma
 *	In load(), an extra argument specifies the maximum number of lines to load.
 *	In sdescribe() functions, extra spaces are removed.
 *	
 *	Revision 1.11  2003/04/11 14:20:11  bergsma
 *	When xdescribing PRE or SCRIPT, do not expand double quotes to \".
 *	
 *	Revision 1.10  2003/04/11 01:59:34  bergsma
 *	When describing a literal, the literal must be in external format, and depending on whether
 *	or not the literal is for XML, a message, or normal.
 *	
 *	Revision 1.9  2003/04/04 16:47:26  bergsma
 *	When outputing <b>, <i>, etc... do not start new lines when describing.
 *	
 *	Revision 1.8  2003/01/14 15:53:24  bergsma
 *	Use 'ending' variable in arg list rather than expression.
 *	
 *	Revision 1.7  2003/01/12 13:45:37  bergsma
 *	V3.1.0
 *	1. Incorporate instance_pushSTATUS in functions.
 *	2. Fix error reporting typedef in describe, missing "hex"
 *	3. In describe, compact result if for a message.
 *	4. In describe, allow ending to be ";", ",", or " ".
 *	5. Make unlink() function subject to AS_RESTRICTED compile option
 *	
 *	Revision 1.6  2002/11/29 19:52:10  bergsma
 *	1. Improved handling for <script> <style> <pre> and <textarea> tags when
 *	doing an xdescribe function.
 *	2. Added load() function.
 *	
 *	Revision 1.5  2002/11/21 23:50:57  bergsma
 *	When describing XML with script (javascript) tags, do not interpret data
 *	inside the tag.
 *	
 *	Revision 1.4  2002/09/16 18:01:38  bergsma
 *	Add isMSG argument to lHyp_fileio_describe and lHyp_fileio_describe2.  When
 *	set to true, the gHyp_util_unparseString will convert "|" to "\174".
 *	Currently used in gHyp_fileio_describeData().
 *	
 *	Revision 1.3  2002/09/16 15:23:44  bergsma
 *	Added gHyp_fileio_describeData, used by query() and event() functions,
 *	when the argument style is new. See route.c
 *	
 *	Revision 1.2  2002/09/03 21:21:43  bergsma
 *	Increased size of buffer to handle expansion of characters to XML '&' format
 *	
 */

/**********************	HYPERSCRIPT INTERFACE ********************************/

#include <sys/stat.h>

#include "auto.h"	/* System Interface and Function Prototypes */

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ************************/
#define MAX_TYPES 23
static char *gzaType[MAX_TYPES] = { "null",	/* 0 */
			     "list",	/* 1 */
			     "str",	/* 2 */
			     "byte",	
			     "ubyte",
			     "boolean",
			     "binary",
			     "char",	
			     "uchar",	
			     "attr",
			     "short",	
			     "ushort",
			     "long",
			     "ulong",
			     "int",
			     "float",
			     "double",
			     "hex",
			     "octal",
			     "unicode",
			     "handle",
			     "datetime",
			     "raw"	  /* 22 */
} ;


static unsigned long gsCrctab[256] = {
  0x0,
  0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B,
  0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6,
  0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
  0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC,
  0x5BD4B01B, 0x569796C2, 0x52568B75, 0x6A1936C8, 0x6ED82B7F,
  0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A,
  0x745E66CD, 0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
  0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5, 0xBE2B5B58,
  0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033,
  0xA4AD16EA, 0xA06C0B5D, 0xD4326D90, 0xD0F37027, 0xDDB056FE,
  0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
  0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4,
  0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D, 0x34867077, 0x30476DC0,
  0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5,
  0x2AC12072, 0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16,
  0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA, 0x7897AB07,
  0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C,
  0x6211E6B5, 0x66D0FB02, 0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1,
  0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
  0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B,
  0xBB60ADFC, 0xB6238B25, 0xB2E29692, 0x8AAD2B2F, 0x8E6C3698,
  0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D,
  0x94EA7B2A, 0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E,
  0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2, 0xC6BCF05F,
  0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34,
  0xDC3ABDED, 0xD8FBA05A, 0x690CE0EE, 0x6DCDFD59, 0x608EDB80,
  0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
  0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A,
  0x58C1663D, 0x558240E4, 0x51435D53, 0x251D3B9E, 0x21DC2629,
  0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C,
  0x3B5A6B9B, 0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
  0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623, 0xF12F560E,
  0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65,
  0xEBA91BBC, 0xEF68060B, 0xD727BBB6, 0xD3E6A601, 0xDEA580D8,
  0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
  0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2,
  0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B, 0x9B3660C6, 0x9FF77D71,
  0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74,
  0x857130C3, 0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640,
  0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C, 0x7B827D21,
  0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A,
  0x61043093, 0x65C52D24, 0x119B4BE9, 0x155A565E, 0x18197087,
  0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
  0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D,
  0x2056CD3A, 0x2D15EBE3, 0x29D4F654, 0xC5A92679, 0xC1683BCE,
  0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB,
  0xDBEE767C, 0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18,
  0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4, 0x89B8FD09,
  0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662,
  0x933EB0BB, 0x97FFAD0C, 0xAFB010B1, 0xAB710D06, 0xA6322BDF,
  0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

#define CRC32_XINIT 0xFFFFFFFFL		/* initial value */
#define CRC32_XOROT 0xFFFFFFFFL		/* final xor value */
#define MINIMUM_CHECKSUM_LEN	 8
#define MAXIMUM_CHECKSUM_LEN	99
/* NAACCR 6.0 Specifications */
#define NAACCR_60_CHECKSUM_POS	942
#define NAACCR_60_CHECKSUM_LEN	10
/* function prototypes */
unsigned long CalcCRC32(unsigned char *, unsigned long, unsigned long, unsigned long);
static unsigned long  gsCrctable32[256] =
{
 0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL,
 0x076DC419L, 0x706AF48FL, 0xE963A535L, 0x9E6495A3L,
 0x0EDB8832L, 0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
 0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L, 0x90BF1D91L,
 0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,
 0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
 0x136C9856L, 0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL,
 0x14015C4FL, 0x63066CD9L, 0xFA0F3D63L, 0x8D080DF5L,
 0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L, 0xA2677172L,
 0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
 0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L,
 0x32D86CE3L, 0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
 0x26D930ACL, 0x51DE003AL, 0xC8D75180L, 0xBFD06116L,
 0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L, 0xB8BDA50FL,
 0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
 0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL,
 0x76DC4190L, 0x01DB7106L, 0x98D220BCL, 0xEFD5102AL,
 0x71B18589L, 0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
 0x7807C9A2L, 0x0F00F934L, 0x9609A88EL, 0xE10E9818L,
 0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
 0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL,
 0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L,
 0x65B0D9C6L, 0x12B7E950L, 0x8BBEB8EAL, 0xFCB9887CL,
 0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
 0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,
 0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL,
 0x4369E96AL, 0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
 0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL, 0xDD0D7CC9L,
 0x5005713CL, 0x270241AAL, 0xBE0B1010L, 0xC90C2086L,
 0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
 0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L,
 0x59B33D17L, 0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL,
 0xEDB88320L, 0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
 0xEAD54739L, 0x9DD277AFL, 0x04DB2615L, 0x73DC1683L,
 0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,
 0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
 0xF00F9344L, 0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL,
 0xF762575DL, 0x806567CBL, 0x196C3671L, 0x6E6B06E7L,
 0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
 0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
 0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L,
 0xD1BB67F1L, 0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
 0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L, 0x41047A60L,
 0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL, 0x4669BE79L,
 0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
 0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL,
 0xC5BA3BBEL, 0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L,
 0xC2D7FFA7L, 0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
 0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL, 0x026D930AL,
 0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
 0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
 0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L,
 0x86D3D2D4L, 0xF1D4E242L, 0x68DDB3F8L, 0x1FDA836EL,
 0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
 0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,
 0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L,
 0xA00AE278L, 0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
 0xA7672661L, 0xD06016F7L, 0x4969474DL, 0x3E6E77DBL,
 0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L, 0x37D83BF0L,
 0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
 0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L,
 0xBAD03605L, 0xCDD70693L, 0x54DE5729L, 0x23D967BFL,
 0xB3667A2EL, 0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
 0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL, 0x2D02EF8DL
};

/**********************	FUNCTION DEFINITIONS ********************************/

#if defined (AS_VMS) && defined ( __cplusplus )
extern "C" int unlink ( const char * ) ;
#else
#ifndef AS_WINDOWS
extern int unlink ( const char * ) ;
#endif
#endif


sBYTE gHyp_fileio_dataType ( char *dt ) {

  int i ;

  for ( i=0; i<MAX_TYPES; i++ ) {
    if ( strcmp ( dt, gzaType[i] ) == 0 ) return i ;
  }
  return TYPE_LIST ;
}

char* gHyp_fileio_dataTypeStr ( int i ) {

  if ( i >= 0 && i < MAX_TYPES ) 
    return gzaType[i] ;
  else
    return NULL ;
}

static int lHyp_fileio_describe2 ( sData *pParent, 
				   int parentContext,
				   sLOGICAL firstValue,
				   sLOGICAL lastValue,
				   int indent,
				   sLOGICAL doIndent,
				   char *pOutput,
				   FILE *fp,
				   sLOGICAL isXML,
                                   sLOGICAL isJSON,
				   sLOGICAL isPRE,
				   sLOGICAL isTXT,
				   sLOGICAL isMSG,
				   sLOGICAL isSCR,
				   sLOGICAL isMRK,
				   sLOGICAL isAttrOn,
				   char ending,
				   sData *pResult,
				   int *pContentLength ) 
{
  sData
    *pData ;

  int
    n,
    i,
    outputLen,
    indentLen,
    outLen,
    newOffset=0,
    tmpOutputLen=0,
    size,
    ss,
    maxLen,
    context,
    numNonAttributes=0 ;

  char
    c,
    spacer[2] = {0,0},
    tag[TOKEN_SIZE+1],
    *pOut,
    *pValue ;

 char
    newOutput[MAX_STREAM_LENGTH],
    tmpOutput[MAX_STREAM_LENGTH],
    prefixStr[VALUE_SIZE+1],
    suffixStr[VALUE_SIZE+1],
    value[MAX_INPUT_LENGTH],
    value2[MAX_INPUT_LENGTH] ;

  sBYTE
    dataType,
    tokenType,
    parentDataType,
    parentTokenType ;

  sLOGICAL
    firstVal,
    lastVal,
    noValue,
    isParentDynamic,
    isParentVector = FALSE ;

  sData
    *pLine ;

  outputLen = strlen ( pOutput ) ;

  for ( i=0; i<indent; i++ ) prefixStr[i] = ' ' ;
  /* Calculate indent */
  prefixStr[indent] = '\0' ;

  indentLen = doIndent ? indent : 0 ; 

  if ( indent > 0 ) {
    if ( lastValue ) 
      strcpy ( suffixStr, "" ) ;
    else {
      if ( isXML )
	strcpy ( suffixStr, " " ) ;
      else if ( isJSON )
	strcpy ( suffixStr, "," ) ;
      else
	strcpy ( suffixStr, "," ) ;
    }
  }
  else {
    if ( lastValue ) {
      suffixStr[0] = ending ; suffixStr[1] = '\0' ;
    }
  }

 
  /* Print out information on data being described */
  parentTokenType = gHyp_data_tokenType ( pParent ) ;
  parentDataType = gHyp_data_dataType ( pParent ) ;

  if ( parentTokenType != TOKEN_VARIABLE ) {
    /* Output what is there and start a new line */ 
    if ( outputLen > indentLen ) {
      pOut = ( isPRE || isTXT ) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
      outLen = strlen ( pOut ) ;
      if ( !isMRK ) {
        if ( fp )
	  fprintf ( fp, "%s\n", pOut ) ;
        else if ( pResult ) {
	  pLine = gHyp_data_new ( NULL ) ;
	  gHyp_data_setStr ( pLine, pOut ) ;
	  gHyp_data_append ( pResult, pLine ) ;
	}
        else
	  gHyp_util_log ( pOut ) ;
	*pContentLength += outLen ;
        *pOutput = '\0' ;
        if ( doIndent ) strcat ( pOutput, prefixStr ) ;
        outputLen = strlen ( pOutput ) ;
      }
    }
  }

  switch ( parentTokenType ) {

  case TOKEN_REFERENCE :
    
    if ( isXML )
      newOffset = sprintf ( newOutput,
			    "%s%s",  
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;
    else if ( isJSON )
      newOffset = sprintf ( newOutput,
			    "%s\"%s\"",  
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;
    else {

      if ( !isMSG )
        newOffset = sprintf ( newOutput,
			    "&'%s'%s",
			    gHyp_data_getLabel ( pParent ),
    			    suffixStr ) ;
      else {
	strcpy ( value, gHyp_data_getLabel( pParent ) ) ;
	n = gHyp_util_unparseString ( value2, value, strlen(value), MAX_OUTPUT_LENGTH, TRUE, FALSE, FALSE,"'" ) ;
        newOffset = sprintf ( newOutput,
			    "&'%s'%s",
			    value2,
    			    suffixStr ) ;

      }
    }
    break ;
    
  case TOKEN_LITERAL : 
    
    pValue = value ;
    if ( isMSG )
      n = gHyp_data_getStr_msg ( pParent, 
			      value, 
			      MAX_OUTPUT_LENGTH,
			      0,
			      FALSE ) ;

    else if ( isXML ) {
      if ( !isSCR && !isPRE && !isTXT ) 
        n = gHyp_data_getStr_xml ( pParent, 
			      value, 
			      MAX_OUTPUT_LENGTH,
			      0,
			      FALSE ) ;

      else 
        n = gHyp_data_getStr_nonulls ( pParent, 
			        value, 
			        MAX_OUTPUT_LENGTH,
			        0,
			        FALSE ) ;

    }
    else if ( isJSON ) {
      n = gHyp_data_getStr_external ( pParent, 
			      value, 
			      MAX_OUTPUT_LENGTH,
			      0,
			      FALSE ) ;

    }
    else
      n = gHyp_data_getStr_external ( pParent, 
			      value, 
			      MAX_OUTPUT_LENGTH,
			      0,
			      FALSE ) ;

    if ( isXML ) {

      if ( pResult ) {

	/* Leave out \n if there is only one value */
	pData = gHyp_data_getParent ( pParent ) ;
	if ( pData && 
	     gHyp_data_getLast  ( pData ) == pParent && 
	     gHyp_data_getFirst ( pData ) == pParent)
          newOffset = sprintf ( newOutput,
			    "%s",
			    pValue ) ;
	else
	  
	  newOffset = sprintf ( newOutput,
			    "%s\n",
			    pValue ) ;

      }

      else 
        newOffset = sprintf ( newOutput,
			    "%s",
			    pValue ) ;
    }
    else if ( isJSON )
      newOffset = sprintf ( newOutput,
			    "\"%s\"%s",
			    pValue,
			    suffixStr ) ;
    else 
      newOffset = sprintf ( newOutput,
			    "\"%s\"%s",
			    pValue,
			    suffixStr ) ;
    break ;
    
  case TOKEN_UNIDENTIFIED : 
    
    if ( isXML )
      /*
      newOffset = sprintf ( newOutput,
			    "<%s datatype=\"str\">%s", 
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;
      */
      newOffset = sprintf ( newOutput,
			    "<%s>%s", 
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;
    else if ( isJSON )

        newOffset = sprintf ( newOutput,
			    "\"%s\"%s",
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;

    else
      if ( !isMSG ) 
        newOffset = sprintf ( newOutput,
			    "'%s'%s",
			    gHyp_data_getLabel ( pParent ),
			    suffixStr ) ;
      else {
	strcpy ( value, gHyp_data_getLabel( pParent ) ) ;
	n = gHyp_util_unparseString ( value2, value, strlen(value), MAX_OUTPUT_LENGTH, TRUE, FALSE, FALSE, "'" ) ;
        newOffset = sprintf ( newOutput,
			    "&'%s'%s",
			    value2,
    			    suffixStr ) ;

      }

    break ;

  case TOKEN_CONSTANT :

    if ( parentDataType == TYPE_CHAR ||
         parentDataType == TYPE_UCHAR ||
	 parentDataType == TYPE_ATTR ) {
      if ( isXML )
        newOffset = sprintf ( newOutput,
			      "%s%s",
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
      else if ( isJSON )
        newOffset = sprintf ( newOutput,
			      "%s%s",
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
      else
        newOffset = sprintf ( newOutput,
			      "\"%s\"%s",
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
    }
    else {
      if ( isXML ) {
	if ( parentDataType == TYPE_UNICODE ) {
	  newOffset = sprintf ( newOutput,
			        /*"&#%05hu;",*/ 
			        "&#x%04x;", 
			        gHyp_data_getInt ( pParent, parentContext, TRUE ) ) ;
	}
	else {
	  newOffset = sprintf ( newOutput,
			      "%s%s", 
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
	}
      }
      else if ( isJSON )
	newOffset = sprintf ( newOutput,
			      "%s%s", 
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
      else
	newOffset = sprintf ( newOutput,
			      "%s%s", 
			      gHyp_data_getLabel ( pParent ),
			      suffixStr ) ;
    }
    break ;

  case TOKEN_VARIABLE :

    isParentVector = ( gHyp_data_dataType ( pParent ) > TYPE_STRING ) ;
    
    if ( parentContext != -1 ) {
 
      /* Subscript - print out a single value from the parent variable */
      if ( isXML )
        n = gHyp_data_getStr_xml ( pParent, 
			      value, 
			      MAX_OUTPUT_LENGTH,
			      parentContext,
			      isParentVector ) ;
      else
        n = gHyp_data_getStr_external ( pParent, 
			      value, 
			      MAX_OUTPUT_LENGTH,
			      parentContext,
			      isParentVector ) ;
      pValue = value ;
      if ( isXML ) {
	if ( parentDataType == TYPE_UNICODE )
	  newOffset = sprintf ( newOutput, "%s", pValue ) ;
        else
  	  newOffset = sprintf ( newOutput, "%s%s", pValue, suffixStr ) ;
      }
      else
	newOffset = sprintf ( newOutput, "%s%s", pValue, suffixStr ) ;

      break ;
    }
    else {

      /* Non-subscripted, print out all the variable's values */
      size = gHyp_data_getCount ( pParent ) ;
      isParentDynamic = gHyp_data_isDynamic ( pParent ) ;
      
      /* This will be our new primary buffer */
      /*gHyp_util_debug("VARIABLE, '%s'", pOutput ) ;*/

      newOutput[0] = '\0' ;
      newOffset = 0 ;  

      if ( isXML ) {

	if ( parentDataType == TYPE_ATTR ) {

	  if ( !isAttrOn && outputLen > indentLen ) {
            pOut = (isPRE || isTXT ) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
	    outLen = strlen ( pOut ) ;
	    if ( fp )
	      fprintf ( fp, "%s\n", pOut ) ;
	    else if ( pResult ) {
	      pLine = gHyp_data_new ( NULL ) ;
	      gHyp_data_setStr ( pLine, pOut ) ;
	      gHyp_data_append ( pResult, pLine ) ;
	    }
	    else
	      gHyp_util_log ( pOut ) ;
	    *pContentLength += outLen ;
	    *pOutput = '\0' ;
	    if ( doIndent ) strcat ( pOutput, prefixStr ) ;
	    outputLen = strlen ( pOutput ) ;
	  }

	  if ( isAttrOn ) 
	    spacer[0] = ' ' ;
	  else
	    spacer[0] = '\0' ;

	  newOffset = sprintf ( newOutput,
				"%s%s=",
				spacer,
				gHyp_data_getLabel ( pParent ) ) ;
	}
	else {

	  /* Not an attribute - start of an element - could be a list, could be a constant */

	  /* Output what is there and start a new line */ 
	  if ( !isMRK ) {
	    if ( outputLen > indentLen ) {
	      pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
	      outLen = strlen ( pOut ) ;
	      if ( fp )
	        fprintf ( fp, "%s\n", pOut ) ;
	      else if ( pResult ) {
	        pLine = gHyp_data_new ( NULL ) ;
	        gHyp_data_setStr ( pLine, pOut ) ;
	        gHyp_data_append ( pResult, pLine ) ;
	      }
	      else 
	        gHyp_util_log ( pOut ) ;
	      *pContentLength += outLen ;
	      *pOutput = '\0' ;
	      if ( doIndent ) strcat ( pOutput, prefixStr ) ;
	      outputLen = strlen ( pOutput ) ;
	    }
	  }

	  if ( parentDataType > TYPE_STRING && parentTokenType == TOKEN_VARIABLE ) 
	   
	    newOffset = sprintf ( newOutput,
				"<%s datatype=\"%s\"",
				gHyp_data_getLabel ( pParent ),
				gzaType[gHyp_data_getDataType( pParent )] ) ;
	  else
	    newOffset = sprintf ( newOutput,
				"<%s",
				gHyp_data_getLabel ( pParent ) ) ;


	  isAttrOn = TRUE ;

	  if ( isXML ) {
	    strcpy ( tag, gHyp_data_getLabel(pParent) ) ;
	    gHyp_util_lowerCase( tag, strlen ( tag ) ) ;
	    if ( !isPRE ) isPRE = ( strcmp ("pre",tag)==0) ;
	    if ( !isTXT ) isTXT = ( strcmp ("textarea",tag)==0 ) ;
	    if ( !isSCR ) isSCR = ( strcmp ( "script",tag)==0 || strcmp("style",tag)==0 ) ;
	    isMRK = ( strcmp ("b", tag ) == 0  || 
		strcmp ("big",tag) == 0 ||
		strcmp ("blink",tag) == 0 ||
		strcmp ("i",tag) == 0 ||
		strcmp ("small",tag) == 0 ||
		strcmp ("strike",tag) == 0 ||
		strcmp ("sub",tag) == 0 ||
		strcmp ("sup",tag) == 0 ||
		strcmp ("tt",tag) == 0 ) ;
	  }
	}
      }
      else {

	/* Not XML */

	/* Output what is there and start a new line */
	if ( outputLen > indentLen ) {
	  pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
 	  outLen = strlen ( pOut ) ;
	  if ( fp )
	    fprintf ( fp, "%s\n", pOut ) ;
	  else if ( pResult ) {
	    pLine = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pLine, pOut ) ;
	    gHyp_data_append ( pResult, pLine ) ;
	  }
	  else 
	    gHyp_util_log ( pOut ) ;
	  *pContentLength += outLen ;
	  *pOutput = '\0' ;
	  if ( doIndent ) strcat ( pOutput, prefixStr ) ;
	  outputLen = strlen ( pOutput ) ;
	}

	if ( isMSG ) {
	  strcpy ( value, gHyp_data_getLabel( pParent ) ) ;
          gHyp_util_unparseString ( value2, value, strlen(value), MAX_OUTPUT_LENGTH, TRUE, FALSE, FALSE,"'" ) ;
	}
	else
	  strcpy ( value2, gHyp_data_getLabel( pParent ) ) ;
	
        if ( isJSON ) {
          if ( gHyp_data_getCount ( pParent ) == 1 || gHyp_data_getDataType ( pParent ) == TYPE_ATTR )
	    newOffset = sprintf ( newOutput,
                             "\"%s\" : ",
			     value2 ) ;
          else
            newOffset = sprintf ( newOutput,
                              "\"%s\" : {",
			     value2 ) ;
        }
        else {
	  if ( isParentDynamic ) {
	    newOffset = sprintf ( newOutput,
			     "%s '%s' = {",
			     gzaType[parentDataType],
			     value2 ) ;
	  }
	  else {
	    newOffset = sprintf ( newOutput, 
			     "%s '%s'[%u] = {",
			     gzaType[parentDataType],
			     value2,
			     size ) ;
	  }
        }
      }

      /* Print (recursively) all the parents values */

      /* Special case, print out CHAR and UCHAR and ATTR values as character strings */
      if ( parentDataType == TYPE_CHAR || 
	   parentDataType == TYPE_UCHAR || 
	   parentDataType == TYPE_ATTR ) {

	if ( isXML ) {
	  if ( parentDataType != TYPE_ATTR ) {
	    numNonAttributes++ ;
	    isAttrOn = FALSE ;
 	    strcat ( newOutput, ">" ) ;
	    newOffset++ ;
	  }
	}

	pData = NULL ;
	context = -1 ;
	ss = -1 ;
	i = 0 ;
	noValue = TRUE ;
	value[0] = '\0' ;

	maxLen = MAX_OUTPUT_LENGTH ; /*VALUE_SIZE ;*/
	while ( (pData = gHyp_data_nextValue ( pParent, 
					       pData, 
					       &context,
					       ss ))) {
	  c = (char) gHyp_data_getRaw ( pData,
					context,
					isParentVector ) ;
	  value[i++] = c ;
	  /* If the character will be unprintable, then unparsing it
	   * will cost real estate, we must subtract from the cutoff point 
	   */
	  if ( c < 32 ) maxLen -= 3 ;  /*Figure allow 3 extra chars for expansion */
	  noValue = FALSE ;

	  if ( i  >= maxLen || c == '\n' || c == '\000' ) {

	    /* Output what is there and start a new line */
  	    maxLen = MAX_OUTPUT_LENGTH ; /*VALUE_SIZE ;*/
	    value[i] = '\0' ;
	    pValue = value ;
            
            /*
            gHyp_util_debug("Sizeof value is %d %d %d",sizeof(value),strlen(value),MAX_OUTPUT_LENGTH);
	    */
            if ( !isSCR && !isPRE && !isTXT ) {
	      gHyp_util_unparseString ( value2 ,
					value, 
					i, 
					MAX_OUTPUT_LENGTH, /*VALUE_SIZE,*/ 
					isMSG, 
					isXML,
					FALSE,
					"") ;
	      pValue = value2 ;
	    }
	    if ( isXML ) {
	      if ( parentDataType == TYPE_ATTR )
	        tmpOutputLen = sprintf ( tmpOutput, "\"%s\"", pValue ) ;
	      else
	        tmpOutputLen = sprintf ( tmpOutput, "%s",  pValue ) ;
	    }
	    else
	      tmpOutputLen = sprintf ( tmpOutput, "\"%s\",",  pValue ) ;

	    if ( tmpOutputLen + newOffset >= MAX_OUTPUT_LENGTH ) {

	      /* Output what is there and start a new line */
	      if ( newOffset > indentLen ) {
                pOut = (isPRE || isTXT) ? newOutput+MIN((unsigned int)indent+2,strspn (newOutput," \t\n")): newOutput ;
	        outLen = strlen ( pOut ) ;
	        if ( fp )
		  fprintf ( fp, "%s\n", pOut ) ;
	        else if ( pResult ) {
		  pLine = gHyp_data_new ( NULL ) ;
		  gHyp_data_setStr ( pLine, pOut ) ;
		  gHyp_data_append ( pResult, pLine ) ;
		}
	        else 
		  gHyp_util_log ( pOut ) ;
		*pContentLength += outLen ;
	        newOutput[0] = '\0' ;
		newOffset = 0 ;
	        if ( doIndent ) strcat ( newOutput, prefixStr ) ;
	        newOffset = strlen ( newOutput ) ;
	      }

	    }

	    strcat ( newOutput, tmpOutput ) ;
	    newOffset += tmpOutputLen ;
	    tmpOutputLen = 0 ;
	    tmpOutput[0] = '\0' ;
	    value[0] = '\0' ;
	    i = 0 ;
	  }
	}

	if ( noValue && isXML ) {
          /* For XML, got to have at least a <CR> if there are no values */
	  value[0] = '\0';
          i = 0 ;
	}

	if ( i >= 0 ) {

	  /* Add to output */
	  value[i] = '\0' ;
	  pValue = value ;
	  if ( !isSCR && !isPRE && !isTXT ) {	  
	    gHyp_util_unparseString ( value2, 
				      value, 
				      i, 
				      MAX_OUTPUT_LENGTH, /*VALUE_SIZE,*/ 
				      isMSG, 
				      isXML,
				      FALSE,
				      "") ;
	    pValue = value2 ;
	  }
	  if ( isXML ) {
	    if ( parentDataType == TYPE_ATTR ) 
	      tmpOutputLen = sprintf ( tmpOutput, "\"%s\"", pValue ) ;
	    else
	      tmpOutputLen = sprintf ( tmpOutput, "%s", pValue ) ;
	  }
	  else
	    tmpOutputLen = sprintf ( tmpOutput, "\"%s\"", pValue ) ;

	  strcat ( newOutput, tmpOutput ) ;
	  newOffset += tmpOutputLen ;
	  tmpOutputLen = 0 ;
	  tmpOutput[0] = '\0' ;
	}

      }
      else {
      
	/* Not CHAR or UCHAR or ATTR */
	pData = NULL ;
	ss = -1 ; 
	context = -1 ;
	firstVal = TRUE ;
	numNonAttributes = 0 ;
	while ( (pData = gHyp_data_nextValue ( pParent, 
					       pData, 
					       &context,
					       ss ) ) ) {

	  lastVal = ( context == size-1 ) ;
	  tokenType = gHyp_data_tokenType ( pData ) ;
	  dataType = gHyp_data_dataType ( pData ) ;

	  if ( dataType != TYPE_ATTR ) numNonAttributes++ ;
	  if ( isXML ) {

            /* If there are no attributes at all, then close the element with a ">" */ 
	    if ( numNonAttributes == 1 && isAttrOn ) {
	      strcat ( newOutput, ">" ) ;
	      isAttrOn = FALSE ;
	      newOffset = strlen ( newOutput ) ;
	    }

	  }

	  if ( isParentVector ) {

	    /* Print out a single value */
	    strcat ( pOutput, newOutput ) ;
	    newOffset = 0 ;
	    newOutput[0] = '\0' ;

	    newOffset = lHyp_fileio_describe2 ( pData,
						context,
						firstVal,
						lastVal,
						indent+2,
						doIndent,
						pOutput,
						fp,
						isXML,
                                                isJSON,
						isPRE,
						isTXT,
						isMSG,
						isSCR,
						isMRK,
						isAttrOn,
						ending,
						pResult,
						pContentLength ) ;
	    firstVal = FALSE ;
	  }
	  else {

	    /* List of values, some could be list variables, some constants. */

	    if ( tokenType == TOKEN_VARIABLE && newOffset > 0 ) {

	      /* New variables (except XML attributes) always starts a new line. 
	       * Flush output 
	       */
	      if ( !(isXML && isAttrOn)) { 
	        /* Pretend first value again so that the indent will occur */
	        firstVal = TRUE ;
	      }
	    }

	    /* Recursively print out the values of the variable. */
	    strcat ( pOutput, newOutput ) ;
	    newOffset = 0 ;
	    newOutput[0] = '\0' ;
	    outputLen = lHyp_fileio_describe2 ( pData,
						     -1,
						     firstVal,
						     lastVal,
						     indent+2,
						     doIndent,
						     pOutput,
						     fp,
						     isXML,
                                                     isJSON,
						     isPRE,
						     isTXT,
						     isMSG,
						     isSCR,
						     isMRK,
						     isAttrOn,
						     ending,
						     pResult,
						     pContentLength ) ;

	    if ( tokenType != TOKEN_VARIABLE )  firstVal = FALSE ;
	  }
	  
	} /* End while loop over elements */

      } /* End not CHAR or UCHAR or ATTR */

      if ( isXML ) { 

	if ( parentDataType != TYPE_ATTR ) { 

	  if ( numNonAttributes == 0 )
	    /* Only a single <element/> is required. */
	    tmpOutputLen = sprintf ( tmpOutput, " %s", "/>" ) ;	    	
	  else {
	    /* Return the trailing "</element>" */
	    if ( !isMRK ) {
	      /* Output what is there and start a new line */ 
	      if ( outputLen > indentLen ) {
                pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent+2,strspn (pOutput," \t\n")): pOutput ;
	        outLen = strlen ( pOut ) ;
	        if ( fp )
		  fprintf ( fp, "%s\n", pOut ) ;
	        else if ( pResult ) {
		  pLine = gHyp_data_new ( NULL ) ;
		  gHyp_data_setStr ( pLine, pOut ) ;
		  gHyp_data_append ( pResult, pLine ) ;
		}
	        else 
		  gHyp_util_log ( pOut ) ;
		*pContentLength += outLen ;
	        *pOutput = '\0' ;
	        if ( doIndent ) strcat ( pOutput, prefixStr ) ;
	        outputLen = strlen ( pOutput ) ;
	      }
	    }
	    if ( pResult ) 
	      tmpOutputLen = sprintf ( tmpOutput, "</%s>\n",  gHyp_data_getLabel (pParent)) ;
	    else
	      tmpOutputLen = sprintf ( tmpOutput, "</%s>",  gHyp_data_getLabel (pParent)) ;
	  }
	  strcat ( newOutput, tmpOutput ) ;
	  newOffset = strlen ( newOutput ) ;
	  tmpOutputLen = 0 ;
	  tmpOutput[0] = '\0' ;
	}

	if ( lastValue ) {
	  strcat ( pOutput, newOutput ) ;
	  newOffset = 0 ;
	  newOutput[0] = '\0' ;
	  if ( numNonAttributes > 0 ) {
	    if ( !isMRK ) {
	      pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")): pOutput ;
	      outLen = strlen ( pOut ) ;
	      if ( fp )
	        fprintf ( fp, "%s\n", pOut ) ;
	      else if ( pResult ) {
	        pLine = gHyp_data_new ( NULL ) ;
	        gHyp_data_setStr ( pLine, pOut ) ;
	        gHyp_data_append ( pResult, pLine ) ;
	      }
	      else 
	        gHyp_util_log ( pOut ) ;

	      *pContentLength += outLen ;
	      *pOutput = '\0' ;
	      if ( doIndent ) strcat ( pOutput, prefixStr ) ;
	      outputLen = strlen ( pOutput ) /*- 2 */;
	      *(pOutput+outputLen) = '\0' ;
	    }
	  }
	}
      }
      else {

        /* Not XML - JASON or HS */

	/* Return the trailing "}" */
	if ( lastValue && indent == 0 ) {
	  suffixStr[0] = ending ; suffixStr[1] = '\0' ;
	}
	
	/* Output what is there and start a new line */ 
	if ( outputLen > indentLen ) {
          pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
	  outLen = strlen ( pOut ) ;
	  if ( fp )
	    fprintf ( fp, "%s\n", pOut ) ;
	  else if ( pResult ) {
	    pLine = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pLine, pOut ) ;
	    gHyp_data_append ( pResult, pLine ) ;
	  }
	  else 
	    gHyp_util_log ( pOut ) ;
	  *pContentLength += outLen ;
	  *pOutput = '\0' ;
	  if ( doIndent ) strcat ( pOutput, prefixStr ) ;
	  outputLen = strlen ( pOutput ) ;
	}
	
        if ( isJSON && ((gHyp_data_getCount(pParent)==1) || (gHyp_data_getDataType ( pParent ) == TYPE_ATTR)) )
          tmpOutputLen = sprintf ( tmpOutput, "%s", suffixStr ) ;
        else
          tmpOutputLen = sprintf ( tmpOutput, "}%s", suffixStr ) ;

	strcat ( newOutput, tmpOutput ) ;
	newOffset += tmpOutputLen ;
	tmpOutputLen = 0 ;
	tmpOutput[0] = '\0' ;
	if ( lastValue ) {

	  strcat ( pOutput, newOutput ) ;
	  newOffset = 0 ;
	  newOutput[0] = '\0' ;
          pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")) : pOutput ;
	  outLen = strlen ( pOut ) ;
	  if ( fp )
	    fprintf ( fp, "%s\n", pOut ) ;
	  else if ( pResult ) {
	    pLine = gHyp_data_new ( NULL ) ;
	    gHyp_data_setStr ( pLine, pOut ) ;
	    gHyp_data_append ( pResult, pLine ) ;
	  }
	  else 
	    gHyp_util_log ( pOut ) ;
	  *pContentLength += outLen ;
	  *pOutput = '\0' ;
	  if ( doIndent ) strcat ( pOutput, prefixStr ) ;
	  outputLen = strlen ( pOutput ) /*- 2 */;
	  *(pOutput+outputLen) = '\0' ;
	}
      }
    } 
    break ;

  } /* End switch */

  /* For REFERENCE, LITERAL, UNIDENTIFIED, CONSTANT, the variable newOutput contains
   * the element+suffix.  If there is no room to add it to the main pOutput string, 
   * then we flush the main output string and replace its contents with the
   * new element.  Otherwise, the new element is concatenated to the main output
   * string and we return.
   *
   * For VARIABLES, if the element was subscripted, then just that element is
   * taken, and the behaviour is as above for REFERENCE, LITERAL, UNIDENTIFIED, CONSTANT.
   *
   * If the VARIABLE is not subscripted, then....
   */

  /* Return pOutput */  

  if ( ((outputLen + newOffset) > MAX_OUTPUT_LENGTH) ||
       (pResult && ((outputLen + newOffset) > VALUE_SIZE) ) ) {

    pOut = (isPRE || isTXT) ? pOutput+MIN((unsigned int)indent,strspn (pOutput," \t\n")): pOutput ;
    outLen = strlen ( pOut ) ;
    if ( fp )
      fprintf ( fp, "%s\n", pOut ) ;
    else if ( pResult ) {
      pLine = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr ( pLine, pOut ) ;
      gHyp_data_append ( pResult, pLine ) ;
    }
    else
      gHyp_util_log ( pOut ) ;
    *pContentLength += outLen ;
    outputLen = 0 ;
    *pOutput = '\0' ;
  }
  
  strcat ( pOutput, newOutput ) ;
  newOffset = 0 ;
  newOutput[0] = '\0' ;
  return strlen ( pOutput ) ;
} 

void lHyp_fileio_describe ( sInstance *pAI, 
			    sCode *pCode, 
			    sLOGICAL isPARSE,
			    sLOGICAL isFILE,
			    sLOGICAL isXML,
                            sLOGICAL isJSON,
			    sLOGICAL isMSG,
			    sLOGICAL isString ) 
{
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData,
      *pVariable,
      *pResult=NULL ;

    char
      ending,
      output[MAX_BUFFER_SIZE+1],
      tag[TOKEN_SIZE+1] ;

    sLOGICAL
      isPRE=FALSE,
      isTXT=FALSE,
      isSCR=FALSE,
      isAttrOn=FALSE,
      isMRK=FALSE,
      doIndent=FALSE,
      firstValue,
      lastValue ;

    int
      argCount = gHyp_parse_argCount ( pParse ),
      ss,
      indent,
      contentLength;
    
    FILE
      *fp=NULL ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( isFILE ) {

      doIndent = TRUE ;
      if ( argCount != 2 ) {

	if ( isXML )
	  gHyp_instance_error ( 
			       pAI, STATUS_ARGUMENT,
			       "Invalid arguments. Usage: xfdescribe ( variable, handle )" ) ;
	else if ( isJSON )
	  gHyp_instance_error ( 
			       pAI, STATUS_ARGUMENT,
			       "Invalid arguments. Usage: jfdescribe ( variable, handle )" ) ;
	else
	  gHyp_instance_error ( 
			       pAI, STATUS_ARGUMENT,
			       "Invalid arguments. Usage: fdescribe ( variable, handle )" ) ;
      }
	
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

      fp = (FILE*) gHyp_data_getHandle ( pData,
					 gHyp_data_getSubScript(pData),
					 TRUE ) ;
      if ( fp == (FILE*) 0 || fp == (FILE*) 1  )	
	gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;
    }
    else if ( isString ) { 

      doIndent = FALSE ;

      if ( argCount > 2 ) {
	if ( isXML )
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: result = xsdescribe ( variable [,indent] )" ) ;      
	else if ( isJSON )
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: result = jsdescribe ( variable [,indent] )" ) ;      
	else
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: result = sdescribe ( variable [,indent] )" ) ;      
      }

      if ( argCount == 2 ) {
        pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
        doIndent  = gHyp_data_getBool ( pData,gHyp_data_getSubScript ( pData  ), TRUE ) ;
      }
    }
    else {

      doIndent = TRUE ;

      if ( argCount != 1 ) {
	if ( isXML ) 
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: xdescribe ( variable )" ) ;
	else if ( isJSON ) 
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: jdescribe ( variable )" ) ;
	else
	  gHyp_instance_error ( 
            pAI, STATUS_ARGUMENT,
	    "Invalid arguments. Usage: describe ( variable )" ) ;
      }
    }


    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    ss = gHyp_data_getSubScript ( pData ) ;

    if ( (pVariable = gHyp_data_getVariable ( pData ) ) )
      pData = pVariable ;
    
    if ( isString ) pResult = gHyp_data_new ( "_data_" ) ; ;

    /* Do the describe on pData */

    if ( isXML ) {
      strcpy ( tag, gHyp_data_getLabel(pData) ) ;
      gHyp_util_lowerCase ( tag, strlen ( tag ) ) ;
      isPRE = ( strcmp ("pre", tag ) == 0 ) ;
      isTXT = ( strcmp ("textarea", tag ) == 0 ) ;
      isSCR = ( strcmp ("script", tag ) == 0 || strcmp ("style",tag) == 0 ) ;
      isMRK = ( strcmp ("b", tag ) == 0  || 
		strcmp ("big",tag) == 0 ||
		strcmp ("blink",tag) == 0 ||
		strcmp ("i",tag) == 0 ||
		strcmp ("small",tag) == 0 ||
		strcmp ("strike",tag) == 0 ||
		strcmp ("sub",tag) == 0 ||
		strcmp ("sup",tag) == 0 ||
		strcmp ("tt",tag) == 0 ) ;
    }

    indent = 0 ;
    firstValue = TRUE ;
    lastValue = TRUE ;
    output[0] = '\0' ;
    ending = isXML ? ' ' : ';' ;
    contentLength = 0 ;
    lHyp_fileio_describe2 ( pData, 
			    ss, 
			    firstValue,
			    lastValue,
			    indent, 
			    doIndent,
			    output, 
			    fp,
			    isXML,
                            isJSON,
			    isPRE,
			    isTXT,
			    isMSG,
			    isSCR,
			    isMRK,
			    isAttrOn,
			    ending,
			    pResult,
			    &contentLength ) ;
    if ( output[0] ) {
      if ( fp )
        fprintf ( fp, "%s\n", output ) ;
      else if ( pResult ) {
	pData = gHyp_data_new ( NULL ) ;
	gHyp_data_setStr ( pData, output ) ;
	gHyp_data_append ( pResult, pData ) ;
      }
      else if ( !isString )
        gHyp_util_log  ( output ) ;
    }
    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }	
}

sData *gHyp_fileio_describeData (   sInstance *pAI, 
                                    sData *pData, 
                                    char ending, 
                                    sLOGICAL isXML,
                                    sLOGICAL isJSON,
                                    int *pContentLength ) 
{
    sData
      *pContainer,
      *pLine ;

    char
      output[MAX_BUFFER_SIZE+1] ;

    sLOGICAL
      firstValue,
      lastValue ;

    int
      ss,
      indent ;

    /* Set variable name to blank */
    pContainer = gHyp_data_new ( " " ) ;

    indent = 0 ;
    firstValue = TRUE ;
    lastValue = TRUE ;
    ss = gHyp_data_getSubScript ( pData ) ;
    output[0] = '\0' ;
    lHyp_fileio_describe2 ( pData, 
			    ss, 
			    firstValue,
			    lastValue,
			    indent, 
			    FALSE,  /* doIndent */
			    output, 
			    NULL,
			    isXML,  /* isXML */
                            isJSON,/* isJSON */
			    FALSE,  /* isPRE */
			    FALSE,  /* isTXT */
			    TRUE,   /* isMSG */
			    FALSE,  /* isSCR */
			    FALSE,  /* isMRK */
			    FALSE,  /* isAttrOn */
			    ending, /* Ending character */
 			    pContainer,
			    pContentLength ) ;
    if ( output[0] ) {
      pLine = gHyp_data_new ( NULL ) ;
      gHyp_data_setStr ( pLine, output ) ;
      gHyp_data_append ( pContainer, pLine ) ;
    }

    return pContainer ;

}

void gHyp_fileio_sdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: sdescribe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, FALSE, FALSE, FALSE, TRUE ) ;
}

void gHyp_fileio_fdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fdescribe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, TRUE, FALSE, FALSE, FALSE, FALSE ) ;
}

void gHyp_fileio_describe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: describe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, FALSE, FALSE, FALSE, FALSE ) ;
}

void gHyp_fileio_xfdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: xfdescribe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, TRUE, TRUE, FALSE, FALSE, FALSE ) ;
}

void gHyp_fileio_xdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: xdescribe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, TRUE, FALSE, FALSE, FALSE ) ;
}

void gHyp_fileio_xsdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: xdescribe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, TRUE, FALSE, FALSE, TRUE ) ;
}

void gHyp_fileio_jfdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: jfdescribe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, TRUE, FALSE, TRUE, FALSE, FALSE ) ;
}

void gHyp_fileio_jdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: jdescribe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, FALSE, TRUE, FALSE, FALSE ) ;
}

void gHyp_fileio_jsdescribe ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: jsdescribe ( variable )
   *	Returns description of variable.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   * Return value:
   *
   *	none
   *
   */
  lHyp_fileio_describe ( pAI, pCode, isPARSE, FALSE, FALSE, TRUE, FALSE, TRUE ) ;
}


static void lHyp_fileio_put (	sInstance 	*pAI, 
				sCode 		*pCode, 	
				sLOGICAL 	isStr,
				sLOGICAL 	isFile,
				sLOGICAL	isBinary,
				sLOGICAL	isPARSE )
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: put(), puts(), fput(), fputs(), fwrite()
   *	The put function puts spaces between the values, the puts function
   *	appends the values together.
   *
   * Arguments:
   *
   *	pAI							[R]
   *	- pointer to instance object
   *
   *	pCode							[R]
   *	- pointer to code object
   *
   *	isStr							[R]
   *	- puts and fputs
   *
   *	isFile							[R]
   *	- fput and fputs
   *
   *	isBinary						[R]
   *	- use fwrite
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData,
      *pResult ;
    
    char
      *pValue,
      *pMsg,
      *pEndMsg,
      putMessage[MAX_OUTPUT_LENGTH+1],
      quote,
      value[MAX_OUTPUT_LENGTH+1] ;
    
    FILE
      *fp = NULL ; 
    
    sLOGICAL
      hasNewline,
      isFirstValue = TRUE,
      isVector ;

    sBYTE
      tokenType,
      dataType ;
    
    int
      context,
      ss,
      valueLen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( isFile ) {
      if ( argCount != 2 ) {
        if ( isStr ) {
	  if ( isBinary )
  	    gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: fwrite ( data, handle )" ) ;
	  else
	    gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: fputs ( text, handle )" ) ;
        }
	else {
	  if ( isBinary )
	    gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: fwrite ( data, handle )" ) ;
	  else
	    gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: fput ( variable, handle )" ) ;
	}
      }
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      fp = (FILE*) gHyp_data_getHandle ( pData,
					 gHyp_data_getSubScript(pData),
					 TRUE ) ;
      if ( fp == (FILE*) 0 || fp == (FILE*) 1 )	
	gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;
    }
    else {
      if ( argCount != 1 ) {
        if ( isStr )
	  gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: puts ( text )" ) ;
        else	
	  gHyp_instance_error ( pAI, STATUS_ARGUMENT,
    		"Invalid arguments. Usage: put ( variable )" ) ;
      }
    }

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    pMsg = putMessage ;
    pEndMsg = pMsg + MAX_OUTPUT_LENGTH ;
    pResult = NULL ;
    ss = gHyp_data_getSubScript ( pData ) ; 
    context = -1 ;
    quote = '\0'; 
    isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;
    while ( (pResult = gHyp_data_nextValue ( pData, 
					     pResult, 
					     &context,
					     ss ) ) ) {
      if ( isBinary ) {
	valueLen = gHyp_data_getStr ( pResult, 
				     value, 
				     MAX_OUTPUT_LENGTH, 
				     context, 
				     isVector ) ;

        if ( valueLen == -1 ) continue ;	
        pValue = value ;

      }
      else {
        tokenType = gHyp_data_tokenType ( pResult ) ;
        dataType = gHyp_data_dataType ( pResult ) ;

        /* If not printing a string, separate all values with spaces. */
	if ( 	!isStr && 
		!isFirstValue && 
		pMsg < pEndMsg ) { *pMsg++ = ' ' ; *pMsg = '\0' ; }
		
	isFirstValue = FALSE ;

	valueLen = gHyp_data_getStr_nonulls ( pResult, 
				     value, 
				     MAX_OUTPUT_LENGTH, 
				     context, 
				     isVector ) ;

        if ( valueLen == -1 ) continue ;	
        pValue = value ;

        quote = '\0' ;
	if ( !isStr ) {
          if ( tokenType == TOKEN_VARIABLE && !isVector )
	    quote = '\'' ;	/* Deliminate with single quotes */
	  else if ( dataType == TYPE_STRING )
	    quote = '"' ;		/* Deliminate with double quotes */
	}
        if ( quote ) valueLen += 2 ;

      }

      hasNewline = ( value[valueLen-1] == '\n' ) ;

      if ( ( pMsg + valueLen + 3 ) > pEndMsg ) {
      
	/* Cannot fit, output what's there and start a new line */
	if ( isFile ) {
	  if ( isBinary )
	    fwrite ( putMessage, 1, (pMsg-putMessage), fp ) ; 
	  else
	    fprintf ( fp, "%s\n", putMessage ) ;
	}
	else
	  gHyp_util_log ( putMessage ) ;    

	pMsg = putMessage ;
      }

      /* Copy the value */
      if ( isBinary )
	memcpy ( pMsg, pValue, valueLen ) ;
      else {
	if ( quote )
	  sprintf ( pMsg, "%c%s%c", quote, pValue, quote ) ;
	else
	  sprintf ( pMsg, "%s", pValue ) ;
      }

      pMsg += valueLen ;
      *pMsg = '\0' ;

      if ( hasNewline ) {

	/* Ends with newline */
	if ( isFile ) {
	  if ( isBinary )
	    fwrite ( putMessage, 1, (pMsg-putMessage), fp ) ; 
	  else
	    fprintf ( fp, "%s", putMessage ) ;
	}
	else
	  gHyp_util_log ( putMessage ) ;    

	pMsg = putMessage ;
      }

    }	
    if ( context == -2 && ss != -1 )
      gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			    "Subscript '%d' is out of bounds",ss ) ;

    /* Print remainder, if any */
    if ( pMsg > putMessage ) {
      if ( isFile ) {
	if ( isBinary )
	  fwrite ( putMessage, 1, (pMsg-putMessage), fp ) ;
	else
	  fprintf ( fp, "%s\n", putMessage ) ;
      }
      else
        gHyp_util_log ( putMessage ) ;    
    }
    else if ( pMsg == putMessage ) {
      if ( !isBinary ) {
        if ( isFile )
          fprintf ( fp, "\n" ) ;
        else
	  gHyp_util_log ( "" ) ;  
      }
    }

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


void gHyp_fileio_getTokens ( char *pBuf, sData *pResult )
{

  sData
    *pValue ;
  
  char
    *pEndBuf,
    *pValStart,
    *pValEnd ;

  pEndBuf = pBuf + strlen ( pBuf ) ;

  /* Remove unprintables (newline or carriage return) at end of line */
  while ( pEndBuf > pBuf && *(pEndBuf-1) < ' '  ) *(--pEndBuf) = '\0' ;
      
  /* Extract token values and strings. */
  
  pValStart = pBuf ;      
  do {
    
    /* Scan for first printable character */
    pValStart += strspn ( pValStart, " \t" ) ;
    
    if ( *pValStart == '"' ) {
      
      /* Found start of LITERAL */
      pValStart++ ;
      
      /* Look for end of LITERAL */
      pValEnd = strchr ( pValStart, '"' ) ;
      if ( pValEnd == NULL ) 
	pValEnd = pEndBuf ;
      else
	*pValEnd = '\0' ;
      
      /* Create value element */
      pValue = gHyp_data_new ( NULL ) ;
      if ( pValStart < pValEnd )
	gHyp_data_setStr ( pValue, pValStart ) ;
      else
	gHyp_data_setNull ( pValue ) ;
      
    }
    else if ( *pValStart == '\'' ) {
      
      /* Found start of single quoted variable */
      pValStart++ ;
      
      /* Look for end of variable */
      pValEnd = strchr ( pValStart, '\'' ) ;
      if ( pValEnd == NULL ) 
	pValEnd = pEndBuf ;
      else
	*pValEnd = '\0' ;
      
      /* Create value element */
      pValue = gHyp_data_new ( NULL ) ;
      if ( pValStart < pValEnd )
	gHyp_data_setReference ( pValue, pValStart, NULL ) ;
      else
	gHyp_data_setNull ( pValue ) ;
      
    }
    else  {
      
      /* Found a CONSTANT. Find end. */
      pValEnd = pValStart + strcspn ( pValStart, " \t" ) ;
      *pValEnd = '\0' ;
      
      /* CONSTANT OR IDENTIFIER */
      pValue = gHyp_data_new ( NULL ) ;
      if ( pValStart < pValEnd ) {
	gHyp_data_setToken ( pValue, pValStart ) ;
      }
      else
	gHyp_data_setNull ( pValue ) ;
    }
    
    /* Append value to list. */
    gHyp_data_append ( pResult, pValue ) ;
    
    /* Advance past end of value */
    pValStart = pValEnd + 1 ;
  } 
  while ( pValStart < pEndBuf ) ;

  return ;
}

void gHyp_fileio_fopen ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fopen ( file, mode )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   * 
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData,
      *pResult ;
    
    char
      fileSpec[MAX_PATH_SIZE+1],
      mode[VALUE_SIZE+1],
      *pFileSpec,
      *pMode ;
    
    FILE
      *pFILE=NULL ;
    
    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;	

    if ( argCount != 2 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: fopen ( file, mode )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr ( pData, 
			   mode, 
			   VALUE_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pMode = mode ;
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;
        
    if ( !(guRunFlags & RUN_RESTRICTED) ) {

      pFILE = fopen ( pFileSpec, pMode ) ;
      if ( !pFILE ) {
        gHyp_instance_warning ( pAI, STATUS_FILE,
			      "Could not open file '%s'", pFileSpec ) ;
      }
      else
        gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
    }
    else
      gHyp_instance_warning ( pAI, STATUS_RESTRICTED, "The fopen() function is RESTRICTED" ) ;


    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) pFILE ) ;
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_fclose ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fclose ( handle )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */	

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData ;
    
    FILE
      *pFILE ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: fclose ( handle )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    pFILE = (FILE*) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE);
    if ( fclose ( pFILE ) != 0 )
      gHyp_instance_warning ( pAI, STATUS_FILE, "Failed to close file" ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}


void gHyp_fileio_fwrite ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fwrite( data, handle )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */

  lHyp_fileio_put ( pAI, pCode, FALSE, TRUE, TRUE, isPARSE ) ;
}


void gHyp_fileio_fput ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fput ( variable, handle )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */

  lHyp_fileio_put ( pAI, pCode, FALSE, TRUE, FALSE, isPARSE ) ;
}

void gHyp_fileio_fputs ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fputs ( string, handle )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */

  lHyp_fileio_put ( pAI, pCode, TRUE, TRUE, FALSE, isPARSE ) ;
}

void gHyp_fileio_fget ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fget ( handle )
   *	Returns list of values input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pResult ;
    
    char
      *pBuf ;
    
    char
      buf[MAX_INPUT_LENGTH+1] ;
    
    FILE
      *fp ;    

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: fget ( handle )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    fp = (FILE*) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE ) ;

    if ( fp == (FILE*) 0 || fp == (FILE*) 1 )	
      gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;

    pBuf = fgets ( buf, MAX_INPUT_LENGTH, fp ) ;
    if ( !pBuf && !feof(fp)  )
      gHyp_instance_error ( pAI, 
			    STATUS_FILE, 
			    "Failed to read from file" ) ;

    pResult = gHyp_data_new ( "_fget_" ) ;
    if ( pBuf ) {
      gHyp_fileio_getTokens ( pBuf, pResult ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;

    /* Push the result on the stack */
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_fgets ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fgets ( handle )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      buflen,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sData	
      *pData,
      *pResult ;

    char
      *pBuf ;

    char
      buf[MAX_INPUT_LENGTH+1] ;

    FILE
      *fp ;    

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: fgets ( handle )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    fp = (FILE*) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    if ( fp == (FILE*) 0 || fp == (FILE*) 1 )	
      gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;
    pBuf = fgets ( buf, MAX_INPUT_LENGTH, fp ); 
    if ( !pBuf && !feof ( fp ) )
      gHyp_instance_error ( pAI,
			    STATUS_FILE, 
			    "Failed to read from file" ) ;
    
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_fgets_", TYPE_STRING ) ;
    if ( pBuf ) {
      buflen = strlen ( pBuf ) ;
#ifdef AS_PROMIS
      /* Remove unprintables (newline or carriage return) at end of line */
      while ( buflen > 0 && *(pBuf+buflen-1) < ' ' ) { 
	buflen-- ;
	*(pBuf+buflen) = '\0' ;
      }
#endif
      if ( buflen == 0 ) 
	gHyp_data_setNull ( pResult ) ;
      else
	gHyp_data_setText ( pResult, pBuf, buflen ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_fread ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: fread ( handle )
   *	Returns the binary data that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      bufLen,
      width,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sData	
      *pData,
      *pResult ;

    char
      *pBuf ;

    char
      buf[MAX_INPUT_LENGTH+1] ;

    FILE
      *fp ;    

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: fread ( handle )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    fp = (FILE*) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    if ( fp == (FILE*) 0 || fp == (FILE*) 1 )	
      gHyp_instance_error ( 	pAI, STATUS_FILE,
    				"Invalid file handle" ) ;

    bufLen = fread ( buf, 1, MAX_INPUT_LENGTH, fp ); 
    if ( bufLen < 0 && !feof ( fp ) )
      gHyp_instance_error ( pAI,
			    STATUS_FILE, 
			    "Failed to read from file" ) ;
    
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_fread_", TYPE_STRING ) ;

    if ( bufLen == 0 )
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;
    else {

      /* Add the content to the content data area */
      pBuf = buf ;
      while ( bufLen > 0 ) {

	pData = gHyp_data_new ( NULL ) ;

	width = MIN ( bufLen, INTERNAL_VALUE_SIZE ) ;
	gHyp_data_setStr_n ( pData, (char*) pBuf, width ) ;
	gHyp_data_append ( pResult, pData ) ;

	bufLen -= width ;
	pBuf += width ;

      }
    }

    gHyp_stack_push ( pStack, pResult ) ;
  }
}
void gHyp_fileio_get ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: get ( prompt )
   *	Returns list of values input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   *	- version 2.1-0
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    int	   	
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    sData
      *pData,
      *pResult ;

    char
      *pBuf,
      prompt[VALUE_SIZE+1],
      *pPrompt ;

    char
      buf[MAX_INPUT_LENGTH+1] ;
    
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: get ( prompt )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr_nonulls ( pData, 
			    prompt, 
			    VALUE_SIZE, 
			    gHyp_data_getSubScript(pData),
			    TRUE ) ;
    pPrompt = prompt ;
    gHyp_util_output2 ( pPrompt ) ;
    pBuf = fgets ( buf, MAX_INPUT_LENGTH, stdin ); 
    if ( !pBuf && !feof ( stdin ) )
      gHyp_instance_error ( pAI, 
			    STATUS_FILE, 
			    "Failed to read from input stream" ) ;

    pResult = gHyp_data_new ( "_get_" ) ;
    if ( pBuf ) {
      gHyp_fileio_getTokens ( pBuf, pResult ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;
      
    /* Push the result on the stack */
    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_gets ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: gets ( prompt )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      n,
      buflen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pResult ;
    
    char
      *pBuf,
      prompt[VALUE_SIZE+1],
      *pPrompt,
      buf[MAX_INPUT_LENGTH+1] ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: gets ( prompt )" ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr_nonulls ( pData, 
			    prompt, 
			    VALUE_SIZE, 
			    gHyp_data_getSubScript(pData),
			    TRUE ) ;
    pPrompt = prompt ;
    gHyp_util_output2 ( pPrompt ) ;
    pBuf = fgets ( buf, MAX_INPUT_LENGTH, stdin ); 
    if ( !pBuf && !feof ( stdin ) )
        gHyp_instance_error ( pAI,
			      STATUS_FILE,
			      "Failed to read from input stream" ) ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_gets_", TYPE_STRING ) ;
    if ( pBuf ) {
      buflen = strlen ( pBuf ) ;
      /* Remove unprintables (newline or carriage return) at end of line */
      while ( buflen > 0 && *(pBuf+buflen-1) < ' ' ) { 
	buflen-- ;
	*(pBuf+buflen) = '\0' ;
      }
      if ( buflen == 0 ) 
	gHyp_data_setNull ( pResult ) ;
      else
	gHyp_data_setText ( pResult, pBuf, buflen ) ;
    }
    else
      gHyp_instance_setStatus ( pAI, STATUS_EOF ) ;

    gHyp_stack_push ( pStack, pResult ) ;
  }
}

void gHyp_fileio_log ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: log ( file, text )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pFile,
      *pValue,
      *pData=NULL ;

    char
      *pStr,
      *pFileSpec=NULL,
      *pMsg,
      *pEndMsg,
      fileSpec[MAX_PATH_SIZE+1],
      logMessage[MAX_LOG_LENGTH+1],
      value[VALUE_SIZE+1] ;
    
    sBYTE
      lastTokenType,
      nextTokenType ;
    
    sLOGICAL
      isVector ;
    
    FILE
      *pFILElog = NULL ;
    
    int 
      n,
      context,
      ss,
      valueLen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    if ( argCount > 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: log ( [file [,text]] )" ) ;

    if ( argCount == 2 )
      pData = gHyp_stack_popRdata ( pStack, pAI ) ;

    if ( argCount > 0 ) {
      pFile = gHyp_stack_popRvalue ( pStack, pAI ) ; 
      n = gHyp_data_getStr ( pFile, 
			     fileSpec, 
			     MAX_PATH_SIZE,
			     gHyp_data_getSubScript(pFile),
			     TRUE) ;
      pFileSpec = fileSpec ;

      if ( !(guRunFlags & RUN_RESTRICTED) ) 
      
        pFILElog = fopen ( pFileSpec, "a+"
#ifdef AS_VMS
			 , "shr=get,put"
#endif
			 ) ;
    
    }

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount == 0 ) {

      gsLog = stdout ;
      gzLogName[0] = '\0' ;
    }
    else if ( !pFILElog ) {
    
      if ( (guRunFlags & RUN_RESTRICTED) ) 
        gHyp_instance_warning ( pAI, STATUS_RESTRICTED,"The log() function is RESTRICTED" ) ;
      else
        gHyp_instance_warning ( pAI, STATUS_FILE, 
			      "Could not open file '%s'", pFileSpec ) ;
    }
    else if ( argCount == 1 ) {
    
      if ( gzLogName[0] ) {
	/* Close previous one */
	fclose ( gsLog ) ;
      }
      /* Set new log file */
      gsLog = pFILElog ; 
      strcpy ( gzLogName, pFileSpec ) ; 

    }
    else {

      pMsg = logMessage ;
      pEndMsg = pMsg + MAX_LOG_LENGTH ;
      gsCurTime = time ( NULL ) ;
      sprintf (	pMsg, "%s:", gHyp_util_timeStamp ( gsCurTime ) ) ;
      pMsg += (DATETIME_SIZE+1) ;

      isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING );
      lastTokenType = TOKEN_NULL ;
      pValue = NULL ;
      ss = gHyp_data_getSubScript ( pData ) ; 
      context = -1 ;
      while ( (pValue = gHyp_data_nextValue ( pData, 
					       pValue, 
					       &context,
					       ss ) ) ) {
	
	/* Put spaces between tokens that are not literals. */
	nextTokenType = gHyp_data_tokenType ( pValue ) ;
	if (	lastTokenType   != TOKEN_NULL &&
		( lastTokenType != TOKEN_LITERAL || 
		  nextTokenType != TOKEN_LITERAL) &&
		pMsg < pEndMsg ) *pMsg++ = ' ' ;
	  
	valueLen = gHyp_data_getStr_nonulls ( pValue, 
				       value, 
				       VALUE_SIZE,
				       context,
				       isVector ) ;
	pStr = value ;
	
        if ( (pMsg + valueLen) > pEndMsg ) {
	
	  /* Cannot fit, wrap onto next line */
	  int truncLen = pEndMsg - pMsg ;
	  strncpy ( pMsg, pStr, truncLen ) ;
	  pMsg += truncLen ;
	  *pMsg = '\0' ;
	  pStr += truncLen ;
	  valueLen -= truncLen ;
          fprintf ( pFILElog, "%s\n", logMessage ) ;
	  pMsg = logMessage ;
	}
	  
	/* Copy the value (or what's left after truncation) */
	strncpy ( pMsg, pStr, valueLen ) ;
        pMsg += valueLen ;
	
	/* Remember last token type */
	lastTokenType = nextTokenType ;

	/* Cap it  */
	*pMsg = '\0' ;
	
      }
      if ( context == -2 && ss != -1 )
	gHyp_instance_error ( pAI, STATUS_BOUNDS, 
			      "Subscript '%d' is out of bounds", ss ) ;

      /* Print remainder, if any */
      if ( pMsg > logMessage+DATETIME_SIZE+1 ) 
	fprintf ( pFILElog, "%s\n", logMessage ) ;
      
      fclose ( pFILElog ) ;
    }
    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_fileio_put ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: put ( variable )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */
  lHyp_fileio_put ( pAI, pCode, FALSE, FALSE, FALSE, isPARSE ) ;
}


void gHyp_fileio_puts ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: puts ( string )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   */
  lHyp_fileio_put ( pAI, pCode, TRUE, FALSE, FALSE, isPARSE ) ;
}

void gHyp_fileio_unlink ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: unlink ( file )
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   *	
   *
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    sData
      *pData ;
    
    char
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1] ;
    
    int
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;

    if ( argCount != 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: unlink ( filespec )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;
    if ( !(guRunFlags & RUN_RESTRICTED) ) {

#ifdef AS_WINDOWS
      if ( _unlink ( pFileSpec ) == -1 ) {
#else
      if ( unlink ( pFileSpec ) == -1 ) {
#endif
        gHyp_instance_warning ( pAI, STATUS_FILE, "Failed to unlink file %s",pFileSpec ) ;
      }
    }
    else
      gHyp_instance_warning ( pAI, STATUS_RESTRICTED, "The fopen() function is RESTRICTED" ) ;

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_fileio_echo ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: echo ( "text" )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      n,
      buflen,
      eosIndex,
      hypIndex,
      hypCount,
      tokenType,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pValue,
      *pData,
      *pResult ;
    
    sHyp
      *pHyp ;

    sLOGICAL
      isEOF = FALSE,
      isEOT = FALSE ;

    char
      *pBuf,
      endOfText[VALUE_SIZE+1],
      *pEOT,
      buf[MAX_INPUT_LENGTH+1] ;

    if ( argCount > 1 ) gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: echo ( endOfText )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
    n = gHyp_data_getStr_nonulls ( pData, 
			    endOfText, 
			    VALUE_SIZE, 
			    gHyp_data_getSubScript(pData),
			    TRUE ) ;
    pEOT = endOfText ;

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_echo_", TYPE_STRING ) ;
    
    pHyp = gHyp_frame_getHyp ( pFrame ) ;
    hypCount = gHyp_hyp_getHypCount ( pHyp ) ;
    eosIndex = hypIndex = gHyp_frame_getHypIndex ( pFrame ) ;
    pBuf = buf ;
 
    if ( hypCount == eosIndex ) {

      while ( !isEOT && !isEOF && pBuf ) {

        pBuf = fgets ( buf, MAX_INPUT_LENGTH, gsPP );
        isEOF = feof ( gsPP ) ;      
        if ( !pBuf && !isEOF )
          gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
	
        if ( pBuf ) {
	  buflen = strlen ( pBuf ) ;
	  isEOT = ( strncmp ( pBuf, pEOT, n ) == 0 ) ;
	  if ( !isEOT ) {
	    pValue = gHyp_data_new ( NULL ) ;
	    if ( buflen == 0 ) 
	      gHyp_data_setNull ( pValue ) ;
	    else {
              /*
	      gHyp_util_trim ( pBuf ) ;
	      buflen = strlen ( pBuf ) ;
	      */
              buflen = gHyp_util_parseString ( pBuf ) ;
	      gHyp_data_setStr_n ( pValue, pBuf, buflen ) ;
	    }
	    gHyp_data_append ( pResult, pValue ) ;
	  }
	}
      }
    }
    else {

      /* Probably in dereference or in a method */
      while ( !isEOT && eosIndex < hypCount && pBuf ) {
	        
	pCode = gHyp_hyp_code ( pHyp, eosIndex ) ;
	tokenType = gHyp_hyp_tokenType ( pCode ) ;
	pBuf = gHyp_hyp_token ( pCode ) ;
	/*gHyp_util_debug("TOKEN %s",pBuf);*/
	buflen = strlen ( pBuf ) ;
	isEOT = ( strncmp ( pBuf, pEOT, n ) == 0 ) ;
	if ( !isEOT ) {
	  pValue = gHyp_data_new ( NULL ) ;
	  if ( buflen > 0 ) {
	    if ( tokenType == TOKEN_LITERAL ) {
	      buflen = sprintf ( buf, "\"%s\"", pBuf ) ;
	      pBuf = buf ; ;
	    }
	    else if ( tokenType == TOKEN_UNIDENTIFIED ) {
	      buflen = sprintf ( buf, "'%s'", pBuf ) ;
	      pBuf = buf ;
  	    }
	    gHyp_data_setStr_n ( pValue, pBuf, buflen ) ;
	    gHyp_data_append ( pResult, pValue ) ;
	  }
	}
	eosIndex++ ;
      }
      if ( eosIndex > hypIndex ) {
	 gHyp_frame_setHypIndex ( pFrame, eosIndex - 1 ) ;
      }
    }
    gHyp_stack_push ( pStack, pResult ) ;
  }
}


static sData* lHyp_fileio_parseCSV( sData* pData, 
                                    char *elementTag, 
                                    int maxLines, 
                                    sFrame* pFrame, 
                                    FILE *pFILE, 
                                    sInstance*  pAI  ) 
{
  /* Description:
   *
   *	Parse CSV ata from a file or from sData
   *
   * Arguments:
   *
   * 
   *
   * Return value:
   *
   *	none
   */

  sData
    *pResult,
    *pTag,
    *pElement,
    *pHeader[MAX_SQL_ITEMS],
    *pLabel,
    *pValue=NULL ;

  int
    context,
    ss,
    headerCount=0,
    itemCount,
    line,
    tokLen,
    n,
    buflen ;
  
  sLOGICAL
      isEOF = FALSE ;

  char
      deliminator,
      *delim = ",",
      *pBuf,
      *pTok,
      *pLastTok,
      buf[MAX_INPUT_LENGTH+1] ;

  pResult = gHyp_data_new ( NULL ) ;
  gHyp_data_setVariable ( pResult, "_csv_", TYPE_STRING ) ;
  
  if ( pData ) {
    /* From a data source */
    pValue = NULL ;
    context = 0 ;
    ss = -1 ;
    pValue = gHyp_data_nextValue ( pData, 
				   pValue, 
				   &context,
				   ss ) ;
    if ( !pValue ) return pResult ;
    n = gHyp_data_getStr ( pValue, 
			   buf, 
			   MAX_INPUT_LENGTH,
			   context,
			   TRUE ) ; 
    pBuf = buf ;
  }
  else {

    /* From a file:  Read header line first */
    line = 0 ;
    pBuf = buf ;
    pBuf = fgets ( buf, VALUE_SIZE, pFILE );
    isEOF = feof ( pFILE ) ;      
    if ( !pBuf && !isEOF ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI,
			    STATUS_FILE,
			    "Failed to read from input stream" ) ;
    }
  }

  if ( pBuf ) {

    gHyp_util_trim ( pBuf ) ;
    buflen = strlen ( pBuf ) ;

    /* Parse out the header fields:
     *
     * VID,Real-Time ID,Format,Units,Description,Limit CEID,Min,Max,Enumerated Type Description
     * 22,,U1,"","Specify Abort Level",,,,
     * 23,,U4,,"ALID of alarm causing latest transition",,,,
     * 24,,L,"","List of Enabled alarms",,,,
     * 25,,L,"","List of which alarms are ON",,,,
     *
     * creating a structure like:
     *
     *    csvData = load_csv "csv.dat" ;
     *    describe csvData ;
     *
     *    list csvData = {
     *      list element = {
     *        list 'VID' = 22 
     *        list 'Real-Time ID' = {},
     *        list 'Format' = "U1",
     *        list 'Units' = "",
     *        list 'Description' = "Specify Abort Level",
     *        list 'Limit CEID' = {},
     *        list 'Min' = {},
     *        lust 'Max' = {},
     *        list 'Enumerated Type Description' = {}
     *      },
     *      list element = {
     *        list 'VID' = 32 
     *        list 'Real-Time ID' = {},
     *      
     */
    headerCount = 0 ;

    /* Obtain the first token/field (name) */
    pTok = strtok (buf, delim);
     
    if ( pTok == NULL || *pTok == '\0' ) {
      gHyp_data_delete ( pResult ) ;
      gHyp_instance_error ( pAI,
			    STATUS_FILE,
			    "Invalid header field in CSV file" ) ;
    }          
       
    /* Set each header field */
    while ( pTok ) {

      /* Save header name */
      pElement  = gHyp_data_new ( pTok ) ;
      pHeader[headerCount++] = pElement ;
 
      /* Get next header name */
      pLastTok = pTok ;
      tokLen = strlen ( pLastTok ) ;

      /*gHyp_util_debug("%x %x",(pLastTok + tokLen),(pBuf + buflen)) ; */
      if ( (pLastTok + tokLen) == (pBuf + buflen ) ) break ;

      pTok = strtok(NULL, delim);
 
      /* It shouldn't be a null or empty record. */
      if ( pTok == NULL || *pTok == '\0'  ) {
        while ( headerCount-- ) gHyp_data_delete ( pHeader[headerCount] ) ;
        gHyp_data_delete ( pResult ) ;
        gHyp_instance_error ( pAI,
			      STATUS_FILE,
			      "Empty header field in CVS header row" ) ;
      }

      /* Two comma's in a row - a,,b - is an error! */
      if ( pTok != ( pLastTok + tokLen + 1 ) ) {
        while ( headerCount-- ) gHyp_data_delete ( pHeader[headerCount] ) ;
        gHyp_data_delete ( pResult ) ;
        gHyp_instance_error ( pAI,
			      STATUS_FILE,
			      "Missing header field in CVS header row" ) ;
      }
    }
  } 
    
  while ( !isEOF && pBuf ) {

    if ( pData ) {
      /* From a data source */
      pValue = gHyp_data_nextValue (  pData, 
				      pValue, 
				      &context,
				      ss ) ;
      if ( !pValue ) {
        while ( headerCount-- ) gHyp_data_delete ( pHeader[headerCount] ) ;
        return pResult ;
      }
      n = gHyp_data_getStr (  pValue, 
  			      buf, 
			      MAX_INPUT_LENGTH,
			      context,
			      TRUE ) ; 
      pBuf = buf ;
    }
    else {
      pBuf = fgets ( buf, VALUE_SIZE, pFILE );
      isEOF = feof ( pFILE ) ;      
      if ( !pBuf && !isEOF ) {
	gHyp_data_delete ( pResult ) ;
        gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
      }
    }

    if ( pBuf ) {

      /* Get headerCount elements from each line.
       * If one is missing, then its an empty value
       */
      gHyp_util_trim ( pBuf ) ;
      buflen = strlen ( pBuf ) ;

      pTag = gHyp_data_new ( elementTag ) ;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           /* Obtain the first value */
      pTok = strtok (buf, delim);
        
      /* Set each header field */
      itemCount = 0 ;
      while ( itemCount < headerCount ) {
            
        pLabel = gHyp_data_new ( gHyp_data_getLabel ( pHeader[itemCount] ) ) ;
        if ( pTok && *pTok ) {

          /* Not an empty value - parse the token into its natural format,
           * unless it is a literal enclosed in single or double quotes
           */
          tokLen = strlen ( pTok ) ;
          deliminator = *pTok ;
          if ( ( deliminator == '\'' || deliminator == '"' ) && 
                 *(pTok+tokLen-1) == deliminator ) {
            /* Token is deliminated, remove */
            tokLen = tokLen - 2 ;
            pTok++ ;
            *(pTok+tokLen) = '\0' ;
            pElement = gHyp_data_new ( NULL ) ;
            gHyp_data_setStr_n ( pElement, pTok, tokLen ) ;
          }
          else {
            pElement = gHyp_data_new ( NULL ) ;
            gHyp_data_setToken ( pElement, pTok ) ;
          }
          gHyp_data_append ( pLabel, pElement ) ;
          gHyp_data_append ( pTag, pLabel ) ;

          pLastTok = pTok ;
          tokLen = strlen ( pLastTok ) ;
                
          /* Get next token value */
          pTok = strtok(NULL, delim);

          if ( pTok ) {

           /* See what we skipped */

            if ( (pLastTok + tokLen) == (pBuf + buflen ) ) {
              /* End of buffer - complete the values and break loop */
              while ( itemCount < headerCount-1 ) {
                itemCount++ ;
                pLabel = gHyp_data_new ( gHyp_data_getLabel ( pHeader[itemCount] ) ) ;
                gHyp_data_append ( pTag, pLabel ) ;
              }
              break ;
            }

            /* How many skipped items? */
            n = ( pTok - ( pLastTok + tokLen + 1 ) ) ; 
            while ( n-- && itemCount < headerCount-1 ) {
              itemCount++ ;
              pLabel = gHyp_data_new ( gHyp_data_getLabel ( pHeader[itemCount] ) ) ;
              gHyp_data_append ( pTag, pLabel ) ;
            }
          }

        }
        else {
          /* Empty value */
          gHyp_data_append ( pTag, pLabel ) ;
        }

        /* Next value */
        itemCount++ ;

      }
     
      gHyp_data_append ( pResult, pTag ) ;
      line++ ;
      if ( maxLines != -1 && line >= maxLines ) break ;
    }
  }
 
  if ( pFILE ) fclose ( pFILE ) ;

  while ( headerCount-- ) gHyp_data_delete ( pHeader[headerCount] ) ;

  return pResult ;
}

void gHyp_fileio_load_csv( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: load_csv ( fileSpec, tagName, maxLines )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      maxLines,
      n,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pResult ;

    char
      elementTag[VALUE_SIZE+1],
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1] ;
    FILE
      *pFILE ;

    if ( argCount != 1 && argCount != 2 && argCount != 3) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: load_csv ( fileSpec [, elementTag, [,maxlines]] )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    strcpy ( elementTag, "element" ) ;
    maxLines = -1 ;
      
    if ( argCount == 3 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      maxLines = gHyp_data_getInt ( pData, gHyp_data_getSubScript (pData), TRUE ) ;
    }

    if ( argCount >= 2 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData, 
			     elementTag, 
			     VALUE_SIZE,
			     gHyp_data_getSubScript(pData),
			     TRUE ) ;    
    }
    
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;

    pFILE = fopen ( pFileSpec, "r" ) ;
    if ( !pFILE ) {
      gHyp_instance_warning ( pAI, STATUS_FILE,
			      "Could not open file '%s'", pFileSpec ) ;
      pResult = NULL ;
    }
    else {
      pResult = lHyp_fileio_parseCSV ( NULL, elementTag, maxLines, pFrame, pFILE, pAI ) ;
    }

    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_fileio_csv ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{    

  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: csv ( csvData, tagName )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */
  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    char
      elementTag[VALUE_SIZE+1];

    int
      n,
      maxLines,
      argCount = gHyp_parse_argCount ( pParse ) ;

   sData
      *pResult,
      *pData ;

    if ( argCount == 2 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData, 
			     elementTag, 
			     VALUE_SIZE,
			     gHyp_data_getSubScript(pData),
			     TRUE ) ;    
    }
    else
      strcpy ( elementTag, "element" ) ;

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;

    maxLines = gHyp_data_getCount ( pData ) ;
    pResult = lHyp_fileio_parseCSV ( pData, elementTag, maxLines, pFrame, NULL, pAI ) ;
  
    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }  
}

void gHyp_fileio_load( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: load ( fileSpec )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      maxLines,
      line,
      n,
      buflen,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pValue,
      *pData,
      *pResult ;

    sLOGICAL
      isEOF = FALSE ;

    char
      *pBuf,
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1],
      buf[MAX_INPUT_LENGTH+1] ;

    FILE
      *pFILE ;

    if ( argCount != 1 && argCount != 2) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: load ( fileSpec [,maxlines] )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount == 2 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      maxLines = gHyp_data_getInt ( pData, gHyp_data_getSubScript (pData), TRUE ) ;
    }
    else
      maxLines = -1 ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;

    pFILE = fopen ( pFileSpec, "r" ) ;
    if ( !pFILE ) {
      gHyp_instance_warning ( pAI, STATUS_FILE,
			      "Could not open file '%s'", pFileSpec ) ;
      pResult = NULL ;
    }
    else {

      pResult = gHyp_data_new ( NULL ) ;
      gHyp_data_setVariable ( pResult, "_load_", TYPE_STRING ) ;
    
      line = 0 ;
      pBuf = buf ;
      while ( !isEOF && pBuf ) {

	pBuf = fgets ( buf, VALUE_SIZE, pFILE );
        isEOF = feof ( pFILE ) ;      
        if ( !pBuf && !isEOF ) {
	  gHyp_data_delete ( pResult ) ;
          gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
	}
        if ( pBuf ) {
	  buflen = strlen ( pBuf ) ;
	  pValue = gHyp_data_new ( NULL ) ;
	  if ( buflen == 0 ) 
	    gHyp_data_setNull ( pValue ) ;
	  else {
	    /*gHyp_util_trim ( pBuf ) ;*/
	    gHyp_data_setStr ( pValue, pBuf ) ;
	  }
	  gHyp_data_append ( pResult, pValue ) ;
	  line++ ;
	  if ( maxLines != -1 && line >= maxLines ) break ;
	}
      }
 
      fclose ( pFILE ) ;
    }

    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }
}

void gHyp_fileio_load_binary( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: load_binary ( fileSpec )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      n,
      buflen,
      fileSize,
      filePtr,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pValue,
      *pData,
      *pResult ;

    char
      *pBuf,
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1],
      buf[MAX_INPUT_LENGTH+1] ;

    FILE
      *pFILE ;


  struct stat
    stats ;

  if ( argCount != 1 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
    	"Invalid arguments. Usage: load_binary ( fileSpec  )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
    n = gHyp_data_getStr ( pData, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
    pFileSpec = fileSpec ;

    pFILE = fopen ( pFileSpec, "rb" ) ;
    if ( !pFILE ) {
      gHyp_instance_warning ( pAI, STATUS_FILE,
			      "Could not open file '%s'", pFileSpec ) ;
      pResult = NULL ;
    }
    else {

      if ( stat ( pFileSpec, &stats ) < 0 )
	gHyp_instance_error ( pAI, STATUS_FILE,
			      "Could not stat file '%s'", pFileSpec ) ;
      fileSize = stats.st_size ;
      filePtr = 0 ;

      pResult = gHyp_data_new ( NULL ) ;
      gHyp_data_setVariable ( pResult, "_load_binary_", TYPE_STRING ) ;
    
      pBuf = buf ;
      while ( 1 ) {

        buflen = fread ( buf, 1, INTERNAL_VALUE_SIZE, pFILE );
        if ( buflen < 0 ) {
	  gHyp_data_delete ( pResult ) ;
          gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
	}
	else if ( buflen == 0 || filePtr >= fileSize ) {
	  break ;
	}
        else  {

	  if ( filePtr+buflen > fileSize ) {
            /*gHyp_util_debug ( "Would exceed file size!!!" ) ;*/
	    buflen = fileSize - filePtr ;
	  }
	  filePtr += buflen ;

	  pValue = gHyp_data_new ( NULL ) ;
	  gHyp_data_setStr_n ( pValue, pBuf, buflen ) ;
	  gHyp_data_append ( pResult, pValue ) ;
	}
      }
 
      fclose ( pFILE ) ;
    }

    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;
  }

}



void gHyp_fileio_checksum ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: checksum ( fileSpec [,isText] )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      ss,
      context,
      n,
      fileSize=0,
      filePtr=0,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pValue,
      *pResult ;

    char
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1] ;

    FILE
      *pFILE ;

    sLOGICAL
      isVector=FALSE,
      isFile = FALSE,
      isText = FALSE ;

    struct stat
      stats ;

    unsigned long 
      crc = 0;
    long 
      length = 0;
    long 
      high_bits = 0;
    long 
      controls = 0;
    long 
      bytes_read;
    int 
      insep= 0 ;		/* Are we in a separator sequence? */

    unsigned char 
      buf[CRC_BUFLEN],
      c,
      *cp;

    if ( argCount != 1 && argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid arguments. Usage: checksum ( [fileSpec|value] [, isText]  )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
  
    if ( argCount == 2 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
      isText = gHyp_data_getBool ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    }

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    ss = gHyp_data_getSubScript ( pData ) ; 
    context=-1;
    isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;

    /* See if the value is the name of a file */
    pValue = gHyp_data_getValue ( pData, ss, TRUE ) ;
    n = gHyp_data_getStr ( pValue, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   ss,
			   TRUE ) ;
    pFileSpec = fileSpec ;

    if ( isText ) 
      pFILE = fopen ( pFileSpec, "rb" ) ;
    else
      pFILE = fopen ( pFileSpec, "r" ) ;

    if ( !pFILE ) {

      /* Not a file.  */
      isFile = FALSE ;
      pValue = NULL ;
   }
    else {

      isFile = TRUE ;
      if ( stat ( pFileSpec, &stats ) < 0 )
	gHyp_instance_error ( pAI, STATUS_FILE,
			      "Could not stat file '%s'", pFileSpec ) ;
      fileSize = stats.st_size ;
      filePtr = 0 ;
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_checksum_", TYPE_STRING ) ;
      
    if (isText) {

      /* Process implied leading space character */
      length = 1;
      insep = 1;
      crc = (crc << 8) ^ gsCrctab[((crc >> 24) ^ (unsigned char) 32) & 0xFF];
    }

    cp = buf ;

    while ( 1 ) {

      if ( isFile ) {

        bytes_read = fread ( buf, 1, CRC_BUFLEN, pFILE );
        if ( bytes_read < 0 ) {
	  gHyp_data_delete ( pResult ) ;
          gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
	}
	else if ( bytes_read == 0 || filePtr >= fileSize ) {
	  break ;
	}
        else  {

	  if ( filePtr+bytes_read > fileSize ) {
            /*gHyp_util_debug ( "Would exceed file size!!!" ) ;*/
	    bytes_read = fileSize - filePtr ;
	  }
	  filePtr += bytes_read ;
	}
      }
      else {

	pValue = gHyp_data_nextValue ( pData, 
				       pValue, 
				       &context,
					ss ) ;
	if ( !pValue ) break ;

	bytes_read = gHyp_data_getStr ( pValue, 
				     (char*) buf, 
				     CRC_BUFLEN, 
				     context, 
				     isVector ) ;

      }

      cp = buf ;

      if ( isText ) {

  	while (bytes_read--) {

	  c = *(cp++);
	  if (c >= 128) {
	    high_bits++;
	  } 
	  else if (c < ' ') {
	    switch (c) {
	      case '\t':
	      case '\n':
	      case '\r':
    	      case '\f':
	        break;
	      default:
	        controls++;
	    }
	  }
	  if (c > ' ') {
	    crc = (crc << 8) ^ gsCrctab[((crc >> 24) ^ c) & 0xFF];
	    length++;
	    insep = 0;
	  } 
	  else if (!insep) {
	    crc = (crc << 8) ^ gsCrctab[((crc >> 24) ^ (unsigned char) 32) & 0xFF];
	    length++;
	    insep = 1;
	  }
	}
      } 
      else {	/* Binary file */

	length += bytes_read;
	while (bytes_read--) {
	  c = *(cp++);
	  if (c >= 128) {
	    high_bits++;
	  } 
	  else if (c < ' ') {
	    switch (c) {
	      case '\t':
	      case '\n':
	      case '\r':
	      case '\f':
		break;
	      default:
		controls++;
	    }
	  }
	  crc = (crc << 8) ^ gsCrctab[((crc >> 24) ^ c) & 0xFF];
	} 
      }

    } /* End while */

    if ( isFile  ) fclose ( pFILE ) ;

    if ( isText ) {

      /* Add implied trailing space character, if needed */
      if (!insep) {
	crc = (crc << 8) ^ gsCrctab[((crc >> 24) ^ (unsigned char) 32) & 0xFF];
	length++;
      }
    }

    bytes_read = length;
    while (bytes_read > 0) {
      crc = (crc << 8) ^ gsCrctab[((crc >> 24) ^ bytes_read) & 0xFF];
      bytes_read >>= 8;
    }

    crc = ~crc & 0xFFFFFFFF;
    /*gHyp_util_debug("CRC-Checksum: %lu %ld", crc, length);*/
      
    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_newConstant_raw ( pData, TYPE_HEX, &crc ) ;
    gHyp_data_append ( pResult, pData ) ;

    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setInt ( pData, length ) ;
    gHyp_data_append ( pResult, pData ) ;

    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }

}

static unsigned long lHyp_fileio_calcCRC32(
    unsigned char *p, 
    unsigned long reclen, 
    unsigned long checksumpos, 
    unsigned long checksumlen)
{
	unsigned long j;

	/* initialize value */
	unsigned long crc = CRC32_XINIT;

	/* process each byte prior to checksum field */
	for (j = 1; j < checksumpos; j++) {
		crc = gsCrctable32[(crc ^ *p++) & 0xFFL] ^ (crc >> 8);
	}

	/* skip checksum position */
	j += checksumlen;
	p += checksumlen;

	/* process remaining bytes in record */
	while (j <= reclen) {
		crc = gsCrctable32[(crc ^ *p++) & 0xFFL] ^ (crc >> 8);
		j++;
	}

	/* return XOR out value */
	return crc ^ CRC32_XOROT;
}

void gHyp_fileio_crc32 ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE ) 
{
  /* Description:
   *
   *	PARSE or EXECUTE the built-in function: crc32 ( fileSpec [,isText] )
   *	Returns the string that was input.
   *
   * Arguments:
   *
   *	pAI				[R]
   *	- pointer to Instance object
   *
   *	pCode				[R]
   *	- pointer to Code object
   *
   *	isPARSE				[R]
   *	- STATE_PARSE if TRUE, STATE_EXECUTE if FALSE
   * 
   *
   * Return value:
   *
   *	none
   */

  sFrame	*pFrame = gHyp_instance_frame ( pAI ) ;
  sParse	*pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {
  
    sStack 	
      *pStack = gHyp_frame_stack ( pFrame ) ;
    
    int
      ss,
      context,
      bytes_read,
      n,
      fileSize=0,
      filePtr=0,
      argCount = gHyp_parse_argCount ( pParse ) ;
    
    sData
      *pData,
      *pValue,
      *pResult ;

    char
      *pFileSpec,
      fileSpec[MAX_PATH_SIZE+1] ;

    FILE
      *pFILE ;

    sLOGICAL
      isVector=FALSE,
      isFile = FALSE,
      isText = FALSE ;

    struct stat
      stats ;

    unsigned long 
      crc = 0 ;

    char 
      buf[CRC_BUFLEN],
      *pStream = buf,
      *pAnchor = pStream,
      *pEndOfStream = pStream ;

    if ( argCount != 1 && argCount != 2 ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
      "Invalid arguments. Usage: crc32 ( [fileSpec|value] [, isText]  )" ) ;

    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;
  
    if ( argCount == 2 ) {
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
      isText = gHyp_data_getBool ( pData, gHyp_data_getSubScript(pData), TRUE ) ;
    }

    pData = gHyp_stack_popRdata ( pStack, pAI ) ;
    ss = gHyp_data_getSubScript ( pData ) ; 
    context=-1;
    isVector = ( gHyp_data_getDataType ( pData ) > TYPE_STRING ) ;

    /* See if the value is the name of a file */
    pValue = gHyp_data_getValue ( pData, ss, TRUE ) ;
    n = gHyp_data_getStr ( pValue, 
			   fileSpec, 
			   MAX_PATH_SIZE,
			   ss,
			   TRUE ) ;
    pFileSpec = fileSpec ;

    if ( isText ) 
      pFILE = fopen ( pFileSpec, "rb" ) ;
    else
      pFILE = fopen ( pFileSpec, "r" ) ;

    if ( !pFILE ) {

      /* Not a file.  */
      isFile = FALSE ;
      pValue = NULL ;
   }
    else {

      isFile = TRUE ;
      if ( stat ( pFileSpec, &stats ) < 0 )
	gHyp_instance_error ( pAI, STATUS_FILE,
			      "Could not stat file '%s'", pFileSpec ) ;
      fileSize = stats.st_size ;
      filePtr = 0 ;
    }

    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setVariable ( pResult, "_crc32_", TYPE_STRING ) ;
      
    pStream = buf ;

    if ( isFile ) {

      bytes_read = fread ( buf, 1, CRC_BUFLEN, pFILE );
      if ( bytes_read < 0 ) {
	gHyp_data_delete ( pResult ) ;
        gHyp_instance_error ( pAI,
			        STATUS_FILE,
			        "Failed to read from input stream" ) ;
      }
      else  {

	if ( filePtr+bytes_read > fileSize ) {
          /*gHyp_util_debug ( "Would exceed file size!!!" ) ;*/
	  bytes_read = fileSize - filePtr ;
	}
	filePtr += bytes_read ;
      }
    }
    else {

      pStream = gHyp_util_readStream (	pStream, pAnchor, &pEndOfStream,
					&bytes_read, pData, 
					&pValue, &context, ss, isVector, 
					NULL ) ;

    }

    pStream = buf ;
    crc = lHyp_fileio_calcCRC32( (unsigned char*) pStream, bytes_read, bytes_read, 0 ) ;

    if ( isFile  ) fclose ( pFILE ) ;

    /*
	s = p+checksumpos-1;
	while (checksumlen--) {
		*(s+checksumlen) = *(HexString + (CRCValue & Mask));
		CRCValue >>= 4;	 
	}
    */

    /*gHyp_util_debug("CRC32: %lu %ld", crc, length);*/
      
    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_newConstant_raw ( pData, TYPE_HEX, &crc ) ;
    gHyp_data_append ( pResult, pData ) ;

    pData = gHyp_data_new ( NULL ) ;
    gHyp_data_setInt ( pData, bytes_read ) ;
    gHyp_data_append ( pResult, pData ) ;

    if ( pResult )
      gHyp_stack_push ( pStack, pResult ) ;
    else
      gHyp_instance_pushSTATUS ( pAI, pStack ) ;

  }

}
