#include "Condition.h"
#include "Utils.h"
#include <algorithm>

bool CodeCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(GetItemsTxt(pItem).dwCode);
	return m_Expression->Evaluate(pItem);
}

void CodeCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool TypeCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(GetItemsTxt(pItem).dwCode);
	return m_Expression->Evaluate(pItem);
}

void TypeCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool ClassCondition::Evaluate(Unit* pItem) {
	return m_Expression->Evaluate(pItem);
}

void ClassCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::ParseCall(Token::CLASS, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool RarityCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(static_cast<int32_t>(pItem->pItemData->dwRarity));
	return m_Expression->Evaluate(pItem);
}

void RarityCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool EtherealCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue((pItem->pItemData->dwItemFlags & ItemFlags::ETHEREAL) == ItemFlags::ETHEREAL);
	return m_Expression->Evaluate(pItem);
}

void EtherealCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool RunewordCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue((pItem->pItemData->dwItemFlags & ItemFlags::RUNEWORD) == ItemFlags::RUNEWORD);
	return m_Expression->Evaluate(pItem);
}

void RunewordCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool PrefixCondition::Evaluate(Unit* pItem) {
	uint8_t isIdentified = (pItem->pItemData->dwItemFlags & ItemFlags::IDENTIFIED) == ItemFlags::IDENTIFIED;
	if (!isIdentified) {
		return false;
	}
	for (auto& prefix : pItem->pItemData->wMagicPrefix) {
		m_Left->SetValue(prefix);
		if (m_Expression->Evaluate(pItem)) {
			return true;
		}
	}
	return false;
}

void PrefixCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool SuffixCondition::Evaluate(Unit* pItem) {
	uint8_t isIdentified = (pItem->pItemData->dwItemFlags & ItemFlags::IDENTIFIED) == ItemFlags::IDENTIFIED;
	if (!isIdentified) {
		return false;
	}
	for (auto& prefix : pItem->pItemData->wMagicSuffix) {
		m_Left->SetValue(prefix);
		if (m_Expression->Evaluate(pItem)) {
			return true;
		}
	}
	return false;
}

void SuffixCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool ItemLevelCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(pItem->pItemData->dwItemLevel);
	return m_Expression->Evaluate(pItem);
}

void ItemLevelCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool QualityCondition::Evaluate(Unit* pItem) {
	ItemsTxt txt = GetItemsTxt(pItem);
	if (txt.dwCode == txt.dwUltraCode) {
		m_Left->SetValue(2);
	}
	else if (txt.dwCode == txt.dwUberCode) {
		m_Left->SetValue(1);
	}
	else if (txt.dwCode == txt.dwNormCode) {
		m_Left->SetValue(0);
	}
	else {
		m_Left->SetValue(-1);
	}
	return m_Expression->Evaluate(pItem);
}

void QualityCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool AreaLevelCondition::Evaluate(Unit* pItem) {
	return false;
}

void AreaLevelCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool CharacterLevelCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(D2COMMON_STATLIST_GetUnitStatUnsigned(D2CLIENT_GetPlayerUnit(), Stat::LEVEL, 0));
	return m_Expression->Evaluate(pItem);
}

void CharacterLevelCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool DifficultyCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(D2CLIENT_GetDifficulty());
	return m_Expression->Evaluate(pItem);
}

void DifficultyCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool RuneCondition::Evaluate(Unit* pItem) {
	if (!D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::RUNE)) {
		return false;
	}
	int nRuneNumber = std::stoi(std::string(&GetItemsTxt(pItem).szCode[1], 3));
	m_Left->SetValue(nRuneNumber);
	return m_Expression->Evaluate(pItem);
}

void RuneCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool IdCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(pItem->dwLineId);
	return m_Expression->Evaluate(pItem);
}

void IdCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool GoldCondition::Evaluate(Unit* pItem) {
	if (!D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::GOLD)) {
		return false;
	}
	m_Left->SetValue(D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, Stat::GOLD, 0));
	return m_Expression->Evaluate(pItem);
}

void GoldCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool StatsCondition::Evaluate(Unit* pItem) {
	return m_Expression->Evaluate(pItem);
}

void StatsCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	for (auto stat : CustomStats) {
		replace(m_Value, stat.first, stat.second);
	}
	m_Expression = Parser::Parse(m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool DefenseCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, Stat::ARMORCLASS, 0));
	return m_Expression->Evaluate(pItem);
}

void DefenseCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool ArmorCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::ANY_ARMOR));
	return m_Expression->Evaluate(pItem);
}

void ArmorCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool WeaponCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::WEAPON));
	return m_Expression->Evaluate(pItem);
}

void WeaponCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool PriceCondition::Evaluate(Unit* pItem) {
	Unit* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pItem != NULL && pPlayer != NULL) {
		int nPrice = D2COMMON_ITEMS_GetTransactionCost(pPlayer, pItem, D2CLIENT_GetDifficulty(), D2CLIENT_GetQuestFlags(), 0x201, 1);
		m_Left->SetValue(nPrice);
		return m_Expression->Evaluate(pItem);
	}
	return false;
}

void PriceCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool ModeCondition::Evaluate(Unit* pItem) {
	return false;
}

void ModeCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool IdentifiedCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue((pItem->pItemData->dwItemFlags & ItemFlags::IDENTIFIED) == ItemFlags::IDENTIFIED);
	return m_Expression->Evaluate(pItem);
}

void IdentifiedCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool SocketsCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, Stat::ITEM_NUMSOCKETS, 0));
	return m_Expression->Evaluate(pItem);
}

void SocketsCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool WidthCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(GetItemsTxt(pItem).nInvWidth);
	return m_Expression->Evaluate(pItem);
}

void WidthCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool HeightCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(GetItemsTxt(pItem).nInvHeight);
	return m_Expression->Evaluate(pItem);
}

void HeightCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};

bool RandomCondition::Evaluate(Unit* pItem) {
	m_Left->SetValue(rand() % 100);	//random between 0-99
	return m_Expression->Evaluate(pItem);
}

void RandomCondition::Initialize(std::unordered_map<std::wstring, int32_t> variables) {
	m_Left = new Variable();
	m_Expression = Parser::Parse(m_Left, m_Value.c_str());
	m_Expression->SetVariables(variables);
};
