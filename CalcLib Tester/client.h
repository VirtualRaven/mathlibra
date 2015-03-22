#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED
#include "modules/functions.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "core/interpreter.h"
#include "core/corax_virtual_machine.h"
#include "core/core_math.h"
#include "core/profiling.h"
#include "ptr_protect.h"
#ifndef DEBUG
#pragma comment(lib, "CalcLib.lib")
#else
#pragma comment(lib, "CalcLibd.lib")
#endif
#include "stdErr.h"

#endif // CLIENT_H_INCLUDED
