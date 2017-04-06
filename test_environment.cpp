#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter_semantic_error.hpp"
#include "environment.hpp"

Expression result;
Expression beginEx(std::string("begin"));
Expression ifEx(std::string("if"));
Expression compEx(std::string("<"));
Expression aEx(std::string("a"));
Expression bEx(std::string("b"));
Expression opEx(std::string("*"));
Expression opPlus (std::string("+"));
Expression doubleEx(100.);
Expression double1Ex(1.);
Expression double2Ex(2.);
Expression trueEx(true);
Expression falseEx(false);
Expression defineEx(std::string("define"));

// will return an expression with an empty string in the "type" parameter on error

TEST_CASE("Test evaluate begin", "[environment]") {
	{
		// test with no arguments
		Environment test;
		result = test.evaluateBegin(beginEx);
		REQUIRE(result.getType().empty() == true);
	}
	{
		// test with undefined symbol
		Environment test;
		test.clearEnvironment();
		beginEx.arguments.push(&aEx);
		beginEx.arguments.push(&doubleEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result.getType().empty() == true);
		beginEx.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		beginEx.arguments.push(&doubleEx);
		beginEx.arguments.push(&trueEx);
		beginEx.arguments.push(&falseEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result == Expression(false));
		beginEx.arguments.pop();
		beginEx.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&doubleEx);
		beginEx.arguments.push(&defineEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result == Expression(100.));
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&doubleEx);
		beginEx.arguments.push(&defineEx);
		beginEx.arguments.push(&bEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result.getType().empty() == true);
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		beginEx.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&doubleEx);
		beginEx.arguments.push(&defineEx);
		beginEx.arguments.push(&aEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result == Expression(100.));
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		beginEx.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&doubleEx);
		beginEx.arguments.push(&trueEx);
		beginEx.arguments.push(&defineEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result == Expression(100.));
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		beginEx.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		aEx.arguments.push(&doubleEx);
		defineEx.arguments.push(&aEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result.getType().empty() == true);
		aEx.arguments.pop();
		defineEx.arguments.pop();
	}
	{
		Environment test;
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		compEx.makeComp();
		ifEx.arguments.push(&compEx);
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&falseEx);
		beginEx.arguments.push(&ifEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		Expression begin2(std::string("begin"));
		begin2.arguments.push(&doubleEx);
		beginEx.arguments.push(&begin2);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result == Expression(100.));
		begin2.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		compEx.makeComp();
		beginEx.arguments.push(&compEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
		beginEx.arguments.pop();
	}
	{
		Environment test;
		opEx.arguments.push(&doubleEx);
		opEx.arguments.push(&double2Ex);
		opEx.makeOp();
		beginEx.arguments.push(&opEx);
		result = test.evaluateBegin(beginEx);
		REQUIRE(result == Expression(200.));
		opEx.arguments.pop();
		opEx.arguments.pop();
		beginEx.arguments.pop();
	}
}

TEST_CASE("test evaluate define", "[environment]") {
	{
		Environment test;
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&doubleEx);
		ifEx.arguments.push(&bEx);
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&ifEx);
		result = test.evaluateDefine(defineEx);
		REQUIRE(result == Expression(100.));
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		defineEx.arguments.pop();
		defineEx.arguments.pop();
	}
	{
		Environment test;
		Expression exp2(std::string("define"));
		exp2.arguments.push(&aEx);
		exp2.arguments.push(&doubleEx);
		defineEx.arguments.push(&bEx);
		defineEx.arguments.push(&exp2);
		result = test.evaluateDefine(defineEx);
		REQUIRE(result == Expression(100.));
		exp2.arguments.pop();
		exp2.arguments.pop();
		defineEx.arguments.pop();
		defineEx.arguments.pop();
	}
	{
		Environment test;
		Expression exp2(std::string("begin"));
		exp2.arguments.push(&doubleEx);
		exp2.arguments.push(&double2Ex);
		defineEx.arguments.push(&bEx);
		defineEx.arguments.push(&exp2);
		result = test.evaluateDefine(defineEx);
		REQUIRE(result == Expression(2.));
		exp2.arguments.pop();
		exp2.arguments.pop();
		defineEx.arguments.pop();
		defineEx.arguments.pop();
	}
	{
		Environment test;
		Expression exp2(std::string("begin"));
		exp2.arguments.push(&doubleEx);
		exp2.arguments.push(&double2Ex);
		defineEx.arguments.push(&bEx);
		defineEx.arguments.push(&exp2);
		result = test.evaluateDefine(defineEx);
		REQUIRE(result == Expression(2.));
		exp2.arguments.pop();
		exp2.arguments.pop();
		defineEx.arguments.pop();
		defineEx.arguments.pop();
	}
}

