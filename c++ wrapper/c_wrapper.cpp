// c++ wrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "export.h"
#include "native_class_wrapper.h"
interface::calc_lib_interface* LibInit()
{
	return  dynamic_cast<interface::calc_lib_interface*>( new native::core_native_wrapper);
}




