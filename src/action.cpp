#include "action.h"
#include "ast.h"
#include "astbranch.h"
#include "astleaf.h"
#include "runner.h"
#include "variable.h"

Action::Action( const class AstBranch *pRoot ) :
	pRoot( pRoot ),
	pAst( NULL )
{
	sName = dynamic_cast<AstLeaf *>(
		*(*pRoot->getBranchBegin()).begin()
		)->getStrValue();
}

Action::~Action()
{
	delete pAst;
	pAst = NULL;
}

const Bu::FString &Action::getName() const
{
	return sName;
}

void Action::call( class Runner *pRunner )
{
	pRunner->run( (*(pRoot->getBranchBegin()+1)).begin() );
}

Action *Action::genDefaultAll()
{
	Ast *pAst = new Ast();
	pAst->addNode( AstNode::typeActionDef );
		pAst->openBranch();
			pAst->addNode( AstNode::typeString, "all" );
		pAst->openBranch();
			pAst->addNode( AstNode::typeProcessTarget );
				pAst->openBranch();
					pAst->addNode( AstNode::typeString, "build" );
				pAst->openBranch();
					pAst->addNode( AstNode::typeFunction );
						pAst->openBranch();
							pAst->addNode( AstNode::typeString, "targets" );
					pAst->closeNode();
			pAst->closeNode();
	pAst->closeNode();
	Action *pRet = new Action(
		dynamic_cast<const AstBranch *>( *pAst->getNodeBegin() )
		);
	pRet->pAst = pAst;

	return pRet;
}

Action *Action::genDefaultClean()
{
	Ast *pAst = new Ast();
	pAst->addNode( AstNode::typeActionDef );
		pAst->openBranch();
			pAst->addNode( AstNode::typeString, "clean" );
		pAst->openBranch();
			pAst->addNode( AstNode::typeProcessTarget );
				pAst->openBranch();
					pAst->addNode( AstNode::typeString, "clean" );
				pAst->openBranch();
					pAst->addNode( AstNode::typeFunction );
						pAst->openBranch();
							pAst->addNode( AstNode::typeString, "targets" );
					pAst->closeNode();
			pAst->closeNode();
	pAst->closeNode();
	Action *pRet = new Action(
		dynamic_cast<const AstBranch *>( *pAst->getNodeBegin() )
		);
	pRet->pAst = pAst;

	return pRet;
}

Action *Action::genDefaultCleanAll()
{
	Ast *pAst = new Ast();
	pAst->addNode( AstNode::typeActionDef );
		pAst->openBranch();
			pAst->addNode( AstNode::typeString, "clean-all" );
		pAst->openBranch();
			pAst->addNode( AstNode::typeProcessTarget );
				pAst->openBranch();
					pAst->addNode( AstNode::typeString, "clean" );
				pAst->openBranch();
					pAst->addNode( AstNode::typeFunction );
						pAst->openBranch();
							pAst->addNode( AstNode::typeString, "targets" );
					pAst->closeNode();
			pAst->closeNode();
	pAst->closeNode();
	Action *pRet = new Action(
		dynamic_cast<const AstBranch *>( *pAst->getNodeBegin() )
		);
	pRet->pAst = pAst;

	return pRet;
}

Action *Action::genDefaultDefault()
{
	Ast *pAst = new Ast();
	pAst->addNode( AstNode::typeActionDef );
		pAst->openBranch();
			pAst->addNode( AstNode::typeString, "default" );
		pAst->openBranch();
			pAst->addNode( AstNode::typeProcessTarget );
				pAst->openBranch();
					pAst->addNode( AstNode::typeString, "build" );
				pAst->openBranch();
					pAst->addNode( AstNode::typeFunction );
						pAst->openBranch();
							pAst->addNode( AstNode::typeString, "targets" );
					pAst->closeNode();
			pAst->closeNode();
	pAst->closeNode();
	Action *pRet = new Action(
		dynamic_cast<const AstBranch *>( *pAst->getNodeBegin() )
		);
	pRet->pAst = pAst;

	return pRet;
}

