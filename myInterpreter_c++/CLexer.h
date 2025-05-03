#pragma once
/*
	CLexer is a simple lexical analyzer for a custom language.
	It reads input from a string, tokenizes it, and provides methods to access the tokens.
*/
#include <string>
#include <cctype>      // isdigit �Լ�
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
	int _position;//�Է¿��� ���� ��ġ
	int _read_position;//�Է¿��� ���� �д� ��ġ(���� ���� ���� pc���� ���� ����)
	char _ch;//���� �������� ����
};

