#ifndef FUNCTION_EXECUTE_H
#define FUNCTION_EXECUTE_H

#include "function.h"

class FunctionExecute : public Function
{
public:
    FunctionExecute();
    virtual ~FunctionExecute();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );
};

#endif
