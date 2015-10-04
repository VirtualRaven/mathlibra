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

		interface::wrapper_exception_info ex_inf;

	public:
		core_native_wrapper();
		~core_native_wrapper();
		void set_arg(std::string str);
		bool interpret_arg();
		double execute_arg();
		wrapper_exception_info get_exception_info();
		bool exceptionOccured();
		//Fúnction unit
		std::vector<std::string> getFunctionNames();

		//Memroy unit 
		std::vector<std::string> getVariableNames();
		double getVariableValue(std::string  name);
		mem_obj_api getVariable(std::string name);
                mem_obj_api getVariable(size_t index);
                void setVariable(std::string name, double value);
		void createVariable(std::string name, double value);
		void clearVariables();
                void manageVariable(std::string,double value,bool const);
                bool  isConstVariable(std::string);
                unsigned int getNumVariables();
                bool isDefined(std::string name);

		//Plugins
		void enablePlugins();

		
	};
}
