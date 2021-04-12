#pragma once

#include <Windows.h>
#include <cstdint>
#include "D2Constants.h"

struct ItemsTxt
{
	char szFlippyFile[32];					//0x00
	char szInvFile[32];						//0x20
	char szUniqueInvFile[32];				//0x40
	char szSetInvFile[32];					//0x60
	union
	{
		uint32_t dwCode;					//0x80
		char szCode[4];						//0x80
	};
	uint32_t dwNormCode;					//0x84
	uint32_t dwUberCode;					//0x88
	uint32_t dwUltraCode;					//0x8C
	uint32_t dwAlternateGfx;				//0x90
	uint32_t dwPspell;						//0x94
	uint16_t wState;						//0x98
	uint16_t wCurseState[2];				//0x9A
	uint16_t wStat[3];						//0x9E
	uint32_t dwCalc[3];						//0xA4
	uint32_t dwLen;							//0xB0
	uint8_t nSpellDesc;						//0xB4
	uint8_t pad0xB5;						//0xB5
	uint16_t wSpellDescStr;					//0xB6
	uint32_t dwSpellDescCalc;				//0xB8
	uint32_t dwBetterGem;					//0xBC
	uint32_t dwWeapClass;					//0xC0
	uint32_t dwWeapClass2Hand;				//0xC4
	uint32_t dwTransmogrifyType;			//0xC8
	int32_t dwMinAc;						//0xCC
	int32_t dwMaxAc;						//0xD0
	uint32_t dwGambleCost;					//0xD4
	int32_t dwSpeed;						//0xD8
	uint32_t dwBitField1;					//0xDC
	uint32_t dwCost;						//0xE0
	uint32_t dwMinStack;					//0xE4
	uint32_t dwMaxStack;					//0xE8
	uint32_t dwSpawnStack;					//0xEC
	uint32_t dwGemOffset;					//0xF0
	uint16_t wNameStr;						//0xF4
	uint16_t wVersion;						//0xF6
	uint16_t wAutoPrefix;					//0xF8
	uint16_t wMissileType;					//0xFA
	uint8_t nRarity;						//0xFC
	uint8_t nLevel;							//0xFD
	uint8_t nMinDam;						//0xFE
	uint8_t nMaxDam;						//0xFF
	uint8_t nMinMisDam;						//0x100
	uint8_t nMaxMisDam;						//0x101
	uint8_t n2HandMinDam;					//0x102
	uint8_t n2HandMaxDam;					//0x103
	uint8_t nRangeAdder;					//0x104
	uint8_t unk0x105;						//0x105
	int16_t nStrBonus;						//0x106
	int16_t nDexBonus;						//0x108
	uint16_t wReqStr;						//0x10A
	uint16_t wReqDex;						//0x10C
	uint8_t nAbsorb;						//0x10E
	uint8_t nInvWidth;						//0x10F
	uint8_t nInvHeight;						//0x110
	uint8_t nBlock;							//0x111
	uint8_t nDurability;					//0x112
	uint8_t nNoDurability;					//0x113
	uint8_t nMissile;						//0x114
	uint8_t nComponent;						//0x115
	uint8_t nArmorComp[6];					//0x116
	uint8_t n2Handed;						//0x11C
	uint8_t nUseable;						//0x11D
	ItemType wType[2];						//0x11E
	uint8_t nSubType;						//0x122
	uint8_t unk0x123;						//0x123
	uint16_t wDropSound;					//0x124
	uint16_t wUseSound;						//0x126
	uint8_t nDropSfxFrame;					//0x128
	uint8_t nUnique;						//0x129
	uint8_t nQuest;							//0x12A
	uint8_t nQuestDiffCheck;				//0x12B
	uint8_t nTransparent;					//0x12C
	uint8_t nTransTbl;						//0x12D
	uint8_t pad0x12E;						//0x12E
	uint8_t nLightRadius;					//0x12F
	uint8_t nBelt;							//0x130
	uint8_t nAutoBelt;						//0x131
	uint8_t nStackable;						//0x132
	uint8_t nSpawnable;						//0x133
	uint8_t nSpellIcon;						//0x134
	uint8_t nDurWarning;					//0x135
	uint8_t nQuantityWarning;				//0x136
	uint8_t nHasInv;						//0x137
	uint8_t nGemSockets;					//0x138
	uint8_t nTransmogrify;					//0x139
	uint8_t nTmogMin;						//0x13A
	uint8_t nTmogMax;						//0x13B
	uint8_t nHitClass;						//0x13C
	uint8_t n1or2Handed;					//0x13D
	uint8_t nGemApplyType;					//0x13E
	uint8_t nLevelReq;						//0x13F
	uint8_t nMagicLevel;					//0x140
	int8_t nTransform;						//0x141
	int8_t nInvTrans;						//0x142
	uint8_t nCompactSave;					//0x143
	uint8_t nSkipName;						//0x144
	uint8_t nNameable;						//0x145
	uint8_t nVendorMin[17];					//0x146
	uint8_t nVendorMax[17];					//0x157
	uint8_t nVendorMagicMin[17];			//0x168
	uint8_t nVendorMagicMax[17];			//0x179
	uint8_t nVendorMagicLvl[17];			//0x18A
	uint8_t pad0x19B;						//0x19B
	uint32_t dwNightmareUpgrade;			//0x19C
	uint32_t dwHellUpgrade;					//0x1A0
	uint8_t nPermStoreItem;					//0x1A4
	uint8_t nMultibuy;						//0x1A5
	uint16_t pad0x1A6;						//0x1A6
};

