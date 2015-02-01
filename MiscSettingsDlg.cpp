// MiscSettingsDlg.cpp : implementation file
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
#include "MiscSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiscSettingsDlg dialog


CMiscSettingsDlg::CMiscSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiscSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMiscSettingsDlg)
	m_bUseGridLines = FALSE;
	m_bDefaultOpenSinglePlayer = FALSE;
	m_bIgnoreDataVersions = FALSE;
	m_bAllowQuicksaves = FALSE;
	//}}AFX_DATA_INIT
}


void CMiscSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiscSettingsDlg)
	DDX_Check(pDX, IDC_USE_GRIDLINES, m_bUseGridLines);
	DDX_Check(pDX, IDC_DEFAULTSINGLE, m_bDefaultOpenSinglePlayer);
	DDX_Check(pDX, IDC_IGNORE_DATA_VERSIONS, m_bIgnoreDataVersions);
	DDX_Check(pDX, IDC_ENABLE_QUICKSAVES, m_bAllowQuicksaves);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMiscSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CMiscSettingsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiscSettingsDlg message handlers
