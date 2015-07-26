#include "core/mathNode_interface.h"
#include "core/tree.h"
#include "modules/function_helper.h"
namespace internal_helper
{
	template<typename T, typename T2> double forward_fast(typename function_helper::func_type<T, T2>::type func, tree::nodeDataInterface*n)
	{
		auto wrapper = tree::nodeDataInterface_wrapper_access(n);
		
		return func(function_helper::getData<T>(wrapper->sub1()), function_helper::getData<T2>(wrapper->sub2()));
	}

}
