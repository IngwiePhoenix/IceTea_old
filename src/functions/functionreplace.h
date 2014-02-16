/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_REPLACE_H
#define FUNCTION_REPLACE_H

#include "function.h"

class FunctionReplace : public Function
{
public:
    FunctionReplace();
    virtual ~FunctionReplace();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );
};

#endif
