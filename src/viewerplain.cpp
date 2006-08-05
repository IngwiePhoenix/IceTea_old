#include <stdio.h>
#include "viewerplain.h"
#include "perform.h"

ViewerPlain::ViewerPlain()
{
}

ViewerPlain::~ViewerPlain()
{
}

void ViewerPlain::beginTarget( const char *sName, const char *sType, const char *sOperation, int nPerforms )
{
	sAction = sName;
	bPrinted = false;
}

void ViewerPlain::printHead()
{
	if( bPrinted == false )
	{
		printf("--- %s ---\n", sAction.getString() );
		bPrinted = true;
	}
}

void ViewerPlain::endTarget()
{
	if( bPrinted == true )
	{
		printf("\n");
	}
	else
	{
		printf("Nothing to be done for %s.\n", sAction.getString() );
	}
}

void ViewerPlain::beginPerform( Perform *pPerf )
{
	sTarget = pPerf->getTarget();
}

void ViewerPlain::beginExtraRequiresCheck( const char *sCommand )
{
	printHead();
	printf("   check: %s\n", sTarget.getString() );
}

void ViewerPlain::beginExecute()
{
	printHead();
	printf("   build: %s\n", sTarget.getString() );
}

