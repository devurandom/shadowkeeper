// DaleKeeperView.cpp : implementation of the CDaleKeeperView class
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
#include "InfGame.h"
#include "InfKey.h"
#include "InfBifFile.h"
#include "InfTlk.h"
#include "ItemBrowserCom.h"
#include "SpellBrowserCom.h"
#include "Global.h"
#include "DaleKeeperDoc.h"
#include "DaleKeeperView.h"
#include "PortraitsDlg.h"
#include "NameRefDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperView

IMPLEMENT_DYNCREATE(CDaleKeeperView, CFormView)

BEGIN_MESSAGE_MAP(CDaleKeeperView, CFormView)
	//{{AFX_MSG_MAP(CDaleKeeperView)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_CHARACTER_NEXT, OnCharacterNext)
	ON_COMMAND(ID_CHARACTER_PREV, OnCharacterPrev)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_COMMAND(ID_PAGE_NEXT, OnPageNext)
	ON_COMMAND(ID_PAGE_PREV, OnPagePrev)
	ON_UPDATE_COMMAND_UI(ID_PAGE_NEXT, OnUpdatePageNext)
	ON_UPDATE_COMMAND_UI(ID_PAGE_PREV, OnUpdatePagePrev)
	ON_COMMAND(ID_PAGE_ABILITIES, OnPageAbilities)
	ON_COMMAND(ID_PAGE_SAVES, OnPageSaves)
	ON_COMMAND(ID_PAGE_PROF, OnPageProf)
	ON_COMMAND(ID_PAGE_RESISTANCES, OnPageResistances)
	ON_COMMAND(ID_PAGE_THIEF, OnPageThief)
	ON_COMMAND(ID_PAGE_CHARACTERISTICS, OnPageCharacteristics)
	ON_COMMAND(ID_VIEW_INVENTORY, OnViewInventory)
	ON_BN_CLICKED(IDC_PORTRAIT, OnPortrait)
	ON_BN_CLICKED(IDC_NAME_REF, OnNameRef)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CONVERTTOCRE, OnUpdateToolsConverttocre)
	ON_COMMAND(ID_TOOLS_CONVERTTOCRE, OnToolsConverttocre)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperView construction/destruction

CDaleKeeperView::CDaleKeeperView()
	: CFormView(CDaleKeeperView::IDD)
{
	//{{AFX_DATA_INIT(CDaleKeeperView)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_nCurrentChar = 0;
	m_hPortrait = NULL;
	m_nViewType = DKV_VIEWTYPE_UNKNOWN;

	for (int i=0;i<NUM_VIEW_TABS;i++)
		m_pTabDlg[i] = NULL;
}

CDaleKeeperView::~CDaleKeeperView()
{
	_comItemBrowser.ClearSelected(this);
	_comSpellBrowser.ClearSelected(this);
	
	for (int i=0;i<NUM_VIEW_TABS;i++)
		if (m_pTabDlg[i] && m_pTabDlg[i]->GetSafeHwnd())
			m_pTabDlg[i]->DestroyWindow();

	if (m_hPortrait)
		::DeleteObject(m_hPortrait);
}

void CDaleKeeperView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDaleKeeperView)
	DDX_Control(pDX, IDC_NAME_REF, m_btnNameRef);
	DDX_Control(pDX, IDC_PORTRAIT, m_btnPortrait);
	DDX_Control(pDX, IDC_PARTYSCROLL, m_sbParty);
	DDX_Control(pDX, IDC_NAME, m_edName);
	DDX_Text(pDX, IDC_NAME, m_strName);
	//}}AFX_DATA_MAP
}

BOOL CDaleKeeperView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CDaleKeeperView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit(FALSE);

	m_tab.SubclassDlgItem(IDC_TAB,this);

	// When viewing out of party characters the type is already
	// known and don't want to reassign it.
	if (m_nViewType == DKV_VIEWTYPE_UNKNOWN)
		switch(GetDocument()->m_nGameType)
		{
			case LI_GAMETYPE_SINGLE :
			case LI_GAMETYPE_MULTI :
				m_nViewType = DKV_VIEWTYPE_PARTY;
				break;
			case LI_GAMETYPE_CHR :
				m_nViewType = DKV_VIEWTYPE_CHR;
				break;
			case LI_GAMETYPE_CRE :
				m_nViewType = DKV_VIEWTYPE_CRE;
				break;
		}

	// I turned off the automatic title setting. It didn't like the way I was
	// passing the filename/title into the document and was printing the wrong
	// title.
	SetGameTitle();

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.nMin = 0;
	switch(m_nViewType)
	{
		case DKV_VIEWTYPE_CHR :
		case DKV_VIEWTYPE_CRE :
			si.nMax = 0;
			break;
		case DKV_VIEWTYPE_PARTY :
			si.nMax = GetDocument()->m_infGame.GetPartyCount()-1;
			break;
		case DKV_VIEWTYPE_OOP :
			si.nMax = GetDocument()->m_infGame.GetOutOfPartyCount()-1;
			break;
	}
	si.nPage = 1;
	si.nPos = 0;
	si.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
	m_sbParty.SetScrollInfo(&si);

	// Fits the edit and scrollbar controls to match up with the character
	// portraits.
	ResizeControls();

	if (m_nViewType == DKV_VIEWTYPE_PARTY)
	{
		CPtrList list;
		GetDocument()->m_infGame.GetGlobals(list);
		m_tabGlobals.SetGlobals(list);
	}
	else
		m_tabGlobals.DisableControls();

	m_nCurrentChar = 0;
	LoadChar(m_nCurrentChar);
}

