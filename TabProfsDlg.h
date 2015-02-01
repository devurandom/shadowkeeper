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

#if !defined(AFX_TABPROFSDLG_H__50BEB4A3_60A6_11D4_9BE4_444553540000__INCLUDED_)
#define AFX_TABPROFSDLG_H__50BEB4A3_60A6_11D4_9BE4_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabProfsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabProfsDlg dialog

struct PROFDATA
{
	BYTE	chProf;
	int	nFirstClass;
	int	nSecondClass;
};

class CValueList;
class CTabProfsDlg : public CDialog
{
// Construction
public:
	CTabProfsDlg(CWnd* pParent = NULL);   // standard constructor

	HACCEL m_hAccel;

	int  GetSelectedItem()					{ return(m_lcProfs.GetNextItem(-1,LVNI_SELECTED)); }
	void SetSelectedItem(int nIndex)    { m_lcProfs.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); }

	// Pass in a list of CProfData objects. The counts are set for each item in the list
	// and the objects in the list are deleted and the list is cleared.
	void SetProfs(CObList &list);

	// Pass in an empty list and all the items in the list control with counts set are
	// added to it.
	void GetProfs(CObList &list);

	// Fills the list control with all the known proficiency values.
	void FillList(CValueList &vl);

private:
	void ClearCounts();
	void UpdateCount(BYTE chProf, int nFirstClass, int nSecondClass);


public:

// Dialog Data
	//{{AFX_DATA(CTabProfsDlg)
	enum { IDD = IDD_PROFICIENCIES };
	CListCtrl	m_lcProfs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabProfsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabProfsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void On1stclassdec();
	afx_msg void On1stclassinc();
	afx_msg void On2ndclassdec();
	afx_msg void On2ndclassinc();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPROFSDLG_H__50BEB4A3_60A6_11D4_9BE4_444553540000__INCLUDED_)
