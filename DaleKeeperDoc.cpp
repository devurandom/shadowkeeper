// DaleKeeperDoc.cpp : implementation of the CDaleKeeperDoc class
//

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

#include "stdafx.h"
#include "DaleKeeper.h"
#include "DaleKeeperDoc.h"
#include "DaleKeeperView.h"
#include "SaveGameDlg.h"
#include "CopyFilesDlg.h"
#include "SaveChrDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperDoc

IMPLEMENT_DYNCREATE(CDaleKeeperDoc, CDocument)

BEGIN_MESSAGE_MAP(CDaleKeeperDoc, CDocument)
	//{{AFX_MSG_MAP(CDaleKeeperDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_TOOLS_EDITOUTOFPARTYCHARACTERS, OnToolsEditoutofpartycharacters)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EDITOUTOFPARTYCHARACTERS, OnUpdateToolsEditoutofpartycharacters)
	ON_COMMAND(ID_TOOLS_SPELLMEMORIZATIONCHECK, OnToolsSpellmemorizationcheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperDoc construction/destruction

CDaleKeeperDoc::CDaleKeeperDoc()
{
	m_bRefreshOverrideOnSave = FALSE;
}

CDaleKeeperDoc::~CDaleKeeperDoc()
{
}

BOOL CDaleKeeperDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperDoc serialization

void CDaleKeeperDoc::Serialize(CArchive& ar)
{
}

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperDoc diagnostics

#ifdef _DEBUG
void CDaleKeeperDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDaleKeeperDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperDoc commands

BOOL CDaleKeeperDoc::OpenGame()
{
	CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();

	m_strGamePath = pApp->m_li.strPath;
	m_strGameFile = m_strGamePath + pApp->m_li.strFile;
	m_strGameTitle = pApp->m_li.strTitle;
	if (!m_infGame.Read(m_strGameFile))
	{
		if (m_infGame.GetLastError() == ERR_NONE)
			AfxGetMainWnd()->MessageBox("Unknown error opening the file.","Failed to Open Game",MB_ICONEXCLAMATION);
		else
		{
			CString strError, strMessage;
			strError.LoadString(m_infGame.GetLastError());
			strMessage.Format("Unable to open that saved game.\r\n\r\nError (%d): %s",
				m_infGame.GetLastError(),(const char*)strError);
			AfxGetMainWnd()->MessageBox(strMessage,"Failed to Open Game",MB_ICONEXCLAMATION);
		}

		return(FALSE);
	}

	SetModifiedFlag(FALSE);
	return(TRUE);
}

BOOL CDaleKeeperDoc::OpenChr()
{
	CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();

	m_strGamePath.Empty();
	m_strGameTitle.Empty();
	m_strGameFile = pApp->m_li.strFile;

	if (!m_infChr.Read(m_strGameFile))
	{
		if (m_infChr.GetLastError() == ERR_NONE)
			AfxGetMainWnd()->MessageBox("Unknown error opening the file.","Failed to Open Character",MB_ICONEXCLAMATION);
		else
		{
			CString strError, strMessage;
			strError.LoadString(m_infChr.GetLastError());
			strMessage.Format("Unable to open that character file.\r\n\r\nError (%d): %s",
				m_infChr.GetLastError(),(const char*)strError);
			AfxGetMainWnd()->MessageBox(strMessage,"Failed to Open Character",MB_ICONEXCLAMATION);
		}

		return(FALSE);
	}

	SetModifiedFlag(FALSE);
	return(TRUE);
}

BOOL CDaleKeeperDoc::OpenCre()
{
	CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();

	m_strGamePath.Empty();
	m_strGameTitle.Empty();
	m_strGameFile = pApp->m_li.strFile;

	CFile file;
	if (!file.Open(m_strGameFile,CFile::modeRead))
	{
		AfxGetMainWnd()->MessageBox("Unable to open that CRE File.","Failed to open CRE",MB_ICONEXCLAMATION);
		return(FALSE);
	}

	if (!m_infCre.Read(file))
	{
		if (m_infCre.GetLastError() == ERR_NONE)
			AfxGetMainWnd()->MessageBox("Unknown error opening the file.","Failed to Open CRE",MB_ICONEXCLAMATION);
		else
		{
			CString strError, strMessage;
			strError.LoadString(m_infCre.GetLastError());
			strMessage.Format("Unable to open that CRE file.\r\n\r\nError (%d): %s",
				m_infCre.GetLastError(),(const char*)strError);
			AfxGetMainWnd()->MessageBox(strMessage,"Failed to Open CRE",MB_ICONEXCLAMATION);
		}

		return(FALSE);
	}

	SetModifiedFlag(FALSE);
	return(TRUE);
}

BOOL CDaleKeeperDoc::OpenResCre()
{
	CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();

	m_strGamePath.Empty();
	m_strGameTitle.Empty();
	m_strGameFile = pApp->m_li.strFile;

	CString strResName;
	int nIndex = m_strGameFile.ReverseFind('\\');
	if (nIndex == -1)
		strResName = m_strGameFile;
	else
		strResName = m_strGameFile.Mid(nIndex+1);
	if (strResName.GetLength() > 4 && strResName.Find('.'))
		strResName = strResName.Left(strResName.GetLength()-4);

	CString strErrorTitle("Failed to Open CRE");

	CResInfo *pResInfo = _infKey.GetResInfo(RESTYPE_CRE,strResName);
	if (!pResInfo)
	{
		AfxGetMainWnd()->MessageBox("Unable to find the creature resource.",strErrorTitle,MB_ICONEXCLAMATION);
		return(FALSE);
	}

	CInfBifFile *pBifFile = _infKey.GetBifFile(pResInfo,TRUE);
	if (!pBifFile || !pBifFile->IsValid())
	{
		AfxGetMainWnd()->MessageBox("Unable to locate the resource.",strErrorTitle,MB_ICONEXCLAMATION);
		return(FALSE);
	}

	int nOffset, nSize;
	if (!pBifFile->GetDataOffsetAndSize(pResInfo,nOffset,nSize))
	{
		AfxGetMainWnd()->MessageBox("Unable to find the resource.",strErrorTitle,MB_ICONINFORMATION);
		return(FALSE);
	}

	pBifFile->Seek(nOffset,CFile::begin);

	if (!m_infCre.Read(*pBifFile))
	{
		if (m_infCre.GetLastError() == ERR_NONE)
			AfxGetMainWnd()->MessageBox("Unknown error opening the file.","Failed to Open CRE",MB_ICONEXCLAMATION);
		else
		{
			CString strError, strMessage;
			strError.LoadString(m_infCre.GetLastError());
			strMessage.Format("Unable to open that CRE file.\r\n\r\nError (%d): %s",
				m_infCre.GetLastError(),(const char*)strError);
			AfxGetMainWnd()->MessageBox(strMessage,"Failed to Open CRE",MB_ICONEXCLAMATION);
		}

		return(FALSE);
	}

	// Now that it is loaded it should be treated the same as a regular CRE load.
	// If the user wants to save the CRE it needs to go through the normal process.
	// If they don't want to save it, they don't end up with a stray CRE resource
	// in the override folder.
	m_nGameType = LI_GAMETYPE_CRE;
	m_bRefreshOverrideOnSave = TRUE;

	SetModifiedFlag(FALSE);
	return(TRUE);
}

BOOL CDaleKeeperDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	// Don't call the base class. It tries to open the file and attach
	// an archive to it so it can serialize the data.

	CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();

	m_nGameType = pApp->m_li.nType;

	switch(m_nGameType)
	{
		case LI_GAMETYPE_CHR :
			return(OpenChr());
		case LI_GAMETYPE_SINGLE :
		case LI_GAMETYPE_MULTI :
			return(OpenGame());
		case LI_GAMETYPE_CRE :
			return(OpenCre());
		case LI_GAMETYPE_RESCRE :
			return(OpenResCre());
	}

	return(FALSE);
}