void CDaleKeeperView::SetGameTitle()
{
	CString strTitle;
	CString strFile;

	if (m_nViewType == DKV_VIEWTYPE_CHR || m_nViewType == DKV_VIEWTYPE_CRE)
	{
		int nIndex = GetDocument()->m_strGameFile.ReverseFind('\\');
		if (nIndex != -1)
			strFile = GetDocument()->m_strGameFile.Mid(nIndex+1);
	}

	switch(m_nViewType)
	{
		case DKV_VIEWTYPE_CHR :
			GetParentFrame()->SetWindowText(CString("CHR - ")+GetDocument()->m_infChr.GetName()+CString(" - ")+strFile);
			break;
		case DKV_VIEWTYPE_PARTY :
			strTitle = GetDocument()->m_strGameTitle;
			GetParentFrame()->SetWindowText(strTitle);
			break;
		case DKV_VIEWTYPE_OOP :
			strTitle = GetDocument()->m_strGameTitle;
			strTitle += " - [Out of Party Characters]";
			GetParentFrame()->SetWindowText(strTitle);
			break;
		case DKV_VIEWTYPE_CRE :
			if (GetDocument()->m_infCre.GetLongNameID() == 0xFFFFFFFF)
				GetParentFrame()->SetWindowText("CRE - <No Name> - "+strFile);
			else
			{
				_infTlk.GetString(GetDocument()->m_infCre.GetLongNameID(),strTitle);
				if (strTitle.IsEmpty())
					strTitle = "CRE - <No Name> - "+strFile;
				GetParentFrame()->SetWindowText(CString("CRE - ")+strTitle+CString(" - ")+strFile);
			}
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperView diagnostics

#ifdef _DEBUG
void CDaleKeeperView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDaleKeeperView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDaleKeeperDoc* CDaleKeeperView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDaleKeeperDoc)));
	return (CDaleKeeperDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDaleKeeperView message handlers

void CDaleKeeperView::ResizeControls()
{
	CRect rName, rNewName, rNameRef, rNewNameRef;
	m_edName.GetWindowRect(&rName);
	ScreenToClient(&rName);
	m_btnNameRef.GetWindowRect(&rNameRef);
	ScreenToClient(&rNameRef);
	
	rNewName.left = rName.left;
	rNewName.top = rName.top;
	rNewName.right = rNewName.left + PORTRAIT_WIDTH + 2 - rNameRef.Width();
	rNewName.bottom = rNewName.top + rName.Height();
	m_edName.MoveWindow(&rNewName);

	rNewNameRef.left = rNewName.right;
	rNewNameRef.right = rNewNameRef.left + rNameRef.Width();
	rNewNameRef.top = rName.top;
	rNewNameRef.bottom = rName.bottom;
	m_btnNameRef.MoveWindow(&rNewNameRef);

	CRect rScroll, rNewScroll;
	m_sbParty.GetWindowRect(&rScroll);
	ScreenToClient(&rScroll);

	// -1 to the left is really incorrect, but it looks better. The left side of the bar
	// draws in the same color of the background which makes it look offset to the right
	// an extra space.
	rNewScroll.left = rNewName.left-1;
	rNewScroll.top = rNewName.bottom + PORTRAIT_HEIGHT + 2 + 3;
	rNewScroll.right = rNewNameRef.right;
	rNewScroll.bottom = rNewScroll.top + rScroll.Height() - 1;
	m_sbParty.MoveWindow(&rNewScroll);

	CRect rTab;
	m_tab.GetWindowRect(&rTab);
	ScreenToClient(&rTab);

	CRect rPortButton;
	rPortButton.left = rNewScroll.left;
	rPortButton.right = rNewScroll.right;
	rPortButton.top = rNewScroll.bottom+1;
	rPortButton.bottom = rTab.bottom;
	m_btnPortrait.MoveWindow(&rPortButton);

	m_rPortrait.left = rNewName.left;
	m_rPortrait.top = rNewName.bottom + 2;
	m_rPortrait.right = rNewScroll.right;
	m_rPortrait.bottom = rNewScroll.top - 1;

	m_wndPortrait.Create(m_rPortrait,this,1000);

	// Add the tabs to the control. I used the literal numbers here instead
	// of hte TAB_ defines so that it is obvious what order they are in. They
	// need to be inserted in this order to come out right.
	m_tab.InsertItem(0,"&Abilities");
	m_tab.InsertItem(1,"Characteristics");
	m_tab.InsertItem(2,"Appearance");
	m_tab.InsertItem(3,"&Inventory");
	m_tab.InsertItem(4,"Memorization");
	m_tab.InsertItem(5,"Innate");
	m_tab.InsertItem(6,"Wizard");
	m_tab.InsertItem(7,"Priest");
	m_tab.InsertItem(8,"Saving &Throws");
	m_tab.InsertItem(9,"&Proficiencies");
	m_tab.InsertItem(10,"Resistances");
	m_tab.InsertItem(11,"T&hieves");
	m_tab.InsertItem(12,"State Flags");
	m_tab.InsertItem(13,"Affects");
	m_tab.InsertItem(14,"Global Variables");

	// Create all of the modeless dialogs for the tabs.
	m_tabAbilities.Create(IDD_ABILITIES,&m_tab);
	m_pTabDlg[TAB_ABILITIES] = &m_tabAbilities;
	m_tabSaves.Create(IDD_SAVING_THROWS,&m_tab);
	m_pTabDlg[TAB_SAVES] = &m_tabSaves;
	m_tabProfs.Create(IDD_PROFICIENCIES,&m_tab);
	m_pTabDlg[TAB_PROFS] = &m_tabProfs;
	m_tabResistances.Create(IDD_RESISTANCES,&m_tab);
	m_pTabDlg[TAB_RESISTANCES] = &m_tabResistances;
	m_tabThief.Create(IDD_THIEF_SKILLS,&m_tab);
	m_pTabDlg[TAB_THIEF] = &m_tabThief;
	m_tabCharacteristics.Create(IDD_CHARACTERISTICS,&m_tab);
	m_pTabDlg[TAB_CHARACTERISTICS] = &m_tabCharacteristics;
	m_tabInv.Create(IDD_INVENTORY,&m_tab);
	m_tabInv.SetParentView(this);
	m_pTabDlg[TAB_INVENTORY] = &m_tabInv;
	m_tabInnate.SetInnate();
	m_tabInnate.Create(IDD_SPELLS,&m_tab);
	m_tabInnate.SetParentView(this);
	m_pTabDlg[TAB_SPELL_INNATE] = &m_tabInnate;
	m_tabPriest.Create(IDD_SPELLS,&m_tab);
	m_tabPriest.SetParentView(this);
	m_pTabDlg[TAB_SPELL_PRIEST] = &m_tabPriest;
	m_tabWizard.Create(IDD_SPELLS,&m_tab);
	m_tabWizard.SetParentView(this);
	m_pTabDlg[TAB_SPELL_WIZARD] = &m_tabWizard;
	m_tabMem.Create(IDD_MEMORIZATION,&m_tab);
	m_pTabDlg[TAB_SPELL_MEM] = &m_tabMem;
	m_tabAffects.Create(IDD_AFFECTS,&m_tab);
	m_pTabDlg[TAB_AFFECTS] = &m_tabAffects;
	m_tabGlobals.Create(IDD_GLOBALVARS,&m_tab);
	m_pTabDlg[TAB_GLOBALS] = &m_tabGlobals;
	m_tabAppearance.Create(IDD_APPEARANCE,&m_tab);
	m_pTabDlg[TAB_APPEARANCE] = &m_tabAppearance;
	m_tabStates.Create(IDD_STATEFLAGS,&m_tab);
	m_pTabDlg[TAB_STATE] = &m_tabStates;

	// Find the upper left position for the modeless dialogs.
	CRect rItem;
	m_tab.GetItemRect(0,&rItem);
	int nTop = rItem.Height() + 4;
	int nLeft = 2;

	// Reposition them all.
	CRect rNew;
	for (int i=0;i<NUM_VIEW_TABS;i++)
	{
		m_pTabDlg[i]->GetClientRect(&rTab);

		rNew.left = nLeft;
		rNew.top = nTop;
		rNew.right = rNew.left + rTab.Width()-1;
		rNew.bottom = rNew.top + rTab.Height()-1;

		m_pTabDlg[i]->MoveWindow(&rNew);
	}
	
	// Need to do this by hand the first time rather than through
	// DisplayTab().
	m_pTabDlg[TAB_ABILITIES]->ShowWindow(SW_SHOW);
	m_nCurrentTab = 0;
	m_tabAbilities.m_edStr.SetFocus();
}

BOOL CDaleKeeperView::OnEraseBkgnd(CDC* pDC) 
{
	// Erasing where the portrait is looks like crap. It flashes as it erases and the
	// portrait redraws. Excluding the portrait rectangle from the redraw region prevents
	// the flashing.
	pDC->ExcludeClipRect(&m_rPortrait);

	return(CFormView::OnEraseBkgnd(pDC));
}

void CDaleKeeperView::GetPortrait(int nChar)
{
	if (m_hPortrait)
	{
		::DeleteObject(m_hPortrait);
		m_hPortrait = NULL;
	}

	CString strResName;
	switch(m_nViewType)
	{
		case DKV_VIEWTYPE_PARTY :
			GetDocument()->m_infGame.GetPartyCre(nChar)->GetResLargePortrait(strResName);
			break;
		case DKV_VIEWTYPE_OOP :
			GetDocument()->m_infGame.GetOutOfPartyCre(nChar)->GetResLargePortrait(strResName);
			break;
		case DKV_VIEWTYPE_CHR :
			GetDocument()->m_infChr.GetCre()->GetResLargePortrait(strResName);
			break;
		case DKV_VIEWTYPE_CRE :
			GetDocument()->m_infCre.GetResLargePortrait(strResName);
			break;
	}

	if (GetResourceBitmap(strResName,m_hPortrait))
		m_wndPortrait.SetBitmap(m_hPortrait,BWS_INSET);
	else
		m_wndPortrait.SetBitmap(NULL);
}

void CDaleKeeperView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (m_nViewType == DKV_VIEWTYPE_CHR || m_nViewType == DKV_VIEWTYPE_CRE)
		return;

	switch(nSBCode)
	{
		case SB_LEFT :
		case SB_LINELEFT :
		case SB_PAGELEFT :
			OnCharacterPrev();
			break;
		case SB_RIGHT :
		case SB_LINERIGHT :
		case SB_PAGERIGHT :
			OnCharacterNext();
			break;
		case SB_THUMBTRACK :
			if (nPos != (UINT)m_nCurrentChar && nPos >=0 && nPos < (UINT)(m_nViewType==DKV_VIEWTYPE_PARTY ? GetDocument()->m_infGame.GetPartyCount() :GetDocument()->m_infGame.GetOutOfPartyCount()))
			{
				if (SaveChar(m_nCurrentChar))
				{
					m_nCurrentChar = nPos;
					LoadChar(m_nCurrentChar);
					m_sbParty.SetScrollPos(m_nCurrentChar);
				}
			}
			break;
	}
}

