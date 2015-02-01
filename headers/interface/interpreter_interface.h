#ifndef INTERPRETER_INTERFACE_H_INCLUDED
#define INTERPRETER_INTERFACE_H_INCLUDED

#include "modules/functions.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "interface/CoraxVM_interface.h"
namespace interface
{

class interpreter_interface
{

public:
	/*
        Following functions sets the pointers to modules that the interpreter can use.
        All are modules are optional but for basic functionality a operator_interface should be specified
        The client of the libary is responsible for implementing all modules, a reference implementation
        is provided under modules.

        Warning: interpreter does not copy the modules so the pointers provided must be valid during the lifetime
        of the interpreterm object.
	*/
    //Functions for
	virtual void setMemory(memory* mem)=0;
	virtual void setFunction(math_func::function_interface* functions)=0;
	virtual void setOperator(operators::operators_interface* operators)=0;

	//Basic controll functions for the interpreter
	virtual bool interpret()=0; //Interprets expression provided by set
	virtual number_type exec()=0; //Evaluates and returns the value of the expression
	virtual void set(const char * expression, short lenght)=0; //Sets the expression to be interpret
	virtual ~interpreter_interface();

	
};


}
#endif // INTERPRETER_INTERFACE_H_INCLUDED
