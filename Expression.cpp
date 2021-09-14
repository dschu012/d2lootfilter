#include "Expression.h"
#include <format>
#include <sstream>
#include <algorithm>
#include "D2Ptrs.h"
#include "Utils.h"
#include "ItemFilter.h"


static int32_t GetFilterLevel(Unit* pUnit) {
    return FilterLevel;
}

static int32_t GetPingLevel(Unit* pUnit) {
    return PingLevel;
}

static int32_t GetPlayerLevel(Unit* pUnit) {
    return D2COMMON_STATLIST_GetUnitStatUnsigned(D2CLIENT_GetPlayerUnit(), Stat::LEVEL, 0);
}

static int32_t GetItemLevel(Unit* pUnit) {
    return pUnit->pItemData->dwItemLevel;
}

static int32_t GetLevel(Unit* pUnit) {
    return GetItemsTxt(pUnit).nLevel;
}

static int32_t GetMagicLevel(Unit* pUnit) {
    return GetItemsTxt(pUnit).nMagicLevel;
}

static int32_t GetAffixLevel(int32_t iLvl, int32_t qLvl, int32_t mLvl) {
    if (iLvl > 99) iLvl = 99;
    if (qLvl > iLvl) iLvl = qLvl;
    if (mLvl > 0) return iLvl + mLvl > 99 ? 99 : iLvl + mLvl;
    return ((iLvl) < (99 - ((qLvl) / 2)) ? (iLvl)-((qLvl) / 2) : (iLvl) * 2 - 99);
}

static int32_t GetAffixLevel(Unit* pUnit) {
    return GetAffixLevel(
        GetItemLevel(pUnit),
        GetLevel(pUnit),
        GetMagicLevel(pUnit)
    );
}

static int32_t GetCraftAffixLevel(Unit* pUnit) {
    int32_t cLvl = GetPlayerLevel(pUnit);
    int32_t iLvl = GetItemLevel(pUnit);
    return GetAffixLevel(
        iLvl / 2 + cLvl / 2,
        GetLevel(pUnit),
        GetMagicLevel(pUnit)
    );
}

static std::unordered_map<std::wstring, GlobalVariableFunction> GlobalVariables = {
    { L"Filter Level", &GetFilterLevel },
    { L"Ping Level", &GetPingLevel },
    { L"Character Level", &GetPlayerLevel },
    { L"Item Level", &GetItemLevel },
    { L"Quality Level", &GetLevel },
    { L"Magic Level", &GetMagicLevel },
    { L"Affix Level", &GetAffixLevel },
    { L"Craft Affix Level", &GetCraftAffixLevel }
};

int32_t Logical::Evaluate(Unit* pItem) {
    int32_t left = m_Left->Evaluate(pItem);
    int32_t right = m_Right->Evaluate(pItem);
    int32_t ret = 0;
    //Logger::Info("Doing %d%s%d\n", left, OPS[static_cast<uint8_t>(m_Operator)], right);
    switch (m_Operator) {
    case Token::AND: ret = left && right; break;
    case Token::OR: ret = left || right; break;
    case Token::EQUALS: ret = left == right; break;
    case Token::BANG_EQUALS: ret = left != right; break;
    case Token::GREATER_THAN: ret = left > right; break;
    case Token::GREATER_THAN_EQUALS: ret = left >= right; break;
    case Token::LESS_THAN: ret = left < right; break;
    case Token::LESS_THAN_EQUALS: ret = left <= right; break;
    default:
        break;
        //error...
    }
    //Logger::Info("Return %d%s%d = %d\n", left, OPS[static_cast<uint8_t>(m_Operator)], right, ret);
    return ret;
}

std::wstring Logical::ToString(Unit* pItem) {
    return std::format(L"{}{}{}", m_Left->ToString(pItem), OPS[static_cast<uint8_t>(m_Operator)], m_Right->ToString(pItem));
}

void Logical::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
    m_Left->SetVariables(variables);
    m_Right->SetVariables(variables);
}

int32_t Binary::Evaluate(Unit* pItem) {
    int32_t left = m_Left->Evaluate(pItem);
    int32_t right = m_Right->Evaluate(pItem);
    int32_t ret = 0;
    //Logger::Info("Doing %d%s%d\n", left, OPS[static_cast<uint8_t>(m_Operator)], right);
    switch (m_Operator) {
    case Token::PLUS: ret = left + right; break;
    case Token::MINUS: ret = left - right; break;
    case Token::MULTIPLY: ret = left * right; break;
    case Token::DIVIDE: ret = left / right; break;
    default:
        break;
        //error..
    }
    //Logger::Info("Return %d%s%d = %d\n", left, OPS[static_cast<uint8_t>(m_Operator)], right, ret);
    return ret;
}

