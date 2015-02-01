// ItemFilterDlg.cpp : implementation file
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
#include "ItemFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemFilterDlg dialog


CItemFilterDlg::CItemFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemFilterDlg)
	m_bArmor = FALSE;
	m_bArrows = FALSE;
	m_bAxes = FALSE;
	m_bBelts = FALSE;
	m_bBolts = FALSE;
	m_bBooks = FALSE;
	m_bBoots = FALSE;
	m_bBows = FALSE;
	m_bBullets = FALSE;
	m_bCloaks = FALSE;
	m_bContainers = FALSE;
	m_bCrossbows = FALSE;
	m_bDaggers = FALSE;
	m_bDarts = FALSE;
	m_bFamiliars = FALSE;
	m_bFood = FALSE;
	m_bGems = FALSE;
	m_bGloves = FALSE;
	m_bHalberds = FALSE;
	m_bHammers = FALSE;
	m_bHandToHand = FALSE;
	m_bHelms = FALSE;
	m_bKeys = FALSE;
	m_bMaces = FALSE;
	m_bMisc = FALSE;
	m_bMorningStars = FALSE;
	m_bNecklaces = FALSE;
	m_bPotions = FALSE;
	m_bRings = FALSE;
	m_bScrolls = FALSE;
	m_bShields = FALSE;
	m_bSlings = FALSE;
	m_bSpears = FALSE;
	m_bStaves = FALSE;
	m_bShortSwords = FALSE;
	m_bSwords = FALSE;
	m_bWands = FALSE;
	m_bFlails = FALSE;
	m_bMatchCase = FALSE;
	m_bMatchDescription = FALSE;
	m_bMatchName = FALSE;
	m_strSearchText = _T("");
	//}}AFX_DATA_INIT
}


void CItemFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemFilterDlg)
	DDX_Check(pDX, IDC_VIEWARMOR, m_bArmor);
	DDX_Check(pDX, IDC_VIEWARROWS, m_bArrows);
	DDX_Check(pDX, IDC_VIEWAXES, m_bAxes);
	DDX_Check(pDX, IDC_VIEWBELT, m_bBelts);
	DDX_Check(pDX, IDC_VIEWBOLTS, m_bBolts);
	DDX_Check(pDX, IDC_VIEWBOOKS, m_bBooks);
	DDX_Check(pDX, IDC_VIEWBOOTS, m_bBoots);
	DDX_Check(pDX, IDC_VIEWBOWS, m_bBows);
	DDX_Check(pDX, IDC_VIEWBULLETS, m_bBullets);
	DDX_Check(pDX, IDC_VIEWCLOAKS, m_bCloaks);
	DDX_Check(pDX, IDC_VIEWCONTAINERS, m_bContainers);
	DDX_Check(pDX, IDC_VIEWCROSSBOWS, m_bCrossbows);
	DDX_Check(pDX, IDC_VIEWDAGGERS, m_bDaggers);
	DDX_Check(pDX, IDC_VIEWDARTS, m_bDarts);
	DDX_Check(pDX, IDC_VIEWFAMILIARS, m_bFamiliars);
	DDX_Check(pDX, IDC_VIEWFOOD, m_bFood);
	DDX_Check(pDX, IDC_VIEWGEMS, m_bGems);
	DDX_Check(pDX, IDC_VIEWGLOVES, m_bGloves);
	DDX_Check(pDX, IDC_VIEWHALBERDS, m_bHalberds);
	DDX_Check(pDX, IDC_VIEWHAMMERS, m_bHammers);
	DDX_Check(pDX, IDC_VIEWHANDTOHAND, m_bHandToHand);
	DDX_Check(pDX, IDC_VIEWHELMS, m_bHelms);
	DDX_Check(pDX, IDC_VIEWKEYS, m_bKeys);
	DDX_Check(pDX, IDC_VIEWMACES, m_bMaces);
	DDX_Check(pDX, IDC_VIEWMISC, m_bMisc);
	DDX_Check(pDX, IDC_VIEWMORNINGSTARS, m_bMorningStars);
	DDX_Check(pDX, IDC_VIEWNECKLACES, m_bNecklaces);
	DDX_Check(pDX, IDC_VIEWPOTIONS, m_bPotions);
	DDX_Check(pDX, IDC_VIEWRINGS, m_bRings);
	DDX_Check(pDX, IDC_VIEWSCROLLS, m_bScrolls);
	DDX_Check(pDX, IDC_VIEWSHIELDS, m_bShields);
	DDX_Check(pDX, IDC_VIEWSLINGS, m_bSlings);
	DDX_Check(pDX, IDC_VIEWSPEARS, m_bSpears);
	DDX_Check(pDX, IDC_VIEWSTAVES, m_bStaves);
	DDX_Check(pDX, IDC_VIEWSHORTSWORDS, m_bShortSwords);
	DDX_Check(pDX, IDC_VIEWSWORDS, m_bSwords);
	DDX_Check(pDX, IDC_VIEWWANDS, m_bWands);
	DDX_Check(pDX, IDC_VIEWFLAILS, m_bFlails);
	DDX_Check(pDX, IDC_MATCH_CASE, m_bMatchCase);
	DDX_Check(pDX, IDC_MATCH_DESCRIPTION, m_bMatchDescription);
	DDX_Check(pDX, IDC_MATCH_ITEMNAME, m_bMatchName);
	DDX_Text(pDX, IDC_SEARCHTEXT, m_strSearchText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CItemFilterDlg)
	ON_BN_CLICKED(IDC_DESEL_ALL, OnDeselAll)
	ON_BN_CLICKED(IDC_SEL_ALL, OnSelAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemFilterDlg message handlers

void CItemFilterDlg::OnDeselAll() 
{
	m_bArmor = FALSE;
	m_bArrows = FALSE;
	m_bAxes = FALSE;
	m_bBelts = FALSE;
	m_bBolts = FALSE;
	m_bBooks = FALSE;
	m_bBoots = FALSE;
	m_bBows = FALSE;
	m_bBullets = FALSE;
	m_bCloaks = FALSE;
	m_bContainers = FALSE;
	m_bCrossbows = FALSE;
	m_bDaggers = FALSE;
	m_bDarts = FALSE;
	m_bFamiliars = FALSE;
	m_bFlails = FALSE;
	m_bFood = FALSE;
	m_bGems = FALSE;
	m_bGloves = FALSE;
	m_bHalberds = FALSE;
	m_bHammers = FALSE;
	m_bHandToHand = FALSE;
	m_bHelms = FALSE;
	m_bKeys = FALSE;
	m_bMaces = FALSE;
	m_bMisc = FALSE;
	m_bMorningStars = FALSE;
	m_bNecklaces = FALSE;
	m_bPotions = FALSE;
	m_bRings = FALSE;
	m_bScrolls = FALSE;
	m_bShields = FALSE;
	m_bSlings = FALSE;
	m_bSpears = FALSE;
	m_bStaves = FALSE;
	m_bShortSwords = FALSE;
	m_bSwords = FALSE;
	m_bWands = FALSE;

	UpdateData(FALSE);
}

void CItemFilterDlg::OnSelAll() 
{
	m_bArmor = TRUE;
	m_bArrows = TRUE;
	m_bAxes = TRUE;
	m_bBelts = TRUE;
	m_bBolts = TRUE;
	m_bBooks = TRUE;
	m_bBoots = TRUE;
	m_bBows = TRUE;
	m_bBullets = TRUE;
	m_bCloaks = TRUE;
	m_bContainers = TRUE;
	m_bCrossbows = TRUE;
	m_bDaggers = TRUE;
	m_bDarts = TRUE;
	m_bFamiliars = TRUE;
	m_bFlails = TRUE;
	m_bFood = TRUE;
	m_bGems = TRUE;
	m_bGloves = TRUE;
	m_bHalberds = TRUE;
	m_bHammers = TRUE;
	m_bHandToHand = TRUE;
	m_bHelms = TRUE;
	m_bKeys = TRUE;
	m_bMaces = TRUE;
	m_bMisc = TRUE;
	m_bMorningStars = TRUE;
	m_bNecklaces = TRUE;
	m_bPotions = TRUE;
	m_bRings = TRUE;
	m_bScrolls = TRUE;
	m_bShields = TRUE;
	m_bSlings = TRUE;
	m_bSpears = TRUE;
	m_bStaves = TRUE;
	m_bShortSwords = TRUE;
	m_bSwords = TRUE;
	m_bWands = TRUE;

	UpdateData(FALSE);
}
