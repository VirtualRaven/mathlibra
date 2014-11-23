#include "functions.h"

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


};
