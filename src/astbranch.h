#ifndef AST_BRANCH_H
#define AST_BRANCH_H

#include "bu/list.h"
#include "astnode.h"
#include "bu/formatter.h"

class AstBranch : public AstNode
{
public:
    typedef Bu::List<AstNode *> NodeList;
    typedef Bu::List<NodeList> BranchList;
    AstBranch( const Location &loc, Type eType );
    virtual ~AstBranch();

    void addBranch();
    void addNode( AstNode *pNode );

    BranchList::const_iterator getBranchBegin() const;

private:
    BranchList lBranch;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const AstBranch &l );

#endif
