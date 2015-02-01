// TabAbilitiesDlg.cpp : implementation file
//

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

#include "stdafx.h"
#include "DaleKeeper.h"
#include "TabAbilitiesDlg.h"
#include "ValueList.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabAbilitiesDlg dialog


CTabAbilitiesDlg::CTabAbilitiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabAbilitiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabAbilitiesDlg)
	m_nCha = 3;
	m_nCon = 3;
	m_nDex = 3;
	m_nInt = 3;
	m_nStr = 3;
	m_nStrBonus = 3;
	m_nWis = 3;
	m_nExp = 0;
	m_nGold = 0;
	m_nHP = 1;
	m_nAC = 0;
	m_nThac0 = 0;
	m_nLevelFirstClass = 0;
	m_nLevelSecondClass = 0;
	m_nLevelThirdClass = 0;
	m_nReputation = 0;
	m_nExpForKill = 0;
	m_nCurrentHP = 0;
	//}}AFX_DATA_INIT

	m_hAccel = NULL;
}


void CTabAbilitiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabAbilitiesDlg)
	DDX_Control(pDX, IDC_ATTACKS, m_cbAttacks);
	DDX_Control(pDX, IDC_STR, m_edStr);
	DDX_Text(pDX, IDC_CHA, m_nCha);
	DDV_MinMaxUInt(pDX, m_nCha, 1, 250);
	DDX_Text(pDX, IDC_CON, m_nCon);
	DDV_MinMaxUInt(pDX, m_nCon, 1, 250);
	DDX_Text(pDX, IDC_DEX, m_nDex);
	DDV_MinMaxUInt(pDX, m_nDex, 1, 250);
	DDX_Text(pDX, IDC_INT, m_nInt);
	DDV_MinMaxUInt(pDX, m_nInt, 1, 250);
	DDX_Text(pDX, IDC_STR, m_nStr);
	DDV_MinMaxUInt(pDX, m_nStr, 1, 250);
	DDX_Text(pDX, IDC_STR_PLUS, m_nStrBonus);
	DDV_MinMaxUInt(pDX, m_nStrBonus, 0, 255);
	DDX_Text(pDX, IDC_WIS, m_nWis);
	DDV_MinMaxUInt(pDX, m_nWis, 1, 250);
	DDX_Text(pDX, IDC_EXP, m_nExp);
	DDV_MinMaxUInt(pDX, m_nExp, 0, 4000000000);
	DDX_Text(pDX, IDC_GOLD, m_nGold);
	DDV_MinMaxUInt(pDX, m_nGold, 0, 1000000000);
	DDX_Text(pDX, IDC_BASEHP, m_nHP);
	DDV_MinMaxUInt(pDX, m_nHP, 1, 65000);
	DDX_Text(pDX, IDC_BASEAC, m_nAC);
	DDV_MinMaxInt(pDX, m_nAC, -32000, 32000);
	DDX_Text(pDX, IDC_THAC0, m_nThac0);
	DDV_MinMaxInt(pDX, m_nThac0, -128, 127);
	DDX_Text(pDX, IDC_LEVEL1, m_nLevelFirstClass);
	DDV_MinMaxUInt(pDX, m_nLevelFirstClass, 0, 255);
	DDX_Text(pDX, IDC_LEVEL2, m_nLevelSecondClass);
	DDV_MinMaxUInt(pDX, m_nLevelSecondClass, 0, 255);
	DDX_Text(pDX, IDC_LEVEL3, m_nLevelThirdClass);
	DDV_MinMaxUInt(pDX, m_nLevelThirdClass, 0, 255);
	DDX_Text(pDX, IDC_REPUTATION, m_nReputation);
	DDV_MinMaxUInt(pDX, m_nReputation, 0, 255);
	DDX_Text(pDX, IDC_EXP_FORKILL, m_nExpForKill);
	DDV_MinMaxUInt(pDX, m_nExpForKill, 0, 4000000000);
	DDX_Text(pDX, IDC_CURRENTHP, m_nCurrentHP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabAbilitiesDlg, CDialog)
	//{{AFX_MSG_MAP(CTabAbilitiesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabAbilitiesDlg message handlers

BOOL CTabAbilitiesDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabAbilitiesDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// If ENTER or ESC is hit on any of the controls on a dialog, they send the messages
	// back to the parent dialog. This causes the dialog to close itself down if the
	// IDOK and IDCANCEL messages are not mapped. I was putting the OK and Cancel buttons
	// on the dialogs and hiding them, then mapping the messages and doing nothing, but
	// this is a much easier solution.
	if (wParam == IDOK || wParam == IDCANCEL)
		return(TRUE);
	
	return CDialog::OnCommand(wParam, lParam);
}

BOOL CTabAbilitiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	_vlNumAttacks.FillComboBox(m_cbAttacks);

	return TRUE; 
}

void CTabAbilitiesDlg::SetAttacks(BYTE chAttacks)
{
	if (!CValueList::SetComboBoxValue(m_cbAttacks,chAttacks,FALSE))
		CValueList::SetComboBoxValue(m_cbAttacks,1,FALSE);
}

BYTE CTabAbilitiesDlg::GetAttacks()
{
	return((BYTE)CValueList::GetComboBoxValue(m_cbAttacks));
}

