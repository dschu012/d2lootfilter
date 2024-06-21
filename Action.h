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

	int32_t nWeight = 0;
};


enum class ActionType : uint8_t {
	NONE, SHOW, HIDE, CONTINUE, SET_STYLE, SET_NAME,
	SET_DESCRIPTION, SET_BG_COLOR, SET_INVENTORY_COLOR,
	SET_BORDER_COLOR, CHAT_NOTIFY, PLAY_ALERT, MINIMAP_ICON,
	WEIGHT
};

class Action {
protected:
	ActionType m_Type;
	std::wstring m_Value;
public:
	Action(std::wstring_view value = {}, ActionType type = ActionType::NONE) : m_Value(value), m_Type(type) {};
	virtual ~Action() = default;
	ActionType GetType() const { return m_Type; }
	virtual void Initialize(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) {};
	virtual void SetResult(ActionResult& action, Unit* pItem) const = 0;
};

class ShowAction : public Action {
public:
	ShowAction(std::wstring_view value = {}) : Action(value, ActionType::SHOW) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<ShowAction>(value); }
};

class HideAction : public Action {
public:
	HideAction(std::wstring_view value = {}) : Action(value, ActionType::HIDE) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<HideAction>(value); }
};

class ContinueAction : public Action {
public:
	ContinueAction(std::wstring_view value = {}) : Action(value, ActionType::CONTINUE) { };
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<ContinueAction>(value); }
};

class ColorTextAction : public Action {
public:
	ColorTextAction(std::wstring_view value = {}, ActionType type = ActionType::NONE);
	virtual void SetResult(ActionResult& action, Unit* pItem) const = 0;
};

class PaletteIndexAction : public Action {
protected:
	uint8_t m_PaletteIndex = 0;
public:
	PaletteIndexAction(std::wstring_view value = {}, ActionType type = ActionType::NONE);
	virtual void SetResult(ActionResult& action, Unit* pItem) const = 0;
};

class SetStyleAction : public Action {
public:
	SetStyleAction(std::wstring_view value = {}) : Action(value, ActionType::SET_STYLE) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<SetStyleAction>(value); }
};

class SetNameAction : public ColorTextAction {
public:
	SetNameAction(std::wstring_view value = {}) : ColorTextAction(value, ActionType::SET_NAME) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<SetNameAction>(value); }
};

class SetDescriptionAction : public ColorTextAction {
public:
	SetDescriptionAction(std::wstring_view value = {}) : ColorTextAction(value, ActionType::SET_DESCRIPTION) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<SetDescriptionAction>(value); }
};

class SetBackgroundColorAction : public PaletteIndexAction {
public:
	SetBackgroundColorAction(std::wstring_view value = {}) : PaletteIndexAction(value, ActionType::SET_BORDER_COLOR) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<SetBackgroundColorAction>(value); }
};

class SetInventoryColorAction : public PaletteIndexAction {
public:
	SetInventoryColorAction(std::wstring_view value = {}) : PaletteIndexAction(value, ActionType::SET_INVENTORY_COLOR) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<SetInventoryColorAction>(value); }
};

class SetBorderColorAction : public PaletteIndexAction {
public:
	SetBorderColorAction(std::wstring_view value = {}) : PaletteIndexAction(value, ActionType::SET_BORDER_COLOR) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<SetBorderColorAction>(value); }
};

class ChatNotifyAction : public Action {
protected:
	std::unique_ptr<Expression> m_Expression;
public:
	ChatNotifyAction(std::wstring_view value = {});
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<ChatNotifyAction>(value); }
};

class PlayAlertAction : public Action {
public:
	PlayAlertAction(std::wstring_view value = {}) : Action(value, ActionType::PLAY_ALERT) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<PlayAlertAction>(value); }
};

class MinimapIconAction : public PaletteIndexAction {
public:
	MinimapIconAction(std::wstring_view value = {}) : PaletteIndexAction(value, ActionType::MINIMAP_ICON) {};
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<MinimapIconAction>(value); }
};

class WeightAction : public Action {
protected:
	std::unique_ptr<Expression> m_Expression;
public:
	WeightAction(std::wstring_view value = L"") : Action(value, ActionType::WEIGHT) {};
	void Initialize(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
	void SetResult(ActionResult& action, Unit* pItem) const override;

	static std::unique_ptr<Action> MakeInstance(std::wstring_view value = {}) { return std::make_unique<WeightAction>(value); }
};

class ActionFactory {
public:
	static std::unique_ptr<Action> MakeInstance(std::wstring_view action, std::wstring_view value = {}) {
		static const utility::string_umap_icase<std::wstring, std::unique_ptr<Action>(&)(std::wstring_view)> lookup = {
			{ L"Continue",				ContinueAction::MakeInstance },
			{ L"SetStyle",				SetStyleAction::MakeInstance },
			{ L"SetName",				SetNameAction::MakeInstance },
			{ L"SetDescription",		SetDescriptionAction::MakeInstance },
			{ L"SetBackgroundColor",	SetBackgroundColorAction::MakeInstance },
			{ L"SetInventoryColor",		SetInventoryColorAction::MakeInstance },
			{ L"SetBorderColor",		SetBorderColorAction::MakeInstance },
			{ L"ChatNotify",			ChatNotifyAction::MakeInstance },
			{ L"PlayAlert",				PlayAlertAction::MakeInstance },
			{ L"MinimapIcon",			MinimapIconAction::MakeInstance },
			{ L"Weight",				WeightAction::MakeInstance },
		};

		if (auto search = lookup.find(action); search != lookup.end()) {
			return search->second(value);
		}
		return nullptr;
	}
};