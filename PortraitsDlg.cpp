// PortraitsDlg.cpp : implementation file
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
#include "PortraitsDlg.h"
#include "InfKey.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortraitsDlg dialog


CPortraitsDlg::CPortraitsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortraitsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortraitsDlg)
	//}}AFX_DATA_INIT
}


void CPortraitsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortraitsDlg)
	DDX_Control(pDX, IDC_SMALL_PORTRAITS, m_lbSmall);
	DDX_Control(pDX, IDC_EXTRACT_SMALL, m_btnExtractSmall);
	DDX_Control(pDX, IDC_EXTRACT_LARGE, m_btnExtractLarge);
	DDX_Control(pDX, IDC_LARGE_PORTRAITS, m_lbLarge);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortraitsDlg, CDialog)
	//{{AFX_MSG_MAP(CPortraitsDlg)
	ON_LBN_SELCHANGE(IDC_LARGE_PORTRAITS, OnSelchangeLargePortraits)
	ON_LBN_SELCHANGE(IDC_SMALL_PORTRAITS, OnSelchangeSmallPortraits)
	ON_BN_CLICKED(IDC_EXTRACT_LARGE, OnExtractLarge)
	ON_BN_CLICKED(IDC_EXTRACT_SMALL, OnExtractSmall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortraitsDlg message handlers

BOOL CPortraitsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rLarge;
	m_lbLarge.GetWindowRect(&rLarge);
	ScreenToClient(&rLarge);

	CRect rBmpLarge;
	rBmpLarge.left = rLarge.right+7;
	rBmpLarge.top = rLarge.top;
	rBmpLarge.right = rBmpLarge.left + 110 + 1;
	rBmpLarge.bottom = rBmpLarge.top + 170 + 1;
	m_bwLarge.Create(rBmpLarge,this,1000);

	CRect rExtract;
	m_btnExtractLarge.GetWindowRect(&rExtract);
	ScreenToClient(&rExtract);

	int nButtonHeight = rExtract.Height();

	rExtract.left = rBmpLarge.left;
	rExtract.right = rBmpLarge.right;
	rExtract.bottom = rLarge.bottom;
	rExtract.top = rLarge.bottom - nButtonHeight;
	m_btnExtractLarge.MoveWindow(&rExtract);

	CRect rSmall;
	m_lbSmall.GetWindowRect(&rSmall);
	ScreenToClient(&rSmall);

	CRect rBmpSmall;
	rBmpSmall.left = rSmall.right+7;
	rBmpSmall.top = rSmall.top;
	rBmpSmall.right = rBmpSmall.left + 38 + 1;
	rBmpSmall.bottom = rBmpSmall.top + 60 + 1;
	m_bwSmall.Create(rBmpSmall,this,1001);

	m_btnExtractSmall.GetWindowRect(&rExtract);
	ScreenToClient(&rExtract);

	rExtract.bottom = rSmall.bottom;
	rExtract.top = rExtract.bottom - nButtonHeight;
	m_btnExtractSmall.MoveWindow(&rExtract);

	FillLists();

	m_lbLarge.SelectString(-1,m_strLarge);
	OnSelchangeLargePortraits(); 
	m_lbSmall.SelectString(-1,m_strSmall);
	OnSelchangeSmallPortraits(); 

	return TRUE;
}

void CPortraitsDlg::GetResourceList(CListBox &lb, char chSize, int nWidth, int nHeight)
{
	CObList *pList;
	if (!_infKey.GetResList(RESTYPE_BMP,pList))
		return;
	ASSERT(pList);

	chSize = toupper(chSize);

	int nIndex;
	BMPTOP top;
	CResInfo *pResInfo;
	CInfBifFile *pBmpFile;
	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		pResInfo = (CResInfo*)pList->GetNext(pos);

		// According to the docs, all large portraits end with an 'L'. I'm going to
		// assume all the resources follow this rule.
		if (pResInfo->strName.IsEmpty() || toupper(pResInfo->strName.GetAt(pResInfo->strName.GetLength()-1)) != chSize)
			continue;

		// Make sure the size is agreeable.
		pBmpFile = _infKey.GetBifFile(pResInfo);
		if (!pBmpFile || !pBmpFile->IsValid())
			continue;

		if (pBmpFile->GetData(pResInfo,&top,sizeof(BMPTOP)) == -1)
			continue;

		if (top.bih.biWidth != nWidth || top.bih.biHeight != nHeight)
			continue;

		nIndex = lb.AddString(pResInfo->strName);
		lb.SetItemData(nIndex,1);	// Set to 1 for in-game resources.
	}
}

