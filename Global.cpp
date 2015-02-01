
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

#include "StdAfx.h"
#include "DaleKeeper.h"
#include "SaveCreDlg.h"
#include "InfKey.h"
#include "InfTlk.h"
#include "InfCreature.h"
#include "Inf2DA.h"
#include "ItemBrowserCom.h"
#include "SpellBrowserCom.h"
#include "PalImageList.h"
#include "SpellBitmaps.h"
#include "ValueList.h"
#include "Global.h"

CString _strInstallPath;

CInfKey _infKey;
CInfTlk _infTlk;

CItemBrowserCom	_comItemBrowser;
CSpellBrowserCom	_comSpellBrowser;

CSpellBitmaps _spellBitmaps;

CPalImageList _ilPal;

CInf2DA _2daHateRace;
CInf2DA _2daWeapProf;
CInf2DA _2daKitList;

CValueList _vlAnimations;
CValueList _vlKit;
CValueList _vlGender;
CValueList _vlGenderUser;
CValueList _vlAlignment;
CValueList _vlRacialEnemy;
CValueList _vlRacialEnemyUser;
CValueList _vlNumAttacks;
CValueList _vlProfs;
CValueList _vlProfsUser;
CValueList _vlRace;
CValueList _vlRaceUser;
CValueList _vlClass;
CValueList _vlClassUser;
CValueList _vlKitList;
CValueList _vlAffects;
CValueList _vlEnemyAlly;
CValueList _vlState;
CValueList _vlSpells;

// Set to TRUE in CMainFrame when the message has been successfully processed.
BOOL _bResourcesLoaded = FALSE;

// When set to TRUE log file entries are generated. This can be set by putting
// LOG on the command line to start the editor.
BOOL _bLogFile = FALSE;

UINT _nClipFormatAff = 0;

//////////////////////////////////////////////////////////////////////
// Editor Settings
//

BOOL	_bUseKnownSpellLimit;
BOOL	_bUseMemSpellLimit;
BOOL	_bMemSpellsOnSave;
BOOL	_bDisableSpellIcons;

int	_nKnownSpellLimit;
int	_nMemSpellLimit;

BOOL	_bDefaultOpenSinglePlayer;
BOOL	_bUseGridLines;
BOOL	_bIgnoreDataVersions;
BOOL	_bAllowQuicksaves;

BOOL	_bAllowCHROverwrite;
BOOL	_bAllowCREOverwrite;
//
//////////////////////////////////////////////////////////////////////

#define PATH_KEY_COUNT	6
char *_pszInstall[PATH_KEY_COUNT] =
{
	"Software",
	"Microsoft",
	"Windows",
	"CurrentVersion",
	"App Paths",
	"BG2Main.exe"
};

BOOL FindInstallPath(CString &strPath)
{
	AddToLog("Addtempting to locate installation:\n",TRUE);

	CRegKey rk[PATH_KEY_COUNT];
	int i;

	strPath.Empty();

	AddToLog("   Checking registry.\n",TRUE);
	for (i=0;i<PATH_KEY_COUNT;i++)
		if (rk[i].Open((!i ? HKEY_LOCAL_MACHINE : rk[i-1].m_hKey),_pszInstall[i]) != ERROR_SUCCESS)
			break;

	// If it made it through to the end of the loop the last key in the array should have
	// the value we are looking for.
	if (i == PATH_KEY_COUNT)
	{
		AddToLog("   Registry key found.\n",TRUE);
		DWORD dwSize = _MAX_PATH;
		char szPath[_MAX_PATH+1];
		if (rk[PATH_KEY_COUNT-1].QueryValue(szPath,"Path",&dwSize) == ERROR_SUCCESS)
		{
			AddToLog("   Got path from key.\n",TRUE);
			strPath = szPath;
			strPath.TrimRight();
			if (!strPath.IsEmpty())
			{
				if (strPath.Right(1) != "\\")
					strPath += "\\";
				return(TRUE);
			}
			AddToLog("   Key was empty.\n",TRUE);
		}
		else
		{
			AddToLog("   Failed to read registry key value.\n",TRUE);
			return(FALSE);
		}
	}

	AddToLog("   Searching default installation directory on fixed disks.\n",TRUE);

	// If the registry entry wasn't found search the most likely places to find the file.
	// Assuming people left the default install directories is probably the next fastest
	// way to attempt to find the file. Look on each drive for the default path looking
	// for IDMain.exe.
	DWORD dwDrives = GetLogicalDrives();

	DWORD dwMask;
	CString strFile;
	for (i=2;i<32;i++)
	{	
		dwMask = (DWORD)pow(2,i);
		if (dwDrives&dwMask)
		{
			strFile.Format("%c:\\",'A'+i);
			if (GetDriveType(strFile) == DRIVE_FIXED)
			{
				strFile.Format("%c:\\Program Files\\Black Isle\\BGII - SoA\\dialog.tlk",'A'+i);
				if (GetFileAttributes(strFile) != 0xFFFFFFFF)
				{
					strPath.Format("%c:\\Program Files\\Black Isle\\BGII - SoA\\",'A'+i);
					CString strLog;
					strLog.Format("   Found path: %s\n",(const char *)strPath);
					AddToLog(strLog,TRUE);
					return(TRUE);
				}
			}
		}
	}

	AddToLog("   Failed to locate Shadows of Amn installation.\n",TRUE);
	return(FALSE);
}

