// HSListBox.cpp : Implementation of CHSListBox

/*
 * $Id$
 *
 * Description	: Listbox for displaying output
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
 * Provides a Listbox for displaying output from HyperScript
 *
 * $Log$
 * Revision 1.7  2008-06-06 00:48:36  bergsma
 * no message
 *
 * Revision 1.6  2008-06-01 15:27:37  bergsma
 * New functionality to listbox
 *
 * Revision 1.5  2005-02-11 19:14:35  jbergsma
 * Fixed horizontal scroll bar problem in the list box.
 *
 * Revision 1.4  2005/01/24 19:34:39  jbergsma
 * Parse command tokens instead of eval
 *
 * Revision 1.3  2005/01/18 19:26:10  jbergsma
 * Fixed Puts insert string and scroll operation.
 * Added an Edit Control for entering HyperScript commands.
 *
 * Revision 1.2  2004/12/26 17:24:21  jbergsma
 * Append string to end of listbox and force scroll down
 *
 * Revision 1.1  2004/12/17 17:49:19  jbergsma
 * Implements a Listbox for displaying output from HyperScript
 *
 *
 */

#include "stdafx.h"
#include "HyperScript2.h"
#include "HSListBox.h"
#include "utilx.h"
#include "interface.h"
#include <string>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CHScrollListBox

CHScrollListBox::CHScrollListBox()
{
}

/////////////////////////////////////////////////////////////////////////////
CHScrollListBox::~CHScrollListBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// Get the length of the text in units of the the Device Context

int CHScrollListBox::GetTextLen(LPCTSTR lpszText)
{
	HDC hDC = GetDC();

	SIZE sizeRect;

	GetTextExtentPoint(hDC, lpszText, (int) _tcslen(lpszText), &sizeRect);
	//sizeRect.cx += 3;		// Add a "fudge" factor

	ReleaseDC(hDC);

	return sizeRect.cx;
}

///////////////////////////////////////////////////////////////////////////////
// Set the Horizontal Scroll Bar to the extent of the longest string

void CHScrollListBox::ResetHExtent()
{
	if (GetCount() == 0)
	{
		SetHorizontalExtent(0);
		return;
	}

	int iMaxHExtent = 0;

	for (int i = 0; i < GetCount(); i++)
	{
		string sText;
		char *text = (char *) sText.c_str();
		GetText(i, text);
		int iExt = GetTextLen(text);
		if (iExt > iMaxHExtent)
			iMaxHExtent = iExt;
	}

	SetHorizontalExtent(iMaxHExtent);
}

///////////////////////////////////////////////////////////////////////////////
// Set the Horizontal Scroll Bar to at least the extent of this new string

void CHScrollListBox::SetNewHExtent(LPCTSTR lpszNewString)
{
	int iExt = GetTextLen(lpszNewString);
	if (iExt > GetHorizontalExtent())
		SetHorizontalExtent(iExt);
}

/////////////////////////////////////////////////////////////////////////////
// CHSListBox

// Append a string to the listbox and scroll down.
// Adjust the horizontal scroll bar extent.

int CHSListBox::Puts(LPCTSTR lpszItem)
{
	int status = 0;

	if ( IsActive() && (status = m_lb.InsertString( -1, lpszItem )) )
	{
		m_lb.SetCaretIndex( m_lb.GetCount() - 1 ) ;
		m_lb.SetNewHExtent( lpszItem ) ;
	}

	return status;
}

// Is the listbox active?

BOOL CHSListBox::IsActive()
{
	return m_isActive;
}

// Take the last line(s) from the edit window and send to the HyperScript Parse method

LRESULT CHSListBox::OnChangeEditParse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	static int lastLineCount = 1;			// Keep track of the last line processed
	int lineCount = m_edit_parse.GetLineCount();
	if (lineCount == lastLineCount) return 0;
	lineCount--;							// We are processing the previous line(s)

	// Could be multiple lines to process
	for (int iLine = lastLineCount - 1; iLine < lineCount; iLine++) {

		int len = m_edit_parse.LineLength(m_edit_parse.LineIndex(iLine));
		if (len == 0) continue;

		string strText;
		const char *token;

		strText.resize(len, '\0');
		token = strText.c_str();

		m_edit_parse.GetLine(iLine, (LPTSTR) token, len);

		// Put the token on the Parse Queue for HyperScriptParseTokens
		CHyperScript2::m_hsTokenParseQueue.set_Token( strText );

		strText.erase();
	}

	lastLineCount = lineCount + 1;

	return 0;
}

LRESULT CHSListBox::OnCLEAR(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    m_lb.ResetContent();
    return 0;
}

// Take the last line(s) from the edit window and send to the HyperScript Eval method

LRESULT CHSListBox::OnChangeEditEval(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	static int lastLineCount = 1;			// Keep track of the last line processed
	int lineCount = m_edit_eval.GetLineCount();
	if (lineCount == lastLineCount) return 0;
	lineCount--;							// We are processing the previous line(s)

	// Could be multiple lines to process
	for (int iLine = lastLineCount - 1; iLine < lineCount; iLine++) {

		int len = m_edit_eval.LineLength(m_edit_eval.LineIndex(iLine));
		if (len == 0) continue;

		string strText;
		const char *token;

		strText.resize(len, '\0');
		token = strText.c_str();

		m_edit_eval.GetLine(iLine, (LPTSTR) token, len);

		// DO THIS to eval directly
		//gHyp_hs_eval( token );	// call HyperScript Eval

		UnParseString( strText ) ;

		// DO THIS to put tokens on the queue
		// Put the token on the Eval Queue for HyperScriptEvalTokens
		CHyperScript2::m_hsTokenEvalQueue.set_Token( strText );

		strText.erase();
	}

	lastLineCount = lineCount + 1;

	return 0;
}
