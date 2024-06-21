#pragma once

#include <cstdint>
#include <vector>
#include "D2Structs.h"
#include "Condition.h"
#include "Action.h"

class Rule {
private:
	uint32_t m_LineNumber;
	std::vector<std::unique_ptr<Condition>> m_Conditions;
	std::vector<std::unique_ptr<Action>> m_Actions;
public:
	Rule(uint32_t lineNumber) : m_LineNumber{ lineNumber } {}
	bool Evaluate(Unit* pUnit);
	void EvaluateActionResult(ActionResult& actionResult, Unit* pItem) const;
	uint32_t GetLineNumber() const;
	const std::vector<std::unique_ptr<Condition>>& GetConditions();
	const std::vector<std::unique_ptr<Action>>& GetActions();
	void AddAction(std::unique_ptr<Action> action);
	void AddActions(std::vector<std::unique_ptr<Action>> actions);
	void AddCondition(std::unique_ptr<Condition> condition);
	void AddConditions(std::vector<std::unique_ptr<Condition>> conditions);
};