struct SkillDescTxt							// sgptDataTable + 0xB8C
{
	uint16_t wSkillDesc;					//0x00
	uint8_t nSkillPage;					//0x02
	uint8_t nSkillRow;						//0x03
	uint8_t nSkillColumn;					//0x04
	uint8_t nListRow;						//0x05
	uint8_t nListPool;						//0x06
	uint8_t nIconCel;						//0x07
	uint16_t wStrName;						//0x08
	uint16_t wStrShort;						//0x0A
	uint16_t wStrLong;						//0x0C
	uint16_t wStrAlt;						//0x0E
	uint16_t wStrMana;						//0x10
	uint16_t wDescDam;						//0x12
	uint16_t wDescAtt;						//0x14
	uint16_t pad0x16;						//0x16
	uint32_t dwDamCalc[2];					//0x18
	uint8_t nPrgDamElem[3];				//0x20
	uint8_t pad0x23;						//0x23
	uint32_t dwProgDmgMin[3];				//0x24
	uint32_t dwProgDmgMax[3];				//0x30
	uint16_t wDescMissile[3];				//0x3C
	uint8_t nDescLine[17];					//0x42
	uint8_t pad0x53;						//0x53
	uint16_t wDescTextA[17];				//0x54
	uint16_t wDescTextB[17];				//0x76
	uint32_t dwDescCalcA[17];				//0x98
	uint32_t dwDescCalcB[17];				//0xDC
};

