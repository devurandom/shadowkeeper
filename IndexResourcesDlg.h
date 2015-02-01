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

#if !defined(AFX_INDEXRESOURCESDLG_H__F5183743_5A77_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_INDEXRESOURCESDLG_H__F5183743_5A77_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndexResourcesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndexResourcesDlg dialog

class CInf2DA;
class CValueList;
class CIndexResourcesDlg : public CDialog
{
// Construction
public:
	CIndexResourcesDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL Process2da(const char *pszResource, CInf2DA *p2DA);

	BOOL CreateRacialEnemyList();
	BOOL CreateWeaponProfList();
	BOOL CreateKitList();

	BOOL ProcessList(
		const char *pszResource,					// Resource string.
		CValueList &vl,								// Pointer to a list to receive value pairs.
		BOOL bUseRange=FALSE,						// True to limit values by a range.
		int nMin=0,										// Minimum value for the range.
		int nMax=0,										// Maximum value for the range.
		int nBracketThreshold=-1					// Any values above this value with get [brackets] around the text. -1 disables it.
		);

	BOOL GetLine(const char *pszBuf, int &nStart, CString &strLine);
	BOOL SplitLine(const char *pszLine, int &nValue, CString &strText);

// Dialog Data
	//{{AFX_DATA(CIndexResourcesDlg)
	enum { IDD = IDD_INDEX_RESOURCES };
	CProgressCtrl	m_pcStatus;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndexResourcesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIndexResourcesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnDoResources(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXRESOURCESDLG_H__F5183743_5A77_11D4_9BE4_444553540000__INCLUDED_)
