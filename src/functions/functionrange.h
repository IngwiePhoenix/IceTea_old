/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_RANGE_H
#define FUNCTION_RANGE_H

#include "function.h"

class FunctionRange : public Function
{
public:
    FunctionRange();
    virtual ~FunctionRange();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

};

#endif
