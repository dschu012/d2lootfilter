#pragma once

#include <Windows.h>
#include <cstdint>
#include "D2Constants.h"

struct PlayerData;
struct MonsterData;
struct ObjectData;
struct MissleData;
struct DrlgAct;
struct ItemPath;
struct DynamicPath;
struct StaticPath;
struct AnimSeq;
struct AnimData;
struct GfxData;
struct StatListEx;
struct Corpse;
struct InventoryGrid;
struct InventoryNode;
struct Inventory;
struct Light;
struct QuestRecord;
struct TimerArg;
struct Game;
struct EventList;
struct HoverText;
struct SkillList;
struct Combat;
struct QuestSrv;
struct QuestClt;
struct ParticleStream;
struct Timer;
struct Unit;
struct DrlgEnvironment;
struct Room;
struct RoomEx;
struct RoomCollisionGrid;
struct Client;
struct TimerQueue;
struct InactiveUnitList;
struct MonsterRegion;
struct ObjectControl;
struct QuestInfo;
struct TargetNode;
struct NpcControl;
struct Arena;
struct QuestChain;
struct GfxLight;

enum class InventoryPage : uint8_t {
	INVENTORY = 0x00,
	EQUIP = 0x01,
	BELT = 0x02,
	CUBE = 0x03,
	STASH = 0x04,
	UNK = 0xFF
};

struct BitBuffer
{
	uint8_t* pBuffer;							//0x00
	int32_t nBits;								//0x04
	int32_t nPos;								//0x08
	int32_t nPosBits;							//0x0C
	BOOL bFull;									//0x10
};

struct Inventory
{
	uint32_t dwSignature;					//0x00
	void* pMemPool;							//0x04
	Unit* pOwner;						//0x08
	Unit* pFirstItem;					//0x0C
	Unit* pLastItem;					//0x10
	InventoryGrid* pGrids;			//0x14
	int32_t nGridCount;						//0x18
	uint32_t dwLeftItemGUID;				//0x1C
	Unit* pCursorItem;				//0x20
	uint32_t dwOwnerId;						//0x24
	uint32_t dwItemCount;					//0x28
	InventoryNode* pFirstNode;		//0x2C
	InventoryNode* pLastNode;			//0x30
	Corpse* pFirstCorpse;				//0x34
	Corpse* pLastCorpse;				//0x38
	int32_t nCorpseCount;					//0x3C
};


struct ItemData
{
	ItemRarity dwRarity;		//0x00
	uint32_t dwLowSeed;		//0x04
	uint32_t dwHighSeed;		//0x08
	uint32_t dwOwnerGUID;		//0x0C
	uint32_t dwInitSeed;		//0x10
	uint32_t dwCommandFlags;		//0x14
	uint32_t dwItemFlags;		//0x18
	uint32_t unk0x1C[2];		//0x1C
	uint32_t dwActionStamp;		//0x24
	uint32_t dwFileIndex;		//0x28
	uint32_t dwItemLevel;		//0x2C
	uint16_t wItemFormat;		//0x30
	uint16_t wRarePrefix;		//0x32
	uint16_t wRareSuffix;		//0x34
	uint16_t wAutoPrefix;		//0x36
	uint16_t wMagicPrefix[3];		//0x38
	uint16_t wMagicSuffix[3];		//0x3E
	uint8_t nBodyLoc;			//0x44
	InventoryPage nInvPage;			//0x45
	uint8_t unk0x46[2];		//0x46
	uint8_t nEarLvl;			//0x48
	uint8_t nInvGfxIdx;		//0x49
	char szPlayerName[16];		//0x4A
	uint8_t unk0x5A[2];		//0x5A
	Inventory* pNodeInv;		//0x5C
	uint32_t unk0x60;			//0x60
	Unit* pNextItem;		//0x64
	uint8_t nNodePos;			//0x68
	uint8_t nNodePosOther;		//0x69
};

#pragma pack(push, 1)

struct PathPoint
{
	uint16_t X;
	uint16_t Y;
};

