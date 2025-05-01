#include "CLexer.h"

CLexer::CLexer(const std::string& input)
	:_input(input), _position(0), _read_position(1), _ch(input[0])
{
}

CLexer::~CLexer()
{
}

void CLexer::readChar()
{
	//문자를 읽을수 있나 조사후 하나 읽어오는 함수
	if (this->_read_position >= this->_input.length()) {
		this->_ch = 0;
	}
	else {
		this->_ch = this->_input[this->_read_position];
	}

	this->_position = this->_read_position;
	this->_read_position++;
}

bool CLexer::isLetter(char ch)
{
	return isalpha(ch) || ch == '_';;
}

void CLexer::skipWhitespace()
{
	//공백을 스킵하는 함수
	while (this->_ch == ' ' || this->_ch == '\t' || this->_ch == '\n' || this->_ch == '\r') {
		this->readChar();
	}
}

Token CLexer::nextToken()
{
	Token tok;

	this->skipWhitespace();

	switch (this->_ch) {
	case '=':
		tok = newToken(ASSIGN, this->_ch);
		break;
	case ';':
		tok = newToken(SEMICOLON, this->_ch);
		break;
	case '(':
		tok = newToken(LPAREN, this->_ch);
		break;
	case ')':
		tok = newToken(RPAREN, this->_ch);
		break;
	case ',':
		tok = newToken(COMMA, this->_ch);
		break;	
	case '+':
		tok = newToken(PLUS, this->_ch);
		break;
	case '{':
		tok = newToken(LBRACE, this->_ch);
		break;
	case '}':
		tok = newToken(RBRACE, this->_ch);
		break;
	case 0:
		tok._literal = "";
		tok._type = EOF_TOKEN;
		break;

	default:
		if (isLetter(this->_ch)) {
			tok._literal = this->readIdentifier();
			tok._type = lookupIdent(tok._literal);
			return tok;
		}
		else if (isDigit(this->_ch)) {
			tok._literal = this->readNumber();
			tok._type = INT;
			return tok;
		}
		else {
			tok = newToken(ILLEGAL, this->_ch);
		}
		break;
	}
	this->readChar();
	return tok;
}

std::string CLexer::readIdentifier()
{
	int position = this->_position;

	while (isLetter(this->_ch)) {
		this->readChar();
	}
	int len = this->_position - position;
	return this->_input.substr(position, len);
}

std::string CLexer::readNumber()
{
	int position = this->_position;
	while (isDigit(this->_ch)) {
		this->readChar();
	}
	int len = this->_position - position;
	return this->_input.substr(position, len);
}

