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
	//���ڸ� ������ �ֳ� ������ �ϳ� �о���� �Լ�
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
	//������ ��ŵ�ϴ� �Լ�
	while (this->_ch == ' ' || this->_ch == '\t' || this->_ch == '\n' || this->_ch == '\r') {
		this->readChar();
	}
}

Token CLexer::nextToken()
{
	//���� ��ū�� �о���� �Լ�
	Token tok;

	this->skipWhitespace();

	switch (this->_ch) {
	case '=':
		if (this->peekChar() == '=') {
			tok = newToken(EQ, std::string("=="));
			this->readChar();
		}
		else {
			tok = newToken(ASSIGN, this->_ch);
		}
		break;
	case '!':
		if (this->peekChar() == '=') {
			tok = newToken(NOT_EQ, std::string("!="));
			this->readChar();
		}
		else {
			tok = newToken(BANG, this->_ch);
		}
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
	case '-':
		tok = newToken(MINUS, this->_ch);
		break;
	case '/':
		tok = newToken(SLASH, this->_ch);
		break;
	case '*':
		tok = newToken(ASTERISK, this->_ch);
		break;
	case '<':
		tok = newToken(LT, this->_ch);
		break;
	case '>':
		tok = newToken(GT, this->_ch);
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
		//���ڿ����� �������� ������ �˸´� ��ū ����
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

char CLexer::peekChar()
{
	if (this->_read_position >= this->_input.length()) {
		return 0;
	}
	else {
		return this->_input[this->_read_position];
	}
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

