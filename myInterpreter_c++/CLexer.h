#pragma once
/*
	CLexer is a simple lexical analyzer for a custom language.
	It reads input from a string, tokenizes it, and provides methods to access the tokens.
*/
#include <string>
#include <cctype>      // isdigit 함수
#include "Token.h"
class CLexer
{
public:
	CLexer(const std::string& input);
	~CLexer();
	void readChar();
	bool isLetter(char ch);
	bool isDigit(char ch) { return isdigit(ch); }

	void skipWhitespace();
	
	Token nextToken();
	char peekChar();
	std::string readIdentifier();
	std::string readNumber();
private:
	std::string _input;
	int _position;//입력에서 현재 위치
	int _read_position;//입력에서 현재 읽는 위치(현재 다음 문자 pc값과 같은 역할)
	char _ch;//현재 조사중인 문자
};

