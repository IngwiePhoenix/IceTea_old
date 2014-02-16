/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include "function.h"

class FunctionAst : public Function
{
public:
    FunctionAst( const class AstBranch *pRoot, class Runner *pRunner );
    virtual ~FunctionAst();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

private:
    Bu::String sName;
    const class AstBranch *pRoot;
    class Runner *pRunner;
};

#endif
