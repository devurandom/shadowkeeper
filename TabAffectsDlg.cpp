// TabAffectsDlg.cpp : implementation file
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
#include "TabAffectsDlg.h"
#include "EditAffectDlg.h"
#include "ValueList.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabAffectsDlg dialog

CTabAffectsDlg::CTabAffectsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabAffectsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabAffectsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_hAccel = NULL;
}

void CTabAffectsDlg::OnDestroy() 
{
	ClearList();
	CDialog::OnDestroy();
}

void CTabAffectsDlg::ClearList()
{
	INF_AFF *pAff;
	int nCount = m_lcList.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pAff = (INF_AFF*)m_lcList.GetItemData(i);
		delete pAff;
	}

	m_lcList.DeleteAllItems();
}

void CTabAffectsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabAffectsDlg)
	DDX_Control(pDX, IDC_LIST, m_lcList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabAffectsDlg, CDialog)
	//{{AFX_MSG_MAP(CTabAffectsDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_AFF_DUPLICATE, OnAffDuplicate)
	ON_BN_CLICKED(IDC_AFF_REMOVE, OnAffRemove)
	ON_BN_CLICKED(IDC_AFF_COPY, OnAffCopy)
	ON_BN_CLICKED(IDC_AFF_PASTE, OnAffPaste)
	ON_BN_CLICKED(IDC_AFF_EDIT, OnAffEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabAffectsDlg message handlers

BOOL CTabAffectsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabAffectsDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CTabAffectsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	DWORD dwStyle = m_lcList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcList.SetExtendedStyle(dwStyle);

	m_lcList.InsertColumn(0,"Type",LVCFMT_LEFT,140);
	m_lcList.InsertColumn(1,"Name",LVCFMT_LEFT,105);
	m_lcList.InsertColumn(2,"Param 1",LVCFMT_LEFT,70);
	m_lcList.InsertColumn(3,"Param 2",LVCFMT_LEFT,70);
	m_lcList.InsertColumn(4,"Resource 0",LVCFMT_LEFT,90);
	m_lcList.InsertColumn(5,"Resource 1",LVCFMT_LEFT,90);
	m_lcList.InsertColumn(6,"Resource 2",LVCFMT_LEFT,90);
	m_lcList.InsertColumn(7,"Resource 3",LVCFMT_LEFT,180);
	m_lcList.InsertColumn(8,"Time",LVCFMT_LEFT,90);
	m_lcList.InsertColumn(9,"Flags",LVCFMT_LEFT,75);
	m_lcList.InsertColumn(10,"Target",LVCFMT_LEFT,75);
	
	return TRUE;
}

void CTabAffectsDlg::ScriptName(const char *pszResName, CString &strText)
{
	CString strScript;

	strScript = GetSpellScriptName(pszResName);
	if (strScript.IsEmpty())
		strText = pszResName;
	else
		strText.Format("%s [%s]",(const char *)strScript,pszResName);
}

int CTabAffectsDlg::AddToList(INF_AFF *pAff, int nIndex)
{
	CString strType;
	CString strRes;
	char szBuf[101];

	if (!_vlAffects.GetText(pAff->dwAffectType,strType))
	{
		MakeHexString(pAff->dwAffectType,szBuf,4);
		strType = szBuf;
	}

	if (nIndex == -1)	
	{
		nIndex = m_lcList.GetItemCount();
		nIndex = m_lcList.InsertItem(nIndex,strType);
	}
	else
		m_lcList.SetItemText(nIndex,0,strType);

	memset(szBuf,0,32);
	memcpy(szBuf,pAff->chName,32);
	m_lcList.SetItemText(nIndex,1,szBuf);

	sprintf(szBuf,"%d",pAff->nNP1);
	m_lcList.SetItemText(nIndex,2,szBuf);

	sprintf(szBuf,"%d",pAff->nNP2);
	m_lcList.SetItemText(nIndex,3,szBuf);

	memset(szBuf,0,10);
	memcpy(szBuf,pAff->chResRef0,8);
	ScriptName(szBuf,strRes);
	m_lcList.SetItemText(nIndex,4,strRes);

	memset(szBuf,0,10);
	memcpy(szBuf,pAff->chResRef1,8);
	ScriptName(szBuf,strRes);
	m_lcList.SetItemText(nIndex,5,strRes);

	memset(szBuf,0,10);
	memcpy(szBuf,pAff->chResRef2,8);
	ScriptName(szBuf,strRes);
	m_lcList.SetItemText(nIndex,6,strRes);

	memset(szBuf,0,10);
	memcpy(szBuf,pAff->chResRef3,8);
	ScriptName(szBuf,strRes);
	m_lcList.SetItemText(nIndex,7,strRes);

	sprintf(szBuf,"%d",pAff->nTime);
	m_lcList.SetItemText(nIndex,8,szBuf);

	MakeHexString(pAff->dwFlags,szBuf,8);
	m_lcList.SetItemText(nIndex,9,szBuf);

	MakeHexString(pAff->dwTargetType,szBuf,4);
	m_lcList.SetItemText(nIndex,10,szBuf);

	m_lcList.SetItemData(nIndex,(DWORD)pAff);

	return(nIndex);
}

