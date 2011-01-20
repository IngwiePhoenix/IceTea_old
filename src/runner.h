#ifndef RUNNER_H
#define RUNNER_H

#include "astbranch.h"

class Runner
{
public:
	Runner( class Ast &rAst, class Context &rCont );
	virtual ~Runner();

	/**
	 * Run through and pull out all of the functions.  Maybe more later.
	 */
	void initialize();
	class Variable execExpr( AstBranch::NodeList::const_iterator e );
	class Variable execExpr( AstBranch::NodeList::const_iterator e,
			const class Variable &vIn );
	void run();
	Variable run( AstBranch::NodeList::const_iterator n );
	class Variable execFunc( const class AstBranch *pFunc,
			class Variable &vIn );
	void execProfile( class Target *pTarget, const Bu::String &sProfile );
	void execAction( const Bu::String &sName );

	Context &getContext();

private:
	class Target *buildTarget( const Bu::String &sOutput,
			AstBranch::NodeList::const_iterator n );
	class Rule *buildRule( const Bu::String &sName,
			AstBranch::NodeList::const_iterator n );
	void attachDefaults();
	Variable doSet( const AstBranch *pRoot );

private:
	class Ast &rAst;
	class Context &rCont;
	Target *pCurTarget;
	Rule *pCurRule;
};

#endif