void MakeHexString(DWORD dwValue, char *pszResult, int nZeroPad)
{
	char szFormat[15];

	sprintf(szFormat,"0x%%0%dX",nZeroPad);
	sprintf(pszResult,szFormat,dwValue);
}

void ResizeLastColumn(CListCtrl &lc, BOOL bSubractScroll)
{
	CHeaderCtrl *pHeader = lc.GetHeaderCtrl();
	ASSERT(pHeader);

	int nCount = pHeader->GetItemCount();
	if (!nCount)
		return;

	int nTotal = 0;
	CRect rHeader;
	pHeader->GetClientRect(&rHeader);
	HDITEM hdi;
	hdi.mask = HDI_WIDTH;

	for(int i=0;i<nCount-1;i++)
	{
		pHeader->GetItem(i,&hdi);
		if (hdi.mask & HDI_WIDTH)
			nTotal += hdi.cxy;
	}

	hdi.cxy = rHeader.Width() - nTotal;
	if (bSubractScroll)
		hdi.cxy -= ::GetSystemMetrics(SM_CXHSCROLL);

	pHeader->SetItem(nCount-1,&hdi);
}

KEY_RES_OVERRIDE _keyResOver[] =
{
	{RESTYPE_BMP,"bmp"},
	{RESTYPE_BAM,"bam"},
	{RESTYPE_ITM,"itm"},
	{RESTYPE_SPL,"spl"},
	{RESTYPE_IDS,"ids"},
	{RESTYPE_CRE,"cre"},
	{RESTYPE_2DA,"2da"},
	{RESTYPE_BCS,"bcs"},
	{0,""}
};

ITEMTYPE _itemType[] =
{
	{"Misc",ITEMTYPE_MISC},
	{"Necklace",ITEMTYPE_NECKLACE},
	{"Armor",ITEMTYPE_ARMOR},
	{"Belt",ITEMTYPE_BELT},
	{"Boots",ITEMTYPE_BOOTS},
	{"Arrows",ITEMTYPE_ARROWS},
	{"Gloves",ITEMTYPE_GLOVES},
	{"Helm",ITEMTYPE_HELM},
	{"Key",ITEMTYPE_KEY},
	{"Potion",ITEMTYPE_POTION},
	{"Ring",ITEMTYPE_RING},
	{"Scroll",ITEMTYPE_SCROLL},
	{"Shield",ITEMTYPE_SHIELD},
	{"Food",ITEMTYPE_FOOD},
	{"Bullets",ITEMTYPE_BULLETS},
	{"Bow",ITEMTYPE_BOW},
	{"Dagger",ITEMTYPE_DAGGER},
	{"Mace",ITEMTYPE_MACE},
	{"Sling",ITEMTYPE_SLING},
	{"Short Sword",ITEMTYPE_SHORTSWORD},
	{"Sword",ITEMTYPE_SWORD},
	{"Hammer",ITEMTYPE_HAMMER},
	{"Morning Star",ITEMTYPE_MORNINGSTAR},
	{"Flail",ITEMTYPE_FLAIL},
	{"Darts",ITEMTYPE_DART},
	{"Axe",ITEMTYPE_AXE},
	{"Staff",ITEMTYPE_STAFF},
	{"Crossbow",ITEMTYPE_CROSSBOW},
	{"Hand to Hand",ITEMTYPE_HANDTOHAND},
	{"Spear",ITEMTYPE_SPEAR},
	{"Halberd",ITEMTYPE_HALBERD},
	{"Bolts",ITEMTYPE_BOLTS},
	{"Cloak",ITEMTYPE_CLOAK},
	{"Gold",ITEMTYPE_GOLD},
	{"Gem",ITEMTYPE_GEM},
	{"Wand",ITEMTYPE_WAND},
	{"Container",ITEMTYPE_CONTAINER},
	{"Book",ITEMTYPE_BOOK},
	{"Familiar",ITEMTYPE_FAMILIAR},
	{"Misc2",ITEMTYPE_MISC2},
	{"Misc3",ITEMTYPE_MISC3},
	{"Drink",ITEMTYPE_DRINK},
	{"Greatsword",ITEMTYPE_GREATSWORD},
	{"Instrument",ITEMTYPE_INSTRUMENT},
	{"Large Shield",ITEMTYPE_LARGESHIELD},
	{"Medium Shield",ITEMTYPE_MEDIUMSHIELD},
	{"Small Shield",ITEMTYPE_SMALLSHIELD},
	{"Club",ITEMTYPE_CLUB},
	{"Unknown",0xFFFF},	// This is a special case used by the item browser. It needs to stay the last item.
	{NULL,0}
};

