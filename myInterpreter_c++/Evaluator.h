#pragma once
#include "CAst.h"
#include"CObject.h"

extern const std::shared_ptr<BooleanObject> _TRUE;
extern const std::shared_ptr<BooleanObject> _FALSE;
extern const std::shared_ptr<Null>_NULL;

extern const std::shared_ptr<CObject> TRUE_OBJ;
extern const std::shared_ptr<CObject> FALSE_OBJ;
extern const std::shared_ptr<CObject> NULL_OBJECT;

bool isTruthy(const std::shared_ptr<CObject>& object);
std::shared_ptr<CObject> eval(Node* node);
std::shared_ptr<CObject> evalStatements(const std::vector<std::shared_ptr<Statement>>& stmts);

const std::shared_ptr<CObject> evalProgram(const Program* program);
const std::shared_ptr<CObject> evalPrefixExpression(const std::string& op, std::shared_ptr<CObject>& right);
const std::shared_ptr<CObject>& evalBangOperatorExpression(const std::shared_ptr<CObject>& right);
const std::shared_ptr<CObject> evalMinusPrefixOperatorExpression(const std::shared_ptr<CObject>& right);
const std::shared_ptr<CObject> evalInfixExpression(const std::string& op, const std::shared_ptr<CObject>& left, const std::shared_ptr<CObject>& right);
const std::shared_ptr<CObject> evalIntegerInfixExpression(const std::string& op, const std::shared_ptr<CObject>& left, const std::shared_ptr<CObject>& right);


const std::shared_ptr<CObject> evalIfExpression(const IfExpression* ie);
const std::shared_ptr<CObject> evalBlockStatement(const BlockStatement* block);
const std::shared_ptr<CObject>& nativeBoolToBooleanObject(bool value);

template<typename... Args>
std::shared_ptr<Error> newError(const char* formatStr, Args&&... args);

