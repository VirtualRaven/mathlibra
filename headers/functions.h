#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <cmath>
#include <vector>
#include <iostream>
#include "main.h"
namespace math_func
{
typedef double number_type;
	struct function
	{
		std::string name;
		typedef number_type(*funcPtr)(number_type);
		funcPtr ptr;

		function(std::string name, funcPtr ptr);
	};



	class function_interface
	{
		std::vector<function> funcs;
		typedef number_type(*funcPtr)(number_type);
	public:

		void load(std::vector< function> obj);
		void load(function obj);
		bool isloaded(std::string funcName);
		funcPtr get(std::string funcName);
		void display();

	};

	//Vector containing basic trigometric functions form dervired from math.h
	extern std::vector< math_func::function> std_math_trig_func;
	//Vector containing the very basic functions like sqrt or log form dervired from math.h
	extern std::vector<math_func::function> std_math_func;
	//Vector containing basic numerical functions like ceil dervired from math.h
	extern std::vector<math_func::function> std_math_num_func;
};
#endif
