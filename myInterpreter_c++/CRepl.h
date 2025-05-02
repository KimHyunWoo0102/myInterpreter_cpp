#pragma once
#include "CLexer.h"


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

