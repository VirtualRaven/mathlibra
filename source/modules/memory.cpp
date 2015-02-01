#include "modules/memory.h"

	memoryOops::memoryOops(std::string info)
	{
		this->info = info;
	}
	const char* memoryOops::what()
	{
		return "Memory exception";
	}


	memoryObject::memoryObject(std::string name, number_type value, bool constant):
	    name(name),
	    value(value),
	    constant(constant)
	    {}
	memoryObject::memoryObject() :
		name(""),
		value(0),
		constant(false)
	{}


	number_type memory::get(std::string var)
	{
		mem_it = mem.find(var);
		if (mem_it == mem.end())
		{
			throw memoryOops("requested variable not found");
			return false;

		}
		else return mem_it->second.value;
	}
	bool memory::set(std::string var,number_type value, bool allocateIfNotFound , bool constant )
	{
		mem_it =mem.find(var);
		if (mem_it == mem.end())
		{
			if (allocateIfNotFound)
			{
				mem[var] = memoryObject(var, value, constant);
				return true;
			}
			else
			{
				throw memoryOops("requested variable not found");
				return false;
			}
		 }
				if (mem_it->second.constant == true)
				{
					throw memoryOops("Tried to alter constant variable");
					return false;
				}
				mem_it->second.value = value;
				return true;
	}
	
	std::vector<std::string> memory::allVars()
	{
		std::vector<std::string> re;
		re.resize(mem.size()); //Save a lot of reallocations
		for (this->mem_it  = mem.begin(); mem_it != mem.end(); mem_it++)
		{
			re.push_back(mem_it->second.name);
		}
		return re;
	}
	void memory::empty()
	{
		this->mem.clear();
	}
	 
	number_type* memory::raw_ptr(std::string var)
	{
		mem_it = mem.find(var);
		if (mem_it == mem.end())
		{
			throw memoryOops("requested variable not found");
			return false;

		}
		else return &mem_it->second.value;
	}