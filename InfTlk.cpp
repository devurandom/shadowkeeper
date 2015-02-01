
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
#include "InfTlk.h"
#include "Global.h"

CInfTlk::CInfTlk() : CFile()
{
	m_pEntries = NULL;
}

CInfTlk::~CInfTlk()
{
	delete [] m_pEntries;
}

BOOL CInfTlk::Open(const char *pszFilename)
{
	AddToLog("Opening Dialog.tlk:\n");
	if (!CFile::Open(pszFilename,CFile::modeRead|CFile::shareDenyNone))
		return(FALSE);

	AddToLog("   Reading dialog header.\n");
	if (Read(&m_tlkHeader,sizeof(INF_TLK_HEADER)) != sizeof(INF_TLK_HEADER))
	{
		AfxMessageBox("Unabled to read the header from the .tlk file.");
		return(FALSE);
	}

	AddToLog("   Allocating memory for string entries.\n");
	m_pEntries = new STRINGENTRY[m_tlkHeader.dwStringCount];
	if (!m_pEntries)
	{
		AfxMessageBox("Unable to allocate memory for string index table.");
		return(FALSE);
	}

	AddToLog("   Reading string entries.\n");
	INF_TLK_ENTRY entry;
	for (int i=0;i<(int)m_tlkHeader.dwStringCount;i++)
	{
		if (Read(&entry,sizeof(INF_TLK_ENTRY)) != sizeof(INF_TLK_ENTRY))
		{
			AfxMessageBox("Failed reading a talk file entry.");
			return(FALSE);
		}

		m_pEntries[i].dwOffset = entry.dwOffset + m_tlkHeader.dwStringOffset;
		m_pEntries[i].dwLength = entry.dwLength;
		m_pEntries[i].wUnknown = entry.wUnknown;
	}

	TRACE("TLK String Count : %d\n",m_tlkHeader.dwStringCount);
	AddToLog("   Done with Dialog.tlk\n");

	return(TRUE);
}

BOOL CInfTlk::GetString(DWORD dwIndex, CString &str)
{
	str.Empty();
	if (dwIndex >= m_tlkHeader.dwStringCount)
		return(FALSE);

	if (!m_pEntries[dwIndex].dwLength)
		return(FALSE);

	if (Seek(m_pEntries[dwIndex].dwOffset,CFile::begin) != (int)m_pEntries[dwIndex].dwOffset)
		return(FALSE);

	char *pBuf = str.GetBuffer(m_pEntries[dwIndex].dwLength+1);
	if (!pBuf)
		return(FALSE);
	if (Read(pBuf,m_pEntries[dwIndex].dwLength) != m_pEntries[dwIndex].dwLength)
	{
		str.ReleaseBuffer(0);
		return(FALSE);
	}
	str.ReleaseBuffer(m_pEntries[dwIndex].dwLength);

	return(TRUE);
}

