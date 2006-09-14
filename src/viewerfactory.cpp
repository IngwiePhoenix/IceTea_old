#include "viewerfactory.h"

extern struct PluginInfo plain;
extern struct PluginInfo percent;
extern struct PluginInfo make;

ViewerFactory::ViewerFactory()
{
	registerBuiltinPlugin( &plain );
	registerBuiltinPlugin( &percent );
	registerBuiltinPlugin( &make );
}

ViewerFactory::~ViewerFactory()
{
}
