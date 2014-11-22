#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <cmath>
#include <vector>

namespace math_func
{
	template<typename mathType>
	struct function
	{
		std::string name;
		typedef mathType(*funcPtr)(mathType);
		funcPtr ptr;
		function(std::string name, funcPtr ptr)
		{
			this->name = name;
			this->ptr = ptr;
		}

	};

	
	template<typename mathType>
	class function_interface
	{
		std::vector<function<mathType>> funcs;
		typedef mathType(*funcPtr)(mathType);
	public:
		void load(std::vector< function<mathType>> obj)
		{
			funcs.insert(funcs.end(),obj.begin(),obj.end());
		}
		void load(function<mathType> obj)
		{
			funcs.push_back(obj);
		}
		bool isloaded(std::string funcName)
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
		funcPtr get(std::string funcName)
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
};
#endif