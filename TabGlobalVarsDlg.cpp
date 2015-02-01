// TabGlobalVarsDlg.cpp : implementation file
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
#include "TabGlobalVarsDlg.h"
#include "Global.h"
#include "InfGame.h"
#include "EditVarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabGlobalVarsDlg dialog


CTabGlobalVarsDlg::CTabGlobalVarsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabGlobalVarsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabGlobalVarsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_hAccel = NULL;
}

void CTabGlobalVarsDlg::OnDestroy() 
{
	ClearList();
	CDialog::OnDestroy();
}

void CTabGlobalVarsDlg::ClearList()
{
	INF_GAME_GLOBAL *pGlobal;
	int nCount = m_lcList.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pGlobal = (INF_GAME_GLOBAL*)m_lcList.GetItemData(i);
		delete pGlobal;
	}

	m_lcList.DeleteAllItems();
}

void CTabGlobalVarsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabGlobalVarsDlg)
	DDX_Control(pDX, IDC_VAR_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_VAR_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_VAR_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_LIST, m_lcList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabGlobalVarsDlg, CDialog)
	//{{AFX_MSG_MAP(CTabGlobalVarsDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_VAR_ADD, OnVarAdd)
	ON_BN_CLICKED(IDC_VAR_DELETE, OnVarDelete)
	ON_BN_CLICKED(IDC_VAR_EDIT, OnVarEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabGlobalVarsDlg message handlers

BOOL CTabGlobalVarsDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CTabGlobalVarsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabGlobalVarsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	DWORD dwStyle = m_lcList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcList.SetExtendedStyle(dwStyle);

	m_lcList.InsertColumn(0,"Name",LVCFMT_LEFT,210);
	m_lcList.InsertColumn(1,"Value",LVCFMT_LEFT,100);
	
	return TRUE;
}

int CTabGlobalVarsDlg::AddToList(INF_GAME_GLOBAL *pGlobal, int nIndex)
{
	char szName[33];
	memset(szName,0,33);
	memcpy(szName,pGlobal->chName,32);

	if (nIndex == -1)
		nIndex = m_lcList.InsertItem(m_lcList.GetItemCount(),szName);
	else
		m_lcList.SetItemText(nIndex,0,szName);

	char szValue[100];
	sprintf(szValue,"%d",pGlobal->nValue);
	//MakeHexString(pGlobal->nValue,szValue,8);
	m_lcList.SetItemText(nIndex,1,szValue);
	m_lcList.SetItemData(nIndex,(DWORD)pGlobal);

	return(nIndex);
}

void CTabGlobalVarsDlg::SetGlobals(CPtrList &list)
{
	ClearList();

	INF_GAME_GLOBAL *pGlobal;
	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		pGlobal = (INF_GAME_GLOBAL*)list.GetNext(pos);
		AddToList(pGlobal);
	}

	list.RemoveAll();
}

void CTabGlobalVarsDlg::GetGlobals(CPtrList &list)
{
	INF_GAME_GLOBAL *pGlobal, *pNew;
	int nCount = m_lcList.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pGlobal = (INF_GAME_GLOBAL*)m_lcList.GetItemData(i);

		pNew = new INF_GAME_GLOBAL;
		memcpy(pNew,pGlobal,sizeof(INF_GAME_GLOBAL));

		list.AddTail(pNew);
	}
}

void CTabGlobalVarsDlg::DisableControls()
{
	m_lcList.EnableWindow(FALSE);
	m_btnAdd.EnableWindow(FALSE);
	m_btnEdit.EnableWindow(FALSE);
	m_btnDelete.EnableWindow(FALSE);
}

void CTabGlobalVarsDlg::OnVarAdd() 
{
	CEditVarDlg d;

	d.m_nValue = 0;
	if (d.DoModal() == IDOK)
	{
		INF_GAME_GLOBAL *pGlobal = new INF_GAME_GLOBAL;
		if (!pGlobal)
			return;

		memset(pGlobal,0,sizeof(INF_GAME_GLOBAL));
		strncpy(pGlobal->chName,d.m_strName,32);
		pGlobal->nValue = d.m_nValue;

		int nIndex = AddToList(pGlobal);
		SetSelectedItem(nIndex);
		m_lcList.EnsureVisible(nIndex,FALSE);
	}
}

void CTabGlobalVarsDlg::OnVarDelete() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CString strMessage;
	strMessage.Format("Are you sure you want to delete this variable?\r\n\r\n%s",
		(const char *)m_lcList.GetItemText(nIndex,0));

	if (MessageBox(strMessage,"Delete Global Variable",MB_ICONQUESTION|MB_YESNO) == IDNO)
		return;

	delete (INF_GAME_GLOBAL*)m_lcList.GetItemData(nIndex);
	m_lcList.DeleteItem(nIndex);

	if (nIndex < m_lcList.GetItemCount())
		SetSelectedItem(nIndex);
	else
		SetSelectedItem(m_lcList.GetItemCount()-1);
}

void CTabGlobalVarsDlg::OnVarEdit() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	INF_GAME_GLOBAL *pGlobal = (INF_GAME_GLOBAL*)m_lcList.GetItemData(nIndex);
	char szName[33];
	memset(szName,0,33);
	memcpy(szName,pGlobal->chName,32);

	CEditVarDlg d;

	d.m_bEdit = TRUE;
	d.m_nValue = pGlobal->nValue;
	d.m_strName = szName;
	if (d.DoModal() == IDOK)
	{
		m_lcList.DeleteItem(nIndex);

		memset(pGlobal,0,sizeof(INF_GAME_GLOBAL));
		strncpy(pGlobal->chName,d.m_strName,32);
		pGlobal->nValue = d.m_nValue;

		nIndex = AddToList(pGlobal);
		SetSelectedItem(nIndex);
		m_lcList.EnsureVisible(nIndex,FALSE);
	}
}

void CTabGlobalVarsDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnVarEdit();	
	*pResult = 0;
}
