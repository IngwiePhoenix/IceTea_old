#ifndef FUNCTION_PLUGGER_H
#define FUNCTION_PLUGGER_H

#include "function.h"
#include <bu/plugger.h>
#include <bu/singleton.h>

class FunctionPlugger : public Bu::Plugger<Function>,
    public Bu::Singleton<FunctionPlugger>
{
friend class Bu::Singleton<FunctionPlugger>;
private:
    FunctionPlugger();
    virtual ~FunctionPlugger();
};

#endif
