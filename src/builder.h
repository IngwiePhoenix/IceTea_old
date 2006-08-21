#ifndef BUILDER_H
#define BUILDER_H

#include <stdint.h>
#include <string>
#include "build.tab.h"

class Builder;

#define YY_DECL int yylex( YYSTYPE *yylval_param, YYLTYPE *yylloc_param, Builder &bld )
YY_DECL;

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

};

#endif
