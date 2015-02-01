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

#ifndef __INFKEY__
#define __INFKEY__

#pragma pack(push,1)

struct INF_KEY_HEADER
{
	char		chHeader[4];
	char		chVersion[4];
	DWORD		dwBifCount;
	DWORD		dwResourceCount;
	DWORD		dwBifOffset;
	DWORD		dwResourceOffset;
};

struct INF_KEY_BIF
{
	DWORD		dwFileLength;
	DWORD		dwFilenameOffset;
	WORD		wFilenameLength; // Includes NULL.
	WORD		wFileLocation;
};

struct INF_KEY_RESOURCE
{
	char		chName[8];
	WORD		wType;
	DWORD		dwLocator;
};

#pragma pack(pop)

// Bitvalues for CResInfo flags.
#define RIF_USEOVERRIDE				0x01
#define RIF_CUSTOM					0x02

class CResInfo : public CObject
{
public:
	CResInfo()
	{
		wBifKeyIndex = 0;
		chTileIndex = 0;
		wLocator = 0;
		chFlags = 0;
//		chUseOverride = 0;
		wResourceType = 0;
	}

	CString	strName;					// Resource string name.
	WORD		wBifKeyIndex;			// Index to the bif file.
	BYTE		chTileIndex;			
	WORD		wLocator;				// Locator of the resource in the bif file.
//	BYTE		chUseOverride;			// Used a byte just to save space because there are a lot
											//		resources. Set to 1 when the resource should be
											//		retrieved from the override directory.
	WORD		wResourceType;			// Duplicated since the resources are divided into their
											//		own lists by type. But there are times when it is
											//		handy to have this available with just the resource
											//		info pointer.
	BYTE		chFlags;

public:
	void	SetFlag(BYTE chFlagValue)				{ chFlags |= chFlagValue; }
	void  RemoveFlag(BYTE chFlagValue)			{ chFlags &= (~chFlagValue); }
	BOOL	IsFlagSet(BYTE chFlagValue) const	{ return(chFlags & chFlagValue); }
};

class CBifInfo
{
public:
	CString	strFilename;
	WORD		wLocation;
};

// Made these two wrappers for the pointers just so I didn't have to worry
// about deleting them over the various exit points in the Read function.
class CBifPtr
{
public:
	CBifPtr()			{ptr = NULL;}
	~CBifPtr()			{delete [] ptr;}

	INF_KEY_BIF	*ptr;
};

class CResPtr
{
public:
	CResPtr()			{ptr = NULL;}
	~CResPtr()			{delete [] ptr;}

	INF_KEY_RESOURCE	*ptr;
};

#include "InfBifFile.h"

#define XOR_KEY_LEN 64

extern BYTE _xorKey[XOR_KEY_LEN];

// This object is used for overriden obejct types. When a bif file pointer
// is requested and that resource is an override type a temporary file object
// is created and returned. This object stores the filename and the pointer
// to the bif file object. A list is maintained by the key file. During idle
// processing this list of objects is cleaned up.
class CTempBifFile : public CObject
{
public:
	WORD			wResType;
	CString		strResName;
	CInfBifFile	infFile;
};

// I tried using CMapStringToOb thinking it would provide a fast way to look up
// resource strings. Well, it looks them up fast, but adding all the items to the
// object took a considerable amount of time. I'm just now putting the items in a
// CObList and walking the list looking for the string. This I'm sure technically
// adds time to the lookups, but at least it isn't noticable. I had to make a
// progress dialog for the Map method while loading the resources.
//
// I'm still using a map to distinguish between different resource types.

// Multiplier I'm using to show the progress of opening the bif files.
#define FILE_PROG_MULT			20

class CInfKey
{
public:
	CInfKey();
	~CInfKey();

	// The pointer to a progress control is optional. If the pointer is it will
	// pump messages and update the status bar while it works.
	BOOL Read(const char *pszFilename, CProgressCtrl *pCtrl=NULL);

	// Returns one of the resource lists.
	BOOL GetResList(WORD wResourceType, CObList *&rList);

	// Returns a pointer to one of the resource info objects. You can do this by hand
	// by getting a pointer to the list and searching the list for the resource string.
	// This saves a step and lets this class walk the list for you.
	CResInfo* GetResInfo(WORD wResourceType, const char *pszResName);

	// Pass in a bif index, get the filename back. The bifindex is stored
	// in the CResInfo object.
	BOOL GetBifFile(DWORD nBifKeyIndex, CString &strFilename);

	// Retursn the name of the bif file associated with a resource.
	BOOL	GetBifFile(const CResInfo *pInfo, CString &strFilename);

	// Returns a pointer to the already open bif file. Do NOT close the bif file object.
	// It will be closed in the destructor of this object.
	CInfBifFile* GetBifFile(DWORD nBifKeyIndex);

	// This is the preferred way to get a bif file. If the resource information passed
	// in happens to be an overriden resource, a temporary bif file object will be
	// created and a pointer to that passed back. This way the code doesn't have to ever
	// care about their being a difference. The above version of GetBifFile refers to
	// one of the actual game bif files.
	CInfBifFile* GetBifFile(const CResInfo *pInfo, BOOL bIgnoreOverride=FALSE);

	// Reads resource data into a buffer given a resource type and name.
	// When successful pBuf points to the newly allocated memory and nLen gives the
	// size of the buffer. If bAddNull is TRUE the buffer is created one bigger than
	// needed to hold the data and a null stuffed in there. This is to handle text
	// resources.
	BOOL GetResData(WORD wResourceType, const char *pszResName, BYTE *&pBuf, int &nLen, BOOL bAddNull=FALSE);

	// Called from the app's OnIdle function. It deletes all the temporary bif
	// file objects needed for overriden resources.
	void OnIdleCleanup();

	// Called initially when indexing the resources. Can be called anytime there after
	// to rescan the override directory. This is needed whenever a user has saved/extracted
	// a new CRE resource.
	void RefreshOverride();

	// Returns the number of Bif Files.
	DWORD GetFileCount()					{ return(m_keyHeader.dwBifCount); }

private:
	INF_KEY_HEADER		m_keyHeader;

	// Using this to keep track of the various resource types. Each of the objects
	// a type refers to is a CMapStringToObj which uses the Resource String as
	// the key.
	CMapWordToOb		m_mapResType;
	CBifInfo				*m_pBifInfo;
	CInfBifFile			*m_pBifFiles;

	// Maps the bif key index to the bif file objects.
	CMapWordToOb		m_mapFiles;

	// List of temporary bif files that have been opened. This list is cleaned
	// out during idle processing.
	CObList				m_listTempBif;
};

#endif