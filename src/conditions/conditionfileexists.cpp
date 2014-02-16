/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "conditionfileexists.h"
#include "target.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <bu/sio.h>
using namespace Bu;

#include <bu/plugger.h>
PluginInterface3( pluginConditionFileExists, fileExists, ConditionFileExists,
        Condition, "Mike Buland", 0, 1 );

ConditionFileExists::ConditionFileExists()
{
}

ConditionFileExists::~ConditionFileExists()
{
}

bool ConditionFileExists::shouldExec( class Runner &r, Target &rTarget )
{
    for( StrList::const_iterator j = rTarget.getOutputList().begin(); j; j++ )
    {
        // If any input exists, then return true, we should exec.
        if( !access( (*j).getStr(), F_OK ) )
        {
            return true;
        }
    }

    return false; 
}

Condition *ConditionFileExists::clone()
{
    return new ConditionFileExists();
}

