
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

#if !defined(AFX_OPENSAVEDDLG_H__8C7CE89B_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
#define AFX_OPENSAVEDDLG_H__8C7CE89B_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenSavedDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenSavedDlg dialog

#ifndef __OPENGAMEDATA__
#define __OPENGAMEDATA__

#include "BitmapWnd.h"

class COpenGameData
{
public:
	COpenGameData();

	void GetNumber(CString &strNumber);

	CString	m_strPath;
	CString	m_strName;
	CTime		m_tWritten;
};

#define SORT_NAME_ASC	0
#define SORT_NAME_DES	1
#define SORT_TIME_ASC	2
#define SORT_TIME_DES	3

class COpenSavedDlg : public CDialog
{
// Construction
public:
	COpenSavedDlg(CWnd* pParent = NULL);   // standard constructor

	void ClearList();

	// Reads the save directory and fills the list control with all the
	// games.
	void FillGames();

	// Splits the game name portion off of a saved game directory name.
	void GetName(const char *pszDir, CString &strName);

	// Keeps track of the current sort column and direction. Set to one of the
	// defines above.
	int  m_nSort;

	// Set to TRUE when filling in the list of games. The change notification I'm using
	// to read in the game bitmap gets fired while I'm filling the list. This is just to
	// prevent it from loading the bitmaps every time.
	BOOL m_bLoadingGames;

	CBitmapWnd m_wndBitmap;

	// This is filled in when OK is pushed. It's the name of the diretory that has the
	// game to be loaded.
	CString m_strDirName;

	// Filled in when OK is pushed. This is the title of the saved game.
	CString m_strTitle;
	
// Dialog Data
	//{{AFX_DATA(COpenSavedDlg)
	enum { IDD = IDD_OPEN_GAME };
	CButton	m_btnOK;
	CListCtrl	m_lcGames;
	BOOL	m_bMultiplayer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenSavedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenSavedDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnColumnclickGames(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedGames(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnDblclkGames(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMultiplayer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENSAVEDDLG_H__8C7CE89B_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
