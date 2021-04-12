#include <Windows.h>
#include <fmt/core.h>
#include "Condition.h"
#include "D2Structs.h"
#include "D2Ptrs.h"

std::map<std::wstring, uint32_t> STRICTNESS_LEVEL_TO_ENUM = {
	{ L"None", 0 },
	{ L"Soft", 1 },
	{ L"Regular", 2 },
	{ L"Semi-Strict", 3 },
	{ L"Strict", 4 },
	{ L"Very Strict", 5 },
	{ L"Uber Strict", 6 },
	{ L"Uber+ Strict", 7 }
};

bool AndCondition::Evaluate(Unit* pItem) {
	return std::all_of(m_Conditions.begin(), m_Conditions.end(), [pItem](Condition* c) { return c->Evaluate(pItem); });
}

bool RarityCondition::Evaluate(Unit* pItem) {
	m_Lhs->SetValue(static_cast<std::underlying_type_t<ItemRarity>>(pItem->pItemData->dwRarity));
	return m_Expression->Evaluate(pItem);
}

bool CodeCondition::Evaluate(Unit* pItem) {
	ItemsTxt pItemTxt = D2COMMON_ItemDataTbl->pItemsTxt[pItem->dwLineId];
	m_Lhs->SetValue(pItemTxt.dwCode);
	return m_Expression->Evaluate(pItem);
}

bool TypeCondition::Evaluate(Unit* pItem) {
	ItemsTxt pItemTxt = D2COMMON_ItemDataTbl->pItemsTxt[pItem->dwLineId];
	m_Lhs->SetValue(pItemTxt.dwCode);
	return m_Expression->Evaluate(pItem);
}

bool ClassCondition::Evaluate(Unit* pItem) {
	return m_Expression->Evaluate(pItem);
}

bool EtherealCondition::Evaluate(Unit* pItem) {
	uint8_t isEthereal = (pItem->pItemData->dwItemFlags & ItemFlags::ETHEREAL) == ItemFlags::ETHEREAL;
	m_Lhs->SetValue(isEthereal);
	return m_Expression->Evaluate(pItem);
}

bool RunewordCondition::Evaluate(Unit* pItem) {
	uint8_t isRuneword = (pItem->pItemData->dwItemFlags & ItemFlags::RUNEWORD) == ItemFlags::RUNEWORD;
	m_Lhs->SetValue(isRuneword);
	return m_Expression->Evaluate(pItem);
}

bool PrefixCondition::Evaluate(Unit* pItem) {
	uint8_t isIdentified = (pItem->pItemData->dwItemFlags & ItemFlags::IDENTIFIED) == ItemFlags::IDENTIFIED;
	if (!isIdentified) {
		return false;
	}
	for (auto& prefix : pItem->pItemData->wMagicPrefix) {
		m_Lhs->SetValue(prefix);
		if (m_Expression->Evaluate(pItem)) {
			return true;
		}
	}
	return false;
}

bool SuffixCondition::Evaluate(Unit* pItem) {
	uint8_t isIdentified = (pItem->pItemData->dwItemFlags & ItemFlags::IDENTIFIED) == ItemFlags::IDENTIFIED;
	if (!isIdentified) {
		return false;
	}
	for (auto& prefix : pItem->pItemData->wMagicSuffix) {
		m_Lhs->SetValue(prefix);
		if (m_Expression->Evaluate(pItem)) {
			return true;
		}
	}
	return false;
}

bool ItemLevelCondition::Evaluate(Unit* pItem) {
	m_Lhs->SetValue(pItem->pItemData->dwItemLevel);
	return m_Expression->Evaluate(pItem);
}

