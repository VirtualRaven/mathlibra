#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <cmath>
#include <vector>
#include <iostream>
#include "exception.h"
#include "main.h"
#include "core/tree_interface.h"
namespace test
{
	bool function_module_test1();
}

namespace math_func
{
	struct functionOops : public exception
	{
		functionOops(std::string inf);
		const char* what();

	};
	
	//The interpreter supports to different types if functions. Functions following signature double(*)(double) and double(*)(tree::node*) If possible the first signature should be used.
	//It is recomended not to expose access to end user of libary to the second option as it gives uncontrolled access to the hole abstract tree from the node the function is at and down.
	//A function of the second type is under no cirumstances allowed to change the tree upon execution of the function, the tre must remain unchanged.
	struct m_function
	{
		bool is_general;
		std::string name;
		typedef number_type(*funcPtr)(number_type);
		typedef number_type(*generalFuncPtr)(tree::node_base*);
		union
		{
			funcPtr ptr;
			generalFuncPtr gptr;
		};
		
		m_function(std::string name, funcPtr ptr);
		m_function(std::string name, generalFuncPtr ptr);
		m_function();
		~m_function();
	};





	class function_interface
	{
		std::vector<m_function> funcs;
		m_function cache;
		typedef number_type(*funcPtr)(number_type);
	public:

		void load(std::vector< m_function>& obj);
		void load(m_function obj);
		bool isloaded(std::string funcName);
		bool isGeneral(); //Tests if the function in the cache is general
		void * get(std::string funcName);
		void display();
	};
	

	//Vector containing basic trigometric functions form dervired from math.h
	extern std::vector< math_func::m_function> std_math_trig_func;
	//Vector containing the very basic functions like sqrt or log form dervired from math.h
	extern std::vector<math_func::m_function> std_math_func;
	//Vector containing basic numerical functions like ceil dervired from math.h
	extern std::vector<math_func::m_function> std_math_num_func;

	
};
#endif
