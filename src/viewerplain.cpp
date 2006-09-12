#include "viewerplain.h"
#include "perform.h"
#include "plugger.h"

PluginInterface2( plain, ViewerPlain, Viewer, "Mike Buland", 0, 1 );

ViewerPlain::ViewerPlain()
{
}

ViewerPlain::~ViewerPlain()
{
}

void ViewerPlain::beginCommand( Action::eAction nAct, const std::string &sTarget )
{
	printf("--- %s ---\n", sTarget.c_str() );
}

void ViewerPlain::endCommand()
{
	printf("\n");
}

void ViewerPlain::beginRequiresCheck( bool bCached, const std::string &sName )
{
	printf("     deps: %s\n", sName.c_str() );
}

void ViewerPlain::endRequiresCheck()
{
}

void ViewerPlain::beginPerform( Perform *pPerform )
{
	printf(" %8s: %s\n", pPerform->getRule().c_str(), pPerform->getTarget().c_str() );
}

void ViewerPlain::endPerform()
{
}

