#include "builder.h"

subExceptionDef( BuildException );

Builder::Builder()
{
}

Builder::~Builder()
{
}

void yyparse( Builder &bld );
extern int yydebug;

void Builder::load( const std::string &sFile )
{
	file = sFile;
	scanBegin();
	yydebug = 1;
	yyparse( *this );
	scanEnd();
}

void Builder::error( YYLTYPE *locp, const char *msg )
{
	fflush( stdout );
	throw BuildException("%s: %d.%d-%d.%d: %s",
			file.c_str(),
			locp->first_line, locp->first_column,
			locp->last_line, locp->last_column,
			msg );
}

void Builder::error( const std::string &msg )
{
	fflush( stdout );
	throw BuildException("%s", msg.c_str() );
}

int Builder::getTargetType( const char *sType )
{
	if( !strcmp( sType, "file" ) )
		return 0;
	return -1;
}

bool Builder::isFunction( const char *sFunc )
{
	return true;
}

