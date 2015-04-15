#include "tests.h"


void test::profileInterpreter(std::string& exr)
{
	try

	{
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
		const unsigned int test_lenght = 10000;
		std::cout << "Running test\n";
	
		inter.set(exr.c_str(), exr.size());
		inter.interpret();
		interpreter inter2(std::move(inter));
		auto test1 = [&](){ return inter2.exec(); };
		auto test2 = [&](){inter2.interpret(); };
		std::cout << "interpret: " << func_profile<test_lenght>(test2) << "s\n";
		double exec = func_profile<test_lenght>(test1);
		std::cout << "exec: " << exec << "s\n";
	}
	catch (exception& e)
	{

		std::cout << "[\n Exception: " << e.what() << "\n";
		std::cout << " Description: " << e.desc() << "\n]\n";
	}
}
void test::profileInterpreterVM(std::string& exr)
{
	try

	{
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

		const unsigned int test_lenght = 10000;
		std::cout << "Running test\n";
		CoraxVM::corax_program prgm;
		CoraxVM::Corax_program_builder_module prgm_builder(&inter);
		CoraxVM::corax_runtime runtime;
		inter.set(exr.c_str(), exr.size());
		inter.interpret();
		interpreter inter2(std::move(inter));
		prgm_builder = CoraxVM::Corax_program_builder_module(&inter2);

		auto test1 = [&](){ return inter2.exec(); };
		auto test2 = [&](){inter2.interpret(); };
		auto test3 = [&](){ prgm_builder.create_program(&prgm); };
		auto test4 = [&](){ runtime.run(&prgm); };

		double exec = func_profile<test_lenght>(test1);
		double ccompile = func_profile<test_lenght>(test3);
		double cexec = func_profile<test_lenght>(test4);
		std::cout << "compile: " << ccompile << "s\n";
		std::cout << "run: " << cexec << "s\n";
		std::cout << "virtual machine overhead: " << round(((ccompile + cexec) / exec) * 100) << "%\n";
	}
	catch (exception& e)
	{

		std::cout << "[\n Exception: " << e.what() << "\n";
		std::cout << " Description: " << e.desc() << "\n]\n";
	}
}