struct SkillsTxt								//sgptDataTable + 0xB98
{
	int16_t nSkillId; 						//0x00
	uint16_t unk0x02;						//0x02
	uint32_t dwFlags[2];					//0x04
	int8_t nCharClass;						//0x0C
	uint8_t unk0x0D[3];						//0x0D
	uint8_t nAnim;							//0x10
	uint8_t nMonAnim;						//0x11
	uint8_t nSeqTrans;						//0x12
	uint8_t nSeqNum;						//0x13
	uint8_t nRange;							//0x14
	uint8_t nSelectProc;					//0x15
	uint8_t nSeqInput;						//0x16
	uint8_t pad0x17;						//0x17
	int16_t nITypeA[3];						//0x18
	int16_t nITypeB[3];						//0x1E
	int16_t nETypeA[2];						//0x24
	int16_t nETypeB[2];						//0x28
	uint16_t wSrvStartFunc;					//0x2C
	uint16_t wSrvDoFunc;					//0x2E
	uint16_t wSrvPrgFunc[3];				//0x30
	uint16_t pad0x36;						//0x36
	uint32_t dwPrgCalc[3];					//0x38
	uint8_t nPrgDamage;						//0x44
	uint8_t pad0x45;						//0x45
	uint16_t wSrvMissile;					//0x46
	uint16_t wSrvMissileA;					//0x48
	uint16_t wSrvMissileB;					//0x4A
	uint16_t wSrvMissileC;					//0x4C
	uint16_t wSrvOverlay;					//0x4E
	uint32_t dwAuraFilter;					//0x50
	uint16_t wAuraStat[6];					//0x54
	uint32_t dwAuraLenCalc;					//0x60
	uint32_t dwAuraRangeCalc;				//0x64
	int32_t dwAuraStatCalc[6];				//0x68
	int16_t nAuraState;						//0x80
	int16_t wAuraTargetState;				//0x82
	uint16_t wAuraEvent[3];					//0x84
	uint16_t wAuraEventFunc[3];				//0x8A
	uint16_t wAuraTgtEvent;					//0x90
	uint16_t wAuraTgtEventFunc;				//0x92
	int16_t nPassiveState;					//0x94
	int16_t nPassiveIType;					//0x96
	int16_t nPassiveStat[5];				//0x98
	uint16_t pad0xA2;						//0xA2
	uint32_t dwPassiveCalc[5];				//0xA4
	uint16_t wPassiveEvent;					//0xB8
	uint16_t wPassiveEventFunc;				//0xBA
	uint16_t wSummon;						//0xBC
	uint8_t nPetType;						//0xBE
	uint8_t nSumMode;						//0xBF
	uint32_t dwPetMax;						//0xC0
	uint16_t wSumSkill[5];					//0xC4
	uint16_t pad0xCE;						//0xCE
	uint32_t dwSumSkCalc[5];				//0xD0
	uint16_t wSumUMod;						//0xE4
	uint16_t wSumOverlay;					//0xE6
	uint16_t wCltMissile;					//0xE8
	uint16_t wCltMissileA;					//0xEA
	uint16_t wCltMissileB;					//0xEC
	uint16_t wCltMissileC;					//0xEE
	uint16_t wCltMissileD;					//0xF0
	uint16_t wCltStFunc;					//0xF2
	uint16_t wCltDoFunc;					//0xF4
	uint16_t wCltPrgFunc[3];				//0xF6
	uint16_t wStSound;						//0xFC
	uint16_t nStSoundClass;					//0x0FE	
	uint16_t wDoSound;						//0x100
	uint16_t wDoSoundA;						//0x102
	uint16_t wDoSoundB;						//0x104
	uint16_t wCastOverlay;					//0x106
	uint16_t wTgtOverlay;					//0x108
	uint16_t wTgtSound;						//0x10A
	uint16_t wPrgOverlay;					//0x10C
	uint16_t wPrgSound;						//0x10E
	uint16_t wCltOverlayA;					//0x110
	uint16_t wCltOverlayB;					//0x112
	int32_t dwCltCalc[3];					//0x114
	uint8_t nItemTarget;					//0x120
	uint8_t pad0x121;						//0x121
	uint16_t wItemCastSound;				//0x122
	uint16_t wItemCastOverlay;				//0x124
	uint16_t pad0x126;						//0x126
	uint32_t dwPerDelay;					//0x128
	uint16_t wMaxLvl;						//0x12C
	uint16_t wResultFlags;					//0x12E
	uint32_t dwHitFlags;					//0x130
	uint32_t dwHitClass;					//0x134
	uint32_t dwCalc[4];						//0x138
	int32_t dwParam[8];						//0x148
	uint8_t nWeapSel;						//0x168
	uint8_t pad0x169;						//0x169
	uint16_t wItemEffect;					//0x16A
	uint16_t wItemCltEffect;				//0x16C
	uint16_t pad0x16E;						//0x16E
	uint32_t dwSkPoints;					//0x170
	uint16_t wReqLevel;						//0x174
	uint16_t wReqStr;						//0x176
	uint16_t wReqDex;						//0x178
	uint16_t wReqInt;						//0x17A
	uint16_t wReqVit;						//0x17C
	int16_t nReqSkill[3];					//0x17E
	uint16_t wStartMana;					//0x184
	uint16_t wMinMana;						//0x186
	uint8_t nManaShift;						//0x188
	uint8_t pad0x189;						//0x189
	uint16_t wMana;							//0x18A
	int16_t wLevelMana;						//0x18C
	uint8_t nAttackRank;					//0x18E
	uint8_t nLineOfSight;					//0x18F
	uint32_t dwDelay;						//0x190
	uint16_t wSkillDesc;					//0x194
	uint16_t pad0x196;						//0x196
	uint32_t dwToHit;						//0x198
	uint32_t dwLevToHit;					//0x19C
	uint32_t dwToHitCalc;					//0x1A0
	uint8_t nToHitShift;					//0x1A4
	uint8_t nSrcDam;						//0x1A5
	uint16_t pad0x1A6;						//0x1A6
	uint32_t dwMinDam;						//0x1A8
	uint32_t dwMaxDam;						//0x1AC
	uint32_t dwMinLvlDam[5];				//0x1B0
	uint32_t dwMaxLvlDam[5];				//0x1C4
	uint32_t dwDmgSymPerCalc;				//0x1D8
	uint8_t nEType;							//0x1DC
	uint8_t pad0x1DD[3];					//0x1DD
	uint32_t dwEMin;						//0x1E0
	uint32_t dwEMax;						//0x1E4
	uint32_t dwEMinLev[5];					//0x1E8
	uint32_t dwEMaxLev[5];					//0x1FC
	uint32_t dwEDmgSymPerCalc;				//0x210
	uint32_t dwELen;						//0x214
	uint32_t dwELevLen[3];					//0x218
	uint32_t dwELenSymPerCalc;				//0x224
	uint8_t nRestrict;						//0x228
	uint8_t pad0x229;						//0x229
	int16_t nState[3];						//0x22A
	uint8_t nAiType;						//0x230
	uint8_t pad0x231;						//0x231
	int16_t wAiBonus;						//0x232
	int32_t nCostMult;						//0x234
	int32_t nCostAdd;						//0x238
};