struct DynamicPath
{
	union
	{
		struct
		{
			uint16_t wOffsetX;					//0x00
			uint16_t wPosX;						//0x02
			uint16_t wOffsetY;					//0x04
			uint16_t wPosY;						//0x06
		};

		struct
		{
			uint32_t dwPrecisionX;				//0x00
			uint32_t dwPrecisionY;				//0x04
		};
	};
	uint32_t dwTargetX;						//0x08
	uint32_t dwTargetY;						//0x0C
	uint16_t xSP1;								//0x10
	uint16_t ySP1;								//0x12
	uint16_t xSP2;								//0x14
	uint16_t ySP2;								//0x16
	uint16_t xSP3;								//0x18
	uint16_t ySP3;								//0x1A
	Room* pRoom;						//0x1C
	Room* pRoomNext;					//0x20
	int32_t unk0x24;							//0x24
	int32_t dwPathPoints;						//0x28
	void* unk0x2C;							//0x2C
	Unit* pUnit;						//0x30
	uint32_t dwFlags;							//0x34
	uint32_t unk0x38;							//0x38
	uint32_t dwPathType;						//0x3C
	uint32_t dwPrevPathType;					//0x40
	uint32_t dwUnitSize;						//0x44
	uint32_t dwCollisionPattern;				//0x48
	uint32_t dwCollisionType;					//0x4C
	uint32_t unk0x50;							//0x50
	uint16_t unk0x54;							//0x54
	uint16_t unk0x56;							//0x56
	Unit* pTargetUnit;				//0x58
	uint32_t dwTargetType;						//0x5C
	uint32_t dwTargetId;						//0x60
	uint8_t nDirection;						//0x64
	uint8_t nNewDirection;						//0x65
	uint8_t nDiffDirection;					//0x66
	uint8_t unk0x67;							//0x67
	uint8_t unk0x68[10];						//0x68
	int32_t unk0x72;							//0x72
	int32_t unk0x76;							//0x76
	char unk0x7A[2];						//0x7A
	uint32_t dwVelocity;						//0x7C
	uint32_t unk0x80;							//0x80
	uint32_t dwVelocity2;						//0x84
	uint32_t unk0x88[2];						//0x88
	uint8_t nDist;								//0x90
	uint8_t nDistMax;							//0x91
	uint8_t unk0x92;							//0x92
	uint8_t nStepNum;							//0x93
	uint8_t nDistance;							//0x94
	char unk0x95[3];						//0x95
	//uint32_t dwPathOffset;						//0x98
	PathPoint PathPoints[7];			//0x98
	uint32_t unk0xB8[72];						//0xB8
	int32_t unk0x1D4;							//0x1D4
	PathPoint unk0x1D8[7];			//0x1D8
	char unk0x1DC[12];						//0x1DC
};

struct PathInfo
{
	PathPoint pStartCoord;			//0x00
	PathPoint field_4;				//0x04
	Room* pRoom;						//0x08
	Room* field_C;					//0x0C
	int32_t field_10;							//0x10
	uint8_t field_14;							//0x14
	uint8_t field_15;							//0x15
	uint16_t field_16;							//0x16
	int32_t nDistMax;							//0x18
	uint8_t field_1C;							//0x1C
	uint8_t field_1D;							//0x1D
	uint16_t field_1E;							//0x1E
	int32_t nPathType;							//0x20
	int32_t nUnitSize;							//0x24
	int32_t nCollisionPattern;					//0x28
	int32_t nCollisionType;						//0x2C
	union
	{
		DynamicPath* pDynamicPath;	//0x30 - not sure yet
		PathInfo* pNext;				//0x30
	};
};

struct StaticPath
{
	Room* pRoom;						//0x00
	int32_t nTargetX;							//0x04
	int32_t nTargetY;							//0x08
	int32_t nXPos;								//0x0C
	int32_t nYPos;								//0x10
	uint32_t unk0x14[2];						//0x14
	uint8_t nDirection;						//0x1C
	uint8_t unk0x1D[3];						//0x1D
};


