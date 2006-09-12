#include "functionfactory.h"

extern struct PluginInfo directoriesIn;
extern struct PluginInfo filesIn;
extern struct PluginInfo regexp;
extern struct PluginInfo toString;
extern struct PluginInfo commandToList;
extern struct PluginInfo targets;

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
