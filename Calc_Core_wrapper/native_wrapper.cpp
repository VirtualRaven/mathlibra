#include "stdafx.h"
#include "native_class_wrapper.h"

namespace native
{
	core_native_wrapper::core_native_wrapper()
	{
		//Load functions into the function container
		this->functions.load(math_func::std_math_func);
		this->functions.load(math_func::std_math_num_func);
		this->functions.load(math_func::std_math_trig_func);
		this->functions.load(core_math::lib_core_math);

		//Load the function container into interpreter
		this->inter.setFunction(&this->functions);

		//Load operators into operator function
		this->oper.load(operators::std_operators);
		this->inter.setOperator(&this->oper);
		this->inter.setMemory(&this->mem);

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

	wrapper_exception_info core_native_wrapper::get_exception_info()
	{
		this->exception_occurred = false;
		wrapper_exception_info tmp = ex_inf;
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
			this->mem.set(name,value,false,false);
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
			this->exception_occurred = true;
			return;
		}
	}
	
};

	
