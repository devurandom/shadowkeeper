// DaleKeeper.cpp : Defines the class behaviors for the application.
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
#include "Global.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DaleKeeperDoc.h"
#include "DaleKeeperView.h"
#include "InstallPathDlg.h"
#include "OpenSavedDlg.h"
#include "OpenCreDlg.h"
#include "ItemBrowserFrame.h"
#include "ItemBrowserDoc.h"
#include "ItemBrowserView.h"
#include "SpellBrowserFrame.h"
#include "SpellBrowserDoc.h"
#include "SpellBrowserView.h"
#include "CreatureBrowserFrame.h"
#include "CreatureBrowserDoc.h"
#include "CreatureBrowserView.h"
#include "LimitSpellsDlg.h"
#include "MiscSettingsDlg.h"
#include "OpenChrDlg.h"
#include "StaticUrl.h"
#include "EditListDlg.h"
#include "EditUserListDlg.h"
#include "ValueList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperApp

BEGIN_MESSAGE_MAP(CDaleKeeperApp, CWinApp)
	//{{AFX_MSG_MAP(CDaleKeeperApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_SETTINGS_INSTALLATIONDIRECTORY, OnSettingsInstallationdirectory)
	ON_COMMAND(ID_FILE_OPENSAVEDGAME, OnFileOpensavedgame)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENSAVEDGAME, OnUpdateFileOpensavedgame)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ITEMBROWSER, OnUpdateViewItembrowser)
	ON_COMMAND(ID_VIEW_ITEMBROWSER, OnViewItembrowser)
	ON_COMMAND(ID_HELP_VISITTHEWEBSITE, OnHelpVisitthewebsite)
	ON_COMMAND(ID_HELP_SENDEMAILTOAARON, OnHelpSendemailtoaaron)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPELLBROWSER, OnUpdateViewSpellbrowser)
	ON_COMMAND(ID_VIEW_SPELLBROWSER, OnViewSpellbrowser)
	ON_COMMAND(ID_SETTINGS_SPELLLIMITS, OnSettingsSpelllimits)
	ON_COMMAND(ID_SETTINGS_MISCELLANEOUS, OnSettingsMiscellaneous)
	ON_COMMAND(ID_HELP_VIEWREADMEDOC, OnHelpViewreadmedoc)
	ON_COMMAND(ID_FILE_OPENCHARACTERFILE, OnFileOpencharacterfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENCHARACTERFILE, OnUpdateFileOpencharacterfile)
	ON_COMMAND(ID_SETTINGS_LISTS_KITS, OnSettingsListsKits)
	ON_COMMAND(ID_SETTINGS_LISTS_GENDER, OnSettingsListsGender)
	ON_COMMAND(ID_SETTINGS_LISTS_RACE, OnSettingsListsRace)
	ON_COMMAND(ID_SETTINGS_LISTS_CLASS, OnSettingsListsClass)
	ON_COMMAND(ID_SETTINGS_LISTS_ALIGNMENT, OnSettingsListsAlignment)
	ON_COMMAND(ID_SETTINGS_LISTS_RACIALENEMY, OnSettingsListsRacialenemy)
	ON_COMMAND(ID_SETTINGS_LISTS_NUMBEROFATACKS, OnSettingsListsNumberofatacks)
	ON_COMMAND(ID_SETTINGS_LISTS_PROFICIENCIES, OnSettingsListsProficiencies)
	ON_COMMAND(ID_VIEW_CREATUREBROWSER, OnViewCreaturebrowser)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CREATUREBROWSER, OnUpdateViewCreaturebrowser)
	ON_COMMAND(ID_FILE_OPENCREFILE, OnFileOpencrefile)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_SAVEOVERWRITE_ALLOWONCHRFILES, OnUpdateSettingsSaveoverwriteAllowonchrfiles)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_SAVEOVERWRITE_ALLOWONCREFILES, OnUpdateSettingsSaveoverwriteAllowoncrefiles)
	ON_COMMAND(ID_SETTINGS_SAVEOVERWRITE_ALLOWONCHRFILES, OnSettingsSaveoverwriteAllowonchrfiles)
	ON_COMMAND(ID_SETTINGS_SAVEOVERWRITE_ALLOWONCREFILES, OnSettingsSaveoverwriteAllowoncrefiles)
	ON_COMMAND(ID_SETTINGS_LISTS_AFFECTS, OnSettingsListsAffects)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperApp construction

