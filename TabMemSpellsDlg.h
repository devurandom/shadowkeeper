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

#if !defined(AFX_TABMEMSPELLSDLG_H__58A083E5_775F_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_TABMEMSPELLSDLG_H__58A083E5_775F_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabMemSpellsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabMemSpellsDlg dialog

struct MEMINFO;
class CTabMemSpellsDlg : public CDialog
{
// Construction
public:
	CTabMemSpellsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabMemSpellsDlg)
	enum { IDD = IDD_MEMORIZATION };
	CListCtrl	m_lcList;
	//}}AFX_DATA

	int  GetSelectedItem()					{ return(m_lcList.GetNextItem(-1,LVNI_SELECTED)); }
	void SetSelectedItem(int nIndex)    { m_lcList.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); }

	void SetData(MEMINFO *pMemInfo, int nCount);
	void GetData(MEMINFO *pMemInfo, int nCount);

	HACCEL m_hAccel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabMemSpellsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabMemSpellsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMaxdec();
	afx_msg void OnMaxinc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABMEMSPELLSDLG_H__58A083E5_775F_11D4_9BE4_444553540000__INCLUDED_)
