#include "Rule.h"

bool Rule::Evaluate(Unit* pItem) {
	for (auto& condition : m_Conditions) {
		if (!condition->Evaluate(pItem)) {
			return false;
		}
	}
	return true;
}

void Rule::EvaluateActionResult(ActionResult& actionResult, Unit* pItem) const {
	actionResult.bContinue = false;
	for (auto& action : m_Actions) {
		action->SetResult(actionResult, pItem);
	}
}

uint32_t Rule::GetLineNumber() const {
	return m_LineNumber;
}

const std::vector<std::unique_ptr<Condition>>& Rule::GetConditions() {
	return m_Conditions;
}

const std::vector<std::unique_ptr<Action>>& Rule::GetActions() {
	return m_Actions;
}

void Rule::AddAction(std::unique_ptr<Action> action) {
	m_Actions.push_back(std::move(action));
}

void Rule::AddActions(std::vector<std::unique_ptr<Action>> actions) {
	std::move(actions.begin(), actions.end(), std::back_inserter(m_Actions));
}

void Rule::AddCondition(std::unique_ptr<Condition> condition) {
	m_Conditions.push_back(std::move(condition));
}

void Rule::AddConditions(std::vector<std::unique_ptr<Condition>> conditions) {
	std::move(conditions.begin(), conditions.end(), std::back_inserter(m_Conditions));
}