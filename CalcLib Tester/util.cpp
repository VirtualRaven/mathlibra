#include "main.h"

#include <iostream>
#include <string>
namespace util
{

	void buildInfo()
	{
		std::cout << "-[ Build date: " << __DATE__ << "]\n";
		std::cout << "-[ Execution enviroment " << EXEC_ENV_STRING << "]\n";
#ifdef GIT_COMMIT_HASH
		std::cout  << "Version: " << git_version << "\n\n;
#endif
		std::cout << "-[DEBUG: ";

#ifdef DEBUG
		std::cout << "TRUE ]\n";
#ifdef LEXICAL_ANANALYSIS_DEBUG
		std::cout << "-[LEXICAL_ANANALYSIS_DEBUG: ENABLED ]\n";
#endif
#ifdef STRUCTUAL_INTEGRITY_TEST
		std::cout << "-[STRUCTUAL_INTEGRITY_TEST: ENABLED ]\n";
#endif
#ifdef SYNTAX_TREE_DEBUG_INFO
		std::cout << "-[SYNTAX_TREE_DEBUG_INFO: ENABLED ]\n";
#endif

#ifdef DEBUG_CORAX_INS
		std::cout << "-[DEBUG_CORAX_INS: ENABLED ]\n";
#endif
#else
		std::cout << "False ]\n";
#endif
		std::cout << "\n\n";

	}

	void help()
	{
		std::cout << "\nProgram help page\n";
		std::cout << "General info[1]\nNumbers[2]\nOperators [3]\nFunctions [4]\nVariables[5]\nDebug[6]\nExit[7]\nselect topic> ";
		std::string input;
		std::getline(std::cin, input);
		if (input == "1")
		{
			std::cout << "\nThis program is only meant to showcase an concept of expression handling by using tree like data structures. It in its current state it doesn't serve as an replacement for a calculator or other software as bugs and precisions problems may exist.\n\n" << "The goal of the program is to develop an expression interpretor back end to be utilized in a more advance graphical application.In its current state the application will try to evaluate any expression entered and compute an answer using floating point mathematics.\n\n" << " The expression can contain functions, variables operators and of course values in form of numbers.See the other topics in the help menu for more info.";

			return;
		}
		else if (input == "2")
		{
			std::cout << "\nValues are any real number.\nvalues can be entered as 35, -2 or 2e10\n2e10 is equal to writing 2*10^10 but the first is evaluated faster.\n";
			return;
		}
		else if (input == "3")
		{
			std::cout << "\noperators are + -/ *^\nThey represent addition, subtraction, division, multiplication and raised to.\nparentheses are also accepted to alter order of evaluation\n";
			return;
		}
		else if (input == "4")
		{
			std::cout << "\nfunctions are called using the syntax func(x)\nWere func is the name of the function and x is the expression to act upon.\nA list of functions can be generatet using the menu. The left column is the function name and the right their memory address.\n";
			return;
		}
		else if (input == "5")
		{
			std::cout << "\nAn variable is defined as an variable memory storage unit which is used to represent an number during an calculation. The variable can is defined as an string or single character which can be both upper and lower case.\n\nX, PI,ans are all legal variables.\n\nNames which are the same as any loaded function is illegal as the interpreter will fail to distinguish these from functions\nNames are case sensitive \nX is not equal to x\nans always stores the result of previous calculations\nPI is an example of an constant variable which can not be altered by the assignment operator.\n\nVariable are assigned a value using the assigment operator =.\nX=12 is an legal assigment so is x=(23+45)/2 but 13 =x is illegal as the assigment operator excpects the variable on the left hand side.\n";
			return;
		}
		else if (input == "6")
		{
			std::cout << "\nthe menu item build information of which debug systems are activated, to enable more or less information the program has to be recompiled as the settings depend upon preprocessor defintion in the main.h header.\nAlso include the version string\nLEXICAL_ANANALYSIS_DEBUG:\nOutputs information about how the lexical analysis interprets the different characters in an string.\n\nSYNTAX_TREE_DEBUG_INFO:\nincreased debug messages about memory allocations and deallocations conducted by the tree object\n\nSTRUCTUAL_INTEGRITY_TEST:\nCheck the syntax tree before evaluation to ensure that all pointers are valid.Aborts program execution on failure \n\nDEBUG_CORAX_INS:\nDisplays instructions bytecode instruction memonics executed by CoraxVM\n";

		}
		else if (input == "7")
		{
			return;
		}
		else
		{
			std::cout << "Unkown input, try again\n";
			return help();
		}
	}
}
