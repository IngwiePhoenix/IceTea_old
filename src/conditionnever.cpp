#include "conditionnever.h"
#include "target.h"

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

