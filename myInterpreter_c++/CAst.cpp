#include "CAst.h"

std::string Program::getTokenLiteral() const
{
	if (!this->_statements.empty()) {
		return this->_statements[0]->getTokenLiteral();
	}
	else
		return "";
}