BOOL CDaleKeeperDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	UpdateAllViews(NULL,HINT_SAVEDATA);

	CDaleKeeperView *pView;
	POSITION pos;

	switch(m_nGameType)
	{
		case LI_GAMETYPE_SINGLE :
		case LI_GAMETYPE_MULTI :
			// Closing the out of party view shouldn't do anything.
			pView = (CDaleKeeperView*)pFrame->GetActiveView();
			if (pView && pView->GetViewType() == DKV_VIEWTYPE_OOP)
				return(TRUE);

			if (m_infGame.HasChanged())
			{
				CString strMessage;
				strMessage.Format("Save changes to game '%s'?",(const char *)m_strGameTitle);
				int nResult = pFrame->MessageBox(strMessage,"Shadow Keeper",MB_YESNOCANCEL|MB_ICONEXCLAMATION);
				if (nResult == IDCANCEL)
					return(FALSE);
				if (nResult == IDYES && !DoSave())
					return(FALSE);
			}  

			// Make sure to close down the OOP view so that the document will close too.
			pos = GetFirstViewPosition();
			while(pos)
			{
				pView = (CDaleKeeperView*)GetNextView(pos);
				if (pView->GetViewType() == DKV_VIEWTYPE_OOP)
				{
					PreCloseFrame(pView->GetParentFrame());
					RemoveView(pView);
					pView->GetParentFrame()->DestroyWindow();
					break;
				}
			}
			break;

		case LI_GAMETYPE_CHR :
			if (m_infChr.HasChanged())
			{
				int nResult = pFrame->MessageBox("Save changes to character?","Shadow Keeper",MB_YESNOCANCEL|MB_ICONEXCLAMATION);
				if (nResult == IDCANCEL)
					return(FALSE);
				if (nResult == IDYES && !DoSave())
					return(FALSE);
			}  
			break;

		case LI_GAMETYPE_CRE :
			if (m_infCre.HasChanged())
			{
				int nResult = pFrame->MessageBox("Save changes to creature?","Shadow Keeper",MB_YESNOCANCEL|MB_ICONEXCLAMATION);
				if (nResult == IDCANCEL)
					return(FALSE);
				if (nResult == IDYES && !DoSave())
					return(FALSE);
			}
			break;

	}

	return(TRUE);
}

