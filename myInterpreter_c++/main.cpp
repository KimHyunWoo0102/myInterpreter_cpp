#include "CRepl.h"
#include"test.h"

int main() {
	/*std::cout << "Hello! This is the Monkey Programming Language!" << std::endl;
	std::cout << "Feel Free to type in commands" << std::endl;
	CRepl repl;
	repl.Start(std::cout, std::cin);*/

	//TestLetStatement();
	//TestReturnStatement();
	//TestAstString();
	//TestIdentifierExpression();
	//TestIntegerLiteralExpression();

	//TestParsingPrefixExpression();
	TestParsingInfixExpressions();
	return 0;
}