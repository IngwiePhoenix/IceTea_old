/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "bu/hash.h"
#include "bu/string.h"

#include "variable.h"

class Target;
class Rule;
class Function;
class Action;
class View;

class Context
{
    friend Bu::Formatter &operator<<( Bu::Formatter &f, const Context &c );
public:
    Context();
    virtual ~Context();

    void addTarget( Target *pTarget );
    void addRule( Rule *pRule );
    void addFunction( Function *pFunction );
    void addVariable( const Bu::String &sName, const Variable &vValue );
    void addAction( Action *pAction );
    Action *getAction( const Bu::String &sName );

    void addTargetToTag( Target *pTarget, const Bu::String &sTag );
    void addTargetToTags( Target *pTarget, const StrList &sTags );
    TargetList &getTag( const Bu::String &sTag );

    Variable &getVariable( const Bu::String &sName );
    void delVariable( const Bu::String &sName );

    void pushScope();
    void pushScope( const VarHash &hNewVars );
    VarHash &getScope();
    void popScope();

    Variable call( const Bu::String &sName, Variable &input, VarList lParams );

    Bu::String expand( const Bu::String &sIn );

    Target *getTarget( const Bu::String &sOutput );
    TargetList getExplicitTargets();

    /**
     * This function actually builds the dependancy tree, and is responsible
     * for creating all of the auto-generated targets that are required by the
     * explicitly created targets.
     */
    void buildTargetTree( class Runner &r );

    /**
     * This is done as a final step, it goes through all targets and
     * attaches things that they should have even if they haven't defined them,
     * like a clean profile, they'll get that even if they haven't added one of
     * their own.  The defaults in this routine are added only if they aren't
     * already defined in the target.  It should be excetued after
     * buildTargetTree, which means it doesn't need to affect rules.
     */
    void attachDefaults();

    /**
     * This function generates some default actions if they don't already
     * exist, pretty straight forward, it will create all, clean, and default
     * (default is the same as all).
     */
    void genDefaultActions();

    void writeTargetDot();

    void setView( View *pNewView );
    View *getView();

    void printBasicInfo();

private:
    void buildTargetTree( class Runner &r, class Target *pTarget, const Bu::String &sInput, class Rule *pMaster, StrList &lNewIns );

private:
    typedef Bu::Hash<Bu::String, Target *> TargetHash;
    typedef Bu::Hash<Bu::String, Rule *> RuleHash;
    typedef Bu::Hash<Bu::String, Function *> FunctionHash;
    typedef Bu::Hash<Bu::String, Action *> ActionHash;
    typedef Bu::List<VarHash> ScopeStack;
    typedef Bu::Hash<Bu::String, TargetList> TagHash;
    
    TargetHash hTarget;
    RuleHash hRule;
    FunctionHash hFunction;
    ScopeStack sVars;
    ActionHash hAction;
    TagHash hTag;
    View *pView;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const Context &c );

#endif
