#ifndef CONDITION_PLUGGER_H
#define CONDITION_PLUGGER_H

#include "condition.h"
#include <bu/plugger.h>
#include <bu/singleton.h>

class ConditionPlugger : public Bu::Plugger<Condition>,
    public Bu::Singleton<ConditionPlugger>
{
friend class Bu::Singleton<ConditionPlugger>;
private:
    ConditionPlugger();
    virtual ~ConditionPlugger();
};

#endif
