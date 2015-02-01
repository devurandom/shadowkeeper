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

#if !defined(AFX_EDITAFFECTDLG_H__DA903493_F013_4CC0_9289_18242922217D__INCLUDED_)
#define AFX_EDITAFFECTDLG_H__DA903493_F013_4CC0_9289_18242922217D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditAffectDlg.h : header file
//

#ifndef __EDITAFFECTDLG__
#define __EDITAFFECTDLG__

#include "InfCreature.h"

#define STRUCTDEF_TYPE_NONE		0
#define STRUCTDEF_TYPE_BYTE		1
#define STRUCTDEF_TYPE_INT			2
#define STRUCTDEF_TYPE_DWORD		3
#define STRUCTDEF_TYPE_CHAR		4
#define STRUCTDEF_TYPE_WORD		5

struct STRUCTDEF
{
	int	nOffset;
	char  *pszName;
	int	nType;
	int	nLen;
	int	nPadding;			// Only for hex values, for padding zeroes.
};

extern STRUCTDEF _sdCreAff[];

/////////////////////////////////////////////////////////////////////////////
// CEditAffectDlg dialog

class CEditAffectDlg : public CDialog
{
// Construction
public:
	CEditAffectDlg(CWnd* pParent = NULL);   // standard constructor

	int  GetSelectedItem()					{ return(m_lcList.GetNextItem(-1,LVNI_SELECTED)); }
	void SetSelectedItem(int nIndex)    { m_lcList.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); }

	void SetAffect(INF_AFF *pAff);
	void GetAffect(INF_AFF *pAff);

private:
	INF_AFF	m_aff;

	void LoadValues();
	
	BOOL GetValueString(const STRUCTDEF &sd, char *pszResult);
	BOOL GetDataType(const STRUCTDEF &sd, char *pszResult);
	BOOL AssignData(const STRUCTDEF &sd, const char *pszValue);

	void UpdateList(CEdit &edit, int nIndex, DWORD &dwValue);
	void UpdateList(CEdit &edit, int nIndex, int &dwValue);
	void UpdateList(CEdit &edit, int nIndex, WORD &wValue);
	void UpdateList(CEdit &edit, int nIndex, char *pchValue);	

public:
// Dialog Data
	//{{AFX_DATA(CEditAffectDlg)
	enum { IDD = IDD_AFFECT_EDIT };
	CEdit	m_edType;
	CEdit	m_edResource2;
	CEdit	m_edProbability;
	CEdit	m_edParam2;
	CEdit	m_edParam1;
	CEdit	m_edFlags;
	CEdit	m_edResource0;
	CEdit	m_edResource1;
	CEdit	m_edResource3;
	CEdit	m_edTarget;
	CEdit	m_edTime;
	CListCtrl	m_lcList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditAffectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditAffectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeAffFlags();
	afx_msg void OnChangeAffParam1();
	afx_msg void OnChangeAffParam2();
	afx_msg void OnChangeAffType();
	afx_msg void OnChangeAffTarget();
	afx_msg void OnChangeAffTime();
	afx_msg void OnChangeAffProbability();
	afx_msg void OnChangeAffResource0();
	afx_msg void OnChangeAffResource1();
	afx_msg void OnChangeAffResource2();
	afx_msg void OnChangeAffResource3();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITAFFECTDLG_H__DA903493_F013_4CC0_9289_18242922217D__INCLUDED_)
