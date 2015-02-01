// TabAppearanceDlg.cpp : implementation file
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
#include "TabAppearanceDlg.h"
#include "PalImageList.h"
#include "SetValueDlg.h"
#include "ColorPickerDlg.h"
#include "ValueList.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabAppearanceDlg dialog


CTabAppearanceDlg::CTabAppearanceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabAppearanceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabAppearanceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bAppearance = FALSE;

	m_hAccel = NULL;
}


void CTabAppearanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabAppearanceDlg)
	DDX_Control(pDX, IDC_APPEARANCE, m_cbAppearance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabAppearanceDlg, CDialog)
	//{{AFX_MSG_MAP(CTabAppearanceDlg)
	ON_BN_CLICKED(IDC_SETVALUE_APPEARANCE, OnSetvalueAppearance)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDU_SKIN, OnChangeSkinColor)
	ON_BN_CLICKED(IDU_HAIR, OnChangeHairColor)
	ON_BN_CLICKED(IDU_MAJOR, OnChangeMajorColor)
	ON_BN_CLICKED(IDU_MINOR, OnChangeMinorColor)
	ON_BN_CLICKED(IDU_ARMOR, OnChangeArmorColor)
	ON_BN_CLICKED(IDU_LEATHER, OnChangeLeatherColor)
	ON_BN_CLICKED(IDU_METAL, OnChangeMetalColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabAppearanceDlg message handlers

BOOL CTabAppearanceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabAppearanceDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// If ENTER or ESC is hit on any of the controls on a dialog, they send the messages
	// back to the parent dialog. This causes the dialog to close itself down if the
	// IDOK and IDCANCEL messages are not mapped. I was putting the OK and Cancel buttons
	// on the dialogs and hiding them, then mapping the messages and doing nothing, but
	// this is a much easier solution.
	if (wParam == IDOK || wParam == IDCANCEL)
		return(TRUE);
	
	return CDialog::OnCommand(wParam, lParam);
}

BOOL CTabAppearanceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	_vlAnimations.FillComboBox(m_cbAppearance);

	m_wndHair.Create(
		CRect(CS_LEFTCOL_LEFT,CS_LEFTCOL_TOP,CS_LEFTCOL_LEFT+CS_WIDTH,CS_LEFTCOL_TOP+CS_HEIGHT),
		this,
		IDU_HAIR);
	m_wndMajor.Create(
		CRect(CS_LEFTCOL_LEFT,CS_LEFTCOL_TOP+CS_VERT_SPACE,CS_LEFTCOL_LEFT+CS_WIDTH,CS_LEFTCOL_TOP+CS_VERT_SPACE+CS_HEIGHT),
		this,
		IDU_MAJOR);
	m_wndArmor.Create(
		CRect(CS_LEFTCOL_LEFT,CS_LEFTCOL_TOP+CS_VERT_SPACE*2,CS_LEFTCOL_LEFT+CS_WIDTH,CS_LEFTCOL_TOP+CS_VERT_SPACE*2+CS_HEIGHT),
		this,
		IDU_ARMOR);
	m_wndMetal.Create(
		CRect(CS_LEFTCOL_LEFT,CS_LEFTCOL_TOP+CS_VERT_SPACE*3,CS_LEFTCOL_LEFT+CS_WIDTH,CS_LEFTCOL_TOP+CS_VERT_SPACE*3+CS_HEIGHT),
		this,
		IDU_METAL);

	m_wndSkin.Create(
		CRect(CS_RIGHTCOL_LEFT,CS_RIGHTCOL_TOP,CS_RIGHTCOL_LEFT+CS_WIDTH,CS_RIGHTCOL_TOP+CS_HEIGHT),
		this,
		IDU_SKIN);
	m_wndMinor.Create(
		CRect(CS_RIGHTCOL_LEFT,CS_RIGHTCOL_TOP+CS_VERT_SPACE,CS_RIGHTCOL_LEFT+CS_WIDTH,CS_RIGHTCOL_TOP+CS_VERT_SPACE+CS_HEIGHT),
		this,
		IDU_MINOR);
	m_wndLeather.Create(
		CRect(CS_RIGHTCOL_LEFT,CS_RIGHTCOL_TOP+CS_VERT_SPACE*2,CS_RIGHTCOL_LEFT+CS_WIDTH,CS_RIGHTCOL_TOP+CS_VERT_SPACE*2+CS_HEIGHT),
		this,
		IDU_LEATHER);


	return TRUE;
}

void CTabAppearanceDlg::SetColor(CBitmapWnd &wnd, BYTE &chIndex)
{
	if (chIndex >= _ilPal.GetImageCount())
		chIndex = 0;

	wnd.SetImageList(&_ilPal,chIndex);
}

void CTabAppearanceDlg::SetHairColor(BYTE chIndex)
{
	SetColor(m_wndHair,chIndex);
	m_chHairColor = chIndex;
}

void CTabAppearanceDlg::SetSkinColor(BYTE chIndex)
{
	SetColor(m_wndSkin,chIndex);
	m_chSkinColor = chIndex;
}

void CTabAppearanceDlg::SetMajorColor(BYTE chIndex)
{
	SetColor(m_wndMajor,chIndex);
	m_chMajorColor = chIndex;
}

void CTabAppearanceDlg::SetMinorColor(BYTE chIndex)
{
	SetColor(m_wndMinor,chIndex);
	m_chMinorColor = chIndex;
}

