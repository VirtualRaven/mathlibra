#ifndef INTERPRETER_INTERFACE_H_INCLUDED
#define INTERPRETER_INTERFACE_H_INCLUDED

#include "modules/functions.h"
#include "modules/memory.h"
#include "modules/operators.h"
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
	virtual void setMemory(memory* mem);
	virtual void setFunction(math_func::function_interface* functions);
	virtual void setOperator(operators::operators_interface* operators);

	//Basic controll functions for the interpreter
	virtual bool interpret(); //Interprets expression provided by set
	virtual number_type exec(); //Evaluates and returns the value of the expression
	virtual void set(const char * expression, short lenght); //Sets the expression to be interpret
	virtual ~interpreter_interface();

	//EXPERIMENTAL
	virtual bool compile();
};


}
#endif // INTERPRETER_INTERFACE_H_INCLUDED
