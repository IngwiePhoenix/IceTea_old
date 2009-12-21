#ifndef CONDITION_NEVER_H
#define CONDITION_NEVER_H

#include "condition.h"

class ConditionNever : public Condition
{
public:
	ConditionNever();
	virtual ~ConditionNever();

	virtual bool shouldExec( class Runner &r, class Target &rTarget );
	virtual Condition *clone();
};

#endif
