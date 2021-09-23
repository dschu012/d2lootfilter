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

#define F2(DLL, N, R, C, A, O110f, O113c, O114d) D2FUNC(DLL, N, R, C, A, (GetGameVersion() == D2Version::V114d ? O114d : (GetGameVersion() == D2Version::V113c ? O113c : O110f)))
#define P2(DLL, N, T, O110f, O113c, O114d) D2PTR(DLL, N, T, (GetGameVersion() == D2Version::V114d ? O114d : (GetGameVersion() == D2Version::V113c ? O113c : O110f)))

//Hooks for item filter
P2(D2CLIENT, fpGetItemDescPatch, void, 0x531C9, 0x5612C, 0xE64CE);
P2(D2WIN, callDrawAltDownItemRectPatch, void, 0xB160, 0x12F12, 0x102436);		//Call to D2GFX_DrawSolidRectEx(uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode)
P2(D2CLIENT, callDrawAltDownItemRectRet, void, 0x6A3F0, 0x59473, 0xA72F4);
//1.13c $+59473   |.  A1 0078BC6F   |MOV EAX,DWORD PTR DS:[6FBC7800]
//1.14d $ + A72F4 | > \5F            POP EDI

P2(D2CLIENT, fpGroundHoverUnit, void, 0x15A20, 0x51A80, 0x67A10); //check 1.14d???
P2(D2WIN, callDrawHoverItemRectPatch, void, 0xA89E, 0x1357B, 0x1031C1);			//Call to D2GFX_DrawSolidRectEx(uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode)

//in 110f  D2Client.0x6FAE1710_DrawItemBackground(D2InventoryGridInfoStrc* pInventoryGridInfo, int nX, int nY, BYTE nItemWidth, BYTE nItemHeight, DWORD nColorIndex)
//uses strange wrapper insted of digerct D2GFX_DrawSolidRectEx call
//.text:6FAE179C                 call    sub_6FB5B0F0
//int __fastcall sub_6FB5B0F0(int a1, int a2, int a3, int a4, int a5, int a6)
//{
//	return D2gfx_10055(a1, a2, a1 + a3, a2 + a4, a5, a6);
//}
P2(D2CLIENT, callDrawInventoryItemRectPatch, void, 0x4179C, 0x8C678, 0x83A3F);	//Call to D2GFX_DrawSolidRectEx(uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode)

//from plugy
P2(D2CLIENT, callCommand, void, 0x32BAB, 0x70AB5, 0x7C519);

//113c
//6FB166E1 | .  8B86 C8000000 MOV EAX, DWORD PTR DS : [ESI + 0C8]
//6FB166E7 | .C1E8 12       SHR EAX, 12
//6FB166EA | .  83E0 01       AND EAX, 00000001

//6FB1B363 | .  8B97 C8000000 MOV EDX, DWORD PTR DS : [EDI + 0C8]
//6FB1B369 | .C1EA 12       SHR EDX, 12
//6FB1B36C | .  83E2 01       AND EDX, 00000001

//110f
// .text:6FB4DB5B mov eax, [esi+0C8h]
// .text:6FB4DB61 shr eax, 12h
// .text:6FB4DB64 and eax, 1
// 
// .text:6FB5BADD mov eax, [edi+0C8h]
// .text:6FB5BAE3 shr eax, 12h
// .text:6FB5BAE6 and eax, 1
// 
//Check NODRAW bit 

P2(D2CLIENT, checkUnitNoDrawPatch_1, void, 0xADB5B, 0x666E1, 0xDC7C8);
P2(D2CLIENT, checkUnitNoDrawPatch_2, void, 0xBBADD, 0x6B363, 0x703ED);


