#include "CParser.h"
#include "CAst.h"
#include "CLexer.h"
#include <optional>




// CParser 클래스 생성자: 주어진 lexer를 사용하여 초기화
CParser::CParser(CLexer& lexer)
	:_lexer(lexer)
{
	// 처음 두 토큰을 읽어서 _cur_token과 _peek_token을 설정
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
	// Prefix 파싱 함수 등록 (IDENT와 INT 타입에 대해)
	this->registerPrefix(IDENT, [this]() { return this->parseIdentifier(); });
	this->registerPrefix(INT, [this]() { return this->parseIntegerLiteral(); });
	this->registerPrefix(BANG, [this]() { return this->parsePrefixExpression(); });
	this->registerPrefix(MINUS, [this]() { return this->parsePrefixExpression(); });
	this->registerPrefix(TRUE, [this]() { return this->parseBoolean(); });
	this->registerPrefix(FALSE, [this]() { return this->parseBoolean(); });
	this->registerPrefix(LPAREN, [this]() { return this->parseGroupedExpression(); });
	this->registerPrefix(IF, [this]() { return this->parseIfExpression(); });
	this->registerPrefix(FUNCTION, [this]() {return this->parseFunctionLiteral(); });
	//inFix 파싱

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

// 다음 토큰을 읽어 _cur_token과 _peek_token을 업데이트
void CParser::nextToken()
{
	this->_cur_token = this->_peek_token;  // _cur_token을 _peek_token으로 갱신
	this->_peek_token = this->_lexer.nextToken();  // _peek_token을 lexer에서 읽은 새 토큰으로 갱신
}

// 프로그램을 파싱하는 함수: 프로그램의 시작점에서부터 토큰을 읽고, Statement들을 파싱
std::shared_ptr<Program> CParser::parseProgram() // return type을 shared_ptr로 변경
{
	std::shared_ptr<Program> program = std::make_shared<Program>();  // 새로운 프로그램 생성

	// EOF_TOKEN을 만날 때까지 Statement들을 읽어와 프로그램에 추가
	while (this->_cur_token._type != EOF_TOKEN) {
		std::shared_ptr<Statement> stmt = this->parseStatement();  // Statement 파싱
		if (stmt) {
			program->addStatement(std::move(stmt));  // 파싱된 Statement를 프로그램에 추가
		}
		this->nextToken();  // 다음 토큰으로 이동
	}
	return program;  // 완성된 프로그램 반환
}

// Statement를 파싱하는 함수: LET, RETURN, 또는 일반 표현식 구문을 파싱
std::shared_ptr<Statement> CParser::parseStatement()  // return type을 shared_ptr로 변경
{
	if (this->_cur_token._type == LET)  // LET 문법 파싱
		return parseLetProgram();
	else if (this->_cur_token._type == RETURN)  // RETURN 문법 파싱
		return parseReturnStatement();
	else  // 나머지 표현식 구문 처리
		return parseExpressionStatement();
}

// LET 문법을 파싱하는 함수
std::shared_ptr<LetStatement> CParser::parseLetProgram() // return type을 shared_ptr로 변경
{
	std::shared_ptr<LetStatement> let_stmt = std::make_shared<LetStatement>(this->_cur_token);  // LetStatement 생성
	if (!this->expectPeek(IDENT))  // IDENT가 오지 않으면 파싱 실패
		return nullptr;

	// 변수 이름 설정
	let_stmt->setName(std::make_shared<Identifier>(this->_cur_token, this->_cur_token._literal));

	// '=' 토큰을 확인하고
	if (!this->expectPeek(ASSIGN))
		return nullptr;

	this->nextToken();

	let_stmt->setValue(this->parseExpression(Precedence::LOWEST));

	// 세미콜론(;)을 확인, 없으면 건너뛰기
	if (!this->expectPeek(SEMICOLON))
		this->nextToken();  // 세미콜론 나올 때까지 건너뜀

	return let_stmt;  // 생성된 LetStatement 반환
}

// RETURN 문법을 파싱하는 함수
std::shared_ptr<ReturnStatement> CParser::parseReturnStatement() // return type을 shared_ptr로 변경
{
	std::shared_ptr<ReturnStatement> return_stmt = std::make_shared<ReturnStatement>(this->_cur_token);  // ReturnStatement 생성
	this->nextToken();  // 다음 토큰으로 이동

	return_stmt->setReturnValue(this->parseExpression(Precedence::LOWEST));
	// 세미콜론을 만날 때까지 이동
	while (!this->curTokenIs(SEMICOLON))
		this->nextToken();

	return return_stmt;  // 생성된 ReturnStatement 반환
}

// 표현식 문법을 파싱하는 함수
std::shared_ptr<ExpressionStatement> CParser::parseExpressionStatement() // return type을 shared_ptr로 변경
{
	
	std::shared_ptr<ExpressionStatement> expr_stmt = std::make_shared<ExpressionStatement>(this->_cur_token);  // ExpressionStatement 생성
	expr_stmt->setExpression(this->parseExpression(Precedence::LOWEST));  // 표현식 파싱

	// 세미콜론(;)이 있으면 다음 토큰으로 이동
	if (this->peekTokenIs(SEMICOLON))
		this->nextToken();

	return expr_stmt;  // 생성된 ExpressionStatement 반환
}

// Identifier를 파싱하는 함수
std::shared_ptr<Expression> CParser::parseIdentifier() // return type을 shared_ptr로 변경
{
	return std::make_shared<Identifier>(this->_cur_token, this->_cur_token._literal);  // Identifier 생성 후 반환
}

// Expression을 파싱하는 함수: 연산자 우선순위에 맞춰 표현식을 파싱
std::shared_ptr<Expression> CParser::parseExpression(CParser::Precedence precedence) // return type을 shared_ptr로 변경
{
	// 현재 토큰에 해당하는 파싱 함수 찾기
	auto it = this->_prefix_parse_fns.find(this->_cur_token._type);

	if (it == this->_prefix_parse_fns.end()) {  // 등록된 함수가 없으면 nullptr 반환
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
	// 파싱 함수 실행하여 표현식 파싱
	return left_exp;  // 파싱된 표현식 반환
}

// 문자열을 64비트 정수로 안전하게 변환하는 함수
int64_t safeParseInt64(const std::string& str) {
	char* end;
	errno = 0;  // errno를 0으로 초기화
	int64_t value = std::strtoll(str.c_str(), &end, 10);  // 문자열을 64비트 정수로 변환

	// 변환 실패 또는 범위 오류 처리
	if (end == str.c_str() || errno == ERANGE) {
		return 0;  // 변환 실패 시 0 반환
	}

	return value;  // 변환된 값 반환
}

// 정수 리터럴을 파싱하는 함수
std::shared_ptr<Expression> CParser::parseIntegerLiteral() // return type을 shared_ptr로 변경
{
	std::shared_ptr<IntegerLiteral> lit = std::make_shared<IntegerLiteral>(this->_cur_token);  // IntegerLiteral 생성

	auto value = safeParseInt64(this->_cur_token._literal);  // 문자열을 정수로 변환

	// 변환 실패 시 에러 메시지 추가
	if (value == 0 && this->_cur_token._literal != "0") {
		const std::string msg = "could not parse " + this->_cur_token._literal + " as integer";
		this->errors.push_back(msg);  // 에러 목록에 추가
		return nullptr;  // 실패 시 nullptr 반환
	}

	lit->setValue(value);  // IntegerLiteral의 값을 설정
	return lit;  // 생성된 IntegerLiteral 반환
}

std::shared_ptr<Expression> CParser::parseBoolean()
{
	return std::make_shared<Boolean>(this->_cur_token,this->curTokenIs(TRUE));
}

std::shared_ptr<Expression> CParser::parsePrefixExpression() // return type을 shared_ptr로 변경
{
	std::shared_ptr<PrefixExpression> expression = std::make_shared<PrefixExpression>(this->_cur_token, this->_cur_token._literal);
	this->nextToken();

	expression->setRight(this->parseExpression(Precedence::PREFIX));
	return expression;
}

std::shared_ptr<Expression> CParser::parseInfixExpression(std::shared_ptr<Expression> left) // return type을 shared_ptr로 변경
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
		return std::nullopt;  // 파라미터가 없을 경우 std::nullopt 반환
	}

	this->nextToken(); // '('를 넘김

	// 첫 번째 파라미터 추가
	auto ident = std::make_shared<Identifier>(this->_cur_token, this->_cur_token._literal);
	identifiers.push_back(ident);

	while (this->peekTokenIs(COMMA)) {
		this->nextToken(); // ',' 넘김
		this->nextToken(); // 파라미터 이름 받음
		ident = std::make_shared<Identifier>(this->_cur_token, this->_cur_token._literal);
		identifiers.push_back(ident);
	}

	if (!this->expectPeek(RPAREN)) {
		return std::nullopt;  // RPAREN을 못받았으면 파싱 실패
	}

	return identifiers;  // 파라미터가 정상적으로 파싱되었으면 반환
}


void CParser::noPrefixParseFnError(const TokenType& type)
{
	std::string msg = "no prefix parse function for " + type + " found";
	this->errors.push_back(msg);
}

// 특정 타입의 토큰을 기대하는 함수
bool CParser::expectPeek(const TokenType& type)
{
	if (this->peekTokenIs(type)) {  // _peek_token이 기대하는 타입이면
		this->nextToken();  // 다음 토큰으로 이동
		return true;
	}
	else {
		this->peekError(type);  // 오류 처리
		return false;
	}
}

const CParser::Precedence CParser::peekPrecedence() const
{
	auto it = precedences.find(this->_peek_token._type);

	if (it != precedences.end())//찾았으면
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
