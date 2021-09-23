#include <Windows.h>
#include "D2Ptrs.h"
#include "D2Tables.h"
#include "D2Structs.h"
#include "ItemFilter.h"
#include "Rule.h"
#include "Configuration.h"
#include "Utils.h"
#include "Hooking.h"
#include "Globals.h"
#include <queue>

static D2CLIENT_ItemActionWorld_t fpItemActionWorld;
static D2CLIENT_ItemActionOwned_t fpItemActionOwned;
static D2CLIENT_GetItemName_t fpD2CLIENT_GetItemName;
static D2CLIENT_GetItemName_114d_t fpD2CLIENT_GetItemName_114d;
static D2CLIENT_UNITDRAW_DrawUnit_t fpUNITDRAW_DrawUnit;
static D2CLIENT_AUTOMAP_Draw_t fpAUTOMAP_Draw;
static D2COMMON_UNITS_FreeUnit_t fpUNITS_FreeUnit;
static D2COMMON_DATATBLS_LoadAllTxts_t fpDATATBLS_LoadAllTxts;
static D2CLIENT_DrawGameUI_t fpD2CLIENT_DrawGameUI;
static D2WIN_WndProc_t fpWndProc;

static BOOL(__stdcall* fpCallCommand)(char* sCmd);

static std::queue<uint32_t> AUTOMAP_ITEMS;
static std::unordered_map<uint32_t, ActionResult*> ITEM_ACTIONS;


Configuration* ItemFilter::Config = NULL;

const char* CMD_RELOAD = "/reload";
const char* CMD_FILTERLEVEL = "/fl";
const char* CMD_FILTERLEVEL2 = "/filterlevel";
const char* CMD_PINGLEVEL = "/pl";
const char* CMD_PINGLEVEL2 = "/pinglevel";
const char* CMD_DEBUG = "/debug";
const char* CMD_TEST = "/test";
const char* CMD_CLIP = "/clip";

