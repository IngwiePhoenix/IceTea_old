/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_MATCHES_H
#define FUNCTION_MATCHES_H

#include "function.h"

class FunctionMatches : public Function
{
public:
    FunctionMatches();
    virtual ~FunctionMatches();

    /**
     * Really basic globbing function, it doesn't handle character classes,
     * just * and ?.  We can expand on it later, it may be handy.
     */
    bool globcmp( const Bu::String &sTxt, const Bu::String &sMatches );
    bool matchlist( const Bu::String &sTxt, VarList &lParams );
    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

};

#endif