struct Seed {
	union
	{
		struct
		{
			int32_t nLowSeed;					//0x00
			int32_t nHighSeed;					//0x04
		};
		uint64_t lSeed;		//0x00
	};
};

struct DrlgCoords {
	int32_t nSubtileX;					//0x00
	int32_t nSubtileY;					//0x04
	int32_t nSubtileWidth;				//0x08
	int32_t nSubtileHeight;				//0x0C
	int32_t nTileXPos;					//0x10
	int32_t nTileYPos;					//0x14
	int32_t nTileWidth;					//0x18
	int32_t nTileHeight;				//0x1C
};

struct Room
{
	Room** pRoomsNear;     //0x00
	uint32_t _1;                //0x04
	void* _1s;                //0x08
	uint32_t _1b;                //0x0C
	RoomEx* pRoomEx;            //0x10
	uint32_t _2[2];            //0x14
	Unit** pUnitChanged;    //0x1C
	RoomCollisionGrid* Coll;            //0x20
	uint32_t dwRoomsNear;        //0x24
	uint32_t nPlayerUnits;        //0x28
	DrlgAct* pAct;                //0x2C
	uint32_t _4;                //0x30
	uint32_t nUnknown;            //0x34
	uint32_t _5[4];            //0x38
	Client** pClients;        //0x48
	DrlgCoords pCoords;        //0x4C
	Seed pSeed;            //0x6C
	Unit* pUnitFirst;    //0x74
	uint32_t nNumClients;        //0x78
	Room* pRoomNext;        //0x7C
};

struct DrlgAct
{
	uint32_t _1a;
	DrlgEnvironment* pEnviroment;    //0x04
	uint32_t  nTownLevel;              //0x08
	uint32_t dwMapSeed;                //0x0C
	Room* pRoom;                    //0x10
	/*
	uint32_t dwAct;                    //0x14
	TileData pTileData;                //0x18
	ActMisc* pMisc;                    //0x48 
	uint32_t _4;                        //0x4C
	uint32_t _5;                        //0x50
	uint32_t _6;                        //0x54
	uint32_t _7;                        //0x58
	void* pMemoryPool;                //0x5C
	*/
};

struct Game
{
	uint32_t __0000[4];						//0x00
	Game* pNext;						//0x10
	uint32_t __0014;							//0x14
	LPCRITICAL_SECTION lpCriticalSection;	//0x18
	void* pMemoryPool;						//0x1C
	uint32_t __0020[2];						//0x20
	uint16_t nServerToken;						//0x28
	char szGameName[16];					//0x2A
	char szGamePassword[16];				//0x3A
	char szGameDesc[32];					//0x4A
	uint8_t nGameType;							//0x6A
	uint16_t __006B;							//0x6B
	uint8_t nDifficulty;						//0x6D
	uint16_t __006E;							//0x6E
	BOOL bExpansion;						//0x70
	uint32_t dwGameType;						//0x74
	uint16_t wItemFormat;						//0x78
	uint16_t unk0x7A;							//0x7A
	uint32_t dwInitSeed;						//0x7C
	uint32_t dwObjSeed;						//0x80
	uint32_t InitSeed;							//0x84
	Client* pClientList;				//0x88
	uint32_t nClients;							//0x8C
	uint32_t dwSpawnedPlayers;					//0x90
	uint32_t dwSpawnedMonsters;				//0x94
	uint32_t dwSpawnedObjects;					//0x98
	uint32_t dwSpawnedMissiles;				//0x9C
	uint32_t dwSpawnedItems;					//0xA0
	uint32_t dwSpawnedTiles;					//0xA4
	uint32_t dwGameFrame;						//0xA8
	uint32_t unk0xAC[3];						//0xAC
	TimerQueue* pTimerQueue;			//0xB8
	DrlgAct* pAct[5];					//0xBC
	Seed pGameSeed;					//0xD0
	InactiveUnitList* pInactiveUnitList[5];//0xD8
	uint32_t dwMonSeed;						//0xEC
	MonsterRegion* pMonReg[1024];		//0xF0
	ObjectControl* pObjectControl;	//0x10F0
	QuestInfo* pQuestControl;			//0x10F4
	TargetNode* pUnitNodes[10];		//0x10F8
	Unit* pUnitList[5][128];			//0x1120
	void* pTileList;						//0x1B20
	uint32_t dwUniqueFlags[128];				//0x1B24
	NpcControl* pNpcControl;			//0x1D24
	Arena* pArenaCtrl;				//0x1D28
	void* pPartyControl;					//0x1D2C
	uint8_t nBossFlagList[64];					//0x1D30
	uint32_t dwMonModeData[17];				//0x1D70
	uint32_t nMonModeData;						//0x1DB4
	uint32_t unk0x1DB8[3];						//0x1DB8
	uint32_t nSyncTimer;						//0x1DC4
};

