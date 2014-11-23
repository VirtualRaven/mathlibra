#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <cmath>
#include <vector>
#include <iostream>

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
};
#endif
