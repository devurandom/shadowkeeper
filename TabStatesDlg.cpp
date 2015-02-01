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


// TabStatesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dalekeeper.h"
#include "TabStatesDlg.h"
#include "ValueList.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabStatesDlg dialog


CTabStatesDlg::CTabStatesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabStatesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabStatesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabStatesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabStatesDlg)
	DDX_Control(pDX, IDC_LIST, m_lcList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabStatesDlg, CDialog)
	//{{AFX_MSG_MAP(CTabStatesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabStatesDlg message handlers

BOOL CTabStatesDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CTabStatesDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabStatesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	DWORD dwStyle = m_lcList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcList.SetExtendedStyle(dwStyle);

	m_lcList.InsertColumn(0,"State Flag",LVCFMT_LEFT,210);
	m_lcList.InsertColumn(1,"Value",LVCFMT_LEFT,100);

	int nItem;
	char szBuf[100];
	CValueItem vi;
	POSITION pos = _vlState.GetHeadPosition();
	while(pos)
	{
		vi = _vlState.GetNext(pos);
		MakeHexString(vi.m_nValue,szBuf,8);

		nItem = m_lcList.InsertItem(0,vi.m_strText);
		m_lcList.SetItemText(nItem,1,szBuf);
		m_lcList.SetItemData(nItem,vi.m_nValue);
	}
	
	return TRUE;
}

void CTabStatesDlg::SetFlags(DWORD dwValue)
{
	int nCount = m_lcList.GetItemCount();
	for (int i=0;i<nCount;i++)
		if (dwValue & m_lcList.GetItemData(i))
			m_lcList.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
		else
			m_lcList.SetItemState(i,0,LVIS_SELECTED);
}

DWORD CTabStatesDlg::GetFlags()
{
	DWORD dwValue = 0;
	int nItem;
	POSITION pos = m_lcList.GetFirstSelectedItemPosition();
	while(pos)
	{
		nItem = m_lcList.GetNextSelectedItem(pos);
		dwValue |= m_lcList.GetItemData(nItem);
	}

	return(dwValue);
}