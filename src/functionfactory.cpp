#include "functionfactory.h"

extern struct Bu::PluginInfo directoriesIn;
extern struct Bu::PluginInfo filesIn;
extern struct Bu::PluginInfo regexp;
extern struct Bu::PluginInfo toString;
extern struct Bu::PluginInfo commandToList;
extern struct Bu::PluginInfo targets;

FunctionFactory::FunctionFactory()
{
	registerBuiltinPlugin( &directoriesIn );
	registerBuiltinPlugin( &filesIn );
	registerBuiltinPlugin( &regexp );
	registerBuiltinPlugin( &toString );
	registerBuiltinPlugin( &commandToList );
	registerBuiltinPlugin( &targets );
}

FunctionFactory::~FunctionFactory()
{
}
