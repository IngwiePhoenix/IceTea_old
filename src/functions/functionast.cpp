/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "functionast.h"
#include "astbranch.h"
#include "astleaf.h"
#include "runner.h"
#include "context.h"

#include <bu/sio.h>
using namespace Bu;

FunctionAst::FunctionAst( const AstBranch *pRoot, class Runner *pRunner ) :
    pRoot( pRoot ),
    pRunner( pRunner )
{
    sName = dynamic_cast<AstLeaf *>(
        *(*pRoot->getBranchBegin()).begin()
        )->getStrValue();
}

FunctionAst::~FunctionAst()
{
}

Bu::String FunctionAst::getName() const
{
    return sName;
}

Variable FunctionAst::call( Variable &input, VarList lParams )
{
    pContext->pushScope();
    
    AstBranch::NodeList::const_iterator vName = 
        (*(pRoot->getBranchBegin()+1)).begin();
    VarList::iterator vValue = lParams.begin();
    for( ; vName && vValue; vName++, vValue++ )
    {
        pContext->addVariable(
            dynamic_cast<const AstLeaf *>(*vName)->getStrValue(),
            *vValue
            );
    }
    pContext->addVariable("INPUT", input );
    Variable vRet = pRunner->run( (*(pRoot->getBranchBegin()+2)).begin() );
    pContext->popScope();
    return vRet;
}