struct ItemTypesTxt {						//sgptDataTable + 0xBF8
	char szCode[4];							//0x00
	ItemType nEquiv1;						//0x04
	ItemType nEquiv2;						//0x06
	uint8_t nRepair;						//0x08
	uint8_t nBody;							//0x09
	uint8_t nBodyLoc1;						//0x0A
	uint8_t nBodyLoc2;						//0x0B
	uint16_t wShoots;						//0x0C
	uint16_t wQuiver;						//0x0E
	uint8_t nThrowable;						//0x10
	uint8_t nReload;						//0x11
	uint8_t nReEquip;						//0x12
	uint8_t nAutoStack;						//0x13
	uint8_t nMagic;							//0x14
	uint8_t nRare;							//0x15
	uint8_t nNormal;						//0x16
	uint8_t nCharm;							//0x17
	uint8_t nGem;							//0x18
	uint8_t nBeltable;						//0x19
	uint8_t nMaxSock1;						//0x1A
	uint8_t nMaxSock25;						//0x1B
	uint8_t nMaxSock40;						//0x1C
	uint8_t nTreasureClass;					//0x1D
	uint8_t nRarity;						//0x1E
	uint8_t nStaffMods;						//0x1F
	uint8_t nCostFormula;					//0x20
	uint8_t nClass;							//0x21
	uint8_t nStorePage;						//0x22
	uint8_t nVarInvGfx;						//0x23
	char szInvGfx[6][32];					//0x24
};

