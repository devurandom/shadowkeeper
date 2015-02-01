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

#if !defined(AFX_PORTRAITSDLG_H__2E1C67C1_99B9_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_PORTRAITSDLG_H__2E1C67C1_99B9_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortraitsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPortraitsDlg dialog

#ifndef __PORTRAITSDLG__
#define __PORTRAITSDLG__

#include "BitmapWnd.h"

#pragma pack(push,1)
struct BMPTOP
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
};
#pragma pack(pop)

class CPortraitsDlg : public CDialog
{
// Construction
public:
	CPortraitsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPortraitsDlg)
	enum { IDD = IDD_PORTRAITS };
	CListBox	m_lbSmall;
	CButton	m_btnExtractSmall;
	CButton	m_btnExtractLarge;
	CListBox	m_lbLarge;
	//}}AFX_DATA

	void FillLists();

	void GetResourceList(CListBox &lb, char chSize, int nWidth, int nHeight);
	void GetPortraitList(CListBox &lb, char chSize, int nWidth, int nHeight);

	CBitmapWnd m_bwLarge;
	CBitmapWnd m_bwSmall;

	CString m_strLarge;
	CString m_strSmall;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortraitsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPortraitsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLargePortraits();
	afx_msg void OnSelchangeSmallPortraits();
	afx_msg void OnExtractLarge();
	afx_msg void OnExtractSmall();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTRAITSDLG_H__2E1C67C1_99B9_11D4_9BE4_444553540000__INCLUDED_)
