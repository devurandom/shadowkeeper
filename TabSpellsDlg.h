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

#if !defined(AFX_TABSPELLSDLG_H__C91A59A1_76C1_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_TABSPELLSDLG_H__C91A59A1_76C1_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabSpellsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabSpellsDlg dialog

#ifndef __TABSPELLSDLG__
#define __TABSPELLSDLG__

#include "InfCreature.h"
#include "SpellBrowserCom.h"

// Keep these in column display order, two entries per column. I'm using some
// math to figure out the sorting.
#define TABSPELLSORT_ICO_ASC			0	// Not actually sorting on thse.
#define TABSPELLSORT_ICO_DES			1
#define TABSPELLSORT_LVL_ASC			2
#define TABSPELLSORT_LVL_DES			3
#define TABSPELLSORT_MEM_ASC			4
#define TABSPELLSORT_MEM_DES			5
#define TABSPELLSORT_NAM_ASC			6
#define TABSPELLSORT_NAM_DES			7
#define TABSPELLSORT_RES_ASC			8
#define TABSPELLSORT_RES_DES			9

#define TABSPELL_MAXMEM					50

// Most of this is just for sorting of the list.
class CResInfo;
class CTabSpellData : public CObject
{
public:
	CResInfo *pInfo;
	CString	strName;
	BYTE		chMem;
	BYTE		chLevel;

	// Stored so the original data can be reconstructed.
	BYTE		chTimesCanCast;
};

class CTabSpellsDlg : public CDialog
{
// Construction
public:
	CTabSpellsDlg(CWnd* pParent = NULL);   // standard constructor
	~CTabSpellsDlg();

// Dialog Data
	//{{AFX_DATA(CTabSpellsDlg)
	enum { IDD = IDD_SPELLS };
	CButton	m_btnInnate;
	CButton	m_btnInc;
	CButton	m_btnDec;
	CButton	m_btnClear;
	CButton	m_btnChange;
	CListCtrl	m_lcSpells;
	//}}AFX_DATA

	int  GetSelectedItem()					{ return(m_lcSpells.GetNextItem(-1,LVNI_SELECTED)); }
	void SetSelectedItem(int nIndex)    { m_lcSpells.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); }

	// Sets the spells from a character. wType needs to be one of the INC_CRE_ST defines found
	// in InfCre.h.
	void SetSpells(WORD wType, int nSpellCount, SPELLDATA *pData);

	int  GetSpellCount()						{ return(m_lcSpells.GetItemCount()); }

	// pData must point to an array big enough to hold as many entries as GetSpellCount()
	// returns.
	void GetSpells(SPELLDATA *pData);

	void SetParentView(CView *pView)		{ m_pParentView = pView; }
	void AssignSpell(CHintSpell *pHs);

	// Set to TRUE when the dialog represents the Innate Abilities. This causes the
	// KIT button to show up.
	void SetInnate(BOOL bInnate=TRUE)	{ m_bInnate = bInnate; }

	// Needs to be kept up to date by the view. Used for communicating with the
	// item browser.
	CString	m_strGameName;
	CString	m_strCharName;

private:
	void	ClearList();
	void	UpdateButtonStates();

	// Returns TRUE if the spell is found and set the list index and the pointer to
	// the spell data.
	BOOL	GetSpellByResource(const char *pszResName, int &nIndex, CTabSpellData *&pData);

	// Adds a spell to the list. Returns TRUE is successful.
	BOOL	AddSpellToList(SPELLDATA *pData);

	HACCEL m_hAccel;

	WORD		m_wSpellType;

	// This needs to be set with a call to SetParentView. It controls interaction
	// between the spell browser and the view.
	CView		*m_pParentView;

	// Sort style current in effect.
	int	m_nSort;

	// TRUE when showing innates.
	BOOL	m_bInnate;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSpellsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabSpellsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedSpellList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClearSpell();
	afx_msg void OnChangespell();
	afx_msg void OnDblclkSpellList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclickSpellList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusSpellList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMemInc();
	afx_msg void OnMemDec();
	afx_msg void OnKitInnates();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSPELLSDLG_H__C91A59A1_76C1_11D4_9BE4_444553540000__INCLUDED_)
