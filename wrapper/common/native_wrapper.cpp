/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "native_class_wrapper.h"
#include "core/export_lib_version.h"
#include "function_obj.h"
#include "exception.h"
#include "type_helper.h"
#include "exception_helper.h"
namespace native
{

	template<EXCEPTION T> void wrapperOops()
	{
		__mathlibra__raise<T,WRAPPER>();	
	}	
	const char * CoreVersion = LIB_VERSION;
	core_native_wrapper::core_native_wrapper() : manager(plugin::get_platform_specific_manager()) , exception_occurred(false)
	{

		
		//Load functions into the function container
		this->functions.load(math_func::std_math_func);
		this->functions.load(math_func::std_math_num_func);
		this->functions.load(math_func::std_math_trig_func);
		this->functions.load(math_func::mathlibra_data_constructors);	
		//Load the function container into interpreter
		this->inter.setFunction(&this->functions);

		//Load operators into operator function
		this->oper.load(operators::std_operators);
		this->inter.setOperator(&this->oper);
		this->inter.setMemory(&this->mem);

	}
	core_native_wrapper::~core_native_wrapper()
	{
		delete this->manager;
	}

        void core_native_wrapper::__handle(exception& e)
        {
            this->ex_inf.type = e.what();
            this->ex_inf.desc = e.desc();
            this->ex_inf.isCritical = e.critical();
            this->ex_inf.id = e.id();
            this->exception_occurred=true;
        }
	void core_native_wrapper::set_arg(std::string str)
	{

		try
		{
			inter.set(str.c_str(), str.length());
		}
		catch (exception& e)
		{
		    this->__handle(e);
                }
	}
	void core_native_wrapper::interpret_arg()
	{
		try
		{
			this->inter.interpret();
		}
		catch (exception& e)
		{
		    this->__handle(e);
		}

	}
	interface::type_ptr core_native_wrapper::execute_arg()
	{
		
		try
			
		{	
			return this->inter.exec();
		}
		catch (exception& e)
		{
		    this->__handle(e);
		    return interface::type_ptr(nullptr);
		}
	}

	interface::wrapper_exception_info core_native_wrapper::get_exception_info()
	{
		this->exception_occurred = false;
		interface::wrapper_exception_info tmp = ex_inf;
		ex_inf.desc=nullptr;
                ex_inf.isCritical=false;
                ex_inf.id=0;
		ex_inf.desc=nullptr;
		return tmp;
	}

	bool core_native_wrapper::exceptionOccured()
	{
		return this->exception_occurred;
	}
	std::vector<std::string> core_native_wrapper::getVariableNames()
	{
		try
		{
			return this->mem.allVars();
		}
		catch (exception& e)
		{
		    this->__handle(e);
			return std::vector<std::string>();
		}
	}

	interface::type* core_native_wrapper::getVariableValue(std::string  name)
	{
		try
		{
			return this->mem.get(name);
		}
		catch (exception& e)
		{
		    this->__handle(e);
			return 0;
		}
		
	}
	void core_native_wrapper::setVariable(std::string name, interface::type_ptr&& value)
	{
		try
		{
			this->mem.set_ignore_const(name,std::move(value),false,false);
		}
		catch (exception& e)
		{
		    this->__handle(e);
			return;
		}
	}
	void core_native_wrapper::createVariable(std::string name, interface::type_ptr&& value)
	{
		try
		{
			this->mem.set(name,std::move(value),true,false);
		}
		catch (exception& e)
		{
		    this->__handle(e);
			return;
		}
	}
	void core_native_wrapper::clearVariables()
	{
		try
		{
			this->mem.empty();
		}
		catch (exception& e)
		{
		    this->__handle(e);
			return;
		}
	}

