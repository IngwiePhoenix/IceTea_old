#ifndef BUILDER_H
#define BUILDER_H

#include <stdint.h>
#include <string>
#include <list>
#include <utility>
#include "build.tab.h"

class Build;
class Builder;
class Function;
class FunctionFactory;
class Perform;
class PerformFactory;
class Target;
class TargetFactory;

#define YY_DECL int yylex( YYSTYPE *yylval_param, YYLTYPE *yylloc_param, Builder &bld )
YY_DECL;

typedef std::list<std::string> StringList;
typedef std::list<Function *> FunctionList;
typedef std::list<Perform *> PerformList;

template<class tx, class ty, class tz>
class Triplet
{
public:
	Triplet( const tx &x, const ty &y, const tz &z ) :
		first( x ), second( y ), third( z )
	{}

	Triplet( const Triplet &src ) :
		first( src.first ), second( src.second ), third( src.third )
	{}

	tx first;
	ty second;
	tz third;
};

enum eSetHow
{
	setSet,
	setAdd
};

class Builder
{
	typedef std::pair<std::string, Function *> BuildListItem;
	typedef std::list<BuildListItem> BuildList;
	typedef Triplet<std::string, std::string, int> SetVar;
	typedef std::list<SetVar> SetVarList;
public:
	Builder();
	virtual ~Builder();

	void error( YYLTYPE *locp, const char *msg );
	void error( const std::string &msg );

	Build *load( const std::string &sFile );

private:
	std::string file;
	void scanBegin();
	void scanEnd();

	Build *genBuild();

public: // Target functions
	bool isTarget( const char *sType );
	void newTarget();
	void setTargetRule( const char *sRule );
	void setTargetPrefix( const char *sPrefix );
	void setTargetType( const char *sType );
	void addTargetInput();
	void addTargetRequires();
	void addTargetSet( const char *sVar, const char *sVal, int nHow );

private: // Target variables
	TargetFactory &fTarget;
	class TargetInfo
	{
	public:
		std::string sRule;
		std::string sPrefix;
		std::string sType;
		BuildList lInput;
		BuildList lRequires;
		SetVarList lVar;
	};
	typedef std::pair<BuildList,TargetInfo> TargetTmp;
	typedef std::list<TargetTmp> TargetTmpList;
	TargetTmpList lTargetTmp;

public: // Function functions
	bool isFunction( const char *sFunc );
	void newFunctionCall( const char *sName );
	void addFunctionParam( const char *sParam );

private: // Function variables
	Function *pTmpFunc;
	FunctionFactory &fFunction;

public: // Perform functions
	bool isPerform( const char *sPerf );
	void newPerform( const char *sName );
	void addPerformParam( const char *sParam );

private: // Perform variables
	Perform *pTmpPerform;
	PerformFactory &fPerform;

public: // List functions
	void newList();
	void addListString( const char *str );
	void addListFunc();
	void filterList();

	StringList buildToStringList( const BuildList &lSrc, const StringList &lIn );

private: // List variables
	BuildList lTmp;

public: // Rules functions
	void addRule( const char *sName );
	void addRuleMatches();
	void addRuleProduces();
	void addRuleRequires();
	void addRuleInputFilter();
	void addRulePerform();

private: // Rule variables
	class RuleInfo
	{
	public:
		std::string sName;
		Function *pMatches;
		BuildList lProduces;
		BuildList lRequires;
		FunctionList lFilter;
		PerformList lPerform;
	};

	typedef std::list<RuleInfo> RuleTmpList;
	RuleTmpList lRuleTmp;

public: // Action functions
	void addAction();
	void addAction( const char *sName );
	void addCommand( int nType );

private: // Action variables
	typedef std::pair<int, BuildList> ActionTmpCmd;
	typedef std::list<ActionTmpCmd> ActionTmpCmdList;
	typedef std::pair<std::string, ActionTmpCmdList> ActionTmp;
	typedef std::list<ActionTmp> ActionTmpList;
	ActionTmpList lActions;

public: // Global variable functions
	void addGlobalSet( const char *sVar, const char *sValue, int nHow );

private: // Global variable variables
	SetVarList lGlobalVars;

public: // Debug
	void debugDump();
	void printBuildList( const BuildList &lst );
};

#endif
