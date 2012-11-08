/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionwrite.h"
#include "filemgr.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionWrite, write, FunctionWrite, Function,
        "Mike Buland", 0, 1 );

FunctionWrite::FunctionWrite()
{
}

FunctionWrite::~FunctionWrite()
{
}

Bu::String FunctionWrite::getName() const
{
    return "write";
}

Variable FunctionWrite::call( Variable &input, VarList lParams )
{
    if( lParams.getSize() != 1 )
    {
        throw Bu::ExceptionBase(
            "write takes one parameter, the string to write."
            );
    }
    FileMgr::getInstance().get( (ptrdiff_t)input.getOpaque() ).write(
        lParams.first().toString()
        );
    return Variable();
}

