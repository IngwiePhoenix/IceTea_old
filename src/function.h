/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_H
#define FUNCTION_H

#include "bu/string.h"
#include "variable.h"

class Function
{
public:
    Function();
    virtual ~Function();

    virtual Bu::String getName() const=0;

    virtual Variable call( Variable &input, VarList lParams )=0;

    void setContext( class Context *p );

protected:
    class Context *pContext;
};

#endif
