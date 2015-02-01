// MDIClientWnd.cpp : implementation file
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
#include "dalekeeper.h"
#include "MDIClientWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIClientWnd

CMDIClientWnd::CMDIClientWnd()
{
	m_hBitmap = NULL;
}

CMDIClientWnd::~CMDIClientWnd()
{
}


BEGIN_MESSAGE_MAP(CMDIClientWnd, CWnd)
	//{{AFX_MSG_MAP(CMDIClientWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMDIClientWnd message handlers

BOOL CMDIClientWnd::OnEraseBkgnd(CDC* pDC) 
{
	if (!m_hBitmap)
	{
		CRect rClient;
		GetClientRect(&rClient);
		pDC->FillSolidRect(&rClient,::GetSysColor(COLOR_APPWORKSPACE));
	}

	return(TRUE);
}

void CMDIClientWnd::SetBitmap(HBITMAP hBitmap)
{
	m_hBitmap = hBitmap;
	Invalidate();
}


void CMDIClientWnd::OnPaint() 
{
	CPaintDC dc(this);

	if (!m_hBitmap)
		return;

	CRect rClient;
	GetClientRect(&rClient);

	CDC dcMem;
	if (dcMem.CreateCompatibleDC(&dc))
	{
		CBitmap bmpClient;
		bmpClient.CreateCompatibleBitmap(&dc,rClient.Width(),rClient.Height());
		CBitmap *pOldClientBmp = dcMem.SelectObject(&bmpClient);

		CDC dcBmp;
		if (dcBmp.CreateCompatibleDC(&dc))
		{
			CBitmap *pBitmap = CBitmap::FromHandle(m_hBitmap);

			// If the bitmap is smaller than the rBitmap area, center it.
			CRect rPos;
			BITMAP bmp;
			pBitmap->GetBitmap(&bmp);
			if (bmp.bmWidth < rClient.Width() || bmp.bmHeight < rClient.Height())
			{
				rPos.left = rClient.Width()/2 - bmp.bmWidth/2;
				rPos.top = rClient.Height()/2 - bmp.bmHeight/2;

				if (rPos.left < 0)
					rPos.left = 0;
				if (rPos.top < 0)
					rPos.top = 0;

				rPos.right = rPos.left + bmp.bmWidth - 1;
				rPos.bottom = rPos.top + bmp.bmHeight - 1;
			}
			else
				rPos = rClient;

			dcMem.FillSolidRect(&rClient,::GetSysColor(COLOR_APPWORKSPACE));

			CBitmap *pOldBitmap = dcBmp.SelectObject(pBitmap);
			dcMem.BitBlt(rPos.left,rPos.top,rPos.Width(),rPos.Height(),&dcBmp,0,0,SRCCOPY);
			dcBmp.SelectObject(pOldBitmap);
		}

		dc.BitBlt(0,0,rClient.Width(),rClient.Height(),&dcMem,0,0,SRCCOPY);
		dcMem.SelectObject(pOldClientBmp);
	}
	else
		dc.FillSolidRect(&rClient,::GetSysColor(COLOR_APPWORKSPACE));
}

void CMDIClientWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	Invalidate();

	TRACE("CWnd::OnSize(%d,%d,%d)\n",nType,cx,cy);	
}
