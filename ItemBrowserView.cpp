// ItemBrowserView.cpp : implementation file
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
#include "ItemBrowserView.h"
#include "ItemBrowserCom.h"
#include "Global.h"
#include "ItemFilterDlg.h"
#include "InfTlk.h"
#include "ExportOptionsDlg.h"
#include "ExportResourceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemBrowserView

IMPLEMENT_DYNCREATE(CItemBrowserView, CFormView)

CItemBrowserView::CItemBrowserView()
	: CFormView(CItemBrowserView::IDD)
{
	//{{AFX_DATA_INIT(CItemBrowserView)
	//}}AFX_DATA_INIT
}

CItemBrowserView::~CItemBrowserView()
{
}

void CItemBrowserView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemBrowserView)
	DDX_Control(pDX, IDC_FILTERITEMS, m_btnFilter);
	DDX_Control(pDX, IDC_EXPORTITEMRES, m_btnExportItem);
	DDX_Control(pDX, IDC_EXPORTITEMS, m_btnExportList);
	DDX_Control(pDX, IDC_COUNTPROMPT, m_stCountPrompt);
	DDX_Control(pDX, IDC_ASSIGNITEM, m_btnAssign);
	DDX_Control(pDX, IDC_ITEM_DESC, m_edDescription);
	DDX_Control(pDX, IDC_ITEMCOUNT, m_stItemCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemBrowserView, CFormView)
	//{{AFX_MSG_MAP(CItemBrowserView)
	ON_BN_CLICKED(IDC_ASSIGNITEM, OnAssignitem)
	ON_BN_CLICKED(IDC_FILTERITEMS, OnFilteritems)
	ON_BN_CLICKED(IDC_EXPORTITEMS, OnExportitems)
	ON_BN_CLICKED(IDC_EXPORTITEMRES, OnExportitemres)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemBrowserView diagnostics

#ifdef _DEBUG
void CItemBrowserView::AssertValid() const
{
	CFormView::AssertValid();
}

void CItemBrowserView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CItemBrowserView message handlers

void CItemBrowserView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit(FALSE);	

	GetParentFrame()->SetWindowText("Item Browser");

	CRect rPrompt;
	m_stCountPrompt.GetWindowRect(&rPrompt);
	ScreenToClient(&rPrompt);

	CRect rBitmap;
	rBitmap.left = rPrompt.left;
	rBitmap.right = rBitmap.left + 66;
	rBitmap.top = 10;
	rBitmap.bottom = rBitmap.top + 66;

	m_wndBitmap.Create(rBitmap,this,1000);

	UpdateAssignButton();

	m_lcItems.SubclassDlgItem(IDC_ITEMLIST,this);
	m_lcItems.Init(&m_edDescription,&m_stItemCount,&m_wndBitmap);

	CRect rClient;
	GetClientRect(&rClient);
	OnSize(0,rClient.Width(),rClient.Height());
}

void CItemBrowserView::UpdateAssignButton()
{
	if (_comItemBrowser.HasSelection())
	{
		m_btnAssign.EnableWindow(TRUE);

		CString strText;
		strText.Format("Assign to %s [%s].",_comItemBrowser.GetCharName(),_itemPos[_comItemBrowser.GetPos()]);
		m_btnAssign.SetWindowText(strText);
	}
	else
	{
		m_btnAssign.EnableWindow(FALSE);
		m_btnAssign.SetWindowText("[No Character Selected]");
	}
}

void CItemBrowserView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CHintString *phs;

	switch(lHint)
	{
		case HINT_IBC_CHANGESELECTED :
			UpdateAssignButton();
			break;
		case HINT_IBC_DISPLAYITEM :
			GetParentFrame()->ShowWindow(SW_SHOWNORMAL);
			phs = (CHintString*)pHint;
			m_lcItems.DisplayItem(phs->str);
			break;
		case HINT_CHANGE_GRIDLINES :
			UpdateGridLines();
			break;
	}
}

void CItemBrowserView::OnAssignitem() 
{
	m_lcItems.AssignItem();	
}