TEST_CASE("test evaluate if", "[environment]") {
	{
		Environment test;
		opPlus.arguments.push(&doubleEx);
		opPlus.arguments.push(&double1Ex);
		opPlus.arguments.push(&double2Ex);
		opPlus.makeOp();
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&opPlus);
		result = test.evaluateIf(ifEx);
		REQUIRE(result.getType().empty() == true);
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
	}
	{
		Environment test;
		opPlus.arguments.push(&doubleEx);
		opPlus.arguments.push(&double1Ex);
		opPlus.arguments.push(&double2Ex);
		opPlus.makeOp();
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&opPlus);
		ifEx.arguments.push(&falseEx);
		result = test.evaluateIf(ifEx);
		REQUIRE(result == Expression(103.));
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
	}
	{
		Environment test;
		opPlus.arguments.push(&doubleEx);
		opPlus.arguments.push(&double1Ex);
		opPlus.arguments.push(&double2Ex);
		opPlus.makeOp();
		ifEx.arguments.push(&falseEx);
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&opPlus);
		result = test.evaluateIf(ifEx);
		REQUIRE(result == Expression(103.));
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
	}
	{
		Environment test;
		opPlus.arguments.push(&doubleEx);
		opPlus.arguments.push(&double2Ex);
		opPlus.makeOp();
		opPlus.setSymbol(std::string("-"));
		ifEx.arguments.push(&falseEx);
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&opPlus);
		result = test.evaluateIf(ifEx);
		REQUIRE(result == Expression(98.));
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		opPlus.setSymbol(std::string("+"));
	}
	{
		Environment test;
		opPlus.arguments.push(&doubleEx);
		opPlus.arguments.push(&double1Ex);
		opPlus.arguments.push(&double2Ex);
		opPlus.makeOp();
		opPlus.setSymbol(std::string("-"));
		ifEx.arguments.push(&falseEx);
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&opPlus);
		result = test.evaluateIf(ifEx);
		REQUIRE(result.getType().empty() == true);
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		opPlus.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		opPlus.setSymbol(std::string("+"));
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&doubleEx);
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&defineEx);
		ifEx.arguments.push(&falseEx);
		result = test.evaluateIf(ifEx);
		REQUIRE(result == Expression(100.));
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
	}
	{
		Environment test;
		beginEx.arguments.push(&doubleEx);
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&beginEx);
		ifEx.arguments.push(&falseEx);
		result = test.evaluateIf(ifEx);
		REQUIRE(result == Expression(100.));
		beginEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
	}
	{
		Environment test;
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		compEx.setSymbol(">");
		compEx.makeComp();
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&compEx);
		ifEx.arguments.push(&falseEx);
		result = test.evaluateIf(ifEx);
		REQUIRE(result == Expression(false));
		compEx.arguments.pop();
		compEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		compEx.setSymbol("<");
	}
	{
		Environment test;
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		compEx.setSymbol(">=");
		compEx.makeComp();
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&compEx);
		ifEx.arguments.push(&falseEx);
		result = test.evaluateIf(ifEx);
		REQUIRE(result == Expression(false));
		compEx.arguments.pop();
		compEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		compEx.setSymbol("<");
	}
	{
		Environment test;
		double1Ex.setDouble(2.);
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		compEx.setSymbol(">=");
		compEx.makeComp();
		ifEx.arguments.push(&trueEx);
		ifEx.arguments.push(&compEx);
		ifEx.arguments.push(&falseEx);
		result = test.evaluateIf(ifEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		ifEx.arguments.pop();
		double1Ex.setDouble(1.);
	}
}

TEST_CASE("test evaluate op", "[environment]") {
	{
		Environment test;
		opEx.setSymbol("/");
		opEx.makeOp();
		opEx.arguments.push(&double2Ex);
		opEx.arguments.push(&double1Ex);
		result = test.evaluateEx(opEx);
		REQUIRE(result == Expression(2.));
		opEx.arguments.pop();
		opEx.arguments.pop();
	}
	{
		Environment test;
		opEx.setSymbol("-");
		opEx.makeOp();
		opEx.arguments.push(&double2Ex);
		opEx.arguments.push(&double1Ex);
		result = test.evaluateEx(opEx);
		REQUIRE(result == Expression(1.));
		opEx.arguments.pop();
		opEx.arguments.pop();
	}
	{
		Environment test;
		opEx.setSymbol("-");
		opEx.makeOp();
		opEx.arguments.push(&double2Ex);
		result = test.evaluateEx(opEx);
		REQUIRE(result == Expression(-2.));
		opEx.arguments.pop();
	}
	{
		Environment test;
		opEx.setSymbol("-");
		opEx.makeOp();
		opEx.arguments.push(&double2Ex);
		opEx.arguments.push(&double1Ex);
		opEx.arguments.push(&doubleEx);
		result = test.evaluateEx(opEx);
		REQUIRE(result.getType().empty() == true);
		opEx.arguments.pop();
		opEx.arguments.pop();
		opEx.arguments.pop();
	}
	{
		Environment test;
		opEx.setSymbol("/");
		opEx.makeOp();
		opEx.arguments.push(&double2Ex);
		opEx.arguments.push(&double1Ex);
		opEx.arguments.push(&doubleEx);
		result = test.evaluateEx(opEx);
		REQUIRE(result.getType().empty() == true);
		opEx.arguments.pop();
		opEx.arguments.pop();
		opEx.arguments.pop();
	}
	{
		Environment test;
		opEx.setSymbol("-");
		opEx.makeOp();
		result = test.evaluateEx(opEx);
		REQUIRE(result.getType().empty() == true);
	}
}

