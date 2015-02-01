#if !defined(AFX_OPENCREDLG_H__CFF59146_BD39_4D70_A08F_FC1C93E3642F__INCLUDED_)
#define AFX_OPENCREDLG_H__CFF59146_BD39_4D70_A08F_FC1C93E3642F__INCLUDED_

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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenCreDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenCreDlg dialog

#include "BitmapWnd.h"

#define SORT_CRE_FILE_ASC	0
#define SORT_CRE_FILE_DES	1
#define SORT_CRE_NAME_ASC	2
#define SORT_CRE_NAME_DES	3

struct OPENCREDATA
{
	CString strFile;
	CString strName;
	char	  szPortrait[9];
};

class COpenCreDlg : public CDialog
{
// Construction
public:
	COpenCreDlg(CWnd* pParent = NULL);   // standard constructor
	~COpenCreDlg();

	void FillCharacters();
	void ClearList();

	int		m_nSort;
	CString	m_strPath;

	CBitmapWnd	m_wndPortrait;
	HBITMAP	m_hBitmap;

// Dialog Data
	//{{AFX_DATA(COpenCreDlg)
	enum { IDD = IDD_OPEN_CRE };
	CListCtrl	m_lcList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenCreDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenCreDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnColumnclickCreList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCreList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnItemchangedCreList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENCREDLG_H__CFF59146_BD39_4D70_A08F_FC1C93E3642F__INCLUDED_)
