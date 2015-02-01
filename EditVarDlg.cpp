// EditVarDlg.cpp : implementation file
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
#include "EditVarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVarDlg dialog


CEditVarDlg::CEditVarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditVarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditVarDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_bEdit = TRUE;
}


void CEditVarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVarDlg)
	DDX_Control(pDX, IDC_VAR_VALUE, m_edValue);
	DDX_Text(pDX, IDC_VAR_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditVarDlg, CDialog)
	//{{AFX_MSG_MAP(CEditVarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVarDlg message handlers

BOOL CEditVarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_bEdit)
		SetWindowText("Add Global Variable");

	CString strValue;
	strValue.Format("%d",m_nValue);
	m_edValue.SetWindowText(strValue);
	
	return TRUE;
}

void CEditVarDlg::OnOK() 
{
	UpdateData(TRUE);

	CString strValue;
	m_edValue.GetWindowText(strValue);
	m_nValue = strtol(strValue,NULL,0);

	EndDialog(IDOK);
}
