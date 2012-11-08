/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_REG_EX_H
#define FUNCTION_REG_EX_H

#include "function.h"

namespace Bu
{
    class RegEx;
}

class FunctionRegEx : public Function
{
public:
    FunctionRegEx();
    virtual ~FunctionRegEx();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

    Bu::String replace( Bu::RegEx &re, const Bu::String &sSrc,
        const Bu::String &sPat );
};

#endif
