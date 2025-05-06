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
		"let x = 5;"
		"let y = 10;"
		"let foobar = 838383;";

	CLexer lexer(input);
	CParser parser(lexer);

	std::shared_ptr<Program> program = parser.parseProgram();
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
	std::shared_ptr<Program> program = parser.parseProgram();
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

void TestIdentifierExpression() {
	const std::string input = "foobar;";
	CLexer lexer(input);
	CParser parser(lexer);
	std::shared_ptr<Program> program = parser.parseProgram();
	checkParserErrors(parser);

	if (program->getStatementsSize() != 1) {
		std::cout << "program.Statements does not contain 1 statement. got=" << program->getStatementsSize() << std::endl;
		return;
	}

   const ExpressionStatement* stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));
   if (stmt == nullptr) {
	   std::cout << "program.Statements[0] is not ExpressionStatement. got=" << typeid(program->getStatement(0)).name() << std::endl;
	   return;
   }

   const Identifier* ident = dynamic_cast<const Identifier*>(stmt->getExpression());
   if (ident == nullptr) {
	   std::cout << "stmt.Expression is not Identifier. got=" << typeid(stmt->getExpression()).name() << std::endl;
	   return;
   }

   if (ident->getValue() != "foobar") {
	   std::cout << "ident.Value not foobar. got=" << ident->getValue() << std::endl;
	   return;
   }

   if (ident->getTokenLiteral() != "foobar") {
	   std::cout << "ident.TokenLiteral not foobar. got=" << ident->getTokenLiteral() << std::endl;
	   return;
   }

   std::cout << "TestIdentifierExpression passed!" <<std:: endl;
}

void TestIntegerLiteralExpression()
{
	const std::string input = "5;";

	CLexer lexer(input);
	CParser parser(lexer);
	std::shared_ptr<Program> program = parser.parseProgram();
	checkParserErrors(parser);

	if (program->getStatementsSize() != 1) {
		std::cout << "program.Statements does not contain 1 statement. got=" << program->getStatementsSize() << std::endl;
		return;
	}

	auto stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));
	if (stmt == nullptr) {
		std::cout << "program.Statements[0] is not ExpressionStatement. got=" << typeid(program->getStatement(0)).name() << std::endl;
		return;
	}
	auto expr = stmt->getExpression();  // Expression* 를 받아옴
	auto literal = dynamic_cast<const IntegerLiteral*>(expr);
	if (literal == nullptr) {
		std::cout << "stmt.Expression is not IntegerLiteral. got= <nullPtr>"<< std::endl;
		return;
	}
	if (literal->getValue() != 5) {
		std::cout << "literal.Value not 5. got=" << literal->getValue() << std::endl;
		return;
	}
	if (literal->getTokenLiteral() != "5") {
		std::cout << "literal.TokenLiteral not 5. got=" << literal->getTokenLiteral() << std::endl;
		return;
	}
	std::cout << "TestIntegerLiteralExpression passed!" << std::endl;
}

void TestIfExpression()
{
	const std::string input = "if (x < y) { x }";
	CLexer lexer(input);
	CParser parser(lexer);

	auto program = parser.parseProgram();
	checkParserErrors(parser);

	if (program->getStatementsSize() != 1) {
		std::cout << "program.Statements does not contain 1 statement. got=" << program->getStatementsSize() << std::endl;
		return;
	}

	auto stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));
	if (stmt == nullptr) {
		std::cout << "program.Statements[0] is not ExpressionStatement. got=" << typeid(*(program->getStatement(0))).name() << std::endl;
		return;
	}

	auto exp = dynamic_cast<const IfExpression*>(stmt->getExpression());
	if (exp == nullptr) {
		std::cout << "stmt.Expression is not IfExpression. got=" << typeid(*(stmt->getExpression())).name() << std::endl;
		return;
	}

	if (!testInfixExpression(exp->getCondition(), "x", "<", "y")) {
		return;
	}

	const auto& statements = exp->getConsequence()->getStatements();
	if (statements.size() != 1) {
		std::cout << "consequence does not contain 1 statement. got=" << statements.size() << std::endl;
		return;
	}

	auto consequence_stmt = dynamic_cast<ExpressionStatement*>(statements[0].get());
	if (consequence_stmt == nullptr) {
		std::cout << "Statements[0] is not ExpressionStatement. got=" << typeid(*statements[0]).name() << std::endl;
		return;
	}

	if (!testIdentifier(consequence_stmt->getExpression(), "x")) {
		return;
	}
	
	if (exp->getAlternative() != nullptr) {
		std::cout << "exp.Alternative.Statements was not nullptr. got=" << typeid(*(exp->getAlternative())).name() << std::endl;
	}

	std::cout << "IfTest passed" << std::endl;
}

