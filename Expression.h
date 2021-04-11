#pragma once

#include <Windows.h>
#include <cstdint>
#include <cwctype>
#include <fmt/format.h>
#include <fmt/core.h>
#include <numeric>
#include <sstream>
#include <algorithm>
#include <iostream>

/*
expression     -> equality ;
or             -> and ( ( "or" | "||" ) and )* ;
and            -> equality ( ( "or" | "||" ) equality )* ;
equality       -> comparison ( ( "!=" | "==" ) comparison )* ;
comparison     -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           -> factor ( ( "-" | "+" ) factor )* ;
factor         -> unary ( ( "/" | "*" ) unary )* ;
unary          -> ( "!" | "-" | call ) unary
			   | primary ;
primary        -> NUMBER | VARIABLE | "true" | "false" | "nil"
			   | "(" expression ")" ;
call           -> "(" expression ")" ;
*/

enum class Tokens : uint8_t {
	NONE,
	OR, AND,

	LEFT_PAREN, RIGHT_PAREN,
	BANG, BANG_EQUALS,

	EQUALS, 
	GREATER_THAN, GREATER_THAN_EQUALS,
	LESS_THAN, LESS_THAN_EQUALS,
	_IN,

	MINUS, PLUS,
	MULTIPLY, DIVIDE,

	//funcs
	STAT, CLASS,
	MIN, MININ, MAX,
	COMMA,

	NUMBER, VARIABLE, 
	_TRUE, _FALSE,

	END
};

//https://craftinginterpreters.com/parsing-expressions.html
//https://github.com/munificent/craftinginterpreters/blob/master/java/com/craftinginterpreters/lox/Parser.java
template <class T>
class Expr
{
protected:
	std::wstring GetOperation(Tokens op) {
		switch (op) {
		case Tokens::_IN: return L" in ";
		case Tokens::OR: return L" or ";
		case Tokens::AND: return L" and ";
		case Tokens::LEFT_PAREN: return L"(";
		case Tokens::RIGHT_PAREN: return L")";
		case Tokens::BANG_EQUALS: return L" != ";
		case Tokens::EQUALS: return L" == ";
		case Tokens::GREATER_THAN: return L" > ";
		case Tokens::GREATER_THAN_EQUALS: return L" >= ";
		case Tokens::LESS_THAN: return L" < ";
		case Tokens::LESS_THAN_EQUALS: return L" <= ";
		case Tokens::MINUS: return L" - ";
		case Tokens::PLUS: return L" + ";
		case Tokens::MULTIPLY: return L" * ";
		case Tokens::DIVIDE: return L" / ";
		case Tokens::_TRUE: return L"True";
		case Tokens::_FALSE: return L"False";
		case Tokens::STAT: return L"Stat";
		case Tokens::CLASS: return L"Class";
		case Tokens::MIN: return L"Min";
		case Tokens::MININ: return L"MinIn";
		case Tokens::MAX: return L"Max";
		default:
			return L"ERROR";
		}
	}
public:
	virtual T Evaluate(Unit* pItem) = 0;
	virtual std::wstring ToString() = 0;
	virtual void SetVariables(std::map<std::wstring, T> variables) = 0;
};

template <class T>
class Logical : public Expr<T> {
protected:
	Tokens m_Operator;
	Expr<T>* m_Left;
	Expr<T>* m_Right;
public:
	Logical(Expr<T>* left = nullptr, Expr<T>* right = nullptr, Tokens op = Tokens::NONE) : m_Left(left), m_Right(right), m_Operator(op) {};
	std::wstring ToString() {
		return L"(" + m_Left->ToString() + Expr<T>::GetOperation(m_Operator) + m_Right->ToString() + L")";
	}
	T Evaluate(Unit* pItem) {
		switch (m_Operator) {
		case Tokens::AND: return m_Left->Evaluate(pItem) && m_Right->Evaluate(pItem);
		case Tokens::OR: return m_Left->Evaluate(pItem) || m_Right->Evaluate(pItem);
		case Tokens::EQUALS: return m_Left->Evaluate(pItem) == m_Right->Evaluate(pItem);
		case Tokens::BANG_EQUALS: return m_Left->Evaluate(pItem) != m_Right->Evaluate(pItem);
		case Tokens::GREATER_THAN: return m_Left->Evaluate(pItem) > m_Right->Evaluate(pItem);
		case Tokens::GREATER_THAN_EQUALS: return m_Left->Evaluate(pItem) >= m_Right->Evaluate(pItem);
		case Tokens::LESS_THAN: return m_Left->Evaluate(pItem) < m_Right->Evaluate(pItem);
		case Tokens::LESS_THAN_EQUALS: return m_Left->Evaluate(pItem) <= m_Right->Evaluate(pItem);
		default:
			throw L"Error";
		}
	};
	void SetVariables(std::map<std::wstring, T> variables) {
		m_Left->SetVariables(variables);
		m_Right->SetVariables(variables);
	};
};