ItemFilter::ItemFilter() {
	
	Config = new Configuration();

	//check if plugy already installed a command hook
	//if so we do a trampoline, if not we isntall our own stub.
	bool IsPlugyCommandHook = (*(BYTE*)D2CLIENT_callCommand) == 0xE8;
	
	//alot of 114d functions need diff stubs or they changed from __stdcall to __fastcall
	if (GetGameVersion() == D2Version::V114d) {
		//Item Action Own/World Packet Hooks
		Hooking::TrampolineHook(D2CLIENT_ItemActionWorld, &ItemActionWorld, reinterpret_cast<void**>(&fpItemActionWorld), 9);
		Hooking::TrampolineHook(D2CLIENT_ItemActionOwned, &ItemActionOwned, reinterpret_cast<void**>(&fpItemActionOwned), 9);

		//Item Name Hook
		Hooking::TrampolineHook(D2CLIENT_GetItemName_114d, &GetItemName_114d, reinterpret_cast<void**>(&fpD2CLIENT_GetItemName_114d), 8);

		//Item Desc Hook
		Hooking::SetJmp(D2CLIENT_fpGetItemDescPatch, &GetItemDesc_STUB_114d, 6);
		
		//Automap Draw Hook
		Hooking::TrampolineHook(D2CLIENT_AUTOMAP_Draw, &AUTOMAP_Draw, reinterpret_cast<void**>(&fpAUTOMAP_Draw), 9);

		//Unit Draw Hook
		Hooking::TrampolineHook(D2CLIENT_UNITDRAW_DrawUnit, &UNITDRAW_DrawUnit, reinterpret_cast<void**>(&fpUNITDRAW_DrawUnit), 6);

		//No draw hooks
		Hooking::SetCall(D2CLIENT_checkUnitNoDrawPatch_1, &CheckUnitNoDraw1_STUB, 9);
		Hooking::SetCall(D2CLIENT_checkUnitNoDrawPatch_2, &CheckUnitNoDraw2_STUB_114d, 9);

		//Call Command Hook
		Hooking::SetCall(D2CLIENT_callCommand, &CallCommand_STUB_114d, 5);

		//Item Rect on Ground Hooks
		Hooking::SetCall(D2WIN_callDrawAltDownItemRectPatch, &DrawAltDownItemRect_STUB_114d, 5);
		Hooking::SetCall(D2WIN_callDrawHoverItemRectPatch, &DrawHoverItemRect_STUB, 5);

		//Item Rect in Inv Hook
		Hooking::SetCall(D2CLIENT_callDrawInventoryItemRectPatch, &DrawInventoryItemRect_STUB_114d, 5);

		//Cleanup Cached Item Data Hook
		Hooking::TrampolineHook(D2COMMON_UNITS_FreeUnit, &UNITS_FreeUnit, reinterpret_cast<void**>(&fpUNITS_FreeUnit), 7);

		//Load Datatables Hook
		Hooking::TrampolineHook(D2COMMON_DATATBLS_LoadAllTxts, &DATATBLS_LoadAllTxts, reinterpret_cast<void**>(&fpDATATBLS_LoadAllTxts), 5);
		Hooking::TrampolineHook(D2CLIENT_DrawGameUI, &DrawGameUI, reinterpret_cast<void**>(&fpD2CLIENT_DrawGameUI), 6);
	} else if (GetGameVersion() == D2Version::V113c) {
		//Item Action Own/World Packet Hooks
		Hooking::TrampolineHook(D2CLIENT_ItemActionWorld, &ItemActionWorld, reinterpret_cast<void**>(&fpItemActionWorld), 6);
		Hooking::TrampolineHook(D2CLIENT_ItemActionOwned, &ItemActionOwned, reinterpret_cast<void**>(&fpItemActionOwned), 6);

		//Item Name Hook
		Hooking::TrampolineHook(D2CLIENT_GetItemName, &GetItemName, reinterpret_cast<void**>(&fpD2CLIENT_GetItemName), 5);

		//Item Desc Hook
		Hooking::SetJmp(D2CLIENT_fpGetItemDescPatch, &GetItemDesc_STUB, 6);

		//Automap Draw Hook
		Hooking::TrampolineHook(D2CLIENT_AUTOMAP_Draw, &AUTOMAP_Draw, reinterpret_cast<void**>(&fpAUTOMAP_Draw), 6);

		//Unit Draw Hook
		Hooking::TrampolineHook(D2CLIENT_UNITDRAW_DrawUnit, &UNITDRAW_DrawUnit, reinterpret_cast<void**>(&fpUNITDRAW_DrawUnit), 5);

		//No draw hooks
		Hooking::SetCall(D2CLIENT_checkUnitNoDrawPatch_1, &CheckUnitNoDraw1_STUB, 9);
		Hooking::SetCall(D2CLIENT_checkUnitNoDrawPatch_2, &CheckUnitNoDraw2_STUB, 9);

		//Call Command Hook
		Hooking::SetCall(D2CLIENT_callCommand, &CallCommand_STUB, 5);

		//Item Rect on Ground Hooks
		Hooking::SetCall(D2WIN_callDrawAltDownItemRectPatch, &DrawAltDownItemRect_STUB, 5);
		Hooking::SetCall(D2WIN_callDrawHoverItemRectPatch, &DrawHoverItemRect_STUB, 5);

		//Item Rect in Inv Hook
		Hooking::SetCall(D2CLIENT_callDrawInventoryItemRectPatch, &DrawInventoryItemRect_STUB, 5);

		//Cleanup Cached Item Data Hook
		Hooking::TrampolineHook(D2COMMON_UNITS_FreeUnit, &UNITS_FreeUnit, reinterpret_cast<void**>(&fpUNITS_FreeUnit), 5);

		//Load Datatables Hook
		Hooking::TrampolineHook(D2COMMON_DATATBLS_LoadAllTxts, &DATATBLS_LoadAllTxts, reinterpret_cast<void**>(&fpDATATBLS_LoadAllTxts), 6);
		Hooking::TrampolineHook(D2CLIENT_DrawGameUI, &DrawGameUI, reinterpret_cast<void**>(&fpD2CLIENT_DrawGameUI), 5);
	} else if (GetGameVersion() == D2Version::V110f) {
		//Item Action Own/World Packet Hooks
		Hooking::TrampolineHook(D2CLIENT_ItemActionWorld, &ItemActionWorld, reinterpret_cast<void**>(&fpItemActionWorld), 6);
		Hooking::TrampolineHook(D2CLIENT_ItemActionOwned, &ItemActionOwned, reinterpret_cast<void**>(&fpItemActionOwned), 6);

		//Item Name Hook
		Hooking::TrampolineHook(D2CLIENT_GetItemName_114d, &GetItemName_114d, reinterpret_cast<void**>(&fpD2CLIENT_GetItemName_114d), 5);

		//Item Desc Hook
		Hooking::SetJmp(D2CLIENT_fpGetItemDescPatch, &GetItemDesc_STUB, 6);

		//Automap Draw Hook
		Hooking::TrampolineHook(D2CLIENT_AUTOMAP_Draw, &AUTOMAP_Draw, reinterpret_cast<void**>(&fpAUTOMAP_Draw), 6);

		//Unit Draw Hook
		Hooking::TrampolineHook(D2CLIENT_UNITDRAW_DrawUnit, &UNITDRAW_DrawUnit, reinterpret_cast<void**>(&fpUNITDRAW_DrawUnit), 6);

		//No draw hooks
		Hooking::SetCall(D2CLIENT_checkUnitNoDrawPatch_1, &CheckUnitNoDraw1_STUB, 9);
		Hooking::SetCall(D2CLIENT_checkUnitNoDrawPatch_2, &CheckUnitNoDraw2_STUB, 9);

		//Call Command Hook
		Hooking::SetCall(D2CLIENT_callCommand, &CallCommand_STUB, 5);

		//Item Rect on Ground Hooks
		Hooking::SetCall(D2WIN_callDrawAltDownItemRectPatch, &DrawAltDownItemRect_STUB_110f, 5);
		Hooking::SetCall(D2WIN_callDrawHoverItemRectPatch, &DrawHoverItemRect_STUB, 5);

		//Item Rect in Inv Hook
		Hooking::SetCall(D2CLIENT_callDrawInventoryItemRectPatch, &DrawInventoryItemRect_STUB_110f, 5);

		//Cleanup Cached Item Data Hook
		Hooking::TrampolineHook(D2COMMON_UNITS_FreeUnit, &UNITS_FreeUnit, reinterpret_cast<void**>(&fpUNITS_FreeUnit), 5);

		//Load Datatables Hook
		Hooking::TrampolineHook(D2COMMON_DATATBLS_LoadAllTxts, &DATATBLS_LoadAllTxts, reinterpret_cast<void**>(&fpDATATBLS_LoadAllTxts), 6);
		Hooking::TrampolineHook(D2CLIENT_DrawGameUI, &DrawGameUI, reinterpret_cast<void**>(&fpD2CLIENT_DrawGameUI), 8);
	}
	//User Input/WndProc Hook
	Hooking::TrampolineHook(D2WIN_WndProc, &WndProc, reinterpret_cast<void**>(&fpWndProc), 5);
}

