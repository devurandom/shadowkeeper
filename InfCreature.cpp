
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
#include "InfCreature.h"
#include "InfTlk.h"
#include "Global.h"

CInfCreature::CInfCreature()
{
	m_pMemInfo = NULL;
	m_pKnownSpells = NULL;
	m_pMemSpells = NULL;
	m_bHasChanged = FALSE;
	m_nError = ERR_NONE;

	for (int i=0;i<INF_CRE_SPELLTYPES;i++)
	{
		m_pSpellData[i] = NULL;
		m_nSpellCount[i] = 0;
	}
}

CInfCreature::~CInfCreature()
{
	delete [] m_pMemInfo;

	// These two are really temporary buffers but I have so many exit points on the
	// reading and writing that I left it up to the object to just clean them up
	// when destroyed.
	delete [] m_pKnownSpells;
	delete [] m_pMemSpells;

	for (int i=0;i<INF_CRE_SPELLTYPES;i++)
		delete [] m_pSpellData[i];

	POSITION pos = m_plAffects.GetHeadPosition();
	while(pos)
		delete (INF_AFF*)m_plAffects.GetNext(pos);

	pos = m_plMemButNotKnown.GetHeadPosition();
	while(pos)
		delete (SPELLDATA*)m_plMemButNotKnown.GetNext(pos);
}