void CPortraitsDlg::GetPortraitList(CListBox &lb, char chSize, int nWidth, int nHeight)
{
	char szSearch[MAX_PATH+1];
	char szPath[MAX_PATH+1];
	char szFilename[MAX_PATH+1];
	sprintf(szPath,"%s%s",(const char *)_strInstallPath,DIR_PORTRAITS);
	sprintf(szSearch,"%s*%c.bmp",szPath,chSize);

	WIN32_FIND_DATA fd;
	HANDLE hFile = ::FindFirstFile(szSearch,&fd);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	char szFile[MAX_PATH+1];
	CFile file;

	BMPTOP top;
	int nIndex;
	BOOL bFoundAnother = TRUE;
	while(bFoundAnother)
	{
		strcpy(szFile,fd.cFileName);
		bFoundAnother = ::FindNextFile(hFile,&fd);

		if (strlen(szFile)-4 > 8)
			continue;

		sprintf(szFilename,"%s%s",szPath,szFile);
		if (!file.Open(szFilename,CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary))
			continue;

		if (file.Read(&top,sizeof(BMPTOP)) != sizeof(BMPTOP))
			continue;
		file.Close();

		if (top.bih.biWidth != nWidth || top.bih.biHeight != nHeight)
			continue;

		szFile[strlen(szFile)-4] = '\x0';
		
		// Make sure it isn't alreadyin the list. If it is, then there is a BMP called
		// the same thing as one of hte internal resources.
		if (lb.FindString(-1,szFile) != LB_ERR)
			continue;

		nIndex = lb.AddString(szFile);
		lb.SetItemData(nIndex,0);
	}
}
											  
void CPortraitsDlg::FillLists()
{
	m_lbLarge.ResetContent();
	m_bwLarge.SetBitmap(NULL);
	GetResourceList(m_lbLarge,'M',110,170);
	GetPortraitList(m_lbLarge,'L',110,170);

	m_lbSmall.ResetContent();
	m_bwSmall.SetBitmap(NULL);
	GetResourceList(m_lbSmall,'S',38,60);
	GetPortraitList(m_lbSmall,'S',38,60);
}


void CPortraitsDlg::OnSelchangeLargePortraits() 
{
	int nIndex = m_lbLarge.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	if (m_lbLarge.GetItemData(nIndex))
		m_btnExtractLarge.EnableWindow(TRUE);
	else
		m_btnExtractLarge.EnableWindow(FALSE);

	CString strName;
	m_lbLarge.GetText(nIndex,strName);

	HBITMAP hBitmap;
	GetResourceBitmap(strName,hBitmap,110,170);
	m_bwLarge.SetBitmap(hBitmap);
}

void CPortraitsDlg::OnSelchangeSmallPortraits() 
{
	int nIndex = m_lbSmall.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	if (m_lbSmall.GetItemData(nIndex))
		m_btnExtractSmall.EnableWindow(TRUE);
	else
		m_btnExtractSmall.EnableWindow(FALSE);

	CString strName;
	m_lbSmall.GetText(nIndex,strName);

	HBITMAP hBitmap;
	GetResourceBitmap(strName,hBitmap,38,60);
	m_bwSmall.SetBitmap(hBitmap);
	
}

void CPortraitsDlg::OnExtractLarge() 
{
	int nIndex = m_lbLarge.GetCurSel();
	if (nIndex == LB_ERR || !m_lbLarge.GetItemData(nIndex))
		return;

	CString strRes;
	m_lbLarge.GetText(nIndex,strRes);
	if (ExtractResource(RESTYPE_BMP,strRes,_strInstallPath+DIR_PORTRAITS))
		AfxMessageBox("Bitmap extracted.");
}

void CPortraitsDlg::OnExtractSmall() 
{
	int nIndex = m_lbSmall.GetCurSel();
	if (nIndex == LB_ERR || !m_lbSmall.GetItemData(nIndex))
		return;

	CString strRes;
	m_lbSmall.GetText(nIndex,strRes);
	if (ExtractResource(RESTYPE_BMP,strRes,_strInstallPath+DIR_PORTRAITS))
		AfxMessageBox("Bitmap extracted.");
}

void CPortraitsDlg::OnOK() 
{
	CString strMessage = "You must make a selection from both lists before saving changes.";
	int nIndex = m_lbLarge.GetCurSel();
	if (nIndex == LB_ERR)
	{
		AfxMessageBox(strMessage);
		return;
	}
	m_lbLarge.GetText(nIndex,m_strLarge);
	if (m_strLarge.IsEmpty())
	{
		AfxMessageBox(strMessage);
		return;
	}

	nIndex = m_lbSmall.GetCurSel();
	if (nIndex == LB_ERR)
	{
		AfxMessageBox(strMessage);
		return;
	}
	m_lbSmall.GetText(nIndex,m_strSmall);
	if (m_strSmall.IsEmpty())
	{
		AfxMessageBox(strMessage);
		return;
	}

	CDialog::OnOK();
}
