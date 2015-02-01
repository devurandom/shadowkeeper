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

// CreatureListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "dalekeeper.h"
#include "InfKey.h"
#include "InfCreature.h"
#include "InfTlk.h"
#include "Global.h"
#include "CreatureListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreatureListCtrl

int CALLBACK CreSortByName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrlCreData *pData1 = (CListCtrlCreData*)lParam1;
	CListCtrlCreData *pData2 = (CListCtrlCreData*)lParam2;

	int nName = strcmp(pData1->strName,pData2->strName);
	int nRes = strcmp(pData1->pResInfo->strName,pData2->pResInfo->strName);

	if (lParamSort == CRESORT_DES)
		nName *= -1;

	return(nName ? nName : nRes);
}

int CALLBACK CreSortByResource(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrlCreData *pData1 = (CListCtrlCreData*)lParam1;
	CListCtrlCreData *pData2 = (CListCtrlCreData*)lParam2;

	int nRes = strcmp(pData1->pResInfo->strName,pData2->pResInfo->strName);

	if (lParamSort == CRESORT_DES)
		nRes *= -1;

	return(nRes);
}

int CALLBACK CreSortByType(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrlCreData *pData1 = (CListCtrlCreData*)lParam1;
	CListCtrlCreData *pData2 = (CListCtrlCreData*)lParam2;

	int nType1, nType2;
	if (pData1->pResInfo->IsFlagSet(RIF_CUSTOM))
		nType1 = 0;
	else
		if (!pData1->pResInfo->IsFlagSet(RIF_USEOVERRIDE))
			nType1 = 2;
		else
			nType1 = 3;
	if (pData2->pResInfo->IsFlagSet(RIF_CUSTOM))
		nType2 = 0;
	else
		if (!pData2->pResInfo->IsFlagSet(RIF_USEOVERRIDE))
			nType2 = 2;
		else
			nType2 = 3;

	int nType = (nType1 < nType2 ? -1 : (nType1 > nType2 ? 1 : 0));
	int nName = strcmp(pData1->strName,pData2->strName);
	int nRes = strcmp(pData1->pResInfo->strName,pData2->pResInfo->strName);

	if (lParamSort == CRESORT_DES)
		nType *= -1;

	return((nType ? nType : (nName ? nName : nRes)));
}

CCreatureListCtrl::CCreatureListCtrl()
{
	m_nSortCol = 0;
	m_nSortDir = CRESORT_ASC;

	m_pCount = NULL;
}

CCreatureListCtrl::~CCreatureListCtrl()
{
}

void CCreatureListCtrl::ClearList()
{
	CListCtrlCreData *pData;
	int nCount = GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pData = (CListCtrlCreData*)GetItemData(i);
		delete pData;
	}

	DeleteAllItems();
}

void CCreatureListCtrl::OnDestroy() 
{
	ClearList();
	CListCtrl::OnDestroy();
}

