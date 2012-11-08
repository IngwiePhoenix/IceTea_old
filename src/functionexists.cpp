/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionexists.h"

#include <unistd.h>

#include <bu/plugger.h>
PluginInterface3( pluginFunctionExists, exists, FunctionExists, Function,
        "Mike Buland", 0, 1 );

FunctionExists::FunctionExists()
{
}

FunctionExists::~FunctionExists()
{
}

Bu::String FunctionExists::getName() const
{
    return "exists";
}

Variable FunctionExists::call( Variable &input, VarList lParams )
{
    Bu::String sFile;
    if( input.getType() != Variable::typeNone )
    {
        sFile = input.toString();
    }
    else
    {
        sFile = lParams.first().toString();
    }
    if( access( sFile.getStr(), F_OK ) == 0 )
        return Variable( true );
    else
        return Variable( false );
}

