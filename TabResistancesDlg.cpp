// TabResistancesDlg.cpp : implementation file
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
#include "dalekeeper.h"
#include "TabResistancesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabResistancesDlg dialog


CTabResistancesDlg::CTabResistancesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabResistancesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabResistancesDlg)
	m_nAcid = 0;
	m_nCold = 0;
	m_nCrushing = 0;
	m_nElectricity = 0;
	m_nFire = 0;
	m_nMagic = 0;
	m_nMagicCold = 0;
	m_nMagicFire = 0;
	m_nMissile = 0;
	m_nPiercing = 0;
	m_nSlashing = 0;
	//}}AFX_DATA_INIT

	m_hAccel = NULL;
}


void CTabResistancesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabResistancesDlg)
	DDX_Control(pDX, IDC_RES_ACID, m_edAcid);
	DDX_Text(pDX, IDC_RES_ACID, m_nAcid);
	DDV_MinMaxUInt(pDX, m_nAcid, 0, 255);
	DDX_Text(pDX, IDC_RES_COLD, m_nCold);
	DDV_MinMaxUInt(pDX, m_nCold, 0, 255);
	DDX_Text(pDX, IDC_RES_CRUSHING, m_nCrushing);
	DDV_MinMaxUInt(pDX, m_nCrushing, 0, 255);
	DDX_Text(pDX, IDC_RES_ELECTRICITY, m_nElectricity);
	DDV_MinMaxUInt(pDX, m_nElectricity, 0, 255);
	DDX_Text(pDX, IDC_RES_FIRE, m_nFire);
	DDV_MinMaxUInt(pDX, m_nFire, 0, 255);
	DDX_Text(pDX, IDC_RES_MAGIC, m_nMagic);
	DDV_MinMaxUInt(pDX, m_nMagic, 0, 255);
	DDX_Text(pDX, IDC_RES_MAGICCOLD, m_nMagicCold);
	DDV_MinMaxUInt(pDX, m_nMagicCold, 0, 255);
	DDX_Text(pDX, IDC_RES_MAGICFIRE, m_nMagicFire);
	DDV_MinMaxUInt(pDX, m_nMagicFire, 0, 255);
	DDX_Text(pDX, IDC_RES_MISSILE, m_nMissile);
	DDV_MinMaxUInt(pDX, m_nMissile, 0, 255);
	DDX_Text(pDX, IDC_RES_PIERCING, m_nPiercing);
	DDV_MinMaxUInt(pDX, m_nPiercing, 0, 255);
	DDX_Text(pDX, IDC_RES_SLASHING, m_nSlashing);
	DDV_MinMaxUInt(pDX, m_nSlashing, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabResistancesDlg, CDialog)
	//{{AFX_MSG_MAP(CTabResistancesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabResistancesDlg message handlers

BOOL CTabResistancesDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabResistancesDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CTabResistancesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	return TRUE; 
}
