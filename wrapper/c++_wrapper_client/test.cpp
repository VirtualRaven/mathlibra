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
    
    {
        ptr_protect<interface::calc_lib_interface*, false> calc(InitLib());
        for(string expr : testExpression){
            calc->set_arg(expr);
            auto test =test_error(calc.ptr());
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
