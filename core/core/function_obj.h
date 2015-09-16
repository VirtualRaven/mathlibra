#ifndef FUNC_OBJ_INCLUDED
#define FUNC_OBJ_INCLUDED

#include "tree.h"
#include "modules/memory.h"

namespace function_obj /**< function_obj object to export interpreted expressions. */
{
        /**
         * interpreted_func represents an function object. Interpreted_func creates an an function-
         * object from the provided subtree. The function object provides an local memory context for functions
         * and provides the methods to run the interpreted expression.
         * interpreted_func can be viewed as an functor which is created from an interpreted expression
         */
	class interpreted_func
	{
		memory::memory local_mem;
		memory::memory* global_mem;
		void __make_local_context(tree::node*);
		void __prepare_function();
		tree::rootNode __tree;
	public:
		double operator()(double);
		double exec(double); /**< Evaluate the function. Provides the argument as variable x to the expression*/
		interpreted_func(tree::node *, memory::memory* mem); /**< Create an new interpreted_func. THe  function takes owner ship of the provided tree and creates an local copy of the mem module. **/
		interpreted_func& operator=(tree::node*);
		interpreted_func& operator=(interpreted_func&& func);
		interpreted_func(interpreted_func&& func); /**< Steal func. Steals the interal tree from func and sets func to an empty state.*/
		~interpreted_func();
	};
}

#endif
