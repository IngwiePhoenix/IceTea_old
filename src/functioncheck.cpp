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
Bu::String FunctionCheck::getName() const { return "check"; }

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
	Bu::String prog = "which";
	char* argv[] = { prog.getStr(), command.getStr(), NULL };
	
	// on windows, we'd use "where" instead of "which". Include that in porting... o.o
	
	// Now, lets run the command and return. which returns 0 on success, 1 on error.
	Bu::Process* proc = new Bu::Process(Bu::Process::Both, "which", argv);
	while(proc->isRunning()) {
		// proc->wait does NOT work. So we have to wait ourself.
		if(proc->childExited() || proc->childSignaled()) break;
	}
	if(proc->childExited()) cmd_rt = proc->childExitStatus();
	else if(proc->childSignaled()) cmd_rt = -1; // It totally signaled, so were returning a negative value.
	else cmd_rt = proc->childExitStatus(); // well, lets try, at least?
		
	return new Variable(cmd_rt);
}

