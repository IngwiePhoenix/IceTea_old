#include "conditionalways.h"
#include "target.h"

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

