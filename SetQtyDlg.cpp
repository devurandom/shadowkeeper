// SetQtyDlg.cpp : implementation file
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
#include "SetQtyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetQtyDlg dialog


CSetQtyDlg::CSetQtyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetQtyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetQtyDlg)
	m_strName = _T("");
	m_nQty = 1;
	m_nQty2 = 0;
	m_nQty3 = 0;
	//}}AFX_DATA_INIT

	m_nInGameMax = 1;
}


void CSetQtyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetQtyDlg)
	DDX_Text(pDX, IDC_ITEMNAME, m_strName);
	DDX_Text(pDX, IDC_QTY, m_nQty);
	DDV_MinMaxUInt(pDX, m_nQty, 0, 65535);
	DDX_Text(pDX, IDC_QTY2, m_nQty2);
	DDV_MinMaxUInt(pDX, m_nQty2, 0, 65535);
	DDX_Text(pDX, IDC_QTY3, m_nQty3);
	DDV_MinMaxUInt(pDX, m_nQty3, 0, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetQtyDlg, CDialog)
	//{{AFX_MSG_MAP(CSetQtyDlg)
	ON_BN_CLICKED(IDC_INGAME_MAX, OnIngameMax)
	ON_BN_CLICKED(IDC_MAX_POSSIBLE, OnMaxPossible)
	ON_BN_CLICKED(IDC_INGAME_MAX2, OnIngameMax2)
	ON_BN_CLICKED(IDC_INGAME_MAX3, OnIngameMax3)
	ON_BN_CLICKED(IDC_MAX_POSSIBLE2, OnMaxPossible2)
	ON_BN_CLICKED(IDC_MAX_POSSIBLE3, OnMaxPossible3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetQtyDlg message handlers

void CSetQtyDlg::OnIngameMax() 
{
	m_nQty = m_nInGameMax;
	UpdateData(FALSE);
}

void CSetQtyDlg::OnIngameMax2() 
{
	m_nQty2 = m_nInGameMax;
	UpdateData(FALSE);
}

void CSetQtyDlg::OnIngameMax3() 
{
	m_nQty3 = m_nInGameMax;
	UpdateData(FALSE);
}

void CSetQtyDlg::OnMaxPossible() 
{
	m_nQty = 0xFFFF;
	UpdateData(FALSE);
}

void CSetQtyDlg::OnMaxPossible2() 
{
	m_nQty2 = 0xFFFF;
	UpdateData(FALSE);
}

void CSetQtyDlg::OnMaxPossible3() 
{
	m_nQty3 = 0xFFFF;
	UpdateData(FALSE);
}
