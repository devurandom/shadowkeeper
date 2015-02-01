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

class CValueItem : public CObject
{
public:
	CString	m_strText;
	int		m_nValue;

public:
	// Needed for the CList template class.
	operator=(CValueItem &vi);

	// Both of these assign the m_nValue member. The text version takes
	// a str and does a strtol on it to determine a value.
	operator=(const char *pszValue);
	operator=(int nValue);
};

class CValueList : public CList<CValueItem,CValueItem&>
{
public:
	CValueList();

	BOOL	Load(const char *pszFilename, BOOL bAllowEmpty=FALSE);
	BOOL	Save(const char *pszFilename = NULL);

	BOOL	GetText(int nValue, CString &strText);

	void	FillComboBox(CComboBox &cb);
	static BOOL PASCAL SetComboBoxValue(CComboBox &cb, int nValue, BOOL bDisableOnNoMatch);
	static DWORD PASCAL GetComboBoxValue(CComboBox &cb);

private:
	CString m_strFile;
};