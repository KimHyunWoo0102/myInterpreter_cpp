#include "CParser.h"
#include "CAst.h"
#include "CLexer.h"
#include <optional>




// CParser Ŭ���� ������: �־��� lexer�� ����Ͽ� �ʱ�ȭ
CParser::CParser(CLexer& lexer)
	:_lexer(lexer)
{
	// ó�� �� ��ū�� �о _cur_token�� _peek_token�� ����
	this->nextToken();
	this->nextToken();

	precedences = { { EQ, Precedence::EQUALS },
	{ NOT_EQ,Precedence::EQUALS },
	{ LT,Precedence::LESSGREATER },
	{ GT,Precedence::LESSGREATER },
	{ PLUS,Precedence::SUM },
	{ MINUS,Precedence::SUM },
	{ SLASH,Precedence::PRODUCT },
	{ ASTERISK,Precedence::PRODUCT },
	{LPAREN,Precedence::CALL},
	};
	// Prefix �Ľ� �Լ� ��� (IDENT�� INT Ÿ�Կ� ����)
	this->registerPrefix(IDENT, [this]() { return this->parseIdentifier(); });
	this->registerPrefix(INT, [this]() { return this->parseIntegerLiteral(); });
	this->registerPrefix(BANG, [this]() { return this->parsePrefixExpression(); });
	this->registerPrefix(MINUS, [this]() { return this->parsePrefixExpression(); });
	this->registerPrefix(TRUE, [this]() { return this->parseBoolean(); });
	this->registerPrefix(FALSE, [this]() { return this->parseBoolean(); });
	this->registerPrefix(LPAREN, [this]() { return this->parseGroupedExpression(); });
	this->registerPrefix(IF, [this]() { return this->parseIfExpression(); });
	this->registerPrefix(FUNCTION, [this]() {return this->parseFunctionLiteral(); });
	//inFix �Ľ�

	this->registerInfix(LPAREN, [this](std::shared_ptr<Expression>left) {return this->parseCallExpression(std::move(left)); });
	this->registerInfix(PLUS, [this](std::shared_ptr<Expression> left) { return this->parseInfixExpression(std::move(left)); });
	this->registerInfix(MINUS, [this](std::shared_ptr<Expression> left) { return this->parseInfixExpression(std::move(left)); });
	this->registerInfix(SLASH, [this](std::shared_ptr<Expression> left) { return this->parseInfixExpression(std::move(left)); });
	this->registerInfix(ASTERISK, [this](std::shared_ptr<Expression> left) { return this->parseInfixExpression(std::move(left)); });
	this->registerInfix(EQ, [this](std::shared_ptr<Expression> left) { return this->parseInfixExpression(std::move(left)); });
	this->registerInfix(NOT_EQ, [this](std::shared_ptr<Expression> left) { return this->parseInfixExpression(std::move(left)); });
	this->registerInfix(LT, [this](std::shared_ptr<Expression> left) { return this->parseInfixExpression(std::move(left)); });
	this->registerInfix(GT, [this](std::shared_ptr<Expression> left) { return this->parseInfixExpression(std::move(left)); });

}

// ���� ��ū�� �о� _cur_token�� _peek_token�� ������Ʈ
void CParser::nextToken()
{
	this->_cur_token = this->_peek_token;  // _cur_token�� _peek_token���� ����
	this->_peek_token = this->_lexer.nextToken();  // _peek_token�� lexer���� ���� �� ��ū���� ����
}

// ���α׷��� �Ľ��ϴ� �Լ�: ���α׷��� �������������� ��ū�� �а�, Statement���� �Ľ�
std::shared_ptr<Program> CParser::parseProgram() // return type�� shared_ptr�� ����
{
	std::shared_ptr<Program> program = std::make_shared<Program>();  // ���ο� ���α׷� ����

	// EOF_TOKEN�� ���� ������ Statement���� �о�� ���α׷��� �߰�
	while (this->_cur_token._type != EOF_TOKEN) {
		std::shared_ptr<Statement> stmt = this->parseStatement();  // Statement �Ľ�
		if (stmt) {
			program->addStatement(std::move(stmt));  // �Ľ̵� Statement�� ���α׷��� �߰�
		}
		this->nextToken();  // ���� ��ū���� �̵�
	}
	return program;  // �ϼ��� ���α׷� ��ȯ
}