BOOL CInfCreature::Read(CFile &file)
{
	UINT nCount;
	int i;

	UINT nStartPos = file.GetPosition();

	nCount = file.Read(&m_infCre,sizeof(INF_CRE));
	if (nCount != sizeof(INF_CRE))
	{
		m_nError = ERR_CRE_READHEADER;
		return(FALSE);
	}

	if (memcmp(m_infCre.chSignature,"CRE ",4))
	{
		m_nError = ERR_CRE_MISSINGSIG;
		return(FALSE);
	}

	if (!_bIgnoreDataVersions && memcmp(m_infCre.chVersion,"V1.0",4))
	{
		m_nError = ERR_CRE_BADVERSION;
		return(FALSE);
	}

	// Known spells should follow the header.
	if (m_infCre.dwKnownSpellsOffset)
	{
		try { file.Seek(m_infCre.dwKnownSpellsOffset+nStartPos,CFile::begin); }
		catch(CFileException *pe)
		{
			pe->Delete();
			m_nError = ERR_CRE_READBADKNOWN;
			return(FALSE);
		}

		m_pKnownSpells = new INF_CRE_KNOWNSPELL[m_infCre.dwKnownSpellsCount];
		if (!m_pKnownSpells)
		{
			m_nError = ERR_CRE_OOM_KNOWNSPELLS;
			return(FALSE);
		}

		nCount = file.Read(m_pKnownSpells,sizeof(INF_CRE_KNOWNSPELL)*m_infCre.dwKnownSpellsCount);
		if (nCount != sizeof(INF_CRE_KNOWNSPELL)*m_infCre.dwKnownSpellsCount)
		{
			m_nError = ERR_CRE_READKNOWN;
			return(FALSE);
		}
	}

	// Spell memorization information should follow the known spells listing.
	if (m_infCre.dwSpellMemorizationInfoOffset)
	{
		try { file.Seek(m_infCre.dwSpellMemorizationInfoOffset+nStartPos,CFile::begin); }
		catch(CFileException *pe)
		{
			pe->Delete();
			m_nError = ERR_CRE_READBADMEMINFO;
			return(FALSE);
		}

		m_pMemInfo = new INF_CRE_MEMINFO[m_infCre.dwSpellMemorizationInfoCount];
		if (!m_pMemInfo)
		{
			m_nError = ERR_CRE_OOM_MEMINFO;
			return(FALSE);
		}

		nCount = file.Read(m_pMemInfo,sizeof(INF_CRE_MEMINFO)*m_infCre.dwSpellMemorizationInfoCount);
		if (nCount != sizeof(INF_CRE_MEMINFO)*m_infCre.dwSpellMemorizationInfoCount)
		{
			m_nError = ERR_CRE_READMEMINFO;
			return(FALSE);
		}
	}

	// Memorized spells should be next.
	if (m_infCre.dwMemorizedSpellsOffset)
	{
		try { file.Seek(m_infCre.dwMemorizedSpellsOffset+nStartPos,CFile::begin); }
		catch(CFileException *pe)
		{
			pe->Delete();
			m_nError = ERR_CRE_READBADMEM;
			return(FALSE);
		}

		m_pMemSpells = new INF_CRE_MEMSPELL[m_infCre.dwMemorizedSpellsCount];
		if (!m_pMemSpells)
		{
			m_nError = ERR_CRE_OOM_MEMSPELL;
			return(FALSE);
		}

		nCount = file.Read(m_pMemSpells,sizeof(INF_CRE_MEMSPELL)*m_infCre.dwMemorizedSpellsCount);
		if (nCount != sizeof(INF_CRE_MEMSPELL)*m_infCre.dwMemorizedSpellsCount)
		{
			m_nError = ERR_CRE_READMEM;
			return(FALSE);
		}
	}

	///////////////////////////////////////////////////////////////////
	// Convert the CRE data into the SPELLDATA format.
	//

	// Need to first know how many of each type there are so the memory
	// can be allocated.
	// 10/7/2000 - I was assuming that there would be a single entry in
	//		the Known Spells section for each spell known. This is probably 
	//		a safe assumption, as that would mean the same spell in your
	//		book more than once. I hit a problem with a player who had
	//		the same innate spell listed more than once. This caused grief
	//		with the below conversion. The spell counts we get off and the
	//		offsets in the written file would be incorrect. I'm now making	
	//		sure a Known Spell isn't counted more than one time. I suspect
	//		the character that had the duplicates was edited by hand.
	CStringList slKnown[INF_CRE_SPELLTYPES];
	CString strSpell;
	char szSpell[10];
	memset(szSpell,0,10);

	for (i=0;i<(int)m_infCre.dwKnownSpellsCount;i++)
	{
		if (m_pKnownSpells[i].wSpellType >= INF_CRE_SPELLTYPES)
		{
			m_nError = ERR_CRE_UNKNOWNSPELLTYPE;
			return(FALSE);
		}

		// Make sure this spell has not already been counted.
		memcpy(szSpell,m_pKnownSpells[i].chSpellName,8);
		strSpell = szSpell;
		strSpell.MakeUpper();
		if (slKnown[m_pKnownSpells[i].wSpellType].Find(strSpell))
			continue;
		
		slKnown[m_pKnownSpells[i].wSpellType].AddTail((const char *)strSpell);
		m_nSpellCount[m_pKnownSpells[i].wSpellType]++;
	}

	for (i=0;i<INF_CRE_SPELLTYPES;i++)
	{
		if (m_nSpellCount[i])
		{
			m_pSpellData[i] = new SPELLDATA[m_nSpellCount[i]];
			if (!m_pSpellData[i])
			{
				m_nError = ERR_CRE_OOM_SPELLDATA;
				return(FALSE);
			}
			memset(m_pSpellData[i],0,sizeof(SPELLDATA)*m_nSpellCount[i]);
		}
		else
			m_pSpellData[i] = NULL;
	}

	SPELLDATA *pSpellData;
	int nSpellIndex[INF_CRE_SPELLTYPES];
	memset(&nSpellIndex[0],0,sizeof(int)*INF_CRE_SPELLTYPES);
	memset(szSpell,0,10);

	// Fill up the internal structures with all the known spells.
	int j;
	for (i=0;i<(int)m_infCre.dwKnownSpellsCount;i++)
	{
		if (m_pSpellData[m_pKnownSpells[i].wSpellType])
		{
			// See if the spell is already in our list. This is another fix for the
			// multiple copies of the same known spell.
			for (j=0;j<nSpellIndex[m_pKnownSpells[i].wSpellType];j++)
				if (!strnicmp(m_pKnownSpells[i].chSpellName,(m_pSpellData[m_pKnownSpells[i].wSpellType]+j)->szResName,8))
					break;
			// If not at the end of the list, we found a dupliate.
			if (j<nSpellIndex[m_pKnownSpells[i].wSpellType])
				continue;

			pSpellData = m_pSpellData[m_pKnownSpells[i].wSpellType] + nSpellIndex[m_pKnownSpells[i].wSpellType];

			pSpellData->nTimesMemorized = 0;
			pSpellData->nTimesCanCast = 0;
			memcpy(szSpell,m_pKnownSpells[i].chSpellName,8);
			strcpy(pSpellData->szResName,szSpell);
			pSpellData->wLevel = m_pKnownSpells[i].wSpellLevel;
			pSpellData->wType = m_pKnownSpells[i].wSpellType;

			nSpellIndex[m_pKnownSpells[i].wSpellType]++;
		}
	}

	// If there were any duplicates in the number of known spells the count needs to
	// be changed. The editor will trim out those duplicates when it saves.
	m_infCre.dwKnownSpellsCount = 0;
	for (i=0;i<INF_CRE_SPELLTYPES;i++)
		m_infCre.dwKnownSpellsCount += m_nSpellCount[i];

	// Find all the memorized spells and count up the times memorized.
	int k;
	BOOL bFoundSpell;
	for (i=0;i<(int)m_infCre.dwMemorizedSpellsCount;i++)
	{
		// Find it in the new lists.
		bFoundSpell = FALSE;
		for (j=0;j<INF_CRE_SPELLTYPES;j++)
			for (k=0;k<m_nSpellCount[j];k++)
				if (m_pSpellData[j] && !strnicmp((m_pMemSpells+i)->chSpellName,(m_pSpellData[j]+k)->szResName,8))
				{
					(m_pSpellData[j]+k)->nTimesMemorized++;
					if (m_pMemSpells[i].dwMemorized)
						(m_pSpellData[j]+k)->nTimesCanCast++;
					bFoundSpell = TRUE;
				}

		// If we didn't find the spell in the list then the spell is a memorized
		// but known spell, which is how the sequencers seem to work. To keep it 
		// simple, since we are not editing this list, each of the spells has its
		// own entry in the list. If there are 2 of the same spell, there are two
		// entries in the list (the other spells have 1 entry with a count of the
		// number of times memorized).
		if (!bFoundSpell)
		{
			pSpellData = new SPELLDATA;
			memset(pSpellData,0,sizeof(SPELLDATA));
			memset(szSpell,0,10);

			// The data members are not important other than the spell name. I'm going
			// to assume that if it is mem'd but not known, that it is currently
			// castable. I'm staying with the SPELLDATA structure just for consistency
			// and to have it set up to track data for possible future changes (rather
			// than just keeping a string list of all the spell names).
			memcpy(szSpell,m_pMemSpells[i].chSpellName,8);
			strcpy(pSpellData->szResName,szSpell);

			m_plMemButNotKnown.AddTail(pSpellData);
		}
	}

	delete [] m_pKnownSpells;
	m_pKnownSpells = NULL;
	delete [] m_pMemSpells;
	m_pMemSpells = NULL;

	// This needs to be called to refresh all the counts and indices. The values
	// here will be wrong if the above found any duplicate known spells or mem'd
	// but not known spells.
	UpdateSpellInfo();

	// Item slots next.
	try { file.Seek(m_infCre.dwItemSlotsOffset+nStartPos,CFile::begin); }
	catch(CFileException *pe)
	{
		pe->Delete();
		m_nError = ERR_CRE_READBADITEMSLOTS;
		return(FALSE);
	}

	nCount = file.Read(&m_infItemSlots,sizeof(INF_CRE_ITEMSLOTS));
	if (nCount != sizeof(INF_CRE_ITEMSLOTS))
	{
		m_nError = ERR_CRE_READITEMSLOTS;
		return(FALSE);
	}

	// Item listing next.
	if (m_infCre.dwItemsCount)
	{
		try { file.Seek(m_infCre.dwItemsOffset+nStartPos,CFile::begin); }
		catch(CFileException *pe)
		{
			pe->Delete();
			m_nError = ERR_CRE_READBADITEMLIST;
			return(FALSE);
		}

		if (m_infCre.dwItemsCount > INF_NUM_ITEMSLOTS)
		{
			m_nError = ERR_CRE_READTOOMANYITEMS;
			return(FALSE);
		}

		for (i=0;i<(int)m_infCre.dwItemsCount;i++)
		{
			nCount = file.Read(&m_infItems[i],sizeof(INF_CRE_ITEM));
			if (nCount != sizeof(INF_CRE_ITEM))
			{
				m_nError = ERR_CRE_READITEM;
				return(FALSE);
			}
		}
	}

	if (m_infCre.dwAffectCount)
	{
		try { file.Seek(m_infCre.dwAffectOffset+nStartPos,CFile::begin); }
		catch(CFileException *pe)
		{
			pe->Delete();
			m_nError = ERR_CRE_READBADAFFECTS;
			return(FALSE);
		}

		INF_AFF *pAff;
		for (i=0;i<(int)m_infCre.dwAffectCount;i++)
		{
			pAff = new INF_AFF;
			if (!pAff)
			{
				m_nError = ERR_CRE_OOM_AFFECTS;
				return(FALSE);
			}

			nCount = file.Read(pAff,sizeof(INF_AFF));
			if (nCount != sizeof(INF_AFF))
			{
				m_nError = ERR_CRE_READAFFECTS;
				return(FALSE);
			}

			m_plAffects.AddTail(pAff);
		}
	}

	m_bHasChanged = FALSE;
	return(TRUE);
}

void CInfCreature::GetResLargePortrait(CString &strRes)
{
	char szTemp[9];

	memcpy(szTemp,m_infCre.chLargePortrait,8);
	szTemp[8] = '\0';

	strRes = szTemp;
}

UINT CInfCreature::GetFileSpace()
{
	// Header.
	UINT nSize = sizeof(INF_CRE);

	// Known spells.
	nSize += m_infCre.dwKnownSpellsCount * sizeof(INF_CRE_KNOWNSPELL);

	// Memorized spell info.
	nSize += m_infCre.dwSpellMemorizationInfoCount * sizeof(INF_CRE_MEMINFO);

	// Memorized spells. Note that this value should also represent the number
	// of spells in the m_plMemButNotKnown list.
	UpdateSpellInfo();
	nSize += m_infCre.dwMemorizedSpellsCount * sizeof(INF_CRE_MEMSPELL);

	// Item slot index.
	nSize += sizeof(INF_CRE_ITEMSLOTS);

	// Items.
	nSize += m_infCre.dwItemsCount * sizeof(INF_CRE_ITEM);

	// Affects.
	nSize += m_infCre.dwAffectCount * sizeof(INF_AFF);

	return(nSize);
}

