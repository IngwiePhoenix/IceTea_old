/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionopen.h"
#include "filemgr.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionOpen, open, FunctionOpen, Function,
        "Mike Buland", 0, 1 );

FunctionOpen::FunctionOpen()
{
}

FunctionOpen::~FunctionOpen()
{
}

Bu::String FunctionOpen::getName() const
{
    return "open";
}

Variable FunctionOpen::call( Variable &input, VarList lParams )
{
    if( lParams.getSize() != 2 )
    {
        throw Bu::ExceptionBase(
            "open takes two parameters, filename and mode."
            );
    }
    Bu::String sMode = lParams.last().toString().toLower();
    int iMode = Bu::File::Create;
    if( sMode.find('w') )
        iMode |= Bu::File::Write;
    if( sMode.find('r') )
        iMode |= Bu::File::Read;
    Variable vRet(
        (void *)FileMgr::getInstance().open(
            lParams.first().toString(), iMode
            )
        );
    return vRet;
}

