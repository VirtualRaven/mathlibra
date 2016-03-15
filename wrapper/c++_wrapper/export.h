/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "main_wrapper.h"

extern "C"
{
	interface::calc_lib_interface * InitLib();
        bool mem_error_occured();
	const char* LibVersion();
	const char* LibSHA();
}
