#pragma once

#include <cstdint>
#include <vector>
#include "D2Structs.h"
#include "Condition.h"
#include "Action.h"

class Rule {
private:
	uint32_t m_LineNumber;
	std::vector<Condition*> m_Conditions;
	std::vector<Action*> m_Actions;
public:
	Rule() {}
	bool Evaluate(Unit* pUnit);
	void EvaluateActionResult(ActionResult* pActionResult, Unit* pItem);
	uint32_t GetLineNumber();
	void SetLineNumber(uint32_t lineNumber);
	std::vector<Condition*> GetConditions();
	std::vector<Action*> GetActions();
	void AddAction(Action* pAction, int32_t idx);
	void AddActions(std::vector<Action*> actions);
	void AddCondition(Condition* pCondition);
	void AddConditions(std::vector<Condition*> conditions);
};