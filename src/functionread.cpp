/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionread.h"
#include "filemgr.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionRead, read, FunctionRead, Function,
        "Mike Buland", 0, 1 );

FunctionRead::FunctionRead()
{
}

FunctionRead::~FunctionRead()
{
}

Bu::String FunctionRead::getName() const
{
    return "read";
}

Variable FunctionRead::call( Variable &input, VarList lParams )
{
    Variable vRet;
    if( lParams.getSize() == 1 )
    {
        int iSize = lParams.first().toInt();
        Bu::String sBuf( iSize );
        sBuf.resize(
            FileMgr::getInstance().get( (ptrdiff_t)input.getOpaque() ).read(
                sBuf.getStr(), iSize
                )
            );
        vRet = sBuf;
        return vRet;
    }
    throw Bu::ExceptionBase(
        "read takes zero or one parameters."
        );
}

