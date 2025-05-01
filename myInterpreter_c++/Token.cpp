#include"Token.h"



TokenType ILLEGAL="ILLEGAL";
TokenType EOF_TOKEN = "EOF";

TokenType IDENT = "IDENT";
TokenType INT = "INT";

TokenType ASSIGN = "=";
TokenType PLUS = "+";

TokenType COMMA = ",";
TokenType SEMICOLON = ";";

TokenType LPAREN = "(";
TokenType RPAREN = ")";
TokenType LBRACE = "{";
TokenType RBRACE = "}";

TokenType FUNCTION = "FUNDTION";
TokenType LET = "LET";

const std::map<std::string, TokenType> keywords{
	{ "fn", FUNCTION },
	{ "let", LET }
};

Token newToken(TokenType& type, char& ch)
{
	std::string str(1, ch);
	Token tok{type, str};
	return tok;
}

TokenType lookupIdent(std::string& ident)
{
	auto it = keywords.find(ident);
	if (it != keywords.end()) {
		return it->second;  // 키워드로 등록된 경우
	}
	return IDENT;  // 아니면 일반 식별자
}