BEGIN_MESSAGE_MAP(CCreatureListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCreatureListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreatureListCtrl message handlers

BOOL CCreatureListCtrl::Init(CStatic *pCreatureCount)
{
	m_pCount = pCreatureCount;

	DWORD dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	SetExtendedStyle(dwStyle);

	InsertColumn(0,"Name",LVCFMT_LEFT,180);
	InsertColumn(1,"Resource",LVCFMT_LEFT,85);
	InsertColumn(2,"Type",LVCFMT_LEFT,85);

	FillList();

	return(TRUE);
}

int CCreatureListCtrl::GetSelectedItem()
{
	return(GetNextItem(-1,LVNI_SELECTED));
}

void CCreatureListCtrl::SetSelectedItem(int nIndex)
{ 
	SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
}

int CCreatureListCtrl::AddItem(CResInfo *pResInfo)
{
	CListCtrlCreData *pData;
	CInfBifFile	*pBifFile;
	INF_CRE		cre;
	CString		strName;
	int			nItem;

	pBifFile = _infKey.GetBifFile(pResInfo);
	if (!pBifFile)
		return(-1);

	if (pBifFile->GetData(pResInfo->wLocator,&cre) == -1)
		return(-1);

	_infTlk.GetString(cre.dwLongCreatureName,strName);
	if (strName.IsEmpty())
		strName = "<No Name>";

	if (!m_strSearchText.IsEmpty())
	{
		if (m_bMatchCase)
		{
			if (strName.Find(m_strSearchText) == -1)
				return(-1);
		}
		else
		{
			CString strSearchLower(m_strSearchText);
			strSearchLower.MakeLower();
			CString strFind(strName);
			strFind.MakeLower();
			if (strFind.Find(strSearchLower) == -1)
				return(-1);
		}
	}

	pData = new CListCtrlCreData;
	if (!pData)
		return(-1);

	pData->pResInfo = pResInfo;
	pData->strName = strName;

	nItem = InsertItem(0,strName);
	SetItemText(nItem,1,pResInfo->strName);
	if (pResInfo->IsFlagSet(RIF_CUSTOM))
		SetItemText(nItem,2,"Custom");
	else
		if (pResInfo->IsFlagSet(RIF_USEOVERRIDE))
			SetItemText(nItem,2,"Overridden");
		else
			SetItemText(nItem,2,"Original");
	SetItemData(nItem,(DWORD)pData);

	return(nItem);
}

void CCreatureListCtrl::FillList()
{
	BOOL bSetSelection = FALSE;
	CString strSelection;
	int nSelection = -1;
	CListCtrlCreData *pData;

	int nItem = GetSelectedItem();
	if (nItem != -1)
	{
		pData = (CListCtrlCreData*)GetItemData(nItem);
		if (pData)
		{
			bSetSelection = TRUE;
			strSelection  = pData->pResInfo->strName;
		}
	}

	LockWindowUpdate();
	BeginWaitCursor();
	ClearList();

	CObList		*m_pCreatures;
	CResInfo		*pResInfo;

	_infKey.GetResList(RESTYPE_CRE,m_pCreatures);
	POSITION pos = m_pCreatures->GetHeadPosition();
	while(pos)
	{
		pResInfo = (CResInfo*)m_pCreatures->GetNext(pos);
		if (!pResInfo)
			continue;

		AddItem(pResInfo);
	}
	Sort();

	if (bSetSelection)
	{
		int nCount = GetItemCount();
		for (int i=0;i<nCount;i++)
		{
			pData = (CListCtrlCreData*)GetItemData(i);
			if (pData && pData->pResInfo->strName == strSelection)
			{
				SetSelectedItem(i);
				EnsureVisible(i,FALSE);
				break;
			}
		}
	}

	EndWaitCursor();
	UnlockWindowUpdate();

	if (m_pCount)
	{
		CString strCount;
		strCount.Format("%d",GetItemCount());
		m_pCount->SetWindowText(strCount);
	}
}

void CCreatureListCtrl::Sort()
{
	switch(m_nSortCol)
	{
		case 0 :
			SortItems(CreSortByName,m_nSortDir);
			break;
		case 1 :
			SortItems(CreSortByResource,m_nSortDir);
			break;
		case 2 :
			SortItems(CreSortByType,m_nSortDir);
			break;
	}
}

void CCreatureListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (m_nSortCol == pNMListView->iSubItem)
	{
		// If in the same column reverse the sort order.
		if (m_nSortDir == CRESORT_ASC)
			m_nSortDir = CRESORT_DES;
		else
			m_nSortDir = CRESORT_ASC;
	}
	else
	{
		m_nSortCol = pNMListView->iSubItem;
		m_nSortDir = CRESORT_ASC;
	}

	Sort();

	int nIndex = GetSelectedItem();
	if (nIndex != -1)
		EnsureVisible(nIndex,FALSE);
	
	*pResult = 0;
}

void CCreatureListCtrl::UpdateItem(const char *pszFilename)
{
	CString strFile(pszFilename);
	int nIndex = strFile.ReverseFind('\\');
	if (nIndex == -1)
		return;

	strFile = strFile.Mid(nIndex+1);
	if (strFile.GetLength() > 4)
		strFile = strFile.Left(strFile.GetLength()-4);

	CResInfo *pResInfo = _infKey.GetResInfo(RESTYPE_CRE,strFile);

	// If the resource existed, needs to now be marked as overriden.
	if (pResInfo)
	{
		pResInfo->SetFlag(RIF_USEOVERRIDE);

		CListCtrlCreData *pData;
		int nCount = GetItemCount();
		for (int i=0;i<nCount;i++)
		{
			pData = (CListCtrlCreData*)GetItemData(i);
			if (pData->pResInfo == pResInfo)
			{
				SetItemText(i,2,"Overridden");
				Sort();
			}
		}
	}
	else
	{
		// Was saved as a new resource.
		_infKey.RefreshOverride();
		pResInfo = _infKey.GetResInfo(RESTYPE_CRE,strFile);
		if (!pResInfo)
			return;

		int nItem = AddItem(pResInfo);
		Sort();
	}
}

