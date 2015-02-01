/************************************************************************************
	Copyright (c) 2000 Aaron O'Neil
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

		1) Redistributions of source code must retain the above copyright notice, 
				this list of conditions and the following disclaimer.
		2) Redistributions in binary form must reproduce the above copyright notice, 
				this list of conditions and the following disclaimer in the documentation
				and/or other materials provided with the distribution.
		3) Redistributions in binary form must reproduce the above copyright notice on
				program startup. Additional credits for program modification are acceptable
				but original copyright and credits must be visible at startup.
		4) You may charge a reasonable copying fee for any distribution of Shadow Keeper. 
				You may charge any fee you choose for support of Shadow Keeper. You may not 
				charge a fee for Shadow Keeper itself.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**************************************************************************************/


#if !defined(AFX_STRINGFINDERDLG_H__34C33376_FDE1_442C_BC0F_59D09AEDBF02__INCLUDED_)
#define AFX_STRINGFINDERDLG_H__34C33376_FDE1_442C_BC0F_59D09AEDBF02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StringFinderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStringFinderDlg dialog

class CStringFinderDlg : public CDialog
{
// Construction
public:
	CStringFinderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStringFinderDlg)
	enum { IDD = IDD_STRING_FINDER };
	CStatic	m_stNumFound;
	CEdit	m_edTextMatch;
	CEdit	m_edMaxLength;
	CListCtrl	m_lcStrings;
	BOOL	m_bAlphaNum;
	//}}AFX_DATA

	DWORD m_dwInitialMaxLength;
	DWORD m_dwResultRef;

	int GetSelectedItem()				{ return(m_lcStrings.GetNextItem(-1,LVNI_SELECTED)); }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringFinderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStringFinderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFindStrings();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDblclkStrings(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGFINDERDLG_H__34C33376_FDE1_442C_BC0F_59D09AEDBF02__INCLUDED_)
