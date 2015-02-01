// TabInvDlg.cpp : implementation file
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
#include "TabInvDlg.h"
#include "InfBifFile.h"
#include "InfTlk.h"
#include "ItemBrowserCom.h"
#include "Global.h"
#include "SetQtyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabInvDlg dialog


CTabInvDlg::CTabInvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabInvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabInvDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParentView = NULL;
	m_hNoIcon = NULL;
	m_hAccel = NULL;
}

CTabInvDlg::~CTabInvDlg()
{
	if (m_hNoIcon)
		::DeleteObject(m_hNoIcon);
}

void CTabInvDlg::ClearItems()
{
	int nCount = m_lcInv.GetItemCount();
	for (int i=0;i<nCount;i++)
	{
		m_lcInv.SetItemText(i,2,"");
		m_lcInv.SetItemText(i,3,"");
		m_lcInv.SetItemText(i,4,"");

		m_ilIcons.Replace(i,CBitmap::FromHandle(m_hNoIcon),NULL);
	}
	m_lcInv.Invalidate();

	memset(&m_infItm[0],0,sizeof(INF_ITM)*INF_NUM_ITEMSLOTS);
	memset(&m_infItems[0],0,sizeof(INF_CRE_ITEM)*INF_NUM_ITEMSLOTS);
}

void CTabInvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabInvDlg)
	DDX_Control(pDX, IDC_CHANGEITEM, m_btnChange);
	DDX_Control(pDX, IDC_SETQTY, m_btnQty);
	DDX_Control(pDX, IDC_CLEAR, m_btnClear);
	DDX_Control(pDX, IDC_INVENTORY, m_lcInv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabInvDlg, CDialog)
	//{{AFX_MSG_MAP(CTabInvDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_INVENTORY, OnItemchangedInventory)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_SETQTY, OnSetqty)
	ON_BN_CLICKED(IDC_SETALLQTY, OnSetallqty)
	ON_NOTIFY(NM_DBLCLK, IDC_INVENTORY, OnDblclkInventory)
	ON_BN_CLICKED(IDC_CHANGEITEM, OnChangeitem)
	ON_BN_CLICKED(IDC_IDALL, OnIdall)
	ON_NOTIFY(NM_SETFOCUS, IDC_INVENTORY, OnSetfocusInventory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabInvDlg message handlers

BOOL CTabInvDlg::PreTranslateMessage(MSG* pMsg) 
{
	// This was the only way I could get the accelerators to work while the
	// focus is on the modeless dialog.

	CFrameWnd *pWnd = GetParentFrame();
	if (pWnd && ::TranslateAccelerator(pWnd->GetSafeHwnd(),m_hAccel,pMsg))
		return(TRUE);

	return(CDialog::PreTranslateMessage(pMsg));
}

BOOL CTabInvDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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

BOOL CTabInvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));

	DWORD dwStyle = m_lcInv.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	if (_bUseGridLines)
		dwStyle |= LVS_EX_GRIDLINES;
	m_lcInv.SetExtendedStyle(dwStyle);

	// Create the blank bitmap for use in the control where there aren't any items
	// assigned.
	CDC *pDC = GetDC();
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

	m_hNoIcon = ::CreateDIBitmap(
		pDC->GetSafeHdc(),
		&bih,
		CBM_INIT,
		chBits,
		(BITMAPINFO*)&bih,
		DIB_RGB_COLORS);

	ReleaseDC(pDC);

	m_lcInv.InsertColumn(0,"",LVCFMT_LEFT,38);
	m_lcInv.InsertColumn(1,"Position",LVCFMT_LEFT,75);
	m_lcInv.InsertColumn(2,"Qty",LVCFMT_LEFT,60);
	m_lcInv.InsertColumn(3,"Item");
	ResizeLastColumn(m_lcInv);
	m_lcInv.InsertColumn(4,"Resource",LVCFMT_LEFT,75);

	m_ilIcons.Create(32,32,ILC_COLOR32,INF_NUM_ITEMSLOTS,0);

	// To make the image list easy to manage, every item in the list has its
	// own image. When an item is stuck in teh list, the image for that index
	// is replaced with the proper one. The keeps all the indices straight so
	// that changing a single item in the middle of the list doesn't throw off
	// all the indices to the other images.
	for (i=0;i<INF_NUM_ITEMSLOTS;i++)
		m_ilIcons.Add(CBitmap::FromHandle(m_hNoIcon),RGB(0,0,0));

	m_lcInv.SetImageList(&m_ilIcons,LVSIL_SMALL);

	// Fill in all the positions. I'm either missing some position or there are
	// really only 37 spots in use. Everywhere I copy the data I'm doing the full
	// size, but I don't want that extra item in the list control.
	int nItem;
	for (i=0;i<INF_NUM_ITEMSLOTS-1;i++)
	{
		nItem = m_lcInv.InsertItem(i,"",i);
		m_lcInv.SetItemText(nItem,1,_itemPos[i].pszPos);
	}

	SetSelectedItem(0);
	UpdateButtonStates();

	return TRUE; 
}