template <class T>
class Binary : public Expr<T> {
protected:
	Tokens m_Operator;
	Expr<T>* m_Left;
	Expr<T>* m_Right;
public:
	Binary(Expr<T>* left = nullptr, Expr<T>* right = nullptr, Tokens op = Tokens::NONE) : m_Left(left), m_Right(right), m_Operator(op) {};
	std::wstring ToString() {
		return L"(" + m_Left->ToString() + Expr<T>::GetOperation(m_Operator) + m_Right->ToString() + L")";
	}
	T Evaluate(Unit* pItem) {
		switch (m_Operator) {
		case Tokens::PLUS: return m_Left->Evaluate(pItem) + m_Right->Evaluate(pItem);
		case Tokens::MINUS: return m_Left->Evaluate(pItem) - m_Right->Evaluate(pItem);
		case Tokens::MULTIPLY: return m_Left->Evaluate(pItem) * m_Right->Evaluate(pItem);
		case Tokens::DIVIDE: return m_Left->Evaluate(pItem) / m_Right->Evaluate(pItem);
		default:
			throw L"Error";
		}
	}
	void SetVariables(std::map<std::wstring, T> variables) {
		m_Left->SetVariables(variables);
		m_Right->SetVariables(variables);
	};
};

template <class T>
class In : public Expr<T> {
protected:
	Tokens m_Operator;
	Expr<T>* m_Left;
	Expr<T>* m_Min;
	Expr<T>* m_Max;
public:
	In(Expr<T>* left = nullptr, Expr<T>* min = nullptr, Expr<T>* max = nullptr, Tokens op = Tokens::NONE) : m_Left(left), m_Min(min), m_Max(max), m_Operator(op) {};
	std::wstring ToString() {
		return m_Left->ToString() + Expr<T>::GetOperation(m_Operator) + m_Min->ToString() + L"-" + m_Max->ToString();
	}
	T Evaluate(Unit* pItem) {
		T left = m_Left->Evaluate(pItem);
		return left >= m_Min->Evaluate(pItem)
			&& left <= m_Max->Evaluate(pItem);
	}
	void SetVariables(std::map<std::wstring, T> variables) {
		m_Left->SetVariables(variables);
		m_Min->SetVariables(variables);
		m_Max->SetVariables(variables);
	};
};

template <class T>
class Unary : public Expr<T> {
protected:
	Tokens m_Operator;
	Expr<T>* m_Right;
public:
	Unary(Expr<T>* right = nullptr, Tokens op = Tokens::NONE) : m_Right(right), m_Operator(op) {};
	std::wstring ToString() {
		return Expr<T>::GetOperation(m_Operator) + m_Right->ToString();
	}
	T Evaluate(Unit* pItem) {
		switch (m_Operator) {
		case Tokens::BANG: return !m_Right->Evaluate(pItem);
		case Tokens::MINUS: return -1 * m_Right->Evaluate(pItem);
		default:
			throw L"Error";
		}
	}
	void SetVariables(std::map<std::wstring, T> variables) {
		m_Right->SetVariables(variables);
	};
};

template <class T>
class Literal : public Expr<T> {
protected:
	T m_Value;
public:
	Literal(std::wstring value) {
		m_Value = stol(value, nullptr, 10);
	};
	T Evaluate(Unit* pItem) {
		return m_Value; 
	};
	std::wstring ToString() {
		return std::to_wstring(m_Value);
	};
	void SetVariables(std::map<std::wstring, T> variables) {};
};

template <class T>
class Boolean : public Expr<T> {
protected:
	T m_Value;
public:
	Boolean(T value) : m_Value(value) {};
	T Evaluate(Unit* pItem) {
		return m_Value;
	};
	std::wstring ToString() {
		return std::to_wstring(m_Value);
	};
	void SetVariables(std::map<std::wstring, T> variables) {};
};

