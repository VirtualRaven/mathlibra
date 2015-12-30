
#ifndef PLUGIN_SYSTEM_INCLUDED
#define PLUGIN_SYSTEM_INCLUDED
#include "core/type_interface.h"
#include "core/tree_interface.h"
#include <vector>
/**
 * @namespace plugin Contains all plugin related functions
 */
namespace plugin
{ 
	typedef interface::type*(*function_pointer)(tree::node_base*);
	struct function
	{
		
		function_pointer ptr; /**< Pointer to the function*/
		const char * name; /**< Function name. Pointer to a c-style string contaning the name the function which the user will call the function by*/
		const char * doc; /**<Documentation string. Pointer to a c-style string containing a short description of the function*/ 
		const char * disp_name; /**<Name to display. */
		const char * tag; /**<Catergory tag*/
	};
	
	
	/**
	 * Base plugin class. 
	 * All plugins must contain a sub class of function_plugin_base to be able to load at runtime.
	 * Class provides an interface to load the plugin, get version information, plugin name and handle plugin unload events.
	*/
	class function_plugin_base
	{		
	public:
		virtual  unsigned int function_size() = 0;
		virtual function* get_funcs() = 0;/**< @return Pointer to a list of the functions the plug-in provides*/
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
