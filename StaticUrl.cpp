// StaticUrl.cpp : implementation file
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
#include "StaticUrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticUrl

CStaticUrl::CStaticUrl()
{
	m_hHand = AfxGetApp()->LoadCursor(IDC_HANDCUR);
}

CStaticUrl::~CStaticUrl()
{
}

BEGIN_MESSAGE_MAP(CStaticUrl, CStatic)
	//{{AFX_MSG_MAP(CStaticUrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticUrl message handlers

void CStaticUrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Get the current font for the static control and create an identical
	// font with an underline to it.
	LOGFONT lf;
	CFont *pCurFont = GetFont();
	if (!pCurFont)
		return;

	pCurFont->GetLogFont(&lf);
	lf.lfUnderline = TRUE;
	CFont font;
	font.CreateFontIndirect(&lf);
	CFont *pOldFont = dc.SelectObject(&font);

	CRect rWindow;
	GetWindowRect(&rWindow);

	CString strText;
	GetWindowText(strText);
	CSize szText = dc.GetTextExtent(strText);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,255));
	dc.TextOut(rWindow.Width()/2-szText.cx/2,0,strText);

	dc.SelectObject(pOldFont);
}

void CStaticUrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	Launch();
}

void CStaticUrl::Launch()
{
	if (!m_strUrl.IsEmpty())
		ShellExecute(
			AfxGetMainWnd()->GetSafeHwnd(),
			"open",
			m_strUrl,
			NULL,
			NULL,
			SW_SHOWNORMAL
			);
}

BOOL CStaticUrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (!m_hHand)
		return CStatic::OnSetCursor(pWnd, nHitTest, message);

	::SetCursor(m_hHand);
	return(TRUE);
}
