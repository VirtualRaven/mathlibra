#include "modules/memory.h"
#include <iostream>
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
		std::vector<std::string>::iterator it = re.begin();
		for (this->mem_it  = mem.begin(); mem_it != mem.end(); mem_it++)
		{
			*it = (mem_it->second.name);
			it++;
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
			return nullptr;

		}
		else return &mem_it->second.value;
	}

	bool test::memory_module_test1()
	{
		//Test allocation
		try
		{

			memory mem1;
			memory mem2;
			if (!mem1.set("1", 4242) || !mem1.set("he1", 4242) || !mem1.set("he2", 1111, true, true))
			{
				std::cout << "Failed to allocate test range\n";
				return false;
			}
			mem2 = mem1;
			if (mem2.get("1") != 4242 || mem2.get("he1") != 4242 || mem2.get("he2") != 1111)
			{
				std::cout << "Failed to retrive allocated objects\n";
				return false;
			}
			if (!mem1.set("1", 42) || mem1.get("1") != 42 || !mem1.set("he1", 42) || mem1.get("he1") != 42)
			{
				std::cout << "Failed to change allocated objects\n";
				return false;
			}
			try
			{
				mem1.set("he2", 1111);
				std::cout << "Failed to protect const\n";
				return false;
			}
			catch (memoryOops& e)
			{

				
			}

			memory* mem_ptr = &mem1;

			number_type* x = mem_ptr->raw_ptr("he2");
			if (*x != 1111)
			{
				std::cout << "Failed to read raw_ptr";
			}
			if (!mem_ptr->set("x", 1) || !mem_ptr->set("y", 2) || !mem_ptr->set("z", 3))
			{
				std::cout << "Failed to set x,y,z through pointer\n";
				return false;
			}


			number_type *z = mem_ptr->raw_ptr("z");
			*x = 622;
			if (*x != 622 || *z != 3)
			{
				std::cout << "Failed to get elements by pointer\n";
			}

			mem_ptr->set("z", 22);
			*x = 56;
			if (*x != 56 || *z != 22)
			{
				std::cout << "Failed to get elements by pointer after update\n";
			}

			std::vector<std::string> vec = mem_ptr->allVars();
			if (vec[0] != "1" || vec[1] != "he1" || vec[2] != "he2" || vec[3] != "x" || vec[4] != "y" || vec[5] != "z")
			{
				std::cout << "allVars failed\n";
				return false;
			}
		}
		catch (memoryOops& e)
		{
			std::cout << "Test failed, exception occured " << e.what()<< " " << e.desc() << "\n\n";
			return false;
		}
		std::cout << "Memory module test sucessfull!\n";
		return true;

	}