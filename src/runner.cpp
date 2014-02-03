/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "runner.h"

#include "ast.h"
#include "astnode.h"
#include "astleaf.h"
#include "astbranch.h"
#include "context.h"
#include "functionast.h"
#include "rule.h"
#include "variable.h"
#include "target.h"
#include "action.h"
#include "profile.h"
#include "view.h"

#include "bu/sio.h"
using Bu::sio;

Runner::Runner( Ast &rAst, Context &rCont ) :
    rAst( rAst ),
    rCont( rCont ),
    pCurTarget( NULL ),
    pCurRule( NULL )
{
}

Runner::~Runner()
{
}

void Runner::initialize()
{
    for( Ast::NodeList::const_iterator i = rAst.getNodeBegin(); i; i++ )
    {
        if( (*i)->getType() == AstNode::typeFunctionDef )
        {
            AstBranch *pFnc = dynamic_cast<AstBranch *>(*i);
            rCont.addFunction( new FunctionAst( pFnc, this ) );
        }
        else if( (*i)->getType() == AstNode::typeActionDef )
        {
            AstBranch *pAction = dynamic_cast<AstBranch *>(*i);
            rCont.addAction( new Action( pAction ) );
        }
    }
}

Variable Runner::execFunc( const AstBranch *pFunc, Variable &vIn )
{
    Bu::String sName = dynamic_cast<const AstLeaf *>(
        (*pFunc->getBranchBegin()).first())->getStrValue();

    VarList lParams;
    for( AstBranch::BranchList::const_iterator p = 
        pFunc->getBranchBegin()+1; p; p++ )
    {
        lParams.append( execExpr( (*p).begin() ) );
    }

    return rCont.call( sName, vIn, lParams );
}

Variable Runner::execExpr( AstBranch::NodeList::const_iterator e )
{
    Variable vBlank;
    return execExpr( e, vBlank );
}

