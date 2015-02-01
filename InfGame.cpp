
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
#include "InfGame.h"
#include "Global.h"

CInfGame::CInfGame()
{
	m_pinfOutCharInfo = NULL;
	m_pinfOutParty = NULL;
	m_bHasChanged = FALSE;
	m_nError = ERR_NONE;
	m_pJournal = NULL;
	m_nJournalSize = 0;
	m_nAfterJournalSize = 0;
	m_pAfterJournal = NULL;
}

CInfGame::~CInfGame()
{
	if (m_pinfOutCharInfo)
		delete [] m_pinfOutCharInfo;
	if (m_pinfOutParty)
		delete [] m_pinfOutParty;
	if (m_pJournal)
		delete [] m_pJournal;
	if (m_pAfterJournal)
		delete [] m_pAfterJournal;

	INF_GAME_GLOBAL *pGlobal;
	POSITION pos = m_plGlobals.GetHeadPosition();
	while(pos)
	{	
		pGlobal = (INF_GAME_GLOBAL*)m_plGlobals.GetNext(pos);
		delete pGlobal;
	}
}

BOOL CInfGame::Read(const char *pszFilename)
{
	CFile file;
	UINT nCount;
	int i;

	if (!file.Open(pszFilename,CFile::modeRead))
	{
		m_nError = ERR_GAME_FAILEDOPEN;
		return(FALSE);
	}
	m_strFilename = pszFilename;

	nCount = file.Read(&m_infGame,sizeof(INF_GAME));
	if (nCount != sizeof(INF_GAME))
	{
		m_nError = ERR_GAME_BADHEADER;
		return(FALSE);
	}

	if (memcmp(m_infGame.chSignature,"GAME",4))
	{
		m_nError = ERR_GAME_MISSINGSIG;
		return(FALSE);
	}

	if (!_bIgnoreDataVersions && memcmp(m_infGame.chVersion,"V2.0",4) && memcmp(m_infGame.chVersion,"V2.1",4))
	{
		m_nError = ERR_GAME_BADVERSION;
		return(FALSE);
	}

	// After each section is read I'm checking the file position to be sure it follows
	// with how I expect the file to be built.
	// Character Info for in-party characters should follow the header.
	if (file.GetPosition() != m_infGame.dwInPartyCharOffset)
	{
		m_nError = ERR_GAME_BADPARTYCHARINFO;
		return(FALSE);
	}

	// I fixed it at a maximum of 6 for the editor.
	if (m_infGame.dwInPartyCharCount > INF_MAX_CHARACTERS)
	{
		m_nError = ERR_GAME_TOOMANYINPARTY;
		return(FALSE);
	}

	// Read in all the INF_CHARINFO records.
	nCount = file.Read(&m_infCharInfo[0],sizeof(INF_GAME_CHARINFO)*m_infGame.dwInPartyCharCount);
	if (nCount != sizeof(INF_GAME_CHARINFO)*m_infGame.dwInPartyCharCount)
	{
		m_nError = ERR_GAME_READPARTYCHARINFO;
		return(FALSE);
	}

	for (i=0;i<(int)m_infGame.dwInPartyCharCount;i++)
	{
		if (!m_infParty[i].Read(file))
		{
			m_nError = m_infParty[i].GetLastError();
			return(FALSE);
		}
	}

	// At this point the file should be pointing at the start of the creatures not in
	// the party data.
	if (file.GetPosition() != m_infGame.dwOutPartyCharOffset)
	{
		m_nError = ERR_GAME_BADOUTPARTYCHARINFO;
		return(FALSE);
	}

	if (m_infGame.dwOutPartyCharCount)
	{
		m_pinfOutCharInfo = new INF_GAME_CHARINFO[m_infGame.dwOutPartyCharCount];
		if (!m_pinfOutCharInfo)
		{
			m_nError = ERR_GAME_OOM_OUTPARTYCHARINFO;
			return(FALSE);
		}

		nCount = file.Read(&m_pinfOutCharInfo[0],sizeof(INF_GAME_CHARINFO)*m_infGame.dwOutPartyCharCount);
		if (nCount != sizeof(INF_GAME_CHARINFO)*m_infGame.dwOutPartyCharCount)
		{
			m_nError = ERR_GAME_READOUTPARTYCHARINFO;
			return(FALSE);
		}

		m_pinfOutParty = new CInfCreature[m_infGame.dwOutPartyCharCount];
		if (!m_pinfOutParty)
		{
			m_nError = ERR_GAME_OOM_OUTPARTYCRE;
			return(FALSE);
		}

		for (i=0;i<(int)m_infGame.dwOutPartyCharCount;i++)
		{
			if (!m_pinfOutParty[i].Read(file))
			{
				m_nError = m_pinfOutParty[i].GetLastError();
				return(FALSE);
			}
		}
	}

	// Should be looking at the variables now.
	if (file.GetPosition() != m_infGame.dwGlobalVarOffset)
	{
		m_nError = ERR_GAME_BADVARS;
		return(FALSE);
	}

	INF_GAME_GLOBAL *pGlobal;
	for (i=0;i<(int)m_infGame.dwGlobalVarCount;i++)
	{
		pGlobal = new INF_GAME_GLOBAL;
		if (!pGlobal)
		{
			m_nError = ERR_GAME_OOM_VARS;
			return(FALSE);
		}

		nCount = file.Read(pGlobal,sizeof(INF_GAME_GLOBAL));
		if (nCount != sizeof(INF_GAME_GLOBAL))
		{
			m_nError = ERR_GAME_READVARS;
			return(FALSE);
		}

		m_plGlobals.AddTail(pGlobal);
	}

	// Journal entries should follow the variables.
	if (file.GetPosition() != m_infGame.dwJournalOffset)
	{
		m_nError = ERR_GAME_BADJOURNAL;
		return(FALSE);
	}

	int nFileSize = file.GetLength();
	int nPos = file.GetPosition();

	// I'm not sure if this data is always here or not, but it will affect the size
	// of the journal data.
	if (m_infGame.dwAfterJournalOffset)
		m_nJournalSize = m_infGame.dwAfterJournalOffset - m_infGame.dwJournalOffset;
	else
		m_nJournalSize = nFileSize - nPos;

	m_pJournal = new BYTE[m_nJournalSize];
	if (!m_pJournal)
	{
		m_nError = ERR_GAME_OOM_JOURNAL;
		return(FALSE);
	}

	nCount = file.Read(m_pJournal,m_nJournalSize);
	if (nCount != (UINT)m_nJournalSize)
	{
		m_nError = ERR_GAME_READJOURNAL;
		return(FALSE);
	}

	// Read in that possible extra data after the journal entries.
	if (m_infGame.dwAfterJournalOffset)
	{
		nPos = file.GetPosition();
		m_nAfterJournalSize = nFileSize - nPos;
		m_pAfterJournal = new BYTE[m_nAfterJournalSize];
		if (!m_pAfterJournal)
		{
			m_nError = ERR_GAME_OOM_AFTERJOURNAL;
			return(FALSE);
		}

		nCount = file.Read(m_pAfterJournal,m_nAfterJournalSize);
		if (nCount != (UINT)m_nAfterJournalSize)
		{
			m_nError = ERR_GAME_READAFTERJOURNAL;
			return(FALSE);
		}
	}

	m_bHasChanged = FALSE;
	return(TRUE);
}

