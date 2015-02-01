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

#if !defined(AFX_SPELLBROWSERVIEW_H__6E905D81_704B_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_SPELLBROWSERVIEW_H__6E905D81_704B_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpellBrowserView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpellBrowserView form view

#ifndef __SPELLBROWSERVIEW__
#define __SEPLLBROWSERVIEW__

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "BitmapWnd.h"
#include "SpellListCtrl.h"

#define SBV_MARGIN_RIGHT		90

class CSpellBrowserView : public CFormView
{
protected:
	CSpellBrowserView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSpellBrowserView)

	CBitmapWnd	m_wndBitmap;
	CSpellListCtrl	m_lcSpells;

	void UpdateAssignButton();
	void UpdateGridLines();

// Form Data
public:
	//{{AFX_DATA(CSpellBrowserView)
	enum { IDD = IDD_SPELL_BROWSER };
	CButton	m_btnFilter;
	CStatic	m_stSpellPrompt;
	CButton	m_btnExportList;
	CButton	m_btnExportSpell;
	CButton	m_btnAssign;
	CStatic	m_stSpellCount;
	CEdit	m_edDescription;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpellBrowserView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSpellBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSpellBrowserView)
	afx_msg void OnAssignspell();
	afx_msg void OnExportspells();
	afx_msg void OnExportspellres();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFilterspells();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPELLBROWSERVIEW_H__6E905D81_704B_11D4_9BE4_444553540000__INCLUDED_)
