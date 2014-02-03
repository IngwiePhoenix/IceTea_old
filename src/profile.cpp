/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "profile.h"
#include "ast.h"
#include "astbranch.h"
#include "astleaf.h"
#include "condition.h"

#include "conditionplugger.h"

#include <bu/sio.h>
using namespace Bu;

Profile::Profile( const class AstBranch *pRoot ) :
    pRoot( pRoot ),
    pCond( NULL ),
    pAst( NULL )
{
    sName = dynamic_cast<const AstLeaf *>(
        (*pRoot->getBranchBegin()).first()
        )->getStrValue();

    setCondition();
}

Profile::Profile( const Profile &rSrc ) :
    sName( rSrc.sName ),
    pRoot( rSrc.pRoot ),
    pCond( rSrc.pCond->clone() ),
    pAst( NULL )
{
}

Profile::~Profile()
{
    delete pAst;
    pAst = NULL;
}

const Bu::String &Profile::getName() const
{
    return sName;
}

const AstBranch *Profile::getRoot() const
{
    return pRoot;
}

const Condition *Profile::getCond() const
{
    return pCond;
}

bool Profile::shouldExec( class Runner &r, class Target &rTarget ) const
{
    return pCond->shouldExec( r, rTarget );
}

Profile *Profile::genDefaultClean()
{
    Ast *pAst = new Ast();
    pAst->addNode( AstNode::typeProfile );
        pAst->openBranch();
            pAst->addNode( AstNode::typeString, "clean" );
        pAst->openBranch();
            pAst->addNode( AstNode::typeCondition, "fileExists" );
            pAst->addNode( AstNode::typeFunction );
                pAst->openBranch();
                    pAst->addNode( AstNode::typeString, "unlink" );
                        pAst->openBranch();
                            pAst->addNode( AstNode::typeVariable, "OUTPUT" );
                    pAst->closeNode();
            pAst->closeNode();
    //pAst->closeNode();
    Profile *pRet = new Profile(
        dynamic_cast<const AstBranch *>(*pAst->getNodeBegin())
        );
    pRet->pAst = pAst;

    return pRet;
}

void Profile::setCondition()
{
    for( AstBranch::NodeList::const_iterator i =
         (*(pRoot->getBranchBegin()+1)).begin(); i; i++ )
    {
        if( (*i)->getType() == AstNode::typeCondition )
        {
            Bu::String sCond = dynamic_cast<const AstLeaf *>(*i)->getStrValue();
            delete pCond;
            pCond = ConditionPlugger::getInstance().instantiate( sCond );
        }
    }

    if( pCond == NULL )
    {
        // The default condition
        pCond = ConditionPlugger::getInstance().instantiate("fileTime");
    }
}

