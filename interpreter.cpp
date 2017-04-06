#include "interpreter.hpp"

Interpreter::Interpreter() : errorMessage("") {}

bool Interpreter::parse(std::istream & expression) noexcept {
	std::string wordFromFile;
	std::string parsedWord;
	Tokenizer parsing;
	bool spaceInserted;
	bool hadInput = false;
	char firstChar = ' ';
	if (!expression.good()) {
		errorMessage = "Parse Error: invalid input file";
		return false;
	}
	while (expression >> wordFromFile) {
		if (wordFromFile.size() != 0 && wordFromFile.at(0) == ';') {
			std::getline(expression, wordFromFile);
			continue;
		}
		hadInput = true;
		spaceInserted = false;
		// checks to make sure first character is (
		if (firstChar == ' ') {
			while (wordFromFile.at(0) == '\n' || wordFromFile.at(0) == '\t' || wordFromFile.at(0) == '\r' || wordFromFile.at(0) == ' ') {
				wordFromFile.erase(0, 1);
			}
			firstChar = wordFromFile.at(0);
			if (firstChar != '(' && firstChar != ';') {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
		}
		for (size_t i = 0; i < wordFromFile.length(); i++) {
			if (wordFromFile.at(i) == '(') {
				wordFromFile.insert(i, " ");
				i++;
				i++;
				wordFromFile.insert(i, " ");
				spaceInserted = true;
			}
			else if (wordFromFile.at(i) == ')') {
				wordFromFile.insert(i, " ");
				i++;
				i++;
				wordFromFile.insert(i, " ");
				spaceInserted = true;
			}
		}
		if (spaceInserted) {
			std::istringstream stringStream(wordFromFile);
			while (std::getline(stringStream, parsedWord, ' ')) {
				if (!parsing.parser(parsedWord)) {
					errorMessage = parsing.getErrorMessage();
					return false;
				}
			}
		}
		else if (!parsing.parser(wordFromFile)) {
			return false;
		}
	}
	if (!hadInput) {
		errorMessage = "Parse Error: empty input";
		return false;
	}
	if (parsing.getOpenP() > parsing.getCloseP()) {
		errorMessage = "Parse Error: expected a ')'";
		return false;
	}
	else if (parsing.getOpenP() < parsing.getCloseP()) {
		errorMessage = "Parse Error: expected a ')'";
		return false;
	}
	head = parsing.getHead();
	return true;
}

Expression Interpreter::eval() {
	finalEval.setHead(head);
	Expression evalReturn = finalEval.runAST();
	if (evalReturn.getType().empty()) {
		finalEval.clearEnvironment();
		throw InterpreterSemanticError(finalEval.getErrorMessage());
	}
	return evalReturn;
}

std::string Interpreter::getErrorMessage() {
	return errorMessage;
}