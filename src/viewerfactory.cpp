#include "viewerfactory.h"

extern struct PluginInfo plain;

ViewerFactory::ViewerFactory()
{
	registerBuiltinPlugin( &plain );
}

ViewerFactory::~ViewerFactory()
{
}