// Statement�� �Ľ��ϴ� �Լ�: LET, RETURN, �Ǵ� �Ϲ� ǥ���� ������ �Ľ�
std::shared_ptr<Statement> CParser::parseStatement()  // return type�� shared_ptr�� ����
{
	if (this->_cur_token._type == LET)  // LET ���� �Ľ�
		return parseLetProgram();
	else if (this->_cur_token._type == RETURN)  // RETURN ���� �Ľ�
		return parseReturnStatement();
	else  // ������ ǥ���� ���� ó��
		return parseExpressionStatement();
}

// LET ������ �Ľ��ϴ� �Լ�
std::shared_ptr<LetStatement> CParser::parseLetProgram() // return type�� shared_ptr�� ����
{
	std::shared_ptr<LetStatement> let_stmt = std::make_shared<LetStatement>(this->_cur_token);  // LetStatement ����
	if (!this->expectPeek(IDENT))  // IDENT�� ���� ������ �Ľ� ����
		return nullptr;

	// ���� �̸� ����
	let_stmt->setName(std::make_shared<Identifier>(this->_cur_token, this->_cur_token._literal));

	// '=' ��ū�� Ȯ���ϰ�
	if (!this->expectPeek(ASSIGN))
		return nullptr;

	this->nextToken();

	let_stmt->setValue(this->parseExpression(Precedence::LOWEST));

	// �����ݷ�(;)�� Ȯ��, ������ �ǳʶٱ�
	if (!this->expectPeek(SEMICOLON))
		this->nextToken();  // �����ݷ� ���� ������ �ǳʶ�

	return let_stmt;  // ������ LetStatement ��ȯ
}

// RETURN ������ �Ľ��ϴ� �Լ�
std::shared_ptr<ReturnStatement> CParser::parseReturnStatement() // return type�� shared_ptr�� ����
{
	std::shared_ptr<ReturnStatement> return_stmt = std::make_shared<ReturnStatement>(this->_cur_token);  // ReturnStatement ����
	this->nextToken();  // ���� ��ū���� �̵�

	return_stmt->setReturnValue(this->parseExpression(Precedence::LOWEST));
	// �����ݷ��� ���� ������ �̵�
	while (!this->curTokenIs(SEMICOLON))
		this->nextToken();

	return return_stmt;  // ������ ReturnStatement ��ȯ
}

// ǥ���� ������ �Ľ��ϴ� �Լ�
std::shared_ptr<ExpressionStatement> CParser::parseExpressionStatement() // return type�� shared_ptr�� ����
{
	
	std::shared_ptr<ExpressionStatement> expr_stmt = std::make_shared<ExpressionStatement>(this->_cur_token);  // ExpressionStatement ����
	expr_stmt->setExpression(this->parseExpression(Precedence::LOWEST));  // ǥ���� �Ľ�

	// �����ݷ�(;)�� ������ ���� ��ū���� �̵�
	if (this->peekTokenIs(SEMICOLON))
		this->nextToken();

	return expr_stmt;  // ������ ExpressionStatement ��ȯ
}

// Identifier�� �Ľ��ϴ� �Լ�
std::shared_ptr<Expression> CParser::parseIdentifier() // return type�� shared_ptr�� ����
{
	return std::make_shared<Identifier>(this->_cur_token, this->_cur_token._literal);  // Identifier ���� �� ��ȯ
}