BOOL CInfGame::Write(const char *pszFilename)
{
	CFile file;
	UINT	nOffset;
	int i;

	nOffset = sizeof(INF_GAME);
	for (i=0;i<(int)m_infGame.dwInPartyCharCount;i++)
	{
		if (!i)
			m_infCharInfo[i].dwCREOffset = sizeof(INF_GAME)+m_infGame.dwInPartyCharCount*sizeof(INF_GAME_CHARINFO);
		else
			m_infCharInfo[i].dwCREOffset = m_infCharInfo[i-1].dwCREOffset + m_infCharInfo[i-1].dwCRESize;
		m_infCharInfo[i].dwCRESize = m_infParty[i].GetFileSpace();

		nOffset += sizeof(INF_GAME_CHARINFO);
		nOffset += m_infParty[i].GetFileSpace();
	}
	m_infGame.dwOutPartyCharOffset = nOffset;

	for (i=0;i<(int)m_infGame.dwOutPartyCharCount;i++)
	{
		if (!i)
			m_pinfOutCharInfo[i].dwCREOffset = m_infGame.dwOutPartyCharOffset+m_infGame.dwOutPartyCharCount*sizeof(INF_GAME_CHARINFO);
		else
			m_pinfOutCharInfo[i].dwCREOffset = m_pinfOutCharInfo[i-1].dwCREOffset+m_pinfOutCharInfo[i-1].dwCRESize;
		m_pinfOutCharInfo[i].dwCRESize = m_pinfOutParty[i].GetFileSpace();		

		nOffset += sizeof(INF_GAME_CHARINFO);
		nOffset += m_pinfOutParty[i].GetFileSpace();
	}
	m_infGame.dwGlobalVarOffset = nOffset;

	nOffset += m_infGame.dwGlobalVarCount * sizeof(INF_GAME_GLOBAL);
	m_infGame.dwJournalOffset = nOffset;

	if (m_infGame.dwAfterJournalOffset)
	{
		nOffset += m_nJournalSize;
		m_infGame.dwAfterJournalOffset = nOffset;
	}

	if (!file.Open(pszFilename,CFile::modeCreate|CFile::modeWrite))
	{
		m_nError = ERR_GAME_FAILEDCREATE;
		return(FALSE);
	}

	try{ file.Write(&m_infGame,sizeof(INF_GAME)); }
	catch(CFileException *pe)
	{
		m_nError = ERR_GAME_WRITEHEADER;
		pe->Delete();
		return(FALSE);
	}

	try{ file.Write(&m_infCharInfo[0],m_infGame.dwInPartyCharCount*sizeof(INF_GAME_CHARINFO)); }
	catch(CFileException *pe)
	{
		m_nError = ERR_GAME_WRITEPARTYCHARINFO;
		pe->Delete();
		return(FALSE);
	}

	for (i=0;i<(int)m_infGame.dwInPartyCharCount;i++)
		if (!m_infParty[i].Write(file))
		{
			m_nError = m_infParty[i].GetLastError();
			return(FALSE);
		}

	if (m_infGame.dwOutPartyCharCount > 0)
	{
		try{ file.Write(m_pinfOutCharInfo,m_infGame.dwOutPartyCharCount*sizeof(INF_GAME_CHARINFO)); }
		catch(CFileException *pe)
		{
			m_nError = ERR_GAME_WRITEOUTPARTYCHARINFO;
			pe->Delete();
			return(FALSE);
		}

		for (i=0;i<(int)m_infGame.dwOutPartyCharCount;i++)
		{
			if (!m_pinfOutParty[i].Write(file))
			{
				m_nError = m_pinfOutParty[i].GetLastError();
				return(FALSE);
			}
		}
	}

	if (m_infGame.dwGlobalVarCount)
	{
		INF_GAME_GLOBAL *pGlobal;
		POSITION pos = m_plGlobals.GetHeadPosition();
		while(pos)
		{
			pGlobal = (INF_GAME_GLOBAL*)m_plGlobals.GetNext(pos);

			try{ file.Write(pGlobal,sizeof(INF_GAME_GLOBAL)); }
			catch(CFileException *pe)
			{
				m_nError = ERR_GAME_WRITEVARIABLES;
				pe->Delete();
				return(FALSE);
			}
		}
	}

	if (m_nJournalSize)
	{
		try{ file.Write(m_pJournal,m_nJournalSize); }
		catch(CFileException *pe)
		{
			m_nError = ERR_GAME_WRITEJOURNAL;
			pe->Delete();
			return(FALSE);
		}
	}

	if (m_nAfterJournalSize)
	{
		try{ file.Write(m_pAfterJournal,m_nAfterJournalSize); }
		catch(CFileException *pe)
		{
			m_nError = ERR_GAME_WRITEAFTERJOURNAL;
			pe->Delete();
			return(FALSE);
		}
	}

	m_bHasChanged = FALSE;
	return(TRUE);
}

