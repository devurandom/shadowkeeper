// OpenCreDlg.cpp : implementation file
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
#include "InfCreature.h"
#include "InfTlk.h"
#include "Global.h"
#include "OpenCreDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenCreDlg dialog


COpenCreDlg::COpenCreDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenCreDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenCreDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSort = SORT_CRE_FILE_ASC;
	m_hBitmap = NULL;
}

COpenCreDlg::~COpenCreDlg()
{
	if (m_hBitmap)
		::DeleteObject(m_hBitmap);
}

void COpenCreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenCreDlg)
	DDX_Control(pDX, IDC_CRE_LIST, m_lcList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenCreDlg, CDialog)
	//{{AFX_MSG_MAP(COpenCreDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_CRE_LIST, OnColumnclickCreList)
	ON_NOTIFY(NM_DBLCLK, IDC_CRE_LIST, OnDblclkCreList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CRE_LIST, OnItemchangedCreList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenCreDlg message handlers

BOOL COpenCreDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_lcList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_lcList.SetExtendedStyle(dwStyle);

	m_lcList.InsertColumn(0,"File",LVCFMT_LEFT,100);
	m_lcList.InsertColumn(1,"Character",LVCFMT_LEFT,134);

	CRect rList;
	m_lcList.GetWindowRect(&rList);
	ScreenToClient(&rList);

	CRect rPortrait;
	rPortrait.left = rList.right+5;
	rPortrait.top = rList.top;
	rPortrait.right = rPortrait.left + 112;
	rPortrait.bottom = rPortrait.top + 172;
	m_wndPortrait.Create(rPortrait,this,1000);

	FillCharacters();
	
	return TRUE;
}

void COpenCreDlg::OnDestroy() 
{
	ClearList();
	CDialog::OnDestroy();
}

void COpenCreDlg::ClearList()
{
	OPENCREDATA *pData;

	int nCount = m_lcList.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		pData = (OPENCREDATA*)m_lcList.GetItemData(i);
		delete pData;
	}
	m_lcList.DeleteAllItems();
}

int CALLBACK OpenCreComp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	OPENCREDATA *pData1 = (OPENCREDATA*)lParam1;
	OPENCREDATA *pData2 = (OPENCREDATA*)lParam2;

	switch(lParamSort)
	{
		case SORT_CRE_NAME_ASC :
			if (pData1->strName < pData2->strName)
				return(-1);
			else
				if (pData1->strName > pData2->strName)
					return(1);
			break;
		case SORT_CRE_NAME_DES :
			if (pData1->strName < pData2->strName)
				return(1);
			else
				if (pData1->strName > pData2->strName)
					return(-1);
			break;
		case SORT_CRE_FILE_ASC :
			if (pData1->strFile < pData2->strFile)
				return(-1);
			else
				if (pData1->strFile > pData2->strFile)
					return(1);
			break;
		case SORT_CRE_FILE_DES :
			if (pData1->strFile < pData2->strFile)
				return(1);
			else
				if (pData1->strFile > pData2->strFile)
					return(-1);
			break;
	}

	return(0);
}

void COpenCreDlg::FillCharacters()
{
	void ClearList();
	
	WIN32_FIND_DATA fd;
	OPENCREDATA *pData;
	INF_CRE cre;
	CString strChrPath;
	CString strFind;
	CString strFile;
	CString strName;
	CFile file;
	int nItem;

	strChrPath = _strInstallPath + DIR_OVERRIDE;
	strFind = strChrPath + "*.cre";
	HANDLE hFind = ::FindFirstFile(strFind,&fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	BOOL bNotDone = TRUE;
	while(bNotDone)
	{
		strFile = strChrPath + fd.cFileName;
		if (file.Open(strFile,CFile::modeRead))
		{
			file.Read(&cre,sizeof(INF_CRE));

			pData = new OPENCREDATA;
			if (pData)
			{
				nItem = m_lcList.InsertItem(0,fd.cFileName);
				if (cre.dwLongCreatureName == 0xFFFFFFFF)
					strName = "<No Name>";
				else
					_infTlk.GetString(cre.dwLongCreatureName,strName);
				if (strName.IsEmpty())
					strName = "<No Name>";
				m_lcList.SetItemText(nItem,1,strName);
				
				pData->strFile = fd.cFileName;
				pData->strName = strName;
				memset(pData->szPortrait,0,9);
				memcpy(pData->szPortrait,cre.chLargePortrait,8);

				m_lcList.SetItemData(nItem,(DWORD)pData);
			}
			file.Close();
		}

		bNotDone = ::FindNextFile(hFind,&fd);
	}
	::FindClose(hFind);

	m_lcList.SortItems(OpenCreComp,SORT_CRE_FILE_ASC);
	m_nSort = SORT_CRE_FILE_ASC;
	m_lcList.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
}

void COpenCreDlg::OnColumnclickCreList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nSortCol;
	if (m_nSort == SORT_CRE_FILE_ASC || m_nSort == SORT_CRE_FILE_DES)
		nSortCol = 0;
	else
		nSortCol = 1;

	if (nSortCol == pNMListView->iSubItem)
	{
		switch(nSortCol)
		{
			case 0 :
				if (m_nSort == SORT_CRE_FILE_ASC)
					m_nSort = SORT_CRE_FILE_DES;
				else
					m_nSort = SORT_CRE_FILE_ASC;
				break;
			case 1 :
				if (m_nSort == SORT_CRE_NAME_ASC)
					m_nSort = SORT_CRE_NAME_DES;
				else
					m_nSort = SORT_CRE_NAME_ASC;
				break;
		}
	}
	else
	{
		if (!pNMListView->iSubItem)
			m_nSort = SORT_CRE_FILE_ASC;
		else
			m_nSort = SORT_CRE_NAME_DES;
	}

	m_lcList.SortItems(OpenCreComp,m_nSort);

	*pResult = 0;
}

void COpenCreDlg::OnDblclkCreList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

void COpenCreDlg::OnOK() 
{
	int nItem = m_lcList.GetNextItem(-1,LVNI_SELECTED);
	if (nItem == -1)
		return;

	m_strPath = _strInstallPath + DIR_OVERRIDE;
	m_strPath += m_lcList.GetItemText(nItem,0);

	EndDialog(IDOK);
}

void COpenCreDlg::OnItemchangedCreList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	int nItem = m_lcList.GetNextItem(-1,LVNI_SELECTED);
	if (nItem == -1)
	{
		m_wndPortrait.SetBitmap(NULL);
		return;
	}

	OPENCREDATA *pData = (OPENCREDATA*)m_lcList.GetItemData(nItem);

	if (GetResourceBitmap(pData->szPortrait,m_hBitmap))
		m_wndPortrait.SetBitmap(m_hBitmap);
	else
		m_wndPortrait.SetBitmap(NULL);
}