void CTabInvDlg::GetItems(INF_CRE_ITEM *pItems)
{
	memcpy(pItems,&m_infItems[0],sizeof(INF_CRE_ITEM)*INF_NUM_ITEMSLOTS);
}

void CTabInvDlg::SetItems(INF_CRE_ITEM *pItems)
{
	HBITMAP hBitmap;
	CString strName;
	CString strQty;
	CString strNotFound;
	char	  szResName[9];
	CResInfo *pResInfo;
	int i;

	ClearItems();

	memcpy(&m_infItems[0],pItems,sizeof(INF_CRE_ITEM)*INF_NUM_ITEMSLOTS);

	for(i=0;i<INF_NUM_ITEMSLOTS-1;i++)
		if (m_infItems[i].chResName[0])
		{
			pResInfo = GetItem(m_infItems[i].chResName,&m_infItm[i]);
			if (!pResInfo)
			{
				if (!strNotFound.IsEmpty())
					strNotFound += ",";
				memcpy(szResName,m_infItems[i].chResName,8);
				szResName[8] = '\x0';
				strNotFound += szResName;
				continue;
			}

			if (GetItemBitmap(pResInfo,m_infItm[i],0,GetSysColor(COLOR_WINDOW),hBitmap))
			{
				m_ilIcons.Replace(i,CBitmap::FromHandle(hBitmap),NULL);
				::DeleteObject(hBitmap);
			}

			if (!_infTlk.GetString(m_infItm[i].dwIdentifiedItemName,strName))
				_infTlk.GetString(m_infItm[i].dwGenericItemName,strName);

			strQty.Format("%d/%d/%d",m_infItems[i].wQuantity1,m_infItems[i].wQuantity2,m_infItems[i].wQuantity3);
			m_lcInv.SetItemText(i,2,strQty);

			m_lcInv.SetItemText(i,3,strName);

			memcpy(szResName,m_infItems[i].chResName,8);
			szResName[8] = '\x0';
			m_lcInv.SetItemText(i,4,szResName);
		}

	UpdateButtonStates();

	if (!strNotFound.IsEmpty())
	{
		CString strMessage;
		strMessage.Format("Some items assigned to this character were not found in the "
			"database. If you save this character those items will not be saved.\r\n\r\n"
			"Cannot Find: %s",(const char *)strNotFound);
		MessageBox(strMessage,"Unknown Items",MB_ICONINFORMATION);
	}
}

void CTabInvDlg::UpdateButtonStates()
{
	ASSERT(GetParentFrame());

	int nIndex = GetSelectedItem();
	if (nIndex == -1 || nIndex == INF_ITEMPOS_UNUSED)
	{
		m_btnQty.EnableWindow(FALSE);
		m_btnClear.EnableWindow(FALSE);
		m_btnChange.EnableWindow(FALSE);
		return;
	}
	else
	{
		m_btnChange.EnableWindow(TRUE);
		m_btnQty.EnableWindow(TRUE);
	}
	
	if (m_infItems[nIndex].chResName[0])
		m_btnClear.EnableWindow(TRUE);
	else
		m_btnClear.EnableWindow(FALSE);

	_comItemBrowser.SetSelected(m_strGameName,m_strCharName,nIndex,m_pParentView);
}

void CTabInvDlg::OnItemchangedInventory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	UpdateButtonStates();
}

void CTabInvDlg::OnClear() 
{
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		if (MessageBox("Are you sure you want to clear all inventory items?","Clear All",MB_ICONQUESTION|MB_YESNO) == IDYES)
		{
			ClearItems();
			return;
		}
	}

	int nItem = GetSelectedItem();
	if (nItem == -1)
		return;

	memset(&m_infItems[nItem],0,sizeof(INF_CRE_ITEM));

	m_lcInv.SetItemText(nItem,2,"");
	m_lcInv.SetItemText(nItem,3,"");
	m_lcInv.SetItemText(nItem,4,"");

	m_ilIcons.Replace(nItem,CBitmap::FromHandle(m_hNoIcon),NULL);
	m_lcInv.Invalidate(); 
}

void CTabInvDlg::OnSetqty() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CSetQtyDlg d;

	d.m_nInGameMax = m_infItm[nIndex].wMaxStackable;
	d.m_strName = m_lcInv.GetItemText(nIndex,3);
	d.m_nQty = m_infItems[nIndex].wQuantity1;
	d.m_nQty2 = m_infItems[nIndex].wQuantity2;
	d.m_nQty3 = m_infItems[nIndex].wQuantity3;
	if (d.DoModal() == IDOK)
	{
		m_infItems[nIndex].wQuantity1 = d.m_nQty;
		m_infItems[nIndex].wQuantity2 = d.m_nQty2;
		m_infItems[nIndex].wQuantity3 = d.m_nQty3;
		CString strQty;
		strQty.Format("%d/%d/%d",d.m_nQty,d.m_nQty2,d.m_nQty3);
		m_lcInv.SetItemText(nIndex,2,strQty);
	}
}

