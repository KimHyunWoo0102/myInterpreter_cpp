#include "CRepl.h"
#include"test.h"
#include "CAst.h"

void runTestInfixExpression() {
    // Token 생성
    Token tok(PLUS, "+");

    // 왼쪽, 오른쪽 Expression 준비
    auto left = std::make_shared<IntegerLiteral>(Token(INT, "5"), 5);
    auto right = std::make_shared<IntegerLiteral>(Token(INT, "10"), 10);

    // InfixExpression 생성
    std::string op = "+";
    auto infix = std::make_shared<InfixExpression>(tok, op, left);
    infix->setRight(right);  // 오른쪽 설정

    // ExpressionStatement로 감싸기
    auto stmt = std::make_shared<ExpressionStatement>(tok);
    stmt->setExpression(infix);

    // 테스트 실행
    bool success = testInfixExpression(stmt->getExpression(), int64_t(5), "+", int64_t(10));
    std::cout << (success ? "Test passed" : "❌ Test failed") << std::endl;
}
int main() {
	/*std::cout << "Hello! This is the Monkey Programming Language!" << std::endl;
	std::cout << "Feel Free to type in commands" << std::endl;
	CRepl repl;
	repl.Start(std::cout, std::cin);*/
	//lexerTestVer3();
	//TestLetStatement();
	//TestReturnStatement();
	//TestAstString();
	//TestIdentifierExpression();
	//TestIntegerLiteralExpression();

	//TestParsingPrefixExpression();
	//TestParsingInfixExpressions();
	//TestOperatorPrecedenceParsing();
    //runTestInfixExpression();
   // TestBooleanExpression();

      //estIfExpression();
    //TestIfElseExpression();
    TestFunctionLiteralParsing();
    TestFunctionParameterParsing();
	return 0;
}