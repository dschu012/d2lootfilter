#include "Configuration.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <format>
#include <algorithm>
#include <filesystem>
#include "Utils.h"
#include "mINI.h"
#include "ItemFilter.h"

namespace fs = std::filesystem;

utility::string_umap<std::wstring, std::vector<std::unique_ptr<Action>>> GlobalStyles;
std::map<uint32_t, Rule> GlobalRules;

#define COMMENT_STR L"#"
#define STYLE_STR L"Style"
#define SHOW_STR L"Show"
#define HIDE_STR L"Hide"

#define SETTINGS_FILE "d2lootfilter.ini"

//todo wstring
void Configuration::ReadSettings() {
	mINI::INIFile file(SETTINGS_FILE);
	mINI::INIStructure data;

	file.read(data);

	if (std::string& value = data["Setting"]["Path"]; !value.empty())
		m_Settings.wPath = value;
	if (std::string& value = data["Setting"]["FilterLevel"]; !value.empty())
		m_Settings.nFilterLevel = std::stoi(value);
	if (std::string& value = data["Setting"]["PingLevel"]; !value.empty())
		m_Settings.nPingLevel = std::stoi(value);
}

void Configuration::SaveSettings() {
	mINI::INIFile file(SETTINGS_FILE);
	mINI::INIStructure data;
	
	data["Setting"]["Path"] = m_Settings.wPath;
	data["Setting"]["FilterLevel"] = std::to_string(m_Settings.nFilterLevel);
	data["Setting"]["PingLevel"] = std::to_string(m_Settings.nPingLevel);

	file.write(data);
}

void Configuration::LoadSettings() {
	if (!ItemFilter::IsTxtDataLoaded()) {
		return;
	}
	
	ReadSettings();
	if (!fs::exists(SETTINGS_FILE)) {
		SaveSettings();
	}

	auto t1 = std::chrono::high_resolution_clock::now();

	GlobalRules.clear();
	GlobalStyles.clear();

	uint32_t tokenLineNumber = 0;
	uint32_t currentLineNumber = 0;

	// Create an empty item filter file if it doesn't exist
	std::wfstream in(m_Settings.wPath, std::ios::in | std::ios::app);
	std::wstring line;
	std::vector<std::wstring> lines;

	while (std::getline(in, line)) {
		currentLineNumber++;
		if (line.find(COMMENT_STR) != std::wstring::npos) {
			line = line.erase(line.find(COMMENT_STR));
		}
		line = trim(line);
		if (line.length() == 0) {
			continue;
		}
		if (line.compare(0, 5, STYLE_STR) == 0
			|| line.compare(0, 4, SHOW_STR) == 0
			|| line.compare(0, 4, HIDE_STR) == 0) {
			HandleToken(tokenLineNumber, lines);
			tokenLineNumber = currentLineNumber;
		}
		lines.push_back(line);
	}
	HandleToken(tokenLineNumber, lines);

	InitalizeConditionVariables();
	m_Loaded = true;

	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	DEBUG_LOG(std::format(L"Rules parsed in {}ms\n", ms_int.count()));
}

void Configuration::HandleToken(uint32_t lineNumber, std::vector<std::wstring>& lines) {
	if (lines.empty())
		return;
	std::wstring line = lines[0]; lines.erase(lines.begin());
	if (line.compare(0, 5, STYLE_STR) == 0) {
		std::wstring style = line.substr(5);
		style = trim(style);
		GlobalStyles[style] = ParseActions(lines);
	}
	else {
		if (auto [rule, result] = GlobalRules.try_emplace(lineNumber, ParseRule(lineNumber, lines)); result) {
			rule->second.AddAction(line.compare(0, 4, SHOW_STR) == 0 ? ShowAction::MakeInstance() : HideAction::MakeInstance());
		}
	}
	for (const auto& line : lines) {
		ERROR_LOG(std::format(L"Rule {}: Error parsing {}.", lineNumber, line));
	}
	lines.clear();
}

std::vector<std::unique_ptr<Action>> Configuration::ParseStyle(std::vector<std::wstring>& lines) {
	return ParseActions(lines);
}

Rule Configuration::ParseRule(uint32_t lineNumber, std::vector<std::wstring>& lines) {
	Rule rule{ lineNumber };
	rule.AddConditions(ParseConditions(lines));
	rule.AddActions(ParseActions(lines));
	return rule;
}

std::vector<std::unique_ptr<Condition>> Configuration::ParseConditions(std::vector<std::wstring>& lines) {
	std::vector<std::unique_ptr<Condition>> items;
	lines.erase(std::remove_if(lines.begin(), lines.end(), [&items](std::wstring_view line) {
		size_t pos = (std::min)(line.find_first_of(L' '), line.length());
		if (auto condition = ConditionFactory::MakeInstance(line.substr(0, pos), trim(line.substr(pos)))) {
			items.push_back(std::move(condition));
			return true;
		}
		return false;
		}), lines.end());
	return items;
}

std::vector<std::unique_ptr<Action>> Configuration::ParseActions(std::vector<std::wstring>& lines) {
	std::vector<std::unique_ptr<Action>> items;
	lines.erase(std::remove_if(lines.begin(), lines.end(), [&items](std::wstring_view line) {
		size_t pos = (std::min)(line.find_first_of(L' '), line.length());
		if (auto action = ActionFactory::MakeInstance(line.substr(0, pos), trim(line.substr(pos)))) {
			items.push_back(std::move(action));
			return true;
		}
		return false;
		}), lines.end());
	return items;
}