void CItemBrowserView::OnFilteritems() 
{
	CItemFilterDlg d;
	
	d.m_bArmor = m_lcItems.m_bArmor;
	d.m_bArrows = m_lcItems.m_bArrows;
	d.m_bAxes = m_lcItems.m_bAxes;
	d.m_bBelts = m_lcItems.m_bBelts;
	d.m_bBolts = m_lcItems.m_bBolts;
	d.m_bBooks = m_lcItems.m_bBooks;
	d.m_bBoots = m_lcItems.m_bBoots;
	d.m_bBows = m_lcItems.m_bBows;
	d.m_bBullets = m_lcItems.m_bBullets;
	d.m_bCloaks = m_lcItems.m_bCloaks;
	d.m_bContainers = m_lcItems.m_bContainers;
	d.m_bCrossbows = m_lcItems.m_bCrossbows;
	d.m_bDaggers = m_lcItems.m_bDaggers;
	d.m_bDarts = m_lcItems.m_bDarts;
	d.m_bFamiliars = m_lcItems.m_bFamiliars;
	d.m_bFlails = m_lcItems.m_bFlails;
	d.m_bFood = m_lcItems.m_bFood;
	d.m_bGems = m_lcItems.m_bGems;
	d.m_bGloves = m_lcItems.m_bGloves;
	d.m_bHalberds = m_lcItems.m_bHalberds;
	d.m_bHammers = m_lcItems.m_bHammers;
	d.m_bHandToHand = m_lcItems.m_bHandToHand;
	d.m_bHelms = m_lcItems.m_bHelms;
	d.m_bKeys = m_lcItems.m_bKeys;
	d.m_bMaces = m_lcItems.m_bMaces;
	d.m_bMisc = m_lcItems.m_bMisc;
	d.m_bMorningStars = m_lcItems.m_bMorningStars;
	d.m_bNecklaces = m_lcItems.m_bNecklaces;
	d.m_bPotions = m_lcItems.m_bPotions;
	d.m_bRings = m_lcItems.m_bRings;
	d.m_bScrolls = m_lcItems.m_bScrolls;
	d.m_bShields = m_lcItems.m_bShields;
	d.m_bSlings = m_lcItems.m_bSlings;
	d.m_bSpears = m_lcItems.m_bSpears;
	d.m_bStaves = m_lcItems.m_bStaves;
	d.m_bShortSwords = m_lcItems.m_bShortSwords;
	d.m_bSwords = m_lcItems.m_bSwords;
	d.m_bWands = m_lcItems.m_bWands;
	d.m_bMatchCase = m_lcItems.m_bMatchCase;
	d.m_bMatchDescription = m_lcItems.m_bMatchDescription;
	d.m_bMatchName = m_lcItems.m_bMatchName;
	d.m_strSearchText = m_lcItems.m_strSearchText;

	if (d.DoModal() == IDOK)
	{
		m_lcItems.m_bArmor = d.m_bArmor;
		m_lcItems.m_bArrows = d.m_bArrows;
		m_lcItems.m_bAxes = d.m_bAxes;
		m_lcItems.m_bBelts = d.m_bBelts;
		m_lcItems.m_bBolts = d.m_bBolts;
		m_lcItems.m_bBooks = d.m_bBooks;
		m_lcItems.m_bBoots = d.m_bBoots;
		m_lcItems.m_bBows = d.m_bBows;
		m_lcItems.m_bBullets = d.m_bBullets;
		m_lcItems.m_bCloaks = d.m_bCloaks;
		m_lcItems.m_bContainers = d.m_bContainers;
		m_lcItems.m_bCrossbows = d.m_bCrossbows;
		m_lcItems.m_bDaggers = d.m_bDaggers;
		m_lcItems.m_bDarts = d.m_bDarts;
		m_lcItems.m_bFamiliars = d.m_bFamiliars;
		m_lcItems.m_bFlails = d.m_bFlails;
		m_lcItems.m_bFood = d.m_bFood;
		m_lcItems.m_bGems = d.m_bGems;
		m_lcItems.m_bGloves = d.m_bGloves;
		m_lcItems.m_bHalberds = d.m_bHalberds;
		m_lcItems.m_bHammers = d.m_bHammers;
		m_lcItems.m_bHandToHand = d.m_bHandToHand;
		m_lcItems.m_bHelms = d.m_bHelms;
		m_lcItems.m_bKeys = d.m_bKeys;
		m_lcItems.m_bMaces = d.m_bMaces;
		m_lcItems.m_bMisc = d.m_bMisc;
		m_lcItems.m_bMorningStars = d.m_bMorningStars;
		m_lcItems.m_bNecklaces = d.m_bNecklaces;
		m_lcItems.m_bPotions = d.m_bPotions;
		m_lcItems.m_bRings = d.m_bRings;
		m_lcItems.m_bScrolls = d.m_bScrolls;
		m_lcItems.m_bShields = d.m_bShields;
		m_lcItems.m_bSlings = d.m_bSlings;
		m_lcItems.m_bSpears = d.m_bSpears;
		m_lcItems.m_bStaves = d.m_bStaves;
		m_lcItems.m_bShortSwords = d.m_bShortSwords;
		m_lcItems.m_bSwords = d.m_bSwords;
		m_lcItems.m_bWands = d.m_bWands;
		m_lcItems.m_bMatchCase = d.m_bMatchCase;
		m_lcItems.m_bMatchDescription = d.m_bMatchDescription;
		m_lcItems.m_bMatchName = d.m_bMatchName;
		m_lcItems.m_strSearchText = d.m_strSearchText;
		
		m_lcItems.FillList();
	}
}