template <class T>
class Var : public Expr<T> {
protected:
	std::wstring m_Var;
	T* m_Value = nullptr;
public:
	Var(std::wstring value) : m_Var(value) {};
	~Var() {
		delete m_Value;
	}
	T Evaluate(Unit* pItem) {
		if (m_Value == nullptr) {
			return 0;
		}
		return *m_Value; 
	}
	void SetValue(const T v) {
		if (m_Value == nullptr) {
			m_Value = reinterpret_cast<T*>(malloc(sizeof(T)));
		}
		memcpy(m_Value, &v, sizeof(T));
	}
	void SetVariables(std::map<std::wstring, T> variables) {
		if (variables.contains(m_Var)) {
			SetValue(variables[m_Var]);
		}
	}
	std::wstring ToString() {
		if (m_Value == nullptr) {
			return m_Var + L"(null)";
		}
		return m_Var + L"(" + std::to_wstring(*m_Value) + L")";
	}
};

template <class T>
class Call : public Expr<T> {
protected:
	Tokens m_Func;
	std::vector<Expr<T>*> m_Args;
public:
	Call(Tokens func, std::vector<Expr<T>*> args) : m_Func(func), m_Args(args) {};
	T Evaluate(Unit* pItem) {
		switch (m_Func) {
		case Tokens::STAT: {
			int nArgs = m_Args.size();
			if (nArgs == 0
				|| nArgs > 2) {
				//throw invalid # args error?
				return 0;
			}

			Stat stat = static_cast<Stat>(m_Args[0]->Evaluate(pItem));
			uint16_t nLayer = 0;
			if (nArgs == 2) {
				nLayer = m_Args[1]->Evaluate(pItem);
			}
			T result = D2COMMON_STATLIST_GetUnitStatUnsigned(pItem, stat, nLayer);
			if (stat <= Stat::MAXSTAMINA && stat >= Stat::MAXHP) {
				result >>= 8;
			}
			return result;
		}
		case Tokens::CLASS: {
			if (m_Args.size() != 1) {
				return 0;
			}
			ItemType itemType = static_cast<ItemType>(m_Args[0]->Evaluate(pItem));
			return D2COMMON_ITEMS_CheckItemTypeId(pItem, itemType);
		}
		case Tokens::MAX:
		case Tokens::MININ:
		case Tokens::MIN: {
			std::vector<T> values;
			std::transform(m_Args.begin(), m_Args.end(), std::back_inserter(values),
				[pItem](Expr<T>* arg) -> T { return arg->Evaluate(pItem); });
			T v = values[0];
			bool includeZeros = m_Func == Tokens::MININ;
			bool isMax = m_Func == Tokens::MAX;
			for (auto& value : values) { 
				//exclude zeros. stat doesnt exist on item (i think)
				if (value == 0 && !includeZeros) {
					continue;
				}
				if (isMax && value > v) {
					v = value;
				} else if (value < v) {
					v = value;
				}
			}
			return v;
		}
		default:
			throw L"Unknown function.";
			//throw error?
		}
	}
	void SetVariables(std::map<std::wstring, T> variables) {
		for (auto& expression : m_Args) {
			expression->SetVariables(variables);
		}
	}
	std::wstring ToString() {
		std::wostringstream os;
		for (auto& expression : m_Args) {
			if (&expression != &m_Args.front()) {
				os << ", ";
			}
			os << expression->ToString();
		}
		return Expr<T>::GetOperation(m_Func) + L"(" + os.str() + L")";
	}
};

template <class T>
class ListExpr : public Expr<T> {
protected:
	std::vector<Expr<T>*> m_List;
public:
	T Evaluate(Unit* pItem) {
		return std::any_of(m_List.begin(), m_List.end(), [pItem](Expr<T>* expression) { return expression->Evaluate(pItem); });
	}
	void SetVariables(std::map<std::wstring, T> variables) {
		for (auto& expression : m_List) {
			expression->SetVariables(variables);
		}
	}
	void Push(Expr<T>* expression) { m_List.push_back(expression); }
	std::wstring ToString() {
		std::wostringstream os;
		for (auto& expression : m_List) {
			if (&expression != &m_List.front()) {
				os << " or ";
			}
			os << expression->ToString();
		}
		return os.str();
	}
};


class Token {
protected:
	Tokens m_TokenType;
	std::wstring m_Value;
public:
	Token(Tokens type = Tokens::NONE, std::wstring value = L"") : m_TokenType(type), m_Value(value) {};
	Tokens GetTokenType() { return m_TokenType; }
	std::wstring GetValue() { return m_Value; }
};


