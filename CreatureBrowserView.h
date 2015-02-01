#if !defined(AFX_CREATUREBROWSERVIEW_H__2A89408B_D244_481D_8DFE_953697F9B818__INCLUDED_)
#define AFX_CREATUREBROWSERVIEW_H__2A89408B_D244_481D_8DFE_953697F9B818__INCLUDED_

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
// CreatureBrowserView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreatureBrowserView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "CreatureListCtrl.h"

class CCreatureBrowserView : public CFormView
{
protected:
	CCreatureBrowserView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCreatureBrowserView)

public:
	CCreatureListCtrl m_lcCreatures;

private:
	void UpdateGridLines();

// Form Data
public:
	//{{AFX_DATA(CCreatureBrowserView)
	enum { IDD = IDD_CREATURE_BROWSER };
	CButton	m_btnFilter;
	CButton	m_btnDelete;
	CButton	m_btnRefresh;
	CButton	m_btnExtractAs;
	CButton	m_btnExtract;
	CButton	m_btnEdit;
	CStatic	m_stCreaturePrompt;
	CStatic	m_stCreatureCount;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreatureBrowserView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCreatureBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCreatureBrowserView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCreature();
	afx_msg void OnRefreshCre();
	afx_msg void OnExtractcreAs();
	afx_msg void OnExtractCreature();
	afx_msg void OnDeleteCre();
	afx_msg void OnFilterCreatures();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATUREBROWSERVIEW_H__2A89408B_D244_481D_8DFE_953697F9B818__INCLUDED_)
