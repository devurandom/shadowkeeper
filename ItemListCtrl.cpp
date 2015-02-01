// ItemListCtrl.cpp : implementation file
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
#include "ItemListCtrl.h"
#include "InfTlk.h"
#include "ItemBrowserCom.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemListCtrl

CItemListCtrl::CItemListCtrl()
{
	m_pItems = NULL;
	m_bLoading = FALSE;
	m_nSort = ITEMSORT_CAT_ASC;
	m_pDescriptionWnd = NULL;
	m_hBitmap = NULL;

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

	m_bMatchCase = FALSE;
	m_bMatchDescription = FALSE;
	m_bMatchName = FALSE;
	m_strSearchText.Empty();
}

CItemListCtrl::~CItemListCtrl()
{
	if (m_hBitmap)
		::DeleteObject(m_hBitmap);
}

void CItemListCtrl::OnDestroy() 
{
	ClearList();
	CListCtrl::OnDestroy();
}

void CItemListCtrl::ClearList()
{
	CListCtrlItemData *pData;
	int nCount = GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pData = (CListCtrlItemData*)GetItemData(i);
		delete pData;
	}

	DeleteAllItems();
}

BEGIN_MESSAGE_MAP(CItemListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CItemListCtrl)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemListCtrl message handlers

void CItemListCtrl::Init(CWnd *pDescriptionWnd, CWnd *pCountWnd, CBitmapWnd *pBitmapWnd)
{
	DWORD dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	SetExtendedStyle(dwStyle);

	m_pDescriptionWnd = pDescriptionWnd;
	m_pCountWnd = pCountWnd;
	m_pBitmapWnd = pBitmapWnd;

	InsertColumn(0,"Type",LVCFMT_LEFT,75);
	InsertColumn(1,"Name",LVCFMT_LEFT,5);
	ResizeLastColumn(*this);
	// Off the screen, but still available to see the resource strings.
	InsertColumn(2,"Resource",LVCFMT_LEFT,75);

	_infKey.GetResList(RESTYPE_ITM,m_pItems);

	FillList();
}

int CItemListCtrl::GetSelectedItem()
{
	return(GetNextItem(-1,LVNI_SELECTED));
}

void CItemListCtrl::SetSelectedItem(int nIndex)
{ 
	SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	UpdateItemChanged(nIndex);
}

int CALLBACK ItemListComp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrlItemData *pData1 = (CListCtrlItemData*)lParam1;
	CListCtrlItemData *pData2 = (CListCtrlItemData*)lParam2;

	switch(lParamSort)
	{
		case ITEMSORT_CAT_ASC :
			if (strcmp(_itemType[pData1->nItemIndex].pszType,_itemType[pData2->nItemIndex].pszType) < 0)
				return(-1);
			else
				if (strcmp(_itemType[pData1->nItemIndex].pszType,_itemType[pData2->nItemIndex].pszType) > 0)
					return(1);
				else	// If they are the same sort by the next column.
					if (pData1->strName < pData2->strName)
						return(-1);
					else
						if (pData1->strName > pData2->strName)
							return(1);
			break;
		case ITEMSORT_CAT_DES :
			if (strcmp(_itemType[pData1->nItemIndex].pszType,_itemType[pData2->nItemIndex].pszType) < 0)
				return(1);
			else
				if (strcmp(_itemType[pData1->nItemIndex].pszType,_itemType[pData2->nItemIndex].pszType) > 0)
					return(-1);
				else	// If they are the same sort by the next column.
					if (pData1->strName < pData2->strName)
						return(-11);
					else
						if (pData1->strName > pData2->strName)
							return(2);
			break;
		case ITEMSORT_NAM_ASC :
			if (pData1->strName < pData2->strName)
				return(-1);
			else
				if (pData1->strName > pData2->strName)
					return(1);
			break;
		case ITEMSORT_NAM_DES :
			if (pData1->strName < pData2->strName)
				return(1);
			else
				if (pData1->strName > pData2->strName)
					return(-1);
			break;
		case ITEMSORT_RES_ASC :
			if (pData1->pInfo->strName < pData2->pInfo->strName)
				return(-1);
			else
				if (pData1->pInfo->strName > pData2->pInfo->strName)
					return(1);
			break;
		case ITEMSORT_RES_DES :
			if (pData1->pInfo->strName < pData2->pInfo->strName)
				return(1);
			else
				if (pData1->pInfo->strName > pData2->pInfo->strName)
					return(-1);
			break;
	}

	return(0);
}

