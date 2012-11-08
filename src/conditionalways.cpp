#include "conditionalways.h"
#include "target.h"

#include <bu/plugger.h>
PluginInterface3( pluginConditionAlways, always, ConditionAlways, Condition,
        "Mike Buland", 0, 1 );

ConditionAlways::ConditionAlways()
{
}

ConditionAlways::~ConditionAlways()
{
}

bool ConditionAlways::shouldExec( class Runner &/*r*/, Target &/*rTarget*/ )
{
    return true;
}

Condition *ConditionAlways::clone()
{
    return new ConditionAlways();
}

