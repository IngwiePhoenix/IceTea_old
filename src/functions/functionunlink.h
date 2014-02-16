/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_UNLINK_H
#define FUNCTION_UNLINK_H

#include "function.h"

class FunctionUnlink : public Function
{
public:
    FunctionUnlink();
    virtual ~FunctionUnlink();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );
};

#endif
