#include "modules/functions.h"
//Contains temporary implementation of function module
namespace math_func
{
	functionOops::functionOops(std::string inf)
	{
		info = inf;
	}

	const char* functionOops::what()
	{
		return "function Exception";
	}
	
	typedef double(*double_func)(double);


	math_func::m_function::m_function(std::string name, funcPtr ptr) :name(name), ptr(ptr), is_general(false) {}
	math_func::m_function::m_function(std::string name, generalFuncPtr ptr) : name(name), gptr(ptr), is_general(false) {}
	math_func::m_function::~m_function(){}
	math_func::m_function::m_function() : name(), ptr(nullptr), is_general(false){}



		void function_interface::load(std::vector< m_function>& obj)
		{
			funcs.insert(funcs.end(),obj.begin(),obj.end());
		}
		void function_interface::load(m_function obj)
		{
			funcs.push_back(obj);
		}
		bool function_interface::isloaded(std::string funcName)
		{
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				if (funcs[i].name == funcName)
				{
					cache = funcs[i];
					return true;
				}
			}
			return false;
		}
		
		bool function_interface::isGeneral()
		{
			return this->cache.is_general;
		}
		void * function_interface::get(std::string funcName)
		{
			if (cache.name == funcName)
			{
				return cache.ptr;
			}
			else
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

	


		 std::vector< math_func::m_function> std_math_trig_func = {
			 math_func::m_function("sin", static_cast<double_func>(sin)),
			 math_func::m_function("cos", static_cast<double_func>(cos)),
			 math_func::m_function("tan", static_cast<double_func>(tan)),
			 math_func::m_function("asin", static_cast<double_func>(asin)),
			 math_func::m_function("acos", static_cast<double_func>(acos)),
			 math_func::m_function("atan", static_cast<double_func>(atan))
		};

		std::vector<math_func::m_function> std_math_func = {
			//math_func::m_function("abs", static_cast<double(*)(double)>(abs)), //Only defined with double overload on certain platforms
			math_func::m_function("sqrt", static_cast<double_func>(sqrt)),
			math_func::m_function("log", static_cast<double_func>(log)),
			math_func::m_function("lg", static_cast<double_func>(log10))
		};

		std::vector<math_func::m_function> std_math_num_func = {
			math_func::m_function("ceil", static_cast<double_func>(ceil)),
			math_func::m_function("floor", static_cast<double_func>(floor)),
			math_func::m_function("round", static_cast<double_func>(round))
		};
	
};