BOOL CCreatureListCtrl::ExtractCre(int nItem, BOOL bSaveAs)
{
	CListCtrlCreData *pData = (CListCtrlCreData*)GetItemData(nItem);
	if (!pData)
		return(FALSE);

	CString strFile(_strInstallPath+DIR_OVERRIDE);
	strFile += pData->pResInfo->strName + ".cre";

	CInfBifFile *pBif = _infKey.GetBifFile(pData->pResInfo,TRUE);
	if (!pBif || !pBif->IsValid())
		return(FALSE);

	int nOffset, nSize;
	if (!pBif->GetDataOffsetAndSize(pData->pResInfo,nOffset,nSize))
		return(FALSE);

	// If not going for a SaveAs condition and the resrouce already exists in
	// the Override folder give the user the option of overwriting the file or
	// bring up the SaveAs dialog.
	if (!bSaveAs && pData->pResInfo->IsFlagSet(RIF_USEOVERRIDE|RIF_CUSTOM))
	{
		CString strMessage;
		strMessage.Format("%s.cre already exists. Do you want overwrite the version in "
			"the override directory?",(const char *)pData->pResInfo->strName);
		int nResult = MessageBox(strMessage,"File Exists",MB_ICONQUESTION|MB_YESNOCANCEL);
		if (nResult == IDCANCEL)
			return(FALSE);
		else
			if (nResult == IDNO)
				bSaveAs = TRUE;
	}

	BYTE *pBuf = new BYTE[nSize];
	if (!pBuf)
		return(FALSE);

	pBif->Seek(nOffset,CFile::begin);
	pBif->Read(pBuf,nSize);

	if (bSaveAs)
	{
		CFileDialog d(
			FALSE,
			".cre",
			strFile,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Creature Files (*.cre)|*.cre|All Files (*.*)|*.*||");

		if (d.DoModal() == IDOK)
		{
			strFile = d.GetPathName();
			CFile file;
			if (!file.Open(strFile,CFile::modeCreate|CFile::modeWrite))
			{
				AfxMessageBox("Unable to create resource file.");
				delete [] pBuf;
				return(FALSE);
			}

			file.Write(pBuf,nSize);
			file.Close();
			UpdateItem(strFile);
		}
	}
	else
	{
		// Just need to extract the CRE, nothing special.
		CFile file;
		if (!file.Open(strFile,CFile::modeCreate|CFile::modeWrite))
		{
			AfxMessageBox("Unable to create resource file.");
			delete [] pBuf;
			return(FALSE);
		}

		file.Write(pBuf,nSize);
		file.Close();
		UpdateItem(strFile);
	}

	delete [] pBuf;
	return(TRUE);
}

void CCreatureListCtrl::OverrideChanged()
{
	FillList();
}

void CCreatureListCtrl::EditSelection()
{
	int nItem = GetSelectedItem();
	if (nItem == -1)
		return;

	CListCtrlCreData *pData = (CListCtrlCreData*)GetItemData(nItem);
	if (!pData)
		return;

	// If it isn't already in the override directory, need to extract it.
	BOOL bLoadFromResource;
	if (!pData->pResInfo->IsFlagSet(RIF_USEOVERRIDE|RIF_CUSTOM))
		bLoadFromResource = TRUE;
	else
		bLoadFromResource = FALSE;

	CString strFile(_strInstallPath+DIR_OVERRIDE);
	strFile += pData->pResInfo->strName + ".cre";

	CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();
	pApp->LoadCreatureFile(strFile,bLoadFromResource);
}

void CCreatureListCtrl::ExtractSelection()
{	
	int nItem = GetSelectedItem();
	if (nItem == -1)
		return;

	ExtractCre(nItem,FALSE);
}

void CCreatureListCtrl::ExtractSelectionAs()
{
	int nItem = GetSelectedItem();
	if (nItem == -1)
		return;

	ExtractCre(nItem,TRUE);
}

void CCreatureListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	EditSelection();
}

void CCreatureListCtrl::DeleteSelection()
{
	int nItem = GetSelectedItem();
	if (nItem == -1)
		return;

	CListCtrlCreData *pData = (CListCtrlCreData*)GetItemData(nItem);
	if (!pData)
		return;

	if (!pData->pResInfo->IsFlagSet(RIF_USEOVERRIDE|RIF_CUSTOM))
	{
		AfxMessageBox("You can only delete Overridden and Custom resources.");
		return;
	}

	CString strMessage;
	strMessage.Format("This will delete the file from your Override folder. Are you "
		"sure you want to delete this resource?\r\n\r\n"
		"\tCreature: %s\r\n"
		"\tFilename: %s.cre",
		(const char *)pData->strName,
		(const char *)pData->pResInfo->strName);
	if (MessageBox(strMessage,"Delete CRE File",MB_ICONQUESTION|MB_YESNO) == IDNO)
		return;

	CString strFile(_strInstallPath+DIR_OVERRIDE);
	strFile += pData->pResInfo->strName + ".cre";

	::DeleteFile(strFile);
	_infKey.RefreshOverride();
	FillList();
}