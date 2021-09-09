#pragma once

#include <Windows.h>
#include <string>
#include "Expression.h"
#include "Utils.h"
#include "D2Structs.h"


struct ActionResult {
	std::vector<uint32_t> vMatchedRules;

	bool bHide = false;
	bool bContinue = false;

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


enum class ActionType : uint8_t {
	NONE, SHOW, HIDE, CONTINUE, SET_STYLE, SET_NAME,
	SET_DESCRIPTION, SET_BG_COLOR, SET_INVENTORY_COLOR,
	SET_BORDER_COLOR, CHAT_NOTIFY, PLAY_ALERT, MINIMAP_ICON
};

class Action {
protected:
	ActionType m_Type;
	std::wstring m_Value;
public:
	Action(std::wstring value = L"", ActionType type = ActionType::NONE) : m_Value(value), m_Type(type) {};
	ActionType GetType() { return m_Type; }
	virtual void SetResult(ActionResult* pResult, Unit* pItem) = 0;
};

class ShowAction : public Action {
public:
	ShowAction(std::wstring value = L"") : Action(value, ActionType::SHOW) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class HideAction : public Action {
public:
	HideAction(std::wstring value = L"") : Action(value, ActionType::HIDE) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class ContinueAction : public Action {
public:
	ContinueAction(std::wstring value = L"") : Action(value, ActionType::CONTINUE) { };
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class ColorTextAction : public Action {
public:
	ColorTextAction(std::wstring value = L"", ActionType type = ActionType::NONE);
	virtual void SetResult(ActionResult* pResult, Unit* pItem) = 0;
};

class PaletteIndexAction : public Action {
protected:
	uint8_t m_PaletteIndex = 0;
public:
	PaletteIndexAction(std::wstring value = L"", ActionType type = ActionType::NONE);
	virtual void SetResult(ActionResult* pResult, Unit* pItem) = 0;
};

class SetStyleAction : public Action {
public:
	SetStyleAction(std::wstring value = L"") : Action(value, ActionType::SET_STYLE) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class SetNameAction : public ColorTextAction {
public:
	SetNameAction(std::wstring value = L"") : ColorTextAction(value, ActionType::SET_NAME) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class SetDescriptionAction : public ColorTextAction {
public:
	SetDescriptionAction(std::wstring value = L"") : ColorTextAction(value, ActionType::SET_DESCRIPTION) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class SetBackgroundColorAction : public PaletteIndexAction {
public:
	SetBackgroundColorAction(std::wstring value = L"") : PaletteIndexAction(value, ActionType::SET_BORDER_COLOR) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class SetInventoryColorAction : public PaletteIndexAction {
public:
	SetInventoryColorAction(std::wstring value = L"") : PaletteIndexAction(value, ActionType::SET_INVENTORY_COLOR) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class SetBorderColorAction : public PaletteIndexAction {
public:
	SetBorderColorAction(std::wstring value = L"") : PaletteIndexAction(value, ActionType::SET_BORDER_COLOR) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class ChatNotifyAction : public Action {
protected:
	Expression* m_Expression;
public:
	ChatNotifyAction(std::wstring value = L"");
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class PlayAlertAction : public Action {
public:
	PlayAlertAction(std::wstring value = L"") : Action(value, ActionType::PLAY_ALERT) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};

class MinimapIconAction : public PaletteIndexAction {
public:
	MinimapIconAction(std::wstring value = L"") : PaletteIndexAction(value, ActionType::MINIMAP_ICON) {};
	void SetResult(ActionResult* pResult, Unit* pItem) override;
};