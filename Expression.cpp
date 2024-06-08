#include "Expression.h"
#include <format>
#include <sstream>
#include <algorithm>
#include "D2Ptrs.h"
#include "Utils.h"
#include "ItemFilter.h"


static int32_t GetFilterLevel(Unit* pUnit) {
    return ItemFilter::FilterLevel();
}

static int32_t GetPingLevel(Unit* pUnit) {
    return ItemFilter::PingLevel();
}

static int32_t GetPlayerLevel(Unit* pUnit) {
    return D2COMMON_STATLIST_GetUnitStatUnsigned(D2CLIENT_GetPlayerUnit(), Stat::LEVEL, 0);
}

static int32_t GetItemLevel(Unit* pUnit) {
    return pUnit->pItemData->dwItemLevel;
}

static int32_t GetLevel(Unit* pUnit) {
    return GetItemsTxt(pUnit)->nLevel;
}

static int32_t GetMagicLevel(Unit* pUnit) {
    return GetItemsTxt(pUnit)->nMagicLevel;
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

static const utility::string_umap<std::wstring, GlobalVariableFunction> GlobalVariables = {
    { L"Filter Level", &GetFilterLevel },
    { L"Ping Level", &GetPingLevel },
    { L"Character Level", &GetPlayerLevel },
    { L"Item Level", &GetItemLevel },
    { L"Quality Level", &GetLevel },
    { L"Magic Level", &GetMagicLevel },
    { L"Affix Level", &GetAffixLevel },
    { L"Craft Affix Level", &GetCraftAffixLevel }
};

int32_t Logical::Evaluate(Unit* pItem) const {
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

std::wstring Logical::ToString(Unit* pItem) const {
    return std::format(L"{}{}{}", m_Left->ToString(pItem), OPS[static_cast<uint8_t>(m_Operator)], m_Right->ToString(pItem));
}

void Logical::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
    m_Left->SetVariables(variables);
    m_Right->SetVariables(variables);
}

int32_t Binary::Evaluate(Unit* pItem) const {
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

std::wstring Binary::ToString(Unit* pItem) const {
    return std::format(L"{}{}{}", m_Left->ToString(pItem), OPS[static_cast<uint8_t>(m_Operator)], m_Right->ToString(pItem));
}

void Binary::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
    m_Left->SetVariables(variables);
    m_Right->SetVariables(variables);
}

int32_t In::Evaluate(Unit* pItem) const {
    int32_t left = m_Left->Evaluate(pItem);
    int32_t min = m_Min->Evaluate(pItem);
    int32_t max = m_Max->Evaluate(pItem);
    return left >= min
        && left <= max;
}

std::wstring In::ToString(Unit* pItem) const {
    return std::format(L"{}{}{}-{}", m_Left->ToString(pItem), OPS[static_cast<uint8_t>(m_Operator)], m_Min->ToString(pItem), m_Max->ToString(pItem));
}

void In::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
    m_Left->SetVariables(variables);
    m_Min->SetVariables(variables);
    m_Max->SetVariables(variables);
}

int32_t Unary::Evaluate(Unit* pItem) const {
    int32_t right = m_Right->Evaluate(pItem);
    int32_t ret = 0;
    switch (m_Operator) {
    case Token::BANG: ret = !right; break;
    case Token::MINUS: ret = -right; break;
    default:
        break;
        //error
    }
    return ret;
}

std::wstring Unary::ToString(Unit* pItem) const {
    return std::format(L"{}{}", OPS[static_cast<uint8_t>(m_Operator)], m_Right->ToString(pItem));
}

void Unary::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
    m_Right->SetVariables(variables);
}

int32_t Literal::Evaluate(Unit* pItem) const {
    return m_Value;
}

std::wstring Literal::ToString(Unit* pItem) const {
    return std::format(L"{}", m_Value);
}

void Literal::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
}

int32_t Boolean::Evaluate(Unit* pItem) const {
    return m_Value;
}

std::wstring Boolean::ToString(Unit* pItem) const {
    return std::format(L"{}", m_Value);
}

void Boolean::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
}

Variable::Variable(std::wstring_view variable) {
    m_Variable = variable;
    if (auto search = GlobalVariables.find(m_Variable); search != GlobalVariables.end()) {
        m_Func = search->second;
    }
};

