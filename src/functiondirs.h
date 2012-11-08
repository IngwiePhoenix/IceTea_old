#ifndef FUNCTION_DIRS_H
#define FUNCTION_DIRS_H

#include "function.h"

class FunctionDirs : public Function
{
public:
    FunctionDirs();
    virtual ~FunctionDirs();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

};

#endif
