// MainFrm.cpp : implementation of the CMainFrame class
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
#include "IndexResourcesDlg.h"
#include "InfTlk.h"
#include "InfKey.h"
#include "InfBam.h"
#include "PalImageList.h"
#include "MainFrm.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSG_DORESOURCES,OnDoResources)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_hbmpBackground = NULL;	
}

CMainFrame::~CMainFrame()
{
	if (m_hbmpBackground)
		::DeleteObject(m_hbmpBackground);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
/*	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}*/

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) /*||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndClient.SubclassWindow(m_hWndMDIClient);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= (~FWS_ADDTOTITLE);

	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

#include "InfCreature.h"

LRESULT CMainFrame::OnDoResources(WPARAM wParam, LPARAM lParam)
{
	CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();
	CString strTitle;
	strTitle.Format("Shadow Keeper - Version %s",(const char *)pApp->m_strFileVersion);
	SetWindowText(strTitle);

	if (!_infTlk.Open(_strInstallPath+"Dialog.tlk"))
	{
		MessageBox("Shadow Keeper was unable to read the text resources. Shadow Keeper must "
			"be able to find this file to work properly. Make sure the Installation Path has "
			"been set to the correct directory and restart Shadow Keeper.","Can't Find Dialog.tlk",
			MB_ICONEXCLAMATION);
		return(0);
	}

	CIndexResourcesDlg d;
	if (d.DoModal() != IDOK)
	{
		MessageBox("Shadow Keeper was unable to read the Shadows of Amn resources. Shadow Keeper must "
			"be able to find these files to work properly. Make sure the Installation Path has "
			"been set to the correct directory and restart Shadow Keeper.","Can't Find Chitin.key",
			MB_ICONEXCLAMATION);
		return(0);
	}

	HBITMAP hPal;
	if (GetResourceBitmap("RANGES12",hPal))
	{
		_ilPal.CreateFromBitmap(hPal);
		::DeleteObject(hPal);
	}

	_bResourcesLoaded = TRUE;

/*ResInfo *pBamResInfo = _infKey.GetResInfo(RESTYPE_BAM,"LOADCNTR");
	if (pBamResInfo)
	{
		CInfBifFile *pBamFile = _infKey.GetBifFile(pBamResInfo);
		if (pBamFile && pBamFile->IsValid())
		{
			CInfBam bam;
			CRect rClient;
			GetClientRect(&rClient);

			if (pBamFile->GetBam(pBamResInfo,bam))
			{
				bam.MakeBitmap(0,::GetSysColor(COLOR_APPWORKSPACE),m_hbmpBackground);
				m_wndClient.SetBitmap(m_hbmpBackground);
			}
		}
	} */

/*	CResInfo *pCreInfo = _infKey.GetResInfo(RESTYPE_CRE,"JON1");
	if (pCreInfo)
	{
		CInfBifFile *pBif = _infKey.GetBifFile(pCreInfo);
		if (pBif && pBif->IsValid())
		{
			CInfCreature cre;

			int nDataOffset, nDataSize;
			pBif->GetDataOffsetAndSize(pCreInfo,nDataOffset,nDataSize);
			pBif->Seek(nDataOffset,CFile::begin);

			cre.Read(*pBif,nDataSize);

		}
	} */

	return(0);
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x = 487;
	lpMMI->ptMinTrackSize.y = 347;
	
	CMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
}
