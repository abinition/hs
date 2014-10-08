// WebPickle.h : Declaration of the CWebPickle
/*
 * $Id$
 *
 * $Log$
 * Revision 1.5  2005/01/24 19:45:12  jbergsma
 * Added include of hsx.h.
 *
 * Revision 1.4  2005/01/18 20:09:03  jbergsma
 * Define interfaces to the Web Document.
 * Define GIT cookies to marshal interfaces across apartments.
 * Added methods to process events from the web browser.
 * Added a new method JSCall to execute JavaScript functions.
 *
 * Revision 1.3  2004/12/26 17:42:23  jbergsma
 * Define a global pointer to the listbox
 *
 * Revision 1.2  2004/12/17 18:19:51  jbergsma
 * Added Id and Log tags for cvs.
 *
 */

#ifndef __WEBPICKLE_H_
#define __WEBPICKLE_H_

#include "resource.h"       // main symbols
#include "hsx.h"
#include <atlctl.h>
#include "exdispid.h"	// dispatch IDs for DWebBrowserEvents2

#include "HSListBox.h"
#include "JSCall.h"

/////////////////////////////////////////////////////////////////////////////
// CWebPickle
class ATL_NO_VTABLE CWebPickle : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IWebPickle, &IID_IWebPickle, &LIBID_HSXLib>,
	public IDispatchImpl<IWebPickleUI, &IID_IWebPickleUI, &LIBID_HSXLib>,
	public CComControl<CWebPickle>,
	public IPersistStreamInitImpl<CWebPickle>,
	public IOleControlImpl<CWebPickle>,
	public IOleObjectImpl<CWebPickle>,
	public IOleInPlaceActiveObjectImpl<CWebPickle>,
	public IViewObjectExImpl<CWebPickle>,
	public IOleInPlaceObjectWindowlessImpl<CWebPickle>,
	public IPersistStorageImpl<CWebPickle>,
	public ISpecifyPropertyPagesImpl<CWebPickle>,
	public IQuickActivateImpl<CWebPickle>,
	public IDataObjectImpl<CWebPickle>,
	public IProvideClassInfo2Impl<&CLSID_WebPickle, NULL, &LIBID_HSXLib>,
	public CComCoClass<CWebPickle, &CLSID_WebPickle>
{

public:
	CWebPickle()
	{
		m_bWindowOnly = FALSE;
		m_dwCookie = 0;
		m_dwIWebPickleUICookie = 0;
		m_dwIWebPickleCookie = 0;
	}

	~CWebPickle()
	{
		m_spScripts.Release();
		m_spDocument.Release(); 
		m_spWebBrowser.Release();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WEBPICKLE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWebPickle)
	COM_INTERFACE_ENTRY(IWebPickle)
	COM_INTERFACE_ENTRY(IWebPickleUI)
	COM_INTERFACE_ENTRY2(IDispatch, IWebPickleUI)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

/*
BEGIN_SINK_MAP(CWebPickle)
//	SINK_ENTRY_EX( 0, SHDocVw::DIID_DWebBrowserEvents2, DISPID_ONQUIT, OnQuit)
	SINK_ENTRY_EX( 0, SHDocVw::DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, DocumentComplete)
END_SINK_MAP()
 */

BEGIN_PROP_MAP(CWebPickle)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_MSG_MAP(CWebPickle)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	CHAIN_MSG_MAP(CComControl<CWebPickle>)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// Declare us safe for scripting
BEGIN_CATEGORY_MAP(CWebPickle)
	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
END_CATEGORY_MAP()

// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// IWebPickle
public:

// IWebPickleUI
public:
	STDMETHODIMP JSCall(/*[in]*/BSTR function, /*[in]*/BSTR args);
	STDMETHODIMP Puts(/*[in]*/BSTR bstr);
	STDMETHODIMP CreateListBox();

	// Example method called by the HTML to change the <BODY> background color
	STDMETHODIMP OnClick(IDispatch*, VARIANT);

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	// IOleObject
	STDMETHODIMP SetClientSite( IOleClientSite* );

	// IDispatchImpl
	STDMETHODIMP Invoke( DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT* );

	// DWebBrowserEvents2 
//	STDMETHODIMP OnQuit( void );
	STDMETHODIMP OnDocumentComplete( IDispatch*, VARIANT* );

	// Override GetMiscStatus to set our control invisible at runtime
	STDMETHODIMP GetMiscStatus(DWORD, DWORD *);

	// HyperScript Console
	static CHSListBox m_hsListBox;

	// Cookies for the GIT to marshal interface pointers from any apartment thread
	static DWORD m_dwIWebPickleUICookie;
	static DWORD m_dwIWebPickleCookie;

private:
	IUnknown *m_pUnk;
	CComQIPtr<IWebBrowser2> m_spWebBrowser;
	CComPtr<IHTMLElementCollection> m_spScripts;
	CComPtr<IHTMLDocument2> m_spDocument;
	DWORD m_dwCookie;	// cookie for AtlAdvise
	CJSCall m_jsCall;
};

#endif //__WEBPICKLE_H_
