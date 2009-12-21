#include "ast.h"

#include "astleaf.h"
#include "astbranch.h"

Ast::Ast()
{
}

Ast::~Ast()
{
}

void Ast::addNode( AstNode::Type eType )
{
	switch( eType&AstNode::typeClassMask )
	{
		case AstNode::typeBranch:
			{
				AstBranch *pNode = new AstBranch( eType );
				addNode( pNode );
				sBranch.push( pNode );
			}
			break;

		case AstNode::typeLeaf:
			{
				AstLeaf *pNode = new AstLeaf( eType );
				addNode( pNode );
			}
			break;

		default:
			throw Bu::ExceptionBase("You got it wrong.");
			break;
	}
}

void Ast::addNode( AstNode::Type eType, int iVal )
{
	addNode( new AstLeaf( eType, iVal ) );
}

void Ast::addNode( AstNode::Type eType, float fVal )
{
	addNode( new AstLeaf( eType, fVal ) );
}

void Ast::addNode( AstNode::Type eType, bool bVal )
{
	addNode( new AstLeaf( eType, bVal ) );
}

void Ast::addNode( AstNode::Type eType, const Bu::FString &sVal )
{
	addNode( new AstLeaf( eType, sVal ) );
}

void Ast::addNode( AstNode::Type eType, const char *sVal )
{
	addNode( new AstLeaf( eType, sVal ) );
}

void Ast::addNode( AstNode *pNode )
{
	if( sBranch.isEmpty() )
		lNode.append( pNode );
	else
		sBranch.peek()->addNode( pNode );
}

void Ast::openBranch()
{
	sBranch.peek()->addBranch();
}

void Ast::closeNode()
{
	sBranch.pop();
}

Ast::NodeList::const_iterator Ast::getNodeBegin() const
{
	return lNode.begin();
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Ast &a )
{
	f << "Abstract Syntax Tree:";
	f.incIndent();
	f << f.nl;
	for( Ast::NodeList::const_iterator i = a.getNodeBegin(); i; i++ )
		f << **i << f.nl;
	f << f.nl;
	f.decIndent();
	return f;
}

