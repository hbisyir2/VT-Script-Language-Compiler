#include "tokenize.hpp"
#include "expression.hpp"
#include <iostream>
#include <queue>
#include <stack>
#include <map>

class Environment {
public:
	
	// constructor
	Environment();
	
	// function to run the ast given by both queues
	// Expression runAST(std::queue<Expression> expressions, std::queue<Expression> symbols);
	Expression runAST();

	Expression evaluateBegin(Expression begin);

	Expression evaluateEx(Expression op);

	Expression evaluateIf(Expression ifStart);

	Expression evaluateComp(Expression comparator);

	Expression evaluateDefine(Expression define);

	void setHead(Expression newHead);

	void clearEnvironment();

	std::string getErrorMessage();

private:
	std::map<std::string, std::string> defined;
	std::map<std::string, double> definedDub;
	std::map<std::string, bool> definedBool;
	Expression head;
	std::string errorMessage;
};