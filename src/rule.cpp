#include "rule.h"
#include "builder.h" // for BuildException

Rule::Rule( const char *sName ) :
	sName( sName )
{
}

Rule::~Rule()
{
	regfree( &rWhat );
}

void Rule::debug()
{
	printf("   Rule %s:\n",
		sName.getString()
		);
	printf("      Produces: ");
	if( lProduces.empty() )
		printf("{target}");
	else
	{
		for( std::list<std::string>::iterator i = lProduces.begin();
			 i != lProduces.end(); i++ )
		{
			if( i != lProduces.begin() )
				printf(", ");
			printf("%s", (*i).c_str() );
		}
	}
	printf("\n      Matches ");
	if( mHow == matchOne )
		printf("one ");
	else if( mHow == matchAll )
		printf("all ");
	printf("/%s/\n", sWhat.getString() );

	printf("      Performs ");
	if( pHow == perfCommand )
		printf("command ");
	printf("\"%s\"\n", sPerfCmd.getString() );
}

void Rule::addProduces( const char *sP )
{
	lProduces.push_back( sP );
}

void Rule::setMatches( Matches how, const char *sW )
{
	sWhat = sW;
	mHow = how;

	int nErr = regcomp( &rWhat, sW, REG_EXTENDED|REG_NEWLINE );
	if( nErr )
	{
		size_t length = regerror( nErr, &rWhat, NULL, 0 );
		char *buffer = new char[length];
		(void) regerror( nErr, &rWhat, buffer, length );
		StaticString s( buffer );
		delete[] buffer;
		throw BuildException( s.getString() );
	}
}

void Rule::setPerforms( Perform pwhat, const char *sperfcmd )
{
	pHow = pwhat;
	sPerfCmd = sperfcmd;
}

std::list<std::string> Rule::execute( Builder &bld, std::list<std::string> lInput )
{
	std::list<Rule *> lRule = bld.findRuleChain( this );

	std::list<std::string> ret;

	return ret;

}