void TestIfElseExpression()
{
	const std::string input = "if (x < y) { x } else { y }";
	CLexer lexer(input);
	CParser parser(lexer);

	auto program = parser.parseProgram();
	checkParserErrors(parser);

	if (program->getStatementsSize() != 1) {
		std::cout << "program.Statements does not contain 1 statement. got=" << program->getStatementsSize() << std::endl;
		return;
	}

	auto stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));
	if (stmt == nullptr) {
		std::cout << "program.Statements[0] is not ExpressionStatement. got=" << typeid(*(program->getStatement(0))).name() << std::endl;
		return;
	}

	auto exp = dynamic_cast<const IfExpression*>(stmt->getExpression());
	if (exp == nullptr) {
		std::cout << "stmt.Expression is not IfExpression. got=" << typeid(*(stmt->getExpression())).name() << std::endl;
		return;
	}

	// if 조건문이 기대한대로 "x < y"인지 확인
	if (!testInfixExpression(exp->getCondition(), "x", "<", "y")) {
		return;
	}

	// "if" 조건이 참일 때 실행되는 문장이 1개인지 확인
	const auto& consequence_statements = exp->getConsequence()->getStatements();
	if (consequence_statements.size() != 1) {
		std::cout << "consequence does not contain 1 statement. got=" << consequence_statements.size() << std::endl;
		return;
	}

	auto consequence_stmt = dynamic_cast<ExpressionStatement*>(consequence_statements[0].get());
	if (consequence_stmt == nullptr) {
		std::cout << "consequence statement is not ExpressionStatement. got=" << typeid(*consequence_statements[0]).name() << std::endl;
		return;
	}

	if (!testIdentifier(consequence_stmt->getExpression(), "x")) {
		return;
	}

	// "else" 부분이 있는지 체크
	if (exp->getAlternative() == nullptr) {
		std::cout << "exp.Alternative was nullptr. Expected an else branch." << std::endl;
		return;
	}
	else {
		// "else" 부분이 있을 경우
		const auto& alt_statements = exp->getAlternative()->getStatements();
		if (alt_statements.size() != 1) {
			std::cout << "alternative does not contain 1 statement. got=" << alt_statements.size() << std::endl;
			return;
		}

		auto alt_stmt = dynamic_cast<ExpressionStatement*>(alt_statements[0].get());
		if (alt_stmt == nullptr) {
			std::cout << "Alternative statement is not ExpressionStatement. got=" << typeid(*alt_statements[0]).name() << std::endl;
			return;
		}

		if (!testIdentifier(alt_stmt->getExpression(), "y")) {
			return;
		}
	}

	std::cout << "If-ElseTest passed" << std::endl;
}


bool testIntegerLiteral(const Expression* il_expr, int64_t value) {
	auto integ = dynamic_cast<const IntegerLiteral*>(il_expr);

	if (integ == nullptr)
	{
		std::cout << "il not IntegerLiteral. got = " << typeid(integ).name() << std::endl;
		return false;
	}

	if (integ->getValue() != value) {
		std::cout << "integ.Value not " << value << ". got = " << integ->getValue() << std::endl;
		return false;
	}

	if (integ->getTokenLiteral() != std::to_string(value)) {
		std::cout << "integ.TokenLiteral not " << value << ". got = " << integ->getTokenLiteral() << std::endl;
		return false;
	}

	return true;
}

bool testIdentifier(const Expression* exp, std::string value)
{
	auto ident = dynamic_cast<const Identifier*>(exp);
	if (ident == nullptr) {
		std::cout << "exp not *ast.Identifier. got=" << typeid(*exp).name() << "std::endl";
		return false;
	}

	if (ident->getValue() != value) {
		std::cout << "ident.Value not " << value << ". got=" << ident->getValue() << std::endl;
		return false;
	}

	if (ident->getTokenLiteral() != value) {
		std::cout << "ident.TokenLiteral not " << value << ". got=" << ident->getTokenLiteral() << std::endl;
		return false;
	}

	return true;
}

