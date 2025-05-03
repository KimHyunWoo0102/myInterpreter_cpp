#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "Token.h"
#include "CLexer.h"
#include "CAst.h"

/*
	CParser는 Monkey Language의 구문 분석기를 나타냅니다.
	이 클래스는 CLexer를 사용하여 토큰을 읽고, 구문 트리를 생성하는 기능을 제공합니다.

	Let Statement:
			let <identifier> = <expression>;
	Return Statement:
			return <expression>;

*/

using prefixParseFn = std::function<std::unique_ptr<Expression>()>;
using infixParseFn = std::function<std::unique_ptr<Expression>(std::unique_ptr<Expression>)>;

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
	std::unique_ptr<Program> parseProgram();
	std::unique_ptr<Statement> parseStatement();
	std::unique_ptr<LetStatement> parseLetProgram();
	std::unique_ptr<ReturnStatement> parseReturnStatement();
	std::unique_ptr<ExpressionStatement> parseExpressionStatement();
	std::unique_ptr<Expression> parseIdentifier();
	std::unique_ptr<Expression> parseExpression(CParser::Precedence precedence);
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

	void registerPrefix(const TokenType& type, prefixParseFn fn) {
		_prefix_parse_fns[type] = fn;
	}
	void registerInfix(const TokenType& type, infixParseFn fn) {
		_infix_parse_fns[type] = fn;
	}
private:
	CLexer& _lexer; // 레퍼런스형으로 선언된 lexer
	Token _cur_token; // 현재 토큰
	Token _peek_token; // 다음 토큰
	std::vector<std::string> errors;

	std::map<TokenType, prefixParseFn>_prefix_parse_fns;
	std::map<TokenType, infixParseFn>_infix_parse_fns;
};