struct ItemDataTbl								//sgptDataTable + 0xCD8
{
	int nItemsTxtRecordCount;						//0x00
	ItemsTxt* pItemsTxt;							//0x04
	ItemsTxt* pWeapons;							//0x08
	int nWeaponsTxtRecordCount;						//0x0C
	ItemsTxt* pArmor;								//0x10
	int nArmorTxtRecordCount;						//0x14
	ItemsTxt* pMisc;								//0x18
	int nMiscTxtRecordCount;						//0x1C
};

struct CharItem
{
	uint32_t dwItemCode;				//0x00
	uint8_t nBodyLoc;					//0x04
	int8_t nItemCount;					//0x05
	uint16_t pad0x06;					//0x06
};

struct CharStatsTxt						//sgptDataTable + 0xBC4
{
	wchar_t wszClassName[16];				//0x00
	char szClassName[16];					//0x20
	uint8_t nStr;							//0x30
	uint8_t nDex;							//0x31
	uint8_t nInt;							//0x32
	uint8_t nVit;							//0x33
	uint8_t nStamina;						//0x34
	uint8_t nLifeAdd;						//0x35
	uint8_t nPercentStr;					//0x36
	uint8_t nPercentInt;					//0x37
	uint8_t nPercentDex;					//0x38
	uint8_t nPercentVit;					//0x39
	uint8_t nManaRegen;						//0x3A
	uint8_t pad0x3B;						//0x3B
	uint32_t dwToHitFactor;					//0x3C
	uint8_t nWalkSpeed;						//0x40
	uint8_t nRunSpeed;						//0x41
	uint8_t nRunDrain;						//0x42
	uint8_t nLifePerLevel;					//0x43
	uint8_t nStaminaPerLevel;				//0x44
	uint8_t nManaPerLevel;					//0x45
	uint8_t nLifePerVitality;				//0x46
	uint8_t nStaminaPerVitality;			//0x47
	uint8_t nManaPerMagic;					//0x48
	uint8_t nBlockFactor;					//0x49
	uint16_t pad0x4A;						//0x4A
	uint32_t dwBaseWClass;					//0x4C
	uint8_t nStatsPerLevel;					//0x50
	uint8_t pad0x51;						//0x51
	uint16_t wStrAllSkills;					//0x52
	uint16_t wStrSkillTab[3];				//0x54
	uint16_t wStrClassOnly;					//0x5A
	CharItem pItems[10];					//0x5C
	uint16_t wStartSkill;					//0xAC
	int16_t nBaseSkill[10];					//0xAE
	uint16_t pad0xC2;						//0xC2
};

struct DataTables {
	uint8_t bSkip0x0[0xB8C];				//0x000
	SkillDescTxt* pSkillDescTxt;			//0xB8C
	void* pSkillDescLinker;					//0xB90
	int nSkillDescTxtRecordCount;			//0xB94
	SkillsTxt* pSkillsTxt;					//0xB98
	void* pSkillsLinker;					//0xB9C
	int nSkillsTxtRecordCount;				//0xBA0
	int* nClassSkillCount;					//0xBA4
	int nHighestClassSkillCount;			//0xBA8
	short* nClassSkillList;					//0xBAC
	uint8_t bSkip0xBB0[0x14];				//0xBB0
	CharStatsTxt* pCharStatsTxt;			//0xBC4
	int nCharStatsTxtRecordCount;			//0xBC8
	uint8_t bSkip0xBCC[0x2C];				//0xBCC
	ItemTypesTxt* pItemTypesTxt;			//0xBF8
	int nItemTypesTxtRecordCount;			//0xBFC
};

#define STATIC_ASSERT_EQUAL(x, y) static_assert(x == y);

STATIC_ASSERT_EQUAL(offsetof(DataTables, pSkillDescTxt), 0xB8C);
STATIC_ASSERT_EQUAL(offsetof(DataTables, pSkillsTxt), 0xB98);
STATIC_ASSERT_EQUAL(offsetof(DataTables, pCharStatsTxt), 0xBC4);
STATIC_ASSERT_EQUAL(offsetof(DataTables, pItemTypesTxt), 0xBF8);