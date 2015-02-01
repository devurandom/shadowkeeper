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

#if !defined(AFX_TABAFFECTSDLG_H__BA272ED9_65E7_41B0_8DD6_526E26AF0680__INCLUDED_)
#define AFX_TABAFFECTSDLG_H__BA272ED9_65E7_41B0_8DD6_526E26AF0680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabAffectsDlg.h : header file
//

#ifndef __TABAFFECTSDLG__
#define __TABAFFECTSDLG__

#include "InfCreature.h"

/////////////////////////////////////////////////////////////////////////////
// CTabAffectsDlg dialog

class CTabAffectsDlg : public CDialog
{
// Construction
public:
	CTabAffectsDlg(CWnd* pParent = NULL);   // standard constructor

	int  GetSelectedItem()					{ return(m_lcList.GetNextItem(-1,LVNI_SELECTED)); }
	void SetSelectedItem(int nIndex)    { m_lcList.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); }

	// Adds the Affects to the list control. The points are stored in the list
	// and the list cleared.
	void SetAffects(CPtrList &list);

	// Creats a list of INF_AFF objects. These are new objects not the actual
	// data attached to the list, so freeing them up is ok.
	void GetAffects(CPtrList &list);

private:
	HACCEL m_hAccel;

	void ClearList();

	int AddToList(INF_AFF *pAff, int nIndex=-1);

	void ScriptName(const char *pszResName, CString &strText);

public:

// Dialog Data
	//{{AFX_DATA(CTabAffectsDlg)
	enum { IDD = IDD_AFFECTS };
	CListCtrl	m_lcList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabAffectsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabAffectsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnAffDuplicate();
	afx_msg void OnAffRemove();
	afx_msg void OnAffCopy();
	afx_msg void OnAffPaste();
	afx_msg void OnAffEdit();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABAFFECTSDLG_H__BA272ED9_65E7_41B0_8DD6_526E26AF0680__INCLUDED_)
