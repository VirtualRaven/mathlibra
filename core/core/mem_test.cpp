#include "mem_test.h"
#include "object_counter.h"
#include "exception_helper.h"
#include "type_interface.h"
#include "tree_interface.h"
#include <iostream>
#ifdef TYPE_MEM_TEST
template<typename T> void leak_check(typename util::object_stats<T> stats)
{
	std::cerr << "COUNTABLE ASSERT RUNNING\n" << "STATS: \n" << "total: " << stats.get_total() << "\ncurrent: " << stats.get_current() << "\ndeleted: " << stats.get_deleted() << std::endl;
	if(stats.get_total() != stats.get_deleted())
	{
		__mathlibra__raise<MEMORY_LEAK_ASSERT,INTERPRETER>();
	}	
}
#endif

void debug::check_tree_mem_leak()
{
#ifdef TYPE_MEM_TEST
	std::cerr << "DEBUG LEAK CHECK\n\nNodes:\n";
	leak_check(util::object_stats<tree::nodeDataInterface>());
	std::cerr<< "\nTypes:\n";
	leak_check(util::object_stats<interface::type>());
#else
	std::cerr << "This library was built with memory leak check disabled\n";
#endif
}
bool debug::mem_error_occured() noexcept
{
    try{
#ifdef TYPE_MEM_TEST
        debug::check_tree_mem_leak();
#endif
        return false;
    }
    catch(...){
        return true;
    }
}
