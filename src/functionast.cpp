#include "functionast.h"
#include "astbranch.h"
#include "astleaf.h"
#include "runner.h"
#include "context.h"

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

Bu::FString FunctionAst::getName() const
{
	return sName;
}

Variable FunctionAst::call( Variable &input, VarList /*lParams*/ )
{
	pContext->pushScope();
	pContext->addVariable("INPUT", input );
	Variable vRet = pRunner->run( (*(pRoot->getBranchBegin()+2)).begin() );
	pContext->popScope();
	return vRet;
}

