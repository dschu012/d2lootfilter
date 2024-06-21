#pragma once
#include <string>
#include <format>
#include "D2Structs.h"
#include "D2Ptrs.h"
#include "Expression.h"
#include <unordered_map>
#include "Utils.h"

//to be able to access conditions by type to set variables and such
enum class ConditionType : uint8_t {
	NONE, CODE, TYPE, PLAYERCLASS, CLASS, RARITY, ETHEREAL, RUNEWORD, PREFIX, SUFFIX,
	ITEM_LEVEL, QUALITY, AREA_LEVEL, CHARACTER_LEVEL, DIFFICULTY,
	RUNE, ID, GOLD, STATS, DEFENSE, ARMOR, WEAPON, PRICE, MODE,
	IDENTIFIED, SOCKETS, WIDTH, HEIGHT, RANDOM, OWNED,
	HASWEIGHT
};

static const wchar_t* CONDITIONS[] = { L"", L"Code", L"Type", L"PlayerClass", L"Class", L"Rarity", L"Ethereal", L"Runeword", L"Prefix", L"Suffix", L"ItemLevel", L"Quality", L"AreaLevel", L"CharacterLevel",
	L"Difficulty", L"Rune", L"Id", L"Gold", L"Stats", L"Defense", L"Armor", L"Weapon", L"Price", L"Mode", L"Identified", L"Sockets", L"Width", L"Height", L"Random", L"Owned", L"HasWeight" };

class Condition {
protected:
	ConditionType m_Type;
	std::wstring m_Value;
public:
	Condition(std::wstring_view value = {}, ConditionType type = ConditionType::NONE) : m_Value(value), m_Type(type) {};
	virtual ~Condition() = default;
	ConditionType GetType() const { return m_Type; }
	virtual bool Evaluate(Unit* pItem) = 0;
	virtual void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) = 0;
	virtual std::wstring ToString(Unit* pItem) const = 0;
};


class CodeCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	CodeCondition(std::wstring_view value = {}) : Condition(value, ConditionType::CODE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<CodeCondition>(value); }
};

class PlayerClassCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	PlayerClassCondition(std::wstring_view value = {}) : Condition(value, ConditionType::PLAYERCLASS) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };

	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<PlayerClassCondition>(value); }
};

class TypeCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	TypeCondition(std::wstring_view value = {}) : Condition(value, ConditionType::TYPE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<TypeCondition>(value); }
};

class ClassCondition : public Condition {
protected:
	std::unique_ptr<Expression> m_Expression;
public:
	ClassCondition(std::wstring_view value = {}) : Condition(value, ConditionType::CLASS) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<ClassCondition>(value); }
};

class RarityCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	RarityCondition(std::wstring_view value = {}) : Condition(value, ConditionType::RARITY) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<RarityCondition>(value); }
};

class EtherealCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	EtherealCondition(std::wstring_view value = {}) : Condition(value, ConditionType::ETHEREAL) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<EtherealCondition>(value); }
};

class RunewordCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	RunewordCondition(std::wstring_view value = {}) : Condition(value, ConditionType::RUNEWORD) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<RunewordCondition>(value); }
};

class PrefixCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	PrefixCondition(std::wstring_view value = {}) : Condition(value, ConditionType::PREFIX) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<PrefixCondition>(value); }
};

class SuffixCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	SuffixCondition(std::wstring_view value = {}) : Condition(value, ConditionType::SUFFIX) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<SuffixCondition>(value); }
};

class ItemLevelCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	ItemLevelCondition(std::wstring_view value = {}) : Condition(value, ConditionType::ITEM_LEVEL) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<ItemLevelCondition>(value); }
};

class QualityCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	QualityCondition(std::wstring_view value = {}) : Condition(value, ConditionType::QUALITY) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<QualityCondition>(value); }
};

class AreaLevelCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	AreaLevelCondition(std::wstring_view value = {}) : Condition(value, ConditionType::AREA_LEVEL) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<AreaLevelCondition>(value); }
};

class CharacterLevelCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	CharacterLevelCondition(std::wstring_view value = {}) : Condition(value, ConditionType::CHARACTER_LEVEL) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<CharacterLevelCondition>(value); }
};

class DifficultyCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	DifficultyCondition(std::wstring_view value = {}) : Condition(value, ConditionType::DIFFICULTY) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<DifficultyCondition>(value); }
};

class RuneCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	RuneCondition(std::wstring_view value = {}) : Condition(value, ConditionType::RUNE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<RuneCondition>(value); }
};

class IdCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	IdCondition(std::wstring_view value = {}) : Condition(value, ConditionType::ID) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<IdCondition>(value); }
};

class GoldCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	GoldCondition(std::wstring_view value = {}) : Condition(value, ConditionType::GOLD) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<GoldCondition>(value); }
};

class StatsCondition : public Condition {
protected:
	std::unique_ptr<Expression> m_Expression;
public:
	StatsCondition(std::wstring_view value = {}) : Condition(value, ConditionType::STATS) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<StatsCondition>(value); }
};

class DefenseCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	DefenseCondition(std::wstring_view value = {}) : Condition(value, ConditionType::DEFENSE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<DefenseCondition>(value); }
};

class ArmorCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	ArmorCondition(std::wstring_view value = {}) : Condition(value, ConditionType::ARMOR) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<ArmorCondition>(value); }
};

class WeaponCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	WeaponCondition(std::wstring_view value = {}) : Condition(value, ConditionType::WEAPON) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<WeaponCondition>(value); }
};

class PriceCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	PriceCondition(std::wstring_view value = {}) : Condition(value, ConditionType::PRICE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<PriceCondition>(value); }
};

class ModeCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	ModeCondition(std::wstring_view value = {}) : Condition(value, ConditionType::MODE) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<ModeCondition>(value); }
};

class IdentifiedCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	IdentifiedCondition(std::wstring_view value = {}) : Condition(value, ConditionType::IDENTIFIED) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<IdentifiedCondition>(value); }
};

class SocketsCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	SocketsCondition(std::wstring_view value = {}) : Condition(value, ConditionType::SOCKETS) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<SocketsCondition>(value); }
};

class WidthCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	WidthCondition(std::wstring_view value = {}) : Condition(value, ConditionType::WIDTH) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<WidthCondition>(value); }
};

class HeightCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	HeightCondition(std::wstring_view value = {}) : Condition(value, ConditionType::HEIGHT) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };
	
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<HeightCondition>(value); }
};

class RandomCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	RandomCondition(std::wstring_view value = {}) : Condition(value, ConditionType::RANDOM) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };

	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<RandomCondition>(value); }
};

class OwnedCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	OwnedCondition(std::wstring_view value = {}) : Condition(value, ConditionType::OWNED) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };

	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<OwnedCondition>(value); }
};

class HasWeightCondition : public Condition {
protected:
	Variable m_Left;
	std::unique_ptr<Expression> m_Expression;
public:
	HasWeightCondition(std::wstring_view value = {}) : Condition(value, ConditionType::HASWEIGHT) {};
	bool Evaluate(Unit* pItem) override;
	void Initialize(const utility::string_umap<std::wstring, int32_t>& variables) override;
	std::wstring ToString(Unit* pItem) const override { return std::format(L"{} {}", CONDITIONS[static_cast<uint8_t>(m_Type)], m_Expression->ToString(pItem)); };

	static std::unique_ptr<Condition> MakeInstance(std::wstring_view value = {}) { return std::make_unique<HasWeightCondition>(value); }
};

class ConditionFactory {
public:
	static std::unique_ptr<Condition> MakeInstance(std::wstring_view condition, std::wstring_view value = {}) {
		static const utility::string_umap_icase<std::wstring, std::unique_ptr<Condition>(&)(std::wstring_view)> lookup = {
			{ L"Code",				CodeCondition::MakeInstance },
			{ L"Type",				TypeCondition::MakeInstance },
			{ L"PlayerClass",		PlayerClassCondition::MakeInstance },
			{ L"Class",				ClassCondition::MakeInstance },
			{ L"Rarity",			RarityCondition::MakeInstance },
			{ L"Ethereal",			EtherealCondition::MakeInstance },
			{ L"Runeword",			RunewordCondition::MakeInstance },
			{ L"Prefix",			PrefixCondition::MakeInstance },
			{ L"Suffix",			SuffixCondition::MakeInstance },
			{ L"ItemLevel",			ItemLevelCondition::MakeInstance },
			{ L"Quality",			QualityCondition::MakeInstance },
			{ L"AreaLevel",			AreaLevelCondition::MakeInstance },
			{ L"CharacterLevel",	CharacterLevelCondition::MakeInstance },
			{ L"Difficulty",		DifficultyCondition::MakeInstance },
			{ L"Rune",				RuneCondition::MakeInstance },
			{ L"Identified",		IdentifiedCondition::MakeInstance },
			{ L"Id",				IdCondition::MakeInstance },
			{ L"Gold",				GoldCondition::MakeInstance },
			{ L"Stats",				StatsCondition::MakeInstance },
			{ L"Defense",			DefenseCondition::MakeInstance },
			{ L"Armor",				ArmorCondition::MakeInstance },
			{ L"Weapon",			WeaponCondition::MakeInstance },
			{ L"Price",				PriceCondition::MakeInstance },
			{ L"Mode",				ModeCondition::MakeInstance },
			{ L"Sockets",			SocketsCondition::MakeInstance },
			{ L"Width",				WidthCondition::MakeInstance },
			{ L"Height",			HeightCondition::MakeInstance },
			{ L"Random",			RandomCondition::MakeInstance },
			{ L"Owned",				OwnedCondition::MakeInstance },
			{ L"HasWeight",			HasWeightCondition::MakeInstance },
		};

		if (auto search = lookup.find(condition); search != lookup.end()) {
			return search->second(value);
		}
		return nullptr;
	}
};