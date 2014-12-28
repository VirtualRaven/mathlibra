#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED
#include "interface/interpreter_interface.h"
#include "ptr_protect.h"
typedef ptr_protect<interface::interpreter_interface*,false> interpreter_container;
extern "C"
{
   extern interpreter_container create_interpreter();

}

#endif // INTERFACE_H_INCLUDED
