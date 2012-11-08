/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_UNIQUE_H
#define FUNCTION_UNIQUE_H

#include "function.h"

class FunctionUnique : public Function
{
public:
    FunctionUnique();
    virtual ~FunctionUnique();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );
};

#endif
