// SpellListCtrl.cpp : implementation file
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
#include "SpellListCtrl.h"
#include "SpellBrowserCom.h"
#include "InfTlk.h"
#include "InfCreature.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpellListCtrl

CSpellListCtrl::CSpellListCtrl()
{
	m_pSpells = NULL;
	m_bLoading = FALSE;
	m_nSort = SPELLSORT_TYP_ASC;
	m_pDescriptionWnd = NULL;
	m_pCountWnd = NULL;
	m_hBitmap = NULL;

	m_bMatchCase = FALSE;
	m_bMatchDescription = FALSE;
	m_bMatchName = FALSE;
	m_strSearchText.Empty();
}

CSpellListCtrl::~CSpellListCtrl()
{
	if (m_hBitmap)
		::DeleteObject(m_hBitmap);
}

void CSpellListCtrl::OnDestroy() 
{
	ClearList();
	CListCtrl::OnDestroy();
}

void CSpellListCtrl::ClearList()
{
	CListCtrlSpellData *pData;
	int nCount = GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pData = (CListCtrlSpellData*)GetItemData(i);
		delete pData;
	}

	DeleteAllItems();
}

BEGIN_MESSAGE_MAP(CSpellListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSpellListCtrl)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpellListCtrl message handlers

void CSpellListCtrl::Init(CWnd *pDescriptionWnd, CWnd *pCountWnd, CBitmapWnd *pBitmapWnd)
{
	DWORD dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	SetExtendedStyle(dwStyle);

	m_pDescriptionWnd = pDescriptionWnd;
	m_pCountWnd = pCountWnd;
	m_pBitmapWnd = pBitmapWnd;

	InsertColumn(0,"Type",LVCFMT_LEFT,65);
	InsertColumn(1,"Lvl",LVCFMT_LEFT,30);
	InsertColumn(2,"Name",LVCFMT_LEFT,1);
	ResizeLastColumn(*this);
	// Off the screen, but still available to see the resource strings.
	InsertColumn(3,"Resource",LVCFMT_LEFT,75);
	InsertColumn(4,"Script Name",LVCFMT_LEFT,200);

	_infKey.GetResList(RESTYPE_SPL,m_pSpells);

	FillList();
}

int CSpellListCtrl::GetSelectedItem()
{
	return(GetNextItem(-1,LVNI_SELECTED));
}

void CSpellListCtrl::SetSelectedItem(int nIndex)
{ 
	SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	UpdateSpellChanged(nIndex);
}

