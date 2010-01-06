#include "conditionnever.h"
#include "target.h"

#include <bu/plugger.h>
PluginInterface3( pluginConditionNever, never, ConditionNever, Condition,
		"Mike Buland", 0, 1 );

ConditionNever::ConditionNever()
{
}

ConditionNever::~ConditionNever()
{
}

bool ConditionNever::shouldExec( class Runner &/*r*/, Target &/*rTarget*/ )
{
	return false;
}

Condition *ConditionNever::clone()
{
	return new ConditionNever();
}

