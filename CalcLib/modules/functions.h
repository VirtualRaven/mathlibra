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
	/**
	 * Unit test for function module. Checks the functionality of the function module. @return True if tests passes. 
	 */
	bool function_module_test1();
}

namespace math_func
{
	/**
	 * The exception class for the function module. All exceptions thrown by the function module is of this type
	 */
	struct functionOops : public exception
	{
		functionOops(std::string inf);
		const char* what();

	};
	
	/**
	 * Implements the function runtime type. All functions loaded into the function module @see  function_interface must be wrapped in this class.
	 */
	struct m_function
	{
		bool is_general; /**< @return True if function is of type generalFuncPtr, else if it is of type funcPtr it returns false */
		std::string name; /**< The name of the function. This name is the name the user calls the function by during runtime.@note At present time no name collision tests are done. */
		typedef number_type(*funcPtr)(number_type); /**< @typedef the standard fixed single argument type function. */
		typedef number_type(*generalFuncPtr)(tree::node_base*); /**< @typedef the extended function type that takes an pointer to an node_base. Used for multi argument functions. */ 
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



/**
 * The function module. This module is loaded by the interpreter to implement the runtime functions. It handles function loading, name lookup, address lookup and specification of function type.
 */
 
	class function_interface
	{
		std::vector<m_function> funcs;
		m_function cache;
		typedef number_type(*funcPtr)(number_type);
	public:

		void load(std::vector< m_function>& obj); /**< @param obj An vector of functions to be loaded by the module. */
		void load(m_function obj); /**< @param obj An single object to be loaded by the module. */
		bool isloaded(std::string funcName); /**< Checks if function is loaded. @param funcName The name to be checked if it is loaded. @return True if function is loaded. */
		bool isGeneral(); /**< @return True if function last specified in isloaded() is of type generalFuncPtr. @note the last string to be sent to isloaded() is cached which this function works upon. */
		void * get(std::string funcName); /**< Gets the an function pointer. @param funcName A string naming the function to search for. @return an void pointer to the funcName. @note The return needs to be converted to either an funcPtr or generalFuncPtr using an reinterpret_cast. */
		void display(); /**< Outputs an list of loaded functions to cout. */
	};
	

	/**Vector containing basic trigometric functions form dervired from math.h */
	extern std::vector< math_func::m_function> std_math_trig_func;
	/**Vector containing the very basic functions like sqrt or log form dervired from math.h */
	extern std::vector<math_func::m_function> std_math_func;
	/**Vector containing basic numerical functions like ceil dervired from math.h */
	extern std::vector<math_func::m_function> std_math_num_func;

	
};
#endif
