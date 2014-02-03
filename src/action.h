/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef ACTION_H
#define ACTION_H

#include <bu/string.h>

class Action
{
public:
    Action( const class AstBranch *pRoot );
    virtual ~Action();

    const Bu::String &getName() const;

    void call( class Runner *pRunner );

    static Action *genDefaultAll();
    static Action *genDefaultClean();
    static Action *genDefaultCleanAll();
    static Action *genDefaultDefault();

private:
    Bu::String sName;
    const class AstBranch *pRoot;
    class Ast *pAst;
};

#endif
