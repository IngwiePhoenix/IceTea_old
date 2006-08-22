#include "builder.h"
#include "functionfactory.h"
#include "performfactory.h"
#include "targetfactory.h"

subExceptionDef( BuildException );

Builder::Builder() :
	fFunction( FunctionFactory::getInstance() ),
	fPerform( PerformFactory::getInstance() ),
	fTarget( TargetFactory::getInstance() )
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
	//yydebug = 1;
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

//
// Target functions
//
bool Builder::isTarget( const char *sType )
{
	return fTarget.hasPlugin( sType );
}

//
// Function functions
//
bool Builder::isFunction( const char *sFunc )
{
	return fFunction.hasPlugin( sFunc );
}

void Builder::newFunctionCall( const char *sName )
{
	
}

void Builder::addFunctionParam( const char *sParam )
{
}

//
// Perform functions
//
bool Builder::isPerform( const char *sPerf )
{
	return fPerform.hasPlugin( sPerf );
}

void Builder::newPerform( const char *sName )
{
}

void Builder::addPerformParam( const char *sParam )
{
}