// Expression�� �Ľ��ϴ� �Լ�: ������ �켱������ ���� ǥ������ �Ľ�
std::shared_ptr<Expression> CParser::parseExpression(CParser::Precedence precedence) // return type�� shared_ptr�� ����
{
	// ���� ��ū�� �ش��ϴ� �Ľ� �Լ� ã��
	auto it = this->_prefix_parse_fns.find(this->_cur_token._type);

	if (it == this->_prefix_parse_fns.end()) {  // ��ϵ� �Լ��� ������ nullptr ��ȯ
		this->noPrefixParseFnError(this->_cur_token._type);
		return nullptr;
	}

	auto prefix = it->second;
	auto left_exp = prefix();
	while (!this->peekTokenIs(SEMICOLON) && precedence < this->peekPrecedence()) {
		auto tmp = this->_infix_parse_fns.find(this->_peek_token._type);
		if (tmp == this->_infix_parse_fns.end())
			return left_exp;

		auto infix = tmp->second;
		this->nextToken();
		left_exp = infix(std::move(left_exp));
	}
	// �Ľ� �Լ� �����Ͽ� ǥ���� �Ľ�
	return left_exp;  // �Ľ̵� ǥ���� ��ȯ
}

// ���ڿ��� 64��Ʈ ������ �����ϰ� ��ȯ�ϴ� �Լ�
int64_t safeParseInt64(const std::string& str) {
	char* end;
	errno = 0;  // errno�� 0���� �ʱ�ȭ
	int64_t value = std::strtoll(str.c_str(), &end, 10);  // ���ڿ��� 64��Ʈ ������ ��ȯ

	// ��ȯ ���� �Ǵ� ���� ���� ó��
	if (end == str.c_str() || errno == ERANGE) {
		return 0;  // ��ȯ ���� �� 0 ��ȯ
	}

	return value;  // ��ȯ�� �� ��ȯ
}

// ���� ���ͷ��� �Ľ��ϴ� �Լ�
std::shared_ptr<Expression> CParser::parseIntegerLiteral() // return type�� shared_ptr�� ����
{
	std::shared_ptr<IntegerLiteral> lit = std::make_shared<IntegerLiteral>(this->_cur_token);  // IntegerLiteral ����

	auto value = safeParseInt64(this->_cur_token._literal);  // ���ڿ��� ������ ��ȯ

	// ��ȯ ���� �� ���� �޽��� �߰�
	if (value == 0 && this->_cur_token._literal != "0") {
		const std::string msg = "could not parse " + this->_cur_token._literal + " as integer";
		this->errors.push_back(msg);  // ���� ��Ͽ� �߰�
		return nullptr;  // ���� �� nullptr ��ȯ
	}

	lit->setValue(value);  // IntegerLiteral�� ���� ����
	return lit;  // ������ IntegerLiteral ��ȯ
}

std::shared_ptr<Expression> CParser::parseBoolean()
{
	return std::make_shared<Boolean>(this->_cur_token,this->curTokenIs(TRUE));
}

std::shared_ptr<Expression> CParser::parsePrefixExpression() // return type�� shared_ptr�� ����
{
	std::shared_ptr<PrefixExpression> expression = std::make_shared<PrefixExpression>(this->_cur_token, this->_cur_token._literal);
	this->nextToken();

	expression->setRight(this->parseExpression(Precedence::PREFIX));
	return expression;
}

std::shared_ptr<Expression> CParser::parseInfixExpression(std::shared_ptr<Expression> left) // return type�� shared_ptr�� ����
{
	std::shared_ptr<InfixExpression> expression = std::make_shared<InfixExpression>(this->_cur_token, this->_cur_token._literal, left);

	auto precedence = this->curPrecedence();
	this->nextToken();
	expression->setRight(this->parseExpression(precedence));

	return expression;
}

std::shared_ptr<Expression> CParser::parseGroupedExpression()
{
	this->nextToken();

	auto exp = this->parseExpression(Precedence::LOWEST);

	if (!this->expectPeek(RPAREN))
		return nullptr;

	return exp;
}

std::shared_ptr<Expression> CParser::parseIfExpression()
{
	auto expression = std::make_shared<IfExpression>(this->_cur_token);

	if (!this->expectPeek(LPAREN))
		return nullptr;

	this->nextToken();
	expression->setCondition(this->parseExpression(Precedence::LOWEST));

	if (!this->expectPeek(RPAREN))
		return nullptr;
	if (!this->expectPeek(LBRACE))
		return nullptr;
	expression->setConsequence(this->parseBlockStatement());

	if (this->peekTokenIs(ELSE)) {
		this->nextToken();
		if (!this->expectPeek(LBRACE))
			return nullptr;

		expression->setAlternative(this->parseBlockStatement());
	}
	return expression;
}

