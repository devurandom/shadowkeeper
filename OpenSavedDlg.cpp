// OpenSavedDlg.cpp : implementation file
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
#include "DaleKeeper.h"
#include "OpenSavedDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COpenGameData::COpenGameData()
{
}

void COpenGameData::GetNumber(CString &strNumber)
{
	ASSERT(!m_strPath.IsEmpty());

	strNumber.Empty();
	int nIndex = m_strPath.Find('-');
	if (nIndex == -1)
		return;

	strNumber = m_strPath.Left(nIndex);
	if (!strNumber.IsEmpty())
		strNumber = strNumber.Left(strNumber.GetLength()-1);
}

int CALLBACK OpenGameComp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	COpenGameData *pData1 = (COpenGameData*)lParam1;
	COpenGameData *pData2 = (COpenGameData*)lParam2;

	CString strName1, strName2;
	strName1 = pData1->m_strName;
	strName2 = pData2->m_strName;

	switch(lParamSort)
	{
		case SORT_NAME_ASC :
			if (strName1 < strName2)
				return(-1);
			else
				if (strName1 > strName2)
					return(1);
			break;
		case SORT_NAME_DES :
			if (strName1 < strName2)
				return(1);
			else
				if (strName1 > strName2)
					return(-1);
			break;
		case SORT_TIME_ASC :
			if (pData1->m_tWritten < pData2->m_tWritten)
				return(-1);
			else
				if (pData1->m_tWritten > pData2->m_tWritten)
					return(1);
			break;
		case SORT_TIME_DES :
			if (pData1->m_tWritten < pData2->m_tWritten)
				return(1);
			else
				if (pData1->m_tWritten > pData2->m_tWritten)
					return(-1);
			break;
	}

	return(0);
}

/////////////////////////////////////////////////////////////////////////////
// COpenSavedDlg dialog

COpenSavedDlg::COpenSavedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenSavedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenSavedDlg)
	m_bMultiplayer = FALSE;
	//}}AFX_DATA_INIT

	m_nSort = SORT_TIME_DES;
	m_bLoadingGames = FALSE;
}


void COpenSavedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenSavedDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_GAMES, m_lcGames);
	DDX_Check(pDX, IDC_MULTIPLAYER, m_bMultiplayer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenSavedDlg, CDialog)
	//{{AFX_MSG_MAP(COpenSavedDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_GAMES, OnColumnclickGames)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_GAMES, OnItemchangedGames)
	ON_NOTIFY(NM_DBLCLK, IDC_GAMES, OnDblclkGames)
	ON_BN_CLICKED(IDC_MULTIPLAYER, OnMultiplayer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenSavedDlg message handlers

BOOL COpenSavedDlg::OnInitDialog() 
{
	if (_bDefaultOpenSinglePlayer)
		m_bMultiplayer = FALSE;
	else
		m_bMultiplayer = TRUE;

	CDialog::OnInitDialog();

	DWORD dwStyle = m_lcGames.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_lcGames.SetExtendedStyle(dwStyle);

	m_lcGames.InsertColumn(0,"Game Name",LVCFMT_LEFT,125);
	m_lcGames.InsertColumn(1,"Time Saved",LVCFMT_LEFT,5);
	ResizeLastColumn(m_lcGames);

	CRect rButton;
	m_btnOK.GetWindowRect(&rButton);
	ScreenToClient(&rButton);

	CRect rGames;
	m_lcGames.GetWindowRect(&rGames);
	ScreenToClient(&rGames);

	CRect rBitmap;
	rBitmap.left = rButton.left;
	rBitmap.right = rBitmap.left+102+2;
	rBitmap.top = rGames.top;
	rBitmap.bottom = rBitmap.top+76+2;

	m_wndBitmap.Create(rBitmap,this,1000);

	FillGames();
	ResizeLastColumn(m_lcGames);

	return TRUE;
}

void COpenSavedDlg::OnDestroy() 
{
	ClearList();

	CDialog::OnDestroy();	
}

void COpenSavedDlg::ClearList()
{
	COpenGameData *pData;

	int nCount = m_lcGames.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pData = (COpenGameData*)m_lcGames.GetItemData(i);
		delete pData;
	}

	m_lcGames.DeleteAllItems();
}

void COpenSavedDlg::GetName(const char *pszDir, CString &strName)
{
	strName.Empty();

	CString strDir(pszDir);
	int nIndex = strDir.Find('-');
	if (nIndex == -1)
		return;

	strName = strDir.Mid(nIndex+1);
}

