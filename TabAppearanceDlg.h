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

#if !defined(AFX_TABAPPEARANCEDLG_H__CA0776E1_FB78_42AB_B602_B835E0165DC1__INCLUDED_)
#define AFX_TABAPPEARANCEDLG_H__CA0776E1_FB78_42AB_B602_B835E0165DC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabAppearanceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabAppearanceDlg dialog

#include "BitmapWnd.h"

// Control IDs for the color swatches, added at runtime.
#define IDU_HAIR					5000
#define IDU_SKIN					5001
#define IDU_MAJOR					5002
#define IDU_MINOR					5003
#define IDU_ARMOR					5004
#define IDU_LEATHER				5005
#define IDU_METAL					5006

// For positioning the color swatches.
#define CS_WIDTH					26
#define CS_HEIGHT					26
#define CS_VERT_SPACE			32		// Space between top of one to top of next.
#define CS_LEFTCOL_LEFT			90
#define CS_LEFTCOL_TOP			52
#define CS_RIGHTCOL_LEFT		248
#define CS_RIGHTCOL_TOP			52

class CTabAppearanceDlg : public CDialog
{
// Construction
public:
	CTabAppearanceDlg(CWnd* pParent = NULL);   // standard constructor

	HACCEL m_hAccel;

	void SetHairColor(BYTE chIndex);
	void SetSkinColor(BYTE chIndex);
	void SetMajorColor(BYTE chIndex);
	void SetMinorColor(BYTE chIndex);
	void SetArmorColor(BYTE chIndex);
	void SetLeatherColor(BYTE chIndex);
	void SetMetalColor(BYTE chIndex);

	void SetAppearance(WORD wAppearance);
	WORD GetAppearance();

	// When setting the values initially use the Set functions above.
	// These are kept up to date after that point and the values can
	// be queried at any time.
	BYTE m_chHairColor;
	BYTE m_chSkinColor;
	BYTE m_chMajorColor;
	BYTE m_chMinorColor;
	BYTE m_chArmorColor;
	BYTE m_chLeatherColor;
	BYTE m_chMetalColor;

private:	
	void SetColor(CBitmapWnd &wnd, BYTE &chIndex);

	// Lets the user put any value they like in for any of the lists. The bKnownFlag
	// is list of BOOLs that keep track of whether the box has an unknown value or not.
	BOOL EditValue(CComboBox &cb, DWORD &dwCurrentValue, BOOL &bKnownFlag, int nPad);
	BOOL EditValue(CComboBox &cb, WORD &wValue, BOOL &bKnownFlag, int nPad);
	BOOL SetValue(int nValue, CComboBox &cb);

	CBitmapWnd m_wndHair;
	CBitmapWnd m_wndSkin;
	CBitmapWnd m_wndMajor;
	CBitmapWnd m_wndMinor;
	CBitmapWnd m_wndArmor;
	CBitmapWnd m_wndLeather;
	CBitmapWnd m_wndMetal;

	WORD	m_wAppearance;
	BOOL	m_bAppearance;

public:

// Dialog Data
	//{{AFX_DATA(CTabAppearanceDlg)
	enum { IDD = IDD_APPEARANCE };
	CComboBox	m_cbAppearance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabAppearanceDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabAppearanceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetvalueAppearance();
	//}}AFX_MSG
	afx_msg void OnChangeSkinColor();
	afx_msg void OnChangeHairColor();
	afx_msg void OnChangeMajorColor();
	afx_msg void OnChangeMinorColor();
	afx_msg void OnChangeArmorColor();
	afx_msg void OnChangeLeatherColor();
	afx_msg void OnChangeMetalColor();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABAPPEARANCEDLG_H__CA0776E1_FB78_42AB_B602_B835E0165DC1__INCLUDED_)