Variable Runner::execExpr( AstBranch::NodeList::const_iterator e,
    const Variable &vIn )
{
//  Variable v( vIn );
    VarList lStack;
    lStack.push( vIn );

    for(; e; e++ )
    {
        if( ((*e)->getType()&AstNode::typeClassMask) == AstNode::typeBranch )
        {
            const AstBranch *pBranch = dynamic_cast<const AstBranch *>( *e );
            switch( pBranch->getType() )
            {
                case AstNode::typeFunction:
                    //sio << "FUNC:  " << *pBranch << sio.nl << sio.nl;
                    {
                        Variable v = lStack.peekPop();
                        lStack.push( execFunc( pBranch, v ) );
                    }
                    break;

                case AstNode::typeSet:
                    lStack.push( doSet( pBranch ) );
                    break;

                case AstNode::typeList:
                    {
                        Variable vLst( Variable::typeList );
                        for( AstBranch::BranchList::const_iterator i =
                             pBranch->getBranchBegin(); i; i++ )
                        {
                            vLst.append( execExpr( (*i).begin() ) );
                        }
                        lStack.push( vLst );
                    }
                    break;

                case AstNode::typeExpr:
                    {
                        sio << "!!! typeExpr in an expr maybe should be an error..." << sio.nl;
                        for( AstBranch::BranchList::const_iterator i = 
                            pBranch->getBranchBegin(); i; i++ )
                        {
                            lStack.push(
                                execExpr( (*i).begin() ) // Are they atomic?
                                );
                        }
                        if( lStack.getSize() != 1 )
                        {
                            throw Bu::ExceptionBase(
                                "Something went wrong, expression processing "
                                "left %d elements on stack, should be 1.",
                                lStack.getSize() );
                        }
                    }
                    break;

                default:
                    sio << "?? branch ???: "
                        << (pBranch)->getType();
                    break;
            }
        }
        else
        {
            const AstLeaf *pLeaf = dynamic_cast<const AstLeaf *>( *e );
            switch( pLeaf->getType() )
            {
                case AstNode::typeVariable:
                    try
                    {
                        lStack.push(
                            rCont.getVariable( pLeaf->getStrValue() )
                            );
                    }
                    catch(...)
                    {
                        lStack.push( Variable() );
                    }
                    break;

                case AstNode::typeVariableRef:
                    lStack.push(
                        Variable::mkRef( pLeaf->getStrValue() )
                        );
                    break;

                case AstNode::typeString:
                    lStack.push(
                        rCont.expand( pLeaf->getStrValue() )
                        );
                    break;

                case AstNode::typeInt:
                    lStack.push(
                        pLeaf->getIntValue()
                        );
                    break;

                case AstNode::typeFloat:
                    lStack.push(
                        pLeaf->getFloatValue()
                        );
                    break;

                case AstNode::typeBool:
                    lStack.push(
                        pLeaf->getBoolValue()
                        );
                    break;

                case AstNode::typeVersion:
                    break;

                case AstNode::typeNull:
                    lStack.push(
                        Variable()
                        );
                    break;

                case AstNode::typeCmpEq:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( a == b ) );
                    }
                    break;

                case AstNode::typeCmpLt:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( b < a ) );
                    }
                    break;

                case AstNode::typeCmpGt:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( b > a ) );
                    }
                    break;

                case AstNode::typeCmpNe:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( a != b ) );
                    }
                    break;

                case AstNode::typeCmpLtEq:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( b <= a ) );
                    }
                    break;

                case AstNode::typeCmpGtEq:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( b >= a ) );
                    }
                    break;

                case AstNode::typeOpEq:
                    {
                        Variable ref, val;
                        val = lStack.peekPop();
                        ref = lStack.peekPop();
                        rCont.addVariable( ref.getString(), val );
                        lStack.push( val );
                    }
                    break;

                case AstNode::typeOpPlusEq:
                    {
                        Variable ref, val;
                        val = lStack.peekPop();
                        ref = lStack.peekPop();
                        try
                        {
                            Variable &nVal = rCont.getVariable(
                                ref.getString()
                                );
                            nVal += val;
                            lStack.push( nVal );
                        } catch(...)
                        {
                            rCont.addVariable( ref.getString(), val );
                            lStack.push( val );
                        }
                    }
                    break;

                case AstNode::typeOpPlusEqRaw:
                    {
                        Variable ref, val;
                        val = lStack.peekPop();
                        ref = lStack.peekPop();
                        try
                        {
                            Variable &nVal = rCont.getVariable(
                                ref.getString()
                                );
                            nVal << val;
                            lStack.push( nVal );
                        } catch(...)
                        {
                            rCont.addVariable( ref.getString(), val );
                            lStack.push( val );
                        }
                    }
                    break;

                case AstNode::typeOpPlus:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( b + a ) );
                    }
                    break;

                case AstNode::typeOpMinus:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( b - a ) );
                    }
                    break;

                case AstNode::typeOpMultiply:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( b * a ) );
                    }
                    break;

                case AstNode::typeOpDivide:
                    {
                        Variable a, b;
                        a = lStack.peekPop();
                        b = lStack.peekPop();
                        lStack.push( Variable( b / a ) );
                    }
                    break;

                case AstNode::typeOpNegate:
                    lStack.peek().doNegate();
                    break;

                case AstNode::typeOpNot:
                    lStack.peek().doNot();
                    break;

                default:
                    sio << "?? leaf ???: "
                        << (pLeaf)->getType();
                    break;
            }
        }
    }

    return lStack.peek();
}

void Runner::run()
{
    run( rAst.getNodeBegin() );

    rCont.buildTargetTree( *this );

    rCont.attachDefaults();
    rCont.genDefaultActions();

//  rCont.writeTargetDot();
}

