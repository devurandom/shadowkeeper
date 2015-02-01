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


#ifndef __INFGAME__
#define __INFGAME__

#pragma pack(push,1)

#include "InfCreature.h"

struct INF_GAME
{
	// Should be 'GAME'.
	char		chSignature[0x04];

	char		chVersion[0x04];

	// Game time. 300 units = 1 hour. This value is 2100 units ahead of the time displayed
	// in the game.
	DWORD		dwTime;

	char		chUnknown[0x0C];

	// Party's gold.
	DWORD		dwGold;

	char		chUnknown1[0x04];

	// Offset and count of the number of INF_CHARINFO records. There is one record
	// for each character currently in the party.
	DWORD		dwInPartyCharOffset;
	DWORD		dwInPartyCharCount;

	char		chUnknown2[0x08];

	// Offset and count of the INF_CHARINFO records for characters that are not in
	// your party.
	DWORD		dwOutPartyCharOffset;
	DWORD		dwOutPartyCharCount;

	DWORD		dwGlobalVarOffset;
	DWORD		dwGlobalVarCount;

	// I don't know what it's for.
	char		chAreaRes[0x08];

	char		chUnknown3[0x04];

	DWORD		dwJournalCount;
	DWORD		dwJournalOffset;	  // Length may be 0x0c per entry.

	// Like the reputation values in the CRE, this value is 10 times
	// what is shown in the game. 14 repuation is really 140 here.
	BYTE		chPartyReputation;

	char		chUnknown4[0x13];

	// I don't know anything about this area. It's definitely after the
	// journal entries. There is an offset here that points to another block
	// of data at the tail of the file.
	DWORD		dwAfterJournalOffset;

	char		chUnknown5[0x48];
};

struct INF_GAME_CHARINFO
{
	char		chUnknown[0x02];

	// Position of this character in the party. For characters not in the party this
	// value is 0xFFFF.
	WORD		wPartyPosition;

	// Offset and size of the CRE record for this character.
	DWORD		dwCREOffset;
	DWORD		dwCRESize;

	char		chUnknown1[0x0C];

	// Area the character is currently in.
	char		szArea[0x08];

	// Current position of the character in the area.
	WORD		wPlayerX;
	WORD		wPlayerY;

	// Position of the viewing rectangle.
	WORD		wViewX;
	WORD		wViewY;

	char		chUnknown2[0x98];

	// In the case of the first character (PC) this is the name of the
	// character. The name is NOT found in the character data portion
	// like it is for an NPC.
	char		szName[0x15];

	char		chUnknown3[0x8B];
};

struct INF_GAME_GLOBAL
{
	char		chName[0x20];
	char		chUnknown[0x08];
	int		nValue;
	char		chUnknown2[0x28];
};

#pragma pack(pop)

// Number of character you can have in your party. I've hard coded it to make
// thing easier.
#define INF_MAX_CHARACTERS					6

#define ERR_GAME_FAILEDOPEN				1501
#define ERR_GAME_BADHEADER					1502
#define ERR_GAME_BADPARTYCHARINFO		1503
#define ERR_GAME_TOOMANYINPARTY			1504
#define ERR_GAME_READPARTYCHARINFO		1505
#define ERR_GAME_BADOUTPARTYCHARINFO	1506
#define ERR_GAME_OOM_OUTPARTYCHARINFO	1507
#define ERR_GAME_READOUTPARTYCHARINFO	1508
#define ERR_GAME_OOM_OUTPARTYCRE			1509
#define ERR_GAME_BADVARS					1510
#define ERR_GAME_OOM_VARS					1511
#define ERR_GAME_READVARS					1512
#define ERR_GAME_BADJOURNAL				1513
#define ERR_GAME_FAILEDCREATE				1514
#define ERR_GAME_WRITEHEADER				1515
#define ERR_GAME_WRITEPARTYCHARINFO		1516
#define ERR_GAME_WRITEOUTPARTYCHARINFO	1516
#define ERR_GAME_WRITEVARIABLES			1517
#define ERR_GAME_OOM_JOURNAL				1518
#define ERR_GAME_READJOURNAL				1519
#define ERR_GAME_WRITEJOURNAL				1520
#define ERR_GAME_MISSINGSIG				1521
#define ERR_GAME_BADVERSION				1522
#define ERR_GAME_OOM_AFTERJOURNAL		1523
#define ERR_GAME_READAFTERJOURNAL		1524
#define ERR_GAME_WRITEAFTERJOURNAL		1525

class CInfGame
{
public:
	CInfGame();
	~CInfGame();

	// Called to open and read in a save game file.
	BOOL	Read(const char *pszFilename);
	BOOL	Write(const char *pszFilename);

	int	GetLastError()								{ return(m_nError); }

	int	GetPartyCount()							{ return(m_infGame.dwInPartyCharCount); }
	int	GetOutOfPartyCount()						{ return(m_infGame.dwOutPartyCharCount); }

	void	GetPartyCharName(int nChar, CString &strName);		
	void	SetPartyCharName(int nChar, const char *pszName);

	void	GetOutOfPartyCharName(int nChar, CString &strName);		
	void	SetOutOfPartyCharName(int nChar, const char *pszName);

	int	GetPartyGold()								{ return(m_infGame.dwGold); }
	void	SetPartyGold(DWORD dwGold);

	// When you are in a party, all the characters appear to share this value
	// for their reputation.
	BYTE	GetPartyReputation()						{ return(m_infGame.chPartyReputation/10); }

	// Since this value is shared, when setting this value for the party all of
	// the In-Party characters also get their reputations set.
	void	SetPartyReputation(BYTE chRep);

	CInfCreature*	GetPartyCre(int nChar)		{ return(&m_infParty[nChar]); }
	CInfCreature*	GetOutOfPartyCre(int nChar){ return(&m_pinfOutParty[nChar]); }

	// Returns a list of all the globals set for the game. All of the items in the list
	// are duplicates of the real list so they can be deleted from the returned list.
	BOOL GetGlobals(CPtrList &list);

	// Sets the globals for the game. The pointers in this list are saved in the object.
	// Do not delete any of the items in the list after passing them into this function.
	// The list is cleared after the items have been transferred.
	void SetGlobals(CPtrList &list);

	// Returns TRUE if the game data has changed. It checks the creature objects
	// to see if they ahve been changed as well.
	BOOL	HasChanged();

private:
	CString				m_strFilename;

	// Set to TRUE when any of the data has changed.
	BOOL					m_bHasChanged;

	int					m_nError;

	INF_GAME				m_infGame;
	INF_GAME_CHARINFO	m_infCharInfo[INF_MAX_CHARACTERS];
	CInfCreature		m_infParty[INF_MAX_CHARACTERS];
	INF_GAME_CHARINFO	*m_pinfOutCharInfo;
	CInfCreature		*m_pinfOutParty;

	CPtrList				m_plGlobals;
	//INF_GAME_VARIABLE	*m_pinfVariables;

	// I'm just storing this as a block of bytes. I don't care a lot about what the
	// journal information is.
	int	m_nJournalSize;
	BYTE	*m_pJournal;

	int	m_nAfterJournalSize;
	BYTE	*m_pAfterJournal;
};

#endif