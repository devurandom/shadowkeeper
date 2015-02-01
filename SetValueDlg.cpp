// SetValueDlg.cpp : implementation file
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
#include "SetValueDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetValueDlg dialog


CSetValueDlg::CSetValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetValueDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetValueDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pcbList = NULL;
	m_nPadSize = 0;
}


void CSetValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetValueDlg)
	DDX_Control(pDX, IDC_VALUE, m_edValue);
	DDX_Control(pDX, IDC_COMBO, m_cbKnown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetValueDlg, CDialog)
	//{{AFX_MSG_MAP(CSetValueDlg)
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	ON_EN_CHANGE(IDC_VALUE, OnChangeValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetValueDlg message handlers

BOOL CSetValueDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	FillValues();
	m_edValue.SetWindowText(m_strValue);
	MatchListWithValue();
	
	return TRUE;
}

void CSetValueDlg::SetCurrentValue(DWORD dwValue, int nPadSize)
{
	m_nPadSize = nPadSize;

	char szBuf[100];
	MakeHexString(dwValue,szBuf,m_nPadSize);
	m_strValue = szBuf;
	m_dwValue = dwValue;
}

void CSetValueDlg::FillValues()
{
	m_cbKnown.ResetContent();

	int nIndex;
	DWORD dwValue;
	CString strText;

	int nCount = m_pcbList->GetCount();
	for (int i=0;i<nCount;i++)
	{
		m_pcbList->GetLBText(i,strText);
		dwValue = m_pcbList->GetItemData(i);

		nIndex = m_cbKnown.AddString(strText);
		m_cbKnown.SetItemData(nIndex,dwValue);
	}

	m_nUnknownIndex = m_cbKnown.AddString("Unknown Value");
}

void CSetValueDlg::MatchListWithValue()
{
	DWORD dwValue = strtoul(m_strValue,NULL,0);

	DWORD dwListValue;
	int nCount = m_cbKnown.GetCount();
	for (int i=0;i<nCount;i++)
	{
		if (i == m_nUnknownIndex)
			continue;

		dwListValue = m_cbKnown.GetItemData(i);
		if (dwListValue == dwValue)
		{
			m_cbKnown.SetCurSel(i);
			return;
		}
	}

	m_cbKnown.SetCurSel(m_nUnknownIndex);
}

void CSetValueDlg::OnSelchangeCombo() 
{
	int nIndex = m_cbKnown.GetCurSel();
	if (nIndex == CB_ERR || nIndex == m_nUnknownIndex)
		return;

	DWORD dwValue = m_cbKnown.GetItemData(nIndex);
	char szBuf[100];
	MakeHexString(dwValue,szBuf,m_nPadSize);
	m_edValue.SetWindowText(szBuf);
}

void CSetValueDlg::OnChangeValue() 
{
	m_edValue.GetWindowText(m_strValue);
	MatchListWithValue();
}

void CSetValueDlg::OnOK() 
{
	m_edValue.GetWindowText(m_strValue);
	m_dwValue = strtoul(m_strValue,NULL,0);
	EndDialog(IDOK);
}
