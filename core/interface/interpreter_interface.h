#ifndef INTERPRETER_INTERFACE_H_INCLUDED
#define INTERPRETER_INTERFACE_H_INCLUDED

#include "modules/functions.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "interface/CoraxVM_interface.h"
using namespace math_func;
/**
 * @namespace interface Contains abstract interface classes for different functions.
*/
namespace interface
{

	class interpreter_interface
	{

	public:
		/*
			Following functions sets the pointers to modules that the interpreter can use.
			All are modules are optional but for basic functionality a operator_interface should be specified
			

			Warning: interpreter does not copy the modules so the pointers provided must be valid during the lifetime
			of the interpreterm object.
			*/
		//Functions for
            
		virtual void setMemory(memory::memory* mem) = 0;
		virtual void setFunction(math_func::function_interface* functions) = 0;
		virtual void setOperator(operators::operators_interface* operators) = 0;

		//Basic controll functions for the interpreter
		virtual void interpret() = 0; //Interprets expression provided by set
		virtual interface::type_ptr exec() = 0; //Evaluates and returns the value of the expression
		virtual void set(const char * expression, size_t lenght) = 0; //Sets the expression to be interpret
		virtual ~interpreter_interface();
		






	};

}
#endif // INTERPRETER_INTERFACE_H_INCLUDED
