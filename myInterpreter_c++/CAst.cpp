#include "CAst.h"
#include <sstream>
#include <string>
#include <vector>

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

std::string IfExpression::String() const
{
	std::string out = "if" + this->_condition->String() + " " + this->_consequence->String();

	if (this->_alternative != nullptr) {
		out += "else " + this->_alternative->String();
	}
	return out;
}

std::string BlockStatement::String() const
{
	std::string out;

	for (auto& a : this->_statements)
		out += a->String();
	return out;
}
std::string join(const std::vector<std::string>& parts, const std::string& delimiter) {
	std::ostringstream oss;
	for (size_t i = 0; i < parts.size(); ++i) {
		oss << parts[i];
		if (i != parts.size() - 1)
			oss << delimiter;
	}
	return oss.str();
}
std::string FunctionLiteral::String() const
{
	std::string out;
	std::vector<std::string> params;

	for (auto it : _parameters) {
		params.push_back(it->String());
	}

	out += this->getTokenLiteral();
	out += "(";
	out += join(params, ", ");
	out += ") ";
	out += this->getBody()->String();

	return out;
}