std::wstring Binary::ToString(Unit* pItem) {
    return std::format(L"{}{}{}", m_Left->ToString(pItem), OPS[static_cast<uint8_t>(m_Operator)], m_Right->ToString(pItem));
}

void Binary::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
    m_Left->SetVariables(variables);
    m_Right->SetVariables(variables);
}

int32_t In::Evaluate(Unit* pItem) {
    int32_t left = m_Left->Evaluate(pItem);
    int32_t min = m_Min->Evaluate(pItem);
    int32_t max = m_Max->Evaluate(pItem);
    return left >= min
        && left <= max;
}

std::wstring In::ToString(Unit* pItem) {
    return std::format(L"{}{}{}-{}", m_Left->ToString(pItem), OPS[static_cast<uint8_t>(m_Operator)], m_Min->ToString(pItem), m_Max->ToString(pItem));
}

void In::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
    m_Left->SetVariables(variables);
    m_Min->SetVariables(variables);
    m_Max->SetVariables(variables);
}

int32_t Unary::Evaluate(Unit* pItem) {
    int32_t right = m_Right->Evaluate(pItem);
    int32_t ret = 0;
    switch (m_Operator) {
    case Token::BANG: ret = !right; break;
    case Token::MINUS: ret = -1 * m_Right->Evaluate(pItem); break;
    default:
        break;
        //error
    }
    return ret;
}

std::wstring Unary::ToString(Unit* pItem) {
    return std::format(L"{}{}", OPS[static_cast<uint8_t>(m_Operator)], m_Right->ToString(pItem));
}

void Unary::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
    m_Right->SetVariables(variables);
}

int32_t Literal::Evaluate(Unit* pItem) {
    return m_Value;
}

std::wstring Literal::ToString(Unit* pItem) {
    return std::format(L"{}", m_Value);
}

void Literal::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
}

int32_t Boolean::Evaluate(Unit* pItem) {
    return m_Value;
}

std::wstring Boolean::ToString(Unit* pItem) {
    return std::format(L"{}", m_Value);
}

void Boolean::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
}

Variable::Variable(std::wstring variable) {
    m_Variable = variable;
    if (GlobalVariables.contains(m_Variable)) {
        m_Func = GlobalVariables[m_Variable];
    }
};

int32_t Variable::Evaluate(Unit* pItem) {
    if (m_Func != nullptr) {
        return m_Func(pItem);
    }
    return m_Value.value_or(0);
}

std::wstring Variable::ToString(Unit* pItem) {
    if (m_Func != nullptr) {
        return std::format(L"{}({})", m_Variable, m_Func(pItem));
    }
    if (!m_Value) {
        return std::format(L"{}(nullptr)", m_Variable);
    }
    return std::format(L"{}({})", m_Variable, m_Value.value());
}

void Variable::SetValue(const int32_t v) {
    if (m_Func != nullptr) {
        return;
    }
    m_Value = v;
}

void Variable::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
    if (variables.contains(m_Variable)) {
        SetValue(variables[m_Variable]);
    }
}

int32_t Call::EvaluateClass(Unit* pItem, std::vector<int32_t>& args) {
    for (auto& arg : args) {
        if (D2COMMON_ITEMS_CheckItemTypeId(pItem, static_cast<ItemType>(arg))) {
            return 1;
        }
    }
    return 0;
}

int32_t Call::EvaluateChargedSkill(Unit* pItem, Stat stat, std::vector<int32_t>& args) {
    if (!(pItem->pStatListEx && (pItem->pStatListEx->dwFlags & StatlistFlags::EXTENDED))) {
        return 0;
    }
    StatList* pStatList = pItem->pStatListEx->pMyLastList;
    while (pStatList && !(0x40 & pStatList->dwFlags))
    {
        pStatList = pStatList->pPrevLink;
    }
    if (!pStatList) {
        return 0;
    }
    int32_t value = 0;
    int32_t layer = 0;
    if (args.size() > 0) layer = args[0];
    StatsArray arr = pStatList->Stats;
    for (uint8_t i = 0; i < arr.nStatCount; i++) {
        UnitStat stat = arr.pStat[i];
        if (stat.nStat == static_cast<uint16_t>(Stat::ITEM_CHARGED_SKILL)
            && (stat.nLayer >> 6) == layer) {
            int32_t level = stat.nStat & 0x3F;
            value = (level > value) ? level : value;
        }
    }
    return value;
}

