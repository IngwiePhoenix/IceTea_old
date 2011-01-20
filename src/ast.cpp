#include "ast.h"

#include "astleaf.h"
#include "astbranch.h"

#include "build.tab.h"

Ast::Ast()
{
}

Ast::~Ast()
{
}

void Ast::addNode( YYLTYPE &loc, AstNode::Type eType )
{
	switch( eType&AstNode::typeClassMask )
	{
		case AstNode::typeBranch:
			{
				AstBranch *pNode = new AstBranch( loc, eType );
				addNode( pNode );
				sBranch.push( pNode );
			}
			break;

		case AstNode::typeLeaf:
			{
				AstLeaf *pNode = new AstLeaf( loc, eType );
				addNode( pNode );
			}
			break;

		default:
			throw Bu::ExceptionBase("You got it wrong.");
			break;
	}
}

void Ast::addNode( YYLTYPE &loc, AstNode::Type eType, int iVal )
{
	addNode( new AstLeaf( loc, eType, iVal ) );
}

void Ast::addNode( YYLTYPE &loc, AstNode::Type eType, float fVal )
{
	addNode( new AstLeaf( loc, eType, fVal ) );
}

void Ast::addNode( YYLTYPE &loc, AstNode::Type eType, bool bVal )
{
	addNode( new AstLeaf( loc, eType, bVal ) );
}

void Ast::addNode( YYLTYPE &loc, AstNode::Type eType, const Bu::String &sVal )
{
	addNode( new AstLeaf( loc, eType, sVal ) );
}

void Ast::addNode( YYLTYPE &loc, AstNode::Type eType, const char *sVal )
{
	addNode( new AstLeaf( loc, eType, sVal ) );
}

void Ast::addNode( AstNode::Type eType )
{
	YYLTYPE none = {-1, -1, -1, -1};
	addNode( none, eType );
}

void Ast::addNode( AstNode::Type eType, int iVal )
{
	YYLTYPE none = {-1, -1, -1, -1};
	addNode( none, eType, iVal );
}

void Ast::addNode( AstNode::Type eType, float fVal )
{
	YYLTYPE none = {-1, -1, -1, -1};
	addNode( none, eType, fVal );
}

void Ast::addNode( AstNode::Type eType, bool bVal )
{
	YYLTYPE none = {-1, -1, -1, -1};
	addNode( none, eType, bVal );
}

void Ast::addNode( AstNode::Type eType, const Bu::String &sVal )
{
	YYLTYPE none = {-1, -1, -1, -1};
	addNode( none, eType, sVal );
}

void Ast::addNode( AstNode::Type eType, const char *sVal )
{
	YYLTYPE none = {-1, -1, -1, -1};
	addNode( none, eType, sVal );
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

