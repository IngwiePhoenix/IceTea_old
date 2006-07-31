#include "performcmd.h"
#include "builder.h"

PerformCmd::PerformCmd( const char *sCmd, const char *sTarget ) :
	sCommand( sCmd ),
	sTarget( sTarget )
{
}

PerformCmd::~PerformCmd()
{
}

void PerformCmd::execute( class Builder &bld )
{
	printf("%s\n", sCommand.getString() );
	system( sCommand.getString() );
}

