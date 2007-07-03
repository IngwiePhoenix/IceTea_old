#include "performfactory.h"

extern struct Bu::PluginInfo command;

PerformFactory::PerformFactory()
{
	registerBuiltinPlugin( &command );
}

PerformFactory::~PerformFactory()
{
}