BOOL CInfCreature::Write(CFile &file)
{
	// If there are no entries of a specific type, the offset for that particular
	// area is set to zero. This is unlike the GAME file which stores the offsets
	// of where that datablock would be stored, although no data is written.
	UpdateSpellInfo();

	UINT nOffset = sizeof(INF_CRE);
	if (m_infCre.dwKnownSpellsCount)
		m_infCre.dwKnownSpellsOffset = nOffset;
	else
		m_infCre.dwKnownSpellsOffset = 0;
	nOffset += m_infCre.dwKnownSpellsCount * sizeof(INF_CRE_KNOWNSPELL);
	if (m_infCre.dwSpellMemorizationInfoCount)
		m_infCre.dwSpellMemorizationInfoOffset = nOffset;
	else
		m_infCre.dwSpellMemorizationInfoOffset = 0;
	nOffset += m_infCre.dwSpellMemorizationInfoCount * sizeof(INF_CRE_MEMINFO);
	if (m_infCre.dwMemorizedSpellsCount)
		m_infCre.dwMemorizedSpellsOffset = nOffset;
	else
		m_infCre.dwMemorizedSpellsOffset = 0;
	nOffset += m_infCre.dwMemorizedSpellsCount * sizeof(INF_CRE_MEMSPELL);
	m_infCre.dwItemSlotsOffset = nOffset;
	nOffset += sizeof(INF_CRE_ITEMSLOTS);
	if (m_infCre.dwItemsCount)
		m_infCre.dwItemsOffset = nOffset;
	else
		m_infCre.dwItemsOffset = 0;
	nOffset += m_infCre.dwItemsCount * sizeof(INF_CRE_ITEM);
	if (m_infCre.dwAffectCount)
		m_infCre.dwAffectOffset = nOffset;
	else
		m_infCre.dwAffectOffset = 0;

	// You apparently can't resurrect a player that has hit points. Even if they
	// are dead. So if any of the death flags are set, I'm putting their hp to 0.
	// This should fix any games where SK restored their hp and they were dead. I
	// don't know for sure if CRE_STAT_DEAD is always set when you die and the others
	// are just to tell what type of death it was, or if if it's possible to just
	// have the dead-specific flags set. So I'm just checking them all to be safe.
	if (m_infCre.dwStateFlags & CRE_STAT_DEAD
			|| m_infCre.dwStateFlags & CRE_STAT_ACID_DEAD
			|| m_infCre.dwStateFlags & CRE_STAT_FLAME_DEAD
			|| m_infCre.dwStateFlags & CRE_STAT_EXPLODE_DEAD
			|| m_infCre.dwStateFlags & CRE_STAT_STONE_DEAD
			|| m_infCre.dwStateFlags & CRE_STAT_FROZEN_DEAD)
	{
		m_infCre.wCurrentHP = 0;
	}

	try { file.Write(&m_infCre,sizeof(INF_CRE)); }
	catch(CFileException *pe)
	{
		m_nError = ERR_CRE_WRITEHEADER;
		pe->Delete();
		return(FALSE);
	}

	int i,j;
	char szSpell[8];
	int nWriteCount = 0;
	INF_CRE_KNOWNSPELL infKnown;
	for (i=0;i<INF_CRE_SPELLTYPES;i++)
		for (j=0;j<m_nSpellCount[i];j++)
		{
			memset(szSpell,0,8);
			memcpy(infKnown.chSpellName,(m_pSpellData[i]+j)->szResName,8);
			infKnown.wSpellLevel = (m_pSpellData[i]+j)->wLevel;
			infKnown.wSpellType = (m_pSpellData[i]+j)->wType;

			try { file.Write(&infKnown,sizeof(INF_CRE_KNOWNSPELL)); }
			catch(CFileException *pe)
			{
				m_nError = ERR_CRE_WRITEKNOWNSPELLS;
				pe->Delete();
				return(FALSE);
			}
			nWriteCount++;
		}

	try { file.Write(m_pMemInfo,m_infCre.dwSpellMemorizationInfoCount*sizeof(INF_CRE_MEMINFO)); }
	catch(CFileException *pe)
	{
		m_nError = ERR_CRE_WRITEMEMINFO;
		pe->Delete();
		return(FALSE);
	}

	int k;
	INF_CRE_MEMSPELL infMem;
	nWriteCount = 0;
	for (i=0;i<INF_CRE_SPELLTYPES;i++)
		for (j=0;j<m_nSpellCount[i];j++)
			if ((m_pSpellData[i]+j)->nTimesMemorized)
			{
				memset(szSpell,0,8);
				memcpy(infMem.chSpellName,(m_pSpellData[i]+j)->szResName,8);

				for (k=0;k<(m_pSpellData[i]+j)->nTimesMemorized;k++)
				{
					if (_bMemSpellsOnSave || k < (m_pSpellData[i]+j)->nTimesCanCast)
						infMem.dwMemorized = 1;
					else
						infMem.dwMemorized = 0;

					try { file.Write(&infMem,sizeof(INF_CRE_MEMSPELL)); }
					catch(CFileException *pe)
					{
						m_nError = ERR_CRE_WRITEMEMSPELLS;
						pe->Delete();
						return(FALSE);
					}
					nWriteCount++;
				}
			}

	SPELLDATA *pSpellData;
	POSITION pos = m_plMemButNotKnown.GetHeadPosition();
	while(pos)
	{
		pSpellData = (SPELLDATA*)m_plMemButNotKnown.GetNext(pos);

		memset(szSpell,0,8);
		memcpy(infMem.chSpellName,pSpellData->szResName,8);
		infMem.dwMemorized = 1;

		try { file.Write(&infMem,sizeof(INF_CRE_MEMSPELL)); }
		catch(CFileException *pe)
		{
			m_nError = ERR_CRE_WRITENOTKNOWNSPELL;
			pe->Delete();
			return(FALSE);
		}

	}

	try { file.Write(&m_infItemSlots,sizeof(INF_CRE_ITEMSLOTS)); }
	catch(CFileException *pe)
	{
		m_nError = ERR_CRE_WRITEITEMSLOTS;
		pe->Delete();
		return(FALSE);
	}

	try { file.Write(&m_infItems[0],m_infCre.dwItemsCount*sizeof(INF_CRE_ITEM)); }
	catch(CFileException *pe)
	{
		m_nError = ERR_CRE_WRITEITEMS;
		pe->Delete();
		return(FALSE);
	}

	if (m_infCre.dwAffectCount)
	{
		if (m_infCre.dwAffectCount != (DWORD)m_plAffects.GetCount())
		{
			m_nError = ERR_CRE_BADAFFECTCOUNT;
			return(FALSE);
		}

		INF_AFF *pAff;
		POSITION pos = m_plAffects.GetHeadPosition();
		while(pos)
		{
			pAff = (INF_AFF*)m_plAffects.GetNext(pos);
			try { file.Write(pAff,sizeof(INF_AFF)); }
			catch(CFileException *pe)
			{
				m_nError = ERR_CRE_WRITEAFFECTS;
				pe->Delete();
				return(FALSE);
			}
		}
	}

	m_bHasChanged = FALSE;
	return(TRUE);
}

