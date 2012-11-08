/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