//Pointers
P2(D2COMMON, gpDataTables, DataTables*, 0x96A20, -11170, 0x344304);   //need to check for 110f
P2(D2COMMON, ItemDataTbl, ItemDataTbl, 0xAA2E0, 0x9FB94, 0x4426EC);
P2(D2CLIENT, GetHoverItem, Unit*, 0x1158F0, 0x11BC38, 0x3BCBF4); // 1.13c: 6FB44DBD 1.14d: 004876BE
P2(D2CLIENT, AutomapOn, BOOL, 0x11A6D0, 0xFADA8, 0x3A27E8);
//Can't say I understand these
P2(D2CLIENT, Divisor, int, 0xD7BC0, 0xF16B0, 0x311254);
P2(D2CLIENT, Offset, POINT, 0x1119C8, 0x11C1F8, 0x3A5198);    //unshure for 110f
P2(D2CLIENT, ResolutionX, uint32_t, 0xD40EC, 0xDBC48, 0x31146C);

P2(D2CLIENT, ServerSideUnitHashTables, UnitHashTable, 0x11B600, 0x10A608, 0x3A5E70); // Updated 1.14d //007A5E70-BASE
P2(D2CLIENT, ClientSideUnitHashTables, UnitHashTable, 0x11AA00, 0x109A08, 0x3A5270); // Updated 1.14d //007A5270-BASE

/*
FUNCPTR(D2COMMON, MapToAbsScreen, void __stdcall, (long* pX, long* pY), 0x243260)             // Updated 1.14d //00643260-BASE
FUNCPTR(D2COMMON, AbsScreenToMap, void __stdcall, (long* ptMouseX, long* ptMouseY), 0x243510) // Updated 1.14d  //00643510-BASE
*/

//Functions
//1.14d 004B3870 near here.
F2(D2COMMON, ITEMS_GetTransactionCost, int, __stdcall, (Unit* pPlayer, Unit* pItem, uint8_t nDifficulty, BitBuffer* pQuestFlags, int nVendorId, int nTransactionType), -10775 , -10107, 0x22FDC0);
F2(D2COMMON, ITEMS_CheckItemTypeId, BOOL, __stdcall, (Unit* pItem, ItemType nItemType), -10731, -10744, 0x229BB0);
F2(D2COMMON, STATLIST_GetUnitStatUnsigned, uint32_t, __stdcall, (const Unit* pUnit, Stat nStatId, uint16_t nLayer), -10520, -10973, 0x225480);
//todo check 1.14d
F2(D2COMMON, DATATBLS_LoadAllTxts, void, __stdcall, (void* pMemPool, int a2, int a3), -10576, -10943, 0x219300);
F2(D2COMMON, UNITS_FreeUnit, void, __stdcall, (Unit* pUnit), -11260, -10124, 0x220300);
F2(D2CLIENT, DrawGameUI, void, __stdcall, (), 0x5E650, 0x285C0, 0x96B10);

//110f 0xBA720 looks way more complex that 0x6CC00 113c, need to be checked
F2(D2CLIENT, UNITDRAW_DrawUnit, BOOL, __fastcall, (Unit* pUnit, uint32_t dwColorTint, int nXpos, int nYpos, BOOL bFade, BOOL bDrawOverlays), 0xBA720, 0x6CC00, 0x71EC0);

F2(D2CLIENT, ItemActionWorld, void, __fastcall, (uint8_t* pBitstream), 0x14420, 0xAE080, 0x5EB10);
F2(D2CLIENT, ItemActionOwned, void, __fastcall, (uint8_t* pBitstream), 0x14590, 0xAE870, 0x5EC70);

F2(D2CLIENT, AUTOMAP_Draw, void, __stdcall, (void), 0x2EF10, 0x5FFE0, 0x59E90);
F2(D2CLIENT, GetQuestFlags, BitBuffer*, __stdcall, (void), 0x5BD20, 0x45A00, 0xB32D0);

//for 110f propably __fastcall
F2(D2CLIENT, GetDifficulty, uint8_t, __stdcall, (void), 0xC090, 0x41930, 0x4DCD0);

F2(D2CLIENT, GetPlayerUnit, Unit*, __stdcall, (void), 0x883D0, 0xA4D60, 0x63DD0);

