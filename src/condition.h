/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef CONDITION_H
#define CONDITION_H

class Condition
{
public:
    Condition();
    virtual ~Condition();

    virtual bool shouldExec( class Runner &r, class Target &rTarget )=0;
    virtual Condition *clone()=0;

private:
};

#endif
