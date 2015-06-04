#include "interface/interface.h"
#include "interpreter.h"
#include "corax_virtual_machine.h"
#include "ptr_protect.h"



interface::interpreter_interface* create_interpreter()
{
    return new interpreter;
}

interface::corax_runtime_interface* create_corax_runtime()
{
    return new CoraxVM::corax_runtime;
}
interface::corax_program* create_corax_program()
{
    return new CoraxVM::corax_program;
}
