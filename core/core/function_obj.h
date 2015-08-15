#ifndef FUNC_OBJ_INCLUDED
#define FUNC_OBJ_INCLUDED
#include "interpreter.h"
#include "modules/memory.h"
namespace math_func
{
	class interpreted_func
	{
		memory::memory local_mem;
		memory::memory* global_mem;
		void __make_local_context(tree::node*);
		void __prepare_function();
		tree::rootNode __tree;
	public:
		double operator()(double);
		double exec(double);
		interpreted_func(tree::node *, memory::memory* mem);
		interpreted_func& operator=(tree::node*);
		interpreted_func& operator=(interpreted_func&& func);
		interpreted_func(interpreted_func&& func);
		~interpreted_func();
	};
}

#endif