void CDaleKeeperView::GetNameText(int nChar)
{
	// NPC names cannot be changed without modifing the Dialog.tlk
	// file. From now on I'm going to look up the proper name for
	// NPCs and grey the box out.
	if (m_dwLongName == 0xFFFFFFFF)
	{
		switch(m_nViewType)
		{
			case DKV_VIEWTYPE_PARTY :
				GetDocument()->m_infGame.GetPartyCharName(nChar,m_strName);
				m_edName.EnableWindow(TRUE);
				break;
			case DKV_VIEWTYPE_OOP :
				GetDocument()->m_infGame.GetOutOfPartyCharName(nChar,m_strName);
				m_edName.EnableWindow(TRUE);
				break;
			case DKV_VIEWTYPE_CHR :
				m_strName = GetDocument()->m_infChr.GetName();
				m_edName.EnableWindow(TRUE);
				break;
			case DKV_VIEWTYPE_CRE :
				m_strName = "<No Name>";
				m_edName.EnableWindow(FALSE);
				break;
		}
	}
	else
	{
		_infTlk.GetString(m_dwLongName,m_strName);
		m_edName.EnableWindow(FALSE);
	}
}

CInfCreature* CDaleKeeperView::GetCre(int nChar)
{
	switch(m_nViewType)
	{
		case DKV_VIEWTYPE_PARTY :
			return(GetDocument()->m_infGame.GetPartyCre(nChar));
		case DKV_VIEWTYPE_OOP :
			return(GetDocument()->m_infGame.GetOutOfPartyCre(nChar));
		case DKV_VIEWTYPE_CHR :
			return(GetDocument()->m_infChr.GetCre());
		case DKV_VIEWTYPE_CRE :
			return(&GetDocument()->m_infCre);
	}

	return(NULL);
}

