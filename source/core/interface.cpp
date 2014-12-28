#include "interface/interface.h"
#include "interpreter.h"
#include "ptr_protect.h"

interpreter_container create_interpreter()
{
    return interpreter_container(new interpreter);
}
