#pragma once

#include "CLexer.h"
#include"CObject.h"
#include<memory>

void TestEvalIntegerExpression();
std::shared_ptr<CObject> testEval(const std::string &input);
bool testIntegerObject(const std::shared_ptr<CObject> obj,const int64_t expected);

void TestEvalBooleanExpression();
bool testBooleanObject(const std::shared_ptr<CObject>obj, const bool expected);

void TestIfElseExpressions();
void TestBangOperator();

bool testNullObject(const std::shared_ptr<CObject>& obj);
void TestReturnStatements();
void TestErrorHandling();

