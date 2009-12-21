#ifndef ACTION_H
#define ACTION_H

#include <bu/fstring.h>

class Action
{
public:
	Action( const class AstBranch *pRoot );
	virtual ~Action();

	const Bu::FString &getName() const;

	void call( class Runner *pRunner );

	static Action *genDefaultAll();
	static Action *genDefaultClean();
	static Action *genDefaultDefault();

private:
	Bu::FString sName;
	const class AstBranch *pRoot;
	class Ast *pAst;
};

#endif