void CDaleKeeperDoc::OnFileSave() 
{
	UpdateAllViews(NULL,HINT_SAVEDATA);

	DoSave();
}

BOOL CDaleKeeperDoc::SaveGame()
{
	if (!m_infGame.HasChanged())
		return(TRUE);

	// Create a new saved game name so that it will be obvious which games
	// have been edited.

	// See if there is already an edited tag on the title.
	int nEditNum = 1;
	int nIndex = m_strGameTitle.Find(" (Edited ");
	if (nIndex != -1)
	{
		nEditNum = atoi(m_strGameTitle.Mid(nIndex+8));
		nEditNum++;
	}

	CString strGameTitle;
	strGameTitle.Format("%s (Edited %04d)",(nIndex == -1 ? (const char *)m_strGameTitle : (const char *)m_strGameTitle.Left(nIndex)),nEditNum);

	CSaveGameDlg d;
	d.m_strFilename = strGameTitle;
	if (d.DoModal() != IDOK)
		return(FALSE);
	strGameTitle = d.m_strFilename;

	// Now we need to find the largest folder number being used.
	CString strFind;
	WIN32_FIND_DATA fd;
	int nMaxFile;

	strFind = _strInstallPath + (m_nGameType == LI_GAMETYPE_MULTI ? CString(DIR_MULTISAVE) : CString(DIR_SINGLESAVE)) + "*";
	HANDLE hFind = ::FindFirstFile(strFind,&fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return(FALSE);

	nMaxFile = 1;
	BOOL bNotDone = TRUE;
	while(bNotDone)
	{
		// Make sure it's a directory, and not the '.' or '..'.
		if (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY && isdigit(fd.cFileName[0]) && strcmp(fd.cFileName,".") && strcmp(fd.cFileName,".."))
		{
			if (atoi(fd.cFileName) > nMaxFile)
				nMaxFile = atoi(fd.cFileName);
		}
		bNotDone = ::FindNextFile(hFind,&fd);
	}
	::FindClose(hFind);

	// Make it the next available number.
	nMaxFile++;

	CString strDir;
	strDir.Format("%s%s%09d-%s",
		(const char *)_strInstallPath,
		(m_nGameType == LI_GAMETYPE_MULTI ? DIR_MULTISAVE : DIR_SINGLESAVE),
		nMaxFile,
		(const char *)strGameTitle);

	// Make sure that the directory really doesn't exist.
	if (GetFileAttributes(strDir) != 0xFFFFFFFF)
	{
		CString strMessage;
		strMessage.Format("Unable to locate a new save game directory. An attempt was made and found an already existing directory.\r\n\r\n'%s'\r\n\r\nThe game was not saved.",(const char *)strDir);
		AfxMessageBox(strMessage);
		return(FALSE);
	}

	// Also make sure we aren't creating a directory number that already exists.
	CString strSaveNum;
	strSaveNum.Format("%09d*",nMaxFile);
	strFind = _strInstallPath + (m_nGameType == LI_GAMETYPE_MULTI ? CString(DIR_MULTISAVE) : CString(DIR_SINGLESAVE)) + strSaveNum;
	hFind = ::FindFirstFile(strFind,&fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		CString strMessage;
		strMessage.Format("Unable to locate a new save game number. An attempt was made and found an already existing number.\r\n\r\nNumber = %d\r\n\r\nThe game was not saved.",nMaxFile);
		AfxMessageBox(strMessage);
		::FindClose(hFind);
		return(FALSE);
	}
	::FindClose(hFind);

	if (!::CreateDirectory(strDir,NULL))
	{
		CString strMessage;
		strMessage.Format("Unable to create a new directory:\r\n\r\n'%s'\r\n\r\nThe game was not saved.",(const char *)strDir);
		AfxMessageBox(strMessage);
		return(FALSE);
	}

	// Get a list of all the files that need to be copied.
	strFind = m_strGamePath + "*";
	hFind = ::FindFirstFile(strFind,&fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("Unable to retrieve a list of existing saved game files for copying.");
		return(FALSE);
	}

	CCopyFilesDlg dlgCopy;

	bNotDone = TRUE;
	while(bNotDone)
	{
		// Only copy files and there is no need to copy Icewind.gam which will be reconstructed
		// on the save.
		if (!(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && strcmpi(fd.cFileName,"Baldur.gam"))
			dlgCopy.m_slSource.AddTail(fd.cFileName);

		bNotDone = ::FindNextFile(hFind,&fd);
	}
	::FindClose(hFind);

	strDir += "\\";
	dlgCopy.m_strTargetDir = strDir;
	dlgCopy.m_strSourceDir = m_strGamePath;
	if (dlgCopy.DoModal() != IDOK)
	{
		::RemoveDirectory(strDir);
		AfxMessageBox("File copy canceled. The game was not saved.");
		return(FALSE);
	}

	CString strSaveFile;
	strSaveFile = strDir + "Baldur.gam";

	if (!m_infGame.Write(strSaveFile))
	{
		dlgCopy.DeleteFiles();
		::RemoveDirectory(strDir);

		CString strError, strMessage;
		strError.LoadString(m_infGame.GetLastError());
		strMessage.Format("Unable to save the game.\r\n\r\nError (%d): %s",
			m_infGame.GetLastError(),(const char*)strError);
		AfxGetMainWnd()->MessageBox(strMessage,"Failed to Save Game",MB_ICONEXCLAMATION);
		return(FALSE);
	}

	// Now that it is saved, make the document point at that new file and directory.
	m_strGamePath = strDir;
	m_strGameFile = strSaveFile;
	m_strGameTitle = strGameTitle;

	UpdateAllViews(NULL,HINT_FILESAVED);

	CString strMessage;
	strMessage.Format("Game saved as: %s",(const char *)m_strGameTitle);
	AfxGetMainWnd()->MessageBox(strMessage,"Save Game",MB_ICONINFORMATION);

	return(TRUE);
}

BOOL CDaleKeeperDoc::SaveChr()
{
	if (!m_infChr.HasChanged())
		return(TRUE);

	CString strFile;
	if (_bAllowCHROverwrite)
	{
		CFileDialog d(
			FALSE,
			".chr",
			m_strGameFile,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Characters (*.chr)|*.chr|All Files (*.*)|*.*||");

		if (d.DoModal() == IDCANCEL)
			return(FALSE);

		strFile = d.GetPathName();

		// If the file exists, make a backup of it.
		if (::GetFileAttributes(strFile) != 0xFFFFFFFF)
		{
			CString strBackup(strFile);
			if (strBackup.GetLength() > 4)
			{
				strBackup.TrimRight();
				CString strExt(strBackup.Right(4));
				strExt.MakeLower();

				if (strExt == ".chr")
					strBackup = strBackup.Left(strBackup.GetLength()-4);
				strBackup += ".bak";
			}
			else
				strBackup += ".bak";

			if (!::CopyFile(m_strGameFile,strBackup,FALSE))
			{
				AfxMessageBox("Unable to create a backup of this character file. Save Canceled.");
				return(FALSE);
			}
		}
	}
	else
	{
		CSaveChrDlg d;
		d.m_strPath = m_strGameFile;
		if (d.DoModal() == IDCANCEL)
			return(FALSE);

		strFile = d.m_strPath;
	}

	if (!m_infChr.Write(strFile))
	{
		CString strError, strMessage;
		strError.LoadString(m_infChr.GetLastError());
		strMessage.Format("Unable to save the character.\r\n\r\nError (%d): %s",
			m_infChr.GetLastError(),(const char*)strError);
		AfxGetMainWnd()->MessageBox(strMessage,"Failed to Save Character",MB_ICONEXCLAMATION);
		return(FALSE);
	}

	m_strGameFile = strFile;
	m_strGamePath = strFile;

	UpdateAllViews(NULL,HINT_FILESAVED);
	AfxGetMainWnd()->MessageBox("Character Saved.","Save Character",MB_ICONINFORMATION);
		
	return(TRUE);
}

BOOL CDaleKeeperDoc::SaveCre()
{
	if (!m_infCre.HasChanged())
		return(TRUE);

	CString strSavedAs;
	if (!SaveCreFile(&m_infCre,m_strGameFile,strSavedAs))
		return(FALSE);

	// When the resource is saved under a different name the override needs
	// to be looked at again to catch the new creatures for the browser.
	if (m_bRefreshOverrideOnSave || strcmpi(strSavedAs,m_strGameFile))
	{
		m_bRefreshOverrideOnSave = FALSE;
		_infKey.RefreshOverride();

		CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();
		pApp->NotifyAllDocuments(HINT_OVERRIDE_CHANGED);
	}

	m_strGameFile = strSavedAs;
	m_strGamePath = strSavedAs;

	UpdateAllViews(NULL,HINT_FILESAVED);
	AfxGetMainWnd()->MessageBox("Creature Saved.","Save Creature",MB_ICONINFORMATION);
		
	return(TRUE);
}

BOOL CDaleKeeperDoc::DoSave()
{
	switch(m_nGameType)
	{
		case LI_GAMETYPE_SINGLE :
		case LI_GAMETYPE_MULTI :
			return(SaveGame());
		case LI_GAMETYPE_CHR :
			return(SaveChr());
		case LI_GAMETYPE_CRE :
			return(SaveCre());
	}

	return(FALSE);
}

void CDaleKeeperDoc::OnToolsEditoutofpartycharacters() 
{
	if (m_nGameType != LI_GAMETYPE_SINGLE && m_nGameType != LI_GAMETYPE_MULTI)
		return;

	CDaleKeeperView *pView;
	POSITION pos = GetFirstViewPosition();
	while(pos)
	{
		pView = (CDaleKeeperView*)GetNextView(pos);
		if (pView->GetViewType() == DKV_VIEWTYPE_OOP)
		{
			// Already showing, get rid of it.
			UpdateAllViews(NULL,HINT_SAVEDATA);
			PreCloseFrame(pView->GetParentFrame());
			RemoveView(pView);
			pView->GetParentFrame()->DestroyWindow();

			return;
		}
	}

	CFrameWnd* pFrame = GetDocTemplate()->CreateNewFrame(this, NULL);

	// I got this GetDescdantWindow type cast to view stuff from the MFC
	// framework.
	CWnd* pWnd = pFrame->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
	if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
	{
		CDaleKeeperView *pDKView = (CDaleKeeperView*)pWnd;
		pDKView->SetViewType(DKV_VIEWTYPE_OOP);
	}
	GetDocTemplate()->InitialUpdateFrame(pFrame, this);
}

void CDaleKeeperDoc::OnUpdateToolsEditoutofpartycharacters(CCmdUI* pCmdUI) 
{
	if (m_nGameType == LI_GAMETYPE_SINGLE || m_nGameType == LI_GAMETYPE_MULTI)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);

	CDaleKeeperView *pView;
	POSITION pos = GetFirstViewPosition();
	while(pos)
	{
		pView = (CDaleKeeperView*)GetNextView(pos);
		if (pView->GetViewType() == DKV_VIEWTYPE_OOP)
		{
			pCmdUI->SetCheck(1);
			return;
		}
	}
	pCmdUI->SetCheck(0);
}

BOOL CDaleKeeperDoc::CheckCreMem(CInfCreature *pCre, CString &strErrors)
{
	strErrors.Empty();

	int nInfoCount = pCre->GetMemorizationInfoCount();
	MEMINFO *pMemInfo = new MEMINFO[nInfoCount];
	if (!pMemInfo)
		return(FALSE);
	pCre->GetMemorizationInfo(pMemInfo);

	int nSpellType;
	int nSpellCount;
	SPELLDATA *pSpellData;

	CString strDetail, strType;
	int i,j;
	int nNumMemorized;

	for (nSpellType=0;nSpellType<INF_CRE_SPELLTYPES;nSpellType++)
	{
		nSpellCount = pCre->GetSpellCount(nSpellType);
		pSpellData = new SPELLDATA[nSpellCount];
		if (!pSpellData)
		{
			delete [] pMemInfo;
			return(FALSE);
		}
		pCre->GetSpells(nSpellType,pSpellData);

		// For each line if the memorization info table make sure there are not more
		// memorized for that level than set here.
		for (i=0;i<nInfoCount;i++)
			if (pMemInfo[i].wType == nSpellType)
			{
				nNumMemorized = 0;
				for (j=0;j<nSpellCount;j++)
					if (pMemInfo[i].wLevel == pSpellData[j].wLevel && pSpellData[j].nTimesMemorized)
						nNumMemorized += pSpellData[j].nTimesMemorized;

				if (nNumMemorized > pMemInfo[i].wNumMemorizable)
				{
					if (strErrors.IsEmpty())
						strErrors.Format("[Type]\t[Level]\t[Memorized]\t[Can Memorize]");
					switch(pMemInfo[i].wType)
					{
						case INF_CRE_ST_PRIEST :
							strType = "Priest";
							break;
						case INF_CRE_ST_WIZARD :
							strType = "Wizard";
							break;
						case INF_CRE_ST_INNATE :
							strType = "Innate";
							break;
						default :
							strType = "Unknown";
							break;
					}
					strDetail.Format("\r\n%s\t%d\t%d\t\t%d",
						(const char *)strType,
						pMemInfo[i].wLevel+1,
						nNumMemorized,
						pMemInfo[i].wNumMemorizable);
					strErrors += strDetail;
				}
			}


		delete [] pSpellData;
	}

	delete [] pMemInfo;

	return(!strErrors.IsEmpty());
}

void CDaleKeeperDoc::CheckCreMemChr()
{
	CString strErrors;
	if (CheckCreMem(m_infChr.GetCre(),strErrors))
	{
		CString strErr;
		strErr.LoadString(IDS_ERR_MEMMISMATCH);

		CString strMessage;
		strMessage.Format(strErr,m_infChr.GetName());
		strMessage += strErrors;

		AfxMessageBox(strMessage,MB_ICONINFORMATION);
	}
}

void CDaleKeeperDoc::CheckCreMemCre()
{
	CString strErrors;
	if (CheckCreMem(&m_infCre,strErrors))
	{
		CString strErr;
		strErr.LoadString(IDS_ERR_MEMMISMATCH);

		CString strMessage, strName;
		m_infCre.GetName(strName);
		strMessage.Format(strErr,(const char *)strName);
		strMessage += strErrors;

		AfxMessageBox(strMessage,MB_ICONINFORMATION);
	}
}

void CDaleKeeperDoc::CheckCreMemGame()
{
	CString strErrors;

	int nPartyCount = m_infGame.GetPartyCount();
	int nOutPartyCount = m_infGame.GetOutOfPartyCount();
	int i;

	for (i=0;i<nPartyCount;i++)
	{
		if (CheckCreMem(m_infGame.GetPartyCre(i),strErrors))
		{
			CString strErr;
			strErr.LoadString(IDS_ERR_MEMMISMATCH);

			CString strMessage, strName;
			m_infGame.GetPartyCharName(i,strName);
			strMessage.Format(strErr,(const char *)strName);
			strMessage += strErrors;

			strMessage += "\r\n\r\nContinue checking characters?";

			if (AfxMessageBox(strMessage,MB_ICONINFORMATION|MB_YESNO) == IDNO)
				return;
		}
	}

	for (i=0;i<nOutPartyCount;i++)
	{
		if (CheckCreMem(m_infGame.GetOutOfPartyCre(i),strErrors))
		{
			CString strErr;
			strErr.LoadString(IDS_ERR_MEMMISMATCH);

			CString strMessage, strName;
			m_infGame.GetOutOfPartyCharName(i,strName);
			strName = CString("(Out of Party) ") + strName;
			strMessage.Format(strErr,(const char *)strName);
			strMessage += strErrors;

			strMessage += "\r\n\r\nContinue checking characters?";

			if (AfxMessageBox(strMessage,MB_ICONINFORMATION|MB_YESNO) == IDNO)
				return;
		}
	}

}

void CDaleKeeperDoc::OnToolsSpellmemorizationcheck() 
{
	// Force the current character to update it's CRE.
	UpdateAllViews(NULL,HINT_SAVEDATA);

	switch(m_nGameType)
	{
		case LI_GAMETYPE_SINGLE :
		case LI_GAMETYPE_MULTI :
			CheckCreMemGame();
			break;
		case LI_GAMETYPE_CHR :
			CheckCreMemChr();
			break;
		case LI_GAMETYPE_CRE :
			CheckCreMemCre();
			break;
		default:
			return;
	}

	AfxMessageBox("Done checking spell counts.",MB_ICONINFORMATION);
}
