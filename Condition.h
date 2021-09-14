#pragma once
#include <string>
#include <format>
#include "D2Structs.h"
#include "D2Ptrs.h"
#include "Expression.h"
#include <unordered_map>

#pragma warning( push )
#pragma warning( disable : 26495 )

//to be able to access conditions by type to set variables and such
enum class ConditionType : uint8_t {
	NONE, CODE, TYPE, PLAYERCLASS, CLASS, RARITY, ETHEREAL, RUNEWORD, PREFIX, SUFFIX,
	ITEM_LEVEL, QUALITY, AREA_LEVEL, CHARACTER_LEVEL, DIFFICULTY,
	RUNE, ID, GOLD, STATS, DEFENSE, ARMOR, WEAPON, PRICE, MODE,
	IDENTIFIED, SOCKETS, WIDTH, HEIGHT, RANDOM, OWNED
};

static const wchar_t* CONDITIONS[] = { L"", L"Code", L"Type", L"PlayerClass", L"Class", L"Rarity", L"Ethereal", L"Runeword", L"Prefix", L"Suffix", L"ItemLevel", L"Quality", L"AreaLevel", L"CharacterLevel",
	L"Difficulty", L"Rune", L"Id", L"Gold", L"Stats", L"Defense", L"Armor", L"Weapon", L"Price", L"Mode", L"Identified", L"Sockets", L"Width", L"Height", L"Random", L"Owned" };

class Condition {
protected:
	ConditionType m_Type;
	std::wstring m_Value;
public:
	Condition(std::wstring value = L"", ConditionType type = ConditionType::NONE) : m_Value(value), m_Type(type) {};
	ConditionType GetType() { return m_Type; }
	virtual bool Evaluate(Unit* pItem) = 0;
	virtual void Initialize(std::unordered_map<std::wstring, int32_t> variables) {};
	virtual std::wstring ToString(Unit* pItem) = 0;
};


class CodeCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	CodeCondition(std::wstring value = L"") : Condition(value, ConditionType::CODE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class PlayerClassCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	PlayerClassCondition(std::wstring value = L"") : Condition(value, ConditionType::PLAYERCLASS) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };

};

class TypeCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	TypeCondition(std::wstring value = L"") : Condition(value, ConditionType::TYPE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class ClassCondition : public Condition {
protected:
	Variable* m_Left;
	Expression* m_Expression;
public:
	ClassCondition(std::wstring value = L"") : Condition(value, ConditionType::CLASS) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class RarityCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	RarityCondition(std::wstring value = L"") : Condition(value, ConditionType::RARITY) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class EtherealCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	EtherealCondition(std::wstring value = L"") : Condition(value, ConditionType::ETHEREAL) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class RunewordCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	RunewordCondition(std::wstring value = L"") : Condition(value, ConditionType::RUNEWORD) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class PrefixCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	PrefixCondition(std::wstring value = L"") : Condition(value, ConditionType::PREFIX) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class SuffixCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	SuffixCondition(std::wstring value = L"") : Condition(value, ConditionType::SUFFIX) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class ItemLevelCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	ItemLevelCondition(std::wstring value = L"") : Condition(value, ConditionType::ITEM_LEVEL) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class QualityCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	QualityCondition(std::wstring value = L"") : Condition(value, ConditionType::QUALITY) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class AreaLevelCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	AreaLevelCondition(std::wstring value = L"") : Condition(value, ConditionType::AREA_LEVEL) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class CharacterLevelCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	CharacterLevelCondition(std::wstring value = L"") : Condition(value, ConditionType::CHARACTER_LEVEL) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class DifficultyCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	DifficultyCondition(std::wstring value = L"") : Condition(value, ConditionType::DIFFICULTY) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class RuneCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	RuneCondition(std::wstring value = L"") : Condition(value, ConditionType::RUNE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class IdCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	IdCondition(std::wstring value = L"") : Condition(value, ConditionType::ID) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class GoldCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	GoldCondition(std::wstring value = L"") : Condition(value, ConditionType::GOLD) {};
	bool Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	
};

class StatsCondition : public Condition {
protected:
	Expression* m_Expression;
public:
	StatsCondition(std::wstring value = L"") : Condition(value, ConditionType::STATS) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class DefenseCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	DefenseCondition(std::wstring value = L"") : Condition(value, ConditionType::DEFENSE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class ArmorCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	ArmorCondition(std::wstring value = L"") : Condition(value, ConditionType::ARMOR) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class WeaponCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	WeaponCondition(std::wstring value = L"") : Condition(value, ConditionType::WEAPON) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class PriceCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	PriceCondition(std::wstring value = L"") : Condition(value, ConditionType::PRICE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class ModeCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	ModeCondition(std::wstring value = L"") : Condition(value, ConditionType::MODE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class IdentifiedCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	IdentifiedCondition(std::wstring value = L"") : Condition(value, ConditionType::IDENTIFIED) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class SocketsCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	SocketsCondition(std::wstring value = L"") : Condition(value, ConditionType::SOCKETS) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class WidthCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	WidthCondition(std::wstring value = L"") : Condition(value, ConditionType::WIDTH) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class HeightCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	HeightCondition(std::wstring value = L"") : Condition(value, ConditionType::HEIGHT) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
};

class RandomCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	RandomCondition(std::wstring value = L"") : Condition(value, ConditionType::RANDOM) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };

};

class OwnedCondition : public Condition {
protected:
	Variable* m_Left;
	ListExpression* m_Expression;
public:
	OwnedCondition(std::wstring value = L"") : Condition(value, ConditionType::OWNED) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(std::unordered_map<std::wstring, int32_t> variables) override;
	std::wstring ToString(Unit* pItem) override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };

};

#pragma warning( pop )