int32_t Variable::Evaluate(Unit* pItem) const {
    if (m_Func != nullptr) {
        return m_Func(pItem);
    }
    return m_Value.value_or(0);
}

std::wstring Variable::ToString(Unit* pItem) const {
    if (m_Func != nullptr) {
        return std::format(L"{}({})", m_Variable, m_Func(pItem));
    }
    if (!m_Value) {
        return std::format(L"{}(nullptr)", m_Variable);
    }
    return std::format(L"{}({})", m_Variable, m_Value.value());
}

void Variable::SetValue(int32_t v) {
    if (m_Func != nullptr) {
        return;
    }
    m_Value = v;
}

void Variable::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
    if (auto search = variables.find(m_Variable); search != variables.end()) {
        SetValue(search->second);
    }
}

int32_t Call::EvaluateClass(Unit* pItem, const std::vector<int32_t>& args) const {
    for (auto& arg : args) {
        if (D2COMMON_ITEMS_CheckItemTypeId(pItem, static_cast<ItemType>(arg))) {
            return 1;
        }
    }
    return 0;
}

int32_t Call::EvaluateChargedSkill(Unit* pItem, Stat stat, const std::vector<int32_t>& args) const {
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

int32_t Call::EvaluateStat(Unit* pItem, Stat stat, const std::vector<int32_t>& args) const {
    int32_t layer = 0;
    if (args.size() > 0) layer = args[0];
    return D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, stat, layer);
}

