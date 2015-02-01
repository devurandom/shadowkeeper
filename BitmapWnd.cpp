// BitmapWnd.cpp : implementation file
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
#include "BitmapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapWnd

CBitmapWnd::CBitmapWnd()
{
	m_hBitmap = NULL;
	m_bDeleteBitmap = TRUE;
	m_nFrameStyle = BWS_RAISED;
	m_pImageList = NULL;
}

CBitmapWnd::~CBitmapWnd()
{
	if (m_bDeleteBitmap && m_hBitmap)
		::DeleteObject(m_hBitmap);
}


BEGIN_MESSAGE_MAP(CBitmapWnd, CWnd)
	//{{AFX_MSG_MAP(CBitmapWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBitmapWnd message handlers

BOOL CBitmapWnd::Create(CRect &rPos, CWnd *pParent, UINT nID)
{
	BOOL bResult = CWnd::Create(
		NULL,
		"",
		WS_VISIBLE|WS_CHILD,
		rPos,
		pParent,
		nID
		);

	return(bResult);
}

BOOL CBitmapWnd::LoadBitmap(const char *pszFilename, int nFrameStyle)
{
	if (m_bDeleteBitmap && m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	m_bDeleteBitmap = TRUE;
	m_nFrameStyle = nFrameStyle;
	m_pImageList = NULL;

	m_hBitmap = (HBITMAP)::LoadImage(0,pszFilename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	Invalidate();

	return(m_hBitmap != NULL);
}

void CBitmapWnd::SetBitmap(HBITMAP hBmp, int nFrameStyle)
{
	if (m_bDeleteBitmap && m_hBitmap)
		::DeleteObject(m_hBitmap);
	m_bDeleteBitmap = FALSE;
	m_hBitmap = hBmp;
	m_nFrameStyle = nFrameStyle;
	m_pImageList = NULL;

	Invalidate();
}

void CBitmapWnd::SetImageList(CImageList *pImageList, int nDisplayItem, int nFrameStyle)
{
	if (m_bDeleteBitmap && m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	m_bDeleteBitmap = FALSE;
	m_hBitmap = NULL;
	m_nFrameStyle = nFrameStyle;
	m_pImageList = pImageList;

	if (nDisplayItem < m_pImageList->GetImageCount())
		m_nDisplayItem = nDisplayItem;
	else
		m_nDisplayItem = 0;

	Invalidate();
}

BOOL CBitmapWnd::OnEraseBkgnd(CDC* pDC) 
{
	return(TRUE);
}

void CBitmapWnd::OnPaint() 
{
	CPaintDC dc(this);

	CRect rClient;
	GetClientRect(&rClient);

	CRect rBitmap(rClient);
	rBitmap.DeflateRect(1,1);

	CDC dcMem;
	if (dcMem.CreateCompatibleDC(&dc))
	{
		CBitmap bmpClient;
		bmpClient.CreateCompatibleBitmap(&dc,rClient.Width(),rClient.Height());
		CBitmap *pOldClientBmp = dcMem.SelectObject(&bmpClient);

		dcMem.FillSolidRect(&rClient,GetSysColor(COLOR_BTNFACE));

		if (m_hBitmap || m_pImageList)
		{
			CDC dcBmp;
			if (dcBmp.CreateCompatibleDC(&dc))
			{
				if (m_hBitmap)
				{
					CBitmap *pBitmap = CBitmap::FromHandle(m_hBitmap);

					// If the bitmap is smaller than the rBitmap area, center it.
					BITMAP bmp;
					CRect rPos;
					pBitmap->GetBitmap(&bmp);
					if (bmp.bmWidth < rBitmap.Width() || bmp.bmHeight < rBitmap.Height())
					{
						rPos.left = rBitmap.Width()/2 - bmp.bmWidth/2;
						rPos.top = rBitmap.Height()/2 - bmp.bmHeight/2;
						rPos.right = rPos.left + bmp.bmWidth - 1;
						rPos.bottom = rPos.top + bmp.bmHeight - 1;
					}
					else
						rPos = rBitmap;

					CBitmap *pOldBitmap = dcBmp.SelectObject(pBitmap);
					dcMem.BitBlt(rPos.left,rPos.top,rPos.Width(),rPos.Height(),&dcBmp,0,0,SRCCOPY);
					dcBmp.SelectObject(pOldBitmap);
				}
				else
				{
					IMAGEINFO ii;
					m_pImageList->GetImageInfo(m_nDisplayItem,&ii);

					CRect rImage(ii.rcImage);
					POINT pt;
					SIZE sz;
					if (rImage.Width() < rBitmap.Width() || rImage.Height() < rBitmap.Height())
					{
						pt.x = rBitmap.Width()/2 - rImage.Width()/2;
						pt.y = rBitmap.Height()/2 - rImage.Height()/2;
					}
					else
					{
						pt.x = rBitmap.left;
						pt.y = rBitmap.top;
					}

					sz.cx = rImage.Width();
					sz.cy = rImage.Height();

					POINT ptOrigin;
					ptOrigin.x = 0;
					ptOrigin.y = 0;

					m_pImageList->DrawIndirect(
						&dcMem,
						m_nDisplayItem,
						pt,
						sz,
						ptOrigin);
				}
			}
		}

		if (m_nFrameStyle == BWS_RAISED)
			dcMem.Draw3dRect(&rClient,GetSysColor(COLOR_BTNHILIGHT),GetSysColor(COLOR_BTNSHADOW));
		else
			dcMem.Draw3dRect(&rClient,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));

		dc.BitBlt(0,0,rClient.Width(),rClient.Height(),&dcMem,0,0,SRCCOPY);
		dcMem.SelectObject(pOldClientBmp);
	}
	else
		dc.FillSolidRect(&rClient,GetSysColor(COLOR_BTNFACE));
}


void CBitmapWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	DWORD wParam, lParam;

	wParam = MAKEWPARAM(GetDlgCtrlID(),BN_CLICKED);
	lParam = (DWORD)GetSafeHwnd();
	GetParent()->PostMessage(WM_COMMAND,wParam,lParam);
}
