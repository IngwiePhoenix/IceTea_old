#ifndef FUNCTION_OPEN_H
#define FUNCTION_OPEN_H

#include "function.h"

class FunctionOpen : public Function
{
public:
    FunctionOpen();
    virtual ~FunctionOpen();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

};

#endif
