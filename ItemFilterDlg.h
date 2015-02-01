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

#if !defined(AFX_ITEMFILTERDLG_H__241D26A1_93DE_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_ITEMFILTERDLG_H__241D26A1_93DE_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemFilterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemFilterDlg dialog

class CItemFilterDlg : public CDialog
{
// Construction
public:
	CItemFilterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CItemFilterDlg)
	enum { IDD = IDD_ITEMFILTER };
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
	BOOL	m_bFlails;
	BOOL	m_bMatchCase;
	BOOL	m_bMatchDescription;
	BOOL	m_bMatchName;
	CString	m_strSearchText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemFilterDlg)
	afx_msg void OnDeselAll();
	afx_msg void OnSelAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMFILTERDLG_H__241D26A1_93DE_11D4_9BE4_444553540000__INCLUDED_)
