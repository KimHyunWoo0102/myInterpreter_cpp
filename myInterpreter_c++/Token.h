#pragma once
#include <string>
#include <map>

using TokenType = std::string;

typedef struct _token{
	TokenType _type;
	std::string _literal;
} Token;


extern TokenType ILLEGAL;
extern TokenType EOF_TOKEN;

extern TokenType IDENT;
extern TokenType INT;

extern TokenType ASSIGN;
extern TokenType PLUS;

extern TokenType COMMA;
extern TokenType SEMICOLON;

extern TokenType LPAREN;
extern TokenType RPAREN;
extern TokenType LBRACE;
extern TokenType RBRACE;

extern TokenType FUNCTION;
extern TokenType LET;

extern const std::map<std::string, TokenType> keywords;

Token newToken(TokenType& type, char& ch);
TokenType lookupIdent(std::string& ident);