#include"Token.h"



TokenType ILLEGAL="ILLEGAL";
TokenType EOF_TOKEN = "EOF";

TokenType IDENT = "IDENT";
TokenType INT = "INT";
TokenType TRUE = "TRUE";
TokenType FALSE = "FALSE";
TokenType IF = "IF";
TokenType ELSE = "ELSE";
TokenType RETURN = "RETURN";

TokenType ASSIGN = "=";
TokenType PLUS = "+";
TokenType MINUS = "-";
TokenType SLASH = "/";
TokenType ASTERISK = "*";
TokenType BANG = "!";
TokenType EQ = "==";
TokenType NOT_EQ = "!=";

TokenType LT = "<";
TokenType GT = ">";

TokenType COMMA = ",";
TokenType SEMICOLON = ";";

TokenType LPAREN = "(";
TokenType RPAREN = ")";
TokenType LBRACE = "{";
TokenType RBRACE = "}";

TokenType FUNCTION = "FUNCTION";
TokenType LET = "LET";

const std::map<std::string, TokenType> keywords{
	{ "fn", FUNCTION },
	{ "let", LET },
	{"true",TRUE},
	{"false",FALSE},
	{"if",IF},
	{"else",ELSE},
	{"return",RETURN}
};

Token newToken(const TokenType& type, const char& ch)
{
	std::string str(1, ch);
	Token tok{type, str};
	return tok;
}

Token newToken(const TokenType& type,const std::string& str)
{
	Token tok{ type, str };
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

std::ostream& operator<<(std::ostream& os, const Token& tok)
{
	
	return os << "{type: " << tok._type << ", literal: " << tok._literal << "}\n";	// TODO: 여기에 return 문을 삽입합니다.
}
