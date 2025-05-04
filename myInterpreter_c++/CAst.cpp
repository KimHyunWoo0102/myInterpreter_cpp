#include "CAst.h"

std::string Program::getTokenLiteral() const
{
	if (!this->_statements.empty()) {
		return this->_statements[0]->getTokenLiteral();
	}
	else
		return "";
}

std::string Program::String() const
{
	std::string out;
	for (const auto& stmt : _statements) {
		out += stmt->String();
	}
	return out;
}

std::string LetStatement::String() const
{
	std::string out;
	out += this->getTokenLiteral() + " ";
	out += this->getName()->String();
	out += " = ";

	if (this->getValue() != nullptr) {
		out += this->getValue()->String();
	}
	out += ";";
	
	return out;
}

std::string ReturnStatement::String() const
{
	std::string out;

	out += this->getReturnValue()->String() + " ";

	if (this->getReturnValue() != nullptr) {
		out += this->getReturnValue()->String();
	}

	out += ";";
	return out;
}

std::string ExpressionStatement::String() const
{
	if (this->getExpression() != nullptr) {
		return this->getExpression()->String();
	}
	return "";
}

std::string PrefixExpression::String() const
{
	std::string out = "";

	out += "(";
	out += this->getOperator();
	out += this->getRight()->String();
	out += ")";

	return out;
}

std::string InfixExpression::String() const
{
	std::string out;
	out += "(";
	out += this->getLeft()->String();
	out += " " + this->getOperator() + " ";
	out += this->getRight()->String();
	out += ")";

	return out;
}
