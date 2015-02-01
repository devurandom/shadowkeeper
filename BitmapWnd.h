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

#if !defined(AFX_BITMAPWND_H__8C7CE89D_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
#define AFX_BITMAPWND_H__8C7CE89D_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BitmapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapWnd window

#ifndef __BITMAPWND__
#define __BITMAPWND__

#define BWS_RAISED			0
#define BWS_INSET				1

class CBitmapWnd : public CWnd
{
// Construction
public:
	CBitmapWnd();

	BOOL Create(CRect &rPos, CWnd *pParent, UINT nID);

	// The object loads the bitmap from a file and destroys it when done.
	BOOL LoadBitmap(const char *pszFilename, int nFrameStyle = BWS_RAISED);

	// Handle to a valid bitmap is passed in. The bitmap is not destroyed
	// by the object.
	void SetBitmap(HBITMAP hBmp, int nFrameStyle = BWS_RAISED);

	// An image list is used to draw the picture on the button.
	void SetImageList(CImageList *pImageList, int nDisplayItem, int nFrameStyle = BWS_RAISED);

private:
	HBITMAP m_hBitmap;

	// Set to TRUE if hte bitmap is loaded from a file. Set to false when
	// SetBitmap is called.
	BOOL m_bDeleteBitmap;

	CImageList *m_pImageList;

	// When using an Image List this is the index of the item to draw.
	int  m_nDisplayItem;

	// Use defines at the top of the file for raised or inset borders.
	int  m_nFrameStyle;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitmapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBitmapWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPWND_H__8C7CE89D_597C_11D4_9BE4_E8E92BBF2D0A__INCLUDED_)
