/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifdef ENABLE_CORAX
#ifndef CORAXVM_INTERFACE_INCLUDED
 #define CORAXVM_INTERFACE_INCLUDED
  #include "modules/functions.h"
  #include "modules/memory.h"
  #include "modules/operators.h"
namespace interface
{




    class corax_program
    {
        public:
        virtual void clear()=0;
        virtual ~corax_program();
    };




  class corax_runtime_interface
    {
    public:
     virtual void setOperator(operators::operators_interface* operators) =0;
	 virtual void setMemory(memory::memory* mem) = 0;
     virtual void setFunction(math_func::function_interface* functions)=0;
     virtual number_type run(interface::corax_program * prgm_)=0;

     virtual ~corax_runtime_interface();

    };
}
 #endif // CORAXVM_INTERFACE_INCLUDED
#endif //ENABLE_CORAX