struct UnitHashTable {
	Unit* table[128];
};

struct Unit
{
	UnitType dwUnitType;					//0x00
	union							//0x04
	{
		uint32_t dwLineId;					// the id in the txt file
		PlayerClass dwClassId;
	};
	void* pMemoryPool;					//0x08
	uint32_t dwUnitId;						//0x0C
	union {								//0x10
		uint32_t dwAnimMode;
		ItemAnimationMode eItemAnimMode;
	};
	union							//0x14
	{
		PlayerData* pPlayerData;
		ItemData* pItemData;
		MonsterData* pMonsterData;
		ObjectData* pObjectData;
		MissleData* pMissileData;
	};
	uint32_t dwAct;						//0x18
	DrlgAct* pDrlgAct;				        //0x1C
	uint32_t dwLoSeed;						//0x20
	uint32_t dwHiSeed;						//0x24
	uint32_t dwInitSeed;					//0x28
	union							//0x2C
	{
		ItemPath* pItemPath;
		DynamicPath* pDynamicPath;
		StaticPath* pStaticPath;
	};
	AnimSeq* pAnimSeq;				        //0x30
	uint32_t dwSeqFrameCount;					//0x34
	uint32_t dwSeqFrame;					//0x38
	uint32_t dwAnimSpeed;					//0x3C
	uint32_t dwSeqMode;					//0x40
	uint32_t dwGFXcurrentFrame;				//0x44
	uint32_t dwFrameCount;					//0x48
	uint16_t wAnimSpeed;					//0x4C
	uint16_t wActionFrame;					//0x4E
	AnimData* pAnimData;				        //0x50
	GfxData* pGfxData;				        //0x54
	GfxData* pGfxDataCopy;				        //0x58
	StatListEx* pStatListEx;				//0x5C
	Inventory* pInventory;				        //0x60
	union
	{
		struct									//Server Unit
		{
			uint32_t dwInteractGUID;			//0x064
			uint32_t dwInteractType;			//0x068
			uint16_t nInteract;					//0x06C
			uint16_t nUpdateType;				//0x06E
			Unit* pUpdateUnit;			//0x070
			QuestChain* pQuestEventList;	//0x074
			BOOL bSparkChest;					//0x078
			void* pTimerParams;					//0x07C
			Game* pGame;					//0x080
			uint32_t __084[3];					//0x084
			Timer* pSrvTimerList;			//0x090
		};

