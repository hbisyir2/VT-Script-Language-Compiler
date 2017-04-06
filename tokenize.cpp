#include "tokenize.hpp"

Tokenizer::Tokenizer(): openP(0), closeP(0), getBegin(false), head(nullptr), errorMessage("") {
	head = new Expression;
};

bool Tokenizer::parser(std::string tokenized) {

	Expression* temp;
	bool atomBool;
	double atomDouble;
	std::string::size_type sz;
	if (tokenized.length() == 0) return true;
	if (end == true) {
		errorMessage = "Parse Error: too many arguments";
		return false;
	}
	if (tokenized == "(") {
		if (getBegin) {
			errorMessage = "Parse Error: invalid parenthesis";
			return false;
		}
		openP++;
		getBegin = true;
	}
	else if (tokenized == ")") {
		closeP++;
		if (getBegin) {
			errorMessage = "Parse Error: invalid parenthesis";
			return false;
		}
		if (!place->isHead()) place = place->parent;
		else end = true;
	}
	else if (tokenized == "True") {
		atomBool = true;
		Expression toAdd(atomBool);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
			}
		}
	}
	else if (tokenized == "False") {
		atomBool = false;
		Expression toAdd(atomBool);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
			}
		}
	}
	// if atom is a number
	else if ((int)tokenized.at(0) >= 48 && (int)tokenized.at(0) <= 57) { 
		int eCount = 0;
		int periodCount = 0;
		if (tokenized.at(tokenized.length() - 1) == 'e') {
			errorMessage = "Parse Error: invalid variable name " + tokenized;
			return false;
		}
		for (int i = 0; i < tokenized.size(); i++) {
			if (((int)tokenized.at(i) < 48 || (int)tokenized.at(i) > 57) && tokenized.at(i) != '.' && tokenized.at(i) != 'e') {
				errorMessage = "Parse Error: invalid variable name " + tokenized;
				return false;
			}
			if (tokenized.at(i) == '.') {
				if (eCount > 0) {
					errorMessage = "Parse Error: invalid variable name " + tokenized;
					return false;
				}
				periodCount++;
			}
			if (tokenized.at(i) == 'e') eCount++;
		}
		if (periodCount > 1 || eCount > 1) {
			errorMessage = "Parse Error: invalid variable name " + tokenized;
			return false;
		}
		atomDouble = std::stod(tokenized, &sz);
		Expression toAdd(atomDouble);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
			}
		}
	}
	// if atom is a negative number
	else if (tokenized.size() > 1 && tokenized.at(0) == '-' && (int)tokenized.at(1) >= 48 && (int)tokenized.at(1) <= 57) {
		int eCount = 0;
		int periodCount = 0;
		if (tokenized.at(tokenized.length() - 1) == 'e') {
			errorMessage = "Parse Error: invalid variable name " + tokenized;
			return false;
		}
		for (int i = 1; i < tokenized.size(); i++) {
			if (((int)tokenized.at(i) < 48 || (int)tokenized.at(i) > 57) && tokenized.at(i) != '.' && tokenized.at(i) != 'e') {
				errorMessage = "Parse Error: invalid variable name " + tokenized;
				return false;
			}
			if (tokenized.at(i) == '.') {
				if (eCount > 0) {
					errorMessage = "Parse Error: invalid variable name " + tokenized;
					return false;
				}
				periodCount++;
			}
			if (tokenized.at(i) == 'e') eCount++;
		}
		if (periodCount > 1 || eCount > 1) {
			errorMessage = "Parse Error: invalid variable name " + tokenized;
			return false;
		}
		atomDouble = std::stod(tokenized, &sz);
		Expression toAdd(atomDouble);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
			}
		}
	}
	// if token is a special form
	else if (tokenized == "define" || tokenized == "begin" || tokenized == "if") {
		Expression toAdd(tokenized);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
			}
		}
	}
	// if token is a comparator operator
	else if (tokenized == "not" || tokenized == "and" || tokenized == "or" || tokenized == "<" ||
		tokenized == "<=" || tokenized == ">" || tokenized == ">=" || tokenized == "=") {
		Expression toAdd(tokenized);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
			place->makeComp();
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->makeComp();
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
				temp->makeComp();
			}
		}
	}
	// if token is a numerical operator
	else if (tokenized == "+" || tokenized == "-" || tokenized == "*" || tokenized == "/") {
		Expression toAdd(tokenized);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
			place->makeOp();
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->makeOp();
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
				temp->makeOp();
			}
		}
	}
	// if token is pi
	else if (tokenized == "pi") {
		double piVal = atan2(0, -1);
		Expression toAdd(piVal);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
			place->setSymbol("pi");
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->setSymbol("pi");
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
				temp->setSymbol("pi");
			}
		}
	}
	// all other symbols
	else {
		Expression toAdd(tokenized);
		if (head->getType().empty()) {
			if (!getBegin) {
				errorMessage = "Parse Error: expected a '('";
				return false;
			}
			getBegin = false;
			head = new Expression(toAdd);
			place = head;
			place->makeHead();
		}
		else {
			place->arguments.push(new Expression(toAdd));
			if (getBegin) {
				temp = place;
				place = place->arguments.back();
				place->parent = temp;
				getBegin = false;
			}
			else {
				temp = place->arguments.back();
				temp->parent = place;
			}
		}
	}
	return true;
}

int Tokenizer::getOpenP() {
	return openP;
}

int Tokenizer::getCloseP() {
	return closeP;
}

Expression Tokenizer::getHead() {
	return *head;
}

std::string Tokenizer::getErrorMessage() {
	return errorMessage;
}