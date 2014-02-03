/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionfilename.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionFileName, fileName, FunctionFileName, Function,
        "Mike Buland", 0, 1 );

FunctionFileName::FunctionFileName()
{
}

FunctionFileName::~FunctionFileName()
{
}

Bu::String FunctionFileName::getName() const
{
    return "fileName";
}

Variable FunctionFileName::call( Variable &input, VarList /*lParams*/ )
{
    Bu::String sFile;
    sFile = input.getString();

    Bu::String::const_iterator i = sFile.begin();
    for(;;)
    {
        Bu::String::const_iterator b = i.find('/');
        if( !b )
        {
            return Variable( Bu::String( i ) );
        }
        i = b+1;
        if( !i )
        {
            return Variable( Bu::String( i ) );
        }
    }
}