CDaleKeeperApp::CDaleKeeperApp()
{
	m_pItemBrowserTemplate = NULL;
	m_pSpellBrowserTemplate = NULL;
	m_pCreatureBrowserTemplate = NULL;
	m_pEditorTemplate = NULL;

#ifdef _DEBUG
	// Enabling this here before all the BIF crap is loaded takes a considerable
	// amount of time. It took at least 5 minutes to index the resources.

//	afxMemDF = allocMemDF | checkAlwaysMemDF;
#endif
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDaleKeeperApp object

CDaleKeeperApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperApp initialization

void CDaleKeeperApp::FetchFileVersion()
{
	CString strExe(m_pszExeName);
	strExe += ".exe";
	m_strFileVersion = "Version Failed";

	DWORD dwHandle;
	DWORD dwSize = GetFileVersionInfoSize((char*)((const char*)strExe),&dwHandle);
	if (!dwSize)
		return;

	char *pData = new char[dwSize];
	ASSERT(pData);
	if (!pData)
	{
		m_strFileVersion = "Version Failed";
		return;
	}

	if (GetFileVersionInfo((char*)((const char*)strExe),dwHandle,dwSize,pData))
	{
		VS_FIXEDFILEINFO *pFileInfo;
		UINT   nResultLen;
		if (VerQueryValue(pData,"\\",(LPVOID*)(&pFileInfo),&nResultLen))
		{
			m_strFileVersion.Format("%d.%d.%d",
				HIWORD(pFileInfo->dwFileVersionMS),
				LOWORD(pFileInfo->dwFileVersionMS),
				HIWORD(pFileInfo->dwFileVersionLS));
		}
	}

	if (_bLogFile)
		m_strFileVersion += " (Logs)";

	delete [] pData;
}

int CDaleKeeperApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}