		struct									//Client Unit
		{
			GfxLight* pLight;				//0x064
			uint32_t dwStartLight;				//0x068
			int32_t nPaletteIndex;				//0x06C
			BOOL bUnitSfx;						//0x070
			uint32_t dwSfxMode;					//0x074
			void* pUnitSfxData;					//0x078
			uint32_t dwSfxTicks;				//0x07C
			uint32_t dwSfxAsyncTicks;			//0x080
			uint32_t dwSfxStepTicks;			//0x084
			BOOL bHasActiveSound;				//0x088
			uint16_t nLastClickX;				//0x08C
			uint16_t nLastClickY;				//0x08E
			EventList* pCltTimerList;		//0x090
		};
	};
	uint32_t dwOwnerType;					//0x94
	uint32_t dwOwnerGUID;					//0x98
	uint32_t unk0x9C[2];					//0x9C
	HoverText* pHoverText;				        //0xA4
	SkillList* pSkills;				        //0xA8
	Combat* pCombat;					//0xAC
	uint32_t dwLastHitClass;					//0xB0
	uint32_t unk0xB4;						//0xB4
	uint32_t dwDropItemCode;					//0xB8
	uint32_t unk0xBC[2];					//0xBC
	uint32_t dwFlags;						//0xC4
	uint32_t dwFlagEx;						//0xC8
	union							//0xCC
	{
		QuestSrv* pSrvQuestData;	                //Server pUnit
		QuestClt* pCltQuestData;	                //Client pUnit
	};
	uint32_t dwNodeIndex; //0xD0
	uint32_t dwTickCount;					//0xD4
	union							//0xD8
	{
		uint32_t dwSrvTickCount;		                //Server pUnit
		ParticleStream* pParticleStream;	        //Client pUnit		
	};
	Timer* pTimer;					        //0xDC
	Unit* pChangeNextUnit;				        //0xE0
	Unit* pRoomNext;					//0xE4
	Unit* pListNext;					//0xE8
	void* pMsgFirst;					//0xEC
	void* pMsgLast;						//0xF0
};

struct LayerUnitStatId
{
	// We can not use a struct as function parameters here as it has a different effect when using the __fastcall calling convetion.
	// Instead we just use D2SLayerStatIdStrc::PackedType so that we may easily change it later
	using PackedType = int32_t;

	union
	{
		struct
		{
			uint16_t nLayer;				//0x00
			uint16_t nStat;					//0x02
		};
		PackedType nPackedValue;			//0x00
	};

	static LayerUnitStatId Make(uint16_t wLayer, uint16_t wStatId) { return { wLayer, wStatId }; }
	static LayerUnitStatId MakeFromStatId(uint16_t wStatId) { return { 0, wStatId }; }
	static LayerUnitStatId FromPackedType(PackedType nPackedValue) {
		LayerUnitStatId ls;
		ls.nPackedValue = nPackedValue;
		return ls;
	}
};

struct UnitStat : LayerUnitStatId
{
	int32_t nValue;							//0x04
};

struct StatsArray
{
	UnitStat* pStat;						//0x00 An Array[wStatCount]
	uint16_t nStatCount;					//0x04
	uint16_t nCapacity;						//0x06
	static const int nGrowthAmount = 4;
	static const int nShrinkThreshold = 8;
};

struct D2ModStatsArrayStrc
{
	LayerUnitStatId* pStat;				//0x00 An Array[wStatCount]
	uint16_t nStatCount;					//0x04
	uint16_t nCapacity;						//0x06
	static const int nGrowthAmount = 4;
	static const int nShrinkThreshold = 8;
};

struct StatList
{
	void* pMemPool;							//0x00
	Unit* pUnit;						//0x04
	uint32_t dwOwnerType;					//0x08
	uint32_t dwOwnerId;						//0x0C
	uint32_t dwFlags;						//0x10 D2C_StatlistFlags
	uint32_t dwStateNo;						//0x14
	int32_t dwExpireFrame;					//0x18
	uint32_t dwSkillNo;						//0x1C
	uint32_t dwSLvl;						//0x20
	StatsArray Stats;					//0x24
	StatList* pPrevLink;				//0x2C
	StatList* pNextLink;				//0x30
	StatList* pParent;				//0x34
	void* fpStatRemove;						//0x38
};

struct StatListEx : public StatList
{
	StatList* pMyLastList;			//0x3C
	StatList* pMyStats;				//0x40
	Unit* pOwner;						//0x44
	StatsArray FullStats;				//0x48
	D2ModStatsArrayStrc ModStats;			//0x50
	uint32_t* StatFlags;					//0x58 8bytes per states
	void* fpCallBack;						//0x5C
	Game* pGame;						//0x60
};

#pragma pack(pop)