#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstddef>  // for size_t (optional)

// Token을 정의한 헤더
#include "Token.h"
// Node 인터페이스
class Node {
public:
	Node() = default;
	Node(const Token& token) : _token(token) {}
	virtual ~Node() = default;

	virtual std::string getTokenLiteral() const { return _token._literal; }
	virtual std::string String() const{ return ""; }
protected:
	Token _token;
};

// Statement 인터페이스
class Statement : public Node {
public:
	Statement() = default;
	Statement(const Token& token) : Node(token) {}
	virtual ~Statement() = default;

	virtual std::string getStatementNode() = 0;
};

// Program 클래스
class Program : public Node {
public:
	Program() = default;
	Program(const Token& token) : Node(token) {}
	Program(std::vector<std::unique_ptr<Statement>>&& stmts) {
		_statements = std::move(stmts);
	}
	virtual ~Program() = default;

	virtual std::string getTokenLiteral() const override;

	size_t getStatementsSize() const {
		return _statements.size();
	}
	std::string String() const override;

	const Statement* getStatement(size_t index) const {
		if (index >= _statements.size()) return nullptr;
		return _statements[index].get();
	}

	void addStatement(std::unique_ptr<Statement> statement) {
		_statements.push_back(std::move(statement));
	}

private:
	std::vector<std::unique_ptr<Statement>> _statements;
};

// Expression 인터페이스
class Expression : public Node {
public:
	Expression() = default;
	Expression(const Token& token) : Node(token) {}
	virtual ~Expression() = default;
	virtual std::string getExpressionNode() = 0;
};

// Identifier 클래스
class Identifier : public Expression {
public:
	Identifier() = default;
	Identifier(const Token& token, const std::string& value)
		: Expression(token), _value(value) {
	}

	virtual ~Identifier() = default;
	std::string String() const override {
		return _value;
	}
	virtual std::string getExpressionNode() override { return "Identifier"; }

	const std::string& getValue() const {
		return _value;
	}

private:
	std::string _value;
};

class IntegerLiteral : public Expression {
public:
	IntegerLiteral() = default;
	IntegerLiteral(const Token& token)
		: Expression(token), _value(0) {
	}
	/*IntegerLiteral(const Token& token, const std::string& value)
		: Expression(token), _value(0) {
		_value = std::stoll(value);
	}*/
	IntegerLiteral(const Token& token, const int64_t value)
		: Expression(token), _value(value) {
	}
	virtual ~IntegerLiteral() = default;

	std::string String() const override {
		return _token._literal;
	}
	virtual std::string getExpressionNode() override { return "IntegerLiteral"; }
	const int64_t& getValue() const {
		return _value;
	}
	void setValue(int64_t value) {
		_value = value;
	}
private:
	int64_t _value;
};

class PrefixExpression : public Expression {
public:
	PrefixExpression() = default;
	PrefixExpression(Token token):Expression(token){}
	PrefixExpression(Token token, std::string& op)
		:Expression(token), _operator(op) {}
	~PrefixExpression() = default;

	const std::string& getOperator() const{ return _operator; }
	const Expression* getRight() const{ return _right.get(); }

	void setOperator(std::string& op) { _operator = op; }
	void setRight(std::unique_ptr<Expression> right) { _right = std::move(right); }

	virtual std::string getExpressionNode()override { return "PrefixExpression"; }
	virtual std::string String() const override;

private:
	std::string _operator;
	std::unique_ptr<Expression>_right;
};

class InfixExpression :public Expression {
public:
	InfixExpression() = default;
	InfixExpression(Token token):Expression(token){}
	InfixExpression(Token token, std::string& op)
		:Expression(token), _operator(op) {}
	~InfixExpression() = default;

	const std::string& getOperator()const { return _operator; }
	const Expression* getLeft()const { return _left.get(); }
	const Expression* getRight()const { return _right.get(); }

	void setOperator(std::string& op) { _operator = op; }
	void setLeft(std::unique_ptr<Expression> left) { _right = std::move(left); }
	void setRight(std::unique_ptr<Expression> right) { _right = std::move(right); }

	virtual std::string getExpressionNode()override { return "PrefixExpression"; }
	virtual std::string String() const override;
private:
	std::unique_ptr<Expression> _left;
	std::string _operator;
	std::unique_ptr<Expression>_right;
};
// LetStatement 클래스
class LetStatement : public Statement {
public:
	LetStatement() = default;
	LetStatement(const Token& token) : Statement(token) {}
	virtual ~LetStatement() = default;

	std::string String() const override;
	virtual std::string getStatementNode() override { return "LetStatement"; }
	
	const Identifier* getName() const { return _name.get(); }
	const Expression* getValue() const { return _value.get(); }

	void setName(std::unique_ptr<Identifier> name) {
		_name = std::move(name);
	}

	void setValue(std::unique_ptr<Expression> value) {
		_value = std::move(value);
	}

private:
	std::unique_ptr<Identifier> _name;
	std::unique_ptr<Expression> _value;
};

class ReturnStatement : public Statement {
public:
	ReturnStatement() = default;
	ReturnStatement(const Token& token) : Statement(token) {}
	virtual ~ReturnStatement() = default;

	std::string String() const override;
	virtual std::string getStatementNode() override { return "ReturnStatement"; }
	const Expression* getReturnValue() const { return _return_value.get(); }
	void setReturnValue(std::unique_ptr<Expression> return_value) {
		_return_value = std::move(return_value);
	}
private:
	std::unique_ptr<Expression> _return_value;
};

// ExpressionStatement 클래스
class ExpressionStatement : public Statement {
public:
	ExpressionStatement() = default;
	ExpressionStatement(const Token& token) : Statement(token) {}
	virtual ~ExpressionStatement() = default;
	
	std::string String() const override;
	virtual std::string getStatementNode() override { return "ExpressionStatement"; }

	const Expression* getExpression() const { return _expression.get(); }
	void setExpression(std::unique_ptr<Expression> expression) {
		_expression = std::move(expression);
	}
private:
	std::unique_ptr<Expression> _expression;
};