// ColorPickerDlg.cpp : implementation file
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
#include "ColorPickerDlg.h"
#include "PalImageList.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPickerDlg dialog


CColorPickerDlg::CColorPickerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorPickerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorPickerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nSelection = -1;
}


void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorPickerDlg)
	DDX_Control(pDX, IDC_COLOR_LIST, m_lcColors);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorPickerDlg, CDialog)
	//{{AFX_MSG_MAP(CColorPickerDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_COLOR_LIST, OnDblclkColorList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPickerDlg message handlers

BOOL CColorPickerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_lcColors.SetImageList(&_ilPal,LVSIL_NORMAL);

	char szNum[50];
	int  nItem;

	int nCount = _ilPal.GetImageCount();
	for(int i=0;i<nCount;i++)
	{
		MakeHexString(i,szNum,2);

		nItem = m_lcColors.InsertItem(i,szNum,i);
		m_lcColors.SetItemData(nItem,i);
	}

	if (m_nSelection != -1)
	{
		for (i=0;i<nCount;i++)
			if ((int)m_lcColors.GetItemData(i) == m_nSelection)
			{
				m_lcColors.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				m_lcColors.EnsureVisible(i,FALSE);
			}
	}
	
	return(TRUE);
}

void CColorPickerDlg::OnOK() 
{
	int nItem = m_lcColors.GetNextItem(-1,LVNI_SELECTED);
	if (nItem == -1)
		return;

	m_nSelection = (int)m_lcColors.GetItemData(nItem);

	EndDialog(IDOK);
}

void CColorPickerDlg::OnDblclkColorList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	OnOK();
}
