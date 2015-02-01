// EditAffectDlg.cpp : implementation file
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
#include "EditAffectDlg.h"
#include "EditAffectDataDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditAffectDlg dialog

#define INDEX_TYPE					2
#define INDEX_TARGET					3
#define INDEX_PARAM1					5
#define INDEX_PARAM2					6
#define INDEX_FLAGS					7
#define INDEX_TIME					8
#define INDEX_PROBABILITY			9
#define INDEX_RESOURCE0				11
#define INDEX_RESOURCE1				26
#define INDEX_RESOURCE2				27
#define INDEX_RESOURCE3				33

STRUCTDEF _sdCreAff[] =
{
	{0x0000,"Signature",		STRUCTDEF_TYPE_CHAR,		4,0},
	{0x0004,"Version",		STRUCTDEF_TYPE_CHAR,		4,0},
	{0x0008,"Type",			STRUCTDEF_TYPE_DWORD,	4,8},
	{0x000c,"Target",			STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0010,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0014,"Param 1",		STRUCTDEF_TYPE_INT,		4,0},
	{0x0018,"Param 2",		STRUCTDEF_TYPE_INT,		4,0},
	{0x001c,"Flags 1",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0020,"Time",			STRUCTDEF_TYPE_INT,		4,0},
	{0x0024,"Probability 1",STRUCTDEF_TYPE_WORD,		2,0},
	{0x0026,"Probability 2",STRUCTDEF_TYPE_WORD,		2,0},
	{0x0028,"Resource 0",	STRUCTDEF_TYPE_CHAR,		8,0},
	{0x0030,"Die Sides 1",	STRUCTDEF_TYPE_INT,		4,0},
	{0x0034,"Die Count 1",	STRUCTDEF_TYPE_INT,		4,0},
	{0x0038,"Save Type",		STRUCTDEF_TYPE_INT,		4,0},
	{0x003c,"Save Bonus",	STRUCTDEF_TYPE_INT,		4,0},
	{0x0040,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0044,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0048,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x004c,"Die Sides 2",	STRUCTDEF_TYPE_INT,		4,0},
	{0x0050,"Die Count 2",	STRUCTDEF_TYPE_INT,		4,0},
	{0x0054,"Flags 2",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0058,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x005c,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0060,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0064,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0068,"Resource 1",	STRUCTDEF_TYPE_CHAR,		8,0},
	{0x0070,"Resource 2",	STRUCTDEF_TYPE_CHAR,		8,0},
	{0x0078,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x007c,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0080,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0084,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0088,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x008c,"Resource 3",	STRUCTDEF_TYPE_CHAR,		8,0},
	{0x0094,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0098,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x009c,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00a0,"Local Variable Name",STRUCTDEF_TYPE_CHAR,		32,0},
	{0x00c0,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00c4,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00c8,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00cc,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00d0,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00d4,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00d8,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00dc,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00e0,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00e4,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00e8,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00ec,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00f0,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00f4,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00f8,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x00fc,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0100,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},
	{0x0104,"Unknown",		STRUCTDEF_TYPE_DWORD,	4,8},

	{0x0000,NULL,0,0}
};

CEditAffectDlg::CEditAffectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditAffectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditAffectDlg)
	//}}AFX_DATA_INIT
}


void CEditAffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditAffectDlg)
	DDX_Control(pDX, IDC_AFF_TYPE, m_edType);
	DDX_Control(pDX, IDC_AFF_RESOURCE2, m_edResource2);
	DDX_Control(pDX, IDC_AFF_PROBABILITY, m_edProbability);
	DDX_Control(pDX, IDC_AFF_PARAM2, m_edParam2);
	DDX_Control(pDX, IDC_AFF_PARAM1, m_edParam1);
	DDX_Control(pDX, IDC_AFF_FLAGS, m_edFlags);
	DDX_Control(pDX, IDC_AFF_RESOURCE0, m_edResource0);
	DDX_Control(pDX, IDC_AFF_RESOURCE1, m_edResource1);
	DDX_Control(pDX, IDC_AFF_RESOURCE3, m_edResource3);
	DDX_Control(pDX, IDC_AFF_TARGET, m_edTarget);
	DDX_Control(pDX, IDC_AFF_TIME, m_edTime);
	DDX_Control(pDX, IDC_LIST, m_lcList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditAffectDlg, CDialog)
	//{{AFX_MSG_MAP(CEditAffectDlg)
	ON_EN_CHANGE(IDC_AFF_FLAGS, OnChangeAffFlags)
	ON_EN_CHANGE(IDC_AFF_PARAM1, OnChangeAffParam1)
	ON_EN_CHANGE(IDC_AFF_PARAM2, OnChangeAffParam2)
	ON_EN_CHANGE(IDC_AFF_TYPE, OnChangeAffType)
	ON_EN_CHANGE(IDC_AFF_TARGET, OnChangeAffTarget)
	ON_EN_CHANGE(IDC_AFF_TIME, OnChangeAffTime)
	ON_EN_CHANGE(IDC_AFF_PROBABILITY, OnChangeAffProbability)
	ON_EN_CHANGE(IDC_AFF_RESOURCE0, OnChangeAffResource0)
	ON_EN_CHANGE(IDC_AFF_RESOURCE1, OnChangeAffResource1)
	ON_EN_CHANGE(IDC_AFF_RESOURCE2, OnChangeAffResource2)
	ON_EN_CHANGE(IDC_AFF_RESOURCE3, OnChangeAffResource3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditAffectDlg message handlers

BOOL CEditAffectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_lcList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcList.SetExtendedStyle(dwStyle);

	m_lcList.InsertColumn(0,"Offset",LVCFMT_LEFT,50);
	m_lcList.InsertColumn(1,"Type",LVCFMT_LEFT,60);
	m_lcList.InsertColumn(2,"Len",LVCFMT_LEFT,40);
	m_lcList.InsertColumn(3,"Value",LVCFMT_LEFT,100);
	m_lcList.InsertColumn(4,"Name",LVCFMT_LEFT,120);

	LoadValues();

	return TRUE;
}

void CEditAffectDlg::SetAffect(INF_AFF *pAff)
{
	memcpy(&m_aff,pAff,sizeof(INF_AFF));
}

void CEditAffectDlg::GetAffect(INF_AFF *pAff)
{
	memcpy(pAff,&m_aff,sizeof(INF_AFF));
}

void CEditAffectDlg::LoadValues()
{
	char szBuf[101];

	GetValueString(_sdCreAff[INDEX_TYPE],szBuf);
	m_edType.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_TARGET],szBuf);
	m_edTarget.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_PARAM1],szBuf);
	m_edParam1.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_PARAM2],szBuf);
	m_edParam2.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_PROBABILITY],szBuf);
	m_edProbability.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_TIME],szBuf);
	m_edTime.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_FLAGS],szBuf);
	m_edFlags.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_RESOURCE0],szBuf);
	m_edResource0.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_RESOURCE1],szBuf);
	m_edResource1.SetWindowText(szBuf);

	GetValueString(_sdCreAff[INDEX_RESOURCE2],szBuf);
	m_edResource2.SetWindowText(szBuf);
	
	GetValueString(_sdCreAff[INDEX_RESOURCE3],szBuf);
	m_edResource3.SetWindowText(szBuf);

	m_lcList.DeleteAllItems();

	BYTE *ptr;
	int nItem;
	int nIndex = 0;
	while(_sdCreAff[nIndex].pszName)
	{
		MakeHexString(_sdCreAff[nIndex].nOffset,szBuf,3);
		nItem = m_lcList.InsertItem(nIndex,szBuf);

		ptr = (BYTE*)(&m_aff) + _sdCreAff[nIndex].nOffset;

		GetDataType(_sdCreAff[nIndex],szBuf);
		m_lcList.SetItemText(nItem,1,szBuf);

		sprintf(szBuf,"%d",_sdCreAff[nIndex].nLen);
		m_lcList.SetItemText(nItem,2,szBuf);

		GetValueString(_sdCreAff[nIndex],szBuf);
		m_lcList.SetItemText(nItem,3,szBuf);

		sprintf(szBuf,"%s",_sdCreAff[nIndex].pszName);
		m_lcList.SetItemText(nItem,4,szBuf);

		nIndex++;
	}
}