void CDaleKeeperView::LoadChar(int nChar)
{
	int i;

	GetPortrait(nChar);

	CInfGame *pGame = &GetDocument()->m_infGame;
	CInfCreature *pCre = GetCre(nChar);

	m_dwShortName = pCre->GetShortNameID();
	m_dwLongName = pCre->GetLongNameID();
	GetNameText(nChar);
	
	m_tabAbilities.m_nStr = pCre->GetStr();
	m_tabAbilities.m_nStrBonus = pCre->GetStrBonus();
	m_tabAbilities.m_nDex = pCre->GetDex();
	m_tabAbilities.m_nCon = pCre->GetCon();
	m_tabAbilities.m_nInt = pCre->GetInt();
	m_tabAbilities.m_nWis = pCre->GetWis();
	m_tabAbilities.m_nCha = pCre->GetCha();
	m_tabAbilities.m_nExp = pCre->GetExp();
	m_tabAbilities.m_nExpForKill = pCre->GetExpForKill();
	m_tabAbilities.m_nHP = pCre->GetBaseHP(); 
	m_tabAbilities.m_nCurrentHP = pCre->GetCurrentHP();
	switch(m_nViewType)
	{
		case DKV_VIEWTYPE_PARTY :
		case DKV_VIEWTYPE_OOP :
			m_tabAbilities.m_nGold = pGame->GetPartyGold();
			m_tabAbilities.m_nReputation = pGame->GetPartyReputation();
			break;
		case DKV_VIEWTYPE_CHR :
		case DKV_VIEWTYPE_CRE :
			m_tabAbilities.m_nGold = pCre->GetGold();
			m_tabAbilities.m_nReputation = pCre->GetReputation();
			break;
	}
	m_tabAbilities.SetAttacks(pCre->GetAttacks());
	m_tabAbilities.m_nAC = pCre->GetAC();
	m_tabAbilities.m_nThac0 = pCre->GetThac0();
	m_tabAbilities.m_nLevelFirstClass = pCre->GetFirstClassLevel();
	m_tabAbilities.m_nLevelSecondClass = pCre->GetSecondClassLevel();
	m_tabAbilities.m_nLevelThirdClass = pCre->GetThirdClassLevel();

	CObList list;
	pCre->GetProfs(list);
	m_tabProfs.SetProfs(list);

	CPtrList listPtr;
	pCre->GetAffects(listPtr);
	m_tabAffects.SetAffects(listPtr);

	m_tabSaves.m_nBreath = pCre->GetSaveBreath();
	m_tabSaves.m_nParalyzation = pCre->GetSaveDeath();
	m_tabSaves.m_nPolymorph = pCre->GetSavePoly();
	m_tabSaves.m_nSpells = pCre->GetSaveSpells();
	m_tabSaves.m_nWands = pCre->GetSaveWands();

	m_tabResistances.m_nAcid = pCre->GetResAcid();
	m_tabResistances.m_nCold = pCre->GetResCold();
	m_tabResistances.m_nCrushing = pCre->GetResCrushing();
	m_tabResistances.m_nElectricity = pCre->GetResElectricity();
	m_tabResistances.m_nFire = pCre->GetResFire();
	m_tabResistances.m_nMagic = pCre->GetResMagic();
	m_tabResistances.m_nMagicCold = pCre->GetResMagicCold();
	m_tabResistances.m_nMagicFire = pCre->GetResMagicFire();
	m_tabResistances.m_nMissile = pCre->GetResMissile();
	m_tabResistances.m_nPiercing = pCre->GetResPiercing();
	m_tabResistances.m_nSlashing = pCre->GetResSlashing();

	m_tabThief.m_nOpenLocks = pCre->GetOpenLocks();
	m_tabThief.m_nMoveSilently = pCre->GetMoveSilently();
	m_tabThief.m_nFindTraps = pCre->GetFindTraps();
	m_tabThief.m_nPickPockets = pCre->GetPickPockets();
	m_tabThief.m_nSetTraps = pCre->GetSetTraps();
	m_tabThief.m_nDetectIllusions = pCre->GetDetectIllusions();
	m_tabThief.m_nHideInShadows = pCre->GetHideInShadows();

	m_tabCharacteristics.SetClass(pCre->GetClass());
	m_tabCharacteristics.SetGender(pCre->GetGender());
	m_tabCharacteristics.SetRace(pCre->GetRace());
	m_tabCharacteristics.SetAlignment(pCre->GetAlignment());
	m_tabCharacteristics.SetKit(pCre->GetKit());
	m_tabCharacteristics.SetRacialEnemy(pCre->GetRacialEnemy());
	m_tabCharacteristics.m_nMovementSpeed = pCre->GetSpeed();
	m_tabCharacteristics.SetEnemyAlly(pCre->GetEnemyAlly());

	m_tabAppearance.SetAppearance(pCre->GetAnimationID());
	m_tabAppearance.SetHairColor(pCre->GetHairColor());
	m_tabAppearance.SetSkinColor(pCre->GetSkinColor());
	m_tabAppearance.SetMajorColor(pCre->GetMajorColor());
	m_tabAppearance.SetMinorColor(pCre->GetMinorColor());
	m_tabAppearance.SetArmorColor(pCre->GetArmorColor());
	m_tabAppearance.SetLeatherColor(pCre->GetLeatherColor());
	m_tabAppearance.SetMetalColor(pCre->GetMetalColor());

	m_tabStates.SetFlags(pCre->GetStateFlags());

	INF_CRE_ITEM creItems[INF_NUM_ITEMSLOTS];
	pCre->GetItems(&creItems[0]);

	m_tabInv.m_strCharName = m_strName;
	m_tabInv.m_strGameName = GetDocument()->m_strGameTitle;
	m_tabInv.SetItems(&creItems[0]);

	m_tabInnate.m_strCharName = m_strName;
	m_tabInnate.m_strGameName = GetDocument()->m_strGameTitle;
	m_tabPriest.m_strCharName = m_strName;
	m_tabPriest.m_strGameName = GetDocument()->m_strGameTitle;
	m_tabWizard.m_strCharName = m_strName;
	m_tabWizard.m_strGameName = GetDocument()->m_strGameTitle;

	int nMemInfoCount = pCre->GetMemorizationInfoCount();
	MEMINFO *pMemInfo = NULL;
	if (nMemInfoCount)
	{
		pMemInfo = new MEMINFO[nMemInfoCount];
		pCre->GetMemorizationInfo(pMemInfo);
	}
	m_tabMem.SetData(pMemInfo,nMemInfoCount);
	delete [] pMemInfo;

	int nSpellCount;
	CTabSpellsDlg *pSpellTab;
	SPELLDATA *pSpells;

	for (i=0;i<3;i++)
	{
		switch(i)
		{
			case INF_CRE_ST_INNATE :
				pSpellTab = &m_tabInnate;
				break;
			case INF_CRE_ST_PRIEST :
				pSpellTab = &m_tabPriest;
				break;
			case INF_CRE_ST_WIZARD :
				pSpellTab = &m_tabWizard;
				break;
			default :
				pSpellTab = NULL;
				break;
		}

		if (pSpellTab)
		{
			nSpellCount = pCre->GetSpellCount(i);
			if (!nSpellCount)
				pSpellTab->SetSpells(i,nSpellCount,NULL);
			else
			{
				pSpells = new SPELLDATA[nSpellCount];
				pCre->GetSpells(i,pSpells);
				pSpellTab->SetSpells(i,nSpellCount,pSpells);
				delete pSpells;
				pSpells = NULL;
			}
		}
	}

	for (i=0;i<NUM_VIEW_TABS;i++)
		m_pTabDlg[i]->UpdateData(FALSE);

	UpdateData(FALSE);
}

