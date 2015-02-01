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

#include "StdAfx.h"
#include "ItemBrowserCom.h"
#include "Global.h"
#include "DaleKeeper.h"

CItemBrowserCom::CItemBrowserCom()
{
	m_nPos = 0;
	m_pCurrentView = NULL;
}

CItemBrowserCom::~CItemBrowserCom()
{

}

CDocument* CItemBrowserCom::GetBrowserDoc()
{
	CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();

	POSITION pos = pApp->GetItemBrowserTemplate()->GetFirstDocPosition();
	if (!pos)
		return(NULL);

	CDocument *pDoc = pApp->GetItemBrowserTemplate()->GetNextDoc(pos);

	return(pDoc);
}

void CItemBrowserCom::SetSelected(const char *pszGameName, const char *pszCharName, int nPos, CView *pView)
{
	m_strGameName = pszGameName;
	m_strCharName = pszCharName;
	m_nPos = nPos;
	m_pCurrentView = pView;

	// Need to notify the browser window of the change. If it isn't already open, just leave
	// it closed, this isn't important.
	CDocument *pBrowserDoc = GetBrowserDoc();
	if (!pBrowserDoc)
		return;

	pBrowserDoc->UpdateAllViews(NULL,HINT_IBC_CHANGESELECTED);
}

void CItemBrowserCom::ClearSelected(CView *pView)
{
	if (pView != m_pCurrentView)
		return;

	m_pCurrentView = NULL;

	CDocument *pBrowserDoc = GetBrowserDoc();
	if (!pBrowserDoc)
		return;

	pBrowserDoc->UpdateAllViews(NULL,HINT_IBC_CHANGESELECTED);
}

void CItemBrowserCom::DisplayItem(const char *pszResource)
{
	CDocument *pBrowserDoc = GetBrowserDoc();
	if (!pBrowserDoc)
	{
		CDaleKeeperApp *pApp = (CDaleKeeperApp*)AfxGetApp();
		pApp->OnViewItembrowser();
		pBrowserDoc = GetBrowserDoc();

		// If we still don't have it, there is some problem.
		if (!pBrowserDoc)
			return;
	}
	ASSERT(pBrowserDoc);

	CHintString hsRes(pszResource);
	pBrowserDoc->UpdateAllViews(NULL,HINT_IBC_DISPLAYITEM,&hsRes);
}

void CItemBrowserCom::AssignItem(const char *pszName, CResInfo &resInfo, INF_ITM *pItm)
{
	if (!m_pCurrentView)
		return;

	CHintItem hi;

	hi.strName = pszName;
	hi.nPos = m_nPos;
	hi.pView = m_pCurrentView;

	hi.resInfo.strName = resInfo.strName;
	hi.resInfo.wBifKeyIndex = resInfo.wBifKeyIndex;
	hi.resInfo.chTileIndex = resInfo.chTileIndex;
	hi.resInfo.wLocator = resInfo.wLocator;

	memcpy(&hi.itm,pItm,sizeof(INF_ITM));

	m_pCurrentView->GetDocument()->UpdateAllViews(NULL,HINT_IBC_ASSIGNITEM,&hi);
}