void CItemListCtrl::FillList()
{
	ASSERT(m_pItems);

	m_bLoading = TRUE;
	LockWindowUpdate();
	BeginWaitCursor();

	ClearList();

	CString strFind;
	CString strSearchLower;
	CString strType;
	CString strName;
	CString strBifFile;
	int nItemIndex;
	CResInfo *pResInfo;
	CInfBifFile *pBifFile;
	CListCtrlItemData *pData;
	int nIndex;
	INF_ITM itm;

	strSearchLower = m_strSearchText;
	strSearchLower.MakeLower();

	POSITION pos = m_pItems->GetHeadPosition();
	while(pos)
	{
		pResInfo = (CResInfo*)m_pItems->GetNext(pos);

		pBifFile = _infKey.GetBifFile(pResInfo);
		if (!pBifFile || !pBifFile->IsValid())
		{
			TRACE("GetBifFile failed for item resource: %s\n",(const char *)pResInfo->strName);
			continue;
		}

		if (pBifFile->GetData(pResInfo->wLocator,&itm) == -1)
		{
			TRACE("GetData failed for item resource: %s\n",(const char *)pResInfo->strName);
			continue;
		}

		else if (itm.wCategory == ITEMTYPE_ARMOR && !m_bArmor)
			continue;
		else if (itm.wCategory == ITEMTYPE_ARROWS && !m_bArrows)
			continue;
		else if (itm.wCategory == ITEMTYPE_AXE && !m_bAxes)
			continue;
		else if (itm.wCategory == ITEMTYPE_BELT && !m_bBelts)
			continue;
		else if (itm.wCategory == ITEMTYPE_BOLTS && !m_bBolts)
			continue;
		else if (itm.wCategory == ITEMTYPE_BOOK && !m_bBooks)
			continue;
		else if (itm.wCategory == ITEMTYPE_BOOTS && !m_bBoots)
			continue;
		else if (itm.wCategory == ITEMTYPE_BOW && !m_bBows)
			continue;
		else if (itm.wCategory == ITEMTYPE_BULLETS && !m_bBullets)
			continue;
		else if (itm.wCategory == ITEMTYPE_CLOAK && !m_bCloaks)
			continue;
		else if (itm.wCategory == ITEMTYPE_CONTAINER && !m_bContainers)
			continue;
		else if (itm.wCategory == ITEMTYPE_CROSSBOW && !m_bCrossbows)
			continue;
		else if (itm.wCategory == ITEMTYPE_DAGGER && !m_bDaggers)
			continue;
		else if (itm.wCategory == ITEMTYPE_DART && !m_bDarts)
			continue;
		else if (itm.wCategory == ITEMTYPE_FAMILIAR && !m_bFamiliars)
			continue;
		else if (itm.wCategory == ITEMTYPE_FLAIL && !m_bFlails)
			continue;
		else if (itm.wCategory == ITEMTYPE_FOOD && !m_bFood)
			continue;
		else if (itm.wCategory == ITEMTYPE_GEM && !m_bGems)
			continue;
		else if (itm.wCategory == ITEMTYPE_GLOVES && !m_bGloves)
			continue;
		else if (itm.wCategory == ITEMTYPE_HALBERD && !m_bHalberds)
			continue;
		else if (itm.wCategory == ITEMTYPE_HAMMER && !m_bHammers)
			continue;
		else if (itm.wCategory == ITEMTYPE_HANDTOHAND && !m_bHandToHand)
			continue;
		else if (itm.wCategory == ITEMTYPE_HELM && !m_bHelms)
			continue;
		else if (itm.wCategory == ITEMTYPE_KEY && !m_bKeys)
			continue;
		else if (itm.wCategory == ITEMTYPE_MACE && !m_bMaces)
			continue;
		else if (itm.wCategory == ITEMTYPE_MISC && !m_bMisc)
			continue;
		else if (itm.wCategory == ITEMTYPE_MORNINGSTAR && !m_bMorningStars)
			continue;
		else if (itm.wCategory == ITEMTYPE_NECKLACE && !m_bNecklaces)
			continue;
		else if (itm.wCategory == ITEMTYPE_POTION && !m_bPotions)
			continue;
		else if (itm.wCategory == ITEMTYPE_RING && !m_bRings)
			continue;
		else if (itm.wCategory == ITEMTYPE_SCROLL && !m_bScrolls)
			continue;
		else if (itm.wCategory == ITEMTYPE_SHIELD && !m_bShields)
			continue;
		else if (itm.wCategory == ITEMTYPE_SLING && !m_bSlings)
			continue;
		else if (itm.wCategory == ITEMTYPE_SPEAR && !m_bSpears)
			continue;
		else if (itm.wCategory == ITEMTYPE_STAFF && !m_bStaves)
			continue;
		else if (itm.wCategory == ITEMTYPE_SHORTSWORD && !m_bShortSwords)
			continue;
		else if (itm.wCategory == ITEMTYPE_SWORD && !m_bSwords)
			continue;
		else if (itm.wCategory == ITEMTYPE_WAND && !m_bWands)
			continue;

		if (!_infTlk.GetString(itm.dwIdentifiedItemName,strName))
			if (!_infTlk.GetString(itm.dwGenericItemName,strName))
				strName = "<No Name>";

		if (!m_strSearchText.IsEmpty())
		{
			if (m_bMatchName)
			{
				if (m_bMatchCase)
				{
					if (strName.Find(m_strSearchText) == -1)
						continue;
				}
				else
				{
					strFind = strName;
					strFind.MakeLower();
					if (strFind.Find(strSearchLower) == -1)
						continue;
				}
			}

			if (m_bMatchDescription)
			{
				if (!_infTlk.GetString(itm.dwItemDescriptionIdentified,strFind))
					continue;
				if (m_bMatchCase)
				{
					if (strFind.Find(m_strSearchText) == -1)
						continue;
				}
				else
				{
					strFind.MakeLower();
					if (strFind.Find(strSearchLower) == -1)
						continue;
				}
			}
		}

		GetItemType(itm.wCategory,strType,nItemIndex);

		pData = new CListCtrlItemData;
		if (!pData)
		{
			MessageBox("Unable to allocate memory for the item data.","Out of Memory");
			break;
		}

		pData->pInfo = pResInfo;
		pData->strName = strName;
		pData->nItemIndex = nItemIndex;

		nIndex = InsertItem(0,strType);
		SetItemText(nIndex,1,strName);
		SetItemText(nIndex,2,pResInfo->strName);
		SetItemData(nIndex,(DWORD)pData);
	}

	SortItems(ItemListComp,m_nSort);
	UnlockWindowUpdate();
	if (m_pCountWnd)
	{
		CString strCount;
		strCount.Format("%d",GetItemCount());
		m_pCountWnd->SetWindowText(strCount);
	}
	m_bLoading = FALSE;
	EndWaitCursor();
}

