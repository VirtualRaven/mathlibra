/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef LIB_INTERFACE_INCLUED
#define LIB_INTERFACE_INCLUED
#include <vector>
#include <string>
#include "type_interface.h"
#include "ptr_protect.h"
namespace interface
{

	struct wrapper_exception_info
	{
		const char * type;
		const char * desc;
                unsigned int id;
		bool isCritical;
	};
        struct mem_obj_api
        {
            std::string name;
            type* value;
            bool    isConst;
        };

	struct func_obj_api
	{
	  	std::string name;
		std::string tag;
		std::string doc;
		std::string disp_name;
	};
        /**
         *  calc_lib_interface is the external interface class for mathlibra.
         *  All external functionality of the C++ API is provided by this class.
         **/
	class calc_lib_interface
	{
	public:
         	typedef ptr_protect<double*,true> double_array_ptr;       
                /** Sets the expression to be interpreted. 
                 * @param str The mathlibra expression to be interpreted. 
                 * @note Calling set_arg just set the argument, 
                 * it does not interpret it. Use interpret_arg() for that purpose. 
                 * */
		virtual void set_arg(std::string str) = 0; 
                /**Interprets the expression set by set_arg.
                 * Use exceptionOccured() to check if operation succeeded.
                 * */
		virtual void interpret_arg() = 0;

                /** Evaluates the expression. 
                 * Use exceptionOccured() to check if the operation was successful. 
                 * A call to this function is only valid after a successful call interpret_arg.  
                 * @returns a type_ptr representing the result of the calculation. 
                 * */
		virtual interface::type_ptr execute_arg() = 0; 
		/** Interprets the expression set by set_arg and exports the resulting expression as an function.
		 * @note A call to this function does not invalidate the expression set by interpret_arg() thus it is legal to call this function between a call to interpret_arg and execute_arg.
		 * @note This causes a call to undefineFunction(name) to be made thus to ensure that no name collisions occur. Thus calling this function involves the same dangers as undefineFunction.
		 * @see undefineFunction  
		 * @param name The name of the defined function.
		 * @param visible Selects if the function should be visible and callable by the end user, Setting this to false only makes the function available to the api-user.
		 */
		virtual void defineFunction(std::string name,bool visible=true)=0;
		
		/**
		 * Creates a set of XY-pairs using specified functions.
		 * Map the specified function f onto the set XS = `{Start,Start+Offset,Start+2*Offset,...,End}`
		 * presuming Start < End. The result is returned in a 1d double array packed as 
		 * `{x1,f(x1),x2,f(x2),...,xn,f(xn)}` where xi is element i in XS and   
		 * n=floor((End-Start)/Offset)+1.
		 * @note  If f(xi) does not return a number a exception will occur.
		 * @returns A double* to a array of length 2n.
		 * @param Start the first value.
		 * @param End the last value .
		 * @param Offset the step lenght.
		 * @param Function the name of function f.
		 */
		virtual double_array_ptr map(double Start,
						double End,
						double Offset,
						std::string Function)=0;
			
		/**Unloads specified function.
		 * @param name The name of the function to unload
		 * @warning Removing a user defined function(a function created by defineFunction() or function definition operator ':' ) that
		 * currently is in use will lead to undefined behavior. 
		 */
		virtual void undefineFunction(std::string name)=0;
                /** Get information about the latest exception. 
                 * @returns Information about the latest exception that occurred in mathlibra. 
                 * */
		
		virtual wrapper_exception_info get_exception_info() = 0; 
                /** Checks if an exception has occurred. 
                 * To get more information about the exception use get_exception_info(), 
                 * which returns and clears the exception. 
                 * @note calls to other methods in this class is invalid while an exception is pending. 
                 * */
		virtual bool exceptionOccured() = 0; 

		/** Get all defined functions.
                 * @returns A vector containing all currently defined function names.
                 */
		virtual std::vector<std::string> getFunctionNames() = 0;
                /**
                 * Get all defined function objects. 
                 * @returns a vecctor of function_obj_api objects.
                 */
		virtual std::vector<func_obj_api> getFunctionObjs() = 0;	
		

		//Memory unit 
                 
                /**
                 * Get all defined functions.
                 * @returns a vector containing all currently defined variable names.
                 */
		virtual std::vector<std::string> getVariableNames() = 0;

                /**
                 * Gets the value of an variable.
                 * @param name The name of the variable which value should be retrieved
                 * @returns The variables current value. This pointer is owned by mathlibra and should not be deleted!
                 * @note if name is undefined a exception will occur, 
                 * use isDefined to check if it is safe call this method.
                 * @note The pointer returned points to the value of the variable, 
                 * thus if you wish to update the variable it is possible to directly modify the pointer instead of using setVariable
                 */
		virtual type* getVariableValue(std::string  name) = 0;
                /**
                 * Gets the variables mem_obj_api object.
                 * @param name The name of the variable to get.
                 * @returns A mem_obj_api object containing data about index.
                 */
                virtual mem_obj_api getVariable(std::string name)=0;
                /**
                 * Gets the variables mem_obj_api object.
                 * @param index Which variable to return starting at index 0. 
                 * @returns A mem_obj_api object containing data about index.
                 * @note getVariable(std::String) is preferred over this method as it is faster.
                 */
                virtual mem_obj_api getVariable(size_t index)=0;

                /**
                 * Set the value of an variable. Fails if the variable does not exist.
                 * @param name The name of the variable which value to set.
                 * @param value A type_ptr to the value, mathlibra will assume ownership of the pointer.
                 */
		virtual void setVariable(std::string name, interface::type_ptr&& value) = 0;
                
                /**
                 * 
                 *
                 */
		virtual void createVariable(std::string name, interface::type_ptr&& value) = 0;
		virtual void clearVariables() = 0;
                
                virtual void manageVariable(std::string name,interface::type_ptr&& value,bool isConst)=0;
                virtual bool  isConstVariable(std::string name)=0;
                virtual unsigned int getNumVariables()=0;
                virtual bool isDefined(std::string name)=0;
		//Plugins
		virtual void enablePlugins() = 0;

		//Destruction
		virtual ~calc_lib_interface(){};

	};
}
#endif
