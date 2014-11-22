#include <string>
#include <vector>
#include "exception.h"
struct memoryOops : public exception
{
	memoryOops(std::string info)
	{
		this->info = info;
	}
	const char* what()
	{
		return "Memory exception";
	}
};

template<typename mathType>
struct memoryObject
{
	std::string name;
	mathType value;
	bool constant;
	
	memoryObject(std::string name, mathType value, bool constant = false)
	{
		this->name = name;
		this->value = value; 
		this->constant = constant;
	}
	
};
template<typename mathType>
class memory
{
	std::vector<memoryObject<mathType>> mem;
public:
	mathType get(std::string var)
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
	bool set(std::string var,mathType value, bool allocateIfNotFound = true, bool constant = false)
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
			mem.push_back(memoryObject<mathType>(var, value,constant));
			return true;
			
		}
		else
		{
			throw memoryOops("requested variable not found");
			return false;
		}
	}
	std::vector<std::string> allVars()
	{
		std::vector<std::string> re;

		for (int i = 0; i < mem.size(); i++)
		{
			re.push_back(mem[i].name);
		}
		return re;
	}
	void empty()
	{
		this->mem.clear();
	}
};