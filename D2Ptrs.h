#pragma once

#include <Windows.h>
#include <cstdint>
#include "Utils.h"
#include "D2Tables.h"
#include "D2Structs.h"
#include "Globals.h"

static const uint32_t GAME_EXE = (uint32_t)GetModuleHandle(NULL);
static const uint32_t DLLBASE_D2CLIENT = (uint32_t)LoadLibraryA("D2Client.dll");
static const uint32_t DLLBASE_D2CMP = (uint32_t)LoadLibraryA("D2CMP.dll");
static const uint32_t DLLBASE_D2COMMON = (uint32_t)LoadLibraryA("D2Common.dll");
static const uint32_t DLLBASE_D2GAME = (uint32_t)LoadLibraryA("D2Game.dll");
static const uint32_t DLLBASE_D2GFX = (uint32_t)LoadLibraryA("D2Gfx.dll");
static const uint32_t DLLBASE_D2LANG = (uint32_t)LoadLibraryA("D2Lang.dll");
static const uint32_t DLLBASE_D2LAUNCH = (uint32_t)LoadLibraryA("D2Launch.dll");
static const uint32_t DLLBASE_D2MCPCLIENT = (uint32_t)LoadLibraryA("D2MCPClient.dll");
static const uint32_t DLLBASE_D2MULTI = (uint32_t)LoadLibraryA("D2Multi.dll");
static const uint32_t DLLBASE_D2NET = (uint32_t)LoadLibraryA("D2Net.dll");
static const uint32_t DLLBASE_D2SOUND = (uint32_t)LoadLibraryA("D2Sound.dll");
static const uint32_t DLLBASE_D2WIN = (uint32_t)LoadLibraryA("D2Win.dll");
static const uint32_t DLLBASE_FOG = (uint32_t)LoadLibraryA("Fog.dll");
static const uint32_t DLLBASE_STORM = (uint32_t)LoadLibraryA("Storm.dll");

#define D2FUNC(DLL, N, R, C, A, O) typedef R (C* DLL##_##N##_t) A; __declspec(selectany) extern DLL##_##N##_t DLL##_##N = (DLL##_##N##_t)GetDllOffset((GetGameVersion() > D2Version::V113c ? GAME_EXE : DLLBASE_##DLL), O);   ///
#define D2PTR(DLL, N, T, O) __declspec(selectany) extern T* DLL##_##N = (T*)GetDllOffset((GetGameVersion() > D2Version::V113c ? GAME_EXE : DLLBASE_##DLL), O);

#define F2(DLL, N, R, C, A, O113c, O114d) D2FUNC(DLL, N, R, C, A, (GetGameVersion() == D2Version::V114d ? O114d : O113c))
#define P2(DLL, N, T, O113c, O114d) D2PTR(DLL, N, T, (GetGameVersion() == D2Version::V114d ? O114d : O113c))

//Hooks for item filter
P2(D2CLIENT, fpGetItemDescPatch, void, 0x5612C, 0xE64CE);
P2(D2WIN, callDrawAltDownItemRectPatch, void, 0x12F12, 0x102436);		//Call to D2GFX_DrawSolidRectEx(uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode)
P2(D2CLIENT, callDrawAltDownItemRectRet, void, 0x59473, 0xA72F4);
//1.13c $+59473   |.  A1 0078BC6F   |MOV EAX,DWORD PTR DS:[6FBC7800]
//1.14d $ + A72F4 | > \5F            POP EDI

P2(D2CLIENT, fpGroundHoverUnit, void, 0x51A80, 0x67A10); //check 1.14d???
P2(D2WIN, callDrawHoverItemRectPatch, void, 0x1357B, 0x1031C1);			//Call to D2GFX_DrawSolidRectEx(uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode)
P2(D2CLIENT, callDrawInventoryItemRectPatch, void, 0x8C678, 0x83A3F);	//Call to D2GFX_DrawSolidRectEx(uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode)

//from plugy
P2(D2CLIENT, callCommand, void, 0x70AB5, 0x7C519);

//6FB166E1 | .  8B86 C8000000 MOV EAX, DWORD PTR DS : [ESI + 0C8]
//6FB166E7 | .C1E8 12       SHR EAX, 12
//6FB166EA | .  83E0 01       AND EAX, 00000001

//6FB1B363 | .  8B97 C8000000 MOV EDX, DWORD PTR DS : [EDI + 0C8]
//6FB1B369 | .C1EA 12       SHR EDX, 12
//6FB1B36C | .  83E2 01       AND EDX, 00000001
//Check NODRAW bit 

P2(D2CLIENT, checkUnitNoDrawPatch_1, void, 0x666E1, 0xDC7C8);
P2(D2CLIENT, checkUnitNoDrawPatch_2, void, 0x6B363, 0x703ED);


//Pointers
P2(D2COMMON, gpDataTables, DataTables*, -11170, 0x344304);
P2(D2COMMON, ItemDataTbl, ItemDataTbl, 0x9FB94, 0x4426EC);
P2(D2CLIENT, GetHoverItem, Unit*, 0x11BC38, 0x3BCBF4); // 1.13c: 6FB44DBD 1.14d: 004876BE
P2(D2CLIENT, AutomapOn, BOOL, 0xFADA8, 0x3A27E8);
//Can't say I understand these
P2(D2CLIENT, Divisor, int, 0xF16B0, 0x311254);
P2(D2CLIENT, Offset, POINT, 0x11C1F8, 0x3A5198);
P2(D2CLIENT, ResolutionX, uint32_t, 0xDBC48, 0x31146C);

