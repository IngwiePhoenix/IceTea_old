#ifndef CONDITION_ALWAYS_H
#define CONDITION_ALWAYS_H

#include "condition.h"

class ConditionAlways : public Condition
{
public:
	ConditionAlways();
	virtual ~ConditionAlways();

	virtual bool shouldExec( class Runner &r, class Target &rTarget );
	virtual Condition *clone();
};

#endif
