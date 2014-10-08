// JSCall.cpp: implementation of the CJSCall class.
//
//////////////////////////////////////////////////////////////////////
/*
 * $Id$
 *
 * Description	: Class for calling JavaScript functions
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
 * Provides methods to execute defined JavaScript functions on a web page.
 *
 * $Log$
 * Revision 1.3  2008-05-12 17:49:05  bergsma
 * *** empty log message ***
 *
 * Revision 1.2  2005-02-06 18:18:32  jbergsma
 * Added a new function, GetNextArgument, to deal with embedded commas.
 *
 * Revision 1.1  2005/01/18 19:52:42  jbergsma
 * New class to deal with executing JavaScript from C++
 *
 */

static const char *cvsid = "$Header$";

#include "stdafx.h"
#include "JSCall.h"

#define WHITESPACE	" \n\r\t"

#define CHECK_POINTER(p)\
	ATLASSERT(p != NULL);\
	if(p == NULL)\
	{\
		ShowError("NULL pointer");\
		return false;\
	}

const char * GetSystemErrorMessage(DWORD dwError)
{
	static string strError;
	LPTSTR lpBuffer;

	if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,  dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR) &lpBuffer, 0, NULL))

	{
		strError.assign( "FormatMessage Netive Error" );
	}
	else
	{
		strError.assign(lpBuffer);
		LocalFree(lpBuffer);
	}
	return strError.c_str();
}

// Find the first occurrence of a character from strDelim in strSrc

int FindOneOf( string& strSrc, const string strDelim )
{
	int slen = strSrc.length();
/*
 * The "brute-force way...
 *
	int dlen = strDelim.length();
	int i;
	const char *s = NULL;

	for (s = strSrc.c_str(), i = 0; i < slen; i++) {
		char c = *s++;
		if (strDelim.find(c) < dlen)
			return i;
	}

    return -1;
*/
	// This does the trick.
	int idx = strSrc.find_first_of(strDelim);

	return (idx > 0 && idx < slen) ? idx : -1;
}

// Get the next complete word (token) separated by one of the delimiters
// The token is removed from strSrc

string GetNextToken(string& strSrc, const string strDelim, bool bTrim, bool bFindOneOf)
{
	string strToken;
	strSrc.erase(0, strSrc.find_first_not_of(WHITESPACE));			// TrimLeft
	int idx = bFindOneOf? FindOneOf(strSrc, strDelim) : strSrc.find(strDelim);
	if (idx != -1)
	{
		strToken  = strSrc.substr(0, idx);								// Left
		strSrc = strSrc.substr(idx + 1, strSrc.length() - (idx + 1) );	// Right
	}
	else
	{
		strToken = strSrc;
		strSrc.erase();
	}
	if (bTrim)
	{
		strToken.erase(0, strToken.find_first_not_of(WHITESPACE));	// TrimLeft
		strToken.erase(strToken.find_last_not_of(WHITESPACE) + 1);	// TrimRight
	}
	return strToken;
}

// Get the next complete argument separated by a comma character.
// The argument is removed from strSrc.
// Recognize when a comma is embedded in a text string and ignore it.
// The valid text string delimiters are ", &quot;, <p></p>, and {} ..
// Assume that these quote types won't ever be interspersed,
// but one pair of quotes could be inside the other.
// For example, "&quot;,&quot;" is handled.

// TODO - Recognize all the HTML tags that could contain text strings

#define DBL_QUOTE		0
#define URL_QUOTE		1
#define PARA_QUOTE		2
#define CURLY_QUOTE		3
#define N_QUOTE_TYPES	4

