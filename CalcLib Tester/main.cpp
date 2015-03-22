//#define DEBUG
#include <iostream>
#include <cstring>
#include "client.h"
#include "core_math.h"
#include "ptr_protect.h"
#include "profiling.h"
#include "thread_pool.h"
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

#define P_TEST
#ifdef P_TEST
	try

	{
		
		std::cout << "Running test\n";
		std::string exr = "x=(sqrt(sqrt(5*5)^2)*100)/5*(sin(PI)^2+cos(PI)^2)";
		inter.set(exr.c_str(), exr.size());
		inter.interpret();
		
		interpreter inter2(std::move(inter));
#if defined(CORAX_VM_EXEC)
		prgm_builder = CoraxVM::Corax_program_builder_module(&inter2);
#endif
		/*
		CoraxVM::Corax_program_builder_module prgm_builder(&inter2);
		CoraxVM::corax_program prgm;
		CoraxVM::corax_runtime runtime;
		*/
		//prgm_builder.create_program(&prgm);
		auto test1 = [&](){ return inter2.exec(); };
		auto test2 = [&](){inter2.interpret(); };

		
		const unsigned int test_lenght = 100000;
		std::cout << "interpret: " << func_profile<test_lenght>(test2) << "s\n";
		double exec = func_profile<test_lenght>(test1);
		std::cout << "exec: " << exec << "s\n";

#if defined(CORAX_VM_EXEC)
		auto test3 = [&](){ prgm_builder.create_program(&prgm); };
		auto test4 = [&](){ runtime.run(&prgm); };
		double ccompile = func_profile<test_lenght>(test3);
		double cexec = func_profile<test_lenght>(test4);
		std::cout << "corax compile: " << ccompile << "s\n";
		std::cout << "corax run: " << cexec << "s\n";
		std::cout << "virtual machine overhead: " << round(((ccompile + cexec) / exec) * 100) << "%\n";

#endif
		
		
		
	
		
		
	}
	catch (exception& e)
	{

		std::cout << "[\n Exception: " << e.what() << "\n";
		std::cout << " Description: " << e.desc() << "\n]\n";
	}
	//std::cin.get();
	
	return 0;

#endif //P_TEST


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