void CInfCreature::GetName(CString &strName)
{
	strName.Empty();

	// First try to get the name by string refference.
	_infTlk.GetString(m_infCre.dwLongCreatureName,strName);
	if (strName.IsEmpty())
		_infTlk.GetString(m_infCre.dwShortCreatureName,strName);

	// If we still don't have it, pull the literal out.
	if (strName.IsEmpty())
	{
		char szBuf[21];
		memcpy(szBuf,m_infCre.chName,20);
		szBuf[20] = '\x0';
		strName = szBuf;
	}
}

void CInfCreature::SetName(const char *pszName)
{
	char szName[21];

	memset(szName,0,21);
	if (strlen(pszName) > 20)
		memcpy(szName,pszName,20);
	else
		strcpy(szName,pszName);

	if (memcmp(szName,m_infCre.chName,20))
		m_bHasChanged = TRUE;
	memcpy(m_infCre.chName,szName,20);
}

void CInfCreature::SetSkinColor(BYTE chColor)
{
	if (m_infCre.chSkinColor != chColor)
		m_bHasChanged = TRUE;
	m_infCre.chSkinColor = chColor;
}

void CInfCreature::SetHairColor(BYTE chColor)
{
	if (m_infCre.chHairColor != chColor)
		m_bHasChanged = TRUE;
	m_infCre.chHairColor = chColor;
}

void CInfCreature::SetMajorColor(BYTE chColor)
{
	if (m_infCre.chMajorColor != chColor)
		m_bHasChanged = TRUE;
	m_infCre.chMajorColor = chColor;
}

void CInfCreature::SetMinorColor(BYTE chColor)
{
	if (m_infCre.chMinorColor != chColor)
		m_bHasChanged = TRUE;
	m_infCre.chMinorColor = chColor;
}

void CInfCreature::SetArmorColor(BYTE chColor)
{
	if (m_infCre.chArmorColor != chColor)
		m_bHasChanged = TRUE;
	m_infCre.chArmorColor = chColor;
}

void CInfCreature::SetMetalColor(BYTE chColor)
{
	if (m_infCre.chMetalColor != chColor)
		m_bHasChanged = TRUE;
	m_infCre.chMetalColor = chColor;
}
 
void CInfCreature::SetLeatherColor(BYTE chColor)
{
	if (m_infCre.chLeatherColor != chColor)
		m_bHasChanged = TRUE;
	m_infCre.chLeatherColor = chColor;
}

void CInfCreature::SetShortNameID(DWORD dwValue)
{
	if (m_infCre.dwShortCreatureName != dwValue)
		m_bHasChanged = TRUE;
	m_infCre.dwShortCreatureName = dwValue;
}

void CInfCreature::SetLongNameID(DWORD dwValue)
{
	if (m_infCre.dwLongCreatureName != dwValue)
		m_bHasChanged = TRUE;
	m_infCre.dwLongCreatureName = dwValue;
}

void CInfCreature::SetDualClass(int nValue)
{
	nValue = VALIDATE_RANGE(nValue,0,255);
	if (m_infCre.chDualClass != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chDualClass = nValue;
}

void CInfCreature::SetFirstClassLevel(int nValue)
{
	nValue = VALIDATE_RANGE(nValue,0,255);
	if (m_infCre.chLevelFirstClass != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chLevelFirstClass = nValue;
}

void CInfCreature::SetSecondClassLevel(int nValue)
{
	nValue = VALIDATE_RANGE(nValue,0,255);
	if (m_infCre.chLevelSecondClass != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chLevelSecondClass = nValue;
}

void CInfCreature::SetThirdClassLevel(int nValue)
{
	nValue = VALIDATE_RANGE(nValue,0,255);
	if (m_infCre.chLevelThirdClass != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chLevelThirdClass = nValue;
}

void CInfCreature::SetStr(int nStr)
{
	nStr = VALIDATE_ATTR(nStr);
	if (m_infCre.chStrength != nStr)
		m_bHasChanged = TRUE;
	m_infCre.chStrength = nStr;
}

void CInfCreature::SetStrBonus(int nBonus)
{
	nBonus = VALIDATE_RANGE(nBonus,0,100);
	if (m_infCre.chStrengthBonus != nBonus)
		m_bHasChanged = TRUE;
	m_infCre.chStrengthBonus = nBonus;
}

void CInfCreature::SetDex(int nDex)
{
	nDex = VALIDATE_ATTR(nDex);
	if (m_infCre.chDexterity != nDex)
		m_bHasChanged = TRUE;
	m_infCre.chDexterity = nDex;
}

void CInfCreature::SetCon(int nCon)
{
	nCon = VALIDATE_ATTR(nCon);
	if (m_infCre.chConstitution != nCon)
		m_bHasChanged = TRUE;
	m_infCre.chConstitution = nCon;
}

void CInfCreature::SetInt(int nInt)
{
	nInt = VALIDATE_ATTR(nInt);
	if (m_infCre.chIntelligence != nInt)
		m_bHasChanged = TRUE;
	m_infCre.chIntelligence = nInt;
}

void CInfCreature::SetWis(int nWis)
{
	nWis = VALIDATE_ATTR(nWis);
	if (m_infCre.chWisdom != nWis)
		m_bHasChanged = TRUE;
	m_infCre.chWisdom = nWis;
}

void CInfCreature::SetCha(int nCha)
{
	nCha = VALIDATE_ATTR(nCha);
	if (m_infCre.chCharisma != nCha)
		m_bHasChanged = TRUE;
	m_infCre.chCharisma = nCha;
}

void CInfCreature::SetExp(DWORD dwExp)
{
	if (m_infCre.dwExp != dwExp)
		m_bHasChanged = TRUE;
	m_infCre.dwExp = dwExp;
}

void CInfCreature::SetExpForKill(DWORD dwExpForKill)
{
	if (m_infCre.dwExpForKilling != dwExpForKill)
		m_bHasChanged = TRUE;
	m_infCre.dwExpForKilling = dwExpForKill;
}

void CInfCreature::SetGold(DWORD dwGold)
{
	if (m_infCre.dwGold != dwGold)
		m_bHasChanged = TRUE;
	m_infCre.dwGold = dwGold;
}

void CInfCreature::SetBaseHP(WORD wBaseHP)
{
	if (m_infCre.wBaseHP != wBaseHP)
		m_bHasChanged = TRUE;
	m_infCre.wBaseHP = wBaseHP;
}

void CInfCreature::SetCurrentHP(WORD wCurrentHP)
{
	if (m_infCre.wCurrentHP != wCurrentHP)
		m_bHasChanged = TRUE;
	m_infCre.wCurrentHP = wCurrentHP;
}

void CInfCreature::SetAttacks(int nValue)
{
	if (GetAttacks() != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chAttacks = nValue;
}

void CInfCreature::SetAC(int nValue)
{
	if (GetAC() != nValue)
		m_bHasChanged = TRUE;
	m_infCre.nAC1 = nValue;
	m_infCre.nAC2 = nValue;
}

void CInfCreature::SetThac0(int nValue)
{
	nValue = VALIDATE_RANGE(nValue,-100,100);
	if (GetThac0() != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chTHAC0 = nValue;
}

void CInfCreature::SetSaveBreath(int nValue)
{
	nValue = VALIDATE20(nValue);
	if (m_infCre.chSaveBreath != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chSaveBreath = nValue;
}

void CInfCreature::SetSaveDeath(int nValue)
{
	nValue = VALIDATE20(nValue);
	if (m_infCre.chSaveDeath != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chSaveDeath = nValue;
}

void CInfCreature::SetSaveWands(int nValue)
{
	nValue = VALIDATE20(nValue);
	if (m_infCre.chSaveWands != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chSaveWands = nValue;
}

void CInfCreature::SetSavePoly(int nValue)
{
	nValue = VALIDATE20(nValue);
	if (m_infCre.chSavePoly != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chSavePoly = nValue;
}

void CInfCreature::SetSaveSpells(int nValue)
{
	nValue = VALIDATE20(nValue);
	if (m_infCre.chSaveSpells != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chSaveSpells = nValue;
}

////////////////////////////////////////
// 1st Class Proficiencies.

void CInfCreature::SetProfLargeSwords(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfLargeSwords));
	if (m_infCre.chProfLargeSwords != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfLargeSwords = nValue;
}

void CInfCreature::SetProfSmallSwords(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfSmallSwords));
	if (m_infCre.chProfSmallSwords != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfSmallSwords = nValue;
}

void CInfCreature::SetProfBows(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfBows));
	if (m_infCre.chProfBows != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfBows = nValue;
}

void CInfCreature::SetProfSpears(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfSpears));
	if (m_infCre.chProfSpears != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfSpears = nValue;
}

void CInfCreature::SetProfClubs(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfClubs));
	if (m_infCre.chProfClubs != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfClubs = nValue;
}

void CInfCreature::SetProfFlails(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfFlails));
	if (m_infCre.chProfFlails != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfFlails = nValue;
}

void CInfCreature::SetProfAxes(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfAxes));
	if (m_infCre.chProfAxes != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfAxes = nValue;
}

void CInfCreature::SetProfMissiles(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfMissiles));
	if (m_infCre.chProfMissiles != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfMissiles = nValue;
}

void CInfCreature::SetProfDaggers(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfDaggers));
	if (m_infCre.chProfDaggers != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfDaggers = nValue;
}

void CInfCreature::SetProfGreatSwords(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfGreatSwords));
	if (m_infCre.chProfGreatSwords != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfGreatSwords = nValue;
}

void CInfCreature::SetProfHalberds(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfHalberds));
	if (m_infCre.chProfHalberds != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfHalberds = nValue;
}

