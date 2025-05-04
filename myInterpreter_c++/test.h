#pragma once
#include "CAst.h"
#include "CLexer.h"
#include "CParser.h"
#include <iostream>

void lexerTestVer1();
void lexerTestVer2();
void lexerTestVer3();

void TestLetStatement();
bool testLetStatement(const Statement* stmt, const std::string& name);

void TestReturnStatement();
void TestAstString();

void TestIdentifierExpression();
void TestIntegerLiteralExpression();

bool testIntegerLiteral(const Expression* il_expr, int64_t value);
void TestParsingPrefixExpression();

void TestParsingInfixExpressions();
void checkParserErrors(CParser& parser);