#pragma once
#include <Windows.h>
#include <map>
#include "Rule.h"
#include "Condition.h"
#include "Action.h"

#define COMMENT L"#"

template<typename T> Condition* newInstance(std::wstring value) { return new T(value); }
typedef std::map<std::wstring, Condition* (*)(std::wstring)> ConditionMap;

template<typename T> Action* newInstance(std::wstring value) { return new T(value); }
typedef std::map<std::wstring, Action* (*)(std::wstring)> ActionMap;

struct Line {
	uint32_t nLineNum;
	std::wstring sText;
};


class Config {
private:
	std::wstring m_Path;

	void SetupValidConditions(ConditionMap& validConditions);
	void SetupValidActions(ActionMap& validActions);
	void ParseRule(Rule* rule, std::vector<Rule*>& rules, ConditionMap validConditions, ActionMap validActions, std::vector<Line>& lines);
public:
	Config(std::wstring path);
	std::vector<Rule*> ParseFilter();
};

