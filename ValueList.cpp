
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
#include "ValueList.h"

CValueItem::operator=(CValueItem &vi) 
{ 
	m_strText = vi.m_strText; 
	m_nValue = vi.m_nValue; 
}

CValueItem::operator=(const char *pszValue)
{
	m_nValue = strtol(pszValue,NULL,0);;
}

CValueItem::operator=(int nValue)
{
	m_nValue = nValue;
}

// ========================================================================

CValueList::CValueList()
{
}

BOOL CValueList::Load(const char *pszFilename, BOOL bAllowEmpty)
{
	m_strFile = pszFilename;
	CFile file;
	if (!file.Open(m_strFile,CFile::modeRead))
	{
		if (bAllowEmpty)
			return(TRUE);
		
		return(FALSE);
	}

	CArchive ar(&file,CArchive::load);

	int nCount;

	ar >> nCount;
	if (nCount < 1)
	{
		if (bAllowEmpty)
			return(TRUE);

		return(FALSE);
	}

	CValueItem vi;
	for (int i=0;i<nCount;i++)
	{
		ar >> vi.m_strText;
		ar >> vi.m_nValue;

		AddTail(vi);
	}

	ar.Close();
	file.Close();

	POSITION pos = GetHeadPosition();
	while(pos)
	{
		vi = GetNext(pos);

		TRACE("%s,%d\n",(const char *)vi.m_strText,vi.m_nValue);
	}

	return(TRUE);
}

BOOL CValueList::Save(const char *pszFilename)
{
	CString strFile;
	if (!pszFilename)
		strFile = m_strFile;
	else
		strFile = pszFilename;

	CFile file;
	if (!file.Open(strFile,CFile::modeWrite|CFile::modeCreate))
		return(FALSE);

	CArchive ar(&file,CArchive::store);

	ar << GetCount();

	CValueItem vi;
	POSITION pos = GetHeadPosition();
	while(pos)
	{
		vi = GetNext(pos);

		ar << vi.m_strText;
		ar << vi.m_nValue;
	}

	ar.Close();
	file.Close();

	return(TRUE);
}

void CValueList::FillComboBox(CComboBox &cb)
{
	CValueItem vi;
	int nItem;

	POSITION pos = GetHeadPosition();
	while(pos)
	{
		vi = GetNext(pos);
		nItem = cb.AddString(vi.m_strText);
		cb.SetItemData(nItem,vi.m_nValue);
	}
}

BOOL CValueList::SetComboBoxValue(CComboBox &cb, int nValue, BOOL bDisableOnNoMatch)
{
	CValueItem vi;

	int nCount = cb.GetCount();
	for (int i=0;i<nCount;i++)
		if ((int)cb.GetItemData(i) == nValue)
		{
			cb.SetCurSel(i);
			if (bDisableOnNoMatch)
				cb.EnableWindow(TRUE);
			return(TRUE);
		}

	cb.SetCurSel(-1);
	if (bDisableOnNoMatch)
		cb.EnableWindow(FALSE);
	return(FALSE);

	cb.SetCurSel(-1);
	if (bDisableOnNoMatch)
		cb.EnableWindow(FALSE);
	return(FALSE);
}

DWORD CValueList::GetComboBoxValue(CComboBox &cb)
{
	int nIndex = cb.GetCurSel();
	if (nIndex == CB_ERR)
		return(-1);
	return(cb.GetItemData(nIndex));
}

BOOL CValueList::GetText(int nValue, CString &strText)
{
	CValueItem vi;
	POSITION pos = GetHeadPosition();
	while(pos)
	{
		vi = GetNext(pos);

		if (vi.m_nValue == nValue)
		{
			strText = vi.m_strText;
			return(TRUE);
		}
	}

	return(FALSE);
}

