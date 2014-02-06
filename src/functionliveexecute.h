/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_EXECUTE_H
#define FUNCTION_EXECUTE_H

#include "function.h"

class FunctionLiveExecute : public Function
{
public:
    FunctionLiveExecute();
    virtual ~FunctionLiveExecute();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );
};

#endif
