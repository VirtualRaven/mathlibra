#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED
#include "interface/interpreter_interface.h"
#include "interface/CoraxVM_interface.h"
#include "ptr_protect.h"
typedef ptr_protect<interface::interpreter_interface*,false> interpreter_container;
typedef ptr_protect<interface::corax_runtime_interface*,false> corax_runtime_container;
typedef ptr_protect<interface::corax_program*,false> corax_program_container;
extern "C"
{
   extern interpreter_container create_interpreter();
   extern corax_runtime_container create_corax_runtime();
   extern corax_program_container create_corax_program();

}

#endif // INTERFACE_H_INCLUDED
