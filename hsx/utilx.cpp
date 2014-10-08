// util.cpp : Implementation of utility functions

/*
 * $Id$
 *
 * Description	: Utility functions for the "WebPickle" control
 * Author		: John Bergsma <johnbergsma@hotmail.com>
 * 
 ******************************************************************************!
 !                HyperScript Software Source Code.                            !
 !                                                                             !
 !          ***** Copyright: (c) 2004 Ab Initio Software                       !
 !          ***** All rights reserved - Licensed Material.                     !
 !          ***** Program property of Ab Initio Software                       !
 !                                                                             !
 !******************************************************************************
 * 
 * Purpose		:
 * Provide some commonly used tools.
 *
 * $Log$
 * Revision 1.3  2008-06-06 00:48:36  bergsma
 * no message
 *
 * Revision 1.2  2005-01-18 19:58:22  jbergsma
 * Added functions to insert backslash characters before quote characters
 *
 * Revision 1.1  2004/12/17 18:16:21  jbergsma
 * Utiility functions for the WebPickle
 *
 *
 */

static const char *cvsid = "$Header$";

#include "stdafx.h"
#include "utilx.h"

#include <atlconv.h>

/////////////////////////////////////////////////////////////////////////////
// Utility functions
//

// Convert a wide-character string to a single-byte C style string of length len

size_t ConvertBSTRtoSingleByte(char *dst, const wchar_t **src, size_t len)
{
		USES_CONVERSION;	// declare locals used by the ATL macros

		string str(W2CA(*src));
		strncpy(dst, str.c_str(), len);
		dst[len] = '\0';

		return 1;

        // ANOTHER WAY /////////////////////////////
	// allocate space for a zero-terminated copy of the string
	//char *charPtrString = new char[stdString.size()+1];
	// copy the string
	//std::strcpy(charPtrString, stdString.c_str());
	// use charPtrString
	//...
	// clean up the allocated space
	//delete[] charPtrString;
	///////////////////////////////////
}

// Unparse a string, preparing it to go into an AIP message.
// 1. Change '"' to \" because it will be embedded in a quoted string.
// 2. Change "|" to \174 because it will be hidden in an AIP message.
size_t UnParseString( string& strText )
{
	int i;
	int size;
	int ifirst;
	int ilast;

	size = strText.size();
	ifirst = strText.find_first_of('"');
	ilast = strText.find_last_of('"');
	for (i = ifirst; i <= ilast; i++) {
		if ((strText.c_str())[i] == '"') {
			strText.insert(i++, "\\");
			//strText.replace(i++, 1, "\\042");
			ilast++;
		}
	}

	size = strText.size();
	ifirst = strText.find_first_of('|');
	ilast = strText.find_last_of('|');
	for (i = ifirst; i <= ilast; i++) {
		if ((strText.c_str())[i] == '|') {
			strText.replace(i++, 1, "\\174");
			ilast++;
		}
	}
	
	return 1;
}

// Break a string "abcd\nefgh\nijk" into "abcd|efgh|ijk"
// where essentially newlines are replaced with pipes.
size_t BreakString( string& strText )
{
	int i;
	int size;
	int ifirst;
	int ilast;

	size = strText.size();
	ifirst = strText.find_first_of('\n');
	ilast = strText.find_last_of('\n');
	for (i = ifirst; i <= ilast; i++) {
		if ((strText.c_str())[i] == '\n') {
			strText.replace(i++, 1, "|");
			ilast++;
		}
	}
	
	return 1;
}

