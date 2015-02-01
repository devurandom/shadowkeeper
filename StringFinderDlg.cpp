// StringFinderDlg.cpp : implementation file
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
#include "StringFinderDlg.h"
#include "InfTlk.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStringFinderDlg dialog


CStringFinderDlg::CStringFinderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStringFinderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStringFinderDlg)
	m_bAlphaNum = FALSE;
	//}}AFX_DATA_INIT
}


void CStringFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStringFinderDlg)
	DDX_Control(pDX, IDC_NUM_FOUND, m_stNumFound);
	DDX_Control(pDX, IDC_TEXT_MATCH, m_edTextMatch);
	DDX_Control(pDX, IDC_MAX_LENGTH, m_edMaxLength);
	DDX_Control(pDX, IDC_STRINGS, m_lcStrings);
	DDX_Check(pDX, IDC_ALPHANUM, m_bAlphaNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStringFinderDlg, CDialog)
	//{{AFX_MSG_MAP(CStringFinderDlg)
	ON_BN_CLICKED(IDC_FIND_STRINGS, OnFindStrings)
	ON_NOTIFY(NM_DBLCLK, IDC_STRINGS, OnDblclkStrings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStringFinderDlg message handlers

BOOL CStringFinderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle = m_lcStrings.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcStrings.SetExtendedStyle(dwStyle);

	m_lcStrings.InsertColumn(0,"String",LVCFMT_LEFT,350);
	m_lcStrings.InsertColumn(1,"Reference",LVCFMT_LEFT,5);
	ResizeLastColumn(m_lcStrings);

	CString strText;
	strText.Format("%u",m_dwInitialMaxLength);
	m_edMaxLength.SetWindowText(strText);

	OnFindStrings();

	return(TRUE);
}

void CStringFinderDlg::OnFindStrings() 
{
	CString strMaxLength, strTextMatch;

	m_edMaxLength.GetWindowText(strMaxLength);
	m_edTextMatch.GetWindowText(strTextMatch);

	DWORD dwMaxLength = strtoul(strMaxLength,NULL,0);
	strTextMatch.TrimLeft();
	strTextMatch.TrimRight();

	if (strTextMatch.IsEmpty() && (!dwMaxLength || dwMaxLength > 100))
	{
		if (MessageBox("You have not selected any criteria, or a large maximum length. "
				"This will generate a lot of entries in the list. The string file is large "
				"(more than 7MB) -- this operation might take a while and may take a large "
				"chunk of memory.\r\n\r\nContinue filling the list?",
				"Potentially a Lot of Matches",MB_ICONQUESTION|MB_YESNO) == IDNO)
			return;
	}

	UpdateData(TRUE);
	BeginWaitCursor();

	m_lcStrings.LockWindowUpdate();
	m_lcStrings.DeleteAllItems();

	CString strText;
	char szRef[100];
	int nItem;
	int nLen;
	int i;
	for (DWORD dwIndex=0;dwIndex<_infTlk.m_tlkHeader.dwStringCount;dwIndex++)
	{
		if (_infTlk.m_pEntries[dwIndex].wUnknown != 0x03)
			continue;

		// Good idea to pick a length when filtering. The lengths are available
		// without having to hit the disk (fast).
		if (dwMaxLength && _infTlk.m_pEntries[dwIndex].dwLength > dwMaxLength)
			continue;

		if (!_infTlk.GetString(dwIndex,strText))
			continue;

		strText.TrimRight();
		if (strText.IsEmpty())
			continue;

		if (m_bAlphaNum)
		{
			nLen = strText.GetLength();
			for (i=0;i<nLen;i++)
				if (!isalnum(strText.GetAt(i)) && !isspace(strText.GetAt(i)))
					break;
			if (i < nLen)
				continue;
		}

		if (!strTextMatch.IsEmpty() && strText.Find(strTextMatch) == -1)
			continue;

		MakeHexString(dwIndex,szRef,8);
		nItem = m_lcStrings.InsertItem(0,strText);
		m_lcStrings.SetItemText(nItem,1,szRef);
	}
	m_lcStrings.UnlockWindowUpdate();

	int nCount = m_lcStrings.GetItemCount();
	strText.Format("%u",nCount);
	m_stNumFound.SetWindowText(strText);

	EndWaitCursor();
}

void CStringFinderDlg::OnOK() 
{
	int nItem = GetSelectedItem();
	if (nItem == -1)
		return;

	CString strRef;
	strRef = m_lcStrings.GetItemText(nItem,1);
	m_dwResultRef = strtoul(strRef,NULL,0);

	BeginWaitCursor();
	CDialog::OnOK();
}

void CStringFinderDlg::OnCancel() 
{
	BeginWaitCursor();
	CDialog::OnCancel();
}

void CStringFinderDlg::OnDblclkStrings(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}
