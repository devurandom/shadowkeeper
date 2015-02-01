
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
#include "InfKey.h"
#include "Global.h"

BYTE _xorKey[XOR_KEY_LEN] =
{
	0x88,0xa8,0x8f,0xba,0x8a,0xd3,0xb9,0xf5,0xed,0xb1,0xcf,0xea,0xaa,0xe4,0xb5,0xfb,
	0xeb,0x82,0xf9,0x90,0xca,0xc9,0xb5,0xe7,0xdc,0x8e,0xb7,0xac,0xee,0xf7,0xe0,0xca,
	0x8e,0xea,0xca,0x80,0xce,0xc5,0xad,0xb7,0xc4,0xd0,0x84,0x93,0xd5,0xf0,0xeb,0xc8,
	0xb4,0x9d,0xcc,0xaf,0xa5,0x95,0xba,0x99,0x87,0xd2,0x9d,0xe3,0x91,0xba,0x90,0xca
};


CInfKey::CInfKey()
{
	m_pBifInfo = NULL;
}

CInfKey::~CInfKey()
{
	delete [] m_pBifInfo;

	OnIdleCleanup();

	CObList *pResList;
	WORD wKey;
	POSITION posList;
	POSITION posType = m_mapResType.GetStartPosition();
	while(posType)
	{
		m_mapResType.GetNextAssoc(posType,wKey,(CObject*&)pResList);
		posList = pResList->GetHeadPosition();
		while(posList)
			delete (CResInfo*)pResList->GetNext(posList);

		delete pResList;
	}

	delete [] m_pBifFiles;
}

void CInfKey::OnIdleCleanup()
{
	POSITION pos = m_listTempBif.GetHeadPosition();
	while(pos)
		delete (CTempBifFile*)m_listTempBif.GetNext(pos);
	m_listTempBif.RemoveAll();
}