//init any constants used in rules i.e. item names/skill names
void Configuration::InitalizeConditionVariables() {
	//replace any variables used in the filter. i.e. item names/codes/rune numbers/etc...
	InitializeTypesCodesAndRunes();
	InitializeClass();
	InitializeRaritiesAndQualities();
	InitializeOther();
	InitalizeActionVariables();

	/*
	for (auto &rule : GlobalRules) {
		for (auto& condition : rule->GetConditions()) {
			Logger::Info("%s\n", condition->ToString().c_str());
		}
	}
	*/
}

void Configuration::InitalizeActionVariables() {
	utility::string_umap<std::wstring, int32_t> variables;
	for (auto& rule : GlobalRules) {
		for (auto& action : rule.second.GetActions()) {
			switch (action->GetType()) {
			default:
				action->Initialize(variables);
				break;
			}
		}
	}
	for (auto& style : GlobalStyles) {
		for (auto& action : style.second) {
			switch (action->GetType()) {
			default:
				action->Initialize(variables);
				break;
			}
		}
	}
}

void Configuration::InitializeTypesCodesAndRunes() {
	static utility::string_umap<std::wstring, int32_t> names;
	static utility::string_umap<std::wstring, int32_t> codes;
	static utility::string_umap<std::wstring, int32_t> runes;

	DataTables* sgptDataTables = *D2COMMON_gpDataTables;

	if (names.empty()) {
		for (int i = 0; i < D2COMMON_ItemDataTbl->nItemsTxtRecordCount; i++) {
			ItemsTxt* pItemTxt = &D2COMMON_ItemDataTbl->pItemsTxt[i];
			std::wstring wNameStr = D2LANG_GetStringFromTblIndex(pItemTxt->wNameStr);
			auto [it, result] = names.try_emplace(wNameStr, pItemTxt->dwCode);
			// Mapping duplicated names to an empty code
			if (!result && it->second != pItemTxt->dwCode) {
				it->second = 0x20202020;
			}
			std::wstring wCode = std::wstring(4, L' ');
			mbstowcs(&wCode[0], pItemTxt->szCode, 4);
			wCode = trim(wCode);
			codes[wCode] = pItemTxt->dwCode;
			if (pItemTxt->wType[0] == ItemType::RUNE) {
				int nRuneNumber = std::stoi(std::string(&pItemTxt->szCode[1], 3));
				runes[wNameStr] = nRuneNumber;
				size_t nFound = wNameStr.find(L" ");
				if (nFound != std::wstring::npos) {
					runes[wNameStr.substr(0, nFound)] = nRuneNumber;
				}
			}
		}
	}

	static utility::string_umap<std::wstring, int32_t> classNames;
	if (classNames.empty()) {
		for (int i = 0; i < sgptDataTables->nCharStatsTxtRecordCount; i++) {
			CharStatsTxt* pCharStats = &sgptDataTables->pCharStatsTxt[i];
			std::wstring className = std::wstring(pCharStats->wszClassName);
			className = trim(className);
			classNames[className] = i;
		}
	}

	for (auto& rule : GlobalRules) {
		for (auto& condition : rule.second.GetConditions()) {
			switch (condition->GetType()) {
			case ConditionType::TYPE:
				condition->Initialize(names);
				break;
			case ConditionType::CODE:
				condition->Initialize(codes);
				break;
			case ConditionType::RUNE:
				condition->Initialize(runes);
				break;
			case ConditionType::PLAYERCLASS:
				condition->Initialize(classNames);
				break;
			default:
				break;
			}
		}
	}
}


void Configuration::InitializeClass() {
	for (auto& rule : GlobalRules) {
		for (auto& condition : rule.second.GetConditions()) {
			switch (condition->GetType()) {
			case ConditionType::CLASS:
				condition->Initialize(ItemTypes);
				break;
			default:
				break;
			}
		}
	}
}

void Configuration::InitializeRaritiesAndQualities() {
	for (auto& rule : GlobalRules) {
		for (auto& condition : rule.second.GetConditions()) {
			switch (condition->GetType()) {
			case ConditionType::RARITY:
				condition->Initialize(Rarities);
				break;
			case ConditionType::QUALITY:
				condition->Initialize(Qualities);
				break;
			default:
				break;
			}
		}
	}
}


void Configuration::InitializeOther() {
	const utility::string_umap<std::wstring, int32_t> variables;

	for (auto& rule : GlobalRules) {
		for (auto& condition : rule.second.GetConditions()) {
			switch (condition->GetType()) {
			case ConditionType::STATS:
			case ConditionType::ETHEREAL:
			case ConditionType::RUNEWORD:
			case ConditionType::PREFIX:
			case ConditionType::SUFFIX:
			case ConditionType::ITEM_LEVEL:
			case ConditionType::AREA_LEVEL:
			case ConditionType::CHARACTER_LEVEL:
			case ConditionType::DIFFICULTY:
			case ConditionType::ID:
			case ConditionType::GOLD:
			case ConditionType::DEFENSE:
			case ConditionType::ARMOR:
			case ConditionType::WEAPON:
			case ConditionType::PRICE:
			case ConditionType::MODE:
			case ConditionType::IDENTIFIED:
			case ConditionType::SOCKETS:
			case ConditionType::WIDTH:
			case ConditionType::HEIGHT:
			case ConditionType::RANDOM:
			case ConditionType::OWNED:
			case ConditionType::HASWEIGHT:
				condition->Initialize(variables);
				break;
			default:
				break;
			}
		}
	}
}

#undef COMMENT_STR
#undef STYLE_STR
#undef SHOW_STR
#undef HIDE_STR