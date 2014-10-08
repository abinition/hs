// hsx.cpp : Implementation of DLL Exports.

/*
 * $Id$
 *
 * Description	: Main entry point for the "WebPickle" control
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
 * Initialize the COM objects and start a new thread to handle
 * parsing of the HyperScript tokens
 *
 * $Log$
 * Revision 1.2  2004/12/26 17:26:23  jbergsma
 * Start a second thread to handle the Eval tokens
 *
 * Revision 1.1  2004/12/17 17:52:50  jbergsma
 * Main entry point for the "WebPickle" ActiveX control
 *
 *
 */

static const char *cvsid = "$Header$";

// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f hsxps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "hsx.h"

#include "hsx_i.c"
#include "WebPickle.h"
#include "HyperScript2.h"

IGlobalInterfaceTable *g_pGIT = NULL;

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_WebPickle, CWebPickle)
OBJECT_ENTRY(CLSID_HyperScript2, CHyperScript2)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
		ATLASSERT(g_pGIT == NULL);
		if ( SUCCEEDED( CoCreateInstance( CLSID_StdGlobalInterfaceTable, 0,
					CLSCTX_INPROC_SERVER,
					IID_IGlobalInterfaceTable,
					(void**)&g_pGIT)))
		{
			_Module.Init(ObjectMap, hInstance, &LIBID_HSXLib);

			// Track down interface leak
			//_Module.m_nIndexBreakAt = 4;

			DisableThreadLibraryCalls(hInstance);

			UINT *pthrdaddr = NULL;
			UINT *ethrdaddr = NULL;

	// Spawn a thread to process the HyperScript Parse token queue.
			_beginthreadex(NULL, 0, HyperScriptParseTokens, NULL, 0, pthrdaddr);
	// Spawn a thread to process the HyperScript Eval token queue
			_beginthreadex(NULL, 0, HyperScriptEvalTokens, NULL, 0, ethrdaddr);
		}
    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
		if ( g_pGIT)
			g_pGIT->Release();
        _Module.Term();
	}

    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


