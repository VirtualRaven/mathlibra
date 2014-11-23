#define DEBUG
#include <iostream>
#include "interpreter.h"
#include "stdErr.h"
#include "functions.h"

/* TODO
	Add function inpterpeting and execution PARTIALI IMPLEMENTED
	Add varible interpeting and memmory system. DONE
	Add syntax checking and error reporting
	Add polyargument functions
    Verify that exception system does not intoduce memmory leaks

*/
std::vector< math_func::function> std_math_trig_func = {
	math_func::function("sin", static_cast<double(*)(double)>(sin)),
	math_func::function("cos", static_cast<double(*)(double)>(cos)),
	math_func::function("tan", static_cast<double(*)(double)>(tan)),
	math_func::function("asin", static_cast<double(*)(double)>(asin)),
	math_func::function("acos", static_cast<double(*)(double)>(acos)),
	math_func::function("atan", static_cast<double(*)(double)>(atan))
};

std::vector<math_func::function> std_math_func = {
	math_func::function("abs", static_cast<double(*)(double)>(abs)),
	math_func::function("sqrt", static_cast<double(*)(double)>(sqrt)),
	math_func::function("log", static_cast<double(*)(double)>(log)),
	math_func::function("lg", static_cast<double(*)(double)>(log10))
};

std::vector<math_func::function> std_math_num_func = {
	math_func::function("ceil", static_cast<double(*)(double)>(ceil)),
	math_func::function("floor", static_cast<double(*)(double)>(floor)),
	math_func::function("round", static_cast<double(*)(double)>(round))
};

bool menu(memory& mem,math_func::function_interface& func)
{
	std::cout << "PRINT VARIABLES [1]\nEMPTY VARIABLE TABLE[2]\nPRINT FUNCS[3]\nEXIT [4]\nMenu> ";
	std::string input;
	std::getline(std::cin, input);
	if (input == "1")
	{
		std::vector<std::string> vars = mem.allVars();
		for (unsigned int i = 0; i < vars.size(); i++)
		{
			std::cout <<"Variable "<<i+1 << ": " << vars[i] << " = " << mem.get(vars[i]) << "\n";
		}
		if (vars.size() == 0)
		{
			std::cout << "TABLE EMPTY\n";
		}

		return false;
	}
	else if (input == "2")
	{
		mem.empty();
		return false;
	}
	else if (input == "3")
	{
		func.display();
		return false;
	}
	else if (input == "4")
	{
		return true;
	}
	else
	{
		std::cout << "Unkown input, try again\n";
		return menu(mem,func);
	}
}
int main()
{
	std::cout << "Calculator backend test\nLukas Rahmn 2014\n";

	interpreter inter;
	std::string expression = "";
	//Init memory unit
	memory mem; //Create memory unit
	mem.set("PI", 3.14159f, true, true);  //Add constat variable PI with value 3.14 
	inter.setMemory(&mem); //Assign memory unit to interpreter
	//init func unit
	math_func::function_interface functions; //Create function unit
	functions.load(std_math_trig_func); // Load std_math_trig_funct into function unit
	functions.load(std_math_func);
	functions.load(std_math_num_func);
	inter.setFunction(&functions);

	//err_redirect err; //remove cerr stream

	bool exit = false;
	do
	{
		std::cout << "> ";
		std::getline(std::cin, expression);
		if (expression == "menu")
		{
			exit = menu(mem,functions);
		}
		else
		{


			try
			{
				inter.set(expression.c_str(), expression.size());
				if (inter.interpret())
				{
					mem.set("ans",inter.exec());
					std::cout << expression << " = " << mem.get("ans") << std::endl;
				}
				else
				{
					std::cout << "Failed to interpret expression!" << std::endl;
				}

			}

			catch (exception& e)
			{

				std::cout << "[\n Exception: " << e.what() << "\n";
				std::cout << " Description: " << e.desc() << "\n]\n";
			}
			/*catch (...)
			{
			std::cout << "Catched unknown exception\n";
			}*/
		}
	} while (!exit);
	return 0;

}
