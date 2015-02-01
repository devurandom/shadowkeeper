// TabSpellsDlg.cpp : implementation file
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
#include "TabSpellsDlg.h"
#include "SpellBitmaps.h"
#include "InfBifFile.h"
#include "InfKey.h"
#include "InfTlk.h"
#include "Inf2DA.h"
#include "SpellBrowserCom.h"
#include "Global.h"
#include "AddAbilitiesDlg.h"
#include "ChangesMadeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSpellsDlg dialog


CTabSpellsDlg::CTabSpellsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabSpellsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabSpellsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_hAccel = NULL;
	m_nSort = TABSPELLSORT_LVL_ASC;
	m_pParentView = NULL;
	m_bInnate = FALSE;
}

CTabSpellsDlg::~CTabSpellsDlg()
{
}

void CTabSpellsDlg::OnDestroy() 
{
	ClearList();
	CDialog::OnDestroy();
}

void CTabSpellsDlg::ClearList()
{
	CTabSpellData *pData;
	int nCount = m_lcSpells.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pData = (CTabSpellData*)m_lcSpells.GetItemData(i);
		delete pData;
	}

	m_lcSpells.DeleteAllItems();
}

void CTabSpellsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabSpellsDlg)
	DDX_Control(pDX, IDC_KIT_INNATES, m_btnInnate);
	DDX_Control(pDX, IDC_MEM_INC, m_btnInc);
	DDX_Control(pDX, IDC_MEM_DEC, m_btnDec);
	DDX_Control(pDX, IDC_CLEAR_SPELL, m_btnClear);
	DDX_Control(pDX, IDC_CHANGESPELL, m_btnChange);
	DDX_Control(pDX, IDC_SPELL_LIST, m_lcSpells);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabSpellsDlg, CDialog)
	//{{AFX_MSG_MAP(CTabSpellsDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SPELL_LIST, OnItemchangedSpellList)
	ON_BN_CLICKED(IDC_CLEAR_SPELL, OnClearSpell)
	ON_BN_CLICKED(IDC_CHANGESPELL, OnChangespell)
	ON_NOTIFY(NM_DBLCLK, IDC_SPELL_LIST, OnDblclkSpellList)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SPELL_LIST, OnColumnclickSpellList)
	ON_NOTIFY(NM_SETFOCUS, IDC_SPELL_LIST, OnSetfocusSpellList)
	ON_BN_CLICKED(IDC_MEM_INC, OnMemInc)
	ON_BN_CLICKED(IDC_MEM_DEC, OnMemDec)
	ON_BN_CLICKED(IDC_KIT_INNATES, OnKitInnates)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSpellsDlg message handlers

BOOL CTabSpellsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabSpellsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	DWORD dwStyle = m_lcSpells.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcSpells.SetExtendedStyle(dwStyle);

	if (m_bInnate)
		m_btnInnate.ShowWindow(SW_SHOW);
	else
		m_btnInnate.ShowWindow(SW_HIDE);

	m_lcSpells.InsertColumn(0,"",LVCFMT_LEFT,38);
	m_lcSpells.InsertColumn(1,"Level",LVCFMT_LEFT,40);
	m_lcSpells.InsertColumn(2,"xMem",LVCFMT_LEFT,40);
	m_lcSpells.InsertColumn(3,"Spell",LVCFMT_LEFT,75);
	ResizeLastColumn(m_lcSpells);
	m_lcSpells.InsertColumn(4,"Resource",LVCFMT_LEFT,75);

	m_lcSpells.SetImageList(&_spellBitmaps.m_ilSpells,LVSIL_SMALL);

	return TRUE; 
}