void CTabInvDlg::OnSetallqty() 
{
	CSetQtyDlg d;
	CString strQty;

	d.m_nInGameMax = 0xFFFF;
	d.m_strName = "All Stackable Items";
	d.m_nQty = 100;
	d.m_nQty2 = 0;
	d.m_nQty3 = 0;
	if (d.DoModal() == IDOK)
	{
		for (int i=0;i<INF_NUM_ITEMSLOTS-1;i++)
			if (m_infItm[i].wMaxStackable > 1 || m_infItm[i].wCategory == ITEMTYPE_WAND)
			{
				m_infItems[i].wQuantity1 = d.m_nQty;
				m_infItems[i].wQuantity2 = d.m_nQty2;
				m_infItems[i].wQuantity3 = d.m_nQty3;
				strQty.Format("%d",d.m_nQty);
				m_lcInv.SetItemText(i,2,strQty);
			}
	}
}

void CTabInvDlg::OnChangeitem() 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		return;

	CString strRes;
	strRes = m_lcInv.GetItemText(nIndex,4);
	_comItemBrowser.DisplayItem(strRes);
}

void CTabInvDlg::OnDblclkInventory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	OnChangeitem();
}

void CTabInvDlg::AssignItem(CHintItem *pHi)
{
	if (pHi->nPos < 0 || pHi->nPos >= INF_NUM_ITEMSLOTS-1)
		return;

	// Wipe out the current item for a fresh start.
	memset(&m_infItems[pHi->nPos],0,sizeof(INF_CRE_ITEM));

	// Make a local copy of the ITM data.
	memcpy(&m_infItm[pHi->nPos],&pHi->itm,sizeof(INF_ITM));

	// Create the INF_CRE_ITEM data. The only fields I'm really sure about
	// are the resource name, the quantity and the identified flags. I'm leaving
	// the rest of them set to 0.
	memset(m_infItems[pHi->nPos].chResName,0,8);
	memcpy(m_infItems[pHi->nPos].chResName,pHi->resInfo.strName,min(pHi->resInfo.strName.GetLength(),8));
	m_infItems[pHi->nPos].wQuantity2 = 0;
	m_infItems[pHi->nPos].wQuantity3 = 0;
	if (pHi->itm.wMaxStackable > 1)
		m_infItems[pHi->nPos].wQuantity1 = pHi->itm.wMaxStackable;
	else
		if (pHi->itm.wCategory == ITEMTYPE_WAND)
			m_infItems[pHi->nPos].wQuantity1 = 100;
		else
			m_infItems[pHi->nPos].wQuantity1 = 0;
	m_infItems[pHi->nPos].chIdentified = 1;

	HBITMAP hBitmap;
	if (!GetItemBitmap(&pHi->resInfo,pHi->itm,0,GetSysColor(COLOR_WINDOW),hBitmap))
		m_ilIcons.Replace(pHi->nPos,CBitmap::FromHandle(m_hNoIcon),NULL);
	else
	{
		m_ilIcons.Replace(pHi->nPos,CBitmap::FromHandle(hBitmap),NULL);
		::DeleteObject(hBitmap);
	}

	CString strQty;
	if (pHi->itm.wMaxStackable > 1 || pHi->itm.wCategory == ITEMTYPE_WAND)
	{
		strQty.Format("%d/%d/%d",m_infItems[pHi->nPos].wQuantity1,m_infItems[pHi->nPos].wQuantity2,m_infItems[pHi->nPos].wQuantity3);
		m_lcInv.SetItemText(pHi->nPos,2,strQty);
	}
	m_lcInv.SetItemText(pHi->nPos,3,pHi->strName);

	char szResName[9];
	memcpy(szResName,m_infItems[pHi->nPos].chResName,8);
	szResName[8] = '\x0';
	m_lcInv.SetItemText(pHi->nPos,4,szResName);
	m_lcInv.Invalidate();
}


void CTabInvDlg::OnIdall() 
{
	if (MessageBox("This will identify all unknown items in this character's inventory. Continue?","ID All Items",MB_ICONQUESTION|MB_YESNO) != IDYES)
		return;

	CString strName;

	for (int i=0;i<INF_NUM_ITEMSLOTS-1;i++)
		if (i != INF_ITEMPOS_UNUSED && m_infItems[i].chResName[0] && m_infItems[i].chIdentified != 1)
		{
			m_infItems[i].chIdentified = 1;

			if (!_infTlk.GetString(m_infItm[i].dwIdentifiedItemName,strName))
				_infTlk.GetString(m_infItm[i].dwGenericItemName,strName);

			m_lcInv.SetItemText(i,3,strName);
		}
}

void CTabInvDlg::OnSetfocusInventory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = GetSelectedItem();
	if (nIndex == -1)
		_comItemBrowser.ClearSelected(m_pParentView);
	else
		_comItemBrowser.SetSelected(m_strGameName,m_strCharName,nIndex,m_pParentView);
	*pResult = 0;
}
