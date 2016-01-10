#include "Core_Includes.h"
#include "main_wrapper.h"
namespace native
{
	extern const char * CoreVersion ;
        using interface::mem_obj_api;
        using interface::wrapper_exception_info;        

	class core_native_wrapper : public interface::calc_lib_interface
	{
		interpreter inter;
		memory::memory mem;
		operators::operators_interface oper;
		math_func::function_interface functions;
		plugin::plugin_manager * manager;
		bool exception_occurred;
                void __handle(exception& e);
		interface::wrapper_exception_info ex_inf;

	public:
		core_native_wrapper();
		~core_native_wrapper();
		void set_arg(std::string str);
		void interpret_arg();
		interface::type_ptr execute_arg();
		wrapper_exception_info get_exception_info();
		bool exceptionOccured();
		//F�nction unit
		std::vector<std::string> getFunctionNames();
		std::vector<interface::func_obj_api> getFunctionObjs();

		//Memroy unit 
		std::vector<std::string> getVariableNames();
		interface::type* 	getVariableValue(std::string  name);
		mem_obj_api getVariable(std::string name);
                mem_obj_api getVariable(size_t index);
                void setVariable(std::string name, interface::type_ptr&& value);
		void createVariable(std::string name, interface::type_ptr&& value);
		void clearVariables();
                void manageVariable(std::string,interface::type_ptr&& value,bool isConst);
                bool  isConstVariable(std::string);
                unsigned int getNumVariables();
                bool isDefined(std::string name);

		//Plugins
		void enablePlugins();

		
	};
}
