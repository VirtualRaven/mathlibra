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
	/*Warning the interpeter does not copy the memory object.
	Therefore must the pointer remain valid throug the lifetime of the interpreter object*/

	virtual void setMemory(memory* mem);
	virtual void setFunction(math_func::function_interface* functions);
	virtual void setOperator(operators::operators_interface* operators);
	virtual bool interpret();
	virtual number_type exec();
	virtual void set(const char * expression, short lenght);
	virtual ~interpreter_interface();
};


}
#endif // INTERPRETER_INTERFACE_H_INCLUDED
