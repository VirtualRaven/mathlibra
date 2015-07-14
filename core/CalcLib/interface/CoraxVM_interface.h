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
