#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstddef>  // for size_t (optional)

// Token을 정의한 헤더
#include "Token.h"
#include <optional>
class BlockStatement;//전방선언
// Node 인터페이스
class Node {
public:
	Node() = default;
	Node(const Token& token) : _token(token) {}
	virtual ~Node() = default;

	virtual std::string getTokenLiteral() const { return _token._literal; }
	virtual std::string String() const { return ""; }
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
	Program(std::vector<std::shared_ptr<Statement>>&& stmts) {
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

	void addStatement(std::shared_ptr<Statement> statement) {
		_statements.push_back(std::move(statement));
	}

private:
	std::vector<std::shared_ptr<Statement>> _statements;
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

class FunctionLiteral : public Expression {
public:
	// 기본 생성자
	FunctionLiteral() = default;
	FunctionLiteral(Token token) :Expression(token) {}
	// 파라미터와 바디를 받는 생성자
	FunctionLiteral(const std::vector<std::shared_ptr<Identifier>>& parameters,
		const std::shared_ptr<BlockStatement>& body)
		: _parameters(parameters), _body(body) {
	}

	virtual ~FunctionLiteral() = default;

	// 문자열로 출력 (구현 필요)
	virtual std::string String() const override;

	virtual std::string getExpressionNode() override { return "FunctionLiteral"; }

	// Getter for parameters
	const std::vector<std::shared_ptr<Identifier>>& getParameters() const {
		return _parameters;
	}

	// Setter for parameters
	void setParameters(std::optional<std::vector<std::shared_ptr<Identifier>>> parameters)
	{
		if (parameters) {
			// 값이 있을 경우에만 set
			this->_parameters = *parameters;
		}
		else {
			// 파라미터가 없을 경우 처리 로직 (필요한 경우)
			this->_parameters.clear();
		}
	}

	// Add one parameter
	void addParameter(const std::shared_ptr<Identifier>& param) {
		_parameters.push_back(param);
	}

	// Getter for body
	const std::shared_ptr<BlockStatement>& getBody() const {
		return _body;
	}

	// Setter for body
	void setBody(const std::shared_ptr<BlockStatement>& body) {
		_body = body;
	}

private:
	std::vector<std::shared_ptr<Identifier>> _parameters;
	std::shared_ptr<BlockStatement> _body;
};

class IntegerLiteral : public Expression {
public:
	IntegerLiteral() = default;
	IntegerLiteral(const Token& token)
		: Expression(token), _value(0) {
	}
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
	PrefixExpression(Token token) :Expression(token) {}
	PrefixExpression(Token token, std::string& op)
		:Expression(token), _operator(op) {
	}
	virtual ~PrefixExpression() = default;

	const std::string& getOperator() const { return _operator; }
	const Expression* getRight() const { return _right.get(); }

	void setOperator(std::string& op) { _operator = op; }
	void setRight(std::shared_ptr<Expression> right) { _right = std::move(right); }

	virtual std::string getExpressionNode()override { return "PrefixExpression"; }
	virtual std::string String() const override;

private:
	std::string _operator;
	std::shared_ptr<Expression> _right;
};

class InfixExpression :public Expression {
public:
	InfixExpression() = default;
	InfixExpression(Token token) :Expression(token) {}
	InfixExpression(Token token, std::string& op)
		:Expression(token), _operator(op) {
	}
	InfixExpression(Token token, std::string& op, std::shared_ptr<Expression> left)
		:Expression(token), _operator(op), _left(std::move(left)) {
	}
	virtual ~InfixExpression() = default;

	const std::string& getOperator()const { return _operator; }
	const Expression* getLeft()const { return _left.get(); }
	const Expression* getRight()const { return _right.get(); }

	void setOperator(std::string& op) { _operator = op; }
	void setLeft(std::shared_ptr<Expression> left) { _right = std::move(left); }
	void setRight(std::shared_ptr<Expression> right) { _right = std::move(right); }

	virtual std::string getExpressionNode()override { return "PrefixExpression"; }
	virtual std::string String() const override;
private:
	std::shared_ptr<Expression> _left;
	std::string _operator;
	std::shared_ptr<Expression> _right;
};

class Boolean :public Expression {
public:
	Boolean() = default;
	Boolean(Token token):Expression(token){}
	Boolean(Token token,bool value):Expression(token),_value(value){}
	virtual ~Boolean() = default;

	const bool getValue()const { return _value; }
	virtual std::string getExpressionNode()override { return "PrefixExpression"; }
	virtual std::string String() const override { return this->_token._literal; }

private:
	bool _value;
};


class IfExpression :public Expression {
public:
	IfExpression() = default;
	IfExpression(Token token):Expression(token){}
	IfExpression(Token token, std::shared_ptr<BlockStatement> consequence, std::shared_ptr< BlockStatement> alternative)
		:Expression(token), _consequence(consequence), _alternative(alternative) {}
	virtual ~IfExpression() = default;

	const BlockStatement* getConsequence()const { return _consequence.get(); }
	const BlockStatement* getAlternative()const { return _alternative.get(); }
	const Expression* getCondition()const { return _condition.get(); }

	void setCondition(std::shared_ptr<Expression> condition) {
		_condition = std::move(condition);
	}

	void setConsequence(std::shared_ptr<BlockStatement> consequence) {
		_consequence = std::move(consequence);
	}

	void setAlternative(std::shared_ptr<BlockStatement> alternative) {
		_alternative = std::move(alternative);
	}

	virtual std::string getExpressionNode()override { return "IfExpression"; }
	virtual std::string String() const override;

private:
	std::shared_ptr<Expression> _condition;
	std::shared_ptr<BlockStatement> _consequence;
	std::shared_ptr< BlockStatement> _alternative;
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

	void setName(std::shared_ptr<Identifier> name) {
		_name = std::move(name);
	}

	void setValue(std::shared_ptr<Expression> value) {
		_value = std::move(value);
	}

private:
	std::shared_ptr<Identifier> _name;
	std::shared_ptr<Expression> _value;
};

class ReturnStatement : public Statement {
public:
	ReturnStatement() = default;
	ReturnStatement(const Token& token) : Statement(token) {}
	virtual ~ReturnStatement() = default;

	std::string String() const override;
	virtual std::string getStatementNode() override { return "ReturnStatement"; }
	const Expression* getReturnValue() const { return _return_value.get(); }
	void setReturnValue(std::shared_ptr<Expression> return_value) {
		_return_value = std::move(return_value);
	}
private:
	std::shared_ptr<Expression> _return_value;
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
	void setExpression(std::shared_ptr<Expression> expression) {
		_expression = std::move(expression);
	}
private:
	std::shared_ptr<Expression> _expression;
};

class BlockStatement : public Statement {
public:
	BlockStatement() = default;

	BlockStatement(const Token& token) :Statement(token) {}

	virtual ~BlockStatement() = default;

	std::string String() const override;
	virtual std::string getStatementNode() override { return "BlockStatement"; }

	const std::vector<std::shared_ptr<Statement>>& getStatements() const { return _statements; }
	const size_t getStatementsSize()const { return _statements.size(); }
	void addStatement(std::shared_ptr<Statement> stmt) {
		_statements.push_back(std::move(stmt));
	}

private:
	std::vector<std::shared_ptr<Statement>> _statements;
};
