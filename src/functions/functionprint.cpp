/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "FunctionPrint.h"
#include "helper_macros.h"

#include <bu/plugger.h>
#include <bu/sio.h>
PluginInterface3( pluginFunctionPrint, print, FunctionPrint, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionPrint, "print")  

FunctionPrint::FunctionPrint() {}
FunctionPrint::~FunctionPrint() {}

Variable FunctionPrint::call( Variable &input, VarList lParams ) {	
	for(VarList::iterator vi = lParams.begin(); vi; vi++){
		Bu::sio << (*vi).toString() << " " << Bu::sio.flush;
	}
	Bu::sio << Bu::sio.nl;
	
	return new Variable(Variable::typeNone);
}

