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

#ifndef __SPELLBROWSERCOM__
#define __SPELLBROWSERCOM__

#include "InfKey.h"
#include "InfBifFile.h"

// This class is designed for communicating between the saved game views and the
// object browser view.
//
// See ItemBrowserCom for a description of the role of this object.

#define HINT_SBC_CHANGESELECTED					100100
#define HINT_SBC_DISPLAYSPELL						100101
#define HINT_SBC_ASSIGNSPELL						100102

// Used for the HINT_IBD_DISPLAYITEM. The string for the resource is set in
// this object. This object gets sent along as hint data.

// Sent as the hint data along with HINT_SBD_ASSIGNITEM.
class CHintSpell : public CObject
{
public:
	CString	strName;
	CResInfo	*pResInfo;
	WORD		wSpellType;		// The INF_CRE_ST_ value. This is different from what is in the SPL.
	INF_SPL	spl;
	CView		*pView;			// View that should handle this message.
};

class CSpellBrowserCom : public CObject
{
public:
	CSpellBrowserCom();
	~CSpellBrowserCom();

	////////////////////////////////////////////////////////////////////////////////
	// View interfaces.
	//

	// Called each time the selection changes in the spell tab list or the
	// character changes. This should always reflect the current character.
	// This function sends the browser document the hint: HINT_SBC_CHANGESELECTED.
	//
	// pszGameName - Name of the saved game.
	// pszCharName - Name of the current character.
	// nSpellType - Needs to be one of the INF_CRE_ST_ values.
	// pView - This object is used to send document hints back from the browser.
	void SetSelected(const char *pszGameName, const char *pszCharName, CView *pView);

	// This should be called whenver a view closes. If the view pointer
	// passed in is the same as the currently selected view the browser will
	// be informed that assignments are no longer possible. It is necessary to
	// pass in a pointer to the view requesting the clear because multiple
	// views may be open and the one closing may not be the one that has set
	// the current selection. This function sends the browser document the hint:
	// HINT_SBC_CHANGESELECTED.
	void ClearSelected(CView *pView);

	// Called to display and select a particular spell in the spell browser's list.
	// If the spell browser is not currently open, it will be opened. This function
	// sends the browser document the hint: HINT_SBC_DISPLAYSPELL and passes a
	// pointer to a CString that contains the resource as the CObject*.
	void DisplayItem(const char *pszResource);

	////////////////////////////////////////////////////////////////////////////////
	// Spell browser interfaces.
	//

	// Returns TRUE if this object has the information needed to communicate with
	// a view. The spell browser uses this value to disable/enable the assignment
	// button.
	BOOL HasSelection()										{ return(m_pCurrentView != NULL); }

	// Called to tell the view to assign the item.
	// pszName - The item's name from the .tlk file.
	// resInfo - Filled in resource info structure.
	// itm - Filled in INF_ITM structure.
	// wActualSpellType - The spell type expressed as a INF_CRE_ST value. Different
	//		from the internal resource value.
	void AssignItem(const char *pszName, CResInfo *pResInfo, INF_SPL *pSpl, WORD wSpellType);

	const char* GetGameName()								{ return(m_strGameName); }
	const char* GetCharName()								{ return(m_strCharName); }

private:
	// Returns a pointer to the item browser document if it is open. Returns NULL
	// otherwise.
	CDocument*	GetBrowserDoc();

	// Pointer to the document that has set the current selection.
	CView			*m_pCurrentView;

	CString		m_strGameName;
	CString		m_strCharName;

};

#endif
