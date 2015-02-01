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

#if !defined(AFX_SPELLLISTCTRL_H__6E905D84_704B_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_SPELLLISTCTRL_H__6E905D84_704B_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpellListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpellListCtrl window

#ifndef __SEPLLLISTCTRL__
#define __SPELLLISTCTRL__

#include "InfKey.h"
#include "BitmapWnd.h"

// Keep these in column display order, two entries per column. I'm using some
// math to figure out the sorting.
#define SPELLSORT_TYP_ASC			0
#define SPELLSORT_TYP_DES			1
#define SPELLSORT_LVL_ASC			2
#define SPELLSORT_LVL_DES			3
#define SPELLSORT_NAM_ASC			4
#define SPELLSORT_NAM_DES			5
#define SPELLSORT_RES_ASC			6
#define SPELLSORT_RES_DES			7
#define SPELLSORT_SNM_ASC			8
#define SPELLSORT_SNM_DES			9

class CListCtrlSpellData : public CObject
{
public:
	CResInfo *pInfo;
	CString	strName;
	CString	strScriptName;
	BYTE		chType;
	BYTE		chLevel;
};

class CSpellListCtrl : public CListCtrl
{
// Construction
public:
	CSpellListCtrl();

	void	Init(CWnd *pDescriptionWnd, CWnd *pCountWnd, CBitmapWnd *pBitmapWnd);
	int	GetSelectedItem();
	void	SetSelectedItem(int nIndex);

	// Called by Init the first time. If any filters are changed after that
	// this needs to get called to refresh it.
	void FillList();

	// Call to make the list display a specific spell by resource string.
	void DisplaySpell(const char *pszResource);

	// Call to inform the saved game view to assign an spell.
	void AssignSpell();

	BOOL	m_bMatchCase;
	BOOL	m_bMatchDescription;
	BOOL	m_bMatchName;
	CString	m_strSearchText;

private:
	void ClearList();
	
	void UpdateSpellChanged(int nIndex);
	void UpdateDescription(INF_SPL &spl);
	void UpdateIcon(CResInfo *pInfo,INF_SPL &spl);

	// Just a reference to another list. Don't delete this object.
	CObList		*m_pSpells;

	// Windows that get updated by this control but belong to the parent.
	CWnd			*m_pDescriptionWnd;
	CWnd			*m_pCountWnd;
	CBitmapWnd	*m_pBitmapWnd;

	// Set to TRUE while filling the item list. Used to prevent it form
	// looking up descriptions on items as they are added to the list.
	BOOL	m_bLoading;

	// Sort style current in effect.
	int	m_nSort;

	// Handle to the item's bitmap.
	HBITMAP			m_hBitmap;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpellListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpellListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpellListCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPELLLISTCTRL_H__6E905D84_704B_11D4_9BE4_444553540000__INCLUDED_)
