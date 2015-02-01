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

#ifndef __INFCREATURE__
#define __INFCREATURE__

#pragma pack(push,1)

// Proficiencies are packed 3 bits each. Hi 3 bits is the 1st class of a
// dual class character.
#define HITRIBBLE(byte)					((unsigned char)((byte >> 3) & 0x07))
#define LOTRIBBLE(byte)					((unsigned char)(byte & 0x07))
#define MAKETRIBBLE(loTrib,hiTrib)	((unsigned char)((hiTrib << 3) | (loTrib & 0x07)))

#define CRE_STAT_DEAD			0x00000800
#define CRE_STAT_ACID_DEAD		0x00000400
#define CRE_STAT_FLAME_DEAD	0x00000200
#define CRE_STAT_EXPLODE_DEAD	0x00000100
#define CRE_STAT_STONE_DEAD	0x00000080
#define CRE_STAT_FROZEN_DEAD	0x00000040

struct INF_CRE
{
	char		chSignature[0x04];
	char		chVersion[0x04];
	DWORD		dwLongCreatureName;
	DWORD		dwShortCreatureName;

	// I'm not real positive about this. I created a bunch of characters
	// and of them all, only the dual-class character had any value for
	// this byte.
	BYTE		chDualClass;

	// Saw this in a message. Haven't tested it: 0x80 = Can't export, 0x08 = Can.
	BYTE		chExportable;

	char		chUnknown[0x02];

	DWORD		dwExpForKilling;
	DWORD		dwExp;
	DWORD		dwGold;
	DWORD		dwStateFlags;
	WORD		wCurrentHP;
	WORD		wBaseHP;
	WORD		wAnimationID;

	char		chUnknown1[0x02];

	BYTE		chMetalColor;
	BYTE		chMinorColor;
	BYTE		chMajorColor;
	BYTE		chSkinColor;
	BYTE		chLeatherColor;
	BYTE		chArmorColor;
	BYTE		chHairColor;

	char		chUnknown2;

	char		chSmallPortrait[8];
	char		chLargePortrait[8];
	BYTE		chReputation;
	BYTE		chHideInShadows;
	short		nAC1;
	short		nAC2;
	short		nACModCrushing;
	short		nACModMissile;
	short		nACModPiercing;
	short		nACModSlashing;
	char		chTHAC0;
	BYTE		chAttacks;
	BYTE		chSaveDeath;
	BYTE		chSaveWands;
	BYTE		chSavePoly;
	BYTE		chSaveBreath;
	BYTE		chSaveSpells;
	BYTE		chResistFire;
	BYTE		chResistCold;
	BYTE		chResistElectricity;
	BYTE		chResistAcid;
	BYTE		chResistMagic;
	BYTE		chResistMagicFire;
	BYTE		chResistMagicCold;
	BYTE		chResistSlashing;
	BYTE		chResistCrushing;
	BYTE		chResistPiercing;
	BYTE		chResistMissile;

	BYTE		chDetectIllusions;
	BYTE		chSetTraps;
	BYTE		chLore;
	BYTE		chOpenLocks;
	BYTE		chMoveSilently;
	BYTE		chFindTraps;
	BYTE		chPickPockets;
	BYTE		chFatique;
	BYTE		chIntoxication;
	BYTE		chLuck;

	// These proficienciens seem to be obsolete in BGII. Instead they are
	// tacked onto the end of the CRE with affect structs. I've made two
	// function to replace these: GetProf and SetProf. It uses the 
	// proficiency values from global.h.
	BYTE		chProfLargeSwords;
	BYTE		chProfSmallSwords;
	BYTE		chProfBows;
	BYTE		chProfSpears;
	BYTE		chProfAxes;
	BYTE		chProfMissiles;
	BYTE		chProfGreatSwords;
	BYTE		chProfDaggers;
	BYTE		chProfHalberds;
	BYTE		chProfMaces;
	BYTE		chProfFlails;
	BYTE		chProfHammers;
	BYTE		chProfClubs;
	BYTE		chProfQuarterstaffs;
	BYTE		chProfCrossbows; 

