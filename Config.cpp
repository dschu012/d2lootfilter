#include "Config.h"
#include <fmt/format.h>
#include <fmt/core.h>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "Condition.h"
#include "Utils.h"


namespace fs = std::filesystem;

#define CONDITION(NAME) m_ValidConditions[L#NAME " "] = &newInstance<NAME##Condition>
#define ACTION(NAME) m_ValidActions[L#NAME " "] = &newInstance<NAME##Action>

Config::Config(std::wstring path) : m_Path(path) {
	SetupValidConditions();
	SetupValidActions();
}

void Config::SetupValidConditions() {
	CONDITION(Type);
	CONDITION(Code);
	CONDITION(Class); //Helm, Sword, etc... (ItemTypes.txt)
	//Quality
	CONDITION(Ethereal);
	CONDITION(Rarity); //Rare, Magic, Set, Unique
	CONDITION(Runeword);
	CONDITION(Prefix);
	CONDITION(Suffix);
	//A, I, C Level
	CONDITION(ItemLevel);
	CONDITION(Quality);
	CONDITION(AreaLevel);
	CONDITION(CharacterLevel);
	CONDITION(Difficulty);
	CONDITION(Rune);
	CONDITION(Id);  //Id from Sets.txt or Uniques.txt
	CONDITION(Gold);
	//CONDITION(Quantity);
	//Stats
	CONDITION(Stats);
	CONDITION(Defense);
	//Armor
	CONDITION(Armor);
	//Weapons
	CONDITION(Weapon);
	//Misc
	CONDITION(Price);
	CONDITION(Mode); //Expansion / Classic
	CONDITION(Identified);
	CONDITION(Sockets);
	CONDITION(Width);
	CONDITION(Height);
}

void Config::SetupValidActions() {
	ACTION(SetStyle);
	ACTION(SetName);
	ACTION(SetDescription);
	ACTION(SetBackgroundColor);
	ACTION(SetInventoryColor);
	ACTION(SetBorderColor);
	ACTION(ChatNotify);
	ACTION(PlayAlert); //todo.
	ACTION(MinimapIcon); //todo.
}

void Config::HandleToken(ConfigToken token, std::wstring* line, uint32_t lineNum, std::vector<Rule*>& rules, std::map<std::wstring, std::vector<Action*>>& styles, std::vector<Line>& lines) {
	switch (token) {
	case ConfigToken::STYLE: {
		ParseStyle(line, styles, lines);
		break;
	}
	case ConfigToken::SHOW: {
		Rule* rule = new Rule(lineNum);
		rule->AddAction(new ShowAction());
		ParseRule(rule, rules, lines);
		break;
	}
	case ConfigToken::HIDE: {
		Rule* rule = new Rule(lineNum);
		rule->AddAction(new HideAction());
		ParseRule(rule, rules, lines);
		break;
	}
	default:
		break;
	}
	lines.clear();
}

void Config::ParseStyle(std::wstring* line, std::map<std::wstring, std::vector<Action*>>& styles, std::vector<Line>& lines) {
	std::wstring style = std::wstring(trim(line->substr(6)));
	for (auto line : lines) {
		std::wstring s = line.sText;
		//ignore comments or blank lines
		if (s.find(COMMENT) != std::string::npos) {
			s = s.erase(s.find(COMMENT));
		}
		s = trim(s);
		if (s.length() == 0) {
			continue;
		}
		std::wstring conditionName = L"";
		std::wstring value = L"";
		for (auto validAction : m_ValidActions) {
			int l = validAction.first.length();
			if (s.compare(0, l, validAction.first) == 0) {
				conditionName = s.substr(0, l);
				value = s.substr(l);
				break;
			}
		}
		if (!conditionName.empty() && !value.empty()) {
			styles[style].push_back(m_ValidActions[conditionName](value));
		}
	}
}

void Config::ParseRule(Rule* rule, std::vector<Rule*> &rules, std::vector<Line>& lines) {
	AndCondition* andCondition = new AndCondition();
	for (auto line : lines) {
		std::wstring s = line.sText;
		//ignore comments or blank lines
		if (s.find(COMMENT) != std::string::npos) {
			s = s.erase(s.find(COMMENT));
		}
		s = trim(s);
		if (s.length() == 0) {
			continue;
		}
		if (s.compare(0, 8, L"Continue") == 0) {
			rule->SetIsContinue(true);
			continue;
		}
		std::wstring conditionName = L"";
		std::wstring value = L"";
		for (auto validCondition : m_ValidConditions) {
			int l = validCondition.first.length();
			if (s.compare(0, l, validCondition.first) == 0) {
				conditionName = s.substr(0, l);
				value = s.substr(l);
				break;
			}
		}
		if (!conditionName.empty() && !value.empty()) {
			andCondition->AddCondition(m_ValidConditions[conditionName](value));
		}
		else {
			for (auto validAction : m_ValidActions) {
				int l = validAction.first.length();
				if (s.compare(0, l, validAction.first) == 0) {
					conditionName = s.substr(0, l);
					value = s.substr(l);
					break;
				}
			}
			if (!conditionName.empty() && !value.empty()) {
				rule->AddAction(m_ValidActions[conditionName](value));
			}
		}
	}
	rule->AddCondition(andCondition);
	rules.push_back(rule);
	lines.clear();
}

void Config::ParseFilter(std::vector<Rule*>& rules, std::map<std::wstring, std::vector<Action*>>& styles) {
	if (!fs::exists(m_Path)) {
		MessageBoxA(nullptr, "No item.filter found.", "Error", MB_OK | MB_ICONSTOP);
		exit(0);
	}

	std::wifstream f(m_Path);
	std::wstring s;
	uint32_t tokenLineNum = 0;
	uint32_t lineNum = 0;
	ConfigToken token = ConfigToken::NONE;
	Rule* r = NULL;
	std::wstring* line = NULL;
	std::vector<Line> lines;

	while (std::getline(f, s)) {
		lineNum++;
		//ignore comments or blank lines
		if (s.find(COMMENT) != std::string::npos) {
			s = s.erase(s.find(COMMENT));
		}
		s = trim(s);
		if (s.length() == 0) {
			continue;
		}
		if (s.compare(0, 5, L"Style") == 0) {
			tokenLineNum = lineNum;
			HandleToken(token, line, tokenLineNum, rules, styles, lines);
			token = ConfigToken::STYLE;
			line = new std::wstring(s);
		}
		else if (s.compare(0, 4, L"Show") == 0) {
			tokenLineNum = lineNum;
			HandleToken(token, line, tokenLineNum, rules, styles, lines);
			token = ConfigToken::SHOW;
		}
		else if (s.compare(0, 4, L"Hide") == 0) {
			tokenLineNum = lineNum;
			HandleToken(token, line, tokenLineNum, rules, styles, lines);
			token = ConfigToken::HIDE;
		}
		else {
			lines.push_back({ lineNum, s });
		}
	}
	HandleToken(token, line, tokenLineNum, rules, styles, lines);
}
