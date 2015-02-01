// TabProfsDlg.cpp : implementation file
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
#include "TabProfsDlg.h"
#include "InfCreature.h"
#include "ValueList.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabProfsDlg dialog


CTabProfsDlg::CTabProfsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabProfsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabProfsDlg)
	//}}AFX_DATA_INIT

	m_hAccel = NULL;
}


void CTabProfsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabProfsDlg)
	DDX_Control(pDX, IDC_PROFS, m_lcProfs);
	//}}AFX_DATA_MAP
}

void CTabProfsDlg::OnDestroy() 
{
	int nCount = m_lcProfs.GetItemCount();
	for (int i=0;i<nCount;i++)
		delete (PROFDATA*)m_lcProfs.GetItemData(i);
	
	CDialog::OnDestroy();
}

BEGIN_MESSAGE_MAP(CTabProfsDlg, CDialog)
	//{{AFX_MSG_MAP(CTabProfsDlg)
	ON_BN_CLICKED(IDC_1STCLASSDEC, On1stclassdec)
	ON_BN_CLICKED(IDC_1STCLASSINC, On1stclassinc)
	ON_BN_CLICKED(IDC_2NDCLASSDEC, On2ndclassdec)
	ON_BN_CLICKED(IDC_2NDCLASSINC, On2ndclassinc)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabProfsDlg message handlers

BOOL CTabProfsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabProfsDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CTabProfsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	DWORD dwStyle = m_lcProfs.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcProfs.SetExtendedStyle(dwStyle);

	m_lcProfs.InsertColumn(0,"Proficiency",LVCFMT_LEFT,180);
	m_lcProfs.InsertColumn(1,"1st Class",LVCFMT_LEFT,66);
	m_lcProfs.InsertColumn(2,"2nd Class",LVCFMT_LEFT,64);

	FillList(_vlProfs);
	FillList(_vlProfsUser);

	SetSelectedItem(0);

	return TRUE; 
}

void CTabProfsDlg::FillList(CValueList &vl)
{
	CValueItem vi;
	PROFDATA *pData;
	int nItem;

	POSITION pos = vl.GetHeadPosition();
	while(pos)
	{
		vi = vl.GetNext(pos);

		pData = new PROFDATA;
		pData->chProf = vi.m_nValue;
		pData->nFirstClass = 0;
		pData->nSecondClass = 0;

		nItem = m_lcProfs.InsertItem(0,vi.m_strText);
		m_lcProfs.SetItemText(nItem,1,"");
		m_lcProfs.SetItemText(nItem,2,"");
		m_lcProfs.SetItemData(nItem,(DWORD)pData);
	}
}

void CTabProfsDlg::On1stclassdec() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	PROFDATA *pData = (PROFDATA*)m_lcProfs.GetItemData(nIndex);
	if (pData->nFirstClass > 0)
	{
		pData->nFirstClass--;
		if (pData->nFirstClass)
		{
			CString strText;
			strText.Format("%d",pData->nFirstClass);
			m_lcProfs.SetItemText(nIndex,1,strText);
		}
		else
			m_lcProfs.SetItemText(nIndex,1,"");
	}
}

void CTabProfsDlg::On1stclassinc() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	PROFDATA *pData = (PROFDATA*)m_lcProfs.GetItemData(nIndex);
	if (pData->nFirstClass < 7)
	{
		pData->nFirstClass++;
		CString strText;
		strText.Format("%d",pData->nFirstClass);
		m_lcProfs.SetItemText(nIndex,1,strText);
	}
}

void CTabProfsDlg::On2ndclassdec() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	PROFDATA *pData = (PROFDATA*)m_lcProfs.GetItemData(nIndex);
	if (pData->nSecondClass > 0)
	{
		pData->nSecondClass--;
		if (pData->nSecondClass)
		{
			CString strText;
			strText.Format("%d",pData->nSecondClass);
			m_lcProfs.SetItemText(nIndex,2,strText);
		}
		else
			m_lcProfs.SetItemText(nIndex,2,"");
	}
}

void CTabProfsDlg::On2ndclassinc() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	PROFDATA *pData = (PROFDATA*)m_lcProfs.GetItemData(nIndex);
	if (pData->nSecondClass < 7)
	{
		pData->nSecondClass++;
		CString strText;
		strText.Format("%d",pData->nSecondClass);
		m_lcProfs.SetItemText(nIndex,2,strText);
	}
}

void CTabProfsDlg::UpdateCount(BYTE chProf, int nFirstClass, int nSecondClass)
{
	PROFDATA *pItemData;
	int nCount = m_lcProfs.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pItemData = (PROFDATA*)m_lcProfs.GetItemData(i);
		if (pItemData->chProf == chProf)
		{
			CString strText;

			if (nFirstClass < 1)
				m_lcProfs.SetItemText(i,1,"");
			else
			{
				strText.Format("%d",nFirstClass);
				m_lcProfs.SetItemText(i,1,strText);
			}

			if (nSecondClass < 1)
				m_lcProfs.SetItemText(i,2,"");
			else
			{
				strText.Format("%d",nSecondClass);
				m_lcProfs.SetItemText(i,2,strText);
			}

			pItemData->nFirstClass = nFirstClass;
			pItemData->nSecondClass = nSecondClass;
			
			return;
		}
	}
}

void CTabProfsDlg::ClearCounts()
{
	PROFDATA *pItemData;
	int nCount = m_lcProfs.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pItemData = (PROFDATA*)m_lcProfs.GetItemData(i);

		pItemData->nFirstClass = 0;
		pItemData->nSecondClass = 0;

		m_lcProfs.SetItemText(i,1,"");
		m_lcProfs.SetItemText(i,2,"");
	}
}

void CTabProfsDlg::SetProfs(CObList &list)
{
	ClearCounts();

	CProfData *pData;
	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		pData = (CProfData*)list.GetNext(pos);
		UpdateCount(pData->m_chProf,pData->m_nFirstClass,pData->m_nSecondClass);

		// Clear the list, it isn't needed anymore and will save the view from
		// having to do it.
		delete pData;
	}

	list.RemoveAll();
}

void CTabProfsDlg::GetProfs(CObList &list)
{
	CProfData *pProfData;
	PROFDATA *pItemData;
	int nCount = m_lcProfs.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pItemData = (PROFDATA*)m_lcProfs.GetItemData(i);
		if (pItemData->nFirstClass || pItemData->nSecondClass)
		{
			pProfData = new CProfData;
			pProfData->m_chProf = pItemData->chProf;
			pProfData->m_nFirstClass = pItemData->nFirstClass;
			pProfData->m_nSecondClass = pItemData->nSecondClass;

			list.AddTail(pProfData);
		}
	}
}