	char		chUnknown5[0x06];

	BYTE		chTracking;

	char		chUnknown6[0x20];

	DWORD		dwStrRefs[100];
	BYTE		chLevelFirstClass;
	BYTE		chLevelSecondClass;
	BYTE		chLevelThirdClass;

	char		chUnknown7;

	BYTE		chStrength;
	BYTE		chStrengthBonus;
	BYTE		chIntelligence;
	BYTE		chWisdom;
	BYTE		chDexterity;
	BYTE		chConstitution;
	BYTE		chCharisma;
	BYTE		chMorale;
	BYTE		chMoraleBreak;
	BYTE		chRacialEnemy;

	char		chUnknown8[0x02];

	DWORD		dwKit;
	char		chOverrideScript[8];
	char		chClassScript[8];
	char		chRaceScript[8];
	char		chGeneralScript[8];
	char		chDefaultScript[8];

	BYTE		chEnemyAlly;
	BYTE		chGeneral;
	BYTE		chRace;
	BYTE		chClass;
	BYTE		chSpecific;
	BYTE		chGender;

	char		chUnknown9[0x05];

	BYTE		chAlignment;

	char		chUnknown10[0x04];

	char		chName[0x20];
	DWORD		dwKnownSpellsOffset;
	DWORD		dwKnownSpellsCount;
	DWORD		dwSpellMemorizationInfoOffset;
	DWORD		dwSpellMemorizationInfoCount;
	DWORD		dwMemorizedSpellsOffset;
	DWORD		dwMemorizedSpellsCount;
	DWORD		dwItemSlotsOffset;
	DWORD		dwItemsOffset;
	DWORD		dwItemsCount;
	DWORD		dwAffectOffset;
	DWORD		dwAffectCount;

	char		chDialogRef[0x08];
};

#define AFF_TYPE_ABILITY			0x0A
#define AFF_TYPE_SPELL				0x7E		// Don't really have a clue what 7E is, but this is good for now.
#define AFF_TYPE_CONTINGENCY		0xE8
#define AFF_TYPE_PROF				0xE9

#define AFF_TARG_CRE					0x02

struct INF_AFF
{
	char		chSignature[4];			// Zeroed out for V2 EFFs.
	char		chVersion[4];				// Zeroed out for V2 EFFs.

	DWORD		dwAffectType;
	DWORD		dwTargetType;

	DWORD		dwUnknown;

	int		nNP1;
	int		nNP2;
	DWORD		dwFlags;
	int		nTime;
	WORD		wProbability1;
	WORD		wProbability2;
	char		chResRef0[8];
	DWORD		dwDieSides;
	DWORD		dwDieCount;
	DWORD		dwSaveType;
	DWORD		dwSaveBonus;
	DWORD		dwUnknown1[3];
	DWORD		dwDieSides2;
	DWORD		dwDieCount2;
	DWORD		dwFlags2;
	DWORD		dwUnknown2[4];
	char		chResRef1[8];
	char		chResRef2[8];
	DWORD		dwUnknown3[5];
	char		chResRef3[8];
	DWORD		dwUnknown4[3];		
	char		chName[32];			
	DWORD		dwUnknown5[18];	
};

// Spell types as defined in the CRE spell data. Note that it is also important that
// the memorized section of spells should be written out in this same order. The
// memorization info block has indices to the start of each particular spell level
// following the memorization info block. It might work to put them out of order as
// long as you figured out the indices properly for the info block, but I've used a
// simple loop to calculate them, and it's based on writing them out in the order of
// priest, wizard, innate (which is the same order the game uses).
#define INF_CRE_SPELLTYPES		3		// Number of known types.
#define INF_CRE_ST_PRIEST		0
#define INF_CRE_ST_WIZARD		1
#define INF_CRE_ST_INNATE		2

struct INF_CRE_KNOWNSPELL
{
	char		chSpellName[8];
	WORD		wSpellLevel;
	WORD		wSpellType;
};