BOOL CEditAffectDlg::GetDataType(const STRUCTDEF &sd, char *pszResult)
{
	switch(sd.nType)
	{
		case STRUCTDEF_TYPE_NONE :
			strcpy(pszResult,"None");
			break;
		case STRUCTDEF_TYPE_BYTE :
			strcpy(pszResult,"BYTE");
			break;
		case STRUCTDEF_TYPE_INT :
			strcpy(pszResult,"int");
			break;
		case STRUCTDEF_TYPE_DWORD :
			strcpy(pszResult,"DWORD");
			break;
		case STRUCTDEF_TYPE_CHAR :
			strcpy(pszResult,"char");
			break;
		case STRUCTDEF_TYPE_WORD :
			strcpy(pszResult,"WORD");
			break;
		default:
			strcpy(pszResult,"Unknown");
			return(FALSE);
	}
	return(TRUE);
}

BOOL CEditAffectDlg::GetValueString(const STRUCTDEF &sd, char *pszResult)
{
	BYTE *ptr = (BYTE*)(&m_aff) + sd.nOffset;
	switch(sd.nType)
	{
		case STRUCTDEF_TYPE_NONE :
			strcpy(pszResult,"");
			break;
		case STRUCTDEF_TYPE_BYTE :
			sprintf(pszResult,"%d",*((BYTE*)ptr));
			break;
		case STRUCTDEF_TYPE_INT :
			sprintf(pszResult,"%d",*((int*)ptr));
			break;
		case STRUCTDEF_TYPE_DWORD :
			MakeHexString(*((DWORD*)ptr),pszResult,sd.nPadding);
			break;
		case STRUCTDEF_TYPE_CHAR :
			if (sd.nLen == 1)
				sprintf(pszResult,"%d",*((char*)ptr));
			else
			{
				memset(pszResult,0,sd.nLen+1);
				memcpy(pszResult,ptr,sd.nLen);
			}
			break;
		case STRUCTDEF_TYPE_WORD :
			sprintf(pszResult,"%d",*((WORD*)ptr));
			break;
		default:
			strcpy(pszResult,"Error");
			return(FALSE);
	}

	return(TRUE);
}

void CEditAffectDlg::UpdateList(CEdit &edit, int nIndex, DWORD &dwValue)
{
	CString strText;
	edit.GetWindowText(strText);
	dwValue = strtoul(strText,NULL,0);

	char szBuf[101];
	GetValueString(_sdCreAff[nIndex],szBuf);
	m_lcList.SetItemText(nIndex,3,szBuf);
}

void CEditAffectDlg::UpdateList(CEdit &edit, int nIndex, int &nValue)
{
	CString strText;
	edit.GetWindowText(strText);
	nValue = strtol(strText,NULL,0);

	char szBuf[101];
	GetValueString(_sdCreAff[nIndex],szBuf);
	m_lcList.SetItemText(nIndex,3,szBuf);
}

void CEditAffectDlg::UpdateList(CEdit &edit, int nIndex, WORD &wValue)
{
	CString strText;
	edit.GetWindowText(strText);
	wValue = (WORD)strtol(strText,NULL,0);

	char szBuf[101];
	GetValueString(_sdCreAff[nIndex],szBuf);
	m_lcList.SetItemText(nIndex,3,szBuf);
}

void CEditAffectDlg::UpdateList(CEdit &edit, int nIndex, char *pchValue)
{
	CString strText;
	edit.GetWindowText(strText);

	memset(pchValue,0,8);
	int nLen = min(8,strText.GetLength());
	memcpy(pchValue,(const char*)strText,nLen);

	char szBuf[101];
	GetValueString(_sdCreAff[nIndex],szBuf);
	m_lcList.SetItemText(nIndex,3,szBuf);
}

void CEditAffectDlg::OnChangeAffFlags() 
{
	UpdateList(m_edFlags,INDEX_FLAGS,m_aff.dwFlags);
}

void CEditAffectDlg::OnChangeAffParam1() 
{
	UpdateList(m_edParam1,INDEX_PARAM1,m_aff.nNP1);
}

void CEditAffectDlg::OnChangeAffParam2() 
{
	UpdateList(m_edParam2,INDEX_PARAM2,m_aff.nNP2);
}

void CEditAffectDlg::OnChangeAffType() 
{
	UpdateList(m_edType,INDEX_TYPE,m_aff.dwAffectType);
}

