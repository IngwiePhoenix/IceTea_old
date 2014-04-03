/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <fstream>
#include <string>
#include <cerrno>

#include "FunctionGenerate.h"
#include "helper_macros.h"
#include <bu/plugger.h>
#include <stdlib.h>
#include <bu/sio.h>

#include "lite.h"

using namespace Bu;
PluginInterface3( pluginFunctionGenerate, generate, FunctionGenerate, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionGenerate, "generate")  

FunctionGenerate::FunctionGenerate() {}
FunctionGenerate::~FunctionGenerate() {}

Variable FunctionGenerate::call( Variable &input, VarList lParams ) {
	VarList::iterator i = lParams.begin();
	
	Bu::String inputTemplate = (*i).getString();
	++i;
	Bu::String outputFile = (*i).getString();
	Variable rt = new Variable(Variable::typeInt);

	// Read the input file into a string first.
	
	
	return rt;
}

