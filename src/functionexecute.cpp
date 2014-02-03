/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionexecute.h"
#include "context.h"
#include "view.h"

#include <bu/sio.h>
#include <bu/process.h>
using namespace Bu;

#include <bu/plugger.h>
PluginInterface3( pluginFunctionExecute, execute, FunctionExecute, Function,
        "Mike Buland", 0, 1 );

FunctionExecute::FunctionExecute()
{
}

FunctionExecute::~FunctionExecute()
{
}

Bu::String FunctionExecute::getName() const
{
    return "execute";
}

Variable FunctionExecute::call( Variable &/*input*/, VarList lParams )
{
    // TODO This is lame, really lame, we need to exec on our own and process
    // output appropriately.
    pContext->getView()->cmdStarted( lParams.first().getString() );
    Process pCmd( Process::Both, "/bin/bash", "/bin/bash", "-c",
        lParams.first().getString().getStr(), NULL );
    String sStdOut, sStdErr;
    do
    {
        char buf[4096];
        bool out, err;
        pCmd.select( out, err );
        if( err )
        {
            int iRead = pCmd.readErr( buf, 4096 );
            sStdErr.append( buf, iRead );
            //sio << "Read " << iRead << " bytes of stderr." << sio.nl;
        }
        if( out )
        {
            int iRead = pCmd.read( buf, 4096 );
            sStdOut.append( buf, iRead );
            //sio << "Read " << iRead << " bytes of stdout." << sio.nl;
        }
    }
    while( pCmd.isRunning() );

    pContext->getView()->cmdFinished(
        sStdOut, sStdErr, pCmd.childExitStatus()
        );
    if( pCmd.childExited() )
    {
        if( pCmd.childExitStatus() != 0 )
        {
            throw Bu::ExceptionBase("Command exited with errorcode %d.", pCmd.childExitStatus() );
        }
    }
    else
    {
        pContext->getView()->cmdFinished(
            sStdOut, sStdErr, pCmd.childExitStatus()
            );
        throw Bu::ExceptionBase("Command Failed");
    }
    return Variable( pCmd.childExitStatus() );
}

