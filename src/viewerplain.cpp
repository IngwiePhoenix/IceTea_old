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
	printf("--- %s ---\n", sName );
}

void ViewerPlain::endTarget()
{
	printf("\n");
}

void ViewerPlain::beginPerform( Perform *pPerf )
{
	sTarget = pPerf->getTarget();
	printf("   check: %s\n", sTarget.getString() );
}

void ViewerPlain::beginExecute()
{
	printf("   build: %s\n", sTarget.getString() );
}

