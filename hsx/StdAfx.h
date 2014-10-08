// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently
/*
 * $Id$
 *
 * $Log$
 * Revision 1.3  2005-02-06 18:21:49  jbergsma
 * Don't need to include atlwin.h.
 *
 * Revision 1.2  2005/01/18 19:54:26  jbergsma
 * Define a global variable for the Global Interface Table
 *
 * Revision 1.1  2004/12/17 18:04:41  jbergsma
 * Essential include files for ATL and (can be) the precompiled header for cpp
 *
 *
 */

#if !defined(AFX_STDAFX_H__C8A2EDBC_B02C_4E9E_A408_A358BBDA9B16__INCLUDED_)
#define AFX_STDAFX_H__C8A2EDBC_B02C_4E9E_A408_A358BBDA9B16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#define _ATL_ALL_WARNINGS
//#define _ATL_DEBUG_INTERFACES		// Track calls to AddRef and Release
#define _ATL_DEBUG_QI			// Interface level debugging
//#define _ATL_TRACE_LEVEL 2		// Interface leak detection

#include <atlbase.h>

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

extern CComModule _Module;

extern IGlobalInterfaceTable *g_pGIT; // WebPickle objects would register their interface pointers here

#include <atlcom.h>
#include <atlhost.h>
#include <mshtml.h>
#include <exdisp.h>
#include <atlctl.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C8A2EDBC_B02C_4E9E_A408_A358BBDA9B16__INCLUDED)