bool testLiteralExpression(const Expression* exp, std::any expected)
{
	if (typeid(int) == expected.type() || typeid(int64_t) == expected.type()) {
		return testIntegerLiteral(exp, std::any_cast<int64_t>(expected));
	}
	else if (typeid(std::string) == expected.type() || typeid(const char*) == expected.type()) {
		// 만약 expected가 const char*이면 이를 std::string으로 변환
		std::string str_val = (typeid(const char*) == expected.type()) ? std::any_cast<const char*>(expected) : std::any_cast<std::string>(expected);
		return testIdentifier(exp, str_val);  // 변환된 값을 사용
	}
	else if (typeid(bool) == expected.type()) {
		return testBooleanLiteral(exp, std::any_cast<bool>(expected));
	}

	std::cout << "type of exp not handled. got=" << typeid(*exp).name() << std::endl;
	return false;
}


bool testInfixExpression(const Expression* exp, std::any left, std::string op, std::any right)
{
	auto op_exp = dynamic_cast<const InfixExpression*>(exp);

	// 1. 포인터가 null이면 실패
	if (op_exp == nullptr) {
		std::cout << "exp is not ast.InfixExpression. got=" << typeid(*exp).name() << std::endl;
		return false;
	}

	// 2. 연산자가 원하는 것과 다른 경우
	if (op_exp->getOperator() != op) {
		std::cout << "exp.Operator is not '" << op << "'. got='" << op_exp->getOperator() << "'" << std::endl;
		return false;
	}

	// 3. 왼쪽과 오른쪽 표현식이 기대한 값인지 테스트
	if (!testLiteralExpression(op_exp->getLeft(), left)) {
		std::cout << "Left expression test failed." << std::endl;
		return false;
	}

	if (!testLiteralExpression(op_exp->getRight(), right)) {
		std::cout << "Right expression test failed." << std::endl;
		return false;
	}

	return true;
}

bool testBooleanLiteral(const Expression* exp, bool value)
{
	auto bo = dynamic_cast<const Boolean*>(exp);
	if (bo == nullptr)
	{
		std::cout << "exp not *ast.Boolean. got=" << typeid(*exp).name() << std::endl;
		return false;
	}

	if (bo->getValue() != value) {
		std::cout << std::boolalpha;  // bool을 true/false로 출력
		std::cout << "bo.Value not " << value << ". got=" << bo->getValue() << std::endl;
		return false;
	}

	if (bo->getTokenLiteral() != (value ? "true" : "false")) {
		std::cout << "bo.TokenLiteral not " << (value ? "true" : "false")
			<< ". got=" << bo->getTokenLiteral() << std::endl;
		return false;
	}
	return true;
}


void TestParsingPrefixExpression() {
	struct TestCase {
		std::string input;
		std::string expected_operator;
		int64_t expected_value;
	};

	TestCase test_cases[] = {
		{"!5;", "!", 5},
		{"-15;", "-", 15},
		{"!true","!",true},
		{"!false","!",false }
	};

	int len = sizeof(test_cases) / sizeof(test_cases[0]);

	for (int i = 0; i < len; i++) {
		CLexer lexer(test_cases[i].input);
		CParser parser(lexer);
		std::shared_ptr<Program> program = parser.parseProgram();
		checkParserErrors(parser);

		if (program->getStatementsSize() != 1) {
			std::cout << "program.Statements does not contain 1 statement. got=" << program->getStatementsSize() << std::endl;
			return;
		}

		auto stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));
		if (stmt == nullptr) {
			std::cout << "program.Statements[0] is not ExpressionStatement. got=" << typeid(program->getStatement(0)).name() << std::endl;
			return;
		}
		auto expr = stmt->getExpression();
		auto prefix_expr = dynamic_cast<const PrefixExpression*>(expr);

		if (prefix_expr == nullptr) {
			std::cout << "stmt.Expression is not PrefixExpression. got=" << typeid(stmt->getExpression()).name() << std::endl;
			return;
		}

		if (prefix_expr->getOperator() != test_cases[i].expected_operator) {
			std::cout << "prefix_expr.Operator not " << test_cases[i].expected_operator << ". got=" << prefix_expr->getOperator() << std::endl;
			return;
		}

		if (!testIntegerLiteral(prefix_expr->getRight(), test_cases[i].expected_value)) {
			return;
		}
	}

	std::cout << "TestParsingPrefixExpression passed!" << std::endl;
}

