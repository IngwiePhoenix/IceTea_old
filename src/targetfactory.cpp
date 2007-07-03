#include "targetfactory.h"

extern struct Bu::PluginInfo file;

TargetFactory::TargetFactory()
{
	registerBuiltinPlugin( &file );
}

TargetFactory::~TargetFactory()
{
}
