#include "performcmd.h"
#include "builder.h"

PerformCmd::PerformCmd( const char *sCmd, const char *sTarget ) :
	Perform( sTarget ),
	sCommand( sCmd ),
{
}

PerformCmd::~PerformCmd()
{
}

void PerformCmd::execute( class Builder &bld )
{
	printf("%s\n", sCommand.getString() );
	int ret;
	if( (ret = system( sCommand.getString() )) != 0 )
	{
		printf("Error code: %d\n", WEXITSTATUS(ret) );
		exit( WEXITSTATUS(ret) );
	}
}