bool QualityCondition::Evaluate(Unit* pItem) {
	ItemsTxt pItemTxt = D2COMMON_ItemDataTbl->pItemsTxt[pItem->dwLineId];
	if (pItemTxt.dwCode == pItemTxt.dwUltraCode) {
		m_Lhs->SetValue(static_cast<std::underlying_type_t<ItemQualityLevel>>(ItemQualityLevel::ELITE));
	} else if (pItemTxt.dwCode == pItemTxt.dwUberCode) {
		m_Lhs->SetValue(static_cast<std::underlying_type_t<ItemQualityLevel>>(ItemQualityLevel::EXCEPTIONAL));
	} else if (pItemTxt.dwCode == pItemTxt.dwNormCode) {
		m_Lhs->SetValue(static_cast<std::underlying_type_t<ItemQualityLevel>>(ItemQualityLevel::NORMAL));
	} else {
		m_Lhs->SetValue(static_cast<std::underlying_type_t<ItemQualityLevel>>(ItemQualityLevel::UNKNOWN));
	}
	return m_Expression->Evaluate(pItem);
}

bool CharacterLevelCondition::Evaluate(Unit* pItem) {
	Unit* pPlayer = D2CLIENT_GetPlayerUnit();
	if (!pPlayer) {
		return false;
	}
	m_Lhs->SetValue(D2COMMON_STATLIST_GetUnitStatUnsigned(pPlayer, Stat::LEVEL, 0));
	return m_Expression->Evaluate(pItem);
}

bool DifficultyCondition::Evaluate(Unit* pItem) {
	m_Lhs->SetValue(D2CLIENT_GetDifficulty());
	return m_Expression->Evaluate(pItem);
}

bool IdCondition::Evaluate(Unit* pItem) {
	m_Lhs->SetValue(pItem->pItemData->dwFileIndex);
	return m_Expression->Evaluate(pItem);
}

bool GoldCondition::Evaluate(Unit* pItem) {
	return D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::GOLD)
			&& UnsignedStatCondition::Evaluate(pItem);
}

bool StatsCondition::Evaluate(Unit* pItem) {
	return m_Expression->Evaluate(pItem);
}


bool RuneCondition::Evaluate(Unit* pItem) {
	if (!D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::RUNE)) {
		return false;
	}
	ItemsTxt pItemTxt = D2COMMON_ItemDataTbl->pItemsTxt[pItem->dwLineId];
	int nRuneNumber = std::stoi(std::string(&pItemTxt.szCode[1], 3));
	m_Lhs->SetValue(nRuneNumber);
	return m_Expression->Evaluate(pItem);
}

bool ArmorCondition::Evaluate(Unit* pItem) {
	m_Lhs->SetValue(D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::ANY_ARMOR));
	return m_Expression->Evaluate(pItem);
}

bool WeaponCondition::Evaluate(Unit* pItem) {
	m_Lhs->SetValue(D2COMMON_ITEMS_CheckItemTypeId(pItem, ItemType::WEAPON));
	return m_Expression->Evaluate(pItem);
}

bool ModeCondition::Evaluate(Unit* pItem) {
	return false;
}

bool PriceCondition::Evaluate(Unit* pItem) {
	Unit* pPlayer = D2CLIENT_GetPlayerUnit();
	if (pItem != NULL && pPlayer != NULL) {
		int nPrice = D2COMMON_ITEMS_GetTransactionCost(pPlayer, pItem, D2CLIENT_GetDifficulty(), D2CLIENT_GetQuestFlags(), 0x201, 1);
		m_Lhs->SetValue(nPrice);
		return m_Expression->Evaluate(pItem);
	}
	return false;
}

bool IdentifiedCondition::Evaluate(Unit* pItem) {
	uint8_t isIdentified = (pItem->pItemData->dwItemFlags & ItemFlags::IDENTIFIED) == ItemFlags::IDENTIFIED;
	m_Lhs->SetValue(isIdentified);
	return m_Expression->Evaluate(pItem);
}

bool WidthCondition::Evaluate(Unit* pItem) {
	ItemsTxt pItemTxt = D2COMMON_ItemDataTbl->pItemsTxt[pItem->dwLineId];
	m_Lhs->SetValue(pItemTxt.nInvWidth);
	return m_Expression->Evaluate(pItem);
}

bool HeightCondition::Evaluate(Unit* pItem) {
	ItemsTxt pItemTxt = D2COMMON_ItemDataTbl->pItemsTxt[pItem->dwLineId];
	m_Lhs->SetValue(pItemTxt.nInvHeight);
	return m_Expression->Evaluate(pItem);
}