int32_t Call::EvaluateStat(Unit* pItem, Stat stat, std::vector<int32_t>& args) {
    int32_t layer = 0;
    if (args.size() > 0) layer = args[0];
    return D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, stat, layer);
}

int32_t Call::Evaluate(Unit* pItem) {
    std::vector<int32_t> args;
    std::transform(m_Args.begin(), m_Args.end(), std::back_inserter(args),
        [pItem](Expression*& expression) { return expression->Evaluate(pItem); });
    switch (m_Func) {
    case Token::STAT:
    {
        int32_t statId = args[0];
        args.erase(args.begin());
        return EvaluateStat(pItem, static_cast<Stat>(statId), args);
    }
    case Token::CLASS:
        return EvaluateClass(pItem, args);
    case Token::CHARGEDSKILL:
        return EvaluateChargedSkill(pItem, Stat::ITEM_CHARGED_SKILL, args);
    case Token::TABSKILL:
        return EvaluateStat(pItem, Stat::ITEM_ADDSKILL_TAB, args);
    case Token::SKILL:
        return EvaluateStat(pItem, Stat::ITEM_SINGLESKILL, args);
    case Token::CLASSSKILL:
        return EvaluateStat(pItem, Stat::ITEM_ADDCLASSSKILLS, args);
        //by default we exclude 0s in min and max because a stat that doesn't exist on an item
        //will return 0 for the stat value. MinIn (min inclusive) will include zeros.
    case Token::MAX:
        return *std::ranges::max_element(args, [](int32_t o1, int32_t o2) {
            return o1 != 0 && o2 != 0 ? o1 > o2 : false;
            });
    case Token::MIN:
        return *std::ranges::min_element(args, [](int32_t o1, int32_t o2) {
            return o1 != 0 && o2 != 0 ? o1 < o2 : false;
            });
    case Token::MININ:
        return *std::ranges::min_element(args, [](int32_t o1, int32_t o2) {
            return o1 < o2;
            });
    default:
        return 0;
        //throw error?
    }
}

std::wstring Call::ToString(Unit* pItem) {
    std::wostringstream ss;
    for (auto& expression : m_Args) {
        if (&expression != &m_Args.front()) {
            ss << L", ";
        }
        ss << expression->ToString(pItem);
    }
    return std::format(L"{}({})({})", OPS[static_cast<uint8_t>(m_Func)], ss.str(), Evaluate(pItem));
}

void Call::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
    for (auto& expression : m_Args) {
        expression->SetVariables(variables);
    }
}

int32_t ListExpression::Evaluate(Unit* pItem) {
    //return true if any are true
    return std::any_of(m_List.begin(), m_List.end(), [pItem](Expression* expression) { return expression->Evaluate(pItem) != 0; });
}

std::wstring ListExpression::ToString(Unit* pItem) {
    std::wostringstream ss;
    for (auto& expression : m_List) {
        if (&expression != &m_List.front()) {
            ss << L" or ";
        }
        ss << expression->ToString(pItem).c_str();
    }
    return ss.str();
}

void ListExpression::SetVariables(std::unordered_map<std::wstring, int32_t>& variables) {
    for (auto& expression : m_List) {
        expression->SetVariables(variables);
    }
}

bool Tokenizer::IsNull(wchar_t c) {
    return c == L'\0';
}

bool Tokenizer::IsOperator(const wchar_t*& expression) {
    return _wcsnicmp(expression, L" and ", 5) == 0
        || _wcsnicmp(expression, L"true", 4) == 0
        || _wcsnicmp(expression, L"false", 5) == 0
        || _wcsnicmp(expression, L" or ", 4) == 0
        || _wcsnicmp(expression, L"|", 1) == 0
        || _wcsnicmp(expression, L"&", 1) == 0
        || wcsncmp(expression, L",", 1) == 0
        || wcsncmp(expression, L"=", 1) == 0
        || wcsncmp(expression, L"<", 1) == 0
        || wcsncmp(expression, L">", 1) == 0
        || wcsncmp(expression, L"!", 1) == 0
        || wcsncmp(expression, L"(", 1) == 0
        || wcsncmp(expression, L")", 1) == 0
        || wcsncmp(expression, L"+", 1) == 0
        || wcsncmp(expression, L"-", 1) == 0
        || wcsncmp(expression, L"*", 1) == 0
        || wcsncmp(expression, L"/", 1) == 0;
}

