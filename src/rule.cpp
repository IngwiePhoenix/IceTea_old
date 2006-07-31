#include "rule.h"
#include "builder.h" // for BuildException

Rule::Rule( const char *sName ) :
	sName( sName ),
	sProduces("{target}")
{
}

Rule::~Rule()
{
	regfree( &rWhat );
}

void Rule::debug()
{
	printf("   Rule %s produces %s:\n",
		sName.getString(),
		sProduces.getString()
		);
	printf("      Matches ");
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

void Rule::setProduces( const char *sP )
{
	sProduces = sP;
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

