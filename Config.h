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

enum class ConfigToken : uint8_t {
	NONE,
	STYLE,
	SHOW,
	HIDE
};

class Config {
private:
	std::wstring m_Path;
	ConditionMap m_ValidConditions;
	ActionMap m_ValidActions;

	void SetupValidConditions();
	void SetupValidActions();
	void HandleToken(ConfigToken token, std::wstring* line, uint32_t lineNum, std::vector<Rule*>& rules, std::map<std::wstring, std::vector<Action*>>& styles, std::vector<Line>& lines);
	void ParseStyle(std::wstring* line, std::map<std::wstring, std::vector<Action*>>& styles, std::vector<Line>& lines);
	void ParseRule(Rule* rule, std::vector<Rule*>& rules, std::vector<Line>& lines);
public:
	Config(std::wstring path);
	void ParseFilter(std::vector<Rule*>& rules, std::map<std::wstring, std::vector<Action*>>& styles);
};

