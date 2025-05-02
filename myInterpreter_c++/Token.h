#pragma once
#include <string>
#include <map>
#include <iostream>

using TokenType = std::string;

typedef struct _token{
	TokenType _type;
	std::string _literal;
} Token;


extern TokenType ILLEGAL;
extern TokenType EOF_TOKEN;

extern TokenType IDENT;
extern TokenType INT;
extern TokenType TRUE;
extern TokenType FALSE;
extern TokenType IF;
extern TokenType ELSE;
extern TokenType RETURN;

extern TokenType ASSIGN;
extern TokenType PLUS;
extern TokenType MINUS;
extern TokenType SLASH;
extern TokenType ASTERISK;
extern TokenType BANG;
extern TokenType EQ;
extern TokenType NOT_EQ;

extern TokenType LT;
extern TokenType GT;

extern TokenType COMMA;
extern TokenType SEMICOLON;

extern TokenType LPAREN;
extern TokenType RPAREN;
extern TokenType LBRACE;
extern TokenType RBRACE;

extern TokenType FUNCTION;
extern TokenType LET;

extern const std::map<std::string, TokenType> keywords;

Token newToken(const TokenType& type, const char& ch);
Token newToken(const TokenType& type, const std::string& str);
TokenType lookupIdent(std::string& ident);
std::ostream& operator<<(std::ostream& os, const Token& tok);
