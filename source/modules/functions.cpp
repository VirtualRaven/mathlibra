#include "modules/functions.h"
//Contains temporary implementation of function module
namespace math_func
{


		function::function(std::string name, funcPtr ptr)
		{
			this->name = name;
			this->ptr = ptr;
		}




		void function_interface::load(std::vector< function> obj)
		{
			funcs.insert(funcs.end(),obj.begin(),obj.end());
		}
		void function_interface::load(function obj)
		{
			funcs.push_back(obj);
		}
		bool function_interface::isloaded(std::string funcName)
		{
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				if (funcs[i].name == funcName)
				{
					return true;
				}
			}
			return false;
		}

		function_interface::funcPtr function_interface::get(std::string funcName)
		{
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				if (funcs[i].name == funcName)
				{
					return funcs[i].ptr;
				}
			}
			return nullptr;
		}

		void function_interface::display()
		{
			std::cout << "-[ function_interface {\n";
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				std::cout << this->funcs[i].name.c_str() << "\t: " << std::hex << (void *)this->funcs[i].ptr << std::dec << "\n";
			}
			std::cout << " }\nLoaded " << this->funcs.size() << " functions]\n";

		}
		std::vector< math_func::function> std_math_trig_func = {
			math_func::function("sin", static_cast<double(*)(double)>(sin)),
			math_func::function("cos", static_cast<double(*)(double)>(cos)),
			math_func::function("tan", static_cast<double(*)(double)>(tan)),
			math_func::function("asin", static_cast<double(*)(double)>(asin)),
			math_func::function("acos", static_cast<double(*)(double)>(acos)),
			math_func::function("atan", static_cast<double(*)(double)>(atan))
		};

		std::vector<math_func::function> std_math_func = {
			//math_func::function("abs", static_cast<double(*)(double)>(abs)), //Only defined with double overload on certain platforms
			math_func::function("sqrt", static_cast<double(*)(double)>(sqrt)),
			math_func::function("log", static_cast<double(*)(double)>(log)),
			math_func::function("lg", static_cast<double(*)(double)>(log10))
		};

		std::vector<math_func::function> std_math_num_func = {
			math_func::function("ceil", static_cast<double(*)(double)>(ceil)),
			math_func::function("floor", static_cast<double(*)(double)>(floor)),
			math_func::function("round", static_cast<double(*)(double)>(round))
		};
};
