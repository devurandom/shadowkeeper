// TabMemSpellsDlg.cpp : implementation file
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
#include "TabMemSpellsDlg.h"
#include "InfCreature.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabMemSpellsDlg dialog


CTabMemSpellsDlg::CTabMemSpellsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabMemSpellsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabMemSpellsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hAccel = NULL;
}


void CTabMemSpellsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabMemSpellsDlg)
	DDX_Control(pDX, IDC_LIST, m_lcList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabMemSpellsDlg, CDialog)
	//{{AFX_MSG_MAP(CTabMemSpellsDlg)
	ON_BN_CLICKED(IDC_MAXDEC, OnMaxdec)
	ON_BN_CLICKED(IDC_MAXINC, OnMaxinc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabMemSpellsDlg message handlers

BOOL CTabMemSpellsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabMemSpellsDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CTabMemSpellsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	DWORD dwStyle = m_lcList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcList.SetExtendedStyle(dwStyle);

	m_lcList.InsertColumn(0,"Type",LVCFMT_LEFT,65);
	m_lcList.InsertColumn(1,"Level",LVCFMT_LEFT,60);
	m_lcList.InsertColumn(2,"Max Can Memorize",LVCFMT_LEFT,1);
	ResizeLastColumn(m_lcList,TRUE);

	return TRUE; 
}

void CTabMemSpellsDlg::OnMaxdec() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CString strNum;
	int nNum;
	
	strNum = m_lcList.GetItemText(nIndex,2);
	nNum = atoi(strNum);
	if (nNum > 0)
	{
		strNum.Format("%d",nNum-1);
		m_lcList.SetItemText(nIndex,2,strNum);
	}
}

void CTabMemSpellsDlg::OnMaxinc() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CString strNum;
	int nNum;
	
	strNum = m_lcList.GetItemText(nIndex,2);
	nNum = atoi(strNum);
	strNum.Format("%d",nNum+1);
	m_lcList.SetItemText(nIndex,2,strNum);
}

void CTabMemSpellsDlg::SetData(MEMINFO *pMemInfo, int nCount)
{
	m_lcList.DeleteAllItems();

	CString strType, strNum, strLevel;
	int nIndex;

	for (int i=0;i<nCount;i++)
	{
		switch(pMemInfo[i].wType)
		{
			case INF_CRE_ST_INNATE :
				strType = "Innate";
				break;
			case INF_CRE_ST_WIZARD :
				strType = "Wizard";
				break;
			case INF_CRE_ST_PRIEST :
				strType = "Priest";
				break;
			default :
				strType = "Unknown";	// Would be bad to have this happen.
				break;
		}
		strNum.Format("%d",pMemInfo[i].wNumMemorizable);
		strLevel.Format("%d",pMemInfo[i].wLevel+1);

		nIndex = m_lcList.InsertItem(i,strType);
		if (nIndex == -1)
			continue;
		m_lcList.SetItemText(nIndex,1,strLevel);
		m_lcList.SetItemText(nIndex,2,strNum);
		m_lcList.SetItemData(nIndex,pMemInfo[i].wType);
	}
}

void CTabMemSpellsDlg::GetData(MEMINFO *pMemInfo, int nCount)
{
	int nListCount = m_lcList.GetItemCount();

	for (int i=0;i<nCount&&i<nListCount;i++)
	{
		pMemInfo[i].wLevel = atoi(m_lcList.GetItemText(i,1))-1;
		pMemInfo[i].wNumMemorizable = atoi(m_lcList.GetItemText(i,2));
		pMemInfo[i].wType = (WORD)m_lcList.GetItemData(i);
	}
}
