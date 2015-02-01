// SpellBrowserView.cpp : implementation file
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
#include "SpellBrowserCom.h"
#include "SpellBrowserView.h"
#include "InfCreature.h"
#include "InfTlk.h"
#include "Global.h"
#include "ExportOptionsDlg.h"
#include "ExportResourceDlg.h"
#include "SpellFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpellBrowserView

IMPLEMENT_DYNCREATE(CSpellBrowserView, CFormView)

CSpellBrowserView::CSpellBrowserView()
	: CFormView(CSpellBrowserView::IDD)
{
	//{{AFX_DATA_INIT(CSpellBrowserView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSpellBrowserView::~CSpellBrowserView()
{
}

void CSpellBrowserView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpellBrowserView)
	DDX_Control(pDX, IDC_FILTERSPELLS, m_btnFilter);
	DDX_Control(pDX, IDC_SPELLPROMPT, m_stSpellPrompt);
	DDX_Control(pDX, IDC_EXPORTSPELLS, m_btnExportList);
	DDX_Control(pDX, IDC_EXPORTSPELLRES, m_btnExportSpell);
	DDX_Control(pDX, IDC_ASSIGNSPELL, m_btnAssign);
	DDX_Control(pDX, IDC_SPELLCOUNT, m_stSpellCount);
	DDX_Control(pDX, IDC_SPELL_DESC, m_edDescription);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpellBrowserView, CFormView)
	//{{AFX_MSG_MAP(CSpellBrowserView)
	ON_BN_CLICKED(IDC_ASSIGNSPELL, OnAssignspell)
	ON_BN_CLICKED(IDC_EXPORTSPELLS, OnExportspells)
	ON_BN_CLICKED(IDC_EXPORTSPELLRES, OnExportspellres)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_FILTERSPELLS, OnFilterspells)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpellBrowserView diagnostics

#ifdef _DEBUG
void CSpellBrowserView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSpellBrowserView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpellBrowserView message handlers

void CSpellBrowserView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	ResizeParentToFit(FALSE);	
	GetParentFrame()->SetWindowText("Spell Browser");

	m_lcSpells.SubclassDlgItem(IDC_SPELLLIST,this);

	CRect rSpells;
	m_lcSpells.GetWindowRect(&rSpells);
	ScreenToClient(&rSpells);

	CRect rBitmap;
	rBitmap.left = rSpells.right + 15;
	rBitmap.right = rBitmap.left + 34;
	rBitmap.top = 10;
	rBitmap.bottom = rBitmap.top + 34;

	m_wndBitmap.Create(rBitmap,this,1000);

	m_lcSpells.Init(&m_edDescription,&m_stSpellCount,&m_wndBitmap);

	UpdateAssignButton();

	CRect rClient;
	GetClientRect(&rClient);
	OnSize(0,rClient.Width(),rClient.Height());
}

void CSpellBrowserView::UpdateAssignButton()
{
	if (_comSpellBrowser.HasSelection())
	{
		m_btnAssign.EnableWindow(TRUE);

		CString strText;
		strText.Format("Assign to %s.",_comSpellBrowser.GetCharName());
		m_btnAssign.SetWindowText(strText);
	}
	else
	{ 
		m_btnAssign.EnableWindow(FALSE);
		m_btnAssign.SetWindowText("[No Character Selected]");
	}
}

void CSpellBrowserView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CHintString *phs;

	switch(lHint)
	{
		case HINT_SBC_CHANGESELECTED :
			UpdateAssignButton();
			break;
		case HINT_SBC_DISPLAYSPELL :
			GetParentFrame()->ShowWindow(SW_SHOWNORMAL);
			phs = (CHintString*)pHint;
			m_lcSpells.DisplaySpell(phs->str);
			break;
		case HINT_CHANGE_GRIDLINES :
			UpdateGridLines();
			break;
	}
}

void CSpellBrowserView::OnAssignspell() 
{
	m_lcSpells.AssignSpell();	
}

