#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include "exception_helper.h"
#include "main.h"

/**
 * @namespace memory includes the memory module
 */
namespace memory 
{

	
	

	struct memoryObject
	{
		std::string name;
		number_type value;
		bool constant;
		memoryObject(std::string name, number_type value, bool constant = false);
		memoryObject();
	};

	class memory
	{
		std::map<std::string, memoryObject> mem;
		std::map<std::string, memoryObject>::iterator mem_it;
		std::map<std::string, memoryObject>::iterator mem_it2;
                size_t mem_it2_index;
	public:
		number_type get(std::string var);
		bool set(std::string var, number_type value, bool allocateIfNotFound = true, bool constant = false);
		bool set_ignore_const(std::string var, number_type value, bool allocateIfNotFound=false, bool constant=false);
                std::vector<std::string> allVars();
		bool exists(std::string var);
                void empty();
		number_type* raw_ptr(std::string var);

                //Updated external api
                memoryObject get_obj(std::string name); /**<Get an memoryObject by name. Tries to return an memoryObject registered as name. Throws an memoryOops if an undefined variable is requested. @returns An copy of the internal memoryObject. @see memory::exists. */     
                memoryObject get_obj(size_t index); /**< Get an memoryObject by index. Returns the memoryOject number index, use size to get maximum allowed index. @see memory::size. @note Due to the implementation of the internal oemory container the memory class do not support random access by integer index, instead it itterates through the structure to get the index. Thus get_obj(size_t index) is O(n) but provides caching to spead up sequential reads.  */
                size_t get_size();

	};

}

namespace test
{
	bool memory_module_test1();
}

#endif // MEMORY_H_INCLUDED
