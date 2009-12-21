#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include "function.h"

class FunctionAst : public Function
{
public:
	FunctionAst( const class AstBranch *pRoot, class Runner *pRunner );
	virtual ~FunctionAst();

	virtual Bu::FString getName() const;
	virtual Variable call( Variable &input, VarList lParams );

private:
	Bu::FString sName;
	const class AstBranch *pRoot;
	class Runner *pRunner;
};

#endif
