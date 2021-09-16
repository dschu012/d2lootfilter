#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "Rule.h"

extern std::unordered_map<std::wstring, std::vector<Action*>> GlobalStyles;
extern std::map<uint32_t, Rule*> GlobalRules;

struct Settings {
	std::string wPath;
	int32_t nFilterLevel;
	int32_t nPingLevel;
};

class Configuration {
private:
	Settings* m_Settings;
	bool m_Loaded = false;

	void HandleToken(uint32_t lineNumber, std::vector<std::wstring>& lines);
	std::vector<Action*> ParseStyle(std::vector<std::wstring>& lines);
	Rule* ParseRule(std::vector<std::wstring>& lines);
	std::vector<Condition*> ParseConditions(std::vector<std::wstring>& lines);
	std::vector<Action*> ParseActions(std::vector<std::wstring>& lines);
	void ReadSettings();
	void InitalizeConditionVariables();
	void InitializeTypesCodesAndRunes();
	void InitializeClass();
	void InitializeRaritiesAndQualities();
	void InitializeOther();
public:
	Configuration();

	void SaveSettings();
	void Load();
	bool IsLoaded() { return m_Loaded; }
};

