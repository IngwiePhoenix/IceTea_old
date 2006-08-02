#include "performcmd.h"
#include "builder.h"
#include "viewer.h"

PerformCmd::PerformCmd( const char *sCmd, const char *sTarget ) :
	Perform( sTarget ),
	sCommand( sCmd )
{
}

PerformCmd::~PerformCmd()
{
}

void PerformCmd::execute( class Builder &bld )
{
	bld.view().executeCmd( sCommand );
	int ret;
	if( (ret = system( sCommand.getString() )) != 0 )
	{
		printf("Error code: %d\n", WEXITSTATUS(ret) );
		exit( WEXITSTATUS(ret) );
	}
}

