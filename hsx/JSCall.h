// JSCall.h: interface for the CJSCall class.
//
//////////////////////////////////////////////////////////////////////
/*
 * $Id$
 *
 * $Log$
 * Revision 1.2  2005-02-06 18:18:59  jbergsma
 * Added a new function, GetNextArgument, to deal with embedded commas.
 *
 * Revision 1.1  2005/01/18 19:52:51  jbergsma
 * New class to deal with executing JavaScript from C++
 *
 */

#if !defined(AFX_JSCALL_H__1776529D_F89A_4506_9B56_B5BCF5D247D2__INCLUDED_)
#define AFX_JSCALL_H__1776529D_F89A_4506_9B56_B5BCF5D247D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
#include "stringArray.h"

class CJSCall
{
public:
	CJSCall();
	virtual ~CJSCall();

	HRESULT SetDocument(CComPtr<IHTMLDocument2>& spDocument);
	LPDISPATCH GetHtmlDocument() const;
	const string GetLastError() const;
	bool GetJScript(CComPtr<IDispatch>& spDisp);
	bool GetJScripts(CComPtr<IHTMLElementCollection>& spColl);

	string ScanJScript(string& strAText, CstringArray& args);

	bool CallJScript(const string strFunc,CComVariant* pVarResult = NULL);
	bool CallJScript(const string strFunc,const string strArg1,CComVariant* pVarResult = NULL);
	bool CallJScript(const string strFunc,const string strArg1,const string strArg2,CComVariant* pVarResult = NULL);
	bool CallJScript(const string strFunc,const string strArg1,const string strArg2,const string strArg3,CComVariant* pVarResult = NULL);
	bool CallJScript(const string strFunc,const CstringArray& paramArray,CComVariant* pVarResult = NULL);

protected:
	void ShowError(LPCSTR lpszText);

	CComPtr<IHTMLDocument2> m_spDoc;
	string m_strError;
};

inline
void CJSCall::ShowError(LPCSTR lpszText)
{
	m_strError.assign("JSCall Error: ");
	m_strError.append(string(lpszText));
}
inline 
const string CJSCall::GetLastError() const
{
	return m_strError;
}
inline
LPDISPATCH CJSCall::GetHtmlDocument() const
{
	return m_spDoc;
}

string GetNextToken(string& strSrc, const string strDelim,bool bTrim = false, bool bFindOneOf = true);
string GetNextArgument(string& strSrc);
size_t RemoveSlashQuote( string& ) ;

#endif // !defined(AFX_JSCALL_H__1776529D_F89A_4506_9B56_B5BCF5D247D2__INCLUDED_)
