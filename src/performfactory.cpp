#include "performfactory.h"

extern struct PluginInfo command;

PerformFactory::PerformFactory()
{
	registerBuiltinPlugin( &command );
}

PerformFactory::~PerformFactory()
{
}