BOOL CDaleKeeperView::SaveChar(int nChar)
{
	for (int i=0;i<NUM_VIEW_TABS;i++)
		if (!m_pTabDlg[i]->UpdateData(TRUE))
		{
			DisplayTab(i);
			return(FALSE);
		}

	CInfGame *pGame = &GetDocument()->m_infGame;
	CInfCreature *pCre = GetCre(nChar);

	pCre->SetStr(m_tabAbilities.m_nStr);
	pCre->SetStrBonus(m_tabAbilities.m_nStrBonus);
	pCre->SetDex(m_tabAbilities.m_nDex);
	pCre->SetCon(m_tabAbilities.m_nCon);
	pCre->SetInt(m_tabAbilities.m_nInt);
	pCre->SetWis(m_tabAbilities.m_nWis);
	pCre->SetCha(m_tabAbilities.m_nCha);
	pCre->SetExp(m_tabAbilities.m_nExp);
	pCre->SetExpForKill(m_tabAbilities.m_nExpForKill);
	switch(m_nViewType)
	{
		case DKV_VIEWTYPE_PARTY :
		case DKV_VIEWTYPE_OOP :
			pGame->SetPartyGold(m_tabAbilities.m_nGold);
			pGame->SetPartyReputation(m_tabAbilities.m_nReputation);
			break;
		case DKV_VIEWTYPE_CHR :
		case DKV_VIEWTYPE_CRE :
			pCre->SetGold(m_tabAbilities.m_nGold);
			pCre->SetReputation(m_tabAbilities.m_nReputation);
			break;
	}
	pCre->SetBaseHP(m_tabAbilities.m_nHP);
	pCre->SetCurrentHP(m_tabAbilities.m_nCurrentHP);
	pCre->SetAttacks(m_tabAbilities.GetAttacks());
	pCre->SetAC(m_tabAbilities.m_nAC);
	pCre->SetThac0(m_tabAbilities.m_nThac0);
	pCre->SetFirstClassLevel(m_tabAbilities.m_nLevelFirstClass);
	pCre->SetSecondClassLevel(m_tabAbilities.m_nLevelSecondClass);
	pCre->SetThirdClassLevel(m_tabAbilities.m_nLevelThirdClass);

	CObList list;
	m_tabProfs.GetProfs(list);
	pCre->SetProfs(list);

	CPtrList listPtr;
	m_tabAffects.GetAffects(listPtr);
	pCre->SetAffects(listPtr);

	pCre->SetSaveBreath(m_tabSaves.m_nBreath);
	pCre->SetSaveDeath(m_tabSaves.m_nParalyzation);
	pCre->SetSavePoly(m_tabSaves.m_nPolymorph);
	pCre->SetSaveSpells(m_tabSaves.m_nSpells);
	pCre->SetSaveWands(m_tabSaves.m_nWands);

	pCre->SetResAcid(m_tabResistances.m_nAcid);
	pCre->SetResCold(m_tabResistances.m_nCold);
	pCre->SetResCrushing(m_tabResistances.m_nCrushing);
	pCre->SetResElectricity(m_tabResistances.m_nElectricity);
	pCre->SetResFire(m_tabResistances.m_nFire);
	pCre->SetResMagic(m_tabResistances.m_nMagic);
	pCre->SetResMagicCold(m_tabResistances.m_nMagicCold);
	pCre->SetResMagicFire(m_tabResistances.m_nMagicFire);
	pCre->SetResMissile(m_tabResistances.m_nMissile);
	pCre->SetResPiercing(m_tabResistances.m_nPiercing);
	pCre->SetResSlashing(m_tabResistances.m_nSlashing);

	pCre->SetOpenLocks(m_tabThief.m_nOpenLocks);
	pCre->SetMoveSilently(m_tabThief.m_nMoveSilently);
	pCre->SetFindTraps(m_tabThief.m_nFindTraps);
	pCre->SetPickPockets(m_tabThief.m_nPickPockets);
	pCre->SetSetTraps(m_tabThief.m_nSetTraps);
	pCre->SetDetectIllusions(m_tabThief.m_nDetectIllusions);
	pCre->SetHideInShadows(m_tabThief.m_nHideInShadows);

	pCre->SetClass(m_tabCharacteristics.GetClass());
	pCre->SetGender(m_tabCharacteristics.GetGender());
	pCre->SetRace(m_tabCharacteristics.GetRace());
	pCre->SetAlignment(m_tabCharacteristics.GetAlignment());
	pCre->SetKit(m_tabCharacteristics.GetKit());
	pCre->SetRacialEnemy(m_tabCharacteristics.GetRacialEnemy());
	pCre->SetSpeed(m_tabCharacteristics.m_nMovementSpeed);
	pCre->SetEnemyAlly(m_tabCharacteristics.GetEnemyAlly());

	pCre->SetAnimationID(m_tabAppearance.GetAppearance());
	pCre->SetSkinColor(m_tabAppearance.m_chSkinColor);
	pCre->SetHairColor(m_tabAppearance.m_chHairColor);
	pCre->SetMajorColor(m_tabAppearance.m_chMajorColor);
	pCre->SetMinorColor(m_tabAppearance.m_chMinorColor);
	pCre->SetArmorColor(m_tabAppearance.m_chArmorColor);
	pCre->SetLeatherColor(m_tabAppearance.m_chLeatherColor);
	pCre->SetMetalColor(m_tabAppearance.m_chMetalColor);

	pCre->SetStateFlags(m_tabStates.GetFlags());

	INF_CRE_ITEM creItems[INF_NUM_ITEMSLOTS];
	m_tabInv.GetItems(&creItems[0]);
	pCre->SetItems(&creItems[0]);

	// You can't add items to the list so I'm just pulling the same number from
	// the CRE.
	int nMemInfoCount = pCre->GetMemorizationInfoCount();
	MEMINFO *pMemInfo = NULL;
	if (nMemInfoCount)
	{
		pMemInfo = new MEMINFO[nMemInfoCount];
		m_tabMem.GetData(pMemInfo,nMemInfoCount);
		pCre->SetMemorizationInfo(pMemInfo);
		delete [] pMemInfo;
	}

	int nSpellCount;
	CTabSpellsDlg *pSpellTab;
	SPELLDATA *pSpells;

	for (i=0;i<3;i++)
	{
		switch(i)
		{
			case INF_CRE_ST_INNATE :
				pSpellTab = &m_tabInnate;
				break;
			case INF_CRE_ST_PRIEST :
				pSpellTab = &m_tabPriest;
				break;
			case INF_CRE_ST_WIZARD :
				pSpellTab = &m_tabWizard;
				break;
			default :
				pSpellTab = NULL;
				break;
		}

		if (pSpellTab)
		{
			nSpellCount = pSpellTab->GetSpellCount();
			if (!nSpellCount)
				pCre->SetSpells(i,nSpellCount,NULL);
			else
			{
				pSpells = new SPELLDATA[nSpellCount];
				pSpellTab->GetSpells(pSpells);
				pCre->SetSpells(i,nSpellCount,pSpells);
				delete pSpells;
				pSpells = NULL;
			}
		}
	}

	UpdateData(TRUE);

	pCre->SetShortNameID(m_dwShortName);
	pCre->SetLongNameID(m_dwLongName);
	if (m_dwLongName == 0xFFFFFFFF)
		switch(m_nViewType)
		{
			case DKV_VIEWTYPE_PARTY :
				pGame->SetPartyCharName(nChar,m_strName);
				break;
			case DKV_VIEWTYPE_OOP :
				pGame->SetOutOfPartyCharName(nChar,m_strName);
				break;
			case DKV_VIEWTYPE_CHR :
				GetDocument()->m_infChr.SetName(m_strName);
				break;
		}

	return(TRUE);
}

