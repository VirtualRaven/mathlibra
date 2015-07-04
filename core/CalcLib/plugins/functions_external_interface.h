
#ifndef PLUGIN_SYSTEM_INCLUDED
#define PLUGIN_SYSTEM_INCLUDED
#include "modules/functions.h"
#include "../exception.h"
#include <vector>
/**
 * @namespace plugin Contains all plugin related functions
 */
namespace plugin
{ 

	typedef math_func::m_function::generalFuncPtr function_pointer;
	class function_plugin_base;
	
	void plugin_init(function_plugin_base* func, math_func::function_interface * function_unit); /**<Called on all plugins to initilze and load them.*/

	struct function
	{
		
		function_pointer ptr; /**< Pointer to the function*/
		const char * name; /**< Function name. Pointer to a c-style string contaning the name the function which the user will call the function by*/
		const char * doc /**<Documentation string. Pointer to a c-style string containing a short description of the function*/ ;
	};
	
	
	/**
	 * Base plugin class. 
	 * All plugins must contain a sub class of function_plugin_base to be able to load at runtime.
	 * Class provides an interface to load the plugin, get version information, plugin name and handle plugin unload events.
	*/
	class function_plugin_base
	{		
		friend void plugin_init(function_plugin_base* func, math_func::function_interface * function_unit);
	protected:
		unsigned int function_size;
		function* functions;/**< Pointer to a list of the functions the plug-in provides*/
	public:
		
		
		virtual void plugin_init_event() = 0; /**< Called when before the plug-in is loaded. Can be used to initilize memory storage or similar*/
		virtual void plugin_destruction_event() = 0; /**< Called when the plug-in is about to be unloaded. Must finish with "destroy this" */
		//Plugin functions
		virtual const char * version_minor() = 0;  /**<@return Minor version number.*/
		virtual const char * version_major() = 0;		/**<@return Major version number*/
		virtual const char * plugin_name() = 0;		/**<@return The name of the plugin*/
	
	};

	
	typedef function_plugin_base*(*plugin_constructor)();
}

#endif