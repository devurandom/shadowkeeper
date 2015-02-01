#if !defined(AFX_CREATURELISTCTRL_H__0516BD5C_C632_48B3_AFB5_AA46DE36034D__INCLUDED_)
#define AFX_CREATURELISTCTRL_H__0516BD5C_C632_48B3_AFB5_AA46DE36034D__INCLUDED_

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
// CreatureListCtrl.h : header file
//

#ifndef __CREATURELISTCTRL__
#define __CREATURELISTCTRL__

#define CRESORT_ASC			0
#define CRESORT_DES			1

/////////////////////////////////////////////////////////////////////////////
// CCreatureListCtrl window

#include "InfKey.h"

// Used for sorting.
class CListCtrlCreData : public CObject
{
public:
	CString	strName;
	CResInfo	*pResInfo;
};

class CCreatureListCtrl : public CListCtrl
{
// Construction
public:
	CCreatureListCtrl();

	BOOL Init(CStatic *pCreatureCount);

	void	EditSelection();
	void  ExtractSelection();
	void  ExtractSelectionAs();
	void  DeleteSelection();

	// Called from externally when the override directory has been
	// changed.
	void	OverrideChanged();

	int	GetSelectedItem();
	void	SetSelectedItem(int nIndex);

	void	FillList();
	void	ClearList();

	CString m_strSearchText;
	BOOL	m_bMatchCase;

private:
	void	Sort();

	BOOL	ExtractCre(int nItem, BOOL bSaveAs);
	int	AddItem(CResInfo *pResInfo);
	void	UpdateItem(const char *pszFilename);

	int	m_nSortCol;
	int	m_nSortDir;

	CStatic	*m_pCount;
	
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreatureListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCreatureListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCreatureListCtrl)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // __CREATURELISTCTRL__

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATURELISTCTRL_H__0516BD5C_C632_48B3_AFB5_AA46DE36034D__INCLUDED_)