BOOL GetItemType(WORD wValue, CString &strType, int &nIndex)
{
	nIndex = 0;
	while(_itemType[nIndex].pszType)
	{
		if (_itemType[nIndex].wType == wValue)
		{
			strType = _itemType[nIndex].pszType;
			return(TRUE);
		}
		nIndex++;
	}

	// Added an "Unknown" type to the end of the list to make the item browser
	// a little easier to deal with. If the item being lookd for is not in the
	// list then I'm returning the index to the "Unknown" item which is always
	// the versy last valid item in the list.
	nIndex--;
	strType = _itemType[nIndex].pszType;

	return(TRUE);
}

// This needs to match the number of possible slots in a CRE.
ITEMPOS _itemPos[] = 
{
	"Head",
	"Body",
	"Shield",
	"Hands",
	"Ring",
	"Ring",
	"Neck",
	"Waist",
	"Feet",
	"Weapon 1",
	"Weapon 2",
	"Weapon 3",
	"Weapon 4",
	"Quiver 1",
	"Quiver 2",
	"Quiver 3",
	"Unused",
	"Back",
	"Quick 1",
	"Quick 2",
	"Quick 3",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory",
	"Inventory"
};

CResInfo* GetItem(const char *chRes, INF_ITM *pItm)
{
	char szRes[9];
	memcpy(szRes,chRes,8);
	szRes[8] = '\x0';

	CResInfo *pResInfo = _infKey.GetResInfo(RESTYPE_ITM,szRes);
	if (!pResInfo)
		return(NULL);

	CInfBifFile *pItemFile = _infKey.GetBifFile(pResInfo);
	if (!pItemFile || !pItemFile->IsValid())
		return(NULL);

	if (pItemFile->GetData(pResInfo->wLocator,pItm) == -1)
		return(NULL);

	return(pResInfo);
}

void FixTlkDescription(const char *pszDesc, CString &strFixed)
{
	strFixed.Empty();

	int nIndex = 0;
	int nLen = strlen(pszDesc);
	unsigned char ch;

	while(nIndex < nLen)
	{
		ch = pszDesc[nIndex];

		// Make sure every carriage return is followed by a linefeed.
		if (ch == '\r')
		{
			strFixed += ch;			// add the \r 
			nIndex++;
			strFixed += '\n';		// add a \n no matter what

			if (nIndex < nLen && pszDesc[nIndex] == '\n') // if we happened to have a linefeed, skip it now
				nIndex++;
			continue;
		}

		if (ch == '\n')
		{
			strFixed += "\r\n";	// always want \r\n order.
			nIndex++;
			if (nIndex < nLen && pszDesc[nIndex] == '\r') // must have been in reverse order.
				nIndex++;
			continue;
		}

		strFixed += ch;
		nIndex++;
	}
}

BOOL GetItemBitmap(const CResInfo *pInfo, INF_ITM &itm, int nFrameWanted, COLORREF clrTrans, HBITMAP &hBitmap)
{
	char szIconRes[9];
	memset(szIconRes,0,9);
	memcpy(szIconRes,itm.chItemIcon,8);

	CResInfo *pBamResInfo = _infKey.GetResInfo(RESTYPE_BAM,szIconRes);
	if (!pBamResInfo)
		return(FALSE);

	CInfBifFile *pBamFile = _infKey.GetBifFile(pBamResInfo);
	if (!pBamFile || !pBamFile->IsValid())
		return(FALSE);

	CInfBam bam;
	if (!pBamFile->GetBam(pBamResInfo,bam))
		return(FALSE);

	return(bam.MakeBitmap(nFrameWanted,clrTrans,hBitmap,TRUE,32,32));
}

