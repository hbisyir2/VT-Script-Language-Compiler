#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <queue>
#include <map>
#include <sstream>
#include <iostream>
#include <stack>
#include <math.h>
#include "expression.hpp"

class Tokenizer {
public:

	// constructor
	Tokenizer();
	
	// parses strings and pushes them to the stack
	bool parser(std::string wordsToSplit);

	int getOpenP();

	int getCloseP();

	Expression getHead();

	std::string getErrorMessage();

private:
	Expression* head;
	Expression* place;
	bool end;
	bool getBegin;
	std::string errorMessage;
	int openP;
	int closeP;
};

#endif // _TOKENIZER_H_