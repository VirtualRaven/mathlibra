#include "Core_Includes.h"
#include "main_wrapper.h"
namespace native
{
	extern const char * CoreVersion ;
	

	class core_native_wrapper : public interface::calc_lib_interface
	{
		interpreter inter;
		memory mem;
		operators::operators_interface oper;
		math_func::function_interface functions;
		plugin::plugin_manager * manager;
		bool exception_occurred;

		interface::wrapper_exception_info ex_inf;

	public:
		core_native_wrapper();
		~core_native_wrapper();
		void set_arg(std::string str);
		bool interpret_arg();
		double execute_arg();
		interface::wrapper_exception_info get_exception_info();
		bool exceptionOccured();

		//Memroy unit 
		std::vector<std::string> getVariableNames();
		double getVariableValue(std::string  name);
		void setVariable(std::string name, double value);
		void createVariable(std::string name, double value);
		void clearVariables();

		//Plugins
		void enablePlugins();

		
	};
}