BOOL CInfGame::HasChanged()
{
	if (m_bHasChanged)
		return(TRUE);

	int i;
	for (i=0;i<(int)m_infGame.dwInPartyCharCount;i++)
		if (m_infParty[i].HasChanged())
			return(TRUE);

	for (i=0;i<(int)m_infGame.dwOutPartyCharCount;i++)
		if (m_pinfOutParty[i].HasChanged())
			return(TRUE);

	return(FALSE);
}

void CInfGame::SetPartyGold(DWORD dwGold)
{
	if (m_infGame.dwGold != dwGold)
		m_bHasChanged = TRUE;
	m_infGame.dwGold = dwGold;
}

void CInfGame::GetPartyCharName(int nChar, CString &strName)
{
	if (m_infParty[nChar].GetLongNameID() == 0xFFFFFFFF)
	{
		if (m_infCharInfo[nChar].szName[0])
			strName = m_infCharInfo[nChar].szName; 
		else
			m_infParty[nChar].GetName(strName);
	}
	else
		m_infParty[nChar].GetName(strName);
}

void CInfGame::SetPartyCharName(int nChar, const char *pszName)
{
	char szName[21];
	memset(szName,0,21);

	if (strlen(pszName) > 20)
		memcpy(szName,pszName,20);
	else
		strcpy(szName,pszName);

	if (m_infCharInfo[nChar].szName[0])
	{
		if (memcmp(m_infCharInfo[nChar].szName,szName,20))
			m_bHasChanged = TRUE;
		strcpy(m_infCharInfo[nChar].szName,szName);
	}
	else
		m_infParty[nChar].SetName(szName);
}

