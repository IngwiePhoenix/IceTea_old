#include "viewerpercent.h"
#include "perform.h"
#include "plugger.h"

PluginInterface2( percent, ViewerPercent, Viewer, "Mike Buland", 0, 1 );

ViewerPercent::ViewerPercent() :
	nWidth( 15 )
{
}

ViewerPercent::~ViewerPercent()
{
}

void ViewerPercent::beginAction( const std::string &sName, int nCommands )
{
	nTotalCommands = nCommands;
	nCurCommand = 0;
}

void ViewerPercent::endAction()
{
}

void ViewerPercent::beginCommand( Action::eAction nAct, const std::string &sTarget )
{
	this->sTarget = sTarget;
	nCurCommand++;
}

void ViewerPercent::endCommand()
{
	printf("\n");
}

void ViewerPercent::beginPerforms( int nCount )
{
	nTotalPerforms = nCount;
	nCurPerform = 0;
	nLastLen = 0;
}

void ViewerPercent::endPerforms()
{
	int nLen = printf("\r[%2d/%-2d] %s [", 
		nCurCommand, nTotalCommands,
		sTarget.c_str() );
	for( int j = 0; j < nWidth; j++ )
	{
		fputc('#', stdout );
	}
	nLen += nWidth;
	nLen += printf("] 100%%");

	if( nLastLen > nLen )
	{
		int jmax = nLastLen-nLen;
		for( int j = 0; j < jmax; j++ )
		{
			fputc(' ', stdout );
		}
	}
	nLastLen = 0;

	fflush( stdout );
}

void ViewerPercent::beginRequiresCheck( bool bCached, const std::string &sName )
{
}

void ViewerPercent::endRequiresCheck()
{
}

void ViewerPercent::beginPerform( Perform *pPerform )
{
	nCurPerform++;
	int nLen = printf("\r[%2d/%-2d] %s [", 
		nCurCommand, nTotalCommands,
		sTarget.c_str() );
	int jmax = nCurPerform*nWidth/nTotalPerforms;
	for( int j = 0; j < jmax; j++ )
	{
		fputc('#', stdout );
	}
	jmax = nWidth-jmax;
	for( int j = 0; j < jmax; j++ )
	{
		fputc(' ', stdout );
	}
	nLen += nWidth;
	nLen += printf("] %-2d%% %s",
		nCurPerform*100/nTotalPerforms,
		pPerform->getTarget().c_str() );

	if( nLastLen > nLen )
	{
		jmax = nLastLen-nLen;
		for( int j = 0; j < jmax; j++ )
		{
			fputc(' ', stdout );
		}
	}
	nLastLen = nLen;

	fflush( stdout );
}

void ViewerPercent::endPerform()
{
}

