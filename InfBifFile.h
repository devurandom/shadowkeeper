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

#ifndef __INFBIFFILE__
#define __INFBIFFILE__

#pragma pack(push,1)

struct INF_BIF_HEADER
{
	char		chSignature[4];
	char		chVersion[4];
	DWORD		dwFileEntryCount;
	DWORD		dwTileEntryCount;
	DWORD		dwFileEntryOffset;		// Tileset entries will follow the file entries.
};

struct INF_BIF_FILEENTRY
{
	DWORD		dwLocator;
	DWORD		dwOffset;
	DWORD		dwSize;
	WORD		wType;
	char		chUnknown[2];
};

// I didn't add the tileset types. I don't currently have any interest in
// them.

struct INF_ITM
{
	char		chSignature[4];
	char		chVersion[4];
	DWORD		dwGenericItemName;
	DWORD		dwIdentifiedItemName;
	char		chUsedUpItem[8];
	DWORD		dwTypeFlags;
	WORD		wCategory;
	DWORD		dwUsability;
	char		chInvBAMResourceIndex[2];
	WORD		wMinLevel;
	WORD		wMinStrength;
	WORD		wMinStrengthBonus;
	WORD		wMinIntelligence;
	WORD		wMinDexterity;
	WORD		wMinWisdom;
	WORD		wMinConstitution;
	WORD		wMinCharisma;
	DWORD		dwBaseValue;
	WORD		wMaxStackable;
	char		chItemIcon[8];
	WORD		wLore;
	char		chGroundIcon[8];
	DWORD		dwBaseWeight;
	DWORD		dwItemDescriptionGeneric;
	DWORD		dwItemDescriptionIdentified;
	char		chCarrieddIcon[8];
	DWORD		dwEnchantment;
	DWORD		dwAbilityOffset;
	WORD		dwAbilityCount;
	DWORD		dwEffectsOffset;
	WORD		wUnknown;
	WORD		wGlobalEffects;
};

struct INF_SPL
{
	char		chSignature[4];
	char		chVersion[4];
	DWORD		dwGenericSpellName;
	DWORD		dwIdentifiedSpellName;
	char		chCastSoundRef[8];
	DWORD		dwUnknown;
	WORD		wCategory;
	DWORD		dwUsability;
	WORD		wUnknown2;
	WORD		wMinLevel;
	WORD		wMinStrength;
	WORD		wMinStrengthBonus;
	WORD		wMinIntelligence;
	WORD		wMinDexterity;
	WORD		wMinWisdom;
	WORD		wMinConstitution;
	WORD		wMinCharisma;
	DWORD		dwSpellLevel;
	WORD		wUnknown3;
	char		chSpellIcon[8];
	WORD		wUnknown4;
	char		chUnknown[8];
	DWORD		dwUnknown3;
	DWORD		dwSpellDescriptionGeneric;
	DWORD		dwSpellDescriptionIdentified;
	char		chUnknown2[8];
	DWORD		dwUnknown4;
	DWORD		dwAbilityOffset;
	WORD		wAbilityCount;
	DWORD		dwEffectsOffset;
	WORD		wUnknown5;
	WORD		wGlobalEffects;
};

#pragma pack(pop)

class CBifFileEntry : public CObject
{
public:
	DWORD		dwOffset;
	DWORD		dwSize;
};

#include "InfBam.h"

struct INF_CRE;
class CResInfo;
class CInfBifFile : public CFile
{
public:
	CInfBifFile();
	~CInfBifFile();

	// Opens a BIF file for use. This also reads in the index file and hashes
	// bif entries. Files opened bAsOverride are assumed to be a particular type
	// of resource, no lookup is done when getting data.
	BOOL Open(const char *pszFilename, BOOL bAsOverride=FALSE);

	void GetFilename(CString &strFilename)				{ strFilename = m_strFilename; }

	// Retrieves the offset in the bif file and the size of the data for the passed
	// in resource. Returns TRUE if successful finding the information.
	BOOL GetDataOffsetAndSize(const CResInfo *pResInfo, int &nOffset, int &nSize);

	// Given a resource type and a locator returns the data to pData.
	// When it finds the resource it returns the offset from the start of the
	// .bif file. This offset is used when the internal resource has offsets 
	// that start relative to the start of this resource in the file. Returns -1
	// if it fails.
	// If this file is an override resource, the first two parameters are
	// ignored.
	int GetData(WORD wResourceType, WORD wLocator, void *pData, int nBufSize);
	int GetData(const CResInfo *pResInfo, void *pData, int nBufSize);

	// Does the same as above essentially.
	int GetData(WORD wLocator, INF_ITM *pItem);
//	int GetData(WORD wLocator, INF_BAM_HEADER *pBamHeader);
	int GetData(WORD wLocator, INF_SPL *pSpell);
	int GetData(WORD wLocator, INF_CRE *pCre);

	// Retrieves a BAM resource object.
	BOOL GetBam(const CResInfo *pResInfo, CInfBam &bam);

	BOOL IsValid()							{ return(m_bIsValid); }

private:
	INF_BIF_HEADER	m_bifHeader;

	// Used to keep track of the different resource types. The type is the key
	// to this map. Each entry in the map is a CMapWordToOb object, using the 
	// resource locator as a key to the entry.
	CMapWordToOb	m_mapResType;

	// Name of the open file.
	CString			m_strFilename;

	// Set to TRUE when we are looking at an override file.
	BOOL				m_bAsOverride;

	// Set by the open function.
	BOOL				m_bIsValid;
};

#endif