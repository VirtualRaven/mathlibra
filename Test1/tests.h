#include "modules/functions.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "core/interpreter.h"
#include "core/corax_virtual_machine.h"
#include "core/core_math.h"
#include "profiling.h"
#include "core/export_lib_version.h"
#include "ptr_protect.h"

namespace test
{
	void profileInterpreter();
	void profileInterpreterVM();
	double argEvaluation();
}