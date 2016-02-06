#include "main_wrapper.h"

extern "C"
{
	interface::calc_lib_interface * InitLib();
	const char* LibVersion();
	const char* LibSHA();
}
