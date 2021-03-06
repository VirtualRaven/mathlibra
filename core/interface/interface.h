/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED
#include "interface/interpreter_interface.h"
#include "interface/CoraxVM_interface.h"
#include "ptr_protect.h"

extern "C"
{
extern interface::interpreter_interface* create_interpreter();
//extern interface::corax_runtime_interface* create_corax_runtime();
//extern interface::corax_program* create_corax_program();

}

#endif // INTERFACE_H_INCLUDED
