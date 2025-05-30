#pragma once
#include "CLexer.h"
#include<vector>
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
	
	void printParseErrors(std::ostream& os, const std::vector<std::string>&errors);
private:
	const std::string PROMT = ">>";
	const std::string MONKEY_FACE = R"(           
            __,__
   .--.  .-"     "-.  .--.
  / .. \/  .-. .-.  \/ .. \
 | |  '|  /   Y   \ |'  | |
 | \   \  \ 0 | 0 /  /   / |
  \ '- ,\.-"""""""-./, -' /
   ''-' /_   ^ ^   _\ '-''
       |  \._   _./  |
       \   \ '~' /   /
        '._ '-=-' _.'
           '-----'
)";

};

