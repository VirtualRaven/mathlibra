#include "interface/interface.h"
#include "interpreter.h"
#include "corax_virtual_machine.h"
#include "ptr_protect.h"

interpreter_container create_interpreter()
{
    return interpreter_container(new interpreter);
}

corax_runtime_container create_corax_runtime()
{
    return corax_runtime_container(new CoraxVM::corax_runtime);
}
corax_program_container create_corax_program()
{
    return corax_program_container(new CoraxVM::corax_program);
}