struct INF_CRE_MEMINFO
{
	WORD		wSpellLevel;
	WORD		wNumMemorizable1;
	WORD		wNumMemorizable2;
	WORD		wSpellType;
	DWORD		dwSpellIndex;
	DWORD		dwSpellCount;
};

struct INF_CRE_MEMSPELL
{
	char		chSpellName[8];
	DWORD		dwMemorized;		// 00 = not memorized, 01 = memorized.
};

#define INF_NUM_ITEMSLOTS			38

#define INF_ITEMPOS_HEAD			0
#define INF_ITEMPOS_BODY			1
#define INF_ITEMPOS_SHIELD			2
#define INF_ITEMPOS_HANDS			3
#define INF_ITEMPOS_FINGER1		4
#define INF_ITEMPOS_FINGER2		5
#define INF_ITEMPOS_NECK			6
#define INF_ITEMPOS_WAIST			7
#define INF_ITEMPOS_FEET			8
#define INF_ITEMPOS_QUICKWEAPON1	9
#define INF_ITEMPOS_QUICKWEAPON2	10
#define INF_ITEMPOS_QUICKWEAPON3	11
#define INF_ITEMPOS_QUICKWEAPON4	12
#define INF_ITEMPOS_QUIVER1		13
#define INF_ITEMPOS_QUIVER2		14
#define INF_ITEMPOS_QUIVER3		15
#define INF_ITEMPOS_UNUSED			16
#define INF_ITEMPOS_BACK			17
#define INF_ITEMPOS_QUICKSLOT1	18
#define INF_ITEMPOS_QUICKSLOT2	19
#define INF_ITEMPOS_QUICKSLOT3	20
#define INF_ITEMPOS_INV				21		// 16 of these total.

struct INF_CRE_ITEMSLOTS
{
	WORD		wItemSlot[INF_NUM_ITEMSLOTS];
	char		chUnknown[4];
};

struct INF_CRE_ITEM
{
	char		chResName[8];
	char		chUnknown[2];
	WORD		wQuantity1;
	WORD		wQuantity2;
	WORD		wQuantity3;
	char		chIdentified;	// 00 = not ID'd, 01 = ID'd.
	char		chUknown1[3];
};

// This is used by the CInfCre object to hold the spell data. The original
// CRE spell info is read in and converted to a structure of this type. Also,
// this is how it passes data out of the object regarding spells and expects
// data to be passed back in in this format.	This data is memcpy'd so don't
// put any object in it.
struct SPELLDATA
{
	char	szResName[9];
	WORD	wLevel;
	WORD	wType;

	// This is the number of times the creature has the spell set to be memorized.
	int	nTimesMemorized;

	// This is the number of times the creature can still cast the spell. Before
	// they need to rest.
	int	nTimesCanCast;
};

// Used as an intermediate format for the retrieval and setting of the memorization
// maximums for each spell.
struct MEMINFO
{
	WORD	wType;
	WORD	wLevel;
	WORD	wNumMemorizable;
};

// Used for transferring lists of proficiencies in and out of the class.
class CProfData : public CObject
{
public:
	CProfData()	
	{ m_chProf = 0; m_nFirstClass = 0; m_nSecondClass = 0; }

	BYTE	m_chProf;
	int	m_nFirstClass;
	int	m_nSecondClass;
};

#pragma pack(pop)