void CEditAffectDlg::OnChangeAffTarget() 
{
	UpdateList(m_edTarget,INDEX_TARGET,m_aff.dwTargetType);
}

void CEditAffectDlg::OnChangeAffTime() 
{
	UpdateList(m_edTime,INDEX_TIME,m_aff.nTime);
}

void CEditAffectDlg::OnChangeAffProbability() 
{
	UpdateList(m_edProbability,INDEX_PROBABILITY,m_aff.wProbability1);
}

void CEditAffectDlg::OnChangeAffResource0() 
{
	UpdateList(m_edResource0,INDEX_RESOURCE0,m_aff.chResRef0);
}

void CEditAffectDlg::OnChangeAffResource1() 
{
	UpdateList(m_edResource1,INDEX_RESOURCE1,m_aff.chResRef1);
}

void CEditAffectDlg::OnChangeAffResource2() 
{
	UpdateList(m_edResource2,INDEX_RESOURCE2,m_aff.chResRef2);
}

void CEditAffectDlg::OnChangeAffResource3() 
{
	UpdateList(m_edResource3,INDEX_RESOURCE3,m_aff.chResRef3);
}

BOOL CEditAffectDlg::AssignData(const STRUCTDEF &sd, const char *pszValue)
{
	BYTE *ptr = (BYTE*)(&m_aff) + sd.nOffset;
	switch(sd.nType)
	{
		case STRUCTDEF_TYPE_NONE :
			return(FALSE);
		case STRUCTDEF_TYPE_BYTE :
			*((BYTE*)ptr) = (BYTE)strtol(pszValue,NULL,0);
			break;
		case STRUCTDEF_TYPE_INT :
			*((int*)ptr) = strtol(pszValue,NULL,0);
			break;
		case STRUCTDEF_TYPE_DWORD :
			*((DWORD*)ptr) = strtoul(pszValue,NULL,0);
			break;
		case STRUCTDEF_TYPE_CHAR :
			if (sd.nLen == 1)
				*((char*)ptr) = *pszValue;
			else
			{
				int nLen = min(sd.nLen,(int)strlen(pszValue));
				memset(ptr,0,sd.nLen);
				memcpy(ptr,pszValue,nLen);
			}
			break;
		case STRUCTDEF_TYPE_WORD :
			*((WORD*)ptr) = (WORD)strtol(pszValue,NULL,0);
			break;
		default:
			return(FALSE);
	}
	return(TRUE);
}

void CEditAffectDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	char szValue[101], szType[101];
	GetValueString(_sdCreAff[nIndex],szValue);
	GetDataType(_sdCreAff[nIndex],szType);

	CEditAffectDataDlg d;

	d.m_strName = _sdCreAff[nIndex].pszName;
	d.m_strValue = szValue;
	d.m_strDataType = szType;
	if (d.DoModal() == IDOK)
	{
		AssignData(_sdCreAff[nIndex],d.m_strValue);

		GetValueString(_sdCreAff[nIndex],szValue);
		m_lcList.SetItemText(nIndex,3,szValue);

		switch(nIndex)
		{
			case INDEX_TYPE :
				m_edType.SetWindowText(szValue);
				break;
			case INDEX_TARGET	:
				m_edTarget.SetWindowText(szValue);
				break;
			case INDEX_PARAM1 :
				m_edParam1.SetWindowText(szValue);
				break;
			case INDEX_PARAM2 :
				m_edParam2.SetWindowText(szValue);
				break;
			case INDEX_FLAGS :
				m_edFlags.SetWindowText(szValue);
				break;
			case INDEX_TIME :
				m_edTime.SetWindowText(szValue);
				break;
			case INDEX_PROBABILITY :
				m_edProbability.SetWindowText(szValue);
				break;
			case INDEX_RESOURCE0 :
				m_edResource0.SetWindowText(szValue);
				break;
			case INDEX_RESOURCE1 :
				m_edResource1.SetWindowText(szValue);
				break;
			case INDEX_RESOURCE2 :
				m_edResource2.SetWindowText(szValue);
				break;
			case INDEX_RESOURCE3 :
				m_edResource3.SetWindowText(szValue);
				break;
		}
	}
}
