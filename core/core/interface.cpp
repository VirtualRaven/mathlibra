/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "interface/interface.h"
#include "interpreter.h"
#include "corax_virtual_machine.h"
#include "ptr_protect.h"



interface::interpreter_interface* create_interpreter()
{
    return new interpreter;
}
#ifdef ENABLE_CORAX
interface::corax_runtime_interface* create_corax_runtime()
{
    return new CoraxVM::corax_runtime;
}
interface::corax_program* create_corax_program()
{
    return new CoraxVM::corax_program;
}
#endif
