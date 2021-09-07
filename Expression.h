#pragma once
#include <cstdint>
#include "D2Structs.h"
#include "D2Constants.h"
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>


/*
expression     -> or ;
or             -> and ( ( "or" | "||" ) and )* ;
and            -> equality ( ( "and" | "&&" ) equality )* ;
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

enum class Token : uint8_t {
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
	TABSKILL, CLASSSKILL, CHARGEDSKILL, SKILL,
	MIN, MININ, MAX,
	COMMA,

	NUMBER, VARIABLE,
	_TRUE, _FALSE,

	END
};

static const wchar_t* OPS[] = { L"", L" or ", L" and ", L"(", L")", L"!", L"!=", L"=", L">", L">=", L"<", L"<=", L" in ", L"-", L"+", L"*", L"/", L"Stat", L"Class", L"TabSkill", L"ClassSkill", L"ChargedSkill", L"Skill", L"Min", L"MinIn", L"Max", L",", L"", L"", L"True", L"False", L"" };

class Expression {
public:
	virtual int32_t Evaluate(Unit* pItem) = 0;
	virtual std::wstring ToString(Unit* pItem) = 0;
	virtual void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) = 0;
};

class Logical : public Expression {
protected:
	Token m_Operator;
	Expression* m_Left;
	Expression* m_Right;
public:
	Logical(Expression* left = nullptr, Expression* right = nullptr, Token op = Token::NONE) : m_Left(left), m_Right(right), m_Operator(op) {};
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

class Binary : public Expression {
protected:
	Token m_Operator;
	Expression* m_Left;
	Expression* m_Right;
public:
	Binary(Expression* left = nullptr, Expression* right = nullptr, Token op = Token::NONE) : m_Left(left), m_Right(right), m_Operator(op) {};
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

class In : public Expression {
protected:
	Token m_Operator;
	Expression* m_Left;
	Expression* m_Min;
	Expression* m_Max;
public:
	In(Expression* left = nullptr, Expression* min = nullptr, Expression* max = nullptr, Token op = Token::NONE) : m_Left(left), m_Min(min), m_Max(max), m_Operator(op) {};
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

class Unary : public Expression {
protected:
	Token m_Operator;
	Expression* m_Right;
public:
	Unary(Expression* right = nullptr, Token op = Token::NONE) : m_Right(right), m_Operator(op) {};
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

class Literal : public Expression {
protected:
	int32_t m_Value;
public:
	Literal(std::wstring value) { m_Value = stol(value); };
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

class Boolean : public Expression {
protected:
	int32_t m_Value;
public:
	Boolean(int32_t value) { m_Value = value; };
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

typedef int32_t(*GlobalVariableFunction)(Unit* pUnit);

class Variable : public Expression {
protected:
	std::wstring m_Variable;
	std::optional<int32_t> m_Value;
	GlobalVariableFunction m_Func;
public:
	Variable(std::wstring variable = L"");
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetValue(const int32_t v);
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

class Call : public Expression {
protected:
	Token m_Func;
	std::vector<Expression*> m_Args;

	int32_t EvaluateClass(Unit* pItem, std::vector<int32_t>& args);
	int32_t EvaluateChargedSkill(Unit* pItem, Stat stat, std::vector<int32_t>& args);
	int32_t EvaluateStat(Unit* pItem, Stat stat, std::vector<int32_t>& args);
public:
	Call(Token func, std::vector<Expression*> args) : m_Func(func), m_Args(args) {};
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

class ListExpression : public Expression {
protected:
	std::vector<Expression*> m_List;
public:
	void Push(Expression* expression) { m_List.push_back(expression); }
	int32_t Evaluate(Unit* pItem) override;
	std::wstring ToString(Unit* pItem) override;
	void SetVariables(std::unordered_map<std::wstring, int32_t>& variables) override;
};

class TokenizerToken {
protected:
	Token m_TokenType;
	std::wstring m_Value;
public:
	TokenizerToken(Token type = Token::NONE, std::wstring value = L"") : m_TokenType(type), m_Value(value) {};
	Token GetTokenType() { return m_TokenType; }
	std::wstring GetValue() { return m_Value; }
};

//https://github.com/munificent/craftinginterpreters/blob/master/java/com/craftinginterpreters/lox/Scanner.java
class Tokenizer {
protected:
	int m_Current = 0;
	std::vector<TokenizerToken*> m_Tokens;

	bool IsNull(wchar_t c);
	bool IsOperator(const wchar_t*& expression);
	std::wstring ParseVariable(const wchar_t*& expression);
	std::wstring ParseQuotedVariable(const wchar_t*& expression);
	std::wstring ParseDigit(const wchar_t*& expression);
	void Tokenize(const wchar_t*& expression);
public:
	Tokenizer(const wchar_t* expression) {
		Tokenize(expression);
	};

	std::vector<TokenizerToken*> GetTokens() { return m_Tokens; }
	bool Match(Token t);
	bool Check(Token t);
	TokenizerToken* Previous();
	TokenizerToken* Peek();
	void Reset();
	void Insert(TokenizerToken* t);
};

//https://en.wikipedia.org/wiki/Recursive_descent_parser
class Parser {
protected:
	static Expression* _finishCall(Token call, Tokenizer* t);
	static Expression* _primary(Tokenizer* t, Expression* lhs = nullptr);
	static Expression* _unary(Tokenizer* t, Expression* lhs = nullptr);
	static Expression* _factor(Tokenizer* t, Expression* lhs = nullptr);
	static Expression* _term(Tokenizer* t, Expression* lhs = nullptr);
	static Expression* _comparison(Tokenizer* t, Expression* lhs = nullptr);
	static Expression* _equality(Tokenizer* t, Expression* lhs = nullptr);
	static Expression* _and(Tokenizer* t, Expression* lhs = nullptr);
	static Expression* _or(Tokenizer* t, Expression* lhs = nullptr);
	static Expression* _expression(Tokenizer* t, Expression* lhs = nullptr);
public:
	static Expression* Parse(const wchar_t* expression);
	static ListExpression* Parse(Variable* lhs, const wchar_t* expression);
	static Expression* ParseCall(Token func, const wchar_t* args);
};
