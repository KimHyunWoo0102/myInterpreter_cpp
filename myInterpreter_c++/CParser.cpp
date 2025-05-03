#include "CParser.h"
#include "CAst.h"
#include "CLexer.h"

CParser::CParser(CLexer& lexer)
	:_lexer(lexer)
{
	this->nextToken();
	this->nextToken();
}

void CParser::nextToken()
{
	this->_cur_token = this->_peek_token;
	this->_peek_token = this->_lexer.nextToken();
}

std::unique_ptr<Program> CParser::parseProgram()
{
	std::unique_ptr<Program> program = std::make_unique<Program>();
	
	while (this->_cur_token._type != EOF_TOKEN) {
		std::unique_ptr<Statement> stmt = this->parseStatement();
		if (stmt) {
			program->addStatement(std::move(stmt));
		}
		this->nextToken();
	}
	return program;
}

std::unique_ptr<Statement> CParser::parseStatement()
{
	if (this->_cur_token._type == LET) {
		return parseLetProgram();
	}
	else if (this->_cur_token._type == RETURN) {
		return parseReturnStatement();
	}
	return nullptr;
}

std::unique_ptr<LetStatement> CParser::parseLetProgram()
{
	std::unique_ptr<LetStatement> let_stmt = std::make_unique<LetStatement>(this->_cur_token);
	if (!this->expectPeek(IDENT))
		return nullptr;

	let_stmt->setName(std::make_unique<Identifier>(this->_cur_token,this->_cur_token._literal));

	if (!this->expectPeek(ASSIGN))
		return nullptr;
	if (!this->expectPeek(SEMICOLON))
		this->nextToken();//세미콜론 나올때까지 건너뜀

	return let_stmt;
}

std::unique_ptr<ReturnStatement> CParser::parseReturnStatement()
{
	std::unique_ptr<ReturnStatement> return_stmt = std::make_unique<ReturnStatement>(this->_cur_token);
	this->nextToken();

	while (!this->curTokenIs(SEMICOLON))this->nextToken();
	
	return return_stmt;
}

bool CParser::expectPeek(const TokenType& type)
{
	if (this->peekTokenIs(type)) {
		this->nextToken();
		return true;
	}
	else {
		this->peekError(type);
		return false;
	}
}

