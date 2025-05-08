#include "CRepl.h"
#include "CParser.h"
#include <sstream>

void CRepl::Start(std::ostream& os, std::istream& is)
{
	os << "Hello! hyunwoo !! this is the Monkey programming lauage." << std::endl;
	os << "Feel free to type in commands." << std::endl;

	std::string line;

	while (true) {
		Print(os, PROMT);

		if (!std::getline(is, line))
			break;

		CLexer lexer(line);
		CParser parser(lexer);

		auto program = parser.parseProgram();

		if (!parser.Errors().empty()) {
			printParseErrors(os, parser.Errors());
			continue;
		}

		if (program != nullptr) {
			os << program->String() << '\n';
		}
	}
}

std::ostream& CRepl::Print(std::ostream& os, const std::string& str)
{
	os << str;
	return os;
}

std::ostream& CRepl::Print(std::ostream& os, const Token& tok)
{
	os << tok;
	return os;
}

void CRepl::printParseErrors(std::ostream& os, const std::vector<std::string>& errors)
{
	os << this->MONKEY_FACE;
	os << "Woops! We ran into some issues here!\n";
	os << "Parser errors:\n";
	for (const auto& msg : errors) {
		os << "\t" << msg << '\n';
	}
}
