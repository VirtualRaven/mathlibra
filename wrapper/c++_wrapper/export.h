#include "main_wrapper.h"

extern "C"
{
	interface::calc_lib_interface * InitLib();
        bool mem_error_occured();
	const char* LibVersion();
	const char* LibSHA();
}
