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


// DaleKeeper.h : main header file for the DALEKEEPER application
//

#if !defined(AFX_DALEKEEPER_H__8C7CE885_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
#define AFX_DALEKEEPER_H__8C7CE885_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperApp:
// See DaleKeeper.cpp for the implementation of this class
//

// Filled before OpenDocumentFile is called. Contains
// information on the type of file being opened.
#define LI_GAMETYPE_SINGLE			0x00
#define LI_GAMETYPE_MULTI			0x01
#define LI_GAMETYPE_CHR				0x02
#define LI_GAMETYPE_CRE				0x03
// RESCRE loads up the CRE resource from the game files. The
// path still needs to be set as if it was going to be loaded
// from the override directory. This allows people to view
// the resource without having to extract it. They can then
// save the file if they want keep the changes.
#define LI_GAMETYPE_RESCRE			0x04

struct LOADINFO
{
	int		nType;
	CString	strPath;
	CString	strFile;
	CString	strTitle;
};

struct VALUELIST;
class CDaleKeeperApp : public CWinApp
{
public:
	CDaleKeeperApp();

	void LoadSavedGame(const char *pszPath, const char *pszTitle, BOOL bMultiPlayer);
	void LoadCharacterFile(const char *pszFilename);
	void LoadCreatureFile(const char *pszFilename, BOOL bFromResource=FALSE);

	BOOL LoadLists();

	// Retrieves the resource file version.
	void FetchFileVersion();
	CString m_strFileVersion;

	// Sends a document hint to all the open documents;
	void NotifyAllDocuments(LPARAM lHint);

	CMultiDocTemplate* GetItemBrowserTemplate()			{ return(m_pItemBrowserTemplate); }
	CMultiDocTemplate* GetSpellBrowserTemplate()			{ return(m_pSpellBrowserTemplate); }
	CMultiDocTemplate* GetCreatureBrowserTemplate()		{ return(m_pCreatureBrowserTemplate); }

	// Filled in before OpenDocumentFile is called for editing character data.
	LOADINFO	m_li;

private:

	// This is being corrupted somewhere. I'm making it private and accessing
	// through a function to make sure I'm not accidentally assigning it a new
	// value somewhere. (Turned out this wasn't the problem, but leaving it like
	// this anyway.)
	CMultiDocTemplate *m_pItemBrowserTemplate;
	CMultiDocTemplate *m_pSpellBrowserTemplate;
	CMultiDocTemplate *m_pCreatureBrowserTemplate;

	CMultiDocTemplate *m_pEditorTemplate;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaleKeeperApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDaleKeeperApp)
	afx_msg void OnAppAbout();
	afx_msg void OnSettingsInstallationdirectory();
	afx_msg void OnFileOpensavedgame();
	afx_msg void OnUpdateFileOpensavedgame(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewItembrowser(CCmdUI* pCmdUI);
	afx_msg void OnViewItembrowser();
	afx_msg void OnHelpVisitthewebsite();
	afx_msg void OnHelpSendemailtoaaron();
	afx_msg void OnUpdateViewSpellbrowser(CCmdUI* pCmdUI);
	afx_msg void OnViewSpellbrowser();
	afx_msg void OnSettingsSpelllimits();
	afx_msg void OnSettingsMiscellaneous();
	afx_msg void OnHelpViewreadmedoc();
	afx_msg void OnFileOpencharacterfile();
	afx_msg void OnUpdateFileOpencharacterfile(CCmdUI* pCmdUI);
	afx_msg void OnSettingsListsKits();
	afx_msg void OnSettingsListsGender();
	afx_msg void OnSettingsListsRace();
	afx_msg void OnSettingsListsClass();
	afx_msg void OnSettingsListsAlignment();
	afx_msg void OnSettingsListsRacialenemy();
	afx_msg void OnSettingsListsNumberofatacks();
	afx_msg void OnSettingsListsProficiencies();
	afx_msg void OnViewCreaturebrowser();
	afx_msg void OnUpdateViewCreaturebrowser(CCmdUI* pCmdUI);
	afx_msg void OnFileOpencrefile();
	afx_msg void OnUpdateSettingsSaveoverwriteAllowonchrfiles(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSettingsSaveoverwriteAllowoncrefiles(CCmdUI* pCmdUI);
	afx_msg void OnSettingsSaveoverwriteAllowonchrfiles();
	afx_msg void OnSettingsSaveoverwriteAllowoncrefiles();
	afx_msg void OnSettingsListsAffects();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DALEKEEPER_H__8C7CE885_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