Variable Runner::run( AstBranch::NodeList::const_iterator n )
{
    /* Execute the top level code. */

    Variable vReturn;
    Bu::List<Ast::NodeList::const_iterator> sI;
    sI.push( n );
//  for( Ast::NodeList::const_iterator i = rAst.getNodeBegin(); i; i++ )
    while( !sI.isEmpty() )
    {
        while( !sI.isEmpty() && !(sI.peek()) )
        {
            sI.pop();
        }
        if( sI.isEmpty() )
            break;
        Ast::NodeList::const_iterator &i = sI.peek();
        if( ((*i)->getType()&AstNode::typeClassMask) == AstNode::typeLeaf )
        {
            const AstLeaf *pExpr = dynamic_cast<const AstLeaf *>( *i );
            switch( pExpr->getType() )
            {
                case AstNode::typeError:
                    {
                        Bu::String sMsg = rCont.expand( pExpr->getStrValue() );
                        rCont.getView()->userError( sMsg.getStr() );
                        throw Bu::ExceptionBase( sMsg.getStr() );
                    }
                    break;

                case AstNode::typeWarning:
                    rCont.getView()->userWarning(
                        rCont.expand( pExpr->getStrValue() )
                        );
                    break;

                case AstNode::typeNotice:
                    rCont.getView()->userNotice(
                        rCont.expand( pExpr->getStrValue() )
                        );
                    break;

                case AstNode::typeCondition:
                    break;

                case AstNode::typeDisplay:
                    if( pCurTarget )
                    {
                        pCurTarget->setDisplay(
                            rCont.expand( pExpr->getStrValue() )
                            );
                    }
                    else if( pCurRule )
                    {
                        pCurRule->setDisplay(
                            rCont.expand( pExpr->getStrValue() )
                            );
                    }
                    break;

                case AstNode::typePushPrefix:
                case AstNode::typePopPrefix:
                    break;
/*
                case AstNode::typeCondition:
                    if( pCurTarget )
                    {
                        if( pExpr->getStrValue() == "filetime" )
                        {
                            pCurTarget->setCondition(
                                new ConditionFileTime()
                                );
                        }
                    }
                    else if( pCurRule )
                    {
                        if( pExpr->getStrValue() == "filetime" )
                        {
                            pCurRule->setCondition(
                                new ConditionFileTime()
                                );
                        }
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "You can only set a condition in a target or rule.");
                    }
                    break;
*/
                default:
                    sio << "Leaf?  " << (*i)->getType() << sio.nl;
                    break;
            }
        }
        else
        {
            const AstBranch *pExpr = dynamic_cast<const AstBranch *>( *i );
            switch( pExpr->getType() )
            {
                case AstNode::typeSet:
                    {
                        // This is effectively legacy, if we add the set
                        // keyword back in I want it to work.
                        doSet( pExpr );
                    }
                    break;

                case AstNode::typeUnset:
                    {
                        AstBranch::NodeList::const_iterator n =
                            (*pExpr->getBranchBegin()).begin();
                        Bu::String sVar = dynamic_cast<const AstLeaf *>(
                            *n )->getStrValue();
                        rCont.delVariable( sVar );
                    }
                    break;

                case AstNode::typeIf:
                    {
                        AstBranch::BranchList::const_iterator b =
                            pExpr->getBranchBegin();

                        Variable v = execExpr( (*b).begin() );
                        if( v.getType() != Variable::typeBool )
                        {
                            throw Bu::ExceptionBase(
                                "If statement evaluated to non-boolean.");
                        }
                        b++;
                        if( v.getBool() )
                        {
                            i++;
                            sI.push( (*b).begin() );
                            continue;
                        }
                        else
                        {
                            b++;
                            if( b )
                            {
                                i++;
                                sI.push( (*b).begin() );
                                continue;
                            }
                        }
                    }
                    break;

                case AstNode::typeFor:
                    {
                        AstBranch::BranchList::const_iterator b =
                            pExpr->getBranchBegin();
                        Bu::String sVar = dynamic_cast<const AstLeaf *>(
                            (*b).first() )->getStrValue();
                        b++;
                        Variable v = execExpr( (*b).begin() );
                        b++;
                        for( VarList::const_iterator vi = v.getList().begin();
                             vi; vi++ )
                        {
                            rCont.addVariable( sVar, *vi );
                            run( (*b).begin() );
                        }
                    }
                    break;

                case AstNode::typeFunction:
                    {
                        Variable vIn;
                        execFunc( pExpr, vIn );
                    }
                    break;

                case AstNode::typeReturn:
                    vReturn = execExpr( (*pExpr->getBranchBegin()).begin() );
                    return vReturn;
                    break;

                case AstNode::typeFunctionDef:
                case AstNode::typeActionDef:
                    // We ignore these, we already dealt with them
                    break;

                case AstNode::typeTarget:
                    // This actually runs exactly like a for loop, if there's
                    // only one item, then we only go once, if it's a list, go
                    // more than once :-P
                    if( pCurTarget == NULL )
                    {
                        AstBranch::BranchList::const_iterator b =
                            pExpr->getBranchBegin();
                        Variable vLoop = execExpr( (*b).begin() );
                        b++;
                        if( vLoop.getType() == Variable::typeString )
                        {
                            rCont.addTarget(
                                buildTarget(
                                    vLoop.getString(), (*b).begin()
                                    )
                                );
                        }
                        else if( vLoop.getType() == Variable::typeList )
                        {
                            for( VarList::iterator i = vLoop.begin(); i; i++ )
                            {
                                rCont.addTarget(
                                    buildTarget(
                                        (*i).getString(), (*b).begin()
                                        )
                                    );
                            }
                        }
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "You cannot declare a target within "
                            "a target decleration.");
                    }
                    break;

                case AstNode::typeRuleDef:
                    if( pCurRule == NULL )
                    {
                        AstBranch::BranchList::const_iterator b =
                            pExpr->getBranchBegin();
                        Bu::String sName = dynamic_cast<const AstLeaf *>(
                            (*b).first()
                            )->getStrValue();
                        b++;
                        rCont.addRule( buildRule( sName, (*b).begin() ) );
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "You cannot declare a rule within "
                            "a rule decleration.");
                    }
                    break;

                case AstNode::typeInput:
                    if( pCurTarget != NULL )
                    {
                        Variable vRet = execExpr(
                            (*pExpr->getBranchBegin()).begin()
                            );
                        if( vRet.getType() == Variable::typeString )
                        {
                            pCurTarget->addInput( vRet.getString() );
                        }
                        else if( vRet.getType() == Variable::typeList )
                        {
                            for( VarList::iterator i = vRet.begin(); i; i++ )
                            {
                                pCurTarget->addInput(
                                    (*i).getString()
                                    );
                            }
                        }
                    }
                    else if( pCurRule != NULL )
                    {
                        pCurRule->setInput( pExpr );
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "input can only occur within a target or rule.");
                    }
                    break;

                case AstNode::typeRequires:
                    if( pCurTarget != NULL )
                    {
                        Variable vRet = execExpr(
                            (*pExpr->getBranchBegin()).begin()
                            );
                        if( vRet.getType() == Variable::typeString )
                        {
                            pCurTarget->addRequires( vRet.getString() );
                        }
                        else if( vRet.getType() == Variable::typeList )
                        {
                            for( VarList::iterator i = vRet.begin(); i; i++ )
                            {
                                pCurTarget->addRequires(
                                    (*i).getString()
                                    );
                            }
                        }
                    }
                    else if( pCurRule != NULL )
                    {
                        pCurRule->addRequires( pExpr );
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "requires can only occur within a target or rule.");
                    }
                    break;

                case AstNode::typeRule:
                    if( pCurTarget )
                    {
                        pCurTarget->setRule(
                            dynamic_cast<const AstLeaf *>(
                                (*pExpr->getBranchBegin()).first()
                                )->getStrValue()
                            );
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "rule can only occur within a target.");
                    }
                    break;

                case AstNode::typeProfile:
                    if( pCurTarget )
                    {
                        pCurTarget->addProfile( pExpr );
                    }
                    else if( pCurRule )
                    {
                        pCurRule->addProfile( pExpr );
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "profile can only occur within a target or rule.");
                    }
                    break;

                case AstNode::typeOutput:
                    if( pCurRule )
                    {
                        pCurRule->addOutput( pExpr );
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "output can only occur within a rule.");
                    }
                    break;

                case AstNode::typeProcessTarget:
                    {
                        AstBranch::BranchList::const_iterator b =
                            pExpr->getBranchBegin();
                        Bu::String sProfile = dynamic_cast<const AstLeaf *>(
                            (*b).first()
                            )->getStrValue();
                        b++;
                        Variable vTargs = execExpr( (*b).begin() );
                        if( vTargs.getType() == Variable::typeString )
                        {
                            rCont.getTarget( vTargs.getString() )->process(
                                *this, sProfile
                                );
                        }
                        else if( vTargs.getType() == Variable::typeList )
                        {
                            for( VarList::iterator v = vTargs.begin();
                                 v; v++ ) {
                                rCont.getTarget( (*v).getString() )->process(
                                    *this, sProfile
                                    );
                            }
                        }
                    }
                    break;

                case AstNode::typeTag:
                    if( pCurTarget )
                    {
                        AstBranch::BranchList::const_iterator b =
                            pExpr->getBranchBegin();
                        Variable vTags = execExpr( (*b).begin() );
                        if( vTags.getType() == Variable::typeList )
                        {
                            for( VarList::iterator i = vTags.begin(); i; i++ )
                            {
                                rCont.addTargetToTag( pCurTarget, (*i).toString() );
                            }
                        }
                        else
                        {
                            Bu::String sTag = vTags.toString();
                            if( sTag.isSet() )
                            {
                                rCont.addTargetToTag( pCurTarget, sTag );
                            }
                            else
                            {
                                throw Bu::ExceptionBase(
                                    "A tag evaluted to empty string."
                                    );
                            }
                        }
                    }
                    else if( pCurRule )
                    {
                        AstBranch::BranchList::const_iterator b =
                            pExpr->getBranchBegin();
                        Variable vTags = execExpr( (*b).begin() );
                        if( vTags.getType() == Variable::typeList )
                        {
                            for( VarList::iterator i = vTags.begin(); i; i++ )
                            {
                                pCurRule->addTag( (*i).toString() );
                            }
                        }
                        else
                        {
                            Bu::String sTag = vTags.toString();
                            if( sTag.isSet() )
                            {
                                pCurRule->addTag( sTag );
                            }
                            else
                            {
                                throw Bu::ExceptionBase(
                                    "A tag evaluted to empty string."
                                    );
                            }
                        }
                    }
                    else
                    {
                        throw Bu::ExceptionBase(
                            "tag can only occur within a target or rule.");
                    }
                    break;

                case AstNode::typeExpr:
                    execExpr( (*pExpr->getBranchBegin()).begin() );
                    break;

                default:
                    sio << "Branch?  " << (*i)->getType() << sio.nl;
                    break;
            }
        }

        i++;
    }

    return vReturn;
}

