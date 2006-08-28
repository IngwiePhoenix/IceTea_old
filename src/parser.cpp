#include "parser.h"
#include "build.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

void Parser::error( YYLTYPE *locp, const char *msg )
{
	fflush( stdout );
	throw BuildException("%s: %d.%d-%d.%d: %s",
			file.c_str(),
			locp->first_line, locp->first_column,
			locp->last_line, locp->last_column,
			msg );
}

void Parser::error( const std::string &msg )
{
	fflush( stdout );
	throw BuildException("%s", msg.c_str() );
}