void TestParsingInfixExpressions()
{
	struct test_case {
		std::string input;
		int64_t left_value;
		std::string op;
		int64_t right_value;
	};

	test_case infixTest[] = {
		{"5+5;",5,"+",5},
		{"5-5;",5,"-",5},
		{"5*5;",5,"*",5},
		{"5/5;",5,"/",5},
		{"5>5;",5,">",5},
		{"5<5;",5,"<",5},
		{"5==5;",5,"==",5},
		{"5!=5;",5,"!=",5},
		{"true == true",true,"==",true},
		{"true != false",true,"!=",false},
		{"false == false",false,"==",false}
	};

	int len = sizeof(infixTest) / sizeof(infixTest[0]);

	for (int i = 0; i < len; i++)
	{
		CLexer lexer(infixTest[i].input);
		CParser parser(lexer);
		std::shared_ptr<Program> program = parser.parseProgram();
		checkParserErrors(parser);

		if (program->getStatementsSize() != 1) {
			std::cout << "program.Statements does not contain 1 statements. got = " << program->getStatementsSize() << std::endl;
			return;
		}

		auto stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));

		if (stmt == nullptr) {
			std::cout << "program.Statements[0] is not ast.ExpressionStatement. got = " << typeid(stmt).name() << std::endl;
			return;
		}

		auto expr = dynamic_cast<const InfixExpression*>(stmt->getExpression());

		if (expr == nullptr) {
			std::cout << "exp is not ast.InfixExpression. got " << typeid(expr).name() << std::endl;
			return;
		}

		if (expr->getOperator() != infixTest[i].op) {
			std::cout << "exp.Operator is not " << infixTest[i].op << ". got = " << expr->getOperator() << std::endl;
			return;
		}
		if (!testIntegerLiteral(expr->getRight(), infixTest[i].right_value)) {
			return;
		}
		if (!testInfixExpression(stmt->getExpression(), infixTest[i].left_value, infixTest[i].op, infixTest[i].right_value))
			return;
	}

	std::cout << "TestParsingInfixExpressions passed!" << std::endl;
}

void TestOperatorPrecedenceParsing()
{
	struct test {
		std::string input;
		std::string expected;
	};

	test test_case[] = {
		{"true","true"},
		{"false","false"},
		{"3 > 5 == false","((3 > 5) == false)"},
		{"3 < 5 ==true","((3 < 5) == true)"},
		{"1 + (2 + 3) + 4","((1 + (2 + 3)) + 4)"},
		{"(5 + 5) * 2","((5 + 5) * 2)"},
		{"2 / (5 + 5)","(2 / (5 + 5))"},
		{"-(5 + 5)","(-(5 + 5))"},
		{"!(true == true)","(!(true == true))"}
	};


	for (auto tt : test_case) {
		CLexer lexer(tt.input);
		CParser parser(lexer);
		const std::shared_ptr <Program>program = parser.parseProgram();
		checkParserErrors(parser);

		auto actual = program->String();

		if (actual != tt.expected) {
			std::cout << "expected = " << tt.expected << ", got = " << actual << "\n";
		}
	}
}

