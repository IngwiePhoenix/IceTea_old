#ifndef BUILDER_H
#define BUILDER_H

#include <stdint.h>
#include <string>
#include "build.tab.h"
#include "exceptions.h"

class Builder;

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
	bool isFunction( const char *sFunc );

private:
	std::string file;
	void scanBegin();
	void scanEnd();

};

#endif
