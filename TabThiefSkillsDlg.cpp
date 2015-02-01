// TabThiefSkillsDlg.cpp : implementation file
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
#include "TabThiefSkillsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabThiefSkillsDlg dialog


CTabThiefSkillsDlg::CTabThiefSkillsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabThiefSkillsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabThiefSkillsDlg)
	m_nFindTraps = 0;
	m_nMoveSilently = 0;
	m_nOpenLocks = 0;
	m_nPickPockets = 0;
	m_nDetectIllusions = 0;
	m_nHideInShadows = 0;
	m_nSetTraps = 0;
	//}}AFX_DATA_INIT

	m_hAccel = NULL;
}


void CTabThiefSkillsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabThiefSkillsDlg)
	DDX_Control(pDX, IDC_MOVE_SILENTLY, m_edMoveSilently);
	DDX_Text(pDX, IDC_FIND_TRAPS, m_nFindTraps);
	DDV_MinMaxUInt(pDX, m_nFindTraps, 0, 255);
	DDX_Text(pDX, IDC_MOVE_SILENTLY, m_nMoveSilently);
	DDV_MinMaxUInt(pDX, m_nMoveSilently, 0, 255);
	DDX_Text(pDX, IDC_OPEN_LOCKS, m_nOpenLocks);
	DDV_MinMaxUInt(pDX, m_nOpenLocks, 0, 255);
	DDX_Text(pDX, IDC_PICK_POCKETS, m_nPickPockets);
	DDV_MinMaxUInt(pDX, m_nPickPockets, 0, 255);
	DDX_Text(pDX, IDC_DETECTILLUSIONS, m_nDetectIllusions);
	DDV_MinMaxUInt(pDX, m_nDetectIllusions, 0, 255);
	DDX_Text(pDX, IDC_HIDEINSHADOWS, m_nHideInShadows);
	DDV_MinMaxUInt(pDX, m_nHideInShadows, 0, 255);
	DDX_Text(pDX, IDC_SETTRAPS, m_nSetTraps);
	DDV_MinMaxUInt(pDX, m_nSetTraps, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabThiefSkillsDlg, CDialog)
	//{{AFX_MSG_MAP(CTabThiefSkillsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabThiefSkillsDlg message handlers

BOOL CTabThiefSkillsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabThiefSkillsDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CTabThiefSkillsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	return TRUE; 
}