void CTabAppearanceDlg::SetArmorColor(BYTE chIndex)
{
	SetColor(m_wndArmor,chIndex);
	m_chArmorColor = chIndex;
}

void CTabAppearanceDlg::SetLeatherColor(BYTE chIndex)
{
	SetColor(m_wndLeather,chIndex);
	m_chLeatherColor = chIndex;
}

void CTabAppearanceDlg::SetMetalColor(BYTE chIndex)
{
	SetColor(m_wndMetal,chIndex);
	m_chMetalColor = chIndex;
}

BOOL CTabAppearanceDlg::SetValue(int nValue, CComboBox &cb)
{
	int nCount = cb.GetCount();
	for (int i=0;i<nCount;i++)
		if ((int)cb.GetItemData(i) == nValue)
		{
			cb.SetCurSel(i);
			cb.EnableWindow(TRUE);
			return(TRUE);
		}

	cb.SetCurSel(-1);
	cb.EnableWindow(FALSE);
	return(FALSE);
}

BOOL CTabAppearanceDlg::EditValue(CComboBox &cb, DWORD &dwCurrentValue, BOOL &bKnownFlag, int nPad)
{
	CSetValueDlg d;

	d.SetCombo(&cb);
	if (bKnownFlag)
		d.SetCurrentValue(dwCurrentValue,nPad);
	else
	{
		int nIndex = cb.GetCurSel();
		if (nIndex == -1)
			return(FALSE);
		d.SetCurrentValue(cb.GetItemData(nIndex),nPad);
	}
	if (d.DoModal() != IDOK)
		return(FALSE);

	dwCurrentValue = d.GetValue();
	if (!SetValue(dwCurrentValue,cb))
		bKnownFlag = TRUE;
	else
		bKnownFlag = FALSE;

	return(TRUE);
}

BOOL CTabAppearanceDlg::EditValue(CComboBox &cb, WORD &wValue, BOOL &bKnownFlag, int nPad)
{
	DWORD dwValue = wValue;
	if (!EditValue(cb,dwValue,bKnownFlag,nPad))
		return(FALSE);

	wValue = (WORD)dwValue;
	return(TRUE);
}

void CTabAppearanceDlg::SetAppearance(WORD wAppearance)
{
	if (!CValueList::SetComboBoxValue(m_cbAppearance,wAppearance,TRUE))
	{
		m_wAppearance = wAppearance;
		m_bAppearance = TRUE;
	}
	else
		m_bAppearance = FALSE;
}

WORD CTabAppearanceDlg::GetAppearance()
{
	if (m_bAppearance)
		return(m_wAppearance);

	return((WORD)CValueList::GetComboBoxValue(m_cbAppearance));
}

void CTabAppearanceDlg::OnSetvalueAppearance() 
{
	EditValue(m_cbAppearance,m_wAppearance,m_bAppearance,4);
}

void CTabAppearanceDlg::OnChangeSkinColor()
{
	CColorPickerDlg d;

	d.m_nSelection = m_chSkinColor;
	if (d.DoModal() == IDOK)
	{
		m_chSkinColor = (BYTE)d.m_nSelection;
		m_wndSkin.SetImageList(&_ilPal,m_chSkinColor);
	}
}

void CTabAppearanceDlg::OnChangeHairColor()
{
	CColorPickerDlg d;

	d.m_nSelection = m_chHairColor;
	if (d.DoModal() == IDOK)
	{
		m_chHairColor = (BYTE)d.m_nSelection;
		m_wndHair.SetImageList(&_ilPal,m_chHairColor);
	}
}

void CTabAppearanceDlg::OnChangeMajorColor()
{
	CColorPickerDlg d;

	d.m_nSelection = m_chMajorColor;
	if (d.DoModal() == IDOK)
	{
		m_chMajorColor = (BYTE)d.m_nSelection;
		m_wndMajor.SetImageList(&_ilPal,m_chMajorColor);
	}
}

void CTabAppearanceDlg::OnChangeMinorColor()
{
	CColorPickerDlg d;

	d.m_nSelection = m_chMinorColor;
	if (d.DoModal() == IDOK)
	{
		m_chMinorColor = (BYTE)d.m_nSelection;
		m_wndMinor.SetImageList(&_ilPal,m_chMinorColor);
	}
}

void CTabAppearanceDlg::OnChangeArmorColor()
{
	CColorPickerDlg d;

	d.m_nSelection = m_chArmorColor;
	if (d.DoModal() == IDOK)
	{
		m_chArmorColor = (BYTE)d.m_nSelection;
		m_wndArmor.SetImageList(&_ilPal,m_chArmorColor);
	}
}

void CTabAppearanceDlg::OnChangeLeatherColor()
{
	CColorPickerDlg d;

	d.m_nSelection = m_chLeatherColor;
	if (d.DoModal() == IDOK)
	{
		m_chLeatherColor = (BYTE)d.m_nSelection;
		m_wndLeather.SetImageList(&_ilPal,m_chLeatherColor);
	}
}

void CTabAppearanceDlg::OnChangeMetalColor()
{
	CColorPickerDlg d;

	d.m_nSelection = m_chMetalColor;
	if (d.DoModal() == IDOK)
	{
		m_chMetalColor = (BYTE)d.m_nSelection;
		m_wndMetal.SetImageList(&_ilPal,m_chMetalColor);
	}
}