TEST_CASE("test evaluate comp", "[environment]") {
	{
		Environment test;
		compEx.setSymbol(">");
		compEx.makeComp();
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(false));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("<");
		compEx.makeComp();
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol(">");
		compEx.makeComp();
		compEx.arguments.push(&double2Ex);
		compEx.arguments.push(&double1Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol(">=");
		compEx.makeComp();
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(false));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("<=");
		compEx.makeComp();
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		double1Ex.setDouble(2.);
		compEx.setSymbol("=");
		compEx.makeComp();
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		double1Ex.setDouble(2.);
		compEx.setSymbol("<=");
		compEx.makeComp();
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		double1Ex.setDouble(2.);
		compEx.setSymbol(">=");
		compEx.makeComp();
		compEx.arguments.push(&double1Ex);
		compEx.arguments.push(&double2Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		double1Ex.setDouble(2.);
		compEx.setSymbol("=");
		compEx.makeComp();
		compEx.arguments.push(&trueEx);
		compEx.arguments.push(&double2Ex);
		result = test.evaluateComp(compEx);
		REQUIRE(result.getType().empty() == true);
		compEx.arguments.pop();
		compEx.arguments.pop();
		double1Ex.setDouble(1.);
	}
	{
		Environment test;
		compEx.setSymbol("not");
		compEx.makeComp();
		compEx.arguments.push(&trueEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(false));
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("not");
		compEx.makeComp();
		compEx.arguments.push(&falseEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("and");
		compEx.makeComp();
		compEx.arguments.push(&falseEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(false));
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("and");
		compEx.makeComp();
		compEx.arguments.push(&falseEx);
		compEx.arguments.push(&trueEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(false));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("and");
		compEx.makeComp();
		falseEx.setBool(true);
		compEx.arguments.push(&falseEx);
		compEx.arguments.push(&trueEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
		falseEx.setBool(false);
	}
	{
		Environment test;
		compEx.setSymbol("or");
		compEx.makeComp();
		compEx.arguments.push(&falseEx);
		compEx.arguments.push(&trueEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("or");
		compEx.makeComp();
		compEx.arguments.push(&falseEx);
		compEx.arguments.push(&doubleEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result.getType().empty() == true);
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("and");
		compEx.makeComp();
		compEx.arguments.push(&falseEx);
		compEx.arguments.push(&doubleEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result.getType().empty() == true);
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("not");
		compEx.makeComp();
		compEx.arguments.push(&falseEx);
		compEx.arguments.push(&doubleEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result.getType().empty() == true);
		compEx.arguments.pop();
		compEx.arguments.pop();
	}
	{
		Environment test;
		compEx.setSymbol("not");
		compEx.makeComp();
		compEx.arguments.push(&doubleEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result.getType().empty() == true);
		compEx.arguments.pop();
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&doubleEx);
		test.evaluateDefine(defineEx);
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		compEx.setSymbol("not");
		compEx.makeComp();
		compEx.arguments.push(&aEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result.getType().empty() == true);
		compEx.arguments.pop();
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&trueEx);
		test.evaluateDefine(defineEx);
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		compEx.setSymbol("not");
		compEx.makeComp();
		compEx.arguments.push(&aEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(false));
		compEx.arguments.pop();
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&trueEx);
		test.evaluateDefine(defineEx);
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		compEx.setSymbol("and");
		compEx.makeComp();
		compEx.arguments.push(&aEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
	}
	{
		Environment test;
		defineEx.arguments.push(&aEx);
		defineEx.arguments.push(&trueEx);
		test.evaluateDefine(defineEx);
		defineEx.arguments.pop();
		defineEx.arguments.pop();
		compEx.setSymbol("or");
		compEx.makeComp();
		compEx.arguments.push(&aEx);
		result = test.evaluateComp(compEx);
		REQUIRE(result == Expression(true));
		compEx.arguments.pop();
	}
}
