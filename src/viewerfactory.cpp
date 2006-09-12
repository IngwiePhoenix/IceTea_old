#include "viewerfactory.h"

extern struct PluginInfo plain;
extern struct PluginInfo percent;

ViewerFactory::ViewerFactory()
{
	registerBuiltinPlugin( &plain );
	registerBuiltinPlugin( &percent );
}

ViewerFactory::~ViewerFactory()
{
}
