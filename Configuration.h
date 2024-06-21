#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "Rule.h"

extern utility::string_umap<std::wstring, std::vector<std::unique_ptr<Action>>> GlobalStyles;
extern std::map<uint32_t, Rule> GlobalRules;

struct Settings {
	std::string wPath;
	int32_t nFilterLevel;
	int32_t nPingLevel;
};

class Configuration {
private:
	Settings m_Settings;
	bool m_Loaded = false;

	void HandleToken(uint32_t lineNumber, std::vector<std::wstring>& lines);
	std::vector<std::unique_ptr<Action>> ParseStyle(std::vector<std::wstring>& lines);
	Rule ParseRule(uint32_t lineNumber, std::vector<std::wstring>& lines);
	std::vector<std::unique_ptr<Condition>> ParseConditions(std::vector<std::wstring>& lines);
	std::vector<std::unique_ptr<Action>> ParseActions(std::vector<std::wstring>& lines);
	void ReadSettings();
	void InitalizeConditionVariables();
	void InitializeTypesCodesAndRunes();
	void InitializeClass();
	void InitializeRaritiesAndQualities();
	void InitializeOther();
public:
	friend class ItemFilter;

	Configuration() : m_Settings{ "./item.filter", 6, 6 } {}
	~Configuration() { SaveSettings(); }

	void SaveSettings();
	void LoadSettings();
	bool IsLoaded() { return m_Loaded; }
};

