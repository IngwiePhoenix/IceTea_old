/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef CONDITION_FILE_TIME_H
#define CONDITION_FILE_TIME_H

#include "condition.h"

class ConditionFileTime : public Condition
{
public:
    ConditionFileTime();
    virtual ~ConditionFileTime();

    virtual bool shouldExec( class Runner &r, class Target &rTarget );
    virtual Condition *clone();
};

#endif
