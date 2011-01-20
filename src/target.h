#ifndef TARGET_H
#define TARGET_H

#include "types.h"
#include <bu/formatter.h>

class Target
{
	friend Bu::Formatter &operator<<( Bu::Formatter &f, const Target &t );
public:
	Target( bool bExplicit );
	Target( const Bu::String &sOutput, bool bExplicit );
	virtual ~Target();

	void addInput( const Bu::String &sInput );
	const StrList &getInputList() const;
	void resetInputList( const StrList &lInputs );

	void addRequires( const Bu::String &sReq );
	void addRequires( const AstBranch *pBr );
	const StrList &getRequiresList() const;
	/**
	 * This function will get the cached requires if they exist, build them
	 * if they don't.  Use this is conditions, but use buildRequires to force
	 * a rebuild even if cached data exists.
	 */
	void gatherRequires( class Runner &r );
	void buildRequires( class Runner &r );

	void addOutput( const Bu::String &sOutput );
	const StrList &getOutputList() const;

	void setPrefix( const Bu::String &sPrefix );
	const Bu::String &getPrefix() const;

	void setRule( const Bu::String &sRule );
	const Bu::String &getRule() const;
	bool hasRule() const;

	bool isExplicit() const;

	void addDep( Target *pDep );
	const TargetList &getDepList() const;

	void addProfile( const class AstBranch *pProfRoot );
	void addProfile( const class Profile *pSrc );
	bool hasProfile( const Bu::String &sName ) const;
	const class Profile *getProfile( const Bu::String &sName ) const;

	void setVars( const VarHash &hNewVars );
	const VarHash &getVars() const;

	void setDisplay( const Bu::String &sNewDisplay );
	const Bu::String &getDisplay() const;

	void process( class Runner &r, const Bu::String &sProfile );

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
	Bu::String sPrefix;
	Bu::String sRule;
	VarHash hVars;
	TargetList lDeps;
	ProfileHash hProfiles;
	Bu::String sDisplay;
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