BOOL CDaleKeeperApp::InitInstance()
{
	// Some people are having trouble with the editor not finding the ULD files.
	// I'm going to force the working directory to that of the EXE file.
	char szFilename[MAX_PATH+1];
	DWORD dwLen = ::GetModuleFileName(NULL,szFilename,MAX_PATH);
	if (dwLen)
	{
		CString strPath(szFilename);
		int nIndex = strPath.ReverseFind('\\');
		if (nIndex != -1)
		{
			strPath = strPath.Left(nIndex+1);
			::SetCurrentDirectory(strPath);
		}
	}
	
	::DeleteFile(LOGFILE);

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("AIO"));

	CString strSection;

	strSection = "Settings";
	_strInstallPath		= GetProfileString(strSection,"Install Path","");

	_bUseKnownSpellLimit = GetProfileInt(strSection,"Use Known Spell Limit",TRUE);
	_bUseMemSpellLimit	= GetProfileInt(strSection,"Use Mem Spell Limit",TRUE);
	_nKnownSpellLimit		= GetProfileInt(strSection,"Known Spell Limit",20);
	_nMemSpellLimit		= GetProfileInt(strSection,"Mem Spell Limit",12);
	_bMemSpellsOnSave		= GetProfileInt(strSection,"Mem Spells On Save",TRUE);
	_bDisableSpellIcons	= GetProfileInt(strSection,"Disable Spell Icons",FALSE);

	_bDefaultOpenSinglePlayer = GetProfileInt(strSection,"Default Open Single",TRUE);
	_bUseGridLines			= GetProfileInt(strSection,"Use Grid Lines",FALSE);
	_bIgnoreDataVersions = GetProfileInt(strSection,"Ignore Data Versions",FALSE);
	_bAllowQuicksaves		= GetProfileInt(strSection,"Allow Quicksave Editing",FALSE);

	strSection = "Allow Overwrite";
	_bAllowCHROverwrite	= GetProfileInt(strSection,"CHR",FALSE);
	_bAllowCREOverwrite	= GetProfileInt(strSection,"CRE",FALSE);

	if (_strInstallPath.IsEmpty())
		FindInstallPath(_strInstallPath);

	_nClipFormatAff = RegisterClipboardFormat("Infinity CRE Affect");

	// They can still cancel out and not set a path, but at least they were informed as
	// to why things aren't going to work.
	if (_strInstallPath.IsEmpty())
	{
		AfxMessageBox("Shadow Keeper was unable to locate your Shadows of Amn isntallation. "
			"You will need to set the path yourself.");
		OnSettingsInstallationdirectory();
	}

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	AddToLog("Retrieving file version.\n",TRUE);
	FetchFileVersion();
	AddToLog("   File version retrieved.\n",TRUE);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pEditorTemplate = new CMultiDocTemplate(
		IDR_DALEKETYPE,
		RUNTIME_CLASS(CDaleKeeperDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDaleKeeperView));
	AddDocTemplate(m_pEditorTemplate);

	m_pItemBrowserTemplate = new CMultiDocTemplate(
		IDR_ITEMBROWSER,
		RUNTIME_CLASS(CItemBrowserDoc),
		RUNTIME_CLASS(CItemBrowserFrame),
		RUNTIME_CLASS(CItemBrowserView));
	AddDocTemplate(m_pItemBrowserTemplate);

	m_pSpellBrowserTemplate = new CMultiDocTemplate(
		IDR_SPELLBROWSER,
		RUNTIME_CLASS(CSpellBrowserDoc),
		RUNTIME_CLASS(CSpellBrowserFrame),
		RUNTIME_CLASS(CSpellBrowserView));
	AddDocTemplate(m_pSpellBrowserTemplate);

	m_pCreatureBrowserTemplate = new CMultiDocTemplate(
		IDR_CREATUREBROWSER,
		RUNTIME_CLASS(CCreatureBrowserDoc),
		RUNTIME_CLASS(CCreatureBrowserFrame),
		RUNTIME_CLASS(CCreatureBrowserView));
	AddDocTemplate(m_pCreatureBrowserTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// LOG is the only current option I have so easy to check for.
	if (m_lpCmdLine)
	{
		CString strCmdLine(m_lpCmdLine);
		strCmdLine.MakeLower();
		if (!strCmdLine.IsEmpty() && strCmdLine.Find("log") != -1)
			_bLogFile = TRUE;
	}

	if (!LoadLists())
		return(FALSE);

	// Dispatch commands specified on the command line
/*	if (!ProcessShellCommand(cmdInfo))
		return FALSE; */

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// I'm posting the message here just to make sure it happens after any other
	// initialization that maybe have to be done and sitting in the queue. CMainFrame
	// processes this message by poping up a status bar dialog as it works through
	// the .key file.
	::PostMessage(pMainFrame->GetSafeHwnd(),MSG_DORESOURCES,0,0);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	CStaticUrl m_suHacking;
	CStaticUrl m_suZlib;
	CStaticUrl m_suEmail;
	CStaticUrl m_suVisit;

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnEmail();
	afx_msg void OnWebsite();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_EMAIL, OnEmail)
	ON_BN_CLICKED(IDC_WEBSITE, OnWebsite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_suHacking.SubclassDlgItem(IDC_HACKING,this);
	m_suZlib.SubclassDlgItem(IDC_ZLIB,this);
	m_suEmail.SubclassDlgItem(IDC_MAILTO,this);
	m_suVisit.SubclassDlgItem(IDC_VISIT,this);

	m_suHacking.m_strUrl = "http://www.ugcs.caltech.edu/~jedwin/baldur.html";
	m_suZlib.m_strUrl = "http://www.freesoftware.com/pub/infozip/zlib/";
	m_suEmail.m_strUrl = _pszEmailAddress;
	m_suVisit.m_strUrl = _pszWebAddress;

	return(TRUE);
}

