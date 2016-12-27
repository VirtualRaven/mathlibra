/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
//#define DEBUG
#include <iostream>

#include <cstring>
#include <iomanip> 
#include "tests.h"
#include "core/function_obj.h"
#include "core/type.h"
#include "core/type_helper.h"
#include "core/mem_test.h"
/* TODO
	Add function inpterpeting and execution DONE
	Add varible interpeting and memmory system. DONE
	Add syntax checking and error reporting PARTERLY
	Add polyargument functions
    Verify that exception system does not intoduce memmory leaks

*/
std::vector<std::string> split(std::string str,char delim)
{
    std::vector<std::string> strs;
    strs.push_back(std::string());
    for(auto s = str.begin(); s < str.end(); s++)
    {
        if(*s==delim)
        {
            strs.push_back(std::string());
        } 
        else
        {
            strs.back().push_back(*s);
        }
    }
    if(strs.back().size() == 0)
    {
        strs.pop_back();
    }
    return strs;
}
bool parse_output=false;
bool menu(memory::memory& mem,math_func::function_interface& func,std::string input)
{
        if(input.size() == 0)
        {
	    std::cout << "PRINT VARIABLES [1]\nEMPTY VARIABLE TABLE[2]\nPRINT FUNCS[3]\nPRINT BUILD INFO[4]\nPRINT HELP[5]\nRUN TESTS [6]\nTOGGLE PARSE OUTPUT[7]\nEXIT [8]\nMenu> ";
	    std::getline(std::cin, input);
        }
	if (input == "1")
	{
		std::vector<std::string> vars = mem.allVars();
		for (unsigned int i = 0; i < vars.size(); i++)
		{
			std::cout <<"Variable "<<i+1 << ": " << vars[i] << " = " << mem.get(vars[i])->toString() << "\n";
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
		std::string exr = "x=(sqrt(sqrt(5*5)^2)*100)/5*(sin(PI)^2+cos(PI)^2)";
		test::profileInterpreter(exr);
//		test::profileInterpreterVM(exr);
		return false;
	}
        else if(input == "7"){
            std::cout << "Toggling parser output...\n";
            parse_output=!parse_output;
            return false;
        }
	else if (input == "8")
	{
		
		return true;
	}
	else
	{
		std::cout << "Unkown input, try again\n";
		return menu(mem,func,"");
	}
	return false;
}
int main(int argc, char* argv[])
{
    

	
	{

#ifndef DEBUG
		err_redirect err; //remove cerr stream
#endif
		std::cout << std::setprecision(20);
		
		interpreter inter; 
		std::string expression = "";
	
	memory::memory mem; //Create memory unit
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
	functions.load(math_func::mathlibra_data_constructors);
	inter.setFunction(&functions);
	auto my_manager = plugin::get_platform_specific_manager();
	if(my_manager != nullptr)
	{
	try
	{	my_manager->loadPlugins(&functions);

	}
	catch (exception& e)
	{

		std::cout << "[" << e.what() <<" ("<< e.id() << ") " <<  "] "<< e.desc() << std::endl;
	}
	}
#if defined(CORAX_VM_EXEC)
	CoraxVM::corax_program prgm;
	CoraxVM::Corax_program_builder_module prgm_builder(&inter);
	CoraxVM::corax_runtime runtime;
#endif


	bool exit = false;
     std::cout << "Calculator backend test\nLukas Rahmn 2016\n\nEnter an expression or write menu to open the menu\n\n";
	do
	{

		std::cout << "> ";
		std::getline(std::cin, expression);
                std::vector<std::string> arg=split(expression,' ');
                if(arg.size() == 1 && arg[0] == "menu") 
		{
			exit = menu(mem,functions,"");
		}
                else if(arg.size() == 2 && arg[0] == "menu")
                {
                    exit = menu(mem,functions,arg[1]);   
                }
		else
		{


			try
			{
				inter.set(expression.c_str(), expression.size());
                                if(parse_output)
                                {
                                    auto tmp = inter.extendedInterpret();
                                    for(auto it : tmp)
                                        std::cout << (std::string)it << std::endl;
                                    std::cout << std::endl;
                                }
                                else
				    inter.interpret();
#if  defined(SYNTAX_TREE_EXEC)
					
					mem.set("ans",inter.exec());
					
#elif defined(CORAX_VM_EXEC)
					prgm_builder.create_program(&prgm);
					mem.set("ans", runtime.run(&prgm));
#else
#error "WARNING, no execution enviroment selected"
#endif
					std::cout << expression << " = " << mem.get("ans")->toString() << std::endl;
				

			}

			catch (exception& e)
			{
		            std::cout << "[" << e.what() <<" ("<< e.id() << ") " <<  "] "<< e.desc() << std::endl;
			}
		}
	} while (!exit);
		}
	debug::check_tree_mem_leak();		
	return 0;

}
