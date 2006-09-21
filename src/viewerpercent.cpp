#include "viewerpercent.h"
#include "perform.h"
#include "plugger.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

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

	struct winsize scrn;

	ioctl( fileno( stdout ), TIOCGWINSZ, &scrn );

	nTermWidth = scrn.ws_col;
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
	nCurPerform = nTotalPerforms;

	printPercent("");
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
	printPercent( pPerform->getTarget() );
}

void ViewerPercent::printPercent( const std::string &sCur )
{
	char buf[2048];
	char *bi = buf;
	int nLen = sprintf( buf, "\r[%d/%d] %s [", 
		nCurCommand, nTotalCommands,
		sTarget.c_str() );
	bi += nLen;
	int jmax = nCurPerform*nWidth/nTotalPerforms;
	for( int j = 0; j < jmax; j++ )
	{
		*bi = '#';
		bi++;
	}
	jmax = nWidth-jmax;
	for( int j = 0; j < jmax; j++ )
	{
		*bi = ' ';
		bi++;
	}
	nLen += nWidth;
	nLen += sprintf( bi, "] %-2d%% %s",
		nCurPerform*100/nTotalPerforms,
		sCur.c_str() );
	bi = buf + nLen;
	if( (int)(bi - buf) >= nTermWidth )
	{
		nLastLen = nLen = nTermWidth;
		strcpy( buf+nTermWidth-3, "...");
	}
	else
	{
		if( nLastLen > nLen )
		{
			jmax = nLastLen-nLen;
			for( int j = 0; j < jmax; j++ )
			{
				*bi = ' ';
				bi++;
			}
		}
		nLastLen = nLen;
		*bi = '\0';
	}

	fputs( buf, stdout );

	fflush( stdout );
}

void ViewerPercent::endPerform()
{
}

