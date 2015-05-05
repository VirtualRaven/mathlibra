#ifndef PLUGIN_SYSTEM_INCLUDED
#define PLUGIN_SYSTEM_INCLUDED
#include "modules/functions.h"
#include "../exception.h"
#include <vector>

namespace plugin
{ 

	typedef math_func::m_function::generalFuncPtr function_pointer;
	class function_plugin_base;
	void plugin_init(function_plugin_base* func, math_func::function_interface * function_unit);

	struct function
	{
		//Pointer to function
		function_pointer ptr;
		//Pointer to string literial describing function usage including argument definitions
		const char * name;
		const char * doc;
	};
	
	

	class function_plugin_base
	{		

		//function list to be populated with plugin functions
		
		friend void plugin_init(function_plugin_base* func, math_func::function_interface * function_unit);
	protected:
		std::vector<function> functions;
	public:
		
		//Called before plugin initialization
		virtual void plugin_init_event() = 0;

		//Plugin functions
		virtual const char * version_minor()=0; 
		virtual const char * version_major()=0;
		virtual const char * plugin_name() = 0;
	};

	struct pluginOops : public exception
	{
		pluginOops(function_plugin_base* plugin, std::string inf);
		const char* what();
	};

	typedef function_plugin_base*(*plugin_constructor)();
}

#endif