int CALLBACK SpellListComp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrlSpellData *pData1 = (CListCtrlSpellData*)lParam1;
	CListCtrlSpellData *pData2 = (CListCtrlSpellData*)lParam2;

	switch(lParamSort)
	{
		case SPELLSORT_TYP_ASC :
			if (pData1->chType < pData2->chType)
				return(-1);
			else
				if (pData1->chType > pData2->chType)
					return(1);
				else	// If they are the same sort by the next column.
					if (pData1->chLevel < pData2->chLevel)
						return(-1);
					else
						if (pData1->chLevel > pData2->chLevel)
							return(1);
						else	// If they are the same level, sort by name.
							if (pData1->strName < pData2->strName)
								return(-1);
							else
								if (pData1->strName > pData2->strName)
									return(1);

			break;
		case SPELLSORT_TYP_DES :
			if (pData1->chType < pData2->chType)
				return(1);
			else
				if (pData1->chType > pData2->chType)
					return(-1);
				else	// If they are the same sort by the next column.
					if (pData1->chLevel < pData2->chLevel)
						return(-1);
					else
						if (pData1->chLevel > pData2->chLevel)
							return(1);
						else	// If they are the same level, sort by name.
							if (pData1->strName < pData2->strName)
								return(-1);
							else
								if (pData1->strName > pData2->strName)
									return(1);
			break;
		case SPELLSORT_LVL_ASC :
			if (pData1->chLevel < pData2->chLevel)
				return(-1);
			else
				if (pData1->chLevel > pData2->chLevel)
					return(1);
				else	// If they are the same level, sort by name.
					if (pData1->strName < pData2->strName)
						return(-1);
					else
						if (pData1->strName > pData2->strName)
							return(1);
			break;
		case SPELLSORT_LVL_DES :
			if (pData1->chLevel < pData2->chLevel)
				return(1);
			else
				if (pData1->chLevel > pData2->chLevel)
					return(-1);
				else	// If they are the same level, sort by name.
					if (pData1->strName < pData2->strName)
						return(-1);
					else
						if (pData1->strName > pData2->strName)
							return(1);
			break;
		case SPELLSORT_NAM_ASC :
			if (pData1->strName < pData2->strName)
				return(-1);
			else
				if (pData1->strName > pData2->strName)
					return(1);
			break;
		case SPELLSORT_NAM_DES :
			if (pData1->strName < pData2->strName)
				return(1);
			else
				if (pData1->strName > pData2->strName)
					return(-1);
			break;
		case SPELLSORT_RES_ASC :
			if (pData1->pInfo->strName < pData2->pInfo->strName)
				return(-1);
			else
				if (pData1->pInfo->strName > pData2->pInfo->strName)
					return(1);
			break;
		case SPELLSORT_RES_DES :
			if (pData1->pInfo->strName < pData2->pInfo->strName)
				return(1);
			else
				if (pData1->pInfo->strName > pData2->pInfo->strName)
					return(-1);
			break;
		case SPELLSORT_SNM_ASC :
			if (pData1->strScriptName < pData2->strScriptName)
				return(-1);
			else
				if (pData1->strScriptName > pData2->strScriptName)
					return(1);
				else
					if (pData1->pInfo->strName < pData2->pInfo->strName)
						return(-1);
					else
						if (pData1->pInfo->strName > pData2->pInfo->strName)
							return(1);
			break;
		case SPELLSORT_SNM_DES :
			if (pData1->strScriptName < pData2->strScriptName)
				return(1);
			else
				if (pData1->strScriptName > pData2->strScriptName)
					return(-1);
				else
					if (pData1->pInfo->strName < pData2->pInfo->strName)
						return(-1);
					else
						if (pData1->pInfo->strName > pData2->pInfo->strName)
							return(1);
			break;
	}

	return(0);
}

