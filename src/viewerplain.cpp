#include "viewerplain.h"
#include "perform.h"
#include "plugger.h"

PluginInterface2( plain, ViewerPlain, Viewer, "Mike Buland", 0, 1 );

ViewerPlain::ViewerPlain() :
	bCmdClean( true ),
	bRunClean( true )
{
}

ViewerPlain::~ViewerPlain()
{
}

void ViewerPlain::beginCommand( Action::eAction nAct, const std::string &sTarget )
{
	bCmdClean = true;
	this->sTarget = sTarget;
}

void ViewerPlain::endCommand()
{
	if( bCmdClean == false )
		printf("\n");
}

void ViewerPlain::printHead()
{
	if( bCmdClean == true )
	{
		bRunClean = false;
		bCmdClean = false;
		printf("--- %s ---\n", sTarget.c_str() );
	}
}

void ViewerPlain::beginRequiresCheck( bool bCached, const std::string &sName )
{
	printHead();
	printf("     deps: %s\n", sName.c_str() );
}

void ViewerPlain::endRequiresCheck()
{
}

void ViewerPlain::beginPerform( Perform *pPerform )
{
	printHead();
	printf(" %8s: %s\n", pPerform->getRule().c_str(), pPerform->getTarget().c_str() );
}

void ViewerPlain::endPerform()
{
}

void ViewerPlain::endAction()
{
	if( bRunClean == true )
	{
		printf("Nothing to be done.\n\n");
	}
}

