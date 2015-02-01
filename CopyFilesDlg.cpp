// CopyFilesDlg.cpp : implementation file
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
#include "CopyFilesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyFilesDlg dialog


CCopyFilesDlg::CCopyFilesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyFilesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyFilesDlg)
	//}}AFX_DATA_INIT
}


void CCopyFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyFilesDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCopyFilesDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyFilesDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyFilesDlg message handlers

BOOL CCopyFilesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetTimer(1,100,NULL);
	
	return TRUE;
}

void CCopyFilesDlg::DeleteFiles()
{
	CString strTarget;
	CString strFile;
	POSITION pos = m_slSource.GetHeadPosition();
	while(pos)
	{
		strFile = m_slSource.GetNext(pos);
		strTarget = m_strTargetDir+strFile;

		::DeleteFile(strTarget);
	}
}

void CCopyFilesDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(1);

	CString strSource;
	CString strTarget;
	CString strFile;
	POSITION pos = m_slSource.GetHeadPosition();
	while(pos)
	{
		strFile = m_slSource.GetNext(pos);
		strSource = m_strSourceDir+strFile;
		strTarget = m_strTargetDir+strFile;

		if (!::CopyFile(strSource,strTarget,TRUE))
		{
			DeleteFiles();
			EndDialog(IDCANCEL);
			return;
		}
	}
	EndDialog(IDOK);
}
