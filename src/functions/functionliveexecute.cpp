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
#include "helper_macros.h"
#include <bu/sio.h>
#include <bu/process.h>
using namespace Bu;

#include <bu/plugger.h>
PluginInterface3( pluginFunctionLiveExecute, live_execute, FunctionLiveExecute, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionLiveExecute, "live_execute")

FunctionLiveExecute::FunctionLiveExecute(){}
FunctionLiveExecute::~FunctionLiveExecute(){}


Variable FunctionLiveExecute::call( Variable &/*input*/, VarList lParams )
{
    pContext->getView()->cmdStarted( lParams.first().getString() );
    #ifdef __WIN32__
    const char* name = "cmd.exe";
    const char* st = "/c";
    #else
    const char* name = "/bin/bash";
    const char* st = "-c";
    #endif
    const char* command = lParams.first().getString().getStr();
    char* argv[] = { (char*)name, (char*)st, (char*)command, NULL };
    Process pCmd(Process::None, name, argv);
    pCmd.wait();
    return Variable( pCmd.childExitStatus() );
}

