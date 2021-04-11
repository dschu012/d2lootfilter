#include "Config.h"
#include <fmt/format.h>
#include <fmt/core.h>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "Condition.h"
#include "Utils.h"


namespace fs = std::filesystem;

#define CONDITION(NAME) validConditions[L#NAME " "] = &newInstance<NAME##Condition>
#define ACTION(NAME) validActions[L#NAME " "] = &newInstance<NAME##Action>

Config::Config(std::wstring path) : m_Path(path) {
}

void Config::SetupValidConditions(ConditionMap& validConditions) {
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

void Config::SetupValidActions(ActionMap& validActions) {
	ACTION(SetName);
	ACTION(SetDescription);
	ACTION(SetBackgroundColor);
	ACTION(SetInventoryColor);
	ACTION(SetBorderColor);
	ACTION(ChatNotify);
	ACTION(PlayAlert); //todo.
	ACTION(MinimapIcon); //todo.
}

void Config::ParseRule(Rule* rule, std::vector<Rule*> &rules, ConditionMap validConditions, ActionMap validActions, std::vector<Line>& lines) {
	if (!rule) {
		return;
	}

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
		for (auto validCondition : validConditions) {
			int l = validCondition.first.length();
			if (s.compare(0, l, validCondition.first) == 0) {
				conditionName = s.substr(0, l);
				value = s.substr(l);
				break;
			}
		}
		if (!conditionName.empty() && !value.empty()) {
			andCondition->AddCondition(validConditions[conditionName](value));
		}
		else {
			for (auto validAction : validActions) {
				int l = validAction.first.length();
				if (s.compare(0, l, validAction.first) == 0) {
					conditionName = s.substr(0, l);
					value = s.substr(l);
					break;
				}
			}
			if (!conditionName.empty() && !value.empty()) {
				rule->AddAction(validActions[conditionName](value));
			}
		}
	}
	rule->AddCondition(andCondition);
	rules.push_back(rule);
	lines.clear();
}

std::vector<Rule*> Config::ParseFilter() {
	if (!fs::exists(m_Path)) {
		MessageBoxA(nullptr, "No item.filter found.", "Error", MB_OK | MB_ICONSTOP);
		exit(0);
	}

	ConditionMap conditionMap;
	ActionMap actionMap;
	SetupValidConditions(conditionMap);
	SetupValidActions(actionMap);

	std::vector<Rule*> rules;
	std::wifstream f(m_Path);
	std::wstring s;
	uint32_t lineNum = 0;
	Rule* r = NULL;
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
		if (s.compare(0, 4, L"Show") == 0) {
			ParseRule(r, rules, conditionMap, actionMap, lines);
			r = new Rule(lineNum, s);
			r->AddAction(new ShowAction());
		}
		else if (s.compare(0, 4, L"Hide") == 0) {
			ParseRule(r, rules, conditionMap, actionMap, lines);
			r = new Rule(lineNum, s);
			r->AddAction(new HideAction());
		}
		else if (r) {
			lines.push_back({ lineNum, s });
		}
		else {
			DEBUG_LOG(L"Invalid Line: {}, expected...\n", lineNum);
		}
	}
	ParseRule(r, rules, conditionMap, actionMap, lines);
	return rules;
}