int CALLBACK SpellTabListComp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTabSpellData *pData1 = (CTabSpellData*)lParam1;
	CTabSpellData *pData2 = (CTabSpellData*)lParam2;

	switch(lParamSort)
	{
		case TABSPELLSORT_LVL_ASC :
			if (pData1->chLevel < pData2->chLevel)
				return(-1);
			else
				if (pData1->chLevel > pData2->chLevel)
					return(1);
				else
					if (pData1->strName < pData2->strName)
						return(-1);
					else
						if (pData1->strName > pData2->strName)
							return(1);
			break;
		case TABSPELLSORT_LVL_DES :
			if (pData1->chLevel < pData2->chLevel)
				return(1);
			else
				if (pData1->chLevel > pData2->chLevel)
					return(-1);
				else
					if (pData1->strName < pData2->strName)
						return(-1);
					else
						if (pData1->strName > pData2->strName)
							return(1);
			break;
		case TABSPELLSORT_MEM_ASC :
			if (pData1->chMem < pData2->chMem)
				return(-1);
			else
				if (pData1->chMem > pData2->chMem)
					return(1);
				else
					if (pData1->chLevel < pData2->chLevel)
						return(-1);
					else
						if (pData1->chLevel > pData2->chLevel)
							return(1);
						else
							if (pData1->strName < pData2->strName)
								return(-1);
							else
								if (pData1->strName > pData2->strName)
									return(1);
			break;
		case TABSPELLSORT_MEM_DES :
			if (pData1->chMem < pData2->chMem)
				return(1);
			else
				if (pData1->chMem > pData2->chMem)
					return(-1);
				else
					if (pData1->chLevel < pData2->chLevel)
						return(-1);
					else
						if (pData1->chLevel > pData2->chLevel)
							return(1);
						else
							if (pData1->strName < pData2->strName)
								return(-1);
							else
								if (pData1->strName > pData2->strName)
									return(1);
			break;
		case TABSPELLSORT_NAM_ASC :
			if (pData1->strName < pData2->strName)
				return(-1);
			else
				if (pData1->strName > pData2->strName)
					return(1);
			break;
		case TABSPELLSORT_NAM_DES :
			if (pData1->strName < pData2->strName)
				return(1);
			else
				if (pData1->strName > pData2->strName)
					return(-1);
			break;
		case TABSPELLSORT_RES_ASC :
			if (pData1->pInfo->strName < pData2->pInfo->strName)
				return(-1);
			else
				if (pData1->pInfo->strName > pData2->pInfo->strName)
					return(1);
			break;
		case TABSPELLSORT_RES_DES :
			if (pData1->pInfo->strName < pData2->pInfo->strName)
				return(1);
			else
				if (pData1->pInfo->strName > pData2->pInfo->strName)
					return(-1);
			break;
	}

	return(0);
}

BOOL CTabSpellsDlg::AddSpellToList(SPELLDATA *pData)
{
	CTabSpellData *pTabData;
	CResInfo *pResInfo;
	CInfBifFile *pBifFile;
	CString strSpell, strLevel, strMem;
	INF_SPL spl;
	char szSpellIcon[10];
	int nIconIndex;
	int nIndex;

	memset(szSpellIcon,0,10);
	pResInfo = _infKey.GetResInfo(RESTYPE_SPL,pData->szResName);
	if (pResInfo)
	{
		pBifFile = _infKey.GetBifFile(pResInfo);
		if (pBifFile && pBifFile->IsValid() && pBifFile->GetData(pResInfo->wLocator,&spl) != -1)
		{
			_infTlk.GetString(spl.dwGenericSpellName,strSpell);
			memcpy(szSpellIcon,spl.chSpellIcon,8);
			nIconIndex = _spellBitmaps.GetImageIndex(szSpellIcon);
		}
		else
			return(FALSE);
	}
	else
		return(FALSE);

	nIndex = m_lcSpells.InsertItem(0,"",nIconIndex);
	if (nIndex == -1)
	{
		AfxMessageBox("Unable to add a spell to the list.");
		UpdateButtonStates();
		return(FALSE);
	}
	strLevel.Format("%d",pData->wLevel+1);
	strMem.Format("%d",pData->nTimesMemorized);

	m_lcSpells.SetItemText(nIndex,1,strLevel);
	m_lcSpells.SetItemText(nIndex,2,strMem);
	m_lcSpells.SetItemText(nIndex,3,strSpell);
	m_lcSpells.SetItemText(nIndex,4,pData->szResName);

	pTabData = new CTabSpellData;
	pTabData->chLevel = (BYTE)pData->wLevel;
	pTabData->chMem = (BYTE)pData->nTimesMemorized;
	pTabData->chTimesCanCast = (BYTE)pData->nTimesCanCast;
	pTabData->strName = strSpell;
	pTabData->pInfo = pResInfo;

	m_lcSpells.SetItemData(nIndex,(DWORD)pTabData);

	return(TRUE);
}

