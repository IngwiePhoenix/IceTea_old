/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <copyfile.h>

#include "FunctionCopy.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionCopy, copy, FunctionCopy, Function,
        "Ingwie Phoenix", 0, 1 );

FunctionCopy::FunctionCopy()
{
}

FunctionCopy::~FunctionCopy()
{
}

Bu::String FunctionCopy::getName() const {
    return "copy";
}

Variable FunctionCopy::call( Variable &input, VarList lParams ) {
	VarList::iterator p = lParams.begin();
	
	// Gather valeus
	Bu::String fromFile = (*p).getString();
	++p; // Let it step to the next arg
	Bu::String toFile = (*p).getString();
	
	// Aaaah....this is what I call convenient :D
	int rc = copyfile(
		fromFile.getStr(),
		toFile.getStr(),
		NULL,
		COPYFILE_ALL
	);
	
	return new Variable(rc);
}

