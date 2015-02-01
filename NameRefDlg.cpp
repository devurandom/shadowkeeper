// NameRefDlg.cpp : implementation file
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
#include "NameRefDlg.h"
#include "StringFinderDlg.h"
#include "InfTlk.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNameRefDlg dialog


CNameRefDlg::CNameRefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNameRefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNameRefDlg)
	//}}AFX_DATA_INIT
}


void CNameRefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNameRefDlg)
	DDX_Control(pDX, IDC_SHORT_REF, m_edShortRef);
	DDX_Control(pDX, IDC_LONG_REF, m_edLongRef);
	DDX_Control(pDX, IDC_SHORT_TEXT, m_edShortText);
	DDX_Control(pDX, IDC_LONG_TEXT, m_edLongText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNameRefDlg, CDialog)
	//{{AFX_MSG_MAP(CNameRefDlg)
	ON_BN_CLICKED(IDC_CLEAR_LONG, OnClearLong)
	ON_BN_CLICKED(IDC_CLEAR_SHORT, OnClearShort)
	ON_EN_CHANGE(IDC_LONG_REF, OnChangeLongRef)
	ON_EN_CHANGE(IDC_SHORT_REF, OnChangeShortRef)
	ON_BN_CLICKED(IDC_FIND_SHORT, OnFindShort)
	ON_BN_CLICKED(IDC_FIND_LONG, OnFindLong)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNameRefDlg message handlers

BOOL CNameRefDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strText;
	char szText[100];

	MakeHexString(m_dwShort,szText,8);
	m_edShortRef.SetWindowText(szText);
	MakeHexString(m_dwLong,szText,8);
	m_edLongRef.SetWindowText(szText);

	UpdateText(m_edShortRef,m_edShortText);
	UpdateText(m_edLongRef,m_edLongText);
	
	return(TRUE);
}

void CNameRefDlg::UpdateText(CEdit &edRef, CEdit &edText)
{
	CString strText;
	edRef.GetWindowText(strText);
	DWORD dwValue = strtoul(strText,NULL,0);

	if (dwValue == 0xFFFFFFFF)
		edText.SetWindowText("");
	else
	{
		_infTlk.GetString(dwValue,strText);
		edText.SetWindowText(strText);
	}
}

void CNameRefDlg::OnClearLong() 
{
	m_dwLong = 0xFFFFFFFF;
	m_edLongRef.SetWindowText("0xFFFFFFFF");
	m_edLongText.SetWindowText("");
}

void CNameRefDlg::OnClearShort() 
{
	m_dwShort = 0xFFFFFFFF;
	m_edShortRef.SetWindowText("0xFFFFFFFF");
	m_edShortText.SetWindowText("");
}

void CNameRefDlg::OnChangeLongRef() 
{
	UpdateText(m_edLongRef,m_edLongText);
}

void CNameRefDlg::OnChangeShortRef() 
{
	UpdateText(m_edShortRef,m_edShortText);
}

void CNameRefDlg::OnOK() 
{
	CString strText;

	m_edShortRef.GetWindowText(strText);
	m_dwShort = strtoul(strText,NULL,0);

	m_edLongRef.GetWindowText(strText);
	m_dwLong = strtoul(strText,NULL,0);

	EndDialog(IDOK);
}

void CNameRefDlg::OnFindShort() 
{
	CStringFinderDlg d;

	d.m_dwInitialMaxLength = 20;
	d.m_bAlphaNum = TRUE;
	if (d.DoModal() == IDOK)
	{
		char szText[100];

		m_dwShort = d.m_dwResultRef;
		MakeHexString(m_dwShort,szText,8);
		m_edShortRef.SetWindowText(szText);
		UpdateText(m_edShortRef,m_edShortText);
	}
}

void CNameRefDlg::OnFindLong() 
{
	CStringFinderDlg d;

	d.m_dwInitialMaxLength = 20;
	d.m_bAlphaNum = TRUE;
	if (d.DoModal() == IDOK)
	{
		char szText[100];

		m_dwLong = d.m_dwResultRef;
		MakeHexString(m_dwLong,szText,8);
		m_edLongRef.SetWindowText(szText);
		UpdateText(m_edLongRef,m_edLongText);
	}
}

