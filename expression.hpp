#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <string>
#include <queue>

class Expression {
public:
	// Default construct an Expression of type None
	Expression();

	// Construct an Expression with a single Boolean atom with value
	Expression(bool value);

	// Construct an Expression with a single Number atom with value
	Expression(double value);

	// Construct an Expression with a single Symbol atom with value
	Expression(const std::string & value);

	// Equality operator for two Expressions, two expressions are equal if the have the same 
	// type, atom value, and number of arguments
	bool operator==(const Expression & exp) const noexcept;

	// returns the symbol of the expression
	//
	std::string getSymbol();

	// makes the current expression an operator
	//
	void makeOp();

	// returns true if the expression is an operator
	//
	bool isOp();

	// returns the double value of the expression
	//
	double getDouble();

	// returns the type of the expression
	//
	std::string getType();

	//
	bool getBool();

	//
	void setDouble(double newDouble);

	//
	void setBool(bool newBool);

	//
	void setType(std::string newType);

	std::queue<Expression*> arguments;

	Expression* parent;

	//
	bool isComp();

	//
	void makeComp();

	//
	bool isHead();

	//
	void makeHead();

	//
	void setSymbol(std::string newSymbol);

private:
	std::string type;
	bool boolVal;
	double doubleVal;
	std::string symbolVal;
	bool op;
	bool comp;
	bool head;
};

#endif // _EXPRESSION_H_