void CItemBrowserView::OnExportitems() 
{
	CExportOptionsDlg d;

	d.m_strFilename = "ItemList.txt";
	if (d.DoModal() == IDCANCEL)
		return;

	CStdioFile file;
	if (!file.Open(d.m_strFilename,CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		AfxMessageBox(CString("Unable to create file: ")+d.m_strFilename);
		return;
	}

	AfxGetApp()->BeginWaitCursor();

	CListCtrlItemData *pData;
	CInfBifFile *pItemFile;
	int nLen;
	int j;
	INF_ITM itm;
	CString strLine;
	int nCount = m_lcItems.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pData = (CListCtrlItemData*)m_lcItems.GetItemData(i);
		if (!pData)
			continue;

		strLine.Format("%s,%s,%s\n",
			(const char *)m_lcItems.GetItemText(i,0),
			(const char *)pData->strName,
			(const char *)pData->pInfo->strName);
		file.WriteString(strLine);

		if (d.m_bDescriptions)
		{
			pItemFile = _infKey.GetBifFile(pData->pInfo);
			if (!pItemFile || !pItemFile->IsValid())
			{
				file.WriteString("   No description.\n\n");
				continue;
			}

			if (pItemFile->GetData(pData->pInfo->wLocator,&itm) == -1)
			{
				file.WriteString("   No description.\n\n");
				continue;
			}

			if (itm.dwItemDescriptionIdentified)
				_infTlk.GetString(itm.dwItemDescriptionIdentified,strLine);
			if (strLine.IsEmpty())
				_infTlk.GetString(itm.dwItemDescriptionGeneric,strLine);

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

	AfxMessageBox("Item list exported as: ItemList.txt");
}

void CItemBrowserView::UpdateGridLines()
{
	DWORD dwStyle = m_lcItems.GetExtendedStyle();

	if (!_bUseGridLines)
		dwStyle &= (~LVS_EX_GRIDLINES);
	else
		dwStyle |= LVS_EX_GRIDLINES;

	m_lcItems.SetExtendedStyle(dwStyle);
}

void CItemBrowserView::OnExportitemres() 
{
	int nIndex = m_lcItems.GetSelectedItem();
	if (nIndex == -1)
		return;

	CListCtrlItemData *pData = (CListCtrlItemData*)m_lcItems.GetItemData(nIndex);
	ASSERT(pData);
	// When fetching the file, be sure and use the bif key index. This function finds
	// the file that contains the resource internally. If you user the CResInfo* version
	// of GetBifFile and the resource is overriden, you'll end up with a pointer
	// to the override file.
	CInfBifFile *pItemFile = _infKey.GetBifFile(pData->pInfo->wBifKeyIndex);
	if (!pItemFile || !pItemFile->IsValid())
	{
		AfxMessageBox("Unable to locate resource file.");
		return;
	}
	int nOffset, nSize;
	if (!pItemFile->GetDataOffsetAndSize(pData->pInfo,nOffset,nSize))
	{
		AfxMessageBox("Unable to locate the resource in the file.");
		return;
	}

	CExportResourceDlg d;
	d.m_strFilename = pData->pInfo->strName + ".ITM";
	if (d.DoModal() == IDOK)
	{
		BYTE *pItem = new BYTE[nSize];
		if (!pItem)
		{
			AfxMessageBox("Unable to allocate memory for the item.");
			return;
		}

		if (pItemFile->GetData(pData->pInfo,pItem,nSize) == -1)
		{
			delete [] pItem;
			AfxMessageBox("Unable to read the item resource.");
			return;
		}

		CFile file;
		if (!file.Open(d.m_strFilename,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
		{
			delete [] pItem;
			AfxMessageBox("Unable to create the file.");
			return;
		}

		file.Write(pItem,nSize);
		file.Close();

		delete [] pItem;
		MessageBox("Item exported. It can be found in your override folder.","Item Exported");
	}
}

void CItemBrowserView::OnSize(UINT nType, int cx, int cy) 
{
	// Make sure they all exist...
	if (!m_lcItems.GetSafeHwnd() || !m_stCountPrompt.GetSafeHwnd() ||
		 !m_btnExportList.GetSafeHwnd() || !m_btnExportItem.GetSafeHwnd() ||
		 !m_btnAssign.GetSafeHwnd() || !m_stItemCount.GetSafeHwnd() || !m_edDescription.GetSafeHwnd())
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
	rList.right = cx - IBV_MARGIN_RIGHT;
	rList.top = 0;
	rList.bottom = rDesc.top-1;
	m_lcItems.MoveWindow(&rList);

	CRect rPrompt;
	m_stCountPrompt.GetWindowRect(&rPrompt);
	CSize szPrompt(rPrompt.Width(),rPrompt.Height());

	rPrompt.left = rList.right+7;
	rPrompt.right = rPrompt.left + szPrompt.cx;
	rPrompt.bottom = rDesc.top - 5;
	rPrompt.top = rPrompt.bottom - szPrompt.cy;
	m_stCountPrompt.MoveWindow(&rPrompt);

	CRect rCount;
	rCount.left = rPrompt.right + 2;
	rCount.right = cx;
	rCount.top = rPrompt.top;
	rCount.bottom = rPrompt.bottom;
	m_stItemCount.MoveWindow(&rCount);

	CRect rExportItem;
	m_btnExportList.GetWindowRect(&rExportItem);
	CSize szExport(rExportItem.Width(),rExportItem.Height());

	rExportItem.left = rList.right+7;
	rExportItem.right = rExportItem.left+szExport.cx;
	rExportItem.bottom = rPrompt.top - 10;
	rExportItem.top = rExportItem.bottom - szExport.cy;
	m_btnExportItem.MoveWindow(&rExportItem);

	CRect rExportList;
	rExportList.left = rList.right+7;
	rExportList.right = rExportItem.right;
	rExportList.bottom = rExportItem.top - 1;
	rExportList.top = rExportList.bottom - szExport.cy;
	m_btnExportList.MoveWindow(&rExportList);

	CRect rFilterList;
	rFilterList.left = rList.right+7;
	rFilterList.right = rExportItem.right;
	rFilterList.bottom = rExportList.top-7;
	rFilterList.top = rFilterList.bottom - szExport.cy;
	m_btnFilter.MoveWindow(&rFilterList);

	CRect rIcon;
	rIcon.left = rList.right+7;
	rIcon.right = rIcon.left + 66;
	rIcon.top = 7;
	rIcon.bottom = rIcon.top + 66;
	m_wndBitmap.MoveWindow(&rIcon);
}

BOOL CItemBrowserView::OnEraseBkgnd(CDC* pDC) 
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

