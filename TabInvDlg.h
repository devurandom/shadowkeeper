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

#if !defined(AFX_TABINVDLG_H__FF74B882_6D14_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_TABINVDLG_H__FF74B882_6D14_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabInvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabInvDlg dialog

#ifndef __TABINVDLG__
#define __TABINVDLG__

#include "InfCreature.h"
#include "InfBifFile.h"
#include "ItemBrowserCom.h"

#define INV_COUNT			16

class CTabInvDlg : public CDialog
{
// Construction
public:
	CTabInvDlg(CWnd* pParent = NULL);   // standard constructor
	~CTabInvDlg();

	void SetItems(INF_CRE_ITEM *pItems);
	void GetItems(INF_CRE_ITEM *pItems);

	int  GetSelectedItem()					{ return(m_lcInv.GetNextItem(-1,LVNI_SELECTED)); }
	void SetSelectedItem(int nIndex)    { m_lcInv.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); }
	void UpdateButtonStates();

	void SetParentView(CView *pView)		{ m_pParentView = pView; }
	void AssignItem(CHintItem *pHi);

	// Needs to be kept up to date by the view. Used for communicating with the
	// item browser.
	CString	m_strGameName;
	CString	m_strCharName;

private:
	void ClearItems();

	HACCEL m_hAccel;

	// Created in the dialog. It's just an all white 32bit DIB.
	HBITMAP	m_hNoIcon;

	CImageList m_ilIcons;

	// This needs to be set by a call to SetParentView. This controls interaction
	// with the Item Browser;
	CView				*m_pParentView;

	INF_CRE_ITEM	m_infItems[INF_NUM_ITEMSLOTS];
	INF_ITM			m_infItm[INF_NUM_ITEMSLOTS];

public:
// Dialog Data
	//{{AFX_DATA(CTabInvDlg)
	enum { IDD = IDD_INVENTORY };
	CButton	m_btnChange;
	CButton	m_btnQty;
	CButton	m_btnClear;
	CListCtrl	m_lcInv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabInvDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabInvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedInventory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClear();
	afx_msg void OnSetqty();
	afx_msg void OnSetallqty();
	afx_msg void OnDblclkInventory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeitem();
	afx_msg void OnIdall();
	afx_msg void OnSetfocusInventory(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABINVDLG_H__FF74B882_6D14_11D4_9BE4_444553540000__INCLUDED_)
