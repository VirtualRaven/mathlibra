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
