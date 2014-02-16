/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionclose.h"
#include "filemgr.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionClose, close, FunctionClose, Function,
        "Mike Buland", 0, 1 );

FunctionClose::FunctionClose()
{
}

FunctionClose::~FunctionClose()
{
}

Bu::String FunctionClose::getName() const
{
    return "close";
}

Variable FunctionClose::call( Variable &input, VarList )
{
    FileMgr::getInstance().close( (ptrdiff_t)input.getOpaque() );
    return Variable();
}

