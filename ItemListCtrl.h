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

#if !defined(AFX_ITEMLISTCTRL_H__B50EF827_66C3_11D4_9BE4_955A1E74BB0D__INCLUDED_)
#define AFX_ITEMLISTCTRL_H__B50EF827_66C3_11D4_9BE4_955A1E74BB0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemListCtrl window

#ifndef __ITEMLISTCTRL__
#define __ITEMLISTCTRL__

#include "InfKey.h"
#include "BitmapWnd.h"

// Keep these in column display order, two entries per column. I'm using some
// math to figure out the sorting.
#define ITEMSORT_CAT_ASC			0
#define ITEMSORT_CAT_DES			1
#define ITEMSORT_NAM_ASC			2
#define ITEMSORT_NAM_DES			3
#define ITEMSORT_RES_ASC			4
#define ITEMSORT_RES_DES			5

class CListCtrlItemData : public CObject
{
public:
	CResInfo *pInfo;
	CString	strName;
	int		nItemIndex;
};

class CItemListCtrl : public CListCtrl
{
// Construction
public:
	CItemListCtrl();

	void	Init(CWnd *pDescriptionWnd, CWnd *pCountWnd, CBitmapWnd *pBitmapWnd);
	int	GetSelectedItem();
	void	SetSelectedItem(int nIndex);

	// Called by Init the first time. If any filters are changed after that
	// this needs to get called to refresh it.
	void FillList();

	// Call to make the list display a specific item by resource string.
	void DisplayItem(const char *pszResource);

	// Call to inform the saved game view to assign an item.
	void AssignItem();

	// Filter flags.
	BOOL	m_bArmor;
	BOOL	m_bArrows;
	BOOL	m_bAxes;
	BOOL	m_bBelts;
	BOOL	m_bBolts;
	BOOL	m_bBooks;
	BOOL	m_bBoots;
	BOOL	m_bBows;
	BOOL	m_bBullets;
	BOOL	m_bCloaks;
	BOOL	m_bContainers;
	BOOL	m_bCrossbows;
	BOOL	m_bDaggers;
	BOOL	m_bDarts;
	BOOL	m_bFamiliars;
	BOOL	m_bFlails;
	BOOL	m_bFood;
	BOOL	m_bGems;
	BOOL	m_bGloves;
	BOOL	m_bHalberds;
	BOOL	m_bHammers;
	BOOL	m_bHandToHand;
	BOOL	m_bHelms;
	BOOL	m_bKeys;
	BOOL	m_bMaces;
	BOOL	m_bMisc;
	BOOL	m_bMorningStars;
	BOOL	m_bNecklaces;
	BOOL	m_bPotions;
	BOOL	m_bRings;
	BOOL	m_bScrolls;
	BOOL	m_bShields;
	BOOL	m_bSlings;
	BOOL	m_bSpears;
	BOOL	m_bStaves;
	BOOL	m_bShortSwords;
	BOOL	m_bSwords;
	BOOL	m_bWands;

	BOOL	m_bMatchCase;
	BOOL	m_bMatchDescription;
	BOOL	m_bMatchName;
	CString	m_strSearchText;

private:
	void ClearList();
	
	void UpdateItemChanged(int nIndex);
	void UpdateDescription(INF_ITM &itm);
	void UpdateIcon(CResInfo *pInfo,INF_ITM &itm);

	// Just a reference to another list. Don't delete this object.
	CObList		*m_pItems;

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

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CItemListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CItemListCtrl)
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

#endif // !defined(AFX_ITEMLISTCTRL_H__B50EF827_66C3_11D4_9BE4_955A1E74BB0D__INCLUDED_)
