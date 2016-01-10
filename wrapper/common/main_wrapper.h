#ifndef LIB_INTERFACE_INCLUED
#define LIB_INTERFACE_INCLUED
#include <vector>
#include <string>
#include "core/type_interface.h"
namespace interface
{

	struct wrapper_exception_info
	{
		const char * type;
		const char * desc;
                unsigned int id;
		bool isCritical;
	};
        struct mem_obj_api
        {
            std::string name;
            type* value;
            bool    isConst;
        };

	struct func_obj_api
	{
	  	std::string name;
		std::string tag;
		std::string doc;
		std::string disp_name;
	};

	class calc_lib_interface
	{
	public:
		virtual void set_arg(std::string str) = 0;
		virtual void interpret_arg() = 0;
		virtual interface::type_ptr execute_arg() = 0;
		virtual wrapper_exception_info get_exception_info() = 0;
		virtual bool exceptionOccured() = 0;

		//Function unit
		virtual std::vector<std::string> getFunctionNames() = 0;
		virtual std::vector<func_obj_api> getFunctionObjs() = 0;	
		

		//Memroy unit 
		virtual std::vector<std::string> getVariableNames() = 0;
		virtual type* getVariableValue(std::string  name) = 0;
                virtual mem_obj_api getVariable(std::string name)=0;
                virtual mem_obj_api getVariable(size_t index)=0;
		virtual void setVariable(std::string name, interface::type_ptr&& value) = 0;
		virtual void createVariable(std::string name, interface::type_ptr&& value) = 0;
		virtual void clearVariables() = 0;
                
                virtual void manageVariable(std::string name,interface::type_ptr&& value,bool isConst)=0;
                virtual bool  isConstVariable(std::string name)=0;
                virtual unsigned int getNumVariables()=0;
                virtual bool isDefined(std::string name)=0;
		//Plugins
		virtual void enablePlugins() = 0;

		//Destruction
		virtual ~calc_lib_interface(){};

	};
}
#endif
