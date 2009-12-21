#ifndef CONDITION_FILE_TIME_H
#define CONDITION_FILE_TIME_H

#include "condition.h"

class ConditionFileTime : public Condition
{
public:
	ConditionFileTime();
	virtual ~ConditionFileTime();

	virtual bool shouldExec( class Runner &r, class Target &rTarget );
	virtual Condition *clone();
};

#endif
