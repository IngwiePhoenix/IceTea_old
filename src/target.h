#ifndef TARGET_H
#define TARGET_H

#include "types.h"
#include <bu/formatter.h>

class Target
{
	friend Bu::Formatter &operator<<( Bu::Formatter &f, const Target &t );
public:
	Target( bool bExplicit );
	Target( const Bu::FString &sOutput, bool bExplicit );
	virtual ~Target();

	void addInput( const Bu::FString &sInput );
	const StrList &getInputList() const;
	void resetInputList( const StrList &lInputs );

	void addRequires( const Bu::FString &sReq );
	void addRequires( const AstBranch *pBr );
	const StrList &getRequiresList() const;
	void buildRequires( class Runner &r );

	void addOutput( const Bu::FString &sOutput );
	const StrList &getOutputList() const;

	void setPrefix( const Bu::FString &sPrefix );
	const Bu::FString &getPrefix() const;

	void setRule( const Bu::FString &sRule );
	const Bu::FString &getRule() const;
	bool hasRule() const;

	bool isExplicit() const;

	void addDep( Target *pDep );
	const TargetList &getDepList() const;

	void addProfile( const class AstBranch *pProfRoot );
	void addProfile( const class Profile *pSrc );
	bool hasProfile( const Bu::FString &sName ) const;
	const class Profile *getProfile( const Bu::FString &sName ) const;

	void setVars( const VarHash &hNewVars );
	const VarHash &getVars() const;

	void setDisplay( const Bu::FString &sNewDisplay );
	const Bu::FString &getDisplay() const;

	void process( class Runner &r, const Bu::FString &sProfile );

	void mergeUnder( const Target *pSrc );

	bool hasRun();

	void resetRun( bool bHasRun=true );
	void setDepCount();
	int getDepCount() const;

	void collapseDeps();

private:
	void mergeUnder( const VarHash &hVars );
	void merge( StrList &lOut, const StrList &lIn );

private:
	bool bExplicit;
	StrList lsInput;
	StrList lsRequires;
	StrList lsOutput;
	Bu::FString sPrefix;
	Bu::FString sRule;
	VarHash hVars;
	TargetList lDeps;
	ProfileHash hProfiles;
	Bu::FString sDisplay;
	bool bRun;
	AstBranchList lbRequires;
	int iDepCount;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const Target &t );

namespace Bu
{
	template<> Bu::Formatter &operator<< <Target>( Bu::Formatter &f, const Target *t );
};

#endif
