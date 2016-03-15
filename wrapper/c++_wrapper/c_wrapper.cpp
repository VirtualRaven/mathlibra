/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
// c++ wrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "export.h"
#include "native_class_wrapper.h"
#include "mem_test.h"
interface::calc_lib_interface* InitLib()
{
	return  dynamic_cast<interface::calc_lib_interface*>( new native::core_native_wrapper);
}


const char* LibVersion()
{
	return LIB_VERSION;	
}
const char* LibSHA()
{
	return LIB_SHA;
}

bool mem_error_occured()
{
    return debug::mem_error_occured();
}