string GetNextArgument(string& strSrc)
{
	string strArg;
	bool bFound = false;
	int commaIndex, vQuoteIndex[N_QUOTE_TYPES], quoteIndex, quoteType, i;
	int len = strSrc.length();
	commaIndex = 0 ;
	commaIndex = strSrc.find_first_of(',',commaIndex);
	
	while ( commaIndex < len && (commaIndex = strSrc.find_first_of(',',commaIndex)) < len) {
	  if (commaIndex == -1) break;
	  if ( (commaIndex == 0 ) || (commaIndex > 0 && strSrc[commaIndex-1] != '\\') ) {
	    // Comma at beginning or comma without backslash.  This one is good
	    break ;
	  }
	  else if ( commaIndex > 0 && strSrc[commaIndex-1] == '\\' ) {
	    // Comma with backslash.  Remove backslash but don't use this comma.
	    strSrc.erase(commaIndex-1, 1 );
	    --len ;
	  }
	  commaIndex++ ;
	}
	

	quoteIndex = 0;

	while (!bFound)
	{
		vQuoteIndex[DBL_QUOTE] = strSrc.find_first_of('"', quoteIndex);
		vQuoteIndex[URL_QUOTE] = strSrc.find("&quot;", quoteIndex);
		vQuoteIndex[PARA_QUOTE] = strSrc.find("<p>", quoteIndex);
		vQuoteIndex[CURLY_QUOTE] = strSrc.find("{", quoteIndex);
		quoteIndex = len;

		for (i = 0; i < N_QUOTE_TYPES; i++)
			if (vQuoteIndex[i] >= 0 && vQuoteIndex[i] < quoteIndex)
			{
				quoteType = i;
				quoteIndex = vQuoteIndex[i];
			}

		if (quoteIndex < 0 || quoteIndex == len)
			bFound = true;

		else if (commaIndex >= 0 && commaIndex < len && commaIndex > quoteIndex)
		{
			switch (quoteType) {

			case DBL_QUOTE:
				quoteIndex = strSrc.find_first_of('"', vQuoteIndex[DBL_QUOTE] + 1);
				break;

			case URL_QUOTE:
				quoteIndex = strSrc.find("&quot;", vQuoteIndex[URL_QUOTE] + 1);
				break;

			case PARA_QUOTE:
				quoteIndex = strSrc.find("</p>", vQuoteIndex[PARA_QUOTE] + 1);
				break;

			case CURLY_QUOTE:
				quoteIndex = strSrc.find("}", vQuoteIndex[CURLY_QUOTE] + 1);
				break;

			default:
				quoteIndex = len;
			}

			while (commaIndex >= 0 && commaIndex < quoteIndex)
				commaIndex = strSrc.find_first_of(',', commaIndex + 1);
			/*
			while ( commaIndex >= 0 && commaIndex < quoteIndex ) {
			  commaIndex = strSrc.find_first_of(',', commaIndex + 1);
			  if (commaIndex == -1) break;
			  if ( (commaIndex == 0 ) || (commaIndex > 0 && strSrc[commaIndex-1] != '\\') ) {
			    // Comma at beginning or comma without backslash.  This one is good
			    break ;
			  }
			  else if ( commaIndex > 0 && strSrc[commaIndex-1] == '\\' ) {
			    // Comma with backslash.  Remove backslash but don't use this comma.
			    strSrc.erase(commaIndex-1, 1 );
			    --len ;
			  }
			  commaIndex++ ;
			}
			*/
			quoteIndex = (quoteIndex == len) ? len : quoteIndex + 1;
		}

		else
			bFound = true;
	}

	if (commaIndex >= 0 && commaIndex < len)
	{
		strArg = strSrc.substr(0, commaIndex);							// Left
		strSrc = strSrc.substr(commaIndex + 1, len - (commaIndex + 1) );// Right
	}

	else
	{
		strArg = strSrc;
		strSrc.erase();
	}

	return strArg;

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJSCall::CJSCall()
{
}

CJSCall::~CJSCall()
{
	m_spDoc.Release();
}

// Save a copy of the IHTMLDocument2 interface pointer

HRESULT CJSCall::SetDocument(CComPtr<IHTMLDocument2>& spDocument)
{
	CHECK_POINTER(spDocument);

	m_spDoc.Detach();

	// Can't use CopyTo because it expects a null pointer p
	IHTMLDocument2 *pDoc = spDocument.p;
	m_spDoc = pDoc;

	if ( !m_spDoc )
	{
		ShowError("Failed to get HTML document COM object");
		return E_FAIL;
	}

	pDoc->AddRef();

	return S_OK;
}

// Get an interface pointer to the JavaScript objects

bool CJSCall::GetJScript(CComPtr<IDispatch>& spDisp)
{
	CHECK_POINTER(m_spDoc);
	HRESULT hr = m_spDoc->get_Script(&spDisp);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

// Get the entire collection of script elements

bool CJSCall::GetJScripts(CComPtr<IHTMLElementCollection>& spColl)
{
	CHECK_POINTER(m_spDoc);
	HRESULT hr = m_spDoc->get_scripts(&spColl);
	ATLASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

// The following methods can be used to execute a JavaScript function

bool CJSCall::CallJScript(const string strFunc,CComVariant* pVarResult)
{
	CstringArray paramArray;
	return CallJScript(strFunc,paramArray,pVarResult);
}

bool CJSCall::CallJScript(const string strFunc,const string strArg1,CComVariant* pVarResult)
{
	CstringArray paramArray;
	paramArray.Add(strArg1);
	return CallJScript(strFunc,paramArray,pVarResult);
}

bool CJSCall::CallJScript(const string strFunc,const string strArg1,const string strArg2,CComVariant* pVarResult)
{
	CstringArray paramArray;
	paramArray.Add(strArg1);
	paramArray.Add(strArg2);
	return CallJScript(strFunc,paramArray,pVarResult);
}

bool CJSCall::CallJScript(const string strFunc,const string strArg1,const string strArg2,const string strArg3,CComVariant* pVarResult)
{
	CstringArray paramArray;
	paramArray.Add(strArg1);
	paramArray.Add(strArg2);
	paramArray.Add(strArg3);
	return CallJScript(strFunc,paramArray,pVarResult);
}

// The method that does the real work to execute the JavaScript through INVOKE

bool CJSCall::CallJScript(const string strFunc, const CstringArray& paramArray, CComVariant* pVarResult)
{
	// Get an interface pointer to all the JavaScript functions
	CComPtr<IDispatch> spScript;
	if(!GetJScript(spScript))
	{
		ShowError("Cannot GetScript");
		return false;
	}

	// See if the function we want to execute exists and get it's dispid
	CComBSTR bstrMember(strFunc.c_str());
	DISPID dispid = NULL;
	HRESULT hr = spScript->GetIDsOfNames(IID_NULL,&bstrMember,1,
											LOCALE_SYSTEM_DEFAULT,&dispid);
	if(FAILED(hr))
	{
		ShowError(GetSystemErrorMessage(hr));
		return false;
	}

	// Construct the dispatch parameter array
	const int arraySize = paramArray.GetSize();

	DISPPARAMS dispparams;
	memset(&dispparams, 0, sizeof dispparams);
	dispparams.cArgs = arraySize;
	dispparams.rgvarg = new VARIANT[dispparams.cArgs];
	
	for( int i = 0; i < arraySize; i++)
	{
		CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
		bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);
		dispparams.rgvarg[i].vt = VT_BSTR;
	}
	dispparams.cNamedArgs = 0;

	EXCEPINFO excepInfo;
	memset(&excepInfo, 0, sizeof excepInfo);
   	CComVariant vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg

	// Execute the JavaScript method
	hr = spScript->Invoke(dispid,IID_NULL,LOCALE_SYSTEM_DEFAULT,
		DISPATCH_METHOD,&dispparams,&vaResult,&excepInfo,&nArgErr);

	delete [] dispparams.rgvarg;

	if(FAILED(hr))
	{
		ShowError(GetSystemErrorMessage(hr));
		return false;
	}
	
	if(pVarResult)
	{
		*pVarResult = vaResult;
	}
	return true;
}

// Returned java script function name, input string is truncating

string CJSCall::ScanJScript(string& strAText, CstringArray& args)
{
	args.RemoveAll();
	string strDelim(WHITESPACE), strSrc(strAText);
	bool bFound = false;

	while(!strSrc.empty())
	{
		string strStart = GetNextToken(strSrc, strDelim);
		if(strStart.compare(0, 8, "function") == 0)
		{
			bFound = true;
			break;
		}
		if(strStart.compare(0, 2, "/*") == 0)
		{
			// Skip comments
			while(!strSrc.empty())
			{
				string strStop = GetNextToken(strSrc, strDelim);
				if(strStop.compare(0, 2, "*/") == 0)
				{
					break;
				}
			}
		}
	}

	if(!bFound)
		return "";
	
	string strFunc = GetNextToken(strSrc, "(", true);
	string strArgs = GetNextToken(strSrc, ")", true);

	// Parse arguments
	string strArg;
	while(!(strArg = GetNextToken(strArgs, ",")).empty())
		args.Add(strArg);

	strAText= strSrc;
	return strFunc;
}

size_t RemoveSlashQuote( string& strText )
{
  //gHyp_util_debug("Arguments before: %s",strArgs.c_str())
  int idx = 0 ;
  int len = strText.length() ;
  while (idx < len && (idx = strText.find_first_of('"', idx) ) < len) {
			
    if (idx == -1) break;

    if ( idx > 0 && strText[idx-1] == '\\' ) {

      strText.erase(idx-1, 1 );
      --len ;
					  		
    }
    idx++ ;
  }
  return len ;
}
