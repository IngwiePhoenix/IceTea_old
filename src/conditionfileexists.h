#ifndef CONDITION_FILE_EXISTS_H
#define CONDITION_FILE_EXISTS_H

#include "condition.h"

class ConditionFileExists : public Condition
{
public:
    ConditionFileExists();
    virtual ~ConditionFileExists();

    virtual bool shouldExec( class Runner &r, class Target &rTarget );
    virtual Condition *clone();
};

#endif
