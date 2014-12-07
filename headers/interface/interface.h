#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED
#include "interface/interpreter_interface.h"

extern "C"
{
   extern interface::interpreter_interface* create_interpreter();

}

#endif // INTERFACE_H_INCLUDED