//seems like  GetItemName in 110f is __fastcall as well as in 114d
F2(D2CLIENT, GetItemName, BOOL, __stdcall, (Unit* pItem, wchar_t* wBuffer, uint32_t dwSize), 0x0, 0x914F0, 0x0);
F2(D2CLIENT, GetItemName_114d, BOOL, __fastcall, (Unit* pItem, wchar_t* wBuffer, uint32_t dwSize), 0x3D360, 0x0, 0x8C060);


F2(D2CLIENT, GetAutomapSize, uint32_t, __stdcall, (void), 0xB4C50, 0x5F080, 0x5A710);   //for 110f - 6FB54C50(B4C50) or 6FB54CB0(B4CBO) - need to check in debuger 

F2(D2CLIENT, FindClientSideUnit, Unit*, __fastcall, (uint32_t dwUnitId, UnitType dwType), 0x869F0, 0xA5B20, 0x63990);
F2(D2CLIENT, FindServerSideUnit, Unit*, __fastcall, (uint32_t dwUnitId, UnitType dwType), 0x86A60, 0xA5B40, 0x639B0);

/*
atleast in 110f we have two simmilar functions
D2ClientMsgTop 07C600	6FB1C600
D2ClientMsgBottom 07C950	6FB1C950
both are __fastcall
assuming we use D2ClientMsgTop
*/

F2(D2CLIENT, PrintGameString, void, __stdcall, (const wchar_t* wMessage, TextColor eColor), 0x0, 0x7D850, 0x0);
F2(D2CLIENT, PrintGameStringe_114d, void, __fastcall, (const wchar_t* wMessage, TextColor eColor), 0x7C600, 0x0, 0x9E3A0);

F2(D2WIN, WndProc, LRESULT, __stdcall, (HWND hWnd, int msg, WPARAM wParam, LPARAM lParam), 0xD9B0, 0x17740, 0xF9A80);
F2(D2WIN, SetTextFont, Font, __fastcall, (Font dwSize), -10127, -10184, 0x102EF0);

/*
There are two functions to draw text in 110F
D2WIN_10117_DrawText  (wchar_t* pString, int nXpos, int nYpos, int nColour, int nTransTbl)
D2WIN_10129_DrawHoverPopUp  (wchar_t* pString, int nXpos, int nYpos, int nColour, int dwTextAlign)
they matches to 110C to
D2WIN_10150_DrawText  (wchar_t* pString, int nXpos, int nYpos, int nColour, int nTransTbl)
D2WIN_10185_DrawHoverPopUp  (wchar_t* pString, int nXpos, int nYpos, int nColour, int dwTextAlign)
Need to cross-check
*/

F2(D2WIN, D2DrawText, void, __fastcall, (const wchar_t* wStr, uint32_t nXpos, uint32_t nYpos, TextColor eColor, uint32_t dwCentered), -10117 , -10150, 0x102320);

//fix 1.14d
F2(D2WIN, GetTextPixelWidthFileNo, uint32_t, __fastcall, (const wchar_t* wStr, uint32_t* dwWidth, uint32_t* dwFileNo), 0x0, -10177, 0x0);     //Not sure about this two but looks like 110f have same f as 114d
F2(D2WIN, GetTextPixelWidth, uint32_t, __fastcall, (const wchar_t* wStr), -10121, 0x0, 0x1017D0);

//006C9030 / .  55            PUSH EBP
//$ + 2C9030 / .  55            PUSH EBP
F2(D2GFX, DrawRect, void, __stdcall, (RECT* pRect, uint8_t nPaletteIndex), -10052, -10035, 0xF62A0);
//__fastcall for 114d: 0x6EFD0
F2(D2GFX, DrawSolidRectEx, void, __stdcall, (uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode), -10055, -10014, 0xF6300);

F2(D2LANG, GetStringFromTblIndex, wchar_t*, __fastcall, (uint16_t nTblIndex), -10004, -10003, 0x124A30);