#define	ERR_CRE_READHEADER			1001
#define	ERR_CRE_READBADKNOWN			1002
#define	ERR_CRE_READTOOMANYSPELLS	1003
#define	ERR_CRE_READKNOWN				1004
#define	ERR_CRE_READBADMEMINFO		1005
#define	ERR_CRE_READTOOMANYMEMINFO	1006
#define	ERR_CRE_READMEMINFO			1007
#define	ERR_CRE_READBADMEM			1008
#define	ERR_CRE_READTOOMANYMEM		1009
#define	ERR_CRE_READMEM				1010
#define	ERR_CRE_READBADITEMSLOTS	1011
#define	ERR_CRE_READITEMSLOTS		1012
#define	ERR_CRE_READBADITEMLIST		1013
#define	ERR_CRE_READTOOMANYITEMS	1014
#define	ERR_CRE_READITEM				1015
#define	ERR_CRE_WRITEHEADER			1016
#define	ERR_CRE_WRITEKNOWNSPELLS	1017
#define	ERR_CRE_WRITEMEMINFO			1018
#define	ERR_CRE_WRITEMEMSPELLS		1019
#define	ERR_CRE_WRITEITEMSLOTS		1020
#define	ERR_CRE_WRITEITEMS			1021
#define	ERR_CRE_OOM_AFFECTS			1022
#define	ERR_CRE_READAFFECTS			1023
#define	ERR_CRE_WRITEAFFECTS			1024
#define	ERR_CRE_MISSINGSIG			1025
#define	ERR_CRE_BADVERSION			1026
#define	ERR_CRE_OOM_KNOWNSPELLS		1027
#define	ERR_CRE_OOM_MEMINFO			1028
#define	ERR_CRE_OOM_MEMSPELL			1029
#define	ERR_CRE_UNKNOWNSPELLTYPE	1030
#define	ERR_CRE_OOM_SPELLDATA		1031
#define	ERR_CRE_READBADAFFECTS		1032
#define	ERR_CRE_BADAFFECTCOUNT		1033
#define	ERR_CRE_WRITENOTKNOWNSPELL	1034

class CInfCreature
{
public:
	CInfCreature();
	~CInfCreature();

	// Read the CRE record from the file. The file position must already be
	// placed in the right spot.
	BOOL	Read(CFile &file);
													 
	// Writes the current data to the file.
	BOOL	Write(CFile &file);

	int	GetLastError()						{ return(m_nError); }

	// Returns the number of bytes needed to store the information on disk.
	// This value is used by CInfGame to create the header entries when 
	// recreating the Icewind.gam file.
	UINT	GetFileSpace();

	// Returns the resource string for the large character portrait.
	void	GetResLargePortrait(CString &strRes);

	void	GetName(CString &strName);
	void	SetName(const char *pszName);

	DWORD GetLongNameID()					{ return(m_infCre.dwLongCreatureName); }
	void	SetLongNameID(DWORD dwValue);
	DWORD GetShortNameID()					{ return(m_infCre.dwShortCreatureName); }
	void	SetShortNameID(DWORD dwValue);

	BYTE	GetDualClass()						{ return(m_infCre.chDualClass); }
	void	SetDualClass(int nValue=0x08);

	BYTE	GetFirstClassLevel()				{ return(m_infCre.chLevelFirstClass); }
	void	SetFirstClassLevel(int nValue);
	BYTE	GetSecondClassLevel()			{ return(m_infCre.chLevelSecondClass); }
	void	SetSecondClassLevel(int nValue);
	BYTE	GetThirdClassLevel()				{ return(m_infCre.chLevelThirdClass); }
	void	SetThirdClassLevel(int nValue);

	BYTE	GetStr()								{ return(m_infCre.chStrength); }
	void	SetStr(int nStr);
	BYTE	GetStrBonus()						{ return(m_infCre.chStrengthBonus); }
	void	SetStrBonus(int nBonus);
	BYTE	GetDex()								{ return(m_infCre.chDexterity); }
	void	SetDex(int nDex);
	BYTE	GetCon()								{ return(m_infCre.chConstitution); }
	void	SetCon(int nCon);
	BYTE	GetInt()								{ return(m_infCre.chIntelligence); }
	void	SetInt(int nInt);
	BYTE	GetWis()								{ return(m_infCre.chWisdom); }
	void	SetWis(int nWis);
	BYTE	GetCha()								{ return(m_infCre.chCharisma); }
	void	SetCha(int nCha);

	BYTE	GetAttacks()						{ return(m_infCre.chAttacks); }
	void	SetAttacks(int nValue);
	short	GetAC()								{ return(m_infCre.nAC1); }
	void	SetAC(int nValue);	
	char	GetThac0()							{ return(m_infCre.chTHAC0); }
	void	SetThac0(int nValue);

