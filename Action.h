#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include "Utils.h"
#include "D2Structs.h"


struct ItemActionResult {
	std::vector<uint32_t> vMatchedRules;

	bool bHide = false;

	bool bBackgroundPaletteIndexSet = false;
	uint8_t nBackgroundPaletteIndex = 0;
	DrawMode eDrawModeAlt = DrawMode::TRANS_25;
	DrawMode eDrawModeHover = DrawMode::TRANS_50;

	bool bBorderPaletteIndexSet = false;
	uint8_t nBorderPaletteIndex = 0;

	bool bInvBackgroundPaletteIndexSet = false;
	uint8_t nInvBackgroundPaletteIndex = 0;

	bool bChatAlert = false;

	bool bItemNameSet = false;
	std::wstring wsItemName = L"";

	bool bItemDescSet = false;
	std::wstring wsItemDesc = L"";

	bool bMinimapIcon = false;
	uint8_t nMinimapIconPaletteIndex = 0;
};



class Action {
protected:
	std::wstring m_Value;
public:
	Action(std::wstring value) : m_Value(value) {};
	~Action() {};
	virtual void SetResult(ItemActionResult* action, Unit* pItem) = 0;
};

class HideAction : public Action {
public:
	HideAction() : Action(L"") {}
	void  SetResult(ItemActionResult* action, Unit* pItem);
};

class ShowAction : public Action {
public:
	ShowAction() : Action(L"") {}
	void SetResult(ItemActionResult* action, Unit* pItem);
};

class ColorTextAction : public Action {
public:
	ColorTextAction(std::wstring value) : Action(value) {
		std::map<std::wstring, std::wstring> COLOR_TO_PALETTE_IDX = {
			{ L"{White}", TEXT_WHITE },
			{ L"{Red}", TEXT_RED },
			{ L"{Green}", TEXT_GREEN },
			{ L"{Blue}", TEXT_BLUE },
			{ L"{Gold}", TEXT_GOLD },
			{ L"{Gray}", TEXT_GRAY },
			{ L"{Black}", TEXT_BLACK },
			{ L"{Tan}", TEXT_TAN },
			{ L"{Orange}", TEXT_ORANGE },
			{ L"{Yellow}", TEXT_YELLOW },
			{ L"{Purple}", TEXT_PURPLE },
			{ L"{Dark Green}", TEXT_DARK_GREEN },
			//Glide Only
			{ L"{Coral}", TEXT_CORAL },
			{ L"{Sage}", TEXT_SAGE },
			{ L"{Teal}", TEXT_TEAL },
			{ L"{Light Gray}", TEXT_LIGHT_GRAY }
		};
		for (auto const& color : COLOR_TO_PALETTE_IDX) {
			replace(m_Value, color.first, color.second);
		}
	}
	virtual void SetResult(ItemActionResult* action, Unit* pItem) = 0;
};

class PaletteIndexAction : public Action {
protected:
	uint8_t m_PaletteIndex = 0;
public:
	PaletteIndexAction(std::wstring value) : Action(value) {
		std::map<std::wstring, uint8_t> COLOR_TO_PALETTE_IDX = {
			{ L"White", 0x20 },
			{ L"Red", 0x0A },
			{ L"Green", 0x84 },
			{ L"Blue", 0x97 },
			{ L"Gold", 0x0D },
			{ L"Gray", 0xD0 },
			{ L"Black", 0x00 },
			{ L"Tan", 0x5A },
			{ L"Orange", 0x60 },
			{ L"Yellow", 0x0C },
			{ L"Purple", 0x9B },
			{ L"Dark Green", 0x76 },
			{ L"Coral", 0x66 },
			{ L"Sage", 0x82 },
			{ L"Teal", 0xCB },
			{ L"Light Gray", 0xD6 }
		};
		if (COLOR_TO_PALETTE_IDX.contains(value)) {
			m_PaletteIndex = COLOR_TO_PALETTE_IDX[value];
		}
		else {
			m_PaletteIndex = static_cast<uint8_t>(std::stoi(value, nullptr, 16));
		}
	}
	virtual void SetResult(ItemActionResult* action, Unit* pItem) = 0;
};

class SetStyleAction : public Action {
public:
	SetStyleAction(std::wstring value) : Action(value) { }
	void SetResult(ItemActionResult* action, Unit* pItem);
};

class SetNameAction : public ColorTextAction {
public:
	SetNameAction(std::wstring value) : ColorTextAction(value) { }
	void SetResult(ItemActionResult* action, Unit* pItem);
};

class SetDescriptionAction : public ColorTextAction {
public:
	SetDescriptionAction(std::wstring value) : ColorTextAction(value) {}
	void  SetResult(ItemActionResult* action, Unit* pItem);
};

class SetBackgroundColorAction : public PaletteIndexAction {
public:
	SetBackgroundColorAction(std::wstring value) : PaletteIndexAction(value) {}
	void SetResult(ItemActionResult* action, Unit* pItem);
};

class SetInventoryColorAction : public PaletteIndexAction {
public:
	SetInventoryColorAction(std::wstring value) : PaletteIndexAction(value) {}
	void SetResult(ItemActionResult* action, Unit* pItem);
};

class SetBorderColorAction : public PaletteIndexAction {
public:
	SetBorderColorAction(std::wstring value) : PaletteIndexAction(value) {}
	void SetResult(ItemActionResult* action, Unit* pItem);
};

class ChatNotifyAction : public Action {
public:
	ChatNotifyAction(std::wstring value) : Action(value) {}
	void  SetResult(ItemActionResult* action, Unit* pItem);
};

class PlayAlertAction : public Action {
public:
	PlayAlertAction(std::wstring value) : Action(value) {}
	void  SetResult(ItemActionResult* action, Unit* pItem);
};

class MinimapIconAction : public PaletteIndexAction {
public:
	MinimapIconAction(std::wstring value) : PaletteIndexAction(value) {}
	void  SetResult(ItemActionResult* action, Unit* pItem);
};