void CInfCreature::SetProfMaces(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfMaces));
	if (m_infCre.chProfMaces != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfMaces = nValue;
}

void CInfCreature::SetProfCrossbows(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfCrossbows));
	if (m_infCre.chProfCrossbows != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfCrossbows = nValue;
}

void CInfCreature::SetProfQuarterStaffs(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfQuarterstaffs));
	if (m_infCre.chProfQuarterstaffs != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfQuarterstaffs = nValue;
}

void CInfCreature::SetProfHammers(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(nValue,HITRIBBLE(m_infCre.chProfHammers));
	if (m_infCre.chProfHammers != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfHammers = nValue;
}

////////////////////////////////////////
// 2nd Class Proficiencies.

void CInfCreature::SetProfLargeSwords2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfLargeSwords),nValue);
	if (m_infCre.chProfLargeSwords != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfLargeSwords = nValue;
}

void CInfCreature::SetProfSmallSwords2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfSmallSwords),nValue);
	if (m_infCre.chProfSmallSwords != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfSmallSwords = nValue;
}

void CInfCreature::SetProfBows2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfBows),nValue);
	if (m_infCre.chProfBows != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfBows = nValue;
}

void CInfCreature::SetProfSpears2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfSpears),nValue);
	if (m_infCre.chProfSpears != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfSpears = nValue;
}

void CInfCreature::SetProfClubs2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfClubs),nValue);
	if (m_infCre.chProfClubs != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfClubs = nValue;
}

void CInfCreature::SetProfFlails2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfFlails),nValue);
	if (m_infCre.chProfFlails != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfFlails = nValue;
}

void CInfCreature::SetProfAxes2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfAxes),nValue);
	if (m_infCre.chProfAxes != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfAxes = nValue;
}

void CInfCreature::SetProfMissiles2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfMissiles),nValue);
	if (m_infCre.chProfMissiles != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfMissiles = nValue;
}

void CInfCreature::SetProfDaggers2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfDaggers),nValue);
	if (m_infCre.chProfDaggers != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfDaggers = nValue;
}

void CInfCreature::SetProfGreatSwords2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfGreatSwords),nValue);
	if (m_infCre.chProfGreatSwords != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfGreatSwords = nValue;
}

void CInfCreature::SetProfHalberds2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfHalberds),nValue);
	if (m_infCre.chProfHalberds != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfHalberds = nValue;
}

void CInfCreature::SetProfMaces2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfMaces),nValue);
	if (m_infCre.chProfMaces != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfMaces = nValue;
}

void CInfCreature::SetProfCrossbows2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfCrossbows),nValue);
	if (m_infCre.chProfCrossbows != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfCrossbows = nValue;
}

void CInfCreature::SetProfQuarterStaffs2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfQuarterstaffs),nValue);
	if (m_infCre.chProfQuarterstaffs != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfQuarterstaffs = nValue;
}

