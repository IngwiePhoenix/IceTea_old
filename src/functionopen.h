/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