void ItemFilter::ReloadFilter() {
	INFO_LOG(std::format(L"Item Filter Loaded (PingLevel: {}, FilterLevel: {})", PingLevel, FilterLevel));
	Config->Load();
	
	ITEM_ACTIONS.clear();
	for (uint8_t i = 0; i < 128; i++) {
		Unit* pUnit = FindUnitFromTable(i, UnitType::ITEM);
		while (pUnit) {
			RunRules(pUnit);
			DoChatAlert(pUnit);
			pUnit = pUnit->pRoomNext;
		}
	}
}

void ItemFilter::RunRules(Unit* pItem) {
	ITEM_ACTIONS[pItem->dwUnitId] = new ActionResult{};
	ActionResult* result = ITEM_ACTIONS[pItem->dwUnitId];

	for (auto entry : GlobalRules) {
		Rule* rule = entry.second;
		if (rule->Evaluate(pItem)) {
			result->vMatchedRules.push_back(rule->GetLineNumber());
			rule->EvaluateActionResult(result, pItem);
			if (!result->bContinue) {
				return;
			}
		}
	}
}

bool ItemFilter::IsItem(Unit* pUnit) {
	return pUnit != NULL && pUnit->dwUnitType == UnitType::ITEM;
}

bool ItemFilter::HasActions(Unit* pUnit) {
	return IsItem(pUnit) && ITEM_ACTIONS.contains(pUnit->dwUnitId);
}

POINT ItemFilter::ScreenToAutomap(int nX, int nY) {
	nX *= 32; nY *= 32;
	int x = ((nX - nY) / 2 / (*(int*)D2CLIENT_Divisor)) - (*D2CLIENT_Offset).x + 8;
	int y = ((nX + nY) / 4 / (*(int*)D2CLIENT_Divisor)) - (*D2CLIENT_Offset).y - 8;
	if (D2CLIENT_GetAutomapSize()) {
		--x;
		y += 5;
	}
	return { x, y };
}

void ItemFilter::DATATBLS_LoadAllTxts(void* pMemPool, int a2, int a3) {
	fpDATATBLS_LoadAllTxts(pMemPool, a2, a3);
	IsTxtDataLoaded = true;

}

void __stdcall ItemFilter::DrawGameUI() {
	if (!Config->IsLoaded()) {
		ItemFilter::ReloadFilter();
	}
	fpD2CLIENT_DrawGameUI();
}

void ItemFilter::HandlePacket(uint8_t* pBitStream, D2GSServerToClientPacketHandlerFn pHandler) {
	/*
	cmd = 0x9c (world)/0x9d (owned)
	[uint8_t bCmd] [uint8_t bAction] [uint8_t bMessageSize?] [uint8_t bUnk?] [uint32_t dwUnitId] ...
	0x9c: [uint8_t[...] itemData]
	0x9d: [uint8_t[0x28] unk?] [uint8_t[...] itemData]
	*/
	uint32_t unitId = *(uint32_t*)&pBitStream[4];

	pHandler(pBitStream);

	Unit* pItem = FindUnit(unitId, UnitType::ITEM);
	if (pItem == NULL) {
		return;
	}

	//TODO: exclude NPC items?

	RunRules(pItem);
	DoChatAlert(pItem);
}

