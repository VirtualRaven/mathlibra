/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "test.h"
#include "main_wrapper.h"
#include "interpreter.h"
#include "ptr_protect.h"
#include "export.h"
#include <iostream>
RETURN_VAL test_error(interface::calc_lib_interface* calc)
{
	if (calc->exceptionOccured())
	{
		auto excp = calc->get_exception_info();
                std::cerr << "-[ ("<<excp.id<<") "<< excp.type << ": " << excp.desc << " ]\n";
                if(excp.isCritical)
                {
                    return RETURN_VAL::CRITICAL_EXCEPTION;
                } else return RETURN_VAL::EXCEPTION;
	        	
	}
	else return RETURN_VAL::SUCCESS;
}
RETURN_VAL runTest(vector<string> testExpression )
{
    
//This extra bracket is here to ensure that calc has gone out of
//scope before calling mem_error_occurred
    {
        ptr_protect<interface::calc_lib_interface*, false> calc(InitLib());
	calc->set_arg("x^3");
	calc->defineFunction("cube");
	auto test = test_error(calc.ptr());
        if( test != RETURN_VAL::SUCCESS){
		std::cerr << "-[ PREVIOUS ERROR CAUSED BY defineFunction() \n";
                return test;   
        }
        for(string expr : testExpression){
            calc->set_arg(expr);
            test =test_error(calc.ptr());
            if( test != RETURN_VAL::SUCCESS){
                return test;   
            }
            calc->interpret_arg();
            test = test_error(calc.ptr());
            if( test != RETURN_VAL::SUCCESS){
                return test;   
            }
            calc->execute_arg();
            test = test_error(calc.ptr());
            if( test != RETURN_VAL::SUCCESS){
                return test;   
            }
        }
    }
    if(mem_error_occured() )
    {
        return RETURN_VAL::MEM_ERROR;
    }
    return RETURN_VAL::SUCCESS;

}
