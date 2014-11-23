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
std::vector< math_func::function<double>> std_math_trig_func = {
	math_func::function<double>("sin", static_cast<double(*)(double)>(sin)),
	math_func::function<double>("cos", static_cast<double(*)(double)>(cos)),
	math_func::function<double>("tan", static_cast<double(*)(double)>(tan)),
	math_func::function<double>("asin", static_cast<double(*)(double)>(asin)),
	math_func::function<double>("acos", static_cast<double(*)(double)>(acos)),
	math_func::function<double>("atan", static_cast<double(*)(double)>(atan))
};

bool menu(memory& mem)
{
	std::cout << "PRINT VARIABLES [1]\nEMPTY VARIABLE TABLE[2]\nEXIT [3]\nMenu> ";
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
		return true;
	}
	else
	{
		std::cout << "Unkown input, try again\n";
		return menu(mem);
	}
}
int main()
{
	std::cout << "Calculator backend test\nLukas Rahmn 2014\n";

	interpreter<double> inter;
	std::string expression = "";
	memory mem;

	mem.set("PI", 3.14,true,true);

	inter.setMemory(&mem);
	err_redirect err;
	bool exit = false;
	do
	{
		std::cout << "> ";
		std::getline(std::cin, expression);
		if (expression == "menu")
		{
			exit = menu(mem);
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