//Hooked Methods
void __fastcall ItemFilter::ItemActionWorld(uint8_t* pBitstream) {
	ItemFilter::HandlePacket(pBitstream, fpItemActionWorld);
}

void __fastcall ItemFilter::ItemActionOwned(uint8_t* pBitstream) {
	ItemFilter::HandlePacket(pBitstream, fpItemActionOwned);
}

BOOL __fastcall ItemFilter::GetItemName_114d(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize) {
	BOOL ret = fpD2CLIENT_GetItemName_114d(pItem, pBuffer, dwSize);
	HandleItemName(pItem, pBuffer, dwSize);
	return ret;
}

BOOL __stdcall ItemFilter::GetItemName(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize) {
	BOOL ret = fpD2CLIENT_GetItemName(pItem, pBuffer, dwSize);
	HandleItemName(pItem, pBuffer, dwSize);
	return ret;
}

void __stdcall ItemFilter::HandleItemName(Unit* pItem, wchar_t* pBuffer, uint32_t dwSize) {
	if (HasActions(pItem)
		&& ITEM_ACTIONS[pItem->dwUnitId]->bItemNameSet) {
		std::wstring copy = ITEM_ACTIONS[pItem->dwUnitId]->wsItemName;
		replace(copy, L"{Name}", pBuffer);
		wcsncpy(pBuffer, copy.c_str(), 0x7d);
	}
}

void __stdcall ItemFilter::GetItemDesc(wchar_t* pBuffer) {
	Unit* pItem = *D2CLIENT_GetHoverItem;
	if (HasActions(pItem)
		&& ITEM_ACTIONS[pItem->dwUnitId]->bItemDescSet) {
		std::wstring copy = ITEM_ACTIONS[pItem->dwUnitId]->wsItemDesc;
		replace(copy, L"{Description}", pBuffer);
		wcsncpy(pBuffer, copy.c_str(), 0x800);
	}
}

void __stdcall ItemFilter::AUTOMAP_Draw() {
	fpAUTOMAP_Draw();
	while (!AUTOMAP_ITEMS.empty()) {
		Unit* pItem = FindUnit(AUTOMAP_ITEMS.front(), UnitType::ITEM);
		AUTOMAP_ITEMS.pop();
		if (pItem == NULL) {
			continue;
		}
		if (HasActions(pItem)
			&& ITEM_ACTIONS[pItem->dwUnitId]->bMinimapIcon) {
			POINT p = ScreenToAutomap(pItem->pStaticPath->nXPos, pItem->pStaticPath->nYPos);
			D2GFX_DrawSolidRectEx(p.x - 5, p.y - 5, p.x + 5, p.y + 5, ITEM_ACTIONS[pItem->dwUnitId]->nMinimapIconPaletteIndex, DrawMode::NORMAL);
		}
	}
}

//nXpos and nYpos don't appear to be right...
BOOL __fastcall ItemFilter::UNITDRAW_DrawUnit(Unit* pUnit, uint32_t dwColorTint, int nXpos, int nYpos, BOOL bFade, BOOL bDrawOverlays) {
	if (HasActions(pUnit)
		&& ITEM_ACTIONS[pUnit->dwUnitId]->bMinimapIcon) {
		//this seems janky, but is used to queue up items to be drawn on automap
		AUTOMAP_ITEMS.push(pUnit->dwUnitId);
	}
	return fpUNITDRAW_DrawUnit(pUnit, dwColorTint, nXpos, nYpos, bFade, bDrawOverlays);
}

void __stdcall ItemFilter::UNITS_FreeUnit(Unit* pUnit) {
	if (HasActions(pUnit)) {
		ITEM_ACTIONS.erase(pUnit->dwUnitId);
	}
	fpUNITS_FreeUnit(pUnit);
}