void TestBooleanExpression()
{
	const std::string input = "false";  // 테스트할 입력 (true)

	CLexer lexer(input);  // Lexer 초기화
	CParser parser(lexer);  // Parser 초기화
	std::shared_ptr<Program> program = parser.parseProgram();  // 프로그램 파싱
	checkParserErrors(parser);  // 파싱 에러 체크

	// 프로그램에 하나의 Statement가 있어야 함
	if (program->getStatementsSize() != 1) {
		std::cout << "program.Statements does not contain 1 statement. got=" << program->getStatementsSize() << std::endl;
		return;
	}

	// 첫 번째 Statement가 ExpressionStatement여야 함
	auto stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));
	if (stmt == nullptr) {
		std::cout << "program.Statements[0] is not ExpressionStatement. got=" << typeid(program->getStatement(0)).name() << std::endl;
		return;
	}

	// Expression 가져오기
	auto expr = stmt->getExpression();

	// BooleanLiteral로 캐스팅
	auto booleanLiteral = dynamic_cast<const Boolean*>(expr);
	if (booleanLiteral == nullptr) {
		std::cout << "stmt.Expression is not BooleanLiteral. got= <nullPtr>" << std::endl;
		return;
	}

	// 불리언 값이 true인지 확인
	if (booleanLiteral->getValue() != false) {
		std::cout << "booleanLiteral.Value not true. got=" << booleanLiteral->getValue() << std::endl;
		return;
	}

	// 토큰 리터럴이 "true"인지 확인
	if (booleanLiteral->getTokenLiteral() != "false") {
		std::cout << "booleanLiteral.TokenLiteral not 'true'. got=" << booleanLiteral->getTokenLiteral() << std::endl;
		return;
	}

	std::cout << "TestBooleanLiteralExpression passed!" << std::endl;
}

void TestFunctionLiteralParsing()
{
	const std::string input{ "fn(x, y) { x + y; }" };
	CLexer lexer(input);
	CParser parser(lexer);
	auto program = parser.parseProgram();
	checkParserErrors(parser);

	if (program->getStatementsSize() != 1) {
		std::cout << "program.Statments does not contain 1 statements. got=" << program->getStatementsSize() << std::endl;
	}

	auto stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));

	if (stmt == nullptr) {
		std::cout << "program.Statement[0] is not ast.ExpressionStatement. got=" << typeid(*(program->getStatement(0))).name() << std::endl;
	}

	auto function = dynamic_cast<const FunctionLiteral*>(stmt->getExpression());
	if (function == nullptr) {
		std::cout << "stmt.Expression is not ast.ExpressionLiteral. got=" << typeid(stmt->getExpression()).name() << std::endl;
	}
	if (function->getParameters().size() != 2) {
		std::cout << "function literal parameters wrong. want 2. got=" << function->getParameters().size() << std::endl;
	}

	auto tmp = function->getParameters();
	testLiteralExpression(tmp[0].get(), "x");
	testLiteralExpression(tmp[1].get(), "y");

	auto tmps = function->getBody()->getStatements();
	if (function->getBody()->getStatementsSize() != 1) {
		std::cout << "function.Body.Statements has not 1 statements. got=" <<typeid(tmps[0]).name()<<std::endl;
	}

	auto body_stmt = dynamic_cast<const ExpressionStatement*>(tmps[0].get());

	if (body_stmt == nullptr) {
		std::cout << "function body stmt is not ast.ExpressionStatement. got=" << typeid(tmps[0]).name() << std::endl;
	}

	testInfixExpression(body_stmt->getExpression(), "x", "+", "y");

	std::cout<<"TestFunctionLiteralParsing passed"<<std::endl;
}
void TestFunctionParameterParsing()
{
	struct tests {
		std::string input;
		std::vector<std::string> expected_params;
	};

	tests test_case[] = {
	{"fn() {};",
	 {}},  // 파라미터 없음, 빈 벡터

	{"fn(x) {};",
	 {"x"}},  // x 파라미터

	{"fn(x, y, z) {};",
	 {"x", "y", "z"}},  // x, y, z 파라미터
	};

	for (auto& tt : test_case) {
		CLexer lexer(tt.input);
		CParser parser(lexer);
		auto program = parser.parseProgram();
		checkParserErrors(parser);

		auto stmt = dynamic_cast<const ExpressionStatement*>(program->getStatement(0));
		auto function = dynamic_cast<const FunctionLiteral*>(stmt->getExpression());

		if (function->getParameters().size() != tt.expected_params.size()) {
			std::cout << "length parameters wrong. want" << tt.expected_params.size() << ", got=" << function->getParameters().size() << std::endl;
		}

		for (int i = 0; i < tt.expected_params.size(); i++) {
			auto& function_params = function->getParameters();
			auto& ident = tt.expected_params[i];
			testLiteralExpression(function_params[i].get(), ident);
		}
	}

	std::cout << "TestFunctionLiteralParsing passed" << std::endl;
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
