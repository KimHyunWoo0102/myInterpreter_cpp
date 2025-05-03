#pragma once
#include <iostream>
#include "CLexer.h"
#include "CAst.h"
#include "CParser.h"

void lexerTestVer1();
void lexerTestVer2();
void lexerTestVer3();

void TestLetStatement();
bool testLetStatement(const Statement* stmt, const std::string& name);

void TestReturnStatement();
void TestAstString();

void TestIdentifierExpression();
void checkParserErrors(CParser& parser);