void __stdcall ItemFilter::DrawDebugInfo(Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd) {
	if (!HasActions(pItem)) {
		return;
	}
	ActionResult* actions = ITEM_ACTIONS[pItem->dwUnitId];
	std::vector<std::wstring> lines;
	std::wostringstream os;
	os << L"Matched Lines #: " << TEXT_GREEN;
	for (auto& match : actions->vMatchedRules) {
		if (&match != &actions->vMatchedRules.front()) {
			os << L", ";
		}
		os << match;
	}
	ItemsTxt txt = GetItemsTxt(pItem);

	lines.push_back(std::format(L"ID: {}{}", TEXT_WHITE, pItem->dwUnitId));
	lines.push_back(std::format(L"File Index: {}{}", TEXT_WHITE, pItem->pItemData->dwFileIndex));
	lines.push_back(std::format(L"Type: {}{}", TEXT_WHITE, std::wstring(D2LANG_GetStringFromTblIndex(txt.wNameStr))));
	std::wstring wCode = std::wstring(4, L' ');
	mbstowcs(&wCode[0], txt.szCode, 4);
	lines.push_back(std::format(L"Code: {}{}", TEXT_WHITE, wCode));
	lines.push_back(std::format(L"Quality: {}{}", TEXT_WHITE, QualitiesLookup[GetQualityLevel(pItem)]));
	lines.push_back(std::format(L"Rarity: {}{}", TEXT_WHITE, RaritiesLookup[static_cast<int32_t>(pItem->pItemData->dwRarity)]));
	lines.push_back(std::format(L"Class: {}{}", TEXT_WHITE, ItemTypesLookup[static_cast<int32_t>(txt.wType[0])]));
	lines.push_back(os.str());
	lines.push_back(std::format(L"Shown: {}{}", TEXT_WHITE, actions->bHide ? L"False" : L"True"));
	if (actions->bItemNameSet) {
		lines.push_back(std::format(L"Name: {}{}", TEXT_WHITE, actions->wsItemName));
	}
	if (actions->bItemDescSet) {
		lines.push_back(std::format(L"Description: {}{}", TEXT_WHITE, actions->wsItemDesc));
	}
	if (actions->bChatAlert) {
		lines.push_back(std::format(L"Chat Alert: {}{}", TEXT_WHITE, actions->bChatAlert ? L"True" : L"False"));
	}
	if (actions->bMinimapIcon) {
		lines.push_back(std::format(L"Minimap Icon: {}{:#04x}", TEXT_WHITE, actions->nMinimapIconPaletteIndex));
	}
	if (actions->bBorderPaletteIndexSet) {
		lines.push_back(std::format(L"Border Color: {}{:#04x}", TEXT_WHITE, actions->nBorderPaletteIndex));
	}
	if (actions->bInvBackgroundPaletteIndexSet) {
		lines.push_back(std::format(L"Inventory Color: {}{:#04x}", TEXT_WHITE, actions->nInvBackgroundPaletteIndex));
	}
	auto width = 0;
	for (auto& line : lines) {
		uint32_t w, fileNo;
		if (GetGameVersion() == D2Version::V114d || GetGameVersion() == D2Version::V110f) {
			w = D2WIN_GetTextPixelWidth(line.c_str());
		} else {
			D2WIN_GetTextPixelWidthFileNo(line.c_str(), &w, &fileNo);
		}
		if (w > width) {
			width = w;
		}
	}

	int mid = (nXEnd - nXStart) / 2 + nXStart;
	bool left = (mid >= (*D2CLIENT_ResolutionX / 2));
	auto pad = 10;
	auto xStart = 0, xEnd = 0;
	if (left) {
		xStart = nXStart - width - pad * 2;
		xEnd = nXStart;
	} else {
		xStart = nXEnd;
		xEnd = nXEnd + width + pad * 2;
	}
	auto lineHeight = 15;
	auto y = nYStart;

	D2GFX_DrawSolidRectEx(xStart, y, xEnd, y + (lines.size() * lineHeight), 0x0, DrawMode::TRANS_75);
	for (auto& line : lines) {
		D2WIN_D2DrawText(line.c_str(), xStart + pad, y += lineHeight, TextColor::GOLD, TRUE);
	}
}

