// HSListBox.h : Declaration of the CHSListBox
/*
 * $Id$
 *
 * $Log$
 * Revision 1.5  2008-05-12 17:49:05  bergsma
 * *** empty log message ***
 *
 * Revision 1.4  2005-02-11 19:15:11  jbergsma
 * Fixed horizontal scroll bar problem in the list box.
 *
 * Revision 1.3  2005/01/18 19:32:44  jbergsma
 * Added an Edit Control for entering HyperScript commands.
 *
 * Revision 1.2  2004/12/26 17:25:36  jbergsma
 * Check for and destroy active dialog window on exit
 *
 * Revision 1.1  2004/12/17 17:49:19  jbergsma
 * Implements a Listbox for displaying output from HyperScript
 *
 *
 */

#ifndef __HSLISTBOX_H_
#define __HSLISTBOX_H_

#include "resource.h"       // main symbols
#include <atlhost.h>
#include "atlcontrols.h"

class CHSListBox;
typedef CDialogImpl<CHSListBox> CDialogListBox;
typedef ATLControls::CListBox CListBox;
typedef ATLControls::CEdit CEdit;

/////////////////////////////////////////////////////////////////////////////
// CHScrollListBox - Corrects the Horizontal Scrolling Problem

class CHScrollListBox : public CListBox
{
// Construction
public:
	CHScrollListBox();

// Implementation
public:
	virtual ~CHScrollListBox();

	void SetNewHExtent(LPCTSTR lpszNewString);
	void ResetHExtent();

private:
	int GetTextLen(LPCTSTR lpszText);
};

/////////////////////////////////////////////////////////////////////////////
// CHSListBox

class CHSListBox : 
	public CDialogListBox
{
public:
	CHSListBox()
	{
		m_isActive = FALSE;
	}

	~CHSListBox()
	{
		// In case the parent window is closed before this dialog is...
		//if (m_isActive)
		//	CDialogListBox::DestroyWindow();
	}

	enum { IDD = IDD_HSLISTBOX };

BEGIN_MSG_MAP(CHSListBox)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCLEAR, OnCLEAR)
	COMMAND_HANDLER(IDC_EDIT_PARSE, EN_CHANGE, OnChangeEditParse)
	COMMAND_HANDLER(IDC_EDIT_EVAL, EN_CHANGE, OnChangeEditEval)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_isActive = TRUE;

		// Cache list box HWND
		m_lb.Attach(CDialogListBox::GetDlgItem(IDC_LIST1));
		// Cache edit box HWND
		m_edit_parse.Attach(CDialogListBox::GetDlgItem(IDC_EDIT_PARSE));
		m_edit_eval.Attach(CDialogListBox::GetDlgItem(IDC_EDIT_EVAL));

		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CDialogListBox::DestroyWindow();
		//CDialogListBox::CloseWindow();
		m_isActive = FALSE;
		return 0;
	}

	LRESULT OnCLEAR(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeEditParse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeEditEval(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// TODO - Find out why this doesn't work
	void SetDialogFocus()
	{
		::SendMessage(CDialogListBox::m_hWnd, WM_NEXTDLGCTL, (WPARAM)IDC_EDIT1, TRUE);
	}

	int Puts(LPCTSTR lpszItem);

	BOOL IsActive();

private:
	CHScrollListBox m_lb;
	CEdit m_edit_parse;
	CEdit m_edit_eval;
	BOOL m_isActive;
};

#endif //__HSLISTBOX_H_
