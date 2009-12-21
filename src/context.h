#ifndef CONTEXT_H
#define CONTEXT_H

#include "bu/hash.h"
#include "bu/fstring.h"

#include "variable.h"

class Target;
class Rule;
class Function;
class Action;
class View;

class Context
{
	friend Bu::Formatter &operator<<( Bu::Formatter &f, const Context &c );
public:
	Context();
	virtual ~Context();

	void addTarget( Target *pTarget );
	void addRule( Rule *pRule );
	void addFunction( Function *pFunction );
	void addVariable( const Bu::FString &sName, const Variable &vValue );
	void addAction( Action *pAction );
	Action *getAction( const Bu::FString &sName );

	void addTargetToTag( Target *pTarget, const Bu::FString &sTag );
	void addTargetToTags( Target *pTarget, const StrList &sTags );
	TargetList &getTag( const Bu::FString &sTag );

	Variable &getVariable( const Bu::FString &sName );
	void delVariable( const Bu::FString &sName );

	void pushScope();
	void pushScope( const VarHash &hNewVars );
	VarHash &getScope();
	void popScope();

	Variable call( const Bu::FString &sName, Variable &input, VarList lParams );

	Bu::FString expand( const Bu::FString &sIn );

	Target *getTarget( const Bu::FString &sOutput );
	TargetList getExplicitTargets();

	/**
	 * This function actually builds the dependancy tree, and is responsible
	 * for creating all of the auto-generated targets that are required by the
	 * explicitly created targets.
	 */
	void buildTargetTree( class Runner &r );

	/**
	 * This is done as a final step, it goes through all targets and
	 * attaches things that they should have even if they haven't defined them,
	 * like a clean profile, they'll get that even if they haven't added one of
	 * their own.  The defaults in this routine are added only if they aren't
	 * already defined in the target.  It should be excetued after
	 * buildTargetTree, which means it doesn't need to affect rules.
	 */
	void attachDefaults();

	/**
	 * This function generates some default actions if they don't already
	 * exist, pretty straight forward, it will create all, clean, and default
	 * (default is the same as all).
	 */
	void genDefaultActions();

	void writeTargetDot();

	void setView( View *pNewView );
	View *getView();

	void printBasicInfo();

private:
	void buildTargetTree( class Runner &r, class Target *pTarget, const Bu::FString &sInput, class Rule *pMaster, StrList &lNewIns );

private:
	typedef Bu::Hash<Bu::FString, Target *> TargetHash;
	typedef Bu::Hash<Bu::FString, Rule *> RuleHash;
	typedef Bu::Hash<Bu::FString, Function *> FunctionHash;
	typedef Bu::Hash<Bu::FString, Action *> ActionHash;
	typedef Bu::List<VarHash> ScopeStack;
	typedef Bu::Hash<Bu::FString, TargetList> TagHash;
	
	TargetHash hTarget;
	RuleHash hRule;
	FunctionHash hFunction;
	ScopeStack sVars;
	ActionHash hAction;
	TagHash hTag;
	View *pView;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const Context &c );

#endif
