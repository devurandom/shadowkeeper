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
#include "InfChr.h"
#include "Global.h"

CInfChr::CInfChr()
{
	m_bHasChanged = FALSE;
	m_nError = ERR_NONE;
}

BOOL CInfChr::Read(const char *pszFilename)
{
	CFile file;
	UINT nCount;

	if (!file.Open(pszFilename,CFile::modeRead))
	{
		m_nError = ERR_CHR_FAILEDOPEN;
		return(FALSE);
	}
	m_strFilename = pszFilename;

	nCount = file.Read(&m_infChr,sizeof(INF_CHR));
	if (nCount != sizeof(INF_CHR))
	{
		m_nError = ERR_CHR_BADHEADER;
		return(FALSE);
	}

	if (memcmp(m_infChr.chSignature,"CHR ",4))
	{
		m_nError = ERR_CHR_MISSINGSIG;
		return(FALSE);
	}

	if (!_bIgnoreDataVersions && memcmp(m_infChr.chVersion,"V2.0",4) && memcmp(m_infChr.chVersion,"V2.1",4))
	{
		m_nError = ERR_CHR_BADVERSION;
		return(FALSE);
	}

	if (!m_infCre.Read(file))
	{
		m_nError = m_infCre.GetLastError();
		return(FALSE);
	}

	m_bHasChanged = FALSE;
	return(TRUE);
}

BOOL CInfChr::Write(const char *pszFilename)
{
	m_infChr.dwDataStart = sizeof(INF_CHR);
	m_infChr.dwDataLen = m_infCre.GetFileSpace();

	CFile file;
	if (!file.Open(pszFilename,CFile::modeCreate|CFile::modeWrite))
	{
		m_nError = ERR_CHR_FAILEDCREATE;
		return(FALSE);
	}

	try{ file.Write(&m_infChr,sizeof(INF_CHR)); }
	catch(CFileException *pe)
	{
		m_nError = ERR_CHR_WRITEHEADER;
		pe->Delete();
		return(FALSE);
	}

	if (!m_infCre.Write(file))
	{
		m_nError = m_infCre.GetLastError();
		return(FALSE);
	}

	m_bHasChanged = FALSE;
	file.Close();
	return(TRUE);
}

void CInfChr::GetName(CString &strName)
{
	strName = m_infChr.szName;
}

void CInfChr::SetName(const char *pszName)
{
	memset(m_infChr.szName,0,21);

	if (strlen(pszName) > 20)
		memcpy(m_infChr.szName,pszName,20);
	else
		strcpy(m_infChr.szName,pszName);

}

BOOL CInfChr::HasChanged()
{
	if (m_bHasChanged || m_infCre.HasChanged())
		return(TRUE);
	
	return(FALSE);
}