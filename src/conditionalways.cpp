/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "conditionalways.h"
#include "target.h"

#include <bu/plugger.h>
PluginInterface3( pluginConditionAlways, always, ConditionAlways, Condition,
        "Mike Buland", 0, 1 );

ConditionAlways::ConditionAlways()
{
}

ConditionAlways::~ConditionAlways()
{
}

bool ConditionAlways::shouldExec( class Runner &/*r*/, Target &/*rTarget*/ )
{
    return true;
}

Condition *ConditionAlways::clone()
{
    return new ConditionAlways();
}

