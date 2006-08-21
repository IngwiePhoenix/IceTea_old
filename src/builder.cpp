#include "builder.h"

Builder::Builder()
{
}

Builder::~Builder()
{
}

void yyparse( Builder &bld );

void Builder::load( const std::string &sFile )
{
	file = sFile;
	scanBegin();
	yyparse( *this );
	scanEnd();
}

void Builder::error( YYLTYPE *locp, const char *msg )
{
	printf("%s\n", msg );
}

void Builder::error( const std::string &msg )
{
	printf("%s\n", msg.c_str() );
}

