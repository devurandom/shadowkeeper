

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
#include "Inf2DA.h"

CInf2DA::CInf2DA()
{
	m_nBufLen = 0;
	m_pszBuf = NULL;

	m_pHeader = NULL;
	m_pData = NULL;

	m_nRows = 0;
	m_nCols = 0;
}

CInf2DA::~CInf2DA()
{
	delete [] m_pszBuf;
	delete [] m_pHeader;
	delete [] m_pData;
}

BOOL CInf2DA::GetLine(int &nIndex, CString &strLine)
{
	strLine.Empty();
	if (nIndex >= m_nBufLen)
		return(FALSE);

	// Read characters up until the end of the buffer, a carriage return or a linefeed.
	while(m_pszBuf[nIndex] && m_pszBuf[nIndex] != '\r' && m_pszBuf[nIndex] != '\n')
	{
		strLine += m_pszBuf[nIndex];
		nIndex++;
	}

	// Position the index to the start of the next line.
	while(m_pszBuf[nIndex] && (m_pszBuf[nIndex] == '\r' || m_pszBuf[nIndex] == '\n'))
		nIndex++;

	return(TRUE);
}

int CInf2DA::ParseLine(const char *pszLine, CString *pRow)
{
	int nLen = strlen(pszLine);
	if (!pszLine || !nLen)
		return(0);

	int nCount = 0;
	int nIndex = 0;
	while(nIndex < nLen)
	{
		// Eat up any whitespace.
		while(nIndex < nLen && isspace(pszLine[nIndex]))
			nIndex++;

		// At end of buffer?
		if (nIndex >= nLen)
			break;

		if (pRow)
		{
			// I've seen some 2da files that have a row or two that has a lot more
			// columns (no data) than there are headers. If there is a pointer to
			// a row I'm assuming that the counts have already been done and that it
			// is ok to use m_nCols.
			if (nCount < m_nCols)
				pRow[nCount].Empty();
			else
				return(nCount);
		}

		// Collect the data up until the next whitespace.
		while(nIndex < nLen && !isspace(pszLine[nIndex]))
		{
			if (pRow)
			{
				// See comments above about m_nCols and long rows.
				if (nCount < m_nCols)
					pRow[nCount] += pszLine[nIndex];
				else
					return(nCount);
			}
			nIndex++;
		}

		nCount++;
		nIndex++;
	}

	return(nCount);
}

BOOL CInf2DA::GetDimensions()
{
	CString strLine;
	int nIndex = 0;

	// Eat up the first two lines. The first line is the version string. I don't know
	// what the second line represents.
	if (!GetLine(nIndex,strLine) || !GetLine(nIndex,strLine))
		return(FALSE);

	// The next line has the header names for the columns.
	if (!GetLine(nIndex,strLine))
		return(FALSE);

	// First determine how many columns there are by the number of headers. The first
	// column doesn't have a header so Number of Headers + 1.
	m_nCols = ParseLine(strLine,NULL) + 1;
	if (!m_nCols)
		return(FALSE);

	// Just need to count how many lines exist now.
	m_nRows = 0;
	while(GetLine(nIndex,strLine))
		m_nRows++;

	return(TRUE);
}

BOOL CInf2DA::Parse(const char *pszText)
{
	m_nBufLen = strlen(pszText);
	if (!m_nBufLen)
		return(FALSE);

	m_pszBuf = new char[m_nBufLen+1];
	if (!m_pszBuf)
		return(FALSE);

	// Keep a local copy around.
	strcpy(m_pszBuf,pszText);

	// Figure out how big the array needs to be.
	if (!GetDimensions())
		return(FALSE);

	CString strLine;
	int i;
	int nIndex = 0;
	
	// Get the column headers. Eat up the first two lines and we want the third.
	// The number of columns I've seen has always been one less than the actual data
	// but I'm allocating enough room for a full row just in case the column header
	// line is missing; in which case it would parse out into a full row.
	m_pHeader = new CString[m_nCols];
	if (!m_pHeader)
		return(FALSE);
	for (i=0;i<3;i++)
		if (!GetLine(nIndex,strLine))
			return(FALSE);

	if (ParseLine(strLine,m_pHeader) != m_nCols-1)
		return(FALSE);

	m_pData = new CString[m_nRows*m_nCols];
	if (!m_pData)
		return(FALSE);

	for (i=0;i<m_nRows;i++)
	{
		if (!GetLine(nIndex,strLine))
			return(FALSE);

		ParseLine(strLine,m_pData+i*m_nCols);
	}

	return(TRUE);
}

CString CInf2DA::GetHeader(int nCol)
{
	if (nCol >= m_nCols-1)
		return(CString(""));

	return(m_pHeader[nCol]);
}

BOOL CInf2DA::GetData(int nRow, int nCol, int &nValue)
{
	if (nRow >= m_nRows || nCol >= m_nCols)
		return(FALSE);

	nValue = strtol(m_pData[nRow*m_nCols+nCol],NULL,0);
	return(TRUE);
}

BOOL CInf2DA::GetData(int nRow, int nCol, DWORD &dwValue)
{
	if (nRow >= m_nRows || nCol >= m_nCols)
		return(FALSE);

	dwValue = strtol(m_pData[nRow*m_nCols+nCol],NULL,0);

	return(TRUE);
}

BOOL CInf2DA::GetData(int nRow, int nCol, CString &strValue)
{
	if (nRow >= m_nRows || nCol >= m_nCols)
		return(FALSE);

	strValue = m_pData[nRow*m_nCols+nCol];
	return(TRUE);
}

BOOL CInf2DA::GetDataByValue(int nSearchCol, int nSearchValue, int nValueCol, CString &strValue)
{
	if (nSearchCol >= m_nCols || nValueCol >= m_nCols)
		return(FALSE);

	for (int i=0;i<m_nRows;i++)
	{
		if (strtol(m_pData[i*m_nCols+nSearchCol],NULL,0) == nSearchValue)
		{
			strValue = m_pData[i*m_nCols+nValueCol];
			return(TRUE);
		}
	}

	return(FALSE);
}
