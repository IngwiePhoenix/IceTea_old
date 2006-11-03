#include "viewerfactory.h"

extern struct PluginInfo plain;
extern struct PluginInfo percent;
extern struct PluginInfo make;
extern struct PluginInfo plainpct;

ViewerFactory::ViewerFactory()
{
	registerBuiltinPlugin( &plain );
	registerBuiltinPlugin( &percent );
	registerBuiltinPlugin( &make );
	registerBuiltinPlugin( &plainpct );
}

ViewerFactory::~ViewerFactory()
{
}
