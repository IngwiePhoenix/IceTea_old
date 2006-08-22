#ifndef BUILDER_H
#define BUILDER_H

#include <stdint.h>
#include <string>
#include <list>
#include <utility>
#include "build.tab.h"
#include "exceptions.h"

class Builder;
class Function;
class FunctionFactory;
class Perform;
class PerformFactory;
class Target;
class TargetFactory;

#define YY_DECL int yylex( YYSTYPE *yylval_param, YYLTYPE *yylloc_param, Builder &bld )
YY_DECL;

subExceptionDecl( BuildException );

typedef std::list<std::string> StringList;

class Builder
{
public:
	Builder();
	virtual ~Builder();

	void error( YYLTYPE *locp, const char *msg );
	void error( const std::string &msg );

	void load( const std::string &sFile );

private:
	std::string file;
	void scanBegin();
	void scanEnd();

public: // Target functions
	bool isTarget( const char *sType );

private: // Target variables
	Target *pTmpTarget;
	TargetFactory &fTarget;

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

	typedef std::pair<std::string, Function *> BuildListItem;
	typedef std::list<BuildListItem> BuildList;

	StringList buildToStringList( BuildList &lSrc, StringList &lIn );

private:
	BuildList lTmp;

public: // Functions for dealing with rules

public: // Functions for dealing with actions
	void addAction();
	void addAction( const char *sName );
	void addCommand( int nType );

private: // Action variables
	typedef std::pair<int, BuildList> ActionTmpCmd;
	typedef std::list<ActionTmpCmd> ActionTmpCmdList;
	typedef std::pair<std::string, ActionTmpCmdList> ActionTmp;
	typedef std::list<ActionTmp> ActionTmpList;
	ActionTmpList lActions;

public: // Debug
	void debugDump();
};

#endif
