#include "targetfactory.h"

extern struct PluginInfo file;

TargetFactory::TargetFactory()
{
	registerBuiltinPlugin( &file );
}

TargetFactory::~TargetFactory()
{
}
