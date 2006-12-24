#include "viewerfactory.h"

extern struct PluginInfo plain;
extern struct PluginInfo percent;
extern struct PluginInfo make;
extern struct PluginInfo plainpct;
extern struct PluginInfo colorpct;

ViewerFactory::ViewerFactory()
{
	registerBuiltinPlugin( &plain );
	registerBuiltinPlugin( &percent );
	registerBuiltinPlugin( &make );
	registerBuiltinPlugin( &plainpct );
	registerBuiltinPlugin( &colorpct );
}

ViewerFactory::~ViewerFactory()
{
}
