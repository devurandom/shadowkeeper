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

// DaleKeeperView.h : interface of the CDaleKeeperView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DALEKEEPERVIEW_H__8C7CE88F_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
#define AFX_DALEKEEPERVIEW_H__8C7CE88F_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __DALEKEEPERVIEW__
#define __DALEKEEPERVIEW__

#define PORTRAIT_WIDTH			110
#define PORTRAIT_HEIGHT			170

#define NUM_VIEW_TABS			15
#define TAB_ABILITIES			0
#define TAB_CHARACTERISTICS	1
#define TAB_APPEARANCE			2
#define TAB_INVENTORY			3
#define TAB_SPELL_MEM			4
#define TAB_SPELL_INNATE		5
#define TAB_SPELL_WIZARD		6
#define TAB_SPELL_PRIEST		7
#define TAB_SAVES					8
#define TAB_PROFS					9
#define TAB_RESISTANCES			10
#define TAB_THIEF					11
#define TAB_STATE					12
#define TAB_AFFECTS				13
#define TAB_GLOBALS				14

#include "BitmapWnd.h"
#include "TabAbilitiesDlg.h"
#include "TabSavesDlg.h"
#include "CharTabCtrl.h"
#include "TabProfsDlg.h"
#include "TabResistancesDlg.h"
#include "TabThiefSkillsDlg.h"
#include "TabCharacteristicsDlg.h"
#include "TabInvDlg.h"
#include "TabSpellsDlg.h"
#include "TabMemSpellsDlg.h"
#include "TabAffectsDlg.h"
#include "TabGlobalVarsDlg.h"
#include "TabAppearanceDlg.h"
#include "TabStatesDlg.h"

#define DKV_VIEWTYPE_UNKNOWN	0
#define DKV_VIEWTYPE_PARTY		1
#define DKV_VIEWTYPE_OOP		2
#define DKV_VIEWTYPE_CHR		3
#define DKV_VIEWTYPE_CRE		4

class CDaleKeeperView : public CFormView
{
protected: // create from serialization only
	CDaleKeeperView();
	DECLARE_DYNCREATE(CDaleKeeperView)


private:
	// Makes the edit control for the name and the scrollpar line up with
	// the portrait. Also used for first time initialiazation of other controls.
	void	ResizeControls();

	// Creates a bitmap from either the game resources, or if it's a custom
	// picture loads the bitmap from teh Portraits directory.
	void	GetPortrait(int nChar);

	// Loads the character data for the form, including the portrait.
	void	LoadChar(int nChar);

	// Takes the data off the form and saves it into the passed in character
	// position. It returns TRUE if the data was validated and saved. Returns
	// FALSE if one of the tabs fails to validate the data. Only switch to
	// a new character if this returns TRUE.
	BOOL	SaveChar(int nChar);

	// Get the text to display in the Name box on the view. This can come from
	// a number of sources. In Party, Out of Party, CHR or a string reference.
	// m_dwShortName and m_dwLongName must be filled prior to this call.
	void	GetNameText(int nChar);

	CInfCreature* GetCre(int nChar=0);

	// Sets the active tab in the tab control.
	void	DisplayTab(int nIndex);

	void	SetGameTitle();

	void	UpdateGridLines();
	void	UpdateGridLines(CListCtrl &list);

	CTabAbilitiesDlg		m_tabAbilities;
	CTabSavesDlg			m_tabSaves;
	CTabProfsDlg			m_tabProfs;
	CTabResistancesDlg	m_tabResistances;
	CTabThiefSkillsDlg	m_tabThief;
	CTabCharacteristicsDlg m_tabCharacteristics;
	CTabInvDlg				m_tabInv;
	CTabSpellsDlg			m_tabInnate;
	CTabSpellsDlg			m_tabPriest;
	CTabSpellsDlg			m_tabWizard;
	CTabMemSpellsDlg		m_tabMem;
	CTabAffectsDlg			m_tabAffects;
	CTabGlobalVarsDlg		m_tabGlobals;
	CTabAppearanceDlg		m_tabAppearance;
	CTabStatesDlg			m_tabStates;

	// Putting all the modeless dialogs in an array to make showing/hiding
	// them easy.
	CDialog *m_pTabDlg[NUM_VIEW_TABS];

	// Keeps track of which tab is being viewed. This is used in the TCN_SELCHANGE
	// notification to figure out which dialog needs to be hidden since the 
	// control will already have the new tab set as the selection;
	int m_nCurrentTab;

	// Current character being edited.
	int	m_nCurrentChar;

	// Current character string references.
	DWORD m_dwShortName;
	DWORD m_dwLongName;

	// Holds the rect for the portrait. This is bigger than the actual portrait
	// to accomodate the little 3d border.
	CRect		m_rPortrait;
	HBITMAP	m_hPortrait;

	CBitmapWnd	m_wndPortrait;

	CCharTabCtrl	m_tab;

	// There are two types of views this can be. They are defined above. It can
	// be either the main party view, or the out of party view. It defaults to
	// the party view.
	int	m_nViewType;

public:
	//{{AFX_DATA(CDaleKeeperView)
	enum { IDD = IDD_DALEKEEPER_FORM };
	CButton	m_btnNameRef;
	CButton	m_btnPortrait;
	CScrollBar	m_sbParty;
	CEdit	m_edName;
	CString	m_strName;
	//}}AFX_DATA

// Attributes
public:
	CDaleKeeperDoc* GetDocument();

	void SetViewType(int nViewType)				{ m_nViewType = nViewType; }
	int  GetViewType()								{ return(m_nViewType); }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaleKeeperView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDaleKeeperView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDaleKeeperView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCharacterNext();
	afx_msg void OnCharacterPrev();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPageNext();
	afx_msg void OnPagePrev();
	afx_msg void OnUpdatePageNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePagePrev(CCmdUI* pCmdUI);
	afx_msg void OnPageAbilities();
	afx_msg void OnPageSaves();
	afx_msg void OnPageProf();
	afx_msg void OnPageResistances();
	afx_msg void OnPageThief();
	afx_msg void OnPageCharacteristics();
	afx_msg void OnViewInventory();
	afx_msg void OnPortrait();
	afx_msg void OnNameRef();
	afx_msg void OnUpdateToolsConverttocre(CCmdUI* pCmdUI);
	afx_msg void OnToolsConverttocre();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DaleKeeperView.cpp
inline CDaleKeeperDoc* CDaleKeeperView::GetDocument()
   { return (CDaleKeeperDoc*)m_pDocument; }
#endif

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DALEKEEPERVIEW_H__8C7CE88F_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