//https://github.com/munificent/craftinginterpreters/blob/master/java/com/craftinginterpreters/lox/Scanner.java
class Tokenizer {
protected:
	std::vector<Token*> m_Tokens;
	const wchar_t* m_Expression;
	int m_Current = 0;

	bool iswnull(wint_t c) {
		return c == L'\0';
	}

	static bool isoperator(const wchar_t*& expression) {
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

	std::wstring parsevariable(const wchar_t*& expression) {
		int s = 0;
		while (!isoperator(expression) && !iswnull(*expression)) {
			expression += 1;
			s += 1;
		}
		return std::wstring(const_cast<const wchar_t*>(expression - s), s);
	}

	std::wstring parsequotedvariable(const wchar_t*& expression) {
		int s = 1;
		expression += 1;
		while (*expression != L'"' && !iswnull(*expression)) {
			expression += 1;
			s += 1;
		}
		if (*expression != L'"') {
			//throw error. unquoted string variable
		}
		expression += 1;
		return std::wstring(const_cast<const wchar_t*>(expression - s), s - 1);
	}

	std::wstring parsedigit(const wchar_t*& expression) {
		int s = 0;
		while (std::iswdigit(*expression) && !iswnull(*expression)) {
			expression += 1;
			s += 1;
		}
		return std::wstring(const_cast<const wchar_t*>(expression - s), s);
	}

	void tokenize(const wchar_t*& expression) {
		while (!iswnull(*expression)) {
			if (*expression == L'(') { expression++; m_Tokens.push_back(new Token(Tokens::LEFT_PAREN)); }
			else if (*expression == L')') { expression++; m_Tokens.push_back(new Token(Tokens::RIGHT_PAREN)); }
			else if (_wcsnicmp(expression, L"class(", 6) == 0) { expression += 5; m_Tokens.push_back(new Token(Tokens::CLASS)); }
			else if (_wcsnicmp(expression, L"minin(", 6) == 0) { expression += 5; m_Tokens.push_back(new Token(Tokens::MININ)); }
			else if (_wcsnicmp(expression, L"stat(", 5) == 0) { expression += 4; m_Tokens.push_back(new Token(Tokens::STAT)); }
			else if (_wcsnicmp(expression, L"false", 5) == 0) { expression += 5; m_Tokens.push_back(new Token(Tokens::_FALSE)); }
			else if (_wcsnicmp(expression, L"true", 4) == 0) { expression += 4; m_Tokens.push_back(new Token(Tokens::_TRUE)); }
			else if (_wcsnicmp(expression, L"min(", 4) == 0) { expression += 3; m_Tokens.push_back(new Token(Tokens::MIN)); }
			else if (_wcsnicmp(expression, L"max(", 4) == 0) { expression += 3; m_Tokens.push_back(new Token(Tokens::MAX)); }
			else if (_wcsnicmp(expression, L"and ", 4) == 0) { expression += 3; m_Tokens.push_back(new Token(Tokens::AND)); }
			else if (_wcsnicmp(expression, L"or ", 3) == 0) { expression += 2; m_Tokens.push_back(new Token(Tokens::OR)); }
			else if (wcsncmp(expression, L"&&", 2) == 0) { expression += 2; m_Tokens.push_back(new Token(Tokens::AND)); }
			else if (wcsncmp(expression, L"||", 2) == 0) { expression += 2; m_Tokens.push_back(new Token(Tokens::OR)); }
			else if (_wcsnicmp(expression, L"in ", 3) == 0) { expression += 2; m_Tokens.push_back(new Token(Tokens::_IN)); }
			else if (wcsncmp(expression, L"!=", 2) == 0) { expression += 2; m_Tokens.push_back(new Token(Tokens::BANG_EQUALS)); }
			else if (wcsncmp(expression, L"==", 2) == 0) { expression += 2; m_Tokens.push_back(new Token(Tokens::EQUALS)); }
			else if (wcsncmp(expression, L">=", 2) == 0) { expression += 2; m_Tokens.push_back(new Token(Tokens::GREATER_THAN_EQUALS)); }
			else if (wcsncmp(expression, L"<=", 2) == 0) { expression += 2; m_Tokens.push_back(new Token(Tokens::LESS_THAN_EQUALS)); }
			else if (*expression == L'>') { expression++; m_Tokens.push_back(new Token(Tokens::GREATER_THAN)); }
			else if (*expression == L'<') { expression++; m_Tokens.push_back(new Token(Tokens::LESS_THAN)); }
			else if (*expression == L'!') { expression++; m_Tokens.push_back(new Token(Tokens::BANG)); }
			else if (*expression == L'=') { expression++; m_Tokens.push_back(new Token(Tokens::EQUALS)); }
			else if (*expression == L',') { expression++; m_Tokens.push_back(new Token(Tokens::COMMA)); }
			else if (*expression == L'-') { expression++; m_Tokens.push_back(new Token(Tokens::MINUS)); }
			else if (*expression == L'+') { expression++; m_Tokens.push_back(new Token(Tokens::PLUS)); }
			else if (*expression == L'*') { expression++; m_Tokens.push_back(new Token(Tokens::MULTIPLY)); }
			else if (*expression == L'/') { expression++; m_Tokens.push_back(new Token(Tokens::DIVIDE)); }
			else if (std::iswdigit(*expression)) { m_Tokens.push_back(new Token(Tokens::NUMBER, parsedigit(expression))); }
			else if (*expression == L'"') { m_Tokens.push_back(new Token(Tokens::VARIABLE, parsequotedvariable(expression))); }
			else if (std::iswalpha(*expression)) { m_Tokens.push_back(new Token(Tokens::VARIABLE, parsevariable(expression))); }
			else if (std::iswspace(*expression)) { expression++; }
			else {
				//error? 
			}
		}
		m_Tokens.push_back(new Token(Tokens::END));
	};
public:
	Tokenizer(const wchar_t* expression) : m_Expression(expression) {
		tokenize(expression);
	};
	bool match(Tokens t) {
		if (m_Tokens[m_Current]->GetTokenType() == t) {
			m_Current++;
			return true;
		}
		return false;
	}

	bool check(Tokens t) {
		return peek() == t;
	}

	Token* previous() {
		if (m_Current <= 0) {
			return nullptr;
		}
		return m_Tokens[m_Current - 1];
	}

	Tokens peek() {
		return m_Tokens[m_Current]->GetTokenType();
	}

	void reset() {
		m_Current = 0;
	}

	void push_front(Token* t) {
		m_Tokens.insert(m_Tokens.begin(), t);
	}
};

template <class T>
class Parser {
protected:

