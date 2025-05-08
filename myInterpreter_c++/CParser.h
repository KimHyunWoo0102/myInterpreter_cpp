#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include<map>

#include "Token.h"
#include "CLexer.h"
#include "CAst.h"

/*
	CParser�� Monkey Language�� ���� �м��⸦ ��Ÿ���ϴ�.
	�� Ŭ������ CLexer�� ����Ͽ� ��ū�� �а�, ���� Ʈ���� �����ϴ� ����� �����մϴ�.

	Let Statement:
			let <identifier> = <expression>;
	Return Statement:
			return <expression>;

*/

using prefixParseFn = std::function<std::shared_ptr<Expression>()>;
using infixParseFn = std::function<std::shared_ptr<Expression>(std::shared_ptr<Expression>)>;

class CParser
{
public:
	CParser(CLexer& lexer);
	~CParser() = default;
	
	enum class Precedence {
		LOWEST,
		EQUALS,
		LESSGREATER,
		SUM,
		PRODUCT,
		PREFIX,
		CALL,
	};

	void nextToken();
	std::shared_ptr<Program> parseProgram();
	std::shared_ptr<Statement> parseStatement();
	std::shared_ptr<LetStatement> parseLetProgram();
	std::shared_ptr<ReturnStatement> parseReturnStatement();
	std::shared_ptr<ExpressionStatement> parseExpressionStatement();
	std::shared_ptr<Expression> parseIdentifier();
	std::shared_ptr<Expression> parseExpression(CParser::Precedence precedence);
	std::shared_ptr<Expression> parseIntegerLiteral();
	std::shared_ptr<Expression> parseBoolean();
	std::shared_ptr<Expression> parsePrefixExpression();
	std::shared_ptr<Expression> parseInfixExpression(std::shared_ptr<Expression> left);
	std::shared_ptr<Expression> parseGroupedExpression();
	std::shared_ptr<Expression> parseIfExpression();
	std::shared_ptr<BlockStatement> parseBlockStatement();
	std::shared_ptr<FunctionLiteral> parseFunctionLiteral();
	std::shared_ptr<Expression>parseCallExpression(std::shared_ptr<Expression> function);
	std::optional<std::vector<std::shared_ptr<Expression>>>parseCallArguments();

	std::optional<std::vector<std::shared_ptr<Identifier>>> parseFunctionParameters();

	void noPrefixParseFnError(const TokenType &token);
	bool curTokenIs(const TokenType& type) const {
		return _cur_token._type == type;
	}
	bool peekTokenIs(const TokenType& type) const {
		return _peek_token._type == type;
	}
	bool expectPeek(const TokenType& type);

	const std::vector<std::string>& Errors() const {return errors;}
	void peekError(const TokenType& type) {
		const std::string msg = "expected next token to be " + type + ", got " + this->_peek_token._type + " instead";
		this->errors.push_back(msg);
	}
	const Precedence peekPrecedence() const;
	const Precedence curPrecedence() const;

	void registerPrefix(const TokenType& type, prefixParseFn fn) {
		_prefix_parse_fns[type] = fn;
	}
	void registerInfix(const TokenType& type, infixParseFn fn) {
		_infix_parse_fns[type] = fn;
	}
private:
	CLexer& _lexer; // ���۷��������� ����� lexer
	Token _cur_token; // ���� ��ū
	Token _peek_token; // ���� ��ū
	std::vector<std::string> errors;

	std::map<TokenType, prefixParseFn>_prefix_parse_fns;
	std::map<TokenType, infixParseFn>_infix_parse_fns;
	std::map<TokenType, Precedence> precedences;
};
