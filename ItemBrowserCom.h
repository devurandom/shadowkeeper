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

#ifndef __ITEMBROWSERCOM__
#define __ITEMBROWSERCOM__

#include "InfKey.h"
#include "InfBifFile.h"

// This class is designed for communicating between the saved game views and the
// object browser view.
//
// There can be many saved games open, but only one item browser. The item browser
// has an "Assign to Character" button on it. That button needs to be able to tell
// the view that called up the browser that the user has made a selection. Also,
// the saved game view needs to be able to tell the item browser what text to stick
// on the assign button so the user can see what it will do when pushed.
//
// This object does all the communication work between the two views, making sure
// that the object browser is loaded, etc...

// I've set these hints as high values on purpose, trying to avoid any conflicts
// with other doc hints. I'm sure I'll forget to watch out for these values, but
// I generally start with low numbers so I doubt I'll get a conflict by accident.
#define HINT_IBC_CHANGESELECTED					100000
#define HINT_IBC_DISPLAYITEM						100001
#define HINT_IBC_ASSIGNITEM						100002

// Sent as the hint data along with HINT_IBD_ASSIGNITEM.
class CHintItem : public CObject
{
public:
	CString	strName;
	CResInfo	resInfo;
	INF_ITM	itm;
	int		nPos;
	CView		*pView;
};

class CItemBrowserCom : public CObject
{
public:
	CItemBrowserCom();
	~CItemBrowserCom();

	////////////////////////////////////////////////////////////////////////////////
	// View interfaces.
	//

	// Called each time the selection changes in the item position list or the
	// character changes. This should always reflect the current character and
	// currently selected item position. This function sends the browser document
	// the hint: HINT_IBC_CHANGESELECTED.
	//
	// pszGameName - Name of the saved game.
	// pszCharName - Name of the current character.
	// nPos - The index of the item position.
	// pView - This object uses the pointer to send document hints back from the browser.
	void SetSelected(const char *pszGameName, const char *pszCharName, int nPos, CView *pView);

	// This should be called whenver a view closes. If the view pointer
	// passed in is the same as the currently selected view the browser will
	// be informed that assignments are no longer possible. It is necessary to
	// pass in a pointer to the view requesting the clear because multiple
	// views may be open and the one closing may not be the one that has set
	// the current selection. This function sends the browser document the hint:
	// HINT_IBC_CHANGESELECTED.
	void ClearSelected(CView *pView);

	// Called to display and select a particular item in the item browser's list.
	// If the item browser is not currently open, it will be opened. This function
	// sends the browser document the hint: HINT_IBC_DISPLAYITEM and passes a
	// pointer to a CString that contains the resource as the CObject*.
	void DisplayItem(const char *pszResource);

	////////////////////////////////////////////////////////////////////////////////
	// Item browser interfaces.
	//

	// Returns TRUE if this object has the information needed to communicate with
	// a view. The item browser uses this value to disable/enable the assignment
	// button.
	BOOL HasSelection()										{ return(m_pCurrentView != NULL); }

	// Called to tell the view to assign the item.
	// pszName - The item's name from the .tlk file.
	// resInfo - Filled in resource info structure.
	// itm - Filled in INF_ITM structure.
	void AssignItem(const char *pszName, CResInfo &resInfo, INF_ITM *pItm);

	const char* GetGameName()								{ return(m_strGameName); }
	const char* GetCharName()								{ return(m_strCharName); }
	int  GetPos()												{ return(m_nPos); }

private:
	// Returns a pointer to the item browser document if it is open. Returns NULL
	// otherwise.
	CDocument*	GetBrowserDoc();

	// Pointer to the document that has set the current selection.
	CView			*m_pCurrentView;

	CString		m_strGameName;
	CString		m_strCharName;
	int			m_nPos;

};

#endif
