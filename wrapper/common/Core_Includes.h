#ifndef CORE_INCLUDED
#define CORE_INCLUDED

#include "core/interpreter.h" //Class containing the interpreter object
#include "core/export_lib_version.h"
#include "modules/functions.h" //The function container for interpreter
#include "modules/memory.h"		//The memory container for the interpreter
#include "modules/operators.h"	//The operetor container for the inrpreter
#include "plugins/plugin_loader.h" //Plugin interface for loading external plugin into core
#include "exception.h"
#endif //CORE_INCLUDED
