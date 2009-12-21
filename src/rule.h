#ifndef RULE_H
#define RULE_H

#include "types.h"
#include <bu/formatter.h>

class Rule
{
	friend Bu::Formatter &operator<<( Bu::Formatter &f, const Rule &t );
public:
	Rule( const Bu::FString &sName );
	virtual ~Rule();

	const Bu::FString &getName() const;

	void setInput( const AstBranch *pNewInput );
	const AstBranch *getInput() const;

	bool hasOutputs() const;

	void addOutput( const AstBranch *pNewOutput );
	void addProfile( const AstBranch *pProfile );

	void prepTarget( class Target *pTarget );
	class Target *createTarget( class Runner &r, const Bu::FString &sInput,
			class Target *pParent );
	bool ruleMatches( class Runner &r, const Bu::FString &sInput );

	void addTag( const Bu::FString &sTag );
	const StrList &getTagList() const;

	void setDisplay( const Bu::FString &sStr );
	const Bu::FString &getDisplay() const;

	void addRequires( const AstBranch *pBr );

private:
	Bu::FString sName;
	Bu::FString sDisplay;
	const AstBranch *pInput;
	AstBranchList lOutput;
	ProfileHash hProfiles;
	StrList lsTags;
	AstBranchList lRequires;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const Rule &t );

namespace Bu
{
	template<> Bu::Formatter &operator<< <Rule>( Bu::Formatter &f, const Rule *t );
};

#endif
