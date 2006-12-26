#include "viewercolorpct.h"
#include "perform.h"
#include "plugger.h"
#include "math.h"

PluginInterface2( colorpct, ViewerColorPct, Viewer, "Mike Buland", 0, 1 );

ViewerColorPct::ViewerColorPct() :
	bRunClean( true )
{
}

ViewerColorPct::~ViewerColorPct()
{
}

void ViewerColorPct::beginCommand( Action::eAction nAct, const std::string &sTarget )
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

void ViewerColorPct::endCommand()
{
	if( sCmd.front().bCmdClean == false )
		printf("\n");
	sCmd.pop_front();
	if(sCmd.empty())
		iCC++;
}

void ViewerColorPct::indent()
{
	if( sCmd.empty() ) return;
	int jmax = sCmd.front().nLevel;
	for( int j = 0; j < jmax; j++ )
	{
		printf("  ");
	}
}

void ViewerColorPct::printHead()
{
	if( sCmd.front().bCmdClean == true )
	{
		bRunClean = false;
		sCmd.front().bCmdClean = false;
		indent();
		if(sCmd.size() > 1)
			printf("\033[37;22m--- \033[34m%s\033[37m ---\n\033[0m", sCmd.front().sTarget.c_str() );
		else
			printf("\033[37;1m--- [\033[32m%d/%d\033[37m] \033[34m%s\033[37m ---\n\033[0m", iCC, iTC, sCmd.front().sTarget.c_str() );
	}
}

void ViewerColorPct::printPerform(const char *sRule, const char *sTarget)
{
	int iPct = (int)round(
			((double)sCmd.front().iCP/(double)sCmd.front().iTP)*100.0
			);
	if(sCmd.size() > 1)
		printf( "\033[37;22m[\033[32m%3d%%\033[37m] \033[35m%8s\033[37m: %s\n\033[0m", iPct, sRule, sTarget );
	else
		printf( "\033[37;1m[\033[32m%3d%%\033[37m] \033[35m%8s\033[37m: %s\n\033[0m", iPct, sRule, sTarget );
}

void ViewerColorPct::beginRequiresCheck( bool bCached, const std::string &sName )
{
	printHead();
	indent();
	printPerform("deps", sName.c_str());
}

void ViewerColorPct::endRequiresCheck()
{
	sCmd.front().iCP++;
}

void ViewerColorPct::skipRequiresCheck( bool bCached, const std::string &sName )
{
	sCmd.front().iCP++;
}

void ViewerColorPct::beginPerform( Perform *pPerform )
{
	printHead();
	indent();
	printPerform(pPerform->getRule().c_str(), pPerform->getTarget().c_str());
}

void ViewerColorPct::endPerform()
{
	sCmd.front().iCP++;
}

void ViewerColorPct::skipPerform( Perform *pPerform )
{
	sCmd.front().iCP++;
}

void ViewerColorPct::beginPerforms( int nCount )
{
	sCmd.front().iTP = nCount*2;
	sCmd.front().iCP = 1;
}

void ViewerColorPct::beginAction( const std::string &sName, int nCommands )
{
	iTC = nCommands;
	iCC = 1;
}

void ViewerColorPct::endAction()
{
	if( bRunClean == true )
	{
		printf("Nothing to be done.\n\n");
	}
	printf("\033[0m");
}