int32_t Call::Evaluate(Unit* pItem) const {
    std::vector<int32_t> args;
    std::ranges::transform(m_Args, std::back_inserter(args),
        [pItem](Expression* expression) { return expression->Evaluate(pItem); }, &std::unique_ptr<Expression>::get);
    switch (m_Func) {
    case Token::STAT:
    {
        int32_t statId = args[0];
        args.erase(args.begin());
        auto stat = static_cast<Stat>(statId);
        int32_t value = EvaluateStat(pItem, stat, args);
        if (stat == Stat::HITPOINTS
            || stat == Stat::MAXHP
            || stat == Stat::MANA
            || stat == Stat::MAXMANA
            || stat == Stat::STAMINA
            || stat == Stat::MAXSTAMINA
            || stat == Stat::ITEM_HP_PERLEVEL
            || stat == Stat::ITEM_MANA_PERLEVEL) {
            value = value >> 8;
        }
        return value;
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

std::wstring Call::ToString(Unit* pItem) const {
    std::wostringstream ss;
    for (auto& expression : m_Args) {
        if (&expression != &m_Args.front()) {
            ss << L", ";
        }
        ss << expression->ToString(pItem);
    }
    return std::format(L"{}({})({})", OPS[static_cast<uint8_t>(m_Func)], ss.str(), Evaluate(pItem));
}

void Call::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
    for (auto& expression : m_Args) {
        expression->SetVariables(variables);
    }
}

int32_t ListExpression::Evaluate(Unit* pItem) const {
    //return true if any are true
    return std::ranges::any_of(m_List, [pItem](Expression* expression) { return expression->Evaluate(pItem) != 0; }, &std::unique_ptr<Expression>::get);
}

std::wstring ListExpression::ToString(Unit* pItem) const {
    std::wostringstream ss;
    for (auto& expression : m_List) {
        if (&expression != &m_List.front()) {
            ss << L" or ";
        }
        ss << expression->ToString(pItem).c_str();
    }
    return ss.str();
}

void ListExpression::SetVariables(const utility::string_umap<std::wstring, int32_t>& variables) {
    for (auto& expression : m_List) {
        expression->SetVariables(variables);
    }
}

bool Tokenizer::IsNull(wchar_t c) const {
    return c == L'\0';
}

bool Tokenizer::IsOperator(const wchar_t*& expression) const {
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

std::wstring_view Tokenizer::ParseVariable(const wchar_t*& expression) const {
    int s = 0;
    while (!IsOperator(expression) && !IsNull(*expression)) {
        expression += 1;
        s += 1;
    }
    return std::wstring_view(expression - s, s);
}

std::wstring_view Tokenizer::ParseQuotedVariable(const wchar_t*& expression) const {
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
    return std::wstring_view(expression - s, s - 1);
}

std::wstring_view Tokenizer::ParseDigit(const wchar_t*& expression) const {
    int s = 0;
    while (std::isdigit(*expression) && !IsNull(*expression)) {
        expression += 1;
        s += 1;
    }
    return std::wstring_view(expression - s, s);
}

void Tokenizer::Tokenize(const wchar_t*& expression) {
    while (!IsNull(*expression)) {
        if (*expression == L'(') { expression++; m_Tokens.emplace_back(Token::LEFT_PAREN); }
        else if (*expression == L')') { expression++; m_Tokens.emplace_back(Token::RIGHT_PAREN); }
        else if (_wcsnicmp(expression, L"chargedskill(", 13) == 0) { expression += 12; m_Tokens.emplace_back(Token::CHARGEDSKILL); }
        else if (_wcsnicmp(expression, L"classskill(", 11) == 0) { expression += 10; m_Tokens.emplace_back(Token::CLASSSKILL); }
        else if (_wcsnicmp(expression, L"tabskill(", 9) == 0) { expression += 8; m_Tokens.emplace_back(Token::TABSKILL); }
        else if (_wcsnicmp(expression, L"skill(", 6) == 0) { expression += 5; m_Tokens.emplace_back(Token::SKILL); }
        else if (_wcsnicmp(expression, L"class(", 6) == 0) { expression += 5; m_Tokens.emplace_back(Token::CLASS); }
        else if (_wcsnicmp(expression, L"minin(", 6) == 0) { expression += 5; m_Tokens.emplace_back(Token::MININ); }
        else if (_wcsnicmp(expression, L"stat(", 5) == 0) { expression += 4; m_Tokens.emplace_back(Token::STAT); }
        else if (_wcsnicmp(expression, L"false", 5) == 0) { expression += 5; m_Tokens.emplace_back(Token::_FALSE); }
        else if (_wcsnicmp(expression, L"true", 4) == 0) { expression += 4; m_Tokens.emplace_back(Token::_TRUE); }
        else if (_wcsnicmp(expression, L"min(", 4) == 0) { expression += 3; m_Tokens.emplace_back(Token::MIN); }
        else if (_wcsnicmp(expression, L"max(", 4) == 0) { expression += 3; m_Tokens.emplace_back(Token::MAX); }
        else if (_wcsnicmp(expression, L"and ", 4) == 0) { expression += 3; m_Tokens.emplace_back(Token::AND); }
        else if (_wcsnicmp(expression, L"or ", 3) == 0) { expression += 2; m_Tokens.emplace_back(Token::OR); }
        else if (wcsncmp(expression, L"&&", 2) == 0) { expression += 2; m_Tokens.emplace_back(Token::AND); }
        else if (wcsncmp(expression, L"||", 2) == 0) { expression += 2; m_Tokens.emplace_back(Token::OR); }
        else if (_wcsnicmp(expression, L"in ", 3) == 0) { expression += 2; m_Tokens.emplace_back(Token::_IN); }
        else if (wcsncmp(expression, L"!=", 2) == 0) { expression += 2; m_Tokens.emplace_back(Token::BANG_EQUALS); }
        else if (wcsncmp(expression, L"==", 2) == 0) { expression += 2; m_Tokens.emplace_back(Token::EQUALS); }
        else if (wcsncmp(expression, L">=", 2) == 0) { expression += 2; m_Tokens.emplace_back(Token::GREATER_THAN_EQUALS); }
        else if (wcsncmp(expression, L"<=", 2) == 0) { expression += 2; m_Tokens.emplace_back(Token::LESS_THAN_EQUALS); }
        else if (*expression == L'>') { expression++; m_Tokens.emplace_back(Token::GREATER_THAN); }
        else if (*expression == L'<') { expression++; m_Tokens.emplace_back(Token::LESS_THAN); }
        else if (*expression == L'!') { expression++; m_Tokens.emplace_back(Token::BANG); }
        else if (*expression == L'=') { expression++; m_Tokens.emplace_back(Token::EQUALS); }
        else if (*expression == L',') { expression++; m_Tokens.emplace_back(Token::COMMA); }
        else if (*expression == L'-') { expression++; m_Tokens.emplace_back(Token::MINUS); }
        else if (*expression == L'+') { expression++; m_Tokens.emplace_back(Token::PLUS); }
        else if (*expression == L'*') { expression++; m_Tokens.emplace_back(Token::MULTIPLY); }
        else if (*expression == L'/') { expression++; m_Tokens.emplace_back(Token::DIVIDE); }
        else if (iswdigit(*expression)) { m_Tokens.emplace_back(Token::NUMBER, ParseDigit(expression)); }
        else if (*expression == L'"') { m_Tokens.emplace_back(Token::VARIABLE, ParseQuotedVariable(expression)); }
        else if (iswalpha(*expression)) { m_Tokens.emplace_back(Token::VARIABLE, ParseVariable(expression)); }
        else if (iswspace(*expression)) { expression++; }
        else {
            //error...
        }
    }
}

bool Tokenizer::Match(Token t) {
    if (m_Current < m_Tokens.size() && m_Tokens[m_Current].GetTokenType() == t) {
        m_Current++;
        return true;
    }
    return false;
}

bool Tokenizer::Check(Token t) const {
    return Peek().GetTokenType() == t;
}

const TokenizerToken& Tokenizer::Previous() const {
    return m_Tokens.at(m_Current - 1);
}

const TokenizerToken& Tokenizer::Peek() const {
    return m_Tokens.at(m_Current);
}

void Tokenizer::Reset() {
    m_Current = 0;
}

void Tokenizer::Insert(const TokenizerToken& t) {
    m_Tokens.insert(m_Tokens.begin() + m_Current, t);
}

std::unique_ptr<Expression> Parser::_finishCall(Tokenizer& t, Token call) {
    std::vector<std::unique_ptr<Expression>> arguments;
    while (!t.Match(Token::RIGHT_PAREN)) {
        do {
            arguments.push_back(std::move(_expression(t)));
        } while (t.Match(Token::COMMA));
    }
    return std::make_unique<Call>(call, std::move(arguments));
}

std::unique_ptr<Expression> Parser::_primary(Tokenizer& t, Variable* lhs) {
    if (lhs != nullptr) { return std::make_unique<VarReference>(*lhs); }
    if (t.Match(Token::VARIABLE)) { return std::make_unique<Variable>(t.Previous().GetValue()); }
    if (t.Match(Token::NUMBER)) { return std::make_unique<Literal>(t.Previous().GetValue()); }
    if (t.Match(Token::_TRUE)) { return std::make_unique<Boolean>(1); }
    if (t.Match(Token::_FALSE)) { return std::make_unique<Boolean>(0); }
    if (t.Match(Token::LEFT_PAREN)) {
        std::unique_ptr<Expression> expr = _expression(t);
        if (t.Match(Token::RIGHT_PAREN)) {
            return expr;
        }
    }
    if (t.Match(Token::STAT)
        || t.Match(Token::CLASS)
        || t.Match(Token::MIN)
        || t.Match(Token::MININ)
        || t.Match(Token::MAX)
        || t.Match(Token::CHARGEDSKILL)
        || t.Match(Token::CLASSSKILL)
        || t.Match(Token::TABSKILL)
        || t.Match(Token::SKILL)
        ) {
        Token call = t.Previous().GetTokenType();
        if (t.Match(Token::LEFT_PAREN)) {
            return _finishCall(t, call);
        }
    }
    return nullptr;
    //throw error. one of the above should of been matched
}

std::unique_ptr<Expression> Parser::_unary(Tokenizer& t, Variable* lhs) {
    if (t.Match(Token::BANG) || t.Match(Token::MINUS)) {
        const TokenizerToken& op = t.Previous();
        std::unique_ptr<Expression> rhs = _unary(t);
        return std::make_unique<Unary>(std::move(rhs), op.GetTokenType());
    }
    std::unique_ptr<Expression> expr = _primary(t, lhs);
    return expr;
}

std::unique_ptr<Expression> Parser::_factor(Tokenizer& t, Variable* lhs) {
    std::unique_ptr<Expression> expr = _unary(t, lhs);
    while (t.Match(Token::MULTIPLY) || t.Match(Token::DIVIDE)) {
        const TokenizerToken& op = t.Previous();
        std::unique_ptr<Expression> rhs = _unary(t);
        expr = std::make_unique<Binary>(std::move(expr), std::move(rhs), op.GetTokenType());
    }
    return expr;
}

std::unique_ptr<Expression> Parser::_term(Tokenizer& t, Variable* lhs) {
    std::unique_ptr<Expression> expr = _factor(t, lhs);
    while (t.Match(Token::PLUS) || t.Match(Token::MINUS)) {
        const TokenizerToken& op = t.Previous();
        std::unique_ptr<Expression> rhs = _factor(t);
        expr = std::make_unique<Binary>(std::move(expr), std::move(rhs), op.GetTokenType());
    }
    return expr;
}

std::unique_ptr<Expression> Parser::_comparison(Tokenizer& t, Variable* lhs) {
    std::unique_ptr<Expression> expr = _term(t, lhs);
    while (t.Match(Token::GREATER_THAN) || t.Match(Token::GREATER_THAN_EQUALS)
        || t.Match(Token::LESS_THAN) || t.Match(Token::LESS_THAN_EQUALS)
        || t.Match(Token::_IN)) {
        const TokenizerToken& op = t.Previous();
        if (op.GetTokenType() != Token::_IN) {
            std::unique_ptr<Expression> rhs = _term(t);
            expr = std::make_unique<Logical>(std::move(expr), std::move(rhs), op.GetTokenType());
        }
        else {
            std::unique_ptr<Expression> min = _primary(t);
            if (t.Match(Token::MINUS)) {
                std::unique_ptr<Expression> max = _primary(t);
                expr = std::make_unique<In>(std::move(expr), std::move(min), std::move(max), op.GetTokenType());
            }
            else {
                //throw error...
            }
        }
    }
    return expr;
}

std::unique_ptr<Expression> Parser::_equality(Tokenizer& t, Variable* lhs) {
    std::unique_ptr<Expression> expr = _comparison(t, lhs);
    while (t.Match(Token::EQUALS) || t.Match(Token::BANG_EQUALS)) {
        const TokenizerToken& op = t.Previous();
        std::unique_ptr<Expression> rhs = _comparison(t);
        expr = std::make_unique<Logical>(std::move(expr), std::move(rhs), op.GetTokenType());
    }
    return expr;
}

std::unique_ptr<Expression> Parser::_and(Tokenizer& t, Variable* lhs) {
    std::unique_ptr<Expression> expr = _equality(t, lhs);
    while (t.Match(Token::AND)) {
        const TokenizerToken& op = t.Previous();
        std::unique_ptr<Expression> rhs = _equality(t);
        expr = std::make_unique<Logical>(std::move(expr), std::move(rhs), op.GetTokenType());
    }
    return expr;
}

std::unique_ptr<Expression> Parser::_or(Tokenizer& t, Variable* lhs) {
    std::unique_ptr<Expression> expr = _and(t, lhs);
    while (t.Match(Token::OR)) {
        const TokenizerToken& op = t.Previous();
        std::unique_ptr<Expression> rhs = _and(t);
        expr = std::make_unique<Logical>(std::move(expr), std::move(rhs), op.GetTokenType());
    }
    return expr;
}

std::unique_ptr<Expression> Parser::_expression(Tokenizer& t, Variable* lhs) {
    std::unique_ptr<Expression> expr = _or(t, lhs);
    return expr;
}

std::unique_ptr<Expression> Parser::Parse(const wchar_t* expression) {
    Tokenizer tokenizer{ expression };
    std::unique_ptr<Expression> expr = _expression(tokenizer);
    return expr;
}

std::unique_ptr<Expression> Parser::Parse(const wchar_t* expression, Variable* lhs) {
    Tokenizer tokenizer{ expression };
    std::unique_ptr<ListExpression> list = std::make_unique<ListExpression>();
    do {
        Token nextToken = tokenizer.Peek().GetTokenType();
        if (nextToken == Token::_IN
            || nextToken == Token::EQUALS || nextToken == Token::BANG_EQUALS
            || nextToken == Token::LESS_THAN || nextToken == Token::LESS_THAN_EQUALS
            || nextToken == Token::GREATER_THAN || nextToken == Token::GREATER_THAN_EQUALS) {
        }
        else {
            tokenizer.Insert(TokenizerToken(Token::EQUALS));
        }
        list->Push(std::move(_expression(tokenizer, lhs)));
    } while (tokenizer.Match(Token::COMMA));
    return list;
}

std::unique_ptr<Expression> Parser::ParseCall(const wchar_t* args, Token func) {
    std::wstring formatted = std::format(L"({})", args);
    Tokenizer tokenizer{ formatted.c_str() };
    tokenizer.Insert(TokenizerToken(func));
    return _expression(tokenizer);
}
