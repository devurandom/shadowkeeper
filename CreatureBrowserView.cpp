// CreatureBrowserView.cpp : implementation file
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
#include "CreatureBrowserView.h"
#include "Global.h"
#include "CreatureFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreatureBrowserView

IMPLEMENT_DYNCREATE(CCreatureBrowserView, CFormView)

CCreatureBrowserView::CCreatureBrowserView()
	: CFormView(CCreatureBrowserView::IDD)
{
	//{{AFX_DATA_INIT(CCreatureBrowserView)
	//}}AFX_DATA_INIT
}

CCreatureBrowserView::~CCreatureBrowserView()
{
}

void CCreatureBrowserView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreatureBrowserView)
	DDX_Control(pDX, IDC_FILTER_CREATURES, m_btnFilter);
	DDX_Control(pDX, IDC_DELETE_CRE, m_btnDelete);
	DDX_Control(pDX, IDC_REFRESH_CRE, m_btnRefresh);
	DDX_Control(pDX, IDC_EXTRACTCRE_AS, m_btnExtractAs);
	DDX_Control(pDX, IDC_EXTRACT_CREATURE, m_btnExtract);
	DDX_Control(pDX, IDC_EDIT_CREATURE, m_btnEdit);
	DDX_Control(pDX, IDC_CREATURE_PROMPT, m_stCreaturePrompt);
	DDX_Control(pDX, IDC_CREATURE_COUNT, m_stCreatureCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreatureBrowserView, CFormView)
	//{{AFX_MSG_MAP(CCreatureBrowserView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_EDIT_CREATURE, OnEditCreature)
	ON_BN_CLICKED(IDC_REFRESH_CRE, OnRefreshCre)
	ON_BN_CLICKED(IDC_EXTRACTCRE_AS, OnExtractcreAs)
	ON_BN_CLICKED(IDC_EXTRACT_CREATURE, OnExtractCreature)
	ON_BN_CLICKED(IDC_DELETE_CRE, OnDeleteCre)
	ON_BN_CLICKED(IDC_FILTER_CREATURES, OnFilterCreatures)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreatureBrowserView diagnostics

#ifdef _DEBUG
void CCreatureBrowserView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCreatureBrowserView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCreatureBrowserView message handlers

void CCreatureBrowserView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit(FALSE);	
	
	GetParentFrame()->SetWindowText("Creature Browser");

	m_lcCreatures.SubclassDlgItem(IDC_CREATURE_LIST,this);
	m_lcCreatures.Init(&m_stCreatureCount);
	
	CRect rClient;
	GetClientRect(&rClient);
	OnSize(0,rClient.Width(),rClient.Height());
}

void CCreatureBrowserView::OnSize(UINT nType, int cx, int cy) 
{
	if (!m_lcCreatures.GetSafeHwnd() || !m_stCreatureCount.GetSafeHwnd() ||
		 !m_stCreaturePrompt.GetSafeHwnd() || !m_btnEdit.GetSafeHwnd() ||
		 !m_btnExtract.GetSafeHwnd() || !m_btnRefresh.GetSafeHwnd() ||
		 !m_btnDelete.GetSafeHwnd() || !m_btnFilter.GetSafeHwnd())
		return;

	CRect rButtonSize;
	m_btnEdit.GetWindowRect(&rButtonSize);

	CRect rPromptSize;
	m_stCreaturePrompt.GetWindowRect(&rPromptSize);

	CRect rEdit;
	rEdit.top = 5;
	rEdit.bottom = rEdit.top + rButtonSize.Height();
	rEdit.right = cx-5;
	rEdit.left = rEdit.right - rButtonSize.Width();
	m_btnEdit.MoveWindow(&rEdit,TRUE);

	CRect rExtract;
	rExtract.left = rEdit.left;
	rExtract.right = rEdit.right;
	rExtract.top = rEdit.bottom + 1;
	rExtract.bottom = rExtract.top + rButtonSize.Height();
	m_btnExtract.MoveWindow(&rExtract,TRUE);

	CRect rExtractAs;
	rExtractAs.left = rEdit.left;
	rExtractAs.right = rEdit.right;
	rExtractAs.top = rExtract.bottom + 1;
	rExtractAs.bottom = rExtractAs.top + rButtonSize.Height();
	m_btnExtractAs.MoveWindow(&rExtractAs,TRUE);

	CRect rRefresh;
	rRefresh.left = rEdit.left;
	rRefresh.right = rEdit.right;
	rRefresh.top = rExtractAs.bottom + 1;
	rRefresh.bottom = rRefresh.top + rButtonSize.Height();
	m_btnRefresh.MoveWindow(&rRefresh,TRUE);

	CRect rDelete;
	rDelete.left = rEdit.left;
	rDelete.right = rEdit.right;
	rDelete.top = rRefresh.bottom + 1;
	rDelete.bottom = rDelete.top + rButtonSize.Height();
	m_btnDelete.MoveWindow(&rDelete,TRUE);

	CRect rFilter;
	rFilter.left = rEdit.left;
	rFilter.right = rEdit.right;
	rFilter.top = rDelete.bottom + 20;
	rFilter.bottom = rFilter.top + rButtonSize.Height();
	m_btnFilter.MoveWindow(&rFilter,TRUE);

	CRect rList;
	rList.left = 0;
	rList.top = 0;
	rList.right = rEdit.left - 5;
	rList.bottom = cy;
	m_lcCreatures.MoveWindow(&rList,TRUE);

	CRect rCountPrompt;
	rCountPrompt.left = rEdit.left;
	rCountPrompt.bottom = cy - 5;
	rCountPrompt.top = rCountPrompt.bottom - rPromptSize.Height();
	rCountPrompt.right = rCountPrompt.left + rPromptSize.Width();
	m_stCreaturePrompt.MoveWindow(&rCountPrompt,TRUE);

	CRect rCount;
	rCount.left = rCountPrompt.right + 5;
	rCount.top = rCountPrompt.top;
	rCount.bottom = rCountPrompt.bottom;
	rCount.right = cx - 5;
	m_stCreatureCount.MoveWindow(&rCount,TRUE);
		
}

BOOL CCreatureBrowserView::OnEraseBkgnd(CDC* pDC) 
{
	// This is not a perfect solution, but it prevents much of the flickering as the
	// controls are being moved around. The controls themselves will still flicker 
	// as they redraw themselves. This pervents the controls being covered by the
	// erasing of the background which is really noticable and ugly.
	CRect rExclude;
	CWnd *pWnd = GetWindow(GW_CHILD);
	while(pWnd)
	{
		if (pWnd->IsWindowVisible())
		{
			pWnd->GetWindowRect(&rExclude);
			ScreenToClient(&rExclude);
			pDC->ExcludeClipRect(&rExclude);
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	} 

	return CFormView::OnEraseBkgnd(pDC);
}

void CCreatureBrowserView::OnEditCreature() 
{
	m_lcCreatures.EditSelection();
}

void CCreatureBrowserView::OnRefreshCre() 
{
	_infKey.RefreshOverride();
	m_lcCreatures.OverrideChanged();
}

void CCreatureBrowserView::OnExtractcreAs() 
{
	m_lcCreatures.ExtractSelectionAs();
}

void CCreatureBrowserView::OnExtractCreature() 
{
	m_lcCreatures.ExtractSelection();
}

void CCreatureBrowserView::OnDeleteCre() 
{
	m_lcCreatures.DeleteSelection();
}

void CCreatureBrowserView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch(lHint)
	{
		case HINT_CHANGE_GRIDLINES :
			UpdateGridLines();
			break;
		case HINT_OVERRIDE_CHANGED :
			m_lcCreatures.OverrideChanged();
			break;
	}
}

void CCreatureBrowserView::UpdateGridLines()
{
	DWORD dwStyle = m_lcCreatures.GetExtendedStyle();

	if (!_bUseGridLines)
		dwStyle &= (~LVS_EX_GRIDLINES);
	else
		dwStyle |= LVS_EX_GRIDLINES;

	m_lcCreatures.SetExtendedStyle(dwStyle);
}


void CCreatureBrowserView::OnFilterCreatures() 
{
	CCreatureFilterDlg d;

	d.m_strSearchText = m_lcCreatures.m_strSearchText;
	d.m_bMatchCase = m_lcCreatures.m_bMatchCase;
	if (d.DoModal() == IDOK)
	{
		m_lcCreatures.m_strSearchText = d.m_strSearchText;
		m_lcCreatures.m_bMatchCase = d.m_bMatchCase;

		m_lcCreatures.FillList();
	}
	
}
