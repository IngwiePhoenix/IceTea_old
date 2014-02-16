/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */
#include "view.h"
#include "context.h"
#include "Functioncheckinit.h"
#include "helper_macros.h"

#include <bu/plugger.h>
#include <bu/sio.h>
PluginInterface3( pluginFunctionCheckInit, check_init, FunctionCheckInit, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionCheckInit, "check_init")  

FunctionCheckInit::FunctionCheckInit() {}
FunctionCheckInit::~FunctionCheckInit() {}

Variable FunctionCheckInit::call( Variable &input, VarList lParams ) {
	VarList::const_iterator i = lParams.begin();
	
	// Fetch values
	Bu::String what = (*i).getString();
	
	// Lets check stuff!
	pContext->getView()->checkBegin(what);
	
	return new Variable(Variable::typeNone);
}