void CTabSpellsDlg::SetSpells(WORD wType, int nSpellCount, SPELLDATA *pData)
{
	ClearList();
	m_wSpellType = wType;

	CString strNotFound;
	for (int i=0;i<nSpellCount;i++)
	{
		if (!AddSpellToList(pData+i))
			if (strNotFound.IsEmpty())
				strNotFound += pData[i].szResName;
			else
				strNotFound += CString(", ") + pData[i].szResName;
	}
	m_lcSpells.SortItems(SpellTabListComp,m_nSort);

	SetSelectedItem(0);
	UpdateButtonStates();

	if (!strNotFound.IsEmpty())
	{
		CString strMessage;
		strMessage.Format("Some spells assigned to this character were not found in the "
			"database. If you save this character those spells will not be saved.\r\n\r\n"
			"Cannot Find: %s",(const char *)strNotFound);
		MessageBox(strMessage,"Unknown Spells",MB_ICONINFORMATION);
	}
}

void CTabSpellsDlg::UpdateButtonStates()
{
	ASSERT(GetParentFrame());

	int nIndex = GetSelectedItem();
	m_btnChange.EnableWindow(TRUE);
	if (nIndex == -1)
	{
		m_btnInc.EnableWindow(FALSE);
		m_btnDec.EnableWindow(FALSE);
		m_btnClear.EnableWindow(FALSE);
	}
	else
	{
		m_btnInc.EnableWindow(TRUE);
		m_btnDec.EnableWindow(TRUE);
		m_btnClear.EnableWindow(TRUE);
	}
	_comSpellBrowser.SetSelected(m_strGameName,m_strCharName,m_pParentView);
}


void CTabSpellsDlg::OnItemchangedSpellList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	UpdateButtonStates();
}

void CTabSpellsDlg::OnClearSpell() 
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		if (MessageBox("Are you sure you want to clear all the spells?","Clear All",MB_ICONQUESTION|MB_YESNO) == IDYES)
		{
			ClearList();
			return;
		}
	}


	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CTabSpellData *pData = (CTabSpellData*)m_lcSpells.GetItemData(nIndex);
	delete pData;

	m_lcSpells.DeleteItem(nIndex);
}

void CTabSpellsDlg::OnChangespell() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
	{
		_comSpellBrowser.DisplayItem("");
	}
	else
	{
		CString strRes;
		strRes = m_lcSpells.GetItemText(nIndex,4);
		_comSpellBrowser.DisplayItem(strRes);
	}
}

void CTabSpellsDlg::OnDblclkSpellList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	OnChangespell();
}


void CTabSpellsDlg::OnColumnclickSpellList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	int nColClicked = pNMListView->iSubItem;

	// Don't do anythign if they click the icon column.
	if (nColClicked == 0)
		return;

	int nSortCol = m_nSort / 2;
	int nMod = m_nSort % 2;

	if (nSortCol == nColClicked)
	{
		// If in the same column reverse the sort order.
		if (nMod)
			nMod = 0;
		else
			nMod = 1;
	}
	else
	{
		nSortCol = nColClicked;
		nMod = 0;
	}

	m_nSort = nSortCol*2 + nMod;
	m_lcSpells.SortItems(SpellTabListComp,m_nSort);

	int nIndex = GetSelectedItem();
	if (nIndex != -1)
		m_lcSpells.EnsureVisible(nIndex,FALSE);
}

void CTabSpellsDlg::OnSetfocusSpellList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	_comSpellBrowser.SetSelected(m_strGameName,m_strCharName,m_pParentView);
	*pResult = 0;
}

