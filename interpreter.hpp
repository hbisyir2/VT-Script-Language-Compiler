#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "expression.hpp"
#include "tokenize.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"
#include <iostream>
#include <queue>

class Interpreter {
public:
	// Default construct an Interpreter with the default environment and an empty AST
	Interpreter();

	// Given a vtscript program as a std::istream, attempt to parse into an internal AST
	// return true on success, false on failure
	bool parse(std::istream & expression) noexcept;

	//bool parsePrompt(std::string stringFromPrompt);

	// Evaluate the current AST and return the resulting Expression
	// throws InterpreterSemanticError if a semantic error is encountered
	// the exception message string should document the nature of the semantic error 
	Expression eval();

	std::string getErrorMessage();
	
private:
	// consider using a queue instead of stack
	Expression head;
	Environment finalEval;
	std::string errorMessage;
};

#endif // _INTERPRETER_H_