#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include "expression.hpp"
#include "interpreter.hpp"
#include "tokenize.hpp"

int main(int argc, char *argv[])
{
	Expression toOutput;
	if (argc == 1) {

		std::string readString;
		Interpreter final;
		std::cout << "vtscript > ";
		while (std::getline(std::cin, readString)) {
			std::istringstream isstring(readString);
			if (!final.parse(isstring)) {
				std::cerr << final.getErrorMessage() << std::endl;
				std::cout << "vtscript > ";
				continue;
			}
			try {
				toOutput = final.eval();
				if (toOutput.getType() == "double") std::cout << "(" << toOutput.getDouble() << ")" << std::endl;
				else if (toOutput.getType() == "bool") {
					if (toOutput.getBool()) std::cout << "(" << "True" << ")" << std::endl;
					else std::cout << "(" << "False" << ")" << std::endl;
				}
			}
			catch (InterpreterSemanticError& e) {
				std::cerr << e.what() << std::endl;
			}
			std::cout << "vtscript > ";
		}
		return EXIT_SUCCESS;
		
	}
	else if (argc == 2) {
		std::ifstream readFile(argv[1]);
		Interpreter final;
		if (!final.parse(readFile)) { 
			std::cerr << final.getErrorMessage() << std::endl;
			return EXIT_FAILURE; 
		}
		try {
			toOutput = final.eval();
			if (toOutput.getType() == "double") std::cout << "(" << toOutput.getDouble() << ")" << std::endl;
			else if (toOutput.getType() == "bool") {
				if (toOutput.getBool()) std::cout << "(" << "True" << ")" << std::endl;
				else std::cout << "(" << "False" << ")" << std::endl;
			}
		}
		catch (InterpreterSemanticError& e) {
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	else if (argc == 3  && strcmp(argv[1], "-e") == 0) {
		std::string stringToEval(argv[2]);
		Interpreter final;
		std::istringstream streamString(stringToEval);
		if (!final.parse(streamString)) {
			std::cerr << final.getErrorMessage() << std::endl;
			return EXIT_FAILURE;
		}
		try {
			toOutput = final.eval();
			if (toOutput.getType() == "double") std::cout << "(" << toOutput.getDouble() << ")" << std::endl;
			else if (toOutput.getType() == "bool") {
				if (toOutput.getBool()) std::cout << "(" << "True" << ")" << std::endl;
				else std::cout << "(" << "False" << ")" << std::endl;
			}
		}
		catch (InterpreterSemanticError& e) {
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	else {
		std::cout << "usage:\nvtscript\nvtscript <file_name>\nvtscript -e \"<expression>\"";
		return EXIT_FAILURE;
	}
}
