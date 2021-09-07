#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "Rule.h"

extern std::unordered_map<std::wstring, std::vector<Action*>> GlobalStyles;
extern std::unordered_map<uint32_t, Rule*> GlobalRules;

class Configuration {
private:
	std::wstring m_Path;
	bool m_Loaded = false;

	void HandleToken(uint32_t lineNumber, std::vector<std::wstring>& lines);
	std::vector<Action*> ParseStyle(std::vector<std::wstring>& lines);
	Rule* ParseRule(std::vector<std::wstring>& lines);
	std::vector<Condition*> ParseConditions(std::vector<std::wstring>& lines);
	std::vector<Action*> ParseActions(std::vector<std::wstring>& lines);
	void InitalizeConditionVariables();
	void InitializeTypesCodesAndRunes();
	void InitializeClass();
	void InitializeRaritiesAndQualities();
	void InitializeOther();
public:
	Configuration(std::wstring sPath);
	void Load();
	bool IsLoaded() { return m_Loaded; }
};

