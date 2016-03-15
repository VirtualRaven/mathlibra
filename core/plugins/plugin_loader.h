/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "modules/functions.h"
#include "../plugins/functions_external_interface.h"
#include <vector>
namespace plugin
{

	void plugin_init(function_plugin_base* func, math_func::function_interface * function_unit);

	class plugin_manager
	{
	protected:
		std::vector < plugin::function_plugin_base*> loaded_plugins;
		virtual void nativeLoadPlugin() = 0;
		void unloadPlugins();
	public:
		
		void loadPlugins(math_func::function_interface*  f_interface);
		
		virtual ~plugin_manager();
	};

	plugin_manager * get_platform_specific_manager();
}
