/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "FunctionQuietExecute.h"
#include "context.h"
#include "view.h"

#include <bu/sio.h>
#include <bu/process.h>
using namespace Bu;

#include <bu/plugger.h>
PluginInterface3( pluginFunctionQuietExecute, execute_quiet, FunctionQuietExecute, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionQuietExecute, "execute_quiet")

FunctionQuietExecute::FunctionQuietExecute() {}
FunctionQuietExecute::~FunctionQuietExecute() {}

Variable FunctionQuietExecute::call( Variable &/*input*/, VarList lParams ) {
	VarList::iterator i = lParams.begin();
	Bu::String cmd = (*i).getString();
	
	#ifdef __WIN32__
	const char* runner = "cmd.exe";
	const char* swt = "/c";
	#else
	const char* runner = "/bin/bash";
	const char* swt = "-c";
	#endif

	Bu::Process proc( Process::Both, runner, runner, swt, cmd.getStr(), NULL );
	
    Bu::String sStdOut, sStdErr;
    do {
        char buf[4096];
        bool out, err;
        proc.select( out, err );
        if( err ) {
            int iRead = proc.readErr( buf, 4096 );
            sStdErr.append( buf, iRead );
        }
        if( out ) {
            int iRead = proc.read( buf, 4096 );
            sStdOut.append( buf, iRead );
        }
    } while( proc.isRunning() );
        
	Variable rt = new Variable(Variable::typeInt);
	
	if(proc.childExited()) {
		rt = proc.childExitStatus();
	} else if(proc.childSignaled()) {
		rt = proc.childSignal();
	} else rt = -1;
	
	return rt;
}