std::wstring Tokenizer::ParseVariable(const wchar_t*& expression) {
    int s = 0;
    while (!IsOperator(expression) && !IsNull(*expression)) {
        expression += 1;
        s += 1;
    }
    return std::wstring(expression - s, s);
}

std::wstring Tokenizer::ParseQuotedVariable(const wchar_t*& expression) {
    int s = 1;
    expression += 1;
    while (*expression != '"' && !IsNull(*expression)) {
        expression += 1;
        s += 1;
    }
    if (*expression != '"') {
        //throw error. unquoted string variable
    }
    expression += 1;
    return std::wstring(expression - s, s - 1);
}

std::wstring Tokenizer::ParseDigit(const wchar_t*& expression) {
    int s = 0;
    while (std::isdigit(*expression) && !IsNull(*expression)) {
        expression += 1;
        s += 1;
    }
    return std::wstring(expression - s, s);
}

void Tokenizer::Tokenize(const wchar_t*& expression) {
    while (!IsNull(*expression)) {
        if (*expression == L'(') { expression++; m_Tokens.push_back(new TokenizerToken(Token::LEFT_PAREN)); }
        else if (*expression == L')') { expression++; m_Tokens.push_back(new TokenizerToken(Token::RIGHT_PAREN)); }
        else if (_wcsnicmp(expression, L"chargedskill(", 13) == 0) { expression += 12; m_Tokens.push_back(new TokenizerToken(Token::CHARGEDSKILL)); }
        else if (_wcsnicmp(expression, L"classskill(", 11) == 0) { expression += 10; m_Tokens.push_back(new TokenizerToken(Token::CLASSSKILL)); }
        else if (_wcsnicmp(expression, L"tabskill(", 9) == 0) { expression += 8; m_Tokens.push_back(new TokenizerToken(Token::TABSKILL)); }
        else if (_wcsnicmp(expression, L"skill(", 6) == 0) { expression += 5; m_Tokens.push_back(new TokenizerToken(Token::SKILL)); }
        else if (_wcsnicmp(expression, L"class(", 6) == 0) { expression += 5; m_Tokens.push_back(new TokenizerToken(Token::CLASS)); }
        else if (_wcsnicmp(expression, L"minin(", 6) == 0) { expression += 5; m_Tokens.push_back(new TokenizerToken(Token::MININ)); }
        else if (_wcsnicmp(expression, L"stat(", 5) == 0) { expression += 4; m_Tokens.push_back(new TokenizerToken(Token::STAT)); }
        else if (_wcsnicmp(expression, L"false", 5) == 0) { expression += 5; m_Tokens.push_back(new TokenizerToken(Token::_FALSE)); }
        else if (_wcsnicmp(expression, L"true", 4) == 0) { expression += 4; m_Tokens.push_back(new TokenizerToken(Token::_TRUE)); }
        else if (_wcsnicmp(expression, L"min(", 4) == 0) { expression += 3; m_Tokens.push_back(new TokenizerToken(Token::MIN)); }
        else if (_wcsnicmp(expression, L"max(", 4) == 0) { expression += 3; m_Tokens.push_back(new TokenizerToken(Token::MAX)); }
        else if (_wcsnicmp(expression, L"and ", 4) == 0) { expression += 3; m_Tokens.push_back(new TokenizerToken(Token::AND)); }
        else if (_wcsnicmp(expression, L"or ", 3) == 0) { expression += 2; m_Tokens.push_back(new TokenizerToken(Token::OR)); }
        else if (wcsncmp(expression, L"&&", 2) == 0) { expression += 2; m_Tokens.push_back(new TokenizerToken(Token::AND)); }
        else if (wcsncmp(expression, L"||", 2) == 0) { expression += 2; m_Tokens.push_back(new TokenizerToken(Token::OR)); }
        else if (_wcsnicmp(expression, L"in ", 3) == 0) { expression += 2; m_Tokens.push_back(new TokenizerToken(Token::_IN)); }
        else if (wcsncmp(expression, L"!=", 2) == 0) { expression += 2; m_Tokens.push_back(new TokenizerToken(Token::BANG_EQUALS)); }
        else if (wcsncmp(expression, L"==", 2) == 0) { expression += 2; m_Tokens.push_back(new TokenizerToken(Token::EQUALS)); }
        else if (wcsncmp(expression, L">=", 2) == 0) { expression += 2; m_Tokens.push_back(new TokenizerToken(Token::GREATER_THAN_EQUALS)); }
        else if (wcsncmp(expression, L"<=", 2) == 0) { expression += 2; m_Tokens.push_back(new TokenizerToken(Token::LESS_THAN_EQUALS)); }
        else if (*expression == L'>') { expression++; m_Tokens.push_back(new TokenizerToken(Token::GREATER_THAN)); }
        else if (*expression == L'<') { expression++; m_Tokens.push_back(new TokenizerToken(Token::LESS_THAN)); }
        else if (*expression == L'!') { expression++; m_Tokens.push_back(new TokenizerToken(Token::BANG)); }
        else if (*expression == L'=') { expression++; m_Tokens.push_back(new TokenizerToken(Token::EQUALS)); }
        else if (*expression == L',') { expression++; m_Tokens.push_back(new TokenizerToken(Token::COMMA)); }
        else if (*expression == L'-') { expression++; m_Tokens.push_back(new TokenizerToken(Token::MINUS)); }
        else if (*expression == L'+') { expression++; m_Tokens.push_back(new TokenizerToken(Token::PLUS)); }
        else if (*expression == L'*') { expression++; m_Tokens.push_back(new TokenizerToken(Token::MULTIPLY)); }
        else if (*expression == L'/') { expression++; m_Tokens.push_back(new TokenizerToken(Token::DIVIDE)); }
        else if (iswdigit(*expression)) { m_Tokens.push_back(new TokenizerToken(Token::NUMBER, ParseDigit(expression))); }
        else if (*expression == L'"') { m_Tokens.push_back(new TokenizerToken(Token::VARIABLE, ParseQuotedVariable(expression))); }
        else if (iswalpha(*expression)) { m_Tokens.push_back(new TokenizerToken(Token::VARIABLE, ParseVariable(expression))); }
        else if (iswspace(*expression)) { expression++; }
        else {
            //error...
        }
    }
}

