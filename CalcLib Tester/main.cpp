//#define DEBUG
#include <iostream>
#include <cstring>

#include "tests.h"
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
	

#ifdef RUN_TESTS
	if (!test::memory_module_test1())
	{
		std::cin.get();
		return false;
	}
	test::profileInterpreter();
	test::profileInterpreterVM();
	return 0;
#endif

#ifndef DEBUG
	err_redirect err; //remove cerr stream
#endif

	
	interpreter inter; 
	std::string expression = "";
	
	memory mem; //Create memory unit
	operators::operators_interface oper;
	math_func::function_interface functions; //Create function unit

	//init and load operators
	oper.load(operators::std_operators);
	inter.setOperator(&oper);

	//Init and load memory unit
	mem.set("PI", 3.14159f, true, true);  //Add constat variable PI with value 3.14
	inter.setMemory(&mem); //Assign memory unit to interpreter
	
	//init and load func unit
	functions.load(math_func::std_math_trig_func); // Load std_math_trig_funct into function unit
	functions.load(math_func::std_math_func);
	functions.load(math_func::std_math_num_func);
	functions.load(core_math::lib_core_math);
	inter.setFunction(&functions);

#if defined(CORAX_VM_EXEC)
	CoraxVM::corax_program prgm;
	CoraxVM::Corax_program_builder_module prgm_builder(&inter);
	CoraxVM::corax_runtime runtime;
#endif


	bool exit = false;
     std::cout << "Calculator backend test\nLukas Rahmn 2015\n\nEnter an expression or write menu to open the menu\n\n";
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
#if  defined(SYNTAX_TREE_EXEC)
					mem.set("ans",inter.exec());
					
#elif defined(CORAX_VM_EXEC)
					prgm_builder.create_program(&prgm);
					mem.set("ans", runtime.run(&prgm));
#else
#error "WARNING, no execution enviroment selected"
#endif
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