// App command to run the dialog
void CDaleKeeperApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.m_strVersion = m_strFileVersion;
	aboutDlg.DoModal();
}

void CAboutDlg::OnEmail() 
{
	m_suEmail.Launch();
}

void CAboutDlg::OnWebsite() 
{
	m_suVisit.Launch();
}

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperApp message handlers

void CDaleKeeperApp::OnSettingsInstallationdirectory() 
{
	CInstallPathDlg d;
	
	d.m_strPath = _strInstallPath;
	if (d.DoModal() == IDOK)
	{
		_strInstallPath = d.m_strPath;

		AfxGetApp()->WriteProfileString("Settings","Install Path",_strInstallPath);

		AfxMessageBox("You must restart Shadow Keeper for the new settings to take effect.");
	}
}

void CDaleKeeperApp::OnUpdateFileOpensavedgame(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_bResourcesLoaded);	
}

void CDaleKeeperApp::OnUpdateFileOpencharacterfile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_bResourcesLoaded);	
}

void CDaleKeeperApp::LoadSavedGame(const char *pszPath, const char *pszTitle, BOOL bMultiPlayer)
{
	m_li.nType = (bMultiPlayer ? LI_GAMETYPE_MULTI : LI_GAMETYPE_SINGLE);
	m_li.strPath = pszPath;
	m_li.strTitle = pszTitle;
	m_li.strFile = "Baldur.gam";

	CString strOpen(pszPath);
	strOpen += "Baldur.gam";

	OpenDocumentFile(strOpen);
}

void CDaleKeeperApp::LoadCharacterFile(const char *pszFilename)
{
	m_li.nType = LI_GAMETYPE_CHR;
	m_li.strTitle.Empty();
	m_li.strFile = pszFilename;
	m_li.strPath.Empty();

	OpenDocumentFile(pszFilename);
}

void CDaleKeeperApp::LoadCreatureFile(const char *pszFilename, BOOL bFromResource)
{
	if (bFromResource)
		m_li.nType = LI_GAMETYPE_RESCRE;
	else
		m_li.nType = LI_GAMETYPE_CRE;
	m_li.strTitle.Empty();
	m_li.strFile = pszFilename;
	m_li.strPath.Empty();

	OpenDocumentFile(pszFilename);
}

void CDaleKeeperApp::OnFileOpencrefile() 
{
	COpenCreDlg d;
	
	if (d.DoModal() == IDOK)
		LoadCreatureFile(d.m_strPath);
}

void CDaleKeeperApp::OnFileOpensavedgame() 
{
	COpenSavedDlg d;
	
	if (d.DoModal() == IDOK)
		LoadSavedGame(d.m_strDirName,d.m_strTitle,d.m_bMultiplayer);
}

void CDaleKeeperApp::OnFileOpencharacterfile() 
{
	COpenChrDlg d;
	
	if (d.DoModal() == IDOK)
		LoadCharacterFile(d.m_strPath);
}

void CDaleKeeperApp::OnUpdateViewItembrowser(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_bResourcesLoaded);
	
	if (m_pItemBrowserTemplate->GetFirstDocPosition())
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0); 
}

void CDaleKeeperApp::OnViewItembrowser() 
{
	if (m_pItemBrowserTemplate->GetFirstDocPosition())
		m_pItemBrowserTemplate->CloseAllDocuments(FALSE);
	else
 		m_pItemBrowserTemplate->OpenDocumentFile(NULL);
}

