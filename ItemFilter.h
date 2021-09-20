#pragma once

#include <cstdint>
#include "D2Structs.h"
#include "Rule.h"
#include "Configuration.h"

typedef void(__fastcall* D2GSServerToClientPacketHandlerFn)(uint8_t* pBitstream);

class ItemFilter
{
private:
	static Configuration* Config;
public:
	ItemFilter();

	static void ReloadFilter();
	static void RunRules(Unit* pItem);
	
	static void ToggleDebug();
	static void DebugRule(uint32_t nLineNumber);

	static void Clip();

	static void DoChatAlert(Unit* pUnit);

	static bool IsItem(Unit* pUnit);
	static bool HasActions(Unit* pUnit);
	static POINT ScreenToAutomap(int nX, int nY);

	
#pragma region Hooks
	//Hooked Methods
	static LRESULT WndProc(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam);

	static void __fastcall ItemActionOwned(uint8_t* pBitstream);
	static void __fastcall ItemActionWorld(uint8_t* pBitstream);
	static void HandlePacket(uint8_t* pBitstream, D2GSServerToClientPacketHandlerFn pHandler);
	static void __stdcall DrawDebugInfo(Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd);
	static void __stdcall DrawGroundItemRect(DWORD retAddress, BOOL isHovered, Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode);
	static void __stdcall DrawInventoryItemRect(Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode);

	static BOOL __fastcall GetItemName_114d(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize);
	static BOOL __stdcall GetItemName(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize);
	static void __stdcall HandleItemName(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize);

	static void __stdcall GetItemDesc(wchar_t* pBuffer);
	static void __stdcall DATATBLS_LoadAllTxts(void* pMemPool, int a2, int a3);
	static void __stdcall DrawGameUI();

	static void __stdcall UNITS_FreeUnit(Unit* pUnit);

	static void __stdcall AUTOMAP_Draw();
	static BOOL __fastcall UNITDRAW_DrawUnit(Unit* pUnit, uint32_t dwColorTint, int nXpos, int nYpos, BOOL bFade, BOOL bDrawOverlays);
	static BOOL __fastcall IsUnitNoDraw(Unit* pUnit);

	static BOOL __stdcall CallCommand(char* wCmd);

#pragma endregion

#pragma region Stubs
	//Stubs
	static void __stdcall DrawAltDownItemRect_STUB_110f();
	static void __stdcall DrawAltDownItemRect_STUB();
	static void __stdcall DrawAltDownItemRect_STUB_114d();

	static void __stdcall DrawHoverItemRect_STUB();

	static void __stdcall DrawInventoryItemRect_STUB_110f();
	static void __stdcall DrawInventoryItemRect_STUB();
	static void __stdcall DrawInventoryItemRect_STUB_114d();

	static void __stdcall CheckUnitNoDraw1_STUB();
	static void __stdcall CheckUnitNoDraw2_STUB();
	static void __stdcall CheckUnitNoDraw2_STUB_114d();

	static void __stdcall GetItemDesc_STUB();
	static void __stdcall GetItemDesc_STUB_114d();

	static void __stdcall CallCommand_STUB();
	static void __stdcall CallCommand_STUB_114d();
#pragma endregion

};