void CDaleKeeperView::OnCharacterNext() 
{
	if (m_nViewType == DKV_VIEWTYPE_CHR || m_nViewType == DKV_VIEWTYPE_CRE)
		return;

	if (m_nCurrentChar < (m_nViewType == DKV_VIEWTYPE_PARTY ? GetDocument()->m_infGame.GetPartyCount()-1 : GetDocument()->m_infGame.GetOutOfPartyCount()-1))
	{
		if (SaveChar(m_nCurrentChar))
		{
			m_nCurrentChar++;
			LoadChar(m_nCurrentChar);
			m_sbParty.SetScrollPos(m_nCurrentChar);
		}
	}
}

void CDaleKeeperView::OnCharacterPrev() 
{
	if (m_nViewType == DKV_VIEWTYPE_CHR || m_nViewType == DKV_VIEWTYPE_CRE)
		return;

	if (m_nCurrentChar > 0)
	{
		if (SaveChar(m_nCurrentChar))
		{
			m_nCurrentChar--;
			LoadChar(m_nCurrentChar);
			m_sbParty.SetScrollPos(m_nCurrentChar);
		}
	}
}

void CDaleKeeperView::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nCurSel = m_tab.GetCurSel();
	if (nCurSel != m_nCurrentTab)
	{
		m_pTabDlg[m_nCurrentTab]->ShowWindow(SW_HIDE);
		m_pTabDlg[nCurSel]->ShowWindow(SW_SHOW);
		m_nCurrentTab = nCurSel;
	}
	
	*pResult = 0;
}

