#include "condition.h"
#include <stdlib.h>

class ConditionRandom : public Condition
{
public:
    ConditionRandom()
    {
    }

    virtual ~ConditionRandom()
    {
    }

    virtual bool shouldExec( class Runner &, class Target & )
    {
        if( (random()/(double)RAND_MAX) >= .5 )
            return true;
        return false;
    }

    virtual Condition *clone()
    {
        return new ConditionRandom();
    }
};

#include <bu/plugger.h>
PluginInterface3( pluginConditionRandom, random, ConditionRandom, Condition,
        "Mike Buland", 0, 1 );

