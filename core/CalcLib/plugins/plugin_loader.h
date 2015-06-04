#include "modules/functions.h"
#include "../plugins/functions_external_interface.h"
#include <vector>
namespace plugin
{
	struct pluginManagerOops : public exception
	{
	public:
		pluginManagerOops(std::string inf);
		const char* what();
	};
	
	void plugin_init(function_plugin_base* func, math_func::function_interface * function_unit);

	class plugin_manager
	{
	protected:
		std::vector < plugin::function_plugin_base*> loaded_plugins;
		virtual void nativeLoadPlugin() = 0;
	public:
		
		void loadPlugins(math_func::function_interface*  f_interface);
		virtual ~plugin_manager();
	};

	plugin_manager * get_platform_specific_manager();
}