BOOL GetSpellBitmap(const CResInfo *pInfo, INF_SPL &spl, COLORREF clrTrans, HBITMAP &hBitmap)
{
	char szIconRes[9];
	memset(szIconRes,0,9);
	memcpy(szIconRes,spl.chSpellIcon,8);

	CResInfo *pBamResInfo = _infKey.GetResInfo(RESTYPE_BAM,szIconRes);
	if (!pBamResInfo)
		return(FALSE);

	CInfBifFile *pBamFile = _infKey.GetBifFile(pBamResInfo);
	if (!pBamFile || !pBamFile->IsValid())
		return(FALSE);

	CInfBam bam;
	if (!pBamFile->GetBam(pBamResInfo,bam))
		return(FALSE);

	return(bam.MakeBitmap(0,clrTrans,hBitmap,TRUE,32,32));
}

BOOL ResizeBitmap(int nWidth, int nHeight, HBITMAP &hBitmap)
{
	CBitmap *pBmp = CBitmap::FromHandle(hBitmap);
	if (!pBmp)
		return(FALSE);

	BITMAP bm;
	pBmp->GetBitmap(&bm);
	
	// If already that size, no need to do anything.
	if (bm.bmWidth == nWidth && bm.bmHeight == nHeight)
		return(TRUE);

	CDC *pDC = AfxGetMainWnd()->GetDC();
	if (!pDC)
		return(FALSE);

	// DC to use to create the new image.
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	// Need to have the original bitmap in a dc to manipulate it.
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(pDC);
	CBitmap *pOldBmp = dcBmp.SelectObject(pBmp);

	// Need a new bitmap in the memory dc to copy onto.
	CBitmap bmpNew;
	bmpNew.CreateCompatibleBitmap(pDC,nWidth,nHeight);
	CBitmap *pOldMemBmp = dcMem.SelectObject(&bmpNew);

	dcMem.SetStretchBltMode(HALFTONE);
	dcMem.SetBrushOrg(0,0);
	dcMem.StretchBlt(0,0,nWidth,nHeight,&dcBmp,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	// Select out the bitmaps.
	dcMem.SelectObject(pOldMemBmp);
	dcBmp.SelectObject(pOldBmp);

	AfxGetMainWnd()->ReleaseDC(pDC);

	// Delete the original and point it at the new one.
	pBmp->Detach();
	::DeleteObject(hBitmap);

	hBitmap = (HBITMAP)bmpNew;
	bmpNew.Detach();

	return(TRUE);

}

BOOL GetResourceBitmap(const char *pszName, HBITMAP &hBitmap, int nWidth, int nHeight)
{
	CResInfo *pBmpInfo = _infKey.GetResInfo(RESTYPE_BMP,pszName);
	if (!pBmpInfo)
	{
		// Didn't find it as a game resource. Check the Portraits directory
		// for a custom bitmap.

		CString strPath = _strInstallPath + CString(DIR_PORTRAITS) + CString(pszName) + ".bmp";
		hBitmap = (HBITMAP)::LoadImage(NULL,strPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		if (!hBitmap)
			return(FALSE);

		if (nWidth || nHeight)
			return(ResizeBitmap(nWidth,nHeight,hBitmap));

		return(TRUE);
	}

	// Found it in the game resource index.
	CInfBifFile *pBmpFile = _infKey.GetBifFile(pBmpInfo);
	if (!pBmpFile || !pBmpFile->IsValid())
		return(FALSE);
	int nDataSize, nDataOffset;
	if (!pBmpFile->GetDataOffsetAndSize(pBmpInfo,nDataOffset,nDataSize))
		return(FALSE);

	BYTE *pBmp = new BYTE[nDataSize];
	if (pBmpFile->GetData(pBmpInfo,pBmp,nDataSize) == -1)
	{
		delete [] pBmpFile;
		return(FALSE);
	}

	CDC *pDC = AfxGetMainWnd()->GetDC();
	hBitmap = ::CreateDIBitmap(
		pDC->GetSafeHdc(),
		(BITMAPINFOHEADER*)(pBmp+sizeof(BITMAPFILEHEADER)),
		CBM_INIT,
		pBmp+((BITMAPFILEHEADER*)pBmp)->bfOffBits,
		(BITMAPINFO*)(pBmp+sizeof(BITMAPFILEHEADER)),
		DIB_RGB_COLORS);
	AfxGetMainWnd()->ReleaseDC(pDC);

	delete [] pBmp;

	if (nWidth || nHeight)
		return(ResizeBitmap(nWidth,nHeight,hBitmap));

	return(TRUE);
}

BOOL ExtractResource(WORD wResourceType, const char *pszResource, const char *pszPath)
{
	int nIndex = 0;
	while(_keyResOver[nIndex].wResType && _keyResOver[nIndex].wResType != wResourceType)
		nIndex++;

	if (!_keyResOver[nIndex].wResType)
		return(FALSE);

	CString strFile;
	strFile.Format("%s%s.%s",pszPath,pszResource,_keyResOver[nIndex].szExt);

	CFileStatus fs;
	if (CFile::GetStatus(strFile,fs))
	{
		CString strMessage;
		strMessage.Format("'%s' already exists. Do you want to overwrite the file?",(const char *)strFile);
		int nResult = AfxGetMainWnd()->MessageBox(strMessage,"File Exists",MB_YESNO|MB_ICONQUESTION);
		if (nResult == IDNO)
			return(FALSE);
	}

	BYTE *pBuf;
	int  nDataSize;
	if (!_infKey.GetResData(wResourceType,pszResource,pBuf,nDataSize))
		return(FALSE);

	CFile file;
	if (!file.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		delete[] pBuf;
		return(FALSE);
	}

	file.Write(pBuf,nDataSize);
	file.Close();

	delete [] pBuf;
	return(TRUE);
}

BOOL SaveCreFile(CInfCreature *pCre, const char *pszFilename, CString &strSavedAs)
{
	CString strFile;
	if (_bAllowCREOverwrite)
	{
		CFileDialog d(
			FALSE,
			"*.cre",
			pszFilename,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Creature Files (*.cre)|*.cre|All Files (*.*)|*.*||");

		if (d.DoModal() == IDCANCEL)
			return(FALSE);

		strFile = d.GetPathName();

		// If the file exists, make a backup of it.
		if (::GetFileAttributes(strFile) != 0xFFFFFFFF)
		{
			CString strBackup(strFile);
			if (strBackup.GetLength() > 4)
			{
				strBackup.TrimRight();
				CString strExt(strBackup.Right(4));
				strExt.MakeLower();

				if (strExt == ".cre")
					strBackup = strBackup.Left(strBackup.GetLength()-4);
				strBackup += ".bak";
			}
			else
				strBackup += ".bak";

			if (!::CopyFile(pszFilename,strBackup,FALSE))
			{
				AfxMessageBox("Unable to create a backup of this creature file. Save Canceled.");
				return(FALSE);
			}
		}
	}
	else
	{
		CSaveCreDlg d;
		d.m_strPath = pszFilename;
		if (d.DoModal() == IDCANCEL)
			return(FALSE);

		strFile = d.m_strPath;
	}

	CFile file;
	if (!file.Open(strFile,CFile::modeCreate|CFile::modeWrite))
	{
		CString strMessage;
		strMessage.Format("Unable to create the creature file: %s",(const char *)strFile);
		AfxGetMainWnd()->MessageBox(strMessage,"Failed to Create File",MB_ICONEXCLAMATION);
		return(FALSE);
	}

	if (!pCre->Write(file))
	{
		CString strError, strMessage;
		strError.LoadString(pCre->GetLastError());
		strMessage.Format("Unable to save the creature file.\r\n\r\nError (%d): %s",
			pCre->GetLastError(),(const char*)strError);
		AfxGetMainWnd()->MessageBox(strMessage,"Failed to Save Creature",MB_ICONEXCLAMATION);
		return(FALSE);
	}
	file.Close();

	strSavedAs = strFile;

	_infKey.RefreshOverride();

	return(TRUE);
}

void AddToLog(const char *pszString, BOOL bForceWrite)
{
	if (!bForceWrite && !_bLogFile)
		return;

	CStdioFile file;

	if (!file.Open(LOGFILE,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::typeText))
		return;

	file.SeekToEnd();

	file.WriteString(pszString);
	file.Close();
}

CString GetSpellScriptName(const char *pszResName)
{
	CString strResult;
	CString strResName(pszResName);

	if (strResName.GetLength() < 4)
		return(strResult);

	strResName.MakeLower();

	int nType;
	if (strResName.Find("wi") != -1)
		nType = 2;
	else
		if (strResName.Find("pr") != -1)
			nType = 1;
		else
			if (strResName.Find("in") != -1)
				nType = 3;
			else
				if (strResName.Find("cl") != -1)
					nType = 4;
				else
					return(strResult);

	char szBuf[100];
	sprintf(szBuf,"%d%s",nType,pszResName+4);
	int nValue = atoi(szBuf);
	if (_vlSpells.GetText(nValue,strResult))
		return(strResult);

	return(strResult);
}

