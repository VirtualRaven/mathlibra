#include "modules/memory.h"
#include <iostream>
#include "core/type.h"
#include "core/type_helper.h"
using memory::memoryObject;	
using interface::type_ptr;
	template<EXCEPTION T> void memoryOops()
	{
		__mathlibra__raise<T,MEMORY>();
	}

	memory::memoryObject::memoryObject(std::string name, number_type value, bool constant):
	    name(name),
	    value(interface::type_ptr(new base_type<double>(&value,1,1))),
	    constant(constant)
	    {}
        memory::memoryObject::memoryObject(std::string name, type_ptr&& value, bool constant):
             name(name),
	    value(std::move(value)),
	    constant(constant)
	    {}    
	memory::memoryObject::memoryObject() :
		name(""),
		value(nullptr),
		constant(false)
	{}
        memory::memoryObject::memoryObject(const memoryObject& e):
            name(e.name),
            value(std::move(type_ptr(e.value.ptr()->copy()))),
            constant(e.constant) {}
        memory::memoryObject::memoryObject(memoryObject&& e):
            name(std::move(e.name)),
            value(std::move(e.value)),
            constant(e.constant) {}
        memoryObject& memory::memoryObject::operator=(const memoryObject& e)
        {
            name=e.name;
            value= std::move(interface::type_ptr(e.value.ptr()->copy()));
            constant = e.constant;
            return *this;
        }
        memoryObject& memory::memoryObject::operator=(memoryObject&& e)
        {
            name=std::move(e.name);
            value =std::move(e.value);
            constant=e.constant;
            return *this;
        }
	type*  memory::memory::get(std::string var)
	{
	    return this->get_obj(var).value.ptr();
        }
	bool memory::memory::exists(std::string var)
        {
            mem_it = mem.find(var);
            return (mem_it != mem.end());
        }
        
        bool memory::memory::set(std::string var, double value, bool allocateIfNotFound, bool constant)
        {
           return this->set(var,type_ptr(make_type(value)),allocateIfNotFound,constant); 
        }
        bool memory::memory::set(std::string var, type_ptr&& value, bool allocateIfNotFound, bool constant)
	{
		mem_it =mem.find(var);
		if (mem_it == mem.end())
		{
			if (allocateIfNotFound)
			{
				mem[var] = std::move(memoryObject(var, std::move(value), constant));
				return true;
			}
			else
			{
				memoryOops<MEM_VAR_NOT_FOUND>();
				return false;
			}
		 }
				if (mem_it->second.constant == true)
				{
					memoryOops<MEM_ALTER_CONST_VAR>();
					return false;
				}
				mem_it->second.value = std::move(value);
				return true;
	}
        bool memory::memory::set_ignore_const(std::string var,  double value, bool allocateIfNotFound, bool constant)
        {
            return this->set_ignore_const(var,type_ptr(make_type(value)),allocateIfNotFound,constant);
        }
        bool memory::memory::set_ignore_const(std::string var,  type_ptr&& value, bool allocateIfNotFound, bool constant)
	{
		mem_it =mem.find(var);
		if (mem_it == mem.end())
		{
			if (allocateIfNotFound)
			{
				mem[var] = std::move(memoryObject(var, std::move(value), constant));
				return true;
			}
			else
			{
				memoryOops<MEM_VAR_NOT_FOUND>();
				return false;
			}
		 }
				mem_it->second.value = std::move(value);
				mem_it->second.constant = constant;
                                return true;
	}
	
	std::vector<std::string> memory::memory::allVars()
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
	void memory::memory::empty()
	{
		this->mem.clear();
	}
	 
	type* memory::memory::raw_ptr(std::string var)
	{
		mem_it = mem.find(var);
		if (mem_it == mem.end())
		{
			memoryOops<MEM_VAR_NOT_FOUND>();
			return nullptr;

		}
		else return mem_it->second.value.ptr();
	}
        memory::memoryObject  memory::memory::get_obj(std::string name)
        {
		mem_it = mem.find(name);
		if (mem_it == mem.end())
		{
			memoryOops<MEM_VAR_NOT_FOUND>();
			return {}; //Dummy expression to avoid warning, will never be executed
		}
		else return mem_it->second;	
        } 
        
        memory::memoryObject  memory::memory::get_obj(size_t index)
        {
            if (index >= this->get_size())
            {
		memoryOops<MEM_INDEX_OUT_OF_BOUNDS>();
		return mem_it->second; // Dummy expression to avoid warning,will never be executed
            } 
	    else
	    {   
		if(mem_it2 == mem.end())
		{
			mem_it2 = mem.begin();
			mem_it2_index=0;
		}


             	if(this->mem_it2_index == index)
            	{
            	    return mem_it2->second;
            	}
            	else if(this->mem_it2_index < index)
            	{
            	    for(unsigned int i = 0;i<(index - this->mem_it2_index);  i++)
            	    {
            	        this->mem_it2++;
            	    }
            	   this->mem_it2_index = index;
            	   return this->mem_it2->second;
            	}
            	else 
            	{
            	    for(unsigned int i =0; i< (this->mem_it2_index - index); i++)
            	    {
            	        this->mem_it2--;
            	    }
            	    this->mem_it2_index = index;
            	    return this->mem_it2->second; 
            	}
	    }

        }
        size_t memory::memory::get_size()
        {
           return this->mem.size(); 
        }

	memory::memory::memory() :
		mem(),
		mem_it(mem.end()),
		mem_it2(mem.end()),
		mem_it2_index(0) {}
/*
	bool test::memory_module_test1()
	{
		//Test allocation
		try
		{

			memory::memory mem1;
			memory::memory mem2;
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
			catch (exception& e)
			{

				
			}

			memory::memory* mem_ptr = &mem1;

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
		catch (exception& e)
		{
			std::cout << "Test failed, exception occured " << e.what()<< " " << e.desc() << "\n\n";
			return false;
		}
		std::cout << "Memory module test sucessfull!\n";
		return true;

	}
  */              
