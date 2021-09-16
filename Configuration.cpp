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

namespace fs = std::filesystem;

std::unordered_map<std::wstring, std::vector<Action*>> GlobalStyles;
std::map<uint32_t, Rule*> GlobalRules;

#define COMMENT_STR L"#"
#define STYLE_STR L"Style"
#define SHOW_STR L"Show"
#define HIDE_STR L"Hide"

#define SETTINGS_FILE "d2lootfilter.ini"

Configuration::Configuration() {
}

std::string GetOrDefault(std::string& configLine, std::string defaultValue) {
	if (configLine.empty()) {
		configLine = defaultValue;
	}
	return configLine;
}

//todo wstring
void Configuration::ReadSettings() {
	Settings* s = new Settings();

	mINI::INIFile file(SETTINGS_FILE);
	mINI::INIStructure data;
	bool exists = fs::exists(SETTINGS_FILE);

	if (exists) {
		file.read(data);
	}

	s->wPath = GetOrDefault(data["Setting"]["Path"], "./item.filter");
	s->nFilterLevel = std::stoi(GetOrDefault(data["Setting"]["FilterLevel"], "6"));
	s->nPingLevel = std::stoi(GetOrDefault(data["Setting"]["PingLevel"], "6"));

	if (!exists) {
		file.generate(data);
	}

	m_Settings = s;
}

void Configuration::SaveSettings() {
	if (!m_Settings) {
		ReadSettings();
	}

	mINI::INIFile file(SETTINGS_FILE);
	mINI::INIStructure data;
	
	data["Setting"]["Path"] = m_Settings->wPath;
	data["Setting"]["FilterLevel"] = std::to_string(FilterLevel);
	data["Setting"]["PingLevel"] = std::to_string(PingLevel);

	file.generate(data);
}

void Configuration::Load() {
	if (!IsTxtDataLoaded) {
		return;
	}
	
	ReadSettings();

	auto t1 = std::chrono::high_resolution_clock::now();

	PingLevel = m_Settings->nPingLevel;
	FilterLevel = m_Settings->nFilterLevel;
	GlobalRules.clear();
	GlobalStyles.clear();

	if (!fs::exists(m_Settings->wPath)) {
		std::ofstream out(m_Settings->wPath);
		out.close();
	}

	uint32_t tokenLineNumber = 0;
	uint32_t currentLineNumber = 0;

	std::wifstream in(m_Settings->wPath);
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
	DEBUG_LOG(std::format(L"Rules parsed in {}ms", ms_int.count()));
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
		Rule* rule = ParseRule(lines);
		if (line.compare(0, 4, SHOW_STR) == 0) rule->AddAction(new ShowAction(), 0);
		else rule->AddAction(new HideAction(), 0);
		rule->SetLineNumber(lineNumber);
		GlobalRules[lineNumber] = rule;
	}
	for (auto line : lines) {
		ERROR_LOG(std::format(L"Rule {}: Error parsing {}.", lineNumber, line));
	}
	lines.clear();
}

std::vector<Action*> Configuration::ParseStyle(std::vector<std::wstring>& lines) {
	return ParseActions(lines);
}

Rule* Configuration::ParseRule(std::vector<std::wstring>& lines) {
	Rule* rule = new Rule();
	rule->AddConditions(ParseConditions(lines));
	rule->AddActions(ParseActions(lines));
	return rule;
}

#define CONDITION(NAME) if(line.compare(0, wcslen(L#NAME), L#NAME) == 0) { uint32_t len = wcslen(L#NAME); items.push_back(new NAME##Condition(line.length() > len ? trim_copy(line.substr(len + 1)) : L"")); match = true; }
#define ACTION(NAME) if(line.compare(0, wcslen(L#NAME), L#NAME) == 0) { uint32_t len = wcslen(L#NAME); items.push_back(new NAME##Action(line.length() > len ? trim_copy(line.substr(len + 1)) : L"")); match = true; }