	DWORD	GetExp()								{ return(m_infCre.dwExp); }
	void	SetExp(DWORD dwExp);
	DWORD GetExpForKill()					{ return(m_infCre.dwExpForKilling); }
	void  SetExpForKill(DWORD dwExp);
	DWORD	GetGold()							{ return(m_infCre.dwGold); }
	void	SetGold(DWORD dwGold);

	WORD	GetBaseHP()							{ return(m_infCre.wBaseHP); }
	void	SetBaseHP(WORD wBaseHP);
	WORD	GetCurrentHP()						{ return(m_infCre.wCurrentHP); }
	void	SetCurrentHP(WORD wCurrentHP);
	BYTE	GetLore()							{ return(m_infCre.chLore); }
	void	SetLore(int nValue);

	// The reputation value seems to be 10x what is shown in the game.
	BYTE	GetReputation()					{ return(m_infCre.chReputation/10); }
	void	SetReputation(int nValue);

	BYTE	GetSaveDeath()						{ return(m_infCre.chSaveDeath); }
	void	SetSaveDeath(int nValue);
	BYTE	GetSaveWands()						{ return(m_infCre.chSaveWands); }
	void	SetSaveWands(int nValue);
	BYTE	GetSavePoly()						{ return(m_infCre.chSavePoly); }
	void	SetSavePoly(int nValue);
	BYTE	GetSaveBreath()					{ return(m_infCre.chSaveBreath); }
	void	SetSaveBreath(int nValue);
	BYTE	GetSaveSpells()					{ return(m_infCre.chSaveSpells); }
	void	SetSaveSpells(int nValue);

	BYTE	GetProfLargeSwords()				{ return(LOTRIBBLE(m_infCre.chProfLargeSwords)); }
	void	SetProfLargeSwords(int nValue);
	BYTE	GetProfSmallSwords()				{ return(LOTRIBBLE(m_infCre.chProfSmallSwords)); }
	void	SetProfSmallSwords(int nValue);
	BYTE	GetProfBows()						{ return(LOTRIBBLE(m_infCre.chProfBows)); }
	void	SetProfBows(int nValue);
	BYTE	GetProfSpears()					{ return(LOTRIBBLE(m_infCre.chProfSpears)); }
	void	SetProfSpears(int nValue);
	BYTE	GetProfClubs()						{ return(LOTRIBBLE(m_infCre.chProfClubs)); }
	void	SetProfClubs(int nValue);
	BYTE	GetProfFlails()					{ return(LOTRIBBLE(m_infCre.chProfFlails)); }
	void	SetProfFlails(int nValue);
	BYTE	GetProfAxes()						{ return(LOTRIBBLE(m_infCre.chProfAxes)); }
	void	SetProfAxes(int nValue);
	BYTE	GetProfMissiles()					{ return(LOTRIBBLE(m_infCre.chProfMissiles)); }
	void	SetProfMissiles(int nValue);
	BYTE	GetProfGreatSwords()				{ return(LOTRIBBLE(m_infCre.chProfGreatSwords)); }
	void	SetProfGreatSwords(int nValue);
	BYTE	GetProfDaggers()					{ return(LOTRIBBLE(m_infCre.chProfDaggers)); }
	void	SetProfDaggers(int nValue);
	BYTE	GetProfHalberds()					{ return(LOTRIBBLE(m_infCre.chProfHalberds)); }
	void	SetProfHalberds(int nValue);
	BYTE	GetProfMaces()						{ return(LOTRIBBLE(m_infCre.chProfMaces)); }
	void	SetProfMaces(int nValue);
	BYTE	GetProfCrossbows()				{ return(LOTRIBBLE(m_infCre.chProfCrossbows)); }
	void	SetProfCrossbows(int nValue);
	BYTE	GetProfQuarterStaffs()			{ return(LOTRIBBLE(m_infCre.chProfQuarterstaffs)); }
	void	SetProfQuarterStaffs(int nValue);
	BYTE	GetProfHammers()					{ return(LOTRIBBLE(m_infCre.chProfHammers)); }
	void	SetProfHammers(int nValue);