/*
Handles the following.
1) Item rect on ground when holding alt
2) Item rect on ground when you hover over
*/
void __stdcall ItemFilter::DrawGroundItemRect(DWORD retAddress, BOOL isHovered, Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode) {
	if (retAddress == reinterpret_cast<DWORD>(D2CLIENT_callDrawAltDownItemRectRet)) {
		//drawing a rect that isnt for an item. this is kinda a hack checking ret address of caller.
		D2GFX_DrawSolidRectEx(nXStart, nYStart, nXEnd, nYEnd, nPaletteIndex, eDrawMode);
		return;
	}
	if (pItem == nullptr) {
		pItem = *D2CLIENT_GetHoverItem;
	}
	if ((isHovered || eDrawMode == DrawMode::NORMAL) && IsFilterDebug) {
		DrawDebugInfo(pItem, nXStart, nYStart, nXEnd, nYEnd);
	}
	if (HasActions(pItem)
		&& (pItem->eItemAnimMode == ItemAnimationMode::DROPPING
			|| pItem->eItemAnimMode == ItemAnimationMode::GROUND)) {
		nPaletteIndex = ITEM_ACTIONS[pItem->dwUnitId]->bBackgroundPaletteIndexSet ? ITEM_ACTIONS[pItem->dwUnitId]->nBackgroundPaletteIndex : nPaletteIndex;
		if (ITEM_ACTIONS[pItem->dwUnitId]->bBorderPaletteIndexSet) {
			auto pad = 1;
			RECT rect = { nXStart + pad, nYStart + pad, nXEnd - pad, nYEnd - pad };
			D2GFX_DrawRect(&rect, ITEM_ACTIONS[pItem->dwUnitId]->nBorderPaletteIndex);
		}
		if (eDrawMode == DrawMode::TRANS_50) {
			eDrawMode = ITEM_ACTIONS[pItem->dwUnitId]->eDrawModeAlt;
		} else {
			eDrawMode = ITEM_ACTIONS[pItem->dwUnitId]->eDrawModeHover;
		}
	}
	//call original
	D2GFX_DrawSolidRectEx(nXStart, nYStart, nXEnd, nYEnd, nPaletteIndex, eDrawMode);
}

void __stdcall ItemFilter::DrawInventoryItemRect(Unit* pItem, uint32_t nXStart, uint32_t nYStart, uint32_t nXEnd, uint32_t nYEnd, uint8_t nPaletteIndex, DrawMode eDrawMode) {
	//0x08: red, 0xea: blue, 0x76: green.
	if (nPaletteIndex == 0xEA
			&& HasActions(pItem)) {
		nPaletteIndex = ITEM_ACTIONS[pItem->dwUnitId]->bInvBackgroundPaletteIndexSet ? ITEM_ACTIONS[pItem->dwUnitId]->nInvBackgroundPaletteIndex : nPaletteIndex;
	}
	//call original
	D2GFX_DrawSolidRectEx(nXStart, nYStart, nXEnd, nYEnd, nPaletteIndex, eDrawMode);
}

//returns result of nodraw flag shift
BOOL __fastcall ItemFilter::IsUnitNoDraw(Unit* pUnit) {
	if (HasActions(pUnit)) {
		if (ITEM_ACTIONS[pUnit->dwUnitId]->bHide
				&& !IsFilterDebug) {
			pUnit->dwFlagEx |= static_cast<std::underlying_type_t<UnitFlagEx>>(UnitFlagEx::NODRAW);
		} else {
			pUnit->dwFlagEx &= ~(static_cast<std::underlying_type_t<UnitFlagEx>>(UnitFlagEx::NODRAW));
		}
	}
	return pUnit->dwFlagEx >> 0x12;
}

BOOL __stdcall ItemFilter::CallCommand(char* sCmd) {
	if (!strncmp(sCmd, CMD_RELOAD, strlen(CMD_RELOAD))) {
		ItemFilter::ReloadFilter();
		return FALSE;
	}

	if (!strncmp(sCmd, CMD_FILTERLEVEL, strlen(CMD_FILTERLEVEL))) {
		FilterLevel = atoi(&sCmd[strlen(CMD_FILTERLEVEL)]);
		Config->SaveSettings();
		ItemFilter::ReloadFilter();
		return FALSE;
	}

	if (!strncmp(sCmd, CMD_FILTERLEVEL2, strlen(CMD_FILTERLEVEL2))) {
		FilterLevel = atoi(&sCmd[strlen(CMD_FILTERLEVEL2)]);
		Config->SaveSettings();
		ItemFilter::ReloadFilter();
		return FALSE;
	}

	if (!strncmp(sCmd, CMD_PINGLEVEL, strlen(CMD_PINGLEVEL))) {
		PingLevel = atoi(&sCmd[strlen(CMD_PINGLEVEL)]);
		Config->SaveSettings();
		ItemFilter::ReloadFilter();
		return FALSE;
	}

	if (!strncmp(sCmd, CMD_PINGLEVEL2, strlen(CMD_PINGLEVEL2))) {
		PingLevel = atoi(&sCmd[strlen(CMD_PINGLEVEL2)]);
		Config->SaveSettings();
		ItemFilter::ReloadFilter();
		return FALSE;
	}

	if (!strncmp(sCmd, CMD_DEBUG, strlen(CMD_DEBUG))) {
		ItemFilter::ToggleDebug();
		return FALSE;
	}

	if (!strncmp(sCmd, CMD_TEST, strlen(CMD_TEST))) {
		uint32_t rule = atoi(&sCmd[strlen(CMD_TEST)]);
		ItemFilter::DebugRule(rule);
		return FALSE;
	}

	if (!strncmp(sCmd, CMD_CLIP, strlen(CMD_CLIP))) {
		ItemFilter::Clip();
		return FALSE;
	}
	return TRUE;
}

