#include "astbranch.h"

AstBranch::AstBranch( const Location &loc, Type eType ) :
    AstNode( loc, eType )
{
}

AstBranch::~AstBranch()
{
}

void AstBranch::addBranch()
{
    lBranch.append( NodeList() );
}

void AstBranch::addNode( AstNode *pNode )
{
    lBranch.last().append( pNode );
}

AstBranch::BranchList::const_iterator AstBranch::getBranchBegin() const
{
    return lBranch.begin();
}

Bu::Formatter &operator<<( Bu::Formatter &f, const AstBranch &l )
{
    f.incIndent();
    f << ":";
    for( AstBranch::BranchList::const_iterator i = l.getBranchBegin(); i; i++ )
    {
        f << f.nl << "Branch:";
        f.incIndent();
        for( AstBranch::NodeList::const_iterator j = i->begin(); j; j++ )
        {
            f << f.nl << **j;
        }
        f.decIndent();
    }
    f.decIndent();
    return f;
}