void CTabSpellsDlg::AssignSpell(CHintSpell *pHs)
{
	// If the view does its job correctly, this should never happen.
	if (pHs->wSpellType != m_wSpellType)
	{
		MessageBox("The spell type you selected cannot be assigned to this list.","Invalid Spell Type",MB_ICONINFORMATION);
		return;
	}

	// Make sure this spell isn't already known.
	CTabSpellData *pTabData;
	CString strRes;
	int nKnownCount = 0;
	int nCount = m_lcSpells.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pTabData = (CTabSpellData*)m_lcSpells.GetItemData(i);

		// When counting, need to ignore the level on an innate and just count it.
		if (m_wSpellType == INF_CRE_ST_INNATE)
			nKnownCount++;
		else
			if (pTabData->chLevel == pHs->spl.dwSpellLevel-1)
				nKnownCount++;

		strRes = m_lcSpells.GetItemText(i,4);
		if (!strcmpi(strRes,pHs->pResInfo->strName))
		{
			CString strMessage;
			strMessage.Format("%s already knows this spell.",(const char *)m_strCharName);
			MessageBox(strMessage,"Known Spell");
			return;
		}
	}

	if (_bUseKnownSpellLimit && nKnownCount >= _nKnownSpellLimit)
	{
		AfxMessageBox("You have reached the limit for known spells of this level.");
		return;
	}

	CString strSpell, strLevel;
	char szSpellIcon[10];
	int nIconIndex;
	int nIndex;
	memset(szSpellIcon,0,10);

	_infTlk.GetString(pHs->spl.dwGenericSpellName,strSpell);
	memcpy(szSpellIcon,pHs->spl.chSpellIcon,8);
	nIconIndex = _spellBitmaps.GetImageIndex(szSpellIcon);

	nIndex = m_lcSpells.InsertItem(0,"",nIconIndex);
	if (nIndex == -1)
	{
		AfxMessageBox("Unable to add spells to the list.");
		UpdateButtonStates();
		return;
	}

	int nSpellLevel = (m_wSpellType == INF_CRE_ST_INNATE ? 1 : pHs->spl.dwSpellLevel);

	// Innates have spell levels, but only room for level 1 innates in the CRE memorized
	// info. Some of the game assigned innates are actually higher level according to
	// the SPL data. So I'm setting anything that is an innate back to the first level.
	strLevel.Format("%d",nSpellLevel);

	m_lcSpells.SetItemText(nIndex,1,strLevel);
	m_lcSpells.SetItemText(nIndex,2,"0");
	m_lcSpells.SetItemText(nIndex,3,strSpell);
	m_lcSpells.SetItemText(nIndex,4,pHs->pResInfo->strName);

	pTabData = new CTabSpellData;
	pTabData->chLevel = (BYTE)nSpellLevel-1; // Expecting level-1 like storedin CRE.
	pTabData->chMem = 0;
	pTabData->chTimesCanCast = 0;
	pTabData->strName = strSpell;
	pTabData->pInfo = pHs->pResInfo;

	m_lcSpells.SetItemData(nIndex,(DWORD)pTabData);
	m_lcSpells.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

	m_lcSpells.SortItems(SpellTabListComp,m_nSort);
	m_lcSpells.EnsureVisible(GetSelectedItem(),FALSE);
	UpdateButtonStates();
}


void CTabSpellsDlg::OnMemInc() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CTabSpellData *pListData;
	CTabSpellData *pData = (CTabSpellData*)m_lcSpells.GetItemData(nIndex);

	if (_bUseMemSpellLimit)
	{
		int nMem = 0;
		int nCount = m_lcSpells.GetItemCount();
		for (int i=0;i<nCount;i++)
		{
			pListData = (CTabSpellData*)m_lcSpells.GetItemData(i);

			if (pData->chLevel == pListData->chLevel)
				nMem += pListData->chMem;
		}													 

		if (nMem >= _nMemSpellLimit)
		{
			CString strMessage;
			strMessage.Format("%s has reached the maximum number of spells memorizable for this level.",(const char*)m_strCharName);
			AfxMessageBox(strMessage);
			return;
		}
	}

	if (pData->chMem < TABSPELL_MAXMEM)
	{
		pData->chMem++;
		pData->chTimesCanCast = pData->chMem;
		CString strMem;
		strMem.Format("%d",pData->chMem);
		m_lcSpells.SetItemText(nIndex,2,strMem);
	}
}

void CTabSpellsDlg::OnMemDec() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CTabSpellData *pData = (CTabSpellData*)m_lcSpells.GetItemData(nIndex);

	if (pData->chMem > 0)
	{
		pData->chMem--;
		pData->chTimesCanCast = pData->chMem;
		CString strMem;
		strMem.Format("%d",pData->chMem);
		m_lcSpells.SetItemText(nIndex,2,strMem);
	}
}

void CTabSpellsDlg::GetSpells(SPELLDATA *pData)
{
	// Resort it back to level ascending order.
	m_lcSpells.SortItems(SpellTabListComp,TABSPELLSORT_LVL_ASC);

	CTabSpellData *pSpellData;

	int nCount = m_lcSpells.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pSpellData = (CTabSpellData*)m_lcSpells.GetItemData(i);

		pData[i].wLevel = pSpellData->chLevel;
		pData[i].wType = m_wSpellType;
		strcpy(pData[i].szResName,pSpellData->pInfo->strName);
		pData[i].nTimesCanCast = pSpellData->chTimesCanCast;
		pData[i].nTimesMemorized = pSpellData->chMem;
	}
}

