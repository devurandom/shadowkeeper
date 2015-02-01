// InstallPathDlg.cpp : implementation file
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
#include "DaleKeeper.h"
#include "InstallPathDlg.h"
#include "BrowseDirsDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstallPathDlg dialog


CInstallPathDlg::CInstallPathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstallPathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInstallPathDlg)
	m_strPath = _T("");
	//}}AFX_DATA_INIT
}


void CInstallPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstallPathDlg)
	DDX_Control(pDX, IDC_PATH_INFO, m_edPathInfo);
	DDX_Text(pDX, IDC_PATH, m_strPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInstallPathDlg, CDialog)
	//{{AFX_MSG_MAP(CInstallPathDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstallPathDlg message handlers

BOOL CInstallPathDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_edPathInfo.SetWindowText(
		"Shadow Keeper needs to know where you installed the game.\r\n\r\n"
		"Search: Makes Shadow Keeper attempt to locate the game for you.\r\n\r\n"
		"Browse: Allows you to pick the directory yourself."
		);
	
	return TRUE;
}

void CInstallPathDlg::OnBrowse() 
{
	CBrowseDirsDlg d;
	
	if (d.DoModal() == IDOK)
	{
		m_strPath = d.m_strPath;
		UpdateData(FALSE);
	}
}

void CInstallPathDlg::OnSearch() 
{
	if (FindInstallPath(m_strPath))
		UpdateData(FALSE);
}


void CInstallPathDlg::OnOK() 
{
	UpdateData(TRUE);

	m_strPath.TrimRight();
	if (m_strPath.IsEmpty())
	{
		MessageBox("You need to enter the path to Shadows of Amn.","No Path");
		return;
	}

	if (m_strPath.Right(1) != "\\")
	{
		m_strPath += "\\";
		UpdateData(FALSE);
	}

	// Verify that the game is really there. If IDMain.exe is not there allow them
	// to continue, but warn them.
	CString strFile(m_strPath+"BGMain.exe");
	DWORD dwAttr = GetFileAttributes(strFile);
	if (dwAttr == 0xFFFFFFFF)
	{
		CString strMessage = "The Shadows of Amn executable was not found at this location. "
			"Are you sure this is the correct directory?";
		if (MessageBox(strMessage,"BGMain.exe Not Found",MB_YESNO|MB_ICONQUESTION) == IDNO)
			return;
	}

	EndDialog(IDOK);
}
