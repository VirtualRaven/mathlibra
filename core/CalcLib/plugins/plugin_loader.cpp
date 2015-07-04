#include"plugins/plugin_loader.h"

#ifdef WINDOWS
#include <Windows.h>
#endif //WINDOWS

#ifdef LINUX
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>
#endif


plugin::pluginManagerOops::pluginManagerOops(std::string inf)
{
	this->_isCritical = true;
	this->info = inf;
}

const char* plugin::pluginManagerOops::what()
{
	return "plugin manager exception";
}

void plugin::plugin_manager::loadPlugins(math_func::function_interface*  f_interface)
{
	this->nativeLoadPlugin();
	for(plugin::function_plugin_base * current_plugin : this->loaded_plugins)
	{
		plugin::plugin_init(current_plugin, f_interface);
	}
	
}

void plugin::plugin_manager::unloadPlugins()
{
	for (plugin::function_plugin_base * plugin : this->loaded_plugins)
	{
		plugin->plugin_destruction_event();
		plugin = nullptr;
	}
}

 plugin::plugin_manager::~plugin_manager()
{

}

 void plugin::plugin_init(function_plugin_base* func,math_func::function_interface * function_unit)
 {
	
	 std::vector<math_func::m_function> __funcs;
	 __funcs.reserve(func->function_size);
	 for (unsigned int i = 0; i < func->function_size; i++)
	 {
		 __funcs.push_back(math_func::m_function(func->functions[i].name, func->functions[i].ptr));
	 }
	 function_unit->load(__funcs);
 }

#ifdef WINDOWS
 //Following code is windows specific
 //Plugin manager for windows
 class windows_plugin_manager : public plugin::plugin_manager
 {
	 //Windows specific functions
	 typedef HINSTANCE DLL;
	 typedef std::string plugin_name_container;
	 std::vector<DLL> dlls;
	 std::vector<plugin_name_container> plugin_names;
	 void get_plugin_names();
	 void load_dll(plugin_name_container container);
	 void unload_all_dlls();
 protected:
	 void nativeLoadPlugin();
 public:
	 virtual ~windows_plugin_manager();

 };

 void  windows_plugin_manager::get_plugin_names()
 {
	 WIN32_FIND_DATA file_data;
	 HANDLE file = FindFirstFile(TEXT(".//plugins//*"), &file_data);
	 if (file != INVALID_HANDLE_VALUE)
	 {
		 do
		 {
			 if (strcmp(file_data.cFileName, ".") != 0 && strcmp(file_data.cFileName, ".."))
			 {
				 if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				 {
					 this->plugin_names.push_back(file_data.cFileName);
				 }
			 }
		 } while (FindNextFile(file, &file_data));
		 FindClose(file);
	 }
	 else
	 {
		 throw plugin::pluginManagerOops("Failed to search folders");
	 }
 }
 
void windows_plugin_manager::load_dll(windows_plugin_manager::plugin_name_container container)
 {
	
	 DLL newDll = LoadLibrary( std::string(std::string(".//plugins//") + container).c_str());
	 if (newDll == NULL)
	 {
		
		 throw plugin::pluginManagerOops("Failed to load plugin");
	 }
	 else
	 {
		 dlls.push_back(newDll);
	 }
	 plugin::plugin_constructor my_constr = (plugin::plugin_constructor)GetProcAddress(newDll, "PLUGIN_ENTRY");
	 if (!my_constr)
	 {
		 throw plugin::pluginManagerOops("Failed to find plugin entry point");
	 }
	 else
	 {
		 auto tmp = my_constr();
		 this->loaded_plugins.push_back(tmp);
	}
	
 }
 
 void  windows_plugin_manager::unload_all_dlls()
 {
	 this->unloadPlugins();
	 for each (DLL current_dll in this->dlls)
	 {
		 FreeLibrary(current_dll);
	 }
 }
 
 void  windows_plugin_manager::nativeLoadPlugin()
 {
	 get_plugin_names();
	 for each (windows_plugin_manager::plugin_name_container  current_name in this->plugin_names )
	 {
		 this->load_dll(current_name);
	 }
 }

 windows_plugin_manager::~windows_plugin_manager()
 {
	 this->unload_all_dlls();
 }

#endif //WINDOWS

#ifdef LINUX
class linux_plugin_manager : public plugin::plugin_manager
 {
	 //Linux specific functions
	 typedef void* Lib;
	 typedef std::string plugin_name_container;
	 std::vector<Lib> libs;
	 std::vector<plugin_name_container> plugin_names;
	 void get_plugin_names();
	 void load_lib(plugin_name_container container);
	 void unload_all_lib();
 protected:
	 void nativeLoadPlugin();
 public:
	 virtual ~linux_plugin_manager();

 };
	
	void linux_plugin_manager::get_plugin_names()
	{
		DIR *dp;
    	struct dirent *dirp;
    	if((dp  = opendir("./plugins/")) == NULL)
		{
        	
		 throw plugin::pluginManagerOops("Failed to search folders");
    	}

    	while ((dirp = readdir(dp)) != NULL) 
		{
			std::string name = dirp->d_name;
			if(name != ".." && name != ".")
			{
				plugin_names.push_back(name);	
			}
        	
    	}
    	closedir(dp);
   
		
	}
	
	void linux_plugin_manager::load_lib(plugin_name_container container)
	{
		 Lib newLib = dlopen( std::string(std::string(".//plugins//") + container).c_str(),RTLD_NOW);
	 if (newLib == NULL)
	 {
		
		 throw plugin::pluginManagerOops(std::string(dlerror()));
	 }
	 else
	 {
		 libs.push_back(newLib);
	 }
	 plugin::plugin_constructor my_constr = (plugin::plugin_constructor)dlsym(newLib, "PLUGIN_ENTRY");
	 if (!my_constr)
	 {
		 throw plugin::pluginManagerOops("Failed to find plugin entry point");
	 }
	 else
	 {
		 auto tmp = my_constr();
		 this->loaded_plugins.push_back(tmp);
	}	
	}
	
	void  linux_plugin_manager::unload_all_lib()
	{
		this->unloadPlugins();
		for(Lib current_lib : libs)
		{
			dlclose(current_lib);
		}
	}
 void  linux_plugin_manager::nativeLoadPlugin()
 {
	 get_plugin_names();
	 for(linux_plugin_manager::plugin_name_container  current_name : this->plugin_names )
	 {
		 this->load_lib(current_name);
	 }
 }

 linux_plugin_manager::~linux_plugin_manager()
 {
	 this->unload_all_lib();
 }
#endif


plugin::plugin_manager*  plugin::get_platform_specific_manager()
 {

#if defined(WINDOWS)	 
	 return new windows_plugin_manager;
#elif defined(LINUX)
	return new linux_plugin_manager;
#else
#error "No platform specific plugin manager available\n Plugins dissabled"
return nullptr;
#endif	 
 }
 
 
 