void CTabAffectsDlg::SetAffects(CPtrList &list)
{
	ClearList();

	INF_AFF *pAff;
	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		pAff = (INF_AFF*)list.GetNext(pos);
		AddToList(pAff);
	}

	list.RemoveAll();
}

void CTabAffectsDlg::GetAffects(CPtrList &list)
{
	INF_AFF *pAff, *pNew;
	int nCount = m_lcList.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pAff = (INF_AFF*)m_lcList.GetItemData(i);

		pNew = new INF_AFF;
		memcpy(pNew,pAff,sizeof(INF_AFF));

		list.AddTail(pNew);
	}
}

void CTabAffectsDlg::OnAffDuplicate() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	INF_AFF *pAff = (INF_AFF*)m_lcList.GetItemData(nIndex);
	INF_AFF *pNew = new INF_AFF;
	memcpy(pNew,pAff,sizeof(INF_AFF));

	nIndex = AddToList(pNew);
	SetSelectedItem(nIndex);
	m_lcList.EnsureVisible(nIndex,FALSE);
}

void CTabAffectsDlg::OnAffRemove() 
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		if (MessageBox("Are you sure you want to clear all affects?","Clear All",MB_ICONQUESTION|MB_YESNO) == IDYES)
		{
			ClearList();
			return;
		}
	}

	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	if (MessageBox("Are you sure want to delete this affect?","Delete Affect",MB_ICONQUESTION|MB_YESNO) == IDNO)
		return;

	INF_AFF *pAff = (INF_AFF*)m_lcList.GetItemData(nIndex);
	delete pAff;
	m_lcList.DeleteItem(nIndex);

	if (nIndex >= m_lcList.GetItemCount())
		nIndex = m_lcList.GetItemCount()-1;

	SetSelectedItem(nIndex);
}

void CTabAffectsDlg::OnAffCopy() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;
	INF_AFF *pAff = (INF_AFF*)m_lcList.GetItemData(nIndex);
	
	if (!OpenClipboard())
	{
		AfxMessageBox("Unable to open the clipboard.");
		return;
	}
	if (!EmptyClipboard())
	{
		AfxMessageBox("Unable to empty the clipboard.");
		::CloseClipboard();
		return;
	}

	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,sizeof(INF_AFF));
	if (!hMem)
	{
		AfxMessageBox("Failed to allocate memory for the clipboard.");
		::CloseClipboard();
		return;
	}

	BYTE *pData = (BYTE*)::GlobalLock(hMem);
	if (!pData)
	{
		AfxMessageBox("Failed to copy data to the clipboard buffer.");
		::CloseClipboard();
		::GlobalFree(hMem);
		return;
	}
	memcpy(pData,pAff,sizeof(INF_AFF));
	::GlobalUnlock(hMem);

	if (!::SetClipboardData(_nClipFormatAff,hMem))
	{
		AfxMessageBox("Failed to set the clipboard data.");
		::CloseClipboard();
		::GlobalFree(hMem);
		return;
	}
	
	::CloseClipboard();
}

void CTabAffectsDlg::OnAffPaste() 
{
	if (!OpenClipboard())
	{
		AfxMessageBox("Unable to open the clipboard.");
		return;
	}

	HGLOBAL hMem = ::GetClipboardData(_nClipFormatAff);
	if (!hMem)
	{
		AfxMessageBox("No affects currently on the clipboard.");
		::CloseClipboard();
		return;
	}

	BYTE *pData = (BYTE*)::GlobalLock(hMem);
	if (!pData)
	{
		AfxMessageBox("Failed to read data from the clipboard buffer.");
		::CloseClipboard();
		return;
	}

	INF_AFF *pAff = new INF_AFF;
	if (!pAff)
	{
		AfxMessageBox("Unable to allocate memory for new affect.");
		::GlobalUnlock(hMem);
		::CloseClipboard();
		return;
	}

	memcpy(pAff,pData,sizeof(INF_AFF));
	int nIndex = AddToList(pAff);
	SetSelectedItem(nIndex);
	m_lcList.EnsureVisible(nIndex,FALSE);

	::GlobalUnlock(hMem);
	::CloseClipboard();
}

void CTabAffectsDlg::OnAffEdit() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;
	INF_AFF *pAff = (INF_AFF*)m_lcList.GetItemData(nIndex);

	CEditAffectDlg d;
	d.SetAffect(pAff);
	if (d.DoModal() == IDOK)
	{
		d.GetAffect(pAff);
		AddToList(pAff,nIndex);
	}
}

void CTabAffectsDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	OnAffEdit();
}