P2(D2CLIENT, ServerSideUnitHashTables, UnitHashTable, 0x10A608, 0x3A5E70) // Updated 1.14d //007A5E70-BASE
P2(D2CLIENT, ClientSideUnitHashTables, UnitHashTable, 0x109A08, 0x3A5270) // Updated 1.14d //007A5270-BASE

/*
FUNCPTR(D2COMMON, MapToAbsScreen, void __stdcall, (long* pX, long* pY), 0x243260)             // Updated 1.14d //00643260-BASE
FUNCPTR(D2COMMON, AbsScreenToMap, void __stdcall, (long* ptMouseX, long* ptMouseY), 0x243510) // Updated 1.14d  //00643510-BASE
*/

//Functions
//1.14d 004B3870 near here.
F2(D2COMMON, ITEMS_GetTransactionCost, int, __stdcall, (Unit* pPlayer, Unit* pItem, uint8_t nDifficulty, BitBuffer* pQuestFlags, int nVendorId, int nTransactionType), -10107, 0x22FDC0);
F2(D2COMMON, ITEMS_CheckItemTypeId, BOOL, __stdcall, (Unit* pItem, ItemType nItemType), -10744, 0x229BB0);
F2(D2COMMON, STATLIST_GetUnitStatUnsigned, uint32_t, __stdcall, (const Unit* pUnit, Stat nStatId, uint16_t nLayer), -10973, 0x225480);
//todo check 1.14d
//F2(D2COMMON, STATLIST_GetUnitStatSigned, int, __stdcall, (Unit* pUnit, Stat nStatId, uint16_t nLayer), -10587, 0x225480);
F2(D2COMMON, DATATBLS_LoadAllTxts, void, __stdcall, (void* pMemPool, int a2, int a3), -10943, 0x219300);
F2(D2COMMON, UNITS_FreeUnit, void, __stdcall, (Unit* pUnit), -10124, 0x220300);
F2(D2COMMON, INVENTORY_GetNextItem, Unit*, __stdcall, (Unit* pItem), 0x0, 0x23DFA0);

F2(D2CLIENT, DrawGameUI, void, __stdcall, (),  0x285C0, 0x96B10);
F2(D2CLIENT, UNITDRAW_DrawUnit, BOOL, __fastcall, (Unit* pUnit, uint32_t dwColorTint, int nXpos, int nYpos, BOOL bFade, BOOL bDrawOverlays), 0x6CC00, 0x71EC0);

F2(D2CLIENT, ItemActionWorld, void, __fastcall, (uint8_t* pBitstream), 0xAE080, 0x5EB10);
F2(D2CLIENT, ItemActionOwned, void, __fastcall, (uint8_t* pBitstream), 0xAE870, 0x5EC70);

F2(D2CLIENT, AUTOMAP_Draw, void, __stdcall, (void), 0x5FFE0, 0x59E90);
F2(D2CLIENT, GetQuestFlags, BitBuffer*, __stdcall, (void), 0x45A00, 0xB32D0);
F2(D2CLIENT, GetDifficulty, uint8_t, __stdcall, (void), 0x41930, 0x4DCD0);
F2(D2CLIENT, GetPlayerUnit, Unit*, __stdcall, (void), 0xA4D60, 0x63DD0);
F2(D2CLIENT, GetItemName, BOOL, __stdcall, (Unit* pItem, wchar_t* wBuffer, uint32_t dwSize), 0x914F0, 0x0);
F2(D2CLIENT, GetItemName_114d, BOOL, __fastcall, (Unit* pItem, wchar_t* wBuffer, uint32_t dwSize), 0x0, 0x8C060);
F2(D2CLIENT, GetAutomapSize, uint32_t, __stdcall, (void), 0x5F080, 0x5A710);

F2(D2CLIENT, FindClientSideUnit, Unit*, __fastcall, (uint32_t dwUnitId, UnitType dwType), 0xA5B20, 0x63990)
F2(D2CLIENT, FindServerSideUnit, Unit*, __fastcall, (uint32_t dwUnitId, UnitType dwType), 0xA5B40, 0x639B0);

F2(D2CLIENT, PrintGameString, void, __stdcall, (const wchar_t* wMessage, TextColor eColor), 0x7D850, 0x0);
F2(D2CLIENT, PrintGameStringe_114d, void, __fastcall, (const wchar_t* wMessage, TextColor eColor), 0x0, 0x9E3A0);

F2(D2WIN, WndProc, LRESULT, __stdcall, (HWND hWnd, int msg, WPARAM wParam, LPARAM lParam), 0x17740, 0xF9A80);
F2(D2WIN, SetTextFont, Font, __fastcall, (Font dwSize), -10184, 0x102EF0);
F2(D2WIN, D2DrawText, void, __fastcall, (const wchar_t* wStr, uint32_t nXpos, uint32_t nYpos, TextColor eColor, uint32_t dwCentered), -10150, 0x102320);

//fix 1.14d
F2(D2WIN, GetTextPixelWidthFileNo, uint32_t, __fastcall, (const wchar_t* wStr, uint32_t* dwWidth, uint32_t* dwFileNo), -10177, 0x0);
F2(D2WIN, GetTextPixelWidth, uint32_t, __fastcall, (const wchar_t* wStr), 0x0, 0x1017D0);

//006C9030 / .  55            PUSH EBP
//$ + 2C9030 / .  55            PUSH EBP
F2(D2GFX, DrawRect, void, __stdcall, (RECT* pRect, uint8_t nPaletteIndex), -10035, 0xF62A0);
//__fastcall for 114d: 0x6EFD0
F2(D2GFX, DrawSolidRectEx, void, __stdcall, (uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode), -10014, 0xF6300);

F2(D2LANG, GetStringFromTblIndex, wchar_t*, __fastcall, (uint16_t nTblIndex), -10003, 0x124A30);