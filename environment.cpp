#include "environment.hpp"

Environment::Environment() : errorMessage("") {}

Expression Environment::runAST() {
	Expression toReturn;
	Expression toReturnError;

	// if the expression is an operator or an if statement
	if (head.isOp()) {
		toReturn = evaluateEx(head);
		if (toReturn.getType().empty()) {
			return toReturnError;
		}
		return toReturn;
	}
	// if the expression is define
	else if (head.getSymbol() == "define") {
		toReturn = evaluateDefine(head);
		if (toReturn.getType().empty()) {
			return toReturnError;
		}
		return toReturn;
	}
	// if the first expression is begin
	else if (head.getSymbol() == "begin") {
		toReturn = evaluateBegin(head);
		if (toReturn.getType().empty()) {
			return toReturnError;
		}
		return toReturn;
	}
	// if the first expression is if
	else if (head.getSymbol() == "if") {
		toReturn = evaluateIf(head);
		if (toReturn.getType().empty()) {
			return toReturnError;
		}
		return toReturn;
	}
	// if the first expression is a comparator
	else if (head.isComp()) {
		toReturn = evaluateComp(head);
		if (toReturn.getType().empty()) {
			return toReturnError;
		}
		return toReturn;
	}
	// if the first expression is a symbol
	else if (head.getType() == "symbol") {
		if (head.arguments.size() > 0) {
			errorMessage = "Eval Error: type does not support arguments";
			return toReturnError;
		}
		if (defined.count(head.getSymbol()) == 1) {
			auto search = defined.find(head.getSymbol());
			if (search->second == "double") {
				auto search = definedDub.find(head.getSymbol());
				toReturn.setType("double");
				toReturn.setDouble(search->second);
				return toReturn;
			}
			else if (search->second == "bool") {
				auto search = definedBool.find(head.getSymbol());
				toReturn.setType("bool");
				toReturn.setBool(search->second);
				return toReturn;
			}
		}
		else {
			errorMessage = "Eval Error: " + head.getSymbol() + " not recognized";
			return toReturnError;
		}
	}
	// if the first expression is a double or bool
	else {
		if (head.getType() == "double") {
			if (head.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
			Expression toReturn(head.getDouble());
			return toReturn;
		}
		else if (head.getType() == "bool") {
			if (head.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
			Expression toReturn(head.getBool());
			return toReturn;
		}
	}
	return toReturn;
}

Expression Environment::evaluateBegin(Expression begin) {
	Expression toReturnError;
	Expression currentArg;
	Expression toReturn;
	if (begin.arguments.size() <= 0) {
		errorMessage = "Eval Error: invalid number of arguments for begin";
		return toReturnError;
	}
	while (begin.arguments.size() > 0) {
		currentArg = *begin.arguments.front();
		begin.arguments.pop();
		if (currentArg.getSymbol() == "begin") {
			toReturn = evaluateBegin(currentArg);
			if (toReturn.getType().empty()) return toReturnError;
		}
		else if (currentArg.getSymbol() == "define") {
			toReturn = evaluateDefine(currentArg);
			if (toReturn.getType().empty()) return toReturnError;
		}
		else if (currentArg.getSymbol() == "if") {
			toReturn = evaluateIf(currentArg);
			if (toReturn.getType().empty()) return toReturnError;
		}
		else if (currentArg.isOp()) {
			toReturn = evaluateEx(currentArg);
			if (toReturn.getType().empty()) return toReturnError;
		}
		else if (currentArg.isComp()) {
			toReturn = evaluateComp(currentArg);
			if (toReturn.getType().empty()) return toReturnError;
		}
		else if (currentArg.getType() == "symbol") {
			if (currentArg.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
			if (defined.count(currentArg.getSymbol()) == 1) {
				auto search = defined.find(currentArg.getSymbol());
				if (search->second == "double") {
					auto search = definedDub.find(currentArg.getSymbol());
					toReturn.setType("double");
					toReturn.setDouble(search->second);
				}
				else if (search->second == "bool") {
					auto search = definedBool.find(currentArg.getSymbol());
					toReturn.setType("bool");
					toReturn.setBool(search->second);
				}
			}
			else {
				errorMessage = "Eval Error: " + currentArg.getSymbol() + " not recognized";
				return toReturnError;
			}
		}
		else {
			if (currentArg.getType() == "bool") {
				if (currentArg.arguments.size() > 0) {
					errorMessage = "Eval Error: type does not support arguments";
					return toReturnError;
				}
				toReturn.setType("bool");
				toReturn.setBool(currentArg.getBool());
			}
			else if (currentArg.getType() == "double") {
				if (currentArg.arguments.size() > 0) {
					errorMessage = "Eval Error: type does not support arguments";
					return toReturnError;
				}
				toReturn.setType("double");
				toReturn.setDouble(currentArg.getDouble());
			}
		}
	}
	return toReturn;
}

Expression Environment::evaluateEx(Expression op) {
	Expression toReturnError;
	Expression arg;
	double math;
	bool firstArg = true;
	int argSize = op.arguments.size();
	// ensure operator has correct number of arguments
	if (op.arguments.size() <= 0) {
		errorMessage = "Eval Error: invalid number of arguments for " + op.getSymbol();
		return toReturnError;
	}
	else if (op.getSymbol() == "-" && op.arguments.size() > 2) {
		errorMessage = "Eval Error: invalid number of arguments for " + op.getSymbol();
		return toReturnError;
	}
	else if (op.getSymbol() == "/" && op.arguments.size() != 2) {
		errorMessage = "Eval Error: invalid number of arguments for " + op.getSymbol();
		return toReturnError;
	}
	while (op.arguments.size() > 0) {
		arg = *op.arguments.front();
		op.arguments.pop();
		if (arg.getSymbol() == "define") {
			arg = evaluateDefine(arg);
			if (arg.getType().empty()) return toReturnError;
		}
		else if (arg.getSymbol() == "if") {
			arg = evaluateIf(arg);
			if (arg.getType().empty()) return toReturnError;
		}
		else if (arg.isOp()) {
			arg = evaluateEx(arg);
			if (arg.getType().empty()) return toReturnError;
		}
		else if (arg.isComp()) {
			errorMessage = "Eval Error: cannot evaluate " + arg.getSymbol() + " with bool type";
			return toReturnError;
		}
		else if (arg.getSymbol() == "begin") {
			arg = evaluateBegin(arg);
			if (arg.getType().empty()) return toReturnError;
		}
		else if (arg.getType() == "bool") {
			if (arg.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
			errorMessage = "Eval Error: cannot evaluate " + arg.getSymbol() + " with bool type";
			return toReturnError;
		}
		else if (arg.getType() == "symbol") {
			if (arg.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
			if (defined.count(arg.getSymbol()) == 1) {
				auto search = defined.find(arg.getSymbol());
				if (search->second == "double") {
					auto search = definedDub.find(arg.getSymbol());
					arg.setType("double");
					arg.setDouble(search->second);
				}
				else if (search->second == "bool") {
					errorMessage = "Eval Error: invalid arguments for operator " + op.getSymbol();
					return toReturnError;
				}
			}
			else {
				errorMessage = "Eval Error: " + arg.getSymbol() + " not recognized";
				return toReturnError;
			}
		}
		if (arg.getType() == "double") {
			if (arg.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
			if (firstArg) {
				math = arg.getDouble();
				firstArg = false;
				if (argSize == 1 && op.getSymbol() == "-") {
					math *= -1;
				}
			}
			else {
				if (op.getSymbol() == "+") math += arg.getDouble();
				else if (op.getSymbol() == "-") {
					math -= arg.getDouble();
				}
				else if (op.getSymbol() == "*") math *= arg.getDouble();
				else if (op.getSymbol() == "/") math /= arg.getDouble();
			}
		}
	}
	Expression toReturn(math);
return toReturn;
}

Expression Environment::evaluateIf(Expression ifStart) {
	Expression toReturn, toReturnError;
	Expression comp, ifTrue, ifFalse;
	Expression evaluate;
	if (ifStart.arguments.size() != 3) {
		errorMessage = "Eval Error: invalid arguments for if";
		return toReturnError;
	}
	comp = *ifStart.arguments.front();
	ifStart.arguments.pop();
	ifTrue = *ifStart.arguments.front();
	ifStart.arguments.pop();
	ifFalse = *ifStart.arguments.front();
	ifStart.arguments.pop();
	if (comp.getType() == "bool");
	else if (comp.getType() == "double" || comp.isOp()) {
		errorMessage = "Eval Error: invalid arguments for if";
		return toReturnError;
	}
	else if (comp.isComp()) {
		comp = evaluateComp(comp);
		if (comp.getType().empty()) return toReturnError;
	}
	else if (comp.getSymbol() == "define") {
		comp = evaluateDefine(comp);
		if (comp.getType().empty()) return toReturnError;
		else if (comp.getType() == "double") {
			errorMessage = "Eval Error: if cannot evaluate this statement";
			return toReturnError;
		}
	}
	else if (comp.getSymbol() == "begin") {
		comp = evaluateBegin(comp);
		if (comp.getType().empty()) return toReturnError;
		else if (comp.getType() == "double") {
			errorMessage = "Eval Error: if cannot evaluate this statement";
			return toReturnError;
		}
	}
	else if (comp.getSymbol() == "if") {
		comp = evaluateIf(comp);
		if (comp.getType().empty()) return toReturnError;
		else if (comp.getType() == "double") {
			errorMessage = "Eval Error: if cannot evaluate this statement";
			return toReturnError;
		}
	}
	else if (comp.getType() == "symbol") {
		if (comp.arguments.size() > 0) {
			errorMessage = "Eval Error: type does not support arguments";
			return toReturnError;
		}
		if (defined.count(comp.getSymbol()) == 1) {
			auto search = defined.find(comp.getSymbol());
			if (search->second == "double") {
				errorMessage = "Eval Error: first argument of if must be type bool";
				return toReturnError;
			}
			else if (search->second == "bool") {
				auto search = definedBool.find(comp.getSymbol());
				comp.setType("bool");
				comp.setBool(search->second);
			}
		}
		else {
			errorMessage = "Eval Error: " + comp.getSymbol() + " is not defined";
			return toReturnError;
		}
	}
	if (comp.getBool()) evaluate = ifTrue;
	else evaluate = ifFalse;
	if (evaluate.getSymbol() == "define") {
		toReturn = evaluateDefine(evaluate);
		return toReturn;
	}
	else if (evaluate.getSymbol() == "begin") {
		toReturn = evaluateBegin(evaluate);
		return toReturn;
	}
	else if (evaluate.getSymbol() == "if") {
		toReturn = evaluateIf(evaluate);
		return toReturn;
	}
	else if (evaluate.isOp()) {
		toReturn = evaluateEx(evaluate);
		return toReturn;
	}
	else if (evaluate.isComp()) {
		toReturn = evaluateComp(evaluate);
		return toReturn;
	}
	else if (evaluate.getType() == "double" || evaluate.getType() == "bool") {
		if (evaluate.arguments.size() > 0) {
			errorMessage = "Eval Error: type does not support arguments";
			return toReturnError;
		}
		return evaluate;
	}
	else if (evaluate.getType() == "symbol") {
		if (evaluate.arguments.size() > 0) {
			errorMessage = "Eval Error: type does not support arguments";
			return toReturnError;
		}
		if (defined.count(evaluate.getSymbol()) == 1) {
			auto search = defined.find(evaluate.getSymbol());
			if (search->second == "double") {
				auto search = definedDub.find(evaluate.getSymbol());
				toReturn.setType("double");
				toReturn.setDouble(search->second);
			}
			else if (search->second == "bool") {
				auto search = definedBool.find(evaluate.getSymbol());
				toReturn.setType("bool");
				toReturn.setBool(search->second);
			}
			return toReturn;
		}
		else {
			errorMessage = "Eval Error: " + evaluate.getSymbol() + " not recognized";
			return toReturnError;
		}
	}

	return toReturn;
}

Expression Environment::evaluateComp(Expression comparator) {
	Expression toReturnError;
	Expression toReturn;
	if (comparator.getSymbol() == "and") toReturn.setBool(true);
	else toReturn.setBool(false);
	int argNum = comparator.arguments.size();
	// make sure comparator has right number of arguments
	if (argNum <= 0) {
		errorMessage = "Eval Error: invalid arguments for " + comparator.getSymbol();
		return toReturnError;
	}
	else if (comparator.getSymbol() == "not" && argNum != 1) {
		errorMessage = "Eval Error: invalid arguments for " + comparator.getSymbol();
		return toReturnError;
	}
	else if (comparator.getSymbol() == "<" && argNum != 2) {
		errorMessage = "Eval Error: invalid arguments for " + comparator.getSymbol();
		return toReturnError;
	}
	else if (comparator.getSymbol() == "<=" && argNum != 2) {
		errorMessage = "Eval Error: invalid arguments for " + comparator.getSymbol();
		return toReturnError;
	}
	else if (comparator.getSymbol() == ">" && argNum != 2) {
		errorMessage = "Eval Error: invalid arguments for " + comparator.getSymbol();
		return toReturnError;
	}
	else if (comparator.getSymbol() == ">=" && argNum != 2) {
		errorMessage = "Eval Error: invalid arguments for " + comparator.getSymbol();
		return toReturnError;
	}
	else if (comparator.getSymbol() == "=" && argNum != 2) {
		errorMessage = "Eval Error: invalid arguments for " + comparator.getSymbol();
		return toReturnError;
	}
	while (!comparator.arguments.empty()) {
		Expression arg1 = *comparator.arguments.front();
		comparator.arguments.pop();
		if (arg1.getSymbol() == "define") {
			arg1 = evaluateDefine(arg1);
			if (arg1.getType().empty()) return toReturnError;
		}
		else if (arg1.getSymbol() == "begin") {
			arg1 = evaluateBegin(arg1);
			if (arg1.getType().empty()) return toReturnError;
		}
		else if (arg1.getSymbol() == "if") {
			arg1 = evaluateIf(arg1);
			if (arg1.getType().empty()) return toReturnError;
		}
		else if (arg1.isComp()) {
			arg1 = evaluateComp(arg1);
			if (arg1.getType().empty()) return toReturnError;
		}
		else if (arg1.isOp()) {
			if (comparator.getSymbol() == "and" || comparator.getSymbol() == "or" || comparator.getSymbol() == "not") {
				errorMessage = "Eval Error: cannot evaluate " + comparator.getSymbol() + " with type double";
				return toReturnError;
			}
			arg1 = evaluateEx(arg1);
			if (arg1.getType().empty()) return toReturnError;
		}
		else if (arg1.getType() == "double") {
			if (arg1.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
			if (comparator.getSymbol() == "and" || comparator.getSymbol() == "or" || comparator.getSymbol() == "not") {
				errorMessage = "Eval Error: cannot evaluate " + comparator.getSymbol() + " with type double";
				return toReturnError;
			}
		}
		else if (arg1.getType() == "symbol") {
			if (arg1.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
			if (defined.count(arg1.getSymbol()) == 1) {
				auto search = defined.find(arg1.getSymbol());
				if (search->second == "double") {
					if (comparator.getSymbol() == "and" || comparator.getSymbol() == "or" || comparator.getSymbol() == "not") {
						errorMessage = "Eval Error: cannot evaluate " + comparator.getSymbol() + " with type double";
						return toReturnError;
					}
					else {
						auto search = definedDub.find(arg1.getSymbol());
						arg1.setType("double");
						arg1.setDouble(search->second);
					}
				}
				else if (search->second == "bool") {
					auto search = definedBool.find(arg1.getSymbol());
					arg1.setType("bool");
					arg1.setBool(search->second);
				}
			}
			else {
				errorMessage = "Eval Error: " + arg1.getSymbol() + " not recognized";
				return toReturnError;
			}
		}
		else if (arg1.getType() == "bool") {
			if (arg1.arguments.size() > 0) {
				errorMessage = "Eval Error: type does not support arguments";
				return toReturnError;
			}
		}
		if (arg1.getType() == "bool" || arg1.getType() == "double") {
			toReturn.setType("bool");
			if (comparator.getSymbol() == "not") {
				if (arg1.getBool()) toReturn.setBool(false);
				else toReturn.setBool(true);
				return toReturn;
			}
			else if (comparator.getSymbol() == "and") {
				if (!arg1.getBool()) toReturn.setBool(false);
			}
			else if (comparator.getSymbol() == "or") {
				if (arg1.getBool()) toReturn.setBool(true);
			}
			else {
				Expression arg2 = *comparator.arguments.front();
				comparator.arguments.pop();
				if (arg2.getSymbol() == "define") {
					arg2 = evaluateDefine(arg2);
					if (arg2.getType().empty()) return toReturnError;
				}
				else if (arg2.getSymbol() == "begin") {
					arg2 = evaluateBegin(arg2);
					if (arg2.getType().empty()) return toReturnError;
				}
				else if (arg2.getSymbol() == "if") {
					arg2 = evaluateIf(arg2);
					if (arg2.getType().empty()) return toReturnError;
				}
				else if (arg2.isComp()) {
					errorMessage = "Eval Error: cannot evaluate " + comparator.getSymbol() + " with type bool";
					return toReturnError;
				}
				else if (arg2.isOp()) {
					arg2 = evaluateEx(arg2);
					if (arg2.getType().empty()) return toReturnError;
				}
				else if (arg2.getType() == "double") {
					if (arg1.arguments.size() > 0) {
						errorMessage = "Eval Error: type does not support arguments";
						return toReturnError;
					}
					if (comparator.getSymbol() == "and" || comparator.getSymbol() == "or" || comparator.getSymbol() == "not") {
						errorMessage = "Eval Error: cannot evaluate " + comparator.getSymbol() + " with type double";
						return toReturnError;
					}
				}
				else if (arg2.getType() == "symbol") {
					if (arg1.arguments.size() > 0) {
						errorMessage = "Eval Error: type does not support arguments";
						return toReturnError;
					}
					if (defined.count(arg2.getSymbol()) == 1) {
						auto search = defined.find(arg2.getSymbol());
						if (search->second == "double") {
							if (comparator.getSymbol() == "and" || comparator.getSymbol() == "or" || comparator.getSymbol() == "not") {
								errorMessage = "Eval Error: cannot evaluate " + comparator.getSymbol() + " with type double";
								return toReturnError;
							}
							else {
								auto search = definedDub.find(arg2.getSymbol());
								arg2.setType("double");
								arg2.setDouble(search->second);
							}
						}
						else if (search->second == "bool") {
							auto search = definedBool.find(arg2.getSymbol());
							arg2.setType("bool");
							arg2.setBool(search->second);
						}
					}
					else {
						errorMessage = "Eval Error: " + arg1.getSymbol() + " not recognized";
						return toReturnError;
					}
				}
				if (arg1.getType() != "double" || arg2.getType() != "double") {
					errorMessage = "Eval Error: cannot evaluate " + comparator.getSymbol() + " with type bool";
					return toReturnError;
				}
				if (comparator.getSymbol() == "<") {
					if (arg1.getDouble() < arg2.getDouble()) toReturn.setBool(true);
					else toReturn.setBool(false);
					return toReturn;
				}
				else if (comparator.getSymbol() == ">") {
					if (arg1.getDouble() > arg2.getDouble()) toReturn.setBool(true);
					else toReturn.setBool(false);
					return toReturn;
				}
				else if (comparator.getSymbol() == ">=") {
					if (arg1.getDouble() >= arg2.getDouble()) toReturn.setBool(true);
					else toReturn.setBool(false);
					return toReturn;
				}
				else if (comparator.getSymbol() == "<=") {
					if (arg1.getDouble() <= arg2.getDouble()) toReturn.setBool(true);
					else toReturn.setBool(false);
					return toReturn;
				}
				else if (comparator.getSymbol() == "=") {
					if (arg1.getDouble() == arg2.getDouble()) toReturn.setBool(true);
					else toReturn.setBool(false);
					return toReturn;
				}
			}
		}
	}
	return toReturn;
}

Expression Environment::evaluateDefine(Expression define) {
	Expression toDefine;
	Expression defineTo;
	Expression evalReturn;
	Expression toReturnError;
	Expression toReturn;
	double defineToDub;
	bool defineToBool;
	std::string defineType;
	if (define.arguments.size() != 2) {
		errorMessage = "Eval Error: invalid number of arguments for define";
		return toReturnError;
	}
	toDefine = *define.arguments.front();
	define.arguments.pop();
	defineTo = *define.arguments.front();
	define.arguments.pop();
	// check if the argument to define is acceptable
	// check if it is an acceptable type
	if (toDefine.getType() == "double" || toDefine.getType() == "bool" || toDefine.isOp() || toDefine.getSymbol() == "define" || toDefine.getSymbol() == "begin" || toDefine.getSymbol() == "if" || toDefine.getSymbol() == "pi") {
		errorMessage = "Eval Error: cannot define this expression";
		return toReturnError;
	}
	// check if the symbol is already defined
	if (defined.count(toDefine.getSymbol()) == 1) {
		errorMessage = "Eval Error: " + toDefine.getSymbol() + " is already defined";
		return toReturnError;
	}

	if (defineTo.isOp()) {
		evalReturn = evaluateEx(defineTo);
		if (evalReturn.getType().empty()) {
			return toReturnError;
		}
		defineToDub = evalReturn.getDouble();
		defineType = "double";
	}
	else if (defineTo.getSymbol() == "define") {
		evalReturn = evaluateDefine(defineTo);
		if (evalReturn.getType().empty()) {
			return toReturnError;
		}
		if (evalReturn.getType() == "double") {
			defineToDub = evalReturn.getDouble();
			defineType = "double";
		}
		else if (evalReturn.getType() == "bool") {
			defineToBool = evalReturn.getBool();
			defineType = "bool";
		}
	}
	else if (defineTo.getSymbol() == "if") {
		evalReturn = evaluateIf(defineTo);
		if (evalReturn.getType().empty()) {
			return toReturnError;
		}
		else if (evalReturn.getType() == "bool") {
			defineToBool = evalReturn.getBool();
			defineType = "bool";
		}
		else if (evalReturn.getType() == "double") {
			defineToDub = evalReturn.getDouble();
			defineType = "double";
		}
	}
	else if (defineTo.getSymbol() == "begin") {
		evalReturn = evaluateBegin(defineTo);
		if (evalReturn.getType().empty()) {
			return toReturnError;
		}
		if (evalReturn.getType() == "double") {
			defineToDub = evalReturn.getDouble();
			defineType = "double";
		}
		else if (evalReturn.getType() == "bool") {
			defineToBool = evalReturn.getBool();
			defineType = "bool";
		}
	}
	else if (defineTo.isComp()) {
		evalReturn = evaluateComp(defineTo);
		if (evalReturn.getType().empty()) {
			return toReturnError;
		}
		defineToBool = evalReturn.getBool();
		defineType = "bool";
	}
	else if (defineTo.getType() == "double") {
		if (defineTo.arguments.size() > 0) {
			errorMessage = "Eval Error: type does not support arguments";
			return toReturnError;
		}
		defineToDub = defineTo.getDouble();
		defineType = "double";
	}
	else if (defineTo.getType() == "bool") {
		if (defineTo.arguments.size() > 0) {
			errorMessage = "Eval Error: type does not support arguments";
			return toReturnError;
		}
		defineToBool = defineTo.getBool();
		defineType = "bool";
	}
	else {
		if (defineTo.arguments.size() > 0) {
			errorMessage = "Eval Error: type does not support arguments";
			return toReturnError;
		}
		if (defined.count(defineTo.getSymbol()) == 1) {
			auto search = defined.find(defineTo.getSymbol());
			if (search->second == "double") {
				auto search = definedDub.find(defineTo.getSymbol());
				defineType = "double";
				defineToDub = search->second;
			}
			else if (search->second == "bool") {
				auto search = definedBool.find(defineTo.getSymbol());
				defineType = "bool";
				defineToBool = search->second;
			}
		}
		else {
			errorMessage = "Eval Error: " + defineTo.getSymbol() + " not recognized";
			return toReturnError;
		}
	}
	if (defineType == "double") {
		defined[toDefine.getSymbol()] = defineType;
		definedDub[toDefine.getSymbol()] = defineToDub;
		toReturn.setType(defineType);
		toReturn.setDouble(defineToDub);
	}
	else if (defineType == "bool") {
		defined[toDefine.getSymbol()] = defineType;
		definedBool[toDefine.getSymbol()] = defineToBool;
		toReturn.setType(defineType);
		toReturn.setBool(defineToBool);
	}
	return toReturn;
}

void Environment::setHead(Expression newHead) {
	head = newHead;
}

void Environment::clearEnvironment() {
	defined.clear();
	definedDub.clear();
	definedBool.clear();
}

std::string Environment::getErrorMessage() {
	return errorMessage;
}