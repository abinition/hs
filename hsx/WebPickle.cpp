// WebPickle.cpp : Implementation of CWebPickle

/*
 * $Id$
 *
 * Description	: Implementation of the IWebPickel interface.
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
 * Provide methods and properties to manipulate the web browser via DHTML
 * and to execute JavaScript functions.
 *
 * $Log$
 * Revision 1.7  2008-05-12 17:49:06  bergsma
 * *** empty log message ***
 *
 * Revision 1.6  2005-02-11 19:19:47  jbergsma
 * Call OnDocumentComplete only once (after page has been loaded).
 *
 * Revision 1.5  2005/02/06 18:22:37  jbergsma
 * Use GetNextArgument to parse the argument list.
 *
 * Revision 1.4  2005/01/24 19:44:33  jbergsma
 * Removed include of hsx.h.
 * Don't need to check if listbox is active.
 *
 * Revision 1.3  2005/01/18 20:05:21  jbergsma
 * Obtain interfaces to the Web Document.
 * Set up the Global Interface Table to marshal interfaces across apartments.
 * Receive and process events from the web browser.
 * Set our control to run invisible and directly imbed into the browser.
 * Added a new method JSCall to execute JavaScript functions.
 *
 * Revision 1.2  2004/12/26 17:41:41  jbergsma
 * Check if dialog is active before writing to it
 *
 * Revision 1.1  2004/12/17 18:15:51  jbergsma
 * The WebPickle functions and COM interface to interact with the UI (browser)
 *
 *
 */

static const char *cvsid = "$Header$";

#include "stdafx.h"
#include "WebPickle.h"

#include <atlconv.h>
#include <string>

using namespace std;

// static members (i.e. "globals")

CHSListBox CWebPickle::m_hsListBox;			// HyperScript Console
DWORD CWebPickle::m_dwIWebPickleUICookie;	// Cookie for IWebPickleUI GIT
DWORD CWebPickle::m_dwIWebPickleCookie;		// Cookie for IWebPickle GIT

/////////////////////////////////////////////////////////////////////////////
// CWebPickle

// Example method called by the HTML to change the <BODY> background color
STDMETHODIMP CWebPickle::OnClick(IDispatch* pdispBody, VARIANT varColor)
{
	CComQIPtr<IHTMLBodyElement> spBody(pdispBody);
	if (spBody != NULL)
		spBody->put_bgColor(varColor);
	return S_OK;
}

// Note: We don't do this here. We embed directly into the running instance of IE...
//
// Create a WebBrowser Control to host this ActiveX control
// This only gets called if m_bWindowOnly = TRUE and we don't set OLEMISC_INVISIBLEATRUNTIME
//
LRESULT CWebPickle::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Wrap the control's window to use it to host contol
	// (not an AtlAxWin, so no CAxHostWindow yet created)
	CAxWindow wnd(m_hWnd);

	// Create a CAxWinHost: It will subclass this window and
	// create a contol based on an HTML resource.
	HRESULT hr = wnd.CreateControl(IDH_WEBPICKLE);

	// The HTML has a hook into this control via window.external.
	// The ATL Object Wizard generates two dual interfaces on the HTML control.
	// The first, IWebPickle, is the default interface available to the control's clients.
	// The second, IWebPickleUI, is an interface given to the WebBrowser control.
	if (SUCCEEDED(hr))
		hr = wnd.SetExternalDispatch(static_cast<IWebPickleUI*>(this));

	// Get the IWebBrowser2 interface of the HTML control.
	if (SUCCEEDED(hr))
		hr = wnd.QueryControl(IID_IWebBrowser2, (void**)&m_spWebBrowser);

	return SUCCEEDED(hr) ? 0 : -1;
}

// IOleObject

