/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionunique.h"

#include <bu/hash.h>
#include <bu/plugger.h>
PluginInterface3( pluginFunctionUnique, unique, FunctionUnique, Function,
        "Mike Buland", 0, 1 );

FunctionUnique::FunctionUnique()
{
}

FunctionUnique::~FunctionUnique()
{
}

Bu::String FunctionUnique::getName() const
{
    return "unique";
}

Variable FunctionUnique::call( Variable &input, VarList lParams )
{
    if( lParams.getSize() > 0 )
        throw Bu::ExceptionBase("Unique does not take any parameters.");
    if( input.getType() != Variable::typeList )
        throw Bu::ExceptionBase("unique does not work on non-list types.");

    Bu::Hash<Bu::String, bool> hHas;

    Variable vOut( Variable::typeList );
    for( VarList::iterator i = input.begin(); i; i++ )
    {
        if( (*i).getType() != Variable::typeString )
            continue;

        Bu::String s = (*i).getString();
        
        if( hHas.has( s ) )
            continue;

        hHas.insert( s, true );
        vOut.append( *i );
    }

    return vOut;
}

