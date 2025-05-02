#include "CRepl.h"
#include <iostream>
void CRepl::Start(std::ostream& os, std::istream& is)
{
	std::string line;
	
	while (true) {
		this->Print(os, PROMT);
		if (!std::getline(is, line))break;
		CLexer lexer(line);
		for (Token tok = lexer.nextToken(); tok._type != EOF_TOKEN; tok = lexer.nextToken()) {
			this->Print(os, tok);
		}
	}
}

std::ostream& CRepl::Print(std::ostream& os, const std::string& str)
{
	return os << str;
	// TODO: ���⿡ return ���� �����մϴ�.
}
std::ostream& CRepl::Print(std::ostream& os, const Token& tok)
{
	return os << tok;
	// TODO: ���⿡ return ���� �����մϴ�.
}
