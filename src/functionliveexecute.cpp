/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */
 
#include <iostream>

#include "FunctionLiveExecute.h"
#include "context.h"
#include "view.h"

#include <bu/sio.h>
#include <bu/process.h>
using namespace Bu;

#include <bu/plugger.h>
PluginInterface3( pluginFunctionLiveExecute, live_execute, FunctionLiveExecute, Function,
        "Ingwie Phoenix", 0, 1 );

FunctionLiveExecute::FunctionLiveExecute()
{
}

FunctionLiveExecute::~FunctionLiveExecute()
{
}

Bu::String FunctionLiveExecute::getName() const
{
    return "live_execute";
}

Variable FunctionLiveExecute::call( Variable &/*input*/, VarList lParams )
{
    pContext->getView()->cmdStarted( lParams.first().getString() );
    Process pCmd(
    	Process::None, 
    	"/bin/bash", "/bin/bash", "-c",
        lParams.first().getString().getStr(), NULL
    );
    pCmd.wait();
    return Variable( pCmd.childExitStatus() );
}

