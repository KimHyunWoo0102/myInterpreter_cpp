#include "CParser.h"
#include "CAst.h"
#include "CLexer.h"

// CParser Ŭ���� ������: �־��� lexer�� ����Ͽ� �ʱ�ȭ
CParser::CParser(CLexer& lexer)
	:_lexer(lexer)
{
	// ó�� �� ��ū�� �о _cur_token�� _peek_token�� ����
	this->nextToken();
	this->nextToken();

	// Prefix �Ľ� �Լ� ��� (IDENT�� INT Ÿ�Կ� ����)
	this->registerPrefix(IDENT, std::bind(&CParser::parseIdentifier, this));
	this->registerPrefix(INT, std::bind(&CParser::parseIntegerLiteral, this));

	this->registerPrefix(BANG, std::bind(& CParser::parsePrefixExpression,this));
	this->registerPrefix(MINUS, std::bind(&CParser::parsePrefixExpression,this));
}

// ���� ��ū�� �о� _cur_token�� _peek_token�� ������Ʈ
void CParser::nextToken()
{
	this->_cur_token = this->_peek_token;  // _cur_token�� _peek_token���� ����
	this->_peek_token = this->_lexer.nextToken();  // _peek_token�� lexer���� ���� �� ��ū���� ����
}

// ���α׷��� �Ľ��ϴ� �Լ�: ���α׷��� �������������� ��ū�� �а�, Statement���� �Ľ�
std::unique_ptr<Program> CParser::parseProgram()
{
	std::unique_ptr<Program> program = std::make_unique<Program>();  // ���ο� ���α׷� ����

	// EOF_TOKEN�� ���� ������ Statement���� �о�� ���α׷��� �߰�
	while (this->_cur_token._type != EOF_TOKEN) {
		std::unique_ptr<Statement> stmt = this->parseStatement();  // Statement �Ľ�
		if (stmt) {
			program->addStatement(std::move(stmt));  // �Ľ̵� Statement�� ���α׷��� �߰�
		}
		this->nextToken();  // ���� ��ū���� �̵�
	}
	return program;  // �ϼ��� ���α׷� ��ȯ
}

// Statement�� �Ľ��ϴ� �Լ�: LET, RETURN, �Ǵ� �Ϲ� ǥ���� ������ �Ľ�
std::unique_ptr<Statement> CParser::parseStatement()
{
	if (this->_cur_token._type == LET)  // LET ���� �Ľ�
		return parseLetProgram();
	else if (this->_cur_token._type == RETURN)  // RETURN ���� �Ľ�
		return parseReturnStatement();
	else  // ������ ǥ���� ���� ó��
		return parseExpressionStatement();
}

// LET ������ �Ľ��ϴ� �Լ�
std::unique_ptr<LetStatement> CParser::parseLetProgram()
{
	std::unique_ptr<LetStatement> let_stmt = std::make_unique<LetStatement>(this->_cur_token);  // LetStatement ����
	if (!this->expectPeek(IDENT))  // IDENT�� ���� ������ �Ľ� ����
		return nullptr;

	// ���� �̸� ����
	let_stmt->setName(std::make_unique<Identifier>(this->_cur_token, this->_cur_token._literal));

	// '=' ��ū�� Ȯ���ϰ�
	if (!this->expectPeek(ASSIGN))
		return nullptr;

	// �����ݷ�(;)�� Ȯ��, ������ �ǳʶٱ�
	if (!this->expectPeek(SEMICOLON))
		this->nextToken();  // �����ݷ� ���� ������ �ǳʶ�

	return let_stmt;  // ������ LetStatement ��ȯ
}

// RETURN ������ �Ľ��ϴ� �Լ�
std::unique_ptr<ReturnStatement> CParser::parseReturnStatement()
{
	std::unique_ptr<ReturnStatement> return_stmt = std::make_unique<ReturnStatement>(this->_cur_token);  // ReturnStatement ����
	this->nextToken();  // ���� ��ū���� �̵�

	// �����ݷ��� ���� ������ �̵�
	while (!this->curTokenIs(SEMICOLON))
		this->nextToken();

	return return_stmt;  // ������ ReturnStatement ��ȯ
}

// ǥ���� ������ �Ľ��ϴ� �Լ�
std::unique_ptr<ExpressionStatement> CParser::parseExpressionStatement()
{
	std::unique_ptr<ExpressionStatement> expr_stmt = std::make_unique<ExpressionStatement>(this->_cur_token);  // ExpressionStatement ����
	expr_stmt->setExpression(this->parseExpression(Precedence::LOWEST));  // ǥ���� �Ľ�

	// �����ݷ�(;)�� ������ ���� ��ū���� �̵�
	if (this->peekTokenIs(SEMICOLON))
		this->nextToken();

	return expr_stmt;  // ������ ExpressionStatement ��ȯ
}

// Identifier�� �Ľ��ϴ� �Լ�
std::unique_ptr<Expression> CParser::parseIdentifier()
{
	return std::make_unique<Identifier>(this->_cur_token, this->_cur_token._literal);  // Identifier ���� �� ��ȯ
}

// Expression�� �Ľ��ϴ� �Լ�: ������ �켱������ ���� ǥ������ �Ľ�
std::unique_ptr<Expression> CParser::parseExpression(CParser::Precedence precedence)
{
	// ���� ��ū�� �ش��ϴ� �Ľ� �Լ� ã��
	auto prefix = this->_prefix_parse_fns.find(this->_cur_token._type);

	if (prefix == this->_prefix_parse_fns.end()) {  // ��ϵ� �Լ��� ������ nullptr ��ȯ
		this->noPrefixParseFnError(this->_cur_token._type);
		return nullptr;
	}

	// �Ľ� �Լ� �����Ͽ� ǥ���� �Ľ�
	std::unique_ptr<Expression> left_expr = prefix->second();
	return left_expr;  // �Ľ̵� ǥ���� ��ȯ
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
std::unique_ptr<Expression> CParser::parseIntegerLiteral() {
	std::unique_ptr<IntegerLiteral> lit = std::make_unique<IntegerLiteral>(this->_cur_token);  // IntegerLiteral ����

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

std::unique_ptr<Expression> CParser::parsePrefixExpression()
{
	std::unique_ptr<PrefixExpression>expression = std::make_unique<PrefixExpression>(this->_cur_token, this->_cur_token._literal);
	this->nextToken();

	expression->setRight(this->parseExpression(Precedence::PREFIX));
	return expression;
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
