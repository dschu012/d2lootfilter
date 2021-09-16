#include <Windows.h>
#include <unordered_map>
#include "Action.h"
#include "D2Structs.h"
#include "D2Ptrs.h"
#include "ItemFilter.h"

typedef std::wstring(*TokenReplaceFunction)(ActionResult* action, Unit* pItem);

#define COLOR(STR, IDX) { L"{"#STR"}", ##IDX## }
static std::unordered_map<std::wstring, std::wstring> COLOR_TO_STRING = {
	COLOR(White, TEXT_WHITE),
	COLOR(Red, TEXT_RED),
	COLOR(Green, TEXT_GREEN),
	COLOR(Blue, TEXT_BLUE),
	COLOR(Gold, TEXT_GOLD),
	COLOR(Gray, TEXT_GRAY),
	COLOR(Grey, TEXT_GRAY),
	COLOR(Black, TEXT_BLACK),
	COLOR(Tan, TEXT_TAN),
	COLOR(Orange, TEXT_ORANGE),
	COLOR(Yellow, TEXT_YELLOW),
	COLOR(Purple, TEXT_PURPLE),
	COLOR(Dark Green, TEXT_DARK_GREEN),
	//Glide Only
	COLOR(Coral, TEXT_CORAL),
	COLOR(Sage, TEXT_SAGE),
	COLOR(Teal, TEXT_TEAL),
	COLOR(Light Gray, TEXT_LIGHT_GRAY),
	COLOR(Light Grey, TEXT_LIGHT_GRAY)
};
#undef COLOR

#define COLOR(STR, IDX) { L#STR, ##IDX## }, { L"\""#STR"\"", ##IDX## } 
static std::unordered_map<std::wstring, uint8_t> COLOR_TO_PALETTE_IDX = {
	COLOR(White, 0x20),
	COLOR(White, 0x20),
	COLOR(Red, 0x0A),
	COLOR(Green, 0x84),
	COLOR(Blue, 0x97),
	COLOR(Gold, 0x0D),
	COLOR(Gray, 0xD0),
	COLOR(Grey, 0xD0),
	COLOR(Black, 0x00),
	COLOR(Tan, 0x5A),
	COLOR(Orange, 0x60),
	COLOR(Yellow, 0x0C),
	COLOR(Purple, 0x9B),
	COLOR(Dark Green, 0x76),
	COLOR(Coral, 0x66),
	COLOR(Sage, 0x82),
	COLOR(Teal, 0xCB),
	COLOR(Light Gray, 0xD6),
	COLOR(Light Grey, 0xD6)
};
#undef COLOR

static std::wstring GetItemName(ActionResult* action, Unit* pItem) {
	if (action->bItemNameSet) {
		return action->wsItemName;
	} else {
		return L"{Name}";
	}
}

static std::wstring GetItemDesc(ActionResult* action, Unit* pItem) {
	if (action->bItemDescSet) {
		return action->wsItemDesc;
	} else {
		return  L"{Description}";
	}
}

static std::wstring GetPotionNumber(ActionResult* action, Unit* pItem) {
	if(D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::HEALING_POTION)
		|| D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::MANA_POTION)) {
		return GetItemCode(pItem).substr(2);
	}
	if (D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::REJUV_POTION)) {
		std::wstring code = GetItemCode(pItem);
		return code == L"rvl" ? L"2" : L"1";
	}
	if (D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::POTION)) {
		return L"1";
	}
	return L"";
}

static std::wstring GetRuneNumber(ActionResult* action, Unit* pItem) {
	if (D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::RUNE)) {
		return std::to_wstring(_wtoi(GetItemCode(pItem).substr(1).c_str()));
	}
	return L"";
}

static std::wstring Newline(ActionResult* action, Unit* pItem) {
	return L"\n";
}

static std::wstring GetItemPrice(ActionResult* action, Unit* pItem) {
	int nPrice = 0;
	Unit* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pItem != NULL && pPlayer != NULL) {
		nPrice = D2COMMON_ITEMS_GetTransactionCost(pPlayer, pItem, D2CLIENT_GetDifficulty(), D2CLIENT_GetQuestFlags(), 0x201, 1);
	}
	return std::to_wstring(nPrice);
}

static std::wstring GetItemSockets(ActionResult* action, Unit* pItem) {
	return std::to_wstring(D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, Stat::ITEM_NUMSOCKETS, 0));
}


ColorTextAction::ColorTextAction(std::wstring value, ActionType type) : Action(value, type) {
	for (auto const& color : COLOR_TO_STRING) {
		replace(m_Value, color.first, color.second);
	}
}

PaletteIndexAction::PaletteIndexAction(std::wstring value, ActionType type) : Action(value, type) {
	if (COLOR_TO_PALETTE_IDX.contains(value)) {
		m_PaletteIndex = COLOR_TO_PALETTE_IDX[value];
	}
	else {
		m_PaletteIndex = static_cast<uint8_t>(std::stoi(value, nullptr, 16));
	}
}


void HideAction::SetResult(ActionResult* action, Unit* pItem) {
	action->bHide = true;
}

void ShowAction::SetResult(ActionResult* action, Unit* pItem) {
	action->bHide = false;
}

void ContinueAction::SetResult(ActionResult* action, Unit* pItem) {
	action->bContinue = true;
}

void SetStyleAction::SetResult(ActionResult* action, Unit* pItem) {
	if (GlobalStyles.contains(m_Value)) {
		for (auto act : GlobalStyles[m_Value]) {
			act->SetResult(action, pItem);
		}
	}
}

void SetNameAction::SetResult(ActionResult* action, Unit* pItem) {
	std::unordered_map<std::wstring, TokenReplaceFunction> TOKEN_REPLACEMENT_FUNCTIONS = {
		{ L"{Name}", &GetItemName },
		{ L"{Sockets}", &GetItemSockets },
		{ L"{Price}", &GetItemPrice },
		{ L"{Rune Number}", &GetRuneNumber },
		{ L"{Potion Number}", &GetPotionNumber },
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

void SetDescriptionAction::SetResult(ActionResult* action, Unit* pItem) {
	std::unordered_map<std::wstring, TokenReplaceFunction> TOKEN_REPLACEMENT_FUNCTIONS = {
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

void SetBackgroundColorAction::SetResult(ActionResult* action, Unit* pItem) {
	action->bBackgroundPaletteIndexSet = true;
	action->nBackgroundPaletteIndex = m_PaletteIndex;
}

void SetInventoryColorAction::SetResult(ActionResult* action, Unit* pItem) {
	action->bInvBackgroundPaletteIndexSet = true;
	action->nInvBackgroundPaletteIndex = m_PaletteIndex;
}

void SetBorderColorAction::SetResult(ActionResult* action, Unit* pItem) {
	action->bBorderPaletteIndexSet = true;
	action->nBorderPaletteIndex = m_PaletteIndex;
}

ChatNotifyAction::ChatNotifyAction(std::wstring value) : Action(value, ActionType::CHAT_NOTIFY) {
	m_Expression = Parser::Parse(value.c_str());
}

void ChatNotifyAction::SetResult(ActionResult* action, Unit* pItem) {
	action->bChatAlert = m_Expression->Evaluate(pItem) != 0;
}

void PlayAlertAction::SetResult(ActionResult* action, Unit* pItem) {
}

void MinimapIconAction::SetResult(ActionResult* action, Unit* pItem) {
	action->bMinimapIcon = true;
	action->nMinimapIconPaletteIndex = m_PaletteIndex;
}