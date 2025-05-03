#pragma once
/*
	CParser�� Monkey Language�� ���� �м��⸦ ��Ÿ���ϴ�.
	�� Ŭ������ CLexer�� ����Ͽ� ��ū�� �а�, ���� Ʈ���� �����ϴ� ����� �����մϴ�.

	Let Statement:
			let <identifier> = <expression>;

*/
#include "CLexer.h"
#include "CAst.h"

class CParser
{
public:
	CParser(CLexer& lexer);
	~CParser() = default;
	
	void nextToken();
	std::unique_ptr<Program> parseProgram();
	std::unique_ptr<Statement> parseStatement();
	std::unique_ptr<LetStatement> parseLetProgram();

	bool curTokenIs(const TokenType& type) const {
		return this->_cur_token._type == type;
	}
	bool peekTokenIs(const TokenType& type) const {
		return this->_peek_token._type == type;
	}
	bool expectPeek(const TokenType& type);

	const std::vector<std::string>& Errors() const {return errors;}
	void peekError(const TokenType& type) {
		const std::string msg = "expected next token to be " + type + ", got " + this->_peek_token._type + " instead";
		this->errors.push_back(msg);
	}
private:
	CLexer& _lexer; // ���۷��������� ����� lexer
	Token _cur_token; // ���� ��ū
	Token _peek_token; // ���� ��ū
	std::vector<std::string> errors;
};

