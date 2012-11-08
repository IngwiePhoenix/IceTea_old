/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_FILES_H
#define FUNCTION_FILES_H

#include "function.h"

class FunctionFiles : public Function
{
public:
    FunctionFiles();
    virtual ~FunctionFiles();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

};

#endif
