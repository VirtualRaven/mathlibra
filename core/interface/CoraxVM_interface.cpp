/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "interface/CoraxVM_interface.h"
namespace interface
{


#ifdef ENABLE_CORAX
    corax_program::~corax_program()
    {

    }
     corax_runtime_interface::~corax_runtime_interface()
    {
    //dtor
    }
#endif // ENABLE_CORAX

}
