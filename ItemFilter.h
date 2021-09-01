#pragma once

#include <cstdint>
#include "D2Structs.h"
#include "Rule.h"
#include "Config.h"

typedef void(__fastcall* D2GSServerToClientPacketHandlerFn)(uint8_t* pBitstream);

class ItemFilter
{
private:
	static Config* Configuration;
	static bool IS_DEBUG_MODE;
public:
	ItemFilter();

	static void PopulateLookupTables();
	static void EmptyLookupTables();
	static void LoadFilter();
	static void InitializeRuleConditions();
	static void RunRules(Unit* pItem);
	
	static void ToggleDebug();
	static bool IsDebug() { return IS_DEBUG_MODE; }

	static void DoChatAlert(Unit* pUnit);

	static bool IsItem(Unit* pUnit);
	static bool HasActions(Unit* pUnit);
	static POINT ScreenToAutomap(int nX, int nY);
	
	//Hooked Methods
	static void __fastcall ItemActionOwned(uint8_t* pBitstream);
	static void __fastcall ItemActionWorld(uint8_t* pBitstream);
	static void HandlePacket(uint8_t* pBitstream, D2GSServerToClientPacketHandlerFn pHandler);
	static void __stdcall DrawDebugInfo(Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd);
	static void __stdcall DrawGroundItemRect(DWORD retAddress, BOOL isHovered, Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode);
	static void __stdcall DrawInventoryItemRect(Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode);

	static BOOL __fastcall GetItemName_114d(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize);
	static BOOL __stdcall GetItemName(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize);
	static void __stdcall HandleItemName(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize);

	static void __stdcall GetItemDesc(Unit* pUnit, wchar_t* pBuffer);
	static void __stdcall DATATBLS_LoadAllTxts(void* pMemPool, int a2, int a3);
	static void __stdcall UNITS_FreeUnit(Unit* pUnit);

	static void __stdcall AUTOMAP_Draw();
	static BOOL __fastcall UNITDRAW_DrawUnit(Unit* pUnit, uint32_t dwColorTint, int nXpos, int nYpos, BOOL bFade, BOOL bDrawOverlays);
	static BOOL __fastcall IsUnitNoDraw(Unit* pUnit);

	//Stubs
	static void __stdcall DrawAltDownItemRect_STUB();
	static void __stdcall DrawAltDownItemRect_STUB_114d();

	static void __stdcall DrawHoverItemRect_STUB();

	static void __stdcall DrawInventoryItemRect_STUB();
	static void __stdcall DrawInventoryItemRect_STUB_114d();

	static void __stdcall CheckUnitNoDraw1_STUB();
	static void __stdcall CheckUnitNoDraw2_STUB();
	static void __stdcall CheckUnitNoDraw2_STUB_114d();

	static void __stdcall GetItemDesc_STUB();
	static void __stdcall GetItemDesc_STUB_114d();

	static LRESULT WndProc(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam);

};

