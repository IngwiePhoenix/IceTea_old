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
}

