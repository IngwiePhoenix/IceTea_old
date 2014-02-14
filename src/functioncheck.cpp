/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */


#include "FunctionCheck.h"
#include "helper_macros.h"
#include <bu/plugger.h>
#include <bu/process.h>
#include <stdio.h>
#include <bu/sio.h>

PluginInterface3( pluginFunctionCheck, check, FunctionCheck, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionCheck, "check");

FunctionCheck::FunctionCheck()
{
}

FunctionCheck::~FunctionCheck()
{
}


Variable FunctionCheck::call( Variable &input, VarList lParams ) {
	VarList::iterator p = lParams.begin();
	
	// Gather values
	Bu::String command = (*p).getString();	
	int cmd_rt;
	#ifdef __WIN32__
	Bu::String prog = "where.exe";
	#else
	Bu::String prog = "which";
	#endif
	char* argv[] = { prog.getStr(), command.getStr(), NULL };
	
	// Now, lets run the command and return. which returns 0 on success, 1 on error.
	Bu::Process* proc = new Bu::Process(Bu::Process::Both, "which", argv);
	while(proc->isRunning()) {
		// proc->wait does NOT work. So we have to wait ourself.
		if(proc->childExited() || proc->childSignaled()) break;
	}
	if(proc->childExited()) cmd_rt = proc->childExitStatus();
	else if(proc->childSignaled()) cmd_rt = -1; // It totally signaled, so were returning a negative value.
		
	return new Variable(cmd_rt);
}

