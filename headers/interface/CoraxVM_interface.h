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
        virtual bool serilize();
        virtual void clear();
        virtual ~corax_program();
    };




  class corax_runtime_interface
    {
    public:
     virtual void setOperator(operators::operators_interface* operators);
     virtual void setMemory(memory* mem);
     virtual void setFunction(math_func::function_interface* functions);
     virtual number_type run(interface::corax_program * prgm_);
     virtual number_type debug(interface::corax_program *prgm_);
     virtual ~corax_runtime_interface();

    };
}
 #endif // CORAXVM_INTERFACE_INCLUDED
