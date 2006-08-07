#include <stdio.h>
#include "viewermake.h"
#include "perform.h"

ViewerMake::ViewerMake()
{
}

ViewerMake::~ViewerMake()
{
}

void ViewerMake::beginTarget( const char *sName, const char *sType, const char *sOperation, int nPerforms )
{
	//sAction = sName;
	//bPrinted = false;
}

void ViewerMake::printHead()
{
	/*
	if( bPrinted == false )
	{
		printf("--- %s ---\n", sAction.getString() );
		bPrinted = true;
	}*/
}

void ViewerMake::endTarget()
{
	/*if( bPrinted == true )
	{
		printf("\n");
	}
	else
	{
		printf("Nothing to be done for %s.\n", sAction.getString() );
	}*/
}

void ViewerMake::beginPerform( Perform *pPerf )
{
	//sTarget = pPerf->getTarget();
}

void ViewerMake::beginExtraRequiresCheck( const char *sCommand )
{
	//printHead();
	//printf("   check: %s\n", sTarget.getString() );
}

void ViewerMake::beginExecute()
{
	//printHead();
	//printf("   build: %s\n", sTarget.getString() );
}

void ViewerMake::executeCmd( const char *sCmd )
{
	printf("%s\n", sCmd );
}

