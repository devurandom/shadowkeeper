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

#if !defined(AFX_ADDABILITIESDLG_H__D056CE74_095C_4A58_A401_2381A1FB1903__INCLUDED_)
#define AFX_ADDABILITIESDLG_H__D056CE74_095C_4A58_A401_2381A1FB1903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddAbilitiesDlg.h : header file
//

#define KITLIST_INDEX			0
#define KITLIST_NAME				1
#define KITLIST_TITLE			3
#define KITLIST_ABILITY			5

/////////////////////////////////////////////////////////////////////////////
// CAddAbilitiesDlg dialog

class CAddAbilitiesDlg : public CDialog
{
// Construction
public:
	CAddAbilitiesDlg(CWnd* pParent = NULL);   // standard constructor

	// Filled in when OK is pushed.
	int m_nKitIndex;
	int m_nLevelFrom;
	int m_nLevelTo;

private:
	void FillKits();
	void FillLevels();


public:

// Dialog Data
	//{{AFX_DATA(CAddAbilitiesDlg)
	enum { IDD = IDD_SPECIAL_ABILITIES };
	CComboBox	m_cbTo;
	CComboBox	m_cbFrom;
	CComboBox	m_cbKit;
	BOOL	m_bRemoveKit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddAbilitiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddAbilitiesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDABILITIESDLG_H__D056CE74_095C_4A58_A401_2381A1FB1903__INCLUDED_)