void ItemFilter::DoChatAlert(Unit* pUnit) {
	if (HasActions(pUnit)
		&& ITEM_ACTIONS[pUnit->dwUnitId]->bChatAlert
		&& (pUnit->eItemAnimMode == ItemAnimationMode::DROPPING
			|| pUnit->eItemAnimMode == ItemAnimationMode::GROUND)) {
		wchar_t buffer[0x100] = L"";
		if (GetGameVersion() == D2Version::V114d || GetGameVersion() == D2Version::V110f) {
			D2CLIENT_GetItemName_114d(pUnit, buffer, 0x100);
		} else {
			D2CLIENT_GetItemName(pUnit, buffer, 0x100);
		}
		std::wstring result = std::wstring(buffer);
		replace(result, L"\n", L" - ");
		PrintGameString(result, TextColor::WHITE);
	}
}

void ItemFilter::ToggleDebug() {
	IsFilterDebug = !IsFilterDebug;
	INFO_LOG(std::format(L"Debug {}", IsFilterDebug ? L"On" : L"Off"));
}

void ItemFilter::DebugRule(uint32_t nLineNumber) {
	Rule* rule = GlobalRules[nLineNumber];
	if (!rule) {
		ERROR_LOG(std::format(L"No rule found on line {}.", nLineNumber));
		return;
	}
	Unit* pUnit = *D2CLIENT_GetHoverItem;
	if (!IsItem(pUnit)) {
		ERROR_LOG(std::format(L"No item found under cursor."));
		return;
	}
	for (Condition* condition : rule->GetConditions()) {
		TextColor color = condition->Evaluate(pUnit) ? TextColor::GREEN : TextColor::RED;
		PrintGameString(condition->ToString(pUnit), color);
	}
}

void ItemFilter::Clip() {
	Unit* pItem = *D2CLIENT_GetHoverItem;
	if (!IsItem(pItem)) {
		ERROR_LOG(std::format(L"No item found under cursor."));
		return;
	}
	LPTSTR  lptstrCopy;
	HGLOBAL hglbCopy;

	if (!OpenClipboard(NULL)) {
		return;
	}

	// Allocate a global memory object for the text.
	ItemsTxt txt = GetItemsTxt(pItem);

	std::wstring wCode = std::wstring(4, L' ');
	mbstowcs(&wCode[0], txt.szCode, 4);
	wCode = trim(wCode);

	std::wstring s = std::format(L"Show\n\tType {}\n\tCode {}\n\tQuality {}\n\tRarity {}\n\tClass {}\n\n",
		std::wstring(D2LANG_GetStringFromTblIndex(txt.wNameStr)),
		wCode,
		QualitiesLookup[GetQualityLevel(pItem)],
		RaritiesLookup[static_cast<int32_t>(pItem->pItemData->dwRarity)],
		ItemTypesLookup[static_cast<int32_t>(txt.wType[0])]);
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (s.size() + 1) * sizeof(wchar_t));
	if (hglbCopy == NULL)
	{
		CloseClipboard();
		return;
	}

	lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
	memcpy(lptstrCopy, s.c_str(), s.size() * sizeof(wchar_t));
	GlobalUnlock(hglbCopy);

	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hglbCopy);

	CloseClipboard();

	INFO_LOG(std::format(L"Item successfully copied to clipboard."));
}

LRESULT ItemFilter::WndProc(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_SYSKEYUP:
		case WM_KEYUP: {
			//Z debug
			if (wParam == 0x5A
				&& (GetKeyState(VK_LSHIFT) & 0x80) || (GetKeyState(VK_RSHIFT) & 0x80)
				&& (GetKeyState(VK_LCONTROL) & 0x80) || (GetKeyState(VK_RCONTROL) & 0x80)) {
				ItemFilter::ToggleDebug();
			}
			//R reload
			if (wParam == 0x52
				&& (GetKeyState(VK_LCONTROL) & 0x80) || (GetKeyState(VK_RCONTROL) & 0x80)) {
				if (IsTxtDataLoaded) {
					ItemFilter::ReloadFilter();
				}
			}
			break;
		}
		default: {
			return fpWndProc(hWnd, msg, wParam, lParam);
		}
	}
	return fpWndProc(hWnd, msg, wParam, lParam);
}

void __declspec(naked) __stdcall ItemFilter::GetItemDesc_STUB() {
	__asm {
			add esp, 0x808;
			push eax;
			call ItemFilter::GetItemDesc;
			ret 0xc;
	}
}

void __declspec(naked) __stdcall ItemFilter::GetItemDesc_STUB_114d() {
	__asm {
		mov esp, ebp
		pop ebp
		push eax;
		call ItemFilter::GetItemDesc;
		ret 0xc;
	}
}

