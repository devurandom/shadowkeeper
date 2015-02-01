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

// AddAbilitiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dalekeeper.h"
#include "AddAbilitiesDlg.h"
#include "Inf2DA.h"
#include "InfTlk.h"
#include "InfKey.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddAbilitiesDlg dialog


CAddAbilitiesDlg::CAddAbilitiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddAbilitiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddAbilitiesDlg)
	m_bRemoveKit = FALSE;
	//}}AFX_DATA_INIT
}


void CAddAbilitiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddAbilitiesDlg)
	DDX_Control(pDX, IDC_TOLEVEL, m_cbTo);
	DDX_Control(pDX, IDC_FROMLEVEL, m_cbFrom);
	DDX_Control(pDX, IDC_KITTYPE, m_cbKit);
	DDX_Check(pDX, IDC_REMOVE_KIT, m_bRemoveKit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddAbilitiesDlg, CDialog)
	//{{AFX_MSG_MAP(CAddAbilitiesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddAbilitiesDlg message handlers

BOOL CAddAbilitiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	FillKits();
	FillLevels();

	return TRUE;  
}

void CAddAbilitiesDlg::FillLevels()
{
	m_cbFrom.ResetContent();
	m_cbTo.ResetContent();

	char szBuf[50];
	for (int i=0;i<100;i++)
	{
		sprintf(szBuf,"%03d",i+1);
		m_cbFrom.AddString(szBuf);
		m_cbTo.AddString(szBuf);
	}

	m_cbFrom.SelectString(-1,"001");
	m_cbTo.SelectString(-1,"100");
}

void CAddAbilitiesDlg::FillKits()
{
	m_cbKit.ResetContent();

	CString	strName;
	CString	strAbility;
	DWORD		dwStrRef;
	int		nKitIndex;
	int		nItem;
	int nRows = _2daKitList.GetRows();

	for (int i=0;i<nRows;i++)
	{
		_2daKitList.GetData(i,KITLIST_TITLE,dwStrRef);
		if (!dwStrRef || dwStrRef > _infTlk.GetStringCount())
			continue;

		// Just in case the string reference is bad I'm using the kit name
		// givin in the 2da file (it will be ugly all caps with underscores).
		if (!_infTlk.GetString(dwStrRef,strName))
			_2daKitList.GetData(i,KITLIST_NAME,strName);

		// Only add items where there is an actual definition of the abilities.
		// Some of the kits listed have an ability name that doesn't actually map
		// to a 2da resource.
		if (!_2daKitList.GetData(i,KITLIST_ABILITY,strAbility))
			continue;
		if (!_infKey.GetResInfo(RESTYPE_2DA,strAbility))
			continue;

		// Stuffing this in the item data as the reference to which kit was
		// selected.
		if (!_2daKitList.GetData(i,KITLIST_INDEX,nKitIndex))
			continue;

		nItem = m_cbKit.AddString(strName);
		m_cbKit.SetItemData(nItem,nKitIndex);
	}

	m_cbKit.SetCurSel(0);
}


void CAddAbilitiesDlg::OnOK() 
{
	UpdateData(TRUE);

	int nIndex = m_cbKit.GetCurSel();
	if (nIndex == -1)
	{
		AfxMessageBox("You must select a kit first.");
		return;
	}
	m_nKitIndex = m_cbKit.GetItemData(nIndex);

	CString strText;
	nIndex = m_cbFrom.GetCurSel();
	if (nIndex == -1)
	{
		AfxMessageBox("You must first select a starting level.");
		return;
	}
	m_cbFrom.GetLBText(nIndex,strText);
	m_nLevelFrom = atoi(strText);

	nIndex = m_cbTo.GetCurSel();
	if (nIndex == -1)
	{
		AfxMessageBox("You must first select an ending level.");
		return;
	}
	m_cbTo.GetLBText(nIndex,strText);
	m_nLevelTo = atoi(strText);

	if (m_nLevelFrom > m_nLevelTo)
	{
		AfxMessageBox("The starting level must be greater than or equal to the ending level.");
		return;
	}

	EndDialog(IDOK);
}