void CSpellListCtrl::FillList()
{
	ASSERT(m_pSpells);

	m_bLoading = TRUE;
	LockWindowUpdate();
	BeginWaitCursor();

	ClearList();

	CString strFind, strSearchLower;
	CString strLevel;
	CString strName;
	CString strCat;
	CString strBifFile;
	CResInfo *pResInfo;
	CInfBifFile *pBifFile;
	CListCtrlSpellData *pData;
	int nIndex;
	INF_SPL spl;

	strSearchLower = m_strSearchText;
	strSearchLower.MakeLower();

	POSITION pos = m_pSpells->GetHeadPosition();
	while(pos)
	{
		pResInfo = (CResInfo*)m_pSpells->GetNext(pos);

		pBifFile = _infKey.GetBifFile(pResInfo);
		if (!pBifFile || !pBifFile->IsValid())
			continue;

		if (pBifFile->GetData(pResInfo->wLocator,&spl) == -1)
			continue;

		if (!_infTlk.GetString(spl.dwGenericSpellName,strName))
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
				if (!_infTlk.GetString(spl.dwSpellDescriptionGeneric,strFind))
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

		pData = new CListCtrlSpellData;
		if (!pData)
		{
			MessageBox("Unable to allocate memory for the spell data.","Out of Memory");
			break;
		}

		pData->pInfo = pResInfo;
		pData->strName = strName;
		pData->chType = 0;
		pData->chLevel = (BYTE)spl.dwSpellLevel;
		pData->strScriptName = GetSpellScriptName(pResInfo->strName);

		strLevel.Format("%d",spl.dwSpellLevel);

		switch(spl.wCategory)
		{
			case 0x01 :
				pData->chType = SPELLCAT_WIZARD;
				strCat = "Wizard";
				break;
			case 0x02 :
				pData->chType = SPELLCAT_PRIEST;
				strCat = "Priest";
				break;
			case 0x04 :
				pData->chType = SPELLCAT_INNATE;
				strCat = "Innate";
				break;
			default :
				pData->chType = SPELLCAT_UNKNOWN;
				strCat = "Unknown";
				break;
		}

		nIndex = InsertItem(0,strCat);
		SetItemText(nIndex,1,strLevel);
		SetItemText(nIndex,2,strName);
		SetItemText(nIndex,3,pResInfo->strName);
		SetItemText(nIndex,4,pData->strScriptName);
		SetItemData(nIndex,(DWORD)pData);
	}

	SortItems(SpellListComp,m_nSort);
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

void CSpellListCtrl::UpdateDescription(INF_SPL &spl)
{
	if (!m_pDescriptionWnd)
		return;

	CString strDesc;
	strDesc.Empty();

	_infTlk.GetString(spl.dwSpellDescriptionGeneric,strDesc);

	CString strNew;
	FixTlkDescription(strDesc,strNew);
	m_pDescriptionWnd->SetWindowText(strNew);
}

void CSpellListCtrl::UpdateIcon(CResInfo *pInfo, INF_SPL &spl)
{
	if (!m_pBitmapWnd)
		return;

	m_pBitmapWnd->SetBitmap(NULL);
	if (m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	if (!GetSpellBitmap(pInfo,spl,GetSysColor(COLOR_BTNFACE),m_hBitmap))
		return;

	// Make sure it is a valid icon size.
	BITMAP bm;
	if (!::GetObject(m_hBitmap,sizeof(bm),&bm))
		return;

	if (bm.bmWidth != 32 || bm.bmHeight != 32)
		return;

	m_pBitmapWnd->SetBitmap(m_hBitmap);
}

void CSpellListCtrl::UpdateSpellChanged(int nIndex)
{
	CListCtrlSpellData *pData = (CListCtrlSpellData*)GetItemData(nIndex);
	CInfBifFile *pSpellFile = _infKey.GetBifFile(pData->pInfo);
	if (!pSpellFile || !pSpellFile->IsValid())
		return;

	INF_SPL spl;
	if (pSpellFile->GetData(pData->pInfo->wLocator,&spl) == -1)
		return;

	UpdateDescription(spl);
	UpdateIcon(pData->pInfo,spl);
}

void CSpellListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	// This gets called while the list is filling. There is no reason
	// to be looking up the descriptions each time.
	if (m_bLoading)
		return;

	UpdateSpellChanged(pNMListView->iItem);
}

void CSpellListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
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
	SortItems(SpellListComp,m_nSort);

	int nIndex = GetSelectedItem();
	if (nIndex != -1)
		EnsureVisible(nIndex,FALSE);
	
	*pResult = 0;
}

void CSpellListCtrl::DisplaySpell(const char *pszResource)
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
		GetItemText(i,3,szRes,14);
		if (!strcmpi(pszResource,szRes))
		{
			SetSelectedItem(i);
			EnsureVisible(i,FALSE);
			return;
		}
	}
}

void CSpellListCtrl::AssignSpell()
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CListCtrlSpellData *pData = (CListCtrlSpellData*)GetItemData(nIndex);
	CInfBifFile *pSpellFile = _infKey.GetBifFile(pData->pInfo);
	if (!pSpellFile || !pSpellFile->IsValid())
		return;

	INF_SPL spl;
	if (pSpellFile->GetData(pData->pInfo->wLocator,&spl)== -1)
		return;

	WORD wActualSpellType;
	switch(pData->chType)
	{
		case SPELLCAT_WIZARD :
			wActualSpellType = INF_CRE_ST_WIZARD;
			break;
		case SPELLCAT_PRIEST :
			wActualSpellType = INF_CRE_ST_PRIEST;
			break;
		case SPELLCAT_INNATE :
			wActualSpellType = INF_CRE_ST_INNATE;
			break;
	}

	_comSpellBrowser.AssignItem(pData->strName,pData->pInfo,&spl,wActualSpellType);
}

void CSpellListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	AssignSpell();
}
