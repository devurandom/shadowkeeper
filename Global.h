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


// Path to the Icewind Dale installation.
extern CString _strInstallPath;

// This is set to TRUE if the program has indexed the resources. This needs to
// happen for the editor to work properly. Most of the menu items set their state
// by this flag.
extern BOOL _bResourcesLoaded;

// When set to TRUE log file entries are generated. This can be set by putting
// LOG on the command line to start the editor.
extern BOOL _bLogFile;

// Used to register a clipboard format for holding the CRE's AFF data.
extern UINT _nClipFormatAff;

// Holds the indexes for the resources.
class CInfKey;
extern CInfKey _infKey;

// All the string resources are in a .tlk file. Might as well just keep it open
// and available. Item descriptions, for example, are stored in here.
class CInfTlk;
extern CInfTlk	_infTlk;

// Used to communicate between the item browser view and saved game views.
class CItemBrowserCom;
extern CItemBrowserCom	_comItemBrowser;

class CSpellBrowserCom;
extern CSpellBrowserCom	_comSpellBrowser;

class CSpellBitmaps;
extern CSpellBitmaps _spellBitmaps;

// An image list which contains the samples used for setting character
// colors.
class CPalImageList;
extern CPalImageList _ilPal;

class CInf2DA;
extern CInf2DA _2daHateRace;
extern CInf2DA _2daWeapProf;
extern CInf2DA _2daKitList;

//////////////////////////////////////////////////////////////////////
// Editor Settings
//

extern BOOL	_bUseKnownSpellLimit;
extern BOOL	_bUseMemSpellLimit;
extern BOOL _bMemSpellsOnSave;		// Causes memorized spells to be refreshed as castable.
extern BOOL _bDisableSpellIcons;

extern int	_nKnownSpellLimit;
extern int	_nMemSpellLimit;

extern BOOL	_bDefaultOpenSinglePlayer;
extern BOOL _bUseGridLines;
extern BOOL _bIgnoreDataVersions;
extern BOOL _bAllowQuicksaves;

extern BOOL	_bAllowCHROverwrite;
extern BOOL	_bAllowCREOverwrite;

//
///////////////////////////////////////////////////////////////////////////////////

// Used to hold the resource type/file extension pairs for looking for
// extra data in the override foler. The data is filled set in this 
// objects cpp file.
struct KEY_RES_OVERRIDE
{
	WORD		wResType;
	char		szExt[11];
};

extern KEY_RES_OVERRIDE _keyResOver[];

class CValueList;
extern CValueList _vlAnimations;
extern CValueList _vlKit;
extern CValueList _vlGender;
extern CValueList _vlGenderUser;
extern CValueList _vlAlignment;
extern CValueList _vlRacialEnemy;
extern CValueList _vlRacialEnemyUser;
extern CValueList _vlNumAttacks;
extern CValueList _vlProfs;
extern CValueList _vlProfsUser;
extern CValueList _vlRace;
extern CValueList _vlRaceUser;
extern CValueList _vlClass;
extern CValueList _vlClassUser;
extern CValueList _vlKitList;
extern CValueList _vlAffects;
extern CValueList	_vlEnemyAlly;
extern CValueList	_vlState;
extern CValueList	_vlSpells;

#define ITEMTYPE_MISC						0x0000
#define ITEMTYPE_NECKLACE					0x0001
#define ITEMTYPE_ARMOR						0x0002
#define ITEMTYPE_BELT						0x0003
#define ITEMTYPE_BOOTS						0x0004
#define ITEMTYPE_ARROWS						0x0005
#define ITEMTYPE_GLOVES						0x0006
#define ITEMTYPE_HELM						0x0007
#define ITEMTYPE_KEY							0x0008
#define ITEMTYPE_POTION						0x0009
#define ITEMTYPE_RING						0x000A
#define ITEMTYPE_SCROLL						0x000B
#define ITEMTYPE_SHIELD						0x000C
#define ITEMTYPE_FOOD						0x000D
#define ITEMTYPE_BULLETS					0x000E
#define ITEMTYPE_BOW							0x000F
#define ITEMTYPE_DAGGER						0x0010
#define ITEMTYPE_MACE						0x0011
#define ITEMTYPE_SLING						0x0012
#define ITEMTYPE_SHORTSWORD				0x0013
#define ITEMTYPE_SWORD						0x0014
#define ITEMTYPE_HAMMER						0x0015
#define ITEMTYPE_MORNINGSTAR				0x0016
#define ITEMTYPE_FLAIL						0x0017
#define ITEMTYPE_DART						0x0018
#define ITEMTYPE_AXE							0x0019
#define ITEMTYPE_STAFF						0x001A
#define ITEMTYPE_CROSSBOW					0x001B
#define ITEMTYPE_HANDTOHAND				0x001C
#define ITEMTYPE_SPEAR						0x001D
#define ITEMTYPE_HALBERD					0x001E
#define ITEMTYPE_BOLTS						0x001F
#define ITEMTYPE_CLOAK						0x0020
#define ITEMTYPE_GOLD						0x0021
#define ITEMTYPE_GEM							0x0022
#define ITEMTYPE_WAND						0x0023
#define ITEMTYPE_CONTAINER					0x0024
#define ITEMTYPE_BOOK						0x0025
#define ITEMTYPE_FAMILIAR					0x0026
#define ITEMTYPE_INSTRUMENT				0x0029	// Lyre of Progression?
#define ITEMTYPE_MISC2						0x002A	// I don't know what this is. "The Candle" uses it.
#define ITEMTYPE_CLUB						0x002C
#define ITEMTYPE_LARGESHIELD				0x002F
#define ITEMTYPE_MEDIUMSHIELD				0x0031
#define ITEMTYPE_SMALLSHIELD				0x0035
#define ITEMTYPE_MISC3						0x0037	// Telescope uses it.
#define ITEMTYPE_DRINK						0x0038	// Again not sure, wine and some extract uses this.
#define ITEMTYPE_GREATSWORD				0x0039	// Another guess, 2 items use this.

