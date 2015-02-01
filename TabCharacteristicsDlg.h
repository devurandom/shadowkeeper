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

#if !defined(AFX_TABCHARACTERISTICSDLG_H__49818B82_63A4_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_TABCHARACTERISTICSDLG_H__49818B82_63A4_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCharacteristicsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabCharacteristicsDlg dialog

class CTabCharacteristicsDlg : public CDialog
{
// Construction
public:
	CTabCharacteristicsDlg(CWnd* pParent = NULL);   // standard constructor

	HACCEL m_hAccel;

	// Sets the item with teh matching itemdata. If there is no match in the
	// combo box it is greyed out. Returns FALSE is there is no match.
	BOOL	SetValue(int nValue, CComboBox &cb);
	DWORD GetValue(CComboBox &cb);

	void SetAlignment(BYTE chAlignment);
	void SetKit(DWORD dwKit);
	void SetClass(BYTE chClass);
	void SetRace(BYTE chRace);
	void SetGender(BYTE chGender);
	void SetRacialEnemy(BYTE chRacialEnemy);
	void SetEnemyAlly(BYTE chEnemyAlly);

	BYTE GetAlignment();
	DWORD GetKit();
	BYTE GetClass();
	BYTE GetRace();
	BYTE GetGender();
	BYTE GetRacialEnemy();
	BYTE GetEnemyAlly();

private:
	// Lets the user put any value they like in for any of the lists. The bKnownFlag
	// is list of BOOLs that keep track of whether the box has an unknown value or not.
	BOOL EditValue(CComboBox &cb, DWORD &dwCurrentValue, BOOL &bKnownFlag, int nPad);
	BOOL EditValue(CComboBox &cb, BYTE &chValue, BOOL &bKnownFlag, int nPad);
	BOOL EditValue(CComboBox &cb, WORD &wValue, BOOL &bKnownFlag, int nPad);

	BYTE	m_chAlignment;
	BYTE	m_chClass;
	BYTE	m_chRace;
	BYTE	m_chGender;
	BYTE	m_chRacialEnemy;
	DWORD	m_dwKit;
	BYTE	m_chEnemyAlly;

	BOOL	m_bAlignment;
	BOOL	m_bClass;
	BOOL	m_bRace;
	BOOL	m_bGender;
	BOOL	m_bRacialEnemy;
	BOOL	m_bKit;
	BOOL	m_bEnemyAlly;

public:

// Dialog Data
	//{{AFX_DATA(CTabCharacteristicsDlg)
	enum { IDD = IDD_CHARACTERISTICS };
	CComboBox	m_cbEnemyAlly;
	CComboBox	m_cbKit;
	CComboBox	m_cbRacialEnemy;
	CComboBox	m_cbAlignment;
	CComboBox	m_cbClass;
	CComboBox	m_cbRace;
	CComboBox	m_cbGender;
	UINT	m_nMovementSpeed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCharacteristicsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabCharacteristicsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetvalueAlignment();
	afx_msg void OnSetvalueClass();
	afx_msg void OnSetvalueEnemy();
	afx_msg void OnSetvalueGender();
	afx_msg void OnSetvalueKit();
	afx_msg void OnSetvalueRace();
	afx_msg void OnSetvalueEnemyally();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCHARACTERISTICSDLG_H__49818B82_63A4_11D4_9BE4_444553540000__INCLUDED_)
