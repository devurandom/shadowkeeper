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

// EditUserListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dalekeeper.h"
#include "EditUserListDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditUserListDlg dialog


CEditUserListDlg::CEditUserListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditUserListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditUserListDlg)
	m_strDisplayText = _T("");
	//}}AFX_DATA_INIT

	m_bEdit = FALSE;
	m_nZeroPad = 2;
	m_nRangeType = RANGE_BYTE;
}


void CEditUserListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditUserListDlg)
	DDX_Control(pDX, IDC_VALUE, m_edValue);
	DDX_Control(pDX, IDC_RANGE, m_stRange);
	DDX_Text(pDX, IDC_DISPLAYTEXT, m_strDisplayText);
	DDV_MaxChars(pDX, m_strDisplayText, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditUserListDlg, CDialog)
	//{{AFX_MSG_MAP(CEditUserListDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditUserListDlg message handlers

BOOL CEditUserListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_bEdit)
		SetWindowText("Edit User List Item");
	else
		SetWindowText("Add User List Item");

	char szValue[101];
	MakeHexString(m_dwValue,szValue,m_nZeroPad);
	m_edValue.SetWindowText(szValue);

	switch(m_nRangeType)
	{
		case RANGE_BYTE :
			m_stRange.SetWindowText("0x00 to 0xFF");
			break;
		case RANGE_WORD :
			m_stRange.SetWindowText("0x0000 to 0xFFFF");
			break;
		case RANGE_DWORD :
			m_stRange.SetWindowText("0x00000000 to 0xFFFFFFFF");
			break;
	}
	
	return(TRUE);
}

void CEditUserListDlg::SetRange(int nRangeType)
{
	m_nRangeType = nRangeType;
	switch(m_nRangeType)
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
			m_nRangeType = RANGE_DWORD; // Catch-all.
			break;
	}
}


void CEditUserListDlg::OnOK() 
{
	UpdateData(TRUE);

	CString strValue;
	m_edValue.GetWindowText(strValue);
	m_dwValue = strtol(strValue,NULL,0);

	BOOL bOutOfRange = FALSE;
	switch(m_nRangeType)
	{
		case RANGE_BYTE :
			if (m_dwValue > 0xFF)
				bOutOfRange = TRUE;
			break;
		case RANGE_WORD :
			if (m_dwValue > 0xFFFF)
				bOutOfRange = TRUE;
			break;
	}

	if (bOutOfRange)
	{
		MessageBox("Value is out of range.","Invalid Value",MB_ICONEXCLAMATION);
		return;
	}

	EndDialog(IDOK);
}