void CDaleKeeperApp::OnUpdateViewSpellbrowser(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_bResourcesLoaded);
	
	if (m_pSpellBrowserTemplate->GetFirstDocPosition())
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0); 
}

void CDaleKeeperApp::OnViewSpellbrowser() 
{
	if (m_pSpellBrowserTemplate->GetFirstDocPosition())
		m_pSpellBrowserTemplate->CloseAllDocuments(FALSE);
	else
		m_pSpellBrowserTemplate->OpenDocumentFile(NULL);
}

void CDaleKeeperApp::OnViewCreaturebrowser() 
{
	if (m_pCreatureBrowserTemplate->GetFirstDocPosition())
		m_pCreatureBrowserTemplate->CloseAllDocuments(FALSE);
	else
		m_pCreatureBrowserTemplate->OpenDocumentFile(NULL);
}

void CDaleKeeperApp::OnUpdateViewCreaturebrowser(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_bResourcesLoaded);
	
	if (m_pCreatureBrowserTemplate->GetFirstDocPosition())
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0); 
}

void CDaleKeeperApp::OnHelpVisitthewebsite() 
{
	HINSTANCE hResult = ShellExecute(
		AfxGetMainWnd()->GetSafeHwnd(),
		"open",
		_pszWebAddress,
		NULL,
		NULL,
		SW_SHOWNORMAL
		);
}

void CDaleKeeperApp::OnHelpSendemailtoaaron() 
{
	HINSTANCE hResult = ShellExecute(
		AfxGetMainWnd()->GetSafeHwnd(),
		"open",
		_pszEmailAddress,
		NULL,
		NULL,
		SW_SHOWNORMAL
		);
}

BOOL CDaleKeeperApp::OnIdle(LONG lCount) 
{
	LONG lReturnCount = CWinApp::OnIdle(lCount);

	if (!lReturnCount)
		_infKey.OnIdleCleanup();

	return(lReturnCount);
}

void CDaleKeeperApp::OnSettingsSpelllimits() 
{
	CLimitSpellsDlg d;
	
	d.m_bUseKnownSpellLimit = _bUseKnownSpellLimit;
	d.m_bUseMemSpellLimit = _bUseMemSpellLimit;
	d.m_nKnownSpellLimit = _nKnownSpellLimit;
	d.m_nMemSpellLimit = _nMemSpellLimit;
	d.m_bMemSpellsOnSave = _bMemSpellsOnSave;
	d.m_bDisableSpellIcons = _bDisableSpellIcons;
	if (d.DoModal() == IDOK)
	{
		_bUseKnownSpellLimit = d.m_bUseKnownSpellLimit;
		_bUseMemSpellLimit = d.m_bUseMemSpellLimit;
		_nKnownSpellLimit = d.m_nKnownSpellLimit;
		_nMemSpellLimit = d.m_nMemSpellLimit;
		_bMemSpellsOnSave = d.m_bMemSpellsOnSave;
		_bDisableSpellIcons = d.m_bDisableSpellIcons;

		CString strSection("Settings");
		WriteProfileInt(strSection,"Use Known Spell Limit",_bUseKnownSpellLimit);
		WriteProfileInt(strSection,"Use Mem Spell Limit",_bUseMemSpellLimit);
		WriteProfileInt(strSection,"Known Spell Limit",_nKnownSpellLimit);
		WriteProfileInt(strSection,"Mem Spell Limit",_nMemSpellLimit);
		WriteProfileInt(strSection,"Mem Spells On Save",_bMemSpellsOnSave);
		WriteProfileInt(strSection,"Disable Spell Icons",_bDisableSpellIcons);
	}
}

