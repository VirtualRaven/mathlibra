#include "functions_external_interface.h"


plugin::pluginOops::pluginOops(function_plugin_base* plugin, std::string inf)
{
	this->_isCritical = false;
	this->info = (std::string(plugin->plugin_name()) + std::string(plugin->version_major()) + "." + std::string(plugin->version_minor()) + inf);

}

const char* plugin::pluginOops::what()
{
	return "plugin exception";
}

