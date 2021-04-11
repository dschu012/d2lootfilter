#pragma once

#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include "Condition.h"
#include "Action.h"



class Rule {
private:
	uint32_t m_LineNum;
	bool m_IsContinue = false;
	std::wstring m_Text;
	std::vector<Condition*> m_Conditions;
	std::vector<Action*> m_Actions;
public:
	Rule(uint32_t lineNum, std::wstring text);
	bool Evaluate(Unit* pItem);
	void EvaluateActionResult(ItemActionResult* action, Unit* pItem);
	void SetIsContinue(bool isContinue) { m_IsContinue = isContinue; };
	bool IsContinue() { return m_IsContinue; };
	void InitializeConditions();
	void AddCondition(Condition* condition) { m_Conditions.push_back(condition); };
	void AddAction(Action* condition) { m_Actions.push_back(condition); };
	uint32_t GetLine() { return m_LineNum; }
};

