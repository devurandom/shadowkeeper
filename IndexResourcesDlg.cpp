// IndexResourcesDlg.cpp : implementation file
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
#include "IndexResourcesDlg.h"
#include "InfKey.h"
#include "Inf2DA.h"
#include "InfTlk.h"
#include "SpellBitmaps.h"
#include "ValueList.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexResourcesDlg dialog


CIndexResourcesDlg::CIndexResourcesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIndexResourcesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexResourcesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIndexResourcesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexResourcesDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_pcStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexResourcesDlg, CDialog)
	//{{AFX_MSG_MAP(CIndexResourcesDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexResourcesDlg message handlers

BOOL CIndexResourcesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// The only way I could get the dialog to display itself before loading
	// the resources was to use a timer. I tried just posting a message to
	// the dialog but it would get called before the dialog was displayed.
	SetTimer(1,100,NULL);

	return TRUE;
}

void CIndexResourcesDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(1);

	if (!_infKey.Read(_strInstallPath+"Chitin.key",&m_pcStatus))
	{
		AddToLog("!! Failed to read Chitin.key\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!_spellBitmaps.LoadImages(_bDisableSpellIcons,&m_pcStatus))
	{
		AddToLog("!! Failed to load spell bitmaps.\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!ProcessList("ANIMATE",_vlAnimations))
	{
		AddToLog("!! Error reading ANIMATE.IDS\n\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!ProcessList("CLASS",_vlClass,FALSE,0,0,50))
	{
		AddToLog("!! Error reading CLASS.IDS\n\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!ProcessList("RACE",_vlRace,FALSE,0,0,50))
	{
		AddToLog("!! Error reading RACE.IDS\n\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!ProcessList("GENDER",_vlGender))
	{
		AddToLog("!! Error reading GENDER.IDS\n\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!ProcessList("EA",_vlEnemyAlly))
	{
		AddToLog("!! Error reading EA.IDS\n\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!ProcessList("STATE",_vlState))
	{
		AddToLog("!! Error reading STATE.IDS\n\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!ProcessList("SPELL",_vlSpells))
	{
		AddToLog("!! Error reading SPELL.IDS\n\n");
		EndDialog(IDCANCEL);
		return;
	}

	if (!Process2da("HATERACE",&_2daHateRace))
	{
		EndDialog(IDCANCEL);
		return;
	}

	if (!Process2da("WEAPPROF",&_2daWeapProf))
	{
		EndDialog(IDCANCEL);
		return;
	}

	if (!Process2da("KITLIST",&_2daKitList))
	{
		EndDialog(IDCANCEL);
		return;
	}

	AddToLog("Creating racial enemy list.\n");
	if (!CreateRacialEnemyList())
	{
		AddToLog("   Failed creating list.\n\n");
		EndDialog(IDCANCEL);
		return;
	}
	AddToLog("   List created.\n\n");

	AddToLog("Creating weapon proficiency list.\n");
	if (!CreateWeaponProfList())
	{
		AddToLog("   Failed creating list.\n\n");
		EndDialog(IDCANCEL);
		return;
	}
	AddToLog("   List created.\n\n");

	AddToLog("Creating kit ability list.\n");
	if (!CreateKitList())
	{
		AddToLog("   Failed creating list.\n\n");
		EndDialog(IDCANCEL);
		return;
	}
	AddToLog("   List created.\n\n");


	EndDialog(IDOK);
}

BOOL CIndexResourcesDlg::Process2da(const char *pszResource, CInf2DA *p2DA)
{
	CString strLog;
	strLog.Format("Reading %s.2da\n",pszResource);
	AddToLog(strLog);

	BYTE *pBuf;
	int nLen;

	if (!_infKey.GetResData(RESTYPE_2DA,pszResource,pBuf,nLen,TRUE))
	{
		strLog.Format("   Unable to read %s.2da\n\n",pszResource);
		AddToLog(strLog);
		return(FALSE);
	}
	if (!p2DA->Parse(pBuf))
	{
		strLog.Format("   Unable to parse %s.2da\n\n",pszResource);
		AddToLog(strLog);
		delete [] pBuf;
		return(FALSE);
	}

	delete [] pBuf;
	AddToLog("   Done reading resource.\n\n");

	return(TRUE);
}

BOOL CIndexResourcesDlg::GetLine(const char *pszBuf, int &nStart, CString &strLine)
{
	strLine.Empty();

	while(pszBuf[nStart] && pszBuf[nStart] != '\r' && pszBuf[nStart] != '\n')
	{
		strLine += pszBuf[nStart];
		nStart++;
	}

	// Read the newline that should follow a CR.
	if (pszBuf[nStart] && (pszBuf[nStart] == '\r' || pszBuf[nStart] == '\n'))
	{
		nStart++;		// Skip the \r.
		if (pszBuf[nStart] && (pszBuf[nStart] == '\n' || pszBuf[nStart] == '\r'))
			nStart++;	// Skip the \n.
	}

	return(TRUE);
}

BOOL CIndexResourcesDlg::SplitLine(const char *pszLine, int &nValue, CString &strText)
{
	CString strLine(pszLine);
	strLine.TrimLeft();
	strLine.TrimRight();

	int nIndex = strLine.Find(' ');
	if (nIndex == -1)
		return(FALSE);

	if (strLine.Left(2) == "0x")
		nValue = strtoul(strLine,NULL,0);
	else
		nValue = atoi(strLine);

	strText = strLine.Mid(nIndex+1);

	// Get rid of any underscores, replace with spaces. Also check for all caps.
	int nLen = strText.GetLength();
	BOOL bAllUpper = TRUE;

	char ch;
	nIndex = 0;
	while(nIndex < nLen)
	{
		ch = strText.GetAt(nIndex);

		if (ch == '_')
			strText.SetAt(nIndex,' ');

		if (isalpha(ch) && islower(ch))
			bAllUpper = FALSE;

		nIndex++;
	}

	if (bAllUpper)
	{
		nIndex = 0;
		while(nIndex < nLen)
		{
			if (nIndex && isupper(strText.GetAt(nIndex)) && isalnum(strText.GetAt(nIndex-1)))
				strText.SetAt(nIndex,tolower(strText.GetAt(nIndex)));
			nIndex++;
		}
	}

	strText.TrimRight();
	strText.TrimLeft();
	return(TRUE);
}

BOOL CIndexResourcesDlg::ProcessList(const char *pszResource, CValueList &vl, BOOL bUseRange, int nMin, int nMax, int nBracketThreshold)
{
	CString strResource;
	strResource.Format("Reading resource: %s.IDS\n",pszResource);
	AddToLog(strResource);

	CResInfo *pResInfo = _infKey.GetResInfo(RESTYPE_IDS,pszResource);
	if (!pResInfo)
	{
		AddToLog("   Unable to locate resource.\n");
		return(FALSE);
	}

	CInfBifFile *pFile = _infKey.GetBifFile(pResInfo);
	if (!pFile || !pFile->IsValid())
	{
		CString strFile;
		_infKey.GetBifFile(pResInfo,strFile);
		strResource.Format("   Unable to locate bif file.\n"
								 "      pFile: %X\n"
								 "      Filename: %s\n"
								 "      Index: %d\n"
								 "      Is Valid: %d\n",
								 pFile,(const char *)strFile,pResInfo->wBifKeyIndex,
								 (pFile ? pFile->IsValid() : 0));
		AddToLog(strResource);
		return(FALSE);
	}

	int nSize, nOffset;
	if (!pFile->GetDataOffsetAndSize(pResInfo,nOffset,nSize))
	{
		AddToLog("   Unable to locate resource data.\n");
		return(FALSE);
	}

	char *pszBuf = new char[nSize+1];
	if (!pszBuf)
		return(FALSE);
	pszBuf[nSize] = '\x0';

	pFile->Seek(nOffset,CFile::begin);
	pFile->Read(pszBuf,nSize);

	// If the first two bytes of the buffer are 0xFF then the text is encrypted
	// with an XOR key.
	char *pszUsePtr;
	int nUseSize;
	if ((BYTE)(pszBuf[0]) == 0xFF && (BYTE)(pszBuf[1]) == 0xFF)
	{
		nUseSize = nSize-2;
		pszUsePtr = pszBuf+2;

		for (int i=0;i<nUseSize;i++)
			pszUsePtr[i] ^= _xorKey[i%XOR_KEY_LEN];
	}
	else
	{
		nUseSize = nSize;
		pszUsePtr = pszBuf;
	}

	int nBufIndex;
	CString strLine;
	CValueItem vi;

	// Eat up that first line. I'm not sure if this is supposed to be the count of 
	// lines or not. It was close, but not quite right for the one type I looked at.
	nBufIndex = 0;
	GetLine(pszUsePtr,nBufIndex,strLine);
	while(nBufIndex < nUseSize)
	{
		GetLine(pszUsePtr,nBufIndex,strLine);
		SplitLine(strLine,vi.m_nValue,vi.m_strText);

		// If we are using a limiting range make, check it. Rather than assiging
		// the values to different variables and checking before adding to the
		// list I'm just adding them directly to the list. If they don't qualify
		// to be added I'm not incrementing the index count so it'll be overwritten
		// next time through.
		if (!bUseRange || (vi.m_nValue >= nMin && vi.m_nValue <= nMax))
		{
			if (nBracketThreshold != -1 && vi.m_nValue > nBracketThreshold)
				vi.m_strText = CString("[ ")+vi.m_strText+" ]";

			vl.AddTail(vi);
		}
	}

	delete [] pszBuf;

	AddToLog("   Done reading resource.\n\n");
	return(TRUE);
}

BOOL CIndexResourcesDlg::CreateRacialEnemyList()
{
	int nRows = _2daHateRace.GetRows();

	CValueItem vi;

	int nValue;
	for (int i=0;i<nRows;i++)
	{
		_2daHateRace.GetData(i,1,nValue);
		_infTlk.GetString(nValue,vi.m_strText);
		_2daHateRace.GetData(i,2,vi.m_nValue);

		_vlRacialEnemy.AddTail(vi);
	}

	return(TRUE);
}

BOOL CIndexResourcesDlg::CreateWeaponProfList()
{
	int nRows = _2daWeapProf.GetRows();

	CValueItem vi;

	int nStrRef, nValue;
	for (int i=0;i<nRows;i++)
	{
		_2daWeapProf.GetData(i,2,nStrRef);
		_2daWeapProf.GetData(i,1,nValue);

		// There are a bunch of entries labled "Extra" that I want to avoid
		// putting into the list. They all have a max value for their text string.
		//
		// 89 is the start of the new proficiencies. It looks like the older ones
		// from BG1 are still in it as well and I want to skip those.
		if (nValue < 89)
			continue;

		_infTlk.GetString(nStrRef,vi.m_strText);
		if (vi.m_strText.IsEmpty())
			continue;

		vi.m_nValue = nValue;

		_vlProfs.AddTail(vi);
	}

	return(TRUE);
}

BOOL CIndexResourcesDlg::CreateKitList()
{
	int nRows = _2daKitList.GetRows();

	CValueItem vi;

	CString strAbilityRef;
	int nStrRef;
	for (int i=0;i<nRows;i++)
	{
		// The table appears to have bad data in it. Not all of the kits have a 
		// valid link to another 2da file -- they just seem to be missing. I'm
		// making sure to only put kits in the list that have a valid link.
		_2daKitList.GetData(i,5,strAbilityRef);
		if (!_infKey.GetResInfo(RESTYPE_2DA,strAbilityRef))
			continue;

		_2daKitList.GetData(i,3,nStrRef);
		_infTlk.GetString(nStrRef,vi.m_strText);
		_2daKitList.GetData(i,0,vi.m_nValue);

		_vlKitList.AddTail(vi);
	}

	return(TRUE);
}
