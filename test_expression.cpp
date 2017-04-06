#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "expression.hpp"

TEST_CASE("Test different constructors", "[expression]") {
	{
		Expression empty;
		REQUIRE(empty.getType().empty() == true);
	}
	{
		bool test = true;
		Expression expBool(test);
		REQUIRE(expBool.getType() == "bool");
		REQUIRE(expBool.getBool() == true);
	}
	{
		double test = 6.0;
		Expression expDub(test);
		REQUIRE(expDub.getType() == "double");
		REQUIRE(expDub.getDouble() == 6.0);
	}
	{
		std::string test = "testString";
		Expression expSym(test);
		REQUIRE(expSym.getType() == "symbol");
		REQUIRE(expSym.getSymbol() == "testString");
	}
}

TEST_CASE("test setting functions", "[expression]") {
	{
		Expression testEx;
		testEx.setType("double");
		testEx.setDouble(10.1);
		REQUIRE(testEx.getType() == "double");
		REQUIRE(testEx.getDouble() == 10.1);
	}
	{
		Expression testEx;
		testEx.setType("symbol");
		testEx.setSymbol("changeSymbolTo");
		REQUIRE(testEx.getType() == "symbol");
		REQUIRE(testEx.getSymbol() == "changeSymbolTo");
	}
	{
		Expression testEx;
		testEx.setType("bool");
		testEx.setBool(true);
		REQUIRE(testEx.getType() == "bool");
		REQUIRE(testEx.getBool() == true);
	}
}

TEST_CASE("test functions that change parameters", "[expression]") {
	{
		Expression testEx;
		REQUIRE(testEx.isOp() == false);
		testEx.makeOp();
		REQUIRE(testEx.isOp() == true);
	}
	{
		Expression testEx;
		REQUIRE(testEx.isComp() == false);
		testEx.makeComp();
		REQUIRE(testEx.isComp() == true);
	}
	{
		Expression testEx;
		REQUIRE(testEx.isHead() == false);
		testEx.makeHead();
		REQUIRE(testEx.isHead() == true);
	}
}

TEST_CASE("test assigning children and parent", "[expression]") {
	Expression testEx;
	Expression child1;
	Expression child2;
	Expression parent;
	REQUIRE(testEx.arguments.size() == 0);
	REQUIRE_NOTHROW(testEx.arguments.push(&child1));
	REQUIRE(testEx.arguments.size() == 1);
	REQUIRE_NOTHROW(testEx.arguments.push(&child2));
	REQUIRE(testEx.arguments.size() == 2);
	REQUIRE_NOTHROW(testEx.parent = &parent);
}

TEST_CASE("test == operator", "[expression]") {
	Expression equal1;
	Expression equal2;
	Expression child;
	bool result;
	result = (equal1 == equal2);
	REQUIRE(result == true);

	equal1.setType("double");
	equal2.setType("bool");
	result = (equal1 == equal2);
	REQUIRE(result == false);

	equal2.setType("double");
	result = (equal1 == equal2);
	REQUIRE(result == true);

	equal1.setDouble(5.0);
	equal2.setDouble(5.2);
	result = (equal1 == equal2);
	REQUIRE(result == false);

	equal2.setDouble(5.0);
	result = (equal1 == equal2);
	REQUIRE(result == true);

	equal1.setType("bool");
	equal2.setType("bool");
	result = (equal1 == equal2);
	REQUIRE(result == true);

	equal1.setBool(true);
	equal2.setBool(false);
	result = (equal1 == equal2);
	REQUIRE(result == false);

	equal1.setBool(false);
	result = (equal1 == equal2);
	REQUIRE(result == true);

	equal1.setType("symbol");
	equal2.setType("symbol");
	result = (equal1 == equal2);
	REQUIRE(result == true); 

	equal1.setSymbol("test1");
	equal2.setSymbol("test2");
	result = (equal1 == equal2);
	REQUIRE(result == false);

	equal2.setSymbol("test1");
	result = (equal1 == equal2);
	REQUIRE(result == true);

	equal1.arguments.push(&child);
	result = (equal1 == equal2);
	REQUIRE(result == false);

	equal2.arguments.push(&child);
	result = (equal1 == equal2);
	REQUIRE(result == true);
}