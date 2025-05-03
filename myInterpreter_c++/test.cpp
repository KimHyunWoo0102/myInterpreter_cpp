#include "test.h"
#include "Token.h"

void lexerTestVer1()
{
	const std::string input = "=+(){},;";

	Token expected_tokens[9] = {
		{ASSIGN, "="},
		{PLUS, "+"},
		{LPAREN, "("},
		{RPAREN, ")"},
		{LBRACE, "{"},
		{RBRACE, "}"},
		{COMMA, ","},
		{SEMICOLON, ";"},
		{EOF_TOKEN, ""}
	};

	CLexer l(input);

	for (int i = 0; i < 9; i++) {
		Token token = l.nextToken();
		if (token._type != expected_tokens[i]._type) {
			std::cout << "expected token type " << expected_tokens[i]._type << ", got " << token._type << std::endl;
		}
		if (token._literal != expected_tokens[i]._literal) {
			std::cout << "expected token value " << expected_tokens[i]._literal << ", got " << token._literal << std::endl;
		}
	}

	std::cout << "All tests passed!" << std::endl;
}

void lexerTestVer2() {
	const std::string input = "let five = 5;"
		"let ten = 10;"
		"let add = fn(x, y) {"
		"	x + y;"
		"};"
		"let result=add(ten,five);";
	Token expected_tokens[] = {
		{LET, "let"},
		{IDENT, "five"},
		{ASSIGN, "="},
		{INT, "5"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "ten"},
		{ASSIGN, "="},
		{INT, "10"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "add"},
		{ASSIGN, "="},
		{FUNCTION, "fn"},
		{LPAREN, "("},
		{IDENT, "x"},
		{COMMA, ","},
		{IDENT, "y"},
		{RPAREN, ")"},
		{LBRACE, "{"},
		{IDENT, "x"},
		{PLUS, "+"},
		{IDENT, "y"},
		{SEMICOLON, ";"},
		{RBRACE, "}"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "result"},
		{ASSIGN, "="},
		{IDENT, "add"},
		{LPAREN, "("},
		{IDENT, "ten"},
		{COMMA, ","},
		{IDENT, "five"},
		{RPAREN, ")"},
		{SEMICOLON, ";"},
		{EOF_TOKEN, ""}
	};

	int len = sizeof(expected_tokens) / sizeof(expected_tokens[0]);

	CLexer l(input);

	for (int i = 0; i < 9; i++) {
		Token token = l.nextToken();
		if (token._type != expected_tokens[i]._type) {
			std::cout << "expected token type " << expected_tokens[i]._type << ", got " << token._type << std::endl;
		}
		if (token._literal != expected_tokens[i]._literal) {
			std::cout << "expected token value " << expected_tokens[i]._literal << ", got " << token._literal << std::endl;
		}
	}

	std::cout << "All tests passed!" << std::endl;
}

void lexerTestVer3() {
	const std::string input = "let five = 5;"
		"let ten = 10;"
		"let sub = fn(x, y) {"
		"	x - y;"
		"};"
		"let result = sub(ten,five);"
		"!-/*5;"
		"5<10> 5;"
		"if (5 < 10) {"
		"	return true;"
		"} else {"
		"	return false;"
		"}"
		"10 == 10;"
		"10 != 9;";

	Token expected_tokens[] = {
		{LET, "let"},
		{IDENT, "five"},
		{ASSIGN, "="},
		{INT, "5"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "ten"},
		{ASSIGN, "="},
		{INT, "10"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "sub"},
		{ASSIGN, "="},
		{FUNCTION, "fn"},
		{LPAREN, "("},
		{IDENT, "x"},
		{COMMA, ","},
		{IDENT, "y"},
		{RPAREN, ")"},
		{LBRACE, "{"},
		{IDENT, "x"},
		{MINUS, "-"},
		{IDENT, "y"},
		{SEMICOLON, ";"},
		{RBRACE, "}"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "result"},
		{ASSIGN, "="},
		{IDENT, "sub"},
		{LPAREN, "("},
		{IDENT, "ten"},
		{COMMA, ","},
		{IDENT, "five"},
		{RPAREN, ")"},
		{SEMICOLON, ";"},
		{BANG, "!"},
		{MINUS, "-"},
		{SLASH, "/"},
		{ASTERISK, "*"},
		{INT, "5"},
		{SEMICOLON, ";"},
		{INT, "5"},
		{LT, "<"},
		{INT, "10"},
		{GT, ">"},
		{INT, "5"},
		{SEMICOLON, ";"},
		{IF, "if"},
		{LPAREN, "("},
		{INT, "5"},
		{LT, "<"},
		{INT, "10"},
		{RPAREN, ")"},
		{LBRACE, "{"},
		{RETURN, "return"},
		{TRUE, "true"},
		{SEMICOLON, ";"},
		{RBRACE, "}"},
		{ELSE, "else"},
		{LBRACE, "{"},
		{RETURN, "return"},
		{FALSE, "false"},
		{SEMICOLON, ";"},
		{RBRACE, "}"},
		{INT, "10"},
		{EQ, "=="},
		{INT, "10"},
		{SEMICOLON, ";"},
		{INT, "10"},
		{NOT_EQ, "!="},
		{INT, "9"},
		{SEMICOLON, ";"},
		{EOF_TOKEN, ""}
	};

	int len = sizeof(expected_tokens) / sizeof(expected_tokens[0]);

	CLexer l(input);

	for (int i = 0; i < len; i++) {
		Token token = l.nextToken();
		if (token._type != expected_tokens[i]._type) {
			std::cout << "expected token type " << expected_tokens[i]._type << ", got " << token._type << std::endl;
		}
		if (token._literal != expected_tokens[i]._literal) {
			std::cout << "expected token value " << expected_tokens[i]._literal << ", got " << token._literal << std::endl;
		}
	}

	std::cout << "All tests passed!" << std::endl;

}