std::vector<Condition*> Configuration::ParseConditions(std::vector<std::wstring>& lines) {
	std::vector<Condition*> items;
	lines.erase(std::remove_if(lines.begin(), lines.end(), [&items](std::wstring const& line) {
		bool match = false;
		CONDITION(Code)
		else CONDITION(Type)
		else CONDITION(PlayerClass)
		else CONDITION(Class)
		else CONDITION(Rarity)
		else CONDITION(Ethereal)
		else CONDITION(Runeword)
		else CONDITION(Prefix)
		else CONDITION(Suffix)
		else CONDITION(ItemLevel)
		else CONDITION(Quality)
		else CONDITION(AreaLevel)
		else CONDITION(CharacterLevel)
		else CONDITION(Difficulty)
		else CONDITION(Rune)
		else CONDITION(Identified)
		else CONDITION(Id)
		else CONDITION(Gold)
		else CONDITION(Stats)
		else CONDITION(Defense)
		else CONDITION(Armor)
		else CONDITION(Weapon)
		else CONDITION(Price)
		else CONDITION(Mode)
		else CONDITION(Sockets)
		else CONDITION(Width)
		else CONDITION(Height)
		else CONDITION(Random)
		else CONDITION(Owned)
		return match;
		}), lines.end());
	return items;
}

std::vector<Action*> Configuration::ParseActions(std::vector<std::wstring>& lines) {
	std::vector<Action*> items;
	lines.erase(std::remove_if(lines.begin(), lines.end(), [&items](std::wstring const& line) {
		bool match = false;
		ACTION(Continue)
		else ACTION(SetStyle)
		else ACTION(SetName)
		else ACTION(SetDescription)
		else ACTION(SetBackgroundColor)
		else ACTION(SetInventoryColor)
		else ACTION(SetBorderColor)
		else ACTION(ChatNotify)
		else ACTION(PlayAlert)
		else ACTION(MinimapIcon)
		return match;
		}), lines.end());
	return items;
}

#undef CONDITION
#undef ACTION

//init any constants used in rules i.e. item names/skill names
void Configuration::InitalizeConditionVariables() {
	//replace any variables used in the filter. i.e. item names/codes/rune numbers/etc...
	InitializeTypesCodesAndRunes();
	InitializeClass();
	InitializeRaritiesAndQualities();
	InitializeOther();

	/*
	for (auto &rule : GlobalRules) {
		for (auto& condition : rule->GetConditions()) {
			Logger::Info("%s\n", condition->ToString().c_str());
		}
	}
	*/
}

void Configuration::InitializeTypesCodesAndRunes() {
	std::unordered_map<std::wstring, int32_t> names;
	std::unordered_map<std::wstring, int32_t> codes;
	std::unordered_map<std::wstring, int32_t> runes;

	DataTables* sgptDataTables = *D2COMMON_gpDataTables;

	for (int i = 0; i < D2COMMON_ItemDataTbl->nItemsTxtRecordCount; i++) {
		auto pItemTxt = D2COMMON_ItemDataTbl->pItemsTxt[i];
		std::wstring wNameStr = D2LANG_GetStringFromTblIndex(pItemTxt.wNameStr);
		names[wNameStr] = pItemTxt.dwCode;
		std::wstring wCode = std::wstring(4, L' ');
		mbstowcs(&wCode[0], pItemTxt.szCode, 4);
		wCode = trim(wCode);
		codes[wCode] = pItemTxt.dwCode;
		if (pItemTxt.wType[0] == ItemType::RUNE) {
			int nRuneNumber = std::stoi(std::string(&pItemTxt.szCode[1], 3));
			runes[wNameStr] = nRuneNumber;
			size_t nFound = wNameStr.find(L" ");
			if (nFound != std::wstring::npos) {
				runes[wNameStr.substr(0, nFound)] = nRuneNumber;
			}
		}
	}

	std::unordered_map<std::wstring, int32_t> classNames;

	for (int32_t i = 0; i < sgptDataTables->nCharStatsTxtRecordCount; i++) {
		CharStatsTxt charStats = sgptDataTables->pCharStatsTxt[i];
		std::wstring className = std::wstring(charStats.wszClassName);
		className = trim(className);
		classNames[className] = i;
	}

	for (auto& rule : GlobalRules) {
		for (auto& condition : rule.second->GetConditions()) {
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
		for (auto& condition : rule.second->GetConditions()) {
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
		for (auto& condition : rule.second->GetConditions()) {
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
	std::unordered_map<std::wstring, int32_t> variables;

	for (auto& rule : GlobalRules) {
		for (auto& condition : rule.second->GetConditions()) {
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