#ifndef FUNCTION_EXISTS_H
#define FUNCTION_EXISTS_H

#include "function.h"

class FunctionExists : public Function
{
public:
    FunctionExists();
    virtual ~FunctionExists();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

};

#endif
