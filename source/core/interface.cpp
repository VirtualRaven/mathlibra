#include "interface/interface.h"
#include "interpreter.h"
interface::interpreter_interface* create_interpreter()
{
    return new interpreter;
}