struct ITEMTYPE
{
	char *pszType;
	WORD wType;
};
extern ITEMTYPE _itemType[];

struct ITEMPOS
{
	char *pszPos;
};
extern ITEMPOS _itemPos[];

// Like above with the ITEMCATs, these need to be in sorting order. They
// are only used by the editor internally, so it doesn't matter what number
// are assigned.
#define SPELLCAT_INNATE						0x00
#define SPELLCAT_PRIEST						0x01
#define SPELLCAT_WIZARD						0x02
#define SPELLCAT_UNKNOWN					0x03

// Attempts to figure out where Icewind Dale is installed.
extern BOOL FindInstallPath(CString &strPath);

// Resizes the last column in a list control to the width of the window.
extern void ResizeLastColumn(CListCtrl &lc, BOOL bSubractScroll=FALSE);

// Takes a double word values and creates a "0x" prefaced zero padded string.
extern void MakeHexString(DWORD dwValue, char *pszResult, int nZeroPad=2);

// The descriptions are inconsistent on how they control \r\n in the middle of
// text descriptions. Sometimes it's \r\n sometimes \n\r and sometimes just one or
// the other. The edit control must have them in \r\n order to properly display.
extern void FixTlkDescription(const char *pszDesc, CString &strFixed);

// Take a text buffer and converts it to word-broken 80 column lines.
extern void Format80Columns(const char *pszText, CString &strFormat);

// Returns the item type description given an item value. Returns TRUE
// if the value is found in the list.
extern BOOL GetItemType(WORD wValue, CString &strType, int &nIndex);
extern BOOL GetItemCategory(BYTE chValue, CString &strCategory);
extern BOOL GetItemCategory(WORD wItemType, BYTE &chCategory);

// Pass in a resource string for an item, get the ITM structure returned.
struct INF_ITM;
struct INF_SPL;

class CResInfo;
extern CResInfo* GetItem(const char *pszRes, INF_ITM *pItm);

// Used to pass CStrings along as a Doc/View hint.
class CHintString : public CObject
{
public:
	CHintString(const char *pszString)			{ str = pszString; }

	CString	str;
};

// Pass in a resource string for an item and it returns a handle to a bitmap
// for that item.
extern BOOL GetItemBitmap(const CResInfo *pInfo, INF_ITM &itm, int nFrameWanted, COLORREF clrTrans, HBITMAP &hBitmap);
extern BOOL GetSpellBitmap(const CResInfo *pInfo, INF_SPL &spl, COLORREF clrTrans, HBITMAP &hBitmap);

// Pass in a spell resource name like "SPWI102" and get back a string that corresponds
// in the SPELL.IDS list.
extern CString GetSpellScriptName(const char *pszResName);

// Pass in a resource string and it will try to load the BMP resource. If the
// bitmap doesn't exist as a resource it will try to load a file named that
// from the portraits directory.
// If nWidth and nHeight are non-zero this means you want the bitmap fit to
// that specific size.
extern BOOL GetResourceBitmap(const char *pszName, HBITMAP &hBitmap, int nWidth=0, int nHeight=0);

// Extracts a resource from the game and writes it to the path given. The path
// should be a valid directory name. The file created will be the type of 
// name of the resource followed by the resource extension.
extern BOOL ExtractResource(WORD wResourceType, const char *pszResource, const char *pszPath);

class CInfCreature;
extern BOOL SaveCreFile(CInfCreature *pCre, const char *pszFilename, CString &strSavedAs);

// When logging is enabled it opens/writes the string/closes the file.
extern void AddToLog(const char *pszString, BOOL bForceWrite = FALSE);
