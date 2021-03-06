/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <cmath>
#include <map>
#include <iostream>
#include "exception.h"
#include "main.h"
#include "core/tree_interface.h"
#include "core/function_obj.h"

namespace test
{
	/**
	 * Unit test for function module. Checks the functionality of the function module. @return True if tests passes. 
	 */
	bool function_module_test1();
}

/**
 * @namespace math_func Contains the function module
*/
namespace math_func
{
       using namespace function_obj; 
	        
        struct load_test_return
        {   
            bool loaded;
            bool functor;
        };
 
	struct func_obj_info
	{
	  	std::string name;
		std::string tag;
		std::string doc;
		std::string disp_name;
                std::string signature;
	};

       
	enum func_type {FAST,GENERAL,USER};
	/**
	 * Implements the function runtime type. All functions loaded into the function module @see  function_interface must be wrapped in this class.
	 */
	struct m_function
	{
		func_type type; /**< @return True if function is of type generalFuncPtr, else if it is of type funcPtr it returns false */
		std::string name; /**< The name of the function. This name is the name the user calls the function by during runtime.@note At present time no name collision tests are done. */
		std::string tag;
		std::string doc;
		std::string disp_name;
		typedef interface::type*(*generalFuncPtr)(tree::node_base*); /**< @typedef the extended function type that takes an pointer to an node_base. Used for multi argument functions. */ 
		union
		{
			generalFuncPtr gptr;
		        interpreted_func* uptr;
                };
		m_function(m_function&& func);	
		m_function(std::string name,
				std::string tag,
				std::string doc,
				std::string disp_name, 
				generalFuncPtr ptr);

		m_function(std::string name,
				std::string tag,
				std::string doc,
				std::string disp_name, 
				interpreted_func* ptr);
                m_function();
		virtual ~m_function();
	};
	/**
	 * A constant implementation of m_function.
	 * This class can not contain user functions thus it 
	 * can be copied.
	 */
	struct m_function_const : public m_function
	{

		m_function_const(std::string name,
				std::string tag,
				std::string doc,
				std::string disp_name, 
				generalFuncPtr ptr);

		m_function_const(const m_function_const&);
	};



/**
 * The function module. This module is loaded by the interpreter to implement the runtime functions. It handles function loading, name lookup, address lookup and specification of function type.
 */
 	typedef void(*u_ptr)();

	class function_interface
	{
		typedef std::map<std::string,m_function> map_type;
                map_type funcs;
		map_type::iterator cache;
	public:

		void load(const std::vector< m_function_const>& obj); /**< @param obj An vector of functions to be loaded by the module. */
		void load(m_function&& obj); /**< @param obj An single object to be loaded by the module. */
		void load(const m_function_const& obj);
		void unload(std::string name); /**< Unloads the specified function */
                load_test_return isloaded(std::string funcName); /**< Checks if function is loaded. @param funcName The name to be checked if it is loaded. @return True if function is loaded. */
		func_type type(); /**< @return True if function last specified in isloaded() is of type generalFuncPtr. @note the last string to be sent to isloaded() is cached which this function works upon. */
		u_ptr get(std::string funcName); /**< Gets the an function pointer. @param funcName A string naming the function to search for. @return an void pointer to the funcName. @note The return needs to be converted to either an funcPtr or generalFuncPtr using an reinterpret_cast. */
                interpreted_func* getFunctor(std::string funcName); /**<Gets an functor object representing an userprovided function. */ 
                void display(); /**< Outputs an list of loaded functions to cout. */
		std::vector<std::string> getFunctionNames();
		std::vector<func_obj_info> getFunctionObjs();
		~function_interface();
	};
	

	/**Vector containing basic trigometric functions form dervired from math.h */
	extern std::vector< math_func::m_function_const> std_math_trig_func;
	/**Vector containing the very basic functions like sqrt or log form dervired from math.h */
	extern std::vector<math_func::m_function_const> std_math_func;
	/**Vector containing basic numerical functions like ceil dervired from math.h */
	extern std::vector<math_func::m_function_const> std_math_num_func;
	/**Data connstructors for bracket matrice initilization */
	extern std::vector<math_func::m_function_const> mathlibra_data_constructors; 
	
}
#endif
