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

// EditListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dalekeeper.h"
#include "Global.h"
#include "EditListDlg.h"
#include "EditUserListDlg.h"
#include "ValueList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditListDlg dialog


CEditListDlg::CEditListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pvl = NULL;
	m_nZeroPad = 0;
}


void CEditListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditListDlg)
	DDX_Control(pDX, IDC_LIST, m_lcList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditListDlg, CDialog)
	//{{AFX_MSG_MAP(CEditListDlg)
	ON_BN_CLICKED(IDC_ADDLISTITEM, OnAddlistitem)
	ON_BN_CLICKED(IDC_DELETELISTITEM, OnDeletelistitem)
	ON_BN_CLICKED(IDC_EDITLISTITEM, OnEditlistitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListDlg message handlers

BOOL CEditListDlg::OnInitDialog() 
{
	ASSERT(m_pvl);

	CDialog::OnInitDialog();

	SetWindowText(CString("Edit List - ")+m_strListName);
	
	DWORD dwStyle = m_lcList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcList.SetExtendedStyle(dwStyle);

	m_lcList.InsertColumn(0,"Display Text",LVCFMT_LEFT,200);
	m_lcList.InsertColumn(1,"Value",LVCFMT_LEFT,5);
	ResizeLastColumn(m_lcList);

	switch(m_nRange)
	{
		case RANGE_BYTE :
			m_nZeroPad = 2;
			break;
		case RANGE_WORD :
			m_nZeroPad = 4;
			break;
		case RANGE_DWORD :
		default:
			m_nZeroPad = 8;
			break;
	}

	FillList();

	return(TRUE);
}

int CEditListDlg::AddItem(const char *pszText, DWORD dwValue)
{
	char szValue[101];

	int nItem = m_lcList.InsertItem(0,pszText);
	MakeHexString(dwValue,szValue,m_nZeroPad);
	m_lcList.SetItemText(nItem,1,szValue);

	SetSelectedItem(nItem);
	m_lcList.EnsureVisible(nItem,FALSE);

	return(nItem);
}

void CEditListDlg::FillList()
{
	CValueItem vi;
	POSITION pos = m_pvl->GetHeadPosition();
	while(pos)
	{
		vi = m_pvl->GetNext(pos);
		AddItem(vi.m_strText,vi.m_nValue);
	}
}

int CEditListDlg::GetSelectedItem()
{
	return(m_lcList.GetNextItem(-1,LVNI_SELECTED));
}

void CEditListDlg::SetSelectedItem(int nIndex)
{ 
	m_lcList.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
}

void CEditListDlg::OnAddlistitem() 
{
	CEditUserListDlg d;
	
	d.m_bEdit = FALSE;
	d.SetRange(m_nRange);
	d.m_strDisplayText.Empty();
	d.m_dwValue = 0;
	if (d.DoModal() == IDOK)
		AddItem(d.m_strDisplayText,d.m_dwValue);
}


void CEditListDlg::OnEditlistitem() 
{
	int nItem = GetSelectedItem();
	if (nItem == -1)
		return;

	CEditUserListDlg d;

	d.m_bEdit = TRUE;
	d.SetRange(m_nRange);
	d.m_strDisplayText = m_lcList.GetItemText(nItem,0);
	d.m_dwValue = strtol(m_lcList.GetItemText(nItem,1),NULL,0);
	if (d.DoModal() == IDOK)
	{
		m_lcList.DeleteItem(nItem);
		nItem = AddItem(d.m_strDisplayText,d.m_dwValue);
		m_lcList.EnsureVisible(nItem,FALSE);
		SetSelectedItem(nItem);
	}
}

void CEditListDlg::OnDeletelistitem() 
{
	int nItem = GetSelectedItem();
	if (nItem == -1)
		return;

	CString strText;
	strText = m_lcList.GetItemText(nItem,0);

	CString strMessage;
	strMessage = "Are you sure you want to delete this item?\r\n\r\n";
	strMessage += strText;

	if (MessageBox(strMessage,"Delete Item",MB_ICONQUESTION|MB_YESNO) == IDYES)
		m_lcList.DeleteItem(nItem);
}

void CEditListDlg::OnOK() 
{
	m_pvl->RemoveAll();

	int nCount = m_lcList.GetItemCount();

	CString strValue;
	CValueItem vi;
	for (int i=0;i<nCount;i++)
	{
		vi.m_strText = m_lcList.GetItemText(i,0);
		vi = m_lcList.GetItemText(i,1);

		m_pvl->AddTail(vi);
	}

	EndDialog(IDOK);
}


void CEditListDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditlistitem();
}
