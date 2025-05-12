#include "evaluator_test.h"
#include "CParser.h"
#include "Evaluator.h"
#include <any>

void TestEvalIntegerExpression()
{
	struct tests {
		std::string input;
		int64_t expected;
	};

	tests test_case[]{
		{"5 + 5 + 5 + 5",20},
		{"5 - 5",0},
		{"5 + 5 - 5",5},
		{"5 * 5",25},
		{"5 * 2 + 10",20},
		{"5 + 2 * 10",25},
		{"2 * (5 + 10)",30},
		{"3 * 3 * 3 + 10",37},
		{"3 * 3 * (3 + 10)",117},
		{"(5 + 10 * 2 + 15 / 3) * 2 + -10",50}
	};

	for (auto& each : test_case) {
		auto evaluated = testEval(each.input);
		testIntegerObject(evaluated, each.expected);
	}
}

std::shared_ptr<CObject> testEval(const std::string& input)
{
	CLexer lexer(input);
	CParser parser(lexer);
	auto program = parser.parseProgram();
	return eval(program.get());
}

bool testIntegerObject(const std::shared_ptr<CObject> obj,const int64_t expected)
{
	auto result = dynamic_cast<const Integer*>(obj.get());
	if (result == nullptr) {
		std::cout << "object is not Integer. got=" << typeid(*obj).name() << std::endl;
		return false;
	}
	if (result->getValue() != expected) {
		std::cout << "object has wrong value. got=" << result->getValue() << ", want=" << expected << std::endl;
		return false;
	}

	return false;
}

void TestEvalBooleanExpression()
{
	struct {
		std::string input;
		bool expected;
	}tests[]{
		{"true",true},
		{"false",false}
	};

	for (auto& tt : tests) {
		auto evaluated = testEval(tt.input);
		testBooleanObject(evaluated, tt.expected);
	}
}

bool testBooleanObject(const std::shared_ptr<CObject> obj, const bool expected)
{
	auto result = dynamic_cast<const BooleanObject*>(obj.get());
	if (result == nullptr) {
		std::cout << "object is not BooleanObject. got=" << typeid(*obj).name() << std::endl;
		return false;
	}
	if (result->getValue()!=expected) {
		std::cout << "object has wrong value. get=" << result->getValue() << ", want=" << expected << std::endl;
		return false;
	}
	return true;
}

void TestIfElseExpressions()
{
	struct {
		std::string input;
		std::any expected;
	}tests[]{
		{"if (true){ 10 }", 10},
		{ "if (false){ 10 }",nullptr },
		{ "if (1) {10}",10 },
		{ "if (1 < 2) { 10 }",10 },
		{ "if (1 > 2) { 10 }",nullptr },
		{ "if (1 > 2) { 10 } else { 20 }",20 },
		{ "if (1 < 2) { 10 } else { 20 }",10 }
	};

	for (auto& tt : tests) {
		auto evaluated = testEval(tt.input);

		try {
			int integer = std::any_cast<int>(tt.expected);
			testIntegerObject(evaluated, static_cast<int64_t>(integer));
		}
		catch (const std::bad_any_cast& e) {
			testNullObject(evaluated);
		}
	}

}

void TestBangOperator()
{
	struct {
		std::string input;
		bool expected;
	}tests[]{
		{"!false",true},
		{"!5",false},
		{"!!true",true},
		{"!!false",false},
		{"!!5",true}
	};

	for (const auto& tt : tests) {
		auto evaluated = testEval(tt.input);
		testBooleanObject(evaluated, tt.expected);
	}
}

bool testNullObject(const std::shared_ptr<CObject>& obj)
{
	if (obj != NULL_OBJECT)
	{
		std::cout << "object is not NULL. got=" << typeid(*obj).name() << std::endl;
		return false;
	}
	return true;
}

void TestReturnStatements()
{
	struct {
		std::string input;
		int64_t expected;
	}tests[]{
		{"return 10;",10},
		{"return 10; 9;",10},
		{"return 2 * 5; 9;",10},
		{"9; return 2 * 5; 9;",10},
		{ "if (10 > 1) { if (10 > 1) { return 10; } return 1; }", 10 }
	};

	for (auto& tt : tests) {
		auto evaluated = testEval(tt.input);
		testIntegerObject(evaluated, tt.expected);
	}
}

void TestErrorHandling()
{
	struct {
		std::string input;
		std::string expectedMessage;
	}tests[] = {
		{"5 + true;","type mismatch: INTEGER + BOOLEAN"},
		{"5 + true; 5;","type mismatch: INTEGER + BOOLEAN"},
		{"-true;","unknown operator: -BOOLEAN"},
		{"true + false;","unknown operator: BOOLEAN + BOOLEAN"},
		{"5; true + false; 5;","unknown operator: BOOLEAN + BOOLEAN"},
		{"if (10>1) { true + false; }","unknown operator: BOOLEAN + BOOLEAN"},
		{"if (10>1) { if (10>1) { return true + false;} return 1;}","unknown operator: BOOLEAN + BOOLEAN"}
	};

	for (const auto& tt : tests) {
		auto evaluated = testEval(tt.input);

		auto errObj = dynamic_cast<Error*>(evaluated.get());
		if (!errObj) {
			std::cout << "no error object returned. got=" << typeid(evaluated).name() << std::endl;
			continue;
		}
		if (errObj->getMessage() != tt.expectedMessage) {
			std::cout << "wrong error message. expected=" << tt.expectedMessage << ", got=" << errObj->getMessage() << std::endl;
		}
	}
}