void CDaleKeeperView::DisplayTab(int nIndex)
{
	// Make sure it's not out of range and that the tab isn't already the
	// current tab.
	if (nIndex < 0 || nIndex >= NUM_VIEW_TABS || m_nCurrentTab == nIndex)
		return;

	m_pTabDlg[m_nCurrentTab]->ShowWindow(SW_HIDE);
	m_tab.SetCurSel(nIndex);
	m_pTabDlg[nIndex]->ShowWindow(SW_SHOW);
	m_nCurrentTab = nIndex;

	// Make sure the focus is visible when a tab is switched. This is probably
	// not ALWAYS what it needs to do, but most of the time if you are switching
	// tabs you'll want the focus with it. The problem is that the focus can 
	// remain behind on a tab that is no longer visible.
	switch(m_nCurrentTab)
	{
		case TAB_ABILITIES :
			m_tabAbilities.m_edStr.SetFocus();
			break;
		case TAB_SAVES :
			m_tabSaves.m_edParalyzation.SetFocus();
			break;
		case TAB_PROFS :
			m_tabProfs.m_lcProfs.SetFocus();
			break;
		case TAB_RESISTANCES :
			m_tabResistances.m_edAcid.SetFocus();
			break;
		case TAB_THIEF :
			m_tabThief.m_edMoveSilently.SetFocus();
			break;
		case TAB_CHARACTERISTICS :
			m_tabCharacteristics.m_cbGender.SetFocus();
			break;
		case TAB_INVENTORY :
			m_tabInv.m_lcInv.SetFocus();
			break;
		case TAB_SPELL_INNATE :
			m_tabInnate.m_lcSpells.SetFocus();
			break;
		case TAB_SPELL_PRIEST :
			m_tabPriest.m_lcSpells.SetFocus();
			break;
		case TAB_SPELL_WIZARD :
			m_tabWizard.m_lcSpells.SetFocus();
			break;
		case TAB_SPELL_MEM :
			m_tabMem.m_lcList.SetFocus();
			break;
		case TAB_AFFECTS :
			m_tabAffects.m_lcList.SetFocus();
			break;
		case TAB_GLOBALS :
			m_tabGlobals.m_lcList.SetFocus();
			break;
		case TAB_APPEARANCE :
			m_tabAppearance.m_cbAppearance.SetFocus();
			break;
		case TAB_STATE :
			m_tabStates.m_lcList.SetFocus();
			break;
	}
}

void CDaleKeeperView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CHintItem *pHi;
	CHintSpell *pHs;
	CPtrList list;

	switch(lHint)
	{
		case HINT_SAVEDATA :
			if (m_nViewType == DKV_VIEWTYPE_PARTY)
			{
				m_tabGlobals.GetGlobals(list);
				GetDocument()->m_infGame.SetGlobals(list);
			}
			SaveChar(m_nCurrentChar);
			break;
		case HINT_FILESAVED :
			SetGameTitle();
			LoadChar(m_nCurrentChar);
			break;
		case HINT_IBC_ASSIGNITEM :
			pHi = (CHintItem*)pHint;
			if (pHi->pView == this)
				m_tabInv.AssignItem(pHi);
			break;
		case HINT_SBC_ASSIGNSPELL :
			pHs = (CHintSpell*)pHint;
			if (pHs->pView == this)
				switch(pHs->wSpellType)
				{
					case INF_CRE_ST_INNATE :
						DisplayTab(TAB_SPELL_INNATE);
						m_tabInnate.AssignSpell(pHs);
						break;
					case INF_CRE_ST_PRIEST :
						DisplayTab(TAB_SPELL_PRIEST);
						m_tabPriest.AssignSpell(pHs);
						break;
					case INF_CRE_ST_WIZARD :
						DisplayTab(TAB_SPELL_WIZARD);
						m_tabWizard.AssignSpell(pHs);
						break;
				}
			break;
		case HINT_CHANGE_GRIDLINES :
			UpdateGridLines();
			break;
	}
}

