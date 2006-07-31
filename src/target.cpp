#include "target.h"

Target::Target( const char *sName ) :
	sName( sName )
{
}

Target::~Target()
{
}

void Target::setRule( const char *sRule )
{
	this->sRule = sRule;
}

void Target::debug()
{
	printf("   %s:\n      Rule: %s\n",
		sName.getString(),
		sRule.getString()
		);
	printf("      Input list:\n");
	for( std::list<std::string>::iterator i = lInput.begin();
		 i != lInput.end(); i++ )
	{
		printf("         %s\n", (*i).c_str() );
	}
	printf("      Output list:\n");
	for( std::list<std::string>::iterator i = lOutput.begin();
		 i != lOutput.end(); i++ )
	{
		printf("         %s\n", (*i).c_str() );
	}
}

void Target::addInput( const char *sInput )
{
	lInput.push_back( sInput );
}

void Target::addOutput( const char *sOutput )
{
	lOutput.push_back( sOutput );
}