STDMETHODIMP CWebPickle::SetClientSite(IOleClientSite* pClientSite)
{
	HRESULT hr = IOleObjectImpl<CWebPickle>::SetClientSite( pClientSite ); // let base class handle it

	if (!pClientSite)
	{
		ATLASSERT(m_spWebBrowser);

		if (m_spWebBrowser)
		{
			// Done with event processing
			AtlUnadvise(m_spWebBrowser, DIID_DWebBrowserEvents2, m_dwCookie);
		}

		ATLASSERT(g_pGIT);

		if (g_pGIT)
		{
			// Done with unmarshaling interface pointers
			g_pGIT->RevokeInterfaceFromGlobal( m_dwIWebPickleUICookie );
			g_pGIT->RevokeInterfaceFromGlobal( m_dwIWebPickleCookie );
		}

		return hr;
	}

	m_pUnk = GetUnknown();

	// Register the interface pointers in the GIT
	hr = g_pGIT->RegisterInterfaceInGlobal( m_pUnk, IID_IWebPickleUI, &m_dwIWebPickleUICookie );
	if (SUCCEEDED(hr))
		hr = g_pGIT->RegisterInterfaceInGlobal( m_pUnk, IID_IWebPickle, &m_dwIWebPickleCookie );

	CComPtr<IOleContainer> spContainer;
	if (SUCCEEDED(hr))
		hr = m_spClientSite->GetContainer(&spContainer);

	ATLASSERT(spContainer);

	if (SUCCEEDED(hr))
	{
		// Set up the event sink to handle IE events
		//
		CComQIPtr<IServiceProvider, &IID_IServiceProvider>
			spServiceProvider(spContainer);

		ATLASSERT(spServiceProvider);

		if (spServiceProvider)
		{
			spServiceProvider->QueryService(IID_IWebBrowserApp,
						IID_IWebBrowser2,
						(void**)&m_spWebBrowser);
			ATLASSERT(m_spWebBrowser);

			if (m_spWebBrowser)
			{
				// Advise the browser that we want to receive events from it
				AtlAdvise(m_spWebBrowser, GetUnknown(),
							DIID_DWebBrowserEvents2, &m_dwCookie);

				// Get the IHTMLDocument2 interface
				IHTMLDocument2* pdispDoc;
				hr = m_spWebBrowser->get_Document( (IDispatch **) &pdispDoc );
				if (FAILED(hr)) return hr;
				m_spDocument.Attach(pdispDoc);
				if ( !m_spDocument ) return E_FAIL;

				// Cache this interface for JSCall
				if (SUCCEEDED(hr))
					hr = m_jsCall.SetDocument(m_spDocument);

				m_spScripts = NULL;
			}
		}
	}

	return hr;
}

// Override the IDispatchImpl::Invoke to sink IE events
//
STDMETHODIMP CWebPickle::Invoke(DISPID dispidMember, 
								REFIID riid, 
								LCID lcid,
								WORD wFlags, 
								DISPPARAMS* pDispParams, 
								VARIANT* pvarResult, 
								EXCEPINFO* pExcepInfo,
								UINT* puArgErr)
{
	HRESULT hr = S_OK ;

	if (riid != IID_NULL)
		return DISP_E_UNKNOWNINTERFACE;

	if (!pDispParams)
		return DISP_E_PARAMNOTOPTIONAL;

	//if ( dispidMember < 0 ) 
	//  return DISP_E_UNKNOWNINTERFACE ;

	switch (dispidMember)
	{
		//
		// The parameters for this DISPID:
		// [0]: Maximum progress - VT_I4
		// [1]: Amount of total progress - VT_I4
		//
		case DISPID_PROGRESSCHANGE:
			if (pDispParams->cArgs != 0)
			{
				// Make sure that you access the
				// correct data member of the rgvarg array.
				// To do this, check the type of data to
				// make sure it is correct.
				//
				if (pDispParams->cArgs > 1
					&& pDispParams->rgvarg[1].vt == VT_I4
					&& pDispParams->rgvarg[0].vt == VT_I4)
				{
					if (-1 == pDispParams->rgvarg[1].lVal)	// Indicates Progress completed
						// TODO - how to get IDispatch of the caller to pass into OnDocumentComplete
						//hr = OnDocumentComplete(m_spWebBrowser, pvarResult);
						break;	// Don't do anything here right now
				}
			}

			break;

		case DISPID_DOCUMENTCOMPLETE:
			// TODO - how to get IDispatch of the caller to pass into OnDocumentComplete
			hr = OnDocumentComplete(m_spWebBrowser, pvarResult);

			break;

		default:
			// Call the base class implementation of Invoke
			// so that IWebPickle methods and properties will
			// work correctly.
			//
			// Find out which interface called us...
			//
			CComQIPtr<IWebPickleUI> spWebPickleUI = m_pUnk;
			CComQIPtr<IWebPickle> spWebPickle = m_pUnk;
			//
			// And call the appropriate base class
			//
			if (this == spWebPickleUI)
				hr = IDispatchImpl<IWebPickleUI, &IID_IWebPickleUI, 
					&LIBID_HSXLib>::Invoke(	dispidMember, riid, lcid,
								wFlags, pDispParams,
								pvarResult, pExcepInfo, puArgErr );
			else if (this == spWebPickle)
				hr = IDispatchImpl<IWebPickle, &IID_IWebPickle, 
						&LIBID_HSXLib>::Invoke(	dispidMember, riid, lcid,
										wFlags, pDispParams,
										pvarResult, pExcepInfo, puArgErr );
			else
				hr = DISP_E_UNKNOWNINTERFACE;

			if ( hr == DISP_E_MEMBERNOTFOUND ) hr = S_OK;

			break;
	}

	return hr;
}

