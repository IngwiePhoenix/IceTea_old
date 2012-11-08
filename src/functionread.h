#ifndef FUNCTION_READ_H
#define FUNCTION_READ_H

#include "function.h"

class FunctionRead : public Function
{
public:
    FunctionRead();
    virtual ~FunctionRead();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

};

#endif
