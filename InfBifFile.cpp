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
#include "InfBifFile.h"
#include "InfCreature.h"
#include "InfKey.h"
#include "Global.h"
#include "zlib.h"


CInfBifFile::CInfBifFile() : CFile()
{
	m_bAsOverride = FALSE;
	m_bIsValid = FALSE;
}

CInfBifFile::~CInfBifFile()
{
	CBifFileEntry *pFileEntry;
	CMapWordToOb *pBifEntries;
	WORD wKey;
	POSITION posBifEntries;

	POSITION posType = m_mapResType.GetStartPosition();
	while(posType)
	{
		m_mapResType.GetNextAssoc(posType,wKey,(CObject*&)pBifEntries);
		posBifEntries = pBifEntries->GetStartPosition();
		while(posBifEntries)
		{
			pBifEntries->GetNextAssoc(posBifEntries,wKey,(CObject*&)pFileEntry);
			delete pFileEntry;
		}

		delete pBifEntries;
	}
}

BOOL CInfBifFile::Open(const char *pszFilename, BOOL bAsOverride)
{
	char szLogBuf[1000];

	sprintf(szLogBuf,"      File: %s\n",pszFilename);
	AddToLog(szLogBuf);

	if (!CFile::Open(pszFilename,CFile::modeRead|CFile::shareDenyNone))
	{
		AddToLog("         Failed to open the file.\n");
		return(FALSE);
	}

	m_bAsOverride = bAsOverride;

	// Only need to read in the catalog if it is a real bif file.
	if (!m_bAsOverride)
	{
		AddToLog("         Reading BIF header.\n");
		if (Read(&m_bifHeader,sizeof(INF_BIF_HEADER)) != sizeof(INF_BIF_HEADER))
			return(FALSE);

		// Make sure it is a BIFF format I'm prepared to deal with. "BIFC" pops up a bit.
		if (memcmp(m_bifHeader.chSignature,"BIFF",4))
			return(FALSE);

		if (!_bIgnoreDataVersions && memcmp(m_bifHeader.chVersion,"V1  ",4))
			return(FALSE);

		AddToLog("         Seeking to file entries.\n");
		if (Seek(m_bifHeader.dwFileEntryOffset,CFile::begin) != (int)m_bifHeader.dwFileEntryOffset)
			return(FALSE);

		INF_BIF_FILEENTRY bifFileEntry;
		CMapWordToOb *pBifEntries;
		CBifFileEntry *pFileEntry;

		sprintf(szLogBuf,"         Number of file entries: %d\n",m_bifHeader.dwFileEntryCount);
		AddToLog(szLogBuf);
		for (int i=0;i<(int)m_bifHeader.dwFileEntryCount;i++)
		{
			if (Read(&bifFileEntry,sizeof(INF_BIF_FILEENTRY)) != sizeof(INF_BIF_FILEENTRY))
			{
				AddToLog("         Failed to read bif file entry.\n");
				return(FALSE);
			}

			// See if this resource type is already in the main map. If not add it.
			if (!m_mapResType.Lookup(bifFileEntry.wType,(CObject*&)pBifEntries))
			{
				pBifEntries = new CMapWordToOb;
				if (!pBifEntries)
				{
					AddToLog("         Failed to allocate memory for resource map.\n");
					return(FALSE);
				}
				m_mapResType.SetAt(bifFileEntry.wType,pBifEntries);
			}

			pFileEntry = new CBifFileEntry;
			if (!pFileEntry)
			{
				AddToLog("         Failed to allocate memory for file entry information.\n");
				return(FALSE);
			}

			pFileEntry->dwOffset = bifFileEntry.dwOffset;
			pFileEntry->dwSize = bifFileEntry.dwSize;

			pBifEntries->SetAt((WORD)bifFileEntry.dwLocator,pFileEntry);
		}
	}
	AddToLog("         Finished reading file entries.\n");

	m_strFilename = pszFilename;
	m_bIsValid = TRUE;

	return(TRUE);
}