bool Tokenizer::Match(Token t) {
    if (m_Current < m_Tokens.size() && m_Tokens[m_Current]->GetTokenType() == t) {
        m_Current++;
        return true;
    }
    return false;
}

bool Tokenizer::Check(Token t) {
    return Peek()->GetTokenType() == t;
}

TokenizerToken* Tokenizer::Previous() {
    if (m_Current <= 0) {
        return nullptr;
    }
    return m_Tokens[m_Current - 1];
}

TokenizerToken* Tokenizer::Peek() {
    return m_Tokens[m_Current];
}

void Tokenizer::Reset() {
    m_Current = 0;
}

void Tokenizer::Insert(TokenizerToken* t) {
    m_Tokens.insert(m_Tokens.begin() + m_Current, t);
}

Expression* Parser::_finishCall(Token call, Tokenizer* t) {
    std::vector<Expression*> arguments;
    while (!t->Match(Token::RIGHT_PAREN)) {
        do {
            arguments.push_back(_expression(t));
        } while (t->Match(Token::COMMA));
    }
    return new Call(call, arguments);
}

Expression* Parser::_primary(Tokenizer* t, Expression* lhs) {
    if (lhs != nullptr) { return lhs; }
    if (t->Match(Token::VARIABLE)) { return new Variable(t->Previous()->GetValue()); }
    if (t->Match(Token::NUMBER)) { return new Literal(t->Previous()->GetValue()); }
    if (t->Match(Token::_TRUE)) { return new Boolean(1); }
    if (t->Match(Token::_FALSE)) { return new Boolean(0); }
    if (t->Match(Token::LEFT_PAREN)) {
        Expression* expr = _expression(t);
        if (t->Match(Token::RIGHT_PAREN)) {
            return expr;
        }
    }
    if (t->Match(Token::STAT)
        || t->Match(Token::CLASS)
        || t->Match(Token::MIN)
        || t->Match(Token::MININ)
        || t->Match(Token::MAX)
        || t->Match(Token::CHARGEDSKILL)
        || t->Match(Token::CLASSSKILL)
        || t->Match(Token::TABSKILL)
        || t->Match(Token::SKILL)
        ) {
        Token call = t->Previous()->GetTokenType();
        if (t->Match(Token::LEFT_PAREN)) {
            return _finishCall(call, t);
        }
    }
    return nullptr;
    //throw error. one of the above should of been matched
}