std::shared_ptr<BlockStatement> CParser::parseBlockStatement()
{
	auto block = std::make_shared<BlockStatement>(this->_cur_token);
	this->nextToken();

	while (!this->curTokenIs(RBRACE) && !this->curTokenIs(EOF_TOKEN)) {
		auto stmt = this->parseStatement();
		if (stmt != nullptr) {
			block->addStatement(stmt);
		}
		this->nextToken();
	}
	return block;
}

std::shared_ptr<FunctionLiteral> CParser::parseFunctionLiteral()
{
	auto lit = std::make_shared<FunctionLiteral>(this->_cur_token);

	if (!this->expectPeek(LPAREN))
		return nullptr;

	lit->setParameters(this->parseFunctionParameters());
	if (!this->expectPeek(LBRACE))
		return nullptr;

	lit->setBody(this->parseBlockStatement());
	return lit;
}

std::shared_ptr<Expression> CParser::parseCallExpression(std::shared_ptr<Expression> function)
{
	std::shared_ptr<CallExpression> exp=make_shared<CallExpression>(this->_cur_token, function);
	exp->setArguments(*this->parseCallArguments());

	return exp;
}

std::optional<std::vector<std::shared_ptr<Expression>>> CParser::parseCallArguments()
{
	std::vector<std::shared_ptr<Expression>>args;

	if (this->peekTokenIs(RPAREN)) {
		this->nextToken();
		return args;
	}

	this->nextToken();
	args.push_back(this->parseExpression(Precedence::LOWEST));

	while (this->peekTokenIs(COMMA)) {
		this->nextToken();
		this->nextToken();
		args.push_back(this->parseExpression(Precedence::LOWEST));
	}

	if (!this->expectPeek(RPAREN))
		return std::nullopt;
	return args;
}

std::optional<std::vector<std::shared_ptr<Identifier>>> CParser::parseFunctionParameters()
{
	std::vector<std::shared_ptr<Identifier>> identifiers;

	if (this->peekTokenIs(RPAREN)) {
		this->nextToken();
		return std::nullopt;  // �Ķ���Ͱ� ���� ��� std::nullopt ��ȯ
	}

	this->nextToken(); // '('�� �ѱ�

	// ù ��° �Ķ���� �߰�
	auto ident = std::make_shared<Identifier>(this->_cur_token, this->_cur_token._literal);
	identifiers.push_back(ident);

	while (this->peekTokenIs(COMMA)) {
		this->nextToken(); // ',' �ѱ�
		this->nextToken(); // �Ķ���� �̸� ����
		ident = std::make_shared<Identifier>(this->_cur_token, this->_cur_token._literal);
		identifiers.push_back(ident);
	}

	if (!this->expectPeek(RPAREN)) {
		return std::nullopt;  // RPAREN�� ���޾����� �Ľ� ����
	}

	return identifiers;  // �Ķ���Ͱ� ���������� �Ľ̵Ǿ����� ��ȯ
}


void CParser::noPrefixParseFnError(const TokenType& type)
{
	std::string msg = "no prefix parse function for " + type + " found";
	this->errors.push_back(msg);
}

// Ư�� Ÿ���� ��ū�� ����ϴ� �Լ�
bool CParser::expectPeek(const TokenType& type)
{
	if (this->peekTokenIs(type)) {  // _peek_token�� ����ϴ� Ÿ���̸�
		this->nextToken();  // ���� ��ū���� �̵�
		return true;
	}
	else {
		this->peekError(type);  // ���� ó��
		return false;
	}
}

const CParser::Precedence CParser::peekPrecedence() const
{
	auto it = precedences.find(this->_peek_token._type);

	if (it != precedences.end())//ã������
		return it->second;

	return Precedence::LOWEST;
}

const CParser::Precedence CParser::curPrecedence() const
{
	auto it = precedences.find(this->_cur_token._type);

	if (it != precedences.end())
		return it->second;

	return Precedence::LOWEST;
}