	BYTE	GetProfLargeSwords2()				{ return(HITRIBBLE(m_infCre.chProfLargeSwords)); }
	void	SetProfLargeSwords2(int nValue);
	BYTE	GetProfSmallSwords2()				{ return(HITRIBBLE(m_infCre.chProfSmallSwords)); }
	void	SetProfSmallSwords2(int nValue);
	BYTE	GetProfBows2()							{ return(HITRIBBLE(m_infCre.chProfBows)); }
	void	SetProfBows2(int nValue);
	BYTE	GetProfSpears2()						{ return(HITRIBBLE(m_infCre.chProfSpears)); }
	void	SetProfSpears2(int nValue);
	BYTE	GetProfClubs2()						{ return(HITRIBBLE(m_infCre.chProfClubs)); }
	void	SetProfClubs2(int nValue);
	BYTE	GetProfFlails2()						{ return(HITRIBBLE(m_infCre.chProfFlails)); }
	void	SetProfFlails2(int nValue);
	BYTE	GetProfAxes2()							{ return(HITRIBBLE(m_infCre.chProfAxes)); }
	void	SetProfAxes2(int nValue);
	BYTE	GetProfMissiles2()					{ return(HITRIBBLE(m_infCre.chProfMissiles)); }
	void	SetProfMissiles2(int nValue);
	BYTE	GetProfGreatSwords2()				{ return(HITRIBBLE(m_infCre.chProfGreatSwords)); }
	void	SetProfGreatSwords2(int nValue);
	BYTE	GetProfDaggers2()						{ return(HITRIBBLE(m_infCre.chProfDaggers)); }
	void	SetProfDaggers2(int nValue);
	BYTE	GetProfHalberds2()					{ return(HITRIBBLE(m_infCre.chProfHalberds)); }
	void	SetProfHalberds2(int nValue);
	BYTE	GetProfMaces2()						{ return(HITRIBBLE(m_infCre.chProfMaces)); }
	void	SetProfMaces2(int nValue);
	BYTE	GetProfCrossbows2()					{ return(HITRIBBLE(m_infCre.chProfCrossbows)); }
	void	SetProfCrossbows2(int nValue);
	BYTE	GetProfQuarterStaffs2()				{ return(HITRIBBLE(m_infCre.chProfQuarterstaffs)); }
	void	SetProfQuarterStaffs2(int nValue);
	BYTE	GetProfHammers2()						{ return(HITRIBBLE(m_infCre.chProfHammers)); }
	void	SetProfHammers2(int nValue);

	BYTE	GetResFire()						{ return(m_infCre.chResistFire); }
	void	SetResFire(int nValue);
	BYTE	GetResCold()						{ return(m_infCre.chResistCold); }
	void	SetResCold(int nValue);
	BYTE	GetResElectricity()				{ return(m_infCre.chResistElectricity); }
	void	SetResElectricity(int nValue);
	BYTE	GetResAcid()						{ return(m_infCre.chResistAcid); }
	void	SetResAcid(int nValue);
	BYTE	GetResMagic()						{ return(m_infCre.chResistMagic); }
	void	SetResMagic(int nValue);
	BYTE	GetResMagicFire()					{ return(m_infCre.chResistMagicFire); }
	void	SetResMagicFire(int nValue);
	BYTE	GetResMagicCold()					{ return(m_infCre.chResistMagicCold); }
	void	SetResMagicCold(int nValue);
	BYTE	GetResSlashing()					{ return(m_infCre.chResistSlashing); }
	void	SetResSlashing(int nValue);
	BYTE	GetResCrushing()					{ return(m_infCre.chResistCrushing); }
	void	SetResCrushing(int nValue);
	BYTE	GetResPiercing()					{ return(m_infCre.chResistPiercing); }
	void	SetResPiercing(int nValue);
	BYTE	GetResMissile()					{ return(m_infCre.chResistMissile); }
	void	SetResMissile(int nValue);

