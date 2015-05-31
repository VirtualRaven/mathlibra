#include "stdafx.h"
#include "managed_class_wrapper.h"
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>
#include "version.h"
namespace calculator
{

	calc_engine_exception::calc_engine_exception(String^ desc, String^ type, bool crit)
	{
		this->description = desc;
		this->type = type;
		this->isCritical = crit;
	}

	void calc_engine::throwIfFailed()
	{
		if (this->wrp->exceptionOccured())
		{
			interface::wrapper_exception_info tmp_exec = this->wrp->get_exception_info();
			String^ type = msclr::interop::marshal_as<String^>(tmp_exec.type);
			String^ desc = msclr::interop::marshal_as<String^>(tmp_exec.desc);
			throw gcnew calculator::calc_engine_exception(desc,type,tmp_exec.isCritical);
			return;
		}
	}

	calc_engine::calc_engine() : wrp(new native::core_native_wrapper)
	{

	}
	calc_engine::~calc_engine()
	{
		wrp->~core_native_wrapper();
		delete wrp;
		wrp = nullptr;
	}

	void calc_engine::SetArg(String^ str)
	{
		this->wrp->set_arg(msclr::interop::marshal_as<std::string>(str));
	}
	bool calc_engine::InterpretArg()
	{
		this->wrp->interpret_arg();
		this->throwIfFailed();
		return true;
		
		

	}
	double calc_engine::ExecuteArg()
	{
		double tmp = this->wrp->execute_arg();
		this->throwIfFailed();
		return tmp;

	}
	List<String^> ^ calc_engine::GetVariableNames()
	{
		std::vector<std::string> tmp = this->wrp->getVariableNames();
		this->throwIfFailed();
		List<String^>^ managed_list = gcnew List < String^ >(tmp.size());
		for (auto x = tmp.begin(); x != tmp.end(); x++)
		{
			managed_list->Add(gcnew String(msclr::interop::marshal_as<String^>(x->c_str())));
		}
		return managed_list;
	
	}
	double calc_engine::GetVariableValue(String^ name)
	{
		double tmp = this->wrp->getVariableValue(msclr::interop::marshal_as<std::string>(name));
		this->throwIfFailed();
		return tmp;
	}
	void calc_engine::SetVariable(String^ name, double value)
	{
		this->wrp->setVariable(msclr::interop::marshal_as<std::string>(name), value);
		this->throwIfFailed();
	}
	void calc_engine::CreateVariable(String^ name, double value)
	{
		this->wrp->createVariable(msclr::interop::marshal_as<std::string>(name), value);
		this->throwIfFailed();
	}
	void calc_engine::ClearVariables()
	{
		this->wrp->clearVariables();
		this->throwIfFailed();
	}
	void calc_engine::enablePlugins()
	{
		this->wrp->enablePlugins();
		this->throwIfFailed();
	}
	
	String^ calc_engine::get_Version()
		{
			{
				return msclr::interop::marshal_as<String^>(version::wrapper_git_version) + "-" + msclr::interop::marshal_as<String^>(native::CoreVersion);
			}
		}
	
	
	
}


//Test Libary
/*
int main()
{
	calculator::calc_engine^ eng = gcnew calculator::calc_engine();
	while (true)
	{

		try
		{
			
			eng->set_arg(System::Console::ReadLine());
			eng->interpret_arg();
			System::Console::WriteLine(eng->execute_arg());
		}
		catch (Exception^ e)
		{
			System::Console::WriteLine( e->ToString());
		}
	}

}
*/