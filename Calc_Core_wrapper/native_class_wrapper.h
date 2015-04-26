#include "Core_Includes.h"
namespace native
{
	extern const char * CoreVersion ;
	struct wrapper_exception_info
	{
		std::string type;
		std::string desc;
		bool isCritical;
	};

	class core_native_wrapper
	{
		interpreter inter;
		memory mem;
		operators::operators_interface oper;
		math_func::function_interface functions;
		bool exception_occurred;

		wrapper_exception_info ex_inf;

	public:
		core_native_wrapper();
		void set_arg(std::string str);
		bool interpret_arg();
		double execute_arg();
		wrapper_exception_info get_exception_info();
		bool exceptionOccured();

		//Memroy unit 
		std::vector<std::string> getVariableNames();
		double getVariableValue(std::string  name);
		void setVariable(std::string name, double value);
		void createVariable(std::string name, double value);
		void clearVariables();

	

		
	};
}