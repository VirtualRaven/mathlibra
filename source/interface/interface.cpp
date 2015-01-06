#include "interface/interpreter_interface.h"
namespace interface
{


	/*Warning the interpeter does not copy the memory object.
	Therefore must the pointer remain valid throug the lifetime of the interpreter object*/

	 void interpreter_interface::setMemory(memory* mem)
	 {

	 }
	 void interpreter_interface::setFunction(math_func::function_interface* functions)
	 {

	 }
	 void interpreter_interface::setOperator(operators::operators_interface* operators)
	 {

	 }
	 bool interpreter_interface::interpret()
	 {
        return false;
	 }
	 number_type interpreter_interface::exec()
	 {
        return -1;
	 }
	 void interpreter_interface::set(const char * expression, short lenght)
	 {

	 }
	 interpreter_interface::~interpreter_interface()
	 {

	 }
	 bool interpreter_interface::compile()
	 {
	     return false;
	 }



}
