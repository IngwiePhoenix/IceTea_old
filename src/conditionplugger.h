/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef CONDITION_PLUGGER_H
#define CONDITION_PLUGGER_H

#include "condition.h"
#include <bu/plugger.h>
#include <bu/singleton.h>

class ConditionPlugger : public Bu::Plugger<Condition>,
    public Bu::Singleton<ConditionPlugger>
{
friend class Bu::Singleton<ConditionPlugger>;
private:
    ConditionPlugger();
    virtual ~ConditionPlugger();
};

#endif
