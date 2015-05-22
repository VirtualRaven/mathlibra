#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED
#include "modules/functions.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "core/interpreter.h"
#include "core/corax_virtual_machine.h"

#include "profiling.h"
#include "plugins/plugin_loader.h"
#include "core/export_lib_version.h"
#include "ptr_protect.h"
#ifdef WINDOWS
#pragma comment(lib, "CalcLib.lib")
#endif //WINDOWS
#include "stdErr.h"

#endif // CLIENT_H_INCLUDED
