// util.h : Declaration of utility functions
/*
 * $Id$
 *
 * $Log$
 * Revision 1.2  2005-01-18 19:58:47  jbergsma
 * Added functions to insert backslash characters before quote characters
 *
 * Revision 1.1  2004/12/17 18:16:21  jbergsma
 * Utiility functions for the WebPickle
 *
 *
 */

#include <wchar.h>
#include <string>

using namespace std;

size_t ConvertBSTRtoSingleByte(char*, const wchar_t**, size_t);
size_t UnParseString( string& );
