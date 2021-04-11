#include <Windows.h>
#include <map>
#include <fmt/core.h>
#include "Action.h"
#include "D2Structs.h"
#include "D2Ptrs.h"
#include "ItemFilter.h"

typedef std::wstring(*TokenReplaceFunction)(ItemActionResult* action, Unit* pItem);

static std::wstring GetItemName(ItemActionResult* action, Unit* pItem) {
	if (action->bItemNameSet) {
		return action->wsItemName;
	} else {
		return L"{Name}";
	}
}

static std::wstring GetItemDesc(ItemActionResult* action, Unit* pItem) {
	if (action->bItemDescSet) {
		return action->wsItemDesc;
	} else {
		return  L"{Description}";
	}
}

static std::wstring GetRuneNumber(ItemActionResult* action, Unit* pItem) {
	if (!D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::RUNE)) {
		return L"";
	}
	ItemsTxt pItemTxt = D2COMMON_ItemDataTbl->pItemsTxt[pItem->dwLineId];
	return std::to_wstring(std::stoi(std::string(&pItemTxt.szCode[1], 3)));
}

static std::wstring Newline(ItemActionResult* action, Unit* pItem) {
	return L"\n";
}

static std::wstring GetItemPrice(ItemActionResult* action, Unit* pItem) {
	int nPrice = 0;
	Unit* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pItem != NULL && pPlayer != NULL) {
		nPrice = D2COMMON_ITEMS_GetTransactionCost(pPlayer, pItem, D2CLIENT_GetDifficulty(), D2CLIENT_GetQuestFlags(), 0x201, 1);
	}
	return std::to_wstring(nPrice);
}

static std::wstring GetItemSockets(ItemActionResult* action, Unit* pItem) {
	return std::to_wstring(D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, Stat::ITEM_NUMSOCKETS, 0));
}

void HideAction::SetResult(ItemActionResult* action, Unit* pItem) {
	action->bHide = true;
}

void ShowAction::SetResult(ItemActionResult* action, Unit* pItem) {
	action->bHide = false;
}

void SetNameAction::SetResult(ItemActionResult* action, Unit* pItem) {
	std::map<std::wstring, TokenReplaceFunction> TOKEN_REPLACEMENT_FUNCTIONS = {
		{ L"{Name}", &GetItemName },
		{ L"{Sockets}", &GetItemSockets },
		{ L"{Price}", &GetItemPrice },
		{ L"{Rune Number}", &GetRuneNumber },
		{ L"{Newline}", &Newline }
	};
	//we got here from a CONTINUE
	std::wstring result = m_Value;
	for (const auto& token : TOKEN_REPLACEMENT_FUNCTIONS) {
		replace(result, token.first, token.second(action, pItem));
	}
	action->wsItemName = result;
	action->bItemNameSet = true;
}

void SetDescriptionAction::SetResult(ItemActionResult* action, Unit* pItem) {
	std::map<std::wstring, TokenReplaceFunction> TOKEN_REPLACEMENT_FUNCTIONS = {
		{ L"{Description}", &GetItemDesc },
		{ L"{Sockets}", &GetItemSockets },
		{ L"{Price}", &GetItemPrice },
		{ L"{Rune Number}", &GetRuneNumber },
		{ L"{Newline}", &Newline }
	};
	//we got here from a CONTINUE
	std::wstring result = m_Value;
	for (const auto& token : TOKEN_REPLACEMENT_FUNCTIONS) {
		replace(result, token.first, token.second(action, pItem));
	}
	action->wsItemDesc = result;
	action->bItemDescSet = true;
}

void SetBackgroundColorAction::SetResult(ItemActionResult* action, Unit* pItem) {
	action->bBackgroundPaletteIndexSet = true;
	action->nBackgroundPaletteIndex = m_PaletteIndex;
}

void SetInventoryColorAction::SetResult(ItemActionResult* action, Unit* pItem) {
	action->bInvBackgroundPaletteIndexSet = true;
	action->nInvBackgroundPaletteIndex = m_PaletteIndex;
}

void SetBorderColorAction::SetResult(ItemActionResult* action, Unit* pItem) {
	action->bBorderPaletteIndexSet = true;
	action->nBorderPaletteIndex = m_PaletteIndex;
}

void ChatNotifyAction::SetResult(ItemActionResult* action, Unit* pItem) {
	action->bChatAlert = true;
}

void PlayAlertAction::SetResult(ItemActionResult* action, Unit* pItem) {
}

void MinimapIconAction::SetResult(ItemActionResult* action, Unit* pItem) {
	action->bMinimapIcon = true;
	action->nMinimapIconPaletteIndex = m_PaletteIndex;
}
