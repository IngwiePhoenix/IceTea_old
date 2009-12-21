#ifndef PROFILE_H
#define PROFILE_H

#include <bu/fstring.h>

class Profile
{
public:
	Profile( const class AstBranch *pRoot );
	Profile( const Profile &rSrc );
	virtual ~Profile();

	const Bu::FString &getName() const;
	const class AstBranch *getRoot() const;
	const class Condition *getCond() const;
	bool shouldExec( class Runner &r, class Target &rTarget ) const;

	static Profile *genDefaultClean();

private:
	void setCondition();

private:
	Bu::FString sName;
	const class AstBranch *pRoot;
	class Condition *pCond;
	class Ast *pAst;
};

#endif
