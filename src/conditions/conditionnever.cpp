/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "conditionnever.h"
#include "target.h"

#include <bu/plugger.h>
PluginInterface3( pluginConditionNever, never, ConditionNever, Condition,
        "Mike Buland", 0, 1 );

ConditionNever::ConditionNever()
{
}

ConditionNever::~ConditionNever()
{
}

bool ConditionNever::shouldExec( class Runner &/*r*/, Target &/*rTarget*/ )
{
    return false;
}

Condition *ConditionNever::clone()
{
    return new ConditionNever();
}