BOOL CInfBifFile::GetDataOffsetAndSize(const CResInfo *pResInfo, int &nOffset, int &nSize)
{
	CMapWordToOb *pBifEntries;
	CBifFileEntry *pFileEntry;

	// If this is an override the offset is the beginning of the file and the size
	// is the size of the file.
	if (m_bAsOverride)
	{
		nOffset = 0;
		nSize = GetLength();
		return(TRUE);
	}

	if (!m_mapResType.Lookup(pResInfo->wResourceType,(CObject*&)pBifEntries))
		return(FALSE);

	if (!pBifEntries->Lookup(pResInfo->wLocator,(CObject*&)pFileEntry))
		return(FALSE);

	nSize = pFileEntry->dwSize;
	nOffset = pFileEntry->dwOffset;

	return(TRUE);
}

int CInfBifFile::GetData(const CResInfo *pResInfo, void *pData, int nBufSize)
{
	return(GetData(pResInfo->wResourceType, pResInfo->wLocator, pData, nBufSize));
}

int CInfBifFile::GetData(WORD wResourceType, WORD wLocator, void *pData, int nBufSize)
{
	CMapWordToOb *pBifEntries;
	CBifFileEntry *pFileEntry;
	DWORD dwBytesToRead;

	if (!m_bAsOverride)
	{
		if (!m_mapResType.Lookup(wResourceType,(CObject*&)pBifEntries))
			return(-1);

		if (!pBifEntries->Lookup(wLocator,(CObject*&)pFileEntry))
			return(-1);

		if (Seek(pFileEntry->dwOffset,CFile::begin) != (int)pFileEntry->dwOffset)
			return(-1);

		dwBytesToRead = min((DWORD)nBufSize,pFileEntry->dwSize);
	}
	else
	{
		SeekToBegin();
		dwBytesToRead = nBufSize;
	}

	if (Read(pData,dwBytesToRead) != dwBytesToRead)
		return(-1);

	return((m_bAsOverride ? 0 : pFileEntry->dwOffset));
}

int CInfBifFile::GetData(WORD wLocator, INF_ITM *pItem)
{
	return(GetData(RESTYPE_ITM,wLocator,pItem,sizeof(INF_ITM)));
}

int CInfBifFile::GetData(WORD wLocator, INF_SPL *pSpell)
{
	return(GetData(RESTYPE_SPL,wLocator,pSpell,sizeof(INF_SPL)));
}

int CInfBifFile::GetData(WORD wLocator, INF_CRE *pCre)
{
	return(GetData(RESTYPE_CRE,wLocator,pCre,sizeof(INF_CRE)));
}

/*int CInfBifFile::GetData(WORD wLocator, INF_BAM_HEADER *pBamHeader)
{
	return(GetData(RESTYPE_BAM,wLocator,pBamHeader,sizeof(INF_BAM_HEADER)));
} */

BOOL CInfBifFile::GetBam(const CResInfo *pResInfo, CInfBam &bam)
{
	if (pResInfo->wResourceType != RESTYPE_BAM)
		return(FALSE);

	int nDataOffset, nDataSize;
	if (!GetDataOffsetAndSize(pResInfo,nDataOffset,nDataSize))
		return(FALSE);

	BYTE *pData = new BYTE[nDataSize];
	if (!pData)
		return(FALSE);

	Seek(nDataOffset,CFile::begin);
	Read(pData,nDataSize);

	// If this is an uncompressed bam we can just attach the buffer to the
	// bam file object and be done with it.
	if (!memcmp(pData,"BAM ",4))
	{
		bam.SetBuffer(pData,nDataSize);
		return(TRUE);
	}

	unsigned long nExpandSize = *((int*)(pData+8));
	BYTE *pExpand = new BYTE[nExpandSize];

	if (!pExpand)
	{
		delete [] pData;
		return(FALSE);
	}

	if (uncompress(pExpand,&nExpandSize,pData+12,nDataSize-12) != Z_OK)
	{
		delete [] pData;
		delete [] pExpand;
		return(FALSE);
	}
	delete [] pData;

	bam.SetBuffer(pExpand,nExpandSize);

	return(TRUE);
}
