#include "expression.hpp"

Expression::Expression() : type(""), boolVal(false), doubleVal(0), symbolVal(""), op(false), comp(false), head(false), parent(nullptr) {}

Expression::Expression(bool value) : type("bool"), boolVal(value), doubleVal(0), symbolVal(""), op(false), comp(false), head(false), parent(nullptr) {}

Expression::Expression(double value) : type("double"), boolVal(false), doubleVal(value), symbolVal(""), op(false), comp(false), head(false), parent(nullptr) {}

Expression::Expression(const std::string & value) : type("symbol"), boolVal(false), doubleVal(0), symbolVal(value), op(false), comp(false), head(false), parent(nullptr) {}

bool Expression::operator==(const Expression & exp) const noexcept {
	if (this->type != exp.type) return false;
	if (this->type == "bool") {
		if (this->boolVal != exp.boolVal) return false;
	}
	if (this->type == "double") {
		if (this->doubleVal != exp.doubleVal) return false;
	}
	if (this->type == "symbol") {
		if (this->symbolVal != exp.symbolVal) return false;
	}
	if (this->arguments.size() != exp.arguments.size()) return false;
	return true;
}

std::string Expression::getSymbol() {
	return symbolVal;
}

void Expression::makeOp() {
	op = true;
}

bool Expression::isOp() {
	return op;
}

double Expression::getDouble() {
	return doubleVal;
}

std::string Expression::getType() {
	return type;
}

bool Expression::getBool() {
	return boolVal;
}

void Expression::setDouble(double newDouble) {
	doubleVal = newDouble;
}

void Expression::setBool(bool newBool) {
	boolVal = newBool;
}

void Expression::setType(std::string newType) {
	type = newType;
}

bool Expression::isComp() {
	return comp;
}

void Expression::makeComp() {
	comp = true;
}

bool Expression::isHead() {
	return head;
}

void Expression::makeHead() {
	head = true;
}

void Expression::setSymbol(std::string newSymbol) {
	symbolVal = newSymbol;
}