	BYTE	GetOpenLocks()						{ return(m_infCre.chOpenLocks); }
	void	SetOpenLocks(int nValue);
	BYTE	GetMoveSilently()					{ return(m_infCre.chMoveSilently); }
	void	SetMoveSilently(int nValue);
	BYTE	GetFindTraps()						{ return(m_infCre.chFindTraps); }
	void	SetFindTraps(int nValue);
	BYTE	GetPickPockets()					{ return(m_infCre.chPickPockets); }
	void	SetPickPockets(int nValue);
	BYTE	GetHideInShadows()				{ return(m_infCre.chHideInShadows); }
	void	SetHideInShadows(int nValue);
	BYTE	GetDetectIllusions()				{ return(m_infCre.chDetectIllusions); }
	void	SetDetectIllusions(int nValue);
	BYTE	GetSetTraps()						{ return(m_infCre.chSetTraps); }
	void  SetSetTraps(int nValue);

	BYTE	GetRace()							{ return(m_infCre.chRace); }
	void	SetRace(int nValue);
	BYTE  GetGender()							{ return(m_infCre.chGender); }
	void	SetGender(int nValue);
	BYTE	GetAlignment()						{ return(m_infCre.chAlignment); }
	void	SetAlignment(int nValue);
	BYTE	GetClass()							{ return(m_infCre.chClass); }
	void	SetClass(int nValue);
	DWORD	GetKit()								{ return(m_infCre.dwKit); }
	void	SetKit(DWORD dwValue);
	BYTE	GetRacialEnemy()					{ return(m_infCre.chRacialEnemy); }
	void	SetRacialEnemy(int nValue);
	BYTE	GetEnemyAlly()						{ return(m_infCre.chEnemyAlly); }
	void	SetEnemyAlly(int nValue);

	WORD	GetAnimationID()					{ return(m_infCre.wAnimationID); }
	void	SetAnimationID(WORD wValue);

	DWORD GetStateFlags()					{ return(m_infCre.dwStateFlags); }
	void  SetStateFlags(DWORD dwValue);

	void	GetSmallPortrait(CString &strResource);
	void	SetSmallPortrait(const char *pszResource);
	void	GetLargePortrait(CString &strResource);
	void	SetLargePortrait(const char *pszResource);

	BYTE	GetSkinColor()						{ return(m_infCre.chSkinColor); }
	void	SetSkinColor(BYTE chColor);
	BYTE	GetHairColor()						{ return(m_infCre.chHairColor); }
	void	SetHairColor(BYTE chColor);
	BYTE	GetMajorColor()					{ return(m_infCre.chMajorColor); }
	void	SetMajorColor(BYTE chColor);
	BYTE	GetMinorColor()					{ return(m_infCre.chMinorColor); }
	void	SetMinorColor(BYTE chColor);
	BYTE	GetArmorColor()					{ return(m_infCre.chArmorColor); }
	void	SetArmorColor(BYTE chColor);
	BYTE	GetMetalColor()					{ return(m_infCre.chMetalColor); }
	void	SetMetalColor(BYTE chColor);
	BYTE	GetLeatherColor()					{ return(m_infCre.chLeatherColor); }
	void	SetLeatherColor(BYTE chColor);

	// Returns the movement modifier of a character. These are actually character
	// affects. If it exists the value is returned, if not zero is.
	DWORD	GetSpeed();

	// Sets the character speed affect. If Zero is passed in the affect is removed.
	void	SetSpeed(DWORD dwSpeed);

	// Gets a list proficiencies from teh affects list. The passed in list will
	// be filled with CProfData objects. The list should be empty before being
	// passed in. The function will just add the profs to the tail of the list
	// if you don't clear it first. The caller is responsible for deleting the
	// objects in the list.
	void GetProfs(CObList &list);

	// Pass in a list of CProfData objects. Creates the affect records for the
	// CRE. All the items in the passed in are deleted and the list is cleared.
	void SetProfs(CObList &list);