void Runner::execProfile( Target *pTarget, const Bu::String &sProfile )
{
    rCont.pushScope( pTarget->getVars() );
    run( (*(pTarget->getProfile( sProfile )->getRoot()->
        getBranchBegin()+1)).begin() );
    rCont.popScope();
}

void Runner::execAction( const Bu::String &sName )
{
    try
    {
        Action *pAct = rCont.getAction( sName );

        pAct->call( this );
    }
    catch( Bu::HashException &e )
    {
        Bu::String sError("No such action '" + sName + "' found.");
        rCont.getView()->sysError( sError );
    }
}

Context &Runner::getContext()
{
    return rCont;
}

Target *Runner::buildTarget( const Bu::String &sOutput,
        AstBranch::NodeList::const_iterator n )
{
    Target *pTrg = NULL;
    try
    {
        pTrg = rCont.getTarget( sOutput );
        rCont.pushScope( pTrg->getVars() );
    }
    catch( Bu::HashException &e )
    {
        pTrg = new Target( sOutput, true );
        rCont.pushScope();
    }

    // sio << " (target) \"" << sOutput << "\" explicit." << sio.nl;

    rCont.addVariable("OUTPUT", sOutput );
    pCurTarget = pTrg;
    run( n );

    rCont.addVariable("INPUT", pTrg->getInputList() );
    pCurTarget = NULL;
    
    pTrg->setVars( rCont.getScope() );
    rCont.popScope();

    return pTrg;
}

