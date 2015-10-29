#include "native_class_wrapper.h"
#include "core/export_lib_version.h"
namespace native
{
	const char * CoreVersion = LIB_VERSION;
	core_native_wrapper::core_native_wrapper() : manager(plugin::get_platform_specific_manager()) , exception_occurred(false)
	{

		
		//Load functions into the function container
		this->functions.load(math_func::std_math_func);
		this->functions.load(math_func::std_math_num_func);
		this->functions.load(math_func::std_math_trig_func);
		
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
	void core_native_wrapper::set_arg(std::string str)
	{

		try
		{
			inter.set(str.c_str(), str.length());
		}
		catch (exception& e)
		{
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->exception_occurred = true;

		}
		catch (...)
		{
			this->exception_occurred = true;
		}
	}
	bool core_native_wrapper::interpret_arg()
	{
		try
		{
			return this->inter.interpret();
		}
		catch (exception& e)
		{
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->exception_occurred = true;
			return false;
		}
		catch (...)
		{
			this->exception_occurred = true;
			return false;
		}

	}
	double core_native_wrapper::execute_arg()
	{
		
		try
			
		{	
			return this->inter.exec();
		}
		catch (exception& e)
		{
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->exception_occurred = true;
			return 0;
		}
		catch (...)
		{
			this->exception_occurred = true;
			return false;
		}
	}

	interface::wrapper_exception_info core_native_wrapper::get_exception_info()
	{
		this->exception_occurred = false;
		interface::wrapper_exception_info tmp = ex_inf;
		ex_inf.desc.clear();
		ex_inf.desc.clear();
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
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->exception_occurred = true;
			return std::vector<std::string>();
		}
	}

	double core_native_wrapper::getVariableValue(std::string  name)
	{
		try
		{
			return this->mem.get(name);
		}
		catch (exception& e)
		{
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->exception_occurred = true;
			return 0;
		}
		
	}
	void core_native_wrapper::setVariable(std::string name, double value)
	{
		try
		{
			this->mem.set_ignore_const(name,value,false,false);
		}
		catch (exception& e)
		{
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->exception_occurred = true;
			return;
		}
	}
	void core_native_wrapper::createVariable(std::string name, double value)
	{
		try
		{
			this->mem.set(name,value,true,false);
		}
		catch (exception& e)
		{
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->exception_occurred = true;
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
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->ex_inf.isCritical = e.critical();
			this->exception_occurred = true;
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
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->ex_inf.isCritical = e.critical();
			this->exception_occurred = true;
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
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->ex_inf.isCritical = e.critical();
			this->exception_occurred = true;
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
						});
			}

			return	vec;	
		}
		catch (exception& e)
		{
			this->ex_inf.type = e.what();
			this->ex_inf.desc = e.desc();
			this->ex_inf.isCritical = e.critical();
			this->exception_occurred = true;
			return std::vector<interface::func_obj_api>();
		}
	}
	void core_native_wrapper::manageVariable(std::string name,double value,bool isConst)
        {
            try
            {
               this->mem.set_ignore_const(name,value,true,isConst);
            }
            catch (exception& e)
	    {
		this->ex_inf.type = e.what();
		this->ex_inf.desc = e.desc();
		this->ex_inf.isCritical = e.critical();
		this->exception_occurred = true;
	    }
        }
        bool  core_native_wrapper::isConstVariable(std::string name)
        {
            try
            {
                auto obj = this->mem.get_obj(name);
                return obj.constant;
            }
            catch (exception& e)
	    {
		this->ex_inf.type = e.what();
		this->ex_inf.desc = e.desc();
		this->ex_inf.isCritical = e.critical();
		this->exception_occurred = true;
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
		this->ex_inf.type = e.what();
		this->ex_inf.desc = e.desc();
		this->ex_inf.isCritical = e.critical();
		this->exception_occurred = true;
	    }
            return{};
        }
	mem_obj_api core_native_wrapper::getVariable(std::string name)
        {
          
            try
            {
               auto obj = this->mem.get_obj(name);
               mem_obj_api tmp_obj;
               tmp_obj.value = obj.value;
               tmp_obj.isConst = obj.constant;
               tmp_obj.name = obj.name;
               return tmp_obj;
            }
            catch (exception& e)
	    {
		this->ex_inf.type = e.what();
		this->ex_inf.desc = e.desc();
		this->ex_inf.isCritical = e.critical();
		this->exception_occurred = true;
	    }  
            return {};
        }
        mem_obj_api core_native_wrapper::getVariable(size_t index)
        {
            try
            {
               auto obj = this->mem.get_obj(index);
               mem_obj_api tmp_obj;
               tmp_obj.value = obj.value;
               tmp_obj.isConst = obj.constant;
               tmp_obj.name = obj.name;
                return tmp_obj; 
            }
            catch (exception& e)
	    {
		this->ex_inf.type = e.what();
		this->ex_inf.desc = e.desc();
		this->ex_inf.isCritical = e.critical();
		this->exception_occurred = true;
	    }
            return {};
        }
                
}

	
