// SaveChrDlg.cpp : implementation file
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
#include "SaveChrDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveChrDlg dialog


CSaveChrDlg::CSaveChrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveChrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveChrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSaveChrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveChrDlg)
	DDX_Control(pDX, IDC_SAVE_MSG, m_stSaveMsg);
	DDX_Control(pDX, IDC_CHR_FILENAME, m_edFilename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveChrDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveChrDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveChrDlg message handlers

BOOL CSaveChrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nIndex = m_strPath.ReverseFind('\\');
	if (nIndex == -1 || nIndex == m_strPath.GetLength()-1)
		return(FALSE);

	m_edFilename.SetWindowText(m_strPath.Mid(nIndex+1));
	
	return TRUE;
}

void CSaveChrDlg::OnOK() 
{
	CString strFile;
	m_edFilename.GetWindowText(strFile);

	// Make sure there is something there.
	strFile.TrimRight();
	strFile.TrimLeft();
	if (strFile.IsEmpty())
	{
		AfxMessageBox("You must enter a new filename to save the character under.");
		return;
	}

	// Make sure it ends in ".chr"
	int nIndex = strFile.Find('.');
	if (nIndex == -1)
		strFile += ".chr";
	else
	{
		CString strExt = strFile.Mid(nIndex+1);
		if (strnicmp(strExt,"chr",3))
			strFile += ".chr";
	}

	// Make sure it doesn't already exist.
	CString strFilePath = _strInstallPath + CString(DIR_CHARACTERS) + strFile;
	CFileStatus fs;
	if (CFile::GetStatus(strFilePath,fs))
	{
		AfxMessageBox("A file already exists with that name.");
		return;
	}

	m_strPath = strFilePath;
	EndDialog(IDOK);
}