void CDaleKeeperView::OnUpdatePageNext(CCmdUI* pCmdUI) 
{
	if (m_nCurrentTab < NUM_VIEW_TABS-1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CDaleKeeperView::OnUpdatePagePrev(CCmdUI* pCmdUI) 
{
	if (m_nCurrentTab > 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CDaleKeeperView::OnPageNext() 
{
	DisplayTab(m_nCurrentTab+1);
}

void CDaleKeeperView::OnPagePrev() 
{
	DisplayTab(m_nCurrentTab-1);
}

void CDaleKeeperView::OnPageAbilities() 
{
	DisplayTab(TAB_ABILITIES);
}

void CDaleKeeperView::OnPageSaves() 
{
	DisplayTab(TAB_SAVES);
}

void CDaleKeeperView::OnPageProf() 
{
	DisplayTab(TAB_PROFS);	
}

void CDaleKeeperView::OnPageResistances() 
{
	DisplayTab(TAB_RESISTANCES);
}

void CDaleKeeperView::OnPageThief() 
{
	DisplayTab(TAB_THIEF);
}

void CDaleKeeperView::OnPageCharacteristics() 
{
	DisplayTab(TAB_CHARACTERISTICS);
}

void CDaleKeeperView::OnViewInventory() 
{
	DisplayTab(TAB_INVENTORY);
}

void CDaleKeeperView::OnPortrait() 
{
	CPortraitsDlg d;

	CInfGame *pGame = &GetDocument()->m_infGame;
	CInfCreature *pCre;
	switch(m_nViewType)
	{
		case DKV_VIEWTYPE_PARTY :
			pCre = pGame->GetPartyCre(m_nCurrentChar);
			break;
		case DKV_VIEWTYPE_OOP :
			pCre = pGame->GetOutOfPartyCre(m_nCurrentChar);
			break;
		case DKV_VIEWTYPE_CHR :
			pCre = GetDocument()->m_infChr.GetCre();
			break;
		case DKV_VIEWTYPE_CRE :
			pCre = &GetDocument()->m_infCre;
			break;
	}

	pCre->GetSmallPortrait(d.m_strSmall);
	pCre->GetLargePortrait(d.m_strLarge);
	if (d.DoModal() == IDOK)
	{
		pCre->SetSmallPortrait(d.m_strSmall);
		pCre->SetLargePortrait(d.m_strLarge);
		GetPortrait(m_nCurrentChar);
	}
}

void CDaleKeeperView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		_comSpellBrowser.SetSelected(GetDocument()->m_strGameTitle,m_strName,this);
		_comItemBrowser.SetSelected(GetDocument()->m_strGameTitle,m_strName,m_tabInv.GetSelectedItem(),this);
	}
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CDaleKeeperView::UpdateGridLines()
{
	UpdateGridLines(m_tabInv.m_lcInv);
	UpdateGridLines(m_tabMem.m_lcList);
	UpdateGridLines(m_tabInnate.m_lcSpells);
	UpdateGridLines(m_tabPriest.m_lcSpells);
	UpdateGridLines(m_tabWizard.m_lcSpells);
	UpdateGridLines(m_tabProfs.m_lcProfs);
	UpdateGridLines(m_tabStates.m_lcList);
	UpdateGridLines(m_tabGlobals.m_lcList);
	UpdateGridLines(m_tabAffects.m_lcList);
}

void CDaleKeeperView::UpdateGridLines(CListCtrl &list)
{
	DWORD dwStyle = list.GetExtendedStyle();

	if (!_bUseGridLines)
		dwStyle &= (~LVS_EX_GRIDLINES);
	else
		dwStyle |= LVS_EX_GRIDLINES;

	list.SetExtendedStyle(dwStyle);
}

void CDaleKeeperView::OnNameRef() 
{
	CNameRefDlg d;

	d.m_dwShort = m_dwShortName;
	d.m_dwLong = m_dwLongName;
	if (d.DoModal() == IDOK)
	{
		m_dwShortName = d.m_dwShort;
		m_dwLongName = d.m_dwLong;

		GetNameText(m_nCurrentChar);
		UpdateData(FALSE);
	}
}

void CDaleKeeperView::OnUpdateToolsConverttocre(CCmdUI* pCmdUI) 
{
	if (m_nViewType == DKV_VIEWTYPE_CRE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CDaleKeeperView::OnToolsConverttocre() 
{
	CInfCreature *pCre = GetCre(m_nCurrentChar);	

	CString strSavedAs;
	CString strFile(_strInstallPath+DIR_OVERRIDE);
	strFile += m_strName + ".cre";

	if (SaveCreFile(pCre,strFile,strSavedAs))
	{
		CString strMessage;
		strMessage.Format("%s has been converted to a CRE file.",(const char *)m_strName);
		MessageBox(strMessage,"Conversion to CRE",MB_ICONINFORMATION);
	}

}
