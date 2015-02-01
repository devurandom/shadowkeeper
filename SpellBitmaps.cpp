
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
#include "SpellBitmaps.h"
#include "InfKey.h"
#include "InfTlk.h"
#include "Global.h"

CSpellBitmaps::CSpellBitmaps()
{
	m_nEmptyIndex = 0;
}

CSpellBitmaps::~CSpellBitmaps()
{
	CString strKey;
	CSpellBitmapInfo *pInfo;
	POSITION pos = m_mapInfo.GetStartPosition();
	while(pos)
	{
		m_mapInfo.GetNextAssoc(pos,strKey,(CObject*&)pInfo);
		delete pInfo;
	}
}

BOOL CSpellBitmaps::LoadImages(BOOL bBlankOnly, CProgressCtrl *pCtrl)
{
	if (!m_ilSpells.Create(32,32,ILC_COLOR32,270,30))
		return(FALSE);

	CObList *pSpells;
	if (!_infKey.GetResList(RESTYPE_SPL,pSpells))
		return(FALSE);

	CDC *pDC = AfxGetMainWnd()->GetDC();
	BITMAPINFOHEADER bih;
	memset(&bih,0,sizeof(BITMAPINFOHEADER));

	BYTE chBits[32*32*sizeof(COLORREF)];
	COLORREF clrWhite = RGB(255,255,255);
	for (int i=0;i<32*32;i++)
		memcpy(chBits+i*sizeof(COLORREF),&clrWhite,sizeof(COLORREF));

	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = 32;
	bih.biHeight = 32;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;

	HBITMAP hNoIcon = ::CreateDIBitmap(
		pDC->GetSafeHdc(),
		&bih,
		CBM_INIT,
		chBits,
		(BITMAPINFO*)&bih,
		DIB_RGB_COLORS);

	AfxGetMainWnd()->ReleaseDC(pDC);

	m_nEmptyIndex = m_ilSpells.Add(CBitmap::FromHandle(hNoIcon),RGB(0,0,0));
	::DeleteObject(hNoIcon);

	if (bBlankOnly)
	{
		pCtrl->SetRange(0,1);
		pCtrl->SetPos(1);
		return(TRUE);
	}

	MSG msg;
	if (pCtrl)
	{
		pCtrl->SetRange32(0,pSpells->GetCount()-1);
		pCtrl->SetPos(0);
	}

	CResInfo *pResInfo;
	CInfBifFile *pBifFile;
	CSpellBitmapInfo *pInfo;
	int nIndex;
	int nCount = 1;
	char szSpellIcon[10];
	CString strName;
	HBITMAP hBitmap;
	BITMAP bm;
	INF_SPL spl;

	memset(szSpellIcon,0,10);
	POSITION pos = pSpells->GetHeadPosition();
	while(pos)
	{
		if (pCtrl)
		{
			pCtrl->SetPos(++nCount);

			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		
		pResInfo = (CResInfo*)pSpells->GetNext(pos);

		pBifFile = _infKey.GetBifFile(pResInfo);
		if (!pBifFile || !pBifFile->IsValid())
			continue;

		if (pBifFile->GetData(pResInfo->wLocator,&spl) == -1)
			continue;

		memcpy(szSpellIcon,spl.chSpellIcon,8);

		// See if we already have the icon in the list.
		if (m_mapInfo.Lookup(szSpellIcon,(CObject*&)pInfo))
			continue;

		if (!GetSpellBitmap(pResInfo,spl,GetSysColor(COLOR_WINDOW),hBitmap))
			continue;

		// At least one of the icon images is the wrong size. This causes the
		// image control to get confused somehow. So now I'm checking to make
		// sure the image really is 32x32 and not adding it if isn't. Berserk
		// "SPIN117" has a goofy sized icon, in fact it's a squirrel. It has 2
		// frames, like an inventory object where the rest of the spell icons
		// have just a single frame. Also, this description for this innate
		// is messed up, it says barkskin. This probably isn't supposed to be
		// available. The game has some cloud with lightning icon when you give
		// this character that ability. No clue where it pulls that icon from.
		if (!::GetObject(hBitmap,sizeof(bm),&bm) || bm.bmWidth != 32 || bm.bmHeight != 32)
		{
			::DeleteObject(hBitmap);
			hBitmap = NULL;
			continue;
		}

		nIndex = m_ilSpells.Add(CBitmap::FromHandle(hBitmap),RGB(0,0,0));
		::DeleteObject(hBitmap);
		hBitmap = NULL;
		if (nIndex == -1)
			continue;

		pInfo = new CSpellBitmapInfo;
		if (!pInfo)
			return(FALSE);

		pInfo->nIndex = nIndex;
		m_mapInfo.SetAt(szSpellIcon,pInfo);
	}

	return(TRUE);
}

int CSpellBitmaps::GetImageIndex(const char *pszResource)
{
	CSpellBitmapInfo *pInfo;
	if (!m_mapInfo.Lookup(pszResource,(CObject*&)pInfo))
		return(m_nEmptyIndex);

	return(pInfo->nIndex);
}

