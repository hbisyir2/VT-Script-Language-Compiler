#include "catch.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "tokenize.hpp"

TEST_CASE("Test Tokenizer with empty input", "[tokenizer]") {
	
	Tokenizer test;
	
	std::string emptyInput = "";
	
	bool result;
	
	result = test.parser(emptyInput);
	
	REQUIRE(result == true);
}

TEST_CASE("test with empty expressions", "[tokenizer]") {
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			// should return false on close parenthesis
			if (i == 2) REQUIRE(result == false);
			else REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "(", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			// should return false on second open parenthesis
			if (i == 1) {
				REQUIRE(result == false);
				break;
			}
			else REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", ")", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			// should return false on close parenthesis
			if (i == 1) {
				REQUIRE(result == false);
				break;
			}
			else REQUIRE(result == true);
		}
	}
}

TEST_CASE("test with atoms with no parenthesis", "[tokenizer]") {
	{
		Tokenizer test;
		bool result;
		std::string input = "a";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "1.9";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "-20.7";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "1e5";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "True";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "False";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "begin";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "define";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "if";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "*";
		result = test.parser(input);
		REQUIRE(result == false);
	}
	{
		Tokenizer test;
		bool result;
		std::string input = "pi";
		result = test.parser(input);
		REQUIRE(result == false);
	}
}

TEST_CASE("test valid single atom inputs", "[tokenizer]") {
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = {"(", "a", ")"};
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "1.9", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "-20.7", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "1e5", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "True", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "False", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "begin", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "define", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "if", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "*", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "pi", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getOpenP() == 1);
		REQUIRE(test.getCloseP() == 1);
	}
}

TEST_CASE("test expressions with arguments", "[tokenizer]") {
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "define", "a", "100", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "if", "a", "100", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "1.0", "a", "100", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "True", "a", "100", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "/", "a", "100", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "-6.5", "a", "100", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "pi", "a", "100", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "False", "a", "100", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "someSymbol", "testWord", "50", ")" };
		for (int i = 0; i < 5; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "begin", "(", "a", ")", "(", "100", ")", ")" };
		for (int i = 0; i < 9; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "begin", "(", "True", ")", "(", "False", ")", ")" };
		for (int i = 0; i < 9; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "begin", "(", "if", ")", "(", "define", ")", ")" };
		for (int i = 0; i < 9; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	Tokenizer test;
	bool result;
	std::string inputs[] = { "(", "begin", "(", "-4.5", ")", "(", "/", ")", ")" };
	for (int i = 0; i < 9; i++) {
		result = test.parser(inputs[i]);
		REQUIRE(result == true);
	}
}

TEST_CASE("test arguments within arguments", "[tokenizer]") {
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "begin", "(", "define", "b", "20", ")", "6.4", ")" };
		for (int i = 0; i < 9; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getHead().getSymbol() == "begin");
		REQUIRE(test.getOpenP() == 2);
		REQUIRE(test.getCloseP() == 2);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "begin", "9.0", "(", "define", "b", "20", ")", ")" };
		for (int i = 0; i < 9; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getHead().getSymbol() == "begin");
		REQUIRE(test.getOpenP() == 2);
		REQUIRE(test.getCloseP() == 2);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "begin", "(", "define", "longerWord", "(", "begin", "100", "(", "*", "100", "100", ")", ")", ")", "True", ")" };
		for (int i = 0; i < 17; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getHead().getSymbol() == "begin");
		REQUIRE(test.getOpenP() == 4);
		REQUIRE(test.getCloseP() == 4);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "500", "(", "define", "longerWord", "(", "begin", "100", "(", "t", "100", "100", ")", ")", ")", "True", ")" };
		for (int i = 0; i < 17; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getHead().getDouble() == 500);
		REQUIRE(test.getOpenP() == 4);
		REQUIRE(test.getCloseP() == 4);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "1e5", "(", "False", "longerWord", "(", "begin", "100", "(", "*", "100", "100", ")", ")", ")", "True", ")" };
		for (int i = 0; i < 17; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getHead().getDouble() == 1e5);
		REQUIRE(test.getOpenP() == 4);
		REQUIRE(test.getCloseP() == 4);
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "-70", "(", "True", "False", "(", ">=", "100", "(", "-8e2", "100", "100", ")", ")", ")", "undefinedSymbol", ")" };
		for (int i = 0; i < 17; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
		REQUIRE(test.getHead().getDouble() == -70.);
		REQUIRE(test.getOpenP() == 4);
		REQUIRE(test.getCloseP() == 4);
	}
}

TEST_CASE("test symbol names", "[tokenizer]") {
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "8startWithNumber", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			if (i == 1) REQUIRE(result == false);
			else if (i == 0) REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "4.5.6", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			if (i == 1) REQUIRE(result == false);
			else if (i == 0) REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "1e5e3", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			if (i == 1) REQUIRE(result == false);
			else if (i == 0) REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "-9.3.4", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			if (i == 1) REQUIRE(result == false);
			else if (i == 0) REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "-8.3e3.5", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			if (i == 1) REQUIRE(result == false);
			else if (i == 0) REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "2.1e6.7", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			if (i == 1) REQUIRE(result == false);
			else if (i == 0) REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "8e2.1", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			if (i == 1) REQUIRE(result == false);
			else if (i == 0) REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "-8e2.1", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			if (i == 1) REQUIRE(result == false);
			else if (i == 0) REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "valid8Name", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "another_valid_name", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	{
		Tokenizer test;
		bool result;
		std::string inputs[] = { "(", "valid.name_78*", ")" };
		for (int i = 0; i < 3; i++) {
			result = test.parser(inputs[i]);
			REQUIRE(result == true);
		}
	}
	Tokenizer test;
	bool result;
	std::string inputs[] = { "(", "100.", ")" };
	for (int i = 0; i < 3; i++) {
		result = test.parser(inputs[i]);
		REQUIRE(result == true);
	}
}