	void core_native_wrapper::enablePlugins()
	{
		try
		{
			this->manager->loadPlugins(&this->functions);
		}
		catch (exception& e)
		{
		    this->__handle(e);
			return;
		}
	}
	std::vector<std::string> core_native_wrapper::getFunctionNames()
	{
		try
		{
			return this->functions.getFunctionNames();
		}
		catch (exception& e)
		{
		    this->__handle(e);
			return std::vector<std::string>();
		}
	}
	std::vector<interface::func_obj_api> core_native_wrapper::getFunctionObjs()
	{
		try	
		{	std::vector<interface::func_obj_api> vec;
		       	auto in_vec = this->functions.getFunctionObjs();
			vec.reserve(in_vec.size());
			for(size_t i =0; i<in_vec.size(); i++)
			{	
				vec.push_back({std::move(in_vec[i].name),
						std::move(in_vec[i].tag),
						std::move(in_vec[i].doc),
						std::move(in_vec[i].disp_name),
                                                std::move(in_vec[i].signature)
						});
			}

			return	vec;	
		}
		catch (exception& e)
		{
		    this->__handle(e);
                    return std::vector<interface::func_obj_api>();
		}
	}
	void core_native_wrapper::manageVariable(std::string name,interface::type_ptr&& value,bool isConst)
        {
            try
            {
               this->mem.set_ignore_const(name,std::move(value),true,isConst);
            }
            catch (exception& e)
	    {
		    this->__handle(e);
	    }
        }
        bool  core_native_wrapper::isConstVariable(std::string name)
        {
            try
            {
		memory::memoryObject& obj = this->mem.get_obj(name);
                return obj.constant;
            }
            catch (exception& e)
	    {
		    this->__handle(e);
	    }    
            return {};
        }
        unsigned int core_native_wrapper::getNumVariables()
        {
            return  this->mem.get_size(); 
        }
        bool core_native_wrapper::isDefined(std::string name)
        {
            try
            {
               return this->mem.exists(name);
            }
            catch (exception& e)
	    {
		    this->__handle(e);
	    }
            return{};
        }
	mem_obj_api core_native_wrapper::getVariable(std::string name)
        {
          
            try
            {
               memory::memoryObject& obj = this->mem.get_obj(name);
               mem_obj_api tmp_obj;
               tmp_obj.value = obj.value.ptr();
               tmp_obj.isConst = obj.constant;
               tmp_obj.name = obj.name;
               return tmp_obj;
            }
            catch (exception& e)
	    {
		    this->__handle(e);
	    }  
            return {};
        }
        mem_obj_api core_native_wrapper::getVariable(size_t index)
        {
            try
            {
	       memory::memoryObject& obj = this->mem.get_obj(index);
               mem_obj_api tmp_obj;
               tmp_obj.value = obj.value.ptr();
               tmp_obj.isConst = obj.constant;
               tmp_obj.name = obj.name;
                return tmp_obj; 
            }
            catch (exception& e)
	    {
		    this->__handle(e);
	    }
            return {};
        }
	void core_native_wrapper::defineFunction(std::string name,bool visible)
	{
		try{

			function_obj::interpreted_func f(inter.createFunction());
			//Ensure that no name collision occurs
			this->undefineFunction(name);
			if(visible)
			{
				functions.load(m_function(name,
							"API_DEFINED",
							"",
							name,
							new interpreted_func(std::move(f))
							));
			}
			else
			{
				api_funcs.insert(std::pair<std::string,function_obj::interpreted_func>(std::move(name),std::move(f)));	
			}
		}
		catch(exception& e)
		{
			this->__handle(e);
		}
		return;
	}
	void core_native_wrapper::undefineFunction(std::string name)
	{
		auto it = api_funcs.find(name);
		if (it != api_funcs.end())
		{
			api_funcs.erase(it);
		}
		functions.unload(name);
	}

	interface::calc_lib_interface::double_array_ptr core_native_wrapper::map(double Start,
					double End,
					double Offset,
					std::string Function)
		{
			try{
				interpreted_func* f = nullptr;
				auto it = api_funcs.find(Function);
				if(it == api_funcs.end())
				{
					if(functions.isloaded(Function).loaded && functions.type() == math_func::func_type::USER)
					{
						f = functions.getFunctor(Function);
					}
					else
					{
						wrapperOops<MAP_FUNCTION_NOT_FOUND>();
					}				
				}
				else
				{
					f = &it->second;
				}
				int buffsize = (int)(2*(End-Start)/Offset)+2;
				double_array_ptr buffer(new double[buffsize]);
				auto ptr = buffer.ptr();
				for(int i =0; i < buffsize; i+=2)
				{
					double val = Start+(i/2)*Offset;
					ptr[i]=val;
					interface::type_ptr ret(f->exec(interface::type_ptr(make_type(val))));
					if(ret->isNumber())
					{
						ptr[i+1] = ret->toNumber();
					}		
					else
					{
						wrapperOops<MAP_FUNCTION_NOT_NUMER_TYPE>();
					}
				}
				return buffer;

			}
			catch(exception& e)
			{
				this->__handle(e);
				return double_array_ptr(nullptr);
			}

		}
                
}

	
