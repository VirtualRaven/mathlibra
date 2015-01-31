#define DEBUG
#include <iostream>
#include <cstring>
#include "client.h"
#include "ptr_protect.h"


/* TODO
	Add function inpterpeting and execution DONE
	Add varible interpeting and memmory system. DONE
	Add syntax checking and error reporting PARTERLY
	Add polyargument functions
    Verify that exception system does not intoduce memmory leaks

*/


bool menu(memory& mem,math_func::function_interface& func)
{
	std::cout << "PRINT VARIABLES [1]\nEMPTY VARIABLE TABLE[2]\nPRINT FUNCS[3]\nPRINT BUILD INFO[4]\nPRINT HELP[5]\nEXIT [6]\nMenu> ";
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
		util::buildInfo();
		return false;
	}
	else if (input == "5")
	{
		util::help();
		return false;
	}
	else if (input == "6")
	{
		return true;
	}
	else
	{
		std::cout << "Unkown input, try again\n";
		return menu(mem,func);
	}
}
int main(int argc, char* argv[])
{
    std::cout << sizeof(void*);
    std::cout << sizeof(number_type);
    ptr_protect<char*,false> my_char = safe_alloc<char>();

    //Load core functions from libary
	
	interface::interpreter_interface * inter  = create_interpreter(); //Load interpreter from CORE
	interface::corax_runtime_interface * runtime = create_corax_runtime();
	interface::corax_program* prgm = create_corax_program();

	std::string expression = "";
	//Init memory unit
	memory mem; //Create memory unit
	operators::operators_interface oper;
	oper.load(operators::std_operators);
	inter->setOperator(&oper);
	mem.set("PI", 3.14159f, true, true);  //Add constat variable PI with value 3.14
	inter->setMemory(&mem); //Assign memory unit to interpreter
	//init func unit
	math_func::function_interface functions; //Create function unit
	functions.load(math_func::std_math_trig_func); // Load std_math_trig_funct into function unit
	functions.load(math_func::std_math_func);
	functions.load(math_func::std_math_num_func);
	inter->setFunction(&functions);


	err_redirect err; //remove cerr stream

	bool exit = false;
	if( argc > 1)
    {

        try
			{
				inter->set(argv[1],strlen(argv[1]));
				if (inter->interpret())
				{
					mem.set("ans",inter->exec());
					std::cout << mem.get("ans") << std::endl;
					inter->compile(prgm);
					std::cout << runtime->run(prgm) << std::endl;
				}
				else
				{
					std::cout << "Failed to interpret expression!" << std::endl;
					return 1;
				}

			}

			catch (exception& e)
			{

				std::cout << "[\n Exception: " << e.what() << "\n";
				std::cout << " Description: " << e.desc() << "\n]\n";
				return 1;
			}
			return 0;
    }
     std::cout << "Calculator backend test\nLukas Rahmn 2014\n\nEnter an expression or write menu to open the menu\n\n";
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
				inter->set(expression.c_str(), expression.size());
				if (inter->interpret())
				{
					mem.set("ans",inter->exec());
					std::cout << expression << " = " << mem.get("ans") << std::endl;
					inter->compile(prgm);
					std::cout << "CoraxVM: " << runtime->run(prgm) << std::endl;
					prgm->clear();
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