BOOL CInfKey::Read(const char *pszFilename, CProgressCtrl *pCtrl)
{
	CResPtr res;
	CBifPtr bif;
	CFile file;

	char szLogBuf[1000];

	AddToLog("Reading Chitin.key.\n");
	AddToLog("   Opening file.\n");
	if (!file.Open(pszFilename,CFile::modeRead|CFile::shareDenyNone))
		return(FALSE);

	AddToLog("   Reading header.\n");
	if (file.Read(&m_keyHeader,sizeof(INF_KEY_HEADER)) != sizeof(INF_KEY_HEADER))
		return(FALSE);

	AddToLog("   Allocating memory for file keys.\n");
	bif.ptr = new INF_KEY_BIF[m_keyHeader.dwBifCount];
	if (!bif.ptr)
		return(FALSE);

	if (file.Seek(m_keyHeader.dwBifOffset,CFile::begin) != (int)m_keyHeader.dwBifOffset)
		return(FALSE);

	AddToLog("   Reading keys.\n");
	if (file.Read(bif.ptr,sizeof(INF_KEY_BIF)*m_keyHeader.dwBifCount) != sizeof(INF_KEY_BIF)*m_keyHeader.dwBifCount)
		return(FALSE);

	AddToLog("   Allocating memory for resource keys.\n");
	res.ptr = new INF_KEY_RESOURCE[m_keyHeader.dwResourceCount];
	if (!res.ptr)
		return(FALSE);

	if (file.Seek(m_keyHeader.dwResourceOffset,CFile::begin) != (int)m_keyHeader.dwResourceOffset)
		return(FALSE);

	AddToLog("   Reading resource keys.\n");
	if (file.Read(res.ptr,sizeof(INF_KEY_RESOURCE)*m_keyHeader.dwResourceCount) != sizeof(INF_KEY_RESOURCE)*m_keyHeader.dwResourceCount)
		return(FALSE);

	// Find all the filename for the bif entries.
	AddToLog("   Allocating memory for bif information.\n");
	m_pBifInfo = new CBifInfo[m_keyHeader.dwBifCount];
	if (!m_pBifInfo)
		return(FALSE);

	AddToLog("   Allocating memory for bif files.\n");
	m_pBifFiles = new CInfBifFile[m_keyHeader.dwBifCount];
	if (!m_pBifFiles)
		return(FALSE);

	AddToLog("   Reading bif file information.\n");
	char szPath[MAX_PATH];
	for (int i=0;i<(int)m_keyHeader.dwBifCount;i++)
	{
		if (file.Seek(bif.ptr[i].dwFilenameOffset,CFile::begin) != (int)bif.ptr[i].dwFilenameOffset)
			return(FALSE);

		if (file.Read(szPath,bif.ptr[i].wFilenameLength) != bif.ptr[i].wFilenameLength)
			return(FALSE);

		// This is a fix for a Mac version. For some reason the filenames are listed with
		// a colon as the first character instead of a backslash.
		if (szPath[0] == ':')
			szPath[0] = '\\';

		sprintf(szLogBuf,"      %s\n",szPath);
		AddToLog(szLogBuf);
		m_pBifInfo[i].strFilename = szPath;
		m_pBifInfo[i].wLocation = bif.ptr[i].wFileLocation;
	}

	MSG msg;
	if (pCtrl)
	{
		// I'm multiplying the number of bif files by a larger number because there
		// are so many resources. If each file doesn't represent a large chunk the
		// progress control won't be able to show anything.
		pCtrl->SetRange32(0,m_keyHeader.dwResourceCount+m_keyHeader.dwBifCount*FILE_PROG_MULT);
		pCtrl->SetPos(0);
	}

	int nBcsCount = 0;

	AddToLog("   Indexing resources.\n");
	// I'm storing the names combined with the resource type to make a unique key.
	CResInfo *pResInfo;
	CObList	*pResList;
	char szName[9];
	for (i=0;i<(int)m_keyHeader.dwResourceCount;i++)
	{
		// I'm not using all the resources types. This is an attempt to speed it up
		// a little bit. The map classes take a little while to add all the items.
		switch(res.ptr[i].wType)
		{
			case RESTYPE_ITM :
			case RESTYPE_BAM :
			case RESTYPE_BMP :
			case RESTYPE_SPL :
			case RESTYPE_CRE :
			case RESTYPE_IDS :
			case RESTYPE_2DA :
			case RESTYPE_BS :
				break;

			default:
				continue;
		}

		// Find the resource.
		if (!m_mapResType.Lookup(res.ptr[i].wType,(CObject*&)pResList))
		{
			sprintf(szLogBuf,"      Allocating new map for resource type: %d\n",res.ptr[i].wType);
			AddToLog(szLogBuf);

			// If this type of resource isn't known yet, add one to the resource
			// type map.
			pResList = new CObList;
			if (!pResList)
				return(FALSE);
			AddToLog("      New map allocated.\n");
			m_mapResType.SetAt(res.ptr[i].wType,pResList);
		}

		memcpy(szName,res.ptr[i].chName,8);
		szName[8] = '\x0';

		pResInfo = new CResInfo;
		if (!pResInfo)
			return(FALSE);

		pResInfo->strName = szName;
		pResInfo->wBifKeyIndex = (WORD)((res.ptr[i].dwLocator & 0xFFF00000) >> 20);
		pResInfo->wLocator  = (WORD)(res.ptr[i].dwLocator & 0x00003FFF);
		pResInfo->chTileIndex = (BYTE)((res.ptr[i].dwLocator & 0x000FC000) >> 14);
		pResInfo->chFlags = 0;
		pResInfo->wResourceType = res.ptr[i].wType;

		pResList->AddTail(pResInfo);

		if (pCtrl)
		{
			pCtrl->SetPos(i+1);

			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	} 

	AddToLog("   Opening BIF files.\n");
	// Open up and index all the BIF files. I was doing this on an as needed
	// basis but it started being a pain since the files I tended to use were
	// used multiple times. Each time a bif file is opened I index all the 
	// resources in the file -- it was starting to add up when doing items.
	CString strBifFile;
	for (i=0;i<(int)m_keyHeader.dwBifCount;i++)
	{
		if (pCtrl)
		{
			pCtrl->SetPos(pCtrl->GetPos()+FILE_PROG_MULT);

			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		strBifFile = _strInstallPath + m_pBifInfo[i].strFilename;
		if (!m_pBifFiles[i].Open(strBifFile))
			continue;
	}

	RefreshOverride();

	return(TRUE);
}

BOOL CInfKey::GetResList(WORD wResourceType, CObList *&rList)
{
	return(m_mapResType.Lookup(wResourceType,(CObject*&)rList));
}

CResInfo* CInfKey::GetResInfo(WORD wResourceType, const char *pszResName)
{
	CObList *pList;
	if (!GetResList(wResourceType,pList))
		return(FALSE);

	CResInfo *pResInfo;
	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		pResInfo = (CResInfo*)pList->GetNext(pos);
		if (!strcmpi(pResInfo->strName,pszResName))
			return(pResInfo);
	}

	return(NULL);
}

BOOL CInfKey::GetBifFile(DWORD nBifKeyIndex, CString &strFilename)
{
	if (nBifKeyIndex < 0 || nBifKeyIndex >= m_keyHeader.dwBifCount)
		return(FALSE);

	strFilename = m_pBifInfo[nBifKeyIndex].strFilename;
	return(TRUE);
}

CInfBifFile* CInfKey::GetBifFile(DWORD nBifKeyIndex)
{
	if (nBifKeyIndex < 0 || nBifKeyIndex >= m_keyHeader.dwBifCount)
		return(NULL);

	return(&m_pBifFiles[nBifKeyIndex]);
}

BOOL CInfKey::GetBifFile(const CResInfo *pInfo, CString &strFilename)
{
	if (pInfo->IsFlagSet(RIF_USEOVERRIDE))
	{
		// Need to find the extension for this resource type.
		int nIndex = 0;
		while(_keyResOver[nIndex].szExt[0])
		{
			if (_keyResOver[nIndex].wResType == pInfo->wResourceType)
				break;
			nIndex++;
		}

		if (!_keyResOver[nIndex].szExt[0])
			return(FALSE);

		strFilename.Format("%s%s%s.%s",
			(const char *)_strInstallPath,
			DIR_OVERRIDE,
			(const char *)pInfo->strName,
			_keyResOver[nIndex].szExt);

		return(TRUE);
	}

	if (pInfo->wBifKeyIndex < 0 || pInfo->wBifKeyIndex >= m_keyHeader.dwBifCount)
		return(FALSE);

	strFilename = m_pBifInfo[pInfo->wBifKeyIndex].strFilename;
	return(TRUE);
}

CInfBifFile* CInfKey::GetBifFile(const CResInfo *pInfo, BOOL bIgnoreOverride)
{
	// If it isn't an overriden resource, get the actual bif file.
	if (!pInfo->IsFlagSet(RIF_USEOVERRIDE) || bIgnoreOverride)
		return(GetBifFile(pInfo->wBifKeyIndex));

	// Check and see if we already have it open in the temp list.
	CTempBifFile *pTempFile;
	POSITION pos = m_listTempBif.GetHeadPosition();
	while(pos)
	{
		pTempFile = (CTempBifFile*)m_listTempBif.GetNext(pos);
		if (pTempFile->wResType == pInfo->wResourceType && !strcmpi(pTempFile->strResName,pInfo->strName))
			return(&pTempFile->infFile);
	}

	// Need to find the extension for this resource type.
	int nIndex = 0;
	while(_keyResOver[nIndex].szExt[0])
	{
		if (_keyResOver[nIndex].wResType == pInfo->wResourceType)
			break;
		nIndex++;
	}

	if (!_keyResOver[nIndex].szExt[0])
		return(NULL);

	// Didn't find one, so need to create a new object and open the 
	// file.
	pTempFile = new CTempBifFile;

	CString strFile;
	strFile.Format("%s%s%s.%s",
		(const char *)_strInstallPath,
		DIR_OVERRIDE,
		(const char *)pInfo->strName,
		_keyResOver[nIndex].szExt);

	if (!pTempFile->infFile.Open(strFile,TRUE))
	{
		delete pTempFile;
		return(NULL);
	}

	pTempFile->wResType = pInfo->wResourceType;
	pTempFile->strResName = pInfo->strName;

	m_listTempBif.AddTail(pTempFile);

	return(&pTempFile->infFile);
}

BOOL CInfKey::GetResData(WORD wResourceType, const char *pszResName, BYTE *&pBuf, int &nLen, BOOL bAddNull)
{
	CResInfo *pResInfo = GetResInfo(wResourceType, pszResName);
	if (!pResInfo)
		return(FALSE);

	CInfBifFile *pBifFile = GetBifFile(pResInfo);
	if (!pBifFile || !pBifFile->IsValid())
		return(FALSE);

	int nDataOffset;
	if (!pBifFile->GetDataOffsetAndSize(pResInfo,nDataOffset,nLen))
		return(FALSE);

	pBuf = new BYTE[(bAddNull ? nLen+1 : nLen)];
	if (!pBuf)
		return(FALSE);

	if (pBifFile->GetData(pResInfo,pBuf,nLen) == -1)
	{
		delete [] pBuf;
		return(FALSE);
	}

	if (bAddNull)
		pBuf[nLen] = '\x0';
	return(TRUE);
}

void CInfKey::RefreshOverride()
{
	AddToLog("   Scanning override folder for additional resources:\n");
	// Check the override folder for additional resources. I'm not including
	// this in the progress control, it should be really fast to just check
	// the existence of the files.
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	BOOL bFoundFile;
	CString strFile;
	CString strRes;
	CResInfo *pResInfo;
	CObList	*pResList;
	char szLogBuf[1000];
	int nIndex;
	int i = 0;

	while(_keyResOver[i].szExt[0])
	{
		sprintf(szLogBuf,"      Resource extension: %s\n",_keyResOver[i].szExt);
		AddToLog(szLogBuf);

		strFile.Format("%s%s*.%s",(const char *)_strInstallPath,DIR_OVERRIDE,_keyResOver[i].szExt);
		hFind = ::FindFirstFile(strFile,&fd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			bFoundFile = TRUE;
			while(bFoundFile)
			{
				sprintf(szLogBuf,"         %s\n",fd.cFileName);
				AddToLog(szLogBuf);

				// Pull off the name up to the extension. That will be the resource ID.
				strRes = fd.cFileName;
				nIndex = strRes.Find('.');
				if (nIndex != -1)
					strRes = strRes.Left(nIndex);

				// See if we already have this resource information loaded.
				AddToLog("            Looking for existing resource.\n");
				pResInfo = GetResInfo(_keyResOver[i].wResType,strRes);
				if (pResInfo)
				{
					// We already have it, just mark it to use overriden version.
					pResInfo->SetFlag(RIF_USEOVERRIDE); 
					AddToLog("               Resources exists, using overriden version.\n");
				}
				else
				{
					// Don't have it. Must be a new item. So need to make a CResInfo object for it.
					if (GetResList(_keyResOver[i].wResType,pResList))
					{
						AddToLog("            Allocating memory for new resource.\n");
						pResInfo = new CResInfo;
						if (!pResInfo)
						{
							AddToLog("         Failed to allocate memory for new resrouce.\n");
							break;
						}

						// The only relevant members of the object are the name and that it is an
						// overriden item. The rest of the members are used for looking up in a BIF
						// file.
						pResInfo->strName = strRes;
						pResInfo->SetFlag(RIF_USEOVERRIDE|RIF_CUSTOM);
						pResInfo->wResourceType = _keyResOver[i].wResType;

						pResList->AddTail(pResInfo);
					}
				}

				bFoundFile = ::FindNextFile(hFind,&fd);
			}

			::FindClose(hFind);
		}
		i++;
	}


	AddToLog("   Rechecking CRE overrides.\n");
	// When refreshing, it's also possible that some of the Overriden
	// resources have been deleted. Need to remove the ovrride flag on those.
	// For the momement, CRE are the only ones I care about for this.
	// I don't want to delete any ResInfo objects at the point. It is possible
	// that a custom resource was deleted and should be removed from the list,
	// but I don't want to cause problems if anything has a pointer to the
	// resource at the time this is run.
	CObList	*m_pCreatures;
	CString	strOverridePath(_strInstallPath+DIR_OVERRIDE);
	CString	strCreFile;
	_infKey.GetResList(RESTYPE_CRE,m_pCreatures);
	POSITION pos = m_pCreatures->GetHeadPosition();
	while(pos)
	{
		pResInfo = (CResInfo*)m_pCreatures->GetNext(pos);
		if (!pResInfo)
			continue;

		if (pResInfo->IsFlagSet(RIF_USEOVERRIDE))
		{
			strCreFile = strOverridePath + pResInfo->strName + ".cre";
			if (::GetFileAttributes(strCreFile) == 0xFFFFFFFF)
				pResInfo->RemoveFlag(RIF_USEOVERRIDE);
		}
	}

	AddToLog("   Done with override refresh.\n\n");
}