void CInfCreature::SetProfHammers2(int nValue)
{
	nValue = VALIDATE_PROF(nValue);
	nValue = MAKETRIBBLE(LOTRIBBLE(m_infCre.chProfHammers),nValue);
	if (m_infCre.chProfHammers != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chProfHammers = nValue;
}

void CInfCreature::SetResFire(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistFire != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistFire = nValue;
}

void CInfCreature::SetResCold(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistCold != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistCold = nValue;
}

void CInfCreature::SetResElectricity(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistElectricity != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistElectricity = nValue;
}

void CInfCreature::SetResAcid(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistAcid != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistAcid = nValue;
}

void CInfCreature::SetResMagic(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistMagic != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistMagic = nValue;
}

void CInfCreature::SetResMagicFire(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistMagicFire != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistMagicFire = nValue;
}

void CInfCreature::SetResMagicCold(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistMagicCold != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistMagicCold = nValue;
}

void CInfCreature::SetResSlashing(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistSlashing != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistSlashing = nValue;
}

void CInfCreature::SetResCrushing(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistCrushing != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistCrushing = nValue;
}

void CInfCreature::SetResPiercing(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistPiercing != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistPiercing = nValue;
}

void CInfCreature::SetResMissile(int nValue)
{
	nValue = VALIDATE_RES(nValue);
	if (m_infCre.chResistMissile != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chResistMissile = nValue;
}

void CInfCreature::SetLore(int nValue)
{
	nValue = VALIDATE_RANGE(nValue,0,255);
	if (m_infCre.chLore != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chLore = nValue;
}

void CInfCreature::SetReputation(int nValue)
{
	nValue = VALIDATE_RANGE(nValue,0,20);
	nValue *= 10;
	if (m_infCre.chReputation != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chReputation = nValue;
}

void CInfCreature::SetOpenLocks(int nValue)
{
	nValue = VALIDATE_THIEF(nValue);
	if (m_infCre.chOpenLocks != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chOpenLocks = nValue;
}

void CInfCreature::SetMoveSilently(int nValue)
{
	nValue = VALIDATE_THIEF(nValue);
	if (m_infCre.chMoveSilently != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chMoveSilently = nValue;
}

void CInfCreature::SetFindTraps(int nValue)
{
	nValue = VALIDATE_THIEF(nValue);
	if (m_infCre.chFindTraps != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chFindTraps = nValue;
}

void CInfCreature::SetPickPockets(int nValue)
{
	nValue = VALIDATE_THIEF(nValue);
	if (m_infCre.chPickPockets != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chPickPockets = nValue;
}

void CInfCreature::SetHideInShadows(int nValue)
{
	nValue = VALIDATE_THIEF(nValue);
	if (m_infCre.chHideInShadows != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chHideInShadows = nValue;
}

void CInfCreature::SetDetectIllusions(int nValue)
{
	nValue = VALIDATE_THIEF(nValue);
	if (m_infCre.chDetectIllusions != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chDetectIllusions = nValue;
}

void CInfCreature::SetSetTraps(int nValue)
{
	nValue = VALIDATE_THIEF(nValue);
	if (m_infCre.chSetTraps != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chSetTraps = nValue;
}

void CInfCreature::SetRace(int nValue)
{
	if (m_infCre.chRace != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chRace = nValue;
}

void CInfCreature::SetGender(int nValue)
{
	if (m_infCre.chGender != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chGender = nValue;
}

void CInfCreature::SetAlignment(int nValue)
{
	if (m_infCre.chAlignment != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chAlignment = nValue;
}

void CInfCreature::SetClass(int nValue)
{
	if (m_infCre.chClass != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chClass = nValue;
}

void CInfCreature::SetKit(DWORD dwValue)
{
	if (m_infCre.dwKit != dwValue)
		m_bHasChanged = TRUE;
	m_infCre.dwKit = dwValue;
}

void CInfCreature::SetRacialEnemy(int nValue)
{
	if (m_infCre.chRacialEnemy != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chRacialEnemy = nValue;
}

void CInfCreature::SetEnemyAlly(int nValue)
{
	if (m_infCre.chEnemyAlly != nValue)
		m_bHasChanged = TRUE;
	m_infCre.chEnemyAlly = nValue;
}

void CInfCreature::SetAnimationID(WORD wValue)
{
	if (m_infCre.wAnimationID != wValue)
		m_bHasChanged = TRUE;
	m_infCre.wAnimationID = wValue;
}

void CInfCreature::SetStateFlags(DWORD dwValue)
{
	if (m_infCre.dwStateFlags != dwValue)
		m_bHasChanged = TRUE;
	m_infCre.dwStateFlags = dwValue;
}

void CInfCreature::GetItems(INF_CRE_ITEM *pItems)
{
	memset(pItems,0,sizeof(INF_CRE_ITEM)*INF_NUM_ITEMSLOTS);

	for (int i=0;i<INF_NUM_ITEMSLOTS;i++)
		if (m_infItemSlots.wItemSlot[i] < INF_NUM_ITEMSLOTS)
			memcpy(pItems+i,&m_infItems[m_infItemSlots.wItemSlot[i]],sizeof(INF_CRE_ITEM));
}

void CInfCreature::SetItems(const INF_CRE_ITEM *pItems)
{
	// Going to first recreate the data locally so I can compare it to the existing
	// data to see if it has changed.
	INF_CRE_ITEMSLOTS infItemSlots;
	INF_CRE_ITEM		infItems[INF_NUM_ITEMSLOTS];

	// Unused slots are marked with 0xFFFF. I don't know what the last 4 bytes are
	// used for so I'm just keeping them as whatever I read in.
	memset(&infItemSlots,0xFF,sizeof(WORD)*INF_NUM_ITEMSLOTS);
	memcpy(infItemSlots.chUnknown,m_infItemSlots.chUnknown,4);

	int nIndex = 0;
	for (int i=0;i<INF_NUM_ITEMSLOTS;i++)
		if (pItems[i].chResName[0])
		{
			infItemSlots.wItemSlot[i] = nIndex;
			memcpy(&infItems[nIndex],&pItems[i],sizeof(INF_CRE_ITEM));
			nIndex++;
		}

	// My local buffers should now be in the state the CRE expects.
	if (memcmp(&infItemSlots,&m_infItemSlots,sizeof(INF_CRE_ITEMSLOTS)) || 
		 memcmp(&infItems[0],&m_infItems[0],sizeof(INF_CRE_ITEM)*nIndex))
	{
		// Something has changed, mark the CRE as changed, copy the new data
		// and update the header wit the new count.
		m_bHasChanged = TRUE;

		memcpy(&m_infItemSlots,&infItemSlots,sizeof(INF_CRE_ITEMSLOTS));

		memcpy(&m_infItems[0],&infItems[0],sizeof(INF_CRE_ITEM)*nIndex);
		m_infCre.dwItemsCount = nIndex;

		// It's not necessary to update the offsets here. Everything is already
		// pulled apart into various areas. The offsets will be recalculated when
		// the object is saved.
	}
}

void CInfCreature::GetSpells(int nType, SPELLDATA *pData)
{
	if (nType < 0 || nType >= INF_CRE_SPELLTYPES)
		return;

	memcpy(pData,m_pSpellData[nType],sizeof(SPELLDATA)*m_nSpellCount[nType]);
}

void CInfCreature::UpdateSpellInfo()
{
	int i,j,k;
	int nKnown = 0;
	int nMem = 0;

	// Clear out all the values to recalc in the memorization info data.
	for (i=0;i<(int)m_infCre.dwSpellMemorizationInfoCount;i++)
	{
		m_pMemInfo[i].dwSpellCount = 0;
		m_pMemInfo[i].dwSpellIndex = 0;
	}

	POSITION pos;
	for (i=0;i<INF_CRE_SPELLTYPES;i++)
	{
		for (j=0;j<m_nSpellCount[i];j++)
		{
			// If it's in the list, it's known.
			nKnown++;
			if ((m_pSpellData[i]+j)->nTimesMemorized)
			{
				// It will appear in the memorized list one time for each time memorized.
				nMem += (m_pSpellData[i]+j)->nTimesMemorized;

				// Find the matching spell info line and update the memorized spell counts. There is only
				// one entry in the memorization block for innate spells. I'm ignoring the level marked
				// on the innate for this purpose.
				for (k=0;k<(int)m_infCre.dwSpellMemorizationInfoCount;k++)
					if ((m_pSpellData[i]+j)->wType == m_pMemInfo[k].wSpellType && ((m_pSpellData[i]+j)->wType == INF_CRE_ST_INNATE || (m_pSpellData[i]+j)->wLevel == m_pMemInfo[k].wSpellLevel)) 
					{
						m_pMemInfo[k].dwSpellCount += (m_pSpellData[i]+j)->nTimesMemorized;

						// Sorcerers have a weird spell table. For each spell they know, it appears in the 
						// memorized list as many times as they are allowed to cast a spell of that level.
						// So if a sorcerer can cast 6 level 1 spells, every level 1 spell they know will 
						// appear in the memorized list 6 times.

						// I took the below back out after getting a few complaints about it raising
						// the spells memorizable above what is normal. The problem is that the editor
						// counts up how many spells you have set to be memorized, but doesn't take into
						// account any stat bonuses you might get that would raise the number of spells
						// you can memorize, or any equipment that does the same thing. It's easier to
						// just remove it.
/*						if (m_pMemInfo[k].dwSpellCount > m_pMemInfo[k].wNumMemorizable1)
						{
							if (m_infCre.chClass != CLASS_SORCERER || m_pMemInfo[k].wSpellType == INF_CRE_ST_INNATE)
							{
								TRACE("Increasing spell memorization limit - Name: %s, Type: %d, Level: %d, Original Count: %d, New Count: %d\n",m_infCre.chName,m_pMemInfo[k].wSpellType,m_pMemInfo[k].wSpellLevel,m_pMemInfo[k].wNumMemorizable1,m_pMemInfo[k].dwSpellCount);
								m_pMemInfo[k].wNumMemorizable1 = (WORD)m_pMemInfo[k].dwSpellCount;
								m_pMemInfo[k].wNumMemorizable2 = (WORD)m_pMemInfo[k].dwSpellCount;
							}
						} */
					}
			}
		}

		// Make sure to count any of the memorized but not known spells as an innate.
		if (i == INF_CRE_ST_INNATE)
		{
			for (k=0;k<(int)m_infCre.dwSpellMemorizationInfoCount;k++)
				if (m_pMemInfo[k].wSpellType == INF_CRE_ST_INNATE)
				{
					pos = m_plMemButNotKnown.GetHeadPosition();
					while(pos)
					{
						m_plMemButNotKnown.GetNext(pos);

						// Don't care about anything except that we have one in the list.
						m_pMemInfo[k].dwSpellCount++;

						if (m_pMemInfo[k].dwSpellCount > m_pMemInfo[k].wNumMemorizable1)
						{
							TRACE("Increasing spell memorization limit - Name: %s, Type: %d, Level: %d, Original Count: %d, New Count: %d\n",m_infCre.chName,m_pMemInfo[k].wSpellType,m_pMemInfo[k].wSpellLevel,m_pMemInfo[k].wNumMemorizable1,m_pMemInfo[k].dwSpellCount);
							m_pMemInfo[k].wNumMemorizable1 = (WORD)m_pMemInfo[k].dwSpellCount;
							m_pMemInfo[k].wNumMemorizable2 = (WORD)m_pMemInfo[k].dwSpellCount;
						}
					}
				}
		}
	}

	m_infCre.dwKnownSpellsCount = nKnown;
	m_infCre.dwMemorizedSpellsCount = nMem + m_plMemButNotKnown.GetCount();

	// Need to now calculate the spell indices for the memorization info.
	int nIndex = 0;
	for (i=0;i<(int)m_infCre.dwSpellMemorizationInfoCount;i++)
	{
		m_pMemInfo[i].dwSpellIndex = nIndex;
		nIndex += m_pMemInfo[i].dwSpellCount;
	}
}

void CInfCreature::SetSpells(int nType, int nSpellCount, const SPELLDATA *pData)
{
	if (nType < 0 || nType >= INF_CRE_SPELLTYPES)
		return;

	// Get rid of the old data.
	delete [] m_pSpellData[nType];

	if (nSpellCount)
	{
		m_pSpellData[nType] = new SPELLDATA[nSpellCount];
		memcpy(m_pSpellData[nType],pData,sizeof(SPELLDATA)*nSpellCount);
	}
	else
		m_pSpellData[nType] = NULL;
	m_nSpellCount[nType] = nSpellCount;

	UpdateSpellInfo();

	// It would be a real pain to figure out if the spell information has 
	// changed. So for now I'm just marking the record as changed. This means
	// that even scrolling through different characters in a party will cause
	// the record to changed and it will always ask if you want to save your
	// changes. Future todo....
	m_bHasChanged = TRUE;
}

void CInfCreature::GetMemorizationInfo(MEMINFO *pMemInfo)
{
	for (int i=0;i<(int)m_infCre.dwSpellMemorizationInfoCount;i++)
	{
		pMemInfo[i].wLevel = m_pMemInfo[i].wSpellLevel;
		pMemInfo[i].wType = m_pMemInfo[i].wSpellType;
		pMemInfo[i].wNumMemorizable = m_pMemInfo[i].wNumMemorizable1;
	}
}

void CInfCreature::SetMemorizationInfo(const MEMINFO *pMemInfo)
{
	// I'm searching the internal list for matching values rather than just looping through
	// and assigning the data (like in the GetMem...) to keep the data in the preferred oreder.
	// This way even if externally the list is passed back in a screwed up order, it will
	// still end up internally the way the Write code expects it.

	int i,j;
	for (i=0;i<(int)m_infCre.dwSpellMemorizationInfoCount;i++)
		for (j=0;j<(int)m_infCre.dwSpellMemorizationInfoCount;j++)
			if (m_pMemInfo[j].wSpellLevel == pMemInfo[i].wLevel && m_pMemInfo[j].wSpellType == m_pMemInfo[i].wSpellType)
			{
				m_pMemInfo[j].wNumMemorizable1 = pMemInfo[i].wNumMemorizable;
				m_pMemInfo[j].wNumMemorizable2 = pMemInfo[i].wNumMemorizable;
			}

	// This corrects for any spells that may have been set as more memorized than allowed.
	UpdateSpellInfo();
}

void CInfCreature::GetProfs(CObList &list)
{
	if (m_plAffects.GetCount() < 1)
		return;

	INF_AFF *pAff;
	POSITION pos = m_plAffects.GetHeadPosition();
	while(pos)
	{
		pAff = (INF_AFF*)m_plAffects.GetNext(pos);

		if (pAff->dwAffectType == AFF_TYPE_PROF)
		{
			CProfData *pProf = new CProfData;

			pProf->m_chProf = (BYTE)pAff->nNP2;

			if (m_infCre.chDualClass)
			{
				pProf->m_nFirstClass = HITRIBBLE(pAff->nNP1);
				pProf->m_nSecondClass = LOTRIBBLE(pAff->nNP1);
			}
			else
				pProf->m_nFirstClass = pAff->nNP1;

			list.AddTail(pProf);
		}
	}
	TRACE("CInfCreautre::GetProfs() - %d profs found.\n",list.GetCount());
}

void CInfCreature::SetProfs(CObList &list)
{
	INF_AFF *pAff;

	// Start by deleting all existing profs.
	POSITION posPrev;
	POSITION pos = m_plAffects.GetHeadPosition();
	while(pos)
	{
		posPrev = pos;
		pAff = (INF_AFF*)m_plAffects.GetNext(pos);

		if (pAff->dwAffectType == AFF_TYPE_PROF)
		{
			m_infCre.dwAffectCount--;
			if (m_infCre.dwAffectCount < 0)
				m_infCre.dwAffectCount = 0;

			m_plAffects.RemoveAt(posPrev);
			delete pAff;

			// Since I really don't know how the list is ordered I'm starting the
			// loop over each time I delete one.
			pos = m_plAffects.GetHeadPosition();
		}
	}

	if (list.GetCount() < 1)
		return;

	// Add an affect structure back for each proficiency.
	CProfData *pData;
	pos = list.GetHeadPosition();
	while(pos)
	{
		pData = (CProfData*)list.GetNext(pos);

		pAff = new INF_AFF;
		memset(pAff,0,sizeof(INF_AFF));

		pAff->dwAffectType = AFF_TYPE_PROF;
		pAff->nNP2 = pData->m_chProf;

		if (m_infCre.chDualClass)
			pAff->nNP1 = MAKETRIBBLE(pData->m_nSecondClass,pData->m_nFirstClass);
		else
			pAff->nNP1 = pData->m_nFirstClass;

		pAff->dwFlags = 0x09;
		pAff->wProbability1 = 0x64;
		pAff->dwUnknown3[0] = 0xFFFFFFFF;
		pAff->dwUnknown3[1] = 0xFFFFFFFF;
		pAff->dwUnknown3[2] = 0xFFFFFFFF;
		pAff->dwUnknown3[3] = 0xFFFFFFFF;
		pAff->dwUnknown4[2] = 0xFFFFFFFF;
		pAff->dwUnknown5[1] = 0x01;

		m_plAffects.AddTail(pAff);
		m_infCre.dwAffectCount++;

		delete pData;
	}

	list.RemoveAll();
}

DWORD CInfCreature::GetSpeed()
{
	INF_AFF *pAff;
	POSITION pos = m_plAffects.GetHeadPosition();
	while(pos)
	{
		pAff = (INF_AFF*)m_plAffects.GetNext(pos);

		if (pAff->dwAffectType == AFF_TYPE_SPELL && !strnicmp(pAff->chResRef3,"SPCL812",7))
			return(pAff->nNP1);
	}

	return(0);
}

void CInfCreature::SetSpeed(DWORD dwSpeed)
{
	INF_AFF *pAff;

	// First see if it is already in the list.
	POSITION posPrev;
	POSITION pos = m_plAffects.GetHeadPosition();
	while(pos)
	{
		posPrev = pos;
		pAff = (INF_AFF*)m_plAffects.GetNext(pos);

		if (pAff->dwAffectType == AFF_TYPE_SPELL && !strnicmp(pAff->chResRef3,"SPCL812",7))
		{
			if (dwSpeed)
				pAff->nNP1 = dwSpeed;
			else
			{
				// Remove it if setting to 0.
				m_plAffects.RemoveAt(posPrev);
				delete pAff;
				m_infCre.dwAffectCount--;
				if (m_infCre.dwAffectCount < 0)
					m_infCre.dwAffectCount = 0;
			}
			return;
		}
	}

	// If it wasn't in the list and setting to 0, we are done.
	if (!dwSpeed)
		return;

	// Wasn't in the list so need to create a new affect.
	pAff = new INF_AFF;
	memset(pAff,0,sizeof(INF_AFF));

	pAff->dwAffectType = AFF_TYPE_SPELL;
	pAff->dwTargetType = AFF_TARG_CRE;
	pAff->nNP1 = dwSpeed;
	pAff->dwFlags = 0x09;
	pAff->wProbability1 = 0x64;
	pAff->dwFlags2 = 0x02;
	pAff->dwUnknown3[0] = 0xFFFFFFFF;
	pAff->dwUnknown3[1] = 0xFFFFFFFF;
	pAff->dwUnknown3[2] = 0xFFFFFFFF;
	pAff->dwUnknown3[3] = 0xFFFFFFFF;
	pAff->dwUnknown3[4] = 0x01;
	memcpy(pAff->chResRef3,"SPCL812",7);
	pAff->dwUnknown4[2] = 0xFFFFFFFF;
	pAff->dwUnknown5[0] = 0x0F;
	pAff->dwUnknown5[1] = 0x01;

	m_plAffects.AddTail(pAff);
	m_infCre.dwAffectCount++;
}

void CInfCreature::GetSmallPortrait(CString &strResource)
{
	char szRes[9];
	memset(szRes,0,9);
	memcpy(szRes,m_infCre.chSmallPortrait,8);
	strResource = szRes;
}

void CInfCreature::SetSmallPortrait(const char *pszResource)
{
	int nLen = min(8,strlen(pszResource));
	memset(m_infCre.chSmallPortrait,0,8);
	memcpy(m_infCre.chSmallPortrait,pszResource,nLen);
}

void CInfCreature::GetLargePortrait(CString &strResource)
{
	char szRes[9];
	memset(szRes,0,9);
	memcpy(szRes,m_infCre.chLargePortrait,8);
	strResource = szRes;
}

void CInfCreature::SetLargePortrait(const char *pszResource)
{
	int nLen = min(8,strlen(pszResource));
	memset(m_infCre.chLargePortrait,0,8);
	memcpy(m_infCre.chLargePortrait,pszResource,nLen);
}

BOOL CInfCreature::GetAffects(CPtrList &list)
{
	if (m_plAffects.GetCount() < 1)
		return(TRUE);

	INF_AFF *pAff, *pNew;
	POSITION pos = m_plAffects.GetHeadPosition();
	while(pos)
	{
		pAff = (INF_AFF*)m_plAffects.GetNext(pos);

		// Don't return the affects that are already handled in special cases.
		if (pAff->dwAffectType == AFF_TYPE_PROF ||	
			 (pAff->dwAffectType == AFF_TYPE_SPELL && !strnicmp(pAff->chResRef3,"SPCL812",7))
			)
			continue;

		pNew = new INF_AFF;
		if (!pNew)
			return(FALSE);

		memcpy(pNew,pAff,sizeof(INF_AFF));

		list.AddTail(pNew);
	}

	TRACE("CInfCreautre::GetAffects() - %d affects found.\n",list.GetCount());
	return(TRUE);
}

void CInfCreature::SetAffects(CPtrList &list)
{
	INF_AFF *pAff;
	POSITION posPrev;
	POSITION pos = m_plAffects.GetHeadPosition();
	while(pos)
	{
		posPrev = pos;
		pAff = (INF_AFF*)m_plAffects.GetNext(pos);

		// Delete all the affects except those handled elsewhere.
		if (pAff->dwAffectType == AFF_TYPE_PROF ||	
			 (pAff->dwAffectType == AFF_TYPE_SPELL && !strnicmp(pAff->chResRef3,"SPCL812",7))
			)
			continue;

		delete pAff;
		m_plAffects.RemoveAt(posPrev);
		m_infCre.dwAffectCount--;
		if (m_infCre.dwAffectCount < 0)
			m_infCre.dwAffectCount = 0;

		pos = m_plAffects.GetHeadPosition();
	}

	pos = list.GetHeadPosition();
	while(pos)
	{
		pAff = (INF_AFF*)list.GetNext(pos);

		m_plAffects.AddTail(pAff);
		m_infCre.dwAffectCount++;
	}

	list.RemoveAll();
}