void CItemListCtrl::UpdateDescription(INF_ITM &itm)
{
	if (!m_pDescriptionWnd)
		return;

	CString strDesc;
	strDesc.Empty();
	if (itm.dwItemDescriptionIdentified)
		_infTlk.GetString(itm.dwItemDescriptionIdentified,strDesc);

	if (strDesc.IsEmpty())
		_infTlk.GetString(itm.dwItemDescriptionGeneric,strDesc);

	CString strNew;
	FixTlkDescription(strDesc,strNew);
	m_pDescriptionWnd->SetWindowText(strNew);
}

void CItemListCtrl::UpdateIcon(CResInfo *pInfo, INF_ITM &itm)
{
	if (!m_pBitmapWnd)
		return;

	m_pBitmapWnd->SetBitmap(NULL);
	if (m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	// For scrolls, the 2nd frame is the generic rolled up scroll image, change
	// it to display the spell icon image.
	int nFrame;
	if (itm.wCategory == ITEMTYPE_SCROLL)
		nFrame = 0;
	else
		nFrame = 1;

	if (!GetItemBitmap(pInfo,itm,nFrame,GetSysColor(COLOR_BTNFACE),m_hBitmap))
		return;

	m_pBitmapWnd->SetBitmap(m_hBitmap);
}

void CItemListCtrl::UpdateItemChanged(int nIndex)
{
	CListCtrlItemData *pData = (CListCtrlItemData*)GetItemData(nIndex);
	CInfBifFile *pItemFile = _infKey.GetBifFile(pData->pInfo);
	if (!pItemFile || !pItemFile->IsValid())
		return;

	INF_ITM itm;
	if (pItemFile->GetData(pData->pInfo->wLocator,&itm) == -1)
		return;

	UpdateDescription(itm);
	UpdateIcon(pData->pInfo,itm);
}

void CItemListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	// This gets called while the list is filling. There is no reason
	// to be looking up the descriptions each time.
	if (m_bLoading)
		return;

	UpdateItemChanged(pNMListView->iItem);
}

void CItemListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nSortCol = m_nSort / 2;
	int nMod = m_nSort % 2;

	if (nSortCol == pNMListView->iSubItem)
	{
		// If in the same column reverse the sort order.
		if (nMod)
			nMod = 0;
		else
			nMod = 1;
	}
	else
	{
		nSortCol = pNMListView->iSubItem;
		nMod = 0;
	}

	m_nSort = nSortCol*2 + nMod;
	SortItems(ItemListComp,m_nSort);

	int nIndex = GetSelectedItem();
	if (nIndex != -1)
		EnsureVisible(nIndex,FALSE);
	
	*pResult = 0;
}

void CItemListCtrl::DisplayItem(const char *pszResource)
{
	// First make sure that the browser is visible and the active document.
	ASSERT(AfxGetMainWnd());
	ASSERT(GetParentFrame());
	CMDIFrameWnd *pMainFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	pMainFrame->MDIActivate(GetParentFrame());

	// If there is no resource string then don't both looking. It's probably
	// just the view wanting to pick an item for an already empty slot.
	if (!*pszResource)
		return;

	char szRes[15];
	int nCount = GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		GetItemText(i,2,szRes,14);
		if (!strcmpi(pszResource,szRes))
		{
			SetSelectedItem(i);
			EnsureVisible(i,FALSE);
			return;
		}
	}
}

void CItemListCtrl::AssignItem()
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CListCtrlItemData *pData = (CListCtrlItemData*)GetItemData(nIndex);
	CInfBifFile *pItemFile = _infKey.GetBifFile(pData->pInfo);
	if (!pItemFile || !pItemFile->IsValid())
		return;

	INF_ITM itm;
	if (pItemFile->GetData(pData->pInfo->wLocator,&itm) == -1)
		return;

	_comItemBrowser.AssignItem(pData->strName,*pData->pInfo,&itm);
}

void CItemListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	AssignItem();
}
