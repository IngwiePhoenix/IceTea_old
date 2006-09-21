#include "viewerplain.h"
#include "perform.h"
#include "plugger.h"

PluginInterface2( plain, ViewerPlain, Viewer, "Mike Buland", 0, 1 );

ViewerPlain::ViewerPlain() :
	bRunClean( true )
{
}

ViewerPlain::~ViewerPlain()
{
}

void ViewerPlain::beginCommand( Action::eAction nAct, const std::string &sTarget )
{
	Cmd cmd;
	if( sCmd.empty() )
		cmd.nLevel = 0;
	else
		cmd.nLevel = sCmd.front().nLevel+1;
	
	cmd.bCmdClean = true;
	cmd.sTarget = sTarget;

	sCmd.push_front( cmd );
}

void ViewerPlain::endCommand()
{
	if( sCmd.front().bCmdClean == false )
		printf("\n");
	sCmd.pop_front();
}

void ViewerPlain::indent()
{
	if( sCmd.empty() ) return;
	int jmax = sCmd.front().nLevel;
	for( int j = 0; j < jmax; j++ )
	{
		printf("  ");
	}
}

void ViewerPlain::printHead()
{
	if( sCmd.front().bCmdClean == true )
	{
		bRunClean = false;
		sCmd.front().bCmdClean = false;
		indent();
		printf("--- %s ---\n", sCmd.front().sTarget.c_str() );
	}
}

void ViewerPlain::beginRequiresCheck( bool bCached, const std::string &sName )
{
	printHead();
	indent();
	printf("     deps: %s\n", sName.c_str() );
}

void ViewerPlain::endRequiresCheck()
{
}

void ViewerPlain::beginPerform( Perform *pPerform )
{
	printHead();
	indent();
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

