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

#ifndef __INFCHR__
#define __INFCHR__

#pragma pack(push,1)

struct INF_CHR
{
	char	chSignature[4];
	char	chVersion[4];

	char	szName[0x15];

	char	chUnknown[0x0B];

	// Where the NPC_HEADER info starts.
	DWORD	dwDataStart;

	// Total length of the character.
	DWORD	dwDataLen;

	char	chUnknown1[0x34];
};

#pragma pack(pop)

#include "InfCreature.h"

#define ERR_CHR_FAILEDOPEN				2001
#define ERR_CHR_BADHEADER				2002
#define ERR_CHR_MISSINGSIG				2003
#define ERR_CHR_BADVERSION				2004
#define ERR_CHR_FAILEDCREATE			2005
#define ERR_CHR_WRITEHEADER			2006

class CInfChr
{
public:
	CInfChr();

	BOOL	Read(const char *pszFilename);
	BOOL	Write(const char *pszFilename);

	void	GetName(CString &strName);
	const char *GetName()							{ return(m_infChr.szName); }
	void	SetName(const char *pszName);

	CInfCreature *GetCre()							{ return(&m_infCre); }
	
	int	GetLastError()								{ return(m_nError); }
	BOOL	HasChanged();

private:
	INF_CHR			m_infChr;
	CString			m_strFilename;
	CInfCreature	m_infCre;
	int				m_nError;
	BOOL				m_bHasChanged;
};

#endif