	// Get a list of all the character affects. It will not return the proficiencies
	// or the speed affect since those are handled separately.
	BOOL GetAffects(CPtrList &list);

	// Set the list of affects for a character. All existing affects are deleted and
	// this list is used. This does not affect the proficiencies or the move speed affect.
	// This function does not make copies of the data. It retains the pointers passed
	// in via the list. Do not delete the items in this list. The list will be cleared
	// once the items are transferred.
	void SetAffects(CPtrList &list);

	// Needs to be a pointer to an array of INF_NUM_ITEMSLOTS items. This returns a fully
	// expanded copy of the data. Meaning if there are only 10 items in the file there
	// will still be returned a full INF_NUM_ITEMSLOTS array. Each of the items is
	// placed in the array according to the positions in the INF_CRE_ITEMSLOTS. Positions
	// that don't have an item are set to zero.
	void	GetItems(INF_CRE_ITEM *pItems);

	// Should be passed a full INF_NUM_ITEMSLOTS array. Any items that have a non-null
	// resource string will be placed in the characters inventory. Calling this function
	// also sets up the INF_CRE_ITEMSLOTS data.
	void	SetItems(const INF_CRE_ITEM *pItems);

	// Returns the number of spells currently of a particular type. There is obviously
	// no range checking done on this function, so make sure you use the INF_CRE_ST_
	// defines.
	int	GetSpellCount(int nType)		{ return(m_nSpellCount[nType]); }

	// Gets a copy of the spell data. This is a copy of the internal data so pData needs
	// to point to a buffer big enough to hold as many items as GetSpellCount returns.
	void	GetSpells(int nType, SPELLDATA *pData);

	// Sets a list of spells for a particular type. Calling this function copies the data
	// pointed to by pData to internal buffers.
	void	SetSpells(int nType, int nSpellCount, const SPELLDATA *pData);

	int	GetMemorizationInfoCount()		{ return(m_infCre.dwSpellMemorizationInfoCount); }

	// Gets a copy of the memorization maximums. pMemInfo must point to a buffer big enough
	// to hold as many items as GetMemorizationInfoCount returns.
	void	GetMemorizationInfo(MEMINFO *pMemInfo);

	// Must point to an array of GetMemorizationInfoCount size.
	void	SetMemorizationInfo(const MEMINFO *pMemInfo);

	BOOL	HasChanged()						{ return(m_bHasChanged); }

private:
	// Called before calculation or writing any spell information. This function updates
	// the CRE header and recalculates all the data in the memorization info buffer.
	void	UpdateSpellInfo();

private:
	INF_CRE					m_infCre;
	INF_CRE_KNOWNSPELL	*m_pKnownSpells;
	INF_CRE_MEMINFO		*m_pMemInfo;
	INF_CRE_MEMSPELL		*m_pMemSpells;
	INF_CRE_ITEMSLOTS		m_infItemSlots;
	INF_CRE_ITEM			m_infItems[INF_NUM_ITEMSLOTS];	// Can only be as many as there are item slots.
	SPELLDATA				*m_pSpellData[INF_CRE_SPELLTYPES];
	int						m_nSpellCount[INF_CRE_SPELLTYPES];
	CPtrList					m_plAffects;

	// It appears characters can have spells memorized that they don't actually 
	// know. In particular, it looks like when a character casts a spell sequencer
	// a different version of the spell gets added to the morized list. SPWI420
	// I think is the spell, while when you use it, you end up with a SPWI420D in
	// your memorized list. Previously, I assumed all spells in the memorized list
	// had a matching "Known Spell" entry. Any spells in this list get recreated
	// only in the memorized section when writing the CRE. I put them in a separate
	// list to avoid letting the user seem them in the spell boxes and to make life
	// easier on the alrady existing spell code. It seems to be counting these as
	// innate abilities too. Game locks up if you don't count them in the memorization
	// block as an innate.
	CPtrList					m_plMemButNotKnown;

	BOOL	m_bHasChanged;
	int	m_nError;
};

#endif