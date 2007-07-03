#include "viewerfactory.h"

extern struct Bu::PluginInfo plain;
extern struct Bu::PluginInfo percent;
extern struct Bu::PluginInfo make;
extern struct Bu::PluginInfo plainpct;
extern struct Bu::PluginInfo colorpct;

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
