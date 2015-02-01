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

// SpellFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dalekeeper.h"
#include "SpellFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpellFilterDlg dialog


CSpellFilterDlg::CSpellFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpellFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpellFilterDlg)
	m_bMatchCase = FALSE;
	m_bMatchName = FALSE;
	m_strSearchText = _T("");
	m_bMatchDescription = FALSE;
	//}}AFX_DATA_INIT
}


void CSpellFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpellFilterDlg)
	DDX_Check(pDX, IDC_MATCH_CASE, m_bMatchCase);
	DDX_Check(pDX, IDC_MATCH_SPELLNAME, m_bMatchName);
	DDX_Text(pDX, IDC_SEARCHTEXT, m_strSearchText);
	DDX_Check(pDX, IDC_MATCH_DESCRIPTION, m_bMatchDescription);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpellFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CSpellFilterDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpellFilterDlg message handlers
