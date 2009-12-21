#include "viewplugger.h"

extern Bu::PluginInfo pluginViewDefault;
extern Bu::PluginInfo pluginViewMake;
ViewPlugger::ViewPlugger()
{
	registerBuiltinPlugin( &pluginViewDefault );
	registerBuiltinPlugin( &pluginViewMake );
}

ViewPlugger::~ViewPlugger()
{
}

