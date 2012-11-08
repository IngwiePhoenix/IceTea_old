/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef AST_H
#define AST_H

#include "bu/list.h"
#include "bu/stack.h"
#include "bu/string.h"
#include "bu/formatter.h"

#include "astnode.h"

/**
 * Abstract Symbol Tree.  This is the thing that the parser builds for us.  In
 * the end, this is also what we "run" when we run build files.
 */
class Ast
{
public:
    typedef Bu::List<AstNode *> NodeList;
    Ast();
    virtual ~Ast();

    void addNode( struct YYLTYPE &loc, AstNode::Type eType );
    void addNode( struct YYLTYPE &loc, AstNode::Type eType, int iVal );
    void addNode( struct YYLTYPE &loc, AstNode::Type eType, float fVal );
    void addNode( struct YYLTYPE &loc, AstNode::Type eType, bool bVal );
    void addNode( struct YYLTYPE &loc, AstNode::Type eType,
            const Bu::String &sVal );
    void addNode( struct YYLTYPE &loc, AstNode::Type eType, const char *sVal );

    void addNode( AstNode::Type eType );
    void addNode( AstNode::Type eType, int iVal );
    void addNode( AstNode::Type eType, float fVal );
    void addNode( AstNode::Type eType, bool bVal );
    void addNode( AstNode::Type eType, const Bu::String &sVal );
    void addNode( AstNode::Type eType, const char *sVal );
    void addNode( AstNode *pNode );

    void openBranch();

    void closeNode();

    NodeList::const_iterator getNodeBegin() const;

private:
    NodeList lNode;
    typedef Bu::Stack<class AstBranch *> BranchStack;
    BranchStack sBranch;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const Ast &a );

#endif
