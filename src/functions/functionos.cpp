/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "FunctionOS.h"
#include "helper_macros.h"

#include <bu/plugger.h>
#include <bu/sio.h>
PluginInterface3( pluginFunctionOS, OS, FunctionOS, Function,
        "Ingwie Phoenix", 0, 1 );
FUNCTION_NAME(FunctionOS, "OS")  

FunctionOS::FunctionOS() {}
FunctionOS::~FunctionOS() {}

Variable FunctionOS::call( Variable &input, VarList lParams ) {
	Bu::String os;
	
	#ifdef __WIN32__
	os = "win";
	#endif
	
	#ifdef __APPLE__
	os = "mac";
	#endif
	
	#ifdef __linux__
	os = "linux";
	#endif
	
	Variable rt = new Variable(Variable::typeString);
	rt = os;
	return rt;
}

