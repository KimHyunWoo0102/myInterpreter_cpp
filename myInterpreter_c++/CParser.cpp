#include "CParser.h"
#include "CAst.h"
#include "CLexer.h"

// CParser 클래스 생성자: 주어진 lexer를 사용하여 초기화
CParser::CParser(CLexer& lexer)
	:_lexer(lexer)
{
	// 처음 두 토큰을 읽어서 _cur_token과 _peek_token을 설정
	this->nextToken();
	this->nextToken();

	// Prefix 파싱 함수 등록 (IDENT와 INT 타입에 대해)
	this->registerPrefix(IDENT, std::bind(&CParser::parseIdentifier, this));
	this->registerPrefix(INT, std::bind(&CParser::parseIntegerLiteral, this));

	this->registerPrefix(BANG, std::bind(& CParser::parsePrefixExpression,this));
	this->registerPrefix(MINUS, std::bind(&CParser::parsePrefixExpression,this));
}

// 다음 토큰을 읽어 _cur_token과 _peek_token을 업데이트
void CParser::nextToken()
{
	this->_cur_token = this->_peek_token;  // _cur_token을 _peek_token으로 갱신
	this->_peek_token = this->_lexer.nextToken();  // _peek_token을 lexer에서 읽은 새 토큰으로 갱신
}

// 프로그램을 파싱하는 함수: 프로그램의 시작점에서부터 토큰을 읽고, Statement들을 파싱
std::unique_ptr<Program> CParser::parseProgram()
{
	std::unique_ptr<Program> program = std::make_unique<Program>();  // 새로운 프로그램 생성

	// EOF_TOKEN을 만날 때까지 Statement들을 읽어와 프로그램에 추가
	while (this->_cur_token._type != EOF_TOKEN) {
		std::unique_ptr<Statement> stmt = this->parseStatement();  // Statement 파싱
		if (stmt) {
			program->addStatement(std::move(stmt));  // 파싱된 Statement를 프로그램에 추가
		}
		this->nextToken();  // 다음 토큰으로 이동
	}
	return program;  // 완성된 프로그램 반환
}

// Statement를 파싱하는 함수: LET, RETURN, 또는 일반 표현식 구문을 파싱
std::unique_ptr<Statement> CParser::parseStatement()
{
	if (this->_cur_token._type == LET)  // LET 문법 파싱
		return parseLetProgram();
	else if (this->_cur_token._type == RETURN)  // RETURN 문법 파싱
		return parseReturnStatement();
	else  // 나머지 표현식 구문 처리
		return parseExpressionStatement();
}

// LET 문법을 파싱하는 함수
std::unique_ptr<LetStatement> CParser::parseLetProgram()
{
	std::unique_ptr<LetStatement> let_stmt = std::make_unique<LetStatement>(this->_cur_token);  // LetStatement 생성
	if (!this->expectPeek(IDENT))  // IDENT가 오지 않으면 파싱 실패
		return nullptr;

	// 변수 이름 설정
	let_stmt->setName(std::make_unique<Identifier>(this->_cur_token, this->_cur_token._literal));

	// '=' 토큰을 확인하고
	if (!this->expectPeek(ASSIGN))
		return nullptr;

	// 세미콜론(;)을 확인, 없으면 건너뛰기
	if (!this->expectPeek(SEMICOLON))
		this->nextToken();  // 세미콜론 나올 때까지 건너뜀

	return let_stmt;  // 생성된 LetStatement 반환
}

// RETURN 문법을 파싱하는 함수
std::unique_ptr<ReturnStatement> CParser::parseReturnStatement()
{
	std::unique_ptr<ReturnStatement> return_stmt = std::make_unique<ReturnStatement>(this->_cur_token);  // ReturnStatement 생성
	this->nextToken();  // 다음 토큰으로 이동

	// 세미콜론을 만날 때까지 이동
	while (!this->curTokenIs(SEMICOLON))
		this->nextToken();

	return return_stmt;  // 생성된 ReturnStatement 반환
}

// 표현식 문법을 파싱하는 함수
std::unique_ptr<ExpressionStatement> CParser::parseExpressionStatement()
{
	std::unique_ptr<ExpressionStatement> expr_stmt = std::make_unique<ExpressionStatement>(this->_cur_token);  // ExpressionStatement 생성
	expr_stmt->setExpression(this->parseExpression(Precedence::LOWEST));  // 표현식 파싱

	// 세미콜론(;)이 있으면 다음 토큰으로 이동
	if (this->peekTokenIs(SEMICOLON))
		this->nextToken();

	return expr_stmt;  // 생성된 ExpressionStatement 반환
}

// Identifier를 파싱하는 함수
std::unique_ptr<Expression> CParser::parseIdentifier()
{
	return std::make_unique<Identifier>(this->_cur_token, this->_cur_token._literal);  // Identifier 생성 후 반환
}

// Expression을 파싱하는 함수: 연산자 우선순위에 맞춰 표현식을 파싱
std::unique_ptr<Expression> CParser::parseExpression(CParser::Precedence precedence)
{
	// 현재 토큰에 해당하는 파싱 함수 찾기
	auto prefix = this->_prefix_parse_fns.find(this->_cur_token._type);

	if (prefix == this->_prefix_parse_fns.end()) {  // 등록된 함수가 없으면 nullptr 반환
		this->noPrefixParseFnError(this->_cur_token._type);
		return nullptr;
	}

	// 파싱 함수 실행하여 표현식 파싱
	std::unique_ptr<Expression> left_expr = prefix->second();
	return left_expr;  // 파싱된 표현식 반환
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
std::unique_ptr<Expression> CParser::parseIntegerLiteral() {
	std::unique_ptr<IntegerLiteral> lit = std::make_unique<IntegerLiteral>(this->_cur_token);  // IntegerLiteral 생성

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