void CDaleKeeperApp::NotifyAllDocuments(LPARAM lHint)
{
	POSITION pos;
	CDocument *pDoc;
	CMultiDocTemplate *pTemplate;
	for (int i=0;i<4;i++)
	{
		switch(i)
		{
			case 0 :
				pTemplate = m_pEditorTemplate;
				break;
			case 1 :
				pTemplate = m_pItemBrowserTemplate;
				break;
			case 2 :
				pTemplate = m_pSpellBrowserTemplate;
				break;
			case 3 :
				pTemplate = m_pCreatureBrowserTemplate;
				break;
		}
		ASSERT(pTemplate);

		pos = pTemplate->GetFirstDocPosition();
		while(pos)
		{
			pDoc = pTemplate->GetNextDoc(pos);
			ASSERT(pDoc);
			pDoc->UpdateAllViews(NULL,lHint);
		}
	}
}

void CDaleKeeperApp::OnSettingsMiscellaneous() 
{
	CMiscSettingsDlg d;
	
	d.m_bDefaultOpenSinglePlayer = _bDefaultOpenSinglePlayer;
	d.m_bUseGridLines = _bUseGridLines;
	d.m_bIgnoreDataVersions = _bIgnoreDataVersions;
	d.m_bAllowQuicksaves = _bAllowQuicksaves;
	if (d.DoModal() == IDOK)
	{
		_bIgnoreDataVersions = d.m_bIgnoreDataVersions;
		_bDefaultOpenSinglePlayer = d.m_bDefaultOpenSinglePlayer;
		_bAllowQuicksaves = d.m_bAllowQuicksaves;
		if (_bUseGridLines != d.m_bUseGridLines)
		{
			_bUseGridLines = d.m_bUseGridLines;
			NotifyAllDocuments(HINT_CHANGE_GRIDLINES);
		}

		CString strSection;
		strSection = "Settings";

		WriteProfileInt(strSection,"Default Open Single",_bDefaultOpenSinglePlayer);
		WriteProfileInt(strSection,"Use Grid Lines",_bUseGridLines);
		WriteProfileInt(strSection,"Ignore Data Versions",_bIgnoreDataVersions);
		WriteProfileInt(strSection,"Allow Quicksave Editing",_bAllowQuicksaves);
	}
}

void CDaleKeeperApp::OnHelpViewreadmedoc() 
{
	HINSTANCE hResult = ShellExecute(
		AfxGetMainWnd()->GetSafeHwnd(),
		"open",
		"Readme.doc",
		NULL,
		NULL,
		SW_SHOWNORMAL
		);
}

BOOL CDaleKeeperApp::LoadLists()
{
	AddToLog("Loading user defined lists:\n");

	if (!_vlKit.Load(ULF_KITS) ||
		 !_vlGenderUser.Load(ULF_GENDER,TRUE) ||
		 !_vlRaceUser.Load(ULF_RACE,TRUE) ||
		 !_vlClassUser.Load(ULF_CLASS,TRUE) ||
		 !_vlAlignment.Load(ULF_ALIGNMENT) ||
		 !_vlRacialEnemyUser.Load(ULF_RACIALENEMY,TRUE) ||
		 !_vlNumAttacks.Load(ULF_NUMATTACKS) ||
		 !_vlProfsUser.Load(ULF_PROFS,TRUE) ||
		 !_vlAffects.Load(ULF_AFFECTS))
	{
		AfxMessageBox("Shadow Kepeer was unable to locate or open some of the .ULD files. "
			"These files are required for the editor to run properly. Make sure that the "
			"following files are in the same directory as the editor's .EXE file: \r\n\r\n"
			"   Kits.uld\r\n"
			"   Gender.uld\r\n"
			"   Race.uld\r\n"
			"   Class.uld\r\n"
			"   Alignment.uld\r\n"
			"   RacialEnemy.uld\r\n"
			"   NumAttacks.uld\r\n"
			"   Affects.uld\r\n"
			"   Profs.uld\r\n\r\n"
			"Make sure that when you unzipped the files they were unzipped using Long Filenames. "
			"Some older zip utilities may not support this. Some zip utilities also have options "
			"to use the old DOS [8].[3] format when unzipping. If you see files that look like "
			"'Racial~1.uld' you did not unzip them with Long Filenames."
			"\r\n\r\n"
			"If you used a shortcut to start the editor, right click the shortcut and open "
			"the properties window. Make sure that the 'Start In' directory is the same as "
			"the directory the .EXE and .ULD files are in.",
			MB_ICONEXCLAMATION);
		AddToLog("   One or more lists FAILED to load.\n");
		return(FALSE);
	}

	AddToLog("   Lists loaded.\n");
	return(TRUE);
}