void TestLetStatement()
{
	const std::string input=
		"let x  5;"
		"let  = 10;"
		"let 838383;";

	CLexer lexer(input);
	CParser parser(lexer);

	std::unique_ptr<Program> program = parser.parseProgram();
	checkParserErrors(parser);
	if (program == nullptr) {
		std::cout << "ParseProgram() returned nullptr" << std::endl;
		return;
	}

	if (program->getStatementsSize() != 3) {
		std::cout << "program.Statements does not contain 3 statements. got=" << program->getStatementsSize() << std::endl;
		return;
	}

	const std::string expectedIdentifier[]=
	{ "x","y", "foobar" };

	
	for (int i = 0; i < program->getStatementsSize(); i++) {
		const Statement *stmt = program->getStatement(i);
		if(!testLetStatement(stmt,expectedIdentifier[i]))return;
	}
}

bool testLetStatement(const Statement* stmt, const std::string& name) {
	if (stmt->getTokenLiteral() != "let") {
		std::cout << "stmt.TokenLiteral not 'let'. got=" << stmt->getTokenLiteral() <<" instead" << std::endl;
		return false;
	}

	const LetStatement* letStmt = dynamic_cast<const LetStatement*>(stmt);
	if (letStmt == nullptr) {
		std::cout << "stmt is not LetStatement. got=" << typeid(stmt).name() << " instead" << std::endl;
		return false;
	}
	if (letStmt->getName()->getValue()!=name) {
		std::cout << "letStmt.Name.Value not "<<name<<". got = "<<letStmt->getName()->getValue() << " instead" << std::endl;
		return false;
	}

	if (letStmt->getName()->getTokenLiteral() != name) {
		std::cout << "letStmt.Name.TokenLiteral not " << name << ". got=" << letStmt->getName()->getTokenLiteral() << std::endl;
		return false;
	}

	return true;
}

void TestReturnStatement()
{
	const std::string input=
		"return 5;"
		"return 10;"
		"return 993322;";
	CLexer lexer(input);
	CParser parser(lexer);
	std::unique_ptr<Program> program = parser.parseProgram();
	checkParserErrors(parser);

	if (program->getStatementsSize() != 3) {
		std::cout << "program.Statements does not contain 3 statements. got=" << program->getStatementsSize() << std::endl;
		return;
	}

	for (int i = 0; i < program->getStatementsSize(); i++) {
		const Statement* stmt = program->getStatement(i);
		if (stmt->getTokenLiteral() != "return") {
			std::cout << "stmt.TokenLiteral not 'return'. got=" << stmt->getTokenLiteral() << std::endl;
			return;
		}
		const ReturnStatement* returnStmt = dynamic_cast<const ReturnStatement*>(stmt);
		if (returnStmt == nullptr) {
			std::cout << "stmt is not ReturnStatement. got=" << typeid(stmt).name() << std::endl;
			return;
		}
	}
}

void TestAstString()
{
	// 토큰 준비
	Token letToken=newToken(LET, "let");
	Token identToken=newToken(IDENT, "myVar");
	Token valueToken=newToken(IDENT, "anotherVar");

	// Identifier 객체들
	auto name = std::make_unique<Identifier>(identToken, "myVar");
	auto value = std::make_unique<Identifier>(valueToken, "anotherVar");

	// LetStatement 생성
	auto letStmt = std::make_unique<LetStatement>(letToken);
	letStmt->setName(std::move(name));
	letStmt->setValue(std::move(value));

	// Program 객체에 추가
	Program program;
	program.addStatement(std::move(letStmt));

	// AST 문자열 출력
	std::cout << program.String() <<std::endl;
}

void checkParserErrors(CParser& parser)
{
	const std::vector<std::string>& errors = parser.Errors();
	if (errors.empty()) {
		return;
	}
	std::cout << "parser has " << errors.size() << " errors:" << std::endl;
	for (const auto& error : errors) {
		std::cout << "\t" << error << std::endl;
	}
}
