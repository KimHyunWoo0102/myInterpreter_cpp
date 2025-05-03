#pragma once
#include "CLexer.h"

/*
	CPerl is a simple REPL (Read-Eval-Print Loop) interpreter for a custom language.
	It reads input from the user, tokenizes it with CLexer, and prints the tokens to the output stream.
*/
class CRepl
{
public:
	CRepl() = default;
	~CRepl() = default;
	void Start(std::ostream& os, std::istream& is);
	std::ostream& Print(std::ostream& os, const std::string& str);
	std::ostream& Print(std::ostream& os, const Token& tok);
private:
	const std::string PROMT = ">>";
};

