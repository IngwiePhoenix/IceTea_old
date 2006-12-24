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
	if(sCmd.empty())
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
		if(sCmd.size() > 1)
			printf("--- %s ---\n", sCmd.front().sTarget.c_str() );
		else
			printf("--- [%d/%d] %s ---\n", iCC, iTC, sCmd.front().sTarget.c_str() );
	}
}

void ViewerPlainPct::printPerform(const char *sRule, const char *sTarget)
{
	int iPct = (int)round(
			((double)sCmd.front().iCP/(double)sCmd.front().iTP)*100.0
			);
	if(sCmd.size() > 1)
		printf( "[%3d%%] %8s: %s\n", iPct, sRule, sTarget );
	else
		printf( "[%3d%%] %8s: %s\n", iPct, sRule, sTarget );
}

void ViewerPlainPct::beginRequiresCheck( bool bCached, const std::string &sName )
{
	printHead();
	indent();
	printPerform("deps", sName.c_str());
}

void ViewerPlainPct::endRequiresCheck()
{
	sCmd.front().iCP++;
}

void ViewerPlainPct::skipRequiresCheck( bool bCached, const std::string &sName )
{
	sCmd.front().iCP++;
}

void ViewerPlainPct::beginPerform( Perform *pPerform )
{
	printHead();
	indent();
	printPerform(pPerform->getRule().c_str(), pPerform->getTarget().c_str());
}

void ViewerPlainPct::endPerform()
{
	sCmd.front().iCP++;
}

void ViewerPlainPct::skipPerform( Perform *pPerform )
{
	sCmd.front().iCP++;
}

void ViewerPlainPct::beginPerforms( int nCount )
{
	sCmd.front().iTP = nCount*2;
	sCmd.front().iCP = 1;
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