Expression* Parser::_unary(Tokenizer* t, Expression* lhs) {
    if (t->Match(Token::BANG) || t->Match(Token::MINUS)) {
        TokenizerToken* op = t->Previous();
        Expression* rhs = _unary(t);
        return new Unary(rhs, op->GetTokenType());
    }
    Expression* expr = _primary(t, lhs);
    return expr;
}

Expression* Parser::_factor(Tokenizer* t, Expression* lhs) {
    Expression* expr = _unary(t, lhs);
    while (t->Match(Token::MULTIPLY) || t->Match(Token::DIVIDE)) {
        TokenizerToken* op = t->Previous();
        Expression* rhs = _unary(t);
        expr = new Binary(expr, rhs, op->GetTokenType());
    }
    return expr;
}

Expression* Parser::_term(Tokenizer* t, Expression* lhs) {
    Expression* expr = _factor(t, lhs);
    while (t->Match(Token::PLUS) || t->Match(Token::MINUS)) {
        TokenizerToken* op = t->Previous();
        Expression* rhs = _factor(t);
        expr = new Binary(expr, rhs, op->GetTokenType());
    }
    return expr;
}

Expression* Parser::_comparison(Tokenizer* t, Expression* lhs) {
    Expression* expr = _term(t, lhs);
    while (t->Match(Token::GREATER_THAN) || t->Match(Token::GREATER_THAN_EQUALS)
        || t->Match(Token::LESS_THAN) || t->Match(Token::LESS_THAN_EQUALS)
        || t->Match(Token::_IN)) {
        TokenizerToken* op = t->Previous();
        if (op->GetTokenType() != Token::_IN) {
            Expression* rhs = _term(t);
            expr = new Logical(expr, rhs, op->GetTokenType());
        }
        else {
            Expression* min = _primary(t);
            if (t->Match(Token::MINUS)) {
                Expression* max = _primary(t);
                expr = new In(expr, min, max, op->GetTokenType());
            }
            else {
                //throw error...
            }
        }
    }
    return expr;
}

Expression* Parser::_equality(Tokenizer* t, Expression* lhs) {
    Expression* expr = _comparison(t, lhs);
    while (t->Match(Token::EQUALS) || t->Match(Token::BANG_EQUALS)) {
        TokenizerToken* op = t->Previous();
        Expression* rhs = _comparison(t);
        expr = new Logical(expr, rhs, op->GetTokenType());
    }
    return expr;
}

Expression* Parser::_and(Tokenizer* t, Expression* lhs) {
    Expression* expr = _equality(t, lhs);
    while (t->Match(Token::AND)) {
        TokenizerToken* op = t->Previous();
        Expression* rhs = _equality(t);
        expr = new Logical(expr, rhs, op->GetTokenType());
    }
    return expr;
}

Expression* Parser::_or(Tokenizer* t, Expression* lhs) {
    Expression* expr = _and(t, lhs);
    while (t->Match(Token::OR)) {
        TokenizerToken* op = t->Previous();
        Expression* rhs = _and(t);
        expr = new Logical(expr, rhs, op->GetTokenType());
    }
    return expr;
}

Expression* Parser::_expression(Tokenizer* t, Expression* lhs) {
    Expression* expr = _or(t, lhs);
    return expr;
}

Expression* Parser::Parse(const wchar_t* expression) {
    Tokenizer* tokenizer = new Tokenizer(expression);
    Expression* expr = _expression(tokenizer);
    return expr;
}

ListExpression* Parser::Parse(Variable* lhs, const wchar_t* expression) {
    ListExpression* list = new ListExpression();
    Tokenizer* tokenizer = new Tokenizer(expression);
    do {
        auto nextToken = tokenizer->Peek()->GetTokenType();
        if (nextToken == Token::_IN
            || nextToken == Token::EQUALS || nextToken == Token::BANG_EQUALS
            || nextToken == Token::LESS_THAN || nextToken == Token::LESS_THAN_EQUALS
            || nextToken == Token::GREATER_THAN || nextToken == Token::GREATER_THAN_EQUALS) {
        }
        else {
            tokenizer->Insert(new TokenizerToken(Token::EQUALS));
        }
        list->Push(_expression(tokenizer, lhs));
    } while (tokenizer->Match(Token::COMMA));
    return list;
}

Expression* Parser::ParseCall(Token func, const wchar_t* args) {
    std::wstring formatted = std::format(L"({})", args);
    Tokenizer* call = new Tokenizer(formatted.c_str());
    call->Insert(new TokenizerToken(func));
    return _expression(call);
}
