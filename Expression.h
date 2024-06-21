#pragma once
#include <cstdint>
#include "D2Structs.h"
#include "D2Constants.h"
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>
#include "Utils.h"


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

	NUMBER, VARIABLE, VARREFERENCE,
	_TRUE, _FALSE,

	END
};

static const wchar_t* OPS[] = { L"", L" or ", L" and ", L"(", L")", L"!", L"!=", L"=", L">", L">=", L"<", L"<=", L" in ", L"-", L"+", L"*", L"/", L"Stat", L"Class", L"TabSkill", L"ClassSkill", L"ChargedSkill", L"Skill", L"Min", L"MinIn", L"Max", L",", L"", L"", L"", L"True", L"False", L""};

class Expression {
public:
	virtual ~Expression() = default;
	virtual int32_t Evaluate(Unit* pItem) const = 0;
	virtual std::wstring ToString(Unit* pItem) const = 0;
	virtual void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) = 0;
};

class Logical : public Expression {
protected:
	Token m_Operator;
	std::unique_ptr<Expression> m_Left;
	std::unique_ptr<Expression> m_Right;
public:
	Logical(std::unique_ptr<Expression> left = nullptr, std::unique_ptr<Expression> right = nullptr, Token op = Token::NONE) : m_Left(std::move(left)), m_Right(std::move(right)), m_Operator(op) {};
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

class Binary : public Expression {
protected:
	Token m_Operator;
	std::unique_ptr<Expression> m_Left;
	std::unique_ptr<Expression> m_Right;
public:
	Binary(std::unique_ptr<Expression> left = nullptr, std::unique_ptr<Expression> right = nullptr, Token op = Token::NONE) : m_Left(std::move(left)), m_Right(std::move(right)), m_Operator(op) {};
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

class In : public Expression {
protected:
	Token m_Operator;
	std::unique_ptr<Expression> m_Left;
	std::unique_ptr<Expression> m_Min;
	std::unique_ptr<Expression> m_Max;
public:
	In(std::unique_ptr<Expression> left = nullptr, std::unique_ptr<Expression> min = nullptr, std::unique_ptr<Expression> max = nullptr, Token op = Token::NONE) : m_Left(std::move(left)), m_Min(std::move(min)), m_Max(std::move(max)), m_Operator(op) {};
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

class Unary : public Expression {
protected:
	Token m_Operator;
	std::unique_ptr<Expression> m_Right;
public:
	Unary(std::unique_ptr<Expression> right = nullptr, Token op = Token::NONE) : m_Right(std::move(right)), m_Operator(op) {};
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

class Literal : public Expression {
protected:
	int32_t m_Value;
public:
	Literal(std::wstring_view value) : m_Value(std::stoi(std::wstring(value))) {};
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

class Boolean : public Expression {
protected:
	int32_t m_Value;
public:
	Boolean(int32_t value) : m_Value(value) {};
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

typedef int32_t(*GlobalVariableFunction)(Unit* pUnit);

class Variable : public Expression {
protected:
	std::wstring m_Variable;
	std::optional<int32_t> m_Value;
	GlobalVariableFunction m_Func;
public:
	Variable(std::wstring_view variable = {});
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetValue(int32_t v);
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

class VarReference : public Expression {
protected:
	Variable& m_Variable;
public:
	VarReference(Variable& variable) : m_Variable(variable) {}
	int32_t Evaluate(Unit* pItem) const override { return m_Variable.Evaluate(pItem); }
	std::wstring ToString(Unit* pItem) const override { return m_Variable.ToString(pItem); }
	void SetValue(int32_t v) { m_Variable.SetValue(v); }
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variable) override { m_Variable.SetVariables(nLineNumber, variable); }
};

class Call : public Expression {
protected:
	Token m_Func;
	std::vector<std::unique_ptr<Expression>> m_Args;

	int32_t EvaluateClass(Unit* pItem, const std::vector<int32_t>& args) const;
	int32_t EvaluateChargedSkill(Unit* pItem, Stat stat, const std::vector<int32_t>& args) const;
	int32_t EvaluateStat(Unit* pItem, Stat stat, const std::vector<int32_t>& args) const;
public:
	Call(Token func, std::vector<std::unique_ptr<Expression>> args) : m_Func(func), m_Args(std::move(args)) {};
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

class ListExpression : public Expression {
protected:
	std::vector<std::unique_ptr<Expression>> m_List;
public:
	void Push(std::unique_ptr<Expression> expression) { m_List.push_back(std::move(expression)); }
	int32_t Evaluate(Unit* pItem) const override;
	std::wstring ToString(Unit* pItem) const override;
	void SetVariables(uint32_t nLineNumber, const utility::string_umap<std::wstring, int32_t>& variables) override;
};

class TokenizerToken {
protected:
	Token m_TokenType;
	std::wstring_view m_Value;
public:
	TokenizerToken(Token type = Token::NONE, std::wstring_view value = {}) : m_TokenType(type), m_Value(value) {};
	Token GetTokenType() const { return m_TokenType; }
	std::wstring_view GetValue() const { return m_Value; }
};

//https://github.com/munificent/craftinginterpreters/blob/master/java/com/craftinginterpreters/lox/Scanner.java
class Tokenizer {
protected:
	int m_Current = 0;
	std::vector<TokenizerToken> m_Tokens;

	bool IsNull(wchar_t c) const;
	bool IsOperator(const wchar_t*& expression) const;
	std::wstring_view ParseVariable(const wchar_t*& expression) const;
	std::wstring_view ParseQuotedVariable(const wchar_t*& expression) const;
	std::wstring_view ParseDigit(const wchar_t*& expression) const;
	void Tokenize(const wchar_t*& expression);
public:
	Tokenizer(const wchar_t* expression) {
		Tokenize(expression);
	};

	const std::vector<TokenizerToken>& GetTokens() const { return m_Tokens; }
	bool Match(Token t);
	bool Check(Token t) const;
	const TokenizerToken& Previous() const;
	const TokenizerToken& Peek() const;
	void Reset();
	void Insert(const TokenizerToken& t);
};

//https://en.wikipedia.org/wiki/Recursive_descent_parser
class Parser {
protected:
	static std::unique_ptr<Expression> _finishCall(Tokenizer& t, Token call);
	static std::unique_ptr<Expression> _primary(Tokenizer& t, Variable* lhs = nullptr);
	static std::unique_ptr<Expression> _unary(Tokenizer& t, Variable* lhs = nullptr);
	static std::unique_ptr<Expression> _factor(Tokenizer& t, Variable* lhs = nullptr);
	static std::unique_ptr<Expression> _term(Tokenizer& t, Variable* lhs = nullptr);
	static std::unique_ptr<Expression> _comparison(Tokenizer& t, Variable* lhs = nullptr);
	static std::unique_ptr<Expression> _equality(Tokenizer& t, Variable* lhs = nullptr);
	static std::unique_ptr<Expression> _and(Tokenizer& t, Variable* lhs = nullptr);
	static std::unique_ptr<Expression> _or(Tokenizer& t, Variable* lhs = nullptr);
	static std::unique_ptr<Expression> _expression(Tokenizer& t, Variable* lhs = nullptr);
public:
	static std::unique_ptr<Expression> Parse(const wchar_t* expression);
	static std::unique_ptr<Expression> Parse(const wchar_t* expression, Variable* lhs);
	static std::unique_ptr<Expression> ParseCall(const wchar_t* args, Token func);
};