	static Expr<T>* _finishCall(Tokens call, Tokenizer* t) {
		std::vector<Expr<T>*> arguments;
		while (!t->match(Tokens::RIGHT_PAREN)) {
			do {
				arguments.push_back(_expression(t));
			} while (t->match(Tokens::COMMA));
		}
		return new Call<T>(call, arguments);
	}

	static Expr<T>* _primary(Tokenizer* t, Expr<T>* lhs = nullptr) {
		if (lhs != nullptr) { return lhs; }
		if (t->match(Tokens::VARIABLE)) return new Var<T>(t->previous()->GetValue());
		if (t->match(Tokens::NUMBER)) return new Literal<T>(t->previous()->GetValue());
		if (t->match(Tokens::_TRUE)) return new Boolean<T>(1);
		if (t->match(Tokens::_FALSE)) return new Boolean<T>(0);
		if (t->match(Tokens::LEFT_PAREN)) {
			Expr<T>* expr = _expression(t);
			if (t->match(Tokens::RIGHT_PAREN)) {
				return expr;
			}
		}
		if(t->match(Tokens::STAT)
			|| t->match(Tokens::CLASS)
			|| t->match(Tokens::MIN)
			|| t->match(Tokens::MININ)
			|| t->match(Tokens::MAX)) {
			Tokens call = t->previous()->GetTokenType();
			if (t->match(Tokens::LEFT_PAREN)) {
				return _finishCall(call, t);
			}
		}
		//error
		DEBUG_LOG(L"Error");
		throw L"Error";
	}
	static Expr<T>* _unary(Tokenizer* t, Expr<T>* lhs = nullptr) {
		if (t->match(Tokens::BANG) || t->match(Tokens::MINUS)) {
			Token* op = t->previous();
			Expr<T>* rhs = _unary(t);
			return new Unary<T>(rhs, op->GetTokenType());
		}
		return _primary(t, lhs);
	}
	static Expr<T>* _factor(Tokenizer* t, Expr<T>* lhs = nullptr) {
		Expr<T>* expr = _unary(t, lhs);
		while (t->match(Tokens::MULTIPLY) || t->match(Tokens::DIVIDE)) {
			Token* op = t->previous();
			Expr<T>* rhs = _unary(t);
			expr = new Binary<T>(expr, rhs, op->GetTokenType());
		}
		return expr;
	}
	static Expr<T>* _term(Tokenizer* t, Expr<T>* lhs = nullptr) {
		Expr<T>* expr = _factor(t, lhs);
		while (t->match(Tokens::PLUS) || t->match(Tokens::MINUS)) {
			Token* op = t->previous();
			Expr<T>* rhs = _factor(t);
			expr = new Binary<T>(expr, rhs, op->GetTokenType());
		}
		return expr;
	}

