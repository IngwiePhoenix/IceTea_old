#ifndef BUILDER_H
#define BUILDER_H

#include <stdint.h>
#include <string>
#include "build.tab.h"
#include "exceptions.h"

class Builder;
class Function;
class FunctionFactory;
class Perform;
class PerformFactory;

#define YY_DECL int yylex( YYSTYPE *yylval_param, YYLTYPE *yylloc_param, Builder &bld )
YY_DECL;

subExceptionDecl( BuildException );

class Builder
{
public:
	Builder();
	virtual ~Builder();

	void error( YYLTYPE *locp, const char *msg );
	void error( const std::string &msg );

	void load( const std::string &sFile );

	int getTargetType( const char *sType );

private:
	std::string file;
	void scanBegin();
	void scanEnd();

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

public: // Functions for dealing with rules
	void addAction();
	void addAction( const char *sName );
	void addCommand( int nType );
};

#endif