// Get the document script elements after the page has loaded
//
// TODO - How to get the javascript functions that are "sourced" by this document
//        i.e. from <script language="JavaScript" type="text/javascript" src="jssource.js"></script>
//
STDMETHODIMP CWebPickle::OnDocumentComplete( IDispatch * pDisp, VARIANT * URL )
{
	HRESULT hr = S_OK;

	// This is document in frame if pDisp is not the same as IDispatch of IWebBrowser2
	// Only the top level frame IDispatch is the same as IDispatch from IWebBrowser2
	CComQIPtr<IDispatch> spDispatch = m_spWebBrowser;
	bool bFrame = spDispatch != pDisp;

	if ( !bFrame && !m_spScripts )	// Not in frame and have not been called already
	{
		// Get the JavaScript functions from the page
		if (SUCCEEDED(hr)) {
		
		    if (!m_jsCall.GetJScripts(m_spScripts))
				return E_FAIL;

			long count = 0;
			hr = m_spScripts->get_length(&count);
			ATLASSERT(SUCCEEDED(hr));
			
			long idx = -1;
			while(count--)
			{
				idx++;
				
				CComPtr<IDispatch> spDispatch;
				CComVariant name(idx), index(idx);
				hr = m_spScripts->item(name, index, (IDispatch**)&spDispatch);
				ATLASSERT(SUCCEEDED(hr));
				if(FAILED(hr))
					continue;

				CComPtr<IHTMLScriptElement> spScriptObj;
				hr = spDispatch->QueryInterface(IID_IHTMLScriptElement, (void**)&spScriptObj);
				ATLASSERT(SUCCEEDED(hr));
				if(FAILED(hr))
					continue;

				CComBSTR bstr;
				hr = spScriptObj->get_text(&bstr);
				ATLASSERT(SUCCEEDED(hr));

				USES_CONVERSION;	// declare locals used by the ATL macros

				string strText(W2CA(bstr));
				string strFunc;
				CstringArray args;

				while( !(strFunc = m_jsCall.ScanJScript(strText, args)).empty())
				{
					// TODO - collect the function names and args in a list
					// We could present these as a selectable list to the user, for example
					const char *arg = args.GetAt(0);
				}
			}
		}
	}
	return hr;
}

// Override GetMiscStatus to set our control invisible at runtime
//
STDMETHODIMP CWebPickle::GetMiscStatus (DWORD dwAspect, DWORD *pwdStatus)
{
	if (pwdStatus == NULL) return E_POINTER;

	HRESULT hr = IOleObjectImpl<CWebPickle>::GetMiscStatus(dwAspect, pwdStatus);
	if (SUCCEEDED(hr))
		*pwdStatus |= OLEMISC_INVISIBLEATRUNTIME;

	return hr;
}

// Start up a modless dialog to view the output and enter commands
//
STDMETHODIMP CWebPickle::CreateListBox()
{
	if (m_hsListBox.IsActive())
		return S_OK;

	HWND hsListBoxWnd = NULL;

	if (!(hsListBoxWnd = m_hsListBox.CDialogListBox::Create(GetActiveWindow())))
		return E_FAIL;

	m_hsListBox.SetDialogFocus();	// Try to set focus to edit control

	return S_OK;
}

// Append a string to the output listbox
//
STDMETHODIMP CWebPickle::Puts(BSTR bstr)
{
	if (bstr != NULL) {

		USES_CONVERSION;	// declare locals used by the ATL macros

		string str(W2CA(bstr));

		m_hsListBox.Puts( str.c_str() );
	}

	return S_OK;
}

// Call a JavaScript function on the web page
//
STDMETHODIMP CWebPickle::JSCall(BSTR function, BSTR args)
{
	if (function != NULL) {

		USES_CONVERSION;	// declare locals used by the ATL macros

		string strFunc(W2CA(function));
		CstringArray paramArray;
		CComVariant *pVarResult = NULL;

		if (args != NULL) {
			string strArgs(W2CA(args));
			// Parse arguments
			string strArg;
			while(!(strArg = GetNextArgument(strArgs)).empty()) {
			  // Look for \" and remove the '\'
			  RemoveSlashQuote(strArg);
			  paramArray.Add(strArg);
			}
		}

		if (!m_jsCall.CallJScript(strFunc, paramArray, pVarResult))
		{
			string error("CallJScript Error: ");
			error.append(m_jsCall.GetLastError());
			m_hsListBox.Puts( error.c_str() );
			return E_FAIL;
		}
	}

	return S_OK;
}
