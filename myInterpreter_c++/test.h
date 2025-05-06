#pragma once
#include "CAst.h"
#include "CLexer.h"
#include "CParser.h"
#include <iostream>
#include <any>

void lexerTestVer1();
void lexerTestVer2();
void lexerTestVer3();

bool testLetStatement(const Statement* stmt, const std::string& name);
bool testIntegerLiteral(const Expression* il_expr, int64_t value);
bool testIdentifier(const Expression* exp, std::string value);
bool testLiteralExpression(const Expression* exp, std::any expected);
bool testInfixExpression(const Expression* exp, std::any left, std::string op, std::any right);
bool testBooleanLiteral(const Expression* exp, bool value);

void TestLetStatement();
void TestReturnStatement();
void TestAstString();

void TestIdentifierExpression();
void TestIntegerLiteralExpression();
void TestIfExpression();
void TestIfElseExpression();

void TestParsingPrefixExpression();
void TestParsingInfixExpressions();
void TestOperatorPrecedenceParsing();
void TestBooleanExpression();
void TestFunctionLiteralParsing();
void TestFunctionParameterParsing();
void checkParserErrors(CParser& parser);