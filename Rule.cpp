#include "Rule.h"


Rule::Rule(uint32_t lineNum) : m_LineNum(lineNum) {
}

bool Rule::Evaluate(Unit* pItem) {
	return std::all_of(m_Conditions.begin(), m_Conditions.end(), [pItem](Condition* c) { return c->Evaluate(pItem); });
}

void Rule::EvaluateActionResult(ItemActionResult* actionResult, Unit* pItem) {
	for (auto& action : m_Actions) {
		action->SetResult(actionResult, pItem);
	}
}

void Rule::InitializeConditions() {
	for (auto& condition : m_Conditions) {
		condition->Initialize();
	}
}
