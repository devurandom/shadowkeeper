// LimitSpellsDlg.cpp : implementation file
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
#include "LimitSpellsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLimitSpellsDlg dialog


CLimitSpellsDlg::CLimitSpellsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLimitSpellsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLimitSpellsDlg)
	m_nKnownSpellLimit = 1;
	m_nMemSpellLimit = 1;
	m_bUseKnownSpellLimit = FALSE;
	m_bUseMemSpellLimit = FALSE;
	m_bMemSpellsOnSave = FALSE;
	m_bDisableSpellIcons = FALSE;
	//}}AFX_DATA_INIT
}


void CLimitSpellsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLimitSpellsDlg)
	DDX_Text(pDX, IDC_KNOWNLIMIT, m_nKnownSpellLimit);
	DDV_MinMaxUInt(pDX, m_nKnownSpellLimit, 1, 50);
	DDX_Text(pDX, IDC_MEMLIMIT, m_nMemSpellLimit);
	DDV_MinMaxUInt(pDX, m_nMemSpellLimit, 1, 50);
	DDX_Check(pDX, IDC_USE_KNOWNLIMIT, m_bUseKnownSpellLimit);
	DDX_Check(pDX, IDC_USE_MEMLIMIT, m_bUseMemSpellLimit);
	DDX_Check(pDX, IDC_MEMONSAVE, m_bMemSpellsOnSave);
	DDX_Check(pDX, IDC_DISABLE_SPELL_ICONS, m_bDisableSpellIcons);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLimitSpellsDlg, CDialog)
	//{{AFX_MSG_MAP(CLimitSpellsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLimitSpellsDlg message handlers