	static Expr<T>* _comparison(Tokenizer* t, Expr<T>* lhs = nullptr) {
		Expr<T>* expr = _term(t, lhs);
		while (t->match(Tokens::GREATER_THAN) || t->match(Tokens::GREATER_THAN_EQUALS)
			|| t->match(Tokens::LESS_THAN) || t->match(Tokens::LESS_THAN_EQUALS)
			|| t->match(Tokens::_IN)) {
			Token* op = t->previous();
			if (op->GetTokenType() == Tokens::_IN) {
				Expr<T>* min = _primary(t);
				if (t->match(Tokens::MINUS)) {
					Expr<T>* max = _primary(t);
					expr = new In<T>(expr, min, max, op->GetTokenType());
				} else {
					throw L"Error";
				}
			} else {
				Expr<T>* rhs = _term(t);
				expr = new Logical<T>(expr, rhs, op->GetTokenType());
			}
		}
		return expr;
	}
	static Expr<T>* _equality(Tokenizer* t, Expr<T>* lhs = nullptr) {
		Expr<T>* expr = _comparison(t, lhs);
		while (t->match(Tokens::EQUALS) || t->match(Tokens::BANG_EQUALS)) {
			Token* op = t->previous();
			Expr<T>* rhs = _comparison(t);
			expr = new Logical<T>(expr, rhs, op->GetTokenType());
		}
		return expr;
	}
	static Expr<T>* _and(Tokenizer * t, Expr<T>* lhs = nullptr) {
		Expr<T>* expr = _equality(t, lhs);
		while (t->match(Tokens::AND)) {
			Token* op = t->previous();
			Expr<T>* rhs = _equality(t);
			expr = new Logical<T>(expr, rhs, op->GetTokenType());
		}
		return expr;
	}
	static Expr<T>* _or(Tokenizer* t, Expr<T>* lhs = nullptr) {
		Expr<T>* expr = _and(t, lhs);
		while (t->match(Tokens::OR)) {
			Token* op = t->previous();
			Expr<T>* rhs = _and(t);
			expr = new Logical<T>(expr, rhs, op->GetTokenType());
		}
		return expr;
	}
	static Expr<T>* _expression(Tokenizer* t, Expr<T>* lhs = nullptr) {
		return _or(t, lhs);
	}
public:
	//https://en.wikipedia.org/wiki/Recursive_descent_parser
	static Expr<T>* Parse(const wchar_t* expr) {
		Tokenizer* tokenizer = new Tokenizer(expr);
		return _expression(tokenizer);
	}

	static ListExpr<T>* Parse(Var<T>* lhs, const wchar_t* expressions) {
		ListExpr<T>* list = new ListExpr<T>();
		for (auto expression : split(expressions, L",")) {
			Tokenizer* tokenizer = new Tokenizer(expression.c_str());
			if (tokenizer->match(Tokens::_IN)
				|| tokenizer->match(Tokens::EQUALS) || tokenizer->match(Tokens::BANG_EQUALS)
				|| tokenizer->match(Tokens::LESS_THAN) || tokenizer->match(Tokens::LESS_THAN_EQUALS)
				|| tokenizer->match(Tokens::GREATER_THAN) || tokenizer->match(Tokens::GREATER_THAN_EQUALS)) {
			} else {
				tokenizer->push_front(new Token(Tokens::EQUALS));
			}
			tokenizer->reset();
			list->Push(_expression(tokenizer, lhs));
		}
		return list;
	};

	static ListExpr<T>* ParseCall(const wchar_t* func, const wchar_t* expressions) {
		ListExpr<T>* list = new ListExpr<T>();
		for (auto expression : split(expressions, L",")) {
			auto call = fmt::format(L"{}({})", func, expression);
			Tokenizer* tokenizer = new Tokenizer(call.c_str());
			list->Push(_expression(tokenizer));
		}
		return list;
	}
};