BOOL CTabSpellsDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// If ENTER or ESC is hit on any of the controls on a dialog, they send the messages
	// back to the parent dialog. This causes the dialog to close itself down if the
	// IDOK and IDCANCEL messages are not mapped. I was putting the OK and Cancel buttons
	// on the dialogs and hiding them, then mapping the messages and doing nothing, but
	// this is a much easier solution.
	if (wParam == IDOK || wParam == IDCANCEL)
		return(TRUE);
	
	return CDialog::OnCommand(wParam, lParam);
}

void CTabSpellsDlg::OnKitInnates() 
{
	CAddAbilitiesDlg d;
	
	if (d.DoModal() != IDOK)
		return;

	// Grab the 2DA data.
	CString strResAbility;
	if (!_2daKitList.GetDataByValue(KITLIST_INDEX,d.m_nKitIndex,KITLIST_ABILITY,strResAbility))
	{
		AfxMessageBox("Unable to locate the kit ability list.");
		return;
	}

	BYTE *pBuf = NULL;
	int	nBufLen;
	if (!_infKey.GetResData(RESTYPE_2DA,strResAbility,pBuf,nBufLen,TRUE))
	{
		AfxMessageBox("Unable to read the kit abilities.");
		return;
	}
	CInf2DA tda;	
	if (!tda.Parse(pBuf))
	{
		AfxMessageBox("Unable to parse the 2DA file.");
		delete [] pBuf;
		return;
	}
	delete [] pBuf;

	if (d.m_nLevelFrom >= tda.GetCols())
	{
		AfxMessageBox("The start level is higher than the number of levels of data. Nothing to do.");
		return;
	}

	CString strAbility, strChanges, strResName;
	BOOL bAddedAbility;
	CTabSpellData *pData;
	SPELLDATA sd;
	int nSpellIndex;
	int nRowCount = tda.GetRows();
	int nColCount = min(d.m_nLevelTo,tda.GetCols()-1);
	int nRow, nCol;
	for (nCol=d.m_nLevelFrom;nCol<=nColCount;nCol++)
	{
		if (!strChanges.IsEmpty())
			strChanges += "\r\n";
		strAbility.Format("Level %03d: ",nCol);
		strChanges += strAbility;

		bAddedAbility = FALSE;
		for (nRow=0;nRow<nRowCount;nRow++)
		{
			if (!tda.GetData(nRow,nCol,strAbility))
				continue;

			strAbility.MakeUpper();
			if (strAbility.Left(3) != "GA_")
				continue;

			if (bAddedAbility)
				strChanges += ", ";
			else
				bAddedAbility = TRUE;

			strResName = strAbility.Right(strAbility.GetLength()-3);

			// Is the innate already in the list?
			if (GetSpellByResource(strResName,nSpellIndex,pData))
			{
				if (d.m_bRemoveKit)
				{
					delete pData;
					m_lcSpells.DeleteItem(nSpellIndex);
				}
				else
				{
					pData->chMem++;
					pData->chTimesCanCast = pData->chMem;
					CString strMem;
					strAbility.Format("%d",pData->chMem);
					m_lcSpells.SetItemText(nSpellIndex,2,strAbility);
				}
				strChanges += strResName;
			}
			else
				if (!d.m_bRemoveKit)
				{
					strcpy(sd.szResName,strResName);
					sd.nTimesCanCast = 1;
					sd.nTimesMemorized = 1;
					sd.wLevel = 0;
					sd.wType = INF_CRE_ST_INNATE;
					if (AddSpellToList(&sd))
						strChanges += strResName;
				}
		}

	}
	m_lcSpells.SortItems(SpellTabListComp,m_nSort);

	CChangesMadeDlg dlgChanges;
	dlgChanges.m_strChanges = strChanges;
	dlgChanges.DoModal();
}

BOOL CTabSpellsDlg::GetSpellByResource(const char *pszResName, int &nIndex, CTabSpellData *&pData)
{
	int nCount = m_lcSpells.GetItemCount();

	for (nIndex=0;nIndex<nCount;nIndex++)
	{
		pData = (CTabSpellData*)m_lcSpells.GetItemData(nIndex);
		if (pData->pInfo->strName == pszResName)
			return(TRUE);
	}

	return(FALSE);
}