void CDaleKeeperApp::OnSettingsListsKits() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlKit;
	d.m_nRange = RANGE_DWORD;
	d.m_strListName = "Kits";
	if (d.DoModal() == IDOK)
		_vlKit.Save();
}

void CDaleKeeperApp::OnSettingsListsGender() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlGenderUser;
	d.m_nRange = RANGE_BYTE;
	d.m_strListName = "Gender";
	if (d.DoModal() == IDOK)
		_vlGenderUser.Save();
}

void CDaleKeeperApp::OnSettingsListsRace() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlRaceUser;
	d.m_nRange = RANGE_BYTE;
	d.m_strListName = "Race";
	if (d.DoModal() == IDOK)
		_vlRaceUser.Save();
}

void CDaleKeeperApp::OnSettingsListsClass() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlClassUser;
	d.m_nRange = RANGE_BYTE;
	d.m_strListName = "Class";
	if (d.DoModal() == IDOK)
		_vlClassUser.Save();
}

void CDaleKeeperApp::OnSettingsListsAlignment() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlAlignment;
	d.m_nRange = RANGE_BYTE;
	d.m_strListName = "Alignment";
	if (d.DoModal() == IDOK)
		_vlAlignment.Save();
}

void CDaleKeeperApp::OnSettingsListsRacialenemy() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlRacialEnemyUser;
	d.m_nRange = RANGE_BYTE;
	d.m_strListName = "Racial Enemy";
	if (d.DoModal() == IDOK)
		_vlRacialEnemyUser.Save();
}

void CDaleKeeperApp::OnSettingsListsNumberofatacks() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlNumAttacks;
	d.m_nRange = RANGE_BYTE;
	d.m_strListName = "Number of Attacks";
	if (d.DoModal() == IDOK)
		_vlNumAttacks.Save();
}

void CDaleKeeperApp::OnSettingsListsProficiencies() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlProfsUser;
	d.m_nRange = RANGE_BYTE;
	d.m_strListName = "Proficiencies";
	if (d.DoModal() == IDOK)
		_vlProfsUser.Save();
}


void CDaleKeeperApp::OnSettingsListsAffects() 
{
	CEditListDlg d;
	
	d.m_pvl = &_vlAffects;
	d.m_nRange = RANGE_WORD;
	d.m_strListName = "Affect Types";
	if (d.DoModal() == IDOK)
		_vlAffects.Save();
}

void CDaleKeeperApp::OnUpdateSettingsSaveoverwriteAllowonchrfiles(CCmdUI* pCmdUI) 
{
	if (_bAllowCHROverwrite)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CDaleKeeperApp::OnUpdateSettingsSaveoverwriteAllowoncrefiles(CCmdUI* pCmdUI) 
{
	if (_bAllowCREOverwrite)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CDaleKeeperApp::OnSettingsSaveoverwriteAllowonchrfiles() 
{
	if (_bAllowCHROverwrite)
		_bAllowCHROverwrite = FALSE;
	else
		_bAllowCHROverwrite = TRUE;

	WriteProfileInt("Allow Overwrite","CHR",_bAllowCHROverwrite);
}

void CDaleKeeperApp::OnSettingsSaveoverwriteAllowoncrefiles() 
{
	if (_bAllowCREOverwrite)
		_bAllowCREOverwrite = FALSE;
	else
		_bAllowCREOverwrite = TRUE;

	WriteProfileInt("Allow Overwrite","CRE",_bAllowCREOverwrite);
}

