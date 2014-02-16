/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */
#include "view.h"
#include "context.h"
#include "FunctionCheck.h"
#include "helper_macros.h"

#include <bu/plugger.h>
#include <bu/sio.h>
PluginInterface3( pluginFunctionCheck, check, FunctionCheck, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionCheck, "check")  

FunctionCheck::FunctionCheck() {}
FunctionCheck::~FunctionCheck() {}

Variable FunctionCheck::call( Variable &input, VarList lParams ) {
	VarList::const_iterator i = lParams.begin();
	
	// Fetch values
	int outcome = (*i).getInt();
	
	if(outcome == 0) { pContext->getView()->checkSuccess(); }
	else if( outcome > 0 || outcome < 0) { pContext->getView()->checkFail(); }
	
	pContext->getView()->checkEnd(); 
	
	return new Variable((*i));
}

