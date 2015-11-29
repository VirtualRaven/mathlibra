#include "main_wrapper.h"
#include "c++_wrapper/export.h"
#include "ptr_protect.h"
#include <iostream>
#include <vector>

bool test_error(ptr_protect<interface::calc_lib_interface*, false>& calc)
{
	if (calc->exceptionOccured())
	{
		auto excp = calc->get_exception_info();
		std::cerr << "Exception occured\n: " << excp.type << "\n" << excp.desc << "\n";
		return false;
	}
	else return true;
}



int run_test()
{
	//List of expressions to test that the interpreter must handle correctly.
	std::vector < std::string> tests = { "sqrt(42)",  "x=2*(32-1)", "y=2^3", "x-1", "2^-x" };
	std::vector<double> results;
	ptr_protect<interface::calc_lib_interface*, false> calc(InitLib());
	
	//Load external plugins
	/*calc->enablePlugins();
	if (!test_error(calc))
	{
		return false;
	}*/ 
	auto func =calc->getFunctionObjs();
	for (auto obj : func)
	{
		std::cout << obj.name << " " << obj.doc << " " << obj.tag << " "<<obj.disp_name << std::endl;
	}
	auto names = calc->getFunctionNames();
	for (auto name : names)
	{
		std::cout << name << std::endl;
	}
	for (std::string expr : tests)
	{
		calc->set_arg(expr);
		if ( !test_error(calc))
		{
			return false;
		}
		calc->interpret_arg();
		if (!test_error(calc))
		{
			return false;
		}
		results.push_back( calc->execute_arg());
		if (!test_error(calc))
		{
			return false;
		}
	}

	
	interface::mem_obj_api obj =  calc->getVariable(0);
	std::cout << "0: " << obj.name.c_str() << " " << obj.value << " "<<obj.isConst << std::endl;
	return true;
	
}

int main()
{

	return run_test();
	
}
