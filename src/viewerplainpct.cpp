#include "viewerplainpct.h"
#include "perform.h"
#include "plugger.h"
#include "math.h"

PluginInterface2( plainpct, ViewerPlainPct, Viewer, "Mike Buland", 0, 1 );

ViewerPlainPct::ViewerPlainPct() :
	bRunClean( true )
{
}

ViewerPlainPct::~ViewerPlainPct()
{
}

void ViewerPlainPct::beginCommand( Action::eAction nAct, const std::string &sTarget )
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

void ViewerPlainPct::endCommand()
{
	if( sCmd.front().bCmdClean == false )
		printf("\n");
	sCmd.pop_front();
	iCC++;
}

void ViewerPlainPct::indent()
{
	if( sCmd.empty() ) return;
	int jmax = sCmd.front().nLevel;
	for( int j = 0; j < jmax; j++ )
	{
		printf("  ");
	}
}

void ViewerPlainPct::printHead()
{
	if( sCmd.front().bCmdClean == true )
	{
		bRunClean = false;
		sCmd.front().bCmdClean = false;
		indent();
		printf("--- [%d/%d] %s ---\n", iCC, iTC, sCmd.front().sTarget.c_str() );
	}
}

void ViewerPlainPct::beginRequiresCheck( bool bCached, const std::string &sName )
{
	printHead();
	indent();
	printf("[%3d%%]     deps: %s\n", (int)round(((double)iCP/(double)iTP)*100.0), sName.c_str() );
}

void ViewerPlainPct::endRequiresCheck()
{
	iCP++;
	bDidReq = true;
}

void ViewerPlainPct::beginPerform( Perform *pPerform )
{
	printHead();
	indent();
	printf("[%3d%%] %8s: %s\n", (int)round(((double)iCP/(double)iTP)*100.0), pPerform->getRule().c_str(), pPerform->getTarget().c_str() );
}

void ViewerPlainPct::endPerform()
{
	if(!bDidReq)
		iCP++;
	bDidReq = false;
}

void ViewerPlainPct::beginPerforms( int nCount )
{
	iTP = nCount;
	iCP = 1;
	bDidReq = false;
}

void ViewerPlainPct::beginAction( const std::string &sName, int nCommands )
{
	iTC = nCommands;
	iCC = 1;
}

void ViewerPlainPct::endAction()
{
	if( bRunClean == true )
	{
		printf("Nothing to be done.\n\n");
	}
}

