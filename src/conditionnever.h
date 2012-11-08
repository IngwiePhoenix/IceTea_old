/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef CONDITION_NEVER_H
#define CONDITION_NEVER_H

#include "condition.h"

class ConditionNever : public Condition
{
public:
    ConditionNever();
    virtual ~ConditionNever();

    virtual bool shouldExec( class Runner &r, class Target &rTarget );
    virtual Condition *clone();
};

#endif
