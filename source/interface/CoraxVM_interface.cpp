#include "CoraxVM_interface.h"
namespace interface
{


        bool corax_program::serilize()
        {
            return false;
        }
        void corax_program::clear()
        {
            return;
        }

    corax_program::~corax_program()
    {

    }
     corax_runtime_interface::~corax_runtime_interface()
    {
    //dtor
    }
     void corax_runtime_interface::setOperator(operators::operators_interface* operators)
     {
         return;
     }
     void  corax_runtime_interface::setMemory(memory* mem)
     {
         return;
     }
     void  corax_runtime_interface::setFunction(math_func::function_interface* functions)
     {
         return;
     }
     number_type  corax_runtime_interface::run(interface::corax_program * prgm)
     {
         return -1;
     }
     number_type  corax_runtime_interface::debug(interface::corax_program *prgm)
     {
         return -1;
     }

}
