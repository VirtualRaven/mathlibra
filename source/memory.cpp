#include "memory.h"

	memoryOops::memoryOops(std::string info)
	{
		this->info = info;
	}
	const char* memoryOops::what()
	{
		return "Memory exception";
	}


	memoryObject::memoryObject(std::string name, number_type value, bool constant)
	{
		this->name = name;
		this->value = value;
		this->constant = constant;
	}



	number_type memory::get(std::string var)
	{
		for (unsigned int i = 0; i < mem.size(); i++)
		{
			if (mem[i].name == var)
			{
				return mem[i].value;
			}
		}
		throw memoryOops("requested variable not found");
		return false;

	}
	bool memory::set(std::string var,number_type value, bool allocateIfNotFound , bool constant )
	{
		for (unsigned int i = 0; i < mem.size(); i++)
		{
			if (mem[i].name == var)
			{
				if (mem[i].constant == true)
				{
					throw memoryOops("Tried to alter constant variable");
					return false;
				}
				mem[i].value = value;
				return true;
			}
		}
		//Variable not found
		if (allocateIfNotFound)
		{
			mem.push_back(memoryObject(var, value,constant));
			return true;

		}
		else
		{
			throw memoryOops("requested variable not found");
			return false;
		}
	}
	std::vector<std::string> memory::allVars()
	{
		std::vector<std::string> re;

		for (int i = 0; i < mem.size(); i++)
		{
			re.push_back(mem[i].name);
		}
		return re;
	}
	void memory::empty()
	{
		this->mem.clear();
	}