void CSpellBrowserView::OnExportspells() 
{
	CExportOptionsDlg d;

	d.m_strFilename = "SpellList.txt";
	if (d.DoModal() == IDCANCEL)
		return;

	CStdioFile file;
	if (!file.Open(d.m_strFilename,CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		AfxMessageBox(CString("Unable to create file: ")+d.m_strFilename);
		return;
	}

	AfxGetApp()->BeginWaitCursor();

	CListCtrlSpellData *pData;
	CInfBifFile *pSpellFile;
	CString strLine;
	int nLen;
	int j;
	INF_SPL spl;
	int nCount = m_lcSpells.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pData = (CListCtrlSpellData*)m_lcSpells.GetItemData(i);
		if (!pData)
			continue;

		strLine.Format("%s,%d,%s,%s\n",
			(const char *)m_lcSpells.GetItemText(i,0),
			pData->chLevel,
			(const char *)pData->strName,
			(const char *)pData->pInfo->strName);
		file.WriteString(strLine);

		if (d.m_bDescriptions)
		{
			pSpellFile = _infKey.GetBifFile(pData->pInfo);
			if (!pSpellFile || !pSpellFile->IsValid())
			{
				file.WriteString("   No description.\n\n");
				continue;
			}

			if (pSpellFile->GetData(pData->pInfo->wLocator,&spl) == -1)
			{
				file.WriteString("   No description.\n\n");
				continue;
			}

			_infTlk.GetString(spl.dwSpellDescriptionGeneric,strLine);
			if (!strLine.IsEmpty())
			{
				nLen = strLine.GetLength();
				for (j=0;j<nLen;j++)
					if (strLine.GetAt(j) == '\r')
						strLine.SetAt(j,' ');
				file.WriteString("--- [Description] ---\n");
				file.WriteString(strLine);
				file.WriteString("\n---------------------\n\n");
			}
			else
			{
				file.WriteString("   No description.\n\n");
				continue;
			}
		}
	}

	file.Close();

	AfxGetApp()->EndWaitCursor();

	AfxMessageBox(CString("Spell list exported as: ")+d.m_strFilename);
}

void CSpellBrowserView::UpdateGridLines()
{
	DWORD dwStyle = m_lcSpells.GetExtendedStyle();

	if (!_bUseGridLines)
		dwStyle &= (~LVS_EX_GRIDLINES);
	else
		dwStyle |= LVS_EX_GRIDLINES;

	m_lcSpells.SetExtendedStyle(dwStyle);
}