Rule *Runner::buildRule( const Bu::String &sName,
        AstBranch::NodeList::const_iterator n )
{
    Rule *pRule = new Rule( sName );

    rCont.pushScope();
    pCurRule = pRule;
    run( n );
    rCont.popScope();
    pCurRule = NULL;
    
    return pRule;
}

Variable Runner::doSet( const AstBranch *pRoot )
{
    AstBranch::NodeList::const_iterator n =
        (*pRoot->getBranchBegin()).begin();
    Bu::String sVar = dynamic_cast<const AstLeaf *>( *n )->getStrValue();
    n++;
    const AstLeaf *pLeaf = dynamic_cast<const AstLeaf *>( *n );
    n++;
    Variable v = execExpr( n );

    switch( pLeaf->getType() )
    {
        case AstNode::typeOpEq:
            rCont.addVariable( sVar, v );
            break;

        case AstNode::typeOpPlusEq:
            try
            {
                rCont.getVariable( sVar ) += v;
            } catch(...)
            {
                rCont.addVariable( sVar, v );
            }
            break;

        case AstNode::typeOpPlusEqRaw:
            try
            {
                rCont.getVariable( sVar ) << v;
            } catch(...)
            {
                rCont.addVariable( sVar, v );
            }
            break;

        default: break;
    }

    return v;
}

