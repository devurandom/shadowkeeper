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


// DaleKeeperDoc.h : interface of the CDaleKeeperDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DALEKEEPERDOC_H__8C7CE88D_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
#define AFX_DALEKEEPERDOC_H__8C7CE88D_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InfGame.h"
#include "InfChr.h"

// Sent to the view to tell it to take the data form the controls and store
// it. This is sent by the document before saving the data or querying for
// CanClose.
#define HINT_SAVEDATA				100
#define HINT_FILESAVED				101		// Sent when the document has been saved.

class CDaleKeeperDoc : public CDocument
{
protected: // create from serialization only
	CDaleKeeperDoc();
	DECLARE_DYNCREATE(CDaleKeeperDoc)

	// Saves the current file. Returns TRUE if it was successful.
	BOOL DoSave();

// Attributes
public:
	BOOL SaveGame();
	BOOL SaveChr();
	BOOL SaveCre();

	BOOL OpenGame();
	BOOL OpenChr();
	BOOL OpenCre();
	BOOL OpenResCre();

	CInfGame			m_infGame;
	CInfChr			m_infChr;
	CInfCreature	m_infCre;

	CString m_strGamePath;
	CString m_strGameFile;
	CString m_strGameTitle;

	int	m_nGameType;

private:
	// If this is TRUE the overrides will be rescanned when the
	// file is saved. This is used when a user is just viewing a CRE
	// resource and then saves it. When it gets saved it becomes an
	// overridden resource if they saved it with the same name or
	// a custom resource if they gave it a new name. The Creature
	// Browser's list must also be updated to make sure the proper
	// version gets loaded when edit is selected. This only needs to
	// be done the first time the resource is saved. There are checks
	// to refresh is the CRE is saved under a name different from
	// the loaded name, this catches the first time save where they 
	// are the same but still need to be refreshed.
	BOOL	m_bRefreshOverrideOnSave;

	// Checks a CRE to make sure the number of spells actually memorized
	// by the character is not greater than the numbers indicated on the
	// memorization tab. strErrors is a list of all the errors it found.
	// Returns TRUE if it find some mismatches.
	BOOL CheckCreMem(CInfCreature *pCre, CString &strErrors);

	void CheckCreMemGame();
	void CheckCreMemChr();
	void CheckCreMemCre();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaleKeeperDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDaleKeeperDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDaleKeeperDoc)
	afx_msg void OnFileSave();
	afx_msg void OnToolsEditoutofpartycharacters();
	afx_msg void OnUpdateToolsEditoutofpartycharacters(CCmdUI* pCmdUI);
	afx_msg void OnToolsSpellmemorizationcheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DALEKEEPERDOC_H__8C7CE88D_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