void CInfGame::GetOutOfPartyCharName(int nChar, CString &strName)
{
	if (m_pinfOutParty[nChar].GetLongNameID() == 0xFFFFFFFF)
	{
		if (m_pinfOutCharInfo[nChar].szName[0])
			strName = m_pinfOutCharInfo[nChar].szName; 
		else
			m_pinfOutParty[nChar].GetName(strName);
	}
	else
		m_pinfOutParty[nChar].GetName(strName);
}

void CInfGame::SetOutOfPartyCharName(int nChar, const char *pszName)
{
	char szName[21];
	memset(szName,0,21);

	if (strlen(pszName) > 20)
		memcpy(szName,pszName,20);
	else
		strcpy(szName,pszName);

	if (m_pinfOutCharInfo[nChar].szName[0])
	{
		if (memcmp(m_pinfOutCharInfo[nChar].szName,szName,20))
			m_bHasChanged = TRUE;
		strcpy(m_pinfOutCharInfo[nChar].szName,szName);
	}
	else
		m_pinfOutParty[nChar].SetName(szName);
}

void CInfGame::SetPartyReputation(BYTE chRep)
{
	chRep = VALIDATE_RANGE(chRep,0,20);
	if (m_infGame.chPartyReputation != chRep*10)
		m_bHasChanged = TRUE;
	m_infGame.chPartyReputation = chRep*10;

	for (int i=0;i<(int)m_infGame.dwInPartyCharCount;i++)
		m_infParty[i].SetReputation(chRep);
}

BOOL CInfGame::GetGlobals(CPtrList &list)
{
	if (m_plGlobals.GetCount() < 1)
		return(TRUE);

	INF_GAME_GLOBAL *pGlobal, *pNew;
	POSITION pos = m_plGlobals.GetHeadPosition();
	while(pos)
	{
		pGlobal = (INF_GAME_GLOBAL*)m_plGlobals.GetNext(pos);

		pNew = new INF_GAME_GLOBAL;
		if (!pNew)
			return(FALSE);

		memcpy(pNew,pGlobal,sizeof(INF_GAME_GLOBAL));

		list.AddTail(pNew);
	}

	return(TRUE);
}

void CInfGame::SetGlobals(CPtrList &list)
{
	INF_GAME_GLOBAL *pGlobal;
	POSITION pos = m_plGlobals.GetHeadPosition();
	while(pos)
	{
		pGlobal = (INF_GAME_GLOBAL*)m_plGlobals.GetNext(pos);
		delete pGlobal;
	}
	m_plGlobals.RemoveAll();

	pos = list.GetHeadPosition();
	while(pos)
	{
		pGlobal = (INF_GAME_GLOBAL*)list.GetNext(pos);
		m_plGlobals.AddTail(pGlobal);
	}
	m_infGame.dwGlobalVarCount = m_plGlobals.GetCount();

	list.RemoveAll();
}
