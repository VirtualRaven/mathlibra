#ifndef LIB_INTERFACE_INCLUED
#define LIB_INTERFACE_INCLUED
#include <vector>
#include <string>
namespace interface
{

	struct wrapper_exception_info
	{
		std::string type;
		std::string desc;
		bool isCritical;
	};

	class calc_lib_interface
	{
	public:
		virtual void set_arg(std::string str) = 0;
		virtual bool interpret_arg() = 0;
		virtual double execute_arg() = 0;
		virtual wrapper_exception_info get_exception_info() = 0;
		virtual bool exceptionOccured() = 0;

		//Function unit
		virtual std::vector<std::string> getFunctionNames() = 0;

		//Memroy unit 
		virtual std::vector<std::string> getVariableNames() = 0;
		virtual double getVariableValue(std::string  name) = 0;
		virtual void setVariable(std::string name, double value) = 0;
		virtual void createVariable(std::string name, double value) = 0;
		virtual void clearVariables() = 0;

		//Plugins
		virtual void enablePlugins() = 0;

		//Destruction
		virtual ~calc_lib_interface(){};

	};
};
#endif