void __declspec(naked) __stdcall ItemFilter::CheckUnitNoDraw1_STUB() {
	__asm
	{
		push ecx;
		push edx;
		mov ecx, esi;
		call ItemFilter::IsUnitNoDraw;
		pop edx;
		pop ecx;
		ret;
	}
}

void __declspec(naked) __stdcall ItemFilter::CheckUnitNoDraw2_STUB() {
	__asm
	{
		push eax;
		push ecx;
		mov ecx, edi;
		call ItemFilter::IsUnitNoDraw;
		mov edx, eax;
		pop ecx;
		pop eax;
		ret;
	}
}

void __declspec(naked) __stdcall ItemFilter::CheckUnitNoDraw2_STUB_114d() {
	__asm
	{
		push ecx;
		push edx;
		mov ecx, ebx;
		call ItemFilter::IsUnitNoDraw;
		pop edx;
		pop ecx;
		ret;
	}
}

void __declspec(naked) __stdcall ItemFilter::DrawAltDownItemRect_STUB_110f() {
	__asm
	{
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		mov eax, [esp + 0x4c];
		push[eax - 0x4];
		push 0;
		push 0;
		call ItemFilter::DrawGroundItemRect;
		ret 0x18;
	}
}

void __declspec(naked) __stdcall ItemFilter::DrawAltDownItemRect_STUB() {
	__asm
	{
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esi - 0x4];
		push 0;
		push 0;
		call ItemFilter::DrawGroundItemRect;
		ret 0x18;
	}
}

void __declspec(naked) __stdcall ItemFilter::DrawAltDownItemRect_STUB_114d() {
	__asm
	{
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		mov eax, [esp + 0x40];
		push[eax - 0x4];
		push 0;
		push [esp + 0x60];
		call ItemFilter::DrawGroundItemRect;
		ret 0x18;
	}
}

void __declspec(naked) __stdcall ItemFilter::DrawHoverItemRect_STUB() {
	__asm
	{
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		//seems to return item cursor is over on ground
		call D2CLIENT_fpGroundHoverUnit;
		push eax;
		push 1;
		push 0;
		call ItemFilter::DrawGroundItemRect;
		ret 0x18;
	}
}

//Copy what D2Client.6FB5B0F0(guessed Arg1, Arg2, Arg3, Arg4) is doing and put ptItem
void __declspec(naked) __stdcall ItemFilter::DrawInventoryItemRect_STUB_110f() {
	__asm
	{
		push[esp + 0x10];
		push[esp + 0x10];
		mov eax, [esp + 0x10];
		add eax, edx;
		push eax;
		mov eax, [esp + 0x10];
		add eax, ecx;
		push eax;
		push edx;
		push ecx;
		push[esp + 0x38];
		call ItemFilter::DrawInventoryItemRect;
		ret 0x10;
	}
}

void __declspec(naked) __stdcall ItemFilter::DrawInventoryItemRect_STUB() {
	__asm
	{
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x18];
		push[esp + 0x3C];
		call ItemFilter::DrawInventoryItemRect;
		ret 0x18;
	}
}

void __declspec(naked) __stdcall ItemFilter::DrawInventoryItemRect_STUB_114d() {
	__asm
	{
		push[esp + 0x10];
		push[esp + 0x10];
		mov eax, [esp + 0x10];
		add eax, edx;
		push eax;
		mov eax, [esp + 0x10];
		add eax, ecx;
		push eax;
		push edx;
		push ecx;
		push[esp + 0x2c];
		call ItemFilter::DrawInventoryItemRect;
		ret 0x10;
	}
}

//straight from plugy source but after (nopickup instead of soundchoasdebug)
void __declspec(naked) __stdcall ItemFilter::CallCommand_STUB_114d() {
	__asm
	{
		TEST EAX, EAX
		JE MANAGESOUNDCHAOSDEBUG
		PUSH EDI
		CALL ItemFilter::CallCommand
		TEST EAX, EAX
		JNZ MANAGESOUNDCHAOSDEBUG
		ADD DWORD PTR SS : [ESP] , 19
	MANAGESOUNDCHAOSDEBUG :
		RETN 8
	}
}

void __declspec(naked) __stdcall ItemFilter::CallCommand_STUB() {
	__asm
	{
		TEST EAX, EAX
		JE MANAGESOUNDCHAOSDEBUG
		PUSH ESI
		CALL ItemFilter::CallCommand
		TEST EAX, EAX
		JNZ MANAGESOUNDCHAOSDEBUG
		ADD DWORD PTR SS : [ESP] , 21
	MANAGESOUNDCHAOSDEBUG :
		RETN 8
	}
}