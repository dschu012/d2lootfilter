#pragma once

#include <stdexcept>
#include <Windows.h>
#include <map>
#include <fmt/format.h>
#include <fmt/core.h>
#include "Utils.h"
#include "D2Ptrs.h"
#include "D2Structs.h"
#include "Globals.h"
#include "Expression.h"


class Condition {
protected:
	std::wstring m_Value;
public:
	Condition() {};
	Condition(std::wstring value) : m_Value(value) {};
	~Condition() {};
	virtual void Initialize() {};
	virtual bool Evaluate(Unit* pItem) = 0;
};


class AndCondition : public Condition {
protected:
	std::vector<Condition*> m_Conditions;
public:
	AndCondition() {};
	AndCondition(std::vector<Condition*> condition) : m_Conditions(condition) {};
	void AddCondition(Condition* condition) { m_Conditions.push_back(condition); };
	void Initialize() {
		for (auto& condition : m_Conditions) {
			condition->Initialize();
		}
	};
	bool Evaluate(Unit* pItem);
};

/*
Handles scenarios where the LHS of a condition is dynamic. i.e.
FireResist > 10
*/
template <class T>
class LHSCondition : public Condition {
protected:
	Var<T>* m_Lhs;
	ListExpr<T>* m_Expression;
public:
	LHSCondition(std::wstring variable, std::wstring value) : Condition(value) {
		m_Lhs = new Var<T>(variable);
		m_Expression = Parser<T>::Parse(m_Lhs, m_Value.c_str());
	};
	virtual bool Evaluate(Unit* pItem) = 0;
};

template <class T>
class CallCondition : public Condition {
protected:
	ListExpr<T>* m_Expression;
public:
	CallCondition(std::wstring func, std::wstring value) : Condition(value) {
		m_Expression = Parser<T>::ParseCall(func.c_str(), m_Value.c_str());
	};
	virtual bool Evaluate(Unit* pItem) = 0;
};

class UnsignedStatCondition : public LHSCondition<uint32_t> {
protected:
	Stat m_Stat;
public:
	UnsignedStatCondition(std::wstring variable, Stat stat, std::wstring value) : LHSCondition<uint32_t>(variable, value) {
		m_Stat = stat;
	};
	bool Evaluate(Unit* pItem) {
		m_Lhs->SetValue((D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, m_Stat, 0)));
		return m_Expression->Evaluate(pItem);
	}
};

class CodeCondition : public LHSCondition<uint32_t> {
public:
	//todo.. clean this up
	CodeCondition(std::wstring value) : LHSCondition<uint32_t>(L"Code", value) {};
	void Initialize() override {
		m_Expression->SetVariables(ITEM_CODE_LOOKUP_TABLE);
	};
	bool Evaluate(Unit* pItem);
};

//Simple name from Weapons.txt, Armor.txt, Misc.txt
class TypeCondition : public LHSCondition<uint32_t> {
public:
	//todo.. clean this up
	TypeCondition(std::wstring value) : LHSCondition<uint32_t>(L"Type", value) {};
	void Initialize() override {
		m_Expression->SetVariables(ITEM_NAME_LOOKUP_TABLE);
	};
	bool Evaluate(Unit* pItem);
};

//Simple name from ItemTypes.txt
class ClassCondition : public CallCondition<uint16_t> {
public:
	ClassCondition(std::wstring value) : CallCondition<uint16_t>(L"Class", value) {};
	void Initialize() override  {
		m_Expression->SetVariables(ITEM_TYPE_LOOKUP_TABLE);
	};
	bool Evaluate(Unit* pItem);
};

class RarityCondition : public LHSCondition<uint8_t> {
public:
	RarityCondition(std::wstring value) : LHSCondition<uint8_t>(L"Rarity", value) {
		std::map<std::wstring, uint8_t> VARIABLES = {
				{ L"Inferior", 0x1 },
				{ L"Normal", 0x2 },
				{ L"Superior", 0x3 },
				{ L"Magic", 0x4 },
				{ L"Set", 0x5 },
				{ L"Rare", 0x6 },
				{ L"Unique", 0x7 },
				{ L"Crafted", 0x8 },
				{ L"Tempered", 0x9 }
		};
		m_Expression->SetVariables(VARIABLES);
	};
	bool Evaluate(Unit* pItem);
};

class EtherealCondition : public LHSCondition<uint8_t> {
public:
	EtherealCondition(std::wstring value) : LHSCondition<uint8_t>(L"Ethereal", value) {};
	bool Evaluate(Unit* pItem);
};

class RunewordCondition : public LHSCondition<uint8_t> {
public:
	RunewordCondition(std::wstring value) : LHSCondition<uint8_t>(L"Runeword", value) {};
	bool Evaluate(Unit* pItem);
};

class PrefixCondition : public LHSCondition<uint32_t> {
public:
	PrefixCondition(std::wstring value) : LHSCondition<uint32_t>(L"Prefix", value) {};
	bool Evaluate(Unit* pItem);
};

//TODO
class SuffixCondition : public LHSCondition<uint32_t> {
public:
	SuffixCondition(std::wstring value) : LHSCondition<uint32_t>(L"Suffix", value) {};
	bool Evaluate(Unit* pItem);
};

//A, I, C Level
class ItemLevelCondition : public LHSCondition<uint32_t> {
public:
	ItemLevelCondition(std::wstring value) : LHSCondition<uint32_t>(L"ItemLevel", value) {};
	bool Evaluate(Unit* pItem);
};

class QualityCondition : public LHSCondition<uint8_t> {
public:
	QualityCondition(std::wstring value) : LHSCondition<uint8_t>(L"Quality", value) {
		std::map<std::wstring, uint8_t> VARIABLES = {
			{ L"Unknown", static_cast<std::underlying_type_t<ItemQualityLevel>>(ItemQualityLevel::UNKNOWN) },
			{ L"Normal", static_cast<std::underlying_type_t<ItemQualityLevel>>(ItemQualityLevel::NORMAL) },
			{ L"Exceptional", static_cast<std::underlying_type_t<ItemQualityLevel>>(ItemQualityLevel::EXCEPTIONAL) },
			{ L"Elite", static_cast<std::underlying_type_t<ItemQualityLevel>>(ItemQualityLevel::ELITE) }
		};
		m_Expression->SetVariables(VARIABLES);
	};
	bool Evaluate(Unit* pItem);
};

//todo...? kinda useless
class AreaLevelCondition : public Condition {
public:
	AreaLevelCondition(std::wstring value) : Condition(value) {};
	bool Evaluate(Unit* pItem) { return false; };
};

class CharacterLevelCondition : public LHSCondition<int> {
public:
	CharacterLevelCondition(std::wstring value) : LHSCondition<int>(L"CharacterLevel", value) {};
	bool Evaluate(Unit* pItem);
};

class DifficultyCondition : public LHSCondition<uint8_t> {
public:
	DifficultyCondition(std::wstring value) : LHSCondition<uint8_t>(L"Difficulty", value) {
		std::map<std::wstring, uint8_t> VARIABLES = {
				{ L"Normal", 0 },
				{ L"Nightmare", 1 },
				{ L"Hell", 2 }
		};
		m_Expression->SetVariables(VARIABLES);
	};
	bool Evaluate(Unit* pItem);
};

class RuneCondition : public LHSCondition<uint32_t> {
public:
	RuneCondition(std::wstring value) : LHSCondition<uint32_t>(L"Rune", value) {};
	void Initialize() override {
		m_Expression->SetVariables(RUNE_NAME_LOOKUP_TABLE);
	};
	bool Evaluate(Unit* pItem);
};

class IdCondition : public LHSCondition<uint32_t> {
public:
	IdCondition(std::wstring value) : LHSCondition<uint32_t>(L"Id", value) {};
	bool Evaluate(Unit* pItem);
};

class GoldCondition : public UnsignedStatCondition {
public:
	GoldCondition(std::wstring value) : UnsignedStatCondition(L"Gold", Stat::GOLD, value) {};
	bool Evaluate(Unit* pItem);
};

//Stats

//unsigned stat
#define USTAT(x, y)\
 class x##Condition : public UnsignedStatCondition { \
 public: \
	x##Condition(std::wstring value): UnsignedStatCondition(L#x, y, value) {}; \
};

//todo
class AllResistCondition : public Condition {
public:
	AllResistCondition(std::wstring value) : Condition(value) {};
	bool Evaluate(Unit* pItem) { return true; };
};

class StatsCondition : public Condition {
protected:
	Expr<int>* m_Expression;
public:
	StatsCondition(std::wstring value) : Condition(value) {
		for (auto& stat : STAT_LOOKUP_TABLE) {
			replace(m_Value, stat.first, stat.second);
		}
		m_Expression = nullptr;
	};
	void Initialize() {
		for (auto& stat : SKILL_LOOKUP_TABLE) {
			replace(m_Value, stat.first, stat.second);
		}
		m_Expression = Parser<int>::Parse(m_Value.c_str());
	};
	bool Evaluate(Unit* pItem);
};

USTAT(Defense, Stat::ARMORCLASS);

//Armor

class ArmorCondition : public LHSCondition<uint8_t> {
public:
	ArmorCondition(std::wstring value) : LHSCondition<uint8_t>(L"Armor", value) {};
	bool Evaluate(Unit* pItem);
};

//Weapons
class WeaponCondition : public LHSCondition<uint8_t> {
public:
	WeaponCondition(std::wstring value) : LHSCondition<uint8_t>(L"Weapon", value) {};
	bool Evaluate(Unit* pItem);
};

//Misc
class PriceCondition : public LHSCondition<int> {
public:
	PriceCondition(std::wstring value) : LHSCondition<int>(L"Price", value) {};
	bool Evaluate(Unit* pItem);
};

//todo
class ModeCondition : public Condition {
public:
	ModeCondition(std::wstring value) : Condition(value) {};
	bool Evaluate(Unit* pItem);
};

class IdentifiedCondition : public LHSCondition<uint8_t> {
public:
	IdentifiedCondition(std::wstring value) : LHSCondition<uint8_t>(L"Identified", value) {};
	bool Evaluate(Unit* pItem);
};

USTAT(Sockets, Stat::ITEM_NUMSOCKETS);

class WidthCondition : public LHSCondition<uint8_t> {
public:
	WidthCondition(std::wstring value) : LHSCondition<uint8_t>(L"Width", value) {};
	bool Evaluate(Unit* pItem);
};

class HeightCondition : public LHSCondition<uint8_t> {
public:
	HeightCondition(std::wstring value) : LHSCondition<uint8_t>(L"Height", value) {};
	bool Evaluate(Unit* pItem);
};
