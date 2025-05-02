#include "CLexer.h"
#include<iostream>

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
//int main() {
//	//lexerTestVer1();
//	//lexerTestVer2();
//	lexerTestVer3();
//
//	return 0;
//}