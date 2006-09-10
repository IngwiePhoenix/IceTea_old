#include "targetfile.h"
#include "plugger.h"
#include "rule.h"
#include "build.h"

PluginInterface2(file, TargetFile, Target, "Mike Buland", 0, 1 );

TargetFile::TargetFile()
{
}

TargetFile::~TargetFile()
{
}

void TargetFile::check( Build &bld )
{
	printf("Target file checking:  %s\n", getName().c_str() );

	Rule *pRule = bld.getRule( getRule() );
	PerformList lPerf;
	StringList lFinal = pRule->execute( bld, getInput(), lPerf );

	printf("Input:  ");
	for( StringList::iterator i = getInput().begin();
		 i != getInput().end(); i++ )
	{
		if( i != getInput().begin() ) printf(", ");
		printf("%s", (*i).c_str() );
	}
	printf("\nFinal:  ");
	for( StringList::iterator i = lFinal.begin(); i != lFinal.end(); i++ )
	{
		if( i != lFinal.begin() ) printf(", ");
		printf("%s", (*i).c_str() );
	}
	printf("\n");
}

void TargetFile::clean( Build &bld )
{
	printf("Target file cleaning:  %s\n", getName().c_str() );
}