void COpenSavedDlg::FillGames()
{
	ClearList();

	CString strSave, strFind, strPath, strFile;
	CString strName, strTime;
	CFileStatus fs;
	WIN32_FIND_DATA fd;
	COpenGameData *pData;
	int nItem;

	strSave = _strInstallPath + (m_bMultiplayer ? DIR_MULTISAVE : DIR_SINGLESAVE);
	strFind = strSave + "*";
	HANDLE hFind = ::FindFirstFile(strFind,&fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	m_bLoadingGames = TRUE;
	BOOL bNotDone = TRUE;
	while(bNotDone)
	{
		// Make sure it's a directory, and not the '.' or '..'.
		if (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY && isdigit(fd.cFileName[0]) && strcmp(fd.cFileName,".") && strcmp(fd.cFileName,".."))
		{
			strPath = strSave + CString(fd.cFileName) + "\\";
			strFile = strPath + "Baldur.gam";

			// Make sure a saved file exists here.
			if (CFile::GetStatus(strFile,fs))
			{
				GetName(fd.cFileName,strName);

				// There have been problems with people editing the auto saves or quicksaves
				// and then overwriting them when they go back to play -- losing the clean
				// version to fallback on. I'm preventing auto-saves and quick-saves from 
				// showing up as an editing option.
				if (_bAllowQuicksaves || (strcmpi(strName,"auto-save-tob") && strcmpi(strName,"quick-save-tob") &&
					 strcmpi(strName,"auto-save") && strcmpi(strName,"quick-save")))
				{
					pData = new COpenGameData;
					pData->m_strName = strName;
					pData->m_strPath = strPath;
					pData->m_tWritten = fs.m_ctime;

					nItem = m_lcGames.InsertItem(0,strName);
					m_lcGames.SetItemText(nItem,1,pData->m_tWritten.Format("%b %d, %Y at %I:%M %p"));
					m_lcGames.SetItemData(nItem,(DWORD)pData);
				}
			}
		}
		bNotDone = ::FindNextFile(hFind,&fd);
	}
	::FindClose(hFind);

	m_lcGames.SortItems(OpenGameComp,SORT_TIME_DES);
	m_nSort = SORT_TIME_DES;
	m_bLoadingGames = FALSE;
	m_lcGames.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
}

void COpenSavedDlg::OnColumnclickGames(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nSortCol;
	if (m_nSort == SORT_NAME_ASC || m_nSort == SORT_NAME_DES)
		nSortCol = 0;
	else
		nSortCol = 1;

	if (nSortCol == pNMListView->iSubItem)
	{
		switch(nSortCol)
		{
			case 0 :
				if (m_nSort == SORT_NAME_ASC)
					m_nSort = SORT_NAME_DES;
				else
					m_nSort = SORT_NAME_ASC;
				break;
			case 1 :
				if (m_nSort == SORT_TIME_ASC)
					m_nSort = SORT_TIME_DES;
				else
					m_nSort = SORT_TIME_ASC;
				break;
		}
	}
	else
	{
		if (!pNMListView->iSubItem)
			m_nSort = SORT_NAME_ASC;
		else
			m_nSort = SORT_TIME_DES;
	}

	m_lcGames.SortItems(OpenGameComp,m_nSort);

	*pResult = 0;
}

void COpenSavedDlg::OnItemchangedGames(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bLoadingGames)
		return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	COpenGameData *pData = (COpenGameData*)m_lcGames.GetItemData(pNMListView->iItem);

	CString strFile;
	strFile = pData->m_strPath+"Baldur.bmp";
	m_wndBitmap.LoadBitmap(strFile);
	
	*pResult = 0;
}


void COpenSavedDlg::OnOK() 
{
	int nItem = m_lcGames.GetNextItem(-1,LVNI_SELECTED);
	if (nItem == -1)
		return;

	COpenGameData *pData = (COpenGameData*)m_lcGames.GetItemData(nItem);
	m_strDirName = pData->m_strPath;

	m_strTitle = m_lcGames.GetItemText(nItem,0);
	EndDialog(IDOK);
}

void COpenSavedDlg::OnDblclkGames(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();	
	*pResult = 0;
}

void COpenSavedDlg::OnMultiplayer() 
{
	UpdateData(TRUE);
	
	if (m_bMultiplayer)
		SetWindowText("Open Multiplayer Saved Game");
	else
		SetWindowText("Open Single Player Saved Game");

	FillGames();
}