void CSpellBrowserView::OnExportspellres() 
{
	int nIndex = m_lcSpells.GetSelectedItem();
	if (nIndex == -1)
		return;

	CListCtrlSpellData *pData = (CListCtrlSpellData*)m_lcSpells.GetItemData(nIndex);
	ASSERT(pData);
	// When fetching the file, be sure and use the bif key index. This function finds
	// the file that contains the resource internally. If you user the CResInfo* version
	// of GetBifFile and the resource is overriden, you'll end up with a pointer
	// to the override file.
	CInfBifFile *pSpellFile = _infKey.GetBifFile(pData->pInfo->wBifKeyIndex);
	if (!pSpellFile || !pSpellFile->IsValid())
	{
		AfxMessageBox("Unable to locate resource file.");
		return;
	}
	int nOffset, nSize;
	if (!pSpellFile->GetDataOffsetAndSize(pData->pInfo,nOffset,nSize))
	{
		AfxMessageBox("Unable to locate the resource in the file.");
		return;
	}

	CExportResourceDlg d;
	d.m_strFilename = pData->pInfo->strName + ".SPL";
	if (d.DoModal() == IDOK)
	{
		BYTE *pSpell = new BYTE[nSize];
		if (!pSpell)
		{
			AfxMessageBox("Unable to allocate memory for the spell.");
			return;
		}

		if (pSpellFile->GetData(pData->pInfo,pSpell,nSize) == -1)
		{
			delete [] pSpell;
			AfxMessageBox("Unable to read the spell resource.");
			return;
		}

		CFile file;
		if (!file.Open(d.m_strFilename,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
		{
			delete [] pSpell;
			AfxMessageBox("Unable to create the file.");
			return;
		}

		file.Write(pSpell,nSize);
		file.Close();

		delete [] pSpell;
		MessageBox("Spell exported. It can be found in your override folder.","Spell Exported");
	}
}

void CSpellBrowserView::OnSize(UINT nType, int cx, int cy) 
{
	// Make sure they all exist...
	if (!m_lcSpells.GetSafeHwnd() || !m_stSpellPrompt.GetSafeHwnd() ||
		 !m_btnExportList.GetSafeHwnd() || !m_btnExportSpell.GetSafeHwnd() ||
		 !m_btnAssign.GetSafeHwnd() || !m_stSpellCount.GetSafeHwnd() || !m_edDescription.GetSafeHwnd() ||
		 !m_btnFilter.GetSafeHwnd())
		return;

	CRect rAssign;
	m_btnAssign.GetWindowRect(&rAssign);
	CSize szAssign(rAssign.Width(),rAssign.Height());

	rAssign.left = 0;
	rAssign.right = cx;
	rAssign.bottom = cy;
	rAssign.top = rAssign.bottom - szAssign.cy;
	m_btnAssign.MoveWindow(&rAssign);

	CRect rDesc;
	rDesc.left = 0;
	rDesc.right = cx;
	rDesc.top = (long)((cy-szAssign.cy) * .66); // Take up 1/3 of the space left after button.
	rDesc.bottom = rAssign.top-1;
	m_edDescription.MoveWindow(&rDesc);

	CRect rList;
	rList.left = 0;
	rList.right = cx - SBV_MARGIN_RIGHT;
	rList.top = 0;
	rList.bottom = rDesc.top-1;
	m_lcSpells.MoveWindow(&rList);

	CRect rPrompt;
	m_stSpellPrompt.GetWindowRect(&rPrompt);
	CSize szPrompt(rPrompt.Width(),rPrompt.Height());

	rPrompt.left = rList.right+7;
	rPrompt.right = rPrompt.left + szPrompt.cx;
	rPrompt.bottom = rDesc.top - 5;
	rPrompt.top = rPrompt.bottom - szPrompt.cy;
	m_stSpellPrompt.MoveWindow(&rPrompt);

	CRect rCount;
	rCount.left = rPrompt.right + 2;
	rCount.right = cx;
	rCount.top = rPrompt.top;
	rCount.bottom = rPrompt.bottom;
	m_stSpellCount.MoveWindow(&rCount);

	CRect rExportSpell;
	m_btnExportList.GetWindowRect(&rExportSpell);
	CSize szExport(rExportSpell.Width(),rExportSpell.Height());

	CRect rFilter;
	rFilter.left = rList.right+7;
	rFilter.right = rFilter.left+szExport.cx;
	rFilter.bottom = rPrompt.top - 10 - szExport.cy*2 - 10;
	rFilter.top = rFilter.bottom - szExport.cy;
	m_btnFilter.MoveWindow(&rFilter);

	rExportSpell.left = rList.right+7;
	rExportSpell.right = rExportSpell.left+szExport.cx;
	rExportSpell.bottom = rPrompt.top - 10;
	rExportSpell.top = rExportSpell.bottom - szExport.cy;
	m_btnExportSpell.MoveWindow(&rExportSpell);

	CRect rExportList;
	rExportList.left = rList.right+7;
	rExportList.right = rExportList.left+szExport.cx;
	rExportList.bottom = rExportSpell.top - 1;
	rExportList.top = rExportList.bottom - szExport.cy;
	m_btnExportList.MoveWindow(&rExportList);

	CRect rIcon;
	rIcon.left = rList.right+7;
	rIcon.right = rIcon.left + 34;
	rIcon.top = 7;
	rIcon.bottom = rIcon.top + 34;
	m_wndBitmap.MoveWindow(&rIcon);
}

BOOL CSpellBrowserView::OnEraseBkgnd(CDC* pDC) 
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

void CSpellBrowserView::OnFilterspells() 
{
	CSpellFilterDlg d;

	d.m_bMatchCase = m_lcSpells.m_bMatchCase;
	d.m_bMatchDescription = m_lcSpells.m_bMatchDescription;
	d.m_bMatchName = m_lcSpells.m_bMatchName;
	d.m_strSearchText = m_lcSpells.m_strSearchText;
	if (d.DoModal() == IDOK)
	{
		m_lcSpells.m_bMatchCase = d.m_bMatchCase;
		m_lcSpells.m_bMatchDescription = d.m_bMatchDescription;
		m_lcSpells.m_bMatchName = d.m_bMatchName;
		m_lcSpells.m_strSearchText = d.m_strSearchText;

		m_lcSpells.FillList();
	}
}
