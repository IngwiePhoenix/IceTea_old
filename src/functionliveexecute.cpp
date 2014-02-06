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
    	Process::Both, 
    	"/bin/bash", "/bin/bash", "-c",
        lParams.first().getString().getStr(), NULL
    );
    
    bool DoNL=false;

    do {
        char buf[128];
        char errBuf[128];
        bool out, err;
        String sStdOut, sStdErr;
        pCmd.select( out, err );
        if(!DoNL && (out || err)) { DoNL=true; sio << sio.nl; }
        if( err ) {
        	int iRead = pCmd.readErr( errBuf, 128 );
			sStdErr.append( errBuf, iRead );
		}
        if( out ) {
        	int iRead2 = pCmd.read( buf, 128 );
        	sStdOut.append( buf, iRead2 );
        }
        sio << sStdOut << sio.flush;
        serr << sStdErr << serr.flush;
    } while( pCmd.isRunning() );
    
    if( pCmd.childExited() )
    {
        if( pCmd.childExitStatus() != 0 )
        {
            throw Bu::ExceptionBase("Command exited with errorcode %d.", pCmd.childExitStatus() );
        }
    }
    else
    {
        /*pContext->getView()->cmdFinished(
            sStdOut, sStdErr, pCmd.childExitStatus()
            );
        */
        throw Bu::ExceptionBase("Command Failed");
    }
    return Variable